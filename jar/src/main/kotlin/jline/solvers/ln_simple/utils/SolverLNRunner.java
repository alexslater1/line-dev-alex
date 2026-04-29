package jline.solvers.ln_simple.utils;

import jline.lang.constant.SolverType;
import jline.lang.layered.LayeredNetwork;
import jline.solvers.AvgTable;
import jline.solvers.ln.SolverLN;

public class SolverLNRunner {
    public static void main(String[] args) {
        try {
            LayeredNetwork model = LayeredNetworkTestExamples.sc1_simple_chain();
//            model.view();
            SolverLN solverLN = new SolverLN(model, SolverType.MVA) ;
            AvgTable avg = solverLN.getEnsembleAvg();
            if (avg != null) avg.print();
        } catch (Exception e) {
            System.err.println("SolverLN failed: " + e.getMessage());
            e.printStackTrace();
        }
    }
}