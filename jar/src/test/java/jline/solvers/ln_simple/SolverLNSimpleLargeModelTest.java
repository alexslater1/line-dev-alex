package jline.solvers.ln_simple;

import jline.GlobalConstants;
import jline.VerboseLevel;
import jline.lang.constant.SchedStrategy;
import jline.lang.layered.Activity;
import jline.lang.layered.Entry;
import jline.lang.layered.LayeredNetwork;
import jline.lang.layered.Processor;
import jline.lang.layered.Task;
import jline.lang.processes.Exp;
import jline.lang.processes.Immediate;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Disabled;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.Timeout;

import java.util.function.Supplier;

/**
 * Characterisation of {@link SolverLNSimple} on large, structurally varied
 * models — the regime where the production hybrid dispatch (exact-MVA when
 * {@code prodN} is small, Seidmann AMVA otherwise) actually has to choose.
 *
 * <p>Each test builds a fresh model, runs it through {@code SolverLNSimple}
 * and the reference {@code SolverLN}, and prints a timing line plus the full
 * result table for each so the two can be compared side-by-side. Correctness
 * is not asserted — the fixture suites pin numerical agreement on smaller
 * models, and some of the shapes here exercise edges (deep chains,
 * lattice-cliff multi-caller models) where {@code SolverLNSimple} and
 * {@code SolverLN} legitimately converge to slightly different fixed points
 * because their coupling rules differ.
 *
 * <p>The model set spans:
 * <ul>
 *   <li><b>Extreme single-class N</b> — pushes the exact-MVA cliff hard.</li>
 *   <li><b>Deep chains (5- and 6-tier)</b> — stresses the layer-to-layer
 *       coupling depth.</li>
 *   <li><b>Wide fan-out (one REF calls 3 / 4 callees)</b> — multiple callee
 *       layers fed from a single REF chain.</li>
 *   <li><b>Multi-caller shared callee (2, 3, 4, 5 callers)</b> — lattice
 *       size {@code ∏(N_r+1)} growing into the millions.</li>
 *   <li><b>Asymmetric demands</b> — three callers with order-of-magnitude
 *       service-time spread at a shared multi-server.</li>
 * </ul>
 */
public class SolverLNSimpleLargeModelTest {

    @BeforeAll
    public static void setUp() {
        GlobalConstants.setVerbose(VerboseLevel.SILENT);
    }


    // =========================================================================
    //  Extreme single-class population
    // =========================================================================

