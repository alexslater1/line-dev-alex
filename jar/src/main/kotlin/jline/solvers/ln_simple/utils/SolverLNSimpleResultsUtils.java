package jline.solvers.ln_simple.utils;

import jline.VerboseLevel;
import jline.io.Ret;
import jline.lang.ClosedClass;
import jline.lang.Network;
import jline.lang.constant.ActivityPrecedenceType;
import jline.lang.constant.SchedStrategy;
import jline.lang.layered.Activity;
import jline.lang.layered.ActivityPrecedence;
import jline.lang.layered.Entry;
import jline.lang.layered.LayeredNetwork;
import jline.lang.layered.Task;
import jline.lang.nodes.Node;
import jline.lang.nodes.Queue;
import jline.solvers.LayeredNetworkAvgTable;
import jline.solvers.SolverOptions;
import jline.util.matrix.Matrix;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

/**
 * Shared result collection and printing helpers for SolverLNSimple variants.
 */
public final class SolverLNSimpleResultsUtils {

    private SolverLNSimpleResultsUtils() {
    }

    public static LayeredNetworkAvgTable collectAndPrintFinalResults(LayeredNetwork lqnModel, List<Network> ensemble, int nLayers, Map<String, Double> taskSojournCache) {
        List<String> finalNodeNames = new ArrayList<>();
        List<String> finalNodeTypes = new ArrayList<>();
        List<Double> finalQLen = new ArrayList<>();
        List<Double> finalUtil = new ArrayList<>();
        List<Double> finalRespT = new ArrayList<>();
        List<Double> finalResidT = new ArrayList<>();
        List<Double> finalArvR = new ArrayList<>();
        List<Double> finalTput = new ArrayList<>();

        Map<String, Double> processorUtil = new HashMap<>();
        Map<String, Double> refTaskProcQLen = new HashMap<>();

        Map<String, Double> taskQLen = new HashMap<>();
        Map<String, Map<String, Double>> taskClassQLen = new HashMap<>();
        Map<String, Double> taskUtil = new HashMap<>();
        Map<String, Double> taskRespT = new HashMap<>();
        Map<String, Double> taskResidT = new HashMap<>();
        Map<String, Double> taskTput = new HashMap<>();
        Map<String, Double> hostLayerTput = new HashMap<>();
        Map<String, Double> hostLayerResid = new HashMap<>();
        Map<String, Double> processorDemand = new HashMap<>();
        Map<String, Double> processorServers = new HashMap<>();

        Map<String, String> refTaskCalledTask = buildRefTaskCalledTaskMap(lqnModel);
        Map<String, Double> refTaskCalledTaskMean = buildRefTaskCalledTaskMeanMap(lqnModel, refTaskCalledTask);
        Map<String, String> taskCalledTask = buildTaskCalledTaskMap(lqnModel);

        for (int l = 0; l < nLayers; l++) {
            Network layer = ensemble.get(l);
            List<Node> nodes = MvaUtils.getQueueNodes(layer);

            if (nodes.isEmpty()) {
                continue;
            }


            Matrix n = MvaUtils.buildN(layer);
            Matrix z = MvaUtils.buildThinkTimeMatrix(layer);
            Matrix demand = MvaUtils.buildDemandMatrix(nodes, layer);
            Matrix servers = MvaUtils.buildServerMatrix(nodes, n);
            Ret.pfqnMVA res = MvaUtils.callMVA(demand, n, z, servers);
            if (res == null) {
                continue;
            }

            Queue serverQueue = MvaUtils.findNonDelayQueue(layer);
            if (serverQueue == null) {
                continue;
            }

            int serverNodeIndex = -1;
            for (int i = 0; i < nodes.size(); i++) {
                if (nodes.get(i) == serverQueue) {
                    serverNodeIndex = i;
                    break;
                }
            }

            double x = res.X.get(0, 0);
            double q = res.Q.get(serverNodeIndex, 0);
            double u = res.U.get(serverNodeIndex, 0);
            double r = (x > 0) ? (q / x) : Double.NaN;

            String serverQueueName = serverQueue.getName();
            String taskName = stripPrefix(serverQueueName);
            String mainClassTaskName = stripPrefix(MvaUtils.getMainClass(layer).getName());

            if (serverQueueName.startsWith("P:")) {
                List<ClosedClass> hostClasses = MvaUtils.getClosedClasses(layer);
                int R_host = hostClasses.size();
                double c = servers.get(serverNodeIndex, 0);
                boolean isInfProc = isInfProcessor(lqnModel, taskName);
                double c_eff = isInfProc ? 1.0 : c;
                processorServers.put(taskName, c_eff);

                // Total demand and utilization across all hosted classes
                double totalD = 0.0;
                double totalUtil = 0.0;
                for (int r2 = 0; r2 < R_host; r2++) {
                    double x_r = res.X.get(0, r2);
                    double d_r = demand.get(serverNodeIndex, r2);
                    totalD += d_r;
                    if (Double.isFinite(x_r) && x_r > 0 && Double.isFinite(d_r) && c_eff > 0) {
                        totalUtil += x_r * d_r / c_eff;
                    }
                }
                if (!isInfProc) totalUtil = Math.min(totalUtil, 1.0);
                processorDemand.put(taskName, totalD);
                processorUtil.put(taskName, totalUtil);

                // Per-class metrics
                for (int r2 = 0; r2 < R_host; r2++) {
                    double x_r = res.X.get(0, r2);
                    double q_r = res.Q.get(serverNodeIndex, r2);
                    double resp_r = (x_r > 0) ? (q_r / x_r) : Double.NaN;
                    String hostedTaskName = stripPrefix(hostClasses.get(r2).getName());
                    for (Task task : lqnModel.getTasks().values()) {
                        if (!task.getName().equals(hostedTaskName)) continue;
                        if (task.getScheduling() == SchedStrategy.REF) {
                            refTaskProcQLen.put(task.getName(), q_r);
                            taskUtil.put(task.getName(), totalUtil);
                            taskRespT.put(task.getName(), Double.NaN);
                            taskResidT.put(task.getName(), resp_r);
                            taskTput.put(task.getName(), x_r);
                        } else if (Double.isFinite(x_r) && x_r > 0) {
                            hostLayerTput.put(task.getName(), x_r);
                            if (Double.isFinite(resp_r) && resp_r > 0) {
                                hostLayerResid.put(task.getName(), resp_r);
                            }
                        }
                        break;
                    }
                }
            } else if (serverQueueName.startsWith("T:")) {
                Task currentTask = findTaskByName(lqnModel, taskName);
                if (!isRefTask(currentTask)) {
                    taskQLen.put(taskName, q);
                    // Store per-caller-class QLen so asymmetric multiclass attributions are correct.
                    List<ClosedClass> tLayerClasses = MvaUtils.getClosedClasses(layer);
                    Map<String, Double> perCallerQ = new HashMap<String, Double>();
                    for (int rc = 0; rc < tLayerClasses.size(); rc++) {
                        String callerName = stripPrefix(tLayerClasses.get(rc).getName());
                        perCallerQ.put(callerName, res.Q.get(serverNodeIndex, rc));
                    }
                    taskClassQLen.put(taskName, perCallerQ);
                }
                // Task utilization = per-server utilization of its processor; use processorUtil if already set.
                String procForTask = (currentTask != null && currentTask.getProcessor() != null) ? currentTask.getProcessor().getName() : null;
                double u_task = (procForTask != null && processorUtil.containsKey(procForTask)) ? processorUtil.get(procForTask) : u;
                taskUtil.put(taskName, u_task);
                if (isRefTask(currentTask)) {
                    taskRespT.put(taskName, r);
                } else {
                    taskRespT.put(taskName, Double.NaN);
                }
                if (!isRefTask(currentTask)) {
                    // For immediate tasks (zero host demand), the residual time is 0,
                    // not the callee response propagated into the server queue by coupling.
                    double taskHostDemand = computeTaskHostDemand(currentTask);
                    double residToStore = taskHostDemand <= 1e-7 ? 0.0 : r;
                    // For leaf tasks (no sync callees), T: layer demand = callMean × hostDemand,
                    // so r = callMean × R_per_call. resolveTaskResponseTime multiplies by
                    // callMean again → divide here to store per-call residual.
                    if (taskHostDemand > 1e-7 && !taskCalledTask.containsKey(taskName)) {
                        double cm = getTotalInboundCallMean(lqnModel, taskName);
                        if (cm > 1.0 + 1e-9) residToStore = residToStore / cm;
                    }
                    taskResidT.put(taskName, residToStore);
                }
                taskTput.put(taskName, x);
            }
        }

        // For non-leaf non-REF tasks, override taskResidT with hostLayerResid (per-call
        // processor response time). For intermediate tasks, taskResidT from the T: layer
        // includes the callee response (D_local + R_callee); hostLayerResid = D_local only,
        // preventing double-counting when resolveTaskResponseTime adds callee response
        // recursively. Leaf tasks are excluded: their per-call residual is already stored
        // correctly by the T: layer processing above (dividing by callMean when needed).
        for (Task task : lqnModel.getTasks().values()) {
            if (isRefTask(task)) continue;
            if (isLeafNonRefTask(task, taskCalledTask)) continue;
            String tName = task.getName();
            if (!hostLayerResid.containsKey(tName)) continue;
            taskResidT.put(tName, hostLayerResid.get(tName));
        }

        // For non-REF tasks, the host layer gives the task's own throughput only when the
        // processor has non-zero service demand. For intermediate immediate tasks (D=0 processor),
        // the host layer runs unconstrained and gives the wrong throughput; use the task-layer
        // solve instead (Fix 3).
        Map<String, String> taskNameToProc = new HashMap<String, String>();
        for (Task task : lqnModel.getTasks().values()) {
            if (task.getProcessor() != null) {
                taskNameToProc.put(task.getName(), task.getProcessor().getName());
            }
        }
        for (Map.Entry<String, Double> e : hostLayerTput.entrySet()) {
            String tName = e.getKey();
            // Intermediate tasks with NO think time have a T: layer throughput that reflects
            // the actual call-arrival rate — more reliable than P: layer when the task is
            // saturated. Keep the T: layer value for those. Intermediate tasks WITH think time
            // pace themselves via Z; the P: layer correctly models their utilization and
            // throughput, so allow the host-layer override for them.
            if (taskCalledTask.containsKey(tName)) {
                Task t2 = findTaskByName(lqnModel, tName);
                double think2 = (t2 != null && Double.isFinite(t2.getThinkTimeMean())) ? t2.getThinkTimeMean() : 0.0;
                if (think2 <= 1e-9) continue;
            }
            String procName = taskNameToProc.get(tName);
            Double d = (procName != null) ? processorDemand.get(procName) : null;
            Task t = findTaskByName(lqnModel, tName);
            double think = (t != null && Double.isFinite(t.getThinkTimeMean())) ? t.getThinkTimeMean() : 0.0;
            if ((d != null && d > 0) || think > 1e-9) {
                taskTput.put(tName, e.getValue());
            }
        }

        // For processors hosting a single non-REF task, align utilization with the
        // resolved task throughput used in the final table.
        Map<String, List<Task>> processorToNonRefTasks = new HashMap<String, List<Task>>();
        for (Task task : lqnModel.getTasks().values()) {
            if (task.getScheduling() == SchedStrategy.REF || task.getProcessor() == null) {
                continue;
            }
            String procName = task.getProcessor().getName();
            List<Task> hosted = processorToNonRefTasks.get(procName);
            if (hosted == null) {
                hosted = new ArrayList<Task>();
                processorToNonRefTasks.put(procName, hosted);
            }
            hosted.add(task);
        }
        for (Map.Entry<String, List<Task>> e : processorToNonRefTasks.entrySet()) {
            String procName = e.getKey();
            List<Task> hosted = e.getValue();
            if (hosted.size() != 1) {
                continue;
            }
            Task hostedTask = hosted.get(0);
            Double tx = taskTput.get(hostedTask.getName());
            Double d = processorDemand.get(procName);
            Double c = processorServers.get(procName);
            if (tx != null && tx > 0 && d != null && c != null && c > 0) {
                processorUtil.put(procName, Math.min(tx * d / c, 1.0));
                taskUtil.put(hostedTask.getName(), processorUtil.get(procName));
            }
        }

        // Correct processor utilization using per-caller throughputs × per-caller demands.
        // The P: layer aggregate demand is wrong when callers have asymmetric service demands
        // (e.g., T1 and T2 both call T3, but T3's host demand differs per entry called).
        // Skip when the hosted task has a non-zero think time: its P: layer already correctly
        // models its own utilization (the task paces itself with Z seconds between requests,
        // so U = X_task × D where X_task comes from the host layer, not the caller's X).
        for (Task hostedTask : lqnModel.getTasks().values()) {
            if (hostedTask.getScheduling() == SchedStrategy.REF || hostedTask.getProcessor() == null) continue;
            double hostedThink = hostedTask.getThinkTimeMean();
            if (!Double.isNaN(hostedThink) && Double.isFinite(hostedThink) && hostedThink > 1e-9) continue;
            String procName = hostedTask.getProcessor().getName();
            if (isInfProcessor(lqnModel, procName)) continue;
            Double c = processorServers.get(procName);
            if (c == null || c <= 0) continue;
            double utilSum = 0.0;
            boolean hasCaller = false;
            for (Task caller : lqnModel.getTasks().values()) {
                double d = computeCallerDemandOnTask(lqnModel, caller.getName(), hostedTask.getName());
                if (d <= 0) continue;
                Double xCaller = taskTput.get(caller.getName());
                if (xCaller == null || xCaller <= 0) continue;
                utilSum += xCaller * d / c;
                hasCaller = true;
            }
            if (hasCaller) {
                double utilCapped = Math.min(utilSum, 1.0);
                processorUtil.put(procName, utilCapped);
                taskUtil.put(hostedTask.getName(), utilCapped);
            }
        }

        applyRefTaskInheritance(lqnModel, taskQLen, taskClassQLen, taskResidT, taskTput, taskCalledTask, refTaskCalledTask, refTaskCalledTaskMean, taskSojournCache, refTaskProcQLen);

        for (Task task : lqnModel.getTasks().values()) {
            if (task.getScheduling() != SchedStrategy.REF) {
                Double x = taskTput.get(task.getName());
                if (x != null && x > 0) {
                    double effR;
                    if (isLeafNonRefTask(task, taskCalledTask) && hostLayerResid.containsKey(task.getName())) {
                        effR = hostLayerResid.get(task.getName());
                    } else {
                        effR = resolveTaskResponseTime(lqnModel, task.getName(), taskResidT, taskCalledTask, new HashSet<String>());
                    }
                    if (!Double.isNaN(effR) && effR >= 0) {
                        taskQLen.put(task.getName(), x * effR);
                    }
                }
            }
        }

        // Refresh ref-task metrics after non-ref queue lengths have been derived.
        applyRefTaskInheritance(lqnModel, taskQLen, taskClassQLen, taskResidT, taskTput, taskCalledTask, refTaskCalledTask, refTaskCalledTaskMean, taskSojournCache, refTaskProcQLen);

        // For leaf tasks with multi-call (callMean > 1), taskTput was stored as the caller's
        // throughput (e.g. T3.tput=0.317). SolverLN reports T4.tput per T4 CALL: 0.317×7=2.222.
        // Scale up here, AFTER QLen computation (which used caller_tput × aggregate_sojourn).
        for (Task task : lqnModel.getTasks().values()) {
            if (isRefTask(task)) continue;
            if (!isLeafNonRefTask(task, taskCalledTask)) continue;
            String tName = task.getName();
            double maxCM = getTotalInboundCallMean(lqnModel, tName);
            if (maxCM <= 1.0 + 1e-9) continue;
            Double tx = taskTput.get(tName);
            if (tx != null && tx > 0) {
                taskTput.put(tName, tx * maxCM);
            }
        }

        Map<String, Double> entryRespT = new HashMap<>();

        for (jline.lang.layered.Host proc : lqnModel.getHosts().values()) {
            finalNodeNames.add(proc.getName());
            finalNodeTypes.add("Processor");
            finalQLen.add(Double.NaN);
            finalUtil.add(processorUtil.containsKey(proc.getName()) ? processorUtil.get(proc.getName()) : 0.0);
            finalRespT.add(Double.NaN);
            finalResidT.add(Double.NaN);
            finalArvR.add(Double.NaN);
            finalTput.add(Double.NaN);
        }

        for (Task task : lqnModel.getTasks().values()) {
            double resid = taskResidT.containsKey(task.getName()) ? taskResidT.get(task.getName()) : 0.0;
            if (task.getScheduling() != SchedStrategy.REF
                    && isLeafNonRefTask(task, taskCalledTask)
                    && hostLayerResid.containsKey(task.getName())) {
                double hrv = hostLayerResid.get(task.getName());
                // For leaf tasks with multi-call, hostLayerResid is sojourn per caller-visit
                // (callMean already embedded in P: layer demand). Divide to get per-call residual.
                double maxCM = getTotalInboundCallMean(lqnModel, task.getName());
                if (maxCM > 1.0 + 1e-9) hrv = hrv / maxCM;
                resid = hrv;
            }

            finalNodeNames.add(task.getName());
            finalNodeTypes.add(task.getScheduling() == SchedStrategy.REF ? "RefTask" : "Task");
            finalQLen.add(taskQLen.containsKey(task.getName()) ? taskQLen.get(task.getName()) : 0.0);
            finalUtil.add(taskUtil.containsKey(task.getName()) ? taskUtil.get(task.getName()) : 0.0);
            finalRespT.add(taskRespT.containsKey(task.getName()) ? taskRespT.get(task.getName()) : Double.NaN);
            finalResidT.add(resid);
            finalArvR.add(Double.NaN);
            finalTput.add(taskTput.containsKey(task.getName()) ? taskTput.get(task.getName()) : 0.0);
        }

        // Pre-compute demand-weighted QLen fractions for non-REF task entries.
        // When a task has multiple entries with different service demands, the queue length
        // at the shared server should be split proportionally to (callerTput × demand),
        // not equally by throughput fraction.
        Map<String, Double> entryQlenFraction = new HashMap<String, Double>();
        for (Task tk : lqnModel.getTasks().values()) {
            if (tk.getScheduling() == SchedStrategy.REF) continue;
            List<Entry> taskEntries = tk.getEntries();
            if (taskEntries == null || taskEntries.isEmpty()) continue;
            double denominator = 0.0;
            Map<String, Double> numerators = new HashMap<String, Double>();
            for (Entry e : taskEntries) {
                double cTput = computeEntryCallerTput(e, lqnModel, taskTput);
                double D = getEntryHostDemand(e, lqnModel);
                double num = cTput * D;
                numerators.put(e.getName(), num);
                denominator += num;
            }
            for (Entry e : taskEntries) {
                double num = numerators.containsKey(e.getName()) ? numerators.get(e.getName()) : 0.0;
                double frac = (denominator > 1e-12) ? num / denominator : (1.0 / Math.max(1, taskEntries.size()));
                entryQlenFraction.put(e.getName(), frac);
            }
        }

        for (Entry entry : lqnModel.getEntries().values()) {
            String parentTaskName = (entry.getParent() != null) ? entry.getParent().getName() : null;
            double taskQ = (parentTaskName != null && taskQLen.containsKey(parentTaskName)) ? taskQLen.get(parentTaskName) : 0.0;
            double parentTput = (parentTaskName != null && taskTput.containsKey(parentTaskName)) ? taskTput.get(parentTaskName) : 0.0;
            double callerTput = computeEntryCallerTput(entry, lqnModel, taskTput);
            double entryFraction = (parentTput > 0 && callerTput > 0) ? Math.min(1.0, callerTput / parentTput) : 1.0;
            Task parentTask = findTaskByName(lqnModel, parentTaskName);
            double qFraction = (parentTask != null && parentTask.getScheduling() != SchedStrategy.REF
                    && entryQlenFraction.containsKey(entry.getName()))
                    ? entryQlenFraction.get(entry.getName())
                    : entryFraction;
            double q = qFraction * taskQ;
            double t = entryFraction * parentTput;
            double resp;
            if (t > 0 && Double.isFinite(q) && q >= 0) {
                resp = q / t;
            } else {
                resp = resolveEntryRespTime(lqnModel, parentTaskName, taskResidT, taskCalledTask, refTaskCalledTask, refTaskCalledTaskMean);
            }

            finalNodeNames.add(entry.getName());
            finalNodeTypes.add("Entry");
            finalQLen.add(q);
            finalUtil.add(Double.NaN);
            finalRespT.add(resp);
            finalResidT.add(Double.NaN);
            finalArvR.add(Double.NaN);
            finalTput.add(t);

            entryRespT.put(entry.getName(), resp);
        }

        for (Activity act : lqnModel.getActivities().values()) {
            String parentTaskName = (act.getParent() != null) ? act.getParent().getName() : null;
            Task parentTask = (act.getParent() != null) ? findTaskByName(lqnModel, parentTaskName) : null;
            boolean isRefTaskActivity = act.getParent() != null && act.getParent().getScheduling() == SchedStrategy.REF;

            double actFraction = 1.0;
            {
                String boundEntry = act.getBoundToEntry();
                if (boundEntry != null) {
                    Entry boundE = findEntryByName(lqnModel, boundEntry);
                    if (boundE != null) {
                        double pTput = (parentTaskName != null && taskTput.containsKey(parentTaskName)) ? taskTput.get(parentTaskName) : 0.0;
                        double cTput = computeEntryCallerTput(boundE, lqnModel, taskTput);
                        if (pTput > 0 && cTput > 0) actFraction = Math.min(1.0, cTput / pTput);
                    }
                }
            }
            double q;
            if (isRefTaskActivity && parentTask != null) {
                // Distribute T1.QLen across activities proportionally.
                // Q_P1 = jobs queued at the processor for T1 (from the P: layer MVA).
                // Each activity gets: (visitWeightedDemand / totalVisitWeightedDemand) * Q_P1
                //                   + sum_per_sync_call( callMean/totalCallMeanToCallee * callee.QLen )
                Double q_p1 = refTaskProcQLen.get(parentTaskName);
                if (q_p1 == null || !Double.isFinite(q_p1)) {
                    // No P: layer result — fall back to assigning full task QLen.
                    q = actFraction * (taskQLen.containsKey(parentTaskName) ? taskQLen.get(parentTaskName) : 0.0);
                } else {
                    Map<String, Double> visitCounts = computeActivityVisitCountsStatic(parentTask);
                    double d_total = 0.0;
                    for (Activity a : parentTask.getActivities()) {
                        double vc = visitCounts.containsKey(a.getName()) ? visitCounts.get(a.getName()) : 1.0;
                        double hd = a.getHostDemandMean();
                        if (!Double.isNaN(hd) && hd > 1e-8) d_total += vc * hd;
                    }
                    double vc_act = visitCounts.containsKey(act.getName()) ? visitCounts.get(act.getName()) : 1.0;
                    double hd_act = act.getHostDemandMean();
                    double d_act = (!Double.isNaN(hd_act) && hd_act > 1e-8) ? vc_act * hd_act : 0.0;
                    double q_proc = (d_total > 0) ? (d_act / d_total) * q_p1 : 0.0;
                    double q_callee = 0.0;
                    if (act.getSyncCallDests() != null) {
                        for (Map.Entry<Integer, String> syncCall : act.getSyncCallDests().entrySet()) {
                            Entry calledEntry = findEntryByName(lqnModel, syncCall.getValue());
                            if (calledEntry == null || calledEntry.getParent() == null) continue;
                            String calledTaskName = calledEntry.getParent().getName();
                            if (calledTaskName.equals(parentTaskName)) continue;
                            int idx = syncCall.getKey();
                            double m = 1.0;
                            Matrix means = act.getSyncCallMeans();
                            if (means != null && means.getNumCols() > idx) {
                                double mv = means.get(0, idx);
                                if (Double.isFinite(mv) && mv > 0) m = mv;
                            }
                            Map<String, Double> classQLen = taskClassQLen.get(calledTaskName);
                            Double classSpecificQ = (classQLen != null) ? classQLen.get(parentTaskName) : null;
                            if (classSpecificQ != null && classSpecificQ > 0) {
                                double callerTotalCM = getCallerTotalCallMean(lqnModel, parentTaskName, calledTaskName);
                                q_callee += (m / callerTotalCM) * classSpecificQ;
                            } else {
                                double totalCM = getSumInboundCallMean(lqnModel, calledTaskName);
                                Double calleeQLen = taskQLen.get(calledTaskName);
                                if (calleeQLen != null && calleeQLen > 0 && totalCM > 0) {
                                    q_callee += (m / totalCM) * calleeQLen;
                                }
                            }
                        }
                    }
                    q = actFraction * (q_proc + q_callee);
                }
            } else {
                // For non-REF activities, use the demand-weighted entry fraction for QLen.
                String boundE = act.getBoundToEntry();
                double qFrac = (boundE != null && entryQlenFraction.containsKey(boundE))
                        ? entryQlenFraction.get(boundE)
                        : actFraction;
                q = qFrac * ((parentTaskName != null && taskQLen.containsKey(parentTaskName)) ? taskQLen.get(parentTaskName) : 0.0);
            }
            double util;
            if (isRefTaskActivity && parentTask != null) {
                // Split P1's total utilization across activities proportionally to visit-weighted demand.
                Map<String, Double> vcU = computeActivityVisitCountsStatic(parentTask);
                double dTotalU = 0.0;
                for (Activity a : parentTask.getActivities()) {
                    double v = vcU.containsKey(a.getName()) ? vcU.get(a.getName()) : 1.0;
                    double hd = a.getHostDemandMean();
                    if (!Double.isNaN(hd) && hd > 1e-8) dTotalU += v * hd;
                }
                double hdAct = act.getHostDemandMean();
                double vcAct = vcU.containsKey(act.getName()) ? vcU.get(act.getName()) : 1.0;
                double dActU = (!Double.isNaN(hdAct) && hdAct > 1e-8) ? vcAct * hdAct : 0.0;
                double taskU = (parentTaskName != null && taskUtil.containsKey(parentTaskName)) ? taskUtil.get(parentTaskName) : 0.0;
                util = (dTotalU > 0 && dActU > 0) ? actFraction * (dActU / dTotalU) * taskU : 0.0;
            } else {
                // For non-REF activities, split Util by demand-weighted entry fraction.
                String boundEU = act.getBoundToEntry();
                double utilFrac = (boundEU != null && entryQlenFraction.containsKey(boundEU))
                        ? entryQlenFraction.get(boundEU)
                        : actFraction;
                util = utilFrac * ((parentTaskName != null && taskUtil.containsKey(parentTaskName)) ? taskUtil.get(parentTaskName) : 0.0);
                if (act.getHostDemandMean() <= 1e-8) {
                    util = 0.0;
                }
            }

            // Tput: loop body activities fire visitCount times per T1 invocation.
            double actVisitCount = 1.0;
            if (isRefTaskActivity && parentTask != null) {
                Map<String, Double> vcT = computeActivityVisitCountsStatic(parentTask);
                actVisitCount = vcT.containsKey(act.getName()) ? vcT.get(act.getName()) : 1.0;
            }
            double t = actFraction * actVisitCount * ((parentTaskName != null && taskTput.containsKey(parentTaskName)) ? taskTput.get(parentTaskName) : 0.0);

            // RespT = QLen / Tput by Little's law.
            double resp;
            if (t > 0 && Double.isFinite(q) && q >= 0) {
                resp = q / t;
            } else if (parentTask != null
                    && parentTask.getScheduling() != SchedStrategy.REF
                    && !taskHasExternalSyncCall(lqnModel, parentTaskName)) {
                resp = deriveLocalTaskResid(parentTaskName, taskQLen, taskTput, taskResidT);
            } else {
                resp = (parentTaskName != null && taskResidT.containsKey(parentTaskName)) ? taskResidT.get(parentTaskName) : Double.NaN;
            }

            // ResidT: processor-only component, split proportionally for REF task activities.
            double resid;
            if (isRefTaskActivity && parentTask != null) {
                double t1ResidT = (parentTaskName != null && taskResidT.containsKey(parentTaskName)) ? taskResidT.get(parentTaskName) : 0.0;
                Map<String, Double> vcR = computeActivityVisitCountsStatic(parentTask);
                double dTotalR = 0.0;
                for (Activity a : parentTask.getActivities()) {
                    double v = vcR.containsKey(a.getName()) ? vcR.get(a.getName()) : 1.0;
                    double hd = a.getHostDemandMean();
                    if (!Double.isNaN(hd) && hd > 1e-8) dTotalR += v * hd;
                }
                double hdActR = act.getHostDemandMean();
                double vcActR = vcR.containsKey(act.getName()) ? vcR.get(act.getName()) : 1.0;
                double dActR = (!Double.isNaN(hdActR) && hdActR > 1e-8) ? vcActR * hdActR : 0.0;
                resid = (dTotalR > 0 && dActR > 0) ? actFraction * (dActR / dTotalR) * t1ResidT : 0.0;
            } else if (parentTask != null
                    && parentTask.getScheduling() != SchedStrategy.REF
                    && !taskHasExternalSyncCall(lqnModel, parentTaskName)) {
                String boundER = act.getBoundToEntry();
                double residFrac = (boundER != null && entryQlenFraction.containsKey(boundER))
                        ? entryQlenFraction.get(boundER)
                        : actFraction;
                resid = residFrac * deriveLocalTaskResid(parentTaskName, taskQLen, taskTput, taskResidT);
            } else {
                String boundER = act.getBoundToEntry();
                double residFrac = (boundER != null && entryQlenFraction.containsKey(boundER))
                        ? entryQlenFraction.get(boundER)
                        : actFraction;
                resid = residFrac * ((parentTaskName != null && taskResidT.containsKey(parentTaskName)) ? taskResidT.get(parentTaskName) : 0.0);
            }

            finalNodeNames.add(act.getName());
            finalNodeTypes.add("Activity");
            finalQLen.add(q);
            finalUtil.add(util);
            finalRespT.add(resp);
            finalResidT.add(resid);
            finalArvR.add(Double.NaN);
            finalTput.add(t);
        }

        LayeredNetworkAvgTable table = new LayeredNetworkAvgTable(finalQLen, finalUtil, finalRespT, finalResidT, finalArvR, finalTput);
        table.setNodeNames(finalNodeNames);
        table.setNodeTypes(finalNodeTypes);
        SolverOptions opts = new SolverOptions();
        opts.verbose = VerboseLevel.STD;
        table.print(opts, true);
        return table;
    }

