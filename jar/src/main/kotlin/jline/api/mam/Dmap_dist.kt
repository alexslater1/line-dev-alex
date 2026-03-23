/**
 * @file Distance measures for discrete-time MAPs (D-MAPs)
 *
 * Provides squared L2 distance between joint PMFs and autocorrelation
 * functions of discrete-time Markovian Arrival Processes.
 *
 * Reference (continuous-time formulation):
 *   G. Horvath, "Measuring the distance between MAPs and some
 *   applications," in Proc. ASMTA 2015, LNCS 9081, pp. 95-109.
 *   https://link.springer.com/chapter/10.1007/978-3-319-18579-8_8
 *
 * Discrete-time extension by QORE Lab (https://qore.doc.ic.ac.uk/)
 *
 * @since LINE 3.0
 */
package jline.api.mam

import jline.util.matrix.Matrix

/**
 * Computes the joint PMF inner product of two D-MAPs via recursive discrete Lyapunov equations.
 *
 * @param D0A sub-stochastic matrix for non-arrival transitions of the first D-MAP
 * @param D1A arrival transition matrix of the first D-MAP
 * @param D0B sub-stochastic matrix for non-arrival transitions of the second D-MAP
 * @param D1B arrival transition matrix of the second D-MAP
 * @param L   number of inter-arrival intervals (lag parameter)
 * @param alA stationary vector at arrivals of the first D-MAP
 * @param alB stationary vector at arrivals of the second D-MAP
 * @return the inner product value (scalar)
 */
fun dmap_geo_mul_sum(D0A: Matrix, D1A: Matrix, D0B: Matrix, D1B: Matrix, L: Int, alA: Matrix, alB: Matrix): Double {
    var Z = Matrix.dlyap(D0B.transpose(), D0A, alB.transpose().mult(alA))
    for (i in 1 until L) {
        Z = Matrix.dlyap(D0B.transpose(), D0A, D1B.transpose().mult(Z).mult(D1A))
    }
    val NA = D0A.numRows; val NB = D0B.numRows
    val dA = Matrix.eye(NA).add(-1.0, D0A).sumRows()
    val dB = Matrix.eye(NB).add(-1.0, D0B).sumRows()
    return dB.transpose().mult(Z).mult(dA).toDouble()
}

/**
 * Computes the squared L2 distance between lag-L joint PMFs of two D-MAPs.
 *
 * The stationary vectors at arrivals are computed internally.
 *
 * @param D0A sub-stochastic matrix for non-arrival transitions of the first D-MAP
 * @param D1A arrival transition matrix of the first D-MAP
 * @param D0B sub-stochastic matrix for non-arrival transitions of the second D-MAP
 * @param D1B arrival transition matrix of the second D-MAP
 * @param L   lag parameter
 * @return the squared L2 distance
 */
fun dmap_dist(D0A: Matrix, D1A: Matrix, D0B: Matrix, D1B: Matrix, L: Int): Double {
    val alA = dmap_pie(D0A, D1A); val alB = dmap_pie(D0B, D1B)
    return dmap_dist(D0A, D1A, D0B, D1B, L, alA, alB)
}

/**
 * Computes the squared L2 distance between lag-L joint PMFs of two D-MAPs.
 *
 * @param D0A sub-stochastic matrix for non-arrival transitions of the first D-MAP
 * @param D1A arrival transition matrix of the first D-MAP
 * @param D0B sub-stochastic matrix for non-arrival transitions of the second D-MAP
 * @param D1B arrival transition matrix of the second D-MAP
 * @param L   lag parameter
 * @param alA stationary vector at arrivals of the first D-MAP
 * @param alB stationary vector at arrivals of the second D-MAP
 * @return the squared L2 distance
 */
fun dmap_dist(D0A: Matrix, D1A: Matrix, D0B: Matrix, D1B: Matrix, L: Int, alA: Matrix, alB: Matrix): Double {
    return dmap_geo_mul_sum(D0A, D1A, D0A, D1A, L + 1, alA, alA) -
           2 * dmap_geo_mul_sum(D0A, D1A, D0B, D1B, L + 1, alA, alB) +
           dmap_geo_mul_sum(D0B, D1B, D0B, D1B, L + 1, alB, alB)
}

/**
 * Computes the geometric sum for the discrete autocorrelation distance of two D-MAPs.
 *
 * @param D0A sub-stochastic matrix for non-arrival transitions of the first D-MAP
 * @param D1A arrival transition matrix of the first D-MAP
 * @param D0B sub-stochastic matrix for non-arrival transitions of the second D-MAP
 * @param D1B arrival transition matrix of the second D-MAP
 * @param alA stationary vector at arrivals of the first D-MAP
 * @param alB stationary vector at arrivals of the second D-MAP
 * @return the geometric sum value (scalar)
 */
