/**
 * @file GI/M/1 non-preemptive LCFS-D Age of Information analysis
 *
 * Computes mean AoI for a GI/M/1 queue with non-preemptive Last-Come
 * First-Served with Discarding (LCFS-D) discipline.
 *
 * In LCFS-D, when a new update arrives while the server is busy:
 *   - If there's an update waiting in queue, it is discarded
 *   - The new update takes its place in the queue
 *   - When service completes, the waiting update (if any) is served
 *
 * @since LINE 3.0
 */
package jline.api.aoi

import kotlin.math.abs

/**
 * Mean AoI for GI/M/1 non-preemptive LCFS-D queue.
 *
 * @param Y_lst LST of interarrival time distribution
 * @param mu Service rate (exponential service), must be positive
 * @param E_Y Mean interarrival time (first moment), must be positive
 * @param E_Y2 Second moment of interarrival time
 * @return AoiLstResult containing meanAoI, lstAoI (null), peakAoI
 * @throws IllegalArgumentException if parameters are invalid or system is unstable
 */
fun aoi_lcfsd_gim1(Y_lst: LstFunction, mu: Double, E_Y: Double, E_Y2: Double): AoiLstResult {
    require(mu > 0) { "Service rate mu must be positive" }
    require(E_Y > 0) { "Mean interarrival time E_Y must be positive" }

    val lambda = 1.0 / E_Y
    val rho = lambda / mu
    require(rho < 1) { "System unstable: rho = 1/(E_Y*mu) = ${"%.4f".format(rho)} >= 1" }

    // Mean service time
    val E_S = 1.0 / mu

    // Find sigma: probability arriving customer finds server busy
    val sigma = findSigmaGIM1ForLcfsd(Y_lst, mu, rho)

    // Probability arriving update finds server busy: sigma
    // When busy, expected remaining service = 1/mu (memoryless)

    // Mean effective system time
    val E_T_eff = E_S + sigma * E_S

    // Mean AoI for LCFS-D (from Section VI analysis adapted for GI/M/1)
    val meanAoI = E_Y + E_S * (1.0 + sigma) + sigma * E_S / (1.0 + sigma)

    // Mean Peak AoI
    val peakAoI = E_Y + E_T_eff

    // LST is complex for LCFS-D; return null
    return AoiLstResult(meanAoI, null, peakAoI)
}

/**
 * Find sigma for GI/M/1 LCFS-D: root of Y*(mu - mu*sigma) = sigma.
 *
 * @param Y_lst LST of interarrival time distribution
 * @param mu Service rate
 * @param rho Utilization (fallback value)
 * @return The root sigma
 */
private fun findSigmaGIM1ForLcfsd(Y_lst: LstFunction, mu: Double, rho: Double): Double {
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
 * Age of Information LCFS-D GI/M/1 algorithms
 */
@Suppress("unused")
class AoiLcfsdGim1Algo {
    companion object {
        // Class documentation marker for Dokka
    }
}
