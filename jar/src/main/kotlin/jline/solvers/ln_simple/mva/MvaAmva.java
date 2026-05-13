package jline.solvers.ln_simple.mva;

import jline.io.Ret;
import jline.util.matrix.Matrix;

import java.lang.reflect.Method;

/**
 * Seidmann multi-server approximate MVA for {@code SolverLNSimple} layers —
 * the same backend that {@code SolverLN}'s default {@code SolverMVA} path
 * uses on closed multi-server PFQNs.
 *
 * <h2>The Seidmann transform</h2>
 * For each queue station with {@code c} servers and per-class demand {@code L},
 * split the demand into two pieces:
 * <pre>
 *   L'[i,r] = L[i,r] / c[i]                              ← bottleneck demand
 *   ΔZ[r]   = Σ_q L[q,r] · (c[q] − 1) / c[q]             ← in-service portion moved to Z
 * </pre>
 * Then run single-server Bard–Schweitzer on {@code (L', N, Z + ΔZ)}. The
 * intuition: only {@code L/c} of each visit can actually <i>queue</i> at the
 * multi-server station (the bottleneck), so the other {@code L·(c−1)/c} is
 * pure delay that belongs in the think-time vector.
 *
 * <p>This is qualitatively better than naive Schweitzer-multi-server BS
 * ({@code W = L · (1 + Q·(N−1)/(N·c))}) at moderate utilisations: at low load
 * BS sees a near-empty queue and {@code W ≈ L'}, so total response {@code Z' + W ≈ Z + L},
 * the correct IS limit; at saturation {@code L'} dominates and the bottleneck
 * rate is the correct {@code c/L}. The naive formula has no IS floor and
 * overshoots {@code W} at moderate per-server utilisation.
 *
 * <h2>Reconstruction</h2>
 * BS's returned {@code Q} is the <i>in-queue</i> customer count in the
 * Seidmann-transformed single-server view. To recover the total queue length at
 * the original multi-server station, add the in-service portion back:
 * <pre>
 *   Q_total[i,r] = Q_BS[q,r] + X[r] · L[i,r] · (c[i] − 1) / c[i]
 * </pre>
 * Delay-like rows (Delay nodes and INF queues with {@code c ≥ Ntot}) are folded
 * straight into {@code Zeff} before BS sees them, and their {@code Q[i,r]} is
 * computed via Little's law: {@code Q = X · L}.
 *
 * <p>Used as the cheap fallback to {@link MvaLd} when the population lattice
 * {@code ∏(N_r + 1)} crosses the threshold where exact LD-MVA dominates wall
 * time (see {@code MvaInputs.callMVA}).
 */
public final class MvaAmva {

    private MvaAmva() {}

