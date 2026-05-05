package jline.solvers.ln_simple;

import jline.lang.JobClass;
import jline.lang.Network;
import jline.lang.layered.Task;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import jline.io.Ret;
import jline.solvers.ln_simple.utils.SolverLnSimpleBugModels;
import jline.util.matrix.Matrix;
import jline.lang.layered.LayeredNetwork;
import jline.lang.nodes.Delay;
import jline.lang.nodes.Queue;
import jline.lang.processes.Exp;
import jline.solvers.LayeredNetworkAvgTable;
import jline.solvers.ln_simple.utils.SolverLNSimpleResultsUtils;
import jline.solvers.ln_simple.utils.MvaUtils;


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
        double[] prevX = new double[N_LAYERS];

        for (int iter = 0; iter < maxIter; iter++) {
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
                double deltaX = Math.abs(throughput - prevX[l]);
                if (deltaX > maxDeltaX) maxDeltaX = deltaX;
                prevX[l] = throughput;

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
                        JobClass calleeMain = MvaUtils.getMainClass(layer);
                        // Use max of (REF-chain think + chain demands) and (direct caller's think +
                        // local demand) so that intermediate caller think times are not silently dropped.
                        double refChainThink = getRootRefThinkTime(calleeTask) + computeCallerChainDemand(calleeTask);
                        double directCallerThink = getTaskThinkTimeSafe(callerTask) + computeLocalDemand(callerTask);
                        double calleeThink = Math.max(1e-9, Math.min(
                                Math.max(refChainThink, directCallerThink), MAX_PROTECTION));
                        calleeClients.setService(calleeMain, Exp.fitMean(calleeThink));
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
                            double Q_server = res.Q.get(serverNodeIndex, 0);
                            double N_task = N.get(0, 0);
                            if (Double.isFinite(Q_server) && throughput > EPS) {
                                double calleeThinkTime = 0.0;
                                for (Task t : lqnModel.getTasks().values()) {
                                    if (calleeTask.equals(t.getName())) {
                                        double m = t.getThinkTimeMean();
                                        if (!Double.isNaN(m)) calleeThinkTime = m;
                                        break;
                                    }
                                }
                                double Z_callee_host;
                                // If the callee task and its synchronous callees have no server demand,
                                // skip the (N-Q)/X formula which degenerates when D=0 and instead
                                // use the task's think/root-ref-chain demand (no server-side queuing).
                                if (!hasServerDemandInSubtree(calleeTask)) {
                                    // No server-side demand in callee subtree: only the callee's own
                                    // think time contributes at the host. Avoid using chain-level
                                    // demands which create tautologies when D=0.
                                    Z_callee_host = Math.max(1e-9, Math.min(calleeThinkTime, MAX_PROTECTION));
                                } else {
                                    Z_callee_host = Math.max(calleeThinkTime, (N_task - Q_server) / throughput);
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

                    String callerProcessor = taskToProcessor.get(callerTask);
                    if (callerProcessor == null) {
                        continue;
                    }

                    Integer hostLayer = queueNameToLayer.get("P:" + callerProcessor);
                    if (hostLayer == null || hostLayer == l) {
                        continue;
                    }

                    Delay targetClients = findClientsDelay(ensemble.get(hostLayer));
                    if (targetClients == null) {
                        continue;
                    }

                    JobClass targetMain = MvaUtils.getMainClass(ensemble.get(hostLayer));
                    double newThink = getRootRefThinkTime(callerTask) + computeCallerChainDemand(callerTask) + propagatedTaskResp;
                    if (!Double.isFinite(newThink)) {
                        continue;
                    }
                    newThink = Math.max(1e-9, Math.min(newThink, MAX_PROTECTION));
                    targetClients.setService(targetMain, Exp.fitMean(newThink));
                } else {
                    // Host submodel: feed R_proc into the matching task layer
                    double R_proc = Double.NaN;
                    if (Double.isFinite(throughput) && throughput > EPS) {
                        double q = res.Q.get(serverNodeIndex, 0);
                        if (Double.isFinite(q)) R_proc = q / throughput;
                    }

                    if (!Double.isFinite(R_proc)) {
                        continue;
                    }

                    if (R_proc <= EPS) {
                        continue;
                    }

                    String processorName = layerBase;
                    List<String> hostedTasks = processorToTasks.get(processorName);
                    if (hostedTasks == null) {
                        continue;
                    }

                    for (String hostedTask : hostedTasks) {
                        Integer taskLayer = queueNameToLayer.get("T:" + hostedTask);
                        if (taskLayer == null) {
                            // hostedTask is a REF task with no own task layer.
                            // Propagate R_proc into the Clients delay of every task layer
                            // where this REF task is the caller (e.g. T2 layer driven by T1).
                            double safeR = Math.max(1e-9, Math.min(R_proc, MAX_PROTECTION));
                            for (Map.Entry<String, Integer> entry : queueNameToLayer.entrySet()) {
                                if (!entry.getKey().startsWith("T:")) continue;
                                int calleeLay = entry.getValue();
                                Network calleeNet = ensemble.get(calleeLay);
                                JobClass calleeMain = MvaUtils.getMainClass(calleeNet);
                                if (calleeMain == null) continue;
                                if (!hostedTask.equals(stripPrefix(calleeMain.getName()))) continue;
                                Delay calleeClients = findClientsDelay(calleeNet);
                                if (calleeClients == null) continue;
                                double newThink = Math.max(1e-9, Math.min(baseClientsThink[calleeLay] + safeR, MAX_PROTECTION));
                                calleeClients.setService(calleeMain, Exp.fitMean(newThink));
                                hostContribToCalleeClients[calleeLay] = safeR;
                            }
                            continue;
                        }
                        if (taskLayer == l) {
                            continue;
                        }
                        // Skip intermediate tasks: their server service (D_local + R_callee) is
                        // set correctly by the task->caller-service coupling; overwriting with
                        // R_proc (D_local only) would drop the callee response contribution.
                        if (taskHasSyncCallees(hostedTask)) {
                            continue;
                        }
                        Queue tq = MvaUtils.findNonDelayQueue(ensemble.get(taskLayer));
                        if (tq == null) {
                            continue;
                        }

                        JobClass activeClass = findActiveQueueClass(ensemble.get(taskLayer));
                        if (activeClass == null) {
                            continue;
                        }
                        double safeR = Math.max(1e-9, Math.min(R_proc, MAX_PROTECTION));
                        tq.setService(activeClass, Exp.fitMean(safeR));
                    }
                }

            }

            refreshEnsemble();

            if (afterIteration != null) {
                afterIteration.run();
            }

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
