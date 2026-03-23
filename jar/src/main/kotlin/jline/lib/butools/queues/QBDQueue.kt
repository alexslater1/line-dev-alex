/*
 * Copyright (c) 2012-2026, QORE Lab, Imperial College London
 * All rights reserved.
 * Ported from BUTools V2.0
 */
package jline.lib.butools.queues

import jline.lib.butools.MomsFromFactorialMoms
import jline.lib.butools.SimilarityMatrixForVectors
import jline.lib.butools.mam.qbdSolve
import jline.util.matrix.Matrix
import kotlin.math.abs

/**
 * Returns various performance measures of a continuous time QBD queue.
 *
 * @param B Transitions accompanied by service (N x N)
 * @param L Internal transitions (N x N)
 * @param F Transitions accompanied by arrival (N x N)
 * @param L0 Internal transitions when queue is empty (N x N)
 * @param measures Map specifying which measures to compute.
 *        Keys: "ncMoms" (Int), "ncDistr" (Int), "ncDistrMG" (any),
 *              "stMoms" (Int), "stDistr" (double[]), "stDistrME" (any)
 * @param prec Numerical precision
 * @return Map of computed measures
 */
@JvmOverloads
fun qbdQueue(
    B: Matrix,
    L: Matrix,
    F: Matrix,
    L0: Matrix,
    measures: Map<String, Any>,
    prec: Double = 1e-14
): Map<String, Any> {
    val sol = qbdSolve(B, L, F, L0, prec)
    val pi0 = sol.first
    val R = sol.second
    val N = pi0.numCols
    val I = Matrix.eye(N)

    val needST = measures.containsKey("stMoms") || measures.containsKey("stDistr") ||
            measures.containsKey("stDistrME") || measures.containsKey("stDistrPH")
    var eta: Matrix? = null
    var z: Matrix? = null
    var Rh: Matrix? = null

    if (needST) {
        val U = L.add(R.mult(B))
        Rh = U.neg().inv().mult(F)
        val IminusRh = I.sub(Rh!!)
        eta = pi0.mult(F).mult(IminusRh.inv())
        val etaSum = eta!!.elementSum()
        if (etaSum > 0) {
            eta = eta!!.scale(1.0 / etaSum)
        }
        // z = reshape(I, N*N, 1) column-major (Fortran order)
        z = Matrix(N * N, 1)
        for (j in 0 until N) {
            for (i in 0 until N) {
                z!![j * N + i, 0] = I[i, j]
            }
        }
    }

    val result = HashMap<String, Any>()

    // ncMoms
    if (measures.containsKey("ncMoms")) {
        val numMoms = measures["ncMoms"] as Int
        val iR = I.sub(R).inv()
        val factMoms = Matrix(1, numMoms)
        for (m in 1..numMoms) {
            var iRpow = I
            for (k in 0..m) {
                iRpow = iRpow.mult(iR)
            }
            var Rpow = I
            for (k in 0 until m) {
                Rpow = Rpow.mult(R)
            }
            var fact = 1.0
            for (k in 1..m) fact *= k.toDouble()
            factMoms[0, m - 1] = fact * pi0.mult(iRpow).mult(Rpow).elementSum()
        }
        val momsMat = MomsFromFactorialMoms(factMoms)
        result["ncMoms"] = DoubleArray(numMoms) { momsMat[0, it] }
    }

    // ncDistr
    if (measures.containsKey("ncDistr")) {
        val numProbs = measures["ncDistr"] as Int
        val values = DoubleArray(numProbs)
        values[0] = pi0.elementSum()
        var RPow = I
        for (p in 0 until numProbs - 1) {
            RPow = RPow.mult(R)
            values[p + 1] = pi0.mult(RPow).elementSum()
        }
        result["ncDistr"] = values
    }

    // ncDistrMG
    if (measures.containsKey("ncDistrMG")) {
        val iR = I.sub(R).inv()
        val iRR = iR.mult(R)
        val rowSums = Matrix(N, 1)
        for (i in 0 until N) {
            var s = 0.0
            for (j in 0 until N) {
                s += iRR[i, j]
            }
            rowSums[i, 0] = s
        }
        val Bsim = SimilarityMatrixForVectors(rowSums, Matrix.ones(N, 1))
        val Bi = Bsim.inv()
        result["ncDistrMG_alpha"] = pi0.mult(Bi)
        result["ncDistrMG_A"] = Bsim.mult(R).mult(Bi)
    }

    // ncDistrDPH
    if (measures.containsKey("ncDistrDPH")) {
        val iR = I.sub(R).inv()
        val alpha = pi0.mult(R).mult(iR)
        val alphaArr = alpha.toArray1D()
        val diagAlpha = Matrix.diagMatrix(alphaArr)
        val diagAlphaInv = diagAlpha.inv()
        result["ncDistrDPH_alpha"] = alpha
        result["ncDistrDPH_A"] = diagAlphaInv.mult(R.transpose()).mult(diagAlpha)
    }

    // stMoms
    if (measures.containsKey("stMoms") && needST) {
        val numMoms = measures["stMoms"] as Int
        val Z = L.transpose().add(F.transpose()).kron(I).add(B.transpose().kron(Rh!!))
        val iZ = Z.neg().inv()
        // kron(ones(1,N), eta)
        val keta = Matrix(1, N * N)
        for (j in 0 until N) {
            for (i in 0 until N) {
                keta[0, j * N + i] = eta!![0, i]
            }
        }
        val stMoms = DoubleArray(numMoms)
        for (m in 1..numMoms) {
            var fact = 1.0
            for (k in 1..m) fact *= k.toDouble()
            // fact * sum(keta * iZ^(m+1) * (-Z) * z)
            var iZpow = I.kron(I)  // identity of size N^2
            val iZN2 = iZ
            for (k in 0..m) {
                iZpow = iZpow.mult(iZN2)
            }
            stMoms[m - 1] = fact * keta.mult(iZpow).mult(Z.neg()).mult(z!!).elementSum()
        }
        result["stMoms"] = stMoms
    }

    // stDistr
    if (measures.containsKey("stDistr") && needST) {
        val points = measures["stDistr"] as DoubleArray
        val Z = L.transpose().add(F.transpose()).kron(I).add(B.transpose().kron(Rh!!))
        val keta = Matrix(1, N * N)
        for (j in 0 until N) {
            for (i in 0 until N) {
                keta[0, j * N + i] = eta!![0, i]
            }
        }
        val values = DoubleArray(points.size)
        for (p in points.indices) {
            val expZt = Z.scale(points[p]).expm()
            values[p] = 1.0 - keta.mult(expZt).mult(z!!).elementSum()
        }
        result["stDistr"] = values
    }

    // stDistrME
    if (measures.containsKey("stDistrME") && needST) {
        val Z = L.transpose().add(F.transpose()).kron(I).add(B.transpose().kron(Rh!!))
        val keta = Matrix(1, N * N)
        for (j in 0 until N) {
            for (i in 0 until N) {
                keta[0, j * N + i] = eta!![0, i]
            }
        }
        val Bsim = SimilarityMatrixForVectors(z!!, Matrix.ones(z!!.numRows, 1))
        val Bi = Bsim.inv()
        result["stDistrME_alpha"] = keta.mult(Bi)
        result["stDistrME_A"] = Bsim.mult(Z).mult(Bi)
    }

    return result
}
