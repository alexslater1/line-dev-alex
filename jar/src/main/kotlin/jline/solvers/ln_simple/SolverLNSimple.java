package jline.solvers.ln_simple;

import jline.lang.ClosedClass;
import jline.lang.JobClass;
import jline.lang.Network;
import jline.lang.layered.Activity;
import jline.lang.layered.Entry;
import jline.lang.layered.LayeredNetwork;
import jline.lang.layered.Task;
import jline.lang.nodes.Delay;
import jline.lang.nodes.Queue;
import jline.lang.processes.Exp;
import jline.io.Ret;
import jline.solvers.LayeredNetworkAvgTable;
import jline.solvers.ln_simple.utils.MvaUtils;
import jline.solvers.ln_simple.utils.SolverLNSimpleResultsUtils;
import jline.util.matrix.Matrix;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;


public class SolverLNSimple {

    private int N_LAYERS;
    private List<Network> ensemble;
    private final double[] baseClientsThink; // initial Clients Delay think per layer, used as base for coupling
    private double[] hostContribToCalleeClients; // R_proc added to each callee-layer's Clients via host->callee coupling
    private LayeredNetwork lqnModel; // Save the original LQN model
    private LayeredNetworkAvgTable lastAvgTable;
    private final Map<String, String> taskToProcessor = new HashMap<String, String>();
    private final Map<String, List<String>> processorToTasks = new HashMap<String, List<String>>();
    private final Map<String, Integer> queueNameToLayer = new HashMap<String, Integer>();
    private static final double EPS = 1e-12;
    private static final double MAX_PROTECTION = 1e6;

