/**
 * @file Squared L2 distance between autocorrelation functions of two MAPs
 *
 * Computes the squared L2 distance between the autocorrelation functions of
 * two Markovian Arrival Processes using geometric sums and discrete Lyapunov equations.
 *
 * Reference:
 *   G. Horvath, "Measuring the distance between MAPs and some
 *   applications," in Proc. ASMTA 2015, LNCS 9081, pp. 95-109.
 *   https://link.springer.com/chapter/10.1007/978-3-319-18579-8_8
 *
 * @since LINE 3.0
 */
package jline.api.mam

import jline.util.matrix.Matrix

/**
 * Computes the geometric sum needed for the autocorrelation distance.
 *
 * Solves a discrete Lyapunov equation involving the centered embedded chains
 * of the two MAPs.
 *
 * @param A0  hidden transition matrix of the first MAP
 * @param A1  visible transition matrix of the first MAP
 * @param B0  hidden transition matrix of the second MAP
 * @param B1  visible transition matrix of the second MAP
 * @param alA stationary vector at arrivals of the first MAP
 * @param alB stationary vector at arrivals of the second MAP
 * @return the geometric sum value (scalar)
 */
fun map_geo_mul_sum(A0: Matrix, A1: Matrix, B0: Matrix, B1: Matrix, alA: Matrix, alB: Matrix): Double {
    val negA0 = A0.copy(); negA0.scaleEq(-1.0)
    val negB0 = B0.copy(); negB0.scaleEq(-1.0)
    val A0i = negA0.inv()
    val B0i = negB0.inv()
    val NA = A0.numRows
    val NB = B0.numRows

    val PAh = A0i.mult(A1).add(-1.0, Matrix.ones(NA, 1).mult(alA))
    val PBh = B0i.mult(B1).add(-1.0, Matrix.ones(NB, 1).mult(alB))

    val M = Matrix.eye(NA * NB).add(-1.0, PBh.transpose().kron(PAh))
    return if (Math.abs(M.det()) < 1e-10) {
        Double.MAX_VALUE
    } else {
        val X = Matrix.dlyap(PAh, PBh, A0i.sumRows().mult(alB.mult(B0i)))
        alA.mult(A0i).mult(X).mult(B0i).sumCols().toDouble()
    }
}

/**
 * Computes the squared L2 distance between autocorrelation functions of two MAPs.
 *
 * The stationary vectors at arrivals are computed internally.
 *
 * @param A0 hidden transition matrix of the first MAP
 * @param A1 visible transition matrix of the first MAP
 * @param B0 hidden transition matrix of the second MAP
 * @param B1 visible transition matrix of the second MAP
 * @return the squared L2 distance between ACFs
 */
fun map_dist_acf(A0: Matrix, A1: Matrix, B0: Matrix, B1: Matrix): Double {
    val alA = map_pie(A0, A1)
    val alB = map_pie(B0, B1)
    return map_dist_acf(A0, A1, B0, B1, alA, alB)
}

/**
 * Computes the squared L2 distance between autocorrelation functions of two MAPs.
 *
 * Uses the formula based on geometric sums and second-order moments, normalized
 * by the variances of the inter-arrival time distributions.
 *
 * @param A0  hidden transition matrix of the first MAP
 * @param A1  visible transition matrix of the first MAP
 * @param B0  hidden transition matrix of the second MAP
 * @param B1  visible transition matrix of the second MAP
 * @param alA stationary vector at arrivals of the first MAP
 * @param alB stationary vector at arrivals of the second MAP
 * @return the squared L2 distance between ACFs
 */
fun map_dist_acf(A0: Matrix, A1: Matrix, B0: Matrix, B1: Matrix, alA: Matrix, alB: Matrix): Double {
    val momA1 = map_moment(A0, A1, 1)
    val momA2 = map_moment(A0, A1, 2)
    val momB1 = map_moment(B0, B1, 1)
    val momB2 = map_moment(B0, B1, 2)
    val varA = momA2 - momA1 * momA1
    val varB = momB2 - momB1 * momB1

    return (map_geo_mul_sum(A0, A1, A0, A1, alA, alA) - momA2 * momA2 / 4) / (varA * varA) -
           2 * (map_geo_mul_sum(A0, A1, B0, B1, alA, alB) - momA2 * momB2 / 4) / (varA * varB) +
           (map_geo_mul_sum(B0, B1, B0, B1, alB, alB) - momB2 * momB2 / 4) / (varB * varB)
}

/**
 * MAP ACF distance algorithms
 */
@Suppress("unused")
class MapDistAcfAlgo {
    companion object {
        // Class documentation marker for Dokka
    }
}
