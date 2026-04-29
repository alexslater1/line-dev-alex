package jline.solvers.ln_simple.utils;

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

public final class MvaUtils {

    private MvaUtils() {
    }

    public static List<Node> getQueueNodes(Network layer) {
        List<Node> nodes = new ArrayList<Node>();
        for (Node node : layer.getNodes()) {
            if (node instanceof Queue) {
                nodes.add(node);
            }
        }
        return nodes;
    }

    public static JobClass getMainClass(Network net) {
        for (JobClass jc : net.getClasses()) {
            if (jc instanceof ClosedClass && ((ClosedClass) jc).getNumberOfJobs() > 0) {
                return jc;
            }
        }
        return net.getClasses().get(0);
    }

    public static Matrix buildDemandMatrix(List<Node> mvaNodes, Network net) {
        int m = mvaNodes.size();
        Matrix l = new Matrix(m, 1);
        for (int i = 0; i < m; i++) {
            Node node = mvaNodes.get(i);
            double mean = 0.0;
            if (node instanceof ServiceStation) {
                if (node instanceof Delay) {
                    // Delay nodes represent think time; do not place their service time into the demand matrix L
                    mean = 0.0;
                } else {
                    for (JobClass jc : net.getClasses()) {
                        double serviceMean = ((ServiceStation) node).getServiceProcess(jc).getMean();
                        if (!Double.isNaN(serviceMean)) {
                            mean += serviceMean;
                        }
                    }
                }
            }
            l.set(i, 0, mean);
        }
        return l;
    }

    public static Matrix buildN(Network net) {
        Matrix n = new Matrix(1, 1);
        JobClass jc = getMainClass(net);
        if (jc instanceof ClosedClass) {
            n.set(0, 0, ((ClosedClass) jc).getNumberOfJobs());
        } else {
            n.set(0, 0, Double.POSITIVE_INFINITY);
        }
        return n;
    }

    public static Matrix buildServerMatrix(List<Node> mvaNodes, Matrix n) {
        int m = mvaNodes.size();
        Matrix s = new Matrix(m, 1);
        double ntot = 0.0;
        for (int r = 0; r < n.getNumCols(); r++) {
            ntot += n.get(0, r);
        }
        if (ntot < 1.0) {
            ntot = 1.0;
        }
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

    public static Matrix buildThinkTimeMatrix(Network net) {
        JobClass main = getMainClass(net);
        // Look for a Delay node named "Clients" and return its mean for the main class
        for (Node node : net.getNodes()) {
            if (node instanceof Delay && "Clients".equals(node.getName())) {
                double mean = ((Delay) node).getServiceProcess(main).getMean();
                if (Double.isNaN(mean)) mean = 0.0;
                Matrix z = new Matrix(1, 1);
                z.set(0, 0, mean);
                return z;
            }
        }
        return new Matrix(1, 1);
    }

    public static Ret.pfqnMVA callMVA(Matrix l, Matrix n, Matrix z, Matrix s) {
        try {
            Matrix lambda = new Matrix(1, n.getNumCols());
            Matrix mi = Matrix.ones(l.getNumRows(), 1);

            Class<?> cls = Class.forName("jline.api.pfqn.mva.Pfqn_mvamsKt");
            Method method = cls.getMethod(
                    "pfqn_mvams",
                    Matrix.class,
                    Matrix.class,
                    Matrix.class,
                    Matrix.class,
                    Matrix.class,
                    Matrix.class
            );
            Object result = method.invoke(null, lambda, l, n, z, mi, s);
            return (Ret.pfqnMVA) result;
        } catch (Exception e) {
            return null;
        }
    }

    public static Queue findNonDelayQueue(Network net) {
        for (Node node : net.getNodes()) {
            if (node instanceof Queue && !(node instanceof Delay)) {
                return (Queue) node;
            }
        }
        return null;
    }
}
