package jline.solvers.ln_simple;

import jline.GlobalConstants;
import jline.VerboseLevel;
import jline.solvers.ln_simple.fixtures.EvaluationSuite;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.Timeout;

import static jline.solvers.ln_simple.util.assertResultsMatchSolverLN;

/**
 * Evaluation suite tests — one test per fixture in {@link EvaluationSuite}.
 *
 * <p>Each test loads its fixture, runs SolverLNSimple, SolverLN and SolverLQNS,
 * and asserts that SolverLNSimple matches SolverLN under the standard
 * {@code assertResultsMatchSolverLN} protocol (atol=1e-3, rtol=5e-2).
 *
 * <p>Fixture layout follows {@code report/evaluation-suite-spec.md} and the
 * accompanying {@code report/evaluation-suite-manifest.md}.
 */
public class SolverLNSimpleEvaluationTest {

    @BeforeAll
    public static void setUp() {
        GlobalConstants.setVerbose(VerboseLevel.SILENT);
        warmupJvm();
    }

    /** JIT-warm the solver paths so the first few timed fixtures are not
     *  cold-start-noisy. Three iterations of {@code A1_chain_d3_N20} —
     *  the smallest fixture — exercise SolverLNSimple, SolverLN and
     *  SolverLQNS through their typical code paths. Dataset capture is
     *  suspended for the duration so the warmup rows are dropped. */
    private static void warmupJvm() {
        util.dataCaptureSuspended = true;
        try {
            for (int i = 0; i < 3; i++) {
                util.assertResultsMatchSolverLN(EvaluationSuite::A1_chain_d3_N20);
            }
        } catch (Throwable ignored) {
            // Warmup failures are not fatal — log and continue.
            System.err.println("[warmup] iteration failed: " + ignored.getMessage());
        } finally {
            util.dataCaptureSuspended = false;
        }
    }

    // =========================================================================
    // Partition A1: Single-class chains
    // =========================================================================

    @Test @Timeout(180) public void A1_chain_d3_N20()  { assertResultsMatchSolverLN(EvaluationSuite::A1_chain_d3_N20);  }
    @Test @Timeout(180) public void A1_chain_d3_N50()  { assertResultsMatchSolverLN(EvaluationSuite::A1_chain_d3_N50);  }
    @Test @Timeout(180) public void A1_chain_d3_N100() { assertResultsMatchSolverLN(EvaluationSuite::A1_chain_d3_N100); }
    @Test @Timeout(180) public void A1_chain_d5_N20()  { assertResultsMatchSolverLN(EvaluationSuite::A1_chain_d5_N20);  }
    @Test @Timeout(180) public void A1_chain_d5_N50()  { assertResultsMatchSolverLN(EvaluationSuite::A1_chain_d5_N50);  }
    @Test @Timeout(240) public void A1_chain_d5_N100() { assertResultsMatchSolverLN(EvaluationSuite::A1_chain_d5_N100); }
    @Test @Timeout(240) public void A1_chain_d8_N20()  { assertResultsMatchSolverLN(EvaluationSuite::A1_chain_d8_N20);  }
    @Test @Timeout(240) public void A1_chain_d8_N50()  { assertResultsMatchSolverLN(EvaluationSuite::A1_chain_d8_N50);  }
    @Test @Timeout(300) public void A1_chain_d8_N100() { assertResultsMatchSolverLN(EvaluationSuite::A1_chain_d8_N100); }

    // =========================================================================
    // Partition A2: Multi-class with shared callees
    // =========================================================================

