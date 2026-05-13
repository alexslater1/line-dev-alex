package jline.solvers.ln_simple.mva;

import jline.io.Ret;
import jline.lang.ClosedClass;
import jline.lang.JobClass;
import jline.lang.Network;
import jline.lang.nodes.Delay;
import jline.lang.nodes.Node;
import jline.lang.nodes.Queue;
import jline.lang.nodes.ServiceStation;
import jline.lang.nodes.Station;
import jline.solvers.ln_simple.SolverLNSimple;
import jline.util.matrix.Matrix;

import java.util.ArrayList;
import java.util.List;

/**
 * MVA matrix-builder utilities for {@link SolverLNSimple}.
 *
 * <p>Each ensemble layer is a two-node closed network (one {@code Clients} delay
 * plus one queue). These helpers assemble the inputs to the backing
 * {@code pfqn_mvams} routine:
 * <ul>
 *   <li>{@code L} — service-demand matrix at non-Delay stations</li>
 *   <li>{@code N} — per-class population vector</li>
 *   <li>{@code Z} — per-class think time from the {@code Clients} delay</li>
 *   <li>{@code S} — per-station server counts ({@code Integer.MAX_VALUE} mapped to {@code N_total})</li>
 * </ul>
 */
public final class MvaInputs {

    private MvaInputs() {}

    /** All Queue and Delay nodes in {@code layer}, preserving network order. */
    public static List<Node> getQueueNodes(Network layer) {
        List<Node> nodes = new ArrayList<Node>();
        for (Node node : layer.getNodes()) {
            if (node instanceof Queue || node instanceof Delay) {
                nodes.add(node);
            }
        }
        return nodes;
    }

    /** First closed class with positive population (the "main" class); falls back
     *  to the very first class of any kind when the layer has no active customers. */
    public static JobClass getMainClass(Network net) {
        for (JobClass jc : net.getClasses()) {
            if (jc instanceof ClosedClass && ((ClosedClass) jc).getNumberOfJobs() > 0) {
                return jc;
            }
        }
        return net.getClasses().get(0);
    }

    /** Closed classes with N &gt; 0. Classes with N = 0 are filtered so disabled
     *  classes (e.g. the aggregate-T class in rebuilt Case-B layers) do not appear
     *  in the MVA matrices. */
    public static List<ClosedClass> getClosedClasses(Network net) {
        List<ClosedClass> result = new ArrayList<ClosedClass>();
        for (JobClass jc : net.getClasses()) {
            if (jc instanceof ClosedClass && ((ClosedClass) jc).getNumberOfJobs() > 0) {
                result.add((ClosedClass) jc);
            }
        }
        return result;
    }

    /**
     * Build the {@code (numNodes × numClasses)} demand matrix L. Delay nodes get
     * demand 0 (their service time enters through Z, not L). For non-Delay
     * stations:
     * <ul>
     *   <li>Take the class's own service-process mean if it is not NaN.</li>
     *   <li>Otherwise fall back to the first non-NaN class's mean. This is a
     *       safety net for caller classes that the initialiser's demand-fix loop
     *       did not populate — a rare path, kept to avoid passing NaN to MVA.</li>
     * </ul>
     */
    public static Matrix buildDemandMatrix(List<Node> mvaNodes, Network net) {
        List<ClosedClass> closed = getClosedClasses(net);
        int m = mvaNodes.size();
        int R = closed.size();
        Matrix l = new Matrix(m, R);
        for (int r = 0; r < R; r++) {
            JobClass jc = closed.get(r);
            for (int i = 0; i < m; i++) {
                Node node = mvaNodes.get(i);
                double mean = 0.0;
                if (node instanceof ServiceStation && !(node instanceof Delay)) {
                    mean = ((ServiceStation) node).getServiceProcess(jc).getMean();
                    if (Double.isNaN(mean)) {
                        for (JobClass jc2 : net.getClasses()) {
                            double m2 = ((ServiceStation) node).getServiceProcess(jc2).getMean();
                            if (!Double.isNaN(m2)) { mean = m2; break; }
                        }
                        if (Double.isNaN(mean)) mean = 0.0;
                    }
                }
                l.set(i, r, mean);
            }
        }
        return l;
    }

    /** Row vector of class populations. */
    public static Matrix buildN(Network net) {
        List<ClosedClass> closed = getClosedClasses(net);
        if (closed.isEmpty()) throw new IllegalArgumentException("No closed class with positive jobs");
        Matrix n = new Matrix(1, closed.size());
        for (int r = 0; r < closed.size(); r++) {
            n.set(0, r, closed.get(r).getNumberOfJobs());
        }
        return n;
    }

