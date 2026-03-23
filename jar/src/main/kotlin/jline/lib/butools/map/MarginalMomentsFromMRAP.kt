/*
 * Copyright (c) 2012-2026, QORE Lab, Imperial College London
 * All rights reserved.
 * Ported from BUTools V2.0
 */
package jline.lib.butools.map

import jline.lib.butools.ph.momentsFromME
import jline.util.matrix.Matrix
import jline.util.matrix.MatrixCell

/**
 * Returns the moments of the marginal distribution of a continuous
 * marked rational arrival process.
 *
 * @param H The H0...HN matrices of the MRAP (as MatrixCell)
 * @param K Number of moments to compute. If K=0, 2*M-1 moments are computed.
 * @param prec Numerical precision for checking if the input is valid
 * @return The vector of moments
 */
@JvmOverloads
fun marginalMomentsFromMRAP(H: MatrixCell, K: Int = 0, prec: Double = 1e-14): DoubleArray {
    val numMoments = if (K == 0) 2 * H[0].numRows - 1 else K

    if (!checkMRAPRepresentation(H, prec)) {
        throw IllegalArgumentException("MarginalMomentsFromMRAP: Input isn't a valid MRAP representation!")
    }

    val margDist = marginalDistributionFromMRAP(H, prec)
    return momentsFromME(margDist.alpha, margDist.A, numMoments)
}

/**
 * Overload for Array<Matrix>.
 */
@JvmOverloads
fun marginalMomentsFromMRAP(H: Array<Matrix>, K: Int = 0, prec: Double = 1e-14): DoubleArray {
    val numMoments = if (K == 0) 2 * H[0].numRows - 1 else K

    if (!checkMRAPRepresentation(H, prec)) {
        throw IllegalArgumentException("MarginalMomentsFromMRAP: Input isn't a valid MRAP representation!")
    }

    val margDist = marginalDistributionFromMRAP(H, prec)
    return momentsFromME(margDist.alpha, margDist.A, numMoments)
}
