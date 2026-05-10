package jline.solvers.ln_simple;

import static jline.solvers.ln_simple.util.assertResultsMatchSolverLN;

import jline.solvers.ln_simple.utils.LayeredNetworkTestExamples;
import jline.solvers.ln_simple.utils.MulticlassLayeredNetworkExamples;
import org.junit.jupiter.api.Disabled;
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

    //FAILING TESTS - USED TO DEBUG ANY FAILING BUGS/TESTS
    //
    @Test
    @Timeout(120)
    public void testDebugRight() {
        LayeredNetwork model = new LayeredNetwork("sc2");
        Processor P1 = new Processor(model, "P1", 50, SchedStrategy.PS);
        Processor P2 = new Processor(model, "P2", 50, SchedStrategy.PS);
        Processor P3 = new Processor(model, "P3", 50, SchedStrategy.PS);
        Task T1 = new Task(model, "T1", 50, SchedStrategy.REF).on(P1).setThinkTime(1);
        Task T2 = new Task(model, "T2", 50, SchedStrategy.FCFS).on(P2);
        Task T3 = new Task(model, "T3", 50, SchedStrategy.FCFS).on(P3);
        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        Entry E3 = new Entry(model, "E3").on(T3);
        new Activity(model, "A1", Exp.fitMean(1e-6)).on(T1).boundTo(E1).synchCall(E2, 1);
        new Activity(model, "A2", Exp.fitMean(1e-6)).on(T2).boundTo(E2).synchCall(E3, 1).repliesTo(E1);
        new Activity(model, "A3", Exp.fitMean(0.5)).on(T3).boundTo(E3).repliesTo(E2);

        assertResultsMatchSolverLN(model);
    }

}
