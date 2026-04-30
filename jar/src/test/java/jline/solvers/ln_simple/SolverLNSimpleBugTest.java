package jline.solvers.ln_simple;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.Timeout;

import jline.solvers.ln_simple.utils.SolverLnSimpleBugModels;

import static jline.solvers.ln_simple.util.assertResultsMatchSolverLN;

public class SolverLNSimpleBugTest {

    //BUG 1: correct if multiplicity of tasks increases (nt1=2, nt2=2, nt3=3) or stays the same (nt1=2, nt2=2, nt3=2) throughout flow, but incorrect when decreases (nt1=3, nt2=2, nt3=2)


// Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput      
// P1        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN       
// P2        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN       
// P3        	Processor 	NaN       	0.18367   	NaN       	NaN       	NaN       	NaN       
// T1        	RefTask   	0.36735   	0         	NaN       	0         	NaN       	0.40816   
// T2        	Task      	0.36735   	0         	NaN       	0         	NaN       	0.40816   
// T3        	Task      	0.36735   	0.18367   	NaN       	0.90000   	NaN       	0.40816   
// E1        	Entry     	0.36735   	NaN       	0.90000   	NaN       	NaN       	0.40816   
// E2        	Entry     	0.36735   	NaN       	0.90000   	NaN       	NaN       	0.40816   
// E3        	Entry     	0.36735   	NaN       	0.90000   	NaN       	NaN       	0.40816   
// AS1       	Activity  	0.36735   	0         	0.90000   	0         	NaN       	0.40816   
// AS2       	Activity  	0.36735   	0         	0.90000   	0         	NaN       	0.40816   
// AS3       	Activity  	0.36735   	0.18367   	0.90000   	0.90000   	NaN       	0.40816  
    @Test
    @Timeout(120)
    public void testBug1Increasing() {
        assertResultsMatchSolverLN(SolverLnSimpleBugModels.bug1network1());
    }


// Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput      
// P1        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN       
// P2        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN       
// P3        	Processor 	NaN       	0.18367   	NaN       	NaN       	NaN       	NaN       
// T1        	RefTask   	0.36735   	0         	NaN       	0         	NaN       	0.40816   
// T2        	Task      	0.36735   	0         	NaN       	0         	NaN       	0.40816   
// T3        	Task      	0.36735   	0.18367   	NaN       	0.90000   	NaN       	0.40816   
// E1        	Entry     	0.36735   	NaN       	0.90000   	NaN       	NaN       	0.40816   
// E2        	Entry     	0.36735   	NaN       	0.90000   	NaN       	NaN       	0.40816   
// E3        	Entry     	0.36735   	NaN       	0.90000   	NaN       	NaN       	0.40816   
// AS1       	Activity  	0.36735   	0         	0.90000   	0         	NaN       	0.40816   
// AS2       	Activity  	0.36735   	0         	0.90000   	0         	NaN       	0.40816   
// AS3       	Activity  	0.36735   	0.18367   	0.90000   	0.90000   	NaN       	0.40816 
    @Test
    @Timeout(120)
    public void testBug1Equal() {
        assertResultsMatchSolverLN(SolverLnSimpleBugModels.bug1network2());
    }

//     === SolverLN ===
// Node            NodeType        QLen            Util            RespT           ResidT          ArvR            Tput      
// P1              Processor       NaN             0               NaN             NaN             NaN             NaN       
// P2              Processor       NaN             0               NaN             NaN             NaN             NaN       
// P3              Processor       NaN             0.52105         NaN             NaN             NaN             NaN       
// T1              RefTask         0.68423         0               NaN             0               NaN             0.57894   
// T2              Task            0.66752         0               NaN             0               NaN             0.57894   
// T3              Task            0.52105         0.52105         NaN             0.90000         NaN             0.57894   
// E1              Entry           0.68423         NaN             1.18186         NaN             NaN             0.57894   
// E2              Entry           0.66752         NaN             1.15300         NaN             NaN             0.57894   
// E3              Entry           0.52105         NaN             0.90000         NaN             NaN             0.57894   
// AS1             Activity        0.68423         0               1.18186         0               NaN             0.57894   
// AS2             Activity        0.66752         0               1.15300         0               NaN             0.57894   
// AS3             Activity        0.52105         0.52105         0.90000         0.90000         NaN             0.57894   
// === SolverLNSimple ===
// Node            NodeType        QLen            Util            RespT           ResidT          ArvR            Tput      
// P1              Processor       NaN             0               NaN             NaN             NaN             NaN       
// P2              Processor       NaN             0               NaN             NaN             NaN             NaN       
// P3              Processor       NaN             0.18367         NaN             NaN             NaN             NaN       
// T1              RefTask         0.42063         0               NaN             0               NaN             0.59226   
// T2              Task            0.42063         0               NaN             0               NaN             0.39484   
// T3              Task            0.21741         0.18367         NaN             1.06531         NaN             0.20408   
// E1              Entry           0.42063         NaN             1.06531         NaN             NaN             0.59226   
// E2              Entry           0.42063         NaN             1.06531         NaN             NaN             0.39484   
// E3              Entry           0.21741         NaN             1.06531         NaN             NaN             0.20408   
// AS1             Activity        0.42063         0               1.06531         0               NaN             0.59226   
// AS2             Activity        0.42063         0               1.06531         0               NaN             0.39484   
// AS3             Activity        0.21741         0.18367         1.06531         1.06531         NaN             0.20408   
    @Test
    @Timeout(120)
    public void testBug1Decreasing() {
        assertResultsMatchSolverLN(SolverLnSimpleBugModels.bug1network3());
    }















