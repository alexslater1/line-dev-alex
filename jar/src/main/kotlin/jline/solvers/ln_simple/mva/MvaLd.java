package jline.solvers.ln_simple.mva;

import jline.io.Ret;
import jline.solvers.ln_simple.SolverLNSimple;
import jline.util.matrix.Matrix;
import org.apache.commons.math3.util.FastMath;

import java.util.ArrayList;
import java.util.List;

/**
 * Closed-network load-dependent MVA, tuned for the two-station layers
 * {@link SolverLNSimple} solves.
 *
 * <p>This is a faithful, numerically identical re-implementation of
 * {@code jline.api.pfqn.ld.pfqn_mvald} (same recursion, same accumulation order,
 * same stabilisation rule). The difference is purely the data structures it uses
 * internally: {@code pfqn_mvald} keeps its working sets — the marginal queue-length
 * probabilities {@code pi(k|n)} and the per-population throughputs {@code Xs} — in
 * {@link Matrix} objects, but {@code Matrix} is sparse (CSC) backed, so the dense
 * {@code O(N^2)} reads/writes those tables receive each cost {@code O(nnz)}. For a
 * single-class layer with {@code N = 200} customers and a multi-server processor
 * that turns one layer solve into seconds. Here those tables are plain primitive
 * arrays, and only the final results are materialised as {@link Matrix}.
 *
 * <p>The shared API routine is left untouched on purpose; only the LQN
 * decomposition solver routes through this copy (see {@link MvaInputs#callMVA}).
 */
public final class MvaLd {

    private MvaLd() {}

