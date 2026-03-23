/**
 * @file Characteristic maximum M_K for order statistics in Fork-Join analysis
 *
 * Computes the characteristic maximum M_K, which provides a bound for the
 * expected value of the maximum of K i.i.d. random variables. The threshold
 * m_K is the greatest lower bound such that P(X > m_K) <= 1/K, and:
 *
 *   M_K = m_K + K * integral_{m_K}^{inf} P(X > x) dx
 *
 * @since LINE 3.0
 */
package jline.api.fj

import jline.util.Maths

/**
 * Result container for characteristic maximum.
 *
 * @property MK Characteristic maximum M_K
 * @property mK Threshold m_K where P(X > m_K) = 1/K
 */
data class CharMaxResult(@JvmField val MK: Double, @JvmField val mK: Double)

/**
 * Characteristic maximum M_K for exponential distribution.
 *
 *   m_K = ln(K) / mu
 *   M_K = H_K / mu (exact for exponential)
 *
 * @param K  Number of random variables (positive integer)
 * @param mu Rate parameter (positive)
 * @return CharMaxResult with M_K and m_K
 *
 * @throws IllegalArgumentException if K < 1 or mu <= 0
 *
 * Reference: Thomasian, Eq. (47) on page 17:24.
 */
fun fj_char_max_exp(K: Int, mu: Double): CharMaxResult {
    if (K < 1) {
        throw IllegalArgumentException("K must be a positive integer. Got K=$K.")
    }
    if (mu <= 0.0) {
        throw IllegalArgumentException("Rate mu must be positive.")
    }

    val mK = Math.log(K.toDouble()) / mu
    val MK = fj_harmonic(K) / mu

    return CharMaxResult(MK, mK)
}

/**
 * Characteristic maximum M_K for Erlang-k distribution.
 *
 * Finds m_K by solving: exp(-mu*m_K) * sum_{i=0}^{k-1} (mu*m_K)^i/i! = 1/K
 *
 * Then: M_K = (k/mu) * [1 + K * exp(-mu*m_K) * (mu*m_K)^k / k!]
 *
 * @param K       Number of random variables (positive integer)
 * @param kStages Number of Erlang stages (positive integer)
 * @param mu      Rate parameter per stage (positive)
 * @return CharMaxResult with M_K and m_K
 *
 * @throws IllegalArgumentException if parameters are invalid
 *
 * Reference: Thomasian, Eq. (47) on page 17:24.
 */
fun fj_char_max_erlang(K: Int, kStages: Int, mu: Double): CharMaxResult {
    if (K < 1) {
        throw IllegalArgumentException("K must be a positive integer. Got K=$K.")
    }
    if (kStages < 1) {
        throw IllegalArgumentException("Erlang stages must be positive.")
    }
    if (mu <= 0.0) {
        throw IllegalArgumentException("Rate mu must be positive.")
    }

    // Find m_K by bisection: solve S_erlang(m_K) = 1/K
    val target = 1.0 / K
    var lo = 0.0
    var hi = kStages.toDouble() / mu + Math.log(K.toDouble()) / mu
    // Expand hi until survival function at hi is below target
    while (erlangSurvival(hi, kStages, mu) > target) {
        hi *= 2.0
    }

    // Bisection search
    for (iter in 0 until 200) {
        val mid = (lo + hi) / 2.0
        val sVal = erlangSurvival(mid, kStages, mu)
        if (Math.abs(sVal - target) < 1e-12) {
            break
        }
        if (sVal > target) {
            lo = mid
        } else {
            hi = mid
        }
    }
    val mK = (lo + hi) / 2.0

    // M_K = (k/mu) * [1 + K * exp(-mu*m_K) * (mu*m_K)^k / k!]
    val MK = (kStages.toDouble() / mu) * (1.0 + K * Math.exp(-mu * mK) *
            Math.pow(mu * mK, kStages.toDouble()) / Maths.fact(kStages.toDouble()))

    return CharMaxResult(MK, mK)
}

/**
 * Erlang survival function: P(X > x) = exp(-mu*x) * sum_{i=0}^{k-1} (mu*x)^i / i!
 */
private fun erlangSurvival(x: Double, k: Int, mu: Double): Double {
    if (x <= 0.0) {
        return 1.0
    }
    var S = 0.0
    for (i in 0 until k) {
        S += Math.pow(mu * x, i.toDouble()) / Maths.fact(i.toDouble())
    }
    return Math.exp(-mu * x) * S
}

/**
 * Fork-Join characteristic maximum algorithms
 */
@Suppress("unused")
class FJCharMaxAlgo {
    companion object {
        // Class documentation marker for Dokka
    }
}
