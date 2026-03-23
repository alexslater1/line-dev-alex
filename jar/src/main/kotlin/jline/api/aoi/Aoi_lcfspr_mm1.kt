/**
 * @file M/M/1 preemptive LCFS Age of Information analysis
 *
 * Computes mean, variance, and peak AoI for an M/M/1 queue with
 * preemptive Last-Come First-Served (LCFS-PR) discipline.
 *
 * In LCFS-PR, when a new update arrives, it preempts the current update
 * in service (if any). This ensures the freshest update is always served,
 * leading to lower AoI than FCFS.
 *
 * @since LINE 3.0
 */
package jline.api.aoi

import kotlin.math.max

/**
 * Mean, variance, and peak AoI for M/M/1 preemptive LCFS queue.
 *
 * Formulas (from Inoue et al., IEEE Trans. IT, 2019, Section IV):
 *   Mean AoI: E[A] = (1/mu) * (1 + 1/rho) = 1/mu + 1/lambda
 *   Peak AoI: E[Apeak] = 1/mu + 1/lambda
 *
 * Note: LCFS-PR always achieves lower mean AoI than FCFS for M/M/1.
 *
 * @param lambda Arrival rate (Poisson arrivals), must be positive
 * @param mu Service rate (exponential service), must be positive
 * @return AoiResult containing meanAoI, varAoI, peakAoI
 * @throws IllegalArgumentException if parameters are invalid or system is unstable
 */
fun aoi_lcfspr_mm1(lambda: Double, mu: Double): AoiResult {
    require(lambda > 0) { "Arrival rate lambda must be positive" }
    require(mu > 0) { "Service rate mu must be positive" }

    val rho = lambda / mu
    require(rho < 1) { "System unstable: rho = lambda/mu = ${"%.4f".format(rho)} >= 1" }

    // Mean AoI for preemptive LCFS (Proposition 3 / Section IV)
    val meanAoI = (1.0 / mu) * (1.0 + 1.0 / rho)

    // Mean Peak AoI for preemptive LCFS
    val peakAoI = 1.0 / mu + 1.0 / lambda

    // Variance of AoI for preemptive LCFS
    // E[A^2] = 2*(1/lambda^2 + 1/(lambda*mu) + 1/mu^2)
    val E_A = meanAoI
    val E_A2 = 2.0 * (1.0 / (lambda * lambda) + 1.0 / (lambda * mu) + 1.0 / (mu * mu))
    val varAoI = max(0.0, E_A2 - E_A * E_A)

    return AoiResult(meanAoI, varAoI, peakAoI)
}

/**
 * Age of Information LCFS-PR M/M/1 algorithms
 */
@Suppress("unused")
class AoiLcfsprMm1Algo {
    companion object {
        // Class documentation marker for Dokka
    }
}
