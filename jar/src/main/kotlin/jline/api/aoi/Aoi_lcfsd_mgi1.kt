/**
 * @file M/GI/1 non-preemptive LCFS-D Age of Information analysis
 *
 * Computes mean AoI for an M/GI/1 queue with non-preemptive Last-Come
 * First-Served with Discarding (LCFS-D) discipline.
 *
 * In LCFS-D, when a new update arrives while the server is busy:
 *   - If there's an update waiting in queue, it is discarded
 *   - The new update takes its place in the queue
 *   - When service completes, the waiting update (if any) is served
 *
 * This is also known as M/GI/1/2* (buffer size 2 with replacement).
 *
 * @since LINE 3.0
 */
package jline.api.aoi

/**
 * Mean AoI for M/GI/1 non-preemptive LCFS-D queue.
 *
 * The analysis uses the concept of effective service time which includes
 * potential waiting while another update is being served.
 *
 * @param lambda Arrival rate (Poisson arrivals), must be positive
 * @param H_lst LST of service time distribution (for reference; may be null)
 * @param E_H Mean service time (first moment), must be positive
 * @param E_H2 Second moment of service time, must be >= E_H^2
 * @return AoiLstResult containing meanAoI, lstAoI (null), peakAoI
 * @throws IllegalArgumentException if parameters are invalid or system is unstable
 */
fun aoi_lcfsd_mgi1(lambda: Double, H_lst: LstFunction?, E_H: Double, E_H2: Double): AoiLstResult {
    require(lambda > 0) { "Arrival rate lambda must be positive" }
    require(E_H > 0) { "Mean service time E_H must be positive" }
    require(E_H2 >= E_H * E_H) { "Second moment E_H2 must be >= E_H^2" }

    val rho = lambda * E_H
    require(rho < 1) { "System unstable: rho = lambda*E_H = ${"%.4f".format(rho)} >= 1" }

    // Mean interarrival time
    val E_Y = 1.0 / lambda

    // For LCFS-D (also known as M/GI/1/2*), the analysis from Section VI:
    //
    // Key quantity: probability p0 that arriving update finds server idle
    // p0 = 1 - rho (for Poisson arrivals)
    //
    // Residual service time
    val E_H_residual = E_H2 / (2.0 * E_H)

    // Mean effective system time
    val E_T_eff = E_H + rho * E_H_residual

    // Mean AoI for LCFS-D (Proposition 6, simplified form)
    val meanAoI = E_Y + E_H + rho * E_H2 / (2.0 * E_H) + rho * E_H / (1.0 + rho)

    // Mean Peak AoI
    val peakAoI = E_Y + E_T_eff

    // LST is complex for LCFS-D; return null
    return AoiLstResult(meanAoI, null, peakAoI)
}

/**
 * Age of Information LCFS-D M/GI/1 algorithms
 */
@Suppress("unused")
class AoiLcfsdMgi1Algo {
    companion object {
        // Class documentation marker for Dokka
    }
}
