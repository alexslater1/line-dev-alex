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

import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Deque;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
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

    /** True iff {@code task} contains an AND_FORK→AND_JOIN region inside one
     *  of its entries' activity DAG. Used by host-layer coupling to skip
     *  overwriting T:-layer D (which holds caller-perceived MAX) with the
     *  host's processor SUM. */
    public static boolean taskHasAndFork(LayeredNetwork model, String taskName) {
        Task t = findTask(model, taskName);
        if (t == null) return false;
        List<ActivityPrecedence> precs = t.getPrecedences();
        if (precs == null) return false;
        for (ActivityPrecedence p : precs) {
            if (ActivityPrecedenceType.POST_AND.equals(p.getPostType())) {
                List<String> post = p.getPostActs();
                if (post != null && post.size() >= 2) return true;
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

    /** Task's <i>local</i> demand: Σ visitWeight × hostDemand across its activities,
     *  where visitWeight follows the per-task DAG walk (sequence + OR_FORK +
     *  AND_FORK + POST_LOOP). For multi-entry tasks the per-entry walks are
     *  summed: if every entry is called once per outer cycle, each activity
     *  fires once and the result equals Σ hostDemand over the task's DAG-
     *  reachable activities. */
    public static double computeLocalDemand(LayeredNetwork model, String taskName) {
        Task task = findTask(model, taskName);
        if (task == null) return 0.0;
        Map<String, Double> visitWeights = computeActivityVisitWeights(task);
        double total = 0.0;
        for (Activity act : task.getActivities()) {
            double m = act.getHostDemandMean();
            if (Double.isNaN(m) || m <= 1e-7) continue;
            Double w = visitWeights.get(act.getName());
            total += (w != null ? w : 1.0) * m;
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

    /** Caller-perceived host demand for one call into {@code destEntry}: walks
     *  the activity DAG from the bound activity, summing
     *  {@code visitWeight × hostDemand} across reachable activities. For
     *  AND_FORK→AND_JOIN regions, branches contribute their expected max of
     *  exponential service times instead of the per-branch sum — the caller
     *  only waits for the slowest branch.
     *  Returns 0 if the entry has no bound activity. Use
     *  {@link #processorDemandOfEntry} when the SUM (processor-occupancy)
     *  view is needed instead. */
    public static double hostDemandOfEntry(Entry destEntry) {
        return hostDemandOfEntryImpl(destEntry, /*processorSemantics=*/ false);
    }

    /** Processor-occupancy variant of {@link #hostDemandOfEntry}: AND_FORK
     *  branches sum (both consume processor time) rather than collapsing to
     *  E[max]. Used to set P:-layer demand and per-class utilization splits. */
    public static double processorDemandOfEntry(Entry destEntry) {
        return hostDemandOfEntryImpl(destEntry, /*processorSemantics=*/ true);
    }

    private static double hostDemandOfEntryImpl(Entry destEntry, boolean processorSemantics) {
        if (destEntry == null || destEntry.getParent() == null) return 0.0;
        Activity bound = findBoundActivity(destEntry);
        if (bound == null) return 0.0;
        Task task = destEntry.getParent();
        Map<String, Double> weights = computeWeightsFromBound(task, bound);
        double total = 0.0;
        for (Activity a : task.getActivities()) {
            Double w = weights.get(a.getName());
            if (w == null || w <= 1e-12) continue;
            double m = a.getHostDemandMean();
            if (Double.isNaN(m) || m <= 1e-9) continue;
            total += w * m;
        }
        if (!processorSemantics) {
            total += andForkMaxCorrection(task, weights);
        }
        return total;
    }

    /** Replaces each AND_FORK→AND_JOIN region's branch-sum with
     *  {@link #expectedMaxOfExponentials E[max]}. Returns the (non-positive)
     *  correction {@code E[max] − Σ branchDemand} summed over matched
     *  fork/join pairs. This is Heidelberger–Trivedi's CD formula (Franks
     *  thesis eq. 2.6), which is also the join-delay formula LQNS uses for
     *  exponential branches. Multi-activity branches collapse the in-branch
     *  total to a single equivalent exponential — fine when branch CV is
     *  bounded; see Franks §8.2.2 for the higher-fidelity Jiang three-point
     *  approximation. Unmatched forks leave the total unchanged. */
    private static double andForkMaxCorrection(Task task, Map<String, Double> weights) {
        List<ActivityPrecedence> precs = task.getPrecedences();
        if (precs == null || precs.isEmpty()) return 0.0;
        double correction = 0.0;
        for (ActivityPrecedence forkP : precs) {
            if (!ActivityPrecedenceType.POST_AND.equals(forkP.getPostType())) continue;
            List<String> branchStarters = forkP.getPostActs();
            if (branchStarters == null || branchStarters.size() < 2) continue;
            ActivityPrecedence joinP = findMatchingAndJoin(precs, branchStarters);
            if (joinP == null) continue;
            Set<String> joinPreSet = new HashSet<String>(joinP.getPreActs());
            List<Double> branchDemands = new ArrayList<Double>();
            double sum = 0.0;
            for (String branchStart : branchStarters) {
                double d = collectBranchDemand(task, branchStart, joinPreSet, weights);
                if (d > 1e-12) {
                    branchDemands.add(d);
                    sum += d;
                }
            }
            if (branchDemands.size() < 2) continue;
            double emax = expectedMaxOfExponentials(branchDemands);
            correction += (emax - sum);
        }
        return correction;
    }

    /** Expected value of the maximum of {@code n} independent exponential
     *  random variables with means {@code D_i}. Uses the inclusion-exclusion
     *  identity {@code E[max] = Σ_{∅ ≠ S} (−1)^{|S|+1} / Σ_{i∈S} (1/D_i)}.
     *  Has 2^n − 1 terms; cheap for n ≤ ~10 (typical AND_FORK fanout). */
    private static double expectedMaxOfExponentials(List<Double> means) {
        int n = means.size();
        if (n == 0) return 0.0;
        if (n == 1) return means.get(0);
        double total = 0.0;
        int subsets = 1 << n;
        for (int mask = 1; mask < subsets; mask++) {
            double rateSum = 0.0;
            int bits = 0;
            for (int i = 0; i < n; i++) {
                if ((mask & (1 << i)) != 0) {
                    double d = means.get(i);
                    if (d <= 1e-12) { rateSum = Double.POSITIVE_INFINITY; break; }
                    rateSum += 1.0 / d;
                    bits++;
                }
            }
            if (rateSum <= 0 || !Double.isFinite(rateSum)) continue;
            double term = 1.0 / rateSum;
            if ((bits & 1) == 0) term = -term;
            total += term;
        }
        return total;
    }

    /** Find the PRE_AND precedence that joins the fork with these branch
     *  starters: each starter has a forward POST_SEQ path to one join preAct,
     *  with a bijective starter→tip mapping. Handles both direct joins
     *  (starters == join preActs) and multi-activity branches. Returns
     *  {@code null} if no such precedence exists. */
    private static ActivityPrecedence findMatchingAndJoin(List<ActivityPrecedence> precs,
                                                          List<String> branchStarters) {
        Set<String> starterSet = new HashSet<String>(branchStarters);
        for (ActivityPrecedence p : precs) {
            if (!ActivityPrecedenceType.PRE_AND.equals(p.getPreType())) continue;
            List<String> joinPre = p.getPreActs();
            if (joinPre == null) continue;
            Set<String> joinSet = new HashSet<String>(joinPre);
            if (joinSet.size() != starterSet.size()) continue;
            if (joinSet.equals(starterSet)) return p;
            Set<String> reached = new HashSet<String>();
            boolean ok = true;
            for (String start : branchStarters) {
                String hit = firstReachableJoinPre(start, joinSet, precs);
                if (hit == null || !reached.add(hit)) { ok = false; break; }
            }
            if (ok && reached.equals(joinSet)) return p;
        }
        return null;
    }

    /** Forward DFS from {@code start} along POST_SEQ precedences. Returns the
     *  first activity in {@code joinPreActs} reached (inclusive of {@code start}
     *  itself if it is already a join preAct), or {@code null} if none. */
    private static String firstReachableJoinPre(String start, Set<String> joinPreActs,
                                                List<ActivityPrecedence> precs) {
        if (joinPreActs.contains(start)) return start;
        Set<String> visited = new HashSet<String>();
        Deque<String> stack = new ArrayDeque<String>();
        stack.push(start);
        while (!stack.isEmpty()) {
            String cur = stack.pop();
            if (!visited.add(cur)) continue;
            if (joinPreActs.contains(cur)) return cur;
            for (ActivityPrecedence p : precs) {
                if (!ActivityPrecedenceType.POST_SEQ.equals(p.getPostType())) continue;
                List<String> pre = p.getPreActs();
                List<String> post = p.getPostActs();
                if (pre == null || post == null || pre.size() != 1) continue;
                if (!cur.equals(pre.get(0))) continue;
                for (String n : post) stack.push(n);
            }
        }
        return null;
    }

    /** Sum of {@code weight × hostDemand} for one branch of an AND_FORK,
     *  collected by forward DFS from {@code branchStart} along POST_SEQ
     *  precedences, including the branch tip (an activity in {@code joinPreActs})
     *  and stopping there. Avoids re-entering the join. */
    private static double collectBranchDemand(Task task, String branchStart,
                                              Set<String> joinPreActs,
                                              Map<String, Double> weights) {
        List<ActivityPrecedence> precs = task.getPrecedences();
        double demand = 0.0;
        Set<String> visited = new HashSet<String>();
        Deque<String> stack = new ArrayDeque<String>();
        stack.push(branchStart);
        while (!stack.isEmpty()) {
            String actName = stack.pop();
            if (!visited.add(actName)) continue;
            Activity a = findActivityInTask(task, actName);
            if (a != null) {
                Double w = weights.get(actName);
                double m = a.getHostDemandMean();
                if (w != null && w > 1e-12 && !Double.isNaN(m) && m > 1e-9) {
                    demand += w * m;
                }
            }
            if (joinPreActs.contains(actName)) continue;
            for (ActivityPrecedence p : precs) {
                if (!ActivityPrecedenceType.POST_SEQ.equals(p.getPostType())) continue;
                List<String> pre = p.getPreActs();
                List<String> post = p.getPostActs();
                if (pre == null || post == null) continue;
                if (pre.size() != 1 || !actName.equals(pre.get(0))) continue;
                for (String n : post) stack.push(n);
            }
        }
        return demand;
    }

    private static Activity findActivityInTask(Task task, String name) {
        for (Activity a : task.getActivities()) {
            if (name.equals(a.getName())) return a;
        }
        return null;
    }

    /** Activity bound to {@code destEntry}, or {@code null} if none. */
    public static Activity findBoundActivity(Entry destEntry) {
        if (destEntry == null || destEntry.getParent() == null) return null;
        for (Activity a : destEntry.getParent().getActivities()) {
            if (destEntry.getName().equals(a.getBoundToEntry())) return a;
        }
        return null;
    }

    /**
     * Per-visit demand the {@code caller} task imposes on the {@code target} task:
     * Σ {@code visitWeight(act) × callMean × hostDemandOfEntry(destEntry)} for
     * sync calls into entries on {@code target}.
     *
     * <p>For single-entry callers this sums across all of the caller's activities
     * weighted by the per-activity DAG visit weight (one visit traverses the
     * entire task DAG). For multi-entry callers each visit dispatches to one
     * entry's DAG, so we average the per-entry walks — the result is the mean
     * per-visit demand rather than the sum across mutually-exclusive entries.
     */
    public static double callerDemandOnTask(LayeredNetwork model, String callerName, String targetTaskName) {
        return callerDemandOnTaskImpl(model, callerName, targetTaskName, /*processorSemantics=*/ false);
    }

    /** Processor-occupancy variant of {@link #callerDemandOnTask}: every branch
     *  of an AND_FORK in the callee contributes its host demand additively
     *  rather than collapsing into branch-max. Use when setting P:-layer
     *  demand or computing {@code Util = ΣX·D/c}. */
    public static double callerProcessorDemandOnTask(LayeredNetwork model, String callerName, String targetTaskName) {
        return callerDemandOnTaskImpl(model, callerName, targetTaskName, /*processorSemantics=*/ true);
    }

    private static double callerDemandOnTaskImpl(LayeredNetwork model, String callerName,
                                                 String targetTaskName, boolean processorSemantics) {
        Task caller = findTask(model, callerName);
        if (caller == null) return 0.0;

        List<Entry> callerEntries = new ArrayList<Entry>();
        for (Entry e : model.getEntries().values()) {
            if (e.getParent() == caller) callerEntries.add(e);
        }
        if (callerEntries.size() <= 1) {
            Map<String, Double> weights = computeActivityVisitWeights(caller);
            double total = 0.0;
            for (Activity act : caller.getActivities()) {
                Double w = weights.get(act.getName());
                if (w == null || w <= 1e-12) continue;
                total += w * demandFromActivityToTask(model, act, targetTaskName, processorSemantics);
            }
            return total;
        }
        double total = 0.0;
        for (Entry e : callerEntries) {
            Activity bound = findBoundActivity(e);
            if (bound == null) continue;
            Map<String, Double> weights = computeWeightsFromBound(caller, bound);
            for (Activity act : caller.getActivities()) {
                Double w = weights.get(act.getName());
                if (w == null || w <= 1e-12) continue;
                total += w * demandFromActivityToTask(model, act, targetTaskName, processorSemantics);
            }
        }
        return total / callerEntries.size();
    }

    /** Per-visit demand <i>contributed by one activity</i> into {@code targetTaskName}:
     *  Σ {@code callMean × hostDemandOfEntry(destEntry)} across this activity's
     *  sync calls into entries of the target. Caller-side visit weighting is
     *  applied externally so this stays a pure per-activity quantity. When
     *  {@code processorSemantics} is true each call's contribution uses
     *  {@link #processorDemandOfEntry} (SUM over AND_FORK branches) instead of
     *  the caller-perceived {@link #hostDemandOfEntry} (MAX over branches). */
    private static double demandFromActivityToTask(LayeredNetwork model, Activity act,
                                                   String targetTaskName, boolean processorSemantics) {
        Map<Integer, String> dests = act.getSyncCallDests();
        if (dests == null || dests.isEmpty()) return 0.0;
        Matrix means = act.getSyncCallMeans();
        double d = 0.0;
        for (Map.Entry<Integer, String> ce : dests.entrySet()) {
            Entry destEntry = findEntry(model, ce.getValue());
            if (destEntry == null || destEntry.getParent() == null) continue;
            if (!targetTaskName.equals(destEntry.getParent().getName())) continue;
            int idx = ce.getKey();
            double cm = (means != null && means.getNumCols() > idx) ? means.get(0, idx) : 1.0;
            d += cm * (processorSemantics ? processorDemandOfEntry(destEntry) : hostDemandOfEntry(destEntry));
        }
        return d;
    }

    /**
     * Per-activity visit weight for the whole task, summing per-entry DAG walks.
     *
     * <p>The walk starts at each entry's bound activity with weight 1, then
     * propagates forward through precedences:
     * <ul>
     *   <li><b>POST_SEQ</b>: postAct inherits preAct's weight.</li>
     *   <li><b>POST_AND</b>: each postAct inherits preAct's weight (every branch
     *       fires once per visit).</li>
     *   <li><b>POST_OR</b>: postAct[i] inherits preAct's weight × branch
     *       probability ({@code postParams[0][i]}).</li>
     *   <li><b>POST_LOOP</b>: postAct[i] in {@code 0..postParams.cols-1}
     *       inherits preAct's weight × loop count ({@code postParams[0][i]});
     *       the trailing post-act (when {@code postActs.size() == postParams.cols + 1})
     *       inherits preAct's weight × 1.</li>
     *   <li><b>PRE_AND</b> (AND_JOIN): postAct inherits {@code max} of preActs'
     *       weights (they should already be equal coming from a matched fork).</li>
     *   <li><b>PRE_OR</b> (OR_JOIN): postAct inherits sum of preActs' weights —
     *       branches contribute their probability mass.</li>
     * </ul>
     *
     * <p>Multi-entry tasks sum per-entry walks; an activity reachable only
     * from one entry gets weight 1.0 across the task. Tasks with no bound
     * activities OR no precedences fall back to weight 1.0 per activity (the
     * historical bag-of-activities default).
     */
    public static Map<String, Double> computeActivityVisitWeights(Task task) {
        Map<String, Double> total = new HashMap<String, Double>();
        for (Activity a : task.getActivities()) total.put(a.getName(), 0.0);

        List<ActivityPrecedence> precs = task.getPrecedences();
        boolean hasPrecs = precs != null && !precs.isEmpty();

        boolean anyBound = false;
        for (Activity a : task.getActivities()) {
            String be = a.getBoundToEntry();
            if (be == null || be.isEmpty()) continue;
            anyBound = true;
            Map<String, Double> w = computeWeightsFromBound(task, a);
            for (Map.Entry<String, Double> e : w.entrySet()) {
                Double cur = total.get(e.getKey());
                total.put(e.getKey(), (cur != null ? cur : 0.0) + e.getValue());
            }
        }
        if (!anyBound || !hasPrecs) {
            // Bag fallback: tasks without entry bindings or precedences are
            // treated as the historical "every activity fires once" model.
            for (Activity a : task.getActivities()) total.put(a.getName(), 1.0);
        }
        return total;
    }

    /** Backward-compatible alias retained for callers expecting visit counts.
     *  Now returns the DAG-aware per-activity visit weight. */
    public static Map<String, Double> computeActivityVisitCounts(Task task) {
        return computeActivityVisitWeights(task);
    }

    /**
     * Per-activity visit weight for the DAG rooted at one bound activity.
     * The bound activity starts at weight 1.0; all others at 0.0. Precedences
     * are applied in fixed-point iteration (declarations are usually in topo
     * order, but iteration tolerates out-of-order declarations and the loop
     * caps at one pass per activity so it always terminates).
     *
     * <p>Public because {@link EnsembleInitialiser} and the results-collector
     * package both need per-entry weights when computing per-caller demand
     * and per-activity ownership for multi-entry callers.
     */
    public static Map<String, Double> computeWeightsFromBound(Task task, Activity boundAct) {
        List<Activity> activities = task.getActivities();
        List<ActivityPrecedence> precs = task.getPrecedences();

        Map<String, Double> w = new HashMap<String, Double>();
        for (Activity a : activities) w.put(a.getName(), 0.0);
        w.put(boundAct.getName(), 1.0);

        if (precs == null || precs.isEmpty()) return w;

        int maxIter = activities.size() + 5;
        for (int iter = 0; iter < maxIter; iter++) {
            Map<String, Double> next = new HashMap<String, Double>();
            for (Activity a : activities) {
                next.put(a.getName(), a.getName().equals(boundAct.getName()) ? 1.0 : 0.0);
            }
            for (ActivityPrecedence p : precs) {
                applyPrecedence(p, w, next);
            }
            if (mapsEqual(next, w)) return next;
            w = next;
        }
        return w;
    }

    /** Apply one precedence: read fan-in weight from {@code in}, write
     *  contributions to {@code out}. Multiple precedences targeting the same
     *  activity sum (which is what OR-style merging across precedences wants;
     *  AND-style joins are already collapsed to {@code max} on the pre side). */
    private static void applyPrecedence(ActivityPrecedence p,
                                        Map<String, Double> in,
                                        Map<String, Double> out) {
        String preType = p.getPreType();
        String postType = p.getPostType();
        List<String> preActs = p.getPreActs();
        List<String> postActs = p.getPostActs();
        Matrix postParams = p.getPostParams();

        if (preActs == null || preActs.isEmpty() || postActs == null || postActs.isEmpty()) return;

        // Fan-in: aggregate predecessor weights by preType.
        double fanIn;
        if (ActivityPrecedenceType.PRE_AND.equals(preType)) {
            double m = 0.0;
            for (String pre : preActs) {
                Double v = in.get(pre);
                if (v != null && v > m) m = v;
            }
            fanIn = m;
        } else if (ActivityPrecedenceType.PRE_OR.equals(preType)) {
            double s = 0.0;
            for (String pre : preActs) {
                Double v = in.get(pre);
                if (v != null) s += v;
            }
            fanIn = s;
        } else {  // PRE_SEQ — single preAct
            Double v = in.get(preActs.get(0));
            fanIn = (v != null) ? v : 0.0;
        }
        if (fanIn <= 1e-12) return;

        // Fan-out: distribute to postActs by postType.
        if (ActivityPrecedenceType.POST_OR.equals(postType)) {
            int n = postActs.size();
            int paramCols = (postParams != null) ? postParams.getNumCols() : 0;
            for (int i = 0; i < n; i++) {
                double prob = (i < paramCols) ? postParams.get(0, i) : 0.0;
                addWeight(out, postActs.get(i), fanIn * prob);
            }
        } else if (ActivityPrecedenceType.POST_LOOP.equals(postType)) {
            int n = postActs.size();
            int paramCols = (postParams != null) ? postParams.getNumCols() : 0;
            // POST_LOOP has two conventions depending on whether the call
            // supplied an explicit end activity:
            //   * postActs.size() == paramCols + 1 (e.g. Loop(pre, [body, end], n)):
            //     preActs are the loop entry (fires once via fan-in),
            //     postActs[0..paramCols-1] are loop bodies (×count), and
            //     postActs[last] is the end activity firing once.
            //   * postActs.size() == paramCols (e.g. Loop(pre, [body], n)):
            //     no separate end activity. preActs are themselves the loop
            //     body (fire {@code count} times), modelled as a self-loop
            //     with edge weight (N-1)/N back to preAct and 1/N forward to
            //     each postAct (matches LN's reporting on this form, verified
            //     against sc5_loop). The fractional split is what makes the
            //     fixed-point iteration converge: bound activity weight 1
            //     plus self-loop weight (N-1)/N × N = N-1 yields N total.
            boolean hasEndAct = n == paramCols + 1;
            if (hasEndAct) {
                for (int i = 0; i < n; i++) {
                    double mult = (i < paramCols) ? postParams.get(0, i) : 1.0;
                    addWeight(out, postActs.get(i), fanIn * mult);
                }
            } else {
                // preActs are themselves the loop body. Scale their existing
                // weight in {@code out} (from the bound-activity init or
                // earlier precedences) by loopCount; emit postActs once each
                // as the sequential continuation, using the pre-multiplication
                // {@code out} value rather than {@code in} so the fixed-point
                // iteration converges in one pass instead of decaying
                // geometrically.
                double loopCount = (paramCols > 0) ? postParams.get(0, 0) : 1.0;
                double preInTotal = 0.0;
                for (String pre : preActs) {
                    Double cur = out.get(pre);
                    if (cur == null) continue;
                    preInTotal += cur;
                    if (loopCount > 1.0) out.put(pre, cur * loopCount);
                }
                for (String post : postActs) addWeight(out, post, preInTotal);
            }
        } else if (ActivityPrecedenceType.POST_AND.equals(postType)) {
            for (String post : postActs) addWeight(out, post, fanIn);
        } else {  // POST_SEQ — single postAct
            addWeight(out, postActs.get(0), fanIn);
        }
    }

    private static void addWeight(Map<String, Double> map, String key, double delta) {
        if (delta == 0.0 || !map.containsKey(key)) return;
        map.put(key, map.get(key) + delta);
    }

    private static boolean mapsEqual(Map<String, Double> a, Map<String, Double> b) {
        if (a.size() != b.size()) return false;
        for (Map.Entry<String, Double> e : a.entrySet()) {
            Double bv = b.get(e.getKey());
            if (bv == null) return false;
            if (Math.abs(e.getValue() - bv) > 1e-12) return false;
        }
        return true;
    }


    // =================================================================================
    //  Quantities — call means
    // =================================================================================

    /**
     * Expected sync calls <i>per caller-visit</i> from {@code caller} to
     * {@code callee}. For single-entry callers this is the sum across all of
     * the caller's activities (one visit traverses the whole task DAG); for
     * multi-entry callers it is the average across entries of each bound
     * activity's calls into {@code callee}, since each visit dispatches to one
     * entry. Defaults to 1.0 when no calls are found.
     */
    public static double getSyncCallMean(LayeredNetwork model, String callerTask, String calleeTask) {
        if (callerTask == null || calleeTask == null) return 1.0;
        Task caller = findTask(model, callerTask);
        if (caller == null) return 1.0;
        double cm = perVisitCallsFromCallerToTarget(model, caller, calleeTask);
        return cm > 0 ? cm : 1.0;
    }

    /**
     * Maximum <i>per-visit</i> call mean from any caller into {@code target}.
     * Used as the conversion factor between per-visit and per-call quantities at
     * a leaf task (T:-layer X × this = per-call rate; per-visit residual ÷ this =
     * per-call residual).
     *
     * <p>For single-entry callers the per-visit call mean is just the sum across
     * the caller's activities (one visit traverses the whole task DAG). For
     * multi-entry callers each visit runs <i>one</i> entry's bound activity, so
     * we take the average across entries of (sum across that entry's bound
     * activity's sync calls into {@code target}).
     */
    public static double getTotalInboundCallMean(LayeredNetwork model, String targetTask) {
        double max = 0.0;
        for (Task caller : model.getTasks().values()) {
            double cm = perVisitCallsFromCallerToTarget(model, caller, targetTask);
            if (cm > max) max = cm;
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
        Map<String, Double> visitWeights = computeActivityVisitWeights(caller);
        double sum = 0.0;
        for (Activity act : caller.getActivities()) {
            Map<Integer, String> dests = act.getSyncCallDests();
            if (dests == null || dests.isEmpty()) continue;
            Double w = visitWeights.get(act.getName());
            if (w == null || w <= 1e-12) continue;
            Matrix means = act.getSyncCallMeans();
            for (Map.Entry<Integer, String> e : dests.entrySet()) {
                Entry called = findEntry(model, e.getValue());
                if (called == null || called.getParent() == null) continue;
                if (!targetTask.equals(called.getParent().getName())) continue;
                int idx = e.getKey();
                double m = (means != null && means.getNumCols() > idx) ? means.get(0, idx) : 1.0;
                if (Double.isFinite(m) && m > 0) sum += w * m;
            }
        }
        return sum;
    }

    /**
     * Per-visit call mean from {@code caller} into {@code targetTask}: sum across
     * caller's activities weighted by the DAG visit weight (single-entry
     * callers traverse the whole task DAG per visit, so all activities
     * reachable from the bound activity contribute according to their visit
     * weight). For multi-entry callers each visit dispatches to one entry's
     * own DAG, so we average per-entry contributions, with each entry's
     * contribution being the weighted sum across its own reachable activities.
     */
    private static double perVisitCallsFromCallerToTarget(LayeredNetwork model, Task caller, String targetTask) {
        List<Entry> callerEntries = new ArrayList<Entry>();
        for (Entry e : model.getEntries().values()) {
            if (e.getParent() == caller) callerEntries.add(e);
        }
        if (callerEntries.size() <= 1) {
            return sumCallsFromCallerToTarget(model, caller, targetTask);
        }
        double total = 0.0;
        for (Entry e : callerEntries) {
            Activity bound = findBoundActivity(e);
            if (bound == null) continue;
            Map<String, Double> weights = computeWeightsFromBound(caller, bound);
            for (Activity act : caller.getActivities()) {
                Double w = weights.get(act.getName());
                if (w == null || w <= 1e-12) continue;
                Map<Integer, String> dests = act.getSyncCallDests();
                if (dests == null || dests.isEmpty()) continue;
                Matrix means = act.getSyncCallMeans();
                for (Map.Entry<Integer, String> ce : dests.entrySet()) {
                    Entry called = findEntry(model, ce.getValue());
                    if (called == null || called.getParent() == null) continue;
                    if (!targetTask.equals(called.getParent().getName())) continue;
                    int idx = ce.getKey();
                    double m = (means != null && means.getNumCols() > idx) ? means.get(0, idx) : 1.0;
                    if (Double.isFinite(m) && m > 0) total += w * m;
                }
            }
        }
        return total / callerEntries.size();
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
