package jline.solvers.ln_simple;

import jline.lang.constant.ActivityPrecedenceType;
import jline.lang.constant.SchedStrategy;
import jline.lang.layered.Activity;
import jline.lang.layered.ActivityPrecedence;
import jline.lang.layered.Entry;
import jline.lang.layered.Host;
import jline.lang.layered.LayeredNetwork;
import jline.lang.layered.Task;
import jline.util.matrix.Matrix;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

/**
 * Pure read-only queries on a {@link LayeredNetwork} model.
 *
 * <p>Every method here is stateless and idempotent — it computes a value or
 * traverses the LQN graph without mutating anything. Shared between
 * {@link SolverLNSimple} (during iteration and coupling) and
 * {@link jline.solvers.ln_simple.results.ResultsCollector} (during final
 * table assembly).
 *
 * <p>Method families:
 * <ul>
 *   <li><b>Lookups</b> — find by name: {@link #findTask}, {@link #findEntry},
 *       {@link #findCallerTask}.</li>
 *   <li><b>Predicates</b> — boolean tests: {@link #isRefTask},
 *       {@link #isInfScheduledTask}, {@link #taskHasSyncCallees}, etc.</li>
 *   <li><b>Quantities</b> — numerical traversals: demands, call means,
 *       think times, chain sums.</li>
 *   <li><b>Caller maps</b> — graph-wide maps used by results assembly:
 *       {@link #buildTaskCalledTaskMap}, {@link #buildRefTaskCalledTaskMap}.</li>
 *   <li><b>String</b> — {@link #stripPrefix} for layer/class names.</li>
 * </ul>
 */
public final class LqnGraph {

    private LqnGraph() {}

    // =================================================================================
    //  Lookups
    // =================================================================================

    /** First task with the given name, or {@code null}. */
    public static Task findTask(LayeredNetwork model, String name) {
        if (name == null) return null;
        for (Task t : model.getTasks().values()) {
            if (name.equals(t.getName())) return t;
        }
        return null;
    }

    /** First entry with the given name, or {@code null}. */
    public static Entry findEntry(LayeredNetwork model, String name) {
        if (name == null) return null;
        for (Entry e : model.getEntries().values()) {
            if (name.equals(e.getName())) return e;
        }
        return null;
    }

    /** Name of <i>any</i> task whose activities issue a sync call into a task
     *  named {@code calleeName}. Returns {@code null} if no caller exists. */
    public static String findCallerTask(LayeredNetwork model, String calleeName) {
        if (calleeName == null) return null;
        for (Task task : model.getTasks().values()) {
            for (Activity act : task.getActivities()) {
                for (String dest : act.getSyncCallDests().values()) {
                    Entry e = findEntry(model, dest);
                    if (e != null && e.getParent() != null
                            && calleeName.equals(e.getParent().getName())) {
                        return task.getName();
                    }
                }
            }
        }
        return null;
    }


    // =================================================================================
    //  Predicates
    // =================================================================================

    public static boolean isRefTask(Task task) {
        return task != null && task.getScheduling() == SchedStrategy.REF;
    }

    public static boolean isInfScheduledTask(LayeredNetwork model, String taskName) {
        Task t = findTask(model, taskName);
        return t != null && t.getScheduling() == SchedStrategy.INF;
    }

    /** True iff the named host is INF-scheduled (true delay server, no queueing). */
    public static boolean isInfProcessor(LayeredNetwork model, String procName) {
        for (Host h : model.getHosts().values()) {
            if (h.getName().equals(procName)) {
                return h.getScheduling() == SchedStrategy.INF;
            }
        }
        return false;
    }

    /** True iff {@code task} has at least one synchronous-call activity. */
    public static boolean taskHasSyncCallees(LayeredNetwork model, String taskName) {
        Task t = findTask(model, taskName);
        if (t == null) return false;
        for (Activity act : t.getActivities()) {
            Map<Integer, String> dests = act.getSyncCallDests();
            if (dests != null && !dests.isEmpty()) return true;
        }
        return false;
    }

