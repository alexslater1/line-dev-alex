/**
 * @file MAP construction via Bernstein polynomial approximation
 *
 * Converts a continuous distribution (specified by its PDF) to a Markovian
 * Arrival Process (MAP) representation using Bernstein polynomial approximation.
 *
 * @since LINE 3.0
 */
package jline.api.mam

import jline.util.matrix.Matrix
import jline.util.matrix.MatrixCell
import kotlin.math.ln

/**
 * Converts a distribution to a MAP via Bernstein polynomial approximation.
 *
 * Uses the Bernstein approximation of order n to construct a MAP representation
 * of a continuous distribution specified by its probability density function.
 *
 * The caller should rescale the resulting MAP to the desired mean using [map_scale].
 *
 * @param f the probability density function as a lambda (Double) -> Double
 * @param n the number of phases for the approximation (default: 20)
 * @return a MatrixCell containing the MAP representation {D0, D1}
 */
fun map_bernstein(f: (Double) -> Double, n: Int = 20): MatrixCell {
    // Bernstein approximation normalizing constant
    var c = 0.0
    for (i in 1..n) {
        val xi = -ln(i.toDouble() / n)
        val fi = f(xi)
        if (fi.isFinite() && fi > 0) {
            c += fi / i
        }
    }

    // Handle invalid normalizing constant - fallback to Erlang-n
    if (c <= 0 || !c.isFinite()) {
        return map_erlang(1.0, n)
    }

    // Build subgenerator T: diagonal = -[1, 2, ..., n], superdiagonal = [1, 2, ..., n-1]
    val T = Matrix(n, n)
    for (i in 0 until n) {
        T[i, i] = -(i + 1).toDouble()
        if (i < n - 1) {
            T[i, i + 1] = (i + 1).toDouble()
        }
    }

    // Build initial probability vector alpha
    val alpha = Matrix(1, n)
    var alphaSum = 0.0
    for (i in 1..n) {
        val xi = -ln(i.toDouble() / n)
        val fi = f(xi)
        if (fi.isFinite() && fi > 0) {
            val v = fi / (i * c)
            alpha[0, i - 1] = v
            alphaSum += v
        }
    }

    // Normalize alpha
    if (alphaSum > 0) {
        for (j in 0 until n) {
            alpha[0, j] = alpha[0, j] / alphaSum
        }
    } else {
        alpha[0, 0] = 1.0
    }

    // Build D1 = -T * (ones * alpha) = -T * P
    val P = Matrix.ones(n, 1).mult(alpha)
    val negT = T.copy()
    negT.scaleEq(-1.0)
    val D1 = negT.mult(P)

    val MAP = MatrixCell()
    MAP[0] = T
    MAP[1] = D1
    return MAP
}
