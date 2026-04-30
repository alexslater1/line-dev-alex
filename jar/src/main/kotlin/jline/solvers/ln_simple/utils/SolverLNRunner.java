package jline.solvers.ln_simple.utils;

import jline.lang.Network;
import jline.lang.layered.LayeredNetwork;
import jline.solvers.AvgTable;
import jline.solvers.LayeredNetworkAvgTable;
import jline.solvers.ln.SolverLN;
import jline.solvers.ln_simple.SolverLNSimple;

public class SolverLNRunner {
    public static void main(String[] args) {
        try {
            runSolverLNSimple();
        } catch (Exception e) {
            System.err.println("SolverLNSimple failed: " + e.getMessage());
            e.printStackTrace();
        }
    }

    public static void runSolverLNSimple() {
        LayeredNetwork model = SolverLnSimpleBugModels.bug1network3();
        final SolverLNSimple solver = new SolverLNSimple(model);
        final int[] iteration = new int[] {0};
        solver.iterateCoupledMva(100, 1e-4, new Runnable() {
            @Override
            public void run() {
                iteration[0]++;
                if (iteration[0] == 10) {
                    viewEnsemble(solver.getEnsemble());
                }
            }
        });
        LayeredNetworkAvgTable avg = solver.getAvgTable();
        if (avg != null) avg.print();
    }

    public static void runSolverLN() {
        LayeredNetwork model = SolverLnSimpleBugModels.bug1network3();
        SolverLN solver = new SolverLN(model) {
            @Override
            public void post(int it) {
                super.post(it);
                if (it == 10) {
                    viewEnsemble(getEnsemble());
                }
            }
        };
        AvgTable avg = solver.getAvgTable();
        if (avg != null) avg.print();
    }

    private static void viewEnsemble(Iterable<Network> ensemble) {
        for (Network layer : ensemble) {
            layer.view();
        }
    }
}