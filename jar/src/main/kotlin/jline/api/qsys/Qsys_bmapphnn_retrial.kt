/**
 * @file BMAP/PH/N/N bufferless retrial queue analysis
 *
 * Implements analysis of BMAP/PH/N/N bufferless retrial queueing systems
 * with admission control. Uses the QBD-based algorithm from:
 * Dudin et al., "Analysis of BMAP/PH/N-Type Queueing System with Flexible
 * Retrials Admission Control", Mathematics 2025, 13(9), 1434.
 *
 * @since LINE 3.1.0
 */
package jline.api.qsys

import jline.io.line_warning
import jline.util.matrix.Matrix
import org.apache.commons.math3.util.CombinatoricsUtils

/**
 * Result of BMAP/PH/N/N retrial queue analysis.
 *
 * @property L_orbit Mean number of customers in orbit
 * @property N_server Mean number of busy servers
 * @property L_system Mean number in system (orbit + servers)
 * @property Utilization Server utilization (N_server / N)
 * @property Throughput System throughput
 * @property P_idle Probability all servers are idle
 * @property P_empty_orbit Probability orbit is empty
 * @property P_empty_system Probability system is empty (idle and empty orbit)
 * @property pi Stationary distribution (levels x Vd)
 * @property truncLevel Truncation level used
 * @property analyzer Name of analyzer used
 */
data class QsysRetrialResult(
    val L_orbit: Double,
    val N_server: Double,
    val L_system: Double,
    val Utilization: Double,
    val Throughput: Double,
    val P_idle: Double,
    val P_empty_orbit: Double,
    val P_empty_system: Double,
    val pi: Matrix,
    val truncLevel: Int,
    val analyzer: String
)

/**
 * Analyzes a BMAP/PH/N/N bufferless retrial queue.
 *
 * @param D Cell array of BMAP matrices: D[0]=D0 (hidden), D[1..K]=arrival matrices for batch sizes 1..K
 * @param beta PH service initial probability vector (1 x M)
 * @param S PH service subgenerator matrix (M x M)
 * @param N Number of servers (also capacity, hence bufferless)
 * @param alpha Retrial rate per customer in orbit
 * @param gamma Impatience (abandonment) rate per customer in orbit
 * @param p Probability of batch rejection when not enough servers
 * @param R Admission threshold (scalar applied to all BMAP states)
 * @param maxLevel Maximum orbit level for truncation (-1 for auto)
 * @param tolerance Convergence tolerance
 * @param verbose Print progress messages
 * @return QsysRetrialResult with performance metrics
 */
@JvmOverloads
fun qsys_bmapphnn_retrial(
    D: Array<Matrix>,
    beta: Matrix,
    S: Matrix,
    N: Int,
    alpha: Double,
    gamma: Double,
    p: Double,
    R: Int,
    maxLevel: Int = -1,
    tolerance: Double = 1e-10,
    verbose: Boolean = false
): QsysRetrialResult {
    return qsys_bmapphnn_retrial(D, beta, S, N, alpha, gamma, p,
        IntArray(D[0].numRows) { R }, maxLevel, tolerance, verbose)
}

/**
 * Analyzes a BMAP/PH/N/N bufferless retrial queue with per-BMAP-state thresholds.
 *
 * @param D Cell array of BMAP matrices: D[0]=D0 (hidden), D[1..K]=arrival matrices for batch sizes 1..K
 * @param beta PH service initial probability vector (1 x M)
 * @param S PH service subgenerator matrix (M x M)
 * @param N Number of servers (also capacity, hence bufferless)
 * @param alpha Retrial rate per customer in orbit
 * @param gamma Impatience (abandonment) rate per customer in orbit
 * @param p Probability of batch rejection when not enough servers
 * @param R Admission threshold per BMAP state (length V)
 * @param maxLevel Maximum orbit level for truncation (-1 for auto)
 * @param tolerance Convergence tolerance
 * @param verbose Print progress messages
 * @return QsysRetrialResult with performance metrics
 */
