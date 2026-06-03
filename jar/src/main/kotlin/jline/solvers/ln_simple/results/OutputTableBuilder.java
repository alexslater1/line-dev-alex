package jline.solvers.ln_simple.results;

import jline.lang.constant.SchedStrategy;
import jline.lang.layered.Activity;
import jline.lang.layered.Entry;
import jline.lang.layered.Host;
import jline.lang.layered.LayeredNetwork;
import jline.lang.layered.Task;
import jline.solvers.LayeredNetworkAvgTable;
import jline.solvers.ln_simple.LqnGraph;
import jline.util.matrix.Matrix;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;

/**
 * Phase 5 of result assembly: walk the LQN model in declaration order and
 * produce one row per {@code Host}, {@code Task}, {@code Entry}, and
 * {@code Activity}.
 *
 * <p>The table rows are appended in that order. Most values come straight from
 * {@link ResultsState}; the interesting bits are:
 * <ul>
 *   <li><b>Entry/Activity splits</b> — multi-entry tasks send different amounts
 *       of work to the shared server, so we weight by
 *       {@code (callerTput × hostDemand)} instead of by raw caller-throughput.
 *       See {@link #computeEntryQLenFractions}.</li>
 *   <li><b>REF-task activity splits</b> — a REF task's aggregate Q / Util / ResidT
 *       are divided across activities proportionally to their visit-weighted
 *       host demand (loop bodies fire {@code loopCount} times per visit).</li>
 *   <li><b>REF-task activity QLen with callees</b> — additionally accumulates
 *       (callMean / total_callMean) × callee.QLen for every sync call out of
 *       the activity, so an activity that mostly waits for callees gets a
 *       large Q even with small local demand.</li>
 * </ul>
 */
final class OutputTableBuilder {

    private OutputTableBuilder() {}

    static LayeredNetworkAvgTable build(LayeredNetwork model,
                                        Map<String, String> taskCalledTask,
                                        ResultsState st) {
        List<String> names = new ArrayList<String>();
        List<String> types = new ArrayList<String>();
        List<Double> qlen = new ArrayList<Double>();
        List<Double> util = new ArrayList<Double>();
        List<Double> respT = new ArrayList<Double>();
        List<Double> residT = new ArrayList<Double>();
        List<Double> arvR = new ArrayList<Double>();
        List<Double> tput = new ArrayList<Double>();

        appendProcessorRows(model, st, names, types, qlen, util, respT, residT, arvR, tput);
        appendTaskRows(model, taskCalledTask, st, names, types, qlen, util, respT, residT, arvR, tput);

        // Two demand-weighted entry splits:
        //   qlen  — full per-call demand (host + downstream blocking)
        //   util  — host-demand only (processor occupancy share)
        Map<String, Double> entryQlenFraction = computeEntryQLenFractions(model, st);
        Map<String, Double> entryUtilFraction = computeEntryUtilFractions(model, st);

        appendEntryRows(model, entryQlenFraction, taskCalledTask, st,
                names, types, qlen, util, respT, residT, arvR, tput);
        appendActivityRows(model, entryQlenFraction, entryUtilFraction, taskCalledTask, st,
                names, types, qlen, util, respT, residT, arvR, tput);

        LayeredNetworkAvgTable table = new LayeredNetworkAvgTable(qlen, util, respT, residT, arvR, tput);
        table.setNodeNames(names);
        table.setNodeTypes(types);
        return table;
    }


    // =====================================================================
    //  Row appenders
    // =====================================================================

    private static void appendProcessorRows(LayeredNetwork model, ResultsState st,
                                            List<String> names, List<String> types,
                                            List<Double> qlen, List<Double> util,
                                            List<Double> respT, List<Double> residT,
                                            List<Double> arvR, List<Double> tput) {
        for (Host proc : model.getHosts().values()) {
            names.add(proc.getName());
            types.add("Processor");
            qlen.add(Double.NaN);
            util.add(st.processorUtil.containsKey(proc.getName()) ? st.processorUtil.get(proc.getName()) : 0.0);
            respT.add(Double.NaN);
            residT.add(Double.NaN);
            arvR.add(Double.NaN);
            tput.add(Double.NaN);
        }
    }