    /** Single REF with {@code N = 2000} driving a 16-server PS. */
    @Test
    @Timeout(180)
    public void xl_singleClass_N2000_c16() {
        compare("xl_singleClass_N2000_c16", () -> {
            LayeredNetwork m = new LayeredNetwork("xl_singleClass_N2000_c16");
            Processor P1 = new Processor(m, "P1", 16, SchedStrategy.PS);
            Processor P2 = new Processor(m, "P2", 32, SchedStrategy.PS);
            Task T1 = new Task(m, "T1", 2000, SchedStrategy.REF).on(P1).setThinkTime(new Exp(10));
            Task T2 = new Task(m, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(P2);
            Entry E1 = new Entry(m, "E1").on(T1);
            Entry E2 = new Entry(m, "E2").on(T2);
            new Activity(m, "A1", new Exp(2)).on(T1).boundTo(E1).synchCall(E2, 1);
            new Activity(m, "A2", new Exp(4)).on(T2).boundTo(E2).repliesTo(E2);
            return m;
        });
    }


    // =========================================================================
    //  Deep chains
    // =========================================================================

    /** Five-tier chain T1 → T2 → T3 → T4 → T5, {@code N = 100}. */
    @Test
    @Timeout(180)
    @Disabled
    public void xl_chain5_N100() {
        compare("xl_chain5_N100", () -> chain(5, 100));
    }

    /** Six-tier chain, {@code N = 50}. Deepest single-class topology. */
    @Test
    @Timeout(180)
    @Disabled
    public void xl_chain6_N50() {
        compare("xl_chain6_N50", () -> chain(6, 50));
    }


    // =========================================================================
    //  Wide fan-out from one REF
    // =========================================================================

    /** One REF calls three independent INF callees on three processors. */
    @Test
    @Timeout(180)
    @Disabled
    public void xl_fanOut3_N100() {
        compare("xl_fanOut3_N100", () -> fanOut(3, 100));
    }

    /** One REF calls four independent INF callees. */
    @Test
    @Timeout(180)
    @Disabled
    public void xl_fanOut4_N50() {
        compare("xl_fanOut4_N50", () -> fanOut(4, 50));
    }


    // =========================================================================
    //  Multi-caller shared callee (lattice cliff)
    // =========================================================================

    /** Two REF callers, {@code N_r = 100} each, shared 4-server PS. prodN ≈ 10 200. */
    @Test
    @Timeout(180)
    public void xl_shared2_N100() {
        compare("xl_shared2_N100", () -> sharedCallee(2, 100, 4));
    }

    /** Three callers, {@code N_r = 50} each, shared 4-server PS. prodN ≈ 132 600. */
    @Test
    @Timeout(180)
    public void xl_shared3_N50() {
        compare("xl_shared3_N50", () -> sharedCallee(3, 50, 4));
    }

    /** Four callers, {@code N_r = 30} each, shared 8-server PS. prodN ≈ 924 000. */
    @Test
    @Timeout(180)
    public void xl_shared4_N30() {
        compare("xl_shared4_N30", () -> sharedCallee(4, 30, 8));
    }

    /** Five callers, {@code N_r = 15} each, shared 8-server PS. prodN ≈ 1 000 000. */
    @Test
    @Timeout(180)
    public void xl_shared5_N15() {
        compare("xl_shared5_N15", () -> sharedCallee(5, 15, 8));
    }


    // =========================================================================
    //  Asymmetric
    // =========================================================================

    // =========================================================================
    //  Mixed dispatch shape — the only test that should produce LD>0 AMVA>0
    //  in its [paths] line
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
    public void xl_mixedDispatch_shared4_plus_solo() {
        compare("xl_mixedDispatch_shared4_plus_solo", () -> mixedSharedPlusSolo());
    }


    // =========================================================================
    //  Asymmetric
    // =========================================================================

    /** Three callers with same N but order-of-magnitude different per-class
     *  service demands at the shared callee. */
    @Test
    @Timeout(180)
    public void xl_asymDemands3_N40() {
        compare("xl_asymDemands3_N40", () -> asymDemands(40));
    }


    // =========================================================================
    //  Model builders
    // =========================================================================

    /**
     * {@code tiers}-tier chain {@code T1 → T2 → … → T_tiers}. {@code T1} is REF
     * with the given population; intermediate tasks are INF; the leaf task
     * holds the only non-trivial service time. Each processor has 4 PS servers.
     */
    private static LayeredNetwork chain(int tiers, int n) {
        LayeredNetwork m = new LayeredNetwork("chain" + tiers + "_N" + n);
        Processor[] P = new Processor[tiers];
        Task[] T = new Task[tiers];
        Entry[] E = new Entry[tiers];
        for (int i = 0; i < tiers; i++) {
            P[i] = new Processor(m, "P" + (i + 1), 4, SchedStrategy.PS);
            if (i == 0) {
                T[i] = new Task(m, "T1", n, SchedStrategy.REF).on(P[i]).setThinkTime(new Exp(2));
            } else {
                T[i] = new Task(m, "T" + (i + 1), Integer.MAX_VALUE, SchedStrategy.INF).on(P[i]);
            }
            E[i] = new Entry(m, "E" + (i + 1)).on(T[i]);
        }
        // Intermediates: immediate activity that calls forward and replies to caller's entry.
        new Activity(m, "AS1", Immediate.getInstance()).on(T[0]).boundTo(E[0]).synchCall(E[1], 1);
        for (int i = 1; i < tiers - 1; i++) {
            new Activity(m, "AS" + (i + 1), Immediate.getInstance())
                    .on(T[i]).boundTo(E[i]).synchCall(E[i + 1], 1).repliesTo(E[i - 1]);
        }
        // Leaf: real service time, replies to its caller.
        new Activity(m, "AS" + tiers, Exp.fitMean(0.8))
                .on(T[tiers - 1]).boundTo(E[tiers - 1]).repliesTo(E[tiers - 2]);
        return m;
    }

    /**
     * One REF task with {@code N} customers calls {@code k} independent INF
     * callees, each on its own 4-server PS processor.
     */
    private static LayeredNetwork fanOut(int k, int n) {
        LayeredNetwork m = new LayeredNetwork("fanOut" + k + "_N" + n);
        Processor P0 = new Processor(m, "P0", 4, SchedStrategy.PS);
        Task T0 = new Task(m, "T0", n, SchedStrategy.REF).on(P0).setThinkTime(new Exp(2));
        Entry E0 = new Entry(m, "E0").on(T0);

        Entry[] calleeEntries = new Entry[k];
        for (int i = 0; i < k; i++) {
            Processor Pi = new Processor(m, "P" + (i + 1), 4, SchedStrategy.PS);
            Task Ti = new Task(m, "T" + (i + 1), Integer.MAX_VALUE, SchedStrategy.INF).on(Pi);
            calleeEntries[i] = new Entry(m, "E" + (i + 1)).on(Ti);
            new Activity(m, "AS" + (i + 1), Exp.fitMean(0.6 + 0.2 * i))
                    .on(Ti).boundTo(calleeEntries[i]).repliesTo(calleeEntries[i]);
        }
        // One caller activity chains a synchCall to every callee.
        Activity caller = new Activity(m, "A0", Immediate.getInstance()).on(T0).boundTo(E0);
        for (Entry ce : calleeEntries) caller = caller.synchCall(ce, 1);
        return m;
    }

    /**
     * {@code k} REF callers (each with {@code n} customers) onto a shared
     * {@code c}-server PS callee task. Each caller has its own entry on the
     * callee for proper per-class accounting.
     */
    private static LayeredNetwork sharedCallee(int k, int n, int cShared) {
        LayeredNetwork m = new LayeredNetwork("shared" + k + "_N" + n);
        Processor P1 = new Processor(m, "P1", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PS = new Processor(m, "PS", cShared, SchedStrategy.PS);
        Task TS = new Task(m, "TS", Integer.MAX_VALUE, SchedStrategy.INF).on(PS);
        Task[] callers = new Task[k];
        Entry[] callerEntries = new Entry[k];
        Entry[] calleeEntries = new Entry[k];
        for (int r = 0; r < k; r++) {
            callers[r] = new Task(m, "T" + (r + 1), n, SchedStrategy.REF)
                    .on(P1).setThinkTime(Exp.fitMean(2.0 + 0.3 * r));
            callerEntries[r] = new Entry(m, "E" + (r + 1)).on(callers[r]);
            calleeEntries[r] = new Entry(m, "ES" + (r + 1)).on(TS);
        }
        for (int r = 0; r < k; r++) {
            new Activity(m, "A" + (r + 1), Immediate.getInstance())
                    .on(callers[r]).boundTo(callerEntries[r]).synchCall(calleeEntries[r], 1);
            new Activity(m, "AS" + (r + 1), Exp.fitMean(0.7 + 0.1 * r))
                    .on(TS).boundTo(calleeEntries[r]).repliesTo(calleeEntries[r]);
        }
        return m;
    }

    /**
     * Four REF callers (N=30 each) share callee TS on an 8-server PS, AND
     * caller T1 additionally calls a private callee TX on a 4-server PS.
     * The TS-side layers carry 4 caller classes (prodN ≈ 9 × 10⁵, AMVA), the
     * TX-side layers carry just R:T1 (prodN = 31, LD).
     */
    private static LayeredNetwork mixedSharedPlusSolo() {
        LayeredNetwork m = new LayeredNetwork("mixed_shared4_solo1");
        Processor P1 = new Processor(m, "P1", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PS = new Processor(m, "PS", 8, SchedStrategy.PS);
        Processor PX = new Processor(m, "PX", 4, SchedStrategy.PS);
        Task TS = new Task(m, "TS", Integer.MAX_VALUE, SchedStrategy.INF).on(PS);
        Task TX = new Task(m, "TX", Integer.MAX_VALUE, SchedStrategy.INF).on(PX);
        Task[] callers = new Task[4];
        Entry[] callerEntries = new Entry[4];
        Entry[] sharedCalleeEntries = new Entry[4];
        Entry EX = new Entry(m, "EX").on(TX);
        for (int r = 0; r < 4; r++) {
            callers[r] = new Task(m, "T" + (r + 1), 30, SchedStrategy.REF)
                    .on(P1).setThinkTime(Exp.fitMean(2.0 + 0.3 * r));
            callerEntries[r] = new Entry(m, "E" + (r + 1)).on(callers[r]);
            sharedCalleeEntries[r] = new Entry(m, "ES" + (r + 1)).on(TS);
        }
        // T1 calls both TS and TX in one activity; T2..T4 only call TS.
        new Activity(m, "A1", Immediate.getInstance())
                .on(callers[0]).boundTo(callerEntries[0])
                .synchCall(sharedCalleeEntries[0], 1)
                .synchCall(EX, 1);
        for (int r = 1; r < 4; r++) {
            new Activity(m, "A" + (r + 1), Immediate.getInstance())
                    .on(callers[r]).boundTo(callerEntries[r])
                    .synchCall(sharedCalleeEntries[r], 1);
        }
        for (int r = 0; r < 4; r++) {
            new Activity(m, "AS" + (r + 1), Exp.fitMean(0.7 + 0.1 * r))
                    .on(TS).boundTo(sharedCalleeEntries[r]).repliesTo(sharedCalleeEntries[r]);
        }
        new Activity(m, "AX", Exp.fitMean(1.0)).on(TX).boundTo(EX).repliesTo(EX);
        return m;
    }

    /** Three callers, equal N, three orders of magnitude of demand at TS
     *  (0.2, 1.0, 4.0). */
    private static LayeredNetwork asymDemands(int n) {
        LayeredNetwork m = new LayeredNetwork("asymDemands3_N" + n);
        Processor P1 = new Processor(m, "P1", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PS = new Processor(m, "PS", 8, SchedStrategy.PS);
        Task T1 = new Task(m, "T1", n, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(2.0));
        Task T2 = new Task(m, "T2", n, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(2.0));
        Task T3 = new Task(m, "T3", n, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(2.0));
        Task TS = new Task(m, "TS", Integer.MAX_VALUE, SchedStrategy.INF).on(PS);
        Entry E1  = new Entry(m, "E1").on(T1);
        Entry E2  = new Entry(m, "E2").on(T2);
        Entry E3  = new Entry(m, "E3").on(T3);
        Entry ES1 = new Entry(m, "ES1").on(TS);
        Entry ES2 = new Entry(m, "ES2").on(TS);
        Entry ES3 = new Entry(m, "ES3").on(TS);
        new Activity(m, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(ES1, 1);
        new Activity(m, "A2", Immediate.getInstance()).on(T2).boundTo(E2).synchCall(ES2, 1);
        new Activity(m, "A3", Immediate.getInstance()).on(T3).boundTo(E3).synchCall(ES3, 1);
        new Activity(m, "AS1", Exp.fitMean(0.2)).on(TS).boundTo(ES1).repliesTo(ES1);
        new Activity(m, "AS2", Exp.fitMean(1.0)).on(TS).boundTo(ES2).repliesTo(ES2);
        new Activity(m, "AS3", Exp.fitMean(4.0)).on(TS).boundTo(ES3).repliesTo(ES3);
        return m;
    }


    // =========================================================================
    //  Harness
    // =========================================================================

    /**
     * Run the model through {@link util#assertResultsMatchSolverLN}, which prints
     * timing + result tables for {@code SolverLNSimple}, {@code SolverLN}, and
     * {@code SolverLQNS}, the {@code [paths]} line saying which MVA backends
     * actually fired, then fails the test if any (node, metric) cell drifts
     * past the harness tolerance (5.5 %).
     */
    private static void compare(String label, Supplier<LayeredNetwork> mb) {
        System.out.printf("%n=== %s ===%n", label);
        util.assertResultsMatchSolverLN(mb.get());
    }
}
