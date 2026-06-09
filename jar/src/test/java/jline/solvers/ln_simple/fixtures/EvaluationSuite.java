package jline.solvers.ln_simple.fixtures;

import jline.lang.constant.SchedStrategy;
import jline.lang.layered.*;
import jline.lang.processes.Exp;
import jline.lang.processes.Immediate;
import jline.util.matrix.Matrix;

import java.util.Arrays;

/**
 * Evaluation suite of LQN fixtures for systematic comparison of SolverLNSimple
 * against SolverLN and LQNS V5.
 *
 * <p>The suite has three top-level partitions:
 * <ul>
 *   <li><b>Partition A — Core coverage</b>: representative LQN topologies
 *       spanning the project's claimed scope. A1 (single-class chains),
 *       A2 (multi-class shared callees), A3 (multi-server processors),
 *       A4 (mixed topology).</li>
 *   <li><b>Partition B — Scale axis for F24</b>: fixtures of increasing task
 *       count along chain and fan-out shapes; surface F24's per-iteration
 *       cost as a function of task-caller graph size.</li>
 *   <li><b>Partition C — Boundary and stress</b>: C1 (fan-out caller),
 *       C2 (INF-scheduled hosted tasks), C3 (multi-class + multi-entry
 *       interaction), C4 (saturation/threshold boundary cases).</li>
 * </ul>
 *
 * <p>Fixture names follow the spec convention
 * {@code <partition>_<descriptor>_<key variation>}.
 *
 * <p>See {@code report/evaluation-suite-spec.md} and
 * {@code report/evaluation-suite-manifest.md}.
 */
public class EvaluationSuite {

    // =========================================================================
    // Partition A1: Single-class chains
    // Depths {3, 5, 8} × populations {20, 50, 100} = 9 fixtures.
    // Pattern: REF task at tier 1, INF intermediate tasks, leaf with real
    // Exp service demand. Establishes the baseline iteration-count comparison.
    // =========================================================================

    // Trimmed to a depth×population cross (depth sweep {3,5,8} at N=50,
    // population sweep {20,50,100} at depth=5) sharing the d5_N50 corner. The
    // full 3×3 factorial was largely redundant with the B chain scale axis and
    // over-weighted the trivial single-chain case relative to the rest of the
    // corpus.

    /** A1 chain, depth=3, N=50. Baseline iteration-count fixture. */
    public static LayeredNetwork A1_chain_d3_N50() { return chain(3, 50); }

    /** A1 chain, depth=5, N=20. Baseline iteration-count fixture. */
    public static LayeredNetwork A1_chain_d5_N20() { return chain(5, 20); }

    /** A1 chain, depth=5, N=50. Baseline iteration-count fixture. */
    public static LayeredNetwork A1_chain_d5_N50() { return chain(5, 50); }

    /** A1 chain, depth=5, N=100. Baseline iteration-count fixture. */
    public static LayeredNetwork A1_chain_d5_N100() { return chain(5, 100); }

    /** A1 chain, depth=8, N=50. Baseline iteration-count fixture. */
    public static LayeredNetwork A1_chain_d8_N50() { return chain(8, 50); }

    // =========================================================================
    // Partition A2: Multi-class with shared callees
    // Three asymmetry patterns × two populations × two demand ratios = 12.
    //   Cat A: symmetric same-class (mirror staircase Cat A)
    //   Cat B: asymmetric demands (mirror staircase Cat B)
    //   Cat C: asymmetric think times (mirror staircase Cat C)
    // Each fixture: 2 REF callers (each its own class) → entries on a shared
    // single-server PS callee task.
    // =========================================================================

    /** A2 Cat A symmetric, low overall demand, N=20 per class. */
    public static LayeredNetwork A2_multiclassA_2refs_loadLow_N20() {
        return sharedCalleeMC("A2_A_loadLow_N20", 2, 20, 20, 2.0, 2.0, 0.5, 0.5);
    }

    /** A2 Cat A symmetric, high overall demand, N=20 per class. */
    public static LayeredNetwork A2_multiclassA_2refs_loadHigh_N20() {
        return sharedCalleeMC("A2_A_loadHigh_N20", 2, 20, 20, 2.0, 2.0, 1.5, 1.5);
    }

    /** A2 Cat A symmetric, low overall demand, N=50 per class. */
    public static LayeredNetwork A2_multiclassA_2refs_loadLow_N50() {
        return sharedCalleeMC("A2_A_loadLow_N50", 2, 50, 50, 2.0, 2.0, 0.5, 0.5);
    }

    /** A2 Cat A symmetric, high overall demand, N=50 per class. */
    public static LayeredNetwork A2_multiclassA_2refs_loadHigh_N50() {
        return sharedCalleeMC("A2_A_loadHigh_N50", 2, 50, 50, 2.0, 2.0, 1.5, 1.5);
    }

    /** A2 Cat B asymmetric demands 3:1, N=20 per class. */
    public static LayeredNetwork A2_multiclassB_2refs_demands_3to1_N20() {
        return sharedCalleeMC("A2_B_3to1_N20", 2, 20, 20, 2.0, 2.0, 1.5, 0.5);
    }

    /** A2 Cat B asymmetric demands 5:1, N=20 per class. */
    public static LayeredNetwork A2_multiclassB_2refs_demands_5to1_N20() {
        return sharedCalleeMC("A2_B_5to1_N20", 2, 20, 20, 2.0, 2.0, 2.5, 0.5);
    }

    /** A2 Cat B asymmetric demands 3:1, N=50 per class. */
    public static LayeredNetwork A2_multiclassB_2refs_demands_3to1_N50() {
        return sharedCalleeMC("A2_B_3to1_N50", 2, 50, 50, 2.0, 2.0, 1.5, 0.5);
    }

    /** A2 Cat B asymmetric demands 5:1, N=50 per class. */
    public static LayeredNetwork A2_multiclassB_2refs_demands_5to1_N50() {
        return sharedCalleeMC("A2_B_5to1_N50", 2, 50, 50, 2.0, 2.0, 2.5, 0.5);
    }

    /** A2 Cat C asymmetric think times 2:1, N=20 per class. */
    public static LayeredNetwork A2_multiclassC_2refs_think_2to1_N20() {
        return sharedCalleeMC("A2_C_2to1_N20", 2, 20, 20, 4.0, 2.0, 1.0, 1.0);
    }

    /** A2 Cat C asymmetric think times 4:1, N=20 per class. */
    public static LayeredNetwork A2_multiclassC_2refs_think_4to1_N20() {
        return sharedCalleeMC("A2_C_4to1_N20", 2, 20, 20, 8.0, 2.0, 1.0, 1.0);
    }

    /** A2 Cat C asymmetric think times 2:1, N=50 per class. */
    public static LayeredNetwork A2_multiclassC_2refs_think_2to1_N50() {
        return sharedCalleeMC("A2_C_2to1_N50", 2, 50, 50, 4.0, 2.0, 1.0, 1.0);
    }

    /** A2 Cat C asymmetric think times 4:1, N=50 per class. */
    public static LayeredNetwork A2_multiclassC_2refs_think_4to1_N50() {
        return sharedCalleeMC("A2_C_4to1_N50", 2, 50, 50, 8.0, 2.0, 1.0, 1.0);
    }

    // =========================================================================
    // Partition A3: Multi-server processors
    // Three multi-server capacities {c=2, c=4, c=8} × three load regimes = 9.
    // Pattern: single REF class with N=10 customers calls a single INF callee
    // task hosted on a c-server PS processor. Think time Z is tuned to put
    // the resulting M/M/c-style queue at low / medium / high utilisation.
    // Exercises AMVA dispatch path (multi-server PS station with N > c).
    // =========================================================================

    /** A3 multi-server c=2, low utilisation (target u≈0.30). */
    public static LayeredNetwork A3_multiserver_c2_loadLow() {
        return multiServerCallee("A3_c2_low", 2, 10, 15.0, 1.0);
    }

    /** A3 multi-server c=2, medium utilisation (target u≈0.60). */
    public static LayeredNetwork A3_multiserver_c2_loadMedium() {
        return multiServerCallee("A3_c2_med", 2, 10, 7.0, 1.0);
    }

    /** A3 multi-server c=2, high utilisation (target u≈0.85). */
    public static LayeredNetwork A3_multiserver_c2_loadHigh() {
        return multiServerCallee("A3_c2_high", 2, 10, 4.5, 1.0);
    }

    /** A3 multi-server c=4, low utilisation (target u≈0.30). */
    public static LayeredNetwork A3_multiserver_c4_loadLow() {
        return multiServerCallee("A3_c4_low", 4, 10, 7.0, 1.0);
    }

    /** A3 multi-server c=4, medium utilisation (target u≈0.60). */
    public static LayeredNetwork A3_multiserver_c4_loadMedium() {
        return multiServerCallee("A3_c4_med", 4, 10, 3.0, 1.0);
    }

    /** A3 multi-server c=4, high utilisation (target u≈0.85). */
    public static LayeredNetwork A3_multiserver_c4_loadHigh() {
        return multiServerCallee("A3_c4_high", 4, 10, 2.0, 1.0);
    }

    /** A3 multi-server c=8, low utilisation (target u≈0.30). */
    public static LayeredNetwork A3_multiserver_c8_loadLow() {
        return multiServerCallee("A3_c8_low", 8, 20, 7.0, 1.0);
    }

    /** A3 multi-server c=8, medium utilisation (target u≈0.60). */
    public static LayeredNetwork A3_multiserver_c8_loadMedium() {
        return multiServerCallee("A3_c8_med", 8, 20, 3.0, 1.0);
    }

    /** A3 multi-server c=8, high utilisation (target u≈0.85). */
    public static LayeredNetwork A3_multiserver_c8_loadHigh() {
        return multiServerCallee("A3_c8_high", 8, 20, 2.0, 1.0);
    }

    // =========================================================================
    // Partition A4: Mixed topology
    // Six fixtures combining multiple features (multi-class + multi-server,
    // multi-class + multi-entry, chain + fan-out, etc.). These are the closest
    // to real-world LQN use and ground the practical claims.
    // =========================================================================