    //BUG 2: correct if network depth = 2 or 3, incorrect if network depth > 3

    // Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput      
    // P1        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN       
    // P2        	Processor 	NaN       	0.18367   	NaN       	NaN       	NaN       	NaN       
    // T1        	RefTask   	0.36735   	0         	NaN       	0         	NaN       	0.40816   
    // T2        	Task      	0.36735   	0.18367   	NaN       	0.90000   	NaN       	0.40816   
    // E1        	Entry     	0.36735   	NaN       	0.90000   	NaN       	NaN       	0.40816   
    // E2        	Entry     	0.36735   	NaN       	0.90000   	NaN       	NaN       	0.40816   
    // AS1       	Activity  	0.36735   	0         	0.90000   	0         	NaN       	0.40816   
    // AS2       	Activity  	0.36735   	0.18367   	0.90000   	0.90000   	NaN       	0.40816 
    @Test
    @Timeout(120)
    public void testBug2Depth2() {
        assertResultsMatchSolverLN(SolverLnSimpleBugModels.bug2network1());
    }

    // Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput      
    // P1        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN       
    // P2        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN       
    // P3        	Processor 	NaN       	0.18367   	NaN       	NaN       	NaN       	NaN       
    // T1        	RefTask   	0.36735   	0         	NaN       	0         	NaN       	0.40816   
    // T2        	Task      	0.36735   	0         	NaN       	0         	NaN       	0.40816   
    // T3        	Task      	0.36735   	0.18367   	NaN       	0.90000   	NaN       	0.40816   
    // E1        	Entry     	0.36735   	NaN       	0.90000   	NaN       	NaN       	0.40816   
    // E2        	Entry     	0.36735   	NaN       	0.90000   	NaN       	NaN       	0.40816   
    // E3        	Entry     	0.36735   	NaN       	0.90000   	NaN       	NaN       	0.40816   
    // AS1       	Activity  	0.36735   	0         	0.90000   	0         	NaN       	0.40816   
    // AS2       	Activity  	0.36735   	0         	0.90000   	0         	NaN       	0.40816   
    // AS3       	Activity  	0.36735   	0.18367   	0.90000   	0.90000   	NaN       	0.40816  
    @Test
    @Timeout(120)
    public void testBug2Depth3() {
        assertResultsMatchSolverLN(SolverLnSimpleBugModels.bug2network2());
    }

