/**
 * @file GI/M/1 preemptive LCFS Age of Information analysis
 *
 * Computes mean AoI, LST, and peak AoI for a GI/M/1 queue with
 * preemptive Last-Come First-Served (LCFS-PR) discipline.
 *
 * In LCFS-PR, when a new update arrives, it preempts the current update
 * in service (if any). This ensures the freshest update is always served.
 *
 * @since LINE 3.0
 */
package jline.api.aoi

/**
 * Mean AoI and LST for GI/M/1 preemptive LCFS queue.
 *
 * For preemptive LCFS, the AoI simplifies significantly:
 *   E[A] = E[Y] + E[S] = E[Y] + 1/mu
 *   E[Apeak] = E[Y] + 1/mu
 *
 *   LST of AoI: A*(s) = Y*(s) * (mu / (s + mu))
 *   This is the product of interarrival LST and exponential service LST.
 *
 * @param Y_lst LST of interarrival time distribution
 * @param mu Service rate (exponential service), must be positive
 * @param E_Y Mean interarrival time (first moment), must be positive
 * @param E_Y2 Second moment of interarrival time (for reference)
 * @return AoiLstResult containing meanAoI, lstAoI, peakAoI
 * @throws IllegalArgumentException if parameters are invalid or system is unstable
 */
fun aoi_lcfspr_gim1(Y_lst: LstFunction, mu: Double, E_Y: Double, E_Y2: Double): AoiLstResult {
    require(mu > 0) { "Service rate mu must be positive" }
    require(E_Y > 0) { "Mean interarrival time E_Y must be positive" }

    val lambda = 1.0 / E_Y
    val rho = lambda / mu
    require(rho < 1) { "System unstable: rho = 1/(E_Y*mu) = ${"%.4f".format(rho)} >= 1" }

    // Mean service time
    val E_S = 1.0 / mu

    // Mean AoI for preemptive LCFS (Proposition 4)
    val meanAoI = E_Y + E_S

    // Mean Peak AoI (same as mean AoI for preemptive LCFS)
    val peakAoI = E_Y + E_S

    // LST of AoI for preemptive LCFS
    // A*(s) = Y*(s) * (mu / (s + mu))
    val lstAoI = object : LstFunction {
        override fun evaluate(s: Double): Double {
            return Y_lst.evaluate(s) * (mu / (s + mu))
        }
    }

    return AoiLstResult(meanAoI, lstAoI, peakAoI)
}

/**
 * Age of Information LCFS-PR GI/M/1 algorithms
 */
@Suppress("unused")
class AoiLcfsprGim1Algo {
    companion object {
        // Class documentation marker for Dokka
    }
}
