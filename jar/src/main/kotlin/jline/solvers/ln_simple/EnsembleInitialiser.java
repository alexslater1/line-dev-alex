package jline.solvers.ln_simple;

import jline.lang.ClosedClass;
import jline.lang.JobClass;
import jline.lang.Network;
import jline.lang.RoutingMatrix;
import jline.lang.layered.Activity;
import jline.lang.layered.Entry;
import jline.lang.layered.LayeredNetwork;
import jline.lang.layered.Task;
import jline.lang.nodes.Delay;
import jline.lang.nodes.Node;
import jline.lang.nodes.Queue;
import jline.lang.processes.Disabled;
import jline.lang.processes.Exp;
import jline.solvers.ln_simple.mva.MvaInputs;
import jline.util.matrix.Matrix;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * One-shot initialisation of an LQN ensemble for {@link SolverLNSimple}.
 *
 * <p>{@link LayeredNetwork#getEnsemble()} produces a list of per-layer closed
 * PFQNs but leaves several things in a state that the iteration loop cannot
 * use directly. This class fixes them up, returning a {@link Result} struct
 * that the solver stores for the duration of its run.
 *
 * <h2>What this does (in order)</h2>
 * <ol>
 *   <li><b>{@link #snapshotBaselines}</b> — record initial {@code Clients} think
 *       times and index server queues → layer indices.</li>
 *   <li><b>{@link #fixCallerClassDemands}</b> — set per-class demand on every
 *       layer's server. The ensemble builder leaves caller classes as
 *       {@code Disabled}; this loop computes each caller's true demand from the
 *       LQN's synchronous-call graph.</li>
 *   <li><b>{@link #rebuildCaseBHostLayers}</b> — replace single-class aggregate
 *       P: layers with per-caller multiclass models, when the hosted task is
 *       INF-scheduled and called by ≥ 2 distinct REF tasks (Case B).</li>
 *   <li><b>{@link #capTaskLayerPopulations}</b> — bound T:-class populations in
 *       P: layers by the actual chain concurrency.</li>
 * </ol>
 *
 * <p>The {@link #initialise} entry point is the only public method. The result
 * is immutable from the caller's perspective (all maps are populated and never
 * grown again).
 */
public final class EnsembleInitialiser {

    private EnsembleInitialiser() {}

    /** Numerical floor used when callers have NaN / non-positive think time. */
    private static final double EPS = 1e-12;

    /** Output of {@link #initialise}: the data the solver needs to keep around. */
    public static final class Result {
        /** Baseline {@code Clients} think time per layer. Used as the additive
         *  baseline when injecting {@code Z = base + R} into a callee layer
         *  (so repeated injections do not stack). */
        public final double[] baseClientsThink;
        /** Server queue name (e.g. "T:T3", "P:P2") → ensemble layer index. */
        public final Map<String, Integer> queueNameToLayer;
        /** Task name → its processor's name. */
        public final Map<String, String> taskToProcessor;
        /** Processor name → names of tasks running on it. */
        public final Map<String, List<String>> processorToTasks;
        /** Layer index → hosted task name, for P: layers rebuilt with per-caller
         *  classes (Case B). */
        public final Map<Integer, String> rebuiltHostLayers;

        Result(double[] baseClientsThink,
               Map<String, Integer> queueNameToLayer,
               Map<String, String> taskToProcessor,
               Map<String, List<String>> processorToTasks,
               Map<Integer, String> rebuiltHostLayers) {
            this.baseClientsThink = baseClientsThink;
            this.queueNameToLayer = queueNameToLayer;
            this.taskToProcessor = taskToProcessor;
            this.processorToTasks = processorToTasks;
            this.rebuiltHostLayers = rebuiltHostLayers;
        }
    }

    /** Run all four initialisation steps and return the populated result. */
    public static Result initialise(LayeredNetwork model, List<Network> ensemble) {
        int nLayers = ensemble.size();
        double[] baseClientsThink = new double[nLayers];
        Map<String, Integer> queueNameToLayer = new HashMap<String, Integer>();
        Map<String, String> taskToProcessor = new HashMap<String, String>();
        Map<String, List<String>> processorToTasks = new HashMap<String, List<String>>();
        Map<Integer, String> rebuiltHostLayers = new HashMap<Integer, String>();

        snapshotBaselines(ensemble, baseClientsThink, queueNameToLayer);
        snapshotTaskPlacement(model, taskToProcessor, processorToTasks);

        // perCallerHostDemand is built during demand-fixing and consumed by the
        // Case-B rebuild. It is not exposed in the Result — only the rebuild map.
        Map<String, Map<String, Double>> perCallerHostDemand =
                new HashMap<String, Map<String, Double>>();

        fixCallerClassDemands(model, ensemble, perCallerHostDemand);
        rebuildCaseBHostLayers(model, ensemble, perCallerHostDemand, rebuiltHostLayers);
        capTaskLayerPopulations(ensemble);

        return new Result(baseClientsThink, queueNameToLayer,
                taskToProcessor, processorToTasks, rebuiltHostLayers);
    }


    // =================================================================================
    //  Step 1 — baselines and topology snapshots
    // =================================================================================

    private static void snapshotBaselines(List<Network> ensemble,
                                          double[] baseClientsThink,
                                          Map<String, Integer> queueNameToLayer) {
        for (int l = 0; l < ensemble.size(); l++) {
            Network layer = ensemble.get(l);
            Delay clients = findClientsDelay(layer);
            if (clients != null) {
                JobClass mc = MvaInputs.getMainClass(layer);
                double m = clients.getServiceProcess(mc).getMean();
                baseClientsThink[l] = Double.isNaN(m) ? 0.0 : m;
            }
            Queue q = MvaInputs.findNonDelayQueue(layer);
            if (q != null) {
                queueNameToLayer.put(q.getName(), l);
            }
        }
    }

    private static void snapshotTaskPlacement(LayeredNetwork model,
                                              Map<String, String> taskToProcessor,
                                              Map<String, List<String>> processorToTasks) {
        for (Task task : model.getTasks().values()) {
            if (task.getProcessor() == null) continue;
            String taskName = task.getName();
            String procName = task.getProcessor().getName();
            taskToProcessor.put(taskName, procName);
            List<String> hosted = processorToTasks.get(procName);
            if (hosted == null) {
                hosted = new ArrayList<String>();
                processorToTasks.put(procName, hosted);
            }
            hosted.add(taskName);
        }
    }


    // =================================================================================
    //  Step 2 — per-class demand fix-up
    // =================================================================================

    /**
     * Set the correct per-class service demand on every layer's server.
     *
     * <p>The ensemble builder leaves caller classes Disabled at the server, and
     * {@link MvaInputs#buildDemandMatrix} otherwise falls back to the first
     * non-NaN class — which gives <i>every</i> caller class the demand of the
     * first one. That under-counts asymmetric multiclass models. Here we compute
     * each class's true demand:
     *
     * <ul>
     *   <li><b>P: layer, aggregate {@code T:<task>} class</b> — demand =
     *       population-weighted average host demand over all callers
     *       (Schmietendorf–Lazowska style aggregation). Falls back to the task's
     *       own activity demand for REF / self-contained tasks.</li>
     *   <li><b>Per-caller class {@code R:<caller>}</b> — demand =
     *       {@code Σ callMean × hostDemand_of_bound_activity} summed over all
     *       sync calls from {@code caller} to entries served by this layer.</li>
     *   <li><b>Caller class that IS the hosted task</b> (REF task in its own
     *       P: layer) — use the task's own visit-weighted activity demands.</li>
     * </ul>
     *
     * Side effect: populates {@code perCallerHostDemand} for T: layers so the
     * Case-B rebuild can split P: layers by caller.
     */
    private static void fixCallerClassDemands(
            LayeredNetwork model, List<Network> ensemble,
            Map<String, Map<String, Double>> perCallerHostDemand) {

        for (Network layer : ensemble) {
            Queue serverQueue = MvaInputs.findNonDelayQueue(layer);
            if (serverQueue == null) continue;
            String serverName = serverQueue.getName();
            String serverTaskOrProc = LqnGraph.stripPrefix(serverName);

            // tasksOnServer = the LQN tasks whose service this server represents.
            Map<String, Task> tasksOnServer = new HashMap<String, Task>();
            if (serverName.startsWith("T:")) {
                Task t = LqnGraph.findTask(model, serverTaskOrProc);
                if (t != null) tasksOnServer.put(t.getName(), t);
            } else if (serverName.startsWith("P:")) {
                for (Task t : model.getTasks().values()) {
                    if (t.getProcessor() != null
                            && serverTaskOrProc.equals(t.getProcessor().getName())) {
                        tasksOnServer.put(t.getName(), t);
                    }
                }
            } else {
                continue;
            }

            for (ClosedClass hc : MvaInputs.getClosedClasses(layer)) {
                if (serverName.startsWith("P:") && hc.getName().startsWith("T:")) {
                    setAggregateHostDemand(model, serverQueue, hc);
                } else {
                    setPerCallerDemand(model, serverQueue, hc, serverName,
                            serverTaskOrProc, tasksOnServer, perCallerHostDemand);
                }
            }
        }
    }

    /** Population-weighted average host demand across all callers of the hosted task. */
    private static void setAggregateHostDemand(LayeredNetwork model,
                                               Queue serverQueue, ClosedClass hc) {
        String aggTaskName = LqnGraph.stripPrefix(hc.getName());
        double totalN = 0.0, weightedD = 0.0;
        for (Task caller : model.getTasks().values()) {
            int mult = caller.getMultiplicity();
            if (mult <= 0 || mult == Integer.MAX_VALUE) continue;
            double d_r = LqnGraph.callerDemandOnTask(model, caller.getName(), aggTaskName);
            if (d_r <= 0) continue;
            weightedD += mult * d_r;
            totalN += mult;
        }
        if (totalN > 0 && weightedD > 0) {
            serverQueue.setService(hc, Exp.fitMean(weightedD / totalN));
        } else {
            // REF / self-contained task: no inbound calls — use own activity demand.
            double selfDemand = LqnGraph.computeLocalDemand(model, aggTaskName);
            if (selfDemand > 0) serverQueue.setService(hc, Exp.fitMean(selfDemand));
        }
    }

    /**
     * Per-caller R:&lt;caller&gt; (or aggregate T:&lt;caller&gt;) class demand on
     * T:- or P:-layer servers.
     *
     * <p>Two cases on the caller side:
     * <ul>
     *   <li><b>Single-entry caller</b> — sum {@code callMean × hostDemand}
     *       across all of the caller's activities. A single entry call
     *       traverses the whole task DAG, so summing captures the full
     *       per-visit demand.</li>
     *   <li><b>Multi-entry caller</b> — each visit dispatches to <i>one</i>
     *       entry's bound activity, not the union of all activities. Compute
     *       per-entry demand from each entry's bound activity and average
     *       across entries so the result is the mean per-visit demand.</li>
     * </ul>
     */
    private static void setPerCallerDemand(
            LayeredNetwork model, Queue serverQueue, ClosedClass hc,
            String serverName, String serverTaskOrProc,
            Map<String, Task> tasksOnServer,
            Map<String, Map<String, Double>> perCallerHostDemand) {

        String callerTaskName = LqnGraph.stripPrefix(hc.getName());
        Task callerTask = LqnGraph.findTask(model, callerTaskName);
        if (callerTask == null) return;

        List<Entry> callerEntries = new ArrayList<Entry>();
        for (Entry e : model.getEntries().values()) {
            if (e.getParent() == callerTask) callerEntries.add(e);
        }

        double demand;
        if (callerEntries.size() > 1) {
            double totalDemand = 0.0;
            for (Entry e : callerEntries) {
                Activity bound = LqnGraph.findBoundActivity(e);
                if (bound == null) continue;
                totalDemand += demandFromActivityToTargets(model, bound, tasksOnServer);
            }
            demand = totalDemand / callerEntries.size();
        } else {
            demand = 0.0;
            for (Activity act : callerTask.getActivities()) {
                demand += demandFromActivityToTargets(model, act, tasksOnServer);
            }
        }

        if (demand > 0) {
            serverQueue.setService(hc, Exp.fitMean(demand));
            // Remember per-caller demand for T: layers so the Case-B rebuild can use it.
            if (serverName.startsWith("T:")) {
                Map<String, Double> byCaller = perCallerHostDemand.get(serverTaskOrProc);
                if (byCaller == null) {
                    byCaller = new HashMap<String, Double>();
                    perCallerHostDemand.put(serverTaskOrProc, byCaller);
                }
                Double existing = byCaller.get(callerTaskName);
                byCaller.put(callerTaskName, (existing != null ? existing : 0.0) + demand);
            }
        } else if (tasksOnServer.containsKey(callerTask.getName())) {
            // REF task in its own P: layer — no T→T calls. Use own activity demands.
            double selfDemand = LqnGraph.computeLocalDemand(model, callerTask.getName());
            if (selfDemand > 0) serverQueue.setService(hc, Exp.fitMean(selfDemand));
        }
    }

    /** Σ {@code callMean × hostDemandOfBoundActivity} for {@code act}'s sync calls
     *  into entries hosted by {@code tasksOnServer}. */
    private static double demandFromActivityToTargets(LayeredNetwork model, Activity act,
                                                       Map<String, Task> tasksOnServer) {
        Map<Integer, String> dests = act.getSyncCallDests();
        if (dests == null || dests.isEmpty()) return 0.0;
        Matrix means = act.getSyncCallMeans();
        double d = 0.0;
        for (Map.Entry<Integer, String> ce : dests.entrySet()) {
            Entry destEntry = LqnGraph.findEntry(model, ce.getValue());
            if (destEntry == null || destEntry.getParent() == null) continue;
            if (!tasksOnServer.containsKey(destEntry.getParent().getName())) continue;
            int idx = ce.getKey();
            double cm = (means != null && means.getNumCols() > idx) ? means.get(0, idx) : 1.0;
            d += cm * LqnGraph.hostDemandOfBoundActivity(destEntry);
        }
        return d;
    }


    // =================================================================================
    //  Step 3 — Case B rebuild
    // =================================================================================

    /**
     * Case B: a P: host layer whose hosted task is INF-scheduled (a true delay
     * server) and called by multiple distinct REF tasks. The aggregate single-class
     * model loses per-caller demand information; rebuild as multiclass with one
     * closed class per REF caller. Matches what SolverLN constructs.
     */
    private static void rebuildCaseBHostLayers(
            LayeredNetwork model, List<Network> ensemble,
            Map<String, Map<String, Double>> perCallerHostDemand,
            Map<Integer, String> rebuiltHostLayers) {

        for (int l = 0; l < ensemble.size(); l++) {
            Network layer = ensemble.get(l);
            Queue serverQueue = MvaInputs.findNonDelayQueue(layer);
            if (serverQueue == null || !serverQueue.getName().startsWith("P:")) continue;

            // Find the single aggregate T:<task> class (the trigger for rebuild)
            ClosedClass aggClass = null;
            for (JobClass jc : layer.getClasses()) {
                if (jc instanceof ClosedClass && jc.getName().startsWith("T:")) {
                    aggClass = (ClosedClass) jc; break;
                }
            }
            if (aggClass == null) continue;

            String hostedTaskName = LqnGraph.stripPrefix(aggClass.getName());
            if (!LqnGraph.isInfScheduledTask(model, hostedTaskName)) continue;
            // Tasks that themselves issue sync calls need a different coupling — skip.
            if (LqnGraph.taskHasSyncCallees(model, hostedTaskName)) continue;

            Map<String, Double> demandsByCaller = perCallerHostDemand.get(hostedTaskName);
            if (demandsByCaller == null || demandsByCaller.size() < 2) continue;

            Delay clientDelay = findClientsDelay(layer);
            if (clientDelay == null) continue;

            // Validate every caller. If any is malformed, leave the layer alone.
            List<String> callerNames = new ArrayList<String>();
            List<Integer> callerMults = new ArrayList<Integer>();
            List<Double> callerThinks = new ArrayList<Double>();
            List<Double> callerDemands = new ArrayList<Double>();
            boolean valid = true;
            for (Map.Entry<String, Double> e : demandsByCaller.entrySet()) {
                String callerName = e.getKey();
                Double D_r = e.getValue();
                Task caller = LqnGraph.findTask(model, callerName);
                if (D_r == null || D_r <= 0 || caller == null) { valid = false; break; }
                int mult_r = caller.getMultiplicity();
                if (mult_r <= 0 || mult_r == Integer.MAX_VALUE) { valid = false; break; }
                double think_r = caller.getThinkTimeMean();
                if (Double.isNaN(think_r) || think_r <= 0) think_r = EPS;
                callerNames.add(callerName);
                callerMults.add(mult_r);
                callerThinks.add(think_r);
                callerDemands.add(D_r);
            }
            if (!valid || callerNames.isEmpty()) continue;

            // Add one R:<caller> closed class per caller (constructor self-registers).
            List<ClosedClass> newClasses = new ArrayList<ClosedClass>();
            for (int i = 0; i < callerNames.size(); i++) {
                ClosedClass cc = new ClosedClass(layer, "R:" + callerNames.get(i),
                        callerMults.get(i), clientDelay);
                cc.setReferenceClass(true);
                newClasses.add(cc);
            }

            // Disable the aggregate class. getClosedClasses() filters out N=0
            // classes, so it disappears from MVA matrices.
            aggClass.setPopulation(0);
            clientDelay.setService(aggClass, Disabled.getInstance());
            serverQueue.setService(aggClass, Disabled.getInstance());

            // Per-class service times: think at Clients, raw host demand at the queue.
            for (int i = 0; i < newClasses.size(); i++) {
                clientDelay.setService(newClasses.get(i), Exp.fitMean(callerThinks.get(i)));
                serverQueue.setService(newClasses.get(i), Exp.fitMean(callerDemands.get(i)));
            }

            // Re-link serial Clients ↔ Queue routing for every class.
            RoutingMatrix P = layer.initRoutingMatrix();
            for (JobClass jc : layer.getClasses()) {
                P.addConnection(jc, jc, clientDelay, serverQueue, 1.0);
                P.addConnection(jc, jc, serverQueue, clientDelay, 1.0);
            }
            layer.link(P);

            rebuiltHostLayers.put(l, hostedTaskName);
        }
    }


    // =================================================================================
    //  Step 4 — population cap
    // =================================================================================

    /**
     * Cap each T:-layer class population in every P: host layer to the actual
     * maximum concurrency at that task. The processor's <i>server count</i> is
     * left untouched (it reflects physical multiplicity, not chain population).
     */
    private static void capTaskLayerPopulations(List<Network> ensemble) {
        Map<String, Double> taskEffectiveN = new HashMap<String, Double>();
        for (Network layer : ensemble) {
            Queue serverQueue = MvaInputs.findNonDelayQueue(layer);
            if (serverQueue == null || !serverQueue.getName().startsWith("T:")) continue;
            String taskName = LqnGraph.stripPrefix(serverQueue.getName());
            double totalN = 0.0;
            for (JobClass jc : layer.getClasses()) {
                if (!(jc instanceof ClosedClass)) continue;
                double n = ((ClosedClass) jc).getNumberOfJobs();
                if (n > 0 && n < Integer.MAX_VALUE) totalN += n;
            }
            if (totalN > 0) taskEffectiveN.put(taskName, totalN);
        }
        for (Network layer : ensemble) {
            Queue serverQueue = MvaInputs.findNonDelayQueue(layer);
            if (serverQueue == null || !serverQueue.getName().startsWith("P:")) continue;
            for (JobClass jc : layer.getClasses()) {
                if (!(jc instanceof ClosedClass)) continue;
                ClosedClass cc = (ClosedClass) jc;
                if (!cc.getName().startsWith("T:") || cc.getNumberOfJobs() <= 0) continue;
                Double effN = taskEffectiveN.get(LqnGraph.stripPrefix(cc.getName()));
                if (effN != null && cc.getNumberOfJobs() > effN) {
                    cc.setPopulation(effN);
                }
            }
        }
    }


    // =================================================================================
    //  Helpers private to this class
    // =================================================================================

    private static Delay findClientsDelay(Network net) {
        for (Node node : net.getNodes()) {
            if (node instanceof Delay && "Clients".equals(node.getName())) return (Delay) node;
        }
        return null;
    }
}
