/**
 * @file Maximum response time computations for Fork-Join systems
 *
 * Computes the expected value of the maximum response time across K parallel
 * M/M/1 queues, which serves as an upper bound for the K-way Fork-Join
 * response time. Includes exact exponential formula, Erlang approximation,
 * and EVD (Extreme Value Distribution) approximation.
 *
 * @since LINE 3.0
 */
package jline.api.fj

import jline.util.Maths

/**
 * Compute maximum response time R_K^max(rho) for K M/M/1 queues.
 *
 * R_K^max(rho) = H_K * R(rho) = H_K / (mu - lambda)
 *
 * This serves as an upper bound to the K-way Fork-Join response time.
 *
 * @param K      Number of parallel servers (positive integer)
 * @param lambda Arrival rate
 * @param mu     Service rate (mu > lambda for stability)
 * @return Expected maximum response time R_K^max(rho)
 *
 * @throws IllegalArgumentException if K < 1 or system is unstable
 *
 * Reference: Thomasian, Eq. (1) on page 17:8.
 */
fun fj_rmax(K: Int, lambda: Double, mu: Double): Double {
    if (K < 1) {
        throw IllegalArgumentException("K must be a positive integer. Got K=$K.")
    }

    val rho = lambda / mu
    if (rho >= 1.0) {
        throw IllegalArgumentException(
            "System is unstable: rho = lambda/mu = ${"%.4f".format(rho)} >= 1. Require lambda < mu."
        )
    }

    val H_K = fj_harmonic(K)
    val R_rho = 1.0 / (mu - lambda)

    return H_K * R_rho
}

/**
 * Maximum response time R_K^max for Erlang service times.
 *
 * Computes the expected maximum response time for K M/E_k/1 queues.
 * For K=2, uses a closed-form formula. For general K, uses numerical
 * integration.
 *
 * @param K      Number of parallel servers (positive integer)
 * @param k      Number of Erlang stages (positive integer)
 * @param lambda Arrival rate
 * @param mu     Service rate per Erlang stage (mean service = k/mu)
 * @return Expected maximum response time
 *
 * @throws IllegalArgumentException if K < 1, k < 1, or system is unstable
 *
 * Reference: Thomasian, Eq. (33) and (34) on page 17:18.
 */
fun fj_rmax_erlang(K: Int, k: Int, lambda: Double, mu: Double): Double {
    if (K < 1) {
        throw IllegalArgumentException("K must be a positive integer. Got K=$K.")
    }
    if (k < 1) {
        throw IllegalArgumentException("k (number of stages) must be a positive integer. Got k=$k.")
    }

    val meanService = k.toDouble() / mu
    val rho = lambda * meanService
    if (rho >= 1.0) {
        throw IllegalArgumentException("System is unstable: rho = ${"%.4f".format(rho)} >= 1.")
    }

    // M/E_k/1 response time using Pollaczek-Khinchin formula
    val cv2 = 1.0 / k
    val R_single = meanService * (1.0 + rho * (1.0 + cv2) / (2.0 * (1.0 - rho)))

    if (K == 2) {
        // Closed-form for K=2 (Eq. 34)
        val muResp = k.toDouble() / R_single
        var correction = 0.0
        for (m in 0 until k) {
            for (n in 0 until k) {
                correction += Maths.binomialCoeff(m + n, m) *
                        Math.pow(muResp, m.toDouble()) *
                        Math.pow(muResp, n.toDouble()) /
                        Math.pow(2.0 * muResp, (m + n + 1).toDouble())
            }
        }
        return 2.0 * R_single - correction
    } else {
        // Numerical integration for general K
        // Approximate response time distribution as Erlang with adjusted parameters
        val cv2Response = Math.max((cv2 + rho) / (1.0 + rho), 1.0 / 20.0)
        val kResp = Math.max(1, Math.ceil(1.0 / cv2Response).toInt())
        val muResp = kResp.toDouble() / R_single

        // Expected maximum: integral_0^inf [1 - F(t)^K] dt
        // Use composite Simpson's rule for numerical integration
        val upperLimit = R_single * 20.0
        val numPoints = 10001 // Odd number for Simpson's rule
        val h = upperLimit / (numPoints - 1)

        var integral = 0.0
        for (i in 0 until numPoints) {
            val t = i * h
            val cdfVal = erlangCdf(t, kResp, muResp)
            val integrandVal = 1.0 - Math.pow(cdfVal, K.toDouble())

            val weight = when {
                i == 0 || i == numPoints - 1 -> 1.0
                i % 2 == 1 -> 4.0
                else -> 2.0
            }
            integral += weight * integrandVal
        }
        integral *= h / 3.0

        return integral
    }
}

/**
 * Maximum response time using Extreme Value Distribution (EVD) approximation.
 *
 *   R_K^max(rho) = R(rho) + (sqrt(6)*ln(K)/pi) * sigma_R(rho)
 *
 * Optionally uses a calibrated version from Thomasian et al. [2007] that
 * divides the correction term by 1.27 for improved accuracy.
 *
 * @param K          Number of parallel servers (positive integer)
 * @param R          Mean response time
 * @param sigmaR     Standard deviation of response time
 * @param calibrated Use calibrated formula (default: false)
 * @return Approximate maximum response time
 *
 * @throws IllegalArgumentException if K < 1, R <= 0, or sigmaR < 0
 *
 * Reference: Thomasian, Eq. (43) on page 17:21.
 */
fun fj_rmax_evd(K: Int, R: Double, sigmaR: Double, calibrated: Boolean = false): Double {
    if (K < 1) {
        throw IllegalArgumentException("K must be a positive integer. Got K=$K.")
    }
    if (R <= 0.0) {
        throw IllegalArgumentException("Mean response time R must be positive.")
    }
    if (sigmaR < 0.0) {
        throw IllegalArgumentException("Standard deviation sigma_R must be non-negative.")
    }

    var correctionFactor = Math.sqrt(6.0) * Math.log(K.toDouble()) / Math.PI
    if (calibrated) {
        correctionFactor /= 1.27
    }

    return R + correctionFactor * sigmaR
}

/**
 * Compute Erlang CDF: F(t) = 1 - exp(-mu*t) * sum_{j=0}^{k-1} (mu*t)^j / j!
 */
private fun erlangCdf(t: Double, k: Int, mu: Double): Double {
    if (t <= 0.0) {
        return 0.0
    }
    var S = 0.0
    for (j in 0 until k) {
        S += Math.pow(mu * t, j.toDouble()) / Maths.fact(j.toDouble())
    }
    return 1.0 - Math.exp(-mu * t) * S
}

/**
 * Fork-Join maximum response time algorithms
 */
@Suppress("unused")
class FJRmaxAlgo {
    companion object {
        // Class documentation marker for Dokka
    }
}
