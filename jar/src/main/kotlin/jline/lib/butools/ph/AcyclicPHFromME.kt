/*
 * Copyright (c) 2012-2026, QORE Lab, Imperial College London
 * All rights reserved.
 * Ported from BUTools V2.0
 */
package jline.lib.butools.ph

import jline.lib.butools.reptrans.extendToMarkovian
import jline.lib.butools.reptrans.similarityMatrix
import jline.lib.butools.reptrans.transformToAcyclic
import jline.util.matrix.Matrix

/**
 * Transforms an arbitrary matrix-exponential representation
 * to an acyclic phase-type representation.
 *
 * @param alpha Initial vector of the distribution (shape 1 x N)
 * @param A Matrix parameter of the distribution (shape N x N)
 * @param maxSize The maximum number of phases for the result. The default value is 100.
 * @param prec Vector and matrix entries smaller than the precision
 *        are considered to be zeros. The default value is 1e-14.
 * @return PHRepresentation containing the initial probability vector and
 *         transient generator matrix of the Markovian acyclic representation.
 * @throws IllegalArgumentException if no Markovian acyclic representation has been found.
 *
 * References:
 *   Mocanu, S., Commault, C.: "Sparse representations of phase-type distributions,"
 *   Stoch. Models 15, 759-778 (1999)
 */
fun acyclicPHFromME(alpha: Matrix, A: Matrix, maxSize: Int = 100, prec: Double = 1e-14): PHRepresentation {
    val G = transformToAcyclic(A, maxSize, prec)

    // Find transformation matrix
    val T = similarityMatrix(A, G)
    var gamma = alpha.mult(T)

    // Check if gamma has negative entries
    var hasNeg = false
    for (i in 0 until gamma.length()) {
        if (gamma[0, i] < -prec) {
            hasNeg = true
            break
        }
    }

    return if (hasNeg) {
        val result = extendToMarkovian(gamma, G, maxSize, prec)
        if (!checkPHRepresentation(result.beta, result.B, prec)) {
            throw IllegalArgumentException("AcyclicPHFromME: No acyclic representation found up to the given size and precision!")
        }
        PHRepresentation(result.beta, result.B)
    } else {
        if (!checkPHRepresentation(gamma, G, prec)) {
            throw IllegalArgumentException("AcyclicPHFromME: Result is not a valid PH representation!")
        }
        PHRepresentation(gamma, G)
    }
}
