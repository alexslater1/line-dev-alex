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
            // For non-REF leaf tasks with hostLayerResid, prefer that. With
            // multi-call (callMean > 1) divide by max inbound callMean to get
            // per-call residual.
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
            double entryFraction = (parentTput > 0 && callerTput > 0) ? Math.min(1.0, callerTput / parentTput) : 1.0;
            Task parentTask = LqnGraph.findTask(model, parentTaskName);
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
                    : nonRefActivityQLen(act, parentTaskName, actFraction, entryQlenFraction, st);

            double activityUtil = isRefActivity
                    ? actFraction * computeRefActivityDemandFraction(parentTask, act)
                            * mapGet(st.taskUtil, parentTaskName, 0.0)
                    : nonRefActivityUtil(act, parentTaskName, actFraction, entryUtilFraction, st);

            // Throughput: loop bodies on REF tasks fire visitCount times per visit.
            double actVisitCount = 1.0;
            if (isRefActivity) {
                Map<String, Double> vc = LqnGraph.computeActivityVisitCounts(parentTask);
                actVisitCount = vc.containsKey(act.getName()) ? vc.get(act.getName()) : 1.0;
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
                String boundER = act.getBoundToEntry();
                double residFrac = (boundER != null && entryQlenFraction.containsKey(boundER))
                        ? entryQlenFraction.get(boundER) : actFraction;
                resid = residFrac * deriveLocalTaskResid(parentTaskName, st);
            } else {
                // Non-REF activity in a task that issues external sync calls.
                // Residence at the processor is just the host-demand portion, scaled
                // by this activity's share of the parent's visit rate. Equivalent to
                // util_AS / X_parent = X_AS · hostDemand_AS / X_parent (LN's
                // per-call processor residence convention).
                double hd = act.getHostDemandMean();
                if (Double.isNaN(hd) || hd <= 1e-9) {
                    resid = 0.0;
                } else {
                    resid = actFraction * hd;
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

    /** Activity's share of parent-task throughput = bound-entry caller-tput / parent-tput. */
    private static double computeActFraction(Activity act, LayeredNetwork model,
                                             Map<String, Double> taskTput,
                                             String parentTaskName) {
        String boundEntry = act.getBoundToEntry();
        if (boundEntry == null) return 1.0;
        Entry boundE = LqnGraph.findEntry(model, boundEntry);
        if (boundE == null) return 1.0;
        double pTput = mapGet(taskTput, parentTaskName, 0.0);
        double cTput = computeEntryCallerTput(boundE, model, taskTput);
        if (pTput > 0 && cTput > 0) return Math.min(1.0, cTput / pTput);
        return 1.0;
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

    /** Non-REF activity QLen: split parent's Q by the demand-weighted entry fraction. */
    private static double nonRefActivityQLen(Activity act, String parentTaskName,
                                             double actFraction,
                                             Map<String, Double> entryQlenFraction,
                                             ResultsState st) {
        String boundE = act.getBoundToEntry();
        double qFrac = (boundE != null && entryQlenFraction.containsKey(boundE))
                ? entryQlenFraction.get(boundE) : actFraction;
        return qFrac * mapGet(st.taskQLen, parentTaskName, 0.0);
    }

    /** Non-REF activity Util: parent task's processor utilization × this
     *  activity's host-demand share among the task's entries. Uses the
     *  host-demand-weighted entry fraction (no downstream contribution) so
     *  the share matches the processor-only utilization split.
     *  Zero-demand activities (immediate) contribute zero. */
    private static double nonRefActivityUtil(Activity act, String parentTaskName,
                                             double actFraction,
                                             Map<String, Double> entryUtilFraction,
                                             ResultsState st) {
        double hd = act.getHostDemandMean();
        if (Double.isNaN(hd) || hd <= 1e-8) return 0.0;
        String boundEU = act.getBoundToEntry();
        double utilFrac = (boundEU != null && entryUtilFraction.containsKey(boundEU))
                ? entryUtilFraction.get(boundEU) : actFraction;
        return utilFrac * mapGet(st.taskUtil, parentTaskName, 0.0);
    }


    // =====================================================================
    //  Entry/Activity demand-share helpers
    // =====================================================================

    /**
     * Demand-weighted entry-Q split. For each non-REF task, weight its entries
     * by {@code (callerTput × perCallDemand)} and produce a fraction summing to 1.
     *
     * <p>{@code perCallDemand} is the full per-call cost of an entry: the bound
     * activity's host demand plus, for each downstream sync call, the per-call
     * response time at the callee (= cached per-visit sojourn ÷ per-visit
     * callMean). Using the full demand — not just the bound activity's host
     * demand — keeps per-entry Q proportional to {@code X · D_per_call} when
     * the entry blocks for downstream callees, which is the basis of LN's
     * {@code Q_entry = X_entry · R_entry} reporting.
     */
    /**
     * Host-demand-weighted entry-Util split. For each non-REF task, weight its
     * entries by {@code (callerTput × hostDemandOfBoundActivity)} (the processor
     * occupancy share — no downstream contribution). Companion to
     * {@link #computeEntryQLenFractions}: utilization splits by host-demand
     * only, queue length splits by full per-call demand.
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
                double D = getEntryHostDemand(e);
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
        double d = getEntryHostDemand(entry);
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
                rPerCall = LqnGraph.hostDemandOfBoundActivity(destEntry);
            }
            d += cm * rPerCall;
        }
        return d;
    }

    /**
     * Share of a parent task's visit-weighted demand attributable to this activity.
     * Used to split a REF task's Q, Util, ResidT proportionally across activities.
     * Returns 0 if either the activity or the parent has no positive demand.
     */
    private static double computeRefActivityDemandFraction(Task parentTask, Activity act) {
        Map<String, Double> visitCounts = LqnGraph.computeActivityVisitCounts(parentTask);
        double dTotal = 0.0;
        for (Activity a : parentTask.getActivities()) {
            double vc = visitCounts.containsKey(a.getName()) ? visitCounts.get(a.getName()) : 1.0;
            double hd = a.getHostDemandMean();
            if (!Double.isNaN(hd) && hd > 1e-8) dTotal += vc * hd;
        }
        double vcAct = visitCounts.containsKey(act.getName()) ? visitCounts.get(act.getName()) : 1.0;
        double hdAct = act.getHostDemandMean();
        double dAct = (!Double.isNaN(hdAct) && hdAct > 1e-8) ? vcAct * hdAct : 0.0;
        return (dTotal > 0 && dAct > 0) ? dAct / dTotal : 0.0;
    }

    /** Total call rate into {@code entry} = Σ over callers of caller_tput × callMean. */
    private static double computeEntryCallerTput(Entry entry, LayeredNetwork model,
                                                 Map<String, Double> taskTput) {
        double total = 0.0;
        for (Task task : model.getTasks().values()) {
            for (Activity act : task.getActivities()) {
                if (act.getSyncCallDests() == null) continue;
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
                    total += x * callMean;
                }
            }
        }
        return total;
    }

    /** Host demand of the activity bound to {@code entry}. 0 if missing / NaN. */
    private static double getEntryHostDemand(Entry entry) {
        if (entry.getParent() == null) return 0.0;
        for (Activity act : entry.getParent().getActivities()) {
            if (entry.getName().equals(act.getBoundToEntry())) {
                double hd = act.getHostDemandMean();
                return (Double.isNaN(hd) || hd < 0) ? 0.0 : hd;
            }
        }
        return 0.0;
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

    /** Per-call residual fallback: prefer Q/X if both finite, else the residual map. */
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
