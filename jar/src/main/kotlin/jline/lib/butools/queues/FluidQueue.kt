/*
 * Copyright (c) 2012-2026, QORE Lab, Imperial College London
 * All rights reserved.
 * Ported from BUTools V2.0
 */
package jline.lib.butools.queues

import jline.lib.butools.SimilarityMatrixForVectors
import jline.lib.butools.mam.generalFluidSolve
import jline.util.matrix.Matrix

/**
 * Returns various performance measures of a fluid queue.
 *
 * @param Q Generator of background Markov chain (N x N)
 * @param Rin Diagonal matrix of fluid input rates (N x N)
 * @param Rout Diagonal matrix of fluid output rates (N x N)
 * @param measures Map specifying which measures to compute
 * @param Q0 Generator when fluid level is zero (null = same as Q)
 * @param prec Numerical precision
 * @return Map of computed measures
 */
@JvmOverloads
fun fluidQueue(
    Q: Matrix,
    Rin: Matrix,
    Rout: Matrix,
    measures: Map<String, Any>,
    Q0: Matrix? = null,
    prec: Double = 1e-14
): Map<String, Any> {
    val Rdiff = Rin.sub(Rout)
    val sol = generalFluidSolve(Q, Rdiff, Q0, prec)
    val mass0 = sol.mass0
    val ini = sol.ini
    val K = sol.K
    val clo = sol.clo

    val needST = measures.containsKey("stMoms") || measures.containsKey("stDistr") ||
            measures.containsKey("stDistrME") || measures.containsKey("stDistrPH")

    var iniKi: Matrix? = null
    var lambd = 0.0
    val Nq = Q.numRows

    if (needST) {
        // iniKi = ini * inv(-K) via linsolve: K' \ (-ini') transposed
        iniKi = K.transpose().leftMatrixDivide(ini.transpose().neg()).transpose()
        lambd = mass0.mult(Rin).elementSum() + iniKi!!.mult(clo).mult(Rin).elementSum()
    }

    val result = HashMap<String, Any>()

    // flMoms
    if (measures.containsKey("flMoms")) {
        val numMoms = measures["flMoms"] as Int
        val iK = K.neg().inv()
        val flMoms = DoubleArray(numMoms)
        for (m in 1..numMoms) {
            var fact = 1.0
            for (k in 1..m) fact *= k.toDouble()
            var iKpow = Matrix.eye(K.numRows)
            for (k in 0..m) {
                iKpow = iKpow.mult(iK)
            }
            flMoms[m - 1] = fact * ini.mult(iKpow).mult(clo).elementSum()
        }
        result["flMoms"] = flMoms
    }

    // flDistr
    if (measures.containsKey("flDistr")) {
        val points = measures["flDistr"] as DoubleArray
        val iK = K.neg().inv()
        val Ifl = Matrix.eye(K.numRows)
        val mass0Sum = mass0.elementSum()
        val values = DoubleArray(points.size)
        for (p in points.indices) {
            val expKt = K.scale(points[p]).expm()
            values[p] = mass0Sum + ini.mult(Ifl.sub(expKt)).mult(iK).mult(clo).elementSum()
        }
        result["flDistr"] = values
    }

    // flDistrPH
    if (measures.containsKey("flDistrPH")) {
        // Delta = Diag(Linsolve(K.T, -ini.T)) = Diag((K^T)^{-1} * (-ini^T))
        val xCol = K.transpose().leftMatrixDivide(ini.transpose().neg())
        val deltaArr = xCol.toArray1D()
        val Delta = Matrix.diagMatrix(deltaArr)
        val DeltaInv = Delta.inv()
        // A = Delta^{-1} * K^T * Delta
        result["flDistrPH_A"] = DeltaInv.mult(K.transpose()).mult(Delta)
        // alpha = sum(clo, 1)^T * Delta
        val cloRowSumsPH = clo.sumRows() // column vector
        result["flDistrPH_alpha"] = cloRowSumsPH.transpose().mult(Delta)
    }

    // flDistrME
    if (measures.containsKey("flDistrME")) {
        val iK = K.neg().inv()
        val cloRowSums = Matrix(clo.numRows, 1)
        for (i in 0 until clo.numRows) {
            var s = 0.0
            for (j in 0 until clo.numCols) {
                s += clo[i, j]
            }
            cloRowSums[i, 0] = s
        }
        val target = iK.mult(cloRowSums)
        val Bsim = SimilarityMatrixForVectors(target, Matrix.ones(K.numRows, 1))
        val Bi = Bsim.inv()
        result["flDistrME_alpha"] = ini.mult(Bi)
        result["flDistrME_A"] = Bsim.mult(K).mult(Bi)
    }

    // stMoms
    if (measures.containsKey("stMoms") && needST) {
        val numMoms = measures["stMoms"] as Int
        val Ifl = Matrix.eye(K.numRows)
        val Z = Q.transpose().kron(Ifl).add(Rout.kron(K))
        val iZ = Z.neg().inv()
        // kini = kron(ones(1,N), ini/lambd)
        val iniScaled = ini.scale(1.0 / lambd)
        val kini = Matrix(1, Nq * iniScaled.numCols)
        for (j in 0 until Nq) {
            for (i in 0 until iniScaled.numCols) {
                kini[0, j * iniScaled.numCols + i] = iniScaled[0, i]
            }
        }
        // kclo = reshape(-K^{-1}*clo*Rin, (N*ini.size, 1), 'F') column-major
        val iK = K.neg().inv()
        val kcloMat = iK.mult(clo).mult(Rin)
        val kclo = Matrix(Nq * ini.numCols, 1)
        for (j in 0 until Nq) {
            for (i in 0 until ini.numCols) {
                kclo[j * ini.numCols + i, 0] = kcloMat[i, j]
            }
        }
        val stMoms = DoubleArray(numMoms)
        for (m in 1..numMoms) {
            var fact = 1.0
            for (k in 1..m) fact *= k.toDouble()
            var iZpow = Matrix.eye(Z.numRows)
            for (k in 0..m) {
                iZpow = iZpow.mult(iZ)
            }
            stMoms[m - 1] = fact * kini.mult(iZpow).mult(Z.neg()).mult(kclo).elementSum()
        }
        result["stMoms"] = stMoms
    }

    // stDistr
    if (measures.containsKey("stDistr") && needST) {
        val points = measures["stDistr"] as DoubleArray
        val Ifl = Matrix.eye(K.numRows)
        val Z = Q.transpose().kron(Ifl).add(Rout.kron(K))
        val iniScaled = ini.scale(1.0 / lambd)
        val kini = Matrix(1, Nq * iniScaled.numCols)
        for (j in 0 until Nq) {
            for (i in 0 until iniScaled.numCols) {
                kini[0, j * iniScaled.numCols + i] = iniScaled[0, i]
            }
        }
        val iK = K.neg().inv()
        val kcloMat = iK.mult(clo).mult(Rin)
        val kclo = Matrix(Nq * ini.numCols, 1)
        for (j in 0 until Nq) {
            for (i in 0 until ini.numCols) {
                kclo[j * ini.numCols + i, 0] = kcloMat[i, j]
            }
        }
        val iZ = Z.neg().inv()
        val values = DoubleArray(points.size)
        for (p in points.indices) {
            val expZt = Z.scale(points[p]).expm()
            values[p] = 1.0 - kini.mult(expZt).mult(iZ).mult(Z.neg()).mult(kclo).elementSum()
        }
        result["stDistr"] = values
    }

    // stDistrME
    if (measures.containsKey("stDistrME") && needST) {
        val Ifl = Matrix.eye(K.numRows)
        val Z = Q.transpose().kron(Ifl).add(Rout.kron(K))
        val iniScaled = ini.scale(1.0 / lambd)
        val kini = Matrix(1, Nq * iniScaled.numCols)
        for (j in 0 until Nq) {
            for (i in 0 until iniScaled.numCols) {
                kini[0, j * iniScaled.numCols + i] = iniScaled[0, i]
            }
        }
        val iK = K.neg().inv()
        val kcloMat = iK.mult(clo).mult(Rin)
        val kclo = Matrix(Nq * ini.numCols, 1)
        for (j in 0 until Nq) {
            for (i in 0 until ini.numCols) {
                kclo[j * ini.numCols + i, 0] = kcloMat[i, j]
            }
        }
        val Bsim = SimilarityMatrixForVectors(kclo, Matrix.ones(kclo.numRows, 1))
        val Bi = Bsim.inv()
        result["stDistrME_alpha"] = kini.mult(Bi)
        result["stDistrME_A"] = Bsim.mult(Z).mult(Bi)
    }

    return result
}
