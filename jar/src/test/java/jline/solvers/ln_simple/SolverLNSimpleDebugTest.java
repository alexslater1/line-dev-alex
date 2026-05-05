package jline.solvers.ln_simple;

import static jline.solvers.ln_simple.util.assertResultsMatchSolverLN;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.Timeout;

import jline.lang.constant.SchedStrategy;
import jline.lang.layered.Activity;
import jline.lang.layered.Entry;
import jline.lang.layered.LayeredNetwork;
import jline.lang.layered.Processor;
import jline.lang.layered.Task;
import jline.lang.processes.Exp;
import jline.lang.processes.Immediate;

public class SolverLNSimpleDebugTest {
    @Test
    @Timeout(120)
    public void testDebug() {
        LayeredNetwork  model = new LayeredNetwork("test");
        Processor p1 = new Processor(model, "P1", 2, SchedStrategy.PS);
        Processor p2 = new Processor(model, "P2", 1, SchedStrategy.PS);
        Processor p3 = new Processor(model, "P3", 3, SchedStrategy.PS);
        Processor p4 = new Processor(model, "P4", 2, SchedStrategy.PS);
        Task t1 = new Task(model, "T1", 2, SchedStrategy.REF).on(p1);
        Task t2 = new Task(model, "T2", 2, SchedStrategy.FCFS).on(p2);
        Task t3 = new Task(model, "T3", 2, SchedStrategy.FCFS).on(p3);
        Task t4 = new Task(model, "T4", 2, SchedStrategy.FCFS).on(p4);
        Entry e1 = new Entry(model, "E1").on(t1);
        Entry e2 = new Entry(model, "E2").on(t2);
        Entry e3 = new Entry(model, "E3").on(t3);
        Entry e4 = new Entry(model, "E4").on(t4);
        new Activity(model, "AS1", Immediate.getInstance()).on(t1).boundTo(e1).synchCall(e2, 5);
        new Activity(model, "AS2", Immediate.getInstance()).on(t2).boundTo(e2).synchCall(e3, 4);
        new Activity(model, "AS3", Immediate.getInstance()).on(t3).boundTo(e3).synchCall(e4, 7);
        new Activity(model, "AS4", Exp.fitMean(0.9)).on(t4).boundTo(e4).repliesTo(e3);

        assertResultsMatchSolverLN(model);
    }

}
