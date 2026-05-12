package jline.solvers.ln_simple.fixtures;

import java.util.Arrays;

import jline.examples.java.basic.LayeredModel;
import jline.lang.constant.SchedStrategy;
import jline.lang.layered.*;
import jline.lang.processes.Exp;
import jline.lang.processes.Immediate;
import jline.util.matrix.Matrix;

public class SingleclassLNExamples {

    // Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput      
    // --------------------------------------------------------------------------------------------
    // P1        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN       
    // P2        	Processor 	NaN       	0.31034   	NaN       	NaN       	NaN       	NaN       
    // T1        	RefTask   	0.31034   	0         	NaN       	0         	NaN       	0.34483   
    // T2        	Task      	0.31034   	0.31034   	NaN       	0.90000   	NaN       	0.34483   
    // E1        	Entry     	0.31034   	NaN       	0.90000   	NaN       	NaN       	0.34483   
    // E2        	Entry     	0.31034   	NaN       	0.90000   	NaN       	NaN       	0.34483   
    // AS1       	Activity  	0.31034   	0         	0.90000   	0         	NaN       	0.34483   
    // AS2       	Activity  	0.31034   	0.31034   	0.90000   	0.90000   	NaN       	0.34483   
    // --------------------------------------------------------------------------------------------
    public static LayeredNetwork twoLayerSingleProcessorNetwork() {
        LayeredNetwork model = new LayeredNetwork("TwoLayerLN");

        Processor P1 = new Processor(model, "P1", 1, SchedStrategy.PS);
        Processor P2 = new Processor(model, "P2", 1, SchedStrategy.PS);

        Task T1 = new Task(model, "T1", 1, SchedStrategy.REF).on(P1).setThinkTime(new Exp(1.0/2));
        Task T2 = new Task(model, "T2", 1, SchedStrategy.FCFS).on(P2);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);

        new Activity(model, "AS1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1.0);
        new Activity(model, "AS2", Exp.fitMean(0.9)).on(T2).boundTo(E2).repliesTo(E2);

        return model;
    }