    private static double computeEntryCallerTput(Entry entry, LayeredNetwork lqnModel, Map<String, Double> taskTput) {
        double total = 0.0;
        for (Task task : lqnModel.getTasks().values()) {
            for (Activity act : task.getActivities()) {
                if (act.getSyncCallDests() == null) continue;
                for (Map.Entry<Integer, String> syncCall : act.getSyncCallDests().entrySet()) {
                    if (!syncCall.getValue().equals(entry.getName())) continue;
                    Double x = taskTput.get(task.getName());
                    if (x == null || x <= 0) continue;
                    double callMean = 1.0;
                    int idx = syncCall.getKey();
                    Matrix means = act.getSyncCallMeans();
                    if (means != null && means.getNumCols() > idx) {
                        double m = means.get(0, idx);
                        if (Double.isFinite(m) && m > 0) callMean = m;
                    }
                    total += x * callMean;
                }
            }
        }
        return total;
    }

    private static double computeCallerDemandOnTask(LayeredNetwork lqnModel, String callerTaskName, String targetTaskName) {
        Task callerTask = findTaskByName(lqnModel, callerTaskName);
        if (callerTask == null) return 0.0;
        double totalDemand = 0.0;
        for (Activity act : callerTask.getActivities()) {
            Map<Integer, String> callDests = act.getSyncCallDests();
            Matrix callMeans = act.getSyncCallMeans();
            if (callDests == null || callMeans == null) continue;
            for (int ci = 0; ci < callDests.size(); ci++) {
                String destEntryName = callDests.get(ci);
                Entry destEntry = null;
                outer:
                for (Task t : lqnModel.getTasks().values()) {
                    for (Entry e : t.getEntries()) {
                        if (e.getName().equals(destEntryName)) { destEntry = e; break outer; }
                    }
                }
                if (destEntry == null || destEntry.getParent() == null) continue;
                if (!targetTaskName.equals(destEntry.getParent().getName())) continue;
                for (Activity serverAct : destEntry.getParent().getActivities()) {
                    if (destEntryName.equals(serverAct.getBoundToEntry())) {
                        double callMean = callMeans.get(0, ci);
                        totalDemand += callMean * serverAct.getHostDemandMean();
                        break;
                    }
                }
            }
        }
        return totalDemand;
    }

