/**
 * @file Bard-Schweitzer approximate MVA for FCFS scheduling with weighted priorities
 *
 * Implements the Bard-Schweitzer approximate MVA algorithm specialized for FCFS
 * scheduling with optional weighted priority extensions. Uses relative priority
 * weighting based on queue lengths to approximate per-class waiting times under
 * FCFS discipline.
 *
 * @since LINE 3.0
 */
package jline.api.pfqn.mva

import jline.io.Ret
import jline.util.Maths
import jline.util.matrix.Matrix
import org.apache.commons.math3.util.FastMath

/**
 * Bard-Schweitzer approximate MVA for FCFS scheduling with weighted priorities
 *
 * @param L       - the service demand matrix (M x R)
 * @param N       - the population vector (1 x R)
 * @param Z       - the think times vector (1 x R)
 * @param tol     - max tolerance admitted between successive iterations
 * @param maxiter - maximum number of iterations
 * @param QN0     - initial queue lengths (M x R), null for uniform initialization
 * @param weight  - weight matrix for relative priorities (M x R), null for ones
 * @return - the performance metrics for this network.
 */

@JvmOverloads
fun pfqn_bsfcfs(
    L: Matrix,
    N: Matrix,
    Z: Matrix? = null,
    tol: Double = 1.0e-6,
    maxiter: Int = 1000,
    QN0: Matrix? = null,
    weight: Matrix? = null
): Ret.pfqnAMVA {
    val M = L.numRows
    val R = L.numCols

    val Zmat = Z ?: Matrix(1, R)

    val CN = Matrix(M, R)
    val QN: Matrix
    if (QN0 == null || QN0.isEmpty) {
        QN = N.repmat(M, 1)
        for (i in 0..<QN.numRows) {
            for (j in 0..<QN.numCols) {
                QN[i, j] = QN[i, j] / M
            }
        }
    } else {
        QN = QN0.copy()
        // Add small epsilon to avoid zero problems as in MATLAB
        for (i in 0..<QN.numRows) {
            for (j in 0..<QN.numCols) {
                QN[i, j] = QN[i, j] + FastMath.ulp(1.0)
            }
        }
    }

    val weightMatrix = weight ?: Matrix.ones(M, R)

    val XN = Matrix(1, R)
    val UN = Matrix(M, R)
    val relprio = Matrix(M, R)

    var it = 1
    while (it <= maxiter) {
        val QN_1 = Matrix(QN)

        // Calculate relative priorities
        for (ist in 0..<M) {
            for (r in 0..<R) {
                relprio[ist, r] = QN[ist, r] * weightMatrix[ist, r]
            }
        }

        for (r in 0..<R) {
            for (ist in 0..<M) {
                CN[ist, r] = L[ist, r]
                for (s in 0..<R) {
                    if (s != r) {
                        // FCFS approximation with weighted priorities
                        CN[ist, r] = CN[ist, r] + L[ist, s] * QN[ist, s] * relprio[ist, s] / relprio[ist, r]
                    } else {
                        CN[ist, r] = CN[ist, r] + L[ist, r] * QN[ist, r] * (N[r] - 1) / N[r] * relprio[ist, s] / relprio[ist, r]
                    }
                }
            }
            XN[r] = N[r] / (Zmat[r] + Matrix.extractColumn(CN, r, null).elementSum())
        }
        for (r in 0..<R) {
            for (ist in 0..<M) {
                QN[ist, r] = XN[r] * CN[ist, r]
            }
        }
        for (r in 0..<R) {
            for (ist in 0..<M) {
                UN[ist, r] = XN[r] * L[ist, r]
            }
        }

        var maxabs = Double.MIN_VALUE
        for (i in 0..<QN.numRows) {
            for (j in 0..<QN.numCols) {
                val absValue = FastMath.abs(1 - QN[i, j] / QN_1[i, j])
                maxabs = Maths.max(maxabs, absValue)
            }
        }
        if (maxabs < tol) {
            break
        }
        it++
    }

    val RN = XN.repmat(M, 1)
    for (i in 0..<RN.numRows) {
        for (j in 0..<RN.numCols) {
            RN[i, j] = QN[i, j] / RN[i, j]
        }
    }
    return Ret.pfqnAMVA(QN, UN, RN, null, CN, XN, it)
}

/**
 * PFQN bsfcfs algorithms
 */
@Suppress("unused")
class PfqnBsfcfsAlgo {
    companion object {
        // Class documentation marker for Dokka
    }
}
