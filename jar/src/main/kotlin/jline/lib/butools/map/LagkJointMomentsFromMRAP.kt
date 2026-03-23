/*
 * Copyright (c) 2012-2026, QORE Lab, Imperial College London
 * All rights reserved.
 * Ported from BUTools V2.0
 */
package jline.lib.butools.map

import jline.lib.butools.mc.drpSolve
import jline.util.matrix.Matrix
import jline.util.matrix.MatrixCell

/**
 * Returns the lag-L joint moments of a continuous marked rational arrival process.
 *
 * @param H The H0...HN matrices of the MRAP (as MatrixCell)
 * @param K The dimension of the matrix of joint moments to compute.
 *          If K=0, the MxM joint moments will be computed.
 * @param L The lag at which the joint moments are computed. Default is 1.
 * @param prec Numerical precision to check if the input is valid.
 * @return List of matrices containing the lag-L joint moments
 */
@JvmOverloads
fun lagkJointMomentsFromMRAP(
    H: MatrixCell,
    K: Int = 0,
    L: Int = 1,
    prec: Double = 1e-14
): MatrixCell {
    if (!checkMRAPRepresentation(H, prec)) {
        throw IllegalArgumentException("LagkJointMomentsFromMRAP: Input isn't a valid MRAP representation!")
    }

    val M = H.size() - 1
    val N = H[0].numRows
    val actualK = if (K == 0) N - 1 else K

    // sumH = sum of H[1] ... H[M]
    var sumH = Matrix(N, N)
    for (i in 1..M) {
        sumH = sumH.add(H[i])
    }

    // KEY CHANGE from DMRAP: iH0 = (-H0)^{-1} instead of (I-H0)^{-1}
    val iH0 = H[0].neg().inv()

    // pi = DRPSolve(iH0 * sumH) -- stationary distribution of embedded DTMC
    val P = iH0.mult(sumH)
    val pi = drpSolve(P)

    // Compute H0 powers: H0p[i] = i! * (-H0)^{-(i+1)} for factorial moments
    // H0p[0] = I
    // H0p[1] = iH0
    // H0p[i] = i * H0p[i-1] * iH0   (accumulates factorial)
    val H0p = arrayOfNulls<Matrix>(actualK + 1)
    var Pw = Matrix.eye(N)
    H0p[0] = Pw

    Pw = Pw.mult(iH0)
    H0p[1] = Pw

    for (i in 2..actualK) {
        Pw = Pw.scale(i.toDouble()).mult(iH0)
        H0p[i] = Pw
    }

    // Pl = (iH0*sumH)^(L-1) -- transition matrix power for lag
    var Pl = Matrix.eye(N)
    val transitionMatrix = iH0.mult(sumH)
    for (i in 0 until L - 1) {
        Pl = Pl.mult(transitionMatrix)
    }

    // Compute joint moments for each type
    val Nm = MatrixCell(M)
    for (m in 0 until M) {
        val Nmm = Matrix(actualK + 1, actualK + 1)
        for (i in 0..actualK) {
            for (j in 0..actualK) {
                // Nmm(i,j) = sum(pi * H0p{i} * iH0 * H{m+1} * Pl * H0p{j})
                val temp = pi.mult(H0p[i]!!).mult(iH0).mult(H[m + 1]).mult(Pl).mult(H0p[j]!!)
                Nmm[i, j] = temp.elementSum()
            }
        }

        // Continuous case: H0p terms directly yield regular moments (no conversion needed)
        Nm[m] = Nmm
    }

    return Nm
}

/**
 * Overload for Array<Matrix>.
 */
@JvmOverloads
fun lagkJointMomentsFromMRAP(
    H: Array<Matrix>,
    K: Int = 0,
    L: Int = 1,
    prec: Double = 1e-14
): MatrixCell {
    val cell = MatrixCell(H.size)
    for (i in H.indices) {
        cell[i] = H[i]
    }
    return lagkJointMomentsFromMRAP(cell, K, L, prec)
}
