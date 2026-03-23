/*
 * Copyright (c) 2012-2026, QORE Lab, Imperial College London
 * All rights reserved.
 * Ported from BUTools V2.0
 */
package jline.lib.butools.map

import jline.lib.butools.mc.drpSolve
import jline.lib.butools.ph.PHRepresentation
import jline.util.matrix.Matrix
import jline.util.matrix.MatrixCell

/**
 * Returns the phase type distributed marginal distribution
 * of a continuous marked rational arrival process.
 *
 * @param H The H0...HN matrices of the MRAP (as MatrixCell)
 * @param prec Numerical precision for checking if the input is valid
 * @return The PHRepresentation containing alpha (initial vector) and A (matrix parameter)
 */
@JvmOverloads
fun marginalDistributionFromMRAP(H: MatrixCell, prec: Double = 1e-14): PHRepresentation {
    if (!checkMRAPRepresentation(H, prec)) {
        throw IllegalArgumentException("MarginalDistributionFromMRAP: Input isn't a valid MRAP representation!")
    }

    val n = H[0].numRows

    // Sum H1...HN
    var sumH = H[1].copy()
    for (i in 2 until H.size()) {
        sumH = sumH.add(H[i])
    }

    // KEY CHANGE from DMRAP: iH0 = (-H0)^{-1} instead of (I-H0)^{-1}
    // P = inv(-H0) * sumH
    val iH0 = H[0].neg().inv()
    val P = iH0.mult(sumH)

    // alpha = DRPSolve(P)
    val alpha = drpSolve(P)

    return PHRepresentation(alpha, H[0])
}

/**
 * Overload for Array<Matrix>.
 */
@JvmOverloads
fun marginalDistributionFromMRAP(H: Array<Matrix>, prec: Double = 1e-14): PHRepresentation {
    if (!checkMRAPRepresentation(H, prec)) {
        throw IllegalArgumentException("MarginalDistributionFromMRAP: Input isn't a valid MRAP representation!")
    }

    val n = H[0].numRows

    // Sum H1...HN
    var sumH = H[1].copy()
    for (i in 2 until H.size) {
        sumH = sumH.add(H[i])
    }

    // KEY CHANGE from DMRAP: iH0 = (-H0)^{-1} instead of (I-H0)^{-1}
    // P = inv(-H0) * sumH
    val iH0 = H[0].neg().inv()
    val P = iH0.mult(sumH)

    // alpha = DRPSolve(P)
    val alpha = drpSolve(P)

    return PHRepresentation(alpha, H[0])
}
