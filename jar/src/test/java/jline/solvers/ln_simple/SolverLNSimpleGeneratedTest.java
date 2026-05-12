package jline.solvers.ln_simple;

import jline.GlobalConstants;
import jline.VerboseLevel;
import jline.solvers.ln_simple.utils.LayeredNetworkTestExamples;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Disabled;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.Timeout;

import static jline.solvers.ln_simple.util.assertResultsMatchSolverLN;

public class SolverLNSimpleGeneratedTest {

    @BeforeAll
    public static void setUp() {
        GlobalConstants.setVerbose(VerboseLevel.SILENT);
    }

    @Test
    @Timeout(120)
    public void testScenario1SimpleChain() throws Exception {
        assertResultsMatchSolverLN(LayeredNetworkTestExamples.sc1_simple_chain());
    }

    @Disabled
    @Test
    @Timeout(120)
    public void testScenario2ThreeTier() throws Exception {
        assertResultsMatchSolverLN(LayeredNetworkTestExamples.sc2_three_tier());
    }

    
    @Test
    @Timeout(120)
    public void testScenario3Fanout() throws Exception {
        assertResultsMatchSolverLN(LayeredNetworkTestExamples.sc3_fanout());
    }

    
    @Test
    @Timeout(120)
    public void testScenario4Serial() throws Exception {
        assertResultsMatchSolverLN(LayeredNetworkTestExamples.sc4_serial());
    }

    
    @Test
    @Timeout(120)
    public void testScenario5Loop() throws Exception {
        assertResultsMatchSolverLN(LayeredNetworkTestExamples.sc5_loop());
    }

    @Disabled //slight error
    @Test
    @Timeout(120)
    public void testScenario6MultiCall() throws Exception {
        assertResultsMatchSolverLN(LayeredNetworkTestExamples.sc6_multi_call());
    }

    //@Disabled
    @Test
    @Timeout(120)
    public void testScenario7Multicore() throws Exception {
        assertResultsMatchSolverLN(LayeredNetworkTestExamples.sc7_multicore());
    }

    @Test
    @Timeout(120)
    public void testScenario8Immediate() throws Exception {
        assertResultsMatchSolverLN(LayeredNetworkTestExamples.sc8_immediate());
    }

    
    @Test
    @Timeout(120)
    public void testScenario9TwoCalls() throws Exception {
        assertResultsMatchSolverLN(LayeredNetworkTestExamples.sc9_two_calls());
    }

    @Disabled
    @Test
    @Timeout(120)
    public void testScenario10DeepChain() throws Exception {
        assertResultsMatchSolverLN(LayeredNetworkTestExamples.sc10_deep_chain());
    }
    
}
