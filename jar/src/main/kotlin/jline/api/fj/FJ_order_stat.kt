/**
 * @file Order statistics and G(K) bound factors for Fork-Join analysis
 *
 * Computes order statistics (CDF and expected value of the k-th smallest of K
 * i.i.d. random variables) and the G(K) scaling factors used in the
 * mean-variance approximation X_K^max ~ mu + sigma * G(K).
 *
 * @since LINE 3.0
 */
package jline.api.fj

import jline.util.Maths

/**
 * Result container for G(K) bound factors.
 *
 * @property K            Number of random variables
 * @property exponential  G(K) for exponential distribution
 * @property uniform      G(K) for uniform distribution
 * @property evd          G(K) for extreme value distribution
 * @property upperBound   Upper bound valid for any distribution
 */
data class GKBoundResult(
    val K: Int,
    val exponential: Double,
    val uniform: Double,
    val evd: Double,
    val upperBound: Double
)

/**
 * Compute G(K) factors for expected maximum approximation.
 *
 * G(K) is used in: X_K^max ~ mu_X + sigma_X * G(K)
 *
 * Available formulas:
 *   - Exponential: G(K) = H_K - 1
 *   - Uniform:     G(K) = sqrt(3) * (K-1) / (K+1)
 *   - EVD:         G(K) = sqrt(6) * ln(K) / pi
 *   - Upper bound: G(K) <= (K-1) / sqrt(2K-1)
 *
 * @param K    Number of random variables (positive integer)
 * @param type "exp", "uniform", "evd", "bound", or "all" (default)
 * @return G(K) value for specified type, or -1 if "all" (use fj_gk_bound_all)
 *
 * @throws IllegalArgumentException if K < 1 or unknown type
 *
 * Reference: Thomasian, Eq. (22) and (23) on page 17:16.
 * David, "Order Statistics", Wiley, 1970.
 */
fun fj_gk_bound(K: Int, type: String = "exp"): Double {
    if (K < 1) {
        throw IllegalArgumentException("K must be a positive integer. Got K=$K.")
    }

    return when (type.lowercase()) {
        "exp" -> fj_harmonic(K) - 1.0
        "uniform" -> Math.sqrt(3.0) * (K - 1) / (K + 1)
        "evd" -> Math.sqrt(6.0) * Math.log(K.toDouble()) / Math.PI
        "bound" -> (K - 1).toDouble() / Math.sqrt(2.0 * K - 1.0)
        else -> throw IllegalArgumentException("Unknown type: $type. Valid: exp, uniform, evd, bound.")
    }
}

/**
 * Compute all G(K) bound factors.
 *
 * @param K Number of random variables (positive integer)
 * @return GKBoundResult with all G(K) values
 *
 * @throws IllegalArgumentException if K < 1
 */
fun fj_gk_bound_all(K: Int): GKBoundResult {
    if (K < 1) {
        throw IllegalArgumentException("K must be a positive integer. Got K=$K.")
    }

    return GKBoundResult(
        K = K,
        exponential = fj_harmonic(K) - 1.0,
        uniform = Math.sqrt(3.0) * (K - 1) / (K + 1),
        evd = Math.sqrt(6.0) * Math.log(K.toDouble()) / Math.PI,
        upperBound = (K - 1).toDouble() / Math.sqrt(2.0 * K - 1.0)
    )
}

/**
 * CDF of k-th order statistic of K i.i.d. random variables.
 *
 * For maximum (k=K): F_{Y_K}(y) = [F_X(y)]^K
 *
 * For k-th order statistic (k-th smallest):
 *   F_{Y_k}(y) = sum_{j=k}^{K} C(K,j) * F_X(y)^j * (1-F_X(y))^{K-j}
 *
 * @param FXy  CDF value F_X(y) at point y
 * @param k    Order of the statistic (1 = minimum, K = maximum)
 * @param K    Total number of random variables
 * @return CDF of k-th order statistic F_{Y_k}(y)
 *
 * @throws IllegalArgumentException if k is out of range
 *
 * Reference: Thomasian, Eq. (18) on page 17:15.
 */
fun fj_order_stat_cdf(FXy: Double, k: Int, K: Int): Double {
    if (k < 1 || k > K) {
        throw IllegalArgumentException("k must satisfy 1 <= k <= K. Got k=$k, K=$K.")
    }

    if (k == K) {
        // Maximum (K-th order statistic)
        return Math.pow(FXy, K.toDouble())
    }

    // General k-th order statistic
    var FYk = 0.0
    for (j in k..K) {
        FYk += Maths.binomialCoeff(K, j) *
                Math.pow(FXy, j.toDouble()) *
                Math.pow(1.0 - FXy, (K - j).toDouble())
    }
    return FYk
}

/**
 * Expected value of the maximum of K i.i.d. random variables
 * via numerical integration.
 *
 *   E[Y_K] = integral_0^inf [1 - F_X(y)^K] dy
 *
 * @param K          Total number of random variables
 * @param cdfFunc    CDF function F_X(y) -- a lambda/function that takes a Double and returns Double
 * @param upperLimit Upper integration limit (where CDF is very close to 1)
 * @return Expected value E[Y_K]
 *
 * @throws IllegalArgumentException if K < 1
 *
 * Reference: Thomasian, Eq. (19) on page 17:15.
 */
fun fj_order_stat_expected_max(K: Int, cdfFunc: (Double) -> Double, upperLimit: Double): Double {
    if (K < 1) {
        throw IllegalArgumentException("K must be a positive integer. Got K=$K.")
    }

    // Composite Simpson's rule
    val numPoints = 10001
    val h = upperLimit / (numPoints - 1)
    var integral = 0.0
    for (i in 0 until numPoints) {
        val t = i * h
        val cdfVal = cdfFunc(t)
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

/**
 * Expected value of the minimum of K i.i.d. random variables
 * via numerical integration.
 *
 *   E[Y_1] = integral_0^inf [1 - F_X(y)]^K dy
 *
 * @param K          Total number of random variables
 * @param cdfFunc    CDF function F_X(y)
 * @param upperLimit Upper integration limit
 * @return Expected value E[Y_1]
 *
 * @throws IllegalArgumentException if K < 1
 */
fun fj_order_stat_expected_min(K: Int, cdfFunc: (Double) -> Double, upperLimit: Double): Double {
    if (K < 1) {
        throw IllegalArgumentException("K must be a positive integer. Got K=$K.")
    }

    // Composite Simpson's rule
    val numPoints = 10001
    val h = upperLimit / (numPoints - 1)
    var integral = 0.0
    for (i in 0 until numPoints) {
        val t = i * h
        val cdfVal = cdfFunc(t)
        val integrandVal = Math.pow(1.0 - cdfVal, K.toDouble())

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
 * Fork-Join order statistics algorithms
 */
@Suppress("unused")
class FJOrderStatAlgo {
    companion object {
        // Class documentation marker for Dokka
    }
}
