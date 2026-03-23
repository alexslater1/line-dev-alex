/*
 * Copyright (c) 2012-2026, QORE Lab, Imperial College London
 * All rights reserved.
 * Ported from BUTools V2.0
 */
package jline.lib.butools.mam

import jline.lib.butools.QBDFundamentalMatrices
import jline.lib.butools.mc.dtmcSolve
import jline.util.matrix.Matrix

/**
 * Returns the parameters of the matrix-geometrically distributed stationary
 * distribution of a QBD.
 *
 * Using vector pi0 and matrix R provided by this function, the stationary
 * solution can be obtained by: pi_k = pi_0 * R^k
 *
 * The implementation auto-detects whether the QBD is in continuous or discrete
 * time based on the diagonal entries of L0.
 *
 * @param B The matrix corresponding to backward transitions (N x N)
 * @param L The matrix corresponding to local transitions (N x N)
 * @param F The matrix corresponding to forward transitions (N x N)
 * @param L0 The matrix corresponding to local transitions at level zero (N x N)
 * @param prec The fundamental matrix R is computed up to this precision (default 1e-14)
 * @return Pair of (pi0, R) where pi0 is the stationary probability vector of level zero
 *         and R is the matrix parameter of the matrix geometrical distribution
 */
fun qbdSolve(B: Matrix, L: Matrix, F: Matrix, L0: Matrix, prec: Double = 1e-14): Pair<Matrix, Matrix> {
    val m = L0.numRows
    val I = Matrix.eye(m)

    // Get fundamental matrix R
    val fundMatrices = QBDFundamentalMatrices(B, L, F, prec, null, "R", null)
    val R = fundMatrices["R"]!!

    // Convert to discrete time problem, if needed (continuous time has negative diagonal)
    var Bdtmc = B
    var L0dtmc = L0
    var negDiagCount = 0
    for (i in 0 until m) {
        if (L0[i, i] < 0) negDiagCount++
    }
    if (negDiagCount > 0) {
        // Continuous time - uniformize
        var maxRate = 0.0
        for (i in 0 until m) {
            if (-L0[i, i] > maxRate) maxRate = -L0[i, i]
        }
        Bdtmc = B.scale(1.0 / maxRate)
        L0dtmc = L0.scale(1.0 / maxRate).add(I)
    }

    // pi0 = DTMCSolve(L0 + R*B) in discrete time
    val transMatrix = L0dtmc.add(1.0, R.mult(Bdtmc))
    var pi0 = dtmcSolve(transMatrix)

    // Normalize: pi0 * inv(I - R) * ones = 1
    val IminusR = I.sub(R)
    val IminusRinv = IminusR.inv()
    val ones = Matrix.ones(m, 1)
    val nr = pi0.mult(IminusRinv).mult(ones)[0, 0]
    if (nr > 0) {
        pi0 = pi0.scale(1.0 / nr)
    }

    return Pair(pi0, R)
}
