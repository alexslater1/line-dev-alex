package jline.solvers.ln_simple;

import jline.lang.ClosedClass;
import jline.lang.JobClass;
import jline.lang.Network;
import jline.lang.RoutingMatrix;
import jline.lang.nodes.ServiceStation;

import java.io.Console;
import java.util.Map;
import java.util.HashMap;
import java.util.List;
import java.util.ArrayList;
import jline.io.Ret;
import jline.util.matrix.Matrix;
import jline.api.pfqn.mva.Pfqn_mvamsKt;
import jline.GlobalConstants;
import jline.lang.constant.SchedStrategy;
import jline.lang.nodes.Delay;
import jline.lang.nodes.Queue;
import jline.lang.processes.Exp;

public class SolverLNSimple {

    int N_LAYERS;
    Network[] ensemble;
    // name index: nodeName -> list of (layer, nodeIndex) pairs
    Map<String, List<int[]>> nameIndexMap = new HashMap<>();

    private void buildTwoLayerModel() {
        N_LAYERS = 2;
        ensemble = new Network[N_LAYERS];

        // Layer 0: top layer where t1 is a terminal and t3 is a server
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

        // Layer 1: bottom layer where t3 is a terminal and p3 is a server
        Network layer1 = new Network("L1");

        Delay t3Delay = new Delay(layer1, "t3");
        Queue p3Queue = new Queue(layer1, "p3", SchedStrategy.FCFS);
        p3Queue.setNumberOfServers(2);
        
        ClosedClass class_l1 = new ClosedClass(layer1, "C1", 2, t3Delay);
        
        t3Delay.setService(class_l1, Exp.fitMean(0)); // When a task m is acting as a client in an MVA submodel l, it is always modelled as infinite server. Its service time demand is found by taking the sum of the waiting times wmj to all of its servers in all other submodels except for those in the current submodel l
        p3Queue.setService(class_l1, Exp.fitMean(0.9));

        RoutingMatrix P1 = layer1.initRoutingMatrix();
        P1.addConnection(t3Delay, p3Queue);
        P1.addConnection(p3Queue, t3Delay);
        layer1.link(P1);

        // Add submodels to ensemble
        ensemble[0] = layer0;
        ensemble[1] = layer1;
    }

    public SolverLNSimple() {
        buildTwoLayerModel();
        buildNameIndex();
    }

    // Build a simple index of node names to their (layer,nodeIndex) locations
    private void buildNameIndex() {
        nameIndexMap.clear();
        for (int l = 0; l < N_LAYERS; l++) {
            Network net = ensemble[l];
            List<jline.lang.nodes.Node> nodes = net.getNodes();
            for (int i = 0; i < nodes.size(); i++) {
                String nm = nodes.get(i).getName();
                List<int[]> list = nameIndexMap.get(nm);
                if (list == null) {
                    list = new ArrayList<>();
                    nameIndexMap.put(nm, list);
                }
                list.add(new int[]{l, i});
            }
        }
    }

    public static void main(String[] args) {
        SolverLNSimple s = new SolverLNSimple();
//        s.applyPfqnMvaToSubmodel(0);
         s.iterateCoupledMva(10, 1e-4);
    }

    /**
    * Build demand and population matrices for the chosen submodel and run pfqn_mva.
    *
    * This method constructs the three matrices expected by the PFQN MVA solver:
    *  - L (M x R): demand matrix where M = number of stations/nodes in the submodel
    *    and R = number of classes. L[i,r] is the service demand (mean service time)
    *    of class r at station i. This includes Delay (infinite-server) nodes when
    *    think time is modelled as an internal Delay.
    *  - N (1 x R): population vector. N[0,r] is the population (number of jobs)
    *    for closed class r.
    *  - Z (1 x R): external think-time vector. Z[0,r] is the external think time
    *    (time spent outside modeled stations) for class r. In this demo we use the
    *    internal-Delay convention (think time is represented by a Delay node that
    *    appears in L), therefore Z is set to zeros to avoid double-counting.
    *
    * After building L,N,Z the method calls the Kotlin PFQN MVA wrapper and prints
    * throughput, queue lengths, utilizations and residence times.
     *
     * @param idx index of the submodel in the ensemble array
     */
    public void applyPfqnMvaToSubmodel(int idx) {
        Network net = ensemble[idx];

        // Build matrices (single-class)
        Matrix Z = new Matrix(1, 1);
        Z.set(0, 0, 0.0);
        Matrix L = buildDemandMatrix(net);
        Matrix N = buildN(net);
        Matrix S = buildServerMatrix(net, N);
        Ret.pfqnMVA res = callMVA(L, N, Z, S);
        if (res == null) return;

        System.out.println("pfqn_mva results for submodel " + idx + ":");
        System.out.println(" - Throughput X: " + (res.X != null ? res.X.toString() : "null"));
        System.out.println(" - Queue lengths Q: " + (res.Q != null ? res.Q.toString() : "null"));
        System.out.println(" - Utilizations U: " + (res.U != null ? res.U.toString() : "null"));
        System.out.println(" - Residence times R: " + (res.R != null ? res.R.toString() : "null"));

        checkInvariant(res, L, N, Z);
        printPerStationResidenceTimes(res, net);
    }

