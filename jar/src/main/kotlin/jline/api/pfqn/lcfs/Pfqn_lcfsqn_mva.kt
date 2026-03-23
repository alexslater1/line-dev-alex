/**
 * Mean Value Analysis for LCFS Queueing Networks
 *
 * Implements exact MVA for 2-station LCFS queueing networks with
 * LCFS and LCFS-PR scheduling disciplines.
 *
 * This implementation uses log-space arithmetic to prevent numerical
 * underflow. The results are mathematically exact (up to floating-point
 * precision) - no approximations are made.
 *
 * Reference:
 *   G. Casale, "A family of multiclass LCFS queueing networks with
 *   order-dependent product-form solutions", QUESTA 2026.
 *
 * @since LINE 3.0
 */
package jline.api.pfqn.lcfs

import jline.util.PopulationLattice
import jline.util.matrix.Matrix
import kotlin.math.exp
import kotlin.math.ln
import kotlin.math.max

/**
 * Result class for LCFS MVA algorithm
 *
 * @property T Throughput vector (1 x R)
 * @property Q Queue length matrix (2 x R)
 * @property U Utilization matrix (2 x R)
 * @property B Back probability matrix (2 x R)
 */
class LcfsqnMvaResult(
    val T: Matrix,
    val Q: Matrix,
    val U: Matrix,
    val B: Matrix
)

/**
 * Mean Value Analysis for multiclass LCFS queueing networks.
 *
 * This function computes performance metrics for a 2-station closed
 * queueing network with:
 *   - Station 1: LCFS (Last-Come-First-Served, non-preemptive)
 *   - Station 2: LCFS-PR (LCFS with Preemption-Resume)
 *
 * @param alpha Vector of inverse service rates at station 1 (LCFS).
 *              alpha(r) = 1/mu(1,r) for class r
 * @param beta Vector of inverse service rates at station 2 (LCFS-PR).
 *             beta(r) = 1/mu(2,r) for class r
 * @param N Population vector, N(r) = number of jobs of class r.
 *          Default: ones(1,R) - one job per class
 * @return LcfsqnMvaResult containing throughputs, queue lengths, utilizations, and back probabilities
 */
