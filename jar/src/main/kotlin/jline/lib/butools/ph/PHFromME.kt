/*
 * Copyright (c) 2012-2026, QORE Lab, Imperial College London
 * All rights reserved.
 * Ported from BUTools V2.0
 */
package jline.lib.butools.ph

import jline.lib.butools.reptrans.findMarkovianRepresentation
import jline.util.matrix.Matrix

/**
 * Obtains a Markovian representation of a matrix exponential
 * distribution of the same size, if possible, using elementary
 * similarity transformations.
 *
 * @param alpha The initial vector of the matrix-exponential distribution (shape 1 x M)
 * @param A The matrix parameter of the matrix-exponential distribution (shape M x M)
 * @param prec A representation is considered to be a Markovian one
 *        if it is closer than the precision. The default value is 1e-14.
 * @return PHRepresentation containing the initial probability vector and
 *         transient generator matrix of the Markovian representation.
 * @throws IllegalArgumentException if the input is not a valid ME representation.
 *
 * References:
 *   G Horvath, M Telek, "A minimal representation of Markov arrival processes
 *   and a moments matching method," Performance Evaluation 64:(9-12)
 *   pp. 1153-1168. (2007)
 */
fun phFromME(alpha: Matrix, A: Matrix, prec: Double = 1e-14): PHRepresentation {
    if (!checkMERepresentation(alpha, A, prec)) {
        throw IllegalArgumentException("PHFromME: Input is not a valid ME representation!")
    }

    val repList = listOf(alpha, A)

    val transfun = { oH: List<Matrix>, B: Matrix ->
        val Binv = B.inv()
        listOf(oH[0].mult(B), Binv.mult(oH[1]).mult(B))
    }

    val evalfun = { oH: List<Matrix>, k: Int ->
        val ao = oH[0]  // alpha (1 x M)
        val Ao = oH[1]  // A (M x M)
        val M = Ao.numRows

        // av = sum(-Ao, axis=1) = exit rate vector (negative row sums of A)
        val av = DoubleArray(M)
        for (i in 0 until M) {
            var rowSum = 0.0
            for (j in 0 until M) {
                rowSum += Ao[i, j]
            }
            av[i] = -rowSum
        }

        // Ad = Ao - diag(diag(Ao)), the off-diagonal part
        // We only need to find minimum of off-diagonal elements for k%2==0
        // or sum of negative off-diagonal elements for k%2==1

        if (k % 2 == 0) {
            // Return -min(min(ao), min(av), min(Ad_offdiag))
            var minVal = Double.MAX_VALUE

            // min(ao)
            for (j in 0 until ao.numCols) {
                val v = ao[0, j]
                if (v < minVal) minVal = v
            }

            // min(av)
            for (i in 0 until M) {
                if (av[i] < minVal) minVal = av[i]
            }

            // min(Ad) - off-diagonal elements of Ao
            for (i in 0 until M) {
                for (j in 0 until M) {
                    if (i != j) {
                        val v = Ao[i, j]
                        if (v < minVal) minVal = v
                    }
                }
            }

            -minVal
        } else {
            // Return -(sum of negative entries in ao + sum of negative entries in av + sum of negative off-diagonal entries in Ao)
            var sumNeg = 0.0

            // sum of negative entries in ao
            for (j in 0 until ao.numCols) {
                val v = ao[0, j]
                if (v < 0.0) sumNeg += v
            }

            // sum of negative entries in av
            for (i in 0 until M) {
                if (av[i] < 0.0) sumNeg += av[i]
            }

            // sum of negative off-diagonal entries in Ao
            for (i in 0 until M) {
                for (j in 0 until M) {
                    if (i != j) {
                        val v = Ao[i, j]
                        if (v < 0.0) sumNeg += v
                    }
                }
            }

            -sumNeg
        }
    }

    val result = findMarkovianRepresentation(repList, transfun, evalfun, prec)
    return PHRepresentation(result[0], result[1])
}