    public SolverLNSimple(LayeredNetwork model) {
        this.lqnModel = model;
        ensemble = model.getEnsemble();
        N_LAYERS = ensemble.size();

        // Capture base Clients think times before any coupling modifies them
        baseClientsThink = new double[N_LAYERS];
        hostContribToCalleeClients = new double[N_LAYERS];
        for (int l = 0; l < N_LAYERS; l++) {
            Delay d = findClientsDelay(ensemble.get(l));
            if (d != null) {
                JobClass mc = MvaUtils.getMainClass(ensemble.get(l));
                double m = d.getServiceProcess(mc).getMean();
                baseClientsThink[l] = Double.isNaN(m) ? 0.0 : m;
            }

            Queue q = MvaUtils.findNonDelayQueue(ensemble.get(l));
            if (q != null) {
                queueNameToLayer.put(q.getName(), l);
            }
        }

        for (Task task : lqnModel.getTasks().values()) {
            if (task.getProcessor() == null) {
                continue;
            }
            String taskName = task.getName();
            String processorName = task.getProcessor().getName();
            taskToProcessor.put(taskName, processorName);

            List<String> hostedTasks = processorToTasks.get(processorName);
            if (hostedTasks == null) {
                hostedTasks = new ArrayList<String>();
                processorToTasks.put(processorName, hostedTasks);
            }
            hostedTasks.add(taskName);
        }

        // Fix per-class server demands in ensemble layers.
        // The ensemble builder sets Disabled (NaN) service at the server for all caller classes.
        // buildDemandMatrix falls back to the first non-NaN class, which is always the first
        // call class — giving the same demand D_T1 to every class including T2, T3, etc.
        // This loop sets the correct per-class demand for each caller class in each layer.
        //
        // Two layer types need fixing:
        //   T: task layers  (e.g. T:T3): have per-caller classes R:T1, R:T2 with demand
        //                                = callMean × hostedActivity.hostDemand
        //   P: host layers (e.g. P:P2):  have either per-caller classes (for REF tasks on
        //                                the processor, correctly named R:Txx) which need
        //                                per-caller demand setting, or a single aggregate
        //                                class (T:Txx). Aggregate classes need population-
        //                                weighted demand over all callers; this is now
        //                                handled by a guard clause in the ClosedClass loop.
        for (int l = 0; l < N_LAYERS; l++) {
            Network layer = ensemble.get(l);
            Queue serverQueue = MvaUtils.findNonDelayQueue(layer);
            if (serverQueue == null) continue;
            String serverQueueName = serverQueue.getName();
            String serverTaskOrProc = stripPrefix(serverQueueName);
            List<ClosedClass> layerClasses = MvaUtils.getClosedClasses(layer);

            // Build tasksOnServer: the set of tasks whose service is represented by this server node.
            // For T: layers: the server IS the task itself (T3 for T:T3).
            // For P: layers: the server is the processor, so tasksOnServer = all tasks on that proc.
            Map<String, Task> tasksOnServer = new HashMap<String, Task>();
            if (serverQueueName.startsWith("T:")) {
                for (Task t : lqnModel.getTasks().values()) {
                    if (t.getName().equals(serverTaskOrProc)) {
                        tasksOnServer.put(t.getName(), t);
                        break;
                    }
                }
            } else if (serverQueueName.startsWith("P:")) {
                for (Task t : lqnModel.getTasks().values()) {
                    if (t.getProcessor() != null && t.getProcessor().getName().equals(serverTaskOrProc)) {
                        tasksOnServer.put(t.getName(), t);
                    }
                }
            } else {
                continue;
            }

            for (ClosedClass hc : layerClasses) {
                // P: layer with aggregate "T:xxx" class: the existing per-caller path yields
                // demand=0 (aggTask has no sync calls to itself). Compute the correct
                // population-weighted average over all tasks that call entries on aggTask.
                if (serverQueueName.startsWith("P:") && hc.getName().startsWith("T:")) {
                    String aggTaskName = stripPrefix(hc.getName());
                    double totalN = 0.0, weightedD = 0.0;
                    for (Task caller : lqnModel.getTasks().values()) {
                        int mult = caller.getMultiplicity();
                        if (mult <= 0 || mult == Integer.MAX_VALUE) continue;
                        double d_r = 0.0;
                        for (Activity act : caller.getActivities()) {
                            Map<Integer, String> callDests = act.getSyncCallDests();
                            Matrix callMeans = act.getSyncCallMeans();
                            for (int ci = 0; ci < callDests.size(); ci++) {
                                String destEntryName = callDests.get(ci);
                                Entry destEntry = null;
                                entrySearch:
                                for (Task t : lqnModel.getTasks().values()) {
                                    for (Entry e : t.getEntries()) {
                                        if (e.getName().equals(destEntryName)) { destEntry = e; break entrySearch; }
                                    }
                                }
                                if (destEntry == null || !aggTaskName.equals(destEntry.getParent().getName())) continue;
                                for (Activity serverAct : destEntry.getParent().getActivities()) {
                                    if (destEntryName.equals(serverAct.getBoundToEntry())) {
                                        d_r += callMeans.get(0, ci) * serverAct.getHostDemandMean();
                                        break;
                                    }
                                }
                            }
                        }
                        if (d_r <= 0) continue;
                        weightedD += mult * d_r;
                        totalN += mult;
                    }
                    if (totalN > 0 && weightedD > 0) {
                        serverQueue.setService(hc, Exp.fitMean(weightedD / totalN));
                    }
                    continue;  // skip existing per-caller path below
                }

                String callerTaskName = stripPrefix(hc.getName());
                Task callerTask = null;
                for (Task t : lqnModel.getTasks().values()) {
                    if (t.getName().equals(callerTaskName)) { callerTask = t; break; }
                }
                if (callerTask == null) continue;

                // Compute demand = sum over all sync calls from callerTask to entries on tasksOnServer
                double demand = 0.0;
                for (Activity act : callerTask.getActivities()) {
                    Map<Integer, String> callDests = act.getSyncCallDests();
                    Matrix callMeans = act.getSyncCallMeans();
                    for (int ci = 0; ci < callDests.size(); ci++) {
                        String destEntryName = callDests.get(ci);
                        Entry destEntry = null;
                        outer:
                        for (Task t : lqnModel.getTasks().values()) {
                            for (Entry e : t.getEntries()) {
                                if (e.getName().equals(destEntryName)) { destEntry = e; break outer; }
                            }
                        }
                        if (destEntry == null) continue;
                        Task serverTask = destEntry.getParent();
                        if (!tasksOnServer.containsKey(serverTask.getName())) continue;
                        for (Activity serverAct : serverTask.getActivities()) {
                            if (destEntryName.equals(serverAct.getBoundToEntry())) {
                                double callMean = callMeans.get(0, ci);
                                demand += callMean * serverAct.getHostDemandMean();
                                break;
                            }
                        }
                    }
                }
                if (demand > 0) {
                    serverQueue.setService(hc, Exp.fitMean(demand));
                }
            }
        }
    }

