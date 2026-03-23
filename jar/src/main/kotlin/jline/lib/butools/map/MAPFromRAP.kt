/*
 * Copyright (c) 2012-2026, QORE Lab, Imperial College London
 * All rights reserved.
 * Ported from BUTools V2.0
 */
package jline.lib.butools.map

import jline.util.matrix.Matrix
import jline.util.matrix.MatrixCell

/**
 * Obtains a Markovian representation of a continuous rational
 * arrival process of the same size, if possible.
 *
 * @param H0 The H0 matrix of the rational arrival process
 * @param H1 The H1 matrix of the rational arrival process
 * @param prec A representation is considered to be Markovian if it is closer than this precision
 * @return Pair of (D0, D1) matrices of the continuous Markovian arrival process
 */
@JvmOverloads
fun mapFromRAP(H0: Matrix, H1: Matrix, prec: Double = 1e-14): Pair<Matrix, Matrix> {
    if (!checkRAPRepresentation(H0, H1, prec)) {
        throw IllegalArgumentException("MAPFromRAP: Input isn't a valid RAP representation!")
    }

    val H = MatrixCell(2)
    H[0] = H0
    H[1] = H1

    val Y = mmapFromMRAP(H, prec)

    return Pair(Y[0], Y[1])
}
