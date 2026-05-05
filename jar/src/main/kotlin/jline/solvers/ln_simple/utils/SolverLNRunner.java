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
        LayeredNetwork  model = new LayeredNetwork("debug2");
        Processor p1 = new Processor(model, "P1", 1, SchedStrategy.PS);
        Processor p2 = new Processor(model, "P2", 1, SchedStrategy.PS);
        Processor p3 = new Processor(model, "P3", 1, SchedStrategy.PS);
        Task t1 = new Task(model, "T1", 1, SchedStrategy.REF).on(p1);
        Task t2 = new Task(model, "T2", 1, SchedStrategy.FCFS).on(p2);
        Task t3 = new Task(model, "T3", 1, SchedStrategy.FCFS).on(p3);
        Entry e1 = new Entry(model, "E1").on(t1);
        Entry e2 = new Entry(model, "E2").on(t2);
        Entry e3 = new Entry(model, "E3").on(t3);
        new Activity(model, "AS1", Exp.fitMean(0.7)).on(t1).boundTo(e1).synchCall(e2, 1);
        new Activity(model, "AS2", Immediate.getInstance()).on(t2).boundTo(e2).synchCall(e3, 1);
        new Activity(model, "AS3", Exp.fitMean(0.9)).on(t3).boundTo(e3).repliesTo(e2);

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