    // === SolverLN ===
    // Node            NodeType        QLen            Util            RespT           ResidT          ArvR            Tput      
    // P1              Processor       NaN             0               NaN             NaN             NaN             NaN       
    // P2              Processor       NaN             0               NaN             NaN             NaN             NaN       
    // P3              Processor       NaN             0               NaN             NaN             NaN             NaN       
    // P4              Processor       NaN             0.35536         NaN             NaN             NaN             NaN       
    // T1              RefTask         0.42063         0               NaN             0               NaN             0.39484   
    // T2              Task            0.42063         0               NaN             0               NaN             0.39484   
    // T3              Task            0.42063         0               NaN             0               NaN             0.39484   
    // T4              Task            0.42063         0.35536         NaN             1.06531         NaN             0.39484   
    // E1              Entry           0.42063         NaN             1.06531         NaN             NaN             0.39484   
    // E2              Entry           0.42063         NaN             1.06531         NaN             NaN             0.39484   
    // E3              Entry           0.42063         NaN             1.06531         NaN             NaN             0.39484   
    // E4              Entry           0.42063         NaN             1.06531         NaN             NaN             0.39484   
    // AS1             Activity        0.42063         0               1.06531         0               NaN             0.39484   
    // AS2             Activity        0.42063         0               1.06531         0               NaN             0.39484   
    // AS3             Activity        0.42063         0               1.06531         0               NaN             0.39484   
    // AS4             Activity        0.42063         0.35536         1.06531         1.06531         NaN             0.39484   
    // === SolverLNSimple ===
    // Node            NodeType        QLen            Util            RespT           ResidT          ArvR            Tput      
    // P1              Processor       NaN             0               NaN             NaN             NaN             NaN       
    // P2              Processor       NaN             0               NaN             NaN             NaN             NaN       
    // P3              Processor       NaN             0               NaN             NaN             NaN             NaN       
    // P4              Processor       NaN             1.00000         NaN             NaN             NaN             NaN       
    // T1              RefTask         0.90000         0               NaN             0               NaN             0.50000   
    // T2              Task            0.90000         0               NaN             0               NaN             0.50000   
    // T3              Task            2.00000         0               NaN             0               NaN             1.11111   
    // T4              Task            2.00000         1.00000         NaN             1.80000         NaN             1.11111   
    // E1              Entry           0.90000         NaN             1.80000         NaN             NaN             0.50000   
    // E2              Entry           0.90000         NaN             1.80000         NaN             NaN             0.50000   
    // E3              Entry           2.00000         NaN             1.80000         NaN             NaN             1.11111   
    // E4              Entry           2.00000         NaN             1.80000         NaN             NaN             1.11111   
    // AS1             Activity        0.90000         0               1.80000         0               NaN             0.50000   
    // AS2             Activity        0.90000         0               1.80000         0               NaN             0.50000   
    // AS3             Activity        2.00000         0               1.80000         0               NaN             1.11111   
    // AS4             Activity        2.00000         1.00000         1.80000         1.80000         NaN             1.11111      
    @Test
    @Timeout(120)
    public void testBug2Depth4() {
        assertResultsMatchSolverLN(SolverLnSimpleBugModels.bug2network3());
    }

    //     === SolverLN ===
    // Node            NodeType        QLen            Util            RespT           ResidT          ArvR            Tput      
    // P1              Processor       NaN             0               NaN             NaN             NaN             NaN       
    // P2              Processor       NaN             0               NaN             NaN             NaN             NaN       
    // P3              Processor       NaN             0               NaN             NaN             NaN             NaN       
    // P4              Processor       NaN             0               NaN             NaN             NaN             NaN       
    // P5              Processor       NaN             0.18367         NaN             NaN             NaN             NaN       
    // T1              RefTask         0.36735         0               NaN             0               NaN             0.40816   
    // T2              Task            0.36735         0               NaN             0               NaN             0.40816   
    // T3              Task            0.36735         0               NaN             0               NaN             0.40816   
    // T4              Task            0.36735         0               NaN             0               NaN             0.40816   
    // T5              Task            0.36735         0.18367         NaN             0.90000         NaN             0.40816   
    // E1              Entry           0.36735         NaN             0.90000         NaN             NaN             0.40816   
    // E2              Entry           0.36735         NaN             0.90000         NaN             NaN             0.40816   
    // E3              Entry           0.36735         NaN             0.90000         NaN             NaN             0.40816   
    // E4              Entry           0.36735         NaN             0.90000         NaN             NaN             0.40816   
    // E5              Entry           0.36735         NaN             0.90000         NaN             NaN             0.40816   
    // AS1             Activity        0.36735         0               0.90000         0               NaN             0.40816   
    // AS2             Activity        0.36735         0               0.90000         0               NaN             0.40816   
    // AS3             Activity        0.36735         0               0.90000         0               NaN             0.40816   
    // AS4             Activity        0.36735         0               0.90000         0               NaN             0.40816   
    // AS5             Activity        0.36735         0.18367         0.90000         0.90000         NaN             0.40816   

