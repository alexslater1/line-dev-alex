/*
 * Copyright (c) 2012-2026, QORE Lab, Imperial College London
 * All rights reserved.
 * Ported from BUTools V2.0
 *
 * Reference:
 * Thummler, Axel, Peter Buchholz, and Miklos Telek.
 * "A novel approach for fitting probability distributions to real
 * trace data with the EM algorithm." Dependable Systems and Networks, 2005.
 */
package jline.lib.butools.fitting

import jline.lib.butools.ph.PHRepresentation
import jline.util.matrix.Matrix
import kotlin.math.abs
import kotlin.math.exp
import kotlin.math.ln

/**
 * Result of PH distribution fitting.
 */
class PHFitResult(val alpha: Matrix, val A: Matrix, val logli: Double)

/**
 * Performs PH distribution fitting using the EM algorithm (G-FIT).
 *
 * @param trace The samples of the trace (inter-arrival or service times)
 * @param orders Array of Erlang branch orders
 * @param maxIter Maximum number of EM iterations (default 200)
 * @param stopCond Relative log-likelihood improvement stop condition (default 1e-7)
 * @param initialGuess Optional initial PH representation
 * @param resultFormat Unused, kept for API compatibility
 * @return A [PHFitResult] containing alpha, A, and log-likelihood
 */
@JvmOverloads
fun phFromTrace(
    trace: DoubleArray,
    orders: IntArray,
    maxIter: Int = 200,
    stopCond: Double = 1e-7,
    initialGuess: PHRepresentation? = null,
    resultFormat: String? = null
): PHFitResult {
    val result = phFromTraceInternal(trace, orders, maxIter, stopCond, initialGuess)
    return PHFitResult(result.first.alpha, result.first.A, result.second)
}

/**
 * Convenience overload: when totalOrder is a single integer, tries all
 * possible branch-number / order combinations whose total number of
 * states equals [totalOrder], and returns the one with the highest
 * log-likelihood.
 */
@JvmOverloads
fun phFromTrace(
    trace: DoubleArray,
    totalOrder: Int,
    maxIter: Int = 200,
    stopCond: Double = 1e-7
): PHFitResult {
    val result = phFromTraceAllOrders(trace, totalOrder, maxIter, stopCond)
    return PHFitResult(result.first.alpha, result.first.A, result.second)
}

// ---- Internal implementation ----

/**
 * Enumerates all distinct sorted partitions of [sumOrders] into
 * [branches] positive integers (each >= 1).
 *
 * For example, allOrders(3, 6) returns partitions like [1,1,4], [1,2,3], [2,2,2].
 */
private fun allOrders(branches: Int, sumOrders: Int): List<IntArray> {
    if (branches == 1) {
        return listOf(intArrayOf(sumOrders))
    }
    val result = ArrayList<IntArray>()
    for (i in 0 until sumOrders - branches + 1) {
        val subPartitions = allOrders(branches - 1, sumOrders - i - 1)
        for (sub in subPartitions) {
            val combined = IntArray(sub.size + 1)
            System.arraycopy(sub, 0, combined, 0, sub.size)
            combined[sub.size] = i + 1
            combined.sort()
            // Check if we already have this partition
            var duplicate = false
            for (existing in result) {
                if (existing.contentEquals(combined)) {
                    duplicate = true
                    break
                }
            }
            if (!duplicate) {
                result.add(combined)
            }
        }
    }
    return result
}

/**
 * Tries all branch-number / order combinations for a given total order
 * and returns the best-fitting PH representation with its log-likelihood.
 */
private fun phFromTraceAllOrders(
    trace: DoubleArray,
    totalOrder: Int,
    maxIter: Int,
    stopCond: Double
): Pair<PHRepresentation, Double> {
    var bestPH: PHRepresentation? = null
    var bestLogLi = Double.NEGATIVE_INFINITY

    for (br in 2..totalOrder) {
        val allOrd = allOrders(br, totalOrder)
        for (ordk in allOrd) {
            val res = phFromTraceInternal(trace, ordk, maxIter, stopCond, null)
            if (res.second > bestLogLi) {
                bestLogLi = res.second
                bestPH = res.first
            }
        }
    }

    if (bestPH == null) {
        // Fallback: single branch of the given order
        val res = phFromTraceInternal(trace, intArrayOf(totalOrder), maxIter, stopCond, null)
        return res
    }
    return Pair(bestPH, bestLogLi)
}

/**
 * Core G-FIT EM algorithm implementation.
 *
 * Fits a mixture of Erlang distributions to [trace] data. The number
 * of Erlang branches is orders.size (M) and their orders are given
 * by the entries of [orders].
 *
 * @return A pair of (PHRepresentation, logLikelihood)
 */
