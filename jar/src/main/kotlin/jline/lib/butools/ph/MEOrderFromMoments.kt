/*
 * Copyright (c) 2012-2026, QORE Lab, Imperial College London
 * All rights reserved.
 * Ported from BUTools V2.0
 */
package jline.lib.butools.ph

import jline.lib.butools.ReducedMomsFromMoms
import jline.util.matrix.Matrix
import kotlin.math.abs

/**
 * Returns the order of ME distribution that can realize
 * the given moments.
 *
 * The order is determined using the Hankel determinant approach:
 * we build Hankel matrices of increasing size from the reduced
 * moments (prepended with 1) and check when the determinant
 * becomes zero.
 *
 * @param moms The list of moments
 * @param prec Precision used to detect if the determinant of the
 *        Hankel matrix is zero. The default value is 1e-12.
 * @return The order of ME distribution that can realize the given moments
 *
 * References:
 *   L. Bodrog, A. Horvath, M. Telek, "Moment characterization of matrix
 *   exponential and Markovian arrival processes," Annals of Operations
 *   Research, vol. 160, pp. 51-68, 2008.
 */
fun meOrderFromMoments(moms: DoubleArray, prec: Double = 1e-12): Int {
    val sizem = (moms.size + 1) / 2
    val rmoms = ReducedMomsFromMoms(moms)
    // Prepend 1 to reduced moments: rmoms_full = [1, rmoms[0], rmoms[1], ...]
    val rmomsAll = DoubleArray(rmoms.size + 1)
    rmomsAll[0] = 1.0
    for (i in rmoms.indices) {
        rmomsAll[i + 1] = rmoms[i]
    }

    for (k in 1..sizem) {
        val hankel = Matrix(k, k)
        for (i in 0 until k) {
            for (j in 0 until k) {
                val idx = i + j
                if (idx < rmomsAll.size) {
                    hankel[i, j] = rmomsAll[idx]
                }
            }
        }
        if (abs(hankel.det()) < prec) {
            return k - 1
        }
    }
    return sizem
}
