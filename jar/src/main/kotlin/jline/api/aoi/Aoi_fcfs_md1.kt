/**
 * @file M/D/1 FCFS Age of Information analysis
 *
 * Computes mean, variance, and peak AoI for an M/D/1 queue with
 * First-Come First-Served (FCFS) discipline.
 * M/D/1: Poisson arrivals with rate lambda, deterministic service time d.
 *
 * @since LINE 3.0
 */
package jline.api.aoi

import kotlin.math.max

/**
 * Mean, variance, and peak AoI for M/D/1 FCFS queue.
 *
 * Uses M/GI/1 FCFS results with deterministic service.
 * For M/D/1:
 *   E[H] = d, E[H^2] = d^2 (deterministic)
 *   rho = lambda * d
 *   E[W] = lambda * d^2 / (2*(1-rho))  (Pollaczek-Khinchine)
 *   E[A] = 1/lambda + E[W] + d + lambda*d^2/(2*(1-rho))
 *
 * @param lambda Arrival rate (Poisson arrivals), must be positive
 * @param d Deterministic service time, must be positive
 * @return AoiResult containing meanAoI, varAoI, peakAoI
 * @throws IllegalArgumentException if parameters are invalid or system is unstable
 */
fun aoi_fcfs_md1(lambda: Double, d: Double): AoiResult {
    require(lambda > 0) { "Arrival rate lambda must be positive" }
    require(d > 0) { "Service time d must be positive" }

    val rho = lambda * d
    require(rho < 1) { "System unstable: rho = lambda*d = ${"%.4f".format(rho)} >= 1" }

    // Service time moments (deterministic)
    val E_H = d
    val E_H2 = d * d

    // Mean waiting time (Pollaczek-Khinchine for M/G/1)
    val E_W = lambda * E_H2 / (2.0 * (1.0 - rho))

    // Mean system time (sojourn time)
    val E_T = E_W + E_H

    // Mean interarrival time
    val E_Y = 1.0 / lambda

    // Mean AoI for M/G/1 FCFS (Proposition 1)
    val meanAoI = E_Y + E_T + lambda * E_H2 / (2.0 * (1.0 - rho))

    // Mean Peak AoI
    val peakAoI = E_T + E_Y

    // Variance of AoI
    val E_Y2 = 2.0 / (lambda * lambda) // Second moment of exponential
    val varAoI = max(0.0,
        E_Y2 - E_Y * E_Y + 2.0 * E_W * E_H / (1.0 - rho) +
                E_H2 * rho / ((1.0 - rho) * (1.0 - rho))
    )

    return AoiResult(meanAoI, varAoI, peakAoI)
}

/**
 * Age of Information FCFS M/D/1 algorithms
 */
@Suppress("unused")
class AoiFcfsMd1Algo {
    companion object {
        // Class documentation marker for Dokka
    }
}