@JvmOverloads
fun qsys_bmapphnn_retrial(
    D: Array<Matrix>,
    beta: Matrix,
    S: Matrix,
    N: Int,
    alpha: Double,
    gamma: Double,
    p: Double,
    R: IntArray,
    maxLevel: Int = -1,
    tolerance: Double = 1e-10,
    verbose: Boolean = false
): QsysRetrialResult {

    // BMAP parameters
    val K = D.size - 1  // Maximum batch size
    val V = D[0].numRows  // Number of BMAP states
    val M = S.numRows     // Number of PH service phases

    // Compute generator of fundamental process: D^(1) = sum(D_k)
    val D1_gen = Matrix(V, V)
    for (k in 0 until D.size) {
        for (i in 0 until V) {
            for (j in 0 until V) {
                D1_gen[i, j] = D1_gen[i, j] + D[k][i, j]
            }
        }
    }

    // Stationary distribution of fundamental process
    val theta = computeStationaryVector(D1_gen)

    // Mean arrival rate: lambda = theta * sum(k * D_k) * e
    val sumKDk = Matrix(V, V)
    for (k in 1 until D.size) {
        for (i in 0 until V) {
            for (j in 0 until V) {
                sumKDk[i, j] = sumKDk[i, j] + k.toDouble() * D[k][i, j]
            }
        }
    }
    var lambda = 0.0
    for (i in 0 until V) {
        for (j in 0 until V) {
            lambda += theta[j] * sumKDk[j, i]
        }
    }

    // PH service parameters
    val S0 = Matrix(M, 1)  // exit rate vector = -S * ones
    for (i in 0 until M) {
        var rowSum = 0.0
        for (j in 0 until M) {
            rowSum += S[i, j]
        }
        S0[i, 0] = -rowSum
    }

    // Mean service time: b1 = beta * (-S)^{-1} * ones
    val negSinv = S.scale(-1.0).inv()
    val onesM = Matrix.ones(M, 1)
    val b1 = beta.mult(negSinv).mult(onesM)[0, 0]

    // Compute T_n values: T_n = C(n+M-1, M-1) = number of service states with n busy
    val T = IntArray(N + 1)
    for (n in 0..N) {
        T[n] = CombinatoricsUtils.binomialCoefficient(n + M - 1, M - 1).toInt()
    }
    var d = 0
    for (n in 0..N) {
        d += T[n]
    }

    // Build state mapping
    val stateMap = buildStateMap(N, M, T)

    // Determine truncation level
    val rho = lambda * b1 / N

    if (verbose) {
        println("Solving BMAP/PH/N/N retrial queue...")
        println("  V=$V, M=$M, N=$N, K=$K")
        println("  d=$d, block size Vd=${V * d}")
        println("  lambda=${"%.4f".format(lambda)}, mu=${"%.4f".format(1.0 / b1)}")
        println("  Offered load rho=${"%.4f".format(rho)}")
    }

    val truncLevel = if (maxLevel > 0) {
        maxLevel
    } else {
        Math.max(100, Math.ceil(50.0 / (1.0 - Math.min(rho, 0.99))).toInt())
    }

    if (verbose) {
        println("  Truncation level: $truncLevel")
    }

    // Build and solve the system
    val Vd = V * d
    val totalDim = (truncLevel + 1) * Vd

    if (verbose) {
        println("Total matrix dimension: $totalDim x $totalDim")
    }

    // Context for helper functions
    val ctx = RetrialContext(D, beta, S, S0, M, N, V, K, d, T, R, alpha, gamma, p, stateMap)

    // Build generator matrix Q
    val Q = Matrix(totalDim, totalDim)

    for (i in 0..truncLevel) {
        if (verbose && i % 20 == 0) {
            println("  Level $i/$truncLevel")
        }

        val rowStart = i * Vd
        val rowEnd = (i + 1) * Vd

        for (j in Math.max(0, i - 1)..Math.min(truncLevel, i + K)) {
            val colStart = j * Vd
            val Qij = buildGeneratorLevel(ctx, i, j)
            // Copy Qij into Q
            for (r in 0 until Vd) {
                for (c in 0 until Vd) {
                    Q[rowStart + r, colStart + c] = Qij[r, c]
                }
            }
        }
    }

    // Ensure rows sum to zero
    for (i in 0 until totalDim) {
        var rowSum = 0.0
        for (j in 0 until totalDim) {
            if (j != i) {
                rowSum += Q[i, j]
            }
        }
        Q[i, i] = -rowSum
    }

    // Solve pi * Q = 0, pi * e = 1
    if (verbose) {
        println("Solving linear system...")
    }

    // Replace last column with ones for normalization
    for (i in 0 until totalDim) {
        Q[i, totalDim - 1] = 1.0
    }

    val b = Matrix(totalDim, 1)
    b[totalDim - 1, 0] = 1.0

    // Solve Q' * pi' = b
    val Qt = Q.transpose()
    val piCol = Matrix(totalDim, 1)
    Matrix.solveSafe(Qt, b, piCol)
    val piVec = DoubleArray(totalDim)
    for (i in 0 until totalDim) {
        piVec[i] = piCol.get(i, 0)
    }

    // Reshape to level structure: pi[level][block_idx]
    val piMatrix = Matrix(truncLevel + 1, Vd)
    for (i in 0..truncLevel) {
        for (j in 0 until Vd) {
            piMatrix[i, j] = piVec[i * Vd + j]
        }
    }

    // Handle numerical issues
    var hasNeg = false
    for (i in 0 until piMatrix.numRows) {
        for (j in 0 until piMatrix.numCols) {
            if (piMatrix[i, j] < -1e-8) {
                hasNeg = true
            }
            if (piMatrix[i, j] < 0) {
                piMatrix[i, j] = 0.0
            }
        }
    }
    if (hasNeg) {
        line_warning("qsys_bmapphnn_retrial", "Negative probabilities detected, clipping to zero")
    }

    // Renormalize
    var piSum = 0.0
    for (i in 0 until piMatrix.numRows) {
        for (j in 0 until piMatrix.numCols) {
            piSum += piMatrix[i, j]
        }
    }
    if (piSum > 0) {
        for (i in 0 until piMatrix.numRows) {
            for (j in 0 until piMatrix.numCols) {
                piMatrix[i, j] = piMatrix[i, j] / piSum
            }
        }
    }

    // Compute performance measures

    // Mean number in orbit
    var L_orbit = 0.0
    for (i in 1..truncLevel) {
        var levelProb = 0.0
        for (j in 0 until Vd) {
            levelProb += piMatrix[i, j]
        }
        L_orbit += i.toDouble() * levelProb
    }

    // Mean number of busy servers
    var N_server = 0.0
    for (i in 0..truncLevel) {
        for (nu in 0 until V) {
            for (n in 0..N) {
                val offset = nu * d + getBlockOffset(T, n)
                for (t in 0 until T[n]) {
                    val idx = offset + t
                    if (idx < Vd) {
                        N_server += n.toDouble() * piMatrix[i, idx]
                    }
                }
            }
        }
    }

    // Probability all servers idle
    var P_idle = 0.0
    for (i in 0..truncLevel) {
        for (nu in 0 until V) {
            val offset = nu * d  // n=0 starts at offset 0 within the nu block
            P_idle += piMatrix[i, offset]
        }
    }

    // Probability orbit empty
    var P_empty_orbit = 0.0
    for (j in 0 until Vd) {
        P_empty_orbit += piMatrix[0, j]
    }

    // Probability system empty
    var P_empty = 0.0
    for (nu in 0 until V) {
        val offset = nu * d
        P_empty += piMatrix[0, offset]
    }

    if (verbose) {
        println("Solution complete.")
    }

    return QsysRetrialResult(
        L_orbit = L_orbit,
        N_server = N_server,
        L_system = L_orbit + N_server,
        Utilization = N_server / N,
        Throughput = N_server / b1,
        P_idle = P_idle,
        P_empty_orbit = P_empty_orbit,
        P_empty_system = P_empty,
        pi = piMatrix,
        truncLevel = truncLevel,
        analyzer = "LINE:qsys_bmapphnn_retrial"
    )
}

