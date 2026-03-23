/*
 * Copyright (c) 2012-2026, QORE Lab, Imperial College London
 * All rights reserved.
 * Ported from BUTools V2.0
 */
package jline.lib.butools.ph

import jline.util.matrix.Matrix

/**
 * Checks if the given ME distribution has positive density.
 *
 * This is done by trying to transform the ME distribution to
 * a monocyclic PH representation. If successful, the density
 * is positive.
 *
 * @param alpha Initial vector of the matrix-exponential distribution to check (shape 1 x M)
 * @param A Matrix parameter of the matrix-exponential distribution to check (shape M x M)
 * @param maxSize The procedure tries to transform the ME distribution
 *        to phase-type up to order maxSize. The default value is 100.
 * @param prec Numerical precision. The default value is 1e-14.
 * @return True, if the given matrix-exponential distribution has a positive density.
 *
 * Notes:
 *   This procedure calls MonocyclicPHFromME, and can be time consuming.
 */
fun checkMEPositiveDensity(alpha: Matrix, A: Matrix, maxSize: Int = 100, prec: Double = 1e-14): Boolean {
    if (!checkMERepresentation(alpha, A, prec)) {
        return false
    }
    return try {
        val result = monocyclicPHFromME(alpha, A, maxSize, prec)
        checkPHRepresentation(result.alpha, result.A, prec)
    } catch (e: Exception) {
        false
    }
}