    private static void appendTaskRows(LayeredNetwork model,
                                       Map<String, String> taskCalledTask,
                                       ResultsState st,
                                       List<String> names, List<String> types,
                                       List<Double> qlen, List<Double> util,
                                       List<Double> respT, List<Double> residT,
                                       List<Double> arvR, List<Double> tput) {
        for (Task task : model.getTasks().values()) {
            double resid = st.taskResidT.containsKey(task.getName()) ? st.taskResidT.get(task.getName()) : 0.0;
            // For non-REF leaf tasks with hostLayerResid, prefer that. The
            // value is per-cycle (Q/X at the host layer, both per-cycle);
            // LN reports per-call ResidT, so divide by maxCM to convert.
            if (task.getScheduling() != SchedStrategy.REF
                    && !taskCalledTask.containsKey(task.getName())
                    && st.hostLayerResid.containsKey(task.getName())) {
                double hrv = st.hostLayerResid.get(task.getName());
                double maxCM = LqnGraph.getTotalInboundCallMean(model, task.getName());
                if (maxCM > 1.0 + 1e-9) hrv = hrv / maxCM;
                resid = hrv;
            }

            names.add(task.getName());
            types.add(task.getScheduling() == SchedStrategy.REF ? "RefTask" : "Task");
            qlen.add(st.taskQLen.containsKey(task.getName()) ? st.taskQLen.get(task.getName()) : 0.0);
            util.add(st.taskUtil.containsKey(task.getName()) ? st.taskUtil.get(task.getName()) : 0.0);
            respT.add(st.taskRespT.containsKey(task.getName()) ? st.taskRespT.get(task.getName()) : Double.NaN);
            residT.add(resid);
            arvR.add(Double.NaN);
            tput.add(st.taskTput.containsKey(task.getName()) ? st.taskTput.get(task.getName()) : 0.0);
        }
    }

    private static void appendEntryRows(LayeredNetwork model,
                                        Map<String, Double> entryQlenFraction,
                                        Map<String, String> taskCalledTask,
                                        ResultsState st,
                                        List<String> names, List<String> types,
                                        List<Double> qlen, List<Double> util,
                                        List<Double> respT, List<Double> residT,
                                        List<Double> arvR, List<Double> tput) {
        for (Entry entry : model.getEntries().values()) {
            String parentTaskName = (entry.getParent() != null) ? entry.getParent().getName() : null;
            double taskQ = mapGet(st.taskQLen, parentTaskName, 0.0);
            double parentTput = mapGet(st.taskTput, parentTaskName, 0.0);
            double callerTput = computeEntryCallerTput(entry, model, st.taskTput);
            Task parentTask = LqnGraph.findTask(model, parentTaskName);
            // Single-entry tasks: the entry inherits the full task throughput
            // (no other entry to split with). Skipping the fractional path
            // matters when parentTput was post-corrected for AND_FORK
            // (callerTput < parentTput then, so the min(.) cap would clip
            // wrongly).
            boolean singleEntry = parentTask != null && parentTask.getEntries() != null
                    && parentTask.getEntries().size() == 1;
            double entryFraction = singleEntry ? 1.0
                    : ((parentTput > 0 && callerTput > 0) ? Math.min(1.0, callerTput / parentTput) : 1.0);
            double qFraction = (parentTask != null && parentTask.getScheduling() != SchedStrategy.REF
                    && entryQlenFraction.containsKey(entry.getName()))
                    ? entryQlenFraction.get(entry.getName())
                    : entryFraction;
            double q = qFraction * taskQ;
            double t = entryFraction * parentTput;
            // RespT: prefer Little's law (Q/X). Fallback walks the sync-call chain
            // summing per-call residuals — matches the legacy resolveEntryRespTime.
            double resp = (t > 0 && Double.isFinite(q) && q >= 0)
                    ? q / t
                    : QueueLengthSolver.resolveTaskResponseTime(model, parentTaskName,
                            st.taskResidT, taskCalledTask, new HashSet<String>());

            names.add(entry.getName());
            types.add("Entry");
            qlen.add(q);
            util.add(Double.NaN);
            respT.add(resp);
            residT.add(Double.NaN);
            arvR.add(Double.NaN);
            tput.add(t);
        }
    }

