package jline.solvers.ln_simple;

import jline.lang.ClosedClass;
import jline.lang.JobClass;
import jline.lang.Network;
import jline.lang.layered.Activity;
import jline.lang.layered.Entry;
import jline.lang.layered.LayeredNetwork;
import jline.lang.layered.Task;
import jline.lang.nodes.Delay;
import jline.lang.nodes.Node;
import jline.lang.nodes.Queue;
import jline.lang.processes.Exp;
import jline.io.Ret;
import jline.solvers.LayeredNetworkAvgTable;
import jline.solvers.ln_simple.mva.MvaInputs;
import jline.solvers.ln_simple.results.ResultsCollector;
import jline.util.matrix.Matrix;

import java.util.HashMap;
import java.util.List;
import java.util.Map;


/**
 * Layered Queueing Network (LQN) decomposition solver.
 *
 * <h2>Decomposition model</h2>
 * {@link LayeredNetwork#getEnsemble()} factors the LQN into a list of single-server
 * closed PFQNs, one per processor and one per task. Two layer kinds are used:
 *
 * <ul>
 *   <li><b>{@code T:<task>} task layer</b> — server queue is the task; closed
 *       classes are caller tasks named {@code R:<caller>} with N = caller multiplicity
 *       and demand = total call-demand (sum of {@code callMean × hostDemand} over
 *       all synchronous calls into the task).</li>
 *   <li><b>{@code P:<proc>} host layer</b> — server queue is the processor; classes
 *       are either per-caller {@code R:<caller>} (multi-caller / rebuilt Case B) or
 *       one aggregate {@code T:<task>} class (single-class form).</li>
 * </ul>
 *
 * Each layer also has a {@code Clients} {@link Delay} node whose mean encodes the
 * think / blocking time per class.
 *
 * <h2>Architecture</h2>
 * Work is split across four files:
 * <ul>
 *   <li>{@link EnsembleInitialiser} — one-shot constructor-time init (demand fixing,
 *       Case-B rebuild, population cap, topology snapshots).</li>
 *   <li>This class — public API + the iteration loop and its two coupling stages.</li>
 *   <li>{@link MvaInputs} — MVA matrix builders and the reflection-based call into
 *       {@code pfqn_mvams}.</li>
 *   <li>{@link LqnGraph} — pure read-only queries on the LQN model.</li>
 *   <li>{@link ResultsCollector} (in {@code results/}) — final-pass result assembly
 *       into a {@link LayeredNetworkAvgTable}.</li>
 * </ul>
 *
 * <h2>Iteration</h2>
 * {@link #iterateCoupledMva} runs a bounce sweep (sequence
 * {@code 0,1,...,N-1,N-2,...,1}) over all layers per outer iteration so that
 * information flows both ways through the call chain in a single pass. Each layer
 * is solved by {@code pfqn_mvams}; its result then propagates into coupled
 * layers via {@code Clients} think times and queue service rates. Stop when
 * {@code max |ΔX| < tol}.
 *
 * <h2>Notation (used in comments throughout)</h2>
 * D / L — service demand at a station; N — population; X — throughput;
 * Q — mean queue length; R — response time (Q/X by Little's law);
 * Z — Clients-delay (think) time; callMean — expected sync calls per caller-visit.
 */
public class SolverLNSimple {

    // === Numerical safety constants ===
    /** Minimum positive value used to avoid zero-divides. */
    private static final double EPS = 1e-12;
    /** Hard upper bound on injected think / service times. Keeps MVA inputs finite
     *  when feedback transiently blows up before convergence. */
    private static final double MAX_PROTECTION = 1e6;
    /** Floor for any clamped value (slightly larger than EPS so MVA always sees > 0). */
    private static final double CLAMP_FLOOR = 1e-9;

    // === Model & ensemble ===
    private final LayeredNetwork lqnModel;
    private final List<Network> ensemble;
    private final int N_LAYERS;

    // === Initialisation result (immutable for the lifetime of the solver) ===
    private final double[] baseClientsThink;
    private final Map<String, Integer> queueNameToLayer;
    private final Map<String, String> taskToProcessor;
    private final Map<Integer, String> rebuiltHostLayers;

    // === State accumulated across sweeps ===
    /** Cached server-only response time per callee task (Q_server/X). Used by the
     *  cycle-time formula when updating intermediate callers' Clients delay. */
    private final Map<String, Double> taskSojournCache = new HashMap<String, Double>();
    /** Cached throughput per callee task. Companion to {@link #taskSojournCache}. */
    private final Map<String, Double> taskThroughputCache = new HashMap<String, Double>();

    /** Result table from the most recent {@link #iterateCoupledMva} run. */
    private LayeredNetworkAvgTable lastAvgTable;


    // =================================================================================
    //  Construction
    // =================================================================================

    public SolverLNSimple(LayeredNetwork model) {
        this.lqnModel = model;
        this.ensemble = model.getEnsemble();
        this.N_LAYERS = ensemble.size();

        EnsembleInitialiser.Result init = EnsembleInitialiser.initialise(model, ensemble);
        this.baseClientsThink   = init.baseClientsThink;
        this.queueNameToLayer   = init.queueNameToLayer;
        this.taskToProcessor    = init.taskToProcessor;
        this.rebuiltHostLayers  = init.rebuiltHostLayers;
    }


    // =================================================================================
    //  Public API
    // =================================================================================

    public LayeredNetworkAvgTable getAvgTable() {
        return lastAvgTable;
    }

    public List<Network> getEnsemble() {
        return ensemble;
    }

