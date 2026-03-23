/*
 * Copyright (c) 2012-2026, QORE Lab, Imperial College London
 * All rights reserved.
 * Ported from BUTools V2.0
 */
package jline.lib.butools.fitting

import jline.lib.butools.mc.dtmcSolve
import jline.util.matrix.Matrix
import kotlin.math.*

/**
 * Result of MAP fitting.
 */
class MAPFitResult(val D0: Matrix, val D1: Matrix, val logli: Double)

/**
 * Generates all distinct sorted partitions of [sumorders] into [branches] parts,
 * each part >= 1. Used when `orders` is a single integer to enumerate all
 * branch-number / order combinations.
 */
private fun allOrders(branches: Int, sumorders: Int): List<List<Int>> {
    if (branches == 1) {
        return listOf(listOf(sumorders))
    }
    val result = ArrayList<List<Int>>()
    for (i in 0 until sumorders - branches + 1) {
        val subPartitions = allOrders(branches - 1, sumorders - i - 1)
        for (sub in subPartitions) {
            val candidate = ArrayList<Int>(sub.size + 1)
            candidate.addAll(sub)
            candidate.add(i + 1)
            candidate.sort()
            if (!result.contains(candidate)) {
                result.add(candidate)
            }
        }
    }
    return result
}

/**
 * Computes the factorial of n.
 */
private fun factorial(n: Int): Double {
    var f = 1.0
    for (i in 2..n) {
        f *= i.toDouble()
    }
    return f
}

/**
 * Performs MAP fitting using the EM algorithm (ErCHMM).
 *
 * This implements the Erlang-based Continuous Hidden Markov Model algorithm
 * for fitting a Markovian Arrival Process to trace data, as described in:
 *
 * - Okamura, Hiroyuki, and Tadashi Dohi. "Faster maximum likelihood estimation
 *   algorithms for Markovian arrival processes." QEST 2009.
 * - Horvath, Gabor, and Hiroyuki Okamura. "A Fast EM Algorithm for Fitting
 *   Marked Markovian Arrival Processes with a New Special Structure."
 *   Computer Performance Engineering, Springer 2013.
 *
 * When [orders] contains the explicit Erlang branch orders (e.g., [1,2,3]),
 * the function performs EM fitting with those branches. When [orders] contains
 * a single integer N, all possible branch-number / order combinations summing
 * to N are tested and the best (highest log-likelihood) result is returned.
 *
 * The algorithm:
 * 1. Initialize transition matrix P between Erlang branches and rate parameters
 * 2. Run EM iterations:
 *    - E-step: compute Erlang densities, forward-backward algorithm on the HMM
 *    - M-step: update transition matrix P and Erlang rate parameters
 * 3. Convert the best ErCHMM to MAP(D0, D1) representation
 *
 * @param trace The samples of the trace
 * @param orders The Erlang branch orders. If length 1, all combinations
 *        summing to orders[0] are tried. Otherwise, each entry is the
 *        order of the corresponding Erlang branch.
 * @param maxIter Maximum number of EM iterations (default 200)
 * @param stopCond The algorithm stops when relative log-likelihood
 *        improvement falls below this threshold (default 1e-7)
 * @param initialGuess Optional initial guess as array [D0, D1] of MAP matrices.
 *        If provided, the rate parameters and transition matrix are extracted
 *        from these matrices. If null, a default initial guess matching the
 *        trace mean is used.
 * @return A Pair of (D0, D1) matrices representing the fitted MAP
 */
@JvmOverloads
fun mapFromTrace(
    trace: DoubleArray,
    orders: IntArray,
    maxIter: Int = 200,
    stopCond: Double = 1e-7,
    initialGuess: Array<Matrix>? = null,
    resultFormat: String? = null
): MAPFitResult {
    // If orders has a single element, interpret it as total number of states
    // and try all branch/order combinations
    if (orders.size == 1) {
        val totalStates = orders[0]
        val res = mapFromTraceAllOrders(trace, totalStates, maxIter, stopCond, initialGuess)
        return MAPFitResult(res.first, res.second, res.third)
    }

    val result = mapFromTraceEM(trace, orders, maxIter, stopCond, initialGuess)
    return MAPFitResult(result.first, result.second, result.third)
}

