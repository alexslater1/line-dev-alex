package jline.solvers.ln_simple;

import jline.lang.layered.Entry;
import jline.lang.layered.LayeredNetwork;
import jline.solvers.ln_simple.fixtures.DagFeatureFixtures;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * Diagnostic unit tests for {@link LqnGraph#hostDemandOfEntry} and
 * {@link LqnGraph#processorDemandOfEntry} on the Phase C reply fixtures.
 *
 * <p>These tests check the demand methods in isolation — independent of the
 * full SolverLNSimple pipeline — so a failure here localises Phase C bugs to
 * the reply phase-1 walker. If these pass but {@code SolverLNSimpleDagTest}
 * still fails, the bug is downstream (caller-layer wiring, results
 * collection, etc.).
 */
public class LqnGraphPhase1Test {

    private static final double TOL = 1e-9;

    private static Entry entryOf(LayeredNetwork model, String name) {
        Entry e = LqnGraph.findEntry(model, name);
        assertNotNull(e, "expected entry " + name);
        return e;
    }

    // =========================================================================
    //  Reply registration — make sure the fixtures actually wire replies up.
    // =========================================================================

    @Test
    public void replyFirstPhase_entry_has_reply_registered() {
        LayeredNetwork m = DagFeatureFixtures.D_replyFirstPhase();
        Entry e2 = entryOf(m, "E2");
        assertTrue(e2.getReplyActivity().values().contains("A_reply"),
                "E2 reply registration: " + e2.getReplyActivity());
    }

    @Test
    public void replyAtBound_bound_activity_is_the_reply() {
        LayeredNetwork m = DagFeatureFixtures.D_replyAtBound();
        Entry e2 = entryOf(m, "E2");
        assertTrue(e2.getReplyActivity().values().contains("A_bound"),
                "E2 reply registration: " + e2.getReplyActivity());
    }

    @Test
    public void combinedDag_reply_is_a_join() {
        LayeredNetwork m = DagFeatureFixtures.D_combinedDag();
        Entry e2 = entryOf(m, "E2");
        assertTrue(e2.getReplyActivity().values().contains("A_join"),
                "E2 reply registration: " + e2.getReplyActivity());
    }

    // =========================================================================
    //  Phase-1 (caller-perceived) demand
    // =========================================================================

    @Test
    public void replyFirstPhase_caller_sees_phase1_only() {
        LayeredNetwork m = DagFeatureFixtures.D_replyFirstPhase();
        Entry e2 = entryOf(m, "E2");
        // Phase 1 = A_p1a + A_reply = 0.4 + 0.2 = 0.6
        assertEquals(0.6, LqnGraph.hostDemandOfEntry(e2), TOL);
    }

    @Test
    public void replyAtBound_caller_sees_only_bound_activity() {
        LayeredNetwork m = DagFeatureFixtures.D_replyAtBound();
        Entry e2 = entryOf(m, "E2");
        // Phase 1 = A_bound only = 0.2
        assertEquals(0.2, LqnGraph.hostDemandOfEntry(e2), TOL);
    }

    @Test
    public void combinedDag_caller_sees_phase1_with_andfork_max() {
        LayeredNetwork m = DagFeatureFixtures.D_combinedDag();
        Entry e2 = entryOf(m, "E2");
        // Phase 1 activities (everything up to and including A_join):
        //   A_init       D=0.1  w=1    -> 0.1
        //   A_loop_body  D=0.2  w=2    -> 0.4
        //   A_or_x       D=0.3  w=0.7  -> 0.21
        //   A_or_y       D=0.1  w=0.3  -> 0.03
        //   A_and_p1     D=0.4  w=1    -> 0.4  (collapses to E[max])
        //   A_and_p2     D=0.4  w=1    -> 0.4  (collapses to E[max])
        //   A_join       D=0.1  w=1    -> 0.1
        //   AND-fork correction: E[max(exp(0.4),exp(0.4))] = 0.6; sum=0.8 -> -0.2
        // Phase-1 demand = 0.1 + 0.4 + 0.21 + 0.03 + 0.4 + 0.4 + 0.1 - 0.2 = 1.44
        assertEquals(1.44, LqnGraph.hostDemandOfEntry(e2), 1e-6);
    }

    // =========================================================================
    //  Processor (full DAG) demand — should still include phase 2.
    // =========================================================================

    @Test
    public void replyFirstPhase_processor_sees_full_sequence() {
        LayeredNetwork m = DagFeatureFixtures.D_replyFirstPhase();
        Entry e2 = entryOf(m, "E2");
        // Full DAG = 0.4 + 0.2 + 0.3 = 0.9 (no AND-fork, so processorDemand = sum)
        assertEquals(0.9, LqnGraph.processorDemandOfEntry(e2), TOL);
    }

    @Test
    public void replyAtBound_processor_sees_full_chain() {
        LayeredNetwork m = DagFeatureFixtures.D_replyAtBound();
        Entry e2 = entryOf(m, "E2");
        // Full DAG = 0.2 + 0.4 + 0.5 = 1.1
        assertEquals(1.1, LqnGraph.processorDemandOfEntry(e2), TOL);
    }

    @Test
    public void combinedDag_processor_sees_phase1_plus_phase2_summed() {
        LayeredNetwork m = DagFeatureFixtures.D_combinedDag();
        Entry e2 = entryOf(m, "E2");
        // Processor sum = A_init(0.1) + 2*A_loop_body(0.4) + 0.7*A_or_x(0.21)
        //   + 0.3*A_or_y(0.03) + A_and_p1(0.4) + A_and_p2(0.4) + A_join(0.1)
        //   + A_phase2(0.3) (background; processor sees it)
        // = 0.1 + 0.4 + 0.21 + 0.03 + 0.4 + 0.4 + 0.1 + 0.3 = 1.94
        assertEquals(1.94, LqnGraph.processorDemandOfEntry(e2), 1e-6);
    }

    // =========================================================================
    //  Sanity: a fixture WITHOUT phase-2 (reply at end of chain) should behave
    //  identically under both semantics.
    // =========================================================================

    @Test
    public void seqEntry_no_phase_split_caller_equals_processor() {
        LayeredNetwork m = DagFeatureFixtures.D_seqEntry();
        Entry e2 = entryOf(m, "E2");
        // Reply is at A_seq3 (last activity); phase 1 = full DAG.
        // No AND-fork, so caller == processor == 0.3 + 0.5 + 0.4 = 1.2
        assertEquals(1.2, LqnGraph.hostDemandOfEntry(e2), TOL);
        assertEquals(1.2, LqnGraph.processorDemandOfEntry(e2), TOL);
    }

    @Test
    public void andForkJoin_no_phase_split_processor_sums_branches_caller_collapses() {
        LayeredNetwork m = DagFeatureFixtures.D_andForkJoin();
        Entry e2 = entryOf(m, "E2");
        // Two D=1 branches; reply at A_join (terminal).
        // Caller = E[max(exp(1),exp(1))] = 1.5; processor = 2.0
        assertEquals(1.5, LqnGraph.hostDemandOfEntry(e2), 1e-9);
        assertEquals(2.0, LqnGraph.processorDemandOfEntry(e2), 1e-9);
    }

    // =========================================================================
    //  AND-fork collapsed (E[max], full DAG) and phase-1 sum (no E[max]).
    //  These two flavours separate the AND-fork join-delay gap from the
    //  phase-1/phase-2 split so the overtake formula can use raw phase
    //  totals while the caller-perceived demand keeps E[max] aggregation.
    // =========================================================================

    @Test
    public void combinedDag_andForkCollapsed_includes_phase2_but_uses_emax() {
        LayeredNetwork m = DagFeatureFixtures.D_combinedDag();
        Entry e2 = entryOf(m, "E2");
        // Phase 1 + phase 2 with AND-fork E[max]:
        //   1.44 (phase-1 with E[max]) + 0.30 (A_phase2) = 1.74
        assertEquals(1.74, LqnGraph.andForkCollapsedDemandOfEntry(e2), 1e-6);
    }

    @Test
    public void combinedDag_phase1Sum_is_phase1_without_emax() {
        LayeredNetwork m = DagFeatureFixtures.D_combinedDag();
        Entry e2 = entryOf(m, "E2");
        // Phase 1 only, AND-fork branches SUM (no E[max] collapse):
        //   0.1 + 0.4 + 0.21 + 0.03 + 0.4 + 0.4 + 0.1 = 1.64
        assertEquals(1.64, LqnGraph.phase1SumDemandOfEntry(e2), 1e-6);
    }

    @Test
    public void replyAtBound_no_andfork_collapsed_equals_processor() {
        LayeredNetwork m = DagFeatureFixtures.D_replyAtBound();
        Entry e2 = entryOf(m, "E2");
        // No AND-fork, so collapsed == processor demand (no E[max] to apply).
        assertEquals(0.2, LqnGraph.phase1SumDemandOfEntry(e2), 1e-9);
        assertEquals(1.1, LqnGraph.andForkCollapsedDemandOfEntry(e2), 1e-9);
    }
}
