package jline.solvers.ln_simple.utils;

import java.util.Arrays;

import jline.examples.java.basic.LayeredModel;
import jline.lang.constant.SchedStrategy;
import jline.lang.layered.*;
import jline.lang.processes.Exp;
import jline.lang.processes.Immediate;
import jline.util.matrix.Matrix;

public class LayeredNetworkTestExamples {

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








    

}