// ========== Helper classes and functions ==========

/**
 * Context holding all parameters for the retrial queue construction.
 */
private class RetrialContext(
    val D: Array<Matrix>,
    val beta: Matrix,
    val S: Matrix,
    val S0: Matrix,
    val M: Int,
    val N: Int,
    val V: Int,
    val K: Int,
    val d: Int,
    val T: IntArray,
    val R: IntArray,
    val alpha: Double,
    val gamma: Double,
    val p: Double,
    val stateMap: Array<Array<IntArray>>
)

/**
 * Compute stationary vector of a generator matrix Q.
 * Solves theta * Q = 0, theta * e = 1.
 */
private fun computeStationaryVector(Q: Matrix): DoubleArray {
    val n = Q.numRows
    val A = Q.transpose()
    // Replace last row with ones for normalization
    for (j in 0 until n) {
        A.set(n - 1, j, 1.0)
    }
    val b = Matrix(n, 1)
    b.set(n - 1, 0, 1.0)
    val x = Matrix(n, 1)
    Matrix.solveSafe(A, b, x)
    val theta = DoubleArray(n)
    for (i in 0 until n) {
        theta[i] = x.get(i, 0)
    }
    return theta
}

/**
 * Build mapping from (n, service_state_vector) to linear index.
 * Returns stateMap[n] = array of weak compositions of n into M parts.
 */
