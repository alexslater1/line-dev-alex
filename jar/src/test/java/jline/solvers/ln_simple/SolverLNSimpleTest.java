package jline.solvers.ln_simple;

import jline.GlobalConstants;
import jline.VerboseLevel;
import jline.solvers.ln_simple.utils.LayeredNetworkTestExamples;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Disabled;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.Timeout;

import static jline.solvers.ln_simple.util.assertResultsMatchSolverLN;

public class SolverLNSimpleTest {

    @BeforeAll
    public static void setUp() {
        GlobalConstants.setVerbose(VerboseLevel.SILENT);
    }

    //PASSES
    @Test
    @Timeout(120)
    public void testTwoLayerSingleProcessorNetwork() {
        assertResultsMatchSolverLN(
                LayeredNetworkTestExamples.twoLayerSingleProcessorNetwork());
    }

    //PASSES
    @Test
    @Timeout(120)
    public void testThreeLayerSingleProcessorNetwork() {
        assertResultsMatchSolverLN(
                LayeredNetworkTestExamples.threeLayerSingleProcessorNetwork());
    }

    //PASSES
    @Test
    @Timeout(120)
    public void testTwoLayerMultiProcessorNetwork() {
        assertResultsMatchSolverLN(
                LayeredNetworkTestExamples.twoLayerMultiProcessorNetwork());
    }

    //PASSES
    @Test
    @Timeout(120)
    public void testThreeLayerMultiProcessorNetwork() {
        assertResultsMatchSolverLN(
                LayeredNetworkTestExamples.threeLayerMultiProcessorNetwork());
    }

    //PASSES
    @Test
    @Timeout(120)
    public void testTwoLayerSingleProcessorNetworkUPDATED() {
        assertResultsMatchSolverLN(
                LayeredNetworkTestExamples.twoLayerSingleProcessorNetworkUPDATED());
    }

    //PASSES
    @Test
    @Timeout(120)
    public void testTwoLayerSingleProcessorNetworkUPDATED2() {
        assertResultsMatchSolverLN(
                LayeredNetworkTestExamples.twoLayerSingleProcessorNetworkUPDATED2());
    }

    //FAILS
    @Disabled
    @Test
    @Timeout(120)
    public void testThreeLayerSingleProcessorNetworkUPDATED() {
        assertResultsMatchSolverLN(
                LayeredNetworkTestExamples.threeLayerSingleProcessorNetworkUPDATED());
    }
}