    private static double computeTaskHostDemand(Task task) {
        if (task == null) return 0.0;
        double total = 0.0;
        for (Activity act : task.getActivities()) {
            double m = act.getHostDemandMean();
            if (!Double.isNaN(m) && Double.isFinite(m)) {
                total += m;
            }
        }
        return total;
    }

    private static String stripPrefix(String name) {
        int idx = name.indexOf(':');
        if (idx >= 0 && idx + 1 < name.length()) {
            return name.substring(idx + 1);
        }
        return name;
    }

    public static Map<String, String> buildRefTaskCalledTaskMap(LayeredNetwork lqnModel) {
        Map<String, String> refTaskCalledTask = new HashMap<String, String>();

        for (Task task : lqnModel.getTasks().values()) {
            if (task.getScheduling() != SchedStrategy.REF) {
                continue;
            }

            for (Activity activity : task.getActivities()) {
                if (activity.getSyncCallDests() == null) {
                    continue;
                }

                for (String callDest : activity.getSyncCallDests().values()) {
                    Entry calledEntry = findEntryByName(lqnModel, callDest);
                    if (calledEntry != null && calledEntry.getParent() != null) {
                        String calledTaskName = calledEntry.getParent().getName();
                        if (!task.getName().equals(calledTaskName)) {
                            refTaskCalledTask.put(task.getName(), calledTaskName);
                        }
                    }
                }
            }
        }

        return refTaskCalledTask;
    }

