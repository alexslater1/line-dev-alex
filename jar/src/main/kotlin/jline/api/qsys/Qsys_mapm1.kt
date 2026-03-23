/**
 * @file MAP/M/1 queueing system analysis
 *
 * Implements analysis of MAP/M/1 queues as a convenience wrapper
 * for [qsys_mapmc] with a single server (c=1).
 *
 * The queue has a Markovian Arrival Process (MAP) for arrivals,
 * exponential service times, and a single server.
 *
 * @see qsys_mapmc
 * @since LINE 3.1.0
 */
package jline.api.qsys

import jline.util.matrix.Matrix
import jline.util.matrix.MatrixCell

/**
 * Analyzes a MAP/M/1 queue (single server with exponential service).
 *
 * This is a convenience wrapper for [qsys_mapmc] with c=1,
 * matching the MATLAB implementation which uses Q-MAM for all
 * MAP/M/c queues including c=1.
 *
 * @param D0 MAP hidden transition matrix (n x n)
 * @param D1 MAP arrival transition matrix (n x n)
 * @param mu Exponential service rate
 * @param maxNumComp Maximum number of queue length probabilities (default 1000)
 * @return [QsysMapPhResult] with performance metrics and analyzer set to "Q-MAM:MAP/M/1"
 * @see qsys_mapmc
 */
@JvmOverloads
fun qsys_mapm1(
    D0: Matrix,
    D1: Matrix,
    mu: Double,
    maxNumComp: Int = 1000
): QsysMapPhResult {
    val result = qsys_mapmc(D0, D1, mu, 1, "SylvesCR", maxNumComp)
    return QsysMapPhResult(
        meanQueueLength = result.meanQueueLength,
        meanWaitingTime = result.meanWaitingTime,
        meanSojournTime = result.meanSojournTime,
        utilization = result.utilization,
        queueLengthDist = result.queueLengthDist,
        queueLengthMoments = result.queueLengthMoments,
        sojournTimeMoments = result.sojournTimeMoments,
        analyzer = "Q-MAM:MAP/M/1"
    )
}

/**
 * Simplified MAP/M/1 analysis using MatrixCell input for arrival.
 *
 * @param arrival MAP arrival process as MatrixCell [D0, D1]
 * @param mu Exponential service rate
 * @return [QsysMapPhResult] with performance metrics
 * @see qsys_mapmc
 */
fun qsys_mapm1(arrival: MatrixCell, mu: Double): QsysMapPhResult {
    require(arrival.size() >= 2) { "Arrival MAP must have at least 2 matrices [D0, D1]" }
    return qsys_mapm1(arrival[0], arrival[1], mu)
}

/**
 * Queueing system mapm1 algorithms
 */
@Suppress("unused")
class QsysMapm1Algo {
    companion object {
        // Class documentation marker for Dokka
    }
}
