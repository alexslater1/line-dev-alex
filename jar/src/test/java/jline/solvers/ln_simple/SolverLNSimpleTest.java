package jline.solvers.ln_simple;

import jline.GlobalConstants;
import jline.VerboseLevel;
import jline.solvers.ln_simple.utils.LayeredNetworkTestExamples;
import jline.solvers.ln_simple.utils.SolverLnSimpleBugModels;
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
                LayeredNetworkTestExamples.twoLayerSingleProcessorNetwork());
    }

    @Test
    @Timeout(120)
    public void testThreeLayerSingleProcessorNetwork() {
        assertResultsMatchSolverLN(
                LayeredNetworkTestExamples.threeLayerSingleProcessorNetwork());
    }

    @Test
    @Timeout(120)
    public void testTwoLayerMultiProcessorNetwork() {
        assertResultsMatchSolverLN(
                LayeredNetworkTestExamples.twoLayerMultiProcessorNetwork());
    }

    @Test
    @Timeout(120)
    public void testThreeLayerMultiProcessorNetwork() {
        assertResultsMatchSolverLN(
                LayeredNetworkTestExamples.threeLayerMultiProcessorNetwork());
    }

    @Test
    @Timeout(120)
    public void testTwoLayerSingleProcessorNetworkUPDATED() {
        assertResultsMatchSolverLN(
                LayeredNetworkTestExamples.twoLayerSingleProcessorNetworkUPDATED());
    }

    @Test
    @Timeout(120)
    public void testTwoLayerSingleProcessorNetworkUPDATED2() {
        assertResultsMatchSolverLN(
                LayeredNetworkTestExamples.twoLayerSingleProcessorNetworkUPDATED2());
    }

    @Test
    @Timeout(120)
    public void testThreeLayerSingleProcessorNetworkUPDATED() {
        assertResultsMatchSolverLN(
                LayeredNetworkTestExamples.threeLayerSingleProcessorNetworkUPDATED());
    }

    @Test
    @Timeout(120)
    public void testBug1Network1() {
        assertResultsMatchSolverLN(LayeredNetworkTestExamples.bug1network1());
    }

    @Test
    @Timeout(120)
    public void testBug1Network2() {
        assertResultsMatchSolverLN(LayeredNetworkTestExamples.bug1network2());
    }

    @Test
    @Timeout(120)
    public void testBug1Network3() {
        assertResultsMatchSolverLN(LayeredNetworkTestExamples.bug1network3());
    }

    @Test
    @Timeout(120)
    public void testBug2Network1() {
        assertResultsMatchSolverLN(LayeredNetworkTestExamples.bug2network1());
    }

    @Test
    @Timeout(120)
    public void testBug2Network2() {
        assertResultsMatchSolverLN(LayeredNetworkTestExamples.bug2network2());
    }

    @Test
    @Timeout(120)
    public void testBug2Network3() {
        assertResultsMatchSolverLN(LayeredNetworkTestExamples.bug2network3());
    }

    @Test
    @Timeout(120)
    public void testMultiCall() {
        assertResultsMatchSolverLN(LayeredNetworkTestExamples.multiCallNetwork());
    }

    @Test
    @Timeout(120)
    public void testBug4OnlyLast() {
        assertResultsMatchSolverLN(SolverLnSimpleBugModels.bug4network1());
    }

    @Test
    @Timeout(120)
    public void testBug4FirstAndLast() {
        assertResultsMatchSolverLN(SolverLnSimpleBugModels.bug4network2());
    }
}