private fun buildStateMap(N: Int, M: Int, T: IntArray): Array<Array<IntArray>> {
    return Array(N + 1) { n -> generateCompositions(n, M) }
}

/**
 * Generate all weak compositions of n into M parts (reverse lexicographic).
 */
private fun generateCompositions(n: Int, M: Int): Array<IntArray> {
    if (M == 1) {
        return arrayOf(intArrayOf(n))
    }
    val result = ArrayList<IntArray>()
    for (m1 in n downTo 0) {
        val subComps = generateCompositions(n - m1, M - 1)
        for (sub in subComps) {
            val comp = IntArray(M)
            comp[0] = m1
            System.arraycopy(sub, 0, comp, 1, M - 1)
            result.add(comp)
        }
    }
    return result.toTypedArray()
}

/**
 * Get starting index (0-based) for states with n busy servers.
 */
private fun getBlockOffset(T: IntArray, n: Int): Int {
    var offset = 0
    for (i in 0 until n) {
        offset += T[i]
    }
    return offset
}

/**
 * Matrix L_n: service completion transitions (T[n] x T[n-1]).
 */
private fun computeL(ctx: RetrialContext, n: Int): Matrix? {
    if (n == 0) return null
    val rows = ctx.T[n]
    val cols = ctx.T[n - 1]
    val L = Matrix(rows, cols)
    val compsN = ctx.stateMap[n]
    val compsNm1 = ctx.stateMap[n - 1]

    for (i in compsN.indices) {
        val m = compsN[i]
        for (l in 0 until ctx.M) {
            if (m[l] > 0) {
                val mPrime = m.copyOf()
                mPrime[l] = mPrime[l] - 1
                for (j in compsNm1.indices) {
                    if (compsNm1[j].contentEquals(mPrime)) {
                        L[i, j] = L[i, j] + m[l].toDouble() * ctx.S0[l, 0]
                        break
                    }
                }
            }
        }
    }
    return L
}

/**
 * Matrix A_n: phase change transitions (T[n] x T[n]).
 */
private fun computeA(ctx: RetrialContext, n: Int): Matrix? {
    if (n == 0) return null
    val sz = ctx.T[n]
    val A = Matrix(sz, sz)
    val comps = ctx.stateMap[n]

    for (i in comps.indices) {
        val m = comps[i]
        for (l in 0 until ctx.M) {
            if (m[l] > 0) {
                for (lPrime in 0 until ctx.M) {
                    if (lPrime != l && ctx.S[l, lPrime] > 0) {
                        val mPrime = m.copyOf()
                        mPrime[l] = mPrime[l] - 1
                        mPrime[lPrime] = mPrime[lPrime] + 1
                        for (j in comps.indices) {
                            if (comps[j].contentEquals(mPrime)) {
                                A[i, j] = A[i, j] + m[l].toDouble() * ctx.S[l, lPrime]
                                break
                            }
                        }
                    }
                }
            }
        }
    }
    return A
}

/**
 * Matrix P_n: new arrival transitions (T[n] x T[n+1]).
 */
