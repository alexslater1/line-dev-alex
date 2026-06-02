package jline.solvers.ln_simple.results;

import jline.VerboseLevel;
import jline.lang.Network;
import jline.lang.constant.SchedStrategy;
import jline.lang.layered.LayeredNetwork;
import jline.lang.layered.Task;
import jline.solvers.LayeredNetworkAvgTable;
import jline.solvers.SolverOptions;
import jline.solvers.ln_simple.LqnGraph;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Public entry point for final-result assembly after
 * {@link jline.solvers.ln_simple.SolverLNSimple} has converged.
 *
 * <p>Orchestrates five phases over a shared {@link ResultsState}:
 * <ol>
 *   <li>{@link LayerMetrics#extract} — one final MVA per layer, populate raw metrics.</li>
 *   <li><b>Cross-layer corrections</b> (this file): override per-call residuals
 *       with host-layer values; prefer host-layer throughput where the
 *       processor has real demand; re-derive processor utilization for
 *       asymmetric multiclass models.</li>
 *   <li>{@link QueueLengthSolver#run} — Little's-law-based Q derivation,
 *       with REF inheritance run before and after the non-REF chain walk.</li>
 *   <li><b>Leaf throughput scaling</b> (this file) — multi-call leaves report
 *       throughput per <i>call</i>, not per caller-visit, matching SolverLN.</li>
 *   <li>{@link OutputTableBuilder#build} — emit
 *       Processor → Task → Entry → Activity rows.</li>
 * </ol>
 */
public final class ResultsCollector {

    private ResultsCollector() {}

    /**
     * Build and print the final {@link LayeredNetworkAvgTable}.
     *
     * @param taskSojournCache   per-callee server-only sojourn cached during
     *                           iteration (referenced by REF inheritance, but
     *                           currently passed through unchanged for legacy
     *                           parity — left in the signature in case future
     *                           phases need it).
     * @param rebuiltHostLayers  layer-index → hosted-task-name for Case-B
     *                           rebuilt P: layers.
     */
    public static LayeredNetworkAvgTable collectAndPrintFinalResults(
            LayeredNetwork model, List<Network> ensemble, int nLayers,
            Map<String, Double> taskSojournCache,
            Map<Integer, String> rebuiltHostLayers) {

        ResultsState st = new ResultsState();
        Map<String, String> taskCalledTask = LqnGraph.buildTaskCalledTaskMap(model);
        if (taskSojournCache != null) st.taskSojournCache = taskSojournCache;

        // Phase 1
        LayerMetrics.extract(model, ensemble, rebuiltHostLayers, taskCalledTask, st);

        // Phase 2
        applyCrossLayerCorrections(model, taskCalledTask, st);

        // Phase 3
        QueueLengthSolver.run(model, taskCalledTask, taskSojournCache, st);

        // Phase 4
        scaleLeafTaskThroughputs(model, taskCalledTask, st);

        // Phase 5
        LayeredNetworkAvgTable table = OutputTableBuilder.build(model, taskCalledTask, st);
        SolverOptions opts = new SolverOptions();
        opts.verbose = VerboseLevel.STD;
        table.print(opts, true);
        return table;
    }


    // =====================================================================
    //  Phase 2 — cross-layer corrections
    // =====================================================================

    /**
     * Four small post-Phase-1 fixups, each addressing a specific systematic bias
     * in the raw per-layer values:
     * <ol>
     *   <li><b>Residual override</b> — for non-leaf non-REF tasks, replace
     *       {@code taskResidT} with {@code hostLayerResid} so the chain walker
     *       does not double-count callee response. The T:-layer residual
     *       already embeds callee response; the P:-layer residual is
     *       {@code D_local} only.</li>
     *   <li><b>Throughput override</b> — prefer {@code hostLayerTput} when the
     *       host has real demand or the task has Z &gt; 0. Zero-think immediate
     *       intermediates keep their T:-layer throughput (the host layer is
     *       unconstrained in that case).</li>
     *   <li><b>Single-task processor alignment</b> — for processors hosting one
     *       non-REF task, align Util(P) with X·D/c using the final task tput.</li>
     *   <li><b>Asymmetric-multiclass util re-derivation</b> —
     *       {@code Util = Σ_c (X_c · D_c) / c_servers}. Needed when callers
     *       route to different entries with different host demands on the same
     *       task. Skip if hosted task has Z &gt; 0 (it paces itself correctly).</li>
     * </ol>
     */
    private static void applyCrossLayerCorrections(LayeredNetwork model,
                                                   Map<String, String> taskCalledTask,
                                                   ResultsState st) {
        // (2a) Residual override
        for (Task task : model.getTasks().values()) {
            if (LqnGraph.isRefTask(task)) continue;
            boolean isLeaf = !taskCalledTask.containsKey(task.getName());
            if (isLeaf) continue;
            if (!st.hostLayerResid.containsKey(task.getName())) continue;
            st.taskResidT.put(task.getName(), st.hostLayerResid.get(task.getName()));
        }

        // (2b) Throughput override
        Map<String, String> taskNameToProc = new HashMap<String, String>();
        for (Task task : model.getTasks().values()) {
            if (task.getProcessor() != null) {
                taskNameToProc.put(task.getName(), task.getProcessor().getName());
            }
        }
        for (Map.Entry<String, Double> e : st.hostLayerTput.entrySet()) {
            String tName = e.getKey();
            // Zero-think intermediate tasks: T:-layer X is the real call-arrival
            // rate when saturated; the unconstrained P: layer would over-estimate.
            // Intermediate tasks WITH Z>0 pace themselves; P:-layer X is correct.
            if (taskCalledTask.containsKey(tName)) {
                Task t2 = LqnGraph.findTask(model, tName);
                double think2 = (t2 != null && Double.isFinite(t2.getThinkTimeMean()))
                        ? t2.getThinkTimeMean() : 0.0;
                if (think2 <= 1e-9) continue;
            }
            String procName = taskNameToProc.get(tName);
            Double d = (procName != null) ? st.processorDemand.get(procName) : null;
            Task t = LqnGraph.findTask(model, tName);
            double think = (t != null && Double.isFinite(t.getThinkTimeMean()))
                    ? t.getThinkTimeMean() : 0.0;
            if ((d != null && d > 0) || think > 1e-9) {
                st.taskTput.put(tName, e.getValue());
            }
        }

        // (2c) Single-task processor alignment
        Map<String, List<Task>> procToNonRefTasks = new HashMap<String, List<Task>>();
        for (Task task : model.getTasks().values()) {
            if (task.getScheduling() == SchedStrategy.REF || task.getProcessor() == null) continue;
            String procName = task.getProcessor().getName();
            List<Task> hosted = procToNonRefTasks.get(procName);
            if (hosted == null) {
                hosted = new ArrayList<Task>();
                procToNonRefTasks.put(procName, hosted);
            }
            hosted.add(task);
        }
        for (Map.Entry<String, List<Task>> e : procToNonRefTasks.entrySet()) {
            if (e.getValue().size() != 1) continue;
            Task hostedTask = e.getValue().get(0);
            // For multi-class workloads (asymmetric demand per caller) we must
            // sum X_r · D_r per class, not aggregate tx · d_total. The latter
            // double-counts when callers split throughput unevenly. (2d) does
            // this correctly for finite-server processors; for INF processors
            // (skipped by (2d)) we replicate the per-class sum here.
            String procName = e.getKey();
            boolean isInfProc = LqnGraph.isInfProcessor(model, procName);
            Double c = st.processorServers.get(procName);
            if (c == null || c <= 0) continue;
            if (isInfProc) {
                double utilSum = 0.0;
                boolean hasCaller = false;
                for (Task caller : model.getTasks().values()) {
                    double dr = LqnGraph.callerDemandOnTask(model, caller.getName(), hostedTask.getName());
                    if (dr <= 0) continue;
                    Double xCaller = st.taskTput.get(caller.getName());
                    if (xCaller == null || xCaller <= 0) continue;
                    utilSum += xCaller * dr / c;
                    hasCaller = true;
                }
                if (hasCaller) {
                    st.processorUtil.put(procName, utilSum);
                    st.taskUtil.put(hostedTask.getName(), utilSum);
                }
                continue;
            }
            Double tx = st.taskTput.get(hostedTask.getName());
            Double d = st.processorDemand.get(procName);
            if (tx != null && tx > 0 && d != null) {
                double newUtil = Math.min(tx * d / c, 1.0);
                st.processorUtil.put(procName, newUtil);
                st.taskUtil.put(hostedTask.getName(), newUtil);
            }
        }

        // (2d) Asymmetric-multiclass util re-derivation
        for (Task hostedTask : model.getTasks().values()) {
            if (LqnGraph.isRefTask(hostedTask) || hostedTask.getProcessor() == null) continue;
            double hostedThink = hostedTask.getThinkTimeMean();
            if (!Double.isNaN(hostedThink) && Double.isFinite(hostedThink) && hostedThink > 1e-9) continue;
            String procName = hostedTask.getProcessor().getName();
            if (LqnGraph.isInfProcessor(model, procName)) continue;
            Double c = st.processorServers.get(procName);
            if (c == null || c <= 0) continue;
            double utilSum = 0.0;
            double demandSum = 0.0;
            boolean hasCaller = false;
            for (Task caller : model.getTasks().values()) {
                double d = LqnGraph.callerDemandOnTask(model, caller.getName(), hostedTask.getName());
                if (d <= 0) continue;
                Double xCaller = st.taskTput.get(caller.getName());
                if (xCaller == null || xCaller <= 0) continue;
                utilSum += xCaller * d / c;
                demandSum += xCaller * d;
                hasCaller = true;
            }
            if (hasCaller) {
                double utilCapped = Math.min(utilSum, 1.0);
                st.processorUtil.put(procName, utilCapped);
                st.taskUtil.put(hostedTask.getName(), utilCapped);
                // (2e) Mean per-call processor residence = X-weighted mean of per-class
                // demand = Σ(X_r · D_r) / Σ X_r. Replaces hostLayerResid (which is the
                // population-weighted aggregate D — wrong when X_r and D_r covary).
                // Only override for non-leaf tasks: leaves already have a per-call
                // residual stored by LayerMetrics (it divides by inbound callMean).
                boolean isLeaf = !taskCalledTask.containsKey(hostedTask.getName());
                if (!isLeaf) {
                    Double xTotal = st.taskTput.get(hostedTask.getName());
                    if (xTotal != null && xTotal > 0 && demandSum > 0) {
                        st.taskResidT.put(hostedTask.getName(), demandSum / xTotal);
                    }
                }
            }
        }
    }


    // =====================================================================
    //  Phase 4 — leaf throughput scaling
    // =====================================================================

    /**
     * Multi-call leaves report throughput per <i>call</i>, not per caller-visit.
     * The T: layer ran with demand = {@code callMean × hostDemand} and stored
     * caller-throughput as {@code taskTput}; SolverLN multiplies by the max
     * inbound call mean to get per-call rate.
     *
     * <p>Must happen <i>after</i> Phase 3, because Phase 3 used the caller-rate
     * X (with the embedded callMean factor) to compute Q.
     */
    private static void scaleLeafTaskThroughputs(LayeredNetwork model,
                                                 Map<String, String> taskCalledTask,
                                                 ResultsState st) {
        for (Task task : model.getTasks().values()) {
            if (LqnGraph.isRefTask(task)) continue;
            if (taskCalledTask.containsKey(task.getName())) continue;  // not a leaf
            String tName = task.getName();
            double maxCM = LqnGraph.getTotalInboundCallMean(model, tName);
            if (maxCM <= 1.0 + 1e-9) continue;
            Double tx = st.taskTput.get(tName);
            if (tx != null && tx > 0) {
                st.taskTput.put(tName, tx * maxCM);
            }
        }
    }
}