    /** True iff {@code task} synchronously calls two or more <i>distinct</i> callee
     *  tasks (i.e. fans out). Single-callee callers do not need sibling-callee
     *  accounting or Z-update damping. */
    public static boolean callerFansOut(LayeredNetwork model, String taskName) {
        Task t = findTask(model, taskName);
        if (t == null) return false;
        Set<String> distinctCallees = new HashSet<String>();
        for (Activity act : t.getActivities()) {
            Map<Integer, String> dests = act.getSyncCallDests();
            if (dests == null) continue;
            for (String dest : dests.values()) {
                Entry e = findEntry(model, dest);
                if (e == null || e.getParent() == null) continue;
                String calleeName = e.getParent().getName();
                if (taskName.equals(calleeName)) continue;
                distinctCallees.add(calleeName);
                if (distinctCallees.size() > 1) return true;
            }
        }
        return false;
    }

    /** True iff {@code task} or any (recursive) sync callee has positive host demand. */
    public static boolean hasServerDemandInSubtree(LayeredNetwork model, String taskName) {
        return hasServerDemandInSubtreeImpl(model, taskName, new HashSet<String>());
    }
    private static boolean hasServerDemandInSubtreeImpl(LayeredNetwork model,
                                                        String taskName, Set<String> visited) {
        if (taskName == null || !visited.add(taskName)) return false;
        if (computeLocalDemand(model, taskName) > 1e-9) return true;
        Task t = findTask(model, taskName);
        if (t == null) return false;
        for (Activity act : t.getActivities()) {
            for (String dest : act.getSyncCallDests().values()) {
                Entry e = findEntry(model, dest);
                if (e == null || e.getParent() == null) continue;
                if (hasServerDemandInSubtreeImpl(model, e.getParent().getName(), visited)) return true;
            }
        }
        return false;
    }


    // =================================================================================
    //  Quantities — task / activity demands
    // =================================================================================

    /** Task think time mean, or 0 if NaN / non-finite / unknown. */
    public static double getTaskThinkTimeSafe(LayeredNetwork model, String taskName) {
        Task t = findTask(model, taskName);
        if (t == null) return 0.0;
        double m = t.getThinkTimeMean();
        return (Double.isNaN(m) || !Double.isFinite(m)) ? 0.0 : m;
    }

    /** Walk up the caller chain until a REF task is found, return its think time. */
    public static double getRootRefThinkTime(LayeredNetwork model, String taskName) {
        Set<String> visited = new HashSet<String>();
        String current = taskName;
        while (current != null && visited.add(current)) {
            Task t = findTask(model, current);
            if (isRefTask(t)) {
                double m = t.getThinkTimeMean();
                return (Double.isNaN(m) || !Double.isFinite(m)) ? 0.0 : m;
            }
            current = findCallerTask(model, current);
        }
        return 0.0;
    }

    /** Task's <i>local</i> demand: Σ visitCount × hostDemand across its activities,
     *  where visitCount &gt; 1 for activities inside a POST_LOOP precedence. */
    public static double computeLocalDemand(LayeredNetwork model, String taskName) {
        Task task = findTask(model, taskName);
        if (task == null) return 0.0;
        Map<String, Double> visitCounts = computeActivityVisitCounts(task);
        double total = 0.0;
        for (Activity act : task.getActivities()) {
            double m = act.getHostDemandMean();
            if (Double.isNaN(m) || m <= 1e-7) continue;
            Double c = visitCounts.get(act.getName());
            total += (c != null ? c : 1.0) * m;
        }
        return total;
    }

    /** Sum of {@link #computeLocalDemand} for every caller strictly above {@code taskName}. */
    public static double computeCallerChainDemand(LayeredNetwork model, String taskName) {
        double total = 0.0;
        Set<String> visited = new HashSet<String>();
        String current = taskName;
        while (current != null && visited.add(current)) {
            String caller = findCallerTask(model, current);
            if (caller == null) break;
            total += computeLocalDemand(model, caller);
            current = caller;
        }
        return total;
    }