    private static Map<String, Double> buildRefTaskCalledTaskMeanMap(
            LayeredNetwork lqnModel,
            Map<String, String> refTaskCalledTask) {
        Map<String, Double> refTaskCalledTaskMean = new HashMap<String, Double>();
        for (Task task : lqnModel.getTasks().values()) {
            if (task.getScheduling() != SchedStrategy.REF) {
                continue;
            }
            String calledTaskName = refTaskCalledTask.get(task.getName());
            if (calledTaskName == null) {
                continue;
            }
            refTaskCalledTaskMean.put(task.getName(), getCallMeanToTask(lqnModel, task.getName(), calledTaskName));
        }
        return refTaskCalledTaskMean;
    }

    private static double getCallMeanToTask(LayeredNetwork lqnModel, String callerTaskName, String calleeTaskName) {
        if (callerTaskName == null || calleeTaskName == null) {
            return 1.0;
        }
        Task callerTask = findTaskByName(lqnModel, callerTaskName);
        if (callerTask == null) {
            return 1.0;
        }
        double totalCallMean = 0.0;
        for (Activity activity : callerTask.getActivities()) {
            if (activity.getSyncCallDests() == null) {
                continue;
            }
            for (Map.Entry<Integer, String> syncCall : activity.getSyncCallDests().entrySet()) {
                Entry calledEntry = findEntryByName(lqnModel, syncCall.getValue());
                if (calledEntry == null || calledEntry.getParent() == null) {
                    continue;
                }
                if (!calleeTaskName.equals(calledEntry.getParent().getName())) {
                    continue;
                }
                int idx = syncCall.getKey();
                double m = 1.0;
                Matrix means = activity.getSyncCallMeans();
                if (means != null && means.getNumCols() > idx) {
                    double mean = means.get(0, idx);
                    if (Double.isFinite(mean) && mean > 0) {
                        m = mean;
                    }
                }
                totalCallMean += m;
            }
        }
        return totalCallMean > 0 ? totalCallMean : 1.0;
    }

