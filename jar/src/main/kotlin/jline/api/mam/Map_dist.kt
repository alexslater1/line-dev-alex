/**
 * @file Squared L2 distance between lag-L joint densities of two MAPs
 *
 * Computes the squared L2 distance between the lag-L joint densities of
 * two Markovian Arrival Processes using inner products of joint densities.
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
 * Computes the squared L2 distance between lag-L joint densities of two MAPs.
 *
 * The stationary vectors at arrivals are computed internally.
 *
 * @param A0 hidden transition matrix of the first MAP
 * @param A1 visible transition matrix of the first MAP
 * @param B0 hidden transition matrix of the second MAP
 * @param B1 visible transition matrix of the second MAP
 * @param L  lag parameter
 * @return the squared L2 distance
 */
fun map_dist(A0: Matrix, A1: Matrix, B0: Matrix, B1: Matrix, L: Int): Double {
    val alA = map_pie(A0, A1)
    val alB = map_pie(B0, B1)
    return map_dist(A0, A1, B0, B1, L, alA, alB)
}

/**
 * Computes the squared L2 distance between lag-L joint densities of two MAPs.
 *
 * Uses the identity ||f - g||^2 = <f,f> - 2<f,g> + <g,g> where the inner
 * products are computed via [map_exp_mul_int].
 *
 * @param A0  hidden transition matrix of the first MAP
 * @param A1  visible transition matrix of the first MAP
 * @param B0  hidden transition matrix of the second MAP
 * @param B1  visible transition matrix of the second MAP
 * @param L   lag parameter
 * @param alA stationary vector at arrivals of the first MAP
 * @param alB stationary vector at arrivals of the second MAP
 * @return the squared L2 distance
 */
fun map_dist(A0: Matrix, A1: Matrix, B0: Matrix, B1: Matrix, L: Int, alA: Matrix, alB: Matrix): Double {
    return map_exp_mul_int(A0, A1, A0, A1, L + 1, alA, alA) -
           2 * map_exp_mul_int(A0, A1, B0, B1, L + 1, alA, alB) +
           map_exp_mul_int(B0, B1, B0, B1, L + 1, alB, alB)
}

/**
 * MAP distance algorithms
 */
@Suppress("unused")
class MapDistAlgo {
    companion object {
        // Class documentation marker for Dokka
    }
}
