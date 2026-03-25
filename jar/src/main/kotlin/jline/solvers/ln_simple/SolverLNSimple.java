package jline.solvers.ln_simple;

import jline.lang.ClosedClass;
import jline.lang.JobClass;
import jline.lang.Network;
import jline.lang.RoutingMatrix;
import jline.lang.nodes.ServiceStation;

import java.util.Map;
import java.util.HashMap;
import java.util.List;
import java.util.ArrayList;
import jline.io.Ret;
import jline.util.matrix.Matrix;
import jline.api.pfqn.mva.Pfqn_mvamsKt;
import jline.lang.constant.SchedStrategy;
import jline.lang.nodes.Delay;
import jline.lang.nodes.Queue;
import jline.lang.processes.Exp;

public class SolverLNSimple {

    private static final int SINGLE_CLASS_INDEX = 0;

    private int N_LAYERS;
    private Network[] ensemble;
    // name index: nodeName -> list of (layer, nodeIndex) pairs
    private final Map<String, List<int[]>> nameIndexMap = new HashMap<String, List<int[]>>();

    private void buildTwoLayerModel() {
        // Init configs
        N_LAYERS = 2;
        ensemble = new Network[N_LAYERS];

        // Layer 0: t1 -> t3
        Network layer0 = new Network("L0");
        Delay t1Delay = new Delay(layer0, "t1");
        Queue t3Queue = new Queue(layer0, "t3", SchedStrategy.FCFS);
        t3Queue.setNumberOfServers(2);
        ClosedClass class_l0 = new ClosedClass(layer0, "C1", 2, t1Delay);
        t1Delay.setService(class_l0, Exp.fitMean(0.0001));
        t3Queue.setService(class_l0, Exp.fitMean(0.0001));
        RoutingMatrix P0 = layer0.initRoutingMatrix();
        P0.addConnection(t1Delay, t3Queue);
        P0.addConnection(t3Queue, t1Delay);
        layer0.link(P0);
        ensemble[0] = layer0;

        // Layer 1: t3 -> p3
        Network layer1 = new Network("L1");
        Delay t3Delay = new Delay(layer1, "t3");
        Queue p3Queue = new Queue(layer1, "p3", SchedStrategy.FCFS);
        p3Queue.setNumberOfServers(2);
        ClosedClass class_l1 = new ClosedClass(layer1, "C1", 2, t3Delay);
        //When task m acts as a client: infinite server (demand = sum of the waiting times to all of its servers in all other submodels)
        t3Delay.setService(class_l1, Exp.fitMean(0));
        p3Queue.setService(class_l1, Exp.fitMean(0.9));
        RoutingMatrix P1 = layer1.initRoutingMatrix();
        P1.addConnection(t3Delay, p3Queue);
        P1.addConnection(p3Queue, t3Delay);
        layer1.link(P1);
        ensemble[1] = layer1;
    }

    // Build a simple index of node names to their (layer,nodeIndex) locations
    private void buildNameIndex() {
        nameIndexMap.clear();
        for (int l = 0; l < N_LAYERS; l++) {
            Network net = ensemble[l];
            List<jline.lang.nodes.Node> nodes = net.getNodes();
            for (int i = 0; i < nodes.size(); i++) {
                String nm = nodes.get(i).getName();
                List<int[]> list = nameIndexMap.computeIfAbsent(nm, k -> new ArrayList<>());
                list.add(new int[]{l, i});
            }
        }
    }

    public SolverLNSimple() {
        buildTwoLayerModel();
        buildNameIndex();
    }

    public static void main(String[] args) {
        SolverLNSimple s = new SolverLNSimple();
        s.iterateCoupledMva(10, 1e-4);
    }

    /**
     * Iteratively solve submodels
     * @param maxIter maximum number of fixed-point iterations
     * @param tol convergence tolerance for Z updates
     */
    public void iterateCoupledMva(int maxIter, double tol) {

        // track per-layer throughput to check convergence on throughput changes
        double[] prevX = new double[N_LAYERS];
        for (int i = 0; i < N_LAYERS; i++) prevX[i] = 0.0;

        //outer loop
        for (int iter = 0; iter < maxIter; iter++) {
            System.out.println("iteration " + iter);
            double maxDeltaX = 0.0;

            // solve layers
            int sweepLen = 2 * N_LAYERS - 1;
            for (int si = 0; si < sweepLen; si++) {
                int l = computeSweepLayerIndex(si);
                Network layer = ensemble[l];
                List<jline.lang.nodes.Node> nodes = layer.getNodes();
                int M = layer.getNumberOfNodes();

                JobClass jc0 = getSingleClass(layer);
                System.out.println(" Solving layer " + l + ": Z = " + ((ServiceStation) nodes.get(0)).getServiceProcess(jc0).getMean() + " S = " + ((ServiceStation) nodes.get(1)).getServiceProcess(jc0).getMean());

                // set up mva matrices
                Matrix N = buildN(layer);
                Matrix Z = new Matrix(1, 1);
                Z.set(0, 0, 0.0);
                Matrix L = buildDemandMatrix(layer);
                Matrix S = buildServerMatrix(layer, N);

                // perform mva
                Ret.pfqnMVA res = callMVA(L, N, Z, S);
                if (res == null) return;

                // throughput for this layer
                double throughput = res.X.get(0, 0);

                System.out.println(" Layer " + l + " throughput X=" + throughput);  

                // update convergence metric: change in throughput for this layer
                double deltaX = Math.abs(throughput - prevX[l]);
                if (deltaX > maxDeltaX) maxDeltaX = deltaX;
                prevX[l] = throughput;

                // Node-based coupling: propagate per-station residence times to mapped nodes in other layers
                for (int i = 0; i < M; i++) {
                    String name = nodes.get(i).getName();
                    List<int[]> mapped = nameIndexMap.get(name);
                    if (mapped == null) {
                        continue;
                    }
                    for (int[] location : mapped) {
                        int targetLayer = location[0];
                        int targetNodeIndex = location[1];
                        if (targetLayer == l + 1) {
                            updateThinkTimeFromUtilization(name, i, targetLayer, targetNodeIndex, throughput, res);
                        } else if (targetLayer == l - 1) {
                            updateServiceTimeFromResidence(name, targetLayer, targetNodeIndex, M, res);
                        }
                    }
                }
            }

            if (maxDeltaX < tol) {
                System.out.println(" iterateCoupledMva: converged after " + (iter + 1) + " iterations");
                break;
            }
        }
    }

