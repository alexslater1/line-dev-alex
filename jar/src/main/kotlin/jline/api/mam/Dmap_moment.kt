/**
 * Moment computation for discrete-time MAPs.
 *
 * @since LINE 3.0
 */
package jline.api.mam

import jline.util.matrix.Matrix
import jline.util.matrix.MatrixCell

/**
 * Computes the k-th raw moment of the inter-arrival time of a discrete MAP.
 *
 *   E[T]   = pi * (I-D0)^{-1} * e
 *   E[T^2] = 2*pi*(I-D0)^{-2}*e - E[T]
 *   E[T^3] = 6*pi*(I-D0)^{-3}*e - 6*pi*(I-D0)^{-2}*e + E[T]
 */
fun dmap_moment(D0: Matrix, D1: Matrix, order: Int): Double {
    val n = D0.numRows
    val I = Matrix.eye(n)
    val ImD0inv = I.add(-1.0, D0).inv()
    val e = Matrix.ones(n, 1)
    val pi = dmap_pie(D0, D1)

    return when (order) {
        1 -> pi.mult(ImD0inv).mult(e).toDouble()
        2 -> {
            val m1 = pi.mult(ImD0inv).mult(e).toDouble()
            2.0 * pi.mult(ImD0inv.mult(ImD0inv)).mult(e).toDouble() - m1
        }
        3 -> {
            val m1 = pi.mult(ImD0inv).mult(e).toDouble()
            val ImD0inv2 = ImD0inv.mult(ImD0inv)
            6.0 * pi.mult(ImD0inv2.mult(ImD0inv)).mult(e).toDouble() - 6.0 * pi.mult(ImD0inv2).mult(e).toDouble() + m1
        }
        else -> throw IllegalArgumentException("Moments of order > 3 not implemented for DMAP")
    }
}

fun dmap_moment(DMAP: MatrixCell, order: Int): Double = dmap_moment(DMAP[0], DMAP[1], order)