    public LayeredNetworkAvgTable getAvgTable() {
        return lastAvgTable;
    }

    public List<Network> getEnsemble() {
        return ensemble;
    }

    public void iterateCoupledMva(int maxIter, double tol) {
        iterateCoupledMva(maxIter, tol, null);
    }

    public void iterateCoupledMva(int maxIter, double tol, Runnable afterIteration) {
        long outerStartTime = System.nanoTime();
        double[][] prevX = new double[N_LAYERS][];

        for (int iter = 0; iter < maxIter; iter++) {
            long solveStartTime = System.nanoTime();
            double maxDeltaX = 0.0;

            int sweepLen = 2 * N_LAYERS - 1;
            for (int si = 0; si < sweepLen; si++) {
                int l = computeSweepLayerIndex(si);
                Network layer = ensemble.get(l);

                List<jline.lang.nodes.Node> nodes = MvaUtils.getQueueNodes(layer);
                int M = nodes.size();

                Matrix N = MvaUtils.buildN(layer);
                Matrix Z = MvaUtils.buildThinkTimeMatrix(layer);
                Matrix L = MvaUtils.buildDemandMatrix(nodes, layer);
                Matrix S = MvaUtils.buildServerMatrix(nodes, N);

                Ret.pfqnMVA res = null;
                try {
                    res = MvaUtils.callMVA(L, N, Z, S);
                } catch (Exception e) {
                    e.printStackTrace();
                    return;
                }
                if (res == null) return;

                double throughput = res.X.get(0, 0);
                int R_layer = res.X.getNumCols();
                if (prevX[l] == null) prevX[l] = new double[R_layer];
                for (int r = 0; r < R_layer; r++) {
                    double x_r = res.X.get(0, r);
                    double deltaX = Math.abs(x_r - prevX[l][r]);
                    if (deltaX > maxDeltaX) maxDeltaX = deltaX;
                    prevX[l][r] = x_r;
                }

                // --- Coupling ---
                Queue serverQueue = MvaUtils.findNonDelayQueue(layer);
                if (serverQueue == null) continue;

                String serverQueueName = serverQueue.getName();
                String layerName = layer.getName();
                String layerBase = stripPrefix(layerName);
                boolean isFcfsLayer = serverQueueName.startsWith("T:");

                String callerTask = null;
                String calleeTask = null;
                if (isFcfsLayer) {
                    callerTask = stripPrefix(MvaUtils.getMainClass(layer).getName());
                    calleeTask = stripPrefix(serverQueueName);

                    // Keep task-layer Clients updated even when the current solve is degenerate
                    // (e.g. D=0 and stale Z causes non-finite X). This lets subsequent sweeps
                    // recover to finite values.
                    Delay calleeClients = findClientsDelay(layer);
                    if (calleeClients != null) {
                        List<ClosedClass> callerClasses = MvaUtils.getClosedClasses(layer);
                        for (ClosedClass callerCl : callerClasses) {
                            String callerTask_c = stripPrefix(callerCl.getName());
                            double refChainThink = getRootRefThinkTime(callerTask_c) + computeCallerChainDemand(callerTask_c);
                            double directCallerThink = getTaskThinkTimeSafe(callerTask_c) + computeLocalDemand(callerTask_c);
                            double calleeThink = Math.max(1e-9, Math.min(Math.max(refChainThink, directCallerThink), MAX_PROTECTION));
                            calleeClients.setService(callerCl, Exp.fitMean(calleeThink));
                        }
                    }

                    Integer callerTaskLayer = queueNameToLayer.get("T:" + callerTask);
                    if (callerTaskLayer != null && callerTaskLayer != l) {
                        Delay callerTaskClients = findClientsDelay(ensemble.get(callerTaskLayer));
                        if (callerTaskClients != null) {
                            JobClass callerTaskMain = MvaUtils.getMainClass(ensemble.get(callerTaskLayer));
                            double callerTaskThink = Math.max(1e-9, Math.min(
                                    getRootRefThinkTime(callerTask) + computeCallerChainDemand(callerTask), MAX_PROTECTION));
                            callerTaskClients.setService(callerTaskMain, Exp.fitMean(callerTaskThink));
                        }
                    }
                }

                if (!Double.isFinite(throughput) || throughput <= EPS) {
                    continue;
                }

                // Index of the non-Delay Queue in the MVA node list
                int serverNodeIndex = -1;
                for (int i = 0; i < M; i++) {
                    if (!(nodes.get(i) instanceof Delay)) { serverNodeIndex = i; break; }
                }
                if (serverNodeIndex < 0) continue;

                if (isFcfsLayer) {
                    // Task submodel: feed back to host layers.
                    // Use server-only response (Q_server/X) so that Clients think time injected
                    // by Fix 1 is not double-counted when propagating response to caller/host layers.
                    double R_task_total = Double.NaN;
                    if (Double.isFinite(throughput) && throughput > EPS) {
                        double q_server = res.Q.get(serverNodeIndex, 0);
                        if (Double.isFinite(q_server)) {
                            R_task_total = q_server / throughput;
                        }
                    }

                    if (!Double.isFinite(R_task_total)) {
                        continue;
                    }

                    double syncCallMean = getSyncCallMean(callerTask, calleeTask);
                    double propagatedTaskResp = R_task_total * syncCallMean;

                    Integer callerTaskLayer = queueNameToLayer.get("T:" + callerTask);
                    if (callerTaskLayer != null && callerTaskLayer != l) {
                        // Set caller's task layer server to D_local + R_callee so that the callee
                        // response time is visible to the MVA solve (works for all caller types).
                        Queue callerServerQueue = MvaUtils.findNonDelayQueue(ensemble.get(callerTaskLayer));
                        if (callerServerQueue != null) {
                            JobClass callerServerClass = findActiveQueueClass(ensemble.get(callerTaskLayer));
                            if (callerServerClass != null) {
                                double localDemand = computeLocalDemand(callerTask);
                                double safeD = Math.max(1e-9, Math.min(localDemand + propagatedTaskResp, MAX_PROTECTION));
                                callerServerQueue.setService(callerServerClass, Exp.fitMean(safeD));
                            }
                        }
                    }

                    // Update callee's host think time.
                    String calleeProcessor = taskToProcessor.get(calleeTask);
                    if (calleeProcessor != null && !taskHasSyncCallees(calleeTask)) {
                        Integer calleeHostLayer = queueNameToLayer.get("P:" + calleeProcessor);
                        if (calleeHostLayer != null && calleeHostLayer != l) {
                            int R_task = N.getNumCols();
                            double Q_server_total = 0.0, N_task_total = 0.0, X_task_total = 0.0;
                            for (int r2 = 0; r2 < R_task; r2++) {
                                Q_server_total += res.Q.get(serverNodeIndex, r2);
                                N_task_total += N.get(0, r2);
                                X_task_total += res.X.get(0, r2);
                            }
                            if (Double.isFinite(Q_server_total) && X_task_total > EPS) {
                                double calleeThinkTime = 0.0;
                                for (Task t : lqnModel.getTasks().values()) {
                                    if (calleeTask.equals(t.getName())) {
                                        double m = t.getThinkTimeMean();
                                        if (!Double.isNaN(m)) calleeThinkTime = m;
                                        break;
                                    }
                                }
                                double Z_callee_host;
                                if (!hasServerDemandInSubtree(calleeTask)) {
                                    Z_callee_host = Math.max(1e-9, Math.min(calleeThinkTime, MAX_PROTECTION));
                                } else {
                                    Z_callee_host = Math.max(calleeThinkTime, (N_task_total - Q_server_total) / X_task_total);
                                    Z_callee_host = Math.max(1e-9, Math.min(Z_callee_host, MAX_PROTECTION));
                                }
                                Delay calleeHostClients = findClientsDelay(ensemble.get(calleeHostLayer));
                                if (calleeHostClients != null) {
                                    JobClass calleeHostMain = MvaUtils.getMainClass(ensemble.get(calleeHostLayer));
                                    calleeHostClients.setService(calleeHostMain, Exp.fitMean(Z_callee_host));
                                }
                            }
                        }
                    } else if (calleeProcessor != null && taskHasSyncCallees(calleeTask)) {
                        // Intermediate callee: its host Clients = max(callee think, caller-chain think)
                        // + time the callee waits for its own callees (R_task_total minus D_local).
                        // This covers both the callee-bottleneck and caller-bottleneck cases.
                        Integer calleeHostLayer = queueNameToLayer.get("P:" + calleeProcessor);
                        if (calleeHostLayer != null && calleeHostLayer != l) {
                            double calleeThinkTime = 0.0;
                            for (Task t : lqnModel.getTasks().values()) {
                                if (calleeTask.equals(t.getName())) {
                                    double m = t.getThinkTimeMean();
                                    if (!Double.isNaN(m)) calleeThinkTime = m;
                                    break;
                                }
                            }
                            double calleeLocalDemand = computeLocalDemand(calleeTask);
                            double callerChainZ = getRootRefThinkTime(calleeTask) + computeCallerChainDemand(calleeTask);
                            double Z_callee_host = Math.max(calleeThinkTime, callerChainZ)
                                    + (R_task_total - calleeLocalDemand);
                            Z_callee_host = Math.max(1e-9, Math.min(Z_callee_host, MAX_PROTECTION));
                            Delay calleeHostClients = findClientsDelay(ensemble.get(calleeHostLayer));
                            if (calleeHostClients != null) {
                                JobClass calleeHostMain = MvaUtils.getMainClass(ensemble.get(calleeHostLayer));
                                calleeHostClients.setService(calleeHostMain, Exp.fitMean(Z_callee_host));
                            }
                        }
                    }

                    List<ClosedClass> callerClassesList = MvaUtils.getClosedClasses(layer);
                    for (int r2 = 0; r2 < callerClassesList.size(); r2++) {
                        String callerTask_r = stripPrefix(callerClassesList.get(r2).getName());
                        double x_r2 = res.X.get(0, r2);
                        if (!Double.isFinite(x_r2) || x_r2 <= EPS) continue;
                        double q_r2 = res.Q.get(serverNodeIndex, r2);
                        if (!Double.isFinite(q_r2)) continue;
                        double R_task_r = q_r2 / x_r2;
                        double syncMean_r = getSyncCallMean(callerTask_r, calleeTask);
                        double propagated_r = R_task_r * syncMean_r;
                        String callerProcessor_r = taskToProcessor.get(callerTask_r);
                        if (callerProcessor_r == null) continue;
                        Integer hostLayer_r = queueNameToLayer.get("P:" + callerProcessor_r);
                        if (hostLayer_r == null || hostLayer_r == l) continue;
                        Delay targetClients_r = findClientsDelay(ensemble.get(hostLayer_r));
                        if (targetClients_r == null) continue;
                        JobClass targetClass_r = findClassByTaskName(ensemble.get(hostLayer_r), callerTask_r);
                        if (targetClass_r == null) targetClass_r = MvaUtils.getMainClass(ensemble.get(hostLayer_r));
                        if (targetClass_r == null) continue;
                        double newThink_r = getRootRefThinkTime(callerTask_r) + computeCallerChainDemand(callerTask_r) + propagated_r;
                        if (!Double.isFinite(newThink_r)) continue;
                        newThink_r = Math.max(1e-9, Math.min(newThink_r, MAX_PROTECTION));
                        targetClients_r.setService(targetClass_r, Exp.fitMean(newThink_r));
                    }
                } else {
                    // Host submodel: feed R_proc into the matching task layer, one class at a time
                    List<ClosedClass> hostClasses = MvaUtils.getClosedClasses(layer);
                    int R_host = hostClasses.size();
                    for (int r = 0; r < R_host; r++) {
                        double x_r = res.X.get(0, r);
                        if (!Double.isFinite(x_r) || x_r <= EPS) continue;
                        double q_r = res.Q.get(serverNodeIndex, r);
                        if (!Double.isFinite(q_r)) continue;
                        double R_proc_r = q_r / x_r;
                        if (!Double.isFinite(R_proc_r) || R_proc_r <= EPS) continue;

                        String hostedTask = stripPrefix(hostClasses.get(r).getName());
                        Integer taskLayer = queueNameToLayer.get("T:" + hostedTask);
                        if (taskLayer == null) {
                            // hostedTask is a REF task with no own task layer.
                            // Propagate R_proc into the Clients delay of every task layer
                            // where this REF task is the caller.
                            double safeR = Math.max(1e-9, Math.min(R_proc_r, MAX_PROTECTION));
                            for (Map.Entry<String, Integer> entry : queueNameToLayer.entrySet()) {
                                if (!entry.getKey().startsWith("T:")) continue;
                                int calleeLay = entry.getValue();
                                Network calleeNet = ensemble.get(calleeLay);
                                // Find the class in the callee layer that corresponds to this REF task
                                JobClass calleeClass = findClassByTaskName(calleeNet, hostedTask);
                                if (calleeClass == null) calleeClass = MvaUtils.getMainClass(calleeNet);
                                if (calleeClass == null) continue;
                                if (!hostedTask.equals(stripPrefix(calleeClass.getName()))) continue;
                                Delay calleeClients = findClientsDelay(calleeNet);
                                if (calleeClients == null) continue;
                                double newThink = Math.max(1e-9, Math.min(baseClientsThink[calleeLay] + safeR, MAX_PROTECTION));
                                calleeClients.setService(calleeClass, Exp.fitMean(newThink));
                                hostContribToCalleeClients[calleeLay] = safeR;
                            }
                            continue;
                        }
                        if (taskLayer == l) continue;
                        if (taskHasSyncCallees(hostedTask)) continue;
                        Queue tq = MvaUtils.findNonDelayQueue(ensemble.get(taskLayer));
                        if (tq == null) continue;
                        // Update all classes that currently have non-NaN demand at the task server
                        // (call classes N=0 hold the actual demand; caller classes N>0 are Disabled initially)
                        double safeR = Math.max(1e-9, Math.min(R_proc_r, MAX_PROTECTION));
                        for (JobClass tc : ensemble.get(taskLayer).getClasses()) {
                            double m = tq.getServiceProcess(tc).getMean();
                            if (!Double.isNaN(m)) {
                                tq.setService(tc, Exp.fitMean(safeR));
                            }
                        }
                    }
                }

            }

            double solveTime = (System.nanoTime() - solveStartTime) / 1000000000.0;

            long synchStartTime = System.nanoTime();

            refreshEnsemble();

            if (afterIteration != null) {
                afterIteration.run();
            }

            double synchTime = (System.nanoTime() - synchStartTime) / 1000000000.0;
            double totalRuntime = (System.nanoTime() - outerStartTime) / 1000000000.0;

            System.out.printf("\nIter %2d. Analyze time: %.3fs. Update time: %.3fs. Runtime: %.3fs. ",
                    iter + 1, solveTime, synchTime, totalRuntime);

            if (maxDeltaX < tol) {
                break;
            }
        }

        // Collect final results and print using LayeredNetworkAvgTable
        lastAvgTable = SolverLNSimpleResultsUtils.collectAndPrintFinalResults(lqnModel, ensemble, N_LAYERS);
    }

