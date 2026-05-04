package jline.solvers.ln_simple.utils;

import jline.lang.Network;
import jline.lang.constant.SchedStrategy;
import jline.lang.layered.*;
import jline.lang.processes.Exp;
import jline.lang.processes.Immediate;
import jline.solvers.AvgTable;
import jline.solvers.LayeredNetworkAvgTable;
import jline.solvers.ln.SolverLN;
import jline.solvers.ln_simple.SolverLNSimple;

public class SolverLNRunner {
    public static void main(String[] args) {
        LayeredNetwork model = new LayeredNetwork("Decreasing Task Multiplicity (Failing)");
        Processor P1 = new Processor(model, "P1", 2, SchedStrategy.PS);
        Processor P2 = new Processor(model, "P2", 2, SchedStrategy.PS);
        Task T1 = new Task(model, "T1", 2, SchedStrategy.REF).on(P1);
        Task T2 = new Task(model, "T2", 1, SchedStrategy.FCFS).on(P2);
        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        new Activity(model, "AS1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);
        new Activity(model, "AS2", Exp.fitMean(0.9)).on(T2).boundTo(E2).repliesTo(E1);

        try {
            runSolverLNSimple(model);
        } catch (Exception e) {
            System.err.println("SolverLNSimple failed: " + e.getMessage());
            e.printStackTrace();
        }
    }

    public static void runSolverLNSimple(LayeredNetwork model) {
        final SolverLNSimple solver = new SolverLNSimple(model);
        solver.iterateCoupledMva(100, 1e-4);
        LayeredNetworkAvgTable avg = solver.getAvgTable();
        if (avg != null) avg.print();
    }

    public static void runSolverLN(LayeredNetwork model) {
        SolverLN solver = new SolverLN(model) {};
        AvgTable avg = solver.getAvgTable();
        if (avg != null) avg.print();
    }

    private static void viewEnsemble(Iterable<Network> ensemble) {
        for (Network layer : ensemble) {
            layer.view();
        }
    }
}