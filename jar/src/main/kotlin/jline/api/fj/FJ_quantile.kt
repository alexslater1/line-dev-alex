/**
 * @file Quantile approximation for maximum of K random variables
 *
 * Approximates the q-th quantile of the maximum of K i.i.d. random variables
 * using the standard Gumbel (Type I extreme value) distribution or a general
 * inverse CDF approach.
 *
 * @since LINE 3.0
 */
package jline.api.fj

/**
 * Quantile approximation for maximum of K random variables.
 *
 * Standard Gumbel approximation:
 *   x(K,q) ~ ln(K) - ln(ln(1/q))
 *
 * Note: This approximation is inaccurate for small values of K.
 *
 * @param K Number of random variables (positive integer)
 * @param q Quantile probability (0 < q < 1)
 * @return q-th quantile of the maximum of K random variables
 *
 * @throws IllegalArgumentException if K < 1 or q is out of range
 *
 * Reference: Thomasian, page 17:22.
 */
fun fj_quantile(K: Int, q: Double): Double {
    if (K < 1) {
        throw IllegalArgumentException("K must be a positive integer. Got K=$K.")
    }
    if (q <= 0.0 || q >= 1.0) {
        throw IllegalArgumentException("Quantile q must satisfy 0 < q < 1. Got q=${"%.4f".format(q)}.")
    }

    // Standard Gumbel approximation: x(K,q) = ln(K) - ln(ln(1/q))
    return Math.log(K.toDouble()) - Math.log(Math.log(1.0 / q))
}

/**
 * Quantile of maximum of K random variables using inverse CDF.
 *
 * For general distributions:
 *   The CDF of the maximum is F_max(x) = [F(x)]^K
 *   So the q-th quantile satisfies: F(x) = q^{1/K}
 *   Therefore: x = F^{-1}(q^{1/K})
 *
 * @param K    Number of random variables (positive integer)
 * @param q    Quantile probability (0 < q < 1)
 * @param Finv Inverse CDF (quantile function) of the base distribution
 * @return q-th quantile of the maximum
 *
 * @throws IllegalArgumentException if K < 1 or q is out of range
 *
 * Reference: Thomasian, page 17:22.
 */
fun fj_quantile(K: Int, q: Double, Finv: (Double) -> Double): Double {
    if (K < 1) {
        throw IllegalArgumentException("K must be a positive integer. Got K=$K.")
    }
    if (q <= 0.0 || q >= 1.0) {
        throw IllegalArgumentException("Quantile q must satisfy 0 < q < 1. Got q=${"%.4f".format(q)}.")
    }

    return Finv(Math.pow(q, 1.0 / K))
}

/**
 * Quantile approximation for maximum of K random variables (array version).
 *
 * @param K Number of random variables (positive integer)
 * @param q Array of quantile probabilities (each 0 < q_i < 1)
 * @return Array of q-th quantiles
 */
fun fj_quantile(K: Int, q: DoubleArray): DoubleArray {
    val result = DoubleArray(q.size)
    for (i in q.indices) {
        result[i] = fj_quantile(K, q[i])
    }
    return result
}

/**
 * Fork-Join quantile algorithms
 */
@Suppress("unused")
class FJQuantileAlgo {
    companion object {
        // Class documentation marker for Dokka
    }
}