    private void refreshEnsemble() {
        for (Network layer : ensemble) {
            layer.refreshProcesses();
        }
    }

    private String stripPrefix(String name) {
        int idx = name.indexOf(':');
        if (idx >= 0 && idx + 1 < name.length()) {
            return name.substring(idx + 1);
        }
        return name;
    }

    // --- MVA matrix builders ---

    // Delegated to MvaUtils

    // --- Node/class lookup helpers ---

    private Delay findClientsDelay(Network net) {
        for (jline.lang.nodes.Node node : net.getNodes()) {
            if (node instanceof Delay && "Clients".equals(node.getName())) return (Delay) node;
        }
        return null;
    }

    // findNonDelayQueue delegated to MvaUtils; keep this helper removed to avoid duplication

    private JobClass findActiveQueueClass(Network net) {
        Queue q = MvaUtils.findNonDelayQueue(net);
        if (q == null) return null;
        for (JobClass jc : net.getClasses()) {
            double m = q.getServiceProcess(jc).getMean();
            if (!Double.isNaN(m)) return jc;
        }
        return null;
    }

    private JobClass findClassByTaskName(Network net, String taskName) {
        for (JobClass jc : net.getClasses()) {
            if (taskName.equals(stripPrefix(jc.getName()))) return jc;
        }
        return null;
    }