    /** Per-station server count. Infinite-server stations
     *  ({@code Integer.MAX_VALUE}) are mapped to total population so the MVA
     *  backend treats them as IS, not as a single-server FCFS. */
    public static Matrix buildServerMatrix(List<Node> mvaNodes, Matrix n) {
        int m = mvaNodes.size();
        Matrix s = new Matrix(m, 1);
        double ntot = 0.0;
        for (int r = 0; r < n.getNumCols(); r++) ntot += n.get(0, r);
        if (ntot < 1.0) ntot = 1.0;
        for (int i = 0; i < m; i++) {
            Node node = mvaNodes.get(i);
            double servers = 1.0;
            if (node instanceof Station) {
                int nserv = ((Station) node).getNumberOfServers();
                servers = (nserv == Integer.MAX_VALUE) ? ntot : (double) nserv;
            }
            s.set(i, 0, servers);
        }
        return s;
    }

    /** Row vector Z of per-class think times read from the layer's
     *  {@code Clients} delay node. NaN means are treated as 0. */
    public static Matrix buildThinkTimeMatrix(Network net) {
        List<ClosedClass> closed = getClosedClasses(net);
        int R = closed.size();
        Matrix z = new Matrix(1, R);
        for (Node node : net.getNodes()) {
            if (node instanceof Delay && "Clients".equals(node.getName())) {
                for (int r = 0; r < R; r++) {
                    double mean = ((Delay) node).getServiceProcess(closed.get(r)).getMean();
                    z.set(0, r, Double.isNaN(mean) ? 0.0 : mean);
                }
                break;
            }
        }
        return z;
    }

    /**
     * Threshold on {@code prodN = ∏(N_r + 1)} above which {@link #callMVA}
     * stops using exact LD-MVA ({@link MvaLd}) and falls back to Seidmann
     * multi-server AMVA ({@link MvaAmva}). Exact MVA's cost grows roughly like
     * {@code prodN · Nsum}; AMVA cost is independent of {@code prodN}.
     *
     * <p>Mutable so characterisation tests can drive different values; production
     * callers should leave it at its default.
     */
    public static volatile long exactLatticeMax = 500L;

    /** Per-path dispatch counters. Tests reset before a solve and read after to
     *  see which paths actually fired on a given model. Plain (non-atomic)
     *  fields — {@code SolverLNSimple} is single-threaded. */
    public static long ldCalls = 0L;
    public static long amvaCalls = 0L;

    public static void resetDispatchCounters() {
        ldCalls = 0L; amvaCalls = 0L;
    }

    /** Solve a layer's two-station closed network.
     *
     *  <p>Every layer the LQN ensemble produces has finite per-class populations,
     *  so two paths cover everything:
     *  <ul>
     *    <li>At least one multi-server queue AND lattice size
     *        {@code prodN > }{@link #exactLatticeMax} → {@link MvaAmva}
     *        (Seidmann + BS), cost independent of {@code prodN}.</li>
     *    <li>Otherwise → {@link MvaLd}, exact load-dependent MVA. When every
     *        station has {@code S[i] = 1} the {@code mu} matrix collapses to
     *        all-ones and {@code MvaLd} degenerates to plain exact MVA — same
     *        answer the dedicated {@code pfqn_mva} would give, at a small
     *        constant-factor cost.</li>
     *  </ul>
     */
    public static Ret.pfqnMVA callMVA(Matrix l, Matrix n, Matrix z, Matrix s) {
        final int M = l.getNumRows();
        final int R = n.getNumCols();

        double ntot = 0.0;
        for (int r = 0; r < R; r++) ntot += n.get(0, r);
        final int ntotI = (int) ntot;

        boolean anyMulti = false;
        for (int i = 0; i < s.getNumRows(); i++) {
            double si = s.get(i, 0);
            if (Double.isFinite(si) && si > 1.0) { anyMulti = true; break; }
        }
        if (anyMulti) {
            final long cap = exactLatticeMax;
            long prodN = 1L;
            for (int r = 0; r < R; r++) {
                prodN *= (long) (n.get(0, r) + 1);
                if (prodN > cap) break;
            }
            if (prodN > cap) {
                amvaCalls++;
                return MvaAmva.solve(l, n, z, s);
            }
        }
        ldCalls++;
        Matrix mu = new Matrix(M, ntotI);
        for (int i = 0; i < M; i++) {
            double si = s.get(i, 0);
            for (int j = 0; j < ntotI; j++) {
                mu.set(i, j, Math.min(j + 1.0, si));
            }
        }
        return MvaLd.solve(l, n, z, mu);
    }

    /** The first {@link Queue} that is not a {@link Delay} — i.e. the server
     *  station of a single-server two-node layer. Returns {@code null} if none. */
    public static Queue findNonDelayQueue(Network net) {
        for (Node node : net.getNodes()) {
            if (node instanceof Queue && !(node instanceof Delay)) {
                return (Queue) node;
            }
        }
        return null;
    }
}