    // === SolverLNSimple ===
    // Node            NodeType        QLen            Util            RespT           ResidT          ArvR            Tput      
    // P1              Processor       NaN             0               NaN             NaN             NaN             NaN       
    // P2              Processor       NaN             0               NaN             NaN             NaN             NaN       
    // P3              Processor       NaN             0               NaN             NaN             NaN             NaN       
    // P4              Processor       NaN             0               NaN             NaN             NaN             NaN       
    // P5              Processor       NaN             1.00000         NaN             NaN             NaN             NaN       
    // T1              RefTask         0.45000         0               NaN             0               NaN             0.50000   
    // T2              Task            0.45000         0               NaN             0               NaN             0.50000   
    // T3              Task            180000000.00000 0               NaN             0               NaN             200000000.00000
    // T4              Task            2.00000         0               NaN             0               NaN             2.22222   
    // T5              Task            2.00000         1.00000         NaN             0.90000         NaN             2.22222   
    // E1              Entry           0.45000         NaN             0.90000         NaN             NaN             0.50000   
    // E2              Entry           0.45000         NaN             0.90000         NaN             NaN             0.50000   
    // E3              Entry           180000000.00000 NaN             0.90000         NaN             NaN             200000000.00000
    // E4              Entry           2.00000         NaN             0.90000         NaN             NaN             2.22222   
    // E5              Entry           2.00000         NaN             0.90000         NaN             NaN             2.22222   
    // AS1             Activity        0.45000         0               0.90000         0               NaN             0.50000   
    // AS2             Activity        0.45000         0               0.90000         0               NaN             0.50000   
    // AS3             Activity        180000000.00000 0               0.90000         0               NaN             200000000.00000
    // AS4             Activity        2.00000         0               0.90000         0               NaN             2.22222   
    // AS5             Activity        2.00000         1.00000         0.90000         0.90000         NaN             2.22222 
    @Test
    @Timeout(120)
    public void testBug2Depth5() {
        assertResultsMatchSolverLN(SolverLnSimpleBugModels.bug2network4());
    }

    













    
    //BUG 3: correct if intermediate think time is equal to above or below, incorrect if differs - roughly

    // Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput      
    // P1        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN       
    // P2        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN       
    // P3        	Processor 	NaN       	0.18367   	NaN       	NaN       	NaN       	NaN       
    // T1        	RefTask   	0.36735   	0         	NaN       	0         	NaN       	0.40816   
    // T2        	Task      	0.36735   	0         	NaN       	0         	NaN       	0.40816   
    // T3        	Task      	0.36735   	0.18367   	NaN       	0.90000   	NaN       	0.40816   
    // E1        	Entry     	0.36735   	NaN       	0.90000   	NaN       	NaN       	0.40816   
    // E2        	Entry     	0.36735   	NaN       	0.90000   	NaN       	NaN       	0.40816   
    // E3        	Entry     	0.36735   	NaN       	0.90000   	NaN       	NaN       	0.40816   
    // AS1       	Activity  	0.36735   	0         	0.90000   	0         	NaN       	0.40816   
    // AS2       	Activity  	0.36735   	0         	0.90000   	0         	NaN       	0.40816   
    // AS3       	Activity  	0.36735   	0.18367   	0.90000   	0.90000   	NaN       	0.40816  
    @Test
    @Timeout(120)
    public void testBug3EqualAbove() {
        assertResultsMatchSolverLN(SolverLnSimpleBugModels.bug3network1());
    }

    // Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput      
    // P1        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN       
    // P2        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN       
    // P3        	Processor 	NaN       	0.18367   	NaN       	NaN       	NaN       	NaN       
    // T1        	RefTask   	0.36735   	0         	NaN       	0         	NaN       	0.40816   
    // T2        	Task      	0.36735   	0         	NaN       	0         	NaN       	0.40816   
    // T3        	Task      	0.36735   	0.18367   	NaN       	0.90000   	NaN       	0.40816   
    // E1        	Entry     	0.36735   	NaN       	0.90000   	NaN       	NaN       	0.40816   
    // E2        	Entry     	0.36735   	NaN       	0.90000   	NaN       	NaN       	0.40816   
    // E3        	Entry     	0.36735   	NaN       	0.90000   	NaN       	NaN       	0.40816   
    // AS1       	Activity  	0.36735   	0         	0.90000   	0         	NaN       	0.40816   
    // AS2       	Activity  	0.36735   	0         	0.90000   	0         	NaN       	0.40816   
    // AS3       	Activity  	0.36735   	0.18367   	0.90000   	0.90000   	NaN       	0.40816   
    @Test
    @Timeout(120)
    public void testBug3EqualBoth() {
        assertResultsMatchSolverLN(SolverLnSimpleBugModels.bug3network2());
    }