    @Test @Timeout(180) public void A2_multiclassA_2refs_loadLow_N20()      { assertResultsMatchSolverLN(EvaluationSuite::A2_multiclassA_2refs_loadLow_N20);  }
    @Test @Timeout(180) public void A2_multiclassA_2refs_loadHigh_N20()     { assertResultsMatchSolverLN(EvaluationSuite::A2_multiclassA_2refs_loadHigh_N20); }
    @Test @Timeout(180) public void A2_multiclassA_2refs_loadLow_N50()      { assertResultsMatchSolverLN(EvaluationSuite::A2_multiclassA_2refs_loadLow_N50);  }
    @Test @Timeout(180) public void A2_multiclassA_2refs_loadHigh_N50()     { assertResultsMatchSolverLN(EvaluationSuite::A2_multiclassA_2refs_loadHigh_N50); }
    @Test @Timeout(180) public void A2_multiclassB_2refs_demands_3to1_N20() { assertResultsMatchSolverLN(EvaluationSuite::A2_multiclassB_2refs_demands_3to1_N20); }
    @Test @Timeout(180) public void A2_multiclassB_2refs_demands_5to1_N20() { assertResultsMatchSolverLN(EvaluationSuite::A2_multiclassB_2refs_demands_5to1_N20); }
    @Test @Timeout(180) public void A2_multiclassB_2refs_demands_3to1_N50() { assertResultsMatchSolverLN(EvaluationSuite::A2_multiclassB_2refs_demands_3to1_N50); }
    @Test @Timeout(180) public void A2_multiclassB_2refs_demands_5to1_N50() { assertResultsMatchSolverLN(EvaluationSuite::A2_multiclassB_2refs_demands_5to1_N50); }
    @Test @Timeout(180) public void A2_multiclassC_2refs_think_2to1_N20()   { assertResultsMatchSolverLN(EvaluationSuite::A2_multiclassC_2refs_think_2to1_N20); }
    @Test @Timeout(180) public void A2_multiclassC_2refs_think_4to1_N20()   { assertResultsMatchSolverLN(EvaluationSuite::A2_multiclassC_2refs_think_4to1_N20); }
    @Test @Timeout(180) public void A2_multiclassC_2refs_think_2to1_N50()   { assertResultsMatchSolverLN(EvaluationSuite::A2_multiclassC_2refs_think_2to1_N50); }
    @Test @Timeout(180) public void A2_multiclassC_2refs_think_4to1_N50()   { assertResultsMatchSolverLN(EvaluationSuite::A2_multiclassC_2refs_think_4to1_N50); }

    // =========================================================================
    // Partition A3: Multi-server processors
    // =========================================================================

    @Test @Timeout(180) public void A3_multiserver_c2_loadLow()    { assertResultsMatchSolverLN(EvaluationSuite::A3_multiserver_c2_loadLow);    }
    @Test @Timeout(180) public void A3_multiserver_c2_loadMedium() { assertResultsMatchSolverLN(EvaluationSuite::A3_multiserver_c2_loadMedium); }
    @Test @Timeout(180) public void A3_multiserver_c2_loadHigh()   { assertResultsMatchSolverLN(EvaluationSuite::A3_multiserver_c2_loadHigh);   }
    @Test @Timeout(180) public void A3_multiserver_c4_loadLow()    { assertResultsMatchSolverLN(EvaluationSuite::A3_multiserver_c4_loadLow);    }
    @Test @Timeout(180) public void A3_multiserver_c4_loadMedium() { assertResultsMatchSolverLN(EvaluationSuite::A3_multiserver_c4_loadMedium); }
    @Test @Timeout(180) public void A3_multiserver_c4_loadHigh()   { assertResultsMatchSolverLN(EvaluationSuite::A3_multiserver_c4_loadHigh);   }
    @Test @Timeout(180) public void A3_multiserver_c8_loadLow()    { assertResultsMatchSolverLN(EvaluationSuite::A3_multiserver_c8_loadLow);    }
    @Test @Timeout(180) public void A3_multiserver_c8_loadMedium() { assertResultsMatchSolverLN(EvaluationSuite::A3_multiserver_c8_loadMedium); }
    @Test @Timeout(180) public void A3_multiserver_c8_loadHigh()   { assertResultsMatchSolverLN(EvaluationSuite::A3_multiserver_c8_loadHigh);   }

    // =========================================================================
    // Partition A4: Mixed topology
    // =========================================================================

    @Test @Timeout(240) public void A4_threeTier_multiclass()    { assertResultsMatchSolverLN(EvaluationSuite::A4_threeTier_multiclass);    }
    @Test @Timeout(240) public void A4_multiclass_multiserver()  { assertResultsMatchSolverLN(EvaluationSuite::A4_multiclass_multiserver);  }
    @Test @Timeout(240) public void A4_multiclass_multientry()   { assertResultsMatchSolverLN(EvaluationSuite::A4_multiclass_multientry);   }
    @Test @Timeout(240) public void A4_chainPlusFanout()         { assertResultsMatchSolverLN(EvaluationSuite::A4_chainPlusFanout);         }
    @Test @Timeout(240) public void A4_webApplication()          { assertResultsMatchSolverLN(EvaluationSuite::A4_webApplication);          }
    @Test @Timeout(240) public void A4_databaseBackend()         { assertResultsMatchSolverLN(EvaluationSuite::A4_databaseBackend);         }

    // =========================================================================
    // Partition B: Scale axis for F24
    // =========================================================================

