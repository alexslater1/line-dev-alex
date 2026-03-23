/**
 * @file D/M/1 FCFS Age of Information analysis
 *
 * Computes mean, variance, and peak AoI for a D/M/1 queue with
 * First-Come First-Served (FCFS) discipline.
 * D/M/1: Deterministic arrivals with interarrival time tau (rate 1/tau),
 *        exponential service with rate mu.
 *
 * @since LINE 3.0
 */
package jline.api.aoi

import kotlin.math.abs
import kotlin.math.exp
import kotlin.math.max

/**
 * Mean, variance, and peak AoI for D/M/1 FCFS queue.
 *
 * Uses GI/M/1 FCFS results with deterministic arrivals.
 * The key parameter sigma is the unique root in (0,1) of:
 *   sigma = exp(-mu*tau*(1-sigma))
 * which gives the probability an arriving customer finds the system non-empty.
 *
 * @param tau Deterministic interarrival time, must be positive
 * @param mu Service rate (exponential service), must be positive
 * @return AoiResult containing meanAoI, varAoI, peakAoI
 * @throws IllegalArgumentException if parameters are invalid or system is unstable
 */
fun aoi_fcfs_dm1(tau: Double, mu: Double): AoiResult {
    require(tau > 0) { "Interarrival time tau must be positive" }
    require(mu > 0) { "Service rate mu must be positive" }

    val lambda = 1.0 / tau
    val rho = lambda / mu
    require(rho < 1) { "System unstable: rho = 1/(tau*mu) = ${"%.4f".format(rho)} >= 1" }

    // Compute sigma: unique root in (0,1) of sigma = exp(-mu*tau*(1-sigma))
    // Use bisection method
    val sigma = findSigmaDM1(mu, tau)

    // Mean system delay for GI/M/1
    val E_D = 1.0 / (mu * (1.0 - sigma))

    // Mean AoI
    val meanAoI = tau + E_D + sigma / (mu * (1.0 - sigma))

    // Mean Peak AoI
    val peakAoI = tau + E_D

    // Variance of AoI
    val E_D2 = 2.0 / ((mu * (1.0 - sigma)) * (mu * (1.0 - sigma)))
    val Var_D = E_D2 - E_D * E_D
    val varAoI = max(0.0, Var_D + (sigma / (mu * (1.0 - sigma))) * (sigma / (mu * (1.0 - sigma))))

    return AoiResult(meanAoI, varAoI, peakAoI)
}

/**
 * Find the unique root sigma in (0,1) of the equation:
 *   sigma = exp(-mu*tau*(1-sigma))
 * using bisection method.
 *
 * @param mu Service rate
 * @param tau Deterministic interarrival time
 * @return The root sigma
 */
private fun findSigmaDM1(mu: Double, tau: Double): Double {
    // f(sigma) = exp(-mu*tau*(1-sigma)) - sigma = 0
    var lo = 0.001
    var hi = 0.999
    val maxIter = 100
    val tol = 1e-12

    for (iter in 0 until maxIter) {
        val mid = (lo + hi) / 2.0
        val fMid = exp(-mu * tau * (1.0 - mid)) - mid
        if (abs(fMid) < tol) {
            return mid
        }
        val fLo = exp(-mu * tau * (1.0 - lo)) - lo
        if (fLo * fMid < 0) {
            hi = mid
        } else {
            lo = mid
        }
    }
    return (lo + hi) / 2.0
}

/**
 * Age of Information FCFS D/M/1 algorithms
 */
@Suppress("unused")
class AoiFcfsDm1Algo {
    companion object {
        // Class documentation marker for Dokka
    }
}
