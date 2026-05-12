package jline.solvers.ln_simple;

import jline.GlobalConstants;
import jline.VerboseLevel;
import jline.lang.constant.SchedStrategy;
import jline.lang.layered.*;
import jline.lang.processes.Exp;
import jline.lang.processes.Immediate;
import jline.solvers.ln_simple.fixtures.SingleclassLNExamples;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.Timeout;

import static jline.solvers.ln_simple.util.assertResultsMatchSolverLN;

public class SolverLNSimpleTest {
    //PASSING TESTS - USED TO MAKE SURE ANY NEW CHANGES DONT BREAK THE SOLVER

    @BeforeAll
    public static void setUp() {
        GlobalConstants.setVerbose(VerboseLevel.SILENT);
    }

    @Test
    @Timeout(120)
    public void testTwoLayerSingleProcessorNetwork() {
        assertResultsMatchSolverLN(
                SingleclassLNExamples.twoLayerSingleProcessorNetwork());
    }

    @Test
    @Timeout(120)
    public void testThreeLayerSingleProcessorNetwork() {
        assertResultsMatchSolverLN(
                SingleclassLNExamples.threeLayerSingleProcessorNetwork());
    }

    @Test
    @Timeout(120)
    public void testTwoLayerMultiProcessorNetwork() {
        assertResultsMatchSolverLN(
                SingleclassLNExamples.twoLayerMultiProcessorNetwork());
    }

    @Test
    @Timeout(120)
    public void testThreeLayerMultiProcessorNetwork() {
        assertResultsMatchSolverLN(
                SingleclassLNExamples.threeLayerMultiProcessorNetwork());
    }

    @Test
    @Timeout(120)
    public void testTwoLayerSingleProcessorNetworkUPDATED() {
        assertResultsMatchSolverLN(
                SingleclassLNExamples.twoLayerSingleProcessorNetworkUPDATED());
    }

    @Test
    @Timeout(120)
    public void testTwoLayerSingleProcessorNetworkUPDATED2() {
        assertResultsMatchSolverLN(
                SingleclassLNExamples.twoLayerSingleProcessorNetworkUPDATED2());
    }

    @Test
    @Timeout(120)
    public void testThreeLayerSingleProcessorNetworkUPDATED() {
        assertResultsMatchSolverLN(
                SingleclassLNExamples.threeLayerSingleProcessorNetworkUPDATED());
    }

    @Test
    @Timeout(120)
    public void testBug1Network1() {
        assertResultsMatchSolverLN(SingleclassLNExamples.bug1network1());
    }

    @Test
    @Timeout(120)
    public void testBug1Network2() {
        assertResultsMatchSolverLN(SingleclassLNExamples.bug1network2());
    }

    @Test
    @Timeout(120)
    public void testBug1Network3() {
        assertResultsMatchSolverLN(SingleclassLNExamples.bug1network3());
    }

    @Test
    @Timeout(120)
    public void testBug2Network1() {
        assertResultsMatchSolverLN(SingleclassLNExamples.bug2network1());
    }

    @Test
    @Timeout(120)
    public void testBug2Network2() {
        assertResultsMatchSolverLN(SingleclassLNExamples.bug2network2());
    }

    @Test
    @Timeout(120)
    public void testBug2Network3() {
        assertResultsMatchSolverLN(SingleclassLNExamples.bug2network3());
    }

    @Test
    @Timeout(120)
    public void testMultiCall() {
        assertResultsMatchSolverLN(SingleclassLNExamples.multiCallNetwork());
    }

    @Test
    @Timeout(120)
    public void testBug4OnlyLast() {
        assertResultsMatchSolverLN(SingleclassLNExamples.bug4network1());
    }

    @Test
    @Timeout(120)
    public void testBug4FirstAndLast() {
        assertResultsMatchSolverLN(SingleclassLNExamples.bug4network2());
    }

        @Test
    @Timeout(120)
    public void testBug3EqualAbove() {
        assertResultsMatchSolverLN(SingleclassLNExamples.bug3network1());
    }

    @Test
    @Timeout(120)
    public void testBug3EqualBoth() {
        assertResultsMatchSolverLN(SingleclassLNExamples.bug3network2());
    }

    @Test
    @Timeout(120)
    public void testBug3NotEqual1() {
        assertResultsMatchSolverLN(SingleclassLNExamples.bug3network3());
    }

    @Test
    @Timeout(120)
    public void testBug3NotEqual2() {
        assertResultsMatchSolverLN(SingleclassLNExamples.bug3network4());
    }

    @Test
    @Timeout(120)
    public void testBugNotEqualButPasses() {
        assertResultsMatchSolverLN(SingleclassLNExamples.bug3network5());
    }

    @Test
    @Timeout(120)
    public void testBugDecreasingTaskMultiplicity() {
        LayeredNetwork model = new LayeredNetwork("Decreasing Task Multiplicity (Failing)");

        Processor P1 = new Processor(model, "P1", 2, SchedStrategy.PS);
        Processor P2 = new Processor(model, "P2", 2, SchedStrategy.PS);
        Processor P3 = new Processor(model, "P3", 2, SchedStrategy.PS);

        Task T1 = new Task(model, "T1", 3, SchedStrategy.REF).on(P1);
        Task T2 = new Task(model, "T2", 2, SchedStrategy.FCFS).on(P2);
        Task T3 = new Task(model, "T3", 1, SchedStrategy.FCFS).on(P3);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        Entry E3 = new Entry(model, "E3").on(T3);

        new Activity(model, "AS1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);
        new Activity(model, "AS2", Immediate.getInstance()).on(T2).boundTo(E2).synchCall(E3, 1).repliesTo(E1);
        new Activity(model, "AS3", Exp.fitMean(0.9)).on(T3).boundTo(E3).repliesTo(E2);

        assertResultsMatchSolverLN(model);
    }

    @Test
    @Timeout(120)
    public void testDebugWrong() {
        LayeredNetwork model = new LayeredNetwork("sc2");
        Processor P1 = new Processor(model, "P1", 50, SchedStrategy.PS);
        Processor P2 = new Processor(model, "P2", 50, SchedStrategy.PS);
        Processor P3 = new Processor(model, "P3", 50, SchedStrategy.PS);
        Task T1 = new Task(model, "T1", 50, SchedStrategy.REF).on(P1).setThinkTime(1);
        Task T2 = new Task(model, "T2", 2, SchedStrategy.FCFS).on(P2);
        Task T3 = new Task(model, "T3", 50, SchedStrategy.FCFS).on(P3);
        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        Entry E3 = new Entry(model, "E3").on(T3);
        new Activity(model, "A1", Exp.fitMean(1e-6)).on(T1).boundTo(E1).synchCall(E2, 1);
        new Activity(model, "A2", Exp.fitMean(1e-6)).on(T2).boundTo(E2).synchCall(E3, 1).repliesTo(E1);
        new Activity(model, "A3", Exp.fitMean(0.5)).on(T3).boundTo(E3).repliesTo(E2);

        assertResultsMatchSolverLN(model);
    }
}
