package jline.solvers.ln_simple;

import org.junit.jupiter.api.Disabled;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.Timeout;

import jline.solvers.ln_simple.utils.SolverLnSimpleBugModels;

import static jline.solvers.ln_simple.util.assertResultsMatchSolverLN;

public class SolverLNSimpleBugTest {


    //BUG 3: correct if intermediate think time is equal to above or below, incorrect if differs - roughly

    //PASSES
    @Test
    @Timeout(120)
    public void testBug3EqualAbove() {
        assertResultsMatchSolverLN(SolverLnSimpleBugModels.bug3network1());
    }

    //PASSES
    @Test
    @Timeout(120)
    public void testBug3EqualBoth() {
        assertResultsMatchSolverLN(SolverLnSimpleBugModels.bug3network2());
    }

    //FAILS
//=== SolverLN ===
//    Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput
//    P1        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN
//    P2        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN
//    P3        	Processor 	NaN       	0.18367   	NaN       	NaN       	NaN       	NaN
//    T1        	RefTask   	2.00000   	0         	NaN       	0         	NaN       	2.22222
//    T2        	Task      	0.36735   	0         	NaN       	0         	NaN       	0.40816
//    T3        	Task      	0.36735   	0.18367   	NaN       	0.90000   	NaN       	0.40816
//    E1        	Entry     	2.00000   	NaN       	0.90000   	NaN       	NaN       	2.22222
//    E2        	Entry     	0.36735   	NaN       	0.90000   	NaN       	NaN       	0.40816
//    E3        	Entry     	0.36735   	NaN       	0.90000   	NaN       	NaN       	0.40816
//    AS1       	Activity  	2.00000   	0         	0.90000   	0         	NaN       	2.22222
//    AS2       	Activity  	0.36735   	0         	0.90000   	0         	NaN       	0.40816
//    AS3       	Activity  	0.36735   	0.18367   	0.90000   	0.90000   	NaN       	0.40816
//
//            === SolverLNSimple ===
//    Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput
//    P1        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN
//    P2        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN
//    P3        	Processor 	NaN       	1.00000   	NaN       	NaN       	NaN       	NaN
//    T1        	RefTask   	2.00000   	0         	NaN       	0         	NaN       	2.22222
//    T2        	Task      	2.00000   	0         	NaN       	0         	NaN       	2.22222
//    T3        	Task      	2.00000   	1.00000   	NaN       	0.90000   	NaN       	2.22222
//    E1        	Entry     	2.00000   	NaN       	0.90000   	NaN       	NaN       	2.22222
//    E2        	Entry     	2.00000   	NaN       	0.90000   	NaN       	NaN       	2.22222
//    E3        	Entry     	2.00000   	NaN       	0.90000   	NaN       	NaN       	2.22222
//    AS1       	Activity  	2.00000   	0         	0.90000   	0         	NaN       	2.22222
//    AS2       	Activity  	2.00000   	0         	0.90000   	0         	NaN       	2.22222
//    AS3       	Activity  	2.00000   	1.00000   	0.90000   	0.90000   	NaN       	2.22222
    @Disabled
    @Test
    @Timeout(120)
    public void testBug3NotEqual1() {
        assertResultsMatchSolverLN(SolverLnSimpleBugModels.bug3network3());
    }

    //FAILS
//=== SolverLN ===
//    Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput
//    P1        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN
//    P2        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN
//    P3        	Processor 	NaN       	0.10112   	NaN       	NaN       	NaN       	NaN
//    T1        	RefTask   	0.36735   	0         	NaN       	0         	NaN       	0.40816
//    T2        	Task      	0.20225   	0         	NaN       	0         	NaN       	0.22472
//    T3        	Task      	0.20225   	0.10112   	NaN       	0.90000   	NaN       	0.22472
//    E1        	Entry     	0.36735   	NaN       	0.90000   	NaN       	NaN       	0.40816
//    E2        	Entry     	0.20225   	NaN       	0.90000   	NaN       	NaN       	0.22472
//    E3        	Entry     	0.20225   	NaN       	0.90000   	NaN       	NaN       	0.22472
//    AS1       	Activity  	0.36735   	0         	0.90000   	0         	NaN       	0.40816
//    AS2       	Activity  	0.20225   	0         	0.90000   	0         	NaN       	0.22472
//    AS3       	Activity  	0.20225   	0.10112   	0.90000   	0.90000   	NaN       	0.22472
//
//            === SolverLNSimple ===
//    Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput
//    P1        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN
//    P2        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN
//    P3        	Processor 	NaN       	0.18367   	NaN       	NaN       	NaN       	NaN
//    T1        	RefTask   	0.36735   	0         	NaN       	0         	NaN       	0.40816
//    T2        	Task      	0.36735   	0         	NaN       	0         	NaN       	0.40816
//    T3        	Task      	0.36735   	0.18367   	NaN       	0.90000   	NaN       	0.40816
//    E1        	Entry     	0.36735   	NaN       	0.90000   	NaN       	NaN       	0.40816
//    E2        	Entry     	0.36735   	NaN       	0.90000   	NaN       	NaN       	0.40816
//    E3        	Entry     	0.36735   	NaN       	0.90000   	NaN       	NaN       	0.40816
//    AS1       	Activity  	0.36735   	0         	0.90000   	0         	NaN       	0.40816
//    AS2       	Activity  	0.36735   	0         	0.90000   	0         	NaN       	0.40816
//    AS3       	Activity  	0.36735   	0.18367   	0.90000   	0.90000   	NaN       	0.40816
    @Disabled
    @Test
    @Timeout(120)
    public void testBug3NotEqual2() {
        assertResultsMatchSolverLN(SolverLnSimpleBugModels.bug3network4());
    }

