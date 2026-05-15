package jline.solvers.ln_simple.results;

import jline.io.Ret;
import jline.lang.ClosedClass;
import jline.lang.Network;
import jline.lang.layered.LayeredNetwork;
import jline.lang.layered.Task;
import jline.lang.nodes.Node;
import jline.lang.nodes.Queue;
import jline.solvers.ln_simple.LqnGraph;
import jline.solvers.ln_simple.mva.MvaInputs;
import jline.util.matrix.Matrix;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Phase 1 of result assembly: run one final MVA on every layer in the converged
 * state and stash the per-layer metrics into a {@link ResultsState}.
 *
 * <p>For a P: host layer:
 * <ul>
 *   <li>Total demand and per-server utilization across all hosted classes.</li>
 *   <li>If the layer was rebuilt as Case B, attribute aggregate Q/X back to the
 *       hosted task (so we can override the T:-layer values later).</li>
 *   <li>Otherwise, per-class metrics for each hosted task: REF tasks save
 *       {@code refTaskProcQLen} + populate their {@code task*} row directly;
 *       non-REF tasks contribute to {@code hostLayerTput / hostLayerResid}.</li>
 * </ul>
 *
 * <p>For a T: task layer:
 * <ul>
 *   <li>Per-task QLen and per-caller-class Q breakdown.</li>
 *   <li>Task utilization (inherited from the processor's per-server util).</li>
 *   <li>Per-call residual: divide by callMean for leaves (T: demand already
 *       embeds callMean for them).</li>
 * </ul>
 */
final class LayerMetrics {

    private LayerMetrics() {}

    static void extract(LayeredNetwork model, List<Network> ensemble,
                        Map<Integer, String> rebuiltHostLayers,
                        Map<String, String> taskCalledTask,
                        ResultsState st) {
        for (int l = 0; l < ensemble.size(); l++) {
            Network layer = ensemble.get(l);
            List<Node> nodes = MvaInputs.getQueueNodes(layer);
            if (nodes.isEmpty()) continue;

            Matrix n = MvaInputs.buildN(layer);
            Matrix z = MvaInputs.buildThinkTimeMatrix(layer);
            Matrix demand = MvaInputs.buildDemandMatrix(nodes, layer);
            Matrix servers = MvaInputs.buildServerMatrix(nodes, n);
            Ret.pfqnMVA res = MvaInputs.callMVA(demand, n, z, servers);
            if (res == null) continue;

            Queue serverQueue = MvaInputs.findNonDelayQueue(layer);
            if (serverQueue == null) continue;
            int serverIdx = indexOf(nodes, serverQueue);

            String serverQueueName = serverQueue.getName();
            String taskName = LqnGraph.stripPrefix(serverQueueName);

            if (serverQueueName.startsWith("P:")) {
                extractHostLayer(model, layer, res, serverIdx, demand, servers,
                        taskName, l, rebuiltHostLayers, st);
            } else if (serverQueueName.startsWith("T:")) {
                extractTaskLayer(model, layer, res, serverIdx, taskName,
                        taskCalledTask, st);
            }
        }
    }

    // ----- P: host layer ------------------------------------------------------

    private static void extractHostLayer(LayeredNetwork model, Network layer,
                                         Ret.pfqnMVA res, int serverIdx,
                                         Matrix demand, Matrix servers,
                                         String procName, int layerIdx,
                                         Map<Integer, String> rebuiltHostLayers,
                                         ResultsState st) {
        List<ClosedClass> hostClasses = MvaInputs.getClosedClasses(layer);
        int R_host = hostClasses.size();
        double c = servers.get(serverIdx, 0);
        boolean isInfProc = LqnGraph.isInfProcessor(model, procName);
        double c_eff = isInfProc ? 1.0 : c;
        st.processorServers.put(procName, c_eff);

        // Total demand and utilization across all hosted classes.
        double totalD = 0.0, totalUtil = 0.0;
        for (int r2 = 0; r2 < R_host; r2++) {
            double x_r = res.X.get(0, r2);
            double d_r = demand.get(serverIdx, r2);
            totalD += d_r;
            if (Double.isFinite(x_r) && x_r > 0 && Double.isFinite(d_r) && c_eff > 0) {
                totalUtil += x_r * d_r / c_eff;
            }
        }
        if (!isInfProc) totalUtil = Math.min(totalUtil, 1.0);
        st.processorDemand.put(procName, totalD);
        st.processorUtil.put(procName, totalUtil);

        if (rebuiltHostLayers != null && rebuiltHostLayers.containsKey(layerIdx)) {
            // Rebuilt Case-B host: per-caller R:* classes are really the hosted
            // task's per-class processing. Attribute aggregate Q/X back to it.
            String hostedName = rebuiltHostLayers.get(layerIdx);
            double qTot = 0.0, xTot = 0.0;
            for (int r2 = 0; r2 < R_host; r2++) {
                double x_r = res.X.get(0, r2);
                double q_r = res.Q.get(serverIdx, r2);
                if (Double.isFinite(x_r) && x_r > 0) xTot += x_r;
                if (Double.isFinite(q_r)) qTot += q_r;
            }
            if (xTot > 0) {
                st.hostLayerTput.put(hostedName, xTot);
                st.hostLayerResid.put(hostedName, qTot / xTot);
            }
        } else {
            // Per-class metrics for non-rebuilt layers. REF tasks get their
            // P:-layer Q stashed in refTaskProcQLen for later activity splitting.
            for (int r2 = 0; r2 < R_host; r2++) {
                double x_r = res.X.get(0, r2);
                double q_r = res.Q.get(serverIdx, r2);
                double resp_r = (x_r > 0) ? (q_r / x_r) : Double.NaN;
                String hostedTaskName = LqnGraph.stripPrefix(hostClasses.get(r2).getName());
                Task task = LqnGraph.findTask(model, hostedTaskName);
                if (task == null) continue;
                if (LqnGraph.isRefTask(task)) {
                    st.refTaskProcQLen.put(task.getName(), q_r);
                    st.taskUtil.put(task.getName(), totalUtil);
                    st.taskRespT.put(task.getName(), Double.NaN);
                    st.taskResidT.put(task.getName(), resp_r);
                    st.taskTput.put(task.getName(), x_r);
                } else if (Double.isFinite(x_r) && x_r > 0) {
                    st.hostLayerTput.put(task.getName(), x_r);
                    if (Double.isFinite(resp_r) && resp_r > 0) {
                        st.hostLayerResid.put(task.getName(), resp_r);
                    }
                }
            }
        }
    }

    // ----- T: task layer ------------------------------------------------------

    private static void extractTaskLayer(LayeredNetwork model, Network layer,
                                         Ret.pfqnMVA res, int serverIdx,
                                         String taskName,
                                         Map<String, String> taskCalledTask,
                                         ResultsState st) {
        // Sum across caller classes: task-level X/Q/U are aggregates over all
        // calls into this task. Taking just class 0 ignores R:R2..R:Rn for
        // multi-caller task layers (e.g., a shared task called by 4 REF tasks).
        List<ClosedClass> tLayerClasses = MvaInputs.getClosedClasses(layer);
        int R_task = tLayerClasses.size();
        Queue serverQueue = MvaInputs.findNonDelayQueue(layer);
        double x = 0.0, q_mva = 0.0, q_service = 0.0;
        for (int rc = 0; rc < R_task; rc++) {
            double x_rc = res.X.get(0, rc);
            double q_rc = res.Q.get(serverIdx, rc);
            if (Double.isFinite(x_rc)) x += x_rc;
            if (Double.isFinite(q_rc)) q_mva += q_rc;
            double d_rc = (serverQueue != null)
                    ? serverQueue.getServiceProcess(tLayerClasses.get(rc)).getMean()
                    : Double.NaN;
            if (Double.isFinite(x_rc) && Double.isFinite(d_rc)) {
                q_service += x_rc * d_rc;
            }
        }
        // Two distinct Q semantics:
        //   q_display: visit-weighted time at the task (= Σ X_r · D_r). Reported
        //     on the task row. Matches LN's per-task QLen, which excludes
        //     customers physically waiting at upstream hosts. Used only when at
        //     least one class contributes positive demand — otherwise fall back
        //     to the raw MVA Q (degenerate D ≈ 0 cases).
        //   q_mva: total customers at the queue station from MVA. Used to derive
        //     the per-call response time that feeds upstream propagation
        //     (taskResidT) — it must include layer queueing wait.
        double q_display = q_service > 0 ? q_service : q_mva;
        double u = res.U.get(serverIdx, 0);
        double r = (x > 0) ? (q_mva / x) : Double.NaN;

        Task currentTask = LqnGraph.findTask(model, taskName);
        boolean isRef = LqnGraph.isRefTask(currentTask);

        if (!isRef) {
            st.taskQLen.put(taskName, q_display);
            // Per-caller-class Q so multiclass attribution is correct.
            Map<String, Double> perCallerQ = new HashMap<String, Double>();
            for (int rc = 0; rc < R_task; rc++) {
                String callerName = LqnGraph.stripPrefix(tLayerClasses.get(rc).getName());
                perCallerQ.put(callerName, res.Q.get(serverIdx, rc));
            }
            st.taskClassQLen.put(taskName, perCallerQ);
        }

        // Task utilization = per-server util of its processor (already set in
        // Phase-1 P: branch); fall back to the T: layer's own U if absent.
        String procForTask = (currentTask != null && currentTask.getProcessor() != null)
                ? currentTask.getProcessor().getName() : null;
        double u_task = (procForTask != null && st.processorUtil.containsKey(procForTask))
                ? st.processorUtil.get(procForTask) : u;
        st.taskUtil.put(taskName, u_task);

        st.taskRespT.put(taskName, isRef ? r : Double.NaN);

        if (!isRef) {
            double taskHostDemand = sumTaskHostDemand(currentTask);
            // Immediate tasks (zero host demand): residual is 0, not the callee
            // response that coupling shoved into the server-queue service rate.
            double residToStore = taskHostDemand <= 1e-7 ? 0.0 : r;
            // Leaf tasks (no sync callees): T:-layer demand = callMean × hostDemand,
            // so r = callMean × R_per_call. resolveTaskResponseTime multiplies by
            // callMean again — pre-divide here to store per-call residual.
            if (taskHostDemand > 1e-7 && !taskCalledTask.containsKey(taskName)) {
                double cm = LqnGraph.getTotalInboundCallMean(model, taskName);
                if (cm > 1.0 + 1e-9) residToStore = residToStore / cm;
            }
            st.taskResidT.put(taskName, residToStore);
        }
        st.taskTput.put(taskName, x);
    }

    // ----- helpers ------------------------------------------------------------

    /** Sum of {@code hostDemandMean} across all activities of {@code task}. Used
     *  as the "is this an immediate task?" test (returns 0 iff so). */
    private static double sumTaskHostDemand(Task task) {
        if (task == null) return 0.0;
        double total = 0.0;
        for (jline.lang.layered.Activity act : task.getActivities()) {
            double m = act.getHostDemandMean();
            if (!Double.isNaN(m) && Double.isFinite(m)) total += m;
        }
        return total;
    }

    private static int indexOf(List<Node> nodes, Queue target) {
        for (int i = 0; i < nodes.size(); i++) {
            if (nodes.get(i) == target) return i;
        }
        return -1;
    }
}
