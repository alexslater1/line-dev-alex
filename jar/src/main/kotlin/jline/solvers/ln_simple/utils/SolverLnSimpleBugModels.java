package jline.solvers.ln_simple.utils;

import jline.lang.constant.SchedStrategy;
import jline.lang.layered.Activity;
import jline.lang.layered.Entry;
import jline.lang.layered.LayeredNetwork;
import jline.lang.layered.Processor;
import jline.lang.layered.Task;
import jline.lang.processes.Exp;
import jline.lang.processes.Immediate;

public class SolverLnSimpleBugModels {

    //BUG 1: correct if multiplicity of tasks increases (nt1=2, nt2=2, nt3=3) or stays the same (nt1=2, nt2=2, nt3=2) throughout flow, but incorrect when decreases (nt1=3, nt2=2, nt3=2)

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

        Task T1 = new Task(model, "T1", 3, SchedStrategy.REF).on(P1).setThinkTime(new Exp(1.0/4));
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








    //BUG 2: correct if network depth = 2 or 3, incorrect if network depth > 3

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









    


    //BUG 3: correct if intermediate think equal than above and below, incorrect if differs - roughly

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














    // BUG 4: correct if only last think time set, incorrect if any others are

    public static LayeredNetwork bug4network1() {
        LayeredNetwork model = new LayeredNetwork("Only Last Think Time Set (Passing)");

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
        LayeredNetwork model = new LayeredNetwork("First and Last Think Time Set (Failing)");

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
        LayeredNetwork model = new LayeredNetwork("Second Think Time Set (Failing)");

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
}
