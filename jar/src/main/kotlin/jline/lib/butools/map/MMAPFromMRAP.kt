/*
 * Copyright (c) 2012-2026, QORE Lab, Imperial College London
 * All rights reserved.
 * Ported from BUTools V2.0
 */
package jline.lib.butools.map

import jline.lib.butools.reptrans.findMarkovianRepresentation
import jline.util.matrix.Matrix
import jline.util.matrix.MatrixCell

/**
 * Obtains a Markovian representation of a continuous marked rational
 * arrival process of the same size, if possible.
 *
 * @param H The H0...HN matrices of the MRAP to transform (as MatrixCell)
 * @param prec A representation is considered to be Markovian if it is closer than this precision
 * @return The D0...DN matrices of the MMAP (if found)
 */
@JvmOverloads
fun mmapFromMRAP(H: MatrixCell, prec: Double = 1e-14): MatrixCell {
    if (!checkMRAPRepresentation(H, prec)) {
        throw IllegalArgumentException("MMAPFromMRAP: Input isn't a valid MRAP representation!")
    }

    // Convert MatrixCell to List<Matrix>
    val HList = (0 until H.size()).map { H[it] }

    // Transformation function: nH = inv(B) * oH * B for each matrix
    val transfun = { oH: List<Matrix>, B: Matrix ->
        val Binv = B.inv()
        oH.map { Binv.mult(it).mult(B) }
    }

    // Evaluation function for continuous MAP:
    // For continuous-time MAP, the Markovian property requires:
    //   - D0 off-diagonal elements >= 0
    //   - D0 diagonal elements <= 0
    //   - D1...DK elements >= 0
    // We measure distance from Markovian by the negative elements
    // (no upper bound of 1 as in discrete case).
    val evalfun = { oH: List<Matrix>, k: Int ->
        if (k % 2 == 0) {
            // Measure worst-case negative element across all matrices
            var dist = Double.POSITIVE_INFINITY
            for (i in oH.indices) {
                if (i == 0) {
                    // For D0: off-diagonal must be >= 0, diagonal must be <= 0
                    val n = oH[0].numRows
                    for (r in 0 until n) {
                        for (c in 0 until n) {
                            if (r != c) {
                                // Off-diagonal: should be >= 0
                                dist = minOf(dist, oH[0][r, c])
                            }
                        }
                    }
                } else {
                    // For D1..DK: all elements must be >= 0
                    dist = minOf(dist, oH[i].elementMin())
                }
            }
            -dist
        } else {
            // Sum of negative elements
            var dist = 0.0
            for (i in oH.indices) {
                if (i == 0) {
                    val n = oH[0].numRows
                    for (r in 0 until n) {
                        for (c in 0 until n) {
                            if (r != c) {
                                val v = oH[0][r, c]
                                if (v < 0) dist += v
                            }
                        }
                    }
                } else {
                    for (r in 0 until oH[i].numRows) {
                        for (c in 0 until oH[i].numCols) {
                            val v = oH[i][r, c]
                            if (v < 0) dist += v
                        }
                    }
                }
            }
            -dist
        }
    }

    val result = findMarkovianRepresentation(HList, transfun, evalfun, prec)

    // Convert back to MatrixCell
    val resultCell = MatrixCell(result.size)
    for (i in result.indices) {
        resultCell[i] = result[i]
    }
    return resultCell
}

/**
 * Overload for Array<Matrix>.
 */
@JvmOverloads
fun mmapFromMRAP(H: Array<Matrix>, prec: Double = 1e-14): MatrixCell {
    val cell = MatrixCell(H.size)
    for (i in H.indices) {
        cell[i] = H[i]
    }
    return mmapFromMRAP(cell, prec)
}
