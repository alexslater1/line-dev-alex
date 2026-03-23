/*
 * Copyright (c) 2012-2026, QORE Lab, Imperial College London
 * All rights reserved.
 * Ported from BUTools V2.0
 */
package jline.lib.butools.queues

import jline.lib.butools.MomsFromFactorialMoms
import jline.lib.butools.SimilarityMatrixForVectors
import jline.lib.butools.mam.qbdSolve
import jline.lib.butools.mc.ctmcSolve
import jline.lib.butools.mc.dtmcSolve
import jline.util.matrix.Matrix

/**
 * Returns various performance measures of a continuous time MAP/MAP/1 queue.
 *
 * @param D0 Arrival MAP transitions without arrivals (Na x Na)
 * @param D1 Arrival MAP transitions with arrivals (Na x Na)
 * @param S0 Service MAP transitions without service (Ns x Ns)
 * @param S1 Service MAP transitions with service (Ns x Ns)
 * @param measures Map specifying which measures to compute
 * @param prec Numerical precision
 * @return Map of computed measures
 */
@JvmOverloads
fun mapmap1(
    D0: Matrix,
    D1: Matrix,
    S0: Matrix,
    S1: Matrix,
    measures: Map<String, Any>,
    prec: Double = 1e-14
): Map<String, Any> {
    val Na = D0.numRows
    val Ns = S0.numRows
    val IA = Matrix.eye(Na)
    val IS = Matrix.eye(Ns)

    // QBD parameters via Kronecker products
    val B = IA.kron(S1)
    val L = D0.kron(IS).add(IA.kron(S0))
    val F = D1.kron(IS)
    val L0 = D0.kron(IS)

    val sol = qbdSolve(B, L, F, L0, prec)
    val pi0 = sol.first
    val R = sol.second
    val N = pi0.numCols
    val I = Matrix.eye(N)

    val needST = measures.containsKey("stMoms") || measures.containsKey("stDistr") ||
            measures.containsKey("stDistrME") || measures.containsKey("stDistrPH")
    var eta: Matrix? = null
    var T: Matrix? = null
    var Rh: Matrix? = null

    if (needST) {
        val U = L.add(R.mult(B))
        Rh = U.neg().inv().mult(F)
        T = IA.kron(S0).add(Rh!!.mult(B))
        val IminusRh = I.sub(Rh!!)
        eta = pi0.mult(F).mult(IminusRh.inv())
        val etaSum = eta!!.elementSum()
        if (etaSum > 0) {
            eta = eta!!.scale(1.0 / etaSum)
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

    // stMoms
    if (measures.containsKey("stMoms") && needST) {
        val numMoms = measures["stMoms"] as Int
        val iT = T!!.neg().inv()
        val stMoms = DoubleArray(numMoms)
        for (m in 1..numMoms) {
            var fact = 1.0
            for (k in 1..m) fact *= k.toDouble()
            var iTPow = I
            for (k in 0 until m) iTPow = iTPow.mult(iT)
            stMoms[m - 1] = fact * eta!!.mult(iTPow).elementSum()
        }
        result["stMoms"] = stMoms
    }

    // stDistr
    if (measures.containsKey("stDistr") && needST) {
        val points = measures["stDistr"] as DoubleArray
        val values = DoubleArray(points.size)
        for (p in points.indices) {
            val expTt = T!!.scale(points[p]).expm()
            values[p] = 1.0 - eta!!.mult(expTt).elementSum()
        }
        result["stDistr"] = values
    }

    // stDistrME
    if (measures.containsKey("stDistrME") && needST) {
        result["stDistrME_alpha"] = eta!!
        result["stDistrME_A"] = T!!
    }

    return result
}