    //     Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput
    // --------------------------------------------------------------------------------------------
    //     P1        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN
    //     P2        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN
    //     P3        	Processor 	NaN       	0.31034   	NaN       	NaN       	NaN       	NaN
    //     T1        	RefTask   	0.31034   	0         	NaN       	0         	NaN       	0.34483
    //     T2        	Task      	0.31034   	0         	NaN       	0         	NaN       	0.34483
    //     T3        	Task      	0.31034   	0.31034   	NaN       	0.90000   	NaN       	0.34483
    //     E1        	Entry     	0.31034   	NaN       	0.90000   	NaN       	NaN       	0.34483
    //     E2        	Entry     	0.31034   	NaN       	0.90000   	NaN       	NaN       	0.34483
    //     E3        	Entry     	0.31034   	NaN       	0.90000   	NaN       	NaN       	0.34483
    //     AS1       	Activity  	0.31034   	0         	0.90000   	0         	NaN       	0.34483
    //     AS2       	Activity  	0.31034   	0         	0.90000   	0         	NaN       	0.34483
    //     AS3       	Activity  	0.31034   	0.31034   	0.90000   	0.90000   	NaN       	0.34483
    public static LayeredNetwork threeLayerSingleProcessorNetwork() {
        LayeredNetwork model = new LayeredNetwork("ThreeLayerLN");

        Processor P1 = new Processor(model, "P1", 1, SchedStrategy.PS);
        Processor P2 = new Processor(model, "P2", 1, SchedStrategy.PS);
        Processor P3 = new Processor(model, "P3", 1, SchedStrategy.PS);

        Task T1 = new Task(model, "T1", 1, SchedStrategy.REF).on(P1).setThinkTime(new Exp(1.0/2));
        Task T2 = new Task(model, "T2", 1, SchedStrategy.FCFS).on(P2);
        Task T3 = new Task(model, "T3", 1, SchedStrategy.FCFS).on(P3);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        Entry E3 = new Entry(model, "E3").on(T3);


        new Activity(model, "AS1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);
        new Activity(model, "AS2", Immediate.getInstance()).on(T2).boundTo(E2).synchCall(E3, 1).repliesTo(E1);
        new Activity(model, "AS3", Exp.fitMean(0.9)).on(T3).boundTo(E3).repliesTo(E2);

        return model;
    }

    // Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput      
    // --------------------------------------------------------------------------------------------
    // P1        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN       
    // P2        	Processor 	NaN       	0.45866   	NaN       	NaN       	NaN       	NaN       
    // T1        	RefTask   	0.96150   	0         	NaN       	0         	NaN       	1.01925   
    // T2        	Task      	0.96150   	0.45866   	NaN       	0.94334   	NaN       	1.01925   
    // E1        	Entry     	0.96150   	NaN       	0.94334   	NaN       	NaN       	1.01925   
    // E2        	Entry     	0.96150   	NaN       	0.94334   	NaN       	NaN       	1.01925   
    // AS1       	Activity  	0.96150   	0         	0.94334   	0         	NaN       	1.01925   
    // AS2       	Activity  	0.96150   	0.45866   	0.94334   	0.94334   	NaN       	1.01925   
    // --------------------------------------------------------------------------------------------
    public static LayeredNetwork twoLayerMultiProcessorNetwork() {
        LayeredNetwork model = new LayeredNetwork("TwoLayerLN");

        Processor P1 = new Processor(model, "P1", 2, SchedStrategy.PS);
        Processor P2 = new Processor(model, "P2", 2, SchedStrategy.PS);

        Task T1 = new Task(model, "T1", 3, SchedStrategy.REF).on(P1).setThinkTime(new Exp(1.0/2));
        Task T2 = new Task(model, "T2", 3, SchedStrategy.FCFS).on(P2);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);

        new Activity(model, "AS1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1.0);
        new Activity(model, "AS2", Exp.fitMean(0.9)).on(T2).boundTo(E2).repliesTo(E2);

        return model;
    }

    // Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput      
    // --------------------------------------------------------------------------------------------
    // P1        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN       
    // P2        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN       
    // P3        	Processor 	NaN       	0.45866   	NaN       	NaN       	NaN       	NaN       
    // T1        	RefTask   	0.96150   	0         	NaN       	0         	NaN       	1.01925   
    // T2        	Task      	0.96150   	0         	NaN       	0         	NaN       	1.01925   
    // T3        	Task      	0.96150   	0.45866   	NaN       	0.94334   	NaN       	1.01925   
    // E1        	Entry     	0.96150   	NaN       	0.94334   	NaN       	NaN       	1.01925   
    // E2        	Entry     	0.96150   	NaN       	0.94334   	NaN       	NaN       	1.01925   
    // E3        	Entry     	0.96150   	NaN       	0.94334   	NaN       	NaN       	1.01925   
    // AS1       	Activity  	0.96150   	0         	0.94334   	0         	NaN       	1.01925   
    // AS2       	Activity  	0.96150   	0         	0.94334   	0         	NaN       	1.01925   
    // AS3       	Activity  	0.96150   	0.45866   	0.94334   	0.94334   	NaN       	1.01925   
    // --------------------------------------------------------------------------------------------
    public static LayeredNetwork threeLayerMultiProcessorNetwork() {
        LayeredNetwork model = new LayeredNetwork("ThreeLayerLN");

        Processor P1 = new Processor(model, "P1", 2, SchedStrategy.PS);
        Processor P2 = new Processor(model, "P2", 2, SchedStrategy.PS);
        Processor P3 = new Processor(model, "P3", 2, SchedStrategy.PS);

        Task T1 = new Task(model, "T1", 3, SchedStrategy.REF).on(P1).setThinkTime(new Exp(1.0/2));
        Task T2 = new Task(model, "T2", 3, SchedStrategy.FCFS).on(P2);
        Task T3 = new Task(model, "T3", 3, SchedStrategy.FCFS).on(P3);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        Entry E3 = new Entry(model, "E3").on(T3);


        new Activity(model, "AS1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);
        new Activity(model, "AS2", Immediate.getInstance()).on(T2).boundTo(E2).synchCall(E3, 1).repliesTo(E1);
        new Activity(model, "AS3", Exp.fitMean(0.9)).on(T3).boundTo(E3).repliesTo(E2);

        return model;
    }

    // Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput      
    // --------------------------------------------------------------------------------------------
    // P1        	Processor 	NaN       	0.14706   	NaN       	NaN       	NaN       	NaN       
    // P2        	Processor 	NaN       	0.26471   	NaN       	NaN       	NaN       	NaN       
    // T1        	RefTask   	0.41176   	0.14706   	NaN       	0.50000   	NaN       	0.29412   
    // T2        	Task      	0.26471   	0.26471   	NaN       	0.90000   	NaN       	0.29412   
    // E1        	Entry     	0.41176   	NaN       	1.40000   	NaN       	NaN       	0.29412   
    // E2        	Entry     	0.26471   	NaN       	0.90000   	NaN       	NaN       	0.29412   
    // AS1       	Activity  	0.41176   	0.14706   	1.40000   	0.50000   	NaN       	0.29412   
    // AS2       	Activity  	0.26471   	0.26471   	0.90000   	0.90000   	NaN       	0.29412   
    // --------------------------------------------------------------------------------------------
    public static LayeredNetwork twoLayerSingleProcessorNetworkUPDATED() {
        LayeredNetwork model = new LayeredNetwork("TwoLayerLN");

        Processor P1 = new Processor(model, "P1", 1, SchedStrategy.PS);
        Processor P2 = new Processor(model, "P2", 1, SchedStrategy.PS);

        Task T1 = new Task(model, "T1", 1, SchedStrategy.REF).on(P1).setThinkTime(new Exp(1.0/2));
        Task T2 = new Task(model, "T2", 1, SchedStrategy.FCFS).on(P2);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);

        new Activity(model, "AS1", Exp.fitMean(0.5)).on(T1).boundTo(E1).synchCall(E2, 1.0);
        new Activity(model, "AS2", Exp.fitMean(0.9)).on(T2).boundTo(E2).repliesTo(E2);

        return model;
    }

    // Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput      
    // P1        	Processor 	NaN       	0.14706   	NaN       	NaN       	NaN       	NaN       
    // P2        	Processor 	NaN       	0.18367   	NaN       	NaN       	NaN       	NaN       
    // T1        	RefTask   	0.41176   	0.14706   	NaN       	0.50000   	NaN       	0.29412   
    // T2        	Task      	0.18367   	0.18367   	NaN       	0.90000   	NaN       	0.20408   
    // E1        	Entry     	0.41176   	NaN       	1.40000   	NaN       	NaN       	0.29412   
    // E2        	Entry     	0.18367   	NaN       	0.90000   	NaN       	NaN       	0.20408   
    // AS1       	Activity  	0.41176   	0.14706   	1.40000   	0.50000   	NaN       	0.29412   
    // AS2       	Activity  	0.18367   	0.18367   	0.90000   	0.90000   	NaN       	0.20408   
    public static LayeredNetwork twoLayerSingleProcessorNetworkUPDATED2() {
        LayeredNetwork model = new LayeredNetwork("TwoLayerLN");

        Processor P1 = new Processor(model, "P1", 1, SchedStrategy.PS);
        Processor P2 = new Processor(model, "P2", 1, SchedStrategy.PS);

        Task T1 = new Task(model, "T1", 1, SchedStrategy.REF).on(P1).setThinkTime(new Exp(1.0/2));
        Task T2 = new Task(model, "T2", 1, SchedStrategy.FCFS).on(P2).setThinkTime(new Exp(1.0/4));

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);

        new Activity(model, "AS1", Exp.fitMean(0.5)).on(T1).boundTo(E1).synchCall(E2, 1.0);
        new Activity(model, "AS2", Exp.fitMean(0.9)).on(T2).boundTo(E2).repliesTo(E2);

        return model;
    }


//        Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput
//    --------------------------------------------------------------------------------------------
//        P1        	Processor 	NaN       	0.11111   	NaN       	NaN       	NaN       	NaN
//        P2        	Processor 	NaN       	0.24444   	NaN       	NaN       	NaN       	NaN
//        P3        	Processor 	NaN       	0.20000   	NaN       	NaN       	NaN       	NaN
//        T1        	RefTask   	0.55556   	0.11111   	NaN       	0.50000   	NaN       	0.22222
//        T2        	Task      	0.44444   	0.24444   	NaN       	1.10000   	NaN       	0.22222
//        T3        	Task      	0.20000   	0.20000   	NaN       	0.90000   	NaN       	0.22222
//        E1        	Entry     	0.55556   	NaN       	2.50000   	NaN       	NaN       	0.22222
//        E2        	Entry     	0.44444   	NaN       	2.00000   	NaN       	NaN       	0.22222
//        E3        	Entry     	0.20000   	NaN       	0.90000   	NaN       	NaN       	0.22222
//        AS1       	Activity  	0.55556   	0.11111   	2.50000   	0.50000   	NaN       	0.22222
//        AS2       	Activity  	0.44444   	0.24444   	2.00000   	1.10000   	NaN       	0.22222
//        AS3       	Activity  	0.20000   	0.20000   	0.90000   	0.90000   	NaN       	0.22222
    public static LayeredNetwork threeLayerSingleProcessorNetworkUPDATED() {
        LayeredNetwork model = new LayeredNetwork("ThreeLayerLN");

        Processor P1 = new Processor(model, "P1", 1, SchedStrategy.PS);
        Processor P2 = new Processor(model, "P2", 1, SchedStrategy.PS);
        Processor P3 = new Processor(model, "P3", 1, SchedStrategy.PS);

        Task T1 = new Task(model, "T1", 1, SchedStrategy.REF).on(P1).setThinkTime(new Exp(1.0/2));
        Task T2 = new Task(model, "T2", 1, SchedStrategy.FCFS).on(P2);
        Task T3 = new Task(model, "T3", 1, SchedStrategy.FCFS).on(P3);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        Entry E3 = new Entry(model, "E3").on(T3);


        new Activity(model, "AS1", Exp.fitMean(0.5)).on(T1).boundTo(E1).synchCall(E2, 1);
        new Activity(model, "AS2", Exp.fitMean(1.1)).on(T2).boundTo(E2).synchCall(E3, 1).repliesTo(E1);
        new Activity(model, "AS3", Exp.fitMean(0.9)).on(T3).boundTo(E3).repliesTo(E2);

        return model;
    }