    // --- Helper methods to reduce duplication and clarify intent ---
    // Single-class demand matrix: one column only (class index 0)
    private Matrix buildDemandMatrix(Network net) {
        int M = net.getNumberOfNodes();
        Matrix L = new Matrix(M, 1);
        JobClass jc = net.getClasses().get(0);
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
        JobClass jc = net.getClasses().get(0);
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
            System.out.println(S);
            return Pfqn_mvamsKt.pfqn_mvams(lambda, L, N, Z, Matrix.ones(L.getNumRows(), 1), S);
        } catch (Exception e) {
            System.out.println("pfqn_mva call failed: " + e.getMessage());
            e.printStackTrace();
            return null;
        }
    }

    

    private void checkInvariant(Ret.pfqnMVA res, Matrix L, Matrix N, Matrix Z) {
        try {
            int M = L.getNumRows();
            double Npassed = N.get(0, 0);
            double Zpassed = Z.get(0, 0);
            double Xval = (res.X != null) ? res.X.get(0, 0) : 0.0;
            double Qsum = 0.0;
            if (res.Q != null) {
                for (int i = 0; i < M; i++) Qsum += res.Q.get(i, 0);
            }
            double check = Qsum + Xval * Zpassed;
            System.out.println(" DEBUG input L=\n" + L.toString());
            System.out.println(" DEBUG input N=\n" + N.toString());
            System.out.println(" DEBUG input Z=\n" + Z.toString());
            System.out.println(" DEBUG computed: Npassed=" + Npassed + " Qsum=" + Qsum + " X*Z=" + (Xval * Zpassed) + " Q+XZ=" + check);
            double tol = 1e-9;
            if (Math.abs(check - Npassed) > tol) {
                System.out.println(" WARNING: invariant Q_sum + X*Z != N (diff=" + (check - Npassed) + ")");
            } else {
                System.out.println(" OK: invariant holds (diff=" + (check - Npassed) + ")");
            }
        } catch (Exception _e) {
            System.out.println(" DEBUG invariant check failed: " + _e.getMessage());
        }
    }

    private void printPerStationResidenceTimes(Ret.pfqnMVA res, Network net) {
        if (res.X != null && res.Q != null && res.X.getNumRows() > 0 && res.X.getNumCols() > 0) {
            double xval = res.X.get(0, 0);
            if (xval > 0) {
                System.out.println(" - Per-station residence times (R_i = Q_i / X):");
                int M = net.getNumberOfNodes();
                for (int i = 0; i < M; i++) {
                    double qi = res.Q.get(i, 0);
                    double ri = qi / xval;
                    String nodeName = net.getNodes().get(i).getName();
                    System.out.println("    node[" + i + "] " + nodeName + " R_i=" + ri + " Q_i=" + qi);
                }
            }
        }
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
                int l = (si < N_LAYERS) ? si : (2 * N_LAYERS - 2 - si);
                Network layer = ensemble[l];
                List<jline.lang.nodes.Node> nodes = layer.getNodes();
                int M = layer.getNumberOfNodes();

                JobClass jc0 = layer.getClasses().get(0);
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
                    if (nameIndexMap.containsKey(name)) {
                        List<int[]> mapped = nameIndexMap.get(name);
                        for (int[] x : mapped)
                        {
                            if (x[0] == l+1)
                            {
                                JobClass jc = ensemble[x[0]].getClasses().get(0);
                                double pop = ((ClosedClass) jc).getNumberOfJobs();
                                double util = res.U.get(i, 0);
                                double z = pop * (1 - util) / throughput;
                                Delay dstNode = (Delay) ensemble[x[0]].getNodes().get(x[1]);
                                dstNode.setService(jc, Exp.fitMean(z));
                                System.out.println(" Updating think time for node " + name + " in layer " + x[0] + " to Z=" + z);
                            } else if (x[0] == l-1)
                            {
                                double s = 0.0;
                                for (int j = 0; j < M; j++) {
                                    double qj = res.Q.get(j, 0);
                                    double xj = res.X.get(0, 0);
                                    s += qj / xj;
                                }
                                JobClass jc = ensemble[x[0]].getClasses().get(0);
                                Queue dstNode = (Queue) ensemble[x[0]].getNodes().get(x[1]);
                                dstNode.setService(jc, Exp.fitMean(s));
                                System.out.println(" Updating service time for node " + name + " in layer " + x[0] + " to S=" + s);
                            }
                        }
                    }
                }

                // print layer results summary
            }

            if (maxDeltaX < tol) {
                System.out.println(" iterateCoupledMva: converged after " + (iter + 1) + " iterations");
                break;
            }
        }
    }
    

}
