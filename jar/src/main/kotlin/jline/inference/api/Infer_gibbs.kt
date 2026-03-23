/*
 * Copyright (c) 2012-2026, Imperial College London
 * All rights reserved.
 */
package jline.inference.api

import jline.api.pfqn.mva.pfqn_bs
import jline.util.matrix.Matrix
import java.util.Random

/**
 * Gibbs Sampling demand estimation from trace data.
 *
 * Analyzes arrival/departure traces to reconstruct system state,
 * then uses Gibbs sampling to estimate service demands.
 *
 * @param data cell array: data[3][k] = arrival times (ms), data[4][k] = response times (s),
 *             data[6][k] = throughput per class
 * @param nbCores number of server cores
 * @param tol convergence tolerance
 * @return estimated demands (1 x R array)
 */
fun infer_gibbs(data: Array<Array<DoubleArray?>>, nbCores: Int, tol: Double = 1e-3): DoubleArray {
    val datNeeded = 200000
    val likelihoodSample = 5000
    val nbSamples = 2000

    val nbClasses = data[0].size - 1
    val nbNodes = 2
    val nbJobs = IntArray(nbClasses)

    // Analyze data to get state probabilities
    val analysisResult = analyseData(data, nbJobs, nbClasses, nbNodes, datNeeded)
    val prob = analysisResult.prob
    val N = analysisResult.N
    val N0 = analysisResult.N0

    // Compute used cores
    var usedCores = 0.0
    for (k in 0 until prob.numRows) {
        var queueJobs = 0.0
        for (c in nbClasses until 2 * nbClasses) {
            queueJobs += prob.get(k, c)
        }
        val lastCol = prob.numCols - 1
        val probVal = prob.get(k, lastCol)
        usedCores += if (queueJobs > nbCores) {
            nbCores * probVal
        } else {
            queueJobs * probVal
        }
    }
    val lastRow = prob.numRows - 1
    usedCores /= (1.0 - prob.get(lastRow, prob.numCols - 1))

    // Think times
    val thinkTime = DoubleArray(nbClasses) { k ->
        val tput = data[6][k]
        if (tput != null && tput.isNotEmpty()) {
            (N[k] - N0[k]) / tput.average()
        } else {
            1.0
        }
    }

    val rangeSize = DoubleArray(nbClasses * (nbNodes - 1)) { 1.0 }

    // Build test set from probabilities
    val cumProb = DoubleArray(prob.numRows)
    cumProb[0] = prob.get(0, prob.numCols - 1)
    for (k in 1 until prob.numRows) {
        cumProb[k] = cumProb[k - 1] + prob.get(k, prob.numCols - 1)
    }

    val rng = Random()
    val testset = Array(likelihoodSample) { DoubleArray(nbClasses * nbNodes) }
    for (k in 0 until likelihoodSample) {
        val uniValue = rng.nextDouble()
        val index = cumProb.indexOfFirst { it > uniValue }.let { if (it < 0) cumProb.size - 1 else it }
        for (c in 0 until nbClasses * nbNodes) {
            testset[k][c] = prob.get(index, c)
        }
    }

    // Log factorial table
    val maxN = N.sum() + 1
    val LV = DoubleArray(maxN + 1)
    LV[0] = 0.0
    if (maxN >= 1) LV[1] = 0.0
    for (k in 2..maxN) {
        LV[k] = LV[k - 1] + Math.log((k - 1).toDouble())
    }

    // Compute sumA
    var sumA = 0.0
    for (row in testset) {
        for (c in 0 until nbClasses * nbNodes) {
            val idx = row[c].toInt()
            if (idx >= 0 && idx + 1 < LV.size) {
                sumA += LV[idx + 1]
            }
        }
    }

    // Initial log normalizing constant
    var logGInitial = 0.0
    for (k in 0 until nbClasses) {
        logGInitial += N[k] * Math.log(thinkTime[k] + 1e-15)
        for (j in 1..N[k]) {
            logGInitial -= Math.log(j.toDouble())
        }
    }

    // Gibbs sampling
    val smpl = Array(nbSamples) { DoubleArray(nbClasses * (nbNodes - 1)) }
    var sampleIndex = 0

    var demandOld: DoubleArray? = null

    for (k in 0 until nbSamples / 50) {
        for (s in 0 until 50) {
            if (sampleIndex >= nbSamples) break
            for (h in 0 until nbClasses * (nbNodes - 1)) {
                val theta = if (sampleIndex == 0) {
                    val t = DoubleArray(nbClasses * (nbNodes - 1))
                    for (i in 0 until h) t[i] = smpl[0][i]
                    t
                } else {
                    val t = DoubleArray(nbClasses * (nbNodes - 1))
                    for (i in 0 until h) t[i] = smpl[sampleIndex][i]
                    for (i in h until t.size) t[i] = smpl[sampleIndex - 1][i]
                    t
                }

                val result = gibbsSamplerSimple(
                    thinkTime, theta, testset, h, nbNodes, nbClasses,
                    N, logGInitial, tol, rangeSize[h], LV, sumA, rng
                )
                smpl[sampleIndex][h] = result.value
                logGInitial = result.logGCurrent
                rangeSize[h] = result.rangeSizeDim * 2.0
            }
            sampleIndex++
        }

        if (k == 1) {
            demandOld = DoubleArray(nbClasses * (nbNodes - 1)) { dim ->
                var sum = 0.0
                for (si in 50 until sampleIndex) sum += smpl[si][dim]
                sum / (sampleIndex - 50)
            }
        } else if (k > 1 && demandOld != null) {
            val demandNow = DoubleArray(nbClasses * (nbNodes - 1)) { dim ->
                var sum = 0.0
                for (si in k * 50 until sampleIndex) sum += smpl[si][dim]
                sum / (sampleIndex - k * 50)
            }
            for (dim in demandNow.indices) {
                demandNow[dim] = demandNow[dim] / (k + 1) + demandOld[dim] / (k + 1) * k
            }

            var relChange = 0.0
            for (dim in demandNow.indices) {
                if (Math.abs(demandOld[dim]) > 1e-15) {
                    relChange += Math.abs((demandNow[dim] - demandOld[dim]) / demandOld[dim])
                }
            }
            relChange /= demandNow.size

            if (relChange < tol) {
                val cutoff = sampleIndex / 2
                return DoubleArray(nbClasses * (nbNodes - 1)) { dim ->
                    var sum = 0.0
                    for (si in cutoff until sampleIndex) sum += smpl[si][dim]
                    sum / (sampleIndex - cutoff) * usedCores
                }
            }
            demandOld = demandNow
        }
    }

    val cutoff = sampleIndex / 2
    return DoubleArray(nbClasses * (nbNodes - 1)) { dim ->
        var sum = 0.0
        for (si in cutoff until sampleIndex) sum += smpl[si][dim]
        sum / (sampleIndex - cutoff) * usedCores
    }
}