    public static LayeredNetwork test() {
        LayeredNetwork model = new LayeredNetwork("Test");

        Processor P1 = new Processor(model, "P1", 2, SchedStrategy.PS);
        Processor P2 = new Processor(model, "P2", 3, SchedStrategy.PS);
        Processor P3 = new Processor(model, "P3", 7, SchedStrategy.PS);


        Task T1 = new Task(model, "T1", 2, SchedStrategy.REF).on(P1).setThinkTime(new Exp(1.0/4));
        Task T2 = new Task(model, "T2", 2, SchedStrategy.FCFS).on(P2);
        Task T3 = new Task(model, "T3", 2, SchedStrategy.FCFS).on(P3);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        Entry E3 = new Entry(model, "E3").on(T3);

        new Activity(model, "AS1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);
        new Activity(model, "AS2", Immediate.getInstance()).on(T2).boundTo(E2).synchCall(E3, 1).repliesTo(E1);
        new Activity(model, "AS3", Exp.fitMean(0.9)).on(T3).boundTo(E3).repliesTo(E2);

        return model;
    }

    public static LayeredNetwork bug1network1() {
        LayeredNetwork model = new LayeredNetwork("Increasing Task Multiplicity (Passing)");

        Processor P1 = new Processor(model, "P1", 2, SchedStrategy.PS);
        Processor P2 = new Processor(model, "P2", 2, SchedStrategy.PS);
        Processor P3 = new Processor(model, "P3", 2, SchedStrategy.PS);

        Task T1 = new Task(model, "T1", 2, SchedStrategy.REF).on(P1).setThinkTime(new Exp(1.0/4));
        Task T2 = new Task(model, "T2", 2, SchedStrategy.FCFS).on(P2);
        Task T3 = new Task(model, "T3", 3, SchedStrategy.FCFS).on(P3);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        Entry E3 = new Entry(model, "E3").on(T3);

        new Activity(model, "AS1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);
        new Activity(model, "AS2", Immediate.getInstance()).on(T2).boundTo(E2).synchCall(E3, 1).repliesTo(E1);
        new Activity(model, "AS3", Exp.fitMean(0.9)).on(T3).boundTo(E3).repliesTo(E2);

        return model;
    }

    public static LayeredNetwork bug1network2() {
        LayeredNetwork model = new LayeredNetwork("Equal Task Multiplicity (Passing)");

        Processor P1 = new Processor(model, "P1", 2, SchedStrategy.PS);
        Processor P2 = new Processor(model, "P2", 2, SchedStrategy.PS);
        Processor P3 = new Processor(model, "P3", 2, SchedStrategy.PS);

        Task T1 = new Task(model, "T1", 2, SchedStrategy.REF).on(P1).setThinkTime(new Exp(1.0/4));
        Task T2 = new Task(model, "T2", 2, SchedStrategy.FCFS).on(P2);
        Task T3 = new Task(model, "T3", 2, SchedStrategy.FCFS).on(P3);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        Entry E3 = new Entry(model, "E3").on(T3);

        new Activity(model, "AS1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);
        new Activity(model, "AS2", Immediate.getInstance()).on(T2).boundTo(E2).synchCall(E3, 1).repliesTo(E1);
        new Activity(model, "AS3", Exp.fitMean(0.9)).on(T3).boundTo(E3).repliesTo(E2);

        return model;
    }

    public static LayeredNetwork bug1network3() {
        LayeredNetwork model = new LayeredNetwork("Decreasing Task Multiplicity (Failing)");

        Processor P1 = new Processor(model, "P1", 2, SchedStrategy.PS);
        Processor P2 = new Processor(model, "P2", 2, SchedStrategy.PS);
        Processor P3 = new Processor(model, "P3", 2, SchedStrategy.PS);

        Task T1 = new Task(model, "T1", 2, SchedStrategy.REF).on(P1);
        Task T2 = new Task(model, "T2", 2, SchedStrategy.FCFS).on(P2);
        Task T3 = new Task(model, "T3", 1, SchedStrategy.FCFS).on(P3);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        Entry E3 = new Entry(model, "E3").on(T3);

        new Activity(model, "AS1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);
        new Activity(model, "AS2", Immediate.getInstance()).on(T2).boundTo(E2).synchCall(E3, 1).repliesTo(E1);
        new Activity(model, "AS3", Exp.fitMean(0.9)).on(T3).boundTo(E3).repliesTo(E2);

        return model;
    }

    public static LayeredNetwork bug2network1() {
        LayeredNetwork model = new LayeredNetwork("Depth 2 (Passing)");

        Processor P1 = new Processor(model, "P1", 2, SchedStrategy.PS);
        Processor P2 = new Processor(model, "P2", 3, SchedStrategy.PS);


        Task T1 = new Task(model, "T1", 2, SchedStrategy.REF).on(P1).setThinkTime(new Exp(1.0/4));
        Task T2 = new Task(model, "T2", 2, SchedStrategy.FCFS).on(P2);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);

        new Activity(model, "AS1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);
        new Activity(model, "AS2", Exp.fitMean(0.9)).on(T2).boundTo(E2).repliesTo(E1);

        return model;
    }

    public static LayeredNetwork bug2network2() {
        LayeredNetwork model = new LayeredNetwork("Depth 3 (Passing)");

        Processor P1 = new Processor(model, "P1", 2, SchedStrategy.PS);
        Processor P2 = new Processor(model, "P2", 3, SchedStrategy.PS);
        Processor P3 = new Processor(model, "P3", 7, SchedStrategy.PS);


        Task T1 = new Task(model, "T1", 2, SchedStrategy.REF).on(P1).setThinkTime(new Exp(1.0/4));
        Task T2 = new Task(model, "T2", 2, SchedStrategy.FCFS).on(P2);
        Task T3 = new Task(model, "T3", 2, SchedStrategy.FCFS).on(P3);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        Entry E3 = new Entry(model, "E3").on(T3);

        new Activity(model, "AS1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);
        new Activity(model, "AS2", Immediate.getInstance()).on(T2).boundTo(E2).synchCall(E3, 1).repliesTo(E1);
        new Activity(model, "AS3", Exp.fitMean(0.9)).on(T3).boundTo(E3).repliesTo(E2);

        return model;
    }

    public static LayeredNetwork bug2network3() {
        LayeredNetwork model = new LayeredNetwork("Depth 4 (Failing)");

        Processor P1 = new Processor(model, "P1", 2, SchedStrategy.PS);
        Processor P2 = new Processor(model, "P2", 3, SchedStrategy.PS);
        Processor P3 = new Processor(model, "P3", 7, SchedStrategy.PS);
        Processor P4 = new Processor(model, "P4", 1, SchedStrategy.PS);


        Task T1 = new Task(model, "T1", 2, SchedStrategy.REF).on(P1).setThinkTime(new Exp(1.0/4));
        Task T2 = new Task(model, "T2", 2, SchedStrategy.FCFS).on(P2);
        Task T3 = new Task(model, "T3", 2, SchedStrategy.FCFS).on(P3);
        Task T4 = new Task(model, "T4", 2, SchedStrategy.FCFS).on(P4);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        Entry E3 = new Entry(model, "E3").on(T3);
        Entry E4 = new Entry(model, "E4").on(T4);

        new Activity(model, "AS1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);
        new Activity(model, "AS2", Immediate.getInstance()).on(T2).boundTo(E2).synchCall(E3, 1).repliesTo(E1);
        new Activity(model, "AS3", Immediate.getInstance()).on(T3).boundTo(E3).synchCall(E4, 1).repliesTo(E2);
        new Activity(model, "AS4", Exp.fitMean(0.9)).on(T4).boundTo(E4).repliesTo(E3);

        return model;
    }

    public static LayeredNetwork bug2network4() {
        LayeredNetwork model = new LayeredNetwork("Depth 5 (Failing)");

        Processor P1 = new Processor(model, "P1", 2, SchedStrategy.PS);
        Processor P2 = new Processor(model, "P2", 3, SchedStrategy.PS);
        Processor P3 = new Processor(model, "P3", 7, SchedStrategy.PS);
        Processor P4 = new Processor(model, "P4", 1, SchedStrategy.PS);
        Processor P5 = new Processor(model, "P5", 4, SchedStrategy.PS);

        Task T1 = new Task(model, "T1", 2, SchedStrategy.REF).on(P1).setThinkTime(new Exp(1.0/4));
        Task T2 = new Task(model, "T2", 2, SchedStrategy.FCFS).on(P2);
        Task T3 = new Task(model, "T3", 2, SchedStrategy.FCFS).on(P3);
        Task T4 = new Task(model, "T4", 2, SchedStrategy.FCFS).on(P4);
        Task T5 = new Task(model, "T5", 2, SchedStrategy.FCFS).on(P5);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        Entry E3 = new Entry(model, "E3").on(T3);
        Entry E4 = new Entry(model, "E4").on(T4);
        Entry E5 = new Entry(model, "E5").on(T5);

        new Activity(model, "AS1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);
        new Activity(model, "AS2", Immediate.getInstance()).on(T2).boundTo(E2).synchCall(E3, 1).repliesTo(E1);
        new Activity(model, "AS3", Immediate.getInstance()).on(T3).boundTo(E3).synchCall(E4, 1).repliesTo(E2);
        new Activity(model, "AS4", Immediate.getInstance()).on(T4).boundTo(E4).synchCall(E5, 1).repliesTo(E3);
        new Activity(model, "AS5", Exp.fitMean(0.9)).on(T5).boundTo(E5).repliesTo(E4);

        return model;
    }

    public static LayeredNetwork bug3network1() {
        LayeredNetwork model = new LayeredNetwork("Intermediate Think Time Equal to Below (Passing)");

        Processor P1 = new Processor(model, "P1", 2, SchedStrategy.PS);
        Processor P2 = new Processor(model, "P2", 3, SchedStrategy.PS);
        Processor P3 = new Processor(model, "P3", 7, SchedStrategy.PS);


        Task T1 = new Task(model, "T1", 2, SchedStrategy.REF).on(P1).setThinkTime(new Exp(1.0/4));
        Task T2 = new Task(model, "T2", 2, SchedStrategy.FCFS).on(P2);
        Task T3 = new Task(model, "T3", 2, SchedStrategy.FCFS).on(P3);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        Entry E3 = new Entry(model, "E3").on(T3);

        new Activity(model, "AS1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);
        new Activity(model, "AS2", Immediate.getInstance()).on(T2).boundTo(E2).synchCall(E3, 1).repliesTo(E1);
        new Activity(model, "AS3", Exp.fitMean(0.9)).on(T3).boundTo(E3).repliesTo(E2);

        return model;
    }

    public static LayeredNetwork bug3network2() {
        LayeredNetwork model = new LayeredNetwork("Intermediate Think Time Equal to Above and Below (Passing)");

        Processor P1 = new Processor(model, "P1", 2, SchedStrategy.PS);
        Processor P2 = new Processor(model, "P2", 3, SchedStrategy.PS);
        Processor P3 = new Processor(model, "P3", 7, SchedStrategy.PS);


        Task T1 = new Task(model, "T1", 2, SchedStrategy.REF).on(P1).setThinkTime(new Exp(1.0/4));
        Task T2 = new Task(model, "T2", 2, SchedStrategy.FCFS).on(P2).setThinkTime(new Exp(1.0/4));
        Task T3 = new Task(model, "T3", 2, SchedStrategy.FCFS).on(P3).setThinkTime(new Exp(1.0/4));

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        Entry E3 = new Entry(model, "E3").on(T3);

        new Activity(model, "AS1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);
        new Activity(model, "AS2", Immediate.getInstance()).on(T2).boundTo(E2).synchCall(E3, 1).repliesTo(E1);
        new Activity(model, "AS3", Exp.fitMean(0.9)).on(T3).boundTo(E3).repliesTo(E2);

        return model;
    }
    
    public static LayeredNetwork bug3network3() {
        LayeredNetwork model = new LayeredNetwork("Intermediate Think Time Larger than Above or Below 1 (Failing)");

        Processor P1 = new Processor(model, "P1", 2, SchedStrategy.PS);
        Processor P2 = new Processor(model, "P2", 3, SchedStrategy.PS);
        Processor P3 = new Processor(model, "P3", 7, SchedStrategy.PS);


        Task T1 = new Task(model, "T1", 2, SchedStrategy.REF).on(P1);
        Task T2 = new Task(model, "T2", 2, SchedStrategy.FCFS).on(P2).setThinkTime(new Exp(1.0/4));
        Task T3 = new Task(model, "T3", 2, SchedStrategy.FCFS).on(P3);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        Entry E3 = new Entry(model, "E3").on(T3);

        new Activity(model, "AS1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);
        new Activity(model, "AS2", Immediate.getInstance()).on(T2).boundTo(E2).synchCall(E3, 1).repliesTo(E1);
        new Activity(model, "AS3", Exp.fitMean(0.9)).on(T3).boundTo(E3).repliesTo(E2);

        return model;
    }

        public static LayeredNetwork bug3network4() {
        LayeredNetwork model = new LayeredNetwork("Intermediate Think Time Smaller than Above and Below 2 (Failing)");

        Processor P1 = new Processor(model, "P1", 2, SchedStrategy.PS);
        Processor P2 = new Processor(model, "P2", 3, SchedStrategy.PS);
        Processor P3 = new Processor(model, "P3", 7, SchedStrategy.PS);


        Task T1 = new Task(model, "T1", 2, SchedStrategy.REF).on(P1).setThinkTime(new Exp(1.0/4));
        Task T2 = new Task(model, "T2", 2, SchedStrategy.FCFS).on(P2).setThinkTime(new Exp(1.0/8));
        Task T3 = new Task(model, "T3", 2, SchedStrategy.FCFS).on(P3).setThinkTime(new Exp(1.0/4));

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        Entry E3 = new Entry(model, "E3").on(T3);

        new Activity(model, "AS1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);
        new Activity(model, "AS2", Immediate.getInstance()).on(T2).boundTo(E2).synchCall(E3, 1).repliesTo(E1);
        new Activity(model, "AS3", Exp.fitMean(0.9)).on(T3).boundTo(E3).repliesTo(E2);

        return model;
    }

    public static LayeredNetwork bug3network5() {
        LayeredNetwork model = new LayeredNetwork("Intermediate Think Time Larger than Above and Below 2 (Passing) - Strange Case");

        Processor P1 = new Processor(model, "P1", 2, SchedStrategy.PS);
        Processor P2 = new Processor(model, "P2", 3, SchedStrategy.PS);
        Processor P3 = new Processor(model, "P3", 7, SchedStrategy.PS);


        Task T1 = new Task(model, "T1", 2, SchedStrategy.REF).on(P1).setThinkTime(new Exp(1.0/4));
        Task T2 = new Task(model, "T2", 2, SchedStrategy.FCFS).on(P2).setThinkTime(new Exp(1.0/2));
        Task T3 = new Task(model, "T3", 2, SchedStrategy.FCFS).on(P3).setThinkTime(new Exp(1.0/4));

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        Entry E3 = new Entry(model, "E3").on(T3);

        new Activity(model, "AS1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);
        new Activity(model, "AS2", Immediate.getInstance()).on(T2).boundTo(E2).synchCall(E3, 1).repliesTo(E1);
        new Activity(model, "AS3", Exp.fitMean(0.9)).on(T3).boundTo(E3).repliesTo(E2);

        return model;
    }

    public static LayeredNetwork bug4network1() {
        LayeredNetwork model = new LayeredNetwork("Only Last Service Demand Set (Passing)");

        Processor P1 = new Processor(model, "P1", 2, SchedStrategy.PS);
        Processor P2 = new Processor(model, "P2", 3, SchedStrategy.PS);
        Processor P3 = new Processor(model, "P3", 7, SchedStrategy.PS);


        Task T1 = new Task(model, "T1", 2, SchedStrategy.REF).on(P1).setThinkTime(new Exp(1.0/4));
        Task T2 = new Task(model, "T2", 2, SchedStrategy.FCFS).on(P2);
        Task T3 = new Task(model, "T3", 2, SchedStrategy.FCFS).on(P3);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        Entry E3 = new Entry(model, "E3").on(T3);

        new Activity(model, "AS1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);
        new Activity(model, "AS2", Immediate.getInstance()).on(T2).boundTo(E2).synchCall(E3, 1).repliesTo(E1);
        new Activity(model, "AS3", Exp.fitMean(0.9)).on(T3).boundTo(E3).repliesTo(E2);

        return model;
    }

    public static LayeredNetwork bug4network2() {
        LayeredNetwork model = new LayeredNetwork("First and Last Service Demand Set (Failing)");

        Processor P1 = new Processor(model, "P1", 2, SchedStrategy.PS);
        Processor P2 = new Processor(model, "P2", 3, SchedStrategy.PS);
        Processor P3 = new Processor(model, "P3", 7, SchedStrategy.PS);


        Task T1 = new Task(model, "T1", 2, SchedStrategy.REF).on(P1).setThinkTime(new Exp(1.0/4));
        Task T2 = new Task(model, "T2", 2, SchedStrategy.FCFS).on(P2);
        Task T3 = new Task(model, "T3", 2, SchedStrategy.FCFS).on(P3);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        Entry E3 = new Entry(model, "E3").on(T3);

        new Activity(model, "AS1", Exp.fitMean(0.7)).on(T1).boundTo(E1).synchCall(E2, 1);
        new Activity(model, "AS2", Immediate.getInstance()).on(T2).boundTo(E2).synchCall(E3, 1).repliesTo(E1);
        new Activity(model, "AS3", Exp.fitMean(0.9)).on(T3).boundTo(E3).repliesTo(E2);

        return model;
    }

    public static LayeredNetwork bug4network3() {
        LayeredNetwork model = new LayeredNetwork("Second Service Demand Set (Failing)");

        Processor P1 = new Processor(model, "P1", 2, SchedStrategy.PS);
        Processor P2 = new Processor(model, "P2", 3, SchedStrategy.PS);
        Processor P3 = new Processor(model, "P3", 7, SchedStrategy.PS);

        Task T1 = new Task(model, "T1", 2, SchedStrategy.REF).on(P1).setThinkTime(new Exp(1.0/4));
        Task T2 = new Task(model, "T2", 2, SchedStrategy.FCFS).on(P2);
        Task T3 = new Task(model, "T3", 2, SchedStrategy.FCFS).on(P3);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        Entry E3 = new Entry(model, "E3").on(T3);

        new Activity(model, "AS1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(E2, 1);
        new Activity(model, "AS2", Exp.fitMean(0.9)).on(T2).boundTo(E2).synchCall(E3, 1).repliesTo(E1);
        new Activity(model, "AS3", Immediate.getInstance()).on(T3).boundTo(E3).repliesTo(E2);

        return model;
    }

    public static LayeredNetwork multiCallNetwork() {
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
        return model;
    }

    public static LayeredNetwork sc1_simple_chain() throws Exception {
        LayeredNetwork model = new LayeredNetwork("sc1");

        Processor P1 = new Processor(model, "P1", 1, SchedStrategy.PS);
        Processor P2 = new Processor(model, "P2", 1, SchedStrategy.PS);

        Task T1 = new Task(model, "T1", 50, SchedStrategy.REF).on(P1).setThinkTime(new Exp(1.0));
        Task T2 = new Task(model, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(P2);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);

        new Activity(model, "A1", new Exp(5)).on(T1).boundTo(E1).synchCall(E2, 1);
        new Activity(model, "A2", new Exp(10)).on(T2).boundTo(E2).repliesTo(E2);

        return model;
    }

    public static LayeredNetwork sc2_three_tier() throws Exception {
        LayeredNetwork model = new LayeredNetwork("sc2");

        Processor P1 = new Processor(model, "P1", 2, SchedStrategy.PS);
        Processor P2 = new Processor(model, "P2", 2, SchedStrategy.PS);
        Processor P3 = new Processor(model, "P3", 2, SchedStrategy.PS);

        Task T1 = new Task(model, "T1", 100, SchedStrategy.REF).on(P1).setThinkTime(new Exp(2));
        Task T2 = new Task(model, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(P2);
        Task T3 = new Task(model, "T3", Integer.MAX_VALUE, SchedStrategy.INF).on(P3);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        Entry E3 = new Entry(model, "E3").on(T3);

        new Activity(model, "A1", new Exp(3)).on(T1).boundTo(E1).synchCall(E2, 1);
        new Activity(model, "A2", new Exp(4)).on(T2).boundTo(E2).synchCall(E3, 2).repliesTo(E2);
        new Activity(model, "A3", new Exp(6)).on(T3).boundTo(E3).repliesTo(E3);

        return model;
    }

    public static LayeredNetwork sc3_fanout() throws Exception {
        LayeredNetwork model = new LayeredNetwork("sc3");

        Processor P1 = new Processor(model, "P1", 1, SchedStrategy.PS);
        Processor P2 = new Processor(model, "P2", 1, SchedStrategy.PS);

        Task T1 = new Task(model, "T1", 40, SchedStrategy.REF).on(P1).setThinkTime(new Exp(1));
        Task T2 = new Task(model, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(P2);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        Entry E3 = new Entry(model, "E3").on(T2);

        new Activity(model, "A1", new Exp(2)).on(T1).boundTo(E1).synchCall(E2, 1).synchCall(E3, 1);

        new Activity(model, "A2", new Exp(5)).on(T2).boundTo(E2).repliesTo(E2);
        new Activity(model, "A3", new Exp(7)).on(T2).boundTo(E3).repliesTo(E3);

        return model;
    }

    public static LayeredNetwork sc4_serial() throws Exception {
        LayeredNetwork model = new LayeredNetwork("sc4");

        Processor P1 = new Processor(model, "P1", 1, SchedStrategy.PS);
        Processor P2 = new Processor(model, "P2", 1, SchedStrategy.PS);

        Task T1 = new Task(model, "T1", 30, SchedStrategy.REF).on(P1).setThinkTime(new Exp(2));
        Task T2 = new Task(model, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(P2);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);

        Activity A1 = new Activity(model, "A1", new Exp(1)).on(T1).boundTo(E1);
        Activity A2 = new Activity(model, "A2", new Exp(1)).on(T1).synchCall(E2, 1);

        new Activity(model, "A3", new Exp(3)).on(T2).boundTo(E2).repliesTo(E2);

        T1.addPrecedence(ActivityPrecedence.Serial("A1", "A2"));

        return model;
    }

    public static LayeredNetwork sc5_loop() throws Exception {
        LayeredNetwork model = new LayeredNetwork("sc5");

        Processor P1 = new Processor(model, "P1", 1, SchedStrategy.PS);

        Task T1 = new Task(model, "T1", 20, SchedStrategy.REF).on(P1).setThinkTime(new Exp(1));

        Entry E1 = new Entry(model, "E1").on(T1);

        Activity A1 = new Activity(model, "A1", new Exp(2)).on(T1).boundTo(E1);
        Activity A2 = new Activity(model, "A2", new Exp(3)).on(T1);

        T1.addPrecedence(ActivityPrecedence.Loop("A1", Arrays.asList("A2"), Matrix.singleton(3)));

        return model;
    }

    public static LayeredNetwork sc6_multi_call() throws Exception {
        LayeredNetwork model = new LayeredNetwork("sc6");

        Processor P1 = new Processor(model, "P1", 2, SchedStrategy.PS);
        Processor P2 = new Processor(model, "P2", 2, SchedStrategy.PS);

        Task T1 = new Task(model, "T1", 60, SchedStrategy.REF).on(P1).setThinkTime(new Exp(2));
        Task T2 = new Task(model, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(P2);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);

        new Activity(model, "A1", new Exp(2)).on(T1).boundTo(E1).synchCall(E2, 5);
        new Activity(model, "A2", new Exp(8)).on(T2).boundTo(E2).repliesTo(E2);

        return model;
    }

    public static LayeredNetwork sc7_multicore() throws Exception {
        LayeredNetwork model = new LayeredNetwork("sc7");

        Processor P1 = new Processor(model, "P1", 4, SchedStrategy.PS);
        Processor P2 = new Processor(model, "P2", 8, SchedStrategy.PS);

        Task T1 = new Task(model, "T1", 200, SchedStrategy.REF).on(P1).setThinkTime(new Exp(5));
        Task T2 = new Task(model, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(P2);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);

        new Activity(model, "A1", new Exp(3)).on(T1).boundTo(E1).synchCall(E2, 1);
        new Activity(model, "A2", new Exp(6)).on(T2).boundTo(E2).repliesTo(E2);

        return model;
    }

    public static LayeredNetwork sc8_immediate() throws Exception {
        LayeredNetwork model = new LayeredNetwork("sc8");

        Processor P1 = new Processor(model, "P1", 1, SchedStrategy.PS);
        Processor P2 = new Processor(model, "P2", Integer.MAX_VALUE, SchedStrategy.INF);

        Task T1 = new Task(model, "T1", 10, SchedStrategy.REF).on(P1).setThinkTime(new Exp(1));
        Task T2 = new Task(model, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(P2);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);

        new Activity(model, "A1", new Exp(1)).on(T1).boundTo(E1).synchCall(E2, 1);
        new Activity(model, "A2", Immediate.getInstance()).on(T2).boundTo(E2).repliesTo(E2);

        return model;
    }
    public static LayeredNetwork sc9_two_calls() throws Exception {
        LayeredNetwork model = new LayeredNetwork("sc9");

        Processor P1 = new Processor(model, "P1", 1, SchedStrategy.PS);
        Processor P2 = new Processor(model, "P2", 1, SchedStrategy.PS);

        Task T1 = new Task(model, "T1", 25, SchedStrategy.REF).on(P1).setThinkTime(new Exp(2));
        Task T2 = new Task(model, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(P2);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);

        Activity A1 = new Activity(model, "A1", new Exp(1)).on(T1).boundTo(E1).synchCall(E2, 1);
        Activity A2 = new Activity(model, "A2", new Exp(1)).on(T1).synchCall(E2, 1);

        new Activity(model, "A3", new Exp(4)).on(T2).boundTo(E2).repliesTo(E2);

        T1.addPrecedence(ActivityPrecedence.Serial("A1", "A2"));

        return model;
    }

    public static LayeredNetwork sc10_deep_chain() throws Exception {
        LayeredNetwork model = new LayeredNetwork("sc10");

        Processor P1 = new Processor(model, "P1", 1, SchedStrategy.PS);
        Processor P2 = new Processor(model, "P2", 1, SchedStrategy.PS);
        Processor P3 = new Processor(model, "P3", 1, SchedStrategy.PS);
        Processor P4 = new Processor(model, "P4", 1, SchedStrategy.PS);

        Task T1 = new Task(model, "T1", 80, SchedStrategy.REF).on(P1).setThinkTime(new Exp(3));
        Task T2 = new Task(model, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(P2);
        Task T3 = new Task(model, "T3", Integer.MAX_VALUE, SchedStrategy.INF).on(P3);
        Task T4 = new Task(model, "T4", Integer.MAX_VALUE, SchedStrategy.INF).on(P4);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        Entry E3 = new Entry(model, "E3").on(T3);
        Entry E4 = new Entry(model, "E4").on(T4);

        new Activity(model, "A1", new Exp(2)).on(T1).boundTo(E1).synchCall(E2, 1);
        new Activity(model, "A2", new Exp(3)).on(T2).boundTo(E2).synchCall(E3, 1).repliesTo(E2);
        new Activity(model, "A3", new Exp(4)).on(T3).boundTo(E3).synchCall(E4, 1).repliesTo(E3);
        new Activity(model, "A4", new Exp(5)).on(T4).boundTo(E4).repliesTo(E4);

        return model;
    }





    

}