    /**
     * @param L  service-demand matrix (numStations × numClasses)
     * @param N  population row vector (1 × numClasses)
     * @param Z  think-time row vector (1 × numClasses)
     * @param S  per-station server count (numStations × 1); infinite-server
     *           stations are encoded as a value {@code >= sum(N)}
     * @return performance measures in the same layout as {@link MvaLd#solve}:
     *         {@code X} (1 × R), {@code Q} (M × R, total queue length at each
     *         station), {@code U} (M × 1 per-server utilisation summed across
     *         classes), {@code R} (cycle time, 1 × R), {@code lGN = NaN}.
     */
    public static Ret.pfqnMVA solve(Matrix L, Matrix N, Matrix Z, Matrix S) {
        final int M = L.getNumRows();
        final int R = L.getNumCols();
        double ntot = 0.0;
        for (int r = 0; r < R; r++) ntot += N.get(0, r);

        // Classify rows. Delay-like rows fold into Zeff (Little's law on
        // reconstruction); real queue stations get the Seidmann transform.
        // A station behaves as a delay when it has at least as many servers as
        // customers (every job always finds a free server) or no finite count.
        boolean[] isQueue = new boolean[M];
        int Mq = 0;
        for (int i = 0; i < M; i++) {
            double si = S.get(i, 0);
            boolean delayLike = !Double.isFinite(si) || si >= ntot || si <= 0.0;
            if (!delayLike) { isQueue[i] = true; Mq++; }
        }

        // Seidmann-transformed inputs to BS.  Zeff starts from the original Z
        // plus all delay-like demands (full L), and then gets the per-queue
        // Seidmann bump L·(c−1)/c added to it.
        Matrix Zeff = new Matrix(1, R);
        for (int r = 0; r < R; r++) Zeff.set(0, r, Z.get(0, r));
        for (int i = 0; i < M; i++) if (!isQueue[i]) {
            for (int r = 0; r < R; r++) Zeff.set(0, r, Zeff.get(0, r) + L.get(i, r));
        }

        Matrix qX;
        Matrix qQ_bs;   // BS's "in-queue only" Q at the transformed single-server view
        double[] cq = new double[Math.max(Mq, 1)];
        Matrix Lq_full = new Matrix(Math.max(Mq, 1), R);  // original L for the queue rows

        if (Mq == 0) {
            // Pure-delay limit: every station is IS-like.
            qX = new Matrix(1, R);
            for (int r = 0; r < R; r++) {
                double zr = Zeff.get(0, r);
                qX.set(0, r, zr > 0 ? N.get(0, r) / zr : 0.0);
            }
            qQ_bs = new Matrix(0, R);
        } else {
            // Build L' = L/c for the queue rows and bump Zeff by L·(c−1)/c.
            Matrix Lq_prime = new Matrix(Mq, R);
            int q = 0;
            for (int i = 0; i < M; i++) if (isQueue[i]) {
                double si = S.get(i, 0);
                cq[q] = si;
                for (int r = 0; r < R; r++) {
                    double lir = L.get(i, r);
                    Lq_full.set(q, r, lir);
                    Lq_prime.set(q, r, lir / si);
                    Zeff.set(0, r, Zeff.get(0, r) + lir * (si - 1.0) / si);
                }
                q++;
            }
            try {
                Class<?> cls = Class.forName("jline.api.pfqn.mva.Pfqn_bsKt");
                Method m = cls.getMethod("pfqn_bs", Matrix.class, Matrix.class, Matrix.class);
                Object res = m.invoke(null, Lq_prime, N, Zeff);
                qQ_bs = (Matrix) res.getClass().getField("Q").get(res);
                qX    = (Matrix) res.getClass().getField("X").get(res);
            } catch (Exception e) {
                return null;
            }
        }

        // Reconstruct full M × R outputs in original station order. Queue rows
        // get Q lifted back from "in-queue only" to "total at station" by
        // adding the in-service portion that Seidmann moved into Z. Delay-like
        // rows use Little's law directly.
        Matrix Qfull = new Matrix(M, R);
        Matrix Ufull = new Matrix(M, 1);
        Matrix Rfull = new Matrix(1, R);

        int qi = 0;
        for (int i = 0; i < M; i++) {
            if (isQueue[i]) {
                double c = cq[qi];
                double uSum = 0.0;
                for (int r = 0; r < R; r++) {
                    double x = qX.get(0, r);
                    double lir = Lq_full.get(qi, r);
                    double inService = x * lir * (c - 1.0) / c;
                    Qfull.set(i, r, qQ_bs.get(qi, r) + inService);
                    uSum += x * lir / c;             // per-server util
                }
                Ufull.set(i, 0, uSum);
                qi++;
            } else {
                double uSum = 0.0;
                for (int r = 0; r < R; r++) {
                    double x = qX.get(0, r);
                    double d = L.get(i, r);
                    Qfull.set(i, r, x * d);
                    uSum += x * d;
                }
                Ufull.set(i, 0, uSum);
            }
        }
        // Per-class cycle time, matching pfqn_mvald's CN = N/X − Z.
        for (int r = 0; r < R; r++) {
            double xr = qX.get(0, r);
            Rfull.set(0, r, xr > 0 ? N.get(0, r) / xr - Z.get(0, r) : 0.0);
        }

        return new Ret.pfqnMVA(qX, Qfull, Ufull, Rfull, Double.NaN);
    }
}