private data class AnalysisResult(
    val prob: Matrix,
    val N: IntArray,
    val N0: DoubleArray
)

private fun analyseData(
    data: Array<Array<DoubleArray?>>,
    nbJobs: IntArray,
    nbClasses: Int,
    nbNodes: Int,
    datNeeded: Int
): AnalysisResult {
    val K = nbClasses
    val N = nbJobs.copyOf()
    val N0 = DoubleArray(K)

    // Build event timeline
    val tempTS = ArrayList<Double>()
    val tempClass = ArrayList<Int>()
    val tempLogger = ArrayList<Int>()

    for (i in 0 until K) {
        val arvTimes = data[3][i] ?: continue
        val respTimes = data[4][i] ?: continue
        val tempLen = arvTimes.size
        for (j in 0 until tempLen) {
            tempTS.add(arvTimes[j])
            tempClass.add(i)
            tempLogger.add(1)
            tempTS.add(arvTimes[j] + respTimes[j] * 1000)
            tempClass.add(i)
            tempLogger.add(2)
        }
    }

    // Sort by timestamp
    val indices = (0 until tempTS.size).sortedBy { tempTS[it] }.toIntArray()
    val ts = DoubleArray(indices.size) { tempTS[indices[it]] }
    val classId = IntArray(indices.size) { tempClass[indices[it]] }
    val loggerId = IntArray(indices.size) { tempLogger[indices[it]] }

    val burnin = Math.max(0, ts.size - datNeeded)
    val totalLength = ts.size

    // Count customers at each node over time
    val count = Array(totalLength) { Array(K) { IntArray(nbNodes) } }
    for (k in 0 until K) count[0][k][0] = N[k]

    for (i in 0 until totalLength - 1) {
        for (k in 0 until K) {
            for (nd in 0 until nbNodes) {
                count[i + 1][k][nd] = count[i][k][nd]
            }
        }
        val cls = classId[i]
        val log = loggerId[i] - 1 // convert to 0-based

        count[i + 1][cls][log]--
        if (log == nbNodes - 1) {
            count[i + 1][cls][0]++
        } else {
            count[i + 1][cls][log + 1]++
        }
    }

    // Infer population if not given
    if (N.sum() == 0) {
        for (k in 0 until K) {
            var maxCount = 0
            for (i in 0 until totalLength) {
                for (nd in 0 until nbNodes) {
                    maxCount = Math.max(maxCount, count[i][k][nd])
                }
            }
            N[k] = maxCount
        }
    }

    // Add population to delay station counts
    for (i in 0 until totalLength) {
        for (k in 0 until K) {
            count[i][k][0] += N[k]
        }
    }

    // Flatten count and compute time intervals
    val flatSize = K * nbNodes
    val flatCount = Array(totalLength) { i ->
        val flat = DoubleArray(flatSize)
        for (k in 0 until K) {
            for (nd in 0 until nbNodes) {
                flat[k + nd * K] = count[i][k][nd].toDouble()
            }
        }
        flat
    }

    val timeInterval = DoubleArray(totalLength)
    timeInterval[0] = 0.0
    for (i in 1 until totalLength) {
        timeInterval[i] = ts[i] - ts[i - 1]
    }

    // Compute state probabilities
    val stateMap = LinkedHashMap<String, DoubleArray>()
    for (i in burnin until totalLength) {
        val key = flatCount[i].joinToString(",")
        val entry = stateMap.getOrPut(key) { DoubleArray(flatSize + 1) }
        for (j in 0 until flatSize) entry[j] = flatCount[i][j]
        entry[flatSize] += timeInterval[i]
    }

    // Sort states lexicographically (matching MATLAB sortrows behavior)
    val sortedEntries = stateMap.values.sortedWith(compareBy<DoubleArray> { it[0] }.let { cmp ->
        var c = cmp
        for (j in 1 until flatSize) {
            c = c.thenBy { it[j] }
        }
        c
    })

    val obsLength = ts.last() - ts[burnin]
    val probMatrix = Matrix(sortedEntries.size, flatSize + 1)
    var row = 0
    for (entry in sortedEntries) {
        for (j in 0 until flatSize) {
            probMatrix.set(row, j, entry[j])
        }
        probMatrix.set(row, flatSize, entry[flatSize] / obsLength)
        row++
    }

    // Compute N0
    for (k in 0 until K) {
        for (r in 0 until probMatrix.numRows) {
            N0[k] += probMatrix.get(r, probMatrix.numCols - 1) * probMatrix.get(r, K + k)
        }
    }

    return AnalysisResult(probMatrix, N, N0)
}