private fun computeP(ctx: RetrialContext, n: Int): Matrix? {
    if (n >= ctx.N) return null
    val rows = ctx.T[n]
    val cols = ctx.T[n + 1]
    val P = Matrix(rows, cols)
    val compsN = ctx.stateMap[n]
    val compsNp1 = ctx.stateMap[n + 1]

    for (i in compsN.indices) {
        val m = compsN[i]
        for (l in 0 until ctx.M) {
            val betaL = if (ctx.beta.numCols > 1) ctx.beta[0, l] else ctx.beta[l, 0]
            if (betaL > 0) {
                val mPrime = m.copyOf()
                mPrime[l] = mPrime[l] + 1
                for (j in compsNp1.indices) {
                    if (compsNp1[j].contentEquals(mPrime)) {
                        P[i, j] = P[i, j] + betaL
                        break
                    }
                }
            }
        }
    }
    return P
}

/**
 * Diagonal matrix Delta_n: exit rates (T[n] x T[n]).
 */
private fun computeDelta(ctx: RetrialContext, n: Int): Matrix? {
    if (n == 0) return null
    val sz = ctx.T[n]
    val Delta = Matrix(sz, sz)
    val comps = ctx.stateMap[n]

    for (i in comps.indices) {
        val m = comps[i]
        var total = 0.0
        for (l in 0 until ctx.M) {
            total += m[l].toDouble() * (-ctx.S[l, l])
        }
        Delta[i, i] = total
    }
    return Delta
}

/**
 * Diagonal matrix Gamma^(nu): 0 for n <= R[nu], 1 for n > R[nu].
 */
private fun computeGamma(ctx: RetrialContext, nu: Int): Matrix {
    val Gam = Matrix(ctx.d, ctx.d)
    var offset = 0
    for (n in 0..ctx.N) {
        if (n > ctx.R[nu]) {
            for (t in 0 until ctx.T[n]) {
                Gam[offset + t, offset + t] = 1.0
            }
        }
        offset += ctx.T[n]
    }
    return Gam
}

/**
 * G_{n,n}^{(nu,nu')} matrix for batch losses.
 */
private fun computeG_nn(ctx: RetrialContext, n: Int, nu: Int, nuPrime: Int): Matrix {
    val sz = ctx.T[n]
    val G = Matrix(sz, sz)
    if (n <= ctx.N - ctx.K) {
        return G  // zeros
    }
    var total = 0.0
    for (k in (ctx.N - n + 1)..ctx.K) {
        if (k >= 1 && k <= ctx.K) {
            total += ctx.D[k][nu, nuPrime]
        }
    }
    for (i in 0 until sz) {
        G[i, i] = ctx.p * total
    }
    return G
}

/**
 * Block matrix B^(nu) of size d x d.
 */
private fun computeB(ctx: RetrialContext, nu: Int): Matrix {
    val B = Matrix(ctx.d, ctx.d)

    // Precompute matrices
    val L = arrayOfNulls<Matrix>(ctx.N + 1)
    val A = arrayOfNulls<Matrix>(ctx.N + 1)
    val Pmat = arrayOfNulls<Matrix>(ctx.N + 1)
    val Delta = arrayOfNulls<Matrix>(ctx.N + 1)

    for (n in 0..ctx.N) {
        L[n] = computeL(ctx, n)
        A[n] = computeA(ctx, n)
        Pmat[n] = computeP(ctx, n)
        Delta[n] = computeDelta(ctx, n)
    }

    for (n in 0..ctx.N) {
        val rowStart = getBlockOffset(ctx.T, n)

        // Diagonal block
        val G_nn = computeG_nn(ctx, n, nu, nu)
        if (n == 0) {
            B[rowStart, rowStart] = G_nn[0, 0]
        } else {
            val sz = ctx.T[n]
            for (i in 0 until sz) {
                for (j in 0 until sz) {
                    B[rowStart + i, rowStart + j] = A[n]!![i, j] + Delta[n]!![i, j] + G_nn[i, j]
                }
            }
        }

        // Subdiagonal block
        if (n >= 1) {
            val colStart = getBlockOffset(ctx.T, n - 1)
            val Ln = L[n]!!
            for (i in 0 until Ln.numRows) {
                for (j in 0 until Ln.numCols) {
                    B[rowStart + i, colStart + j] = Ln[i, j]
                }
            }
        }

        // Superdiagonal blocks
        for (k in 1..ctx.K) {
            if (n + k <= ctx.N) {
                val colStart = getBlockOffset(ctx.T, n + k)
                val D_k_nu_nu = ctx.D[k][nu, nu]

                // Compute product of P matrices from n to n+k-1
                var Pprod = Matrix.eye(ctx.T[n])
                for (jj in n until n + k) {
                    if (jj < ctx.N) {
                        Pprod = Pprod.mult(Pmat[jj]!!)
                    }
                }
                val block = Pprod.scale(D_k_nu_nu)
                for (i in 0 until block.numRows) {
                    for (j in 0 until block.numCols) {
                        B[rowStart + i, colStart + j] = block[i, j]
                    }
                }
            }
        }
    }
    return B
}

