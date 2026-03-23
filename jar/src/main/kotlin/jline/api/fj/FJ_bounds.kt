/**
 * @file Upper and lower bounds for K-way Fork-Join response time
 *
 * Computes pessimistic (upper) and optimistic (lower) bounds for the mean response
 * time of a K-way Fork-Join queueing system with Poisson arrivals and exponential
 * service times.
 *
 * Upper bound: R_K^max(rho) = H_K / (mu * (1 - rho))
 * Lower bound: R_K^{F/J(opt)}(rho) = (1/mu) * [H_K + S_{K(K-rho)}]
 *
 * @since LINE 3.0
 */
package jline.api.fj

/**
 * Result container for Fork-Join bounds.
 *
 * @property Rmax Upper bound (pessimistic)
 * @property Rmin Lower bound (optimistic)
 */
data class FJBoundsResult(val Rmax: Double, val Rmin: Double)

/**
 * Compute upper and lower bounds for K-way F/J response time.
 *
 * @param K      Number of parallel servers (positive integer)
 * @param lambda Arrival rate
 * @param mu     Service rate (mu > lambda for stability)
 * @return FJBoundsResult containing upper and lower bounds
 *
 * @throws IllegalArgumentException if K < 1 or system is unstable
 *
 * Reference: A. Thomasian, "Analysis of Fork/Join and Related Queueing Systems",
 * ACM Computing Surveys, Vol. 47, No. 2, Article 17, July 2014.
 * Eq. (1) and Eq. (2) on page 17:9.
 */
fun fj_bounds(K: Int, lambda: Double, mu: Double): FJBoundsResult {
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

    // Upper bound (Eq. 1): R_K^max(rho) = H_K / (mu * (1 - rho))
    val Rmax = H_K / (mu * (1 - rho))

    // Lower bound (Eq. 2): R_K^{F/J(opt)}(rho) = (1/mu) * [H_K + S_{K(K-rho)}]
    // where S_{K(K-rho)} = sum_{j=1}^{K} (1/j) * (rho/(j - rho))
    var S_K = 0.0
    for (j in 1..K) {
        S_K += (1.0 / j) * (rho / (j - rho))
    }
    val Rmin = (1.0 / mu) * (H_K + S_K)

    return FJBoundsResult(Rmax, Rmin)
}

/**
 * Fork-Join bounds algorithms
 */
@Suppress("unused")
class FJBoundsAlgo {
    companion object {
        // Class documentation marker for Dokka
    }
}
