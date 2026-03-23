/*
 * Copyright (c) 2012-2026, QORE Lab, Imperial College London
 * All rights reserved.
 * Ported from BUTools V2.0
 *
 * Reference:
 * Mitchell, Kenneth, and Appie van de Liefvoort.
 * "Approximation models of feed-forward G/G/1/N queueing networks
 * with correlated arrivals." Performance Evaluation 51.2 (2003): 137-152.
 */
package jline.lib.butools.map

import jline.lib.butools.momsFromReducedMoms
import jline.lib.butools.reducedMomsFromMoms
import jline.lib.butools.ph.meFromMoments
import jline.util.matrix.Matrix

/**
 * Returns a rational arrival process that has the same moments
 * and lag autocorrelation coefficients as given.
 *
 * @param moms The vector of marginal moments. To obtain a RAP of
 *             size M, 2*M-1 moments are required.
 * @param corr The vector of lag autocorrelation coefficients. To
 *             obtain a RAP of size M, 2*M-3 coefficients are needed.
 * @return Pair of (H0, H1) matrices of the rational arrival process
 */
fun rapFromMomentsAndCorrelations(moms: DoubleArray, corr: DoubleArray): Pair<Matrix, Matrix> {
    val meResult = meFromMoments(moms)
    val alpha = meResult.alpha
    val D0 = meResult.A

    val M = alpha.numCols

    if (corr.size < 2 * M - 3) {
        throw IllegalArgumentException(
            "RAPFromMomentsAndCorrelations: The number of correlations given is less than the required 2n-3!"
        )
    }

    // rcorr = corr[0:2*M-3] / ((moms[1]/2 - moms[0]^2) / (moms[1] - moms[0]^2))
    val corrFactor = (moms[1] / 2.0 - moms[0] * moms[0]) / (moms[1] - moms[0] * moms[0])
    val numCorr = 2 * M - 3
    val rcorrArray = DoubleArray(numCorr)
    for (i in 0 until numCorr) {
        rcorrArray[i] = corr[i] / corrFactor
    }

    // rcorr = MomsFromReducedMoms(rcorr) -- treat as reduced moments, convert to raw moments
    val rcorrMatrix = Matrix(1, numCorr)
    for (i in 0 until numCorr) {
        rcorrMatrix[0, i] = rcorrArray[i]
    }
    val rcorrMoms = momsFromReducedMoms(rcorrMatrix)
    val rcorrMomsArray = DoubleArray(numCorr)
    for (i in 0 until numCorr) {
        rcorrMomsArray[i] = rcorrMoms[0, i]
    }

    // (tmp, X) = MEFromMoments(rcorrMoms)
    val meResult2 = meFromMoments(rcorrMomsArray)
    val X = meResult2.A

    val NN = X.numRows

    if (NN + 1 != D0.numRows) {
        throw IllegalArgumentException(
            "RAPFromMomentsAndCorrelations: Correlation order is different from ME order!"
        )
    }

    // Build T1: lower triangular with ones (N x N)
    val T1 = Matrix.zeros(NN, NN)
    for (i in 0 until NN) {
        for (j in 0..i) {
            T1[i, j] = 1.0
        }
    }

    // Build U1: upper triangular with 1/(N-i) (N x N)
    val U1 = Matrix.zeros(NN, NN)
    for (i in 0 until NN) {
        for (j in i until NN) {
            U1[i, j] = 1.0 / (NN - i)
        }
    }

    // Build T2: lower triangular with ones (M x M)
    val T2 = Matrix.zeros(M, M)
    for (i in 0 until M) {
        for (j in 0..i) {
            T2[i, j] = 1.0
        }
    }

    // Build U2: upper triangular with 1/(M-i) (M x M)
    val U2 = Matrix.zeros(M, M)
    for (i in 0 until M) {
        for (j in i until M) {
            U2[i, j] = 1.0 / (M - i)
        }
    }

    // Y = -T1^{-1} * U1 * X^{-1} * U1^{-1} * T1
    val Y = T1.inv().neg().mult(U1).mult(X.inv()).mult(U1.inv()).mult(T1)

    // II = eye(N+1); II(2:end,2:end) = Y; Y = II
    val II = Matrix.eye(NN + 1)
    for (i in 0 until NN) {
        for (j in 0 until NN) {
            II[i + 1, j + 1] = Y[i, j]
        }
    }

    // D1 = -D0 * U2^{-1} * T2 * II * T2^{-1} * U2
    val D1 = D0.neg().mult(U2.inv()).mult(T2).mult(II).mult(T2.inv()).mult(U2)

    return Pair(D0, D1)
}
