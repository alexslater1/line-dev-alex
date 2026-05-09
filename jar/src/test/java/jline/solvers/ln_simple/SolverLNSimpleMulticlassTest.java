package jline.solvers.ln_simple;

import jline.GlobalConstants;
import jline.VerboseLevel;
import jline.solvers.ln_simple.utils.MulticlassLayeredNetworkExamples;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.Timeout;

import static jline.solvers.ln_simple.util.assertResultsMatchSolverLN;

public class SolverLNSimpleMulticlassTest {
    // Category A: no shared resources between REF task chains

    @BeforeAll
    public static void setUp() {
        GlobalConstants.setVerbose(VerboseLevel.SILENT);
    }

    @Test @Timeout(120)
    public void testMcA1_TwoClassInfServers() throws Exception {
        assertResultsMatchSolverLN(MulticlassLayeredNetworkExamples.mc_a1_two_class_inf_servers());
    }

    @Test @Timeout(120)
    public void testMcA2_TwoClassSeparatePs() throws Exception {
        assertResultsMatchSolverLN(MulticlassLayeredNetworkExamples.mc_a2_two_class_separate_ps());
    }

    @Test @Timeout(120)
    public void testMcA3_ThreeClassSeparatePs() throws Exception {
        assertResultsMatchSolverLN(MulticlassLayeredNetworkExamples.mc_a3_three_class_separate_ps());
    }

    @Test @Timeout(120)
    public void testMcA4_TwoClassAsymmetricN() throws Exception {
        assertResultsMatchSolverLN(MulticlassLayeredNetworkExamples.mc_a4_two_class_asymmetric_n());
    }

    // Category B: shared single-server PS
    @Test @Timeout(120)
    public void testMcB1_TwoClassSharedPsSymmetric() throws Exception {
        assertResultsMatchSolverLN(MulticlassLayeredNetworkExamples.mc_b1_two_class_shared_ps_symmetric());
    }

    @Test @Timeout(120)
    public void testMcB2_TwoClassSharedPsAsymmetricN() throws Exception {
        assertResultsMatchSolverLN(MulticlassLayeredNetworkExamples.mc_b2_two_class_shared_ps_asymmetric_n());
    }

    @Test @Timeout(120)
    public void testMcB3_TwoClassSharedPsAsymmetricD() throws Exception {
        assertResultsMatchSolverLN(MulticlassLayeredNetworkExamples.mc_b3_two_class_shared_ps_asymmetric_d());
    }

    @Test @Timeout(120)
    public void testMcB4_TwoClassSharedPsFullyAsymmetric() throws Exception {
        assertResultsMatchSolverLN(MulticlassLayeredNetworkExamples.mc_b4_two_class_shared_ps_fully_asymmetric());
    }

    @Test @Timeout(120)
    public void testMcB5_ThreeClassSharedPsAsymmetric() throws Exception {
        assertResultsMatchSolverLN(MulticlassLayeredNetworkExamples.mc_b5_three_class_shared_ps_asymmetric());
    }

    // Category C: shared multi-server PS
    @Test @Timeout(120)
    public void testMcC1_TwoClassShared2ServerPsSymmetric() throws Exception {
        assertResultsMatchSolverLN(MulticlassLayeredNetworkExamples.mc_c1_two_class_shared_2server_ps_symmetric());
    }

    @Test @Timeout(120)
    public void testMcC2_TwoClassShared2ServerPsAsymmetric() throws Exception {
        assertResultsMatchSolverLN(MulticlassLayeredNetworkExamples.mc_c2_two_class_shared_2server_ps_asymmetric());
    }

    @Test @Timeout(120)
    public void testMcC3_ThreeClassShared3ServerPsAsymmetric() throws Exception {
        assertResultsMatchSolverLN(MulticlassLayeredNetworkExamples.mc_c3_three_class_shared_3server_ps_asymmetric());
    }
}
