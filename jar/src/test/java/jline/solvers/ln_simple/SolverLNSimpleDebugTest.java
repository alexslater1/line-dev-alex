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
    //P1        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN
    //P2        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN
    //P3        	Processor 	NaN       	0.42857   	NaN       	NaN       	NaN       	NaN
    //T1        	RefTask   	0.69231   	0         	NaN       	0         	NaN       	0.76923
    //T2        	Task      	0.42857   	0         	NaN       	0         	NaN       	0.47619
    //T3        	Task      	0.42857   	0.42857   	NaN       	0.90000   	NaN       	0.47619
    //E1        	Entry     	0.69231   	NaN       	0.90000   	NaN       	NaN       	0.76923
    //E2        	Entry     	0.42857   	NaN       	0.90000   	NaN       	NaN       	0.47619
    //E3        	Entry     	0.42857   	NaN       	0.90000   	NaN       	NaN       	0.47619
    //AS1       	Activity  	0.69231   	0         	0.90000   	0         	NaN       	0.76923
    //AS2       	Activity  	0.42857   	0         	0.90000   	0         	NaN       	0.47619
    //AS3       	Activity  	0.42857   	0.42857   	0.90000   	0.90000   	NaN       	0.47619
    //=== SolverLNSimple ===
    //Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput
    //P1        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN
    //P2        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN
    //P3        	Processor 	NaN       	0.69231   	NaN       	NaN       	NaN       	NaN
    //T1        	RefTask   	0.69231   	0         	NaN       	0         	NaN       	0.76923
    //T2        	Task      	0.69231   	0         	NaN       	0         	NaN       	0.76923
    //T3        	Task      	0.69231   	0.69231   	NaN       	0.90000   	NaN       	0.76923
    //E1        	Entry     	0.69231   	NaN       	0.90000   	NaN       	NaN       	0.76923
    //E2        	Entry     	0.69231   	NaN       	0.90000   	NaN       	NaN       	0.76923
    //E3        	Entry     	0.69231   	NaN       	0.90000   	NaN       	NaN       	0.76923
    //AS1       	Activity  	0.69231   	0         	0.90000   	0         	NaN       	0.76923
    //AS2       	Activity  	0.69231   	0         	0.90000   	0         	NaN       	0.76923
    //AS3       	Activity  	0.69231   	0.69231   	0.90000   	0.90000   	NaN       	0.76923
    @Test
    @Timeout(120)
    public void testDebug() {
        LayeredNetwork  model = new LayeredNetwork("test");
        Processor p1 = new Processor(model, "P1", 1, SchedStrategy.PS);
        Processor p2 = new Processor(model, "P2", 1, SchedStrategy.PS);
        Processor p3 = new Processor(model, "P3", 1, SchedStrategy.PS);
        Task t1 = new Task(model, "T1", 1, SchedStrategy.REF).on(p1).setThinkTime(Exp.fitMean(0.4));
        Task t2 = new Task(model, "T2", 1, SchedStrategy.FCFS).on(p2).setThinkTime(Exp.fitMean(1.2));
        Task t3 = new Task(model, "T3", 1, SchedStrategy.FCFS).on(p3);
        Entry e1 = new Entry(model, "E1").on(t1);
        Entry e2 = new Entry(model, "E2").on(t2);
        Entry e3 = new Entry(model, "E3").on(t3);
        new Activity(model, "AS1", Immediate.getInstance()).on(t1).boundTo(e1).synchCall(e2, 1);
        new Activity(model, "AS2", Immediate.getInstance()).on(t2).boundTo(e2).repliesTo(e1).synchCall(e3, 1);
        new Activity(model, "AS3", Exp.fitMean(0.9)).on(t3).boundTo(e3).repliesTo(e2);

        assertResultsMatchSolverLN(model);
    }

    //=== SolverLN ===
    //Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput
    //P1        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN
    //P2        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN
    //P3        	Processor 	NaN       	0.42857   	NaN       	NaN       	NaN       	NaN
    //T1        	RefTask   	1.00000   	0         	NaN       	0         	NaN       	1.11111
    //T2        	Task      	0.42857   	0         	NaN       	0         	NaN       	0.47619
    //T3        	Task      	0.42857   	0.42857   	NaN       	0.90000   	NaN       	0.47619
    //E1        	Entry     	1.00000   	NaN       	0.90000   	NaN       	NaN       	1.11111
    //E2        	Entry     	0.42857   	NaN       	0.90000   	NaN       	NaN       	0.47619
    //E3        	Entry     	0.42857   	NaN       	0.90000   	NaN       	NaN       	0.47619
    //AS1       	Activity  	1.00000   	0         	0.90000   	0         	NaN       	1.11111
    //AS2       	Activity  	0.42857   	0         	0.90000   	0         	NaN       	0.47619
    //AS3       	Activity  	0.42857   	0.42857   	0.90000   	0.90000   	NaN       	0.47619
    //=== SolverLNSimple ===
    //Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput
    //P1        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN
    //P2        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN
    //P3        	Processor 	NaN       	1.00000   	NaN       	NaN       	NaN       	NaN
    //T1        	RefTask   	1.00000   	0         	NaN       	0         	NaN       	1.11111
    //T2        	Task      	1.00000   	0         	NaN       	0         	NaN       	1.11111
    //T3        	Task      	1.00000   	1.00000   	NaN       	0.90000   	NaN       	1.11111
    //E1        	Entry     	1.00000   	NaN       	0.90000   	NaN       	NaN       	1.11111
    //E2        	Entry     	1.00000   	NaN       	0.90000   	NaN       	NaN       	1.11111
    //E3        	Entry     	1.00000   	NaN       	0.90000   	NaN       	NaN       	1.11111
    //AS1       	Activity  	1.00000   	0         	0.90000   	0         	NaN       	1.11111
    //AS2       	Activity  	1.00000   	0         	0.90000   	0         	NaN       	1.11111
    //AS3       	Activity  	1.00000   	1.00000   	0.90000   	0.90000   	NaN       	1.11111
    @Test
    @Timeout(120)
    public void testDebug2() {
        LayeredNetwork  model = new LayeredNetwork("test2");
        Processor p1 = new Processor(model, "P1", 1, SchedStrategy.PS);
        Processor p2 = new Processor(model, "P2", 1, SchedStrategy.PS);
        Processor p3 = new Processor(model, "P3", 1, SchedStrategy.PS);
        Task t1 = new Task(model, "T1", 1, SchedStrategy.REF).on(p1);
        Task t2 = new Task(model, "T2", 1, SchedStrategy.FCFS).on(p2).setThinkTime(Exp.fitMean(1.2));
        Task t3 = new Task(model, "T3", 1, SchedStrategy.FCFS).on(p3);
        Entry e1 = new Entry(model, "E1").on(t1);
        Entry e2 = new Entry(model, "E2").on(t2);
        Entry e3 = new Entry(model, "E3").on(t3);
        new Activity(model, "AS1", Immediate.getInstance()).on(t1).boundTo(e1).synchCall(e2, 1);
        new Activity(model, "AS2", Immediate.getInstance()).on(t2).boundTo(e2).repliesTo(e1).synchCall(e3, 1);
        new Activity(model, "AS3", Exp.fitMean(0.9)).on(t3).boundTo(e3).repliesTo(e2);

        assertResultsMatchSolverLN(model);
    }
}