    private static void appendActivityRows(LayeredNetwork model,
                                           Map<String, Double> entryQlenFraction,
                                           Map<String, Double> entryUtilFraction,
                                           Map<String, String> taskCalledTask,
                                           ResultsState st,
                                           List<String> names, List<String> types,
                                           List<Double> qlen, List<Double> util,
                                           List<Double> respT, List<Double> residT,
                                           List<Double> arvR, List<Double> tput) {
        for (Activity act : model.getActivities().values()) {
            String parentTaskName = (act.getParent() != null) ? act.getParent().getName() : null;
            Task parentTask = (act.getParent() != null) ? LqnGraph.findTask(model, parentTaskName) : null;
            boolean isRefActivity = parentTask != null && parentTask.getScheduling() == SchedStrategy.REF;

            // Activity's share of its parent's caller-tput, by its bound entry.
            double actFraction = computeActFraction(act, model, st.taskTput, parentTaskName);

            double q = isRefActivity
                    ? refActivityQLen(act, parentTask, parentTaskName, actFraction,
                                      taskCalledTask, st, model)
                    : nonRefActivityQLen(model, act, parentTaskName, actFraction, entryQlenFraction, st);

            double activityUtil = isRefActivity
                    ? actFraction * computeRefActivityDemandFraction(parentTask, act)
                            * mapGet(st.taskUtil, parentTaskName, 0.0)
                    : nonRefActivityUtil(act, parentTaskName, actFraction, entryUtilFraction, st);

            // Throughput: scale by DAG visit weight (loop bodies fire loopCount
            // times per entry visit, OR_FORK branches fire branch-prob times,
            // etc.). For REF activities the bound activity's weight implicitly
            // accounts for nested loops; for non-REF activities the weight
            // captures per-visit firing rate inside the entry's DAG.
            double actVisitCount = 1.0;
            if (parentTask != null) {
                Map<String, Double> vc = LqnGraph.computeActivityVisitWeights(parentTask);
                Double w = vc.get(act.getName());
                if (w != null && w > 0) actVisitCount = w;
            }
            double t = actFraction * actVisitCount * mapGet(st.taskTput, parentTaskName, 0.0);

            // RespT: prefer Little's law (Q/X). Otherwise fall back, with the
            // "no external sync call" check borrowed from the legacy code.
            double resp;
            if (t > 0 && Double.isFinite(q) && q >= 0) {
                resp = q / t;
            } else if (parentTask != null && !isRefActivity
                    && !taskHasExternalSyncCall(model, parentTaskName)) {
                resp = deriveLocalTaskResid(parentTaskName, st);
            } else {
                resp = mapGet(st.taskResidT, parentTaskName, Double.NaN);
            }

            // ResidT: processor-only component, split proportionally for REF activities.
            double resid;
            if (isRefActivity) {
                double t1ResidT = mapGet(st.taskResidT, parentTaskName, 0.0);
                resid = actFraction * computeRefActivityDemandFraction(parentTask, act) * t1ResidT;
            } else if (parentTask != null
                    && !taskHasExternalSyncCall(model, parentTaskName)) {
                // Leaf non-REF task: split task.ResidT by visit-weighted
                // host-demand share within the task. Pure routing activities
                // (Immediate + no sync calls) report ResidT=0 to match LN.
                double share = activityDemandShare(act, entryUtilFraction);
                Map<Integer, String> syncDestsR = act.getSyncCallDests();
                boolean pureRouting = (syncDestsR == null || syncDestsR.isEmpty())
                        && (Double.isNaN(act.getHostDemandMean()) || act.getHostDemandMean() <= 1e-8);
                if (share > 0) {
                    resid = share * deriveLocalTaskResid(parentTaskName, st);
                } else if (pureRouting) {
                    resid = 0.0;
                } else {
                    String boundER = act.getBoundToEntry();
                    double residFrac = (boundER != null && entryQlenFraction.containsKey(boundER))
                            ? entryQlenFraction.get(boundER) : actFraction;
                    resid = residFrac * deriveLocalTaskResid(parentTaskName, st);
                }
            } else {
                // Non-REF activity in a task that issues external sync calls.
                // Per-cycle processor residence = visit_weight × hostDemand,
                // matching LN's convention where an activity that fires N times
                // per entry visit (POST_LOOP body, OR_FORK branch with prob<1
                // averaged over many calls) reports its total time on the
                // processor per entry visit.
                double hd = act.getHostDemandMean();
                if (Double.isNaN(hd) || hd <= 1e-9) {
                    resid = 0.0;
                } else {
                    resid = actFraction * actVisitCount * hd;
                }
            }

            names.add(act.getName());
            types.add("Activity");
            qlen.add(q);
            util.add(activityUtil);
            respT.add(resp);
            residT.add(resid);
            arvR.add(Double.NaN);
            tput.add(t);
        }
    }


    // =====================================================================
    //  Activity-row helpers
    // =====================================================================

    /** Activity's share of parent-task throughput = bound-entry caller-tput / parent-tput.
     *  For activities not explicitly bound to an entry, falls back to the
     *  entry whose DAG walk reaches them — multi-entry tasks otherwise
     *  default unbound activities to {@code actFraction = 1.0} which inflates
     *  their reported throughput when the task has more than one entry. */
    private static double computeActFraction(Activity act, LayeredNetwork model,
                                             Map<String, Double> taskTput,
                                             String parentTaskName) {
        String boundEntry = act.getBoundToEntry();
        if (boundEntry == null || boundEntry.isEmpty()) {
            // Unbound activity: locate the entry whose DAG reaches it.
            boundEntry = findOwnerEntry(act, act.getParent());
        }
        if (boundEntry == null || boundEntry.isEmpty()) return 1.0;
        Entry boundE = LqnGraph.findEntry(model, boundEntry);
        if (boundE == null) return 1.0;
        // Single-entry tasks: skip the fractional cap (see appendEntryRows).
        Task parent = act.getParent();
        if (parent != null && parent.getEntries() != null && parent.getEntries().size() == 1) {
            return 1.0;
        }
        double pTput = mapGet(taskTput, parentTaskName, 0.0);
        double cTput = computeEntryCallerTput(boundE, model, taskTput);
        if (pTput > 0 && cTput > 0) return Math.min(1.0, cTput / pTput);
        return 1.0;
    }

