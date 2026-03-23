/**
 * @file QBD departure process joint moments computation
 *
 * Computes joint moments E[X_0^i * X_1^j] of consecutive inter-departure times
 * for MAP/MAP/1 queues using matrix-analytic methods.
 *
 * @since LINE 3.0
 */
package jline.api.mam

import jline.util.matrix.Matrix
import jline.util.matrix.MatrixCell
import jline.lib.smc.QBD_CR
import jline.lib.smc.QBD_pi

/**
 * Compute joint moments of consecutive inter-departure times
 *
 * Given arrival MAP and service MAP, computes joint moments E[X_0^i * X_1^j]
 * for each pair (i,j) specified in iset. Uses the stationary distribution of
 * the QBD process and matrix-exponential formulas for the departure process.
 *
 * @param MAPa Arrival MAP (MatrixCell with D0 and D1)
 * @param MAPs Service MAP (MatrixCell with D0 and D1)
 * @param iset Matrix where each row is a pair (i, j) specifying moment orders
 * @return DoubleArray of joint moment values, one per row of iset
 */
fun qbd_depproc_jointmom(MAPa: MatrixCell, MAPs: MatrixCell, iset: Matrix): DoubleArray {
    val na = MAPa[0].numRows
    val ns = MAPs[0].numRows
    val lvlsz = ns * na

    // Construct QBD transition matrices
    val IA = Matrix.eye(na)
    val IS = Matrix.eye(ns)
    val F = MAPa[1].kron(IS)                                    // Forward (arrivals)
    val L = MAPa[0].kron(IS).add(IA.kron(MAPs[0]))             // Local (Kronecker sum)
    val B = IA.kron(MAPs[1])                                     // Backward (services)
    val L0 = MAPa[0].kron(IS)                                   // Boundary local
    val Z = Matrix.zeros(lvlsz, lvlsz)                          // Zero block

    // Solve QBD
    val qbdResult = QBD_CR(B, L, F, null, null, null, null)
    val R = qbdResult["R"] ?: throw RuntimeException("QBD_CR failed to compute R matrix")

    // Compute stationary distribution
    val pi = QBD_pi(B, L0, R, 100, 0, null, 0)

    // Extract v0 (first lvlsz components of pi)
    val v0 = Matrix(1, lvlsz)
    for (j in 0 until lvlsz) {
        v0[0, j] = pi[0, j]
    }

    // Compute lambdaS (service rate)
    val lambdaS = map_lambda(MAPs)

    // Compute initial vectors for the 3-level truncation
    // v0D  = (1/lambdaS) * v0 * R * F
    // v1D  = (1/lambdaS) * v0 * R^2 * F
    // v2Dp = (1/lambdaS) * v0 * R^3 * inv(I - R) * F
    val invLambdaS = 1.0 / lambdaS
    val v0R = v0.mult(R)
    val v0D = v0R.mult(F).scale(invLambdaS)

    val v0R2 = v0R.mult(R)
    val v1D = v0R2.mult(F).scale(invLambdaS)

    val v0R3 = v0R2.mult(R)
    val ImR = Matrix.eye(R.numRows).add(-1.0, R)   // I - R
    val ImRinv = ImR.inv()
    val v2Dp = v0R3.mult(ImRinv).mult(F).scale(invLambdaS)

    // Concatenate z = [v0D, v1D, v2Dp] (1 x 3*lvlsz)
    val z = Matrix(1, 3 * lvlsz)
    for (j in 0 until lvlsz) {
        z[0, j] = v0D[0, j]
        z[0, lvlsz + j] = v1D[0, j]
        z[0, 2 * lvlsz + j] = v2Dp[0, j]
    }

    // Normalize z to probability distribution
    val zSum = z.elementSum()
    z.scaleEq(1.0 / zSum)

    // Build M0 = [L0, F, Z; Z, L, F; Z, Z, L+F]  (3*lvlsz x 3*lvlsz)
    val dim3 = 3 * lvlsz
    val M0 = Matrix.zeros(dim3, dim3)
    M0.insertSubMatrix(0, 0, lvlsz, lvlsz, L0)
    M0.insertSubMatrix(0, lvlsz, lvlsz, 2 * lvlsz, F)
    // Z at (0, 2*lvlsz) is already zero
    // Z at (lvlsz, 0) is already zero
    M0.insertSubMatrix(lvlsz, lvlsz, 2 * lvlsz, 2 * lvlsz, L)
    M0.insertSubMatrix(lvlsz, 2 * lvlsz, 2 * lvlsz, 3 * lvlsz, F)
    // Z at (2*lvlsz, 0) and (2*lvlsz, lvlsz) are already zero
    M0.insertSubMatrix(2 * lvlsz, 2 * lvlsz, 3 * lvlsz, 3 * lvlsz, L.add(F))

    // Build M1 = [Z, Z, Z; B, Z, Z; Z, B, Z]  (3*lvlsz x 3*lvlsz)
    val M1 = Matrix.zeros(dim3, dim3)
    M1.insertSubMatrix(lvlsz, 0, 2 * lvlsz, lvlsz, B)
    M1.insertSubMatrix(2 * lvlsz, lvlsz, 3 * lvlsz, 2 * lvlsz, B)

    // ones vector of size dim3
    val onesVec = Matrix.ones(dim3, 1)

    // Compute joint moments: for each row k of iset,
    // JM(k) = z * factorial(i) * (-M0)^(-(i+1)) * M1 * factorial(j) * (-M0)^(-j) * ones
    val negM0 = M0.neg()                    // -M0
    val negM0inv = negM0.inv()              // (-M0)^(-1)

    val numMoments = iset.numRows
    val JM = DoubleArray(numMoments)

    for (k in 0 until numMoments) {
        val iOrder = iset[k, 0].toInt()     // power for first inter-departure time
        val jOrder = iset[k, 1].toInt()     // power for second inter-departure time

        // Compute (-M0)^(-(i+1)) = ((-M0)^(-1))^(i+1)
        var negM0invPowI1 = Matrix.eye(dim3)
        for (p in 0 until iOrder + 1) {
            negM0invPowI1 = negM0invPowI1.mult(negM0inv)
        }

        // Compute (-M0)^(-j) = ((-M0)^(-1))^j
        var negM0invPowJ = Matrix.eye(dim3)
        for (p in 0 until jOrder) {
            negM0invPowJ = negM0invPowJ.mult(negM0inv)
        }

        // JM(k) = z * fact(i) * (-M0)^(-(i+1)) * M1 * fact(j) * (-M0)^(-j) * ones
        val factI = factorial(iOrder)
        val factJ = factorial(jOrder)

        val temp1 = z.mult(negM0invPowI1).scale(factI)   // z * fact(i) * (-M0)^(-(i+1))
        val temp2 = temp1.mult(M1)                         // ... * M1
        val temp3 = temp2.mult(negM0invPowJ).scale(factJ) // ... * fact(j) * (-M0)^(-j)
        val result = temp3.mult(onesVec)                   // ... * ones

        JM[k] = result[0, 0]
    }

    return JM
}

/**
 * Compute factorial of a non-negative integer
 */
private fun factorial(n: Int): Double {
    if (n <= 1) return 1.0
    var result = 1.0
    for (i in 2..n) {
        result *= i.toDouble()
    }
    return result
}

/**
 * QBD departure process joint moment algorithms
 */
@Suppress("unused")
class QbdDepprocJointmom {
    companion object {
        // Class documentation marker for Dokka
    }
}