    private static double getTotalInboundCallMean(LayeredNetwork lqnModel, String targetTaskName) {
        // Sum call means per caller task, return the max across callers.
        // Correct for Tput scaling: one caller making 2 calls → max=2; two callers each 1 → max=1.
        double maxPerCaller = 0.0;
        for (Task callerTask : lqnModel.getTasks().values()) {
            double callerTotal = 0.0;
            for (Activity activity : callerTask.getActivities()) {
                if (activity.getSyncCallDests() == null) continue;
                for (Map.Entry<Integer, String> syncCall : activity.getSyncCallDests().entrySet()) {
                    Entry calledEntry = findEntryByName(lqnModel, syncCall.getValue());
                    if (calledEntry == null || calledEntry.getParent() == null) continue;
                    if (!targetTaskName.equals(calledEntry.getParent().getName())) continue;
                    int idx = syncCall.getKey();
                    double m = 1.0;
                    Matrix means = activity.getSyncCallMeans();
                    if (means != null && means.getNumCols() > idx) {
                        double mean = means.get(0, idx);
                        if (Double.isFinite(mean) && mean > 0) m = mean;
                    }
                    callerTotal += m;
                }
            }
            if (callerTotal > maxPerCaller) maxPerCaller = callerTotal;
        }
        return maxPerCaller;
    }