    /** Name of the entry whose bound activity's DAG reaches {@code act}, or
     *  {@code null} if {@code act} is not reachable from any entry. Used to
     *  attribute unbound activities (sequence steps, OR branches, AND branches,
     *  loop bodies) back to the entry that triggered them. */
    private static String findOwnerEntry(Activity act, Task parent) {
        if (parent == null) return null;
        for (Activity bound : parent.getActivities()) {
            String boundBE = bound.getBoundToEntry();
            if (boundBE == null || boundBE.isEmpty()) continue;
            Map<String, Double> weights = LqnGraph.computeWeightsFromBound(parent, bound);
            Double w = weights.get(act.getName());
            if (w != null && w > 1e-12) return boundBE;
        }
        return null;
    }

    /**
     * REF-task activity QLen = (visit-weighted demand share × T1.proc-Q)
     *                        + Σ over sync calls of (callMean share × callee.Q).
     *
     * The first term is the activity's share of jobs queued at the processor;
     * the second adds jobs queued downstream while waiting for sync responses.
     */
    private static double refActivityQLen(Activity act, Task parentTask, String parentTaskName,
                                          double actFraction,
                                          Map<String, String> taskCalledTask,
                                          ResultsState st, LayeredNetwork model) {
        Double q_p1 = st.refTaskProcQLen.get(parentTaskName);
        if (q_p1 == null || !Double.isFinite(q_p1)) {
            // No P: layer Q — fall back to assigning full task QLen.
            return actFraction * mapGet(st.taskQLen, parentTaskName, 0.0);
        }
        double activityDemandFrac = computeRefActivityDemandFraction(parentTask, act);
        double q_proc = activityDemandFrac * q_p1;

        double q_callee = 0.0;
        if (act.getSyncCallDests() != null) {
            for (Map.Entry<Integer, String> syncCall : act.getSyncCallDests().entrySet()) {
                Entry calledEntry = LqnGraph.findEntry(model, syncCall.getValue());
                if (calledEntry == null || calledEntry.getParent() == null) continue;
                String calledTaskName = calledEntry.getParent().getName();
                if (calledTaskName.equals(parentTaskName)) continue;

                int idx = syncCall.getKey();
                Matrix means = act.getSyncCallMeans();
                double m = 1.0;
                if (means != null && means.getNumCols() > idx) {
                    double mv = means.get(0, idx);
                    if (Double.isFinite(mv) && mv > 0) m = mv;
                }

                // Prefer per-class Q if available (multiclass), else apportion
                // the callee's total Q by call-mean share across all callers.
                Map<String, Double> classQLen = st.taskClassQLen.get(calledTaskName);
                Double classSpecificQ = (classQLen != null) ? classQLen.get(parentTaskName) : null;
                if (classSpecificQ != null && classSpecificQ > 0) {
                    double callerTotalCM = LqnGraph.getCallerTotalCallMean(model, parentTaskName, calledTaskName);
                    q_callee += (m / callerTotalCM) * classSpecificQ;
                } else {
                    double totalCM = LqnGraph.getSumInboundCallMean(model, calledTaskName);
                    Double calleeQLen = st.taskQLen.get(calledTaskName);
                    if (calleeQLen != null && calleeQLen > 0 && totalCM > 0) {
                        q_callee += (m / totalCM) * calleeQLen;
                    }
                }
            }
        }
        return actFraction * (q_proc + q_callee);
    }

