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
    // P1        	Processor 	NaN       	0.99594   	NaN       	NaN       	NaN       	NaN       
    // P2        	Processor 	NaN       	0.04146   	NaN       	NaN       	NaN       	NaN       
    // T1        	RefTask   	23.43875  	0.66397   	NaN       	1.09001   	NaN       	13.27931  
    // T2        	Task      	8.70307   	0.33197   	NaN       	0.55513   	NaN       	13.27894  
    // T3        	Task      	0.12438   	0.04146   	NaN       	0.02000   	NaN       	6.21891   
    // E1        	Entry     	23.43875  	NaN       	1.76506   	NaN       	NaN       	13.27931  
    // E2        	Entry     	8.70307   	NaN       	0.65540   	NaN       	NaN       	13.27894  
    // E3        	Entry     	0.12438   	NaN       	0.02000   	NaN       	NaN       	6.21891   
    // AS1       	Activity  	23.44143  	0.66397   	1.76526   	1.09001   	NaN       	13.27931  
    // AS2       	Activity  	8.70441   	0.33197   	0.65550   	0.55513   	NaN       	13.27894  
    // AS3       	Activity  	0.12438   	0.04146   	0.02000   	0.02000   	NaN       	6.21891   
    // --------------------------------------------------------------------------------------------
    public static LayeredNetwork lqnBasic() {
        try {
            return LayeredModel.lqn_basic();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }













    //-----------------------------CHATGPT SINGLE-CLASS MODELS-------------------------------------

    // Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput      
    // --------------------------------------------------------------------------------------------
    // P1        	Processor 	NaN       	1.00008   	NaN       	NaN       	NaN       	NaN       
    // P2        	Processor 	NaN       	0.50004   	NaN       	NaN       	NaN       	NaN       
    // T1        	RefTask   	44.99961  	1.00008   	NaN       	8.80651   	NaN       	5.00039   
    // T2        	Task      	0.96363   	0.50004   	NaN       	0.19271   	NaN       	5.00039   
    // E1        	Entry     	44.99961  	NaN       	8.99922   	NaN       	NaN       	5.00039   
    // E2        	Entry     	0.96363   	NaN       	0.19271   	NaN       	NaN       	5.00039   
    // A1        	Activity  	44.99961  	1.00008   	8.99922   	8.80651   	NaN       	5.00039   
    // A2        	Activity  	0.96363   	0.50004   	0.19271   	0.19271   	NaN       	5.00039   
    // --------------------------------------------------------------------------------------------
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

    // Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput      
    // --------------------------------------------------------------------------------------------
    // P1        	Processor 	NaN       	0.99948   	NaN       	NaN       	NaN       	NaN       
    // P2        	Processor 	NaN       	0.74967   	NaN       	NaN       	NaN       	NaN       
    // P3        	Processor 	NaN       	0.99987   	NaN       	NaN       	NaN       	NaN       
    // T1        	RefTask   	96.99009  	0.99948   	NaN       	6.45740   	NaN       	5.99686   
    // T2        	Task      	58.27254  	0.74967   	NaN       	0.45281   	NaN       	5.99733   
    // T3        	Task      	55.57524  	0.99987   	NaN       	4.63188   	NaN       	11.99843  
    // E1        	Entry     	96.99009  	NaN       	16.17347  	NaN       	NaN       	5.99686   
    // E2        	Entry     	58.27254  	NaN       	9.71641   	NaN       	NaN       	5.99733   
    // E3        	Entry     	55.57524  	NaN       	4.63188   	NaN       	NaN       	11.99843  
    // A1        	Activity  	96.99912  	0.99948   	16.17498  	6.45740   	NaN       	5.99686   
    // A2        	Activity  	58.29184  	0.74967   	9.71963   	0.45281   	NaN       	5.99733   
    // A3        	Activity  	55.58231  	0.99987   	4.63247   	4.63188   	NaN       	11.99843  
    // --------------------------------------------------------------------------------------------
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

    // Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput      
    // --------------------------------------------------------------------------------------------
    // P1        	Processor 	NaN       	1.00010   	NaN       	NaN       	NaN       	NaN       
    // P2        	Processor 	NaN       	0.68580   	NaN       	NaN       	NaN       	NaN       
    // T1        	RefTask   	37.99973  	1.00010   	NaN       	18.04080  	NaN       	2.00021   
    // T2        	Task      	1.91439   	0.68580   	NaN       	0.47854   	NaN       	4.00050   
    // E1        	Entry     	37.99973  	NaN       	18.99788  	NaN       	NaN       	2.00021   
    // E2        	Entry     	1.11673   	NaN       	0.55829   	NaN       	NaN       	2.00025   
    // E3        	Entry     	0.79766   	NaN       	0.39878   	NaN       	NaN       	2.00025   
    // A1        	Activity  	37.99967  	1.00010   	18.99785  	18.04080  	NaN       	2.00021   
    // A2        	Activity  	1.11675   	0.40005   	0.55831   	0.27915   	NaN       	2.00025   
    // A3        	Activity  	0.79768   	0.28575   	0.39879   	0.19939   	NaN       	2.00025   
    // --------------------------------------------------------------------------------------------
    public static LayeredNetwork sc3_fanout() throws Exception {
        LayeredNetwork model = new LayeredNetwork("sc3");

        Processor P1 = new Processor(model, "P1", 1, SchedStrategy.PS);
        Processor P2 = new Processor(model, "P2", 1, SchedStrategy.PS);

        Task T1 = new Task(model, "T1", 40, SchedStrategy.REF).on(P1).setThinkTime(new Exp(1));
        Task T2 = new Task(model, "T2", Integer.MAX_VALUE, SchedStrategy.INF).on(P2);

        Entry E1 = new Entry(model, "E1").on(T1);
        Entry E2 = new Entry(model, "E2").on(T2);
        Entry E3 = new Entry(model, "E3").on(T2);

        new Activity(model, "A1", new Exp(2)).on(T1).boundTo(E1)
            .synchCall(E2, 1).synchCall(E3, 1);

        new Activity(model, "A2", new Exp(5)).on(T2).boundTo(E2).repliesTo(E2);
        new Activity(model, "A3", new Exp(7)).on(T2).boundTo(E3).repliesTo(E3);

        return model;
    }

    // Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput      
    // --------------------------------------------------------------------------------------------
    // P1        	Processor 	NaN       	1.00003   	NaN       	NaN       	NaN       	NaN       
    // P2        	Processor 	NaN       	0.16667   	NaN       	NaN       	NaN       	NaN       
    // T1        	RefTask   	29.74999  	1.00003   	NaN       	59.10121  	NaN       	0.50002   
    // T2        	Task      	0.19842   	0.16667   	NaN       	0.39683   	NaN       	0.50002   
    // E1        	Entry     	29.74999  	NaN       	59.49804  	NaN       	NaN       	0.50002   
    // E2        	Entry     	0.19842   	NaN       	0.39683   	NaN       	NaN       	0.50002   
    // A1        	Activity  	14.77578  	0.50002   	29.55060  	29.55060  	NaN       	0.50002   
    // A2        	Activity  	14.97421  	0.50002   	29.94744  	29.55060  	NaN       	0.50002   
    // A3        	Activity  	0.19842   	0.16667   	0.39683   	0.39683   	NaN       	0.50002   
    // --------------------------------------------------------------------------------------------
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

    // Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput      
    // --------------------------------------------------------------------------------------------
    // P1        	Processor 	NaN       	1.00000   	NaN       	NaN       	NaN       	NaN       
    // T1        	RefTask   	15.91736  	1.00000   	NaN       	35.66667  	NaN       	0.54545   
    // E1        	Entry     	15.91736  	NaN       	29.18182  	NaN       	NaN       	0.54545   
    // A1        	Activity  	15.91736  	0.81818   	9.72727   	29.18182  	NaN       	1.63636   
    // A2        	Activity  	3.53719   	0.18182   	6.48485   	6.48485   	NaN       	0.54545   
    // --------------------------------------------------------------------------------------------
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

    // Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput      
    // --------------------------------------------------------------------------------------------
    // P1        	Processor 	NaN       	0.80011   	NaN       	NaN       	NaN       	NaN       
    // P2        	Processor 	NaN       	1.00000   	NaN       	NaN       	NaN       	NaN       
    // T1        	RefTask   	58.40678  	0.80011   	NaN       	1.00557   	NaN       	3.20042   
    // T2        	Task      	55.18124  	1.00000   	NaN       	3.44884   	NaN       	15.99993  
    // E1        	Entry     	58.40678  	NaN       	18.24971  	NaN       	NaN       	3.20042   
    // E2        	Entry     	55.18124  	NaN       	3.44884   	NaN       	NaN       	15.99993  
    // A1        	Activity  	58.40357  	0.80011   	18.24871  	1.00557   	NaN       	3.20042   
    // A2        	Activity  	55.17942  	1.00000   	3.44873   	3.44884   	NaN       	15.99993  
    // --------------------------------------------------------------------------------------------
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

    // Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput      
    // --------------------------------------------------------------------------------------------
    // P1        	Processor 	NaN       	1.00000   	NaN       	NaN       	NaN       	NaN       
    // P2        	Processor 	NaN       	0.25000   	NaN       	NaN       	NaN       	NaN       
    // T1        	RefTask   	197.60000 	1.00000   	NaN       	16.30000  	NaN       	12.00000  
    // T2        	Task      	2.00000   	0.25000   	NaN       	0.16667   	NaN       	12.00000  
    // E1        	Entry     	197.60000 	NaN       	16.46667  	NaN       	NaN       	12.00000  
    // E2        	Entry     	2.00000   	NaN       	0.16667   	NaN       	NaN       	12.00000  
    // A1        	Activity  	197.60001 	1.00000   	16.46667  	16.30000  	NaN       	12.00000  
    // A2        	Activity  	2.00000   	0.25000   	0.16667   	0.16667   	NaN       	12.00000  
    // --------------------------------------------------------------------------------------------
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

    // Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput      
    // --------------------------------------------------------------------------------------------
    // P1        	Processor 	NaN       	1.00000   	NaN       	NaN       	NaN       	NaN       
    // P2        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN       
    // T1        	RefTask   	9.00000   	1.00000   	NaN       	9.00000   	NaN       	1.00000   
    // T2        	Task      	0         	0         	NaN       	0         	NaN       	1.00000   
    // E1        	Entry     	9.00000   	NaN       	9.00000   	NaN       	NaN       	1.00000   
    // E2        	Entry     	0         	NaN       	0         	NaN       	NaN       	1.00000   
    // A1        	Activity  	9.00000   	1.00000   	9.00000   	9.00000   	NaN       	1.00000   
    // A2        	Activity  	0         	0         	0         	0         	NaN       	1.00000   
    // --------------------------------------------------------------------------------------------
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

    // Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput      
    // --------------------------------------------------------------------------------------------
    // P1        	Processor 	NaN       	1.00007   	NaN       	NaN       	NaN       	NaN       
    // P2        	Processor 	NaN       	0.25002   	NaN       	NaN       	NaN       	NaN       
    // T1        	RefTask   	24.74998  	1.00007   	NaN       	48.84124  	NaN       	0.50004   
    // T2        	Task      	0.32761   	0.25002   	NaN       	0.32758   	NaN       	1.00007   
    // E1        	Entry     	24.74998  	NaN       	49.49641  	NaN       	NaN       	0.50004   
    // E2        	Entry     	0.32761   	NaN       	0.32758   	NaN       	NaN       	1.00007   
    // A1        	Activity  	12.37499  	0.50004   	24.74820  	24.42062  	NaN       	0.50004   
    // A2        	Activity  	12.37499  	0.50004   	24.74820  	24.42062  	NaN       	0.50004   
    // A3        	Activity  	0.32761   	0.25002   	0.32758   	0.32758   	NaN       	1.00007   
    // --------------------------------------------------------------------------------------------
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

    // Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput      
    // --------------------------------------------------------------------------------------------
    // P1        	Processor 	NaN       	1.00002   	NaN       	NaN       	NaN       	NaN       
    // P2        	Processor 	NaN       	0.66672   	NaN       	NaN       	NaN       	NaN       
    // P3        	Processor 	NaN       	0.50007   	NaN       	NaN       	NaN       	NaN       
    // P4        	Processor 	NaN       	0.40009   	NaN       	NaN       	NaN       	NaN       
    // T1        	RefTask   	79.33290  	1.00002   	NaN       	37.91178  	NaN       	2.00003   
    // T2        	Task      	3.50834   	0.66672   	NaN       	0.93693   	NaN       	2.00015   
    // T3        	Task      	1.63442   	0.50007   	NaN       	0.48824   	NaN       	2.00026   
    // T4        	Task      	0.65788   	0.40009   	NaN       	0.32886   	NaN       	2.00047   
    // E1        	Entry     	79.33290  	NaN       	39.66584  	NaN       	NaN       	2.00003   
    // E2        	Entry     	3.50834   	NaN       	1.75404   	NaN       	NaN       	2.00015   
    // E3        	Entry     	1.63442   	NaN       	0.81710   	NaN       	NaN       	2.00026   
    // E4        	Entry     	0.65788   	NaN       	0.32886   	NaN       	NaN       	2.00047   
    // A1        	Activity  	79.33257  	1.00002   	39.66568  	37.91178  	NaN       	2.00003   
    // A2        	Activity  	3.50877   	0.66672   	1.75426   	0.93693   	NaN       	2.00015   
    // A3        	Activity  	1.63461   	0.50007   	0.81720   	0.48824   	NaN       	2.00026   
    // A4        	Activity  	0.65793   	0.40009   	0.32889   	0.32886   	NaN       	2.00047   
    // --------------------------------------------------------------------------------------------
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
