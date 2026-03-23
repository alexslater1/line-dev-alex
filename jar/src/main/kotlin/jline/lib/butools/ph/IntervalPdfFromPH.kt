/*
 * Copyright (c) 2012-2026, QORE Lab, Imperial College London
 * All rights reserved.
 * Ported from BUTools V2.0
 */
package jline.lib.butools.ph

import jline.util.matrix.Matrix

/**
 * Returns the approximate probability density function of a
 * continuous phase-type distribution, based on the probability
 * of falling into intervals.
 *
 * The interval pdf is computed as the probability of falling
 * into an interval divided by the interval length.
 *
 * @param alpha Initial probability vector of the phase-type distribution (shape 1 x M)
 * @param A Transient generator matrix of the phase-type distribution (shape M x M)
 * @param intBounds The array of interval boundaries. The pdf is the
 *        probability of falling into an interval divided by
 *        the interval length. If the size of intBounds is K,
 *        the size of the result is K-1.
 * @return A pair of (x, y) where x contains the midpoints of the intervals
 *         and y contains the interval pdf values.
 *
 * Notes:
 *   This method is more suitable for comparisons with empirical
 *   density functions than the exact one (given by PdfFromPH).
 */
fun intervalPdfFromPH(alpha: Matrix, A: Matrix, intBounds: DoubleArray): Pair<DoubleArray, DoubleArray> {
    val steps = intBounds.size
    val n = steps - 1
    val x = DoubleArray(n)
    val y = DoubleArray(n)

    for (i in 0 until n) {
        x[i] = (intBounds[i + 1] + intBounds[i]) / 2.0
        val width = intBounds[i + 1] - intBounds[i]
        if (width > 0.0) {
            // CDF(t) = 1 - alpha * expm(A*t) * ones
            val cdfLower = if (intBounds[i] <= 0.0) {
                0.0
            } else {
                1.0 - alpha.mult(A.scale(intBounds[i]).expm()).elementSum()
            }
            val cdfUpper = 1.0 - alpha.mult(A.scale(intBounds[i + 1]).expm()).elementSum()
            y[i] = (cdfUpper - cdfLower) / width
        } else {
            y[i] = 0.0
        }
    }

    return Pair(x, y)
}

/**
 * Returns the approximate probability density function of a
 * matrix-exponential distribution, based on the probability
 * of falling into intervals.
 *
 * This is equivalent to intervalPdfFromPH for ME distributions.
 */
fun intervalPdfFromME(alpha: Matrix, A: Matrix, intBounds: DoubleArray): Pair<DoubleArray, DoubleArray> {
    return intervalPdfFromPH(alpha, A, intBounds)
}
