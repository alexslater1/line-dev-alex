package jline.solvers.ln_simple.results;

import java.util.HashMap;
import java.util.Map;

/**
 * Mutable bundle of per-task / per-processor maps passed between result-assembly
 * phases. One {@code ResultsState} is created at the start of
 * {@link ResultsCollector#collectAndPrintFinalResults} and threaded through each
 * phase, which populates and refines its fields.
 *
 * <p>Field-by-field semantics:
 * <ul>
 *   <li>{@code taskQLen / taskUtil / taskTput / taskRespT / taskResidT} — values
 *       that will appear on the Task row of the output table, keyed by task name.</li>
 *   <li>{@code taskClassQLen} — task name → caller name → Q at that task on the
 *       T: layer (preserved per caller so asymmetric multiclass models attribute
 *       correctly).</li>
 *   <li>{@code processorUtil / processorDemand / processorServers} — values for
 *       the Processor row, keyed by processor name.</li>
 *   <li>{@code refTaskProcQLen} — REF task name → Q at the processor in its
 *       P: layer. Used to split REF QLen across activities.</li>
 *   <li>{@code hostLayerTput / hostLayerResid} — task name → values pulled from
 *       the task's P: host layer (used to override T:-layer estimates in Phase 2).</li>
 * </ul>
 */
final class ResultsState {

    // Task-level outputs
    final Map<String, Double> taskQLen   = new HashMap<String, Double>();
    final Map<String, Double> taskUtil   = new HashMap<String, Double>();
    final Map<String, Double> taskTput   = new HashMap<String, Double>();
    final Map<String, Double> taskRespT  = new HashMap<String, Double>();
    final Map<String, Double> taskResidT = new HashMap<String, Double>();

    // Per-caller Q at each task's T: layer
    final Map<String, Map<String, Double>> taskClassQLen = new HashMap<String, Map<String, Double>>();

    // Processor-level outputs
    final Map<String, Double> processorUtil    = new HashMap<String, Double>();
    final Map<String, Double> processorDemand  = new HashMap<String, Double>();
    final Map<String, Double> processorServers = new HashMap<String, Double>();

    // REF task's processor Q (separate from refTask.QLen)
    final Map<String, Double> refTaskProcQLen = new HashMap<String, Double>();

    // Host-layer overrides (Phase 2 may copy these into task* maps)
    final Map<String, Double> hostLayerTput  = new HashMap<String, Double>();
    final Map<String, Double> hostLayerResid = new HashMap<String, Double>();

    // Per-callee server-only sojourn (per caller-visit) cached by SolverLNSimple
    // during iteration. OutputTableBuilder uses this to derive per-call response
    // time at each callee (= sojourn / per-visit callMean) when computing the
    // full per-call demand at each entry.
    Map<String, Double> taskSojournCache = new HashMap<String, Double>();
}
