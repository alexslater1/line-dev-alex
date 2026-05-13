package jline.solvers.ln_simple;

import jline.GlobalConstants;
import jline.VerboseLevel;
import jline.lang.constant.SchedStrategy;
import jline.lang.layered.*;
import jline.lang.processes.Exp;
import jline.lang.processes.Immediate;
import jline.solvers.ln_simple.fixtures.MulticlassLNExamples;
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

    @Test @Timeout(120)
    public void testMcA1_TwoClassInfServers() throws Exception {
        assertResultsMatchSolverLN(MulticlassLNExamples.mc_a1_two_class_inf_servers());
    }

    @Test @Timeout(120)
    public void testMcA2_TwoClassSeparatePs() throws Exception {
        assertResultsMatchSolverLN(MulticlassLNExamples.mc_a2_two_class_separate_ps());
    }

    @Test @Timeout(120)
    public void testMcA3_ThreeClassSeparatePs() throws Exception {
        assertResultsMatchSolverLN(MulticlassLNExamples.mc_a3_three_class_separate_ps());
    }

    @Test @Timeout(120)
    public void testMcA4_TwoClassAsymmetricN() throws Exception {
        assertResultsMatchSolverLN(MulticlassLNExamples.mc_a4_two_class_asymmetric_n());
    }

    @Test @Timeout(120)
    public void testMcB1_TwoClassSharedPsSymmetric() throws Exception {
        assertResultsMatchSolverLN(MulticlassLNExamples.mc_b1_two_class_shared_ps_symmetric());
    }

    @Test @Timeout(120)
    public void testMcB2_TwoClassSharedPsAsymmetricN() throws Exception {
        assertResultsMatchSolverLN(MulticlassLNExamples.mc_b2_two_class_shared_ps_asymmetric_n());
    }

    @Test @Timeout(120)
    public void testMcB3_TwoClassSharedPsAsymmetricD() throws Exception {
        assertResultsMatchSolverLN(MulticlassLNExamples.mc_b3_two_class_shared_ps_asymmetric_d());
    }

    @Test @Timeout(120)
    public void testMcB4_TwoClassSharedPsFullyAsymmetric() throws Exception {
        assertResultsMatchSolverLN(MulticlassLNExamples.mc_b4_two_class_shared_ps_fully_asymmetric());
    }

    @Test @Timeout(120)
    public void testMcB5_ThreeClassSharedPsAsymmetric() throws Exception {
        assertResultsMatchSolverLN(MulticlassLNExamples.mc_b5_three_class_shared_ps_asymmetric());
    }

    @Test @Timeout(120)
    public void testMcB6_TwoClassSharedPsAsymmetric() throws Exception {
        assertResultsMatchSolverLN(MulticlassLNExamples.mc_b6_two_class_shared_ps_asymmetric_z());
    }

    @Test @Timeout(120)
    public void testMcC1_TwoClassShared2ServerPsSymmetric() throws Exception {
        assertResultsMatchSolverLN(MulticlassLNExamples.mc_c1_two_class_shared_2server_ps_symmetric());
    }

    @Test @Timeout(120)
    public void testMcC2_TwoClassShared2ServerPsAsymmetric() throws Exception {
        assertResultsMatchSolverLN(MulticlassLNExamples.mc_c2_two_class_shared_2server_ps_asymmetric());
    }

    @Test @Timeout(120)
    public void testMcC3_ThreeClassShared3ServerPsAsymmetric() throws Exception {
        assertResultsMatchSolverLN(MulticlassLNExamples.mc_c3_three_class_shared_3server_ps_asymmetric());
    }

    @Test
    @Timeout(120)
    public void testScenario1SimpleChain() throws Exception {
        assertResultsMatchSolverLN(SingleclassLNExamples.sc1_simple_chain());
    }

    @Test
    @Timeout(120)
    public void testScenario3Fanout() throws Exception {
        assertResultsMatchSolverLN(SingleclassLNExamples.sc3_fanout());
    }


    @Test
    @Timeout(120)
    public void testScenario4Serial() throws Exception {
        assertResultsMatchSolverLN(SingleclassLNExamples.sc4_serial());
    }


    @Test
    @Timeout(120)
    public void testScenario5Loop() throws Exception {
        assertResultsMatchSolverLN(SingleclassLNExamples.sc5_loop());
    }

    @Test
    @Timeout(120)
    public void testScenario7Multicore() throws Exception {
        assertResultsMatchSolverLN(SingleclassLNExamples.sc7_multicore());
    }

    @Test
    @Timeout(120)
    public void testScenario8Immediate() throws Exception {
        assertResultsMatchSolverLN(SingleclassLNExamples.sc8_immediate());
    }


    @Test
    @Timeout(120)
    public void testScenario9TwoCalls() throws Exception {
        assertResultsMatchSolverLN(SingleclassLNExamples.sc9_two_calls());
    }
}
