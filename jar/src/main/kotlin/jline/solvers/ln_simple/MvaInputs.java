package jline.solvers.ln_simple;

import jline.io.Ret;
import jline.lang.ClosedClass;
import jline.lang.JobClass;
import jline.lang.Network;
import jline.lang.nodes.Delay;
import jline.lang.nodes.Node;
import jline.lang.nodes.Queue;
import jline.lang.nodes.ServiceStation;
import jline.lang.nodes.Station;
import jline.util.matrix.Matrix;

import java.lang.reflect.Method;
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

    /** Invoke the Kotlin {@code pfqn_mvams} routine via reflection. Reflection is
     *  used instead of a direct call to avoid a compile-time dependency on the
     *  Kotlin module from this Java code. The closed-network case is forced by
     *  passing a zero {@code lambda} arrival-rate vector. */
    public static Ret.pfqnMVA callMVA(Matrix l, Matrix n, Matrix z, Matrix s) {
        try {
            Matrix lambda = new Matrix(1, n.getNumCols());
            Matrix mi = Matrix.ones(l.getNumRows(), 1);
            Class<?> cls = Class.forName("jline.api.pfqn.mva.Pfqn_mvamsKt");
            Method method = cls.getMethod("pfqn_mvams",
                    Matrix.class, Matrix.class, Matrix.class,
                    Matrix.class, Matrix.class, Matrix.class);
            Object result = method.invoke(null, lambda, l, n, z, mi, s);
            return (Ret.pfqnMVA) result;
        } catch (Exception e) {
            return null;
        }
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