    /** Non-REF activity QLen: split the task's Q by this activity's share of
     *  visit-weighted host demand. Equivalent to LN's
     *  {@code Q_act = (X_act × D_act / Σ X_act × D_act) × Q_task} when all
     *  activities share the same throughput, which holds inside a single task.
     *  Pure routing activities (Immediate, no sync calls — e.g. AND/OR fork
     *  heads, loop heads) report Q=0 since they contribute no host work and
     *  no downstream blocking. */
    private static double nonRefActivityQLen(LayeredNetwork model, Activity act, String parentTaskName,
                                             double actFraction,
                                             Map<String, Double> entryQlenFraction,
                                             ResultsState st) {
        double share = activityFullDemandShare(model, act, st, entryQlenFraction);
        if (share > 0) return share * mapGet(st.taskQLen, parentTaskName, 0.0);
        // Pure routing activity (Immediate + no outgoing sync calls): no host
        // work, no downstream blocking → Q=0 to match LN's reporting.
        Map<Integer, String> syncDests = act.getSyncCallDests();
        if ((syncDests == null || syncDests.isEmpty())
                && (Double.isNaN(act.getHostDemandMean()) || act.getHostDemandMean() <= 1e-8)) {
            return 0.0;
        }
        // Activity has no host demand but issues sync calls: it blocks waiting
        // for the callee. Fall back to the entry-fraction split.
        String boundE = act.getBoundToEntry();
        double qFrac = (boundE != null && entryQlenFraction.containsKey(boundE))
                ? entryQlenFraction.get(boundE) : actFraction;
        return qFrac * mapGet(st.taskQLen, parentTaskName, 0.0);
    }

    /** Non-REF activity Util: parent task's processor utilization × this
     *  activity's visit-weighted host-demand share within the task. Captures
     *  OR_FORK / POST_LOOP / sequence weighting so per-activity utilizations
     *  sum to the task total. Zero-demand activities (Immediate) report zero. */
    private static double nonRefActivityUtil(Activity act, String parentTaskName,
                                             double actFraction,
                                             Map<String, Double> entryUtilFraction,
                                             ResultsState st) {
        double hd = act.getHostDemandMean();
        if (Double.isNaN(hd) || hd <= 1e-8) return 0.0;
        double share = activityDemandShare(act, entryUtilFraction);
        if (share > 0) return share * mapGet(st.taskUtil, parentTaskName, 0.0);
        String boundEU = act.getBoundToEntry();
        double utilFrac = (boundEU != null && entryUtilFraction.containsKey(boundEU))
                ? entryUtilFraction.get(boundEU) : actFraction;
        return utilFrac * mapGet(st.taskUtil, parentTaskName, 0.0);
    }

    /** Share of the task's host work this activity is responsible for, used
     *  for Util splits. Per-activity weight = host demand; activities with
     *  zero or NaN host demand contribute nothing. */
    private static double activityDemandShare(Activity act,
                                              Map<String, Double> entryFraction) {
        return activityShareWithinTask(act, entryFraction, OutputTableBuilder::hostDemandWeight);
    }

    /** Share of the task's Q this activity is responsible for, used for QLen
     *  splits. Per-activity weight = host demand + Σ {@code callMean ·
     *  R_per_call(callee)}, matching LN's {@code Q_act = X_act · R_act}
     *  reporting where R_act includes downstream callee response.
     *  Activities whose effective response is zero (Immediate with no sync
     *  call — e.g. AND/OR fork heads, loop heads) contribute nothing. */
    private static double activityFullDemandShare(LayeredNetwork model, Activity act,
                                                  ResultsState st,
                                                  Map<String, Double> entryFraction) {
        return activityShareWithinTask(act, entryFraction,
                a -> activityEffectiveResponse(model, a, st));
    }

    /** Common shape for {@link #activityDemandShare} and
     *  {@link #activityFullDemandShare}: locate the entry whose DAG reaches
     *  {@code act}, compute the visit-weighted share within that entry's DAG
     *  (using {@code weightFn} to score each activity's contribution), then
     *  scale by the entry's fraction of the task's processor work. The
     *  entry-fraction map is per-entry caller-tput × demand weight (see
     *  {@link #computeEntryUtilFractions} / {@link #computeEntryQLenFractions}). */
    private static double activityShareWithinTask(Activity act,
                                                  Map<String, Double> entryFraction,
                                                  java.util.function.ToDoubleFunction<Activity> weightFn) {
        double actWeight = weightFn.applyAsDouble(act);
        if (actWeight <= 1e-12) return 0.0;
        Task parent = act.getParent();
        if (parent == null) return 0.0;

        String ownerEntry = act.getBoundToEntry();
        if (ownerEntry == null || ownerEntry.isEmpty()) {
            ownerEntry = findOwnerEntry(act, parent);
        }
        if (ownerEntry == null || ownerEntry.isEmpty()) return 0.0;

        Activity bound = null;
        for (Activity a : parent.getActivities()) {
            if (ownerEntry.equals(a.getBoundToEntry())) { bound = a; break; }
        }
        if (bound == null) return 0.0;

        Map<String, Double> weights = LqnGraph.computeWeightsFromBound(parent, bound);
        Double w = weights.get(act.getName());
        if (w == null || w <= 1e-12) return 0.0;
        double numer = w * actWeight;
        double denom = 0.0;
        for (Activity other : parent.getActivities()) {
            Double oW = weights.get(other.getName());
            if (oW == null || oW <= 1e-12) continue;
            double oWeight = weightFn.applyAsDouble(other);
            if (oWeight <= 1e-12) continue;
            denom += oW * oWeight;
        }
        if (denom <= 1e-12) return 0.0;
        double withinEntryShare = numer / denom;

        // For single-entry tasks this is 1.0 by construction; for multi-entry
        // tasks it captures how much of the task is dispatched to this entry.
        double entryShare = (entryFraction != null && entryFraction.containsKey(ownerEntry))
                ? entryFraction.get(ownerEntry) : 1.0;
        return entryShare * withinEntryShare;
    }