/**
 * B_bar^(nu) matrix for successful retrials.
 */
private fun computeBbar(ctx: RetrialContext, nu: Int): Matrix {
    val Bbar = Matrix(ctx.d, ctx.d)
    for (n in 0..Math.min(ctx.R[nu], ctx.N - 1)) {
        val rowStart = getBlockOffset(ctx.T, n)
        val colStart = getBlockOffset(ctx.T, n + 1)
        val P_n = computeP(ctx, n)
        if (P_n != null) {
            for (i in 0 until P_n.numRows) {
                for (j in 0 until P_n.numCols) {
                    Bbar[rowStart + i, colStart + j] = P_n[i, j]
                }
            }
        }
    }
    return Bbar
}

/**
 * B_tilde^(nu, nu') for BMAP state transitions.
 */
private fun computeBtilde(ctx: RetrialContext, nu: Int, nuPrime: Int): Matrix {
    val Btilde = Matrix(ctx.d, ctx.d)

    // Precompute P matrices
    val Pmat = arrayOfNulls<Matrix>(ctx.N + 1)
    for (n in 0..ctx.N) {
        Pmat[n] = computeP(ctx, n)
    }

    for (n in 0..ctx.N) {
        val rowStart = getBlockOffset(ctx.T, n)

        // Diagonal block
        val G_nn = computeG_nn(ctx, n, nu, nuPrime)
        for (i in 0 until G_nn.numRows) {
            for (j in 0 until G_nn.numCols) {
                Btilde[rowStart + i, rowStart + j] = G_nn[i, j]
            }
        }

        // Superdiagonal blocks
        for (k in 1..ctx.K) {
            if (n + k <= ctx.N) {
                val colStart = getBlockOffset(ctx.T, n + k)
                val D_k_nu_nuPrime = ctx.D[k][nu, nuPrime]

                var Pprod = Matrix.eye(ctx.T[n])
                for (jj in n until n + k) {
                    if (jj < ctx.N) {
                        Pprod = Pprod.mult(Pmat[jj]!!)
                    }
                }
                val block = Pprod.scale(D_k_nu_nuPrime)
                for (i in 0 until block.numRows) {
                    for (j in 0 until block.numCols) {
                        Btilde[rowStart + i, colStart + j] = block[i, j]
                    }
                }
            }
        }
    }
    return Btilde
}

/**
 * C_{n,k}^(nu, nu') for partial batch admission to orbit.
 */
private fun computeC(ctx: RetrialContext, n: Int, k: Int, nu: Int, nuPrime: Int): Matrix? {
    if (n < ctx.N - ctx.K + k) {
        return Matrix(ctx.T[n], ctx.T[ctx.N])
    } else if (n < ctx.N) {
        val batchSize = ctx.N - n + k
        if (batchSize < 1 || batchSize > ctx.K) {
            return Matrix(ctx.T[n], ctx.T[ctx.N])
        }
        val D_batch = ctx.D[batchSize][nu, nuPrime]

        // Precompute P matrices
        val Pmat = arrayOfNulls<Matrix>(ctx.N + 1)
        for (nn in 0..ctx.N) {
            Pmat[nn] = computeP(ctx, nn)
        }

        var Pprod = Matrix.eye(ctx.T[n])
        for (jj in n until ctx.N) {
            if (Pmat[jj] != null) {
                Pprod = Pprod.mult(Pmat[jj]!!)
            }
        }
        return Pprod.scale((1.0 - ctx.p) * D_batch)
    } else {
        // n == N
        if (k < 1 || k > ctx.K) {
            return Matrix(ctx.T[ctx.N], ctx.T[ctx.N])
        }
        val D_k = ctx.D[k][nu, nuPrime]
        val C = Matrix.eye(ctx.T[ctx.N])
        return C.scale((1.0 - ctx.p) * D_k)
    }
}