private data class GibbsResult(val value: Double, val logGCurrent: Double, val rangeSizeDim: Double)

private fun gibbsSamplerSimple(
    thinkTime: DoubleArray,
    theta: DoubleArray,
    testset: Array<DoubleArray>,
    index: Int,
    nbNodes: Int,
    nbClasses: Int,
    nbJobs: IntArray,
    logGInitial: Double,
    interval: Double,
    rangeSize: Double,
    LV: DoubleArray,
    sumA: Double,
    rng: Random
): GibbsResult {
    val range = generateSequence(0.0) { it + interval }.takeWhile { it <= rangeSize }.toList().toDoubleArray()
    val rangeLen = range.size

    // Build demand matrix x
    val x = Array(nbNodes) { DoubleArray(nbClasses) }
    x[0] = thinkTime.copyOf()
    for (i in 0 until nbNodes - 1) {
        for (c in 0 until nbClasses) {
            x[i + 1][c] = theta[i * nbClasses + c]
        }
    }

    val indexI = index / nbClasses + 1 // 1-based station
    val indexJ = index % nbClasses

    val logG = DoubleArray(rangeLen)

    // Compute queue lengths using pfqn_bs
    val demandMatrix = Matrix(nbNodes - 1, nbClasses)
    for (i in 0 until nbNodes - 1) {
        for (c in 0 until nbClasses) {
            demandMatrix.set(i, c, x[i + 1][c])
        }
    }
    val popMatrix = Matrix(1, nbClasses)
    for (c in 0 until nbClasses) popMatrix.set(0, c, nbJobs[c].toDouble())
    val thinkMatrix = Matrix(1, nbClasses)
    for (c in 0 until nbClasses) thinkMatrix.set(0, c, thinkTime[c])

    // Find index of current theta value in range
    var indexPrevious = 0
    var minDist = Double.MAX_VALUE
    for (i in range.indices) {
        val dist = Math.abs(range[i] - theta[index])
        if (dist < minDist) {
            minDist = dist
            indexPrevious = i
        }
    }
    logG[indexPrevious] = logGInitial

    // Forward/backward integration of log normalizing constant
    val bsResult = pfqn_bs(demandMatrix, popMatrix, thinkMatrix)
    var QN = bsResult.Q

    for (i in indexPrevious - 1 downTo 0) {
        x[indexI][indexJ] = range[i + 1]
        demandMatrix.set(indexI - 1, indexJ, x[indexI][indexJ])
        val result = pfqn_bs(demandMatrix, popMatrix, thinkMatrix, interval, 1000, QN)
        QN = result.Q
        val qnVal = QN.get(indexI - 1, indexJ)
        val ratio = 1.0 + qnVal / (range[i + 1] + 1e-15) * (-interval)
        logG[i] = if (ratio > 0) logG[i + 1] + Math.log(ratio) else logG[i + 1]
    }

    x[indexI][indexJ] = theta[index]
    demandMatrix.set(indexI - 1, indexJ, x[indexI][indexJ])
    val result2 = pfqn_bs(demandMatrix, popMatrix, thinkMatrix)
    QN = result2.Q

    for (i in indexPrevious + 1 until rangeLen) {
        x[indexI][indexJ] = range[i - 1]
        demandMatrix.set(indexI - 1, indexJ, x[indexI][indexJ])
        val result = pfqn_bs(demandMatrix, popMatrix, thinkMatrix, interval, 1000, QN)
        QN = result.Q
        val qnVal = QN.get(indexI - 1, indexJ)
        val ratio = 1.0 + qnVal / (range[i - 1] + 1e-15) * interval
        logG[i] = if (ratio > 0) logG[i - 1] + Math.log(ratio) else logG[i - 1]
    }

    // Compute log probabilities
    val logProb = DoubleArray(rangeLen) { i ->
        var testSum = 0.0
        for (row in testset) {
            testSum += row[index + nbClasses]
        }
        testSum * Math.log(range[i] + 1e-15) - logG[i] * testset.size
    }

    val maxLogProb = logProb.max()
    val prob = DoubleArray(rangeLen) { Math.exp(logProb[it] - maxLogProb) }
    val probSum = prob.sum()
    for (i in prob.indices) prob[i] /= probSum

    val cumProb = DoubleArray(rangeLen)
    cumProb[0] = prob[0]
    for (i in 1 until rangeLen) cumProb[i] = cumProb[i - 1] + prob[i]

    val rangeSizeDimIdx = cumProb.indexOfFirst { it > 1.0 - 1e-10 }.let { if (it < 0) rangeLen - 1 else it }
    val rangeSizeDim = range[rangeSizeDimIdx] * 2.0

    val randVar = rng.nextDouble()
    val indexProb = cumProb.indexOfFirst { it > randVar }

    return if (indexProb < 0) {
        GibbsResult(theta[index], logGInitial, rangeSizeDim)
    } else {
        GibbsResult(range[indexProb], logG[indexProb], rangeSizeDim)
    }
}
