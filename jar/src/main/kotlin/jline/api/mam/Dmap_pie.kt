/**
 * Stationary vector at arrival epochs for discrete-time MAPs.
 *
 * @since LINE 3.0
 */
package jline.api.mam

import jline.util.matrix.Matrix
import jline.util.matrix.MatrixCell

/**
 * Computes the stationary vector at arrival epochs of a discrete-time MAP.
 *
 * The embedded chain is P = (I - D0)^{-1} * D1, and the stationary vector
 * pi satisfies pi * P = pi, pi * e = 1.
 */
fun dmap_pie(D0: Matrix, D1: Matrix): Matrix {
    val N = D0.numRows
    val I = Matrix.eye(N)
    val P = I.add(-1.0, D0).inv().mult(D1)
    val M = P.transpose().add(-1.0, I)
    for (i in 0 until N) { M.set(i, 0, 1.0) }
    val rhs = Matrix(N, 1, N)
    rhs.set(0, 0, 1.0)
    return Matrix.robustLeftDivide(M, rhs).transpose()
}

fun dmap_pie(DMAP: MatrixCell): Matrix = dmap_pie(DMAP[0], DMAP[1])