    private int computeSweepLayerIndex(int sweepIndex) {
        return (sweepIndex < N_LAYERS) ? sweepIndex : 2 * N_LAYERS - 2 - sweepIndex;
    }

    private double computeLocalDemand(String taskName) {
        for (Task task : lqnModel.getTasks().values()) {
            if (taskName.equals(task.getName())) {
                double total = 0.0;
                for (jline.lang.layered.Activity act : task.getActivities()) {
                    double m = act.getHostDemandMean();
                    if (!Double.isNaN(m) && m > 1e-7) {
                        total += m;
                    }
                }
                return total;
            }
        }
        return 0.0;
    }

    private String findCallerTask(String calleeName) {
        for (Task task : lqnModel.getTasks().values()) {
            for (jline.lang.layered.Activity act : task.getActivities()) {
                for (Map.Entry<Integer, String> e : act.getSyncCallDests().entrySet()) {
                    for (jline.lang.layered.Entry entry : lqnModel.getEntries().values()) {
                        if (e.getValue().equals(entry.getName())) {
                            if (entry.getParent() != null && calleeName.equals(entry.getParent().getName())) {
                                return task.getName();
                            }
                        }
                    }
                }
            }
        }
        return null;
    }

    private double getTaskThinkTimeSafe(String taskName) {
        for (Task t : lqnModel.getTasks().values()) {
            if (taskName.equals(t.getName())) {
                double m = t.getThinkTimeMean();
                return (Double.isNaN(m) || !Double.isFinite(m)) ? 0.0 : m;
            }
        }
        return 0.0;
    }