    // === SolverLN ===
    // Node            NodeType        QLen            Util            RespT           ResidT          ArvR            Tput      
    // P1              Processor       NaN             0               NaN             NaN             NaN             NaN       
    // P2              Processor       NaN             0               NaN             NaN             NaN             NaN       
    // P3              Processor       NaN             0.18367         NaN             NaN             NaN             NaN       
    // T1              RefTask         2.00000         0               NaN             0               NaN             2.22222   
    // T2              Task            0.36735         0               NaN             0               NaN             0.40816   
    // T3              Task            0.36735         0.18367         NaN             0.90000         NaN             0.40816   
    // E1              Entry           2.00000         NaN             0.90000         NaN             NaN             2.22222   
    // E2              Entry           0.36735         NaN             0.90000         NaN             NaN             0.40816   
    // E3              Entry           0.36735         NaN             0.90000         NaN             NaN             0.40816   
    // AS1             Activity        2.00000         0               0.90000         0               NaN             2.22222   
    // AS2             Activity        0.36735         0               0.90000         0               NaN             0.40816   
    // AS3             Activity        0.36735         0.18367         0.90000         0.90000         NaN             0.40816   
    // === SolverLNSimple ===
    // Node            NodeType        QLen            Util            RespT           ResidT          ArvR            Tput      
    // P1              Processor       NaN             0               NaN             NaN             NaN             NaN       
    // P2              Processor       NaN             0               NaN             NaN             NaN             NaN       
    // P3              Processor       NaN             1.00000         NaN             NaN             NaN             NaN       
    // T1              RefTask         0.45000         0               NaN             0               NaN             2.22222   
    // T2              Task            0.45000         0               NaN             0               NaN             0.50000   
    // T3              Task            2.00000         1.00000         NaN             0.90000         NaN             2.22222   
    // E1              Entry           0.45000         NaN             0.90000         NaN             NaN             2.22222   
    // E2              Entry           0.45000         NaN             0.90000         NaN             NaN             0.50000   
    // E3              Entry           2.00000         NaN             0.90000         NaN             NaN             2.22222   
    // AS1             Activity        0.45000         0               0.90000         0               NaN             2.22222   
    // AS2             Activity        0.45000         0               0.90000         0               NaN             0.50000   
    // AS3             Activity        2.00000         1.00000         0.90000         0.90000         NaN             2.22222
    @Test
    @Timeout(120)
    public void testBug3NotEqual1() {
        assertResultsMatchSolverLN(SolverLnSimpleBugModels.bug3network3());
    }

    // === SolverLN ===
    // Node            NodeType        QLen            Util            RespT           ResidT          ArvR            Tput      
    // P1              Processor       NaN             0               NaN             NaN             NaN             NaN       
    // P2              Processor       NaN             0               NaN             NaN             NaN             NaN       
    // P3              Processor       NaN             0.10112         NaN             NaN             NaN             NaN       
    // T1              RefTask         0.36735         0               NaN             0               NaN             0.40816   
    // T2              Task            0.20225         0               NaN             0               NaN             0.22472   
    // T3              Task            0.20225         0.10112         NaN             0.90000         NaN             0.22472   
    // E1              Entry           0.36735         NaN             0.90000         NaN             NaN             0.40816   
    // E2              Entry           0.20225         NaN             0.90000         NaN             NaN             0.22472   
    // E3              Entry           0.20225         NaN             0.90000         NaN             NaN             0.22472   
    // AS1             Activity        0.36735         0               0.90000         0               NaN             0.40816   
    // AS2             Activity        0.20225         0               0.90000         0               NaN             0.22472   
    // AS3             Activity        0.20225         0.10112         0.90000         0.90000         NaN             0.22472   
    // === SolverLNSimple ===
    // Node            NodeType        QLen            Util            RespT           ResidT          ArvR            Tput      
    // P1              Processor       NaN             0               NaN             NaN             NaN             NaN       
    // P2              Processor       NaN             0               NaN             NaN             NaN             NaN       
    // P3              Processor       NaN             0.18367         NaN             NaN             NaN             NaN       
    // T1              RefTask         0.22500         0               NaN             0               NaN             0.40816   
    // T2              Task            0.22500         0               NaN             0               NaN             0.25000   
    // T3              Task            0.36735         0.18367         NaN             0.90000         NaN             0.40816   
    // E1              Entry           0.22500         NaN             0.90000         NaN             NaN             0.40816   
    // E2              Entry           0.22500         NaN             0.90000         NaN             NaN             0.25000   
    // E3              Entry           0.36735         NaN             0.90000         NaN             NaN             0.40816   
    // AS1             Activity        0.22500         0               0.90000         0               NaN             0.40816   
    // AS2             Activity        0.22500         0               0.90000         0               NaN             0.25000   
    // AS3             Activity        0.36735         0.18367         0.90000         0.90000         NaN             0.40816
    @Test
    @Timeout(120)
    public void testBug3NotEqual2() {
        assertResultsMatchSolverLN(SolverLnSimpleBugModels.bug3network4());
    }

    // Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput      
    // P1        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN       
    // P2        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN       
    // P3        	Processor 	NaN       	0.18367   	NaN       	NaN       	NaN       	NaN       
    // T1        	RefTask   	0.36735   	0         	NaN       	0         	NaN       	0.40816   
    // T2        	Task      	0.36735   	0         	NaN       	0         	NaN       	0.40816   
    // T3        	Task      	0.36735   	0.18367   	NaN       	0.90000   	NaN       	0.40816   
    // E1        	Entry     	0.36735   	NaN       	0.90000   	NaN       	NaN       	0.40816   
    // E2        	Entry     	0.36735   	NaN       	0.90000   	NaN       	NaN       	0.40816   
    // E3        	Entry     	0.36735   	NaN       	0.90000   	NaN       	NaN       	0.40816   
    // AS1       	Activity  	0.36735   	0         	0.90000   	0         	NaN       	0.40816   
    // AS2       	Activity  	0.36735   	0         	0.90000   	0         	NaN       	0.40816   
    // AS3       	Activity  	0.36735   	0.18367   	0.90000   	0.90000   	NaN       	0.40816 
    @Test
    @Timeout(120)
    public void testBugNotEqualButPasses() {
        assertResultsMatchSolverLN(SolverLnSimpleBugModels.bug3network5());
    }




























    // BUG 4: correct if only last think time set, incorrect if any others are

    // Node      	NodeType  	QLen      	Util      	RespT     	ResidT    	ArvR      	Tput      
    // P1        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN       
    // P2        	Processor 	NaN       	0         	NaN       	NaN       	NaN       	NaN       
    // P3        	Processor 	NaN       	0.18367   	NaN       	NaN       	NaN       	NaN       
    // T1        	RefTask   	0.36735   	0         	NaN       	0         	NaN       	0.40816   
    // T2        	Task      	0.36735   	0         	NaN       	0         	NaN       	0.40816   
    // T3        	Task      	0.36735   	0.18367   	NaN       	0.90000   	NaN       	0.40816   
    // E1        	Entry     	0.36735   	NaN       	0.90000   	NaN       	NaN       	0.40816   
    // E2        	Entry     	0.36735   	NaN       	0.90000   	NaN       	NaN       	0.40816   
    // E3        	Entry     	0.36735   	NaN       	0.90000   	NaN       	NaN       	0.40816   
    // AS1       	Activity  	0.36735   	0         	0.90000   	0         	NaN       	0.40816   
    // AS2       	Activity  	0.36735   	0         	0.90000   	0         	NaN       	0.40816   
    // AS3       	Activity  	0.36735   	0.18367   	0.90000   	0.90000   	NaN       	0.40816 
    @Test
    @Timeout(120)
    public void testBug4OnlyLast() {
        assertResultsMatchSolverLN(SolverLnSimpleBugModels.bug4network1());
    }

