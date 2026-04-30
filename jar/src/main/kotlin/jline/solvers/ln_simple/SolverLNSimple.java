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

    public static void main(String[] args) throws Exception {
        SolverLNSimple s = new SolverLNSimple(SolverLnSimpleBugModels.bug1network3());
        s.iterateCoupledMva(100, 1e-4);
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
            System.out.println("iteration " + iter);
            double maxDeltaX = 0.0;

            int sweepLen = 2 * N_LAYERS - 1;
            for (int si = 0; si < sweepLen; si++) {
                int l = computeSweepLayerIndex(si);
                System.out.println("sweep layer index " + l);
                Network layer = ensemble.get(l);

                List<jline.lang.nodes.Node> nodes = MvaUtils.getQueueNodes(layer);
                int M = nodes.size();

                Matrix N = MvaUtils.buildN(layer);
                Matrix Z = MvaUtils.buildThinkTimeMatrix(layer);
                Matrix L = MvaUtils.buildDemandMatrix(nodes, layer);
                Matrix S = MvaUtils.buildServerMatrix(nodes, N);

                // Print MVA input matrices for diagnostics
                System.out.println("  MVA inputs for layer " + l + " (" + layer.getName() + "):");
                System.out.println("   N =\n" + N.toString());
                System.out.println("   Z =\n" + Z.toString());
                System.out.println("   L =\n" + L.toString());
                System.out.println("   S =\n" + S.toString());

                // Print node index -> queue mapping and per-class service means
                System.out.println("  Node index mapping:");
                for (int i = 0; i < M; i++) {
                    jline.lang.nodes.Node nd = nodes.get(i);
                    System.out.println("   idx=" + i + " name=" + nd.getName() + " class=" + nd.getClass().getSimpleName());
                    if (nd instanceof jline.lang.nodes.ServiceStation) {
                        for (jline.lang.JobClass jc : layer.getClasses()) {
                            double m = ((jline.lang.nodes.ServiceStation) nd).getServiceProcess(jc).getMean();
                            System.out.println("    svc for class " + jc.getName() + " = " + m);
                        }
                    }
                }

                double firstService = (L.getNumRows() > 0) ? L.get(0, 0) : 0.0;
                System.out.println(" Solving layer " + l + " (" + layer.getName() + "): Z=" + Z.get(0, 0) + " S=" + firstService);

                Ret.pfqnMVA res = null;
                try {
                    res = MvaUtils.callMVA(L, N, Z, S);
                } catch (Exception e) {
                    System.out.println(" MVA threw exception for layer " + l + " (" + layer.getName() + "): " + e.getMessage());
                    e.printStackTrace();
                    return;
                }
                if (res == null) return;

                double throughput = res.X.get(0, 0);
                System.out.println(" Layer " + l + " throughput X=" + throughput);

                // Print per-node MVA results with mapping
                System.out.println("  MVA per-node results (index:name):");
                for (int i = 0; i < M; i++) {
                    String nname = nodes.get(i).getName();
                    double qi = Double.NaN;
                    double ui = Double.NaN;
                    try { qi = res.Q.get(i, 0); } catch (Exception ex) {}
                    try { ui = res.U.get(i, 0); } catch (Exception ex) {}
                    System.out.println("   idx=" + i + " name=" + nname + " Q=" + qi + " U=" + ui);
                }

                double deltaX = Math.abs(throughput - prevX[l]);
                if (deltaX > maxDeltaX) maxDeltaX = deltaX;
                prevX[l] = throughput;

                // --- Coupling ---
                Queue serverQueue = MvaUtils.findNonDelayQueue(layer);
                if (serverQueue == null) continue;

                if (!Double.isFinite(throughput) || throughput <= EPS) {
                    System.out.println(" Warning: non-finite or zero throughput for layer " + l + " (" + layer.getName() + ") -> skipping coupling updates");
                    continue;
                }

                // Index of the non-Delay Queue in the MVA node list
                int serverNodeIndex = -1;
                for (int i = 0; i < M; i++) {
                    if (!(nodes.get(i) instanceof Delay)) { serverNodeIndex = i; break; }
                }
                if (serverNodeIndex < 0) continue;

                String serverQueueName = serverQueue.getName();
                String layerName = layer.getName();
                String layerBase = stripPrefix(layerName);
                boolean isFcfsLayer = serverQueueName.startsWith("T:");

                if (isFcfsLayer) {
                    // Task submodel: feed back to host layers
                    double R_task_total = 0.0;
                    if (Double.isFinite(throughput) && throughput > EPS) {
                        for (int j = 0; j < M; j++) {
                            double qj = res.Q.get(j, 0);
                            if (!Double.isFinite(qj)) continue;
                            R_task_total += qj / throughput;
                        }

                        // If nested sync-call blocking has been propagated into Clients delay,
                        // include only the incremental part over the baseline think time.
                        JobClass layerMain = MvaUtils.getMainClass(layer);
                        if (layerMain != null && layerMain.getName().startsWith("R:")) {
                            Delay layerClients = findClientsDelay(layer);
                            if (layerClients != null) {
                                double clientMean = layerClients.getServiceProcess(layerMain).getMean();
                                if (Double.isFinite(clientMean)) {
                                    // Subtract any R_proc contribution added via host->callee-clients
                                    // coupling so it is not double-counted in the P1 Clients update.
                                    double extraBlocking = clientMean - baseClientsThink[l] - hostContribToCalleeClients[l];
                                    if (extraBlocking > 0) {
                                        R_task_total += extraBlocking;
                                    }
                                }
                            }
                        }
                    } else {
                        R_task_total = Double.NaN;
                    }

                    if (!Double.isFinite(R_task_total)) {
                        System.out.println("  Warning: computed NaN/Inf R_task_total for layer " + l + " -> skipping task->host updates");
                        continue;
                    }

                    String callerTask = stripPrefix(MvaUtils.getMainClass(layer).getName());

                    Integer callerTaskLayer = queueNameToLayer.get("T:" + callerTask);
                    if (callerTaskLayer != null && callerTaskLayer != l) {
                        Delay callerTaskClients = findClientsDelay(ensemble.get(callerTaskLayer));
                        if (callerTaskClients != null) {
                            JobClass callerTaskMain = MvaUtils.getMainClass(ensemble.get(callerTaskLayer));
                            double callerTaskThink = Math.max(1e-9, Math.min(baseClientsThink[callerTaskLayer] + R_task_total, MAX_PROTECTION));
                            callerTaskClients.setService(callerTaskMain, Exp.fitMean(callerTaskThink));
                            System.out.println(" [task->caller-task] " + layerName + " -> L" + callerTaskLayer + " think=" + callerTaskThink);
                        }

                        Delay calleeClients = findClientsDelay(layer);
                        if (calleeClients != null) {
                            JobClass calleeMain = MvaUtils.getMainClass(layer);
                            double calleeThink = Math.max(1e-9, Math.min(baseClientsThink[callerTaskLayer], MAX_PROTECTION));
                            calleeClients.setService(calleeMain, Exp.fitMean(calleeThink));
                            System.out.println(" [caller->callee-think] " + layerName + " think=" + calleeThink);
                        }
                    }

                    // Update callee's host (P:P2) think time.
                    // Z_callee_host = max(calleeTask.thinkTime, (N - Q_server) / X).
                    // The callee task's own think time sets a lower bound: even if the caller's
                    // cycle is fast, the callee is unavailable for at least its own think period.
                    String calleeTask = stripPrefix(serverQueueName);
                    String calleeProcessor = taskToProcessor.get(calleeTask);
                    if (calleeProcessor != null) {
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
                                double Z_callee_host = Math.max(calleeThinkTime, (N_task - Q_server) / throughput);
                                Z_callee_host = Math.max(1e-9, Math.min(Z_callee_host, MAX_PROTECTION));
                                Delay calleeHostClients = findClientsDelay(ensemble.get(calleeHostLayer));
                                if (calleeHostClients != null) {
                                    JobClass calleeHostMain = MvaUtils.getMainClass(ensemble.get(calleeHostLayer));
                                    calleeHostClients.setService(calleeHostMain, Exp.fitMean(Z_callee_host));
                                    System.out.println(" [task->callee-host] " + layerName + " callee=" + calleeTask + " -> L" + calleeHostLayer + " Z=" + Z_callee_host);
                                }
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
                    double newThink = baseClientsThink[hostLayer] + R_task_total;
                    if (!Double.isFinite(newThink)) {
                        continue;
                    }
                    newThink = Math.max(1e-9, Math.min(newThink, MAX_PROTECTION));
                    targetClients.setService(targetMain, Exp.fitMean(newThink));
                    System.out.println(" [task->host] " + layerName + " callerTask=" + callerTask + " callerProcessor=" + callerProcessor + " think=" + newThink);
                } else {
                    // Host submodel: feed R_proc into the matching task layer
                    double R_proc = Double.NaN;
                    if (Double.isFinite(throughput) && throughput > EPS) {
                        double q = res.Q.get(serverNodeIndex, 0);
                        if (Double.isFinite(q)) R_proc = q / throughput;
                    }

                    if (!Double.isFinite(R_proc)) {
                        System.out.println("  Warning: computed NaN/Inf R_proc for layer " + l + " -> skipping host->task updates");
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
                                System.out.println(" [host->callee-clients via ref] " + layerName + " hostedTask=" + hostedTask + " -> L" + calleeLay + " think=" + newThink);
                            }
                            continue;
                        }
                        if (taskLayer == l) {
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
                        System.out.println(" [host->task] " + layerName + " -> " + tq.getName() + " service=" + safeR);
                    }
                }

            }

            refreshEnsemble();

            if (afterIteration != null) {
                afterIteration.run();
            }

            if (maxDeltaX < tol) {
                System.out.println(" iterateCoupledMva: converged after " + (iter + 1) + " iterations");
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
}
