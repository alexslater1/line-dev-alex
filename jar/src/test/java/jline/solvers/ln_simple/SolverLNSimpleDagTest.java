package jline.solvers.ln_simple;

import jline.GlobalConstants;
import jline.VerboseLevel;
import jline.solvers.ln_simple.fixtures.DagFeatureFixtures;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.Timeout;

import static jline.solvers.ln_simple.util.assertResultsMatchSolverLN;

/**
 * Activity-graph (DAG) feature tests for {@code SolverLNSimple}.
 *
 * <p>Each test loads one {@link DagFeatureFixtures} fixture and asserts
 * SolverLNSimple matches SolverLN or SolverLQNS within the standard
 * {@code atol=1e-3, rtol=5e-2} tolerance.
 *
 * <p>Tests are grouped by which implementation phase makes them pass:
 *
 * <ul>
 *   <li><b>Phase A</b> — visit-weight DAG walk (sequence past bound activity,
 *       OR_FORK branch probabilities, POST_LOOP multipliers).</li>
 *   <li><b>Phase B</b> — AND_FORK response-time max-aggregation.</li>
 *   <li><b>Phase C</b> — REPLY phase-1 / phase-2 split.</li>
 * </ul>
 *
 * <p>The integration test {@link #combinedDag()} passes only once all three
 * phases have landed.
 */
public class SolverLNSimpleDagTest {

    @BeforeAll
    public static void setUp() {
        GlobalConstants.setVerbose(VerboseLevel.SILENT);
    }

    // =========================================================================
    //  Phase A — visit-weight DAG walk
    // =========================================================================

    @Test @Timeout(180) public void seqEntry()           { assertResultsMatchSolverLN(DagFeatureFixtures::D_seqEntry); }
    @Test @Timeout(180) public void seqEntry_multiEntry(){ assertResultsMatchSolverLN(DagFeatureFixtures::D_seqEntry_multiEntry); }
    @Test @Timeout(180) public void orForkUneven()       { assertResultsMatchSolverLN(DagFeatureFixtures::D_orForkUneven); }
    @Test @Timeout(180) public void orForkThreeWay()     { assertResultsMatchSolverLN(DagFeatureFixtures::D_orForkThreeWay); }
    @Test @Timeout(180) public void postLoopCall()             { assertResultsMatchSolverLN(DagFeatureFixtures::D_postLoopCall); }
    @Test @Timeout(180) public void postLoopCall2x()           { assertResultsMatchSolverLN(DagFeatureFixtures::D_postLoopCall2x); }
    @Test @Timeout(180) public void postLoopHost()             { assertResultsMatchSolverLN(DagFeatureFixtures::D_postLoopHost); }
    @Test @Timeout(180) public void multiCallToOneEntry()      { assertResultsMatchSolverLN(DagFeatureFixtures::D_multiCallToOneEntry); }
    @Test @Timeout(180) public void postLoopCallWithT2Demand() { assertResultsMatchSolverLN(DagFeatureFixtures::D_postLoopCallWithT2Demand); }

    // =========================================================================
    //  Phase B — AND-fork response-time aggregation
    // =========================================================================

    @Test @Timeout(180) public void andForkJoin()        { assertResultsMatchSolverLN(DagFeatureFixtures::D_andForkJoin); }
    @Test @Timeout(180) public void andForkUnequal()     { assertResultsMatchSolverLN(DagFeatureFixtures::D_andForkUnequal); }

    // =========================================================================
    //  Phase C — REPLY phase split
    // =========================================================================

    @Test @Timeout(180) public void replyFirstPhase()    { assertResultsMatchSolverLN(DagFeatureFixtures::D_replyFirstPhase); }
    @Test @Timeout(180) public void replyAtBound()       { assertResultsMatchSolverLN(DagFeatureFixtures::D_replyAtBound); }

    // =========================================================================
    //  Integration — needs A + B + C
    // =========================================================================

    @Test @Timeout(240) public void combinedDag()        { assertResultsMatchSolverLN(DagFeatureFixtures::D_combinedDag); }
}