    public static final int DEFAULT_MAX_ITER = 100;
    public static final double DEFAULT_TOL = 5e-3;

    public void iterateCoupledMva() {
        iterateCoupledMva(DEFAULT_MAX_ITER, DEFAULT_TOL, null);
    }

    public void iterateCoupledMva(Runnable afterIteration) {
        iterateCoupledMva(DEFAULT_MAX_ITER, DEFAULT_TOL, afterIteration);
    }

    public void iterateCoupledMva(int maxIter, double tol) {
        iterateCoupledMva(maxIter, tol, null);
    }

    /**
     * Outer fixed-point loop. Each outer iteration runs a bounce sweep over all
     * layers (index sequence {@code 0,1,...,N-1,N-2,...,1}) so that information
     * flows both ways through the call chain in a single pass. Convergence:
     * {@code max_l |X_l^(k) - X_l^(k-1)| < tol}.
     */
    public void iterateCoupledMva(int maxIter, double tol, Runnable afterIteration) {
        long outerStartTime = System.nanoTime();
        double[][] prevX = new double[N_LAYERS][];

        for (int iter = 0; iter < maxIter; iter++) {
            long solveStartTime = System.nanoTime();
            double maxDeltaX = 0.0;

            int sweepLen = 2 * N_LAYERS - 1;
            for (int si = 0; si < sweepLen; si++) {
                int l = computeSweepLayerIndex(si);

                LayerSolve s = solveLayer(l);
                if (s == null) return;             // MVA threw; bail out
                if (s.serverIdx < 0) continue;     // No non-Delay queue; nothing to couple

                maxDeltaX = Math.max(maxDeltaX, updateDeltaX(prevX, l, s.res));

                if (s.isTaskLayer)      propagateTaskLayerCoupling(l, s);
                else if (s.isHostLayer) propagateHostLayerCoupling(l, s);
            }

            double solveTime = (System.nanoTime() - solveStartTime) / 1e9;
            long synchStartTime = System.nanoTime();
            if (afterIteration != null) afterIteration.run();
            double synchTime = (System.nanoTime() - synchStartTime) / 1e9;
            double totalRuntime = (System.nanoTime() - outerStartTime) / 1e9;

            System.out.printf("\nIter %2d. Analyze time: %.3fs. Update time: %.3fs. Runtime: %.3fs. ",
                    iter + 1, solveTime, synchTime, totalRuntime);

            if (maxDeltaX < tol) break;
        }

        lastAvgTable = ResultsCollector.collectAndPrintFinalResults(
                lqnModel, ensemble, N_LAYERS, taskSojournCache, rebuiltHostLayers);
    }


    // =================================================================================
    //  Per-layer solve
    // =================================================================================

    /** Bundles a single layer's MVA result with the surrounding context the
     *  coupling stages need. */
    private static final class LayerSolve {
        final Network layer;
        final Matrix N;
        final Matrix L;
        final Ret.pfqnMVA res;
        final Queue serverQueue;
        final int serverIdx;            // index of the (single) non-Delay queue in MVA nodes
        final boolean isTaskLayer;      // server name starts with "T:"
        final boolean isHostLayer;      // server name starts with "P:"

        LayerSolve(Network layer, Matrix N, Matrix L, Ret.pfqnMVA res,
                   Queue serverQueue, int serverIdx) {
            this.layer = layer; this.N = N; this.L = L; this.res = res;
            this.serverQueue = serverQueue; this.serverIdx = serverIdx;
            String name = (serverQueue != null) ? serverQueue.getName() : "";
            this.isTaskLayer = name.startsWith("T:");
            this.isHostLayer = name.startsWith("P:");
        }
    }