    private static double getCallerTotalCallMean(LayeredNetwork lqnModel, String callerTaskName, String targetTaskName) {
        // Sum call means from callerTaskName to all entries on targetTaskName, across all activities.
        // Used as denominator so each activity's contribution is proportional to its share of the caller's total.
        double total = 0.0;
        for (Task callerTask : lqnModel.getTasks().values()) {
            if (!callerTask.getName().equals(callerTaskName)) continue;
            for (Activity activity : callerTask.getActivities()) {
                if (activity.getSyncCallDests() == null) continue;
                for (Map.Entry<Integer, String> syncCall : activity.getSyncCallDests().entrySet()) {
                    Entry calledEntry = findEntryByName(lqnModel, syncCall.getValue());
                    if (calledEntry == null || calledEntry.getParent() == null) continue;
                    if (!targetTaskName.equals(calledEntry.getParent().getName())) continue;
                    int idx = syncCall.getKey();
                    double m = 1.0;
                    Matrix means = activity.getSyncCallMeans();
                    if (means != null && means.getNumCols() > idx) {
                        double mean = means.get(0, idx);
                        if (Double.isFinite(mean) && mean > 0) m = mean;
                    }
                    total += m;
                }
            }
            break;
        }
        return Math.max(1.0, total);
    }

    private static double getSumInboundCallMean(LayeredNetwork lqnModel, String targetTaskName) {
        // Sum call means across ALL callers. Used as denominator when apportioning a callee's
        // QLen across callers: each caller's fraction = (its call mean) / (total load on callee).
        double total = 0.0;
        for (Task callerTask : lqnModel.getTasks().values()) {
            for (Activity activity : callerTask.getActivities()) {
                if (activity.getSyncCallDests() == null) continue;
                for (Map.Entry<Integer, String> syncCall : activity.getSyncCallDests().entrySet()) {
                    Entry calledEntry = findEntryByName(lqnModel, syncCall.getValue());
                    if (calledEntry == null || calledEntry.getParent() == null) continue;
                    if (!targetTaskName.equals(calledEntry.getParent().getName())) continue;
                    int idx = syncCall.getKey();
                    double m = 1.0;
                    Matrix means = activity.getSyncCallMeans();
                    if (means != null && means.getNumCols() > idx) {
                        double mean = means.get(0, idx);
                        if (Double.isFinite(mean) && mean > 0) m = mean;
                    }
                    total += m;
                }
            }
        }
        return total;
    }

