/*
 * Copyright (c) 2012-2026, QORE Lab, Imperial College London
 * All rights reserved.
 * Ported from BUTools V2.0
 */
package jline.lib.butools.ph

import jline.lib.butools.reptrans.mStaircase
import jline.util.matrix.Matrix

/**
 * Returns the minimal representation of the given ME distribution.
 *
 * @param alpha Initial vector of the matrix-exponential distribution (shape 1 x M)
 * @param A Matrix parameter of the matrix-exponential distribution (shape M x M)
 * @param how Determines how the representation is minimized. Possibilities:
 *        "cont": controllability reduction,
 *        "obs": observability reduction,
 *        "obscont": the minimum of observability and controllability order (default),
 *        "moment": moment order.
 * @param prec Precision used by the Staircase algorithm. The default value is 1e-12.
 * @return MERepresentation containing the initial vector and matrix parameter
 *         of the minimal representation.
 *
 * References:
 *   P. Buchholz, M. Telek, "On minimal representation of rational arrival
 *   processes." Madrid Conference on Queueing theory (MCQT), June 2010.
 */
fun minimalRepFromME(alpha: Matrix, A: Matrix, how: String = "obscont", prec: Double = 1e-12): MERepresentation {
    val N = A.numRows

    return when (how) {
        "cont" -> {
            // Controllability reduction
            // H0 = A, H1 = sum(-A, axis=1) * alpha = (-A*ones) * alpha
            val H0 = A
            // sum(-A, axis=1) gives column vector of negative row sums of A
            val negRowSums = Matrix(N, 1)
            for (i in 0 until N) {
                var rowSum = 0.0
                for (j in 0 until N) {
                    rowSum += A[i, j]
                }
                negRowSums[i, 0] = -rowSum
            }
            // H1 = negRowSums * alpha (N x 1) * (1 x N) = (N x N)
            val H1 = negRowSums.mult(alpha)

            val matrices = listOf(H0, H1)
            val ones = Matrix.ones(N, 1)
            val result = mStaircase(matrices, ones, prec)
            val B = result.first
            val n = result.second

            if (n < N) {
                val Binv = B.inv()
                // newAlpha = (alpha * B)[:, 0:n]
                val alphaB = alpha.mult(B)
                val newAlpha = Matrix(1, n)
                for (j in 0 until n) {
                    newAlpha[0, j] = alphaB[0, j]
                }
                // newA = (inv(B) * A * B)[0:n, 0:n]
                val BAB = Binv.mult(A).mult(B)
                val newA = Matrix(n, n)
                for (i in 0 until n) {
                    for (j in 0 until n) {
                        newA[i, j] = BAB[i, j]
                    }
                }
                MERepresentation(newAlpha, newA)
            } else {
                MERepresentation(alpha, A)
            }
        }
        "obs" -> {
            // Observability reduction: work with transpose
            // In Python:
            // G = [H0.T, H1.T] where H0 = A, H1 = sum(-A,1)*alpha
            // B, n = MStaircase(G, alpha.T, precision)
            // return ((alpha*B)[:,0:n], (inv(B)*A*B)[0:n,0:n])
            val H0 = A
            val negRowSums = Matrix(N, 1)
            for (i in 0 until N) {
                var rowSum = 0.0
                for (j in 0 until N) {
                    rowSum += A[i, j]
                }
                negRowSums[i, 0] = -rowSum
            }
            val H1 = negRowSums.mult(alpha)

            val matrices = listOf(H0.transpose(), H1.transpose())
            val alphaT = alpha.transpose()
            val result = mStaircase(matrices, alphaT, prec)
            val B = result.first
            val n = result.second

            if (n < N) {
                val Binv = B.inv()
                // newAlpha = (alpha * B)[:, 0:n]
                val alphaB = alpha.mult(B)
                val newAlpha = Matrix(1, n)
                for (j in 0 until n) {
                    newAlpha[0, j] = alphaB[0, j]
                }
                // newA = (inv(B) * A * B)[0:n, 0:n]
                val BAB = Binv.mult(A).mult(B)
                val newA = Matrix(n, n)
                for (i in 0 until n) {
                    for (j in 0 until n) {
                        newA[i, j] = BAB[i, j]
                    }
                }
                MERepresentation(newAlpha, newA)
            } else {
                MERepresentation(alpha, A)
            }
        }
        "obscont" -> {
            val rep1 = minimalRepFromME(alpha, A, "cont", prec)
            minimalRepFromME(rep1.alpha, rep1.A, "obs", prec)
        }
        "moment" -> {
            val order = meOrder(alpha, A, "moment", prec)
            if (order < N) {
                val moms = momentsFromME(alpha, A, 2 * order - 1)
                meFromMoments(moms)
            } else {
                MERepresentation(alpha, A)
            }
        }
        else -> throw IllegalArgumentException("MinimalRepFromME: Unknown method '$how'!")
    }
}