    @Test @Timeout(180)  public void B_scaleAxis_tasks5_chain()   { assertResultsMatchSolverLN(EvaluationSuite::B_scaleAxis_tasks5_chain);  }
    @Test @Timeout(240)  public void B_scaleAxis_tasks10_chain()  { assertResultsMatchSolverLN(EvaluationSuite::B_scaleAxis_tasks10_chain); }
    @Test @Timeout(360)  public void B_scaleAxis_tasks20_chain()  { assertResultsMatchSolverLN(EvaluationSuite::B_scaleAxis_tasks20_chain); }
    @Test @Timeout(600)  public void B_scaleAxis_tasks40_chain()  { assertResultsMatchSolverLN(EvaluationSuite::B_scaleAxis_tasks40_chain); }
    @Test @Timeout(900)  public void B_scaleAxis_tasks60_chain()  { assertResultsMatchSolverLN(EvaluationSuite::B_scaleAxis_tasks60_chain); }
    @Test @Timeout(180)  public void B_scaleAxis_tasks5_fanout()  { assertResultsMatchSolverLN(EvaluationSuite::B_scaleAxis_tasks5_fanout);  }
    @Test @Timeout(300)  public void B_scaleAxis_tasks10_fanout() { assertResultsMatchSolverLN(EvaluationSuite::B_scaleAxis_tasks10_fanout); }
    @Test @Timeout(600)  public void B_scaleAxis_tasks20_fanout() { assertResultsMatchSolverLN(EvaluationSuite::B_scaleAxis_tasks20_fanout); }
    @Test @Timeout(1200) public void B_scaleAxis_tasks40_fanout() { assertResultsMatchSolverLN(EvaluationSuite::B_scaleAxis_tasks40_fanout); }
    @Test @Timeout(2400) /*LN takes ~16 min — left enabled so baseline capture covers all 61 fixtures.*/ public void B_scaleAxis_tasks60_fanout() { assertResultsMatchSolverLN(EvaluationSuite::B_scaleAxis_tasks60_fanout); }

    // =========================================================================
    // Partition C1: Fan-out caller topologies
    // =========================================================================

    @Test @Timeout(240) public void C1_fanOut2_caller_c1() { assertResultsMatchSolverLN(EvaluationSuite::C1_fanOut2_caller_c1); }
    @Test @Timeout(240) public void C1_fanOut2_caller_c2() { assertResultsMatchSolverLN(EvaluationSuite::C1_fanOut2_caller_c2); }
    @Test @Timeout(240) public void C1_fanOut3_caller_c1() { assertResultsMatchSolverLN(EvaluationSuite::C1_fanOut3_caller_c1); }
    @Test @Timeout(240) public void C1_fanOut3_caller_c2() { assertResultsMatchSolverLN(EvaluationSuite::C1_fanOut3_caller_c2); }
    @Test @Timeout(240) public void C1_fanOut4_caller_c1() { assertResultsMatchSolverLN(EvaluationSuite::C1_fanOut4_caller_c1); }
    @Test @Timeout(240) public void C1_fanOut4_caller_c2() { assertResultsMatchSolverLN(EvaluationSuite::C1_fanOut4_caller_c2); }

    // =========================================================================
    // Partition C2: INF-scheduled hosted tasks
    // =========================================================================

    @Test @Timeout(180) public void C2_infTaskSimple() { assertResultsMatchSolverLN(EvaluationSuite::C2_infTaskSimple); }
    @Test @Timeout(180) public void C2_infTaskShared() { assertResultsMatchSolverLN(EvaluationSuite::C2_infTaskShared); }
    @Test @Timeout(240) public void C2_infTaskDeep()   { assertResultsMatchSolverLN(EvaluationSuite::C2_infTaskDeep);   }

    // =========================================================================
    // Partition C3: Multi-class + multi-entry interaction
    // =========================================================================

    @Test @Timeout(180) public void C3_multiclassMultiEntry_2refs()      { assertResultsMatchSolverLN(EvaluationSuite::C3_multiclassMultiEntry_2refs);      }
    @Test @Timeout(180) public void C3_multiclassMultiEntry_3refs()      { assertResultsMatchSolverLN(EvaluationSuite::C3_multiclassMultiEntry_3refs);      }
    @Test @Timeout(180) public void C3_multiclassMultiEntry_asymmetric() { assertResultsMatchSolverLN(EvaluationSuite::C3_multiclassMultiEntry_asymmetric); }

    // =========================================================================
    // Partition C4: Boundary cases for ablation
    // =========================================================================

    @Test @Timeout(240) public void C4_nearSaturation()     { assertResultsMatchSolverLN(EvaluationSuite::C4_nearSaturation);     }
    @Test @Timeout(240) public void C4_predicateThreshold() { assertResultsMatchSolverLN(EvaluationSuite::C4_predicateThreshold); }
    @Test @Timeout(240) public void C4_lowDemand()          { assertResultsMatchSolverLN(EvaluationSuite::C4_lowDemand);          }
}
