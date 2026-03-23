/*
 * Copyright (c) 2012-2026, QORE Lab, Imperial College London
 * All rights reserved.
 * Ported from BUTools V2.0
 */
package jline.lib.butools.ph

import jline.util.matrix.Matrix
import kotlin.math.min

/**
 * Returns the order of the ME distribution (which is not
 * necessarily equal to the size of the representation).
 *
 * @param alpha The initial vector of the matrix-exponential distribution.
 * @param A The matrix parameter of the matrix-exponential distribution.
 * @param kind Determines which order is computed. Possibilities:
 *        "obs": observability order,
 *        "cont": controllability order,
 *        "obscont": the minimum of observability and controllability order,
 *        "moment": moment order (which is the default).
 * @param prec Precision used to detect if the determinant of the
 *        Hankel matrix is zero (in case of kind="moment" only),
 *        or the tolerance for the rank calculation.
 *        The default value is 1e-10.
 * @return The order of the ME distribution
 *
 * References:
 *   P. Buchholz, M. Telek, "On minimal representation of rational arrival
 *   processes." Madrid Conference on Queueing theory (MCQT), June 2010.
 */
fun meOrder(alpha: Matrix, A: Matrix, kind: String = "moment", prec: Double = 1e-10): Int {
    val N = A.numRows
    return when (kind) {
        "moment" -> {
            val moms = momentsFromME(alpha, A, 2 * N - 1)
            meOrderFromMoments(moms, prec)
        }
        "obs" -> {
            // Observability matrix: row n = alpha * A^n, for n = 0, 1, ..., N-1
            val re = Matrix(N, N)
            var alphaAn = alpha.copy()  // alpha * A^0 = alpha (1 x N)
            for (n in 0 until N) {
                for (j in 0 until N) {
                    re[n, j] = alphaAn[0, j]
                }
                alphaAn = alphaAn.mult(A)
            }
            re.rank()
        }
        "cont" -> {
            // Controllability matrix: row n = sum((A^T)^n, axis=0) = ones^T * (A^T)^n
            // In Python: re[n,:] = np.sum(A.T**n, 0)
            // A.T**n is matrix power of transpose, sum along axis=0 gives row sums of (A^T)^n
            val AT = A.transpose()
            val re = Matrix(N, N)
            var ATn = Matrix.eye(N)  // (A^T)^0 = I
            for (n in 0 until N) {
                // sum(ATn, axis=0) = column sums of ATn = row of ones * ATn
                for (j in 0 until N) {
                    var colSum = 0.0
                    for (i in 0 until N) {
                        colSum += ATn[i, j]
                    }
                    re[n, j] = colSum
                }
                ATn = ATn.mult(AT)
            }
            re.rank()
        }
        "obscont" -> {
            val obsOrder = meOrder(alpha, A, "obs", prec)
            val contOrder = meOrder(alpha, A, "cont", prec)
            min(obsOrder, contOrder)
        }
        else -> throw IllegalArgumentException("MEOrder: Invalid 'kind' parameter '$kind'!")
    }
}
