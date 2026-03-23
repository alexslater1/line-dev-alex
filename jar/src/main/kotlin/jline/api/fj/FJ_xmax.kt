/**
 * @file Expected maximum of K i.i.d. random variables for Fork-Join analysis
 *
 * Computes the expected value of the maximum (order statistic Y_K) of K
 * independent and identically distributed random variables. Functions are
 * provided for exponential, Erlang, hyperexponential, normal, and Pareto
 * distributions, as well as a general mean-variance approximation (David, 1970)
 * and the EMMA method (Sun & Peterson, 2012).
 *
 * @since LINE 3.0
 */
package jline.api.fj

import jline.util.Maths

/**
 * Expected maximum of K i.i.d. exponential random variables.
 *
 *   X_K^max = H_K / mu
 *
 * @param K  Number of parallel servers (positive integer)
 * @param mu Service rate (mean service time is 1/mu)
 * @return Expected maximum service time
 *
 * @throws IllegalArgumentException if K < 1 or mu <= 0
 *
 * Reference: Thomasian, Eq. (10) on page 17:11.
 */
fun fj_xmax_exp(K: Int, mu: Double): Double {
    if (K < 1) {
        throw IllegalArgumentException("K must be a positive integer. Got K=$K.")
    }
    if (mu <= 0.0) {
        throw IllegalArgumentException("Service rate mu must be positive. Got mu=${"%.4f".format(mu)}.")
    }

    return fj_harmonic(K) / mu
}

/**
 * Expected maximum of 2 exponential random variables.
 *
 *   Y_2^max = 1/lambda1 + 1/lambda2 - 1/(lambda1 + lambda2)
 *
 * For identical rates: Y_2^max = 1.5/lambda = H_2/lambda
 *
 * @param lambda1 Rate of first exponential
 * @param lambda2 Rate of second exponential (default: same as lambda1)
 * @return Expected maximum Y_2^max
 *
 * @throws IllegalArgumentException if rates are not positive
 *
 * Reference: Thomasian, Eq. (27) on page 17:17.
 */
fun fj_xmax_2(lambda1: Double, lambda2: Double): Double {
    if (lambda1 <= 0.0 || lambda2 <= 0.0) {
        throw IllegalArgumentException(
            "Rates must be positive. Got lambda1=${"%.4f".format(lambda1)}, lambda2=${"%.4f".format(lambda2)}."
        )
    }
    return 1.0 / lambda1 + 1.0 / lambda2 - 1.0 / (lambda1 + lambda2)
}

/**
 * Expected maximum of 2 i.i.d. exponential random variables (same rate).
 *
 * @param lambda Rate of both exponentials
 * @return Expected maximum Y_2^max = 1.5/lambda
 */
fun fj_xmax_2(lambda: Double): Double {
    return fj_xmax_2(lambda, lambda)
}

/**
 * Expected maximum of K i.i.d. Erlang-k random variables.
 *
 * For k=2 (Erlang-2), uses the closed-form formula:
 *   X_K^max = (1/mu) * sum_{n=1}^{K} C(K,n) * (-1)^{n-1} * sum_{m=1}^{n} C(n,m) * m! / (2*n^{m+1})
 *
 * For general k, uses numerical integration:
 *   X_K^max = integral_0^inf [1 - F_Erlang(t)^K] dt
 *
 * @param K  Number of parallel servers (positive integer)
 * @param k  Number of Erlang stages (positive integer)
 * @param mu Rate parameter per stage (mean service time = k/mu)
 * @return Expected maximum service time
 *
 * @throws IllegalArgumentException if K < 1, k < 1, or mu <= 0
 *
 * Reference: Thomasian, page 17:13.
 */