    //PASSES
    @Test
    @Timeout(120)
    public void testBugNotEqualButPasses() {
        assertResultsMatchSolverLN(SolverLnSimpleBugModels.bug3network5());
    }




























    // BUG 4: correct if only last think time set, incorrect if any others are

    //PASSES
    @Test
    @Timeout(120)
    public void testBug4OnlyLast() {
        assertResultsMatchSolverLN(SolverLnSimpleBugModels.bug4network1());
    }

    //FAILS
//=== SolverLN ===
//    Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput
//    P1        	Processor 	NaN       	0.12500   	NaN       	NaN       	NaN       	NaN
//    P2        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN
//    P3        	Processor 	NaN       	0.16071   	NaN       	NaN       	NaN       	NaN
//    T1        	RefTask   	0.57143   	0.12500   	NaN       	0.70000   	NaN       	0.35714
//    T2        	Task      	0.32143   	0         	NaN       	0         	NaN       	0.35714
//    T3        	Task      	0.32143   	0.16071   	NaN       	0.90000   	NaN       	0.35714
//    E1        	Entry     	0.57143   	NaN       	1.60000   	NaN       	NaN       	0.35714
//    E2        	Entry     	0.32143   	NaN       	0.90000   	NaN       	NaN       	0.35714
//    E3        	Entry     	0.32143   	NaN       	0.90000   	NaN       	NaN       	0.35714
//    AS1       	Activity  	0.57143   	0.12500   	1.60000   	0.70000   	NaN       	0.35714
//    AS2       	Activity  	0.32143   	0         	0.90000   	0         	NaN       	0.35714
//    AS3       	Activity  	0.32143   	0.16071   	0.90000   	0.90000   	NaN       	0.35714
//
//            === SolverLNSimple ===
//    Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput
//    P1        	Processor 	NaN       	0.14286   	NaN       	NaN       	NaN       	NaN
//    P2        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN
//    P3        	Processor 	NaN       	0.18367   	NaN       	NaN       	NaN       	NaN
//    T1        	RefTask   	0.66869   	0.14286   	NaN       	0.70000   	NaN       	0.40816
//    T2        	Task      	0.38298   	0         	NaN       	0         	NaN       	0.42553
//    T3        	Task      	0.36735   	0.18367   	NaN       	0.90000   	NaN       	0.40816
//    E1        	Entry     	0.66869   	NaN       	1.60000   	NaN       	NaN       	0.40816
//    E2        	Entry     	0.38298   	NaN       	0.90000   	NaN       	NaN       	0.42553
//    E3        	Entry     	0.36735   	NaN       	0.90000   	NaN       	NaN       	0.40816
//    AS1       	Activity  	0.66869   	0.14286   	1.60000   	0.70000   	NaN       	0.40816
//    AS2       	Activity  	0.38298   	0         	0.90000   	0         	NaN       	0.42553
//    AS3       	Activity  	0.36735   	0.18367   	0.90000   	0.90000   	NaN       	0.40816
    @Disabled
    @Test
    @Timeout(120)
    public void testBug4FirstAndLast() {
        assertResultsMatchSolverLN(SolverLnSimpleBugModels.bug4network2());
    }

    //FAILS
//=== SolverLN ===
//    Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput
//    P1        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN
//    P2        	Processor 	NaN       	0.18367   	NaN       	NaN       	NaN       	NaN
//    P3        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN
//    T1        	RefTask   	0.36735   	0         	NaN       	0         	NaN       	0.40816
//    T2        	Task      	0.36735   	0.18367   	NaN       	0.90000   	NaN       	0.40816
//    T3        	Task      	0         	0         	NaN       	0         	NaN       	0.40816
//    E1        	Entry     	0.36735   	NaN       	0.90000   	NaN       	NaN       	0.40816
//    E2        	Entry     	0.36735   	NaN       	0.90000   	NaN       	NaN       	0.40816
//    E3        	Entry     	0         	NaN       	0         	NaN       	NaN       	0.40816
//    AS1       	Activity  	0.36735   	0         	0.90000   	0         	NaN       	0.40816
//    AS2       	Activity  	0.36735   	0.18367   	0.90000   	0.90000   	NaN       	0.40816
//    AS3       	Activity  	0         	0         	0         	0         	NaN       	0.40816
//
//            === SolverLNSimple ===
//    Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput
//    P1        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN
//    P2        	Processor 	NaN       	0.18367   	NaN       	NaN       	NaN       	NaN
//    P3        	Processor 	NaN       	NaN       	NaN       	NaN       	NaN       	NaN
//    T1        	RefTask   	0.36735   	0         	NaN       	0         	NaN       	0.40816
//    T2        	Task      	0.36735   	0.18367   	NaN       	0.90000   	NaN       	0.40816
//    T3        	Task      	NaN       	NaN       	NaN       	0         	NaN       	NaN
//    E1        	Entry     	0.36735   	NaN       	0.90000   	NaN       	NaN       	0.40816
//    E2        	Entry     	0.36735   	NaN       	0.90000   	NaN       	NaN       	0.40816
//    E3        	Entry     	NaN       	NaN       	0         	NaN       	NaN       	NaN
//    AS1       	Activity  	0.36735   	0         	0.90000   	0         	NaN       	0.40816
//    AS2       	Activity  	0.36735   	0.18367   	0.90000   	0.90000   	NaN       	0.40816
//    AS3       	Activity  	NaN       	0         	0         	0         	NaN       	NaN
    @Disabled
    @Test
    @Timeout(120)
    public void testBug4Middle() {
        assertResultsMatchSolverLN(SolverLnSimpleBugModels.bug4network3());
    }

}
