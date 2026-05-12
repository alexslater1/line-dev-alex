package jline.solvers.ln_simple.results;

import jline.lang.layered.Activity;
import jline.lang.layered.LayeredNetwork;
import jline.lang.layered.Task;
import jline.solvers.ln_simple.LqnGraph;

import java.util.HashSet;
import java.util.Map;
import java.util.Set;

/**
 * Phase 3 of result assembly: derive task queue lengths from throughputs and
 * residual times, with REF-task inheritance applied twice (before and after the
 * non-REF chain walk).
 *
 * <h2>The dance</h2>
 * <ol>
 *   <li>{@link #applyRefTaskInheritance} once — REF tasks get throughput (from
 *       their host layer) and Q via Little's law on the think station
 *       ({@code Q = N − X·Z}). Non-REF Q is still unknown.</li>
 *   <li>For every non-REF task, set {@code Q = X · R_eff} where {@code R_eff}
 *       walks the sync-call chain summing per-call residuals.</li>
 *   <li>{@link #applyRefTaskInheritance} again — now REF tasks can pull from
 *       the just-computed non-REF callee Q (handled by the second call's
 *       inheritance path; the formulae themselves are unchanged).</li>
 * </ol>
 */
final class QueueLengthSolver {

    private QueueLengthSolver() {}

    /** Run all of Phase 3 on {@code st}. */
    static void run(LayeredNetwork model,
                    Map<String, String> taskCalledTask,
                    Map<String, Double> taskSojournCache,
                    ResultsState st) {
        applyRefTaskInheritance(model, taskCalledTask, taskSojournCache, st);

        for (Task task : model.getTasks().values()) {
            if (LqnGraph.isRefTask(task)) continue;
            Double x = st.taskTput.get(task.getName());
            if (x == null || x <= 0) continue;
            double effR;
            boolean isLeaf = !taskCalledTask.containsKey(task.getName());
            if (isLeaf && st.hostLayerResid.containsKey(task.getName())) {
                effR = st.hostLayerResid.get(task.getName());
            } else {
                effR = resolveTaskResponseTime(model, task.getName(),
                        st.taskResidT, taskCalledTask, new HashSet<String>());
            }
            if (!Double.isNaN(effR) && effR >= 0) {
                st.taskQLen.put(task.getName(), x * effR);
            }
        }

        applyRefTaskInheritance(model, taskCalledTask, taskSojournCache, st);
    }


    /**
     * Populate REF task entries in {@code taskTput} / {@code taskQLen} /
     * {@code taskResidT}.
     *
     * <p>Two cases:
     * <ul>
     *   <li><b>REF task with no sync callee</b> (self-contained): scale its
     *       processor-Q by the bound-entry's share of total visit-weighted
     *       demand. Matches SolverLN's loop convention (entry response counts
     *       only the bound activity, excluding post-loop exit).</li>
     *   <li><b>REF task with a callee</b>: throughput stays from its host layer
     *       (set in Phase 1); Q from Little's law on the think station,
     *       {@code Q = N − X·Z}. ResidT zeroed unless already populated.</li>
     * </ul>
     */
    private static void applyRefTaskInheritance(LayeredNetwork model,
                                                Map<String, String> taskCalledTask,
                                                Map<String, Double> taskSojournCache,
                                                ResultsState st) {
        Map<String, String> refTaskCalledTask = LqnGraph.buildRefTaskCalledTaskMap(model);

        for (Task task : model.getTasks().values()) {
            if (!LqnGraph.isRefTask(task)) continue;

            String calledTaskName = refTaskCalledTask.get(task.getName());
            if (calledTaskName == null) {
                applyRefSelfContained(task, st);
                continue;
            }

            // Throughput: already set from REF's P: layer (Phase 1). Only fall
            // back to callee's tput if REF has no host layer at all.
            if (!st.taskTput.containsKey(task.getName())) {
                Double calledTput = st.taskTput.get(calledTaskName);
                if (calledTput != null) st.taskTput.put(task.getName(), calledTput);
            }

            // Q via Little's law on the think station: jobs are either thinking
            // (X·Z) or working (Q) at any instant. Exact regardless of structure.
            Double t1Tput = st.taskTput.get(task.getName());
            if (t1Tput != null && t1Tput > 0) {
                double zT1 = task.getThinkTimeMean();
                if (Double.isNaN(zT1)) zT1 = 0.0;
                double nT1 = (double) task.getMultiplicity();
                st.taskQLen.put(task.getName(), Math.max(0.0, nT1 - t1Tput * zT1));
            }

            // ResidT zeroed only if not already set by a non-immediate processor.
            if (!st.taskResidT.containsKey(task.getName())) {
                st.taskResidT.put(task.getName(), 0.0);
            }
        }
    }

    private static void applyRefSelfContained(Task task, ResultsState st) {
        Double q_p1 = st.refTaskProcQLen.get(task.getName());
        if (q_p1 == null || !Double.isFinite(q_p1) || q_p1 <= 0) return;

        Map<String, Double> vc = LqnGraph.computeActivityVisitCounts(task);
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
        st.taskQLen.put(task.getName(), fraction * q_p1);
    }


    /**
     * Recursive chain walker: {@code R_eff = R_local + callMean × R_callee},
     * cycle-safe via {@code visited}. Used to compute the per-task effective
     * response time for Phase 3's {@code Q = X · R_eff}.
     */
    static double resolveTaskResponseTime(LayeredNetwork model, String taskName,
                                          Map<String, Double> taskResidT,
                                          Map<String, String> taskCalledTask,
                                          Set<String> visited) {
        if (taskName == null || !visited.add(taskName)) return Double.NaN;

        Double localResid = taskResidT.get(taskName);
        double response = (localResid != null && Double.isFinite(localResid) && localResid > 0)
                ? localResid : 0.0;

        String calledTaskName = taskCalledTask.get(taskName);
        if (calledTaskName == null || calledTaskName.equals(taskName)) return response;

        double callMean = LqnGraph.getSyncCallMean(model, taskName, calledTaskName);
        if (!Double.isFinite(callMean) || callMean <= 0) callMean = 1.0;

        double calleeResponse = resolveTaskResponseTime(model, calledTaskName,
                taskResidT, taskCalledTask, visited);
        if (Double.isNaN(calleeResponse) || calleeResponse <= 0) return response;

        return response + callMean * calleeResponse;
    }
}
