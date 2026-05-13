package jline.solvers.ln_simple;

import jline.lang.constant.SchedStrategy;
import jline.lang.layered.*;
import jline.lang.processes.Exp;
import jline.solvers.ln_simple.fixtures.SingleclassLNExamples;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.Timeout;

import static jline.solvers.ln_simple.util.assertResultsMatchSolverLN;

public class SolverLNSimpleDebugTest {

    @Test
    @Timeout(120)
    public void debugTestTakingLongTime() {
        LayeredNetwork model = new LayeredNetwork("test");
        Processor P1 = new Processor(model, "P1", 4, SchedStrategy.PS);
        Processor P2 = new Processor(model, "P2", 8, SchedStrategy.PS);
        Task T1 = new Task(model, "T1", 200, SchedStrategy.REF).on(P1).setThinkTime(new Exp(5));
        Task T2 = new Task(model, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(P2);
        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        new Activity(model, "A1", new Exp(3)).on(T1).boundTo(E1).synchCall(E2, 1);
        new Activity(model, "A2", new Exp(6)).on(T2).boundTo(E2).repliesTo(E2);
        assertResultsMatchSolverLN(model);
    }
}
