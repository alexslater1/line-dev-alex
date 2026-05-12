package jline.solvers.ln_simple;

import jline.GlobalConstants;
import jline.VerboseLevel;
import jline.solvers.ln_simple.fixtures.SingleclassLNExamples;
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
        assertResultsMatchSolverLN(SingleclassLNExamples.sc1_simple_chain());
    }

    @Disabled
    @Test
    @Timeout(120)
    public void testScenario2ThreeTier() throws Exception {
        assertResultsMatchSolverLN(SingleclassLNExamples.sc2_three_tier());
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

    @Disabled //slight error
    @Test
    @Timeout(120)
    public void testScenario6MultiCall() throws Exception {
        assertResultsMatchSolverLN(SingleclassLNExamples.sc6_multi_call());
    }

    //@Disabled
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

    @Disabled
    @Test
    @Timeout(120)
    public void testScenario10DeepChain() throws Exception {
        assertResultsMatchSolverLN(SingleclassLNExamples.sc10_deep_chain());
    }
    
}
