package jline.solvers;

import jline.GlobalConstants;
import jline.VerboseLevel;
import jline.lang.ClosedClass;
import jline.lang.Network;
import jline.lang.RoutingMatrix;
import jline.lang.constant.SchedStrategy;
import jline.lang.nodes.Delay;
import jline.lang.nodes.Queue;
import jline.lang.processes.Exp;
import jline.solvers.ctmc.SolverCTMC;
import jline.solvers.ctmc.SolverCTMC;
import jline.solvers.ldes.SolverLDES;
import jline.solvers.jmt.SolverJMT;
import jline.solvers.mva.SolverMVA;
import jline.solvers.ssa.SolverSSA;
import jline.util.Maths;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;

import java.util.List;

import static jline.TestTools.*;
import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * Benchmark comparing JMT, LDES, and SSA runtime and accuracy on closed networks
 * with increasing number of queues (powers of 2).
 */
public class SimulatorBenchmark {

    private static final int SAMPLES = 10000;
    private static final int SEED = 23000;
    private static final int POPULATION = 10;
    private static final double THINK_TIME = 1.0;
    private static final double SERVICE_TIME = 0.5;

    @BeforeAll
    public static void setUp() {
        Maths.setRandomNumbersMatlab(false);
        GlobalConstants.setVerbose(VerboseLevel.SILENT);
    }

    /**
     * Build a closed tandem network: Delay -> Q1 -> Q2 -> ... -> QN -> Delay.
     */
    private static Network buildTandem(int numQueues) {
        Network model = new Network("Tandem_" + numQueues + "Q");

        Delay delay = new Delay(model, "Delay");
        Queue[] queues = new Queue[numQueues];
        for (int i = 0; i < numQueues; i++) {
            queues[i] = new Queue(model, "Queue" + (i + 1), SchedStrategy.FCFS);
        }

        ClosedClass cls = new ClosedClass(model, "Class1", POPULATION, delay, 0);

        delay.setService(cls, Exp.fitMean(THINK_TIME));
        for (int i = 0; i < numQueues; i++) {
            queues[i].setService(cls, Exp.fitMean(SERVICE_TIME));
        }

        RoutingMatrix P = model.initRoutingMatrix();
        P.set(cls, cls, delay, queues[0], 1.0);
        for (int i = 0; i < numQueues - 1; i++) {
            P.set(cls, cls, queues[i], queues[i + 1], 1.0);
        }
        P.set(cls, cls, queues[numQueues - 1], delay, 1.0);

        model.link(P);
        return model;
    }

    private static class Result {
        double time;
        double delayTput;
        double q1QLen;
        double q1RespT;
        double q1Util;
    }

    @Test
    public void benchmarkSimulators() {
        int[] queueCounts = {1, 2, 4, 8, 16, 32};

        System.out.println();
        System.out.printf("Closed tandem network: %d jobs, %d samples, think=%.1f, service=%.1f%n",
                POPULATION, SAMPLES, THINK_TIME, SERVICE_TIME);
        System.out.printf("Tolerance: %.0f%%%n%n", LOOSE_COARSE_TOL * 100);

        // Header
        System.out.printf("%-6s %-6s %10s %10s %10s %10s %10s%n",
                "Queues", "Solver", "Time(s)", "Tput", "QLen(Q1)", "RespT(Q1)", "Util(Q1)");
        System.out.println(new String(new char[72]).replace('\0', '-'));

        int totalChecks = 0;
        int passChecks = 0;

        for (int nq : queueCounts) {
            Result mva = runMVA(nq);
            Result jmt = runJMT(nq);
            Result ldes = runLDES(nq);
            Result ssa = runSSA(nq);

            printRow(nq, "MVA", mva);
            printRow(nq, "JMT", jmt);
            printRow(nq, "LDES", ldes);
            printRow(nq, "SSA", ssa);

            // Pairwise comparison of Tput with tolerance
            boolean jmtLdes = withinTol(jmt.delayTput, ldes.delayTput);
            boolean jmtSsa = withinTol(jmt.delayTput, ssa.delayTput);
            boolean ldesSsa = withinTol(ldes.delayTput, ssa.delayTput);
            boolean mvaJmt = withinTol(mva.delayTput, jmt.delayTput);
            boolean mvaLdes = withinTol(mva.delayTput, ldes.delayTput);
            boolean mvaSsa = withinTol(mva.delayTput, ssa.delayTput);

            System.out.printf("  Tput: MVA-JMT=%s(%s) MVA-LDES=%s(%s) MVA-SSA=%s(%s)%n",
                    mvaJmt ? "PASS" : "FAIL", pctDiff(mva.delayTput, jmt.delayTput),
                    mvaLdes ? "PASS" : "FAIL", pctDiff(mva.delayTput, ldes.delayTput),
                    mvaSsa ? "PASS" : "FAIL", pctDiff(mva.delayTput, ssa.delayTput));
            System.out.printf("        JMT-LDES=%s(%s) JMT-SSA=%s(%s) LDES-SSA=%s(%s)%n",
                    jmtLdes ? "PASS" : "FAIL", pctDiff(jmt.delayTput, ldes.delayTput),
                    jmtSsa ? "PASS" : "FAIL", pctDiff(jmt.delayTput, ssa.delayTput),
                    ldesSsa ? "PASS" : "FAIL", pctDiff(ldes.delayTput, ssa.delayTput));
            System.out.println();

            totalChecks += 6;
            if (mvaJmt) passChecks++;
            if (mvaLdes) passChecks++;
            if (mvaSsa) passChecks++;
            if (jmtLdes) passChecks++;
            if (jmtSsa) passChecks++;
            if (ldesSsa) passChecks++;
        }

        System.out.printf("Summary: %d/%d pairwise Tput checks within %.0f%% tolerance%n%n",
                passChecks, totalChecks, LOOSE_COARSE_TOL * 100);

        assertTrue(passChecks == totalChecks,
                String.format("%d/%d pairwise checks failed", totalChecks - passChecks, totalChecks));
    }

