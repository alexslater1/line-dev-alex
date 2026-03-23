/**
 * @file MAP/D/1 queueing system analysis
 *
 * Implements analysis of MAP/D/1 queues as a convenience wrapper
 * for [qsys_mapdc] with a single server (c=1).
 *
 * The queue has a Markovian Arrival Process (MAP) for arrivals,
 * deterministic service times, and a single server.
 *
 * @see qsys_mapdc
 * @since LINE 3.1.0
 */
package jline.api.qsys

import jline.util.matrix.Matrix
import jline.util.matrix.MatrixCell

/**
 * Analyzes a MAP/D/1 queue (single server with deterministic service).
 *
 * This is a convenience wrapper for [qsys_mapdc] with c=1.
 *
 * @param D0 MAP hidden transition matrix (n x n)
 * @param D1 MAP arrival transition matrix (n x n)
 * @param s Deterministic service time (positive scalar)
 * @param maxNumComp Maximum number of queue length components (default 1000)
 * @param numSteps Number of waiting time distribution points per service interval (default 1)
 * @return [QsysMapDcResult] with performance metrics
 * @see qsys_mapdc
 */
@JvmOverloads
fun qsys_mapd1(
    D0: Matrix,
    D1: Matrix,
    s: Double,
    maxNumComp: Int = 1000,
    numSteps: Int = 1
): QsysMapDcResult {
    return qsys_mapdc(D0, D1, s, 1, maxNumComp, numSteps)
}

/**
 * Simplified MAP/D/1 analysis using MatrixCell input for arrival.
 *
 * @param arrival MAP arrival process as MatrixCell [D0, D1]
 * @param s Deterministic service time
 * @return [QsysMapDcResult] with performance metrics
 * @see qsys_mapdc
 */
fun qsys_mapd1(arrival: MatrixCell, s: Double): QsysMapDcResult {
    require(arrival.size() >= 2) { "Arrival MAP must have at least 2 matrices [D0, D1]" }
    return qsys_mapdc(arrival[0], arrival[1], s, 1)
}

/**
 * Queueing system mapd1 algorithms
 */
@Suppress("unused")
class QsysMapd1Algo {
    companion object {
        // Class documentation marker for Dokka
    }
}
