/*
 * Copyright (c) 2012-2026, Imperial College London
 * All rights reserved.
 */
package jline.inference.api

import jline.util.matrix.Matrix

/**
 * Compute queue lengths at arrival from legacy cell data format.
 *
 * Wrapper around [infer_compute_ql_at_arrival] for the legacy cell-based
 * data format. data[3][k] contains arrival times (in ms) and data[4][k]
 * contains response times for class k.
 *
 * @param data cell array in standard format: data[row][class],
 *             data[3][k] = arrival times (ms), data[4][k] = response times (s)
 * @param K number of classes
 * @return list of K matrices, each numSamples(k) x K
 */
fun infer_get_qlen_arrival(data: Array<Array<DoubleArray?>>, K: Int): List<Matrix> {
    val atList = ArrayList<Double>()
    val rtList = ArrayList<Double>()
    val classList = ArrayList<Int>()
    val numObs = IntArray(K)

    for (k in 0 until K) {
        val arvTimes = data[3][k] ?: continue
        val respTimes = data[4][k] ?: continue
        numObs[k] = arvTimes.size
        for (i in arvTimes.indices) {
            atList.add(arvTimes[i] / 1000.0) // convert ms to secs
            rtList.add(respTimes[i])
            classList.add(k)
        }
    }

    val at = atList.toDoubleArray()
    val rt = rtList.toDoubleArray()
    val classVec = classList.toIntArray()
    val n = at.size
    val jobid = IntArray(n) { it }

    val qlUnsorted = infer_compute_ql_at_arrival(at, jobid, rt, jobid, classVec, K)

    // Split into per-class matrices in original order
    val ql = ArrayList<Matrix>(K)
    var counter = 0
    for (k in 0 until K) {
        val mat = Matrix(numObs[k], K)
        for (i in 0 until numObs[k]) {
            for (r in 0 until K) {
                mat.set(i, r, qlUnsorted.get(counter + i, r))
            }
        }
        ql.add(mat)
        counter += numObs[k]
    }

    return ql
}