fun dmap_geo_mul_sum_acf(D0A: Matrix, D1A: Matrix, D0B: Matrix, D1B: Matrix, alA: Matrix, alB: Matrix): Double {
    val NA = D0A.numRows; val NB = D0B.numRows
    val D0Ai = Matrix.eye(NA).add(-1.0, D0A).inv()
    val D0Bi = Matrix.eye(NB).add(-1.0, D0B).inv()
    val PAh = D0Ai.mult(D1A).add(-1.0, Matrix.ones(NA, 1).mult(alA))
    val PBh = D0Bi.mult(D1B).add(-1.0, Matrix.ones(NB, 1).mult(alB))
    val M = Matrix.eye(NA * NB).add(-1.0, PBh.transpose().kron(PAh))
    return if (Math.abs(M.det()) < 1e-10) {
        Double.MAX_VALUE
    } else {
        val X = Matrix.dlyap(PAh, PBh, D0Ai.sumRows().mult(alB.mult(D0Bi)))
        alA.mult(D0Ai).mult(X).mult(D0Bi).sumCols().toDouble()
    }
}

/**
 * Computes the squared L2 distance between autocorrelation functions of two D-MAPs.
 *
 * The stationary vectors at arrivals are computed internally.
 *
 * @param D0A sub-stochastic matrix for non-arrival transitions of the first D-MAP
 * @param D1A arrival transition matrix of the first D-MAP
 * @param D0B sub-stochastic matrix for non-arrival transitions of the second D-MAP
 * @param D1B arrival transition matrix of the second D-MAP
 * @return the squared L2 distance between ACFs
 */
fun dmap_dist_acf(D0A: Matrix, D1A: Matrix, D0B: Matrix, D1B: Matrix): Double {
    val alA = dmap_pie(D0A, D1A); val alB = dmap_pie(D0B, D1B)
    return dmap_dist_acf(D0A, D1A, D0B, D1B, alA, alB)
}

/**
 * Computes the squared L2 distance between autocorrelation functions of two D-MAPs.
 *
 * Uses geometric sums and discrete marginal moments, normalized by variances.
 *
 * @param D0A sub-stochastic matrix for non-arrival transitions of the first D-MAP
 * @param D1A arrival transition matrix of the first D-MAP
 * @param D0B sub-stochastic matrix for non-arrival transitions of the second D-MAP
 * @param D1B arrival transition matrix of the second D-MAP
 * @param alA stationary vector at arrivals of the first D-MAP
 * @param alB stationary vector at arrivals of the second D-MAP
 * @return the squared L2 distance between ACFs
 */
fun dmap_dist_acf(D0A: Matrix, D1A: Matrix, D0B: Matrix, D1B: Matrix, alA: Matrix, alB: Matrix): Double {
    // Discrete marginal moments: mean = alpha * (I-D0)^{-1} * e
    val D0Ai_A = Matrix.eye(D0A.numRows).add(-1.0, D0A).inv()
    val D0Ai_B = Matrix.eye(D0B.numRows).add(-1.0, D0B).inv()
    val eA = Matrix.ones(D0A.numRows, 1)
    val eB = Matrix.ones(D0B.numRows, 1)
    val muA = alA.mult(D0Ai_A).mult(eA).toDouble()
    val muB = alB.mult(D0Ai_B).mult(eB).toDouble()
    // Second moment: alpha * (I+D0) * (I-D0)^{-2} * e
    val m2A = alA.mult(Matrix.eye(D0A.numRows).add(1.0, D0A)).mult(D0Ai_A).mult(D0Ai_A).mult(eA).toDouble()
    val m2B = alB.mult(Matrix.eye(D0B.numRows).add(1.0, D0B)).mult(D0Ai_B).mult(D0Ai_B).mult(eB).toDouble()
    val varA = m2A - muA * muA
    val varB = m2B - muB * muB
    // k=0 correction: alpha * (I-D0)^{-2} * e = (m2 + mu) / 2
    val cA = (m2A + muA) / 2
    val cB = (m2B + muB) / 2
    return (dmap_geo_mul_sum_acf(D0A, D1A, D0A, D1A, alA, alA) - cA * cA) / (varA * varA) -
           2 * (dmap_geo_mul_sum_acf(D0A, D1A, D0B, D1B, alA, alB) - cA * cB) / (varA * varB) +
           (dmap_geo_mul_sum_acf(D0B, D1B, D0B, D1B, alB, alB) - cB * cB) / (varB * varB)
}

/**
 * D-MAP distance algorithms
 */
@Suppress("unused")
class DmapDistAlgo {
    companion object {
        // Class documentation marker for Dokka
    }
}
