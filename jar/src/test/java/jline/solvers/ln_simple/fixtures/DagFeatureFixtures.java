package jline.solvers.ln_simple.fixtures;

import jline.lang.constant.SchedStrategy;
import jline.lang.layered.*;
import jline.lang.processes.Exp;
import jline.lang.processes.Immediate;
import jline.util.matrix.Matrix;

import java.util.Arrays;

/**
 * LQN fixtures that exercise activity-graph features beyond the flat
 * bag-of-activities model SolverLNSimple historically supported.
 *
 * <p>Each feature has a <i>primary</i> fixture exercising the basic case
 * plus an <i>edge-case variant</i> that probes a different code path on
 * the same feature. Integration is covered by {@link #D_combinedDag()}.
 *
 * <ul>
 *   <li>Sequence past bound activity: {@link #D_seqEntry()},
 *       {@link #D_seqEntry_multiEntry()}</li>
 *   <li>OR_FORK branch probabilities: {@link #D_orForkUneven()},
 *       {@link #D_orForkThreeWay()}</li>
 *   <li>POST_LOOP with cross-task call: {@link #D_postLoopCall()},
 *       {@link #D_postLoopHost()}</li>
 *   <li>AND_FORK / AND_JOIN max-aggregation: {@link #D_andForkJoin()},
 *       {@link #D_andForkUnequal()}</li>
 *   <li>REPLY phase split: {@link #D_replyFirstPhase()},
 *       {@link #D_replyAtBound()}</li>
 *   <li>Integration: {@link #D_combinedDag()}</li>
 * </ul>
 *
 * <p>LQNS is the numerical oracle; SolverLN is the secondary reference.
 * {@code SolverLNSimpleDagTest} asserts SolverLNSimple matches at least
 * one within the standard {@code atol=1e-3, rtol=5e-2} tolerance.
 */
public final class DagFeatureFixtures {

    private DagFeatureFixtures() {}

    // =========================================================================
    //  Sequence past bound activity
    // =========================================================================

