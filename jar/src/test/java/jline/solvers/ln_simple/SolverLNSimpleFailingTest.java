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
    //very slightly off
    public void testScenario6MultiCall() throws Exception {
        // 2-tier, N=60, single synchCall multiplier = 5 (heavy multiplicity)
        assertResultsMatchSolverLN(SingleclassLNExamples.sc6_multi_call());
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
    //  Group 4 — Mixed LD + AMVA dispatch (hybrid topology)
    //  The only test that should produce LD>0 AMVA>0 in its [paths] line.
    //  Per-layer dispatch splits: TS-side layers (4 caller classes, prodN huge)
    //  go to AMVA; TX-side layers (single class R:T1, prodN = 31) stay on LD.
    //
    //  Reference points that PASS: xl_shared{2,3,4,5} (pure shared callee,
    //  no solo branch), xl_asymDemands3 (shared callee, asymmetric demand).
    //  Failure is specifically about mixing LD and AMVA paths in one model.
    // =========================================================================

    /**
     * Four REF callers share a callee TS (high-prodN multi-class layers),
     * AND the first caller has its own private callee TX (low-prodN
     * single-class layers). Per-layer dispatch should split: the layers
     * touching the 4-caller chain go to AMVA; the layers around TX stay on LD.
     */
    @Test
    @Timeout(180)
   @Disabled
    //very wrong result
    public void xl_mixedDispatch_shared4_plus_solo() {
        assertResultsMatchSolverLN(mixedSharedPlusSolo());
    }


    // =========================================================================
    //  Group 5 - Multi-caller shared FCFS bottleneck — exercises AMVA at every sweep
    //  step and tests Z-feedback convergence under contention.
    // =========================================================================

    /**
     * Mix of three things designed to keep MVA on the slow path AND make the
     * outer bounce sweep oscillate:
     *   1. Big multiclass lattice (4 REF callers × N=20 → prodN ≈ 1.9e5) ⇒ AMVA
     *      fires, not LD-MVA. AMVA is bounded but its Schweitzer iteration count
     *      grows with class asymmetry, which we provide via varied thinks/demands.
     *   2. Single-thread FCFS bottleneck (TShared, mult=1, PS). Forces heavy
     *      contention; Z values at the four caller hosts feed back through the
     *      Clients delay and oscillate.
     *   3. A second deep call out of the bottleneck (TShared → TLeaf with
     *      synchCall callMean=2) so each cycle in the bounce sweep has more
     *      coupled-state to settle.
     */
    @Test
    @Timeout(300)
    @Disabled
    // very wrong result
    public void pathological_amva_heavy() {
        LayeredNetwork m = new LayeredNetwork("pathological_amva_heavy");
        Processor PRef    = new Processor(m, "PR", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PShared = new Processor(m, "PSh", 1, SchedStrategy.PS);
        Processor PLeaf   = new Processor(m, "PL", 4, SchedStrategy.PS);

        Task TShared = new Task(m, "TShared", 1, SchedStrategy.FCFS).on(PShared);
        Task TLeaf   = new Task(m, "TLeaf", Integer.MAX_VALUE, SchedStrategy.INF).on(PLeaf);

        Entry ELeaf = new Entry(m, "ELeaf").on(TLeaf);
        Entry[] ES = new Entry[4];
        Task[] R = new Task[4];
        Entry[] ER = new Entry[4];
        for (int i = 0; i < 4; i++) {
            ES[i] = new Entry(m, "ES" + (i + 1)).on(TShared);
            R[i] = new Task(m, "R" + (i + 1), 20, SchedStrategy.REF)
                    .on(PRef).setThinkTime(Exp.fitMean(0.5 + 0.7 * i));
            ER[i] = new Entry(m, "ER" + (i + 1)).on(R[i]);
            new Activity(m, "A" + (i + 1), Immediate.getInstance())
                    .on(R[i]).boundTo(ER[i]).synchCall(ES[i], 1);
            new Activity(m, "AS" + (i + 1), Exp.fitMean(0.4 + 0.3 * i))
                    .on(TShared).boundTo(ES[i]).synchCall(ELeaf, 2).repliesTo(ES[i]);
        }
        new Activity(m, "AL", Exp.fitMean(0.3)).on(TLeaf).boundTo(ELeaf).repliesTo(ELeaf);

        assertResultsMatchSolverLN(m);
    }

}
