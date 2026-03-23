/**
 * @file M/D/1 preemptive LCFS Age of Information analysis
 *
 * Computes mean, variance, and peak AoI for an M/D/1 queue with
 * preemptive Last-Come First-Served (LCFS-PR) discipline.
 *
 * In LCFS-PR, when a new update arrives, it preempts the current update
 * in service (if any). For M/D/1, an update is successful if no new
 * arrivals occur during its deterministic service time d.
 *
 * @since LINE 3.0
 */
package jline.api.aoi

/**
 * Mean, variance, and peak AoI for M/D/1 preemptive LCFS queue.
 *
 * For preemptive LCFS with M/GI/1:
 *   E[A] = E[Y] + E[S] = 1/lambda + d
 * where S is the (successful) service time, which equals d.
 *
 * @param lambda Arrival rate (Poisson arrivals), must be positive
 * @param d Deterministic service time, must be positive
 * @return AoiResult containing meanAoI, varAoI, peakAoI
 * @throws IllegalArgumentException if parameters are invalid or system is unstable
 */
fun aoi_lcfspr_md1(lambda: Double, d: Double): AoiResult {
    require(lambda > 0) { "Arrival rate lambda must be positive" }
    require(d > 0) { "Service time d must be positive" }

    val rho = lambda * d
    require(rho < 1) { "System unstable: rho = lambda*d = ${"%.4f".format(rho)} >= 1" }

    // Mean interarrival time
    val E_Y = 1.0 / lambda

    // Mean service time (deterministic)
    val E_S = d

    // Mean AoI for preemptive LCFS (Proposition 3 / Section IV)
    val meanAoI = E_Y + E_S

    // Mean Peak AoI (same formula for preemptive LCFS)
    val peakAoI = E_Y + E_S

    // Variance of AoI for preemptive LCFS
    // Var[A] = Var[Y] + Var[S]
    // Since S is deterministic, Var[S] = 0
    // Var[Y] = 1/lambda^2 (exponential)
    val varAoI = 1.0 / (lambda * lambda)

    return AoiResult(meanAoI, varAoI, peakAoI)
}

/**
 * Age of Information LCFS-PR M/D/1 algorithms
 */
@Suppress("unused")
class AoiLcfsprMd1Algo {
    companion object {
        // Class documentation marker for Dokka
    }
}
