/*
 * Copyright (c) 2012-2026, QORE Lab, Imperial College London
 * All rights reserved.
 * Ported from BUTools V2.0
 */
package jline.lib.butools.map

import jline.lib.butools.ph.momentsFromPH
import jline.util.matrix.Matrix
import jline.util.matrix.MatrixCell

/**
 * Returns the moments of the marginal distribution of a continuous
 * marked Markovian arrival process.
 *
 * @param D The D0...DN matrices of the MMAP (as MatrixCell)
 * @param K Number of moments to compute. If K=0, 2*M-1 moments are computed.
 * @param prec Numerical precision for checking if the input is valid
 * @return The vector of moments
 */
@JvmOverloads
fun marginalMomentsFromMMAP(D: MatrixCell, K: Int = 0, prec: Double = 1e-14): DoubleArray {
    val numMoments = if (K == 0) 2 * D[0].numRows - 1 else K

    if (!checkMMAPRepresentation(D, prec)) {
        throw IllegalArgumentException("MarginalMomentsFromMMAP: Input isn't a valid MMAP representation!")
    }

    val margDist = marginalDistributionFromMMAP(D, prec)
    return momentsFromPH(margDist.alpha, margDist.A, numMoments)
}

/**
 * Overload for Array<Matrix>.
 */
@JvmOverloads
fun marginalMomentsFromMMAP(D: Array<Matrix>, K: Int = 0, prec: Double = 1e-14): DoubleArray {
    val numMoments = if (K == 0) 2 * D[0].numRows - 1 else K

    if (!checkMMAPRepresentation(D, prec)) {
        throw IllegalArgumentException("MarginalMomentsFromMMAP: Input isn't a valid MMAP representation!")
    }

    val margDist = marginalDistributionFromMMAP(D, prec)
    return momentsFromPH(margDist.alpha, margDist.A, numMoments)
}
