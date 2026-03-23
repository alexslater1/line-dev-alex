/*
 * Copyright (c) 2012-2026, QORE Lab, Imperial College London
 * All rights reserved.
 * Ported from BUTools V2.0
 *
 * Reference:
 * P. Buchholz, M. Telek, "On minimal representation of rational arrival
 * processes." Madrid Conference on Queueing theory (MCQT), June 2010.
 */
package jline.lib.butools.map

import jline.lib.butools.reptrans.mStaircase
import jline.util.matrix.Matrix
import jline.util.matrix.MatrixCell

/**
 * Returns the minimal representation of a marked rational arrival process.
 *
 * @param H The list of H0, H1, ..., HK matrices of the marked rational arrival process (as MatrixCell)
 * @param how Determines how the representation is minimized:
 *            "cont" = controllability, "obs" = observability,
 *            "obscont" = both (default)
 * @param precision Precision used by the Staircase algorithm (default 1e-12)
 * @return The D0, D1, ..., DK matrices of the minimal representation (as MatrixCell)
 */
@JvmOverloads
fun minimalRepFromMRAP(H: MatrixCell, how: String = "obscont", precision: Double = 1e-12): MatrixCell {
    val N = H[0].numRows
    val M = H.size()

    when (how) {
        "cont" -> {
            // Controllability reduction
            val matrices = ArrayList<Matrix>(M)
            for (i in 0 until M) {
                matrices.add(H[i])
            }
            val ones = Matrix.ones(N, 1)
            val result = mStaircase(matrices, ones, precision)
            val B = result.first
            val n = result.second
            if (n < N) {
                val Bi = B.inv()
                val ret = MatrixCell(M)
                for (i in 0 until M) {
                    val transformed = Bi.mult(H[i]).mult(B)
                    ret[i] = Matrix.getSubMatrix(transformed, 0, n, 0, n)
                }
                return ret
            } else {
                val ret = MatrixCell(M)
                for (i in 0 until M) {
                    ret[i] = H[i].copy()
                }
                return ret
            }
        }
        "obs" -> {
            // Observability reduction
            // Get marginal distribution alpha from MRAP
            val alpha = marginalDistributionFromMRAP(H, precision)

            // Work with transposed matrices and alpha vector
            val matrices = ArrayList<Matrix>(M)
            for (i in 0 until M) {
                matrices.add(H[i].transpose())
            }
            val result = mStaircase(matrices, alpha.alpha.transpose(), precision)
            val B = result.first
            val n = result.second
            if (n < N) {
                val Bi = B.inv()
                val ret = MatrixCell(M)
                for (i in 0 until M) {
                    // Transform in the original (non-transposed) space
                    val transformed = Bi.mult(H[i]).mult(B)
                    ret[i] = Matrix.getSubMatrix(transformed, 0, n, 0, n)
                }
                return ret
            } else {
                val ret = MatrixCell(M)
                for (i in 0 until M) {
                    ret[i] = H[i].copy()
                }
                return ret
            }
        }
        "obscont" -> {
            // First controllability, then observability
            val D = minimalRepFromMRAP(H, "cont", precision)
            return minimalRepFromMRAP(D, "obs", precision)
        }
        else -> throw IllegalArgumentException("MinimalRepFromMRAP: Unknown method '$how'")
    }
}

/**
 * Overload for Array<Matrix>.
 */
@JvmOverloads
fun minimalRepFromMRAP(H: Array<Matrix>, how: String = "obscont", precision: Double = 1e-12): MatrixCell {
    val cell = MatrixCell(H.size)
    for (i in H.indices) {
        cell[i] = H[i]
    }
    return minimalRepFromMRAP(cell, how, precision)
}
