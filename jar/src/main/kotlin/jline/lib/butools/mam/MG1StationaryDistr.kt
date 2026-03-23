/*
 * Copyright (c) 2012-2026, QORE Lab, Imperial College London
 * All rights reserved.
 * Ported from BUTools V2.0
 */
package jline.lib.butools.mam

import jline.lib.butools.mc.dtmcSolve
import jline.util.matrix.Matrix

/**
 * Returns the stationary distribution of the M/G/1 type Markov chain
 * up to a given level K.
 *
 * Uses the stable Ramaswami formula for computing the stationary distribution.
 *
 * @param A List of matrix blocks of the M/G/1 type generator in the regular part, from 0 to MA-1.
 * @param B List of matrix blocks of the M/G/1 type generator at the boundary, from 0 to MB-1.
 *          If null, B=A is assumed.
 * @param G Matrix G of the M/G/1 type Markov chain (N x N), as returned by mg1FundamentalMatrix.
 *          If null, it will be computed.
 * @param K The stationary distribution is returned up to this level (default 500).
 * @param prec Numerical precision (default 1e-14).
 * @return The stationary probability vector up to level K, shape (1, (K+1)*N)
 */
fun mg1StationaryDistr(
    A: List<Matrix>,
    B: List<Matrix>? = null,
    G: Matrix? = null,
    K: Int = 500,
    prec: Double = 1e-14
): Matrix {
    // Compute G if not provided
    val Gmat = G ?: mg1FundamentalMatrix(A, prec)

    // Get stationary vector of G
    val g = dtmcSolve(Gmat)

    val m = A[0].numRows
    val I = Matrix.eye(m)
    val dega = A.size - 1

    // Make mutable copies of A for in-place modification
    // A_work[i] will hold sum_{v>=i} A_v * G^{v-i} after the loop
    val Awork = Array(A.size) { A[it].copy() }

    // Compute theta, beta, and modified A matrices
    // sumA starts as A[dega], accumulates A[dega]+A[dega-1]+...+A[0]
    // beta = sum_{i>=1} sum_{v>=i} A_v * e
    var sumA = Awork[dega].copy()
    var beta = sumA.sumRows()

    for (i in dega - 1 downTo 1) {
        sumA = sumA.add(1.0, Awork[i])
        // Awork[i] = A[i] + A[i+1]*G (Ramaswami modification)
        Awork[i] = Awork[i].add(1.0, Awork[i + 1].mult(Gmat))
        beta = beta.add(1.0, sumA.sumRows())
    }
    sumA = sumA.add(1.0, Awork[0])

    val theta = dtmcSolve(sumA)
    val drift = theta.mult(beta)[0, 0]

    if (drift >= 1.0) {
        throw IllegalStateException("MG1StationaryDistr: The Markov chain is not positive recurrent (drift=$drift)")
    }

    // Determine boundary matrices
    val useBoundary = B != null
    val Bwork: Array<Matrix>?
    val mb: Int
    val degb: Int

    if (useBoundary) {
        mb = B!!.size
        degb = mb - 1
        Bwork = Array(mb) { B[it].copy() }
        // Compute modified B matrices: sum_v>=k Bv G^{v-k}
        // We need Bwork[i] for i=1..degb
        // Compute sumBB0 and Bbeta
        var sumBB0 = Bwork[degb].copy()
        val Bbeta = Matrix.zeros(m, 1)
        for (i in degb - 1 downTo 1) {
            // Bbeta accumulates (sumBB0)*e at each step
            val rowSums = Matrix.zeros(m, 1)
            for (r in 0 until m) {
                var sum = 0.0
                for (c in 0 until m) {
                    sum += sumBB0[r, c]
                }
                rowSums[r, 0] = sum
            }
            for (r in 0 until m) {
                Bbeta[r, 0] = Bbeta[r, 0] + rowSums[r, 0]
            }
            sumBB0 = sumBB0.add(1.0, Bwork[i])
            Bwork[i] = Bwork[i].add(1.0, Bwork[i + 1].mult(Gmat))
        }

        // Compute Km = B[0] + B[1]*G
        val Km = Bwork[0].add(1.0, Bwork[1].mult(Gmat))
        val kappa = dtmcSolve(Km)

        // Compute psi1, psi2
        val onesM = Matrix.ones(m, 1)
        val betaOnes = beta.copy()
        // g is a row vector
        val gColTimesOnesRow = Matrix(m, m)
        for (r in 0 until m) {
            for (c in 0 until m) {
                gColTimesOnesRow[r, c] = onesM[r, 0] * g[0, c]
            }
        }
        // sumA - (ones-beta)*g = sumA - gColTimesOnesRow + betaOnes*g
        // Actually: inv(I - sumA - (ones(m,1)-beta)*g) where beta = column vec of row sums
        val betaMinusOnes = Matrix(m, 1)
        for (r in 0 until m) {
            betaMinusOnes[r, 0] = onesM[r, 0] - betaOnes[r, 0]
        }
        val outerProd = Matrix(m, m)
        for (r in 0 until m) {
            for (c in 0 until m) {
                outerProd[r, c] = betaMinusOnes[r, 0] * g[0, c]
            }
        }
        val tempMat = I.sub(sumA).sub(1.0, outerProd)
        val tempInv = tempMat.inv()
        val tempSum = tempInv.sumRows()

        // psi1 = (I - A[0] - A[1]) * temp + 1/(1-drift) * sum(A[0], axis=1)
        val IminusA0A1 = I.sub(Awork[0]).sub(1.0, Awork[1])
        val psi1 = IminusA0A1.mult(tempSum)
        val A0sum = Awork[0].sumRows()
        val scaleFactor = 1.0 / (1.0 - drift)
        for (r in 0 until m) {
            psi1[r, 0] = psi1[r, 0] + scaleFactor * A0sum[r, 0]
        }

        // psi2 = ones(mb,1) + (sumBB0 - B[1]) * temp + 1/(1-drift) * Bbeta
        val sumBB0minusB1 = sumBB0.sub(Bwork[1])
        val psi2 = sumBB0minusB1.mult(tempSum)
        for (r in 0 until m) {
            psi2[r, 0] = psi2[r, 0] + 1.0 + scaleFactor * Bbeta[r, 0]
        }

        // tildekappa1 = psi2 + B[1]*inv(I-A[1])*psi1
        val IminusA1inv = I.sub(Awork[1]).inv()
        val tildekappa1 = psi2.add(1.0, Bwork[1].mult(IminusA1inv).mult(psi1))

        // pi0 = 1/(kappa*tildekappa1) * kappa
        val normVal = kappa.mult(tildekappa1)[0, 0]
        val pi0 = kappa.scale(1.0 / normVal)

        // Ramaswami iterations
        val invbarA1 = I.sub(Awork[1]).inv()
        val piList = ArrayList<Matrix>()
        piList.add(pi0)
        var sumpi = pi0.elementSum()
        var numit = 1

        while (sumpi < 1.0 - 1e-10 && numit <= K) {
            var pix: Matrix
            if (numit <= degb) {
                pix = pi0.mult(Bwork[numit])
            } else {
                pix = Matrix.zeros(1, m)
            }
            val jMax = if (numit < dega) numit else dega
            for (j in 1 until jMax) {
                pix = pix.add(1.0, piList[numit - j].mult(Awork[j + 1]))
            }
            pix = pix.mult(invbarA1)
            sumpi += pix.elementSum()
            piList.add(pix)
            numit++
        }

        // Assemble result
        val result = Matrix.zeros(1, (K + 1) * m)
        for (k in 0 until piList.size.coerceAtMost(K + 1)) {
            for (j in 0 until m) {
                result[0, k * m + j] = piList[k][0, j]
            }
        }
        return result
    } else {
        // No boundary matrices: B = A
        // pi0 = (1-drift) * g
        val pi0 = g.scale(1.0 - drift)

        // Ramaswami iterations
        val invbarA1 = I.sub(Awork[1]).inv()
        val piList = ArrayList<Matrix>()
        piList.add(pi0)
        var sumpi = pi0.elementSum()
        var numit = 1

        while (sumpi < 1.0 - 1e-10 && numit <= K) {
            var pix: Matrix
            if (numit <= dega) {
                pix = pi0.mult(Awork[numit])
            } else {
                pix = Matrix.zeros(1, m)
            }
            val jMax = if (numit < dega) numit else dega
            for (j in 1 until jMax) {
                pix = pix.add(1.0, piList[numit - j].mult(Awork[j + 1]))
            }
            pix = pix.mult(invbarA1)
            sumpi += pix.elementSum()
            piList.add(pix)
            numit++
        }

        // Assemble result
        val result = Matrix.zeros(1, (K + 1) * m)
        for (k in 0 until piList.size.coerceAtMost(K + 1)) {
            for (j in 0 until m) {
                result[0, k * m + j] = piList[k][0, j]
            }
        }
        return result
    }
}

/**
 * Helper: Compute row sums of a matrix as a column vector.
 */
private fun Matrix.sumRows(): Matrix {
    val result = Matrix(numRows, 1)
    for (i in 0 until numRows) {
        var sum = 0.0
        for (j in 0 until numCols) {
            sum += this[i, j]
        }
        result[i, 0] = sum
    }
    return result
}

/**
 * Overload accepting Array<Matrix> for A and B.
 */
fun mg1StationaryDistr(
    A: Array<Matrix>,
    B: Array<Matrix>? = null,
    G: Matrix? = null,
    K: Int = 500,
    prec: Double = 1e-14
): Matrix {
    return mg1StationaryDistr(
        A.toList(),
        B?.toList(),
        G, K, prec
    )
}