/**
 * Build generator block Q_{i,j}.
 */
private fun buildGeneratorLevel(ctx: RetrialContext, i: Int, j: Int): Matrix {
    val Vd = ctx.V * ctx.d
    val Q = Matrix(Vd, Vd)

    if (j < Math.max(0, i - 1) || j > i + ctx.K) {
        return Q
    }

    // Precompute matrices
    val B = Array(ctx.V) { nu -> computeB(ctx, nu) }
    val Bbar = Array(ctx.V) { nu -> computeBbar(ctx, nu) }
    val Gam = Array(ctx.V) { nu -> computeGamma(ctx, nu) }

    if (i == j) {
        // Diagonal block
        for (nu in 0 until ctx.V) {
            val rowStart = nu * ctx.d
            for (nuPrime in 0 until ctx.V) {
                val colStart = nuPrime * ctx.d
                if (nu == nuPrime) {
                    val D0_nu_nu = ctx.D[0][nu, nu]
                    for (r in 0 until ctx.d) {
                        for (c in 0 until ctx.d) {
                            var value = B[nu][r, c]
                            if (r == c) {
                                value += D0_nu_nu - i.toDouble() * (ctx.gamma + ctx.alpha) + i.toDouble() * ctx.alpha * Gam[nu][r, c]
                            } else {
                                value += i.toDouble() * ctx.alpha * Gam[nu][r, c]
                            }
                            Q[rowStart + r, colStart + c] = value
                        }
                    }
                } else {
                    val Btilde = computeBtilde(ctx, nu, nuPrime)
                    val D0_nu_nuPrime = ctx.D[0][nu, nuPrime]
                    for (r in 0 until ctx.d) {
                        for (c in 0 until ctx.d) {
                            var value = Btilde[r, c]
                            if (r == c) {
                                value += D0_nu_nuPrime
                            }
                            Q[rowStart + r, colStart + c] = value
                        }
                    }
                }
            }
        }
    } else if (j == i - 1 && i >= 1) {
        // Subdiagonal block
        for (nu in 0 until ctx.V) {
            val rowStart = nu * ctx.d
            val colStart = rowStart
            for (r in 0 until ctx.d) {
                for (c in 0 until ctx.d) {
                    var value = i.toDouble() * ctx.alpha * Bbar[nu][r, c]
                    if (r == c) {
                        value += i.toDouble() * ctx.gamma
                    }
                    Q[rowStart + r, colStart + c] = value
                }
            }
        }
    } else if (j > i && j <= i + ctx.K) {
        // Superdiagonal blocks
        val kk = j - i
        for (nu in 0 until ctx.V) {
            val rowStart = nu * ctx.d
            for (nuPrime in 0 until ctx.V) {
                val colStart = nuPrime * ctx.d
                val block = Matrix(ctx.d, ctx.d)
                for (n in 0..ctx.N) {
                    val C_nk = computeC(ctx, n, kk, nu, nuPrime)
                    if (C_nk != null && C_nk.numCols == ctx.T[ctx.N]) {
                        val nRowStart = getBlockOffset(ctx.T, n)
                        val NColStart = getBlockOffset(ctx.T, ctx.N)
                        for (r in 0 until ctx.T[n]) {
                            for (c in 0 until ctx.T[ctx.N]) {
                                val v = C_nk[r, c]
                                if (v != 0.0) {
                                    block[nRowStart + r, NColStart + c] = block[nRowStart + r, NColStart + c] + v
                                }
                            }
                        }
                    }
                }
                for (r in 0 until ctx.d) {
                    for (c in 0 until ctx.d) {
                        Q[rowStart + r, colStart + c] = block[r, c]
                    }
                }
            }
        }
    }

    return Q
}

/**
 * Queueing system bmapphnn retrial algorithms
 */
@Suppress("unused")
class QsysBmapphnnRetrialAlgo {
    companion object {
        // Class documentation marker for Dokka
    }
}
