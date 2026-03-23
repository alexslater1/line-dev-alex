/**
 * @file Queue-Dependent (QD) approximate MVA solver
 *
 * Implements the Queue-Dependent approximate Mean Value Analysis algorithm for closed
 * queueing networks. Uses gamma and beta scaling functions to model queue-dependent
 * service behavior, providing an iterative fixed-point approximation for performance metrics.
 *
 * @since LINE 3.0
 */
package jline.api.pfqn.mva

import jline.io.Ret
import jline.util.matrix.Matrix
import org.apache.commons.math3.util.FastMath

/**
 * Functional interface for gamma scaling function.
 * Takes a queue length vector A (M x 1) and returns a scaling vector (M x 1).
 */
interface GammaFunction {
    fun apply(A: Matrix): Matrix
}

/**
 * Functional interface for beta scaling function.
 * Takes a queue length matrix Akr (M x R) and returns a scaling matrix (M x R).
 */
interface BetaFunction {
    fun apply(Akr: Matrix): Matrix
}

/**
 * Queue-Dependent (QD) approximate MVA solver
 *
 * @param L   - the service demand matrix (M x R)
 * @param N   - the population vector (1 x R)
 * @param ga  - gamma scaling function (default: ones)
 * @param be  - beta scaling function (default: ones)
 * @param Q0  - initial queue length estimate (M x R), null for proportional initialization
 * @return - queue lengths Q, throughput X, utilization U, and iteration count
 */

@JvmOverloads
fun pfqn_qd(
    L: Matrix,
    N: Matrix,
    ga: GammaFunction? = null,
    be: BetaFunction? = null,
    Q0: Matrix? = null
): Ret.pfqnQd {
    val M = L.numRows
    val R = L.numCols

    val gaFunc = ga ?: object : GammaFunction {
        override fun apply(A: Matrix): Matrix {
            return Matrix.ones(M, 1)
        }
    }

    val beFunc = be ?: object : BetaFunction {
        override fun apply(Akr: Matrix): Matrix {
            return Matrix.ones(M, R)
        }
    }

    val Q: Matrix
    if (Q0 != null) {
        Q = Q0.copy()
    } else {
        // Initialize Q proportionally: L / sum(L,1) * N
        Q = Matrix(M, R)
        for (r in 0..<R) {
            var sumLr = 0.0
            for (i in 0..<M) {
                sumLr += L[i, r]
            }
            if (sumLr > 0) {
                for (i in 0..<M) {
                    Q[i, r] = L[i, r] / sumLr * N[r]
                }
            }
        }
    }

    val Ntot = N.elementSum()
    val delta = (Ntot - 1) / Ntot
    val deltar = Matrix(1, R)
    for (r in 0..<R) {
        deltar[r] = (N[r] - 1) / N[r]
    }

    val Q_1 = Q.copy()
    for (i in 0..<Q_1.numRows) {
        for (j in 0..<Q_1.numCols) {
            Q_1[i, j] = Q_1[i, j] * 10
        }
    }

    val tol = 1e-6
    var iter = 0
    val C = Matrix(M, R)
    val X = Matrix(1, R)
    val U = Matrix(M, R)

    var maxDiff = Double.MAX_VALUE
    while (maxDiff > tol) {
        iter++

        // Copy current Q to Q_1
        for (i in 0..<M) {
            for (j in 0..<R) {
                Q_1[i, j] = Q[i, j]
            }
        }

        // Build Ak vectors (one per class) and Akr matrix
        val Ak = Array<Matrix>(R) { Matrix(M, 1) }
        val Akr = Matrix(M, R)
        for (k in 0..<M) {
            var sumQk = 0.0
            for (s in 0..<R) {
                sumQk += Q[k, s]
            }
            for (r in 0..<R) {
                Ak[r][k, 0] = 1 + delta * sumQk
                Akr[k, r] = 1 + deltar[r] * Q[k, r]
            }
        }

        for (r in 0..<R) {
            val g = gaFunc.apply(Ak[r])
            val b = beFunc.apply(Akr)

            var sumCr = 0.0
            for (k in 0..<M) {
                var sumQk = 0.0
                for (s in 0..<R) {
                    sumQk += Q[k, s]
                }
                C[k, r] = L[k, r] * g[k, 0] * b[k, r] * (1 + delta * sumQk)
                sumCr += C[k, r]
            }

            X[r] = N[r] / sumCr

            for (k in 0..<M) {
                Q[k, r] = X[r] * C[k, r]
                U[k, r] = L[k, r] * g[k, 0] * b[k, r] * X[r]
            }
        }

        maxDiff = 0.0
        for (i in 0..<M) {
            for (j in 0..<R) {
                val diff = FastMath.abs(Q[i, j] - Q_1[i, j])
                if (diff > maxDiff) {
                    maxDiff = diff
                }
            }
        }
    }

    return Ret.pfqnQd(Q, X, U, iter)
}

/**
 * PFQN qd algorithms
 */
@Suppress("unused")
class PfqnQdAlgo {
    companion object {
        // Class documentation marker for Dokka
    }
}
