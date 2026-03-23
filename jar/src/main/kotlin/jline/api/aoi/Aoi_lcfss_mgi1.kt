/**
 * @file M/GI/1 non-preemptive LCFS-S Age of Information analysis
 *
 * Computes mean AoI for an M/GI/1 queue with non-preemptive Last-Come
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

/**
 * Mean AoI for M/GI/1 non-preemptive LCFS-S queue.
 *
 * The formula involves the busy period distribution of M/G/1:
 *   E[B] = E[H] / (1 - rho)
 *   E[B^2] = E[H^2] / (1 - rho)^3
 *
 *   E[A] = 1/lambda + E[H] + lambda * E[H^2] / (2 * (1 - rho)^2)
 *
 * @param lambda Arrival rate (Poisson arrivals), must be positive
 * @param H_lst LST of service time distribution (for reference; may be null)
 * @param E_H Mean service time (first moment), must be positive
 * @param E_H2 Second moment of service time, must be >= E_H^2
 * @return AoiLstResult containing meanAoI, lstAoI (null), peakAoI
 * @throws IllegalArgumentException if parameters are invalid or system is unstable
 */
fun aoi_lcfss_mgi1(lambda: Double, H_lst: LstFunction?, E_H: Double, E_H2: Double): AoiLstResult {
    require(lambda > 0) { "Arrival rate lambda must be positive" }
    require(E_H > 0) { "Mean service time E_H must be positive" }
    require(E_H2 >= E_H * E_H) { "Second moment E_H2 must be >= E_H^2" }

    val rho = lambda * E_H
    require(rho < 1) { "System unstable: rho = lambda*E_H = ${"%.4f".format(rho)} >= 1" }

    // Mean interarrival time
    val E_Y = 1.0 / lambda

    // Busy period moments
    val E_B = E_H / (1.0 - rho)
    val E_B2 = E_H2 / ((1.0 - rho) * (1.0 - rho) * (1.0 - rho))

    // Mean AoI for LCFS-S (Proposition 5, simplified form)
    val meanAoI = E_Y + E_H + lambda * E_H2 / (2.0 * (1.0 - rho) * (1.0 - rho))

    // Mean Peak AoI
    // For LCFS-S, peak AoI involves the residual busy period
    val peakAoI = E_Y + E_H + lambda * E_B2 / (2.0 * E_B)

    // LST is complex for LCFS-S; return null
    return AoiLstResult(meanAoI, null, peakAoI)
}

/**
 * Age of Information LCFS-S M/GI/1 algorithms
 */
@Suppress("unused")
class AoiLcfssMgi1Algo {
    companion object {
        // Class documentation marker for Dokka
    }
}
