/**
 * @file GI/M/1 FCFS Age of Information analysis
 *
 * Computes mean AoI, LST, and peak AoI for a GI/M/1 queue with
 * First-Come First-Served (FCFS) discipline.
 * GI/M/1: General independent arrivals, exponential service with rate mu.
 *
 * @since LINE 3.0
 */
package jline.api.aoi

import kotlin.math.abs

/**
 * Mean AoI and LST for GI/M/1 FCFS queue.
 *
 * Formulas (from Inoue et al., IEEE Trans. IT, 2019, Theorem 3):
 *   The key parameter sigma is the unique root in (0,1) of:
 *     Y*(mu - mu*sigma) = sigma
 *
 *   Mean AoI: E[A] = E[Y] + E[D] + sigma / (mu * (1-sigma))
 *   Peak AoI: E[Apeak] = E[Y] + E[D]
 *
 * @param Y_lst LST of interarrival time distribution
 * @param mu Service rate (exponential service), must be positive
 * @param E_Y Mean interarrival time (first moment), must be positive
 * @param E_Y2 Second moment of interarrival time, must be >= E_Y^2
 * @return AoiLstResult containing meanAoI, lstAoI, peakAoI
 * @throws IllegalArgumentException if parameters are invalid or system is unstable
 */
fun aoi_fcfs_gim1(Y_lst: LstFunction, mu: Double, E_Y: Double, E_Y2: Double): AoiLstResult {
    require(mu > 0) { "Service rate mu must be positive" }
    require(E_Y > 0) { "Mean interarrival time E_Y must be positive" }
    require(E_Y2 >= E_Y * E_Y) { "Second moment E_Y2 must be >= E_Y^2" }

    val lambda = 1.0 / E_Y
    val rho = lambda / mu
    require(rho < 1) { "System unstable: rho = 1/(E_Y*mu) = ${"%.4f".format(rho)} >= 1" }

    // Find sigma: unique root in (0,1) of Y*(mu - mu*sigma) = sigma
    val sigma = findSigmaGIM1(Y_lst, mu)

    // Mean system delay for GI/M/1
    val E_D = 1.0 / (mu * (1.0 - sigma))

    // Mean AoI (Proposition 2)
    val meanAoI = E_Y + E_D + sigma / (mu * (1.0 - sigma))

    // Mean Peak AoI
    val peakAoI = E_Y + E_D

    // LST of AoI (Theorem 3)
    val lstAoI = object : LstFunction {
        override fun evaluate(s: Double): Double {
            // Find sigma(s): root of Y*(s + mu - mu*sigma) = sigma
            val sigma_s = findSigmaGIM1Shifted(Y_lst, mu, s, sigma)
            // System delay LST: D*(s) = (1-sigma) * mu / (s + mu - mu*sigma_s)
            val D_s = (1.0 - sigma) * mu / (s + mu - mu * sigma_s)
            // AoI LST (Theorem 3)
            return (mu * sigma_s) / (s + mu - mu * sigma_s) * D_s
        }
    }

    return AoiLstResult(meanAoI, lstAoI, peakAoI)
}

/**
 * Find the unique root sigma in (0,1) of the equation:
 *   Y*(mu - mu*sigma) = sigma
 * using a combination of bisection and fixed-point iteration.
 *
 * @param Y_lst LST of interarrival time distribution
 * @param mu Service rate
 * @return The root sigma
 */
private fun findSigmaGIM1(Y_lst: LstFunction, mu: Double): Double {
    // Try bisection first
    var lo = 0.001
    var hi = 0.999
    val maxIter = 100
    val tol = 1e-12

    val fLo = Y_lst.evaluate(mu - mu * lo) - lo
    val fHi = Y_lst.evaluate(mu - mu * hi) - hi

    if (fLo * fHi < 0) {
        // Bisection
        for (iter in 0 until maxIter) {
            val mid = (lo + hi) / 2.0
            val fMid = Y_lst.evaluate(mu - mu * mid) - mid
            if (abs(fMid) < tol) {
                return mid
            }
            val fLoNew = Y_lst.evaluate(mu - mu * lo) - lo
            if (fLoNew * fMid < 0) {
                hi = mid
            } else {
                lo = mid
            }
        }
        return (lo + hi) / 2.0
    } else {
        // Fallback: fixed-point iteration
        var sigma = 0.5
        for (iter in 0 until maxIter) {
            val sigmaNew = Y_lst.evaluate(mu - mu * sigma)
            if (abs(sigmaNew - sigma) < tol) {
                return sigmaNew
            }
            sigma = sigmaNew
        }
        return sigma
    }
}

/**
 * Find sigma(s): root of Y*(s + mu - mu*sigma) = sigma
 * for a shifted version used in the LST evaluation.
 *
 * @param Y_lst LST of interarrival time distribution
 * @param mu Service rate
 * @param s Shift parameter
 * @param sigma0 Initial guess (the unshifted sigma)
 * @return The root sigma(s)
 */
private fun findSigmaGIM1Shifted(Y_lst: LstFunction, mu: Double, s: Double, sigma0: Double): Double {
    // Try bisection first
    var lo = 0.001
    var hi = 0.999
    val maxIter = 100
    val tol = 1e-12

    val fLo = Y_lst.evaluate(s + mu - mu * lo) - lo
    val fHi = Y_lst.evaluate(s + mu - mu * hi) - hi

    if (fLo * fHi < 0) {
        for (iter in 0 until maxIter) {
            val mid = (lo + hi) / 2.0
            val fMid = Y_lst.evaluate(s + mu - mu * mid) - mid
            if (abs(fMid) < tol) {
                return mid
            }
            val fLoNew = Y_lst.evaluate(s + mu - mu * lo) - lo
            if (fLoNew * fMid < 0) {
                hi = mid
            } else {
                lo = mid
            }
        }
        return (lo + hi) / 2.0
    } else {
        // Fallback: fixed-point iteration starting from sigma0
        var sigma = sigma0
        for (iter in 0 until maxIter) {
            val sigmaNew = Y_lst.evaluate(s + mu - mu * sigma)
            if (abs(sigmaNew - sigma) < tol) {
                return sigmaNew
            }
            sigma = sigmaNew
        }
        return sigma
    }
}

/**
 * Age of Information FCFS GI/M/1 algorithms
 */
@Suppress("unused")
class AoiFcfsGim1Algo {
    companion object {
        // Class documentation marker for Dokka
    }
}
