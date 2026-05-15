package jline.solvers.ln_simple;

import jline.GlobalConstants;
import jline.VerboseLevel;
import jline.lang.constant.SchedStrategy;
import jline.lang.layered.*;
import jline.lang.processes.Exp;
import jline.lang.processes.Immediate;
import jline.solvers.ln_simple.fixtures.SingleclassLNExamples;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Disabled;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.Timeout;

import static jline.solvers.ln_simple.util.*;

public class SolverLNSimpleFailingTest {

    @BeforeAll
    public static void setUp() {
        GlobalConstants.setVerbose(VerboseLevel.SILENT);
    }

    // =========================================================================
    //  Group 2 — All-Exp deep chains / multi-call scenarios (sc* fixtures)
    //  Real Exp service at every activity, INF intermediaries, single-server PS,
    //  higher N. Likely shares Group 1's root cause plus a demand-multiplier
    //  wrinkle on the synchCall edges.
    //
    //  Reference points that PASS: sc1_simple_chain (N=50, no multiplier),
    //  sc7_multicore (N=200 on 4/8 servers).
    // =========================================================================

    @Test
    @Timeout(120)
   @Disabled
    //100 iters / wrong numbers
    public void testScenario2ThreeTier() throws Exception {
        // 3-tier, N=100, intermediate has synchCall(E3, 2) multi-call demand
        assertResultsMatchSolverLN(SingleclassLNExamples.sc2_three_tier());
    }

    @Test
    @Timeout(120)
   @Disabled
    //100 iters / wrong numbers
    public void testScenario10DeepChain() throws Exception {
        // 4-tier, N=80, all Exp activities
        assertResultsMatchSolverLN(SingleclassLNExamples.sc10_deep_chain());
    }


    // =========================================================================
    //  Group 6 — Intermediate-task throughput not scaled by upstream callMean.
    //
    //  When an upstream task's bound activity issues multiple sync calls into
    //  a multi-entry intermediate task, the intermediate's reported task
    //  throughput comes out as the per-caller-cycle rate rather than the
    //  per-call rate at the intermediate.
    //  {@link jline.solvers.ln_simple.results.ResultsCollector} only scales
    //  LEAF tasks by their max inbound callMean (in {@code scaleLeafTaskThroughputs});
    //  intermediates are skipped.
    // =========================================================================

    /**
     * Single REF caller whose bound activity issues two sync calls into two
     * distinct entries of an intermediate task. Each intermediate entry's bound
     * activity makes one sync call to a leaf.
     *
     * <p>Fails with T2.tput reported at the per-T1-cycle rate (T1.tput) rather
     * than the per-T2-call rate (2 × T1.tput), leaving ER1.RespT roughly 50%
     * above SolverLN's value.
     */
    @Test
    @Timeout(60)
    @Disabled
    public void minimal_multiEntryCaller() {
        LayeredNetwork m = new LayeredNetwork("min_multientry_caller");
        Processor PRef = new Processor(m, "PR", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor P2   = new Processor(m, "P2", 1, SchedStrategy.PS);
        Processor PL   = new Processor(m, "PL", 4, SchedStrategy.PS);

        Task T1    = new Task(m, "T1", 5, SchedStrategy.REF)
                .on(PRef).setThinkTime(Exp.fitMean(1.0));
        Task T2    = new Task(m, "T2", 1, SchedStrategy.FCFS).on(P2);
        Task TLeaf = new Task(m, "TLeaf", Integer.MAX_VALUE, SchedStrategy.INF).on(PL);

        Entry ER1   = new Entry(m, "ER1").on(T1);
        Entry ES1   = new Entry(m, "ES1").on(T2);
        Entry ES2   = new Entry(m, "ES2").on(T2);
        Entry ELeaf = new Entry(m, "ELeaf").on(TLeaf);

        // T1's bound activity routes to both T2 entries (one call each per cycle).
        new Activity(m, "A1", Immediate.getInstance()).on(T1).boundTo(ER1)
                .synchCall(ES1, 1).synchCall(ES2, 1);
        new Activity(m, "AS1", Exp.fitMean(0.4)).on(T2).boundTo(ES1)
                .synchCall(ELeaf, 1).repliesTo(ES1);
        new Activity(m, "AS2", Exp.fitMean(0.5)).on(T2).boundTo(ES2)
                .synchCall(ELeaf, 1).repliesTo(ES2);
        new Activity(m, "AL", Exp.fitMean(0.3)).on(TLeaf).boundTo(ELeaf).repliesTo(ELeaf);

        assertResultsMatchSolverLN(m);
    }

}
