/*
 * Copyright (c) 2012-2026, QORE Lab, Imperial College London
 * All rights reserved.
 * Ported from BUTools V2.0
 */
package jline.lib.butools.map

import jline.util.matrix.Matrix
import jline.util.matrix.MatrixCell

/**
 * Returns the lag-L joint moments of a continuous rational arrival process.
 *
 * @param H0 The H0 matrix of the rational arrival process
 * @param H1 The H1 matrix of the rational arrival process
 * @param K The dimension of the matrix of joint moments to compute.
 *          If K=0, the MxM joint moments will be computed.
 * @param L The lag at which the joint moments are computed. Default is 1.
 * @param prec Numerical precision to check if the input is valid.
 * @return Matrix containing the lag-L joint moments
 */
@JvmOverloads
fun lagkJointMomentsFromRAP(
    H0: Matrix,
    H1: Matrix,
    K: Int = 0,
    L: Int = 1,
    prec: Double = 1e-14
): Matrix {
    if (!checkRAPRepresentation(H0, H1, prec)) {
        throw IllegalArgumentException("LagkJointMomentsFromRAP: Input isn't a valid RAP representation!")
    }

    val H = MatrixCell(2)
    H[0] = H0
    H[1] = H1

    val actualK = if (K == 0) H0.numRows - 1 else K

    val mom = lagkJointMomentsFromMRAP(H, actualK, L, prec)
    return mom[0]
}
