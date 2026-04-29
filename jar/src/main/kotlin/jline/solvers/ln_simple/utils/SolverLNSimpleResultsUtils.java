package jline.solvers.ln_simple.utils;

import jline.VerboseLevel;
import jline.io.Ret;
import jline.lang.Network;
import jline.lang.constant.SchedStrategy;
import jline.lang.layered.Activity;
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

    public static void collectAndPrintFinalResults(LayeredNetwork lqnModel, List<Network> ensemble, int nLayers) {
        List<String> finalNodeNames = new ArrayList<>();
        List<String> finalNodeTypes = new ArrayList<>();
        List<Double> finalQLen = new ArrayList<>();
        List<Double> finalUtil = new ArrayList<>();
        List<Double> finalRespT = new ArrayList<>();
        List<Double> finalResidT = new ArrayList<>();
        List<Double> finalArvR = new ArrayList<>();
        List<Double> finalTput = new ArrayList<>();

        Map<String, Double> processorUtil = new HashMap<>();

        Map<String, Double> taskQLen = new HashMap<>();
        Map<String, Double> taskUtil = new HashMap<>();
        Map<String, Double> taskRespT = new HashMap<>();
        Map<String, Double> taskResidT = new HashMap<>();
        Map<String, Double> taskTput = new HashMap<>();

        Map<String, String> refTaskCalledTask = buildRefTaskCalledTaskMap(lqnModel);
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
                // Compute per-server utilization: X * D / c (pfqn_mvald returns 1-P(idle) which differs for c>1).
                double d = demand.get(serverNodeIndex, 0);
                double c = servers.get(serverNodeIndex, 0);
                double u_per_server = (c > 0 && x > 0) ? Math.min(x * d / c, 1.0) : u;
                processorUtil.put(taskName, u_per_server);

                for (Task task : lqnModel.getTasks().values()) {
                    if (task.getName().equals(mainClassTaskName) && task.getScheduling() == SchedStrategy.REF) {
                        taskQLen.put(task.getName(), q);
                        taskUtil.put(task.getName(), 0.0);
                        taskRespT.put(task.getName(), Double.NaN);
                        taskResidT.put(task.getName(), 0.0);
                        taskTput.put(task.getName(), x);
                        break;
                    }
                }
            } else if (serverQueueName.startsWith("T:")) {
                Task currentTask = findTaskByName(lqnModel, taskName);
                if (!isRefTask(currentTask)) {
                    taskQLen.put(taskName, q);
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
                    taskResidT.put(taskName, r);
                }
                taskTput.put(taskName, x);
            }
        }

        applyRefTaskInheritance(lqnModel, taskQLen, taskResidT, taskTput, refTaskCalledTask);

        for (Task task : lqnModel.getTasks().values()) {
            if (task.getScheduling() != SchedStrategy.REF) {
                Double x = taskTput.get(task.getName());
                if (x != null && x > 0) {
                    double effR = resolveEffectiveResid(task.getName(), taskResidT, taskCalledTask, new HashSet<String>());
                    if (!Double.isNaN(effR) && effR >= 0) {
                        taskQLen.put(task.getName(), x * effR);
                    }
                }
            }
        }

        // Refresh ref-task metrics after non-ref queue lengths have been derived.
        applyRefTaskInheritance(lqnModel, taskQLen, taskResidT, taskTput, refTaskCalledTask);

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
            finalNodeNames.add(task.getName());
            finalNodeTypes.add(task.getScheduling() == SchedStrategy.REF ? "RefTask" : "Task");
            finalQLen.add(taskQLen.containsKey(task.getName()) ? taskQLen.get(task.getName()) : 0.0);
            finalUtil.add(taskUtil.containsKey(task.getName()) ? taskUtil.get(task.getName()) : 0.0);
            finalRespT.add(taskRespT.containsKey(task.getName()) ? taskRespT.get(task.getName()) : Double.NaN);
            finalResidT.add(taskResidT.containsKey(task.getName()) ? taskResidT.get(task.getName()) : 0.0);
            finalArvR.add(Double.NaN);
            finalTput.add(taskTput.containsKey(task.getName()) ? taskTput.get(task.getName()) : 0.0);
        }

        for (Entry entry : lqnModel.getEntries().values()) {
            String parentTaskName = (entry.getParent() != null) ? entry.getParent().getName() : null;
            double q = (parentTaskName != null && taskQLen.containsKey(parentTaskName)) ? taskQLen.get(parentTaskName) : 0.0;
            double resp = resolveEntryRespTime(lqnModel, parentTaskName, taskResidT, taskCalledTask, refTaskCalledTask);
            double t = (parentTaskName != null && taskTput.containsKey(parentTaskName)) ? taskTput.get(parentTaskName) : 0.0;

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
            boolean isRefTaskActivity = act.getParent() != null && act.getParent().getScheduling() == SchedStrategy.REF;
            boolean hasSyncCall = act.getSyncCallDests() != null && !act.getSyncCallDests().isEmpty();

            double q = (parentTaskName != null && taskQLen.containsKey(parentTaskName)) ? taskQLen.get(parentTaskName) : 0.0;
            double util = (parentTaskName != null && taskUtil.containsKey(parentTaskName)) ? taskUtil.get(parentTaskName) : 0.0;
            if (isRefTaskActivity || act.getHostDemandMean() <= 1e-8) {
                util = 0.0;
            }

            double resp;
            if (hasSyncCall) {
                double sumResp = 0.0;
                int count = 0;
                for (String dest : act.getSyncCallDests().values()) {
                    if (entryRespT.containsKey(dest) && !Double.isNaN(entryRespT.get(dest))) {
                        sumResp += entryRespT.get(dest);
                        count++;
                    }
                }
                resp = (count > 0) ? (sumResp / count) : Double.NaN;
            } else {
                resp = (parentTaskName != null && taskResidT.containsKey(parentTaskName)) ? taskResidT.get(parentTaskName) : Double.NaN;
            }

            double resid = (parentTaskName != null && taskResidT.containsKey(parentTaskName)) ? taskResidT.get(parentTaskName) : 0.0;
            if (isRefTaskActivity) {
                resid = 0.0;
            }
            double t = (parentTaskName != null && taskTput.containsKey(parentTaskName)) ? taskTput.get(parentTaskName) : 0.0;

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
                        refTaskCalledTask.put(task.getName(), calledEntry.getParent().getName());
                    }
                }
            }
        }

        return refTaskCalledTask;
    }

    public static void applyRefTaskInheritance(
            LayeredNetwork lqnModel,
            Map<String, Double> taskQLen,
            Map<String, Double> taskResidT,
            Map<String, Double> taskTput,
            Map<String, String> refTaskCalledTask) {

        for (Task task : lqnModel.getTasks().values()) {
            if (task.getScheduling() != SchedStrategy.REF) {
                continue;
            }

            String calledTaskName = refTaskCalledTask.get(task.getName());
            if (calledTaskName == null) {
                continue;
            }

            Double calledTput = taskTput.get(calledTaskName);
            if (calledTput != null) {
                taskTput.put(task.getName(), calledTput);
            }

            Double calledQLen = taskQLen.get(calledTaskName);
            if (calledQLen != null) {
                taskQLen.put(task.getName(), calledQLen);
            }

            taskResidT.put(task.getName(), 0.0);
        }
    }

        public static double resolveEntryRespTime(
            LayeredNetwork lqnModel,
            String parentTaskName,
            Map<String, Double> taskResidT,
            Map<String, String> taskCalledTask,
            Map<String, String> refTaskCalledTask) {

        if (parentTaskName == null) {
            return Double.NaN;
        }

        Double resp = taskResidT.get(parentTaskName);
        if (resp == null) {
            resp = Double.NaN;
        }

        if (!Double.isNaN(resp) && resp > 0) {
            return resp;
        }

        double inherited = resolveEffectiveResid(parentTaskName, taskResidT, taskCalledTask, new HashSet<String>());
        if (!Double.isNaN(inherited) && inherited > 0) {
            return inherited;
        }

        Task parentTask = findTaskByName(lqnModel, parentTaskName);
        if (isRefTask(parentTask)) {
            String calledTaskName = refTaskCalledTask.get(parentTaskName);
            if (calledTaskName != null && taskResidT.containsKey(calledTaskName)) {
                return taskResidT.get(calledTaskName);
            }
        }

        return resp;
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
                        taskCalledTask.put(task.getName(), calledEntry.getParent().getName());
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

    private static Entry findEntryByName(LayeredNetwork lqnModel, String entryName) {
        for (Entry entry : lqnModel.getEntries().values()) {
            if (entry.getName().equals(entryName)) {
                return entry;
            }
        }

        return null;
    }
}