    private double getRootRefThinkTime(String taskName) {
        String current = taskName;
        java.util.Set<String> visited = new java.util.HashSet<String>();
        while (current != null && !visited.contains(current)) {
            visited.add(current);
            for (Task t : lqnModel.getTasks().values()) {
                if (current.equals(t.getName())) {
                    if (t.getScheduling() == jline.lang.constant.SchedStrategy.REF) {
                        double m = t.getThinkTimeMean();
                        return (Double.isNaN(m) || !Double.isFinite(m)) ? 0.0 : m;
                    }
                    break;
                }
            }
            String caller = findCallerTask(current);
            if (caller == null) break;
            current = caller;
        }
        return 0.0;
    }

    private double computeCallerChainDemand(String taskName) {
        double total = 0.0;
        String current = taskName;
        java.util.Set<String> visited = new java.util.HashSet<String>();
        while (current != null && !visited.contains(current)) {
            visited.add(current);
            String caller = findCallerTask(current);
            if (caller == null) break;
            total += computeLocalDemand(caller);
            current = caller;
        }
        return total;
    }

    private boolean taskHasSyncCallees(String taskName) {
        for (Task task : lqnModel.getTasks().values()) {
            if (taskName.equals(task.getName())) {
                for (jline.lang.layered.Activity act : task.getActivities()) {
                    if (act.getSyncCallDests() != null && !act.getSyncCallDests().isEmpty()) {
                        return true;
                    }
                }
                return false;
            }
        }
        return false;
    }