fun fj_xmax_erlang(K: Int, k: Int, mu: Double): Double {
    if (K < 1) {
        throw IllegalArgumentException("K must be a positive integer. Got K=$K.")
    }
    if (k < 1) {
        throw IllegalArgumentException("k (number of stages) must be a positive integer. Got k=$k.")
    }
    if (mu <= 0.0) {
        throw IllegalArgumentException("Rate mu must be positive. Got mu=${"%.4f".format(mu)}.")
    }

    if (k == 2) {
        // Closed-form for Erlang-2
        var outerSum = 0.0
        for (n in 1..K) {
            var innerSum = 0.0
            for (m in 1..n) {
                innerSum += Maths.binomialCoeff(n, m) * Maths.fact(m.toDouble()) / (2.0 * Math.pow(n.toDouble(), (m + 1).toDouble()))
            }
            val sign = if ((n - 1) % 2 == 0) 1.0 else -1.0
            outerSum += Maths.binomialCoeff(K, n) * sign * innerSum
        }
        return outerSum / mu
    } else {
        // Numerical integration for general k
        // Erlang CDF: F(t) = 1 - exp(-mu*t) * sum_{j=0}^{k-1} (mu*t)^j / j!
        val meanErlang = k.toDouble() / mu
        val upperLimit = meanErlang * 10.0 + 10.0 * Math.sqrt(k.toDouble()) / mu

        // Composite Simpson's rule
        val numPoints = 10001
        val h = upperLimit / (numPoints - 1)
        var integral = 0.0
        for (i in 0 until numPoints) {
            val t = i * h
            val cdfVal = erlangCdfGeneral(t, k, mu)
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
 * Expected maximum of K i.i.d. Hyperexponential-2 random variables.
 *
 *   X_K^max = sum_{n=1}^{K} (-1)^{n+1} * sum_{m=0}^{n} C(n,m) * p1^m * p2^{n-m} / (m*mu1 + (n-m)*mu2)
 *
 * @param K   Number of parallel servers (positive integer)
 * @param p1  Probability of branch 1 (0 < p1 < 1)
 * @param mu1 Rate of branch 1
 * @param mu2 Rate of branch 2
 * @return Expected maximum service time
 *
 * @throws IllegalArgumentException if parameters are invalid
 *
 * Reference: Thomasian, Eq. (15) on page 17:13.
 */
fun fj_xmax_hyperexp(K: Int, p1: Double, mu1: Double, mu2: Double): Double {
    if (K < 1) {
        throw IllegalArgumentException("K must be a positive integer. Got K=$K.")
    }
    if (p1 <= 0.0 || p1 >= 1.0) {
        throw IllegalArgumentException("p1 must be in (0,1). Got p1=${"%.4f".format(p1)}.")
    }
    if (mu1 <= 0.0 || mu2 <= 0.0) {
        throw IllegalArgumentException("Rates mu1 and mu2 must be positive.")
    }

    val p2 = 1.0 - p1
    var Xmax = 0.0
    for (n in 1..K) {
        var innerSum = 0.0
        for (m in 0..n) {
            val denominator = m * mu1 + (n - m) * mu2
            if (denominator > 0.0) {
                innerSum += Maths.binomialCoeff(n, m) *
                        Math.pow(p1, m.toDouble()) *
                        Math.pow(p2, (n - m).toDouble()) /
                        denominator
            }
        }
        val sign = if ((n + 1) % 2 == 0) 1.0 else -1.0
        Xmax += sign * innerSum
    }
    return Xmax
}

/**
 * Result container for expected maximum of normal distribution.
 *
 * @property Xmax Expected value of maximum
 * @property Vmax Variance of maximum
 */
data class FJXmaxNormalResult(val Xmax: Double, val Vmax: Double)

/**
 * Expected maximum for normal distribution.
 *
 * Johnson et al. approximation:
 *   E[Y_K] ~ mu + sigma * [sqrt(2*ln(K)) - (ln(ln(K)) - ln(4*pi) + 2*gamma) / (2*sqrt(2*ln(K)))]
 *
 * Arnold approximation:
 *   E[Y_K] ~ mu + sigma * sqrt(2*ln(K))
 *
 * Corrected version adds Petzold bias correction: delta(K) = 0.1727 * K^(-0.2750)
 *
 * Variance approximation: Var[Y_K] ~ 1.64492 * sigma^2 / (2*ln(K))
 *
 * @param K      Number of random variables (K >= 2)
 * @param mu     Mean of the normal distribution
 * @param sigma  Standard deviation
 * @param method "johnson" (default), "arnold", or "corrected"
 * @return FJXmaxNormalResult containing expected maximum and variance
 *
 * @throws IllegalArgumentException if K < 2, sigma < 0, or unknown method
 *
 * Reference: Thomasian, Eq. (45) on page 17:23.
 * Johnson, Kotz, Balakrishnan, "Continuous Univariate Distributions", Vol. 2, 1995.
 */
fun fj_xmax_normal(K: Int, mu: Double, sigma: Double, method: String = "johnson"): FJXmaxNormalResult {
    if (K < 2) {
        throw IllegalArgumentException("K must be at least 2 for normal approximation. Got K=$K.")
    }
    if (sigma < 0.0) {
        throw IllegalArgumentException("Standard deviation sigma must be non-negative.")
    }

    val gammaEM = 0.5772156649015329 // Euler-Mascheroni constant
    val sqrt2lnK = Math.sqrt(2.0 * Math.log(K.toDouble()))

    val GK: Double
    when (method.lowercase()) {
        "arnold" -> {
            GK = sqrt2lnK
        }
        "johnson" -> {
            val correction = (Math.log(Math.log(K.toDouble())) - Math.log(4.0 * Math.PI) + 2.0 * gammaEM) / (2.0 * sqrt2lnK)
            GK = sqrt2lnK - correction
        }
        "corrected" -> {
            val correction = (Math.log(Math.log(K.toDouble())) - Math.log(4.0 * Math.PI) + 2.0 * gammaEM) / (2.0 * sqrt2lnK)
            val deltaK = 0.1727 * Math.pow(K.toDouble(), -0.2750)
            GK = sqrt2lnK - correction - deltaK
        }
        else -> {
            throw IllegalArgumentException("Unknown method: $method. Valid: johnson, arnold, corrected.")
        }
    }

    val Xmax = mu + sigma * GK
    val Vmax = 1.64492 * sigma * sigma / (2.0 * Math.log(K.toDouble()))

    return FJXmaxNormalResult(Xmax, Vmax)
}

/**
 * Result container for expected maximum of Pareto distribution.
 *
 * @property Xmax Expected value of maximum
 * @property MK   Characteristic maximum
 */
data class FJXmaxParetoResult(val Xmax: Double, val MK: Double)

/**
 * Expected maximum for Pareto distribution.
 *
 * Pareto CDF: F(x) = 1 - (k / (k + x))^beta, x >= 0, beta > 2
 *
 * For the standardized Pareto with mean 1: k = beta - 1
 *
 * @param K    Number of random variables (positive integer)
 * @param beta Shape parameter (beta > 2 required for finite moments)
 * @param k    Scale parameter (default: beta - 1 for mean = 1)
 * @return FJXmaxParetoResult containing expected maximum and characteristic maximum
 *
 * @throws IllegalArgumentException if K < 1, beta <= 2, or k <= 0
 *
 * Reference: Thomasian, page 17:24.
 */
fun fj_xmax_pareto(K: Int, beta: Double, k: Double): Double {
    if (K < 1) {
        throw IllegalArgumentException("K must be a positive integer. Got K=$K.")
    }
    if (beta <= 2.0) {
        throw IllegalArgumentException("Shape parameter beta must be > 2 for finite moments. Got beta=${"%.4f".format(beta)}.")
    }
    if (k <= 0.0) {
        throw IllegalArgumentException("Scale parameter k must be positive.")
    }

    // Numerical integration: E[Y_K] = integral_0^inf [1 - F(x)^K] dx
    val upperLimit = k * Math.pow(K.toDouble(), 2.0 / beta) * 10.0

    // Composite Simpson's rule
    val numPoints = 10001
    val h = upperLimit / (numPoints - 1)
    var integral = 0.0
    for (i in 0 until numPoints) {
        val x = i * h
        val F = 1.0 - Math.pow(k / (k + Math.max(x, 0.0)), beta)
        val integrandVal = 1.0 - Math.pow(F, K.toDouble())

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

/**
 * Expected maximum for Pareto distribution with default scale for mean = 1.
 *
 * @param K    Number of random variables (positive integer)
 * @param beta Shape parameter (beta > 2)
 * @return Expected maximum
 */
fun fj_xmax_pareto(K: Int, beta: Double): Double {
    return fj_xmax_pareto(K, beta, beta - 1.0)
}

/**
 * Characteristic maximum M_K for Pareto distribution.
 *
 * m_K = k * (K^(1/beta) - 1)
 * M_K = m_K + K * integral_{m_K}^{inf} S(x) dx
 *
 * For Pareto: integral_{m_K}^{inf} (k/(k+x))^beta dx = k^beta * (k+m_K)^{1-beta} / (beta-1)
 *
 * @param K    Number of random variables (positive integer)
 * @param beta Shape parameter (beta > 2)
 * @param k    Scale parameter (default: beta - 1)
 * @return Characteristic maximum M_K
 */
fun fj_xmax_pareto_char_max(K: Int, beta: Double, k: Double): Double {
    if (K < 1) {
        throw IllegalArgumentException("K must be a positive integer. Got K=$K.")
    }
    if (beta <= 1.0) {
        return Double.POSITIVE_INFINITY
    }

    val mK = k * (Math.pow(K.toDouble(), 1.0 / beta) - 1.0)

    // For Pareto: tail integral = k^beta * (k+m_K)^{1-beta} / (beta-1)
    val tailIntegral = Math.pow(k, beta) * Math.pow(k + mK, 1.0 - beta) / (beta - 1.0)
    return mK + K * tailIntegral
}

/**
 * General approximation for expected maximum of K random variables.
 *
 *   X_K^max ~ mu_X + sigma_X * G(K)
 *
 * G(K) depends on distribution type:
 *   - Exponential: G(K) = H_K - 1
 *   - Uniform:     G(K) = sqrt(3) * (K-1) / (K+1)
 *   - EVD:         G(K) = sqrt(6) * ln(K) / pi
 *   - General:     G(K) <= (K-1) / sqrt(2K-1) (upper bound)
 *
 * @param K        Number of random variables (positive integer)
 * @param muX      Mean of the distribution
 * @param sigmaX   Standard deviation of the distribution
 * @param distType Distribution type: "exp", "uniform", "evd", "bound"
 * @return Expected maximum X_K^max
 *
 * @throws IllegalArgumentException if K < 1, sigmaX < 0, or unknown dist_type
 *
 * Reference: Thomasian, Eq. (22) and (23) on page 17:16.
 * David, "Order Statistics", Wiley, 1970.
 */
fun fj_xmax_approx(K: Int, muX: Double, sigmaX: Double, distType: String = "exp"): Double {
    if (K < 1) {
        throw IllegalArgumentException("K must be a positive integer. Got K=$K.")
    }
    if (sigmaX < 0.0) {
        throw IllegalArgumentException("Standard deviation sigma_X must be non-negative.")
    }

    val GK: Double = when (distType.lowercase()) {
        "exp" -> fj_harmonic(K) - 1.0
        "uniform" -> Math.sqrt(3.0) * (K - 1) / (K + 1)
        "evd" -> Math.sqrt(6.0) * Math.log(K.toDouble()) / Math.PI
        "bound" -> (K - 1).toDouble() / Math.sqrt(2.0 * K - 1.0)
        else -> throw IllegalArgumentException("Unknown distribution type: $distType. Valid: exp, uniform, evd, bound.")
    }

    return muX + sigmaX * GK
}

/**
 * Expected maximum using EMMA (Expected Maximum from Marginal Approximation).
 *
 * Based on: [F_X(E[Y_K])]^K ~ phi = 0.570376
 * Therefore: E[Y_K] ~ F^{-1}(phi^{1/K})
 *
 * For exponential with rate mu:
 *   E[Y_K] = -(1/mu) * ln(1 - phi^{1/K})
 *
 * The constant phi = exp(-exp(-gamma)) ~ 0.570376.
 *
 * @param K  Number of random variables (positive integer)
 * @param mu Rate parameter for exponential distribution
 * @return Approximate expected maximum
 *
 * @throws IllegalArgumentException if K < 1 or mu <= 0
 *
 * Reference: Thomasian, Eq. (44) on page 17:22.
 */
fun fj_xmax_emma(K: Int, mu: Double): Double {
    val PHI = 0.570376

    if (K < 1) {
        throw IllegalArgumentException("K must be a positive integer. Got K=$K.")
    }
    if (mu <= 0.0) {
        throw IllegalArgumentException("Rate mu must be positive.")
    }

    return -(1.0 / mu) * Math.log(1.0 - Math.pow(PHI, 1.0 / K))
}

/**
 * Erlang CDF for general k stages.
 */
private fun erlangCdfGeneral(t: Double, k: Int, mu: Double): Double {
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
 * Fork-Join expected maximum algorithms
 */
@Suppress("unused")
class FJXmaxAlgo {
    companion object {
        // Class documentation marker for Dokka
    }
}