private fun phFromTraceInternal(
    trace: DoubleArray,
    orders: IntArray,
    maxIter: Int,
    stopCond: Double,
    initialGuess: PHRepresentation?
): Pair<PHRepresentation, Double> {
    val M = orders.size    // number of Erlang branches
    val K = trace.size     // number of trace samples

    // --- Initial alpha (branch probabilities) and lambda (branch rates) ---
    val alphav = DoubleArray(M)
    val lambd = DoubleArray(M)

    if (initialGuess != null) {
        // Extract initial branch probabilities and rates from the given PH
        val alphaLen = initialGuess.alpha.length()
        if (alphaLen != M) {
            throw IllegalArgumentException(
                "The length of the initial vector ($alphaLen) is not consistent " +
                "with the number of branches ($M)!"
            )
        }
        for (i in 0 until M) {
            alphav[i] = initialGuess.alpha[0, i]
        }
        // The A matrix diagonal gives -lambda for each branch
        for (i in 0 until M) {
            lambd[i] = -initialGuess.A[i, i]
        }
    } else {
        // Default initialization: uniform branch probabilities,
        // rates chosen so that the mean of the mixture matches the trace mean
        val traceMean = trace.sum() / K

        for (i in 0 until M) {
            alphav[i] = 1.0 / M
        }

        // linspace(1, M, M) -> 1, 2, ..., M
        // lambd[i] = orders[i] * (i+1)
        for (i in 0 until M) {
            lambd[i] = orders[i].toDouble() * (i + 1).toDouble()
        }

        // inim = sum(alphav[i] / (i+1))
        var inim = 0.0
        for (i in 0 until M) {
            inim += alphav[i] / (i + 1).toDouble()
        }

        // Scale lambdas so that the mean matches
        val scaleFactor = inim / traceMean
        for (i in 0 until M) {
            lambd[i] *= scaleFactor
        }
    }

    // Uniform weights
    val W = DoubleArray(K) { 1.0 / K }

    // Q[i][k] = responsibility of branch i for sample k
    val Q = Array(M) { DoubleArray(K) }

    // Precompute log-factorials for each (orders[i] - 1)
    val logFactorials = DoubleArray(M)
    for (i in 0 until M) {
        var lf = 0.0
        for (j in 1 until orders[i]) {
            lf += ln(j.toDouble())
        }
        logFactorials[i] = lf
    }

    var logli = 1e-14
    var ologli = 1.0
    var steps = 1

    while (abs((ologli - logli) / logli) > stopCond && steps <= maxIter) {
        ologli = logli

        // --- E-step ---
        // Q[i,k] = alphav[i] * (lambd[i]*trace[k])^(orders[i]-1) / (orders[i]-1)! * lambd[i] * exp(-lambd[i]*trace[k])
        // Computed in log-space for numerical stability, then exponentiated.
        for (i in 0 until M) {
            val ord = orders[i]
            val lam = lambd[i]
            val logAlpha = ln(alphav[i])
            val logLam = ln(lam)
            val logFact = logFactorials[i]

            for (k in 0 until K) {
                val t = trace[k]
                // log(Q[i,k]) = log(alphav[i]) + (ord-1)*log(lam*t) - logFact + log(lam) - lam*t
                //             = log(alphav[i]) + (ord-1)*log(lam) + (ord-1)*log(t) - logFact + log(lam) - lam*t
                //             = log(alphav[i]) + ord*log(lam) + (ord-1)*log(t) - logFact - lam*t
                val logQ = logAlpha + ord.toDouble() * logLam + (ord - 1).toDouble() * ln(t) - logFact - lam * t
                Q[i][k] = exp(logQ)
            }
        }

        // Normalize columns: Q[i,k] /= sum_i Q[i,k]
        val nor = DoubleArray(K)
        for (k in 0 until K) {
            var s = 0.0
            for (i in 0 until M) {
                s += Q[i][k]
            }
            nor[k] = s
        }
        for (i in 0 until M) {
            for (k in 0 until K) {
                if (nor[k] > 0.0) {
                    Q[i][k] /= nor[k]
                }
            }
        }

        // Log-likelihood: sum(log(nor[k]) * W[k])
        logli = 0.0
        for (k in 0 until K) {
            if (nor[k] > 0.0) {
                logli += ln(nor[k]) * W[k]
            }
        }

        // --- M-step ---
        // v1[i] = sum_k Q[i,k] * W[k]   (weighted responsibility)
        // v2[i] = sum_k Q[i,k] * trace[k] * W[k]
        val v1 = DoubleArray(M)
        val v2 = DoubleArray(M)
        for (i in 0 until M) {
            var s1 = 0.0
            var s2 = 0.0
            for (k in 0 until K) {
                s1 += Q[i][k] * W[k]
                s2 += Q[i][k] * trace[k] * W[k]
            }
            v1[i] = s1
            v2[i] = s2
        }

        // Update branch probabilities and rates
        for (i in 0 until M) {
            alphav[i] = v1[i]
            if (v2[i] > 0.0) {
                lambd[i] = orders[i].toDouble() * v1[i] / v2[i]
            }
        }

        steps++
    }

    // --- Construct PH representation (alpha, A) from Erlang mixture ---
    val N = orders.sum()  // total number of phases

    val alpha = Matrix(1, N)
    val A = Matrix(N, N)

    var ix = 0
    for (i in 0 until M) {
        val ord = orders[i]
        val lam = lambd[i]

        // Initial probability: only the first phase of branch i gets alphav[i]
        alpha[0, ix] = alphav[i]

        if (ord == 1) {
            // Single-phase Erlang (exponential)
            A[ix, ix] = -lam
        } else {
            // Multi-phase Erlang: sub-diagonal transitions at rate lam,
            // diagonal = -lam, last phase has no forward transition
            for (j in 0 until ord) {
                A[ix + j, ix + j] = -lam
                if (j < ord - 1) {
                    A[ix + j, ix + j + 1] = lam
                }
            }
        }

        ix += ord
    }

    return Pair(PHRepresentation(alpha, A), logli)
}