    // Returns true if the given task or any of its synchronous callees (recursively)
    // have host/server demand (i.e., computeLocalDemand > EPS). This helps detect
    // whether a callee subtree contributes any server-side queuing; if not, host-level
    // Clients should not rely on the (N-Q)/X formula.
    private boolean hasServerDemandInSubtree(String taskName) {
        java.util.Set<String> visited = new java.util.HashSet<String>();
        return hasServerDemandInSubtree(taskName, visited);
    }

    private boolean hasServerDemandInSubtree(String taskName, java.util.Set<String> visited) {
        if (taskName == null || visited.contains(taskName)) return false;
        visited.add(taskName);

        double local = computeLocalDemand(taskName);
        if (local > 1e-9) return true;

        // Check synchronous callees recursively
        for (Task task : lqnModel.getTasks().values()) {
            if (!taskName.equals(task.getName())) continue;
            for (jline.lang.layered.Activity act : task.getActivities()) {
                Map<Integer, String> dests = act.getSyncCallDests();
                if (dests == null || dests.isEmpty()) continue;
                for (Map.Entry<Integer, String> e : dests.entrySet()) {
                    for (jline.lang.layered.Entry entry : lqnModel.getEntries().values()) {
                        if (!e.getValue().equals(entry.getName())) continue;
                        if (entry.getParent() == null) continue;
                        String calleeTask = entry.getParent().getName();
                        if (hasServerDemandInSubtree(calleeTask, visited)) return true;
                    }
                }
            }
        }
        return false;
    }

