/*
 * Copyright (c) 2012-2026, QORE Lab, Imperial College London
 * All rights reserved.
 * Ported from BUTools V2.0
 *
 * Reference:
 * Andras Horvath, Gabor Horvath, Miklos Telek, "A traffic based decomposition
 * of two-class queueing networks with priority service," Computer Networks
 * 53:(8) pp. 1235-1248. (2009)
 */
package jline.lib.butools.map

import jline.lib.butools.ph.meFromMoments
import jline.util.matrix.Matrix
import jline.util.matrix.MatrixCell

/**
 * Creates a continuous marked rational arrival process that
 * has the same marginal and lag-1 joint moments as given.
 *
 * @param moms The list of marginal moments. To obtain a continuous
 *             marked rational process of order M, 2*M-1 marginal
 *             moments are required.
 * @param Nm The list of lag-1 joint moment matrices. The length
 *           determines K, the number of arrival types.
 * @return The H0, H1, ..., HK matrices of the continuous marked rational process
 */
fun mrapFromMoments(moms: DoubleArray, Nm: MatrixCell): MatrixCell {
    val meResult = meFromMoments(moms)
    val v = meResult.alpha
    val H0 = meResult.A

    val N = H0.numRows

    // KEY CHANGE from DMRAP: H0i = (-H0)^{-1} instead of (I-H0)^{-1}
    val H0i = H0.neg().inv()
    val Ge = Matrix(N, N)
    val G1 = Matrix(N, N)

    var H0ip = Matrix.eye(N)
    for (i in 0 until N) {
        // Ge(i,:) = v * H0ip
        val row = v.mult(H0ip)
        for (j in 0 until N) {
            Ge[i, j] = row[0, j]
        }

        // G1(:,i) = sum(H0ip, 2) -- row sums of H0ip
        for (j in 0 until N) {
            var sum = 0.0
            for (k in 0 until N) {
                sum += H0ip[j, k]
            }
            G1[j, i] = sum
        }

        // H0ip = H0ip * (i+1) * H0i
        H0ip = H0ip.scale((i + 1).toDouble()).mult(H0i)
    }

    val Gei = Ge.inv()
    val G1i = G1.inv()

    val numTypes = Nm.size()
    val H = MatrixCell(numTypes + 1)
    H[0] = H0

    for (i in 0 until numTypes) {
        // H{i+1} = (-H0) * Gei * Nm{i} * G1i
        // Continuous case: use Nm directly (no factorial moment conversion)
        H[i + 1] = H0.neg().mult(Gei).mult(Nm[i]).mult(G1i)
    }

    return H
}

/**
 * Overload for Array<Matrix>.
 */
fun mrapFromMoments(moms: DoubleArray, Nm: Array<Matrix>): MatrixCell {
    val cell = MatrixCell(Nm.size)
    for (i in Nm.indices) {
        cell[i] = Nm[i]
    }
    return mrapFromMoments(moms, cell)
}

/**
 * Creates a rational arrival process that has the same
 * marginal and lag-1 joint moments as given.
 *
 * This is a convenience wrapper around mrapFromMoments for
 * single arrival type (K=1).
 *
 * @param moms The list of marginal moments
 * @param Nm The matrix of lag-1 joint moments (M x M)
 * @return Pair of (H0, H1) matrices of the rational process
 */
fun rapFromMoments(moms: DoubleArray, Nm: Matrix): Pair<Matrix, Matrix> {
    val result = mrapFromMoments(moms, arrayOf(Nm))
    return Pair(result[0], result[1])
}
