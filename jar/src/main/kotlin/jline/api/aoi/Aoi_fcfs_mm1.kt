/**
 * @file M/M/1 FCFS Age of Information analysis
 *
 * Computes mean, variance, and peak AoI for an M/M/1 queue with
 * First-Come First-Served (FCFS) discipline using closed-form formulas
 * from Inoue et al., IEEE Trans. IT, 2019.
 *
 * @since LINE 3.0
 */
package jline.api.aoi

import kotlin.math.max

/**
 * Mean, variance, and peak AoI for M/M/1 FCFS queue.
 *
 * Formulas (from Inoue et al., IEEE Trans. IT, 2019):
 *   Mean AoI: E[A] = (1/mu) * (1 + 1/rho + rho^2/(1-rho))
 *   Peak AoI: E[Apeak] = (1/mu) * (1 + 1/rho + rho/(1-rho))
 *
 * @param lambda Arrival rate (Poisson arrivals), must be positive
 * @param mu Service rate (exponential service), must be positive
 * @return AoiResult containing meanAoI, varAoI, peakAoI
 * @throws IllegalArgumentException if parameters are invalid or system is unstable
 */
fun aoi_fcfs_mm1(lambda: Double, mu: Double): AoiResult {
    require(lambda > 0) { "Arrival rate lambda must be positive" }
    require(mu > 0) { "Service rate mu must be positive" }

    val rho = lambda / mu
    require(rho < 1) { "System unstable: rho = lambda/mu = ${"%.4f".format(rho)} >= 1" }

    // Mean AoI (Proposition 1 / Corollary from Section III-A)
    val meanAoI = (1.0 / mu) * (1.0 + 1.0 / rho + rho * rho / (1.0 - rho))

    // Mean Peak AoI
    val peakAoI = (1.0 / mu) * (1.0 + 1.0 / rho + rho / (1.0 - rho))

    // Variance of AoI
    // E[A^2] = 2/mu^2 * (1/rho^2 + 1/rho + rho^2/(1-rho)^2 + 1/(1-rho) + rho/(1-rho))
    val E_A = meanAoI
    val E_A2 = (2.0 / (mu * mu)) * (1.0 / (rho * rho) + 1.0 / rho +
            rho * rho / ((1.0 - rho) * (1.0 - rho)) +
            1.0 / (1.0 - rho) + rho / (1.0 - rho))
    val varAoI = max(0.0, E_A2 - E_A * E_A)

    return AoiResult(meanAoI, varAoI, peakAoI)
}

/**
 * Age of Information FCFS M/M/1 algorithms
 */
@Suppress("unused")
class AoiFcfsMm1Algo {
    companion object {
        // Class documentation marker for Dokka
    }
}
