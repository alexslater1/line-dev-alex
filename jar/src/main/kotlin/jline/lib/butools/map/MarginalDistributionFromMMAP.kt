/*
 * Copyright (c) 2012-2026, QORE Lab, Imperial College London
 * All rights reserved.
 * Ported from BUTools V2.0
 */
package jline.lib.butools.map

import jline.lib.butools.ph.PHRepresentation
import jline.util.matrix.Matrix
import jline.util.matrix.MatrixCell

/**
 * Returns the phase type distributed marginal distribution
 * of a continuous marked Markovian arrival process.
 *
 * @param D The D0...DN matrices of the MMAP (as MatrixCell)
 * @param prec Numerical precision for checking if the input is valid
 * @return The PHRepresentation containing alpha (initial vector) and A (transient generator)
 */
@JvmOverloads
fun marginalDistributionFromMMAP(D: MatrixCell, prec: Double = 1e-14): PHRepresentation {
    if (!checkMMAPRepresentation(D, prec)) {
        throw IllegalArgumentException("MarginalDistributionFromMMAP: Input isn't a valid MMAP representation!")
    }

    return marginalDistributionFromMRAP(D, prec)
}

/**
 * Overload for Array<Matrix>.
 */
@JvmOverloads
fun marginalDistributionFromMMAP(D: Array<Matrix>, prec: Double = 1e-14): PHRepresentation {
    if (!checkMMAPRepresentation(D, prec)) {
        throw IllegalArgumentException("MarginalDistributionFromMMAP: Input isn't a valid MMAP representation!")
    }

    return marginalDistributionFromMRAP(D, prec)
}
