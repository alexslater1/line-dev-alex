/**
 * @file Fork-Join response time approximations
 *
 * Implements multiple approximation methods for the mean response time of K-way
 * Fork-Join queueing systems with Poisson arrivals and exponential service times.
 *
 * Methods included:
 * - Exact 2-way (Flatto-Hahn)
 * - Nelson-Tantawi approximation
 * - Varma-Makowski interpolation
 * - Varki et al. approximation
 *
 * @since LINE 3.0
 */
package jline.api.fj

import jline.util.Maths

/**
 * Exact two-way Fork-Join response time R_2^{F/J}(rho).
 *
 * Derived from Flatto and Hahn [1984]:
 *   R_2^{F/J}(rho) = (H_2 - rho/8) * R(rho) = (12 - rho)/8 * R(rho)
 *
 * where H_2 = 1.5 and R(rho) = 1/(mu - lambda) is the M/M/1 mean response time.
 *
 * @param lambda Arrival rate
 * @param mu     Service rate (mu > lambda for stability)
 * @return Exact 2-way F/J response time
 *
 * @throws IllegalArgumentException if system is unstable
 *
 * Reference: Thomasian, Eq. (6) on page 17:10.
 * Flatto and Hahn, SIAM J. Appl. Math., 44(5), 1984.
 */
fun fj_respt_2way(lambda: Double, mu: Double): Double {
    val rho = lambda / mu
    if (rho >= 1.0) {
        throw IllegalArgumentException(
            "System is unstable: rho = lambda/mu = ${"%.4f".format(rho)} >= 1. Require lambda < mu."
        )
    }

    val R_rho = 1.0 / (mu - lambda)
    return (12.0 - rho) / 8.0 * R_rho
}

/**
 * Nelson-Tantawi approximation for K-way F/J response time.
 *
 * Valid for 2 <= K <= 32:
 *   R_K^{F/J}(rho) ~ [H_K/H_2 + (1 - H_K/H_2) * 4*rho/11] * (1.5 - rho/8) / (mu - lambda)
 *
 * @param K      Number of parallel servers (2 <= K <= 32)
 * @param lambda Arrival rate
 * @param mu     Service rate (mu > lambda for stability)
 * @return Approximate K-way F/J response time
 *
 * @throws IllegalArgumentException if K < 2 or system is unstable
 *
 * Reference: Thomasian, Eq. (3) on page 17:10.
 * Nelson and Tantawi, IEEE Trans. Computers, 37(6), 1988.
 */
fun fj_respt_nt(K: Int, lambda: Double, mu: Double): Double {
    if (K < 2) {
        throw IllegalArgumentException("Nelson-Tantawi approximation requires K >= 2. Got K=$K.")
    }

    val rho = lambda / mu
    if (rho >= 1.0) {
        throw IllegalArgumentException(
            "System is unstable: rho = lambda/mu = ${"%.4f".format(rho)} >= 1. Require lambda < mu."
        )
    }

    val H_K = fj_harmonic(K)
    val H_2 = fj_harmonic(2) // H_2 = 1.5

    val S_K = H_K / H_2 + (1.0 - H_K / H_2) * (4.0 * rho / 11.0)
    val R_2_factor = 1.5 - rho / 8.0
    val R_rho = 1.0 / (mu - lambda)

    return S_K * R_2_factor * R_rho
}

/**
 * Varma-Makowski approximation for K-way F/J response time.
 *
 *   R_K^{F/J}(rho) ~ [H_K + (A_K - H_K) * rho] * (mu - lambda)^{-1}
 *
 * where A_K = sum_{i=1}^{K} C(K,i) * (-1)^{i-1} * sum_{m=1}^{i} C(i,m) * (m-1)! / i^{m+1}
 *
 * @param K      Number of parallel servers (positive integer)
 * @param lambda Arrival rate
 * @param mu     Service rate (mu > lambda for stability)
 * @return Approximate K-way F/J response time
 *
 * @throws IllegalArgumentException if K < 1 or system is unstable
 *
 * Reference: Thomasian, Eq. (4) on page 17:10.
 * Varma and Makowski, Performance Evaluation, 20, 1994.
 */
fun fj_respt_vm(K: Int, lambda: Double, mu: Double): Double {
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

    // Compute A_K
    var A_K = 0.0
    for (i in 1..K) {
        var innerSum = 0.0
        for (m in 1..i) {
            innerSum += Maths.binomialCoeff(i, m) * Maths.fact(m - 1) / Math.pow(i.toDouble(), (m + 1).toDouble())
        }
        val sign = if ((i - 1) % 2 == 0) 1.0 else -1.0
        A_K += Maths.binomialCoeff(K, i) * sign * innerSum
    }

    val R_rho = 1.0 / (mu - lambda)
    return (H_K + (A_K - H_K) * rho) * R_rho
}

/**
 * Varki et al. approximation for K-way F/J response time.
 *
 * Mean of pessimistic (upper) and optimistic (lower) bounds:
 *   R_K^{F/J}(rho) ~ (1/mu) * [H_K + (rho/(2*(1-rho))) * (S1 + (1-2*rho)*S2)]
 *
 * where:
 *   S1 = sum_{i=1}^{K} 1/(i - rho)
 *   S2 = sum_{i=1}^{K} 1/(i*(i - rho))
 *
 * @param K      Number of parallel servers (positive integer)
 * @param lambda Arrival rate
 * @param mu     Service rate (mu > lambda for stability)
 * @return Approximate K-way F/J response time
 *
 * @throws IllegalArgumentException if K < 1 or system is unstable
 *
 * Reference: Thomasian, Eq. (5) on page 17:10.
 * Varki et al., IEEE TPDS, 25(6), 2014.
 */
fun fj_respt_varki(K: Int, lambda: Double, mu: Double): Double {
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

    // S1 = sum_{i=1}^{K} 1/(i - rho)
    var S1 = 0.0
    for (i in 1..K) {
        S1 += 1.0 / (i - rho)
    }

    // S2 = sum_{i=1}^{K} 1/(i*(i - rho))
    var S2 = 0.0
    for (i in 1..K) {
        S2 += 1.0 / (i.toDouble() * (i - rho))
    }

    return (1.0 / mu) * (H_K + (rho / (2.0 * (1.0 - rho))) * (S1 + (1.0 - 2.0 * rho) * S2))
}

/**
 * Fork-Join response time algorithms
 */
@Suppress("unused")
class FJResptAlgo {
    companion object {
        // Class documentation marker for Dokka
    }
}
