package jline.solvers.env;

import jline.solvers.SolverResult;
import org.junit.jupiter.api.Test;

/**
 * Temporary test class to print actual JAR SolverENV values for disabled tests.
 * This helps investigate value mismatches without modifying the original test.
 */
public class SolverEnvValueCheck {

    @Test
    public void printErlangStateIndepValues() {
        System.out.println("=== ErlangStateIndependentD1SQ2stage2 (example_randomEnvironment_9, stateindep) ===");
        SolverENV envSolver = SolverEnvTestFixtures.example_randomEnvironment_9();
        envSolver.getAvg();
        SolverResult result = envSolver.result;
        System.out.println("QN dimensions: " + result.QN.getNumRows() + "x" + result.QN.getNumCols());
        for (int i = 0; i < result.QN.getNumRows(); i++) {
            for (int j = 0; j < result.QN.getNumCols(); j++) {
                System.out.printf("QN(%d,%d) = %.6f  (expected: %s)%n", i, j, result.QN.get(i, j),
                    i==0&&j==0 ? "42.0259" : i==1&&j==0 ? "55.8969" : i==2&&j==0 ? "2.0349" : "?");
            }
        }
        System.out.println("UN:");
        for (int i = 0; i < result.UN.getNumRows(); i++) {
            for (int j = 0; j < result.UN.getNumCols(); j++) {
                System.out.printf("UN(%d,%d) = %.6f%n", i, j, result.UN.get(i, j));
            }
        }
        System.out.println("TN:");
        for (int i = 0; i < result.TN.getNumRows(); i++) {
            for (int j = 0; j < result.TN.getNumCols(); j++) {
                System.out.printf("TN(%d,%d) = %.6f%n", i, j, result.TN.get(i, j));
            }
        }
    }

    @Test
    public void printErlangStateDepValues() {
        System.out.println("=== ErlangStateDependentD1SQ2stage2 (example_randomEnvironment_9, statedep) ===");
        SolverENV envSolver = SolverEnvTestFixtures.example_randomEnvironment_9();
        envSolver.setStateDepMethod("statedep");
        envSolver.setRef(0);
        envSolver.getAvg();
        SolverResult result = envSolver.result;
        System.out.println("QN dimensions: " + result.QN.getNumRows() + "x" + result.QN.getNumCols());
        for (int i = 0; i < result.QN.getNumRows(); i++) {
            for (int j = 0; j < result.QN.getNumCols(); j++) {
                System.out.printf("QN(%d,%d) = %.6f  (expected: %s)%n", i, j, result.QN.get(i, j),
                    i==0&&j==0 ? "42.2081" : i==1&&j==0 ? "56.0814" : i==2&&j==0 ? "2.0469" : "?");
            }
        }
    }

    @Test
    public void printStateDependentD1SQ2stage2ParallelValues() {
        System.out.println("=== StateDependentD1SQ2stage2Parallel (example_randomEnvironment_8, statedep) ===");
        SolverENV envSolver = SolverEnvTestFixtures.example_randomEnvironment_8();
        envSolver.setRef(0);
        envSolver.getAvg();
        SolverResult result = envSolver.result;
        System.out.println("QN dimensions: " + result.QN.getNumRows() + "x" + result.QN.getNumCols());
        for (int i = 0; i < result.QN.getNumRows(); i++) {
            for (int j = 0; j < result.QN.getNumCols(); j++) {
                System.out.printf("QN(%d,%d) = %.6f  (expected: %s)%n", i, j, result.QN.get(i, j),
                    i==0&&j==0 ? "47.999" : i==1&&j==0 ? "50.893" : i==2&&j==0 ? "1.164" : "?");
            }
        }
    }

    @Test
    public void printCTMCSolverMulticlassD1SQ2Values() {
        System.out.println("=== CTMCSolverMulticlassD1SQ2TwoClassesOfMMPPTest (renv_twostages_repairmen3) ===");
        System.out.println("--- CTMC result ---");
        SolverENV envSolverCTMC = SolverEnvTestFixtures.renv_twostages_repairmen3();
        SolverResult CTMCresult = envSolverCTMC.runAnalyzerByCTMC();
        System.out.println("CTMC QN:");
        for (int i = 0; i < CTMCresult.QN.getNumRows(); i++) {
            for (int j = 0; j < CTMCresult.QN.getNumCols(); j++) {
                System.out.printf("  CTMC QN(%d,%d) = %.6f%n", i, j, CTMCresult.QN.get(i, j));
            }
        }

        System.out.println("--- ENV (fluid, stateindep) result ---");
        SolverENV envSolver = SolverEnvTestFixtures.renv_twostages_repairmen3();
        envSolver.setStateDepMethod("stateindep");
        envSolver.getAvg();
        SolverResult result = envSolver.result;
        System.out.println("ENV QN:");
        for (int i = 0; i < result.QN.getNumRows(); i++) {
            for (int j = 0; j < result.QN.getNumCols(); j++) {
                System.out.printf("  ENV QN(%d,%d) = %.6f%n", i, j, result.QN.get(i, j));
            }
        }
    }

    @Test
    public void printStateDependentD1SQ3stage2Values() {
        System.out.println("=== StateDependentD1SQ3stage2 (example_randomEnvironment_7, statedep) ===");
        SolverENV envSolver = SolverEnvTestFixtures.example_randomEnvironment_7();
        envSolver.setRef(0);
        envSolver.getAvg();
        SolverResult result = envSolver.result;
        System.out.println("QN dimensions: " + result.QN.getNumRows() + "x" + result.QN.getNumCols());
        for (int i = 0; i < result.QN.getNumRows(); i++) {
            for (int j = 0; j < result.QN.getNumCols(); j++) {
                System.out.printf("QN(%d,%d) = %.6f  (expected: %s)%n", i, j, result.QN.get(i, j),
                    i==0&&j==0 ? "23.9966" : i==1&&j==0 ? "73.5379" : i==2&&j==0 ? "1.1808" : i==3&&j==0 ? "1.3067" : "?");
            }
        }
    }
}
