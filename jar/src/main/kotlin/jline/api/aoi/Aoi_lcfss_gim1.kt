/**
 * @file GI/M/1 non-preemptive LCFS-S Age of Information analysis
 *
 * Computes mean AoI for a GI/M/1 queue with non-preemptive Last-Come
 * First-Served with Set-aside (LCFS-S) discipline.
 *
 * In LCFS-S, when a new update arrives while the server is busy:
 *   - The new update waits in the queue
 *   - When service completes, the most recent update in queue is served next
 *   - The older update remains in queue (is "set aside")
 *
 * @since LINE 3.0
 */
package jline.api.aoi

import kotlin.math.abs

/**
 * Mean AoI for GI/M/1 non-preemptive LCFS-S queue.
 *
 * @param Y_lst LST of interarrival time distribution
 * @param mu Service rate (exponential service), must be positive
 * @param E_Y Mean interarrival time (first moment), must be positive
 * @param E_Y2 Second moment of interarrival time
 * @return AoiLstResult containing meanAoI, lstAoI (null), peakAoI
 * @throws IllegalArgumentException if parameters are invalid or system is unstable
 */
fun aoi_lcfss_gim1(Y_lst: LstFunction, mu: Double, E_Y: Double, E_Y2: Double): AoiLstResult {
    require(mu > 0) { "Service rate mu must be positive" }
    require(E_Y > 0) { "Mean interarrival time E_Y must be positive" }

    val lambda = 1.0 / E_Y
    val rho = lambda / mu
    require(rho < 1) { "System unstable: rho = 1/(E_Y*mu) = ${"%.4f".format(rho)} >= 1" }

    // Mean service time
    val E_S = 1.0 / mu

    // Find sigma: probability arriving customer finds server busy
    val sigma = findSigmaGIM1ForLcfss(Y_lst, mu, rho)

    // Mean system delay
    val E_D = 1.0 / (mu * (1.0 - sigma))

    // Mean AoI for LCFS-S (from Section V analysis adapted for GI/M/1)
    val meanAoI = E_Y + E_S + sigma * E_D

    // Mean Peak AoI
    val peakAoI = E_Y + E_D

    // LST is complex for LCFS-S; return null
    return AoiLstResult(meanAoI, null, peakAoI)
}

/**
 * Find sigma for GI/M/1 LCFS-S: root of Y*(mu - mu*sigma) = sigma.
 *
 * @param Y_lst LST of interarrival time distribution
 * @param mu Service rate
 * @param rho Utilization (fallback value)
 * @return The root sigma
 */
private fun findSigmaGIM1ForLcfss(Y_lst: LstFunction, mu: Double, rho: Double): Double {
    var lo = 0.001
    var hi = 0.999
    val maxIter = 100
    val tol = 1e-12

    val fLo = Y_lst.evaluate(mu - mu * lo) - lo
    val fHi = Y_lst.evaluate(mu - mu * hi) - hi

    if (fLo * fHi < 0) {
        for (iter in 0 until maxIter) {
            val mid = (lo + hi) / 2.0
            val fMid = Y_lst.evaluate(mu - mu * mid) - mid
            if (abs(fMid) < tol) {
                return mid
            }
            val fLoNew = Y_lst.evaluate(mu - mu * lo) - lo
            if (fLoNew * fMid < 0) {
                hi = mid
            } else {
                lo = mid
            }
        }
        return (lo + hi) / 2.0
    } else {
        // Fallback
        return rho
    }
}

/**
 * Age of Information LCFS-S GI/M/1 algorithms
 */
@Suppress("unused")
class AoiLcfssGim1Algo {
    companion object {
        // Class documentation marker for Dokka
    }
}
