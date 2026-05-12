package jline.solvers.ln_simple.fixtures;

import jline.lang.constant.SchedStrategy;
import jline.lang.layered.*;
import jline.lang.processes.Exp;
import jline.lang.processes.Immediate;

/**
 * Multiclass LQN examples for TDD development of SolverLNSimple multiclass support.
 *
 * <p>Categories (each builds on the previous):
 * <pre>
 *   A – No shared resources: classes are independent, no inter-class contention.
 *       Tests that the solver can enumerate multiple REF tasks and solve each
 *       independently. Expected results equal two/three separate single-class solves.
 *
 *   B – Shared single-server PS: classes compete at one 1-server PS processor.
 *       Tests the core multiclass queueing logic. Exact oracle: N-class MVA.
 *       Sub-cases isolate symmetric vs asymmetric N, Z, D and 2 vs 3 classes.
 *
 *   C – Shared multi-server PS: same as B but with m > 1 servers at the shared node.
 *       Tests that the multi-server PS residence time formula is applied per-class.
 * </pre>
 */
public class MulticlassLNExamples {

    // =========================================================================
    // Category A: No shared resources (independent classes)
    //
    // Each REF task calls its own dedicated server. No processor is shared
    // across classes. The solver only needs to discover and iterate over
    // multiple REF tasks — no multiclass queueing theory is required yet.
    // =========================================================================