    public static void applyRefTaskInheritance(
            LayeredNetwork lqnModel,
            Map<String, Double> taskQLen,
            Map<String, Map<String, Double>> taskClassQLen,
            Map<String, Double> taskResidT,
            Map<String, Double> taskTput,
            Map<String, String> taskCalledTask,
            Map<String, String> refTaskCalledTask,
            Map<String, Double> refTaskCalledTaskMean,
            Map<String, Double> taskSojournCache,
            Map<String, Double> refTaskProcQLen) {

        for (Task task : lqnModel.getTasks().values()) {
            if (task.getScheduling() != SchedStrategy.REF) {
                continue;
            }

            String calledTaskName = refTaskCalledTask.get(task.getName());
            if (calledTaskName == null) {
                // REF task has no sync callees. T1.QLen = T1.Tput * E1.RespT where E1.RespT
                // counts only the bound activity's executions (SolverLN convention for loops:
                // the entry response = nLoops * R_boundAct_per_exec, excluding post-loop exit).
                Double q_p1 = (refTaskProcQLen != null) ? refTaskProcQLen.get(task.getName()) : null;
                if (q_p1 != null && Double.isFinite(q_p1) && q_p1 > 0) {
                    Map<String, Double> vc = computeActivityVisitCountsStatic(task);
                    double dTotal = 0.0;
                    for (Activity a : task.getActivities()) {
                        double v = vc.containsKey(a.getName()) ? vc.get(a.getName()) : 1.0;
                        double hd = a.getHostDemandMean();
                        if (!Double.isNaN(hd) && hd > 1e-8) dTotal += v * hd;
                    }
                    double dBound = 0.0;
                    for (Activity a : task.getActivities()) {
                        if (a.getBoundToEntry() != null) {
                            double v = vc.containsKey(a.getName()) ? vc.get(a.getName()) : 1.0;
                            double hd = a.getHostDemandMean();
                            if (!Double.isNaN(hd) && hd > 1e-8) dBound = v * hd;
                            break;
                        }
                    }
                    double fraction = (dTotal > 0 && dBound > 0) ? dBound / dTotal : 1.0;
                    taskQLen.put(task.getName(), fraction * q_p1);
                }
                continue;
            }

            // T1.tput is already set from its processor host layer; only fall back to callee's
            // tput if T1 has no host layer (e.g. no processor assigned).
            if (!taskTput.containsKey(task.getName())) {
                Double calledTput = taskTput.get(calledTaskName);
                if (calledTput != null) {
                    taskTput.put(task.getName(), calledTput);
                }
            }

            // T1.QLen via Little's law for the think station: QLen = N - X * Z.
            // By conservation: T1 jobs either thinking (X*Z) or working (QLen).
            // This is exact regardless of callee structure or callMean.
            Double t1Tput = taskTput.get(task.getName());
            if (t1Tput != null && t1Tput > 0) {
                double zT1 = task.getThinkTimeMean();
                if (Double.isNaN(zT1)) zT1 = 0.0;
                double nT1 = (double) task.getMultiplicity();
                taskQLen.put(task.getName(), Math.max(0.0, nT1 - t1Tput * zT1));
            }

            // Only zero out ResidT if not already set (e.g., from a non-Immediate processor layer).
            if (!taskResidT.containsKey(task.getName())) {
                taskResidT.put(task.getName(), 0.0);
            }
        }
    }

    public static double resolveEntryRespTime(
            LayeredNetwork lqnModel,
            String parentTaskName,
            Map<String, Double> taskResidT,
            Map<String, String> taskCalledTask,
            Map<String, String> refTaskCalledTask,
            Map<String, Double> refTaskCalledTaskMean) {

        if (parentTaskName == null) {
            return Double.NaN;
        }

        return resolveTaskResponseTime(lqnModel, parentTaskName, taskResidT, taskCalledTask, new HashSet<String>());
    }