    private boolean withinTol(double a, double b) {
        double avg = (Math.abs(a) + Math.abs(b)) / 2.0;
        if (avg < 1e-10) return true;
        return Math.abs(a - b) / avg <= LOOSE_COARSE_TOL;
    }

    private void printRow(int nq, String solver, Result r) {
        System.out.printf("%-6d %-6s %10.3f %10.5f %10.5f %10.5f %10.5f%n",
                nq, solver, r.time, r.delayTput, r.q1QLen, r.q1RespT, r.q1Util);
    }

    private String pctDiff(double a, double b) {
        double avg = (Math.abs(a) + Math.abs(b)) / 2.0;
        if (avg < 1e-10) return "0.00%";
        return String.format("%.2f%%", 100.0 * Math.abs(a - b) / avg);
    }

    private Result extract(NetworkAvgTable table, long elapsed) {
        Result r = new Result();
        r.time = elapsed / 1e9;
        List<String> stations = table.getStationNames();
        List<Double> tput = table.getTput();
        List<Double> qlen = table.getQLen();
        List<Double> respT = table.getRespT();
        List<Double> util = table.getUtil();
        for (int i = 0; i < stations.size(); i++) {
            if (stations.get(i).equals("Delay")) {
                r.delayTput = tput.get(i);
            } else if (stations.get(i).equals("Queue1")) {
                r.q1QLen = qlen.get(i);
                r.q1RespT = respT.get(i);
                r.q1Util = util.get(i);
            }
        }
        return r;
    }

    private Result runMVA(int numQueues) {
        Network model = buildTandem(numQueues);
        SolverMVA solver = new SolverMVA(model);
        long t0 = System.nanoTime();
        NetworkAvgTable table = solver.getAvgTable();
        long t1 = System.nanoTime();
        return extract(table, t1 - t0);
    }

    private Result runJMT(int numQueues) {
        Network model = buildTandem(numQueues);
        SolverOptions opts = new SolverOptions();
        opts.seed = SEED;
        opts.samples = SAMPLES;
        SolverJMT solver = new SolverJMT(model, opts);
        long t0 = System.nanoTime();
        NetworkAvgTable table = solver.getAvgTable();
        long t1 = System.nanoTime();
        return extract(table, t1 - t0);
    }

    private Result runLDES(int numQueues) {
        Network model = buildTandem(numQueues);
        SolverLDES solver = new SolverLDES(model, "seed", SEED, "samples", SAMPLES);
        long t0 = System.nanoTime();
        NetworkAvgTable table = solver.getAvgTable();
        long t1 = System.nanoTime();
        return extract(table, t1 - t0);
    }

    private Result runSSA(int numQueues) {
        Network model = buildTandem(numQueues);
        SolverSSA solver = new SolverSSA(model, "seed", SEED, "samples", SAMPLES);
        long t0 = System.nanoTime();
        NetworkAvgTable table = solver.getAvgTable();
        long t1 = System.nanoTime();
        return extract(table, t1 - t0);
    }
}
