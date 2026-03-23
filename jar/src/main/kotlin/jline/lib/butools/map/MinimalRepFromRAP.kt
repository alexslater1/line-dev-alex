/*
 * Copyright (c) 2012-2026, QORE Lab, Imperial College London
 * All rights reserved.
 * Ported from BUTools V2.0
 *
 * Reference:
 * P. Buchholz, M. Telek, "On minimal representation of rational arrival
 * processes." Madrid Conference on Queueing theory (MCQT), June 2010.
 */
package jline.lib.butools.map

import jline.util.matrix.Matrix
import jline.util.matrix.MatrixCell

/**
 * Returns the minimal representation of a rational arrival process.
 *
 * @param H0 The H0 matrix of the rational arrival process
 * @param H1 The H1 matrix of the rational arrival process
 * @param how Determines how the representation is minimized:
 *            "cont" = controllability, "obs" = observability,
 *            "obscont" = both (default)
 * @param precision Precision used by the Staircase algorithm (default 1e-12)
 * @return Pair of (D0, D1) matrices of the minimal representation
 */
@JvmOverloads
fun minimalRepFromRAP(H0: Matrix, H1: Matrix, how: String = "obscont", precision: Double = 1e-12): Pair<Matrix, Matrix> {
    val H = MatrixCell(2)
    H[0] = H0
    H[1] = H1
    val result = minimalRepFromMRAP(H, how, precision)
    return Pair(result[0], result[1])
}