    /** Host demand of the activity bound to {@code destEntry} (i.e. "service time
     *  per call into that entry"). 0 if missing / NaN. */
    public static double hostDemandOfBoundActivity(Entry destEntry) {
        if (destEntry == null || destEntry.getParent() == null) return 0.0;
        for (Activity a : destEntry.getParent().getActivities()) {
            if (destEntry.getName().equals(a.getBoundToEntry())) {
                double m = a.getHostDemandMean();
                return Double.isNaN(m) ? 0.0 : m;
            }
        }
        return 0.0;
    }

    /** Σ callMean × hostDemand from {@code caller}'s activities into entries on {@code target}. */
    public static double callerDemandOnTask(LayeredNetwork model, String callerName, String targetTaskName) {
        Task caller = findTask(model, callerName);
        if (caller == null) return 0.0;
        double total = 0.0;
        for (Activity act : caller.getActivities()) {
            Map<Integer, String> dests = act.getSyncCallDests();
            Matrix means = act.getSyncCallMeans();
            if (dests == null || means == null) continue;
            for (int ci = 0; ci < dests.size(); ci++) {
                Entry destEntry = findEntry(model, dests.get(ci));
                if (destEntry == null || destEntry.getParent() == null) continue;
                if (!targetTaskName.equals(destEntry.getParent().getName())) continue;
                total += means.get(0, ci) * hostDemandOfBoundActivity(destEntry);
            }
        }
        return total;
    }

    /**
     * Returns visit count per task-entry visit for each activity, accounting for
     * POST_LOOP precedences: pre-acts fire {@code loopCount} times per visit;
     * post-acts (loop exits) fire once.
     */
    public static Map<String, Double> computeActivityVisitCounts(Task task) {
        Map<String, Double> counts = new HashMap<String, Double>();
        for (Activity a : task.getActivities()) counts.put(a.getName(), 1.0);
        for (ActivityPrecedence prec : task.getPrecedences()) {
            if (!ActivityPrecedenceType.POST_LOOP.equals(prec.getPostType())) continue;
            Matrix p = prec.getPostParams();
            double loopCount = (p != null && p.getNumRows() > 0 && p.getNumCols() > 0)
                    ? p.get(0, 0) : 1.0;
            for (String actName : prec.getPreActs()) counts.put(actName, loopCount);
        }
        return counts;
    }


    // =================================================================================
    //  Quantities — call means
    // =================================================================================

    /** Total expected sync calls per caller-visit from {@code caller} to {@code callee}.
     *  Defaults to 1.0 when no calls are found. */
    public static double getSyncCallMean(LayeredNetwork model, String callerTask, String calleeTask) {
        if (callerTask == null || calleeTask == null) return 1.0;
        Task caller = findTask(model, callerTask);
        if (caller == null) return 1.0;
        double total = 0.0;
        for (Activity act : caller.getActivities()) {
            Map<Integer, String> dests = act.getSyncCallDests();
            if (dests == null || dests.isEmpty()) continue;
            Matrix means = act.getSyncCallMeans();
            for (Map.Entry<Integer, String> e : dests.entrySet()) {
                Entry called = findEntry(model, e.getValue());
                if (called == null || called.getParent() == null) continue;
                if (!calleeTask.equals(called.getParent().getName())) continue;
                int idx = e.getKey();
                double m = (means != null && means.getNumCols() > idx) ? means.get(0, idx) : 1.0;
                if (Double.isFinite(m) && m > 0) total += m;
            }
        }
        return total > 0 ? total : 1.0;
    }