    /**
     * @param L  service-demand matrix (numStations x numClasses)
     * @param N  population row vector (1 x numClasses), all finite
     * @param Z  think-time row vector (1 x numClasses)
     * @param mu load-dependent service-rate matrix (numStations x sum(N)),
     *           {@code mu[i][j]} = rate of station i with {@code j+1} jobs present
     * @return performance measures in the same layout as {@code pfqn_mvams} returns
     *         for the closed multi-server case: {@code X} (1 x R), {@code Q} (M x R),
     *         {@code U} (M x 1), {@code R} (cycle time, 1 x R), {@code lGN} = last log-G.
     */
    static Ret.pfqnMVA solve(Matrix L, Matrix N, Matrix Z, Matrix mu) {
        boolean warn = true;
        final int M = L.getNumRows();   // number of queues
        final int R = L.getNumCols();   // number of classes

        // Population bounds and lattice size.
        final int[] Ncap = new int[R];
        long prodNl = 1L;
        int Nsum = 0;
        for (int r = 0; r < R; r++) {
            int nr = (int) Math.round(N.get(r));
            Ncap[r] = nr;
            Nsum += nr;
            prodNl *= (nr + 1L);
        }
        final int prodN = (int) prodNl;

        // Lexicographic strides for the population-vector hash.
        final long[] stride = new long[R];
        long acc = 1L;
        for (int r = 0; r < R; r++) {
            stride[r] = acc;
            acc *= (Ncap[r] + 1L);
        }

        // Working sets, kept as primitive arrays (Matrix is sparse-backed and far too
        // slow for the dense O(N^2) access pattern below).
        final double[] L2 = new double[M * R];
        for (int i = 0; i < M; i++) for (int r = 0; r < R; r++) L2[i * R + r] = L.get(i, r);
        final double[] mu2 = new double[M * Math.max(Nsum, 1)];
        for (int i = 0; i < M; i++) for (int j = 0; j < Nsum; j++) mu2[i * Nsum + j] = mu.get(i, j);
        final double[] Z2 = new double[R];
        for (int r = 0; r < R; r++) Z2[r] = Z.get(r);

        final double[] Xs = new double[R * prodN];                 // X for the sub-model with one fewer customer
        // pi[ist] is a (Nsum+1) x prodN table, flattened row-major: pi[ist][k * prodN + h].
        final double[][] pi = new double[M][];
        for (int ist = 0; ist < M; ist++) {
            pi[ist] = new double[(Nsum + 1) * prodN];
            java.util.Arrays.fill(pi[ist], 1.0);                   // matches Matrix.ones(...) initialisation
        }

        final double[] WN = new double[M * R];
        final int[] nvec = new int[R];                             // current population vector
        final int[] hns = new int[R];

        final List<Double> lGN = new ArrayList<Double>();
        lGN.add(0.0);

        // Enumerate population vectors n = (0,...,0) up to N in lexicographic order.
        boolean first = true;
        while (true) {
            if (first) {
                first = false;
            } else {
                int s = R;
                while (s > 0 && nvec[s - 1] == Ncap[s - 1]) { nvec[s - 1] = 0; s--; }
                if (s == 0) break;       // exhausted
                nvec[s - 1]++;
            }

            int nsum = 0;
            long hnl = 0L;
            for (int r = 0; r < R; r++) { nsum += nvec[r]; hnl += stride[r] * nvec[r]; }
            final int hn = (int) hnl;

            java.util.Arrays.fill(WN, 0.0);

            // hashpop(n - e_s) for each class s that has at least one customer.
            for (int s = 0; s < R; s++) {
                if (nvec[s] > 0) hns[s] = hn - (int) stride[s];
                else hns[s] = -1;
            }

            for (int s = 0; s < R; s++) {
                if (nvec[s] > 0) {
                    final int hnsS = hns[s];
                    for (int ist = 0; ist < M; ist++) {
                        final double[] piIst = pi[ist];
                        final double lIstS = L2[ist * R + s];
                        final int muBase = ist * Nsum;
                        double w = 0.0;
                        int k = 0;
                        while (k < nsum) {
                            w += (lIstS / mu2[muBase + k]) * (k + 1) * piIst[k * prodN + hnsS];
                            k++;
                        }
                        WN[ist * R + s] = w;
                    }
                    double colSum = 0.0;
                    for (int ist = 0; ist < M; ist++) colSum += WN[ist * R + s];
                    Xs[s * prodN + hn] = nvec[s] / (Z2[s] + colSum);
                }
            }

            // pi(k|n) for k = 1 .. nsum
            int k = 0;
            while (k < nsum) {
                for (int ist = 0; ist < M; ist++) pi[ist][(k + 1) * prodN + hn] = 0.0;
                for (int s = 0; s < R; s++) {
                    if (nvec[s] > 0) {
                        final int hnsS = hns[s];
                        final double xsN = Xs[s * prodN + hn];
                        for (int ist = 0; ist < M; ist++) {
                            final double[] piIst = pi[ist];
                            piIst[(k + 1) * prodN + hn] +=
                                    (L2[ist * R + s] / mu2[ist * Nsum + k]) * xsN * piIst[k * prodN + hnsS];
                        }
                    }
                }
                k++;
            }

            // pi(0|n)
            for (int ist = 0; ist < M; ist++) {
                final double[] piIst = pi[ist];
                double sumpi = 0.0;
                int kk = 0;
                while (kk < nsum) { sumpi += piIst[(kk + 1) * prodN + hn]; kk++; }
                double p0 = 1 - sumpi;
                if (p0 < 0) {
                    if (warn) {
                        System.err.println("MvaLd: MVA-LD is numerically unstable on this model, "
                                + "LINE will force all probabilities to be non-negative.");
                        warn = false;
                    }
                    piIst[hn] = FastMath.ulp(1.0);
                } else {
                    piIst[hn] = p0;
                }
            }

            // Normalising-constant update (matches pfqn_mvald exactly).
            int lastNnz = -1;
            for (int r = 0; r < R; r++) if (nvec[r] != 0) lastNnz = r;
            if (lastNnz >= 0) {
                int sumn = 0, sumNN = 0, sumnp = 0;
                for (int i = 0; i < R; i++) {
                    if (i < lastNnz) { sumn += nvec[i]; sumNN += Ncap[i]; }
                    else if (i > lastNnz) sumnp += nvec[i];
                }
                if (sumn == sumNN && sumnp == 0) {
                    double logX = FastMath.log(Xs[lastNnz * prodN + hn]);
                    lGN.add(lGN.get(lGN.size() - 1) - logX);
                }
            }
        }

        // The last population vector enumerated is N itself; recover its hash.
        long hNl = 0L;
        for (int r = 0; r < R; r++) hNl += stride[r] * (long) Ncap[r];
        final int hN = (int) hNl;

        Matrix XN = new Matrix(1, R);
        for (int r = 0; r < R; r++) XN.set(0, r, Xs[r * prodN + hN]);

        Matrix QN = new Matrix(M, R);
        for (int i = 0; i < M; i++) {
            for (int r = 0; r < R; r++) {
                QN.set(i, r, WN[i * R + r] * Xs[r * prodN + hN]);
            }
        }

        Matrix UN = new Matrix(M, 1);
        for (int i = 0; i < M; i++) UN.set(i, 0, 1.0 - pi[i][hN]);

        Matrix CN = new Matrix(N.getNumRows(), N.getNumCols());
        for (int i = 0; i < CN.getNumRows(); i++) {
            for (int j = 0; j < CN.getNumCols(); j++) {
                CN.set(i, j, N.get(i, j) / XN.get(i, j) - Z.get(i, j));
            }
        }

        return new Ret.pfqnMVA(XN, QN, UN, CN, lGN.get(lGN.size() - 1));
    }
}
