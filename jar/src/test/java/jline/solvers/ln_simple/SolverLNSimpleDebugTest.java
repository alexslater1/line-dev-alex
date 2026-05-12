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
    //USED TO DEBUG ANY FAILING BUGS/TESTS

    //Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput
    //P1        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN
    //P2        	Processor 	NaN       	0.99986   	NaN       	NaN       	NaN       	NaN
    //T1        	RefTask   	1.62505   	0         	NaN       	0         	NaN       	0.12498
    //T2        	RefTask   	4.87516   	0         	NaN       	0         	NaN       	0.37495
    //T3        	Task      	6.50021   	0.99986   	NaN       	13.00227  	NaN       	0.49993
    //E1        	Entry     	1.62505   	NaN       	13.00227  	NaN       	NaN       	0.12498
    //E2        	Entry     	4.87516   	NaN       	13.00227  	NaN       	NaN       	0.37495
    //E3        	Entry     	1.62505   	NaN       	13.00227  	NaN       	NaN       	0.12498
    //E4        	Entry     	4.87516   	NaN       	13.00227  	NaN       	NaN       	0.37495
    //A1        	Activity  	1.62505   	0         	13.00227  	0         	NaN       	0.12498
    //A2        	Activity  	4.87516   	0         	13.00227  	0         	NaN       	0.37495
    //A3        	Activity  	1.62505   	0.24996   	13.00227  	3.25057   	NaN       	0.12498
    //A4        	Activity  	4.87516   	0.74989   	13.00227  	9.75170   	NaN       	0.37495
    @Test @Timeout(120)
    public void passingSharedServerWithSameDemandsTest() throws Exception {
        LayeredNetwork model = new LayeredNetwork("mc_b2");
        Processor P1 = new Processor(model, "P1", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor P2 = new Processor(model, "P2", 1, SchedStrategy.PS);
        Task T1 = new Task(model, "T1", 2, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(3.0));
        Task T2 = new Task(model, "T2", 6, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(3.0));
        Task T3 = new Task(model, "T3", Integer.MAX_VALUE, SchedStrategy.INF).on(P2);
        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        Entry E3 = new Entry(model, "E3").on(T3);
        Entry E4 = new Entry(model, "E4").on(T3);
        new Activity(model, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E3, 1);
        new Activity(model, "A2", Immediate.getInstance()).on(T2).boundTo(E2).synchCall(E4, 1);
        new Activity(model, "A3", Exp.fitMean(2.0)).on(T3).boundTo(E3).repliesTo(E3);
        new Activity(model, "A4", Exp.fitMean(2.0)).on(T3).boundTo(E4).repliesTo(E4);
        assertResultsMatchSolverLN(model);
    }

    //=== SolverLN ===
    //Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput
    //P1        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN
    //P2        	Processor 	NaN       	0.98869   	NaN       	NaN       	NaN       	NaN
    //T1        	RefTask   	1.53006   	0         	NaN       	0         	NaN       	0.15665
    //T2        	RefTask   	3.29843   	0         	NaN       	0         	NaN       	0.90052
    //T3        	Task      	4.82849   	0.98869   	NaN       	4.56737   	NaN       	1.05717
    //E1        	Entry     	1.53006   	NaN       	9.76747   	NaN       	NaN       	0.15665
    //E2        	Entry     	3.29843   	NaN       	3.66280   	NaN       	NaN       	0.90052
    //E3        	Entry     	1.53006   	NaN       	9.76746   	NaN       	NaN       	0.15665
    //E4        	Entry     	3.29843   	NaN       	3.66280   	NaN       	NaN       	0.90052
    //A1        	Activity  	1.53005   	0         	9.76746   	0         	NaN       	0.15665
    //A2        	Activity  	3.29843   	0         	3.66280   	0         	NaN       	0.90052
    //A3        	Activity  	1.53006   	0.31330   	9.76746   	1.44731   	NaN       	0.15665
    //A4        	Activity  	3.29843   	0.67539   	3.66280   	3.12006   	NaN       	0.90052
    //=== SolverLNSimple ===
    //Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput
    //P1        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN
    //P2        	Processor 	NaN       	0.99403   	NaN       	NaN       	NaN       	NaN
    //T1        	RefTask   	1.29833   	0         	NaN       	0         	NaN       	0.23389
    //T2        	RefTask   	3.89499   	0         	NaN       	0         	NaN       	0.70167
    //T3        	Task      	5.19332   	0.99403   	NaN       	5.55102   	NaN       	0.93556
    //E1        	Entry     	1.29833   	NaN       	5.55102   	NaN       	NaN       	0.23389
    //E2        	Entry     	3.89499   	NaN       	5.55102   	NaN       	NaN       	0.70167
    //E3        	Entry     	2.44391   	NaN       	10.44897  	NaN       	NaN       	0.23389
    //E4        	Entry     	2.74940   	NaN       	3.91836   	NaN       	NaN       	0.70167
    //A1        	Activity  	1.29833   	0         	5.55102   	0         	NaN       	0.23389
    //A2        	Activity  	3.89499   	0         	5.55102   	0         	NaN       	0.70167
    //A3        	Activity  	2.44391   	0.46778   	10.44897  	2.61224   	NaN       	0.23389
    //A4        	Activity  	2.74940   	0.52625   	3.91836   	2.93877   	NaN       	0.70167
    @Test @Timeout(120)
    public void debugFailingSharedServerWithDifferingDemandsTest() throws Exception {
        LayeredNetwork model = new LayeredNetwork("mc_b3");
        Processor P1 = new Processor(model, "P1", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor P2 = new Processor(model, "P2", 1, SchedStrategy.PS);
        Task T1 = new Task(model, "T1", 2, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(3.0));
        Task T2 = new Task(model, "T2", 6, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(3.0));
        Task T3 = new Task(model, "T3", Integer.MAX_VALUE, SchedStrategy.INF).on(P2);
        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        Entry E3 = new Entry(model, "E3").on(T3);
        Entry E4 = new Entry(model, "E4").on(T3);
        new Activity(model, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E3, 1);
        new Activity(model, "A2", Immediate.getInstance()).on(T2).boundTo(E2).synchCall(E4, 1);
        new Activity(model, "A3", Exp.fitMean(2.0)).on(T3).boundTo(E3).repliesTo(E3);
        new Activity(model, "A4", Exp.fitMean(0.75)).on(T3).boundTo(E4).repliesTo(E4);
        assertResultsMatchSolverLN(model);
    }
}