/**
 * Tries all branch-number / order combinations for a given total number of states
 * and returns the MAP with the highest log-likelihood.
 */
private fun mapFromTraceAllOrders(
    trace: DoubleArray,
    totalStates: Int,
    maxIter: Int,
    stopCond: Double,
    initialGuess: Array<Matrix>?
): Triple<Matrix, Matrix, Double> {
    var bestD0: Matrix? = null
    var bestD1: Matrix? = null
    var bestLogli = Double.NEGATIVE_INFINITY

    for (br in 2..totalStates) {
        val allOrd = allOrders(br, totalStates)
        for (ordk in allOrd) {
            val ordArray = ordk.toIntArray()
            val result = mapFromTraceEM(trace, ordArray, maxIter, stopCond, initialGuess)
            if (result.third > bestLogli) {
                bestLogli = result.third
                bestD0 = result.first
                bestD1 = result.second
            }
        }
    }

    return Triple(bestD0!!, bestD1!!, bestLogli)
}

/**
 * Result of the EM algorithm: (D0, D1, logLikelihood).
 */
private fun mapFromTraceEM(
    trace: DoubleArray,
    orders: IntArray,
    maxIter: Int,
    stopCond: Double,
    initialGuess: Array<Matrix>?
): Triple<Matrix, Matrix, Double> {
    val M = orders.size  // number of Erlang branches
    val K = trace.size   // number of trace samples

    // --- Initialization ---
    // alphav[m]: stationary probability of branch m
    // lambd[m]: Erlang rate parameter of branch m
    // P[m1][m2]: transition probability from branch m1 to branch m2
    val alphav = DoubleArray(M)
    val lambd = DoubleArray(M)
    val P = Array(M) { DoubleArray(M) }

    if (initialGuess == null) {
        // Default initialization: uniform alpha, rates matching trace mean
        val trm = trace.sum() / K.toDouble()
        for (m in 0 until M) {
            alphav[m] = 1.0 / M
        }
        // lambd = orders * linspace(1,M,M), scaled to match mean
        // linspace(1,M,M) = [1, 2, ..., M] when M points from 1 to M
        var inim = 0.0
        for (m in 0 until M) {
            val linVal = (m + 1).toDouble()  // linspace(1,M,M)[m]
            lambd[m] = orders[m].toDouble() * linVal
            inim += alphav[m] / linVal
        }
        for (m in 0 until M) {
            lambd[m] = lambd[m] * inim / trm
        }
        // P = ones(M,1) * matrix([alphav]) => each row is alphav
        for (i in 0 until M) {
            for (j in 0 until M) {
                P[i][j] = alphav[j]
            }
        }
    } else {
        // Extract lambda and P from the initial MAP matrices D0, D1
        require(initialGuess.size == 2) { "Initial guess must contain exactly 2 matrices (D0 and D1)" }
        val initLambd = DoubleArray(M)
        val initP = Array(M) { DoubleArray(M) }
        extractErCHMMParams(initialGuess[0], initialGuess[1], orders, initLambd, initP)
        for (m in 0 until M) {
            lambd[m] = initLambd[m]
        }
        for (i in 0 until M) {
            for (j in 0 until M) {
                P[i][j] = initP[i][j]
            }
        }
        // Compute stationary distribution of P
        val Pmat = Matrix(M, M)
        for (i in 0 until M) {
            for (j in 0 until M) {
                Pmat[i, j] = P[i][j]
            }
        }
        val piVec = dtmcSolve(Pmat)
        for (m in 0 until M) {
            alphav[m] = piVec[0, m]
        }
    }

    // --- Working arrays ---
    // Q[m][k]: Erlang density of branch m evaluated at trace sample k
    val Q = Array(M) { DoubleArray(K) }

    // A[k][m]: forward likelihood vector after processing sample k (K x M)
    val A = Array(K) { DoubleArray(M) }
    val Ascale = DoubleArray(K)

    // B[k][m]: backward likelihood vector at sample k (K x M, transposed from Python's M x K)
    val B = Array(K) { DoubleArray(M) }
    val Bscale = DoubleArray(K)

    var logli = 1e-14
    var ologli = 0.0
    var steps = 1

    // --- EM iterations ---
    while (abs((ologli - logli) / logli) > stopCond && steps <= maxIter) {
        ologli = logli

        // E-step: compute Erlang densities
        // Q[m][k] = (lambd[m]*trace[k])^(orders[m]-1) / (orders[m]-1)! * lambd[m] * exp(-lambd[m]*trace[k])
        for (m in 0 until M) {
            val lm = lambd[m]
            val om = orders[m]
            val factVal = factorial(om - 1)
            for (k in 0 until K) {
                val lt = lm * trace[k]
                Q[m][k] = lt.pow(om - 1) / factVal * lm * exp(-lt)
            }
        }

        // Forward likelihood vectors
        // prev = alphav (1 x M)
        val prev = alphav.copyOf()
        var scprev = 0.0
        for (k in 0 until K) {
            // prev = prev . diag(Q[:,k]) . P
            // Step 1: element-wise multiply prev by Q[:,k]
            val temp = DoubleArray(M)
            for (m in 0 until M) {
                temp[m] = prev[m] * Q[m][k]
            }
            // Step 2: multiply temp (1xM) by P (MxM) -> result in prev
            // Safe because temp is a separate array from prev
            for (m in 0 until M) {
                var s = 0.0
                for (j in 0 until M) {
                    s += temp[j] * P[j][m]
                }
                prev[m] = s
            }

            // Scale to avoid underflow
            var sumPrev = 0.0
            for (m in 0 until M) {
                sumPrev += prev[m]
            }
            val scale = log2(sumPrev)
            val invScale = 2.0.pow(-scale)
            for (m in 0 until M) {
                prev[m] *= invScale
            }
            Ascale[k] = scprev + scale
            for (m in 0 until M) {
                A[k][m] = prev[m]
            }
            scprev = Ascale[k]
        }
        // Av[k] = shifted forward: Av[0] = alphav, Av[k] = A[k-1] for k>=1
        // Ascalev[k] = shifted: Ascalev[0] = 0, Ascalev[k] = Ascale[k-1] for k>=1

        // Backward likelihood vectors
        val nnext = DoubleArray(M) { 1.0 }
        scprev = 0.0
        for (k in K - 1 downTo 0) {
            // nnext = diag(Q[:,k]) . P . nnext
            // Step 1: P . nnext (M x 1)
            val Pnnext = DoubleArray(M)
            for (i in 0 until M) {
                var s = 0.0
                for (j in 0 until M) {
                    s += P[i][j] * nnext[j]
                }
                Pnnext[i] = s
            }
            // Step 2: diag(Q[:,k]) . Pnnext = element-wise Q[m][k] * Pnnext[m]
            for (m in 0 until M) {
                nnext[m] = Q[m][k] * Pnnext[m]
            }
            // Scale
            var sumNext = 0.0
            for (m in 0 until M) {
                sumNext += nnext[m]
            }
            val scale = log2(sumNext)
            val invScale = 2.0.pow(-scale)
            for (m in 0 until M) {
                nnext[m] *= invScale
            }
            Bscale[k] = scprev + scale
            for (m in 0 until M) {
                B[k][m] = nnext[m]  // B[k][m] = backward vector at sample k, branch m
            }
            scprev = Bscale[k]
        }
        // Bv[k] = shifted backward: Bv[k] = B[k+1] for k<K-1, Bv[K-1] = ones
        // Bscalev[k] = shifted: Bscalev[k] = Bscale[k+1] for k<K-1, Bscalev[K-1] = 0

        // Compute log-likelihood
        // llh = alphav . B[:,0] = alphav . B[0][:]
        var llh = 0.0
        for (m in 0 until M) {
            llh += alphav[m] * B[0][m]
        }
        logli = (ln(llh) + Bscale[0] * ln(2.0)) / K
        val illh = 1.0 / llh

        // M-step: update parameters
        // Save old alphav before updates (needed for Avv computation below)
        val oldAlphav = alphav.copyOf()

        // AB[k][m] = Av[k][m] * B[k][m]  (element-wise, K x M)
        // where Av[0] = oldAlphav, Av[k] = A[k-1] for k>=1
        // and B is already K x M (transposed from Python's B.T)
        val AB = Array(K) { DoubleArray(M) }
        for (k in 0 until K) {
            for (m in 0 until M) {
                val avKM = if (k == 0) oldAlphav[m] else A[k - 1][m]
                AB[k][m] = avKM * B[k][m]
            }
        }
        // Normalize each row of AB
        for (k in 0 until K) {
            var rowSum = 0.0
            for (m in 0 until M) {
                rowSum += AB[k][m]
            }
            if (rowSum > 0.0) {
                for (m in 0 until M) {
                    AB[k][m] /= rowSum
                }
            }
        }
        // v1[m] = sum over k of AB[k][m]
        val v1 = DoubleArray(M)
        for (m in 0 until M) {
            var s = 0.0
            for (k in 0 until K) {
                s += AB[k][m]
            }
            v1[m] = s
        }
        // v2[m] = sum over k of AB[k][m] * trace[k]
        val v2 = DoubleArray(M)
        for (m in 0 until M) {
            var s = 0.0
            for (k in 0 until K) {
                s += AB[k][m] * trace[k]
            }
            v2[m] = s
        }
        // Update alphav and lambd
        for (m in 0 until M) {
            alphav[m] = v1[m] / K
            lambd[m] = orders[m].toDouble() * v1[m] / v2[m]
        }

        // Update transition matrix P
        // Avv[k][m] = Av[k][m] * Q[m][k]  (element-wise, K x M)
        // Uses oldAlphav for Av[0] since alphav was already updated above
        val Avv = Array(K) { DoubleArray(M) }
        for (k in 0 until K) {
            for (m in 0 until M) {
                val avKM = if (k == 0) oldAlphav[m] else A[k - 1][m]
                Avv[k][m] = avKM * Q[m][k]
            }
        }
        // nor[k] = illh * 2^(Ascalev[k] + Bscalev[k] - Bscale[0])
        // Ascalev[k]: 0 for k=0, Ascale[k-1] for k>=1
        // Bscalev[k]: Bscale[k+1] for k<K-1, 0 for k=K-1
        val nor = DoubleArray(K)
        for (k in 0 until K) {
            val ascaleV = if (k == 0) 0.0 else Ascale[k - 1]
            val bscaleV = if (k < K - 1) Bscale[k + 1] else 0.0
            nor[k] = illh * 2.0.pow(ascaleV + bscaleV - Bscale[0])
        }
        // Multiply each column of Avv by nor
        for (k in 0 until K) {
            for (m in 0 until M) {
                Avv[k][m] *= nor[k]
            }
        }
        // P_new = (Avv^T . Bv^T) * P  (element-wise multiplication with old P)
        // Avv^T is M x K, Bv^T is K x M => product is M x M
        // Bv[k] = B[k+1] for k<K-1, ones for k=K-1
        // Bv^T[k][m] when transposed... Actually in Python:
        //   Bv is M x K (shifted B), Bv.T is K x M
        //   P = (Avv.T . Bv.T) * P where Avv.T is M x K and Bv.T is K x M
        //   So the matrix product Avv.T . Bv.T is M x M
        //   Then element-wise multiply with P
        // In our storage: Avv is K x M, so Avv^T is M x K
        // Bv^T: In Python Bv is M x K, Bv.T is K x M.
        //   Bv[:,k] = B[:,k+1] for k<K-1, ones for k=K-1
        //   In our storage (B is K x M): BvT[k][m] = B[k+1][m] for k<K-1, 1.0 for k=K-1
        // So: result[i][j] = sum_k Avv^T[i][k] * BvT[k][j]
        //                   = sum_k Avv[k][i] * BvT[k][j]
        val AvvT_BvT = Array(M) { DoubleArray(M) }
        for (i in 0 until M) {
            for (j in 0 until M) {
                var s = 0.0
                for (k in 0 until K) {
                    val bvTkj = if (k < K - 1) B[k + 1][j] else 1.0
                    s += Avv[k][i] * bvTkj
                }
                AvvT_BvT[i][j] = s
            }
        }
        // P = AvvT_BvT * P (element-wise)
        for (i in 0 until M) {
            for (j in 0 until M) {
                P[i][j] = AvvT_BvT[i][j] * P[i][j]
            }
        }
        // Normalize rows of P
        for (i in 0 until M) {
            var rowSum = 0.0
            for (j in 0 until M) {
                rowSum += P[i][j]
            }
            if (rowSum > 0.0) {
                for (j in 0 until M) {
                    P[i][j] /= rowSum
                }
            }
        }

        steps++
    }

    // Convert ErCHMM to MAP(D0, D1)
    val N = orders.sum()  // total number of states
    val D0 = Matrix(N, N)
    var ix = 0
    for (i in 0 until M) {
        if (orders[i] == 1) {
            D0[ix, ix] = -lambd[i]
        } else {
            // Erlang chain: lambd[i] * (diag(ones(orders[i]-1), 1) - diag(ones(orders[i])))
            // Diagonal: -lambd[i]
            for (p in 0 until orders[i]) {
                D0[ix + p, ix + p] = -lambd[i]
            }
            // Super-diagonal: +lambd[i]
            for (p in 0 until orders[i] - 1) {
                D0[ix + p, ix + p + 1] = lambd[i]
            }
        }
        ix += orders[i]
    }

    val D1 = Matrix(N, N)
    // indicesTo[j] = cumulative sum of orders before branch j (starting state of branch j)
    val indicesTo = IntArray(M)
    indicesTo[0] = 0
    for (j in 1 until M) {
        indicesTo[j] = indicesTo[j - 1] + orders[j - 1]
    }
    // indicesFrom[i] = last state of branch i
    val indicesFrom = IntArray(M)
    var cumSum = 0
    for (i in 0 until M) {
        cumSum += orders[i]
        indicesFrom[i] = cumSum - 1
    }
    // X = diag(lambd) . P
    // D1[indicesFrom[i], indicesTo[j]] = X[i][j] = lambd[i] * P[i][j]
    for (i in 0 until M) {
        for (j in 0 until M) {
            D1[indicesFrom[i], indicesTo[j]] = lambd[i] * P[i][j]
        }
    }

    return Triple(D0, D1, logli)
}