    /** Host demand mean treated as a non-negative weight: NaN / non-positive
     *  values return 0 so the activity does not contribute. */
    private static double hostDemandWeight(Activity a) {
        double hd = a.getHostDemandMean();
        return (Double.isNaN(hd) || hd <= 1e-8) ? 0.0 : hd;
    }

    /** Per-visit effective service time at this activity: host demand plus,
     *  for each outgoing sync call, {@code callMean × R_per_call(callee)}
     *  derived from {@code taskSojournCache}. Per-call R falls back to the
     *  callee's host demand when no cached sojourn is available (first
     *  iteration or callee has no T: layer). */
    private static double activityEffectiveResponse(LayeredNetwork model, Activity act,
                                                    ResultsState st) {
        double hd = act.getHostDemandMean();
        double r = (Double.isNaN(hd) || hd <= 1e-8) ? 0.0 : hd;
        Map<Integer, String> dests = act.getSyncCallDests();
        if (dests == null || dests.isEmpty()) return r;
        Matrix means = act.getSyncCallMeans();
        for (Map.Entry<Integer, String> ce : dests.entrySet()) {
            int idx = ce.getKey();
            double cm = (means != null && means.getNumCols() > idx) ? means.get(0, idx) : 1.0;
            if (!Double.isFinite(cm) || cm <= 0) continue;
            String destName = ce.getValue();
            Entry destEntry = LqnGraph.findEntry(model, destName);
            if (destEntry == null || destEntry.getParent() == null) continue;
            String calleeTask = destEntry.getParent().getName();
            String callerTask = (act.getParent() != null) ? act.getParent().getName() : null;
            // R_per_call at callee = cached sojourn / per-visit callMean.
            Double sojourn = (st.taskSojournCache != null && callerTask != null)
                    ? st.taskSojournCache.get(calleeTask) : null;
            double rPerCall;
            if (sojourn != null && Double.isFinite(sojourn) && sojourn > 0) {
                double perVisitCM = LqnGraph.getSyncCallMean(model, callerTask, calleeTask);
                rPerCall = (perVisitCM > 0) ? sojourn / perVisitCM : sojourn;
            } else {
                rPerCall = LqnGraph.hostDemandOfEntry(destEntry);
            }
            r += cm * rPerCall;
        }
        return r;
    }


    // =====================================================================
    //  Entry/Activity demand-share helpers
    // =====================================================================

    /**
     * Host-demand-weighted entry-Util split. For each non-REF task, weight its
     * entries by {@code (callerTput × processorDemandOfEntry)} (the processor
     * occupancy share — no downstream contribution, AND_FORK branches summed).
     * Companion to {@link #computeEntryQLenFractions}: utilization splits by
     * processor-occupancy host demand, queue length splits by caller-perceived
     * per-call demand.
     */
    private static Map<String, Double> computeEntryUtilFractions(LayeredNetwork model, ResultsState st) {
        Map<String, Double> fractions = new HashMap<String, Double>();
        for (Task tk : model.getTasks().values()) {
            if (tk.getScheduling() == SchedStrategy.REF) continue;
            List<Entry> taskEntries = tk.getEntries();
            if (taskEntries == null || taskEntries.isEmpty()) continue;
            double denom = 0.0;
            Map<String, Double> numerators = new HashMap<String, Double>();
            for (Entry e : taskEntries) {
                double cTput = computeEntryCallerTput(e, model, st.taskTput);
                // Use the processor-occupancy DAG-walk so multi-activity entries
                // (including AND_FORK branches) get a proportional share of the
                // task's processor occupancy.
                double D = LqnGraph.processorDemandOfEntry(e);
                double num = cTput * D;
                numerators.put(e.getName(), num);
                denom += num;
            }
            for (Entry e : taskEntries) {
                double num = numerators.containsKey(e.getName()) ? numerators.get(e.getName()) : 0.0;
                double frac = (denom > 1e-12) ? num / denom : (1.0 / Math.max(1, taskEntries.size()));
                fractions.put(e.getName(), frac);
            }
        }
        return fractions;
    }