fun pfqn_lcfsqn_mva(alpha: Matrix, beta: Matrix, N: Matrix? = null): LcfsqnMvaResult {
    val R = alpha.length()
    val populationVector = N ?: Matrix.ones(1, R)
    val K = populationVector.elementSum().toInt()

    if (K == 0) {
        return LcfsqnMvaResult(
            Matrix(1, R),
            Matrix(2, R),
            Matrix(2, R),
            Matrix(2, R)
        )
    }

    // Precompute log values for numerical stability
    val logAlpha = DoubleArray(R) { ln(alpha.get(it)) }
    val logBeta = DoubleArray(R) { ln(beta.get(it)) }

    // Precompute products for hash computation
    val prods = Matrix(1, R)
    for (r in 0 until R) {
        var prod = 1.0
        for (j in 0 until r) {
            prod *= (populationVector.get(j) + 1)
        }
        prods.set(r, prod)
    }

    // Calculate total size for arrays
    var totalSize = 1
    for (r in 0 until R) {
        totalSize *= (populationVector.get(r).toInt() + 1)
    }

    // Storage for intermediate results
    val QN = Array(totalSize) { Matrix(2, R) }
    val TN = Array(totalSize) { Matrix(1, R) }
    // Store B in log-scaled form: B_actual = BN_scaled * exp(log_scale_BN)
    val BN_scaled = Array(totalSize) { Matrix(2, R) }
    val log_scale_BN = Array(totalSize) { Array(2) { DoubleArray(R) { Double.NEGATIVE_INFINITY } } }

    // Initialize population iterator
    var n = PopulationLattice.pprod(populationVector)

    while (n.get(0) >= 0) {
        val idx = hashpopLcfs(n, populationVector, R, prods)
        val sumN = n.elementSum().toInt()

        if (sumN == 1) {
            // Base case: single job - no scaling needed
            for (r in 0 until R) {
                if (n.get(r) > 0) {
                    val denom = alpha.get(r) + beta.get(r)
                    QN[idx].set(0, r, alpha.get(r) / denom)
                    QN[idx].set(1, r, beta.get(r) / denom)
                    // At n=1, B = Q directly (no prod(alpha.^n) factor)
                    BN_scaled[idx].set(0, r, alpha.get(r) / denom)
                    BN_scaled[idx].set(1, r, beta.get(r) / denom)
                    log_scale_BN[idx][0][r] = 0.0  // exp(0) = 1, so actual = scaled
                    log_scale_BN[idx][1][r] = 0.0
                    TN[idx].set(0, r, 1.0 / denom)
                }
            }
        } else if (sumN > 1) {
            // Recursive case: multiple jobs
            // log_scale_np = log(prod(alpha.^n))
            var log_scale_np = 0.0
            for (r in 0 until R) {
                log_scale_np += n.get(r) * logAlpha[r]
            }

            for (k in 0 until R) {
                if (n.get(k) > 0) {
                    val n_minus_k = oner(n, k)
                    val idx_k = hashpopLcfs(n_minus_k, populationVector, R, prods)

                    // Compute unscaled waiting time contributions
                    var Wnp_unscaled = 1.0 + QN[idx_k].get(0, k)
                    var Wpr_unscaled = 1.0 + QN[idx_k].get(1, k)

                    for (r in 0 until R) {
                        if (r != k && n.get(r) > 0) {
                            val n_minus_r = oner(n, r)
                            val idx_r = hashpopLcfs(n_minus_r, populationVector, R, prods)

                            // Compute B ratio in log-space
                            if (BN_scaled[idx_k].get(0, r) > 0 && BN_scaled[idx_r].get(0, k) > 0) {
                                val log_ratio_np = ln(BN_scaled[idx_k].get(0, r)) + log_scale_BN[idx_k][0][r] -
                                        ln(BN_scaled[idx_r].get(0, k)) - log_scale_BN[idx_r][0][k]
                                val ratio_np = exp(log_ratio_np)
                                Wnp_unscaled += (alpha.get(k) / alpha.get(r)) * ratio_np * QN[idx_r].get(0, k)
                            }

                            if (BN_scaled[idx_k].get(1, r) > 0 && BN_scaled[idx_r].get(1, k) > 0) {
                                val log_ratio_pr = ln(BN_scaled[idx_k].get(1, r)) + log_scale_BN[idx_k][1][r] -
                                        ln(BN_scaled[idx_r].get(1, k)) - log_scale_BN[idx_r][1][k]
                                val ratio_pr = exp(log_ratio_pr)
                                Wpr_unscaled += (alpha.get(r) / alpha.get(k)) * ratio_pr * QN[idx_r].get(1, k)
                            }
                        }
                    }

                    // log_scale_pr_k = log(alpha(k)^(sum(n)-1) * beta(k))
                    val log_scale_pr_k = (sumN - 1) * logAlpha[k] + logBeta[k]

                    // Compute Y(k) = n(k) / (Wnp + Wpr) using log-sum-exp
                    val log_Wnp = log_scale_np + ln(Wnp_unscaled)
                    val log_Wpr = log_scale_pr_k + ln(Wpr_unscaled)

                    // log-sum-exp for numerical stability
                    val max_log = max(log_Wnp, log_Wpr)
                    val log_sum_W = max_log + ln(exp(log_Wnp - max_log) + exp(log_Wpr - max_log))

                    // B(1,k) = prod(alpha.^n) * n(k) / (Wnp + Wpr)
                    BN_scaled[idx].set(0, k, n.get(k))
                    log_scale_BN[idx][0][k] = log_scale_np - log_sum_W

                    // B(2,k) = alpha(k)^(sum(n)-1) * beta(k) * n(k) / (Wnp + Wpr)
                    BN_scaled[idx].set(1, k, n.get(k))
                    log_scale_BN[idx][1][k] = log_scale_pr_k - log_sum_W
                }
            }

            // Obtain queue lengths: Q(i,k) = B(i,k) + sum_r B(i,r) * Q_{n-e_r}(i,k)
            for (k in 0 until R) {
                if (n.get(k) > 0) {
                    for (station in 0 until 2) {
                        val terms = mutableListOf<Double>()
                        val logScales = mutableListOf<Double>()

                        // First term: B(station,k)
                        if (BN_scaled[idx].get(station, k) > 0) {
                            terms.add(BN_scaled[idx].get(station, k))
                            logScales.add(log_scale_BN[idx][station][k])
                        }

                        // Sum over r: B(station,r) * Q_{n-e_r}(station,k)
                        for (r in 0 until R) {
                            if (n.get(r) > 0) {
                                val n_minus_r = oner(n, r)
                                val idx_r = hashpopLcfs(n_minus_r, populationVector, R, prods)
                                if (BN_scaled[idx].get(station, r) > 0 && QN[idx_r].get(station, k) > 0) {
                                    terms.add(BN_scaled[idx].get(station, r) * QN[idx_r].get(station, k))
                                    logScales.add(log_scale_BN[idx][station][r])
                                }
                            }
                        }

                        QN[idx].set(station, k, logsumexp(terms, logScales))
                    }
                }
            }

            // Obtain throughput
            for (k in 0 until R) {
                if (n.get(k) > 0) {
                    val n_minus_k = oner(n, k)
                    val idx_k = hashpopLcfs(n_minus_k, populationVector, R, prods)

                    // Compute Unp = sum_r alpha(r) * T_{n-e_k}(r)
                    var Unp = 0.0
                    for (r in 0 until R) {
                        Unp += alpha.get(r) * TN[idx_k].get(0, r)
                    }

                    // T(k) = sum_r B(1,r) * T_{n-e_r}(k) + (1/alpha(k)) * B(1,k) * (1-Unp)
                    val terms = mutableListOf<Double>()
                    val logScales = mutableListOf<Double>()

                    for (r in 0 until R) {
                        if (n.get(r) > 0) {
                            val n_minus_r = oner(n, r)
                            val idx_r = hashpopLcfs(n_minus_r, populationVector, R, prods)
                            if (BN_scaled[idx].get(0, r) > 0 && TN[idx_r].get(0, k) > 0) {
                                terms.add(BN_scaled[idx].get(0, r) * TN[idx_r].get(0, k))
                                logScales.add(log_scale_BN[idx][0][r])
                            }
                        }
                    }

                    // Add (1/alpha(k)) * B(1,k) * (1-Unp) term
                    if (BN_scaled[idx].get(0, k) > 0 && (1 - Unp) > 0) {
                        terms.add((1.0 / alpha.get(k)) * BN_scaled[idx].get(0, k) * (1 - Unp))
                        logScales.add(log_scale_BN[idx][0][k])
                    }

                    TN[idx].set(0, k, logsumexp(terms, logScales))
                }
            }
        }

        n = PopulationLattice.pprod(n, populationVector)
    }

    // Get final index
    val finalIdx = totalSize - 1
    val Q = QN[finalIdx]
    val T = TN[finalIdx]

    // Recover actual B values from scaled representation
    val B = Matrix(2, R)
    for (r in 0 until R) {
        if (BN_scaled[finalIdx].get(0, r) > 0) {
            B.set(0, r, BN_scaled[finalIdx].get(0, r) * exp(log_scale_BN[finalIdx][0][r]))
        }
        if (BN_scaled[finalIdx].get(1, r) > 0) {
            B.set(1, r, BN_scaled[finalIdx].get(1, r) * exp(log_scale_BN[finalIdx][1][r]))
        }
    }

    // Compute utilizations
    val U = Matrix(2, R)
    for (r in 0 until R) {
        U.set(0, r, T.get(0, r) * alpha.get(r))
        U.set(1, r, T.get(0, r) * beta.get(r))
    }

    return LcfsqnMvaResult(T, Q, U, B)
}

