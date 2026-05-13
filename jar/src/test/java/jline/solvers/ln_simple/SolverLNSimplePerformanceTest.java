package jline.solvers.ln_simple;

import jline.GlobalConstants;
import jline.VerboseLevel;
import jline.lang.constant.SchedStrategy;
import jline.lang.constant.SolverType;
import jline.lang.layered.Activity;
import jline.lang.layered.Entry;
import jline.lang.layered.LayeredNetwork;
import jline.lang.layered.Processor;
import jline.lang.layered.Task;
import jline.lang.processes.Exp;
import jline.lang.processes.Immediate;
import jline.solvers.LayeredNetworkAvgTable;
import jline.solvers.ln.SolverLN;
import jline.solvers.ln_simple.mva.MvaLd;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.Timeout;

import java.util.function.Supplier;

import static org.junit.jupiter.api.Assertions.assertNotNull;

/**
 * Performance characterisation for {@link SolverLNSimple}.
 *
 * <p>Each test builds a fresh {@link LayeredNetwork}, runs it through
 * {@code SolverLNSimple} and through the reference {@code SolverLN}, and prints
 * a timing line plus the full result table for each. The intent is regression
 * tracking and a visual side-by-side comparison — there is no correctness
 * assertion here. The fixture-based suites already pin numerical agreement on
 * smaller models, and SolverLNSimple has known reporting gaps (e.g. activity
 * QLen for multi-caller shared callees, INF processor utilisation) that would
 * trip a tolerance check at these sizes without being a perf concern.
 *
 * <p>Sample output for one case:
 * <pre>
 * === perf_singleClass_N200 ===
 *
 *   [SolverLNSimple]   0.026 s,  4 iters
 *   Node       …
 *   …
 *
 *   [SolverLN      ]   2.626 s, 53 iters
 *   Node       …
 *   …
 * </pre>
 *
 * <p>The {@link Timeout} on each test is a safety net for regressions, not a
 * performance assertion: a healthy build runs the whole class in well under a
 * minute.
 */
public class SolverLNSimplePerformanceTest {

    @BeforeAll
    public static void setUp() {
        GlobalConstants.setVerbose(VerboseLevel.SILENT);
    }


    // =========================================================================
    //  Axis 1 — single-class population at a multi-server processor
    // =========================================================================

