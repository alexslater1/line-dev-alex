/*
 * Copyright (c) 2012-2026, Imperial College London
 * All rights reserved.
 */
package jline.inference.api

import jline.lang.Network
import jline.lang.nodes.Queue
import jline.util.matrix.Matrix

/**
 * Setup function for MINPS with legacy cell-based data format.
 *
 * Processes legacy cell data format, computes queue lengths at arrival,
 * estimates think-time rates and effective population, then calls
 * [infer_minps] with processed data.
 *
 * @param data cell array in standard format (7 x K+1)
 * @param initSample starting sample index (0-based)
 * @param sampleSize number of samples to use (0 = all)
 * @param V number of servers
 * @param model LINE Network model
 * @param node PS queue node
 * @return estimated demands (1 x R array)
 */
fun infer_minps_setup(
    data: Array<Array<DoubleArray?>>,
    initSample: Int,
    sampleSize: Int,
    V: Int,
    model: Network,
    node: Queue
): DoubleArray {
    var R = data[0].size - 1

    // Count samples per class
    val sampleNumber = IntArray(R) { k ->
        data[3][k]?.size ?: 0
    }

    // Remove classes without samples
    val newR = sampleNumber.count { it > 0 }
    val data2 = Array(7) { arrayOfNulls<DoubleArray>(newR + 1) }
    var rIdx = 0
    for (k in 0..R) {
        if (k < R && sampleNumber[k] > 0 || k == R) {
            for (j in 0 until 7) {
                data2[j][rIdx] = data[j][k]
            }
            rIdx++
        }
    }
    R = newR

    // Get queue length at arrival times
    val qls = infer_get_qlen_arrival(data2, R)

    // Collect all data
    val rtList = ArrayList<Double>()
    val classList = ArrayList<Int>()
    val qlList = ArrayList<DoubleArray>()
    val atList = ArrayList<Double>()

    for (k in 0 until R) {
        val respTimes = data2[4][k] ?: continue
        val arvTimes = data2[3][k] ?: continue
        val qlk = qls[k]

        for (i in respTimes.indices) {
            rtList.add(respTimes[i])
            classList.add(k)
            val qlRow = DoubleArray(R) { r -> qlk.get(i, r) }
            qlList.add(qlRow)
            atList.add(arvTimes[i] / 1000.0) // ms to seconds
        }
    }

    // Sort by arrival time
    val indices = (0 until atList.size).sortedBy { atList[it] }.toIntArray()
    val at = DoubleArray(indices.size) { atList[indices[it]] }
    val rt = DoubleArray(indices.size) { rtList[indices[it]] }
    val classArr = IntArray(indices.size) { classList[indices[it]] }
    val qlMatrix = Matrix(indices.size, R)
    for (i in indices.indices) {
        for (r in 0 until R) {
            qlMatrix.set(i, r, qlList[indices[i]][r])
        }
    }

    // Select sample set
    val actualSampleSize = if (sampleSize == 0) qlMatrix.numRows else sampleSize
    val firstSample = initSample
    val finalSample = Math.min(firstSample + actualSampleSize, qlMatrix.numRows)

    val rtExp = DoubleArray(finalSample - firstSample) { rt[firstSample + it] }
    val classExp = IntArray(finalSample - firstSample) { classArr[firstSample + it] }
    val qlExp = Matrix(finalSample - firstSample, R)
    for (i in 0 until finalSample - firstSample) {
        for (r in 0 until R) {
            qlExp.set(i, r, qlMatrix.get(firstSample + i, r))
        }
    }

    // Remove zero response times
    val valid = (0 until rtExp.size).filter { rtExp[it] > 0 }
    val rtValid = valid.map { rtExp[it] }.toDoubleArray()
    val classValid = valid.map { classExp[it] }.toIntArray()
    val qlValid = Matrix(valid.size, R)
    for (i in valid.indices) {
        for (r in 0 until R) {
            qlValid.set(i, r, qlExp.get(valid[i], r))
        }
    }

    return infer_minps(model, node, rtValid, classValid, qlValid)
}
