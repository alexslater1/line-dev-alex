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

    //FAILING TESTS - USED TO DEBUG ANY FAILING BUGS/TESTS

    //=== SolverLN ===
    //Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput
    //P1        	Processor 	NaN       	1.00000   	NaN       	NaN       	NaN       	NaN
    //P2        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN
    //P3        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN
    //T1        	RefTask   	1.00000   	1.00000   	NaN       	0.70000   	NaN       	1.42857
    //T2        	Task      	0         	0         	NaN       	0         	NaN       	1.42857
    //T3        	Task      	0         	0         	NaN       	0         	NaN       	1.42857
    //E1        	Entry     	1.00000   	NaN       	0.70000   	NaN       	NaN       	1.42857
    //E2        	Entry     	0         	NaN       	0         	NaN       	NaN       	1.42857
    //E3        	Entry     	0         	NaN       	0         	NaN       	NaN       	1.42857
    //AS1       	Activity  	1.00000   	1.00000   	0.70000   	0.70000   	NaN       	1.42857
    //AS2       	Activity  	0.00000   	0         	0.00000   	0         	NaN       	1.42857
    //AS3       	Activity  	0         	0         	0         	0         	NaN       	1.42857
    //
    //=== SolverLNSimple ===
    //Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput
    //P1        	Processor 	NaN       	1.00000   	NaN       	NaN       	NaN       	NaN
    //P2        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN
    //P3        	Processor 	NaN       	NaN       	NaN       	NaN       	NaN       	NaN
    //T1        	RefTask   	0         	1.00000   	NaN       	0.70000   	NaN       	1.42857
    //T2        	Task      	0         	0         	NaN       	0         	NaN       	1.42857
    //T3        	Task      	NaN       	NaN       	NaN       	0         	NaN       	Infinity
    //E1        	Entry     	0         	NaN       	0.70000   	NaN       	NaN       	1.42857
    //E2        	Entry     	0         	NaN       	0         	NaN       	NaN       	1.42857
    //E3        	Entry     	NaN       	NaN       	0         	NaN       	NaN       	Infinity
    //AS1       	Activity  	0         	1.00000   	0.70000   	0.70000   	NaN       	1.42857
    //AS2       	Activity  	0         	0         	0         	0         	NaN       	1.42857
    //AS3       	Activity  	NaN       	0         	0         	0         	NaN       	Infinity
    @Test
    @Timeout(120)
    public void testDebug() {
        LayeredNetwork  model = new LayeredNetwork("test");
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
        new Activity(model, "AS3", Immediate.getInstance()).on(t3).boundTo(e3).repliesTo(e2);

        assertResultsMatchSolverLN(model);
    }

    //=== SolverLN ===
    //Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput
    //P1        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN
    //P2        	Processor 	NaN       	0.18367   	NaN       	NaN       	NaN       	NaN
    //P3        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN
    //T1        	RefTask   	0.36735   	0         	NaN       	0         	NaN       	0.40816
    //T2        	Task      	0.36735   	0.18367   	NaN       	0.90000   	NaN       	0.40816
    //T3        	Task      	0         	0         	NaN       	0         	NaN       	0.40816
    //E1        	Entry     	0.36735   	NaN       	0.90000   	NaN       	NaN       	0.40816
    //E2        	Entry     	0.36735   	NaN       	0.90000   	NaN       	NaN       	0.40816
    //E3        	Entry     	0         	NaN       	0         	NaN       	NaN       	0.40816
    //AS1       	Activity  	0.36735   	0         	0.90000   	0         	NaN       	0.40816
    //AS2       	Activity  	0.36735   	0.18367   	0.90000   	0.90000   	NaN       	0.40816
    //AS3       	Activity  	0         	0         	0         	0         	NaN       	0.40816
    //
    //=== SolverLNSimple ===
    //Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput
    //P1        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN
    //P2        	Processor 	NaN       	1.00000   	NaN       	NaN       	NaN       	NaN
    //P3        	Processor 	NaN       	NaN       	NaN       	NaN       	NaN       	NaN
    //T1        	RefTask   	0.45000   	0         	NaN       	0         	NaN       	0.50000
    //T2        	Task      	2.00000   	1.00000   	NaN       	0.90000   	NaN       	2.22222
    //T3        	Task      	NaN       	NaN       	NaN       	0         	NaN       	NaN
    //E1        	Entry     	0.45000   	NaN       	0.90000   	NaN       	NaN       	0.50000
    //E2        	Entry     	2.00000   	NaN       	0.90000   	NaN       	NaN       	2.22222
    //E3        	Entry     	NaN       	NaN       	0         	NaN       	NaN       	NaN
    //AS1       	Activity  	0.45000   	0         	0.90000   	0         	NaN       	0.50000
    //AS2       	Activity  	2.00000   	1.00000   	0.90000   	0.90000   	NaN       	2.22222
    //AS3       	Activity  	NaN       	0         	0         	0         	NaN       	NaN
    @Test
    @Timeout(120)
    public void testDebug2() {
        LayeredNetwork model = new LayeredNetwork("test2");
        Processor P1 = new Processor(model, "P1", 1, SchedStrategy.PS);
        Processor P2 = new Processor(model, "P2", 1, SchedStrategy.PS);
        Processor P3 = new Processor(model, "P3", 1, SchedStrategy.PS);
        Task T1 = new Task(model, "T1", 1, SchedStrategy.REF).on(P1).setThinkTime(new Exp(1.0/4));
        Task T2 = new Task(model, "T2", 1, SchedStrategy.FCFS).on(P2);
        Task T3 = new Task(model, "T3", 1, SchedStrategy.FCFS).on(P3);
        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        Entry E3 = new Entry(model, "E3").on(T3);
        new Activity(model, "AS1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);
        new Activity(model, "AS2", Exp.fitMean(0.9)).on(T2).boundTo(E2).synchCall(E3, 1).repliesTo(E1);
        new Activity(model, "AS3", Immediate.getInstance()).on(T3).boundTo(E3).repliesTo(E2);

        assertResultsMatchSolverLN(model);
    }
}
