/**
 * @file M/GI/1 preemptive LCFS Age of Information analysis
 *
 * Computes mean AoI, LST, and peak AoI for an M/GI/1 queue with
 * preemptive Last-Come First-Served (LCFS-PR) discipline.
 *
 * In LCFS-PR, when a new update arrives, it preempts the current update
 * in service (if any). This ensures the freshest update is always served.
 *
 * @since LINE 3.0
 */
package jline.api.aoi

/**
 * Mean AoI and LST for M/GI/1 preemptive LCFS queue.
 *
 * For preemptive LCFS, the AoI simplifies significantly:
 *   E[A] = E[Y] + E[H] = 1/lambda + E[H]
 *   E[Apeak] = E[Y] + E[H]
 *
 *   LST of AoI: A*(s) = (lambda/(s+lambda)) * H*(s)
 *   This is the convolution of exponential interarrival and service time.
 *
 * @param lambda Arrival rate (Poisson arrivals), must be positive
 * @param H_lst LST of service time distribution
 * @param E_H Mean service time (first moment), must be positive
 * @param E_H2 Second moment of service time (for reference)
 * @return AoiLstResult containing meanAoI, lstAoI, peakAoI
 * @throws IllegalArgumentException if parameters are invalid or system is unstable
 */
fun aoi_lcfspr_mgi1(lambda: Double, H_lst: LstFunction, E_H: Double, E_H2: Double): AoiLstResult {
    require(lambda > 0) { "Arrival rate lambda must be positive" }
    require(E_H > 0) { "Mean service time E_H must be positive" }

    val rho = lambda * E_H
    require(rho < 1) { "System unstable: rho = lambda*E_H = ${"%.4f".format(rho)} >= 1" }

    // Mean interarrival time
    val E_Y = 1.0 / lambda

    // Mean AoI for preemptive LCFS (Proposition 3)
    val meanAoI = E_Y + E_H

    // Mean Peak AoI (same as mean AoI for preemptive LCFS)
    val peakAoI = E_Y + E_H

    // LST of AoI for preemptive LCFS
    // A*(s) = (lambda / (s + lambda)) * H*(s)
    val lstAoI = object : LstFunction {
        override fun evaluate(s: Double): Double {
            return (lambda / (s + lambda)) * H_lst.evaluate(s)
        }
    }

    return AoiLstResult(meanAoI, lstAoI, peakAoI)
}

/**
 * Age of Information LCFS-PR M/GI/1 algorithms
 */
@Suppress("unused")
class AoiLcfsprMgi1Algo {
    companion object {
        // Class documentation marker for Dokka
    }
}