    // === SolverLN ===
    // Node            NodeType        QLen            Util            RespT           ResidT          ArvR            Tput      
    // P1              Processor       NaN             0.12500         NaN             NaN             NaN             NaN       
    // P2              Processor       NaN             0               NaN             NaN             NaN             NaN       
    // P3              Processor       NaN             0.16071         NaN             NaN             NaN             NaN       
    // T1              RefTask         0.57143         0.12500         NaN             0.70000         NaN             0.35714   
    // T2              Task            0.32143         0               NaN             0               NaN             0.35714   
    // T3              Task            0.32143         0.16071         NaN             0.90000         NaN             0.35714   
    // E1              Entry           0.57143         NaN             1.60000         NaN             NaN             0.35714   
    // E2              Entry           0.32143         NaN             0.90000         NaN             NaN             0.35714   
    // E3              Entry           0.32143         NaN             0.90000         NaN             NaN             0.35714   
    // AS1             Activity        0.57143         0.12500         1.60000         0.70000         NaN             0.35714   
    // AS2             Activity        0.32143         0               0.90000         0               NaN             0.35714   
    // AS3             Activity        0.32143         0.16071         0.90000         0.90000         NaN             0.35714   
    // === SolverLNSimple ===
    // Node            NodeType        QLen            Util            RespT           ResidT          ArvR            Tput      
    // P1              Processor       NaN             0.14286         NaN             NaN             NaN             NaN       
    // P2              Processor       NaN             0               NaN             NaN             NaN             NaN       
    // P3              Processor       NaN             0.18367         NaN             NaN             NaN             NaN       
    // T1              RefTask         0.65306         0.14286         NaN             0.70000         NaN             0.40816   
    // T2              Task            0.36735         0               NaN             0               NaN             0.40816   
    // T3              Task            0.36735         0.18367         NaN             0.90000         NaN             0.40816   
    // E1              Entry           0.65306         NaN             1.60000         NaN             NaN             0.40816   
    // E2              Entry           0.36735         NaN             0.90000         NaN             NaN             0.40816   
    // E3              Entry           0.36735         NaN             0.90000         NaN             NaN             0.40816   
    // AS1             Activity        0.65306         0.14286         1.60000         0.70000         NaN             0.40816   
    // AS2             Activity        0.36735         0               0.90000         0               NaN             0.40816   
    // AS3             Activity        0.36735         0.18367         0.90000         0.90000         NaN             0.40816 
    @Test
    @Timeout(120)
    public void testBug4FirstAndLast() {
        assertResultsMatchSolverLN(SolverLnSimpleBugModels.bug4network2());
    }

    // === SolverLN ===
    // Node            NodeType        QLen            Util            RespT           ResidT          ArvR            Tput      
    // P1              Processor       NaN             0               NaN             NaN             NaN             NaN       
    // P2              Processor       NaN             0.18367         NaN             NaN             NaN             NaN       
    // P3              Processor       NaN             0               NaN             NaN             NaN             NaN       
    // T1              RefTask         0.36735         0               NaN             0               NaN             0.40816   
    // T2              Task            0.36735         0.18367         NaN             0.90000         NaN             0.40816   
    // T3              Task            0               0               NaN             0               NaN             0.40816   
    // E1              Entry           0.36735         NaN             0.90000         NaN             NaN             0.40816   
    // E2              Entry           0.36735         NaN             0.90000         NaN             NaN             0.40816   
    // E3              Entry           0               NaN             0               NaN             NaN             0.40816   
    // AS1             Activity        0.36735         0               0.90000         0               NaN             0.40816   
    // AS2             Activity        0.36735         0.18367         0.90000         0.90000         NaN             0.40816   
    // AS3             Activity        0               0               0               0               NaN             0.40816   
    // === SolverLNSimple ===
    // Node            NodeType        QLen            Util            RespT           ResidT          ArvR            Tput      
    // P1              Processor       NaN             0               NaN             NaN             NaN             NaN       
    // P2              Processor       NaN             0.18367         NaN             NaN             NaN             NaN       
    // P3              Processor       NaN             NaN             NaN             NaN             NaN             NaN       
    // T1              RefTask         0.36735         0               NaN             0               NaN             0.40816   
    // T2              Task            0.36735         0.18367         NaN             0.90000         NaN             0.40816   
    // T3              Task            NaN             NaN             NaN             NaN             NaN             NaN       
    // E1              Entry           0.36735         NaN             0.90000         NaN             NaN             0.40816   
    // E2              Entry           0.36735         NaN             0.90000         NaN             NaN             0.40816   
    // E3              Entry           NaN             NaN             NaN             NaN             NaN             NaN       
    // AS1             Activity        0.36735         0               0.90000         0               NaN             0.40816   
    // AS2             Activity        0.36735         0.18367         NaN             0.90000         NaN             0.40816   
    // AS3             Activity        NaN             0               NaN             NaN             NaN             NaN 
    @Test
    @Timeout(120)
    public void testBug4Middle() {
        assertResultsMatchSolverLN(SolverLnSimpleBugModels.bug4network3());
    }

}