    /**
     * Demand-weighted entry-Q split. For each non-REF task, weight its entries
     * by {@code (callerTput × perCallDemand)} and produce a fraction summing
     * to 1. {@code perCallDemand} is the full per-call cost of an entry: full
     * DAG-walked host demand plus, for each downstream sync call, the per-call
     * response time at the callee (cached per-visit sojourn ÷ per-visit call
     * mean). Using the full demand keeps per-entry Q proportional to
     * {@code X · D_per_call} when the entry blocks for downstream callees,
     * matching LN's {@code Q_entry = X_entry · R_entry} reporting.
     */
    private static Map<String, Double> computeEntryQLenFractions(LayeredNetwork model, ResultsState st) {
        Map<String, Double> fractions = new HashMap<String, Double>();
        for (Task tk : model.getTasks().values()) {
            if (tk.getScheduling() == SchedStrategy.REF) continue;
            List<Entry> taskEntries = tk.getEntries();
            if (taskEntries == null || taskEntries.isEmpty()) continue;
            double denom = 0.0;
            Map<String, Double> numerators = new HashMap<String, Double>();
            for (Entry e : taskEntries) {
                double cTput = computeEntryCallerTput(e, model, st.taskTput);
                double D = perCallDemandAtEntry(model, e, st);
                double num = cTput * D;
                numerators.put(e.getName(), num);
                denom += num;
            }
            for (Entry e : taskEntries) {
                double num = numerators.containsKey(e.getName()) ? numerators.get(e.getName()) : 0.0;
                double frac = (denom > 1e-12) ? num / denom : (1.0 / Math.max(1, taskEntries.size()));
                fractions.put(e.getName(), frac);
            }
        }
        return fractions;
    }

    /**
     * Per-call demand at {@code entry}: bound activity's host demand plus the
     * sum over the bound activity's sync calls of {@code callMean × R_per_call(callee)}.
     * Per-call R at a callee is derived from the cached per-visit sojourn by
     * dividing out the per-visit callMean from the parent task to the callee
     * (the same factor that {@link EnsembleInitialiser#setPerCallerDemand}
     * uses to set the callee's T:-layer demand). Falls back to the callee's
     * own bound-activity host demand when no sojourn is cached yet (first
     * iteration of an unsolved layer).
     */
    private static double perCallDemandAtEntry(LayeredNetwork model, Entry entry, ResultsState st) {
        // Full per-call demand at this entry: walk the DAG from the bound
        // activity to sum visit-weighted host demand across every activity
        // reachable through the entry's precedence chain. Then add the
        // per-call downstream-blocking contribution for each sync call out
        // of the bound activity.
        double d = LqnGraph.hostDemandOfEntry(entry);
        Activity bound = LqnGraph.findBoundActivity(entry);
        if (bound == null) return d;
        Map<Integer, String> dests = bound.getSyncCallDests();
        if (dests == null || dests.isEmpty()) return d;
        Matrix means = bound.getSyncCallMeans();
        String parentTaskName = (entry.getParent() != null) ? entry.getParent().getName() : null;
        for (Map.Entry<Integer, String> ce : dests.entrySet()) {
            Entry destEntry = LqnGraph.findEntry(model, ce.getValue());
            if (destEntry == null || destEntry.getParent() == null) continue;
            String calleeTaskName = destEntry.getParent().getName();
            int idx = ce.getKey();
            double cm = (means != null && means.getNumCols() > idx) ? means.get(0, idx) : 1.0;
            if (!Double.isFinite(cm) || cm <= 0) continue;
            Double sojourn = (st.taskSojournCache != null) ? st.taskSojournCache.get(calleeTaskName) : null;
            double rPerCall;
            if (sojourn != null && Double.isFinite(sojourn) && sojourn > 0 && parentTaskName != null) {
                double perVisitCM = LqnGraph.getSyncCallMean(model, parentTaskName, calleeTaskName);
                rPerCall = (perVisitCM > 0) ? sojourn / perVisitCM : sojourn;
            } else {
                rPerCall = LqnGraph.hostDemandOfEntry(destEntry);
            }
            d += cm * rPerCall;
        }
        return d;
    }