    /**
     * Original "takes a long time" debug shape ({@code N = 200} on an 8-server
     * PS) — the case that motivated the {@link MvaLd} rewrite.
     */
    @Test
    @Timeout(120)
    public void perf_singleClass_N200() {
        compare("perf_singleClass_N200", () -> {
            LayeredNetwork m = new LayeredNetwork("perf_singleClass_N200");
            Processor P1 = new Processor(m, "P1", 8, SchedStrategy.PS);
            Processor P2 = new Processor(m, "P2", 16, SchedStrategy.PS);
            Task T1 = new Task(m, "T1", 200, SchedStrategy.REF).on(P1).setThinkTime(new Exp(5));
            Task T2 = new Task(m, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(P2);
            Entry E1 = new Entry(m, "E1").on(T1);
            Entry E2 = new Entry(m, "E2").on(T2);
            new Activity(m, "A1", new Exp(3)).on(T1).boundTo(E1).synchCall(E2, 1);
            new Activity(m, "A2", new Exp(6)).on(T2).boundTo(E2).repliesTo(E2);
            return m;
        });
    }

    /** Push single-class population to {@code N = 1000} to confirm the cost
     *  stays quadratic in {@code N}. */
    @Test
    @Timeout(120)
    public void perf_singleClass_N1000() {
        compare("perf_singleClass_N1000", () -> {
            LayeredNetwork m = new LayeredNetwork("perf_singleClass_N1000");
            Processor P1 = new Processor(m, "P1", 8, SchedStrategy.PS);
            Processor P2 = new Processor(m, "P2", 16, SchedStrategy.PS);
            Task T1 = new Task(m, "T1", 1000, SchedStrategy.REF).on(P1).setThinkTime(new Exp(10));
            Task T2 = new Task(m, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(P2);
            Entry E1 = new Entry(m, "E1").on(T1);
            Entry E2 = new Entry(m, "E2").on(T2);
            new Activity(m, "A1", new Exp(2)).on(T1).boundTo(E1).synchCall(E2, 1);
            new Activity(m, "A2", new Exp(4)).on(T2).boundTo(E2).repliesTo(E2);
            return m;
        });
    }


    // =========================================================================
    //  Axis 2 — number of caller classes at a shared callee (lattice ∏(N_r+1))
    // =========================================================================

    /** Two REF tasks share a 4-server PS callee, {@code N_r = 50}, prodN ≈ 2 600. */
    @Test
    @Timeout(120)
    public void perf_twoCaller_N50_sharedPs4() {
        compare("perf_twoCaller_N50_sharedPs4", () -> {
            LayeredNetwork m = new LayeredNetwork("perf_twoCaller_N50");
            Processor P1 = new Processor(m, "P1", Integer.MAX_VALUE, SchedStrategy.INF);
            Processor PS = new Processor(m, "PS", 4, SchedStrategy.PS);
            Task T1 = new Task(m, "T1", 50, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(2.0));
            Task T2 = new Task(m, "T2", 50, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(2.0));
            Task TS = new Task(m, "TS", Integer.MAX_VALUE, SchedStrategy.INF).on(PS);
            Entry E1 = new Entry(m, "E1").on(T1);
            Entry E2 = new Entry(m, "E2").on(T2);
            Entry ES1 = new Entry(m, "ES1").on(TS);
            Entry ES2 = new Entry(m, "ES2").on(TS);
            new Activity(m, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(ES1, 1);
            new Activity(m, "A2", Immediate.getInstance()).on(T2).boundTo(E2).synchCall(ES2, 1);
            new Activity(m, "AS1", Exp.fitMean(1.0)).on(TS).boundTo(ES1).repliesTo(ES1);
            new Activity(m, "AS2", Exp.fitMean(1.0)).on(TS).boundTo(ES2).repliesTo(ES2);
            return m;
        });
    }

    /** Three REF tasks share a 4-server PS callee, {@code N_r = 20}, prodN ≈ 9 300. */
    @Test
    @Timeout(120)
    public void perf_threeCaller_N20_sharedPs4() {
        compare("perf_threeCaller_N20_sharedPs4", () -> {
            LayeredNetwork m = new LayeredNetwork("perf_threeCaller_N20");
            Processor P1 = new Processor(m, "P1", Integer.MAX_VALUE, SchedStrategy.INF);
            Processor PS = new Processor(m, "PS", 4, SchedStrategy.PS);
            Task T1 = new Task(m, "T1", 20, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(2.0));
            Task T2 = new Task(m, "T2", 20, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(2.5));
            Task T3 = new Task(m, "T3", 20, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(3.0));
            Task TS = new Task(m, "TS", Integer.MAX_VALUE, SchedStrategy.INF).on(PS);
            Entry E1 = new Entry(m, "E1").on(T1);
            Entry E2 = new Entry(m, "E2").on(T2);
            Entry E3 = new Entry(m, "E3").on(T3);
            Entry ES1 = new Entry(m, "ES1").on(TS);
            Entry ES2 = new Entry(m, "ES2").on(TS);
            Entry ES3 = new Entry(m, "ES3").on(TS);
            new Activity(m, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(ES1, 1);
            new Activity(m, "A2", Immediate.getInstance()).on(T2).boundTo(E2).synchCall(ES2, 1);
            new Activity(m, "A3", Immediate.getInstance()).on(T3).boundTo(E3).synchCall(ES3, 1);
            new Activity(m, "AS1", Exp.fitMean(0.8)).on(TS).boundTo(ES1).repliesTo(ES1);
            new Activity(m, "AS2", Exp.fitMean(1.0)).on(TS).boundTo(ES2).repliesTo(ES2);
            new Activity(m, "AS3", Exp.fitMean(1.2)).on(TS).boundTo(ES3).repliesTo(ES3);
            return m;
        });
    }

    /** Four REF tasks share an 8-server PS callee, {@code N_r = 15}, prodN ≈ 65 500.
     *  Roughly where the exact-MVA cost starts to be noticeable on the wall clock —
     *  a useful upper-bound stress test on the current implementation. */
    @Test
    @Timeout(120)
    public void perf_fourCaller_N15_sharedPs8() {
        compare("perf_fourCaller_N15_sharedPs8", () -> {
            LayeredNetwork m = new LayeredNetwork("perf_fourCaller_N15");
            Processor P1 = new Processor(m, "P1", Integer.MAX_VALUE, SchedStrategy.INF);
            Processor PS = new Processor(m, "PS", 8, SchedStrategy.PS);
            Task TS = new Task(m, "TS", Integer.MAX_VALUE, SchedStrategy.INF).on(PS);
            Task[] callers = new Task[4];
            Entry[] callerEntries = new Entry[4];
            Entry[] calleeEntries = new Entry[4];
            for (int r = 0; r < 4; r++) {
                callers[r] = new Task(m, "T" + (r + 1), 15, SchedStrategy.REF)
                        .on(P1).setThinkTime(Exp.fitMean(2.0 + 0.5 * r));
                callerEntries[r] = new Entry(m, "E" + (r + 1)).on(callers[r]);
                calleeEntries[r] = new Entry(m, "ES" + (r + 1)).on(TS);
            }
            for (int r = 0; r < 4; r++) {
                new Activity(m, "A" + (r + 1), Immediate.getInstance())
                        .on(callers[r]).boundTo(callerEntries[r]).synchCall(calleeEntries[r], 1);
                new Activity(m, "AS" + (r + 1), Exp.fitMean(0.7 + 0.1 * r))
                        .on(TS).boundTo(calleeEntries[r]).repliesTo(calleeEntries[r]);
            }
            return m;
        });
    }


    // =========================================================================
    //  Harness
    // =========================================================================

    /**
     * Run {@code SolverLNSimple} and {@code SolverLN} on a fresh copy of the
     * model and print both timings + result tables under a header for the case.
     */
    private static void compare(String label, Supplier<LayeredNetwork> modelBuilder) {
        System.out.printf("%n=== %s ===%n", label);
        runSolverLNSimple(modelBuilder.get());
        runSolverLNReference(modelBuilder.get());
    }

    private static void runSolverLNSimple(LayeredNetwork model) {
        int[] iters = {0};
        SolverLNSimple solver = new SolverLNSimple(model);
        long start = System.currentTimeMillis();
        util.suppressOutput(() -> solver.iterateCoupledMva(100, 1e-4, () -> iters[0]++));
        long elapsedMs = System.currentTimeMillis() - start;
        LayeredNetworkAvgTable table = solver.getAvgTable();
        assertNotNull(table, "SolverLNSimple produced no result table");
        printRun("SolverLNSimple", elapsedMs, iters[0], table);
    }

    /** Inlined {@code SolverLN} runner so its output uses the same column layout
     *  as the {@code SolverLNSimple} row above. */
    private static void runSolverLNReference(LayeredNetwork model) {
        final SolverLN[] holder = new SolverLN[1];
        final LayeredNetworkAvgTable[] tableHolder = new LayeredNetworkAvgTable[1];
        long start = System.currentTimeMillis();
        util.suppressOutput(() -> {
            holder[0] = new SolverLN(model, SolverType.MVA);
            tableHolder[0] = (LayeredNetworkAvgTable) holder[0].getAvgTable();
        });
        long elapsedMs = System.currentTimeMillis() - start;
        int iters = (holder[0] != null && holder[0].maxitererr != null)
                ? holder[0].maxitererr.size() : -1;
        printRun("SolverLN", elapsedMs, iters, tableHolder[0]);
    }

    private static void printRun(String label, long elapsedMs, int iters,
                                 LayeredNetworkAvgTable table) {
        System.out.printf("%n  [%-14s] %7.3f s, %2d iters%n",
                label, elapsedMs / 1000.0, iters);
        if (table != null) {
            System.out.print(util.formatTable(table));
        }
    }
}