    /** A4: three-tier (web→app→db), 2 classes sharing the middle/db tiers. */
    public static LayeredNetwork A4_threeTier_multiclass() {
        LayeredNetwork m = new LayeredNetwork("A4_threeTier_multiclass");
        Processor PR = new Processor(m, "PR", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PA = new Processor(m, "PA", 4, SchedStrategy.PS);
        Processor PD = new Processor(m, "PD", 2, SchedStrategy.PS);

        Task R1 = new Task(m, "R1", 20, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(2.0));
        Task R2 = new Task(m, "R2", 20, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(3.0));
        Task TA = new Task(m, "TA", Integer.MAX_VALUE, SchedStrategy.INF).on(PA);
        Task TD = new Task(m, "TD", Integer.MAX_VALUE, SchedStrategy.INF).on(PD);

        Entry ER1 = new Entry(m, "ER1").on(R1);
        Entry ER2 = new Entry(m, "ER2").on(R2);
        Entry EA1 = new Entry(m, "EA1").on(TA);
        Entry EA2 = new Entry(m, "EA2").on(TA);
        Entry ED  = new Entry(m, "ED").on(TD);

        new Activity(m, "A_R1", Immediate.getInstance()).on(R1).boundTo(ER1).synchCall(EA1, 1);
        new Activity(m, "A_R2", Immediate.getInstance()).on(R2).boundTo(ER2).synchCall(EA2, 1);
        new Activity(m, "A_A1", Exp.fitMean(0.4)).on(TA).boundTo(EA1).synchCall(ED, 1).repliesTo(EA1);
        new Activity(m, "A_A2", Exp.fitMean(0.6)).on(TA).boundTo(EA2).synchCall(ED, 1).repliesTo(EA2);
        new Activity(m, "A_D",  Exp.fitMean(0.5)).on(TD).boundTo(ED).repliesTo(ED);
        return m;
    }

    /** A4: multi-class callers onto a multi-server callee. */
    public static LayeredNetwork A4_multiclass_multiserver() {
        LayeredNetwork m = new LayeredNetwork("A4_multiclass_multiserver");
        Processor PR = new Processor(m, "PR", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PS = new Processor(m, "PS", 4, SchedStrategy.PS);

        Task R1 = new Task(m, "R1", 30, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(2.0));
        Task R2 = new Task(m, "R2", 30, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(2.5));
        Task TS = new Task(m, "TS", Integer.MAX_VALUE, SchedStrategy.INF).on(PS);

        Entry ER1 = new Entry(m, "ER1").on(R1);
        Entry ER2 = new Entry(m, "ER2").on(R2);
        Entry ES1 = new Entry(m, "ES1").on(TS);
        Entry ES2 = new Entry(m, "ES2").on(TS);

        new Activity(m, "A1", Immediate.getInstance()).on(R1).boundTo(ER1).synchCall(ES1, 1);
        new Activity(m, "A2", Immediate.getInstance()).on(R2).boundTo(ER2).synchCall(ES2, 1);
        new Activity(m, "AS1", Exp.fitMean(1.2)).on(TS).boundTo(ES1).repliesTo(ES1);
        new Activity(m, "AS2", Exp.fitMean(0.8)).on(TS).boundTo(ES2).repliesTo(ES2);
        return m;
    }

    /** A4: multi-class workload with multi-entry callee (each class hits its own entry). */
    public static LayeredNetwork A4_multiclass_multientry() {
        LayeredNetwork m = new LayeredNetwork("A4_multiclass_multientry");
        Processor PR = new Processor(m, "PR", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PS = new Processor(m, "PS", 1, SchedStrategy.PS);

        Task R1 = new Task(m, "R1", 15, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(2.0));
        Task R2 = new Task(m, "R2", 15, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(2.0));
        Task R3 = new Task(m, "R3", 15, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(2.0));
        Task TS = new Task(m, "TS", Integer.MAX_VALUE, SchedStrategy.INF).on(PS);

        Entry ER1 = new Entry(m, "ER1").on(R1);
        Entry ER2 = new Entry(m, "ER2").on(R2);
        Entry ER3 = new Entry(m, "ER3").on(R3);
        Entry ES1 = new Entry(m, "ES1").on(TS);
        Entry ES2 = new Entry(m, "ES2").on(TS);
        Entry ES3 = new Entry(m, "ES3").on(TS);

        new Activity(m, "A1", Immediate.getInstance()).on(R1).boundTo(ER1).synchCall(ES1, 1);
        new Activity(m, "A2", Immediate.getInstance()).on(R2).boundTo(ER2).synchCall(ES2, 1);
        new Activity(m, "A3", Immediate.getInstance()).on(R3).boundTo(ER3).synchCall(ES3, 1);
        new Activity(m, "AS1", Exp.fitMean(0.4)).on(TS).boundTo(ES1).repliesTo(ES1);
        new Activity(m, "AS2", Exp.fitMean(0.6)).on(TS).boundTo(ES2).repliesTo(ES2);
        new Activity(m, "AS3", Exp.fitMean(0.5)).on(TS).boundTo(ES3).repliesTo(ES3);
        return m;
    }

    /** A4: 4-tier multi-class chain with multi-entry T2 (fan-in) and real
     *  Exp demand at every tier. Demand is spread across tiers (not
     *  concentrated at the leaf) and per-tier utilisation kept moderate, so
     *  SolverLN's product-form approximation tracks SolverLQNS closely. */
    public static LayeredNetwork A4_chainPlusFanout() {
        LayeredNetwork m = new LayeredNetwork("A4_multiclass_deepChain");
        Processor PR = new Processor(m, "PR", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor P2 = new Processor(m, "P2", 4, SchedStrategy.PS);
        Processor P3 = new Processor(m, "P3", 4, SchedStrategy.PS);
        Processor P4 = new Processor(m, "P4", 4, SchedStrategy.PS);

        Task R1 = new Task(m, "R1", 15, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(4.0));
        Task R2 = new Task(m, "R2", 15, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(4.0));
        Task T2 = new Task(m, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(P2);
        Task T3 = new Task(m, "T3", Integer.MAX_VALUE, SchedStrategy.INF).on(P3);
        Task T4 = new Task(m, "T4", Integer.MAX_VALUE, SchedStrategy.INF).on(P4);

        Entry ER1 = new Entry(m, "ER1").on(R1);
        Entry ER2 = new Entry(m, "ER2").on(R2);
        Entry E2A = new Entry(m, "E2A").on(T2);
        Entry E2B = new Entry(m, "E2B").on(T2);
        Entry E3  = new Entry(m, "E3").on(T3);
        Entry E4  = new Entry(m, "E4").on(T4);

        new Activity(m, "A_R1", Immediate.getInstance()).on(R1).boundTo(ER1).synchCall(E2A, 1);
        new Activity(m, "A_R2", Immediate.getInstance()).on(R2).boundTo(ER2).synchCall(E2B, 1);
        new Activity(m, "A_2A", Exp.fitMean(0.3)).on(T2).boundTo(E2A).synchCall(E3, 1).repliesTo(E2A);
        new Activity(m, "A_2B", Exp.fitMean(0.4)).on(T2).boundTo(E2B).synchCall(E3, 1).repliesTo(E2B);
        new Activity(m, "A_3",  Exp.fitMean(0.2)).on(T3).boundTo(E3).synchCall(E4, 1).repliesTo(E3);
        new Activity(m, "A_4",  Exp.fitMean(0.3)).on(T4).boundTo(E4).repliesTo(E4);
        return m;
    }

    /** A4: realistic web-app shape — 2 client classes, app tier, shared single-entry DB callee. */
    public static LayeredNetwork A4_webApplication() {
        LayeredNetwork m = new LayeredNetwork("A4_webApplication");
        Processor PC = new Processor(m, "PC", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PApp = new Processor(m, "PApp", 4, SchedStrategy.PS);
        Processor PDB  = new Processor(m, "PDB", Integer.MAX_VALUE, SchedStrategy.INF);

        Task RBrowse = new Task(m, "RBrowse", 15, SchedStrategy.REF).on(PC).setThinkTime(Exp.fitMean(5.0));
        Task RShop   = new Task(m, "RShop",    10, SchedStrategy.REF).on(PC).setThinkTime(Exp.fitMean(4.0));
        Task TApp = new Task(m, "TApp", Integer.MAX_VALUE, SchedStrategy.INF).on(PApp);
        Task TDB  = new Task(m, "TDB",  Integer.MAX_VALUE, SchedStrategy.INF).on(PDB);

        Entry ERB = new Entry(m, "ERB").on(RBrowse);
        Entry ERS = new Entry(m, "ERS").on(RShop);
        Entry EAppB = new Entry(m, "EAppB").on(TApp);
        Entry EAppS = new Entry(m, "EAppS").on(TApp);
        Entry EDB   = new Entry(m, "EDB").on(TDB);

        new Activity(m, "ARB", Immediate.getInstance()).on(RBrowse).boundTo(ERB).synchCall(EAppB, 1);
        new Activity(m, "ARS", Immediate.getInstance()).on(RShop).boundTo(ERS).synchCall(EAppS, 1);
        new Activity(m, "AAppB", Exp.fitMean(0.2)).on(TApp).boundTo(EAppB).synchCall(EDB, 1).repliesTo(EAppB);
        new Activity(m, "AAppS", Exp.fitMean(0.3)).on(TApp).boundTo(EAppS).synchCall(EDB, 1).repliesTo(EAppS);
        new Activity(m, "ADB",   Exp.fitMean(0.5)).on(TDB).boundTo(EDB).repliesTo(EDB);
        return m;
    }

    /** A4: deep chain with multi-server bottleneck at the leaf — lightly loaded. */
    public static LayeredNetwork A4_databaseBackend() {
        LayeredNetwork m = new LayeredNetwork("A4_databaseBackend");
        Processor P1 = new Processor(m, "P1", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor P2 = new Processor(m, "P2", 4, SchedStrategy.PS);
        Processor P3 = new Processor(m, "P3", 4, SchedStrategy.PS);
        Processor P4 = new Processor(m, "P4", 4, SchedStrategy.PS);

        Task T1 = new Task(m, "T1", 20, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(5.0));
        Task T2 = new Task(m, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(P2);
        Task T3 = new Task(m, "T3", Integer.MAX_VALUE, SchedStrategy.INF).on(P3);
        Task T4 = new Task(m, "T4", Integer.MAX_VALUE, SchedStrategy.INF).on(P4);

        Entry E1 = new Entry(m, "E1").on(T1);
        Entry E2 = new Entry(m, "E2").on(T2);
        Entry E3 = new Entry(m, "E3").on(T3);
        Entry E4 = new Entry(m, "E4").on(T4);

        new Activity(m, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);
        new Activity(m, "A2", Immediate.getInstance()).on(T2).boundTo(E2).synchCall(E3, 1).repliesTo(E1);
        new Activity(m, "A3", Immediate.getInstance()).on(T3).boundTo(E3).synchCall(E4, 1).repliesTo(E2);
        new Activity(m, "A4", Exp.fitMean(0.5)).on(T4).boundTo(E4).repliesTo(E3);
        return m;
    }

    // -------------------------------------------------------------------------
    // A4 activity-graph model classes. Step 6 (activity-graph awareness) woven
    // into the realistic-topology subpartition rather than given a partition of
    // its own, exactly as multi-class (Step 3) lives in A2 and multi-server
    // (Step 4) in A3. Each carries one of the four Step-6 capabilities
    // (sequence-past-bound, AND-fork/join, POST_LOOP, reply phase split) inside
    // a realistic shape, on INF/clean hosts so they are positive coverage; the
    // finite-server-host activity-graph boundary lives in C6.
    // -------------------------------------------------------------------------

    /** A4 (activity-graph): realistic three-tier web/app/DB where the app entry
     *  runs a multi-activity sequence (validate -> query DB -> format) past its
     *  bound activity, issuing the DB call mid-sequence. Exercises the Step-6
     *  visit-weight DAG walk reading past the bound activity. */
    public static LayeredNetwork A4_threeTier_sequence() {
        LayeredNetwork m = new LayeredNetwork("A4_threeTier_sequence");
        Processor PC   = new Processor(m, "PC",   Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PApp = new Processor(m, "PApp", 4, SchedStrategy.PS);
        Processor PDB  = new Processor(m, "PDB",  Integer.MAX_VALUE, SchedStrategy.INF);

        Task RClient = new Task(m, "RClient", 20, SchedStrategy.REF).on(PC).setThinkTime(Exp.fitMean(3.0));
        Task TApp = new Task(m, "TApp", Integer.MAX_VALUE, SchedStrategy.INF).on(PApp);
        Task TDB  = new Task(m, "TDB",  Integer.MAX_VALUE, SchedStrategy.INF).on(PDB);

        Entry EC   = new Entry(m, "EC").on(RClient);
        Entry EApp = new Entry(m, "EApp").on(TApp);
        Entry EDB  = new Entry(m, "EDB").on(TDB);

        new Activity(m, "A_client", Immediate.getInstance()).on(RClient).boundTo(EC).synchCall(EApp, 1);

        Activity A_validate = new Activity(m, "A_validate", Exp.fitMean(0.15)).on(TApp).boundTo(EApp);
        Activity A_query    = new Activity(m, "A_query",    Exp.fitMean(0.10)).on(TApp).synchCall(EDB, 1);
        Activity A_format   = new Activity(m, "A_format",   Exp.fitMean(0.10)).on(TApp).repliesTo(EApp);
        TApp.addPrecedence(ActivityPrecedence.Serial(A_validate, A_query, A_format));

        new Activity(m, "A_db", Exp.fitMean(0.4)).on(TDB).boundTo(EDB).repliesTo(EDB);
        return m;
    }

    /** A4 (activity-graph): a service that forks two parallel branches of local
     *  work and joins before replying, on an INF host (branches run on separate
     *  notional servers and do not contend, so the closed-form E[max] join
     *  delay is exact). Realistic "do two things in parallel" shape; exercises
     *  the Step-6 AND-fork / AND-join correction. The finite-server-host
     *  counterpart is the C6 boundary fixture. */
    public static LayeredNetwork A4_parallelService_andFork() {
        LayeredNetwork m = new LayeredNetwork("A4_parallelService_andFork");
        Processor PC   = new Processor(m, "PC",   Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PSvc = new Processor(m, "PSvc", Integer.MAX_VALUE, SchedStrategy.INF);

        Task RClient = new Task(m, "RClient", 20, SchedStrategy.REF).on(PC).setThinkTime(Exp.fitMean(2.0));
        Task TSvc = new Task(m, "TSvc", Integer.MAX_VALUE, SchedStrategy.INF).on(PSvc);

        Entry EC   = new Entry(m, "EC").on(RClient);
        Entry ESvc = new Entry(m, "ESvc").on(TSvc);

        new Activity(m, "A_client", Immediate.getInstance()).on(RClient).boundTo(EC).synchCall(ESvc, 1);

        Activity A_fork = new Activity(m, "A_fork", Immediate.getInstance()).on(TSvc).boundTo(ESvc);
        Activity A_b1   = new Activity(m, "A_b1",   Exp.fitMean(0.6)).on(TSvc);
        Activity A_b2   = new Activity(m, "A_b2",   Exp.fitMean(0.4)).on(TSvc);
        Activity A_join = new Activity(m, "A_join", Immediate.getInstance()).on(TSvc).repliesTo(ESvc);
        TSvc.addPrecedence(ActivityPrecedence.AndFork(A_fork, Arrays.asList(A_b1, A_b2)));
        TSvc.addPrecedence(ActivityPrecedence.AndJoin(Arrays.asList(A_b1, A_b2), A_join));
        return m;
    }

    /** A4 (activity-graph): a client whose service loops over a downstream call
     *  (a realistic retry / batch shape). The loop body issues the cross-task
     *  call, so the Step-6 visit weights must propagate the loop count into the
     *  callee's throughput, not just into local REF demand. */
    public static LayeredNetwork A4_retryLoop() {
        LayeredNetwork m = new LayeredNetwork("A4_retryLoop");
        Processor PC    = new Processor(m, "PC",    Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PSvc  = new Processor(m, "PSvc",  4, SchedStrategy.PS);
        Processor PDown = new Processor(m, "PDown", 4, SchedStrategy.PS);

        Task RClient = new Task(m, "RClient", 15, SchedStrategy.REF).on(PC).setThinkTime(Exp.fitMean(2.0));
        Task TSvc  = new Task(m, "TSvc",  Integer.MAX_VALUE, SchedStrategy.INF).on(PSvc);
        Task TDown = new Task(m, "TDown", Integer.MAX_VALUE, SchedStrategy.INF).on(PDown);

        Entry EC    = new Entry(m, "EC").on(RClient);
        Entry ESvc  = new Entry(m, "ESvc").on(TSvc);
        Entry EDown = new Entry(m, "EDown").on(TDown);

        new Activity(m, "A_client", Immediate.getInstance()).on(RClient).boundTo(EC).synchCall(ESvc, 1);

        new Activity(m, "A_head", Immediate.getInstance()).on(TSvc).boundTo(ESvc);
        new Activity(m, "A_body", Immediate.getInstance()).on(TSvc).synchCall(EDown, 1);
        new Activity(m, "A_end",  Immediate.getInstance()).on(TSvc).repliesTo(ESvc);
        TSvc.addPrecedence(ActivityPrecedence.Loop("A_head",
                Arrays.asList("A_body", "A_end"), Matrix.singleton(3)));

        new Activity(m, "A_down", Exp.fitMean(0.2)).on(TDown).boundTo(EDown).repliesTo(EDown);
        return m;
    }

    /** A4 (activity-graph): a service that replies to its caller and then does
     *  phase-2 background work (e.g. logging) the processor still pays for but
     *  the caller does not wait on. Exercises the Step-6 reply phase-1/phase-2
     *  split and the overtake correction on a single-server PS host. */
    public static LayeredNetwork A4_replyThenLog() {
        LayeredNetwork m = new LayeredNetwork("A4_replyThenLog");
        Processor PC   = new Processor(m, "PC",   Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PSvc = new Processor(m, "PSvc", 1, SchedStrategy.PS);

        Task RClient = new Task(m, "RClient", 15, SchedStrategy.REF).on(PC).setThinkTime(Exp.fitMean(2.0));
        Task TSvc = new Task(m, "TSvc", Integer.MAX_VALUE, SchedStrategy.INF).on(PSvc);

        Entry EC   = new Entry(m, "EC").on(RClient);
        Entry ESvc = new Entry(m, "ESvc").on(TSvc);

        new Activity(m, "A_client", Immediate.getInstance()).on(RClient).boundTo(EC).synchCall(ESvc, 1);

        Activity A_handle = new Activity(m, "A_handle", Exp.fitMean(0.4)).on(TSvc).boundTo(ESvc);
        Activity A_reply  = new Activity(m, "A_reply",  Exp.fitMean(0.2)).on(TSvc).repliesTo(ESvc);
        Activity A_log    = new Activity(m, "A_log",    Exp.fitMean(0.3)).on(TSvc).setPhase(2);
        TSvc.addPrecedence(ActivityPrecedence.Serial(A_handle, A_reply, A_log));
        return m;
    }

    // =========================================================================
    // Partition A5: Activity-graph capability coverage
    // Step 6 (activity-graph awareness) as a positive-coverage model class, peer
    // to A2 (multi-class) and A3 (multi-server) — not a separate top-level
    // partition. Where A4 carries realistic multi-tier models that happen to
    // use activity graphs, A5 is the systematic capability sweep: OR-fork
    // branching (absent from the rest of the corpus), AND-fork E[max] across
    // branch count and structure, multi-entry DAG walks, the reply phase split
    // at the bound activity, the all-features integration DAG, and the
    // multi-class × activity-graph cross. All on INF/clean hosts so they are
    // positive coverage; the finite-server-host AND-fork boundary is C6.
    // =========================================================================

    /** A5: two-branch OR-fork (p=0.7/0.3) into demands 1.0/0.2. Per-call demand
     *  = 0.7·1.0 + 0.3·0.2 = 0.76; exercises fractional visit weights. */
    public static LayeredNetwork A5_orForkUneven() {
        LayeredNetwork m = new LayeredNetwork("A5_orForkUneven");
        Processor PR = new Processor(m, "PR", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PS = new Processor(m, "PS", 4, SchedStrategy.PS);
        Task T1 = new Task(m, "T1", 20, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(2.0));
        Task T2 = new Task(m, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(PS);
        Entry E1 = new Entry(m, "E1").on(T1);
        Entry E2 = new Entry(m, "E2").on(T2);
        new Activity(m, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);
        Activity A_or = new Activity(m, "A_or", Immediate.getInstance()).on(T2).boundTo(E2);
        Activity A_x  = new Activity(m, "A_x",  Exp.fitMean(1.0)).on(T2).repliesTo(E2);
        Activity A_y  = new Activity(m, "A_y",  Exp.fitMean(0.2)).on(T2).repliesTo(E2);
        T2.addPrecedence(ActivityPrecedence.OrFork(A_or, Arrays.asList(A_x, A_y),
                new Matrix(new double[][]{{0.7, 0.3}})));
        return m;
    }

    /** A5: three-branch OR-fork (p=0.5/0.3/0.2). Exercises OR-fork iteration
     *  beyond the two-branch case. */
    public static LayeredNetwork A5_orForkThreeWay() {
        LayeredNetwork m = new LayeredNetwork("A5_orForkThreeWay");
        Processor PR = new Processor(m, "PR", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PS = new Processor(m, "PS", 4, SchedStrategy.PS);
        Task T1 = new Task(m, "T1", 15, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(2.0));
        Task T2 = new Task(m, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(PS);
        Entry E1 = new Entry(m, "E1").on(T1);
        Entry E2 = new Entry(m, "E2").on(T2);
        new Activity(m, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);
        Activity A_or = new Activity(m, "A_or", Immediate.getInstance()).on(T2).boundTo(E2);
        Activity A_b1 = new Activity(m, "A_b1", Exp.fitMean(0.8)).on(T2).repliesTo(E2);
        Activity A_b2 = new Activity(m, "A_b2", Exp.fitMean(0.4)).on(T2).repliesTo(E2);
        Activity A_b3 = new Activity(m, "A_b3", Exp.fitMean(0.2)).on(T2).repliesTo(E2);
        T2.addPrecedence(ActivityPrecedence.OrFork(A_or, Arrays.asList(A_b1, A_b2, A_b3),
                new Matrix(new double[][]{{0.5, 0.3, 0.2}})));
        return m;
    }

    /** A5: three equal AND-fork branches (D=1.0 each) on an INF host. Tests
     *  E[max] beyond two branches: E[max] = D·H_3 = 1.833, processor sum = 3.0. */
    public static LayeredNetwork A5_andForkThreeWay() {
        LayeredNetwork m = new LayeredNetwork("A5_andForkThreeWay");
        Processor PR = new Processor(m, "PR", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PS = new Processor(m, "PS", Integer.MAX_VALUE, SchedStrategy.INF);
        Task T1 = new Task(m, "T1", 15, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(2.0));
        Task T2 = new Task(m, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(PS);
        Entry E1 = new Entry(m, "E1").on(T1);
        Entry E2 = new Entry(m, "E2").on(T2);
        new Activity(m, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);
        Activity A_fork = new Activity(m, "A_fork", Immediate.getInstance()).on(T2).boundTo(E2);
        Activity A_p1 = new Activity(m, "A_p1", Exp.fitMean(1.0)).on(T2);
        Activity A_p2 = new Activity(m, "A_p2", Exp.fitMean(1.0)).on(T2);
        Activity A_p3 = new Activity(m, "A_p3", Exp.fitMean(1.0)).on(T2);
        Activity A_join = new Activity(m, "A_join", Immediate.getInstance()).on(T2).repliesTo(E2);
        T2.addPrecedence(ActivityPrecedence.AndFork(A_fork, Arrays.asList(A_p1, A_p2, A_p3)));
        T2.addPrecedence(ActivityPrecedence.AndJoin(Arrays.asList(A_p1, A_p2, A_p3), A_join));
        return m;
    }

    /** A5: AND-fork whose branches are each a two-activity sequence. Exercises
     *  multi-activity branch collapse (collectBranchDemand walks POST_SEQ within
     *  a branch before the E[max]). */
    public static LayeredNetwork A5_andForkMultiAct() {
        LayeredNetwork m = new LayeredNetwork("A5_andForkMultiAct");
        Processor PR = new Processor(m, "PR", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PS = new Processor(m, "PS", Integer.MAX_VALUE, SchedStrategy.INF);
        Task T1 = new Task(m, "T1", 12, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(2.0));
        Task T2 = new Task(m, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(PS);
        Entry E1 = new Entry(m, "E1").on(T1);
        Entry E2 = new Entry(m, "E2").on(T2);
        new Activity(m, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);
        Activity A_fork = new Activity(m, "A_fork", Immediate.getInstance()).on(T2).boundTo(E2);
        Activity A_p1a  = new Activity(m, "A_p1a",  Exp.fitMean(0.4)).on(T2);
        Activity A_p1b  = new Activity(m, "A_p1b",  Exp.fitMean(0.2)).on(T2);
        Activity A_p2a  = new Activity(m, "A_p2a",  Exp.fitMean(0.3)).on(T2);
        Activity A_p2b  = new Activity(m, "A_p2b",  Exp.fitMean(0.1)).on(T2);
        Activity A_join = new Activity(m, "A_join", Immediate.getInstance()).on(T2).repliesTo(E2);
        T2.addPrecedence(ActivityPrecedence.AndFork(A_fork, Arrays.asList(A_p1a, A_p2a)));
        T2.addPrecedence(ActivityPrecedence.Serial(A_p1a, A_p1b));
        T2.addPrecedence(ActivityPrecedence.Serial(A_p2a, A_p2b));
        T2.addPrecedence(ActivityPrecedence.AndJoin(Arrays.asList(A_p1b, A_p2b), A_join));
        return m;
    }

    /** A5: two-entry callee, each entry a sequence past its bound activity.
     *  Exercises the multi-entry branch of the visit-weight walk (per-entry
     *  walks summed) and per-entry host demand. */
    public static LayeredNetwork A5_seqMultiEntry() {
        LayeredNetwork m = new LayeredNetwork("A5_seqMultiEntry");
        Processor PR = new Processor(m, "PR", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PS = new Processor(m, "PS", 4, SchedStrategy.PS);
        Task T1 = new Task(m, "T1", 15, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(2.0));
        Task T2 = new Task(m, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(PS);
        Entry E1  = new Entry(m, "E1").on(T1);
        Entry E2A = new Entry(m, "E2A").on(T2);
        Entry E2B = new Entry(m, "E2B").on(T2);
        new Activity(m, "A1", Immediate.getInstance()).on(T1).boundTo(E1)
                .synchCall(E2A, 1).synchCall(E2B, 1);
        Activity A_a1 = new Activity(m, "A_a1", Exp.fitMean(0.2)).on(T2).boundTo(E2A);
        Activity A_a2 = new Activity(m, "A_a2", Exp.fitMean(0.4)).on(T2).repliesTo(E2A);
        T2.addPrecedence(ActivityPrecedence.Serial(A_a1, A_a2));
        Activity A_b1 = new Activity(m, "A_b1", Exp.fitMean(0.3)).on(T2).boundTo(E2B);
        Activity A_b2 = new Activity(m, "A_b2", Exp.fitMean(0.5)).on(T2).repliesTo(E2B);
        T2.addPrecedence(ActivityPrecedence.Serial(A_b1, A_b2));
        return m;
    }

    /** A5: REPLY at the bound activity — phase 1 is the bound activity only,
     *  the rest is phase-2 background. Caller-perceived demand drops to the
     *  bound activity's; processor still pays for the full DAG. */
    public static LayeredNetwork A5_replyAtBound() {
        LayeredNetwork m = new LayeredNetwork("A5_replyAtBound");
        Processor PR = new Processor(m, "PR", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PS = new Processor(m, "PS", 1, SchedStrategy.PS);
        Task T1 = new Task(m, "T1", 10, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(2.0));
        Task T2 = new Task(m, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(PS);
        Entry E1 = new Entry(m, "E1").on(T1);
        Entry E2 = new Entry(m, "E2").on(T2);
        new Activity(m, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);
        Activity A_bound = new Activity(m, "A_bound", Exp.fitMean(0.2)).on(T2).boundTo(E2).repliesTo(E2);
        Activity A_bg1   = new Activity(m, "A_bg1",   Exp.fitMean(0.4)).on(T2).setPhase(2);
        Activity A_bg2   = new Activity(m, "A_bg2",   Exp.fitMean(0.5)).on(T2).setPhase(2);
        T2.addPrecedence(ActivityPrecedence.Serial(A_bound, A_bg1, A_bg2));
        return m;
    }

    /** A5: integration DAG — sequence → OR-fork → AND-fork → loop → reply with
     *  phase-2 background, all in one entry on an INF host (where the closed-form
     *  E[max] join is exact). The single fixture that exercises every Step-6
     *  capability at once. */
    public static LayeredNetwork A5_combinedDag() {
        LayeredNetwork m = new LayeredNetwork("A5_combinedDag");
        Processor PR = new Processor(m, "PR", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PS = new Processor(m, "PS", Integer.MAX_VALUE, SchedStrategy.INF);
        Task T1 = new Task(m, "T1", 10, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(2.0));
        Task T2 = new Task(m, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(PS);
        Entry E1 = new Entry(m, "E1").on(T1);
        Entry E2 = new Entry(m, "E2").on(T2);
        new Activity(m, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);
        Activity A_init      = new Activity(m, "A_init",      Exp.fitMean(0.1)).on(T2).boundTo(E2);
        Activity A_loop_head = new Activity(m, "A_loop_head", Immediate.getInstance()).on(T2);
        new Activity(m, "A_loop_body", Exp.fitMean(0.2)).on(T2);
        Activity A_loop_end  = new Activity(m, "A_loop_end",  Immediate.getInstance()).on(T2);
        Activity A_or_x      = new Activity(m, "A_or_x",      Exp.fitMean(0.3)).on(T2);
        Activity A_or_y      = new Activity(m, "A_or_y",      Exp.fitMean(0.1)).on(T2);
        Activity A_after_or  = new Activity(m, "A_after_or",  Immediate.getInstance()).on(T2);
        Activity A_and_p1    = new Activity(m, "A_and_p1",    Exp.fitMean(0.4)).on(T2);
        Activity A_and_p2    = new Activity(m, "A_and_p2",    Exp.fitMean(0.4)).on(T2);
        Activity A_join      = new Activity(m, "A_join",      Exp.fitMean(0.1)).on(T2).repliesTo(E2);
        Activity A_phase2    = new Activity(m, "A_phase2",    Exp.fitMean(0.3)).on(T2).setPhase(2);
        T2.addPrecedence(ActivityPrecedence.Serial(A_init, A_loop_head));
        T2.addPrecedence(ActivityPrecedence.Loop("A_loop_head",
                Arrays.asList("A_loop_body", "A_loop_end"), Matrix.singleton(2)));
        T2.addPrecedence(ActivityPrecedence.OrFork(A_loop_end,
                Arrays.asList(A_or_x, A_or_y), new Matrix(new double[][]{{0.7, 0.3}})));
        T2.addPrecedence(ActivityPrecedence.OrJoin(Arrays.asList(A_or_x, A_or_y), A_after_or));
        T2.addPrecedence(ActivityPrecedence.AndFork(A_after_or, Arrays.asList(A_and_p1, A_and_p2)));
        T2.addPrecedence(ActivityPrecedence.AndJoin(Arrays.asList(A_and_p1, A_and_p2), A_join));
        T2.addPrecedence(ActivityPrecedence.Serial(A_join, A_phase2));
        return m;
    }

    /** A5: multi-class × activity-graph cross — two REF classes each call their
     *  own entry on a shared INF callee, and each entry runs an AND-fork/join.
     *  Exercises per-class visit-weight walks plus the E[max] correction
     *  together (the interaction A2 and the AND-fork fixtures probe separately). */
    public static LayeredNetwork A5_multiclassAndFork() {
        LayeredNetwork m = new LayeredNetwork("A5_multiclassAndFork");
        Processor PR = new Processor(m, "PR", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PS = new Processor(m, "PS", Integer.MAX_VALUE, SchedStrategy.INF);
        Task R1 = new Task(m, "R1", 15, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(2.0));
        Task R2 = new Task(m, "R2", 15, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(2.5));
        Task T2 = new Task(m, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(PS);
        Entry ER1 = new Entry(m, "ER1").on(R1);
        Entry ER2 = new Entry(m, "ER2").on(R2);
        Entry E2a = new Entry(m, "E2a").on(T2);
        Entry E2b = new Entry(m, "E2b").on(T2);
        new Activity(m, "A1", Immediate.getInstance()).on(R1).boundTo(ER1).synchCall(E2a, 1);
        new Activity(m, "A2", Immediate.getInstance()).on(R2).boundTo(ER2).synchCall(E2b, 1);
        Activity Af_a = new Activity(m, "Af_a", Immediate.getInstance()).on(T2).boundTo(E2a);
        Activity Aa1  = new Activity(m, "Aa1",  Exp.fitMean(0.5)).on(T2);
        Activity Aa2  = new Activity(m, "Aa2",  Exp.fitMean(0.5)).on(T2);
        Activity Aj_a = new Activity(m, "Aj_a", Immediate.getInstance()).on(T2).repliesTo(E2a);
        T2.addPrecedence(ActivityPrecedence.AndFork(Af_a, Arrays.asList(Aa1, Aa2)));
        T2.addPrecedence(ActivityPrecedence.AndJoin(Arrays.asList(Aa1, Aa2), Aj_a));
        Activity Af_b = new Activity(m, "Af_b", Immediate.getInstance()).on(T2).boundTo(E2b);
        Activity Ab1  = new Activity(m, "Ab1",  Exp.fitMean(0.4)).on(T2);
        Activity Ab2  = new Activity(m, "Ab2",  Exp.fitMean(0.6)).on(T2);
        Activity Aj_b = new Activity(m, "Aj_b", Immediate.getInstance()).on(T2).repliesTo(E2b);
        T2.addPrecedence(ActivityPrecedence.AndFork(Af_b, Arrays.asList(Ab1, Ab2)));
        T2.addPrecedence(ActivityPrecedence.AndJoin(Arrays.asList(Ab1, Ab2), Aj_b));
        return m;
    }

    // =========================================================================
    // Partition B: Scale axis for F24
    // Task count {5, 10, 20, 40, 60} × shape {chain, fanout} = 10 fixtures.
    // Population N=30 held constant per fixture so runtime differences across
    // fixtures isolate task-caller graph size, not population. Note: spec
    // default is N=50; reduced to N=30 here to keep large fan-out fixtures
    // tractable for SolverLN within reasonable test timeouts (see
    // report/evaluation-suite-divergences.md).
    // =========================================================================

    private static final int B_POP = 30;

    /** B chain, 5 tasks (1 REF + 4 INF), N=30. */
    public static LayeredNetwork B_scaleAxis_tasks5_chain()  { return chain(5, B_POP); }

    /** B chain, 10 tasks, N=30. */
    public static LayeredNetwork B_scaleAxis_tasks10_chain() { return chain(10, B_POP); }

    /** B chain, 20 tasks, N=30. */
    public static LayeredNetwork B_scaleAxis_tasks20_chain() { return chain(20, B_POP); }

    /** B chain, 40 tasks, N=30. */
    public static LayeredNetwork B_scaleAxis_tasks40_chain() { return chain(40, B_POP); }

    /** B chain, 60 tasks, N=30. */
    public static LayeredNetwork B_scaleAxis_tasks60_chain() { return chain(60, B_POP); }

    /** B fan-out, 5 tasks (1 REF + 4 INF callees), N=30. */
    public static LayeredNetwork B_scaleAxis_tasks5_fanout()  { return fanOut(4, B_POP); }

    /** B fan-out, 10 tasks (1 REF + 9 INF callees), N=30. */
    public static LayeredNetwork B_scaleAxis_tasks10_fanout() { return fanOut(9, B_POP); }

    /** B fan-out, 20 tasks (1 REF + 19 INF callees), N=30. */
    public static LayeredNetwork B_scaleAxis_tasks20_fanout() { return fanOut(19, B_POP); }

    /** B fan-out, 40 tasks (1 REF + 39 INF callees), N=30. */
    public static LayeredNetwork B_scaleAxis_tasks40_fanout() { return fanOut(39, B_POP); }

    /** B fan-out, 60 tasks (1 REF + 59 INF callees), N=30. */
    public static LayeredNetwork B_scaleAxis_tasks60_fanout() { return fanOut(59, B_POP); }

    // =========================================================================
    // Partition C1: Fan-out caller topologies
    // Three fan-out widths {2, 3, 4 sibling callees} × two caller capacities
    // {c=1, c=2}. Caller capacity = REF caller task multiplicity *and* its
    // processor's PS server count, simulating a constrained-parallelism caller.
    // These are the fixtures most likely to exhibit oscillation under F19's
    // relax='auto' branch.
    // =========================================================================

    /** C1 fan-out width=2 with single-capacity caller. */
    public static LayeredNetwork C1_fanOut2_caller_c1() { return fanOutCaller("C1_fo2_c1", 2, 1); }

    /** C1 fan-out width=2 with capacity-2 caller. */
    public static LayeredNetwork C1_fanOut2_caller_c2() { return fanOutCaller("C1_fo2_c2", 2, 2); }

    /** C1 fan-out width=3 with single-capacity caller. */
    public static LayeredNetwork C1_fanOut3_caller_c1() { return fanOutCaller("C1_fo3_c1", 3, 1); }

    /** C1 fan-out width=3 with capacity-2 caller. */
    public static LayeredNetwork C1_fanOut3_caller_c2() { return fanOutCaller("C1_fo3_c2", 3, 2); }

    /** C1 fan-out width=4 with single-capacity caller. */
    public static LayeredNetwork C1_fanOut4_caller_c1() { return fanOutCaller("C1_fo4_c1", 4, 1); }

    /** C1 fan-out width=4 with capacity-2 caller. */
    public static LayeredNetwork C1_fanOut4_caller_c2() { return fanOutCaller("C1_fo4_c2", 4, 2); }

    // =========================================================================
    // Partition C2: INF-scheduled hosted tasks
    // Three fixtures with INF-scheduled task layers (no host-layer queueing
    // tail). Exercises Step 5's INF-callee bare-demand heuristic.
    // =========================================================================

    /** C2 simple INF callee: 2-layer with single INF task on INF processor. */
    public static LayeredNetwork C2_infTaskSimple() {
        LayeredNetwork m = new LayeredNetwork("C2_infSimple");
        Processor P1 = new Processor(m, "P1", 4, SchedStrategy.PS);
        Processor P2 = new Processor(m, "P2", Integer.MAX_VALUE, SchedStrategy.INF);

        Task T1 = new Task(m, "T1", 20, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(2.0));
        Task T2 = new Task(m, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(P2);

        Entry E1 = new Entry(m, "E1").on(T1);
        Entry E2 = new Entry(m, "E2").on(T2);
        new Activity(m, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);
        new Activity(m, "A2", Exp.fitMean(0.7)).on(T2).boundTo(E2).repliesTo(E2);
        return m;
    }

    /** C2 shared INF callee: two REF classes call into a shared INF-hosted callee. */
    public static LayeredNetwork C2_infTaskShared() {
        LayeredNetwork m = new LayeredNetwork("C2_infShared");
        Processor PR = new Processor(m, "PR", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PS = new Processor(m, "PS", Integer.MAX_VALUE, SchedStrategy.INF);

        Task R1 = new Task(m, "R1", 20, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(2.0));
        Task R2 = new Task(m, "R2", 20, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(2.5));
        Task TS = new Task(m, "TS", Integer.MAX_VALUE, SchedStrategy.INF).on(PS);

        Entry ER1 = new Entry(m, "ER1").on(R1);
        Entry ER2 = new Entry(m, "ER2").on(R2);
        Entry ES1 = new Entry(m, "ES1").on(TS);
        Entry ES2 = new Entry(m, "ES2").on(TS);

        new Activity(m, "A1", Immediate.getInstance()).on(R1).boundTo(ER1).synchCall(ES1, 1);
        new Activity(m, "A2", Immediate.getInstance()).on(R2).boundTo(ER2).synchCall(ES2, 1);
        new Activity(m, "AS1", Exp.fitMean(0.5)).on(TS).boundTo(ES1).repliesTo(ES1);
        new Activity(m, "AS2", Exp.fitMean(0.8)).on(TS).boundTo(ES2).repliesTo(ES2);
        return m;
    }

    /** C2 deep INF chain: 4-tier chain entirely INF-hosted below the REF. */
    public static LayeredNetwork C2_infTaskDeep() {
        LayeredNetwork m = new LayeredNetwork("C2_infDeep");
        Processor P1 = new Processor(m, "P1", 4, SchedStrategy.PS);
        Processor P2 = new Processor(m, "P2", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor P3 = new Processor(m, "P3", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor P4 = new Processor(m, "P4", Integer.MAX_VALUE, SchedStrategy.INF);

        Task T1 = new Task(m, "T1", 30, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(2.0));
        Task T2 = new Task(m, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(P2);
        Task T3 = new Task(m, "T3", Integer.MAX_VALUE, SchedStrategy.INF).on(P3);
        Task T4 = new Task(m, "T4", Integer.MAX_VALUE, SchedStrategy.INF).on(P4);

        Entry E1 = new Entry(m, "E1").on(T1);
        Entry E2 = new Entry(m, "E2").on(T2);
        Entry E3 = new Entry(m, "E3").on(T3);
        Entry E4 = new Entry(m, "E4").on(T4);

        new Activity(m, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);
        new Activity(m, "A2", Immediate.getInstance()).on(T2).boundTo(E2).synchCall(E3, 1).repliesTo(E1);
        new Activity(m, "A3", Immediate.getInstance()).on(T3).boundTo(E3).synchCall(E4, 1).repliesTo(E2);
        new Activity(m, "A4", Exp.fitMean(0.6)).on(T4).boundTo(E4).repliesTo(E3);
        return m;
    }

    // =========================================================================
    // Partition C3: Multi-class + multi-entry interaction
    // Three fixtures crossing multi-class workloads with multi-entry callees.
    // Probes the interaction surface addressed by the late commit-23 fix.
    // =========================================================================

    /** C3 multi-class + multi-entry: 2 REF classes each hit their own entry on the shared callee. */
    public static LayeredNetwork C3_multiclassMultiEntry_2refs() {
        LayeredNetwork m = new LayeredNetwork("C3_mc_me_2refs");
        Processor PR = new Processor(m, "PR", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PS = new Processor(m, "PS", 1, SchedStrategy.PS);

        Task R1 = new Task(m, "R1", 10, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(2.0));
        Task R2 = new Task(m, "R2", 10, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(2.0));
        Task TS = new Task(m, "TS", Integer.MAX_VALUE, SchedStrategy.INF).on(PS);

        Entry ER1 = new Entry(m, "ER1").on(R1);
        Entry ER2 = new Entry(m, "ER2").on(R2);
        Entry ES1 = new Entry(m, "ES1").on(TS);
        Entry ES2 = new Entry(m, "ES2").on(TS);

        new Activity(m, "A1", Immediate.getInstance()).on(R1).boundTo(ER1).synchCall(ES1, 1);
        new Activity(m, "A2", Immediate.getInstance()).on(R2).boundTo(ER2).synchCall(ES2, 1);
        new Activity(m, "AS1", Exp.fitMean(0.6)).on(TS).boundTo(ES1).repliesTo(ES1);
        new Activity(m, "AS2", Exp.fitMean(0.9)).on(TS).boundTo(ES2).repliesTo(ES2);
        return m;
    }

    /** C3 multi-class + multi-entry: 3 REF classes, 3 entries on shared 1-PS callee. */
    public static LayeredNetwork C3_multiclassMultiEntry_3refs() {
        LayeredNetwork m = new LayeredNetwork("C3_mc_me_3refs");
        Processor PR = new Processor(m, "PR", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PS = new Processor(m, "PS", 1, SchedStrategy.PS);

        Task R1 = new Task(m, "R1", 8, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(2.0));
        Task R2 = new Task(m, "R2", 8, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(2.5));
        Task R3 = new Task(m, "R3", 8, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(3.0));
        Task TS = new Task(m, "TS", Integer.MAX_VALUE, SchedStrategy.INF).on(PS);

        Entry ER1 = new Entry(m, "ER1").on(R1);
        Entry ER2 = new Entry(m, "ER2").on(R2);
        Entry ER3 = new Entry(m, "ER3").on(R3);
        Entry ES1 = new Entry(m, "ES1").on(TS);
        Entry ES2 = new Entry(m, "ES2").on(TS);
        Entry ES3 = new Entry(m, "ES3").on(TS);

        new Activity(m, "A1", Immediate.getInstance()).on(R1).boundTo(ER1).synchCall(ES1, 1);
        new Activity(m, "A2", Immediate.getInstance()).on(R2).boundTo(ER2).synchCall(ES2, 1);
        new Activity(m, "A3", Immediate.getInstance()).on(R3).boundTo(ER3).synchCall(ES3, 1);
        new Activity(m, "AS1", Exp.fitMean(0.5)).on(TS).boundTo(ES1).repliesTo(ES1);
        new Activity(m, "AS2", Exp.fitMean(0.7)).on(TS).boundTo(ES2).repliesTo(ES2);
        new Activity(m, "AS3", Exp.fitMean(0.4)).on(TS).boundTo(ES3).repliesTo(ES3);
        return m;
    }

    /** C3 multi-class + multi-entry asymmetric: 2 REF classes with strongly asymmetric load/demand. */
    public static LayeredNetwork C3_multiclassMultiEntry_asymmetric() {
        LayeredNetwork m = new LayeredNetwork("C3_mc_me_asym");
        Processor PR = new Processor(m, "PR", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PS = new Processor(m, "PS", 1, SchedStrategy.PS);

        Task R1 = new Task(m, "R1", 5,  SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(4.0));
        Task R2 = new Task(m, "R2", 25, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(1.5));
        Task TS = new Task(m, "TS", Integer.MAX_VALUE, SchedStrategy.INF).on(PS);

        Entry ER1 = new Entry(m, "ER1").on(R1);
        Entry ER2 = new Entry(m, "ER2").on(R2);
        Entry ES1 = new Entry(m, "ES1").on(TS);
        Entry ES2 = new Entry(m, "ES2").on(TS);

        new Activity(m, "A1", Immediate.getInstance()).on(R1).boundTo(ER1).synchCall(ES1, 1);
        new Activity(m, "A2", Immediate.getInstance()).on(R2).boundTo(ER2).synchCall(ES2, 1);
        new Activity(m, "AS1", Exp.fitMean(1.5)).on(TS).boundTo(ES1).repliesTo(ES1);
        new Activity(m, "AS2", Exp.fitMean(0.3)).on(TS).boundTo(ES2).repliesTo(ES2);
        return m;
    }

    // =========================================================================
    // Partition C4: Boundary cases for ablation
    // Three fixtures near saturation, near a structural-predicate threshold,
    // or otherwise on the boundary of where the simple solver's approximations
    // hold. Used to characterise *where* the spine's claim breaks down.
    // =========================================================================

    /** C4 near-saturation: target u≈0.95 on a single-server PS bottleneck. */
    public static LayeredNetwork C4_nearSaturation() {
        LayeredNetwork m = new LayeredNetwork("C4_nearSat");
        Processor P1 = new Processor(m, "P1", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor P2 = new Processor(m, "P2", 1, SchedStrategy.PS);

        Task T1 = new Task(m, "T1", 40, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(40.0));
        Task T2 = new Task(m, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(P2);

        Entry E1 = new Entry(m, "E1").on(T1);
        Entry E2 = new Entry(m, "E2").on(T2);
        new Activity(m, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);
        new Activity(m, "A2", Exp.fitMean(1.0)).on(T2).boundTo(E2).repliesTo(E2);
        return m;
    }

    /** C4 predicate threshold: small N with multi-server c such that N≈c (boundary of structural predicate). */
    public static LayeredNetwork C4_predicateThreshold() {
        LayeredNetwork m = new LayeredNetwork("C4_predThresh");
        Processor P1 = new Processor(m, "P1", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor P2 = new Processor(m, "P2", 4, SchedStrategy.PS);

        Task T1 = new Task(m, "T1", 4, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(2.0));
        Task T2 = new Task(m, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(P2);

        Entry E1 = new Entry(m, "E1").on(T1);
        Entry E2 = new Entry(m, "E2").on(T2);
        new Activity(m, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);
        new Activity(m, "A2", Exp.fitMean(1.0)).on(T2).boundTo(E2).repliesTo(E2);
        return m;
    }

    /** C4 low-demand boundary: very small leaf demand relative to think time (near-trivial load). */
    public static LayeredNetwork C4_lowDemand() {
        LayeredNetwork m = new LayeredNetwork("C4_lowDem");
        Processor P1 = new Processor(m, "P1", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor P2 = new Processor(m, "P2", 1, SchedStrategy.PS);

        Task T1 = new Task(m, "T1", 20, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(20.0));
        Task T2 = new Task(m, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(P2);

        Entry E1 = new Entry(m, "E1").on(T1);
        Entry E2 = new Entry(m, "E2").on(T2);
        new Activity(m, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);
        new Activity(m, "A2", Exp.fitMean(0.05)).on(T2).boundTo(E2).repliesTo(E2);
        return m;
    }

    // =========================================================================
    // Partition C5: Saturation regime (fairness restoration)
    // The original B fan-out and C1 fan-out-caller fixtures were tuned down to
    // a moderate-load regime (per-server rho <= 0.7) and the saturated
    // originals exiled to SolverLNSimpleFailingTest "to make the suite pass".
    // C5 restores that regime into the corpus so the load axis is sampled where
    // the solvers actually disagree, not only at its passing point. In this
    // regime LNSimple's closed-network M/M/c tail, SolverLN's R~=D, and LQNS's
    // open-system overtaking all diverge (8-14% on QLen/Util/RespT); the rows
    // are reported honestly (expected match-lqns-only / match-neither) rather
    // than hidden. B itself stays moderate on purpose: its job is to isolate
    // F24 cost against task count, which saturation would confound.
    // =========================================================================

    /** C5 saturated fan-out, k=4 callees, N=30 (the pre-tuning B fan-out shape,
     *  per-server rho ~0.99 at the widest callee). */
    public static LayeredNetwork C5_satFanout_tasks5()  { return saturatedFanOut(4, 30); }

    /** C5 saturated fan-out, k=9 callees, N=30. */
    public static LayeredNetwork C5_satFanout_tasks10() { return saturatedFanOut(9, 30); }

    /** C5 saturated fan-out, k=19 callees, N=30. */
    public static LayeredNetwork C5_satFanout_tasks20() { return saturatedFanOut(19, 30); }

    /** C5 saturated fan-out caller, width 2, c=2 caller (the pre-tuning C1
     *  shape, N = c*20 pushing the widest callee to per-server rho ~1.0). */
    public static LayeredNetwork C5_satFanoutCaller_w2() { return saturatedFanOutCaller("C5_satCaller_w2", 2, 2); }

    /** C5 saturated fan-out caller, width 3, c=2 caller. */
    public static LayeredNetwork C5_satFanoutCaller_w3() { return saturatedFanOutCaller("C5_satCaller_w3", 3, 2); }

    /** C5 saturated fan-out caller, width 4, c=2 caller. */
    public static LayeredNetwork C5_satFanoutCaller_w4() { return saturatedFanOutCaller("C5_satCaller_w4", 4, 2); }

    // =========================================================================
    // Partition C6: Activity-graph boundary (finite-server-host AND-fork)
    // The Step-6 E[max] join correction assumes the parallel branches run on
    // independent notional servers (exact on an INF host). On a finite-server
    // PS host the siblings contend for the shared queue, which the closed-form
    // does not capture (Franks 5.2.1 CCD overlap compensation, out of scope).
    // This is the activity-graph analogue of C5's saturation boundary: the INF
    // counterpart A4_parallelService_andFork passes, so the divergence isolates
    // to the host side. Reported honestly rather than left @Disabled.
    // =========================================================================

    /** C6 AND-fork / AND-join on a single-server PS host: two equal-demand
     *  branches contend for one server, so the closed-form E[max] understates
     *  the join delay. */
    public static LayeredNetwork C6_andForkPsHost() {
        LayeredNetwork m = new LayeredNetwork("C6_andForkPsHost");
        Processor PR = new Processor(m, "PR", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PS = new Processor(m, "PS", 1, SchedStrategy.PS);

        Task T1 = new Task(m, "T1", 15, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(2.0));
        Task T2 = new Task(m, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(PS);

        Entry E1 = new Entry(m, "E1").on(T1);
        Entry E2 = new Entry(m, "E2").on(T2);

        new Activity(m, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);

        Activity A_fork = new Activity(m, "A_fork", Immediate.getInstance()).on(T2).boundTo(E2);
        Activity A_p1   = new Activity(m, "A_p1",   Exp.fitMean(0.5)).on(T2);
        Activity A_p2   = new Activity(m, "A_p2",   Exp.fitMean(0.5)).on(T2);
        Activity A_join = new Activity(m, "A_join", Immediate.getInstance()).on(T2).repliesTo(E2);
        T2.addPrecedence(ActivityPrecedence.AndFork(A_fork, Arrays.asList(A_p1, A_p2)));
        T2.addPrecedence(ActivityPrecedence.AndJoin(Arrays.asList(A_p1, A_p2), A_join));
        return m;
    }

    // =========================================================================
    // Partition C7: Deep-chain stress (convergence-rate boundary)
    // The canonical A1 chains keep their intermediaries Immediate and put the
    // only real demand at the leaf, which converges cleanly even at depth 8 /
    // N=100. C7 carries the deep all-Exp + multi-call regime A1 sidesteps:
    // SolverLNSimple is reported to hit its iteration cap here (its
    // single-iteration max-delta detection cannot quench the small-amplitude
    // oscillation SolverLN's moving-average window filters). Promoted from the
    // failing suite's Group 2 so the chain boundary is in-corpus, not hidden.
    // =========================================================================

    /** C7 deep-chain stress: 3-tier single-class chain, N=100, real Exp service
     *  at the intermediate and leaf, with a multi-call edge (the intermediate
     *  calls the leaf twice per visit) amplifying the per-iteration feedback. */
    public static LayeredNetwork C7_deepChainMultiCall() {
        LayeredNetwork m = new LayeredNetwork("C7_deepChainMultiCall");
        Processor P1 = new Processor(m, "P1", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor P2 = new Processor(m, "P2", 1, SchedStrategy.PS);
        Processor P3 = new Processor(m, "P3", 1, SchedStrategy.PS);

        Task T1 = new Task(m, "T1", 100, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(1.0));
        Task T2 = new Task(m, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(P2);
        Task T3 = new Task(m, "T3", Integer.MAX_VALUE, SchedStrategy.INF).on(P3);

        Entry E1 = new Entry(m, "E1").on(T1);
        Entry E2 = new Entry(m, "E2").on(T2);
        Entry E3 = new Entry(m, "E3").on(T3);

        new Activity(m, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);
        new Activity(m, "A2", Exp.fitMean(0.2)).on(T2).boundTo(E2).synchCall(E3, 2).repliesTo(E1);
        new Activity(m, "A3", Exp.fitMean(0.3)).on(T3).boundTo(E3).repliesTo(E2);
        return m;
    }

    /** C7 deep-chain stress: 4-tier single-class chain, N=80, real Exp service
     *  at every tier (no Immediate intermediaries) — the depth axis the passing
     *  A1_chain_d8 avoids by concentrating demand at the leaf. */
    public static LayeredNetwork C7_deepChainAllExp() {
        LayeredNetwork m = new LayeredNetwork("C7_deepChainAllExp");
        Processor P1 = new Processor(m, "P1", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor P2 = new Processor(m, "P2", 1, SchedStrategy.PS);
        Processor P3 = new Processor(m, "P3", 1, SchedStrategy.PS);
        Processor P4 = new Processor(m, "P4", 1, SchedStrategy.PS);

        Task T1 = new Task(m, "T1", 80, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(1.0));
        Task T2 = new Task(m, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(P2);
        Task T3 = new Task(m, "T3", Integer.MAX_VALUE, SchedStrategy.INF).on(P3);
        Task T4 = new Task(m, "T4", Integer.MAX_VALUE, SchedStrategy.INF).on(P4);

        Entry E1 = new Entry(m, "E1").on(T1);
        Entry E2 = new Entry(m, "E2").on(T2);
        Entry E3 = new Entry(m, "E3").on(T3);
        Entry E4 = new Entry(m, "E4").on(T4);

        new Activity(m, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);
        new Activity(m, "A2", Exp.fitMean(0.2)).on(T2).boundTo(E2).synchCall(E3, 1).repliesTo(E1);
        new Activity(m, "A3", Exp.fitMean(0.2)).on(T3).boundTo(E3).synchCall(E4, 1).repliesTo(E2);
        new Activity(m, "A4", Exp.fitMean(0.2)).on(T4).boundTo(E4).repliesTo(E3);
        return m;
    }

    // =========================================================================
    //  Internal helper builders (shared across partitions)
    // =========================================================================

    /**
     * Pre-tuning saturated fan-out builder used by Partition C5. One REF caller
     * with {@code n} customers (think time Z=0.5 via {@code new Exp(2)}) calls
     * {@code k} INF callees on independent 4-server PS hosts with asymmetric
     * demand {@code 0.6 + 0.05*i}, which drives the widest callee to per-server
     * rho ~0.99. These are the original parameters {@link #fanOut} was tuned
     * away from (demand 0.6 -> 0.3, Z -> 4.0) to land at moderate load.
     */
    private static LayeredNetwork saturatedFanOut(int k, int n) {
        LayeredNetwork m = new LayeredNetwork("satFanOut_k" + k + "_N" + n);
        Processor P0 = new Processor(m, "P0", 4, SchedStrategy.PS);
        Task T0 = new Task(m, "T0", n, SchedStrategy.REF).on(P0).setThinkTime(new Exp(2));
        Entry E0 = new Entry(m, "E0").on(T0);

        Entry[] calleeEntries = new Entry[k];
        for (int i = 0; i < k; i++) {
            Processor Pi = new Processor(m, "P" + (i + 1), 4, SchedStrategy.PS);
            Task Ti = new Task(m, "T" + (i + 1), Integer.MAX_VALUE, SchedStrategy.INF).on(Pi);
            calleeEntries[i] = new Entry(m, "E" + (i + 1)).on(Ti);
            new Activity(m, "AS" + (i + 1), Exp.fitMean(0.6 + 0.05 * i))
                    .on(Ti).boundTo(calleeEntries[i]).repliesTo(calleeEntries[i]);
        }
        Activity caller = new Activity(m, "A0", Immediate.getInstance()).on(T0).boundTo(E0);
        for (Entry ce : calleeEntries) caller = caller.synchCall(ce, 1);
        return m;
    }

    /**
     * Pre-tuning saturated fan-out caller used by Partition C5. REF with
     * N = {@code max(callerC*20, 20)} customers on a {@code callerC}-server PS
     * host calls {@code width} INF callees on 4-server PS hosts with asymmetric
     * demand {@code 0.6 + 0.1*i}; for {@code callerC = 2} the population scaling
     * pushes the widest callee to per-server rho ~1.0. These are the original
     * parameters {@link #fanOutCaller} was tuned away from (N pinned at 20,
     * demand 0.6 -> 0.3) to keep the callees moderate.
     */
    private static LayeredNetwork saturatedFanOutCaller(String name, int width, int callerC) {
        LayeredNetwork m = new LayeredNetwork(name);
        Processor PR = new Processor(m, "PR", callerC, SchedStrategy.PS);
        Task R = new Task(m, "R", Math.max(callerC * 20, 20), SchedStrategy.REF)
                .on(PR).setThinkTime(Exp.fitMean(2.0));
        Entry ER = new Entry(m, "ER").on(R);

        Entry[] calleeEntries = new Entry[width];
        for (int i = 0; i < width; i++) {
            Processor Pi = new Processor(m, "PS" + (i + 1), 4, SchedStrategy.PS);
            Task Ti = new Task(m, "TS" + (i + 1), Integer.MAX_VALUE, SchedStrategy.INF).on(Pi);
            calleeEntries[i] = new Entry(m, "ES" + (i + 1)).on(Ti);
            new Activity(m, "AS" + (i + 1), Exp.fitMean(0.6 + 0.1 * i))
                    .on(Ti).boundTo(calleeEntries[i]).repliesTo(calleeEntries[i]);
        }
        Activity caller = new Activity(m, "A0", Immediate.getInstance()).on(R).boundTo(ER);
        for (Entry ce : calleeEntries) caller = caller.synchCall(ce, 1);
        return m;
    }

    /**
     * Single-class chain of {@code tiers} tasks. Tier 1 is REF with population
     * {@code n}; intermediate tiers are INF on 4-server PS processors; the
     * leaf carries the only non-trivial Exp service demand (mean=0.8). Used
     * by Partition A1 and Partition B (chain shape).
     */
    private static LayeredNetwork chain(int tiers, int n) {
        LayeredNetwork m = new LayeredNetwork("chain_d" + tiers + "_N" + n);
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
        new Activity(m, "AS1", Immediate.getInstance()).on(T[0]).boundTo(E[0]).synchCall(E[1], 1);
        for (int i = 1; i < tiers - 1; i++) {
            new Activity(m, "AS" + (i + 1), Immediate.getInstance())
                    .on(T[i]).boundTo(E[i]).synchCall(E[i + 1], 1).repliesTo(E[i - 1]);
        }
        new Activity(m, "AS" + tiers, Exp.fitMean(0.8))
                .on(T[tiers - 1]).boundTo(E[tiers - 1]).repliesTo(E[tiers - 2]);
        return m;
    }

    /**
     * Fan-out: one REF task with {@code n} customers calls {@code k}
     * independent INF callees, each on its own 4-server PS processor. Used by
     * Partition B (fan-out shape).
     */
    private static LayeredNetwork fanOut(int k, int n) {
        LayeredNetwork m = new LayeredNetwork("fanOut_k" + k + "_N" + n);
        Processor P0 = new Processor(m, "P0", 4, SchedStrategy.PS);
        // Think time tuned so the bottleneck callee stays in the moderate-load
        // regime (per-server rho ≤ 0.8) across the k = 4..59 sweep. The original
        // Z = 0.5 (from {@code new Exp(2)}) saturated the last callee for k ≤ 10,
        // where LN's M/M/c approximation, LQNS's MoL, and LNSimple's load-
        // dependent MVA diverge by 8–14% on QLen/Util.
        Task T0 = new Task(m, "T0", n, SchedStrategy.REF).on(P0).setThinkTime(Exp.fitMean(4.0));
        Entry E0 = new Entry(m, "E0").on(T0);

        Entry[] calleeEntries = new Entry[k];
        for (int i = 0; i < k; i++) {
            Processor Pi = new Processor(m, "P" + (i + 1), 4, SchedStrategy.PS);
            Task Ti = new Task(m, "T" + (i + 1), Integer.MAX_VALUE, SchedStrategy.INF).on(Pi);
            calleeEntries[i] = new Entry(m, "E" + (i + 1)).on(Ti);
            // Demand base shifted from 0.6 to 0.3 so the widest callee's
            // per-server utilisation stays below 0.8 at the populations tested.
            new Activity(m, "AS" + (i + 1), Exp.fitMean(0.3 + 0.05 * i))
                    .on(Ti).boundTo(calleeEntries[i]).repliesTo(calleeEntries[i]);
        }
        Activity caller = new Activity(m, "A0", Immediate.getInstance()).on(T0).boundTo(E0);
        for (Entry ce : calleeEntries) caller = caller.synchCall(ce, 1);
        return m;
    }

    /**
     * Two-class shared-callee builder used by Partition A2.
     *
     * <p>Two REF callers (each its own class) onto a shared single-server PS
     * callee task; each caller hits its own entry on the callee for proper
     * per-class accounting.
     */
    private static LayeredNetwork sharedCalleeMC(String name, int classes,
                                                 int n1, int n2,
                                                 double z1, double z2,
                                                 double d1, double d2) {
        LayeredNetwork m = new LayeredNetwork(name);
        Processor PR = new Processor(m, "PR", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PS = new Processor(m, "PS", 1, SchedStrategy.PS);

        Task R1 = new Task(m, "R1", n1, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(z1));
        Task R2 = new Task(m, "R2", n2, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(z2));
        Task TS = new Task(m, "TS", Integer.MAX_VALUE, SchedStrategy.INF).on(PS);

        Entry ER1 = new Entry(m, "ER1").on(R1);
        Entry ER2 = new Entry(m, "ER2").on(R2);
        Entry ES1 = new Entry(m, "ES1").on(TS);
        Entry ES2 = new Entry(m, "ES2").on(TS);

        new Activity(m, "A1", Immediate.getInstance()).on(R1).boundTo(ER1).synchCall(ES1, 1);
        new Activity(m, "A2", Immediate.getInstance()).on(R2).boundTo(ER2).synchCall(ES2, 1);
        new Activity(m, "AS1", Exp.fitMean(d1)).on(TS).boundTo(ES1).repliesTo(ES1);
        new Activity(m, "AS2", Exp.fitMean(d2)).on(TS).boundTo(ES2).repliesTo(ES2);
        return m;
    }

    /**
     * Single-class multi-server callee builder used by Partition A3.
     *
     * <p>One REF caller with {@code n} customers, think time {@code z}, calls
     * a single INF task hosted on a {@code c}-server PS processor with leaf
     * service demand {@code d}.
     */
    private static LayeredNetwork multiServerCallee(String name, int c, int n,
                                                    double z, double d) {
        LayeredNetwork m = new LayeredNetwork(name);
        Processor PR = new Processor(m, "PR", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PS = new Processor(m, "PS", c, SchedStrategy.PS);

        Task T1 = new Task(m, "T1", n, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(z));
        Task T2 = new Task(m, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(PS);

        Entry E1 = new Entry(m, "E1").on(T1);
        Entry E2 = new Entry(m, "E2").on(T2);
        new Activity(m, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);
        new Activity(m, "A2", Exp.fitMean(d)).on(T2).boundTo(E2).repliesTo(E2);
        return m;
    }

    /**
     * Fan-out caller used by Partition C1. One REF caller of capacity
     * {@code callerC} (task multiplicity = processor PS server count =
     * callerC) drives {@code width} sibling INF callees on independent
     * 4-server PS processors.
     */
    private static LayeredNetwork fanOutCaller(String name, int width, int callerC) {
        LayeredNetwork m = new LayeredNetwork(name);
        Processor PR = new Processor(m, "PR", callerC, SchedStrategy.PS);
        // N kept constant at 20 (rather than {@code callerC × 20}). The original
        // scaling pushed the {@code callerC = 2} cases into a saturation regime
        // (per-server rho ≥ 0.86 at the widest callee) where closed-network MVA
        // variants diverge. Keeping N fixed preserves the "constrained-caller
        // fan-out" structural test — the variation across {1,2} caller capacity
        // is still meaningful because PR's PS-server count is the constraint —
        // while keeping the callees in the moderate-load regime.
        Task R = new Task(m, "R", 20, SchedStrategy.REF)
                .on(PR).setThinkTime(Exp.fitMean(2.0));
        Entry ER = new Entry(m, "ER").on(R);

        Entry[] calleeEntries = new Entry[width];
        for (int i = 0; i < width; i++) {
            Processor Pi = new Processor(m, "PS" + (i + 1), 4, SchedStrategy.PS);
            Task Ti = new Task(m, "TS" + (i + 1), Integer.MAX_VALUE, SchedStrategy.INF).on(Pi);
            calleeEntries[i] = new Entry(m, "ES" + (i + 1)).on(Ti);
            // Demands shifted to keep per-server rho ≤ ~0.7 even at width = 4.
            new Activity(m, "AS" + (i + 1), Exp.fitMean(0.3 + 0.05 * i))
                    .on(Ti).boundTo(calleeEntries[i]).repliesTo(calleeEntries[i]);
        }
        Activity caller = new Activity(m, "A0", Immediate.getInstance()).on(R).boundTo(ER);
        for (Entry ce : calleeEntries) caller = caller.synchCall(ce, 1);
        return m;
    }

    // =========================================================================
    // F19 stress probe fixtures: experimental models designed to provoke true
    // oscillation in the LN coupling iteration at omega = 1.0, so the auto
    // branch's sign-change detector might actually fire — and the 5-iter
    // detection lag might cost auto enough iterations that fixed-mode (omega
    // = 0.5 from iter 1) beats it. These are NOT part of the main Partition
    // A/B/C corpus; they exist only to probe the F19 mechanism in §8.8.
    //
    // Targeted features: heavily-loaded shared FCFS/PS-1 bottleneck, multiple
    // competing classes, tight think times, large populations.
    // =========================================================================

    /** F19 probe: heavy-contention 2-class symmetric PS-1 bottleneck.
     *  Two REF classes (N=50 each, Z=0.5) onto a shared single-server PS
     *  callee with per-class demand 0.95. Per-class arrival rate cannot
     *  exceed ~0.5 simultaneously, so both classes contend at saturation. */
    public static LayeredNetwork F19_sharedFCFS_sym_N50() {
        LayeredNetwork m = new LayeredNetwork("F19_sharedFCFS_sym_N50");
        Processor PR = new Processor(m, "PR", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PS = new Processor(m, "PS", 1, SchedStrategy.PS);
        Task R1 = new Task(m, "R1", 50, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(0.5));
        Task R2 = new Task(m, "R2", 50, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(0.5));
        Task TS = new Task(m, "TS", Integer.MAX_VALUE, SchedStrategy.INF).on(PS);
        Entry ER1 = new Entry(m, "ER1").on(R1);
        Entry ER2 = new Entry(m, "ER2").on(R2);
        Entry ES1 = new Entry(m, "ES1").on(TS);
        Entry ES2 = new Entry(m, "ES2").on(TS);
        new Activity(m, "A1", Immediate.getInstance()).on(R1).boundTo(ER1).synchCall(ES1, 1);
        new Activity(m, "A2", Immediate.getInstance()).on(R2).boundTo(ER2).synchCall(ES2, 1);
        new Activity(m, "AS1", Exp.fitMean(0.95)).on(TS).boundTo(ES1).repliesTo(ES1);
        new Activity(m, "AS2", Exp.fitMean(0.95)).on(TS).boundTo(ES2).repliesTo(ES2);
        return m;
    }

    /** F19 probe: extreme-contention variant. N=100 per class, Z=0.3, D=0.95. */
    public static LayeredNetwork F19_sharedFCFS_sym_N100_extreme() {
        LayeredNetwork m = new LayeredNetwork("F19_sharedFCFS_sym_N100_extreme");
        Processor PR = new Processor(m, "PR", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PS = new Processor(m, "PS", 1, SchedStrategy.PS);
        Task R1 = new Task(m, "R1", 100, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(0.3));
        Task R2 = new Task(m, "R2", 100, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(0.3));
        Task TS = new Task(m, "TS", Integer.MAX_VALUE, SchedStrategy.INF).on(PS);
        Entry ER1 = new Entry(m, "ER1").on(R1);
        Entry ER2 = new Entry(m, "ER2").on(R2);
        Entry ES1 = new Entry(m, "ES1").on(TS);
        Entry ES2 = new Entry(m, "ES2").on(TS);
        new Activity(m, "A1", Immediate.getInstance()).on(R1).boundTo(ER1).synchCall(ES1, 1);
        new Activity(m, "A2", Immediate.getInstance()).on(R2).boundTo(ER2).synchCall(ES2, 1);
        new Activity(m, "AS1", Exp.fitMean(0.95)).on(TS).boundTo(ES1).repliesTo(ES1);
        new Activity(m, "AS2", Exp.fitMean(0.95)).on(TS).boundTo(ES2).repliesTo(ES2);
        return m;
    }

    /** F19 probe: 3-class asymmetric. Demands {0.5, 0.7, 0.9}, N=40 each, Z=0.5. */
    public static LayeredNetwork F19_sharedFCFS_3class_asym() {
        LayeredNetwork m = new LayeredNetwork("F19_sharedFCFS_3class_asym");
        Processor PR = new Processor(m, "PR", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PS = new Processor(m, "PS", 1, SchedStrategy.PS);
        Task R1 = new Task(m, "R1", 40, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(0.3));
        Task R2 = new Task(m, "R2", 40, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(0.5));
        Task R3 = new Task(m, "R3", 40, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(0.8));
        Task TS = new Task(m, "TS", Integer.MAX_VALUE, SchedStrategy.INF).on(PS);
        Entry ER1 = new Entry(m, "ER1").on(R1);
        Entry ER2 = new Entry(m, "ER2").on(R2);
        Entry ER3 = new Entry(m, "ER3").on(R3);
        Entry ES1 = new Entry(m, "ES1").on(TS);
        Entry ES2 = new Entry(m, "ES2").on(TS);
        Entry ES3 = new Entry(m, "ES3").on(TS);
        new Activity(m, "A1", Immediate.getInstance()).on(R1).boundTo(ER1).synchCall(ES1, 1);
        new Activity(m, "A2", Immediate.getInstance()).on(R2).boundTo(ER2).synchCall(ES2, 1);
        new Activity(m, "A3", Immediate.getInstance()).on(R3).boundTo(ER3).synchCall(ES3, 1);
        new Activity(m, "AS1", Exp.fitMean(0.5)).on(TS).boundTo(ES1).repliesTo(ES1);
        new Activity(m, "AS2", Exp.fitMean(0.7)).on(TS).boundTo(ES2).repliesTo(ES2);
        new Activity(m, "AS3", Exp.fitMean(0.9)).on(TS).boundTo(ES3).repliesTo(ES3);
        return m;
    }

    /** F19 probe: 2-tier chain with heavily-loaded FCFS middle tier.
     *  Two classes split between two intermediate entries on FCFS-1 middle. */
    public static LayeredNetwork F19_chainFCFS_2class() {
        LayeredNetwork m = new LayeredNetwork("F19_chainFCFS_2class");
        Processor PR = new Processor(m, "PR", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PM = new Processor(m, "PM", 1, SchedStrategy.FCFS);
        Processor PL = new Processor(m, "PL", Integer.MAX_VALUE, SchedStrategy.INF);
        Task R1 = new Task(m, "R1", 30, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(0.5));
        Task R2 = new Task(m, "R2", 30, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(0.5));
        Task TM = new Task(m, "TM", Integer.MAX_VALUE, SchedStrategy.INF).on(PM);
        Task TL = new Task(m, "TL", Integer.MAX_VALUE, SchedStrategy.INF).on(PL);
        Entry ER1 = new Entry(m, "ER1").on(R1);
        Entry ER2 = new Entry(m, "ER2").on(R2);
        Entry EM1 = new Entry(m, "EM1").on(TM);
        Entry EM2 = new Entry(m, "EM2").on(TM);
        Entry EL  = new Entry(m, "EL").on(TL);
        new Activity(m, "A1", Immediate.getInstance()).on(R1).boundTo(ER1).synchCall(EM1, 1);
        new Activity(m, "A2", Immediate.getInstance()).on(R2).boundTo(ER2).synchCall(EM2, 1);
        new Activity(m, "AM1", Exp.fitMean(0.4)).on(TM).boundTo(EM1).synchCall(EL, 1).repliesTo(EM1);
        new Activity(m, "AM2", Exp.fitMean(0.6)).on(TM).boundTo(EM2).synchCall(EL, 1).repliesTo(EM2);
        new Activity(m, "AL",  Exp.fitMean(0.3)).on(TL).boundTo(EL).repliesTo(EL);
        return m;
    }

    /** F19 probe (round 2): extreme tight-loop. Massive N, micro think time,
     *  single class deep cascade through a near-saturation PS-1. Pushes
     *  closed-model loop gain (N/(R+Z)^2 term) hard. */
    public static LayeredNetwork F19_tightLoop_N500_microZ() {
        LayeredNetwork m = new LayeredNetwork("F19_tightLoop_N500_microZ");
        Processor PR = new Processor(m, "PR", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PS = new Processor(m, "PS", 1, SchedStrategy.PS);
        Task R  = new Task(m, "R",  500, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(0.01));
        Task TS = new Task(m, "TS", Integer.MAX_VALUE, SchedStrategy.INF).on(PS);
        Entry ER = new Entry(m, "ER").on(R);
        Entry ES = new Entry(m, "ES").on(TS);
        new Activity(m, "AR", Immediate.getInstance()).on(R).boundTo(ER).synchCall(ES, 1);
        new Activity(m, "AS", Exp.fitMean(0.7)).on(TS).boundTo(ES).repliesTo(ES);
        return m;
    }

    /** F19 probe (round 2): 4-class shared bottleneck deliberately set so
     *  total uncoupled utilisation = 1.6 (way over saturation). Forces every
     *  class to throttle on every iteration; the throttling cascade may
     *  produce ping-pong sign-change. */
    public static LayeredNetwork F19_overSat_4class() {
        LayeredNetwork m = new LayeredNetwork("F19_overSat_4class");
        Processor PR = new Processor(m, "PR", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PS = new Processor(m, "PS", 1, SchedStrategy.PS);
        int N = 40;
        double Z = 0.2;
        double[] demands = {0.4, 0.4, 0.4, 0.4};
        Task[] R = new Task[4];
        Entry[] ER = new Entry[4];
        Entry[] ES = new Entry[4];
        Task TS = new Task(m, "TS", Integer.MAX_VALUE, SchedStrategy.INF).on(PS);
        for (int i = 0; i < 4; i++) {
            R[i] = new Task(m, "R" + (i+1), N, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(Z));
            ER[i] = new Entry(m, "ER" + (i+1)).on(R[i]);
            ES[i] = new Entry(m, "ES" + (i+1)).on(TS);
            new Activity(m, "A" + (i+1), Immediate.getInstance()).on(R[i]).boundTo(ER[i]).synchCall(ES[i], 1);
            new Activity(m, "AS" + (i+1), Exp.fitMean(demands[i])).on(TS).boundTo(ES[i]).repliesTo(ES[i]);
        }
        return m;
    }

    /** F19 probe (round 2): cascaded bottleneck. Two FCFS-1 stages in series,
     *  each at moderate load, with two classes flowing through both. The
     *  cascade introduces phase-shift between the two stages that may
     *  produce oscillation. */
    public static LayeredNetwork F19_cascade_2stage_2class() {
        LayeredNetwork m = new LayeredNetwork("F19_cascade_2stage_2class");
        Processor PR  = new Processor(m, "PR",  Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PS1 = new Processor(m, "PS1", 1, SchedStrategy.PS);
        Processor PS2 = new Processor(m, "PS2", 1, SchedStrategy.PS);
        Task R1 = new Task(m, "R1", 30, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(0.3));
        Task R2 = new Task(m, "R2", 30, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(0.3));
        Task T1 = new Task(m, "T1", Integer.MAX_VALUE, SchedStrategy.INF).on(PS1);
        Task T2 = new Task(m, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(PS2);
        Entry ER1 = new Entry(m, "ER1").on(R1);
        Entry ER2 = new Entry(m, "ER2").on(R2);
        Entry E1a = new Entry(m, "E1a").on(T1);
        Entry E1b = new Entry(m, "E1b").on(T1);
        Entry E2a = new Entry(m, "E2a").on(T2);
        Entry E2b = new Entry(m, "E2b").on(T2);
        new Activity(m, "A1", Immediate.getInstance()).on(R1).boundTo(ER1).synchCall(E1a, 1);
        new Activity(m, "A2", Immediate.getInstance()).on(R2).boundTo(ER2).synchCall(E1b, 1);
        new Activity(m, "AT1a", Exp.fitMean(0.45)).on(T1).boundTo(E1a).synchCall(E2a, 1).repliesTo(E1a);
        new Activity(m, "AT1b", Exp.fitMean(0.45)).on(T1).boundTo(E1b).synchCall(E2b, 1).repliesTo(E1b);
        new Activity(m, "AT2a", Exp.fitMean(0.5)).on(T2).boundTo(E2a).repliesTo(E2a);
        new Activity(m, "AT2b", Exp.fitMean(0.5)).on(T2).boundTo(E2b).repliesTo(E2b);
        return m;
    }

    /** F19 probe (round 3): 4-stage cascade. Extended the 2-stage cascade
     *  by adding two more FCFS-like stages, each at moderate-to-high load,
     *  so the oscillation amplitude has more room to grow before detection. */
    public static LayeredNetwork F19_cascade_4stage_2class() {
        LayeredNetwork m = new LayeredNetwork("F19_cascade_4stage_2class");
        Processor PR  = new Processor(m, "PR",  Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PS1 = new Processor(m, "PS1", 1, SchedStrategy.PS);
        Processor PS2 = new Processor(m, "PS2", 1, SchedStrategy.PS);
        Processor PS3 = new Processor(m, "PS3", 1, SchedStrategy.PS);
        Processor PS4 = new Processor(m, "PS4", 1, SchedStrategy.PS);
        Task R1 = new Task(m, "R1", 30, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(0.3));
        Task R2 = new Task(m, "R2", 30, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(0.3));
        Task T1 = new Task(m, "T1", Integer.MAX_VALUE, SchedStrategy.INF).on(PS1);
        Task T2 = new Task(m, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(PS2);
        Task T3 = new Task(m, "T3", Integer.MAX_VALUE, SchedStrategy.INF).on(PS3);
        Task T4 = new Task(m, "T4", Integer.MAX_VALUE, SchedStrategy.INF).on(PS4);
        Entry ER1 = new Entry(m, "ER1").on(R1);
        Entry ER2 = new Entry(m, "ER2").on(R2);
        Entry E1a = new Entry(m, "E1a").on(T1);
        Entry E1b = new Entry(m, "E1b").on(T1);
        Entry E2a = new Entry(m, "E2a").on(T2);
        Entry E2b = new Entry(m, "E2b").on(T2);
        Entry E3a = new Entry(m, "E3a").on(T3);
        Entry E3b = new Entry(m, "E3b").on(T3);
        Entry E4a = new Entry(m, "E4a").on(T4);
        Entry E4b = new Entry(m, "E4b").on(T4);
        new Activity(m, "A1", Immediate.getInstance()).on(R1).boundTo(ER1).synchCall(E1a, 1);
        new Activity(m, "A2", Immediate.getInstance()).on(R2).boundTo(ER2).synchCall(E1b, 1);
        new Activity(m, "AT1a", Exp.fitMean(0.45)).on(T1).boundTo(E1a).synchCall(E2a, 1).repliesTo(E1a);
        new Activity(m, "AT1b", Exp.fitMean(0.45)).on(T1).boundTo(E1b).synchCall(E2b, 1).repliesTo(E1b);
        new Activity(m, "AT2a", Exp.fitMean(0.45)).on(T2).boundTo(E2a).synchCall(E3a, 1).repliesTo(E2a);
        new Activity(m, "AT2b", Exp.fitMean(0.45)).on(T2).boundTo(E2b).synchCall(E3b, 1).repliesTo(E2b);
        new Activity(m, "AT3a", Exp.fitMean(0.45)).on(T3).boundTo(E3a).synchCall(E4a, 1).repliesTo(E3a);
        new Activity(m, "AT3b", Exp.fitMean(0.45)).on(T3).boundTo(E3b).synchCall(E4b, 1).repliesTo(E3b);
        new Activity(m, "AT4a", Exp.fitMean(0.5)).on(T4).boundTo(E4a).repliesTo(E4a);
        new Activity(m, "AT4b", Exp.fitMean(0.5)).on(T4).boundTo(E4b).repliesTo(E4b);
        return m;
    }

    /** F19 probe (round 3): 3-stage cascade with 3 classes — more mutual coupling. */
    public static LayeredNetwork F19_cascade_3stage_3class() {
        LayeredNetwork m = new LayeredNetwork("F19_cascade_3stage_3class");
        Processor PR  = new Processor(m, "PR",  Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PS1 = new Processor(m, "PS1", 1, SchedStrategy.PS);
        Processor PS2 = new Processor(m, "PS2", 1, SchedStrategy.PS);
        Processor PS3 = new Processor(m, "PS3", 1, SchedStrategy.PS);
        Task R1 = new Task(m, "R1", 30, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(0.3));
        Task R2 = new Task(m, "R2", 30, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(0.3));
        Task R3 = new Task(m, "R3", 30, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(0.3));
        Task T1 = new Task(m, "T1", Integer.MAX_VALUE, SchedStrategy.INF).on(PS1);
        Task T2 = new Task(m, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(PS2);
        Task T3 = new Task(m, "T3", Integer.MAX_VALUE, SchedStrategy.INF).on(PS3);
        Entry ER1 = new Entry(m, "ER1").on(R1);
        Entry ER2 = new Entry(m, "ER2").on(R2);
        Entry ER3 = new Entry(m, "ER3").on(R3);
        Entry E1a = new Entry(m, "E1a").on(T1);
        Entry E1b = new Entry(m, "E1b").on(T1);
        Entry E1c = new Entry(m, "E1c").on(T1);
        Entry E2a = new Entry(m, "E2a").on(T2);
        Entry E2b = new Entry(m, "E2b").on(T2);
        Entry E2c = new Entry(m, "E2c").on(T2);
        Entry E3a = new Entry(m, "E3a").on(T3);
        Entry E3b = new Entry(m, "E3b").on(T3);
        Entry E3c = new Entry(m, "E3c").on(T3);
        new Activity(m, "A1", Immediate.getInstance()).on(R1).boundTo(ER1).synchCall(E1a, 1);
        new Activity(m, "A2", Immediate.getInstance()).on(R2).boundTo(ER2).synchCall(E1b, 1);
        new Activity(m, "A3", Immediate.getInstance()).on(R3).boundTo(ER3).synchCall(E1c, 1);
        new Activity(m, "AT1a", Exp.fitMean(0.30)).on(T1).boundTo(E1a).synchCall(E2a, 1).repliesTo(E1a);
        new Activity(m, "AT1b", Exp.fitMean(0.30)).on(T1).boundTo(E1b).synchCall(E2b, 1).repliesTo(E1b);
        new Activity(m, "AT1c", Exp.fitMean(0.30)).on(T1).boundTo(E1c).synchCall(E2c, 1).repliesTo(E1c);
        new Activity(m, "AT2a", Exp.fitMean(0.30)).on(T2).boundTo(E2a).synchCall(E3a, 1).repliesTo(E2a);
        new Activity(m, "AT2b", Exp.fitMean(0.30)).on(T2).boundTo(E2b).synchCall(E3b, 1).repliesTo(E2b);
        new Activity(m, "AT2c", Exp.fitMean(0.30)).on(T2).boundTo(E2c).synchCall(E3c, 1).repliesTo(E2c);
        new Activity(m, "AT3a", Exp.fitMean(0.30)).on(T3).boundTo(E3a).repliesTo(E3a);
        new Activity(m, "AT3b", Exp.fitMean(0.30)).on(T3).boundTo(E3b).repliesTo(E3b);
        new Activity(m, "AT3c", Exp.fitMean(0.30)).on(T3).boundTo(E3c).repliesTo(E3c);
        return m;
    }

    /** F19 probe: deep loop with low Z. Single class N=200 with very small
     *  think time, through a 3-call chain ending at a near-saturation PS-1
     *  bottleneck. Aim for high loop gain via tight loop. */
    public static LayeredNetwork F19_deepLoop_lowZ() {
        LayeredNetwork m = new LayeredNetwork("F19_deepLoop_lowZ");
        Processor PR = new Processor(m, "PR", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PA = new Processor(m, "PA", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PB = new Processor(m, "PB", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PS = new Processor(m, "PS", 1, SchedStrategy.PS);
        Task R  = new Task(m, "R",  200, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(0.1));
        Task TA = new Task(m, "TA", Integer.MAX_VALUE, SchedStrategy.INF).on(PA);
        Task TB = new Task(m, "TB", Integer.MAX_VALUE, SchedStrategy.INF).on(PB);
        Task TS = new Task(m, "TS", Integer.MAX_VALUE, SchedStrategy.INF).on(PS);
        Entry ER = new Entry(m, "ER").on(R);
        Entry EA = new Entry(m, "EA").on(TA);
        Entry EB = new Entry(m, "EB").on(TB);
        Entry ES = new Entry(m, "ES").on(TS);
        new Activity(m, "AR", Immediate.getInstance()).on(R).boundTo(ER).synchCall(EA, 1);
        new Activity(m, "AA", Exp.fitMean(0.1)).on(TA).boundTo(EA).synchCall(EB, 1).repliesTo(EA);
        new Activity(m, "AB", Exp.fitMean(0.1)).on(TB).boundTo(EB).synchCall(ES, 1).repliesTo(EB);
        new Activity(m, "AS", Exp.fitMean(0.4)).on(TS).boundTo(ES).repliesTo(ES);
        return m;
    }
}
