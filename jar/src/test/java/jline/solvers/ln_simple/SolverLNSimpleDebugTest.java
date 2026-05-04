package jline.solvers.ln_simple;

import jline.lang.constant.SchedStrategy;
import jline.lang.layered.*;
import jline.lang.processes.Exp;
import jline.lang.processes.Immediate;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.Timeout;

import static jline.solvers.ln_simple.util.assertResultsMatchSolverLN;

public class SolverLNSimpleDebugTest {

//=== SolverLN ===
//    Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput
//    P1        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN
//    P2        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN
//    P3        	Processor 	NaN       	1.00000   	NaN       	NaN       	NaN       	NaN
//    T1        	RefTask   	3.00000   	0         	NaN       	0         	NaN       	1.11111
//    T2        	Task      	2.00000   	0         	NaN       	0         	NaN       	1.11111
//    T3        	Task      	1.00000   	1.00000   	NaN       	0.90000   	NaN       	1.11111
//    E1        	Entry     	3.00000   	NaN       	2.70000   	NaN       	NaN       	1.11111
//    E2        	Entry     	2.00000   	NaN       	1.80000   	NaN       	NaN       	1.11111
//    E3        	Entry     	1.00000   	NaN       	0.90000   	NaN       	NaN       	1.11111
//    AS1       	Activity  	3.00000   	0         	2.70000   	0         	NaN       	1.11111
//    AS2       	Activity  	2.00000   	0         	1.80000   	0         	NaN       	1.11111
//    AS3       	Activity  	1.00000   	1.00000   	0.90000   	0.90000   	NaN       	1.11111
//
//            === SolverLNSimple ===
//    Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput
//    P1        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN
//    P2        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN
//    P3        	Processor 	NaN       	1.00000   	NaN       	NaN       	NaN       	NaN
//    T1        	RefTask   	3.00000   	0         	NaN       	0         	NaN       	1.66667
//    T2        	Task      	3.00000   	0         	NaN       	0         	NaN       	1.66667
//    T3        	Task      	1.00000   	1.00000   	NaN       	0.90000   	NaN       	1.11111
//    E1        	Entry     	3.00000   	NaN       	1.80000   	NaN       	NaN       	1.66667
//    E2        	Entry     	3.00000   	NaN       	1.80000   	NaN       	NaN       	1.66667
//    E3        	Entry     	1.00000   	NaN       	0.90000   	NaN       	NaN       	1.11111
//    AS1       	Activity  	3.00000   	0         	1.80000   	0         	NaN       	1.66667
//    AS2       	Activity  	3.00000   	0         	1.80000   	0         	NaN       	1.66667
//    AS3       	Activity  	1.00000   	1.00000   	0.90000   	0.90000   	NaN       	1.11111
    @Test
    @Timeout(120)
    public void testBugDecreasingTaskMultiplicity() {
        LayeredNetwork model = new LayeredNetwork("Decreasing Task Multiplicity (Failing)");

        Processor P1 = new Processor(model, "P1", 2, SchedStrategy.PS);
        Processor P2 = new Processor(model, "P2", 2, SchedStrategy.PS);
        Processor P3 = new Processor(model, "P3", 2, SchedStrategy.PS);

        Task T1 = new Task(model, "T1", 3, SchedStrategy.REF).on(P1);
        Task T2 = new Task(model, "T2", 2, SchedStrategy.FCFS).on(P2);
        Task T3 = new Task(model, "T3", 1, SchedStrategy.FCFS).on(P3);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        Entry E3 = new Entry(model, "E3").on(T3);

        new Activity(model, "AS1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);
        new Activity(model, "AS2", Immediate.getInstance()).on(T2).boundTo(E2).synchCall(E3, 1).repliesTo(E1);
        new Activity(model, "AS3", Exp.fitMean(0.9)).on(T3).boundTo(E3).repliesTo(E2);

        assertResultsMatchSolverLN(model);
    }
}