    /**
     * A1 — Two classes, each calling a dedicated INF server (no queueing anywhere).
     *
     * <pre>
     * P1 (INF) — T1 (REF, N=3, Z=2.0) → E3 (service=1.0) on T3 / P2 (INF)
     * P1 (INF) — T2 (REF, N=3, Z=3.0) → E4 (service=0.5) on T4 / P3 (INF)
     * </pre>
     *
     * No queuing at any server. First test to write: the solver must not crash
     * with two REF tasks and must produce a non-zero throughput for each class.
     *
     * Expected results (from LN oracle — TODO: fill in):
     */
    public static LayeredNetwork mc_a1_two_class_inf_servers() throws Exception {
        LayeredNetwork model = new LayeredNetwork("mc_a1");

        Processor P1 = new Processor(model, "P1", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor P2 = new Processor(model, "P2", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor P3 = new Processor(model, "P3", Integer.MAX_VALUE, SchedStrategy.INF);

        Task T1 = new Task(model, "T1", 3, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(2.0));
        Task T2 = new Task(model, "T2", 3, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(3.0));
        Task T3 = new Task(model, "T3", Integer.MAX_VALUE, SchedStrategy.INF).on(P2);
        Task T4 = new Task(model, "T4", Integer.MAX_VALUE, SchedStrategy.INF).on(P3);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        Entry E3 = new Entry(model, "E3").on(T3);
        Entry E4 = new Entry(model, "E4").on(T4);

        new Activity(model, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E3, 1);
        new Activity(model, "A2", Immediate.getInstance()).on(T2).boundTo(E2).synchCall(E4, 1);
        new Activity(model, "A3", Exp.fitMean(1.0)).on(T3).boundTo(E3).repliesTo(E3);
        new Activity(model, "A4", Exp.fitMean(0.5)).on(T4).boundTo(E4).repliesTo(E4);

        return model;
    }

    /**
     * A2 — Two classes, each calling a dedicated 1-server PS (independent queueing per class).
     *
     * <pre>
     * P1 (INF) — T1 (REF, N=3, Z=2.0) → E3 (service=1.0) on T3 / P2 (1-server PS)
     * P1 (INF) — T2 (REF, N=3, Z=3.0) → E4 (service=0.5) on T4 / P3 (1-server PS)
     * </pre>
     *
     * Each class experiences its own independent PS queue. The solver must run
     * per-class MVA correctly. Expected result equals two separate single-class solves.
     *
     * Expected results (from LN oracle — TODO: fill in):
     */
    public static LayeredNetwork mc_a2_two_class_separate_ps() throws Exception {
        LayeredNetwork model = new LayeredNetwork("mc_a2");

        Processor P1 = new Processor(model, "P1", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor P2 = new Processor(model, "P2", 1, SchedStrategy.PS);
        Processor P3 = new Processor(model, "P3", 1, SchedStrategy.PS);

        Task T1 = new Task(model, "T1", 3, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(2.0));
        Task T2 = new Task(model, "T2", 3, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(3.0));
        Task T3 = new Task(model, "T3", Integer.MAX_VALUE, SchedStrategy.INF).on(P2);
        Task T4 = new Task(model, "T4", Integer.MAX_VALUE, SchedStrategy.INF).on(P3);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        Entry E3 = new Entry(model, "E3").on(T3);
        Entry E4 = new Entry(model, "E4").on(T4);

        new Activity(model, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E3, 1);
        new Activity(model, "A2", Immediate.getInstance()).on(T2).boundTo(E2).synchCall(E4, 1);
        new Activity(model, "A3", Exp.fitMean(1.0)).on(T3).boundTo(E3).repliesTo(E3);
        new Activity(model, "A4", Exp.fitMean(0.5)).on(T4).boundTo(E4).repliesTo(E4);

        return model;
    }

    /**
     * A3 — Three classes, each calling a dedicated 1-server PS.
     *
     * <pre>
     * P1 (INF) — T1 (REF, N=3, Z=2.0) → E4 (service=1.0) on T4 / P2 (1-server PS)
     * P1 (INF) — T2 (REF, N=3, Z=3.0) → E5 (service=0.5) on T5 / P3 (1-server PS)
     * P1 (INF) — T3 (REF, N=3, Z=4.0) → E6 (service=0.8) on T6 / P4 (1-server PS)
     * </pre>
     *
     * Confirms the solver's REF-task loop generalizes beyond two classes.
     *
     * Expected results (from LN oracle — TODO: fill in):
     */
    public static LayeredNetwork mc_a3_three_class_separate_ps() throws Exception {
        LayeredNetwork model = new LayeredNetwork("mc_a3");

        Processor P1 = new Processor(model, "P1", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor P2 = new Processor(model, "P2", 1, SchedStrategy.PS);
        Processor P3 = new Processor(model, "P3", 1, SchedStrategy.PS);
        Processor P4 = new Processor(model, "P4", 1, SchedStrategy.PS);

        Task T1 = new Task(model, "T1", 3, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(2.0));
        Task T2 = new Task(model, "T2", 3, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(3.0));
        Task T3 = new Task(model, "T3", 3, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(4.0));
        Task T4 = new Task(model, "T4", Integer.MAX_VALUE, SchedStrategy.INF).on(P2);
        Task T5 = new Task(model, "T5", Integer.MAX_VALUE, SchedStrategy.INF).on(P3);
        Task T6 = new Task(model, "T6", Integer.MAX_VALUE, SchedStrategy.INF).on(P4);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        Entry E3 = new Entry(model, "E3").on(T3);
        Entry E4 = new Entry(model, "E4").on(T4);
        Entry E5 = new Entry(model, "E5").on(T5);
        Entry E6 = new Entry(model, "E6").on(T6);

        new Activity(model, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E4, 1);
        new Activity(model, "A2", Immediate.getInstance()).on(T2).boundTo(E2).synchCall(E5, 1);
        new Activity(model, "A3", Immediate.getInstance()).on(T3).boundTo(E3).synchCall(E6, 1);
        new Activity(model, "A4", Exp.fitMean(1.0)).on(T4).boundTo(E4).repliesTo(E4);
        new Activity(model, "A5", Exp.fitMean(0.5)).on(T5).boundTo(E5).repliesTo(E5);
        new Activity(model, "A6", Exp.fitMean(0.8)).on(T6).boundTo(E6).repliesTo(E6);

        return model;
    }

    /**
     * A4 — Two classes, separate PS servers, asymmetric populations.
     *
     * <pre>
     * P1 (INF) — T1 (REF, N=2, Z=2.0) → E3 (service=1.0) on T3 / P2 (1-server PS)
     * P1 (INF) — T2 (REF, N=8, Z=2.0) → E4 (service=1.0) on T4 / P3 (1-server PS)
     * </pre>
     *
     * Same service demands and think times but very different N. Checks that the
     * solver correctly uses each class's own population in its MVA pass.
     *
     * Expected results (from LN oracle — TODO: fill in):
     */
    public static LayeredNetwork mc_a4_two_class_asymmetric_n() throws Exception {
        LayeredNetwork model = new LayeredNetwork("mc_a4");

        Processor P1 = new Processor(model, "P1", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor P2 = new Processor(model, "P2", 1, SchedStrategy.PS);
        Processor P3 = new Processor(model, "P3", 1, SchedStrategy.PS);

        Task T1 = new Task(model, "T1", 2, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(2.0));
        Task T2 = new Task(model, "T2", 8, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(2.0));
        Task T3 = new Task(model, "T3", Integer.MAX_VALUE, SchedStrategy.INF).on(P2);
        Task T4 = new Task(model, "T4", Integer.MAX_VALUE, SchedStrategy.INF).on(P3);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        Entry E3 = new Entry(model, "E3").on(T3);
        Entry E4 = new Entry(model, "E4").on(T4);

        new Activity(model, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E3, 1);
        new Activity(model, "A2", Immediate.getInstance()).on(T2).boundTo(E2).synchCall(E4, 1);
        new Activity(model, "A3", Exp.fitMean(1.0)).on(T3).boundTo(E3).repliesTo(E3);
        new Activity(model, "A4", Exp.fitMean(1.0)).on(T4).boundTo(E4).repliesTo(E4);

        return model;
    }

    // =========================================================================
    // Category B: Shared single-server PS (genuine multiclass contention)
    //
    // Both/all classes call entries on the same task which runs on a single PS
    // processor. This is the core multiclass case. Sub-cases isolate each source
    // of asymmetry so failures are easy to diagnose. Oracle: N-class closed MVA.
    // =========================================================================

    /**
     * B1 — Two classes, shared 1-server PS, fully symmetric (N1=N2, Z1=Z2, D1=D2).
     *
     * <pre>
     * P1 (INF) — T1 (REF, N=4, Z=2.0) → E3 (service=1.0) ┐
     * P1 (INF) — T2 (REF, N=4, Z=2.0) → E4 (service=1.0) ┘ on T3 / P2 (1-server PS)
     * </pre>
     *
     * Symmetric loads: by symmetry X1=X2 and W1=W2. The simplest sanity check
     * for the shared-server logic.
     *
     * Expected results (from LN oracle — TODO: fill in):
     */
    public static LayeredNetwork mc_b1_two_class_shared_ps_symmetric() throws Exception {
        LayeredNetwork model = new LayeredNetwork("mc_b1");

        Processor P1 = new Processor(model, "P1", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor P2 = new Processor(model, "P2", 1, SchedStrategy.PS);

        Task T1 = new Task(model, "T1", 4, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(2.0));
        Task T2 = new Task(model, "T2", 4, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(2.0));
        Task T3 = new Task(model, "T3", Integer.MAX_VALUE, SchedStrategy.INF).on(P2);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        Entry E3 = new Entry(model, "E3").on(T3);
        Entry E4 = new Entry(model, "E4").on(T3);

        new Activity(model, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E3, 1);
        new Activity(model, "A2", Immediate.getInstance()).on(T2).boundTo(E2).synchCall(E4, 1);
        new Activity(model, "A3", Exp.fitMean(1.0)).on(T3).boundTo(E3).repliesTo(E3);
        new Activity(model, "A4", Exp.fitMean(1.0)).on(T3).boundTo(E4).repliesTo(E4);

        return model;
    }

    /**
     * B2 — Two classes, shared 1-server PS, asymmetric N (D and Z equal).
     *
     * <pre>
     * P1 (INF) — T1 (REF, N=2, Z=3.0) → E3 (service=1.0) ┐
     * P1 (INF) — T2 (REF, N=6, Z=3.0) → E4 (service=1.0) ┘ on T3 / P2 (1-server PS)
     * </pre>
     *
     * Only N differs: tests that the MVA iteration uses each class's own population.
     * Isolates N-asymmetry from service-demand asymmetry.
     *
     * Expected results (from LN oracle — TODO: fill in):
     */
    public static LayeredNetwork mc_b2_two_class_shared_ps_asymmetric_n() throws Exception {
        LayeredNetwork model = new LayeredNetwork("mc_b2");

        Processor P1 = new Processor(model, "P1", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor P2 = new Processor(model, "P2", 1, SchedStrategy.PS);

        Task T1 = new Task(model, "T1", 2, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(3.0));
        Task T2 = new Task(model, "T2", 6, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(3.0));
        Task T3 = new Task(model, "T3", Integer.MAX_VALUE, SchedStrategy.INF).on(P2);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        Entry E3 = new Entry(model, "E3").on(T3);
        Entry E4 = new Entry(model, "E4").on(T3);

        new Activity(model, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E3, 1);
        new Activity(model, "A2", Immediate.getInstance()).on(T2).boundTo(E2).synchCall(E4, 1);
        new Activity(model, "A3", Exp.fitMean(1.0)).on(T3).boundTo(E3).repliesTo(E3);
        new Activity(model, "A4", Exp.fitMean(1.0)).on(T3).boundTo(E4).repliesTo(E4);

        return model;
    }

    /**
     * B3 — Two classes, shared 1-server PS, asymmetric service demands (N and Z equal).
     *
     * <pre>
     * P1 (INF) — T1 (REF, N=5, Z=2.0) → E3 (service=2.0)  ┐
     * P1 (INF) — T2 (REF, N=5, Z=2.0) → E4 (service=0.75) ┘ on T3 / P2 (1-server PS)
     * </pre>
     *
     * Only D differs: tests that the correct per-class demand is passed into the
     * shared-server residence time formula. Isolates D-asymmetry.
     *
     * Expected results (from LN oracle — TODO: fill in):
     */
    public static LayeredNetwork mc_b3_two_class_shared_ps_asymmetric_d() throws Exception {
        LayeredNetwork model = new LayeredNetwork("mc_b3");

        Processor P1 = new Processor(model, "P1", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor P2 = new Processor(model, "P2", 1, SchedStrategy.PS);

        Task T1 = new Task(model, "T1", 5, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(2.0));
        Task T2 = new Task(model, "T2", 5, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(2.0));
        Task T3 = new Task(model, "T3", Integer.MAX_VALUE, SchedStrategy.INF).on(P2);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        Entry E3 = new Entry(model, "E3").on(T3);
        Entry E4 = new Entry(model, "E4").on(T3);

        new Activity(model, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E3, 1);
        new Activity(model, "A2", Immediate.getInstance()).on(T2).boundTo(E2).synchCall(E4, 1);
        new Activity(model, "A3", Exp.fitMean(2.0)).on(T3).boundTo(E3).repliesTo(E3);
        new Activity(model, "A4", Exp.fitMean(0.75)).on(T3).boundTo(E4).repliesTo(E4);

        return model;
    }

    /**
     * B4 — Two classes, shared 1-server PS, fully asymmetric (N, Z, D all differ).
     *
     * <pre>
     * P1 (INF) — T1 (REF, N=3, Z=2.0) → E3 (service=2.0)  ┐
     * P1 (INF) — T2 (REF, N=7, Z=4.0) → E4 (service=0.75) ┘ on T3 / P2 (1-server PS)
     * </pre>
     *
     * Full asymmetry across all parameters. The general 2-class case.
     *
     * Expected results (from LN oracle — TODO: fill in):
     */
    public static LayeredNetwork mc_b4_two_class_shared_ps_fully_asymmetric() throws Exception {
        LayeredNetwork model = new LayeredNetwork("mc_b4");

        Processor P1 = new Processor(model, "P1", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor P2 = new Processor(model, "P2", 1, SchedStrategy.PS);

        Task T1 = new Task(model, "T1", 3, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(2.0));
        Task T2 = new Task(model, "T2", 7, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(4.0));
        Task T3 = new Task(model, "T3", Integer.MAX_VALUE, SchedStrategy.INF).on(P2);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        Entry E3 = new Entry(model, "E3").on(T3);
        Entry E4 = new Entry(model, "E4").on(T3);

        new Activity(model, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E3, 1);
        new Activity(model, "A2", Immediate.getInstance()).on(T2).boundTo(E2).synchCall(E4, 1);
        new Activity(model, "A3", Exp.fitMean(2.0)).on(T3).boundTo(E3).repliesTo(E3);
        new Activity(model, "A4", Exp.fitMean(0.75)).on(T3).boundTo(E4).repliesTo(E4);

        return model;
    }

    /**
     * B5 — Three classes, shared 1-server PS, asymmetric.
     *
     * <pre>
     * P1 (INF) — T1 (REF, N=3, Z=2.0) → E4 (service=1.0)  ┐
     * P1 (INF) — T2 (REF, N=4, Z=3.0) → E5 (service=0.5)  ┤ on T4 / P2 (1-server PS)
     * P1 (INF) — T3 (REF, N=5, Z=4.0) → E6 (service=0.75) ┘
     * </pre>
     *
     * Extends B4 to N=3 classes. Tests that the shared-server logic is truly
     * N-class (not special-cased for 2 classes).
     *
     * Expected results (from LN oracle — TODO: fill in):
     */
    public static LayeredNetwork mc_b5_three_class_shared_ps_asymmetric() throws Exception {
        LayeredNetwork model = new LayeredNetwork("mc_b5");

        Processor P1 = new Processor(model, "P1", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor P2 = new Processor(model, "P2", 1, SchedStrategy.PS);

        Task T1 = new Task(model, "T1", 3, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(2.0));
        Task T2 = new Task(model, "T2", 4, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(3.0));
        Task T3 = new Task(model, "T3", 5, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(4.0));
        Task T4 = new Task(model, "T4", Integer.MAX_VALUE, SchedStrategy.INF).on(P2);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        Entry E3 = new Entry(model, "E3").on(T3);
        Entry E4 = new Entry(model, "E4").on(T4);
        Entry E5 = new Entry(model, "E5").on(T4);
        Entry E6 = new Entry(model, "E6").on(T4);

        new Activity(model, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E4, 1);
        new Activity(model, "A2", Immediate.getInstance()).on(T2).boundTo(E2).synchCall(E5, 1);
        new Activity(model, "A3", Immediate.getInstance()).on(T3).boundTo(E3).synchCall(E6, 1);
        new Activity(model, "A4", Exp.fitMean(1.0)).on(T4).boundTo(E4).repliesTo(E4);
        new Activity(model, "A5", Exp.fitMean(0.5)).on(T4).boundTo(E5).repliesTo(E5);
        new Activity(model, "A6", Exp.fitMean(0.75)).on(T4).boundTo(E6).repliesTo(E6);

        return model;
    }

    public static LayeredNetwork mc_b6_two_class_shared_ps_asymmetric_z() throws Exception {
        LayeredNetwork model = new LayeredNetwork("mc_b6");

        Processor P1 = new Processor(model, "P1", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor P2 = new Processor(model, "P2", 1, SchedStrategy.PS);

        Task T1 = new Task(model, "T1", 2, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(3.0));
        Task T2 = new Task(model, "T2", 6, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(5.0));
        Task T3 = new Task(model, "T3", Integer.MAX_VALUE, SchedStrategy.INF).on(P2);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        Entry E3 = new Entry(model, "E3").on(T3);
        Entry E4 = new Entry(model, "E4").on(T3);

        new Activity(model, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E3, 1);
        new Activity(model, "A2", Immediate.getInstance()).on(T2).boundTo(E2).synchCall(E4, 1);
        new Activity(model, "A3", Exp.fitMean(1.0)).on(T3).boundTo(E3).repliesTo(E3);
        new Activity(model, "A4", Exp.fitMean(1.0)).on(T3).boundTo(E4).repliesTo(E4);

        return model;
    }

    // =========================================================================
    // Category C: Shared multi-server PS (m > 1 servers)
    //
    // Same structure as category B but the shared processor has more than one
    // server. Tests that the per-class residence time formula for multi-server
    // PS is applied correctly (load-dependent service rate).
    // =========================================================================

    /**
     * C1 — Two classes, shared 2-server PS, symmetric.
     *
     * <pre>
     * P1 (INF) — T1 (REF, N=5, Z=2.0) → E3 (service=1.5) ┐
     * P1 (INF) — T2 (REF, N=5, Z=2.0) → E4 (service=1.5) ┘ on T3 / P2 (2-server PS)
     * </pre>
     *
     * Symmetric loads on a 2-server PS. First test for the multi-server residence
     * time formula. By symmetry X1=X2 still holds.
     *
     * Expected results (from LN oracle — TODO: fill in):
     */
    public static LayeredNetwork mc_c1_two_class_shared_2server_ps_symmetric() throws Exception {
        LayeredNetwork model = new LayeredNetwork("mc_c1");

        Processor P1 = new Processor(model, "P1", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor P2 = new Processor(model, "P2", 2, SchedStrategy.PS);

        Task T1 = new Task(model, "T1", 5, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(2.0));
        Task T2 = new Task(model, "T2", 5, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(2.0));
        Task T3 = new Task(model, "T3", Integer.MAX_VALUE, SchedStrategy.INF).on(P2);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        Entry E3 = new Entry(model, "E3").on(T3);
        Entry E4 = new Entry(model, "E4").on(T3);

        new Activity(model, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E3, 1);
        new Activity(model, "A2", Immediate.getInstance()).on(T2).boundTo(E2).synchCall(E4, 1);
        new Activity(model, "A3", Exp.fitMean(1.5)).on(T3).boundTo(E3).repliesTo(E3);
        new Activity(model, "A4", Exp.fitMean(1.5)).on(T3).boundTo(E4).repliesTo(E4);

        return model;
    }

    /**
     * C2 — Two classes, shared 2-server PS, asymmetric demands.
     *
     * <pre>
     * P1 (INF) — T1 (REF, N=4, Z=2.0) → E3 (service=2.0)  ┐
     * P1 (INF) — T2 (REF, N=6, Z=3.0) → E4 (service=0.75) ┘ on T3 / P2 (2-server PS)
     * </pre>
     *
     * Full asymmetry on a 2-server PS. Tests the general multi-server case.
     *
     * Expected results (from LN oracle — TODO: fill in):
     */
    public static LayeredNetwork mc_c2_two_class_shared_2server_ps_asymmetric() throws Exception {
        LayeredNetwork model = new LayeredNetwork("mc_c2");

        Processor P1 = new Processor(model, "P1", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor P2 = new Processor(model, "P2", 2, SchedStrategy.PS);

        Task T1 = new Task(model, "T1", 4, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(2.0));
        Task T2 = new Task(model, "T2", 6, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(3.0));
        Task T3 = new Task(model, "T3", Integer.MAX_VALUE, SchedStrategy.INF).on(P2);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        Entry E3 = new Entry(model, "E3").on(T3);
        Entry E4 = new Entry(model, "E4").on(T3);

        new Activity(model, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E3, 1);
        new Activity(model, "A2", Immediate.getInstance()).on(T2).boundTo(E2).synchCall(E4, 1);
        new Activity(model, "A3", Exp.fitMean(2.0)).on(T3).boundTo(E3).repliesTo(E3);
        new Activity(model, "A4", Exp.fitMean(0.75)).on(T3).boundTo(E4).repliesTo(E4);

        return model;
    }

    /**
     * C3 — Three classes, shared 3-server PS, asymmetric.
     *
     * <pre>
     * P1 (INF) — T1 (REF, N=3, Z=2.0) → E4 (service=1.0)  ┐
     * P1 (INF) — T2 (REF, N=5, Z=3.0) → E5 (service=0.5)  ┤ on T4 / P2 (3-server PS)
     * P1 (INF) — T3 (REF, N=4, Z=4.0) → E6 (service=0.75) ┘
     * </pre>
     *
     * Three classes on a 3-server PS. Tests that both the N-class generalization
     * and the multi-server formula work together.
     *
     * Expected results (from LN oracle — TODO: fill in):
     */
    public static LayeredNetwork mc_c3_three_class_shared_3server_ps_asymmetric() throws Exception {
        LayeredNetwork model = new LayeredNetwork("mc_c3");

        Processor P1 = new Processor(model, "P1", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor P2 = new Processor(model, "P2", 3, SchedStrategy.PS);

        Task T1 = new Task(model, "T1", 3, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(2.0));
        Task T2 = new Task(model, "T2", 5, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(3.0));
        Task T3 = new Task(model, "T3", 4, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(4.0));
        Task T4 = new Task(model, "T4", Integer.MAX_VALUE, SchedStrategy.INF).on(P2);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        Entry E3 = new Entry(model, "E3").on(T3);
        Entry E4 = new Entry(model, "E4").on(T4);
        Entry E5 = new Entry(model, "E5").on(T4);
        Entry E6 = new Entry(model, "E6").on(T4);

        new Activity(model, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E4, 1);
        new Activity(model, "A2", Immediate.getInstance()).on(T2).boundTo(E2).synchCall(E5, 1);
        new Activity(model, "A3", Immediate.getInstance()).on(T3).boundTo(E3).synchCall(E6, 1);
        new Activity(model, "A4", Exp.fitMean(1.0)).on(T4).boundTo(E4).repliesTo(E4);
        new Activity(model, "A5", Exp.fitMean(0.5)).on(T4).boundTo(E5).repliesTo(E5);
        new Activity(model, "A6", Exp.fitMean(0.75)).on(T4).boundTo(E6).repliesTo(E6);

        return model;
    }
}
