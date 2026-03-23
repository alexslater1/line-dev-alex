/*
 * Copyright (c) 2012-2026, QORE Lab, Imperial College London
 * All rights reserved.
 * Ported from BUTools V2.0
 *
 * Reference:
 * G Horvath, "Matching marginal moments and lag autocorrelations
 * with MAPs," ValueTools 2013, Torino, Italy (2013).
 */
package jline.lib.butools.map

import jline.lib.butools.APHFrom3Moments
import jline.util.matrix.Matrix
import kotlin.math.ceil
import kotlin.math.sqrt

/**
 * Creates a Markovian arrival process that has the given
 * 2 or 3 marginal moments and lag-1 autocorrelation.
 * The decay of the autocorrelation function can be optionally
 * adjusted as well.
 * The lag-k autocorrelation function rho_k of the
 * resulting MAP is rho_k = r * (corr1/r)^k.
 *
 * @param moms The list of marginal moments to match (length 2 or 3)
 * @param corr1 The lag-1 autocorrelation coefficient to match (default: 0)
 * @param r The decay of the autocorrelation function (optional, null for automatic)
 * @return Pair of (D0, D1) matrices of the Markovian arrival process
 */
@JvmOverloads
fun mapFromFewMomentsAndCorrelations(
    moms: DoubleArray,
    corr1: Double = 0.0,
    r: Double? = null
): Pair<Matrix, Matrix> {
    val m1 = moms[0]
    val c2 = moms[1] / moms[0] / moms[0] - 1.0
    val l3: Double? = if (moms.size > 2) {
        moms[2] * moms[0] / moms[1] / moms[1] - 1.0
    } else {
        null
    }

    val alpha1: Matrix
    val A1: Matrix
    val alpha2: Matrix
    val A2: Matrix
    val p1: Double
    val p2: Double

    if (corr1 >= 0) {
        var rVal = r
        var p1Calc: Double
        var p2Calc: Double

        if (rVal != null && (rVal <= 0 || rVal >= 1)) {
            throw IllegalArgumentException("Parameter r is out of range")
        }

        if (rVal == null) {
            rVal = 2.0 * corr1 / (1.0 + corr1)
            p1Calc = (1.0 - (1.0 + corr1) / 2.0) / (1.0 + c2)
            p2Calc = (1.0 - (1.0 + corr1) / 2.0) * c2 / (1.0 + c2)
        } else {
            p1Calc = (1.0 - corr1 / rVal) / (1.0 + c2)
            p2Calc = (1.0 - corr1 / rVal) * c2 / (1.0 + c2)
        }
        p1 = p1Calc
        p2 = p2Calc

        val m11 = m1 * (1.0 - sqrt(rVal))
        val m12 = m1 * (1.0 + c2 * sqrt(rVal))

        if (l3 == null) {
            val cv21 = (sqrt(c2) * (1.0 + c2) * (1.0 + sqrt(rVal))) /
                (1.0 - sqrt(c2) * (-1.0 + sqrt(rVal)) + c2 * sqrt(rVal))
            val cv22 = -(c2 * (1.0 + c2) * (-1.0 + rVal)) /
                ((1.0 + c2 * sqrt(rVal)) * (1.0 - sqrt(c2) * (-1.0 + sqrt(rVal)) + c2 * sqrt(rVal)))
            val m21 = (cv21 + 1.0) * m11 * m11
            val m22 = (cv22 + 1.0) * m12 * m12
            val aph1Result = aphFrom2Moments(doubleArrayOf(m11, m21))
            alpha1 = aph1Result.first
            A1 = aph1Result.second
            val aph2Result = aphFrom2Moments(doubleArrayOf(m12, m22))
            alpha2 = aph2Result.first
            A2 = aph2Result.second
        } else {
            val cv21 = (c2 + sqrt(rVal)) / (1.0 - sqrt(rVal))
            val cv22 = c2 * (1.0 - sqrt(rVal)) / (1.0 + c2 * sqrt(rVal))
            val l31 = ((1.0 + c2) * l3) /
                (c2 * (1.0 - sqrt(rVal)) + sqrt((1.0 + c2 * sqrt(rVal)) * c2 * (1.0 - sqrt(rVal))))
            val l32 = ((1.0 + c2) * l3) /
                ((1.0 + c2 * sqrt(rVal)) + sqrt((1.0 + c2 * sqrt(rVal)) * c2 * (1.0 - sqrt(rVal))))
            val m21 = (cv21 + 1.0) * m11 * m11
            val m22 = (cv22 + 1.0) * m12 * m12
            val m31 = (l31 + 1.0) * m21 * m21 / m11
            val m32 = (l32 + 1.0) * m22 * m22 / m12
            val aph1Result = APHFrom3Moments(doubleArrayOf(m11, m21, m31))
            alpha1 = aph1Result.initProb
            A1 = aph1Result.getParam(3).value as Matrix
            val aph2Result = APHFrom3Moments(doubleArrayOf(m12, m22, m32))
            alpha2 = aph2Result.initProb
            A2 = aph2Result.getParam(3).value as Matrix
        }
    } else {
        var rVal = r

        if (c2 >= 1) {
            if (rVal != null && (rVal <= 0 || rVal >= 1.0 / c2)) {
                throw IllegalArgumentException("Parameter r is out of range")
            }
            if (rVal == null) {
                rVal = -(2.0 * corr1) / (1.0 - c2 * corr1)
                p1 = 0.5 * (1.0 + (1.0 - c2 * corr1) / 2.0)
            } else {
                p1 = 0.5 * (1.0 - corr1 / rVal)
            }
            p2 = p1
        } else {
            if (rVal != null && (rVal <= 0 || rVal >= 1)) {
                throw IllegalArgumentException("Parameter r is out of range")
            }
            if (rVal == null) {
                rVal = -(2.0 * corr1) / (1.0 - corr1)
                p1 = 0.5 * (1.0 + (1.0 - corr1) / 2.0)
            } else {
                p1 = 0.5 * (1.0 - corr1 / rVal)
            }
            p2 = p1
        }

        val m11 = m1 * (1.0 - sqrt(c2 * rVal))
        val m12 = m1 * (1.0 + sqrt(c2 * rVal))

        if (l3 == null) {
            val cv21 = c2 * (1.0 - rVal) / (1.0 - sqrt(c2 * rVal))
            val cv22 = c2 * (1.0 - rVal) / (1.0 + sqrt(c2 * rVal))
            val m21 = (cv21 + 1.0) * m11 * m11
            val m22 = (cv22 + 1.0) * m12 * m12
            val aph1Result = aphFrom2Moments(doubleArrayOf(m11, m21))
            alpha1 = aph1Result.first
            A1 = aph1Result.second
            val aph2Result = aphFrom2Moments(doubleArrayOf(m12, m22))
            alpha2 = aph2Result.first
            A2 = aph2Result.second
        } else {
            val cv21 = (c2 + sqrt(c2 * rVal)) / (1.0 - sqrt(c2 * rVal))
            val cv22 = (c2 - sqrt(c2 * rVal)) / (1.0 + sqrt(c2 * rVal))
            val l31 = 2.0 * l3 / (1.0 - sqrt(c2 * rVal) + sqrt(1.0 - c2 * rVal))
            val l32 = 2.0 * l3 / (1.0 + sqrt(c2 * rVal) + sqrt(1.0 - c2 * rVal))
            val m21 = (cv21 + 1.0) * m11 * m11
            val m22 = (cv22 + 1.0) * m12 * m12
            val m31 = (l31 + 1.0) * m21 * m21 / m11
            val m32 = (l32 + 1.0) * m22 * m22 / m12
            val aph1Result = APHFrom3Moments(doubleArrayOf(m11, m21, m31))
            alpha1 = aph1Result.initProb
            A1 = aph1Result.getParam(3).value as Matrix
            val aph2Result = APHFrom3Moments(doubleArrayOf(m12, m22, m32))
            alpha2 = aph2Result.initProb
            A2 = aph2Result.getParam(3).value as Matrix
        }
    }

    val N1 = A1.numRows
    val N2 = A2.numRows

    // Build D0 block diagonal
    val D0 = Matrix.zeros(N1 + N2, N1 + N2)
    for (i in 0 until N1) {
        for (j in 0 until N1) {
            D0[i, j] = A1[i, j]
        }
    }
    for (i in 0 until N2) {
        for (j in 0 until N2) {
            D0[N1 + i, N1 + j] = A2[i, j]
        }
    }

    // Build D1 with Kronecker-like structure
    // D1[:N1,:N1] = -sum(A1,1) * alpha1 * (1-p1)
    // D1[:N1,N1:] = -sum(A1,1) * alpha2 * p1
    // D1[N1:,:N1] = -sum(A2,1) * alpha1 * p2
    // D1[N1:,N1:] = -sum(A2,1) * alpha2 * (1-p2)
    val D1 = Matrix.zeros(N1 + N2, N1 + N2)

    // Compute row sums of A1 (exit rate vectors, negative)
    val a1exit = DoubleArray(N1)
    for (i in 0 until N1) {
        var sum = 0.0
        for (j in 0 until N1) {
            sum += A1[i, j]
        }
        a1exit[i] = -sum // -sum(A1,1)
    }

    // Compute row sums of A2 (exit rate vectors, negative)
    val a2exit = DoubleArray(N2)
    for (i in 0 until N2) {
        var sum = 0.0
        for (j in 0 until N2) {
            sum += A2[i, j]
        }
        a2exit[i] = -sum // -sum(A2,1)
    }

    // D1[:N1,:N1] = -sum(A1,1) * alpha1 * (1-p1)
    for (i in 0 until N1) {
        for (j in 0 until N1) {
            D1[i, j] = a1exit[i] * alpha1[0, j] * (1.0 - p1)
        }
    }

    // D1[:N1,N1:] = -sum(A1,1) * alpha2 * p1
    for (i in 0 until N1) {
        for (j in 0 until N2) {
            D1[i, N1 + j] = a1exit[i] * alpha2[0, j] * p1
        }
    }

    // D1[N1:,:N1] = -sum(A2,1) * alpha1 * p2
    for (i in 0 until N2) {
        for (j in 0 until N1) {
            D1[N1 + i, j] = a2exit[i] * alpha1[0, j] * p2
        }
    }

    // D1[N1:,N1:] = -sum(A2,1) * alpha2 * (1-p2)
    for (i in 0 until N2) {
        for (j in 0 until N2) {
            D1[N1 + i, N1 + j] = a2exit[i] * alpha2[0, j] * (1.0 - p2)
        }
    }

    return Pair(D0, D1)
}

/**
 * Returns an acyclic PH which has the same 2 moments as given.
 * Detects the order and the structure automatically to match
 * the given moments.
 *
 * @param moms The moments to match (length 2): first moment and second moment
 * @return Pair of (alpha, A) where alpha is the initial probability vector (1xN)
 *         and A is the transient generator matrix (NxN)
 */
private fun aphFrom2Moments(moms: DoubleArray): Pair<Matrix, Matrix> {
    val m1 = moms[0]
    val m2 = moms[1]
    val cv2 = m2 / m1 / m1 - 1.0
    val lambd = 1.0 / m1
    val N = maxOf(ceil(1.0 / cv2).toInt(), 2)
    val p = 1.0 / (cv2 + 1.0 + (cv2 - 1.0) / (N - 1))

    val A = Matrix(N, N)
    val diagVal = -lambd * p * N
    for (i in 0 until N) {
        A[i, i] = diagVal
    }
    for (i in 0 until N - 1) {
        A[i, i + 1] = -A[i, i]
    }
    A[N - 1, N - 1] = -lambd * N

    val alpha = Matrix.zeros(1, N)
    alpha[0, 0] = p
    alpha[0, N - 1] = 1.0 - p

    return Pair(alpha, A)
}
