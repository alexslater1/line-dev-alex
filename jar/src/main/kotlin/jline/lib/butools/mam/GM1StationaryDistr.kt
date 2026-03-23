/*
 * Copyright (c) 2012-2026, QORE Lab, Imperial College London
 * All rights reserved.
 * Ported from BUTools V2.0
 */
package jline.lib.butools.mam

import jline.lib.butools.mc.dtmcSolve
import jline.util.matrix.Matrix

/**
 * Returns the stationary distribution of the G/M/1 type Markov chain
 * up to a given level K.
 *
 * @param B List of matrix blocks of the G/M/1 type generator at the boundary, from 0 to MB-1.
 *          Each matrix has shape (N, N).
 * @param R Matrix R of the G/M/1 type Markov chain (N x N), as returned by gm1FundamentalMatrix.
 * @param K The stationary distribution is returned up to this level.
 * @return The stationary probability vector up to level K, shape (1, (K+1)*N)
 */
fun gm1StationaryDistr(B: List<Matrix>, R: Matrix, K: Int): Matrix {
    val m = R.numRows
    val I = Matrix.eye(m)

    // Check that spectral radius of R is below 1
    val IminusRinv = I.sub(R).inv()

    // Concatenate B matrices vertically
    val maxb = B.size

    // Compute BR = B[0] + R*B[1] + R^2*B[2] + ... = sum_{i} R^i * B[i]
    // Python: BR = B[(maxb-1)*m:,:]; for i in range(maxb-1,0,-1): BR = R*BR + B[(i-1)*m:i*m,:]
    // This uses Horner's method in reverse
    var BR = B[maxb - 1].copy()
    for (i in maxb - 1 downTo 1) {
        BR = R.mult(BR).add(1.0, B[i - 1])
    }

    // Solve pi0 as the stationary vector of BR
    var pix = dtmcSolve(BR)

    // Normalize: pi0 * inv(I - R) * ones = 1
    val ones = Matrix.ones(m, 1)
    val nr = pix.mult(IminusRinv).mult(ones)[0, 0]
    if (nr > 0) {
        pix = pix.scale(1.0 / nr)
    }

    // Build the distribution up to level K
    val pi = Matrix.zeros(1, (K + 1) * m)
    for (j in 0 until m) {
        pi[0, j] = pix[0, j]
    }

    var sumpi = pix.elementSum()
    var numit = 1
    while (sumpi < 1.0 - 1e-10 && numit <= K) {
        pix = pix.mult(R)
        sumpi += pix.elementSum()
        for (j in 0 until m) {
            pi[0, numit * m + j] = pix[0, j]
        }
        numit++
    }

    return pi
}

/**
 * Overload accepting Array<Matrix>.
 */
fun gm1StationaryDistr(B: Array<Matrix>, R: Matrix, K: Int): Matrix {
    return gm1StationaryDistr(B.toList(), R, K)
}