    // --- Helper methods ---
    // Single-class demand matrix: one column only (class index 0)
    private Matrix buildDemandMatrix(Network net) {
        int M = net.getNumberOfNodes();
        Matrix L = new Matrix(M, 1);
        JobClass jc = getSingleClass(net);
        for (int i = 0; i < M; i++) {
            jline.lang.nodes.Node node = net.getNodes().get(i);
            double mean = 0.0;
            if (node instanceof ServiceStation) {
                mean = ((ServiceStation) node).getServiceProcess(jc).getMean();
            }
            L.set(i, 0, mean);
        }
        return L;
    }

    // Single-class population vector (1 x 1)
    private Matrix buildN(Network net) {
        Matrix N = new Matrix(1, 1);
        JobClass jc = getSingleClass(net);
        if (jc instanceof ClosedClass) {
            double nj = ((ClosedClass) jc).getNumberOfJobs();
            N.set(0, 0, nj);
        } else {
            N.set(0, 0, Double.POSITIVE_INFINITY);
        }
        return N;
    }

    private Matrix buildServerMatrix(Network net, Matrix N) {
        int M = net.getNumberOfNodes();
        Matrix S = new Matrix(M, 1);
        double Ntot = 0.0;
        for (int r = 0; r < N.getNumCols(); r++) Ntot += N.get(0, r);
        if (Ntot < 1.0) Ntot = 1.0;
        for (int i = 0; i < M; i++) {
            jline.lang.nodes.Node node = net.getNodes().get(i);
            double servers = 1.0;
            if (node instanceof jline.lang.nodes.Station) {
                int nserv = ((jline.lang.nodes.Station) node).getNumberOfServers();
                if (nserv == Integer.MAX_VALUE) {
                    servers = Ntot;
                } else {
                    servers = (double) nserv;
                }
            }
            S.set(i, 0, servers);
        }
        return S;
    }

    private Ret.pfqnMVA callMVA(Matrix L, Matrix N, Matrix Z, Matrix S) {
        try {
            Matrix lambda = new Matrix(1, N.getNumCols());
            return Pfqn_mvamsKt.pfqn_mvams(lambda, L, N, Z, Matrix.ones(L.getNumRows(), 1), S);
        } catch (Exception e) {
            System.out.println("pfqn_mva call failed: " + e.getMessage());
            e.printStackTrace();
            return null;
        }
    }

    private JobClass getSingleClass(Network net) {
        return net.getClasses().get(SINGLE_CLASS_INDEX);
    }

    private int computeSweepLayerIndex(int sweepIndex) {
        if (sweepIndex < N_LAYERS) {
            return sweepIndex;
        }
        return 2 * N_LAYERS - 2 - sweepIndex;
    }

    private void updateThinkTimeFromUtilization(String nodeName, int sourceNodeIndex, int targetLayer, int targetNodeIndex,
                                                double throughput, Ret.pfqnMVA res) {
        JobClass jc = getSingleClass(ensemble[targetLayer]);
        double pop = ((ClosedClass) jc).getNumberOfJobs();
        double util = res.U.get(sourceNodeIndex, SINGLE_CLASS_INDEX);
        double z = pop * (1 - util) / throughput;
        Delay dstNode = (Delay) ensemble[targetLayer].getNodes().get(targetNodeIndex);
        dstNode.setService(jc, Exp.fitMean(z));
        System.out.println(" Updating think time for node " + nodeName + " in layer " + targetLayer + " to Z=" + z);
    }

    private void updateServiceTimeFromResidence(String nodeName, int targetLayer, int targetNodeIndex, int nodeCount, Ret.pfqnMVA res) {
        double layerThroughput = res.X.get(SINGLE_CLASS_INDEX, SINGLE_CLASS_INDEX);
        double s = 0.0;
        for (int j = 0; j < nodeCount; j++) {
            s += res.Q.get(j, SINGLE_CLASS_INDEX) / layerThroughput;
        }
        JobClass jc = getSingleClass(ensemble[targetLayer]);
        Queue dstNode = (Queue) ensemble[targetLayer].getNodes().get(targetNodeIndex);
        dstNode.setService(jc, Exp.fitMean(s));
        System.out.println(" Updating service time for node " + nodeName + " in layer " + targetLayer + " to S=" + s);
    }
}