    /**
     * Share of a parent task's visit-weighted demand attributable to this activity.
     * Used to split a REF task's Q, Util, ResidT proportionally across activities.
     *
     * <p>For tasks where every activity is Immediate (D=0) — common in REF
     * tasks that only dispatch sync calls — the visit-weighted share is used
     * instead so the activity still inherits its slice of processor queueing
     * (otherwise an Immediate REF activity's QLen drops the {@code q_p1}
     * contribution and undercounts the cycle time).
     */
    private static double computeRefActivityDemandFraction(Task parentTask, Activity act) {
        Map<String, Double> visitCounts = LqnGraph.computeActivityVisitWeights(parentTask);
        double dTotal = 0.0;
        for (Activity a : parentTask.getActivities()) {
            double vc = visitCounts.containsKey(a.getName()) ? visitCounts.get(a.getName()) : 1.0;
            double hd = a.getHostDemandMean();
            if (!Double.isNaN(hd) && hd > 1e-8) dTotal += vc * hd;
        }
        double vcAct = visitCounts.containsKey(act.getName()) ? visitCounts.get(act.getName()) : 1.0;
        double hdAct = act.getHostDemandMean();
        double dAct = (!Double.isNaN(hdAct) && hdAct > 1e-8) ? vcAct * hdAct : 0.0;
        if (dTotal > 0 && dAct > 0) return dAct / dTotal;

        // All-immediate fallback: share by visit weight so REF activities that
        // make sync calls (but no local host demand) still inherit their slice
        // of the REF processor's queue.
        double vTotal = 0.0;
        for (Activity a : parentTask.getActivities()) {
            double vc = visitCounts.containsKey(a.getName()) ? visitCounts.get(a.getName()) : 1.0;
            if (vc > 1e-12) vTotal += vc;
        }
        if (vTotal > 1e-12 && vcAct > 1e-12) return vcAct / vTotal;
        return 0.0;
    }

    /** Total call rate into {@code entry} =
     *  Σ over callers of {@code caller_tput × visitWeight(callingActivity) × callMean}.
     *  Visit weight captures POST_LOOP / OR_FORK / sequence multiplicity inside
     *  the caller task's DAG, so a loop body that fires 3× per caller visit
     *  contributes 3× its raw call-mean. */
    private static double computeEntryCallerTput(Entry entry, LayeredNetwork model,
                                                 Map<String, Double> taskTput) {
        double total = 0.0;
        for (Task task : model.getTasks().values()) {
            Map<String, Double> visitWeights = LqnGraph.computeActivityVisitWeights(task);
            for (Activity act : task.getActivities()) {
                if (act.getSyncCallDests() == null) continue;
                Double w = visitWeights.get(act.getName());
                if (w == null || w <= 1e-12) continue;
                for (Map.Entry<Integer, String> syncCall : act.getSyncCallDests().entrySet()) {
                    if (!syncCall.getValue().equals(entry.getName())) continue;
                    Double x = taskTput.get(task.getName());
                    if (x == null || x <= 0) continue;
                    int idx = syncCall.getKey();
                    Matrix means = act.getSyncCallMeans();
                    double callMean = 1.0;
                    if (means != null && means.getNumCols() > idx) {
                        double m = means.get(0, idx);
                        if (Double.isFinite(m) && m > 0) callMean = m;
                    }
                    total += x * w * callMean;
                }
            }
        }
        return total;
    }



    // =====================================================================
    //  Misc
    // =====================================================================

    /** True iff any of {@code taskName}'s activities issues a sync call to a
     *  task other than itself. Drives the "use deriveLocalTaskResid fallback"
     *  branch for response/residual when Little's law cannot be applied. */
    private static boolean taskHasExternalSyncCall(LayeredNetwork model, String taskName) {
        Task task = LqnGraph.findTask(model, taskName);
        if (task == null) return false;
        for (Activity act : task.getActivities()) {
            if (act.getSyncCallDests() == null || act.getSyncCallDests().isEmpty()) continue;
            for (String dest : act.getSyncCallDests().values()) {
                Entry e = LqnGraph.findEntry(model, dest);
                if (e == null || e.getParent() == null) continue;
                if (!taskName.equals(e.getParent().getName())) return true;
            }
        }
        return false;
    }

    /** Per-call residual fallback: prefer Q/X if both finite, else the residual map.
     *  {@code st.taskQLen} stores the visit-weighted {@code X · D} (service-only
     *  Q) so {@code Q / X = D}, the host residence per call ignoring queueing.
     *  {@code st.taskResidT} stores the MVA's per-visit response including
     *  queueing, which is not what LN reports for the activity ResidT column. */
    private static double deriveLocalTaskResid(String taskName, ResultsState st) {
        Double q = st.taskQLen.get(taskName);
        Double x = st.taskTput.get(taskName);
        if (q != null && x != null && x > 0 && Double.isFinite(q) && Double.isFinite(x)) {
            return q / x;
        }
        Double r = st.taskResidT.get(taskName);
        return (r != null) ? r : Double.NaN;
    }

    private static double mapGet(Map<String, Double> m, String key, double def) {
        if (key == null) return def;
        Double v = m.get(key);
        return (v != null) ? v : def;
    }
}