/**
 * Extracts ErCHMM parameters (lambda rates and transition matrix P) from a MAP
 * representation (D0, D1). This reverses the MAP-to-ErCHMM conversion:
 * - Each Erlang branch i has order orders[i] and rate lambd[i] = -D0[start, start]
 * - P[i][j] = D1[lastState_i, firstState_j] / lambd[i]
 *
 * @param D0 The D0 matrix of the MAP
 * @param D1 The D1 matrix of the MAP
 * @param orders The Erlang branch orders
 * @param outLambd Output array for rate parameters (length M)
 * @param outP Output array for transition matrix (M x M)
 */
private fun extractErCHMMParams(
    D0: Matrix,
    D1: Matrix,
    orders: IntArray,
    outLambd: DoubleArray,
    outP: Array<DoubleArray>
) {
    val M = orders.size
    var ix = 0
    for (i in 0 until M) {
        outLambd[i] = -D0[ix, ix]
        ix += orders[i]
    }

    // indicesTo and indicesFrom
    val indicesTo = IntArray(M)
    indicesTo[0] = 0
    for (j in 1 until M) {
        indicesTo[j] = indicesTo[j - 1] + orders[j - 1]
    }
    val indicesFrom = IntArray(M)
    var cumSum = 0
    for (i in 0 until M) {
        cumSum += orders[i]
        indicesFrom[i] = cumSum - 1
    }

    for (i in 0 until M) {
        for (j in 0 until M) {
            outP[i][j] = if (outLambd[i] > 0.0) {
                D1[indicesFrom[i], indicesTo[j]] / outLambd[i]
            } else {
                0.0
            }
        }
    }
}