    private static double resolveTaskResponseTime(
            LayeredNetwork lqnModel,
            String taskName,
            Map<String, Double> taskResidT,
            Map<String, String> taskCalledTask,
            Set<String> visited) {

        if (taskName == null || visited.contains(taskName)) {
            return Double.NaN;
        }
        visited.add(taskName);

        Double localResid = taskResidT.get(taskName);
        double response = (localResid != null && Double.isFinite(localResid) && localResid > 0) ? localResid : 0.0;

        String calledTaskName = taskCalledTask.get(taskName);
        if (calledTaskName == null || calledTaskName.equals(taskName)) {
            return response;
        }

        double callMean = getCallMeanToTask(lqnModel, taskName, calledTaskName);
        if (!Double.isFinite(callMean) || callMean <= 0) {
            callMean = 1.0;
        }

        double calleeResponse = resolveTaskResponseTime(lqnModel, calledTaskName, taskResidT, taskCalledTask, visited);
        if (Double.isNaN(calleeResponse) || calleeResponse <= 0) {
            return response;
        }

        return response + callMean * calleeResponse;
    }

    private static double resolveEffectiveResid(
            String taskName,
            Map<String, Double> taskResidT,
            Map<String, String> taskCalledTask,
            Set<String> visited) {

        if (taskName == null || visited.contains(taskName)) {
            return Double.NaN;
        }
        visited.add(taskName);

        Double r = taskResidT.get(taskName);
        if (r != null && !Double.isNaN(r) && r > 0) {
            return r;
        }

        String called = taskCalledTask.get(taskName);
        if (called == null) {
            return (r == null) ? Double.NaN : r;
        }

        return resolveEffectiveResid(called, taskResidT, taskCalledTask, visited);
    }

    private static Map<String, String> buildTaskCalledTaskMap(LayeredNetwork lqnModel) {
        Map<String, String> taskCalledTask = new HashMap<String, String>();

        for (Task task : lqnModel.getTasks().values()) {
            for (Activity activity : task.getActivities()) {
                if (activity.getSyncCallDests() == null) {
                    continue;
                }
                for (String callDest : activity.getSyncCallDests().values()) {
                    Entry calledEntry = findEntryByName(lqnModel, callDest);
                    if (calledEntry != null && calledEntry.getParent() != null) {
                        String calledTaskName = calledEntry.getParent().getName();
                        if (!task.getName().equals(calledTaskName)) {
                            taskCalledTask.put(task.getName(), calledTaskName);
                        }
                    }
                }
            }
        }

        return taskCalledTask;
    }

    public static Task findTaskByName(LayeredNetwork lqnModel, String taskName) {
        for (Task task : lqnModel.getTasks().values()) {
            if (task.getName().equals(taskName)) {
                return task;
            }
        }

        return null;
    }

    public static boolean isRefTask(Task task) {
        return task != null && task.getScheduling() == SchedStrategy.REF;
    }

    private static boolean isLeafNonRefTask(Task task, Map<String, String> taskCalledTask) {
        return task != null
                && task.getScheduling() != SchedStrategy.REF
                && !taskCalledTask.containsKey(task.getName());
    }

    private static boolean hasExternalSyncCall(LayeredNetwork lqnModel, Activity activity, String parentTaskName) {
        if (activity == null || activity.getSyncCallDests() == null || activity.getSyncCallDests().isEmpty()) {
            return false;
        }
        for (String dest : activity.getSyncCallDests().values()) {
            Entry calledEntry = findEntryByName(lqnModel, dest);
            if (calledEntry == null || calledEntry.getParent() == null) {
                continue;
            }
            String calledTaskName = calledEntry.getParent().getName();
            if (parentTaskName == null || !parentTaskName.equals(calledTaskName)) {
                return true;
            }
        }
        return false;
    }

    private static boolean taskHasExternalSyncCall(LayeredNetwork lqnModel, String taskName) {
        Task task = findTaskByName(lqnModel, taskName);
        if (task == null) {
            return false;
        }
        for (Activity activity : task.getActivities()) {
            if (hasExternalSyncCall(lqnModel, activity, taskName)) {
                return true;
            }
        }
        return false;
    }

    private static double deriveLocalTaskResid(
            String taskName,
            Map<String, Double> taskQLen,
            Map<String, Double> taskTput,
            Map<String, Double> taskResidT) {
        Double q = taskQLen.get(taskName);
        Double x = taskTput.get(taskName);
        if (q != null && x != null && x > 0 && Double.isFinite(q) && Double.isFinite(x)) {
            return q / x;
        }
        Double r = taskResidT.get(taskName);
        return (r != null) ? r : Double.NaN;
    }

    private static Entry findEntryByName(LayeredNetwork lqnModel, String entryName) {
        for (Entry entry : lqnModel.getEntries().values()) {
            if (entry.getName().equals(entryName)) {
                return entry;
            }
        }

        return null;
    }

    private static boolean isInfProcessor(LayeredNetwork lqnModel, String procName) {
        for (jline.lang.layered.Host host : lqnModel.getHosts().values()) {
            if (host.getName().equals(procName)) {
                return host.getScheduling() == SchedStrategy.INF;
            }
        }
        return false;
    }

    private static double getEntryHostDemand(Entry entry, LayeredNetwork lqnModel) {
        if (entry.getParent() == null) return 0.0;
        for (Activity act : entry.getParent().getActivities()) {
            if (entry.getName().equals(act.getBoundToEntry())) {
                double hd = act.getHostDemandMean();
                return (Double.isNaN(hd) || hd < 0) ? 0.0 : hd;
            }
        }
        return 0.0;
    }

    // Returns visit count per task-entry visit for each activity, accounting for
    // POST_LOOP precedences (loop body fires loopCount times, loop exit fires once).
    static Map<String, Double> computeActivityVisitCountsStatic(Task task) {
        Map<String, Double> counts = new HashMap<String, Double>();
        for (Activity act : task.getActivities()) {
            counts.put(act.getName(), 1.0);
        }
        for (ActivityPrecedence prec : task.getPrecedences()) {
            if (ActivityPrecedenceType.POST_LOOP.equals(prec.getPostType())) {
                Matrix postParams = prec.getPostParams();
                double loopCount = (postParams != null
                        && postParams.getNumRows() > 0
                        && postParams.getNumCols() > 0)
                        ? postParams.get(0, 0) : 1.0;
                for (String actName : prec.getPreActs()) {
                    counts.put(actName, loopCount);
                }
            }
        }
        return counts;
    }
}