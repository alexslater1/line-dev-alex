/**
 * @file D/M/1 preemptive LCFS Age of Information analysis
 *
 * Computes mean, variance, and peak AoI for a D/M/1 queue with
 * preemptive Last-Come First-Served (LCFS-PR) discipline.
 *
 * In LCFS-PR, when a new update arrives, it preempts the current update
 * in service (if any). For D/M/1, arrivals are deterministic (every tau
 * time units) and service is exponential with rate mu.
 *
 * @since LINE 3.0
 */
package jline.api.aoi

/**
 * Mean, variance, and peak AoI for D/M/1 preemptive LCFS queue.
 *
 * For preemptive LCFS with GI/M/1:
 *   E[A] = E[Y] + E[S] = tau + 1/mu
 * where S is the service time (exponential).
 *
 * @param tau Deterministic interarrival time, must be positive
 * @param mu Service rate (exponential service), must be positive
 * @return AoiResult containing meanAoI, varAoI, peakAoI
 * @throws IllegalArgumentException if parameters are invalid or system is unstable
 */
fun aoi_lcfspr_dm1(tau: Double, mu: Double): AoiResult {
    require(tau > 0) { "Interarrival time tau must be positive" }
    require(mu > 0) { "Service rate mu must be positive" }

    val lambda = 1.0 / tau
    val rho = lambda / mu
    require(rho < 1) { "System unstable: rho = 1/(tau*mu) = ${"%.4f".format(rho)} >= 1" }

    // Mean interarrival time (deterministic)
    val E_Y = tau

    // Mean service time (exponential)
    val E_S = 1.0 / mu

    // Mean AoI for preemptive LCFS (Proposition 4 / Section IV)
    val meanAoI = E_Y + E_S

    // Mean Peak AoI (same formula for preemptive LCFS)
    val peakAoI = E_Y + E_S

    // Variance of AoI for preemptive LCFS
    // Var[A] = Var[Y] + Var[S]
    // Since Y is deterministic, Var[Y] = 0
    // Var[S] = 1/mu^2 (exponential)
    val varAoI = 1.0 / (mu * mu)

    return AoiResult(meanAoI, varAoI, peakAoI)
}

/**
 * Age of Information LCFS-PR D/M/1 algorithms
 */
@Suppress("unused")
class AoiLcfsprDm1Algo {
    companion object {
        // Class documentation marker for Dokka
    }
}