/**
 * Computes hash index for LCFS population lattice.
 */
private fun hashpopLcfs(n: Matrix, N: Matrix, R: Int, prods: Matrix): Int {
    var idx = 0
    for (r in 0 until R) {
        idx += (prods.get(r) * n.get(r)).toInt()
    }
    return idx
}

/**
 * Decrements the r-th component of vector n by 1.
 */
private fun oner(n: Matrix, r: Int): Matrix {
    val result = n.copy()
    result.set(r, result.get(r) - 1)
    return result
}

/**
 * Compute sum of scaled terms using log-sum-exp.
 *
 * Computes: result = sum_i (terms(i) * exp(log_scales(i)))
 *
 * This is mathematically EXACT - we're just computing the sum in a
 * numerically stable way by factoring out the maximum exponent.
 */
private fun logsumexp(terms: List<Double>, logScales: List<Double>): Double {
    if (terms.isEmpty()) {
        return 0.0
    }

    // Filter out zero or negative terms
    val validPairs = terms.zip(logScales)
        .filter { (t, s) -> t > 0 && s.isFinite() }

    if (validPairs.isEmpty()) {
        return 0.0
    }

    // Compute log of each term: log(term * exp(log_scale)) = log(term) + log_scale
    val logValues = validPairs.map { (t, s) -> ln(t) + s }

    // Find maximum for numerical stability
    val maxLog = logValues.maxOrNull() ?: return 0.0

    if (!maxLog.isFinite()) {
        return 0.0
    }

    // log-sum-exp: sum(exp(x)) = exp(max(x)) * sum(exp(x - max(x)))
    return exp(maxLog) * logValues.sumOf { exp(it - maxLog) }
}

/**
 * PFQN LCFS MVA algorithm class marker for Dokka documentation.
 */
@Suppress("unused")
class PfqnLcfsqnMvaAlgo {
    companion object {
        // Class documentation marker for Dokka
    }
}
