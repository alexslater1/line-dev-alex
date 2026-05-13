package jline.solvers.ln_simple;

import jline.GlobalConstants;
import jline.VerboseLevel;
import jline.solvers.ln_simple.fixtures.SingleclassLNExamples;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Disabled;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.Timeout;

import static jline.solvers.ln_simple.util.assertResultsMatchSolverLN;

public class SolverLNSimpleFailingTest {

    @BeforeAll
    public static void setUp() {
        GlobalConstants.setVerbose(VerboseLevel.SILENT);
    }

    @Test
    @Timeout(120)
    @Disabled
    public void testScenario2ThreeTier() throws Exception {
        assertResultsMatchSolverLN(SingleclassLNExamples.sc2_three_tier());
    }

    @Test
    @Timeout(120)
    @Disabled
    public void testScenario6MultiCall() throws Exception {
        assertResultsMatchSolverLN(SingleclassLNExamples.sc6_multi_call());
    }

    @Test
    @Timeout(120)
    @Disabled
    public void testScenario10DeepChain() throws Exception {
        assertResultsMatchSolverLN(SingleclassLNExamples.sc10_deep_chain());
    }
    
}
