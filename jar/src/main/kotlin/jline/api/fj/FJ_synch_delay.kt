/**
 * @file Synchronization delay for two-way Fork-Join systems
 *
 * Computes the mean synchronization delay for a 2-way Fork-Join queueing
 * system. The synchronization delay is the average time tasks spend waiting
 * at synchronization queues for their siblings to complete.
 *
 * @since LINE 3.0
 */
package jline.api.fj

/**
 * Synchronization delay S_2(rho) for two-way Fork-Join.
 *
 *   S_2(rho) = (1/2) * (1 - rho/4) * R(rho)
 *
 * The F/J response time decomposes as:
 *   R_2^{F/J}(rho) = R(rho) + S_2(rho)
 *
 * where R(rho) = 1/(mu - lambda) is the M/M/1 response time.
 *
 * @param lambda Arrival rate
 * @param mu     Service rate (mu > lambda for stability)
 * @return Mean synchronization delay S_2(rho)
 *
 * @throws IllegalArgumentException if system is unstable
 *
 * Reference: Thomasian, Eq. (8) on page 17:11.
 */
fun fj_synch_delay(lambda: Double, mu: Double): Double {
    val rho = lambda / mu
    if (rho >= 1.0) {
        throw IllegalArgumentException(
            "System is unstable: rho = lambda/mu = ${"%.4f".format(rho)} >= 1. Require lambda < mu."
        )
    }

    val R_rho = 1.0 / (mu - lambda)
    return 0.5 * (1.0 - rho / 4.0) * R_rho
}

/**
 * Fork-Join synchronization delay algorithms
 */
@Suppress("unused")
class FJSynchDelayAlgo {
    companion object {
        // Class documentation marker for Dokka
    }
}
