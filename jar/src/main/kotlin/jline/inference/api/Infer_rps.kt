/*
 * Copyright (c) 2012-2026, Imperial College London
 * All rights reserved.
 */
package jline.inference.api

import jline.inference.util.NnlsSolver
import jline.util.matrix.Matrix

/**
 * Regression for Processor Sharing (RPS) demand estimation.
 *
 * Based on mean-value analysis for PS stations:
 *   E[R_r] = E[D_r] * E[Q_bar_A] / V
 *
 * where Q_bar_A is the total number of jobs seen upon admission
 * (including the arriving job) and V is the number of servers.
 *
 * @param rt response time samples (column vector)
 * @param classVec class of each request sample (0-based)
 * @param ql queue length samples (n x R matrix)
 * @param V number of cores/servers
 * @return estimated demands (1 x R array)
 */
fun infer_rps(rt: DoubleArray, classVec: IntArray, ql: Matrix, V: Int): DoubleArray {
    val R = ql.numCols
    val demandEst = DoubleArray(R)

    for (r in 0 until R) {
        val indices = (0 until classVec.size).filter { classVec[it] == r }
        if (indices.isEmpty()) continue

        val respTimes = DoubleArray(indices.size) { rt[indices[it]] }
        val qBarA = DoubleArray(indices.size) { i ->
            val idx = indices[i]
            var totalQL = 0.0
            for (c in 0 until R) {
                totalQL += ql.get(idx, c)
            }
            (totalQL + 1.0) / V
        }

        // NNLS: min ||qBarA * d - respTimes||^2 s.t. d >= 0
        val A = Array(respTimes.size) { doubleArrayOf(qBarA[it]) }
        val result = NnlsSolver.lsqnonneg(A, respTimes)
        demandEst[r] = result[0]
    }

    return demandEst
}