    private double getSyncCallMean(String callerTask, String calleeTask) {
        if (callerTask == null || calleeTask == null) {
            return 1.0;
        }
        Task caller = null;
        for (Task task : lqnModel.getTasks().values()) {
            if (callerTask.equals(task.getName())) {
                caller = task;
                break;
            }
        }
        if (caller == null) {
            return 1.0;
        }

        double totalCallMean = 0.0;
        for (jline.lang.layered.Activity activity : caller.getActivities()) {
            Map<Integer, String> dests = activity.getSyncCallDests();
            if (dests == null || dests.isEmpty()) {
                continue;
            }
            Matrix means = activity.getSyncCallMeans();
            for (Map.Entry<Integer, String> entry : dests.entrySet()) {
                jline.lang.layered.Entry calledEntry = null;
                for (jline.lang.layered.Entry candidate : lqnModel.getEntries().values()) {
                    if (entry.getValue().equals(candidate.getName())) {
                        calledEntry = candidate;
                        break;
                    }
                }
                if (calledEntry == null || calledEntry.getParent() == null) {
                    continue;
                }
                if (!calleeTask.equals(calledEntry.getParent().getName())) {
                    continue;
                }
                int idx = entry.getKey();
                double m = 1.0;
                if (means != null && means.getNumCols() > idx) {
                    m = means.get(0, idx);
                }
                if (Double.isFinite(m) && m > 0) {
                    totalCallMean += m;
                }
            }
        }
        return totalCallMean > 0 ? totalCallMean : 1.0;
    }
}