    /** Build MVA inputs for layer {@code l} and invoke {@code pfqn_mvams}.
     *  Returns {@code null} on solver exception (caller terminates the outer loop). */
    private LayerSolve solveLayer(int l) {
        Network layer = ensemble.get(l);
        List<Node> nodes = MvaInputs.getQueueNodes(layer);

        Matrix N = MvaInputs.buildN(layer);
        Matrix Z = MvaInputs.buildThinkTimeMatrix(layer);
        Matrix L = MvaInputs.buildDemandMatrix(nodes, layer);
        Matrix S = MvaInputs.buildServerMatrix(nodes, N);

        Ret.pfqnMVA res;
        try {
            res = MvaInputs.callMVA(L, N, Z, S);
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
        if (res == null) return null;

        Queue serverQueue = MvaInputs.findNonDelayQueue(layer);
        int serverIdx = -1;
        if (serverQueue != null) {
            for (int i = 0; i < nodes.size(); i++) {
                if (!(nodes.get(i) instanceof Delay)) { serverIdx = i; break; }
            }
        }
        return new LayerSolve(layer, N, L, res, serverQueue, serverIdx);
    }

    /** Update {@code prevX[l]} with this iteration's throughputs and return the
     *  largest absolute change observed at this layer. */
    private double updateDeltaX(double[][] prevX, int l, Ret.pfqnMVA res) {
        int R = res.X.getNumCols();
        if (prevX[l] == null) prevX[l] = new double[R];
        double maxDelta = 0.0;
        for (int r = 0; r < R; r++) {
            double x = res.X.get(0, r);
            maxDelta = Math.max(maxDelta, Math.abs(x - prevX[l][r]));
            prevX[l][r] = x;
        }
        return maxDelta;
    }


    // =================================================================================
    //  Task-layer (T:) coupling
    // =================================================================================

    /**
     * Propagation rules after solving a T:&lt;callee&gt; task layer:
     *
     * <ol>
     *   <li><b>Refresh this layer's Clients delay</b> for each caller class. Uses
     *       a cycle-time formula for saturated zero-think intermediate callers,
     *       otherwise the larger of (root-REF think + caller-chain demand) and
     *       (caller think + caller local demand).</li>
     *   <li><b>Refresh the caller task's own T: layer Clients</b> (if it has one)
     *       with root-REF think + caller-chain demand.</li>
     *   <li>(Skip remaining steps if this layer's main throughput is non-finite.)</li>
     *   <li><b>Compute R_task_total = Q_server / X</b> — server-only response time.
     *       This is per-call for intermediate tasks and per-caller-visit for leaf
     *       tasks (whose T: layer demand already embeds {@code callMean}).</li>
     *   <li><b>Cache</b> sojourn and throughput for the callee.</li>
     *   <li><b>Push response into the caller's T: layer server</b>: D = D_local_caller
     *       + propagatedTaskResp ({@code R_task_total × callMean} for intermediates;
     *       {@code R_task_total} for leaves).</li>
     *   <li><b>Update callee's host layer Clients</b> (skip rebuilt Case-B layers).</li>
     *   <li><b>Per-caller update of each caller's host layer Clients</b> using
     *       this layer's per-class throughput / response.</li>
     * </ol>
     */
    private void propagateTaskLayerCoupling(int l, LayerSolve s) {
        Network layer = s.layer;
        String calleeTask = LqnGraph.stripPrefix(s.serverQueue.getName());
        String callerTask = LqnGraph.stripPrefix(MvaInputs.getMainClass(layer).getName());
        // Sum across caller classes: a multi-class task layer's <i>task</i>-level
        // throughput is the sum of per-caller-class visit rates, not just the
        // first class's tput. Q likewise aggregates across classes; R = Q/X is
        // then the visit-weighted mean per-call response.
        int R_task = s.res.X.getNumCols();
        double throughput = 0.0;
        double q_server = 0.0;
        for (int rc = 0; rc < R_task; rc++) {
            double xr = s.res.X.get(0, rc);
            double qr = s.res.Q.get(s.serverIdx, rc);
            if (Double.isFinite(xr)) throughput += xr;
            if (Double.isFinite(qr)) q_server += qr;
        }

        // (1) + (2): refresh think times even if this solve is degenerate, so that
        // later sweeps can recover to finite values.
        updateTaskLayerClientsThinks(layer, calleeTask);
        updateCallerOwnTaskLayerClients(l, callerTask);

        if (!Double.isFinite(throughput) || throughput <= EPS) return;

        // (4) server-only response time.
        double R_task_total = Double.NaN;
        if (Double.isFinite(q_server)) R_task_total = q_server / throughput;
        if (!Double.isFinite(R_task_total)) return;

        // (5) cache the callee's sojourn / throughput (used by step (1) on later sweeps).
        taskSojournCache.put(calleeTask, R_task_total);
        taskThroughputCache.put(calleeTask, throughput);

        // (6) push response into the caller's T: layer server (D_local + R_callee).
        // For LEAF callees: T: layer demand was set as callMean × hostDemand, so
        //   R_task_total already embeds callMean → no extra multiply.
        // For INTERMEDIATE callees: R_task_total is per single call → the caller
        //   issues callMean calls per visit, so multiply by callMean here.
        double syncCallMean = LqnGraph.getSyncCallMean(lqnModel, callerTask, calleeTask);
        double propagatedTaskResp = LqnGraph.taskHasSyncCallees(lqnModel, calleeTask)
                ? R_task_total * syncCallMean : R_task_total;
        pushResponseToCallerTaskLayer(l, callerTask, calleeTask, propagatedTaskResp, syncCallMean);

        // (7) callee's host-layer Clients.
        updateCalleeHostLayerClients(l, calleeTask, s, R_task_total);

        // (8) per-caller updates to caller host-layer Clients.
        pushPerCallerResponseToHostLayers(l, layer, s, calleeTask);
    }

    /**
     * Step (1): for each caller class in this T: layer, set the Clients delay
     * service to model that caller's blocking + chain time before issuing a call.
     *
     * <p>For callers that synchronously invoke more than one distinct callee,
     * the new Z is under-relaxed: {@code Z = α·target + (1−α)·prior}. MVA's
     * response slope in Z is steep near saturation, and per-callee cache values
     * referenced by sibling Z updates change between sweeps, so plain
     * fixed-point iteration alternates between saturated and unsaturated
     * states. Single-callee chains keep the un-damped path because they have
     * no such cross-feedback.
     */
    private void updateTaskLayerClientsThinks(Network layer, String calleeTask) {
        Delay calleeClients = findClientsDelay(layer);
        if (calleeClients == null) return;

        for (ClosedClass callerCl : MvaInputs.getClosedClasses(layer)) {
            String callerTask_c = LqnGraph.stripPrefix(callerCl.getName());
            double target = computeCallerClientsThink(callerCl, callerTask_c, calleeTask);
            double calleeThink;
            if (LqnGraph.callerFansOut(lqnModel, callerTask_c)) {
                double prior = calleeClients.getServiceProcess(callerCl).getMean();
                calleeThink = (Double.isFinite(prior) && prior > 0)
                        ? Z_RELAX_ALPHA * target + (1.0 - Z_RELAX_ALPHA) * prior
                        : target;
            } else {
                calleeThink = target;
            }
            calleeClients.setService(callerCl, Exp.fitMean(clamp(calleeThink)));
        }
    }

    /** Under-relaxation weight for fan-out caller Clients-Z updates. */
    private static final double Z_RELAX_ALPHA = 0.5;

    /**
     * Z formula for a single caller class.
     *
     * <p>For saturated zero-think intermediate callers we use Little's-law-driven
     * cycle time: an intermediate task with N customers, throughput X_caller and
     * total sojourn at this callee {@code callMean × sojourn} has idle time
     * {@code N/X − callMean × sojourn}. This drives Z → 0 as the caller saturates.
     *
     * <p>Otherwise the caller's blocking before issuing a call is the longer of:
     * <ul><li>chain-think: root REF think time + sum of local demands along the
     * upstream caller chain;</li>
     * <li>direct-think: caller's own think + caller's local demand.</li></ul>
     * Plus the time the caller spends blocked at <i>sibling</i> callees (other
     * tasks called by the same caller activity / task) — without this term, a
     * caller that fans out to k independent callees lets each callee's MVA see
     * Z ≈ caller's own think and saturate independently.
     */
    private double computeCallerClientsThink(ClosedClass callerCl, String callerTask, String calleeTask) {
        Double xCaller = taskThroughputCache.get(callerTask);
        Double sojournCallee = taskSojournCache.get(calleeTask);
        double callerThink = LqnGraph.getTaskThinkTimeSafe(lqnModel, callerTask);

        if (xCaller != null && xCaller > EPS && sojournCallee != null && callerThink <= 1e-9) {
            double nCaller = callerCl.getNumberOfJobs();
            // Time the caller spends per visit at this callee:
            //   leaf: sojournCallee is already a per-visit R (T:-layer D was set as
            //         per-visit callMean × hostDemand), so no extra cMean multiplier.
            //   intermediate: sojournCallee is per single call, so multiply by the
            //         per-visit call mean to get per-visit time at callee.
            double timeAtCalleePerVisit;
            if (LqnGraph.taskHasSyncCallees(lqnModel, calleeTask)) {
                double cMean = Math.max(1.0, LqnGraph.getSyncCallMean(lqnModel, callerTask, calleeTask));
                timeAtCalleePerVisit = cMean * sojournCallee;
            } else {
                timeAtCalleePerVisit = sojournCallee;
            }
            return Math.max(0.0, nCaller / xCaller - timeAtCalleePerVisit);
        }
        double refChainThink = LqnGraph.getRootRefThinkTime(lqnModel, callerTask)
                + LqnGraph.computeCallerChainDemand(lqnModel, callerTask);
        double directThink   = callerThink + LqnGraph.computeLocalDemand(lqnModel, callerTask);
        double siblingBlock  = computeSiblingCalleeBlocking(callerTask, calleeTask);
        return Math.max(refChainThink, directThink) + siblingBlock;
    }

    /**
     * Per-cycle time the caller spends blocked at sibling callees — every callee
     * of {@code callerTask} except {@code excludedCallee}, weighted by call mean.
     *
     * <p>Per-callee contribution = {@code callMean × R(sibling)}:
     * <ul>
     *   <li><b>Leaf sibling</b>: cached {@code R_task_total} already embeds
     *       {@code callMean} (its T: layer demand was set as
     *       {@code callMean × hostDemand}) → use as-is.</li>
     *   <li><b>Intermediate sibling</b>: cached R is per-call → multiply by
     *       caller-specific {@code callMean}.</li>
     * </ul>
     * If the sibling has no cache entry yet (first sweep), fall back to
     * {@code callMean × localDemand(sibling)} as a lower bound so first-iteration
     * Z is finite and non-degenerate.
     */
    private double computeSiblingCalleeBlocking(String callerTask, String excludedCallee) {
        Task caller = LqnGraph.findTask(lqnModel, callerTask);
        if (caller == null) return 0.0;
        Map<String, Double> siblingCallMean = new HashMap<String, Double>();
        for (Activity act : caller.getActivities()) {
            Map<Integer, String> dests = act.getSyncCallDests();
            if (dests == null || dests.isEmpty()) continue;
            Matrix means = act.getSyncCallMeans();
            for (Map.Entry<Integer, String> e : dests.entrySet()) {
                Entry destEntry = LqnGraph.findEntry(lqnModel, e.getValue());
                if (destEntry == null || destEntry.getParent() == null) continue;
                String calleeName = destEntry.getParent().getName();
                if (calleeName.equals(excludedCallee) || calleeName.equals(callerTask)) continue;
                int idx = e.getKey();
                double cMean = (means != null && means.getNumCols() > idx) ? means.get(0, idx) : 1.0;
                if (!Double.isFinite(cMean) || cMean <= 0) cMean = 1.0;
                Double accum = siblingCallMean.get(calleeName);
                siblingCallMean.put(calleeName, (accum != null ? accum : 0.0) + cMean);
            }
        }
        double total = 0.0;
        for (Map.Entry<String, Double> e : siblingCallMean.entrySet()) {
            String sibling = e.getKey();
            double cMean = e.getValue();
            Double rCached = taskSojournCache.get(sibling);
            double rContrib;
            if (rCached != null && Double.isFinite(rCached) && rCached > 0) {
                rContrib = LqnGraph.taskHasSyncCallees(lqnModel, sibling)
                        ? rCached * cMean : rCached;
            } else {
                rContrib = cMean * LqnGraph.computeLocalDemand(lqnModel, sibling);
            }
            total += rContrib;
        }
        return total;
    }

    /** Step (2): keep the caller's own T: layer Clients up-to-date with the
     *  cumulative blocking the caller has experienced. */
    private void updateCallerOwnTaskLayerClients(int currentLayerIdx, String callerTask) {
        Integer callerTaskLayer = queueNameToLayer.get("T:" + callerTask);
        if (callerTaskLayer == null || callerTaskLayer == currentLayerIdx) return;
        Network callerLayer = ensemble.get(callerTaskLayer);
        Delay clients = findClientsDelay(callerLayer);
        if (clients == null) return;
        JobClass mainCls = MvaInputs.getMainClass(callerLayer);
        double newZ = LqnGraph.getRootRefThinkTime(lqnModel, callerTask)
                + LqnGraph.computeCallerChainDemand(lqnModel, callerTask);
        clients.setService(mainCls, Exp.fitMean(clamp(newZ)));
    }

    /**
     * Step (6): write per-class {@code D_local + R_callee_share} to the caller's
     * T: layer server.
     *
     * <p>For a single-class caller layer this reduces to the plain
     * {@code localDemand(callerTask) + propagatedTaskResp}. For a multi-class
     * caller layer (e.g. a shared task called by several REF caller classes)
     * each class's demand is recomputed per-class:
     *
     * <ul>
     *   <li><b>per-class local demand</b> = Σ over class-task activities that
     *       sync-call into {@code callerTask}: {@code callMean × hostDemand(bound activity)}.
     *       (This is the same formula {@link EnsembleInitialiser#setPerCallerDemand}
     *       uses for initial per-class demand.)</li>
     *   <li><b>per-class downstream share</b> =
     *       {@code propagatedTaskResp × (perClassCallMean / totalCallMeanToCallee)},
     *       where {@code perClassCallMean} = Σ over bound activities' sync-calls
     *       into {@code calleeTask}: {@code callMean(boundAct → calleeTask)}.</li>
     * </ul>
     *
     * Apportioning by callMean fraction works for both leaf and intermediate
     * callees because {@code propagatedTaskResp} = total {@code callMean} ×
     * per-call response in both cases (see step (4) → (6) above).
     */
    private void pushResponseToCallerTaskLayer(int currentLayerIdx, String callerTask,
                                                String calleeTask, double propagatedTaskResp,
                                                double totalCallMeanToCallee) {
        Integer callerTaskLayer = queueNameToLayer.get("T:" + callerTask);
        if (callerTaskLayer == null || callerTaskLayer == currentLayerIdx) return;
        Network callerLayer = ensemble.get(callerTaskLayer);
        Queue callerServer = MvaInputs.findNonDelayQueue(callerLayer);
        if (callerServer == null) return;

        List<ClosedClass> classes = MvaInputs.getClosedClasses(callerLayer);
        if (classes.size() <= 1) {
            // Single-class layer: keep the original aggregate formula.
            JobClass cls = findActiveQueueClass(callerLayer);
            if (cls == null) return;
            double localDemand = LqnGraph.computeLocalDemand(lqnModel, callerTask);
            callerServer.setService(cls, Exp.fitMean(clamp(localDemand + propagatedTaskResp)));
            return;
        }

        // Multi-class layer: write per-class D_r = local_r + share_r × propagatedTaskResp.
        boolean canApportion = Double.isFinite(totalCallMeanToCallee) && totalCallMeanToCallee > 0;
        for (ClosedClass cc : classes) {
            String classTaskName = LqnGraph.stripPrefix(cc.getName());
            Task classTask = LqnGraph.findTask(lqnModel, classTaskName);
            if (classTask == null) continue;

            double localPerClass = 0.0;
            double classCallMeanToCallee = 0.0;
            for (Activity act : classTask.getActivities()) {
                Map<Integer, String> dests = act.getSyncCallDests();
                if (dests == null || dests.isEmpty()) continue;
                Matrix means = act.getSyncCallMeans();
                for (Map.Entry<Integer, String> ce : dests.entrySet()) {
                    Entry calledEntry = LqnGraph.findEntry(lqnModel, ce.getValue());
                    if (calledEntry == null || calledEntry.getParent() == null) continue;
                    if (!callerTask.equals(calledEntry.getParent().getName())) continue;
                    int idx = ce.getKey();
                    double classCM = (means != null && means.getNumCols() > idx) ? means.get(0, idx) : 1.0;
                    if (!Double.isFinite(classCM) || classCM <= 0) continue;
                    localPerClass += classCM * LqnGraph.hostDemandOfBoundActivity(calledEntry);
                    Activity bound = LqnGraph.findBoundActivity(calledEntry);
                    if (bound == null) continue;
                    Map<Integer, String> bDests = bound.getSyncCallDests();
                    if (bDests == null || bDests.isEmpty()) continue;
                    Matrix bMeans = bound.getSyncCallMeans();
                    for (Map.Entry<Integer, String> bce : bDests.entrySet()) {
                        Entry bCallee = LqnGraph.findEntry(lqnModel, bce.getValue());
                        if (bCallee == null || bCallee.getParent() == null) continue;
                        if (!calleeTask.equals(bCallee.getParent().getName())) continue;
                        int bIdx = bce.getKey();
                        double bCM = (bMeans != null && bMeans.getNumCols() > bIdx) ? bMeans.get(0, bIdx) : 1.0;
                        if (Double.isFinite(bCM) && bCM > 0) classCallMeanToCallee += classCM * bCM;
                    }
                }
            }

            double share = canApportion ? classCallMeanToCallee / totalCallMeanToCallee : 0.0;
            double downstream = share * propagatedTaskResp;
            double dr = localPerClass + downstream;
            if (!Double.isFinite(dr) || dr <= 0) continue;
            callerServer.setService(cc, Exp.fitMean(clamp(dr)));
        }
    }

    /**
     * Step (7): refresh the callee's <i>host</i>-layer Clients think time so the
     * processor sees the right idle gap between visits.
     *
     * <ul>
     *   <li><b>Leaf callee</b> (no sync callees): if the subtree below has no
     *       server demand at all, Z = intrinsic think (the only source of pacing);
     *       otherwise Z = max(intrinsic think, (N_total − Q_server_total)/X_total)
     *       — Little's law on the customers not currently at the server.</li>
     *   <li><b>Intermediate callee</b>: Z = max(intrinsic think, caller-chain think)
     *       + (R_task_total − D_local_callee). Adds the time the callee itself
     *       waits for its own callees on top of the upstream blocking.</li>
     * </ul>
     *
     * Skipped entirely for rebuilt Case-B layers (they have constant per-caller
     * think times set during the rebuild).
     */
    private void updateCalleeHostLayerClients(int currentLayerIdx, String calleeTask,
                                              LayerSolve s, double R_task_total) {
        String calleeProcessor = taskToProcessor.get(calleeTask);
        if (calleeProcessor == null) return;
        Integer calleeHostLayer = queueNameToLayer.get("P:" + calleeProcessor);
        if (calleeHostLayer == null || calleeHostLayer == currentLayerIdx) return;
        if (rebuiltHostLayers.containsKey(calleeHostLayer)) return;

        boolean isLeaf = !LqnGraph.taskHasSyncCallees(lqnModel, calleeTask);
        double calleeThink = LqnGraph.getTaskThinkTimeSafe(lqnModel, calleeTask);
        double Z_callee_host;

        if (isLeaf) {
            int R_task = s.N.getNumCols();
            double Q_server_total = 0.0, N_task_total = 0.0, X_task_total = 0.0;
            for (int r2 = 0; r2 < R_task; r2++) {
                Q_server_total += s.res.Q.get(s.serverIdx, r2);
                N_task_total   += s.N.get(0, r2);
                X_task_total   += s.res.X.get(0, r2);
            }
            if (!(Double.isFinite(Q_server_total) && X_task_total > EPS)) return;

            if (!LqnGraph.hasServerDemandInSubtree(lqnModel, calleeTask)) {
                Z_callee_host = calleeThink;
            } else {
                Z_callee_host = Math.max(calleeThink, (N_task_total - Q_server_total) / X_task_total);
            }
        } else {
            // Intermediate task: account for time spent waiting for its own callees.
            double localD = LqnGraph.computeLocalDemand(lqnModel, calleeTask);
            double chainZ = LqnGraph.getRootRefThinkTime(lqnModel, calleeTask)
                    + LqnGraph.computeCallerChainDemand(lqnModel, calleeTask);
            Z_callee_host = Math.max(calleeThink, chainZ) + (R_task_total - localD);
        }

        Delay calleeHostClients = findClientsDelay(ensemble.get(calleeHostLayer));
        if (calleeHostClients == null) return;
        JobClass calleeHostMain = MvaInputs.getMainClass(ensemble.get(calleeHostLayer));
        calleeHostClients.setService(calleeHostMain, Exp.fitMean(clamp(Z_callee_host)));
    }

    /**
     * Step (8): for each caller class in this T: layer, push the per-class
     * response (Q_r/X_r) up to the <i>caller</i>'s host layer Clients delay.
     * INF-scheduled callees use R = D (no queueing) since they behave as IS.
     */
    private void pushPerCallerResponseToHostLayers(int currentLayerIdx, Network layer,
                                                   LayerSolve s, String calleeTask) {
        List<ClosedClass> callerClasses = MvaInputs.getClosedClasses(layer);
        boolean calleeIsInf = LqnGraph.isInfScheduledTask(lqnModel, calleeTask);
        boolean calleeIsIntermediate = LqnGraph.taskHasSyncCallees(lqnModel, calleeTask);

        for (int r2 = 0; r2 < callerClasses.size(); r2++) {
            String callerTask_r = LqnGraph.stripPrefix(callerClasses.get(r2).getName());
            double x_r = s.res.X.get(0, r2);
            double q_r = s.res.Q.get(s.serverIdx, r2);
            if (!Double.isFinite(x_r) || x_r <= EPS || !Double.isFinite(q_r)) continue;

            // INF callees act as IS (no queueing) — use D directly, not Q/X.
            double R_task_r = calleeIsInf ? s.L.get(s.serverIdx, r2) : q_r / x_r;
            // Intermediate: R is per-call → multiply by callMean.
            // Leaf: R already embeds callMean (T: layer demand = callMean × hostDemand).
            double propagated = calleeIsIntermediate
                    ? R_task_r * LqnGraph.getSyncCallMean(lqnModel, callerTask_r, calleeTask)
                    : R_task_r;

            String callerProcessor = taskToProcessor.get(callerTask_r);
            if (callerProcessor == null) continue;
            Integer hostLayer = queueNameToLayer.get("P:" + callerProcessor);
            if (hostLayer == null || hostLayer == currentLayerIdx) continue;

            Network callerHost = ensemble.get(hostLayer);
            Delay clients = findClientsDelay(callerHost);
            if (clients == null) continue;
            JobClass cls = findClassByTaskName(callerHost, callerTask_r);
            if (cls == null) cls = MvaInputs.getMainClass(callerHost);
            if (cls == null) continue;

            // Each per-callee invocation of this method only knows the response
            // from one callee; the sibling-block term sums response time from
            // the caller's other callees so the host-layer Z reflects the full
            // caller cycle rather than just the last callee solved.
            double siblingBlock = computeSiblingCalleeBlocking(callerTask_r, calleeTask);
            double newZ = LqnGraph.getRootRefThinkTime(lqnModel, callerTask_r)
                    + LqnGraph.computeCallerChainDemand(lqnModel, callerTask_r)
                    + propagated
                    + siblingBlock;
            if (!Double.isFinite(newZ)) continue;
            clients.setService(cls, Exp.fitMean(clamp(newZ)));
        }
    }


    // =================================================================================
    //  Host-layer (P:) coupling
    // =================================================================================

    /**
     * Propagation rules after solving a P:&lt;proc&gt; host layer:
     *
     * <ul>
     *   <li><b>Rebuilt Case-B layer</b>: each per-caller R:&lt;caller&gt; class's
     *       response time (Q_r/X_r) is the hosted task's per-class service time;
     *       write each directly into the hosted task's T: layer server.</li>
     *   <li><b>Normal layer</b>: for each hosted-task class, take R_proc = Q_r/X_r.
     *       <ul>
     *         <li>If the hosted task has its own T: layer, write R_proc into <i>every</i>
     *             non-Disabled class of that layer's server.</li>
     *         <li>If the hosted task is REF (no T: layer), inject R_proc into the
     *             Clients delay of every T: layer that this REF task calls into,
     *             as Z = baseClientsThink + R_proc.</li>
     *       </ul></li>
     * </ul>
     */
    private void propagateHostLayerCoupling(int l, LayerSolve s) {
        if (rebuiltHostLayers.containsKey(l)) {
            propagateRebuiltHostLayer(l, s);
            return;
        }

        List<ClosedClass> hostClasses = MvaInputs.getClosedClasses(s.layer);
        // For unsaturated multi-server PS processors (c > 1, ρ ≤ 0.9) propagate
        // the bare per-call demand rather than Q/X — the M/M/c queueing tail is
        // negligible in this regime and the bare demand is the per-call service
        // time the task layer should see. Single-server PS keeps Q/X because
        // R = D/(1−ρ) genuinely tracks queueing for c == 1.
        //
        // Exception: INF-scheduled hosted tasks. Their T: layer queue is IS, so
        // it does NOT re-add queueing on top of the demand we write. The
        // host-layer M/M/c queueing is the only place that delay can come from,
        // so we MUST send Q/X for INF callees regardless of total ρ. Without
        // this, fan-out / mixed-INF models lose the host-side queueing tail
        // entirely and underestimate response times at moderate loads.
        int nserv = s.serverQueue.getNumberOfServers();
        if (nserv == Integer.MAX_VALUE || nserv <= 0) nserv = 1;
        double totalRho = 0.0;
        for (int rr = 0; rr < hostClasses.size(); rr++) {
            double xr = s.res.X.get(0, rr);
            double dr = s.serverQueue.getServiceProcess(hostClasses.get(rr)).getMean();
            if (Double.isFinite(xr) && Double.isFinite(dr)) totalRho += xr * dr;
        }
        totalRho /= nserv;
        boolean baseUseBareDemand = nserv > 1 && totalRho <= 0.9;

        for (int r = 0; r < hostClasses.size(); r++) {
            double x_r = s.res.X.get(0, r);
            double q_r = s.res.Q.get(s.serverIdx, r);
            if (!Double.isFinite(x_r) || x_r <= EPS || !Double.isFinite(q_r)) continue;
            double R_proc_r = q_r / x_r;
            if (!Double.isFinite(R_proc_r) || R_proc_r <= EPS) continue;

            String hostedTask = LqnGraph.stripPrefix(hostClasses.get(r).getName());
            Integer taskLayer = queueNameToLayer.get("T:" + hostedTask);
            double D_proc = s.serverQueue.getServiceProcess(hostClasses.get(r)).getMean();
            boolean useBareDemand = baseUseBareDemand
                    && !LqnGraph.isInfScheduledTask(lqnModel, hostedTask);
            double safeR = (useBareDemand && Double.isFinite(D_proc) && D_proc > EPS)
                    ? clamp(D_proc) : clamp(R_proc_r);

            if (taskLayer == null) {
                pushRefTaskHostResponseToCalleeClients(hostedTask, safeR);
            } else {
                if (taskLayer == l) continue;
                if (LqnGraph.taskHasSyncCallees(lqnModel, hostedTask)) continue;
                writeHostResponseToTaskLayerServer(taskLayer, safeR);
            }
        }
    }

    /** Case-B rebuilt layer: each R:&lt;caller&gt; class's Q/X is the per-class
     *  service time of the hosted task. Push directly into its T: layer server. */
    private void propagateRebuiltHostLayer(int l, LayerSolve s) {
        String hostedName = rebuiltHostLayers.get(l);
        Integer hostedTaskLayer = queueNameToLayer.get("T:" + hostedName);
        if (hostedTaskLayer == null || hostedTaskLayer == l) return;
        Queue hostedServer = MvaInputs.findNonDelayQueue(ensemble.get(hostedTaskLayer));
        if (hostedServer == null) return;

        List<ClosedClass> hostClasses = MvaInputs.getClosedClasses(s.layer);
        for (int r = 0; r < hostClasses.size(); r++) {
            double x_r = s.res.X.get(0, r);
            double q_r = s.res.Q.get(s.serverIdx, r);
            if (!Double.isFinite(x_r) || x_r <= EPS || !Double.isFinite(q_r)) continue;
            double R_proc_r = q_r / x_r;
            if (!Double.isFinite(R_proc_r) || R_proc_r <= EPS) continue;
            String callerName = LqnGraph.stripPrefix(hostClasses.get(r).getName());
            JobClass tClass = findClassByTaskName(ensemble.get(hostedTaskLayer), callerName);
            if (tClass == null) continue;
            hostedServer.setService(tClass, Exp.fitMean(clamp(R_proc_r)));
        }
    }

    /** REF task with no T: layer: inject R_proc into Clients of every T: layer
     *  that has this REF task as a caller class, plus the time the REF spends
     *  at sibling callees so a fan-out caller's Z reflects the full cycle and
     *  not just R(this callee). */
    private void pushRefTaskHostResponseToCalleeClients(String refTaskName, double safeR) {
        for (Map.Entry<String, Integer> entry : queueNameToLayer.entrySet()) {
            if (!entry.getKey().startsWith("T:")) continue;
            int calleeLay = entry.getValue();
            Network calleeNet = ensemble.get(calleeLay);
            JobClass calleeClass = findClassByTaskName(calleeNet, refTaskName);
            if (calleeClass == null) calleeClass = MvaInputs.getMainClass(calleeNet);
            if (calleeClass == null) continue;
            if (!refTaskName.equals(LqnGraph.stripPrefix(calleeClass.getName()))) continue;
            Delay calleeClients = findClientsDelay(calleeNet);
            if (calleeClients == null) continue;
            String calleeTaskName = LqnGraph.stripPrefix(entry.getKey());
            double siblingBlock = computeSiblingCalleeBlocking(refTaskName, calleeTaskName);
            double newZ = clamp(baseClientsThink[calleeLay] + safeR + siblingBlock);
            calleeClients.setService(calleeClass, Exp.fitMean(newZ));
        }
    }

    /** Write {@code safeR} into every non-Disabled class on the task's T: layer
     *  queue server — the processor response is the per-call service time the
     *  task layer should see for all callers. For INF tasks on multi-server PS
     *  processors the host-layer R_proc fluctuates between bare-demand and
     *  queueing-tail values across sweeps, so the write is under-relaxed in
     *  that regime to keep the demand channel from oscillating. */
    private void writeHostResponseToTaskLayerServer(int taskLayerIdx, double safeR) {
        Network taskNet = ensemble.get(taskLayerIdx);
        Queue tq = MvaInputs.findNonDelayQueue(taskNet);
        if (tq == null) return;
        boolean dampD = isInfOnMultiServerPs(LqnGraph.stripPrefix(tq.getName()));
        for (JobClass tc : taskNet.getClasses()) {
            double m = tq.getServiceProcess(tc).getMean();
            if (Double.isNaN(m)) continue;
            double value = dampD ? D_RELAX_ALPHA * safeR + (1.0 - D_RELAX_ALPHA) * m : safeR;
            tq.setService(tc, Exp.fitMean(value));
        }
    }

    /** Under-relaxation weight for D-writes into T: layers of INF tasks on
     *  finite-server PS/FCFS processors. */
    private static final double D_RELAX_ALPHA = 0.5;

    /** True iff {@code taskName} is INF-scheduled on a finite-server PS/FCFS
     *  processor. */
    private boolean isInfOnMultiServerPs(String taskName) {
        if (!LqnGraph.isInfScheduledTask(lqnModel, taskName)) return false;
        Task t = LqnGraph.findTask(lqnModel, taskName);
        if (t == null || t.getProcessor() == null) return false;
        if (LqnGraph.isInfProcessor(lqnModel, t.getProcessor().getName())) return false;
        int procServers = t.getProcessor().getMultiplicity();
        return procServers > 1 && procServers != Integer.MAX_VALUE;
    }


    // =================================================================================
    //  Network-introspection helpers (private to the coupling stages)
    // =================================================================================

    private Delay findClientsDelay(Network net) {
        for (Node node : net.getNodes()) {
            if (node instanceof Delay && "Clients".equals(node.getName())) return (Delay) node;
        }
        return null;
    }

    /** Find any class in {@code net} whose service mean at the non-Delay queue
     *  is not NaN — i.e. the "active" class for the layer's queue. */
    private JobClass findActiveQueueClass(Network net) {
        Queue q = MvaInputs.findNonDelayQueue(net);
        if (q == null) return null;
        for (JobClass jc : net.getClasses()) {
            if (!Double.isNaN(q.getServiceProcess(jc).getMean())) return jc;
        }
        return null;
    }

    /** Find a class on {@code net} whose unprefixed name matches {@code taskName}. */
    private JobClass findClassByTaskName(Network net, String taskName) {
        for (JobClass jc : net.getClasses()) {
            if (taskName.equals(LqnGraph.stripPrefix(jc.getName()))) return jc;
        }
        return null;
    }


    // =================================================================================
    //  Misc utilities
    // =================================================================================

    /** Clamp a value into the safe MVA-input range to avoid 0 / NaN / Inf. */
    private static double clamp(double v) {
        return Math.max(CLAMP_FLOOR, Math.min(v, MAX_PROTECTION));
    }

    /** Map sweep index 0..2N-2 to layer index, producing the bounce sequence
     *  {@code 0,1,...,N-1,N-2,...,1}. */
    private int computeSweepLayerIndex(int sweepIndex) {
        return (sweepIndex < N_LAYERS) ? sweepIndex : 2 * N_LAYERS - 2 - sweepIndex;
    }
}
