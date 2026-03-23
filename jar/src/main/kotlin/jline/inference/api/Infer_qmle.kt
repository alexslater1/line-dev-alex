/*
 * Copyright (c) 2012-2026, Imperial College London
 * All rights reserved.
 */
package jline.inference.api

import jline.util.matrix.Matrix

/**
 * Quick Maximum Likelihood Estimation closed-form formula.
 *
 * D(i,j) = Q(i,j) / (N(j) - sum(Q(:,j))) * Z(j) / (1 + sum(Q(i,:)) - Q(i,j)/N(j))
 *
 * @param Q mean queue lengths matrix (M x R)
 * @param N population vector (1 x R or length-R array)
 * @param Z think time vector (1 x R or length-R array)
 * @return demand estimates matrix (M x R)
 */
fun infer_qmle(Q: Matrix, N: DoubleArray, Z: DoubleArray): Matrix {
    val M = Q.numRows
    val R = Q.numCols
    val D = Matrix(M, R)

    for (i in 0 until M) {
        for (j in 0 until R) {
            // sum(Q(:,j))
            var sumQcolJ = 0.0
            for (ii in 0 until M) {
                sumQcolJ += Q.get(ii, j)
            }

            // sum(Q(i,:))
            var sumQrowI = 0.0
            for (jj in 0 until R) {
                sumQrowI += Q.get(i, jj)
            }

            val denom1 = N[j] - sumQcolJ
            val denom2 = 1.0 + sumQrowI - Q.get(i, j) / N[j]

            if (Math.abs(denom1) > 1e-14 && Math.abs(denom2) > 1e-14) {
                D.set(i, j, Q.get(i, j) / denom1 * Z[j] / denom2)
            }
        }
    }

    return D
}