    /** Maximum per-caller total call mean into {@code target}. Used when scaling
     *  leaf-task tput to per-call rates: one caller × 2 calls → 2; two callers × 1
     *  call each → 1. */
    public static double getTotalInboundCallMean(LayeredNetwork model, String targetTask) {
        double max = 0.0;
        for (Task caller : model.getTasks().values()) {
            double sum = sumCallsFromCallerToTarget(model, caller, targetTask);
            if (sum > max) max = sum;
        }
        return max;
    }

    /** Sum of call means from {@code caller} to all entries on {@code target},
     *  across all activities. Denominator when splitting a callee's Q by activity. */
    public static double getCallerTotalCallMean(LayeredNetwork model, String callerName, String targetTask) {
        Task caller = findTask(model, callerName);
        if (caller == null) return 1.0;
        double sum = sumCallsFromCallerToTarget(model, caller, targetTask);
        return Math.max(1.0, sum);
    }

    /** Sum of call means into {@code target} across ALL callers. Denominator when
     *  apportioning a callee's Q across callers: caller_share = caller_mean / total. */
    public static double getSumInboundCallMean(LayeredNetwork model, String targetTask) {
        double total = 0.0;
        for (Task caller : model.getTasks().values()) {
            total += sumCallsFromCallerToTarget(model, caller, targetTask);
        }
        return total;
    }

    private static double sumCallsFromCallerToTarget(LayeredNetwork model, Task caller, String targetTask) {
        double sum = 0.0;
        for (Activity act : caller.getActivities()) {
            Map<Integer, String> dests = act.getSyncCallDests();
            if (dests == null || dests.isEmpty()) continue;
            Matrix means = act.getSyncCallMeans();
            for (Map.Entry<Integer, String> e : dests.entrySet()) {
                Entry called = findEntry(model, e.getValue());
                if (called == null || called.getParent() == null) continue;
                if (!targetTask.equals(called.getParent().getName())) continue;
                int idx = e.getKey();
                double m = (means != null && means.getNumCols() > idx) ? means.get(0, idx) : 1.0;
                if (Double.isFinite(m) && m > 0) sum += m;
            }
        }
        return sum;
    }


    // =================================================================================
    //  Graph-wide caller maps
    // =================================================================================

    /** Caller task name → (last-seen) synchronously-called task name. */
    public static Map<String, String> buildTaskCalledTaskMap(LayeredNetwork model) {
        Map<String, String> result = new HashMap<String, String>();
        for (Task task : model.getTasks().values()) {
            for (Activity act : task.getActivities()) {
                if (act.getSyncCallDests() == null) continue;
                for (String dest : act.getSyncCallDests().values()) {
                    Entry e = findEntry(model, dest);
                    if (e == null || e.getParent() == null) continue;
                    String calledTaskName = e.getParent().getName();
                    if (!task.getName().equals(calledTaskName)) {
                        result.put(task.getName(), calledTaskName);
                    }
                }
            }
        }
        return result;
    }

    /** REF task name → (last-seen) synchronously-called task name. */
    public static Map<String, String> buildRefTaskCalledTaskMap(LayeredNetwork model) {
        Map<String, String> result = new HashMap<String, String>();
        for (Task task : model.getTasks().values()) {
            if (!isRefTask(task)) continue;
            for (Activity act : task.getActivities()) {
                if (act.getSyncCallDests() == null) continue;
                for (String dest : act.getSyncCallDests().values()) {
                    Entry e = findEntry(model, dest);
                    if (e == null || e.getParent() == null) continue;
                    String calledTaskName = e.getParent().getName();
                    if (!task.getName().equals(calledTaskName)) {
                        result.put(task.getName(), calledTaskName);
                    }
                }
            }
        }
        return result;
    }


    // =================================================================================
    //  Misc
    // =================================================================================

    /** Strip the {@code T:} / {@code P:} / {@code R:} prefix from a layer/class name. */
    public static String stripPrefix(String name) {
        int idx = name.indexOf(':');
        return (idx >= 0 && idx + 1 < name.length()) ? name.substring(idx + 1) : name;
    }
}