    /**
     * Three-activity sequence on a single-entry callee.
     * <pre>
     *   T1 (REF, N=20, Z=2) --calls(1)--> E2
     *   T2 (INF):
     *     E2 -> A_seq1(D=0.3) -> A_seq2(D=0.5) -> A_seq3(D=0.4, reply)
     * </pre>
     * Per-call demand on T2 = 1.2; current code reads only A_seq1's demand
     * (0.3) via {@code hostDemandOfBoundActivity}, biasing T1.RespT low.
     */
    public static LayeredNetwork D_seqEntry() {
        LayeredNetwork m = new LayeredNetwork("D_seqEntry");
        Processor PR = new Processor(m, "PR", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PS = new Processor(m, "PS", 4, SchedStrategy.PS);

        Task T1 = new Task(m, "T1", 20, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(2.0));
        Task T2 = new Task(m, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(PS);

        Entry E1 = new Entry(m, "E1").on(T1);
        Entry E2 = new Entry(m, "E2").on(T2);

        new Activity(m, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);

        Activity A_seq1 = new Activity(m, "A_seq1", Exp.fitMean(0.3)).on(T2).boundTo(E2);
        Activity A_seq2 = new Activity(m, "A_seq2", Exp.fitMean(0.5)).on(T2);
        Activity A_seq3 = new Activity(m, "A_seq3", Exp.fitMean(0.4)).on(T2).repliesTo(E2);

        T2.addPrecedence(ActivityPrecedence.Serial(A_seq1, A_seq2, A_seq3));
        return m;
    }

    /**
     * Two-entry callee, each entry has its own sequence past its bound
     * activity. Exercises the multi-entry branch in
     * {@code setPerCallerDemand} / {@code callerDemandOnTask}, where each
     * visit dispatches to one entry's bound activity and the per-visit
     * demand is averaged across entries.
     * <pre>
     *   T1 (REF, N=15, Z=2) --calls(1)--> E2A
     *   T1 also --calls(1)--> E2B (via second activity)
     *   T2 (INF):
     *     E2A -> A_a1(D=0.2) -> A_a2(D=0.4, reply to E2A)
     *     E2B -> A_b1(D=0.3) -> A_b2(D=0.5, reply to E2B)
     * </pre>
     */
    public static LayeredNetwork D_seqEntry_multiEntry() {
        LayeredNetwork m = new LayeredNetwork("D_seqEntry_multiEntry");
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

    // =========================================================================
    //  OR_FORK branch probabilities
    // =========================================================================

    /**
     * Two-branch OR_FORK with 0.7 / 0.3 probabilities. Per-call demand =
     * 0.7×1.0 + 0.3×0.2 = 0.76; current code with no branch-prob handling
     * sees demand 0 (the bound activity is Immediate) or 1.2 (bag sum) at
     * different sites, both wrong.
     */
    public static LayeredNetwork D_orForkUneven() {
        LayeredNetwork m = new LayeredNetwork("D_orForkUneven");
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

        Matrix probs = new Matrix(new double[][]{{0.7, 0.3}});
        T2.addPrecedence(ActivityPrecedence.OrFork(A_or, Arrays.asList(A_x, A_y), probs));
        return m;
    }

    /**
     * Three-branch OR_FORK with 0.5 / 0.3 / 0.2 probabilities. Exercises
     * iteration over postParams beyond the 2-element case.
     */
    public static LayeredNetwork D_orForkThreeWay() {
        LayeredNetwork m = new LayeredNetwork("D_orForkThreeWay");
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

        Matrix probs = new Matrix(new double[][]{{0.5, 0.3, 0.2}});
        T2.addPrecedence(ActivityPrecedence.OrFork(A_or,
                Arrays.asList(A_b1, A_b2, A_b3), probs));
        return m;
    }

    // =========================================================================
    //  POST_LOOP
    // =========================================================================

    /**
     * POST_LOOP wrapping an activity that issues a sync call.
     * <pre>
     *   T2: E2 -> A_head (D=0) -Loop(3)-> [A_body(D=0, syncCall E3),
     *                                       A_loop_end(D=0, reply)]
     *   T3: E3 -> A_leaf (D=0.2, reply)
     * </pre>
     * Each visit to T2 makes 3 calls to T3; current code with no loop
     * multiplier on cross-task call accounting reports T3.tput as 1×T2.tput
     * instead of 3×.
     */
    public static LayeredNetwork D_postLoopCall() {
        LayeredNetwork m = new LayeredNetwork("D_postLoopCall");
        Processor PR = new Processor(m, "PR", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor P2 = new Processor(m, "P2", 4, SchedStrategy.PS);
        Processor P3 = new Processor(m, "P3", 4, SchedStrategy.PS);

        Task T1 = new Task(m, "T1", 10, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(2.0));
        Task T2 = new Task(m, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(P2);
        Task T3 = new Task(m, "T3", Integer.MAX_VALUE, SchedStrategy.INF).on(P3);

        Entry E1 = new Entry(m, "E1").on(T1);
        Entry E2 = new Entry(m, "E2").on(T2);
        Entry E3 = new Entry(m, "E3").on(T3);

        new Activity(m, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);

        new Activity(m, "A_head", Immediate.getInstance()).on(T2).boundTo(E2);
        new Activity(m, "A_body", Immediate.getInstance()).on(T2).synchCall(E3, 1);
        new Activity(m, "A_loop_end", Immediate.getInstance()).on(T2).repliesTo(E2);

        T2.addPrecedence(ActivityPrecedence.Loop("A_head",
                Arrays.asList("A_body", "A_loop_end"), Matrix.singleton(3)));

        new Activity(m, "A_leaf", Exp.fitMean(0.2)).on(T3).boundTo(E3).repliesTo(E3);
        return m;
    }

    /**
     * Direct multi-call to one entry with no POST_LOOP — A_only issues 3
     * calls to E3 via {@code synchCall(E3, 3)}. Functionally equivalent to
     * {@link #D_postLoopCall()} (3 calls to T3 per T2 visit) but exercises
     * the general {@code cm > 1} path instead of POST_LOOP visit-weight
     * propagation. If this passes while {@code D_postLoopCall} fails, the
     * bug is in POST_LOOP-specific handling.
     */
    public static LayeredNetwork D_multiCallToOneEntry() {
        LayeredNetwork m = new LayeredNetwork("D_multiCallToOneEntry");
        Processor PR = new Processor(m, "PR", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor P2 = new Processor(m, "P2", 4, SchedStrategy.PS);
        Processor P3 = new Processor(m, "P3", 4, SchedStrategy.PS);

        Task T1 = new Task(m, "T1", 10, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(2.0));
        Task T2 = new Task(m, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(P2);
        Task T3 = new Task(m, "T3", Integer.MAX_VALUE, SchedStrategy.INF).on(P3);

        Entry E1 = new Entry(m, "E1").on(T1);
        Entry E2 = new Entry(m, "E2").on(T2);
        Entry E3 = new Entry(m, "E3").on(T3);

        new Activity(m, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);
        new Activity(m, "A_only", Immediate.getInstance()).on(T2).boundTo(E2)
                .synchCall(E3, 3).repliesTo(E2);
        new Activity(m, "A_leaf", Exp.fitMean(0.2)).on(T3).boundTo(E3).repliesTo(E3);
        return m;
    }

    /**
     * Same topology as {@link #D_postLoopCall} but with loop count 2 instead
     * of 3 — confirms the bug scales with loop count and isolates it from
     * the specific value of 3.
     */
    public static LayeredNetwork D_postLoopCall2x() {
        LayeredNetwork m = new LayeredNetwork("D_postLoopCall2x");
        Processor PR = new Processor(m, "PR", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor P2 = new Processor(m, "P2", 4, SchedStrategy.PS);
        Processor P3 = new Processor(m, "P3", 4, SchedStrategy.PS);

        Task T1 = new Task(m, "T1", 10, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(2.0));
        Task T2 = new Task(m, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(P2);
        Task T3 = new Task(m, "T3", Integer.MAX_VALUE, SchedStrategy.INF).on(P3);

        Entry E1 = new Entry(m, "E1").on(T1);
        Entry E2 = new Entry(m, "E2").on(T2);
        Entry E3 = new Entry(m, "E3").on(T3);

        new Activity(m, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);

        new Activity(m, "A_head", Immediate.getInstance()).on(T2).boundTo(E2);
        new Activity(m, "A_body", Immediate.getInstance()).on(T2).synchCall(E3, 1);
        new Activity(m, "A_loop_end", Immediate.getInstance()).on(T2).repliesTo(E2);

        T2.addPrecedence(ActivityPrecedence.Loop("A_head",
                Arrays.asList("A_body", "A_loop_end"), Matrix.singleton(2)));

        new Activity(m, "A_leaf", Exp.fitMean(0.2)).on(T3).boundTo(E3).repliesTo(E3);
        return m;
    }

    /**
     * POST_LOOP wrapping sync calls where T2 also has its own host demand —
     * isolates whether the bug interacts with non-zero T2 host work.
     * <pre>
     *   T2: E2 -> A_head (D=0.1) -Loop(3)-> [A_body (D=0.05, syncCall E3 ×1),
     *                                         A_loop_end (D=0, reply)]
     * </pre>
     */
    public static LayeredNetwork D_postLoopCallWithT2Demand() {
        LayeredNetwork m = new LayeredNetwork("D_postLoopCallWithT2Demand");
        Processor PR = new Processor(m, "PR", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor P2 = new Processor(m, "P2", 4, SchedStrategy.PS);
        Processor P3 = new Processor(m, "P3", 4, SchedStrategy.PS);

        Task T1 = new Task(m, "T1", 10, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(2.0));
        Task T2 = new Task(m, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(P2);
        Task T3 = new Task(m, "T3", Integer.MAX_VALUE, SchedStrategy.INF).on(P3);

        Entry E1 = new Entry(m, "E1").on(T1);
        Entry E2 = new Entry(m, "E2").on(T2);
        Entry E3 = new Entry(m, "E3").on(T3);

        new Activity(m, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);

        new Activity(m, "A_head", Exp.fitMean(0.1)).on(T2).boundTo(E2);
        new Activity(m, "A_body", Exp.fitMean(0.05)).on(T2).synchCall(E3, 1);
        new Activity(m, "A_loop_end", Immediate.getInstance()).on(T2).repliesTo(E2);

        T2.addPrecedence(ActivityPrecedence.Loop("A_head",
                Arrays.asList("A_body", "A_loop_end"), Matrix.singleton(3)));

        new Activity(m, "A_leaf", Exp.fitMean(0.2)).on(T3).boundTo(E3).repliesTo(E3);
        return m;
    }

    /**
     * POST_LOOP wrapping host-demand work (no sync call). Exercises the
     * loop-count multiplier on local host demand independently of the
     * cross-task call accounting path.
     * <pre>
     *   T1 (REF, N=15, Z=2) --calls(1)--> E2
     *   T2 (INF):
     *     E2 -> A_head (D=0) -Loop(4)-> [A_body(D=0.3),
     *                                     A_loop_end(D=0, reply)]
     * </pre>
     * Per-call demand on T2 = 4×0.3 = 1.2; bag-sum gives 0.3 (just A_body
     * counted once) or treats A_head as the loop activity (current bug).
     */
    public static LayeredNetwork D_postLoopHost() {
        LayeredNetwork m = new LayeredNetwork("D_postLoopHost");
        Processor PR = new Processor(m, "PR", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PS = new Processor(m, "PS", 4, SchedStrategy.PS);

        Task T1 = new Task(m, "T1", 15, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(2.0));
        Task T2 = new Task(m, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(PS);

        Entry E1 = new Entry(m, "E1").on(T1);
        Entry E2 = new Entry(m, "E2").on(T2);

        new Activity(m, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);

        new Activity(m, "A_head", Immediate.getInstance()).on(T2).boundTo(E2);
        new Activity(m, "A_body", Exp.fitMean(0.3)).on(T2);
        new Activity(m, "A_loop_end", Immediate.getInstance()).on(T2).repliesTo(E2);

        T2.addPrecedence(ActivityPrecedence.Loop("A_head",
                Arrays.asList("A_body", "A_loop_end"), Matrix.singleton(4)));
        return m;
    }

    // =========================================================================
    //  AND_FORK / AND_JOIN
    // =========================================================================

    /**
     * AND_FORK + AND_JOIN, two parallel branches of equal demand on INF
     * hosts. The join completes in max(1.0, 1.0)=1.0; current code sums
     * activity demands so caller perceives R(T2)≈2.0.
     */
    public static LayeredNetwork D_andForkJoin() {
        LayeredNetwork m = new LayeredNetwork("D_andForkJoin");
        Processor PR = new Processor(m, "PR", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PS = new Processor(m, "PS", Integer.MAX_VALUE, SchedStrategy.INF);

        Task T1 = new Task(m, "T1", 20, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(2.0));
        Task T2 = new Task(m, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(PS);

        Entry E1 = new Entry(m, "E1").on(T1);
        Entry E2 = new Entry(m, "E2").on(T2);

        new Activity(m, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);

        Activity A_fork = new Activity(m, "A_fork", Immediate.getInstance()).on(T2).boundTo(E2);
        Activity A_p1   = new Activity(m, "A_p1",   Exp.fitMean(1.0)).on(T2);
        Activity A_p2   = new Activity(m, "A_p2",   Exp.fitMean(1.0)).on(T2);
        Activity A_join = new Activity(m, "A_join", Immediate.getInstance()).on(T2).repliesTo(E2);

        T2.addPrecedence(ActivityPrecedence.AndFork(A_fork, Arrays.asList(A_p1, A_p2)));
        T2.addPrecedence(ActivityPrecedence.AndJoin(Arrays.asList(A_p1, A_p2), A_join));
        return m;
    }

    /**
     * AND_FORK with unequal branches: max should pick the larger (1.0),
     * not the smaller (0.3) or the sum (1.3). Equal-branch fixtures alone
     * don't prove max-aggregation is happening — this one does.
     */
    public static LayeredNetwork D_andForkUnequal() {
        LayeredNetwork m = new LayeredNetwork("D_andForkUnequal");
        Processor PR = new Processor(m, "PR", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PS = new Processor(m, "PS", Integer.MAX_VALUE, SchedStrategy.INF);

        Task T1 = new Task(m, "T1", 15, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(2.0));
        Task T2 = new Task(m, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(PS);

        Entry E1 = new Entry(m, "E1").on(T1);
        Entry E2 = new Entry(m, "E2").on(T2);

        new Activity(m, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);

        Activity A_fork  = new Activity(m, "A_fork",  Immediate.getInstance()).on(T2).boundTo(E2);
        Activity A_slow  = new Activity(m, "A_slow",  Exp.fitMean(1.0)).on(T2);
        Activity A_fast  = new Activity(m, "A_fast",  Exp.fitMean(0.3)).on(T2);
        Activity A_join  = new Activity(m, "A_join",  Immediate.getInstance()).on(T2).repliesTo(E2);

        T2.addPrecedence(ActivityPrecedence.AndFork(A_fork, Arrays.asList(A_slow, A_fast)));
        T2.addPrecedence(ActivityPrecedence.AndJoin(Arrays.asList(A_slow, A_fast), A_join));
        return m;
    }

    // =========================================================================
    //  REPLY phase split
    // =========================================================================

    /**
     * REPLY in the middle of a sequence — second-phase work is background.
     * Caller-perceived demand = phase 1 (0.6); processor host demand = full
     * sequence (0.9). Current code folds phase 2 into caller R.
     */
    public static LayeredNetwork D_replyFirstPhase() {
        LayeredNetwork m = new LayeredNetwork("D_replyFirstPhase");
        Processor PR = new Processor(m, "PR", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PS = new Processor(m, "PS", 1, SchedStrategy.PS);

        Task T1 = new Task(m, "T1", 10, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(2.0));
        Task T2 = new Task(m, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(PS);

        Entry E1 = new Entry(m, "E1").on(T1);
        Entry E2 = new Entry(m, "E2").on(T2);

        new Activity(m, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);

        Activity A_p1a   = new Activity(m, "A_p1a",   Exp.fitMean(0.4)).on(T2).boundTo(E2);
        Activity A_reply = new Activity(m, "A_reply", Exp.fitMean(0.2)).on(T2).repliesTo(E2);
        Activity A_p2    = new Activity(m, "A_p2",    Exp.fitMean(0.3)).on(T2).setPhase(2);

        T2.addPrecedence(ActivityPrecedence.Serial(A_p1a, A_reply, A_p2));
        return m;
    }

    /**
     * REPLY at the bound activity — phase 1 is just the bound activity;
     * everything else is phase-2 background. Caller-perceived demand drops
     * to bound-activity demand only.
     * <pre>
     *   T2: E2 -> A_bound(D=0.2, replies to E2)
     *           -> A_bg1(D=0.4) -> A_bg2(D=0.5)   // background work
     * </pre>
     * Phase 1 = 0.2; phase 2 = 0.9; processor host demand = 1.1.
     */
    public static LayeredNetwork D_replyAtBound() {
        LayeredNetwork m = new LayeredNetwork("D_replyAtBound");
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

    // =========================================================================
    //  Integration
    // =========================================================================

    /**
     * Sequence + POST_LOOP + OR_FORK + AND_FORK + REPLY in one entry DAG.
     * <pre>
     *   T2 (PS-1):
     *     E2 -> A_init(D=0.1) -> A_loop_head(D=0)
     *     A_loop_head -Loop(2)-> [A_loop_body(D=0.2), A_loop_end(D=0)]
     *     A_loop_end -OrFork(0.7/0.3)-> [A_or_x(D=0.3), A_or_y(D=0.1)]
     *     [A_or_x, A_or_y] -OrJoin-> A_after_or(D=0)
     *     A_after_or -AndFork-> [A_and_p1(D=0.4), A_and_p2(D=0.4)]
     *     [A_and_p1, A_and_p2] -AndJoin-> A_join(D=0.1, replies to E2)
     *     A_join -> A_phase2(D=0.3)        // background
     * </pre>
     */
    public static LayeredNetwork D_combinedDag() {
        LayeredNetwork m = new LayeredNetwork("D_combinedDag");
        Processor PR = new Processor(m, "PR", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PS = new Processor(m, "PS", 1, SchedStrategy.PS);

        Task T1 = new Task(m, "T1", 10, SchedStrategy.REF).on(PR).setThinkTime(Exp.fitMean(2.0));
        Task T2 = new Task(m, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(PS);

        Entry E1 = new Entry(m, "E1").on(T1);
        Entry E2 = new Entry(m, "E2").on(T2);

        new Activity(m, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);

        Activity A_init      = new Activity(m, "A_init",      Exp.fitMean(0.1)).on(T2).boundTo(E2);
        Activity A_loop_head = new Activity(m, "A_loop_head", Immediate.getInstance()).on(T2);
        Activity A_loop_body = new Activity(m, "A_loop_body", Exp.fitMean(0.2)).on(T2);
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

        Matrix orProbs = new Matrix(new double[][]{{0.7, 0.3}});
        T2.addPrecedence(ActivityPrecedence.OrFork(A_loop_end,
                Arrays.asList(A_or_x, A_or_y), orProbs));
        T2.addPrecedence(ActivityPrecedence.OrJoin(Arrays.asList(A_or_x, A_or_y), A_after_or));

        T2.addPrecedence(ActivityPrecedence.AndFork(A_after_or, Arrays.asList(A_and_p1, A_and_p2)));
        T2.addPrecedence(ActivityPrecedence.AndJoin(Arrays.asList(A_and_p1, A_and_p2), A_join));

        T2.addPrecedence(ActivityPrecedence.Serial(A_join, A_phase2));
        return m;
    }
}
