/**
 * @file M/GI/1 FCFS Age of Information analysis
 *
 * Computes mean AoI, LST, and peak AoI for an M/GI/1 queue with
 * First-Come First-Served (FCFS) discipline.
 * M/GI/1: Poisson arrivals with rate lambda, general independent service.
 *
 * @since LINE 3.0
 */
package jline.api.aoi

/**
 * Mean AoI and LST for M/GI/1 FCFS queue.
 *
 * Formulas (from Inoue et al., IEEE Trans. IT, 2019, Theorem 2):
 *   LST of AoI: A*(s) = (lambda * H*(s)) / (s + lambda - lambda*H*(s)) * W*(s)
 *   where W*(s) is the LST of waiting time (Pollaczek-Khinchine).
 *
 *   Mean AoI (Proposition 1):
 *   E[A] = E[Y] + E[T] + lambda * E[H^2] / (2*(1-rho))
 *        = 1/lambda + E[W] + E[H] + lambda * E[H^2] / (2*(1-rho))
 *
 * @param lambda Arrival rate (Poisson arrivals), must be positive
 * @param H_lst LST of service time distribution
 * @param E_H Mean service time (first moment), must be positive
 * @param E_H2 Second moment of service time, must be >= E_H^2
 * @return AoiLstResult containing meanAoI, lstAoI, peakAoI
 * @throws IllegalArgumentException if parameters are invalid or system is unstable
 */
fun aoi_fcfs_mgi1(lambda: Double, H_lst: LstFunction, E_H: Double, E_H2: Double): AoiLstResult {
    require(lambda > 0) { "Arrival rate lambda must be positive" }
    require(E_H > 0) { "Mean service time E_H must be positive" }
    require(E_H2 >= E_H * E_H) { "Second moment E_H2 must be >= E_H^2" }

    val rho = lambda * E_H
    require(rho < 1) { "System unstable: rho = lambda*E_H = ${"%.4f".format(rho)} >= 1" }

    // Mean interarrival time
    val E_Y = 1.0 / lambda

    // Mean waiting time (Pollaczek-Khinchine formula)
    val E_W = lambda * E_H2 / (2.0 * (1.0 - rho))

    // Mean system time (sojourn time)
    val E_T = E_W + E_H

    // Mean AoI (Proposition 1)
    val meanAoI = E_Y + E_T + lambda * E_H2 / (2.0 * (1.0 - rho))

    // Mean Peak AoI
    val peakAoI = E_T + E_Y

    // LST of AoI (Theorem 2)
    // A*(s) = (lambda * H*(s)) / (s + lambda - lambda*H*(s)) * W*(s)
    // where W*(s) = (1-rho) * s / (s - lambda + lambda*H*(s))
    val lstAoI = object : LstFunction {
        override fun evaluate(s: Double): Double {
            val H_s = H_lst.evaluate(s)
            // Pollaczek-Khinchine LST for waiting time
            val W_s = (1.0 - rho) * s / (s - lambda + lambda * H_s)
            // AoI LST (Theorem 2)
            return (lambda * H_s) / (s + lambda - lambda * H_s) * W_s
        }
    }

    return AoiLstResult(meanAoI, lstAoI, peakAoI)
}

/**
 * Age of Information FCFS M/GI/1 algorithms
 */
@Suppress("unused")
class AoiFcfsMgi1Algo {
    companion object {
        // Class documentation marker for Dokka
    }
}
