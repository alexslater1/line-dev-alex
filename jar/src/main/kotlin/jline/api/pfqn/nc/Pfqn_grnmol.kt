/**
 * @file Normalizing constant using Grundmann-Moeller quadrature
 *
 * Implements the Grundmann-Moeller simplex quadrature rule for computing normalizing
 * constants in product-form queueing networks. This method uses a sequence of
 * quadrature formulae over the unit simplex to approximate the integral representation
 * of the normalizing constant.
 *
 * @since LINE 3.0
 */
package jline.api.pfqn.nc

import jline.util.Maths
import jline.util.matrix.Matrix
import org.apache.commons.math3.util.FastMath

/**
 * Compute the normalizing constant using Grundmann-Moeller quadrature
 *
 * @param L - service demand matrix (M x R)
 * @param N - population vector (1 x R)
 * @return G - normalizing constant
 */

fun pfqn_grnmol(L: Matrix, N: Matrix): Double {
    val M = L.numRows
    val R = L.numCols
    var G = 0.0
    val S = FastMath.ceil((N.elementSum() - 1) / 2).toInt()

    for (i in 0..S) {
        val cVal = (2 * (S - i) + M).toDouble()
        // w = 2^-(2*S) * (-1)^i * c^(2*S+1) / i! / (i+c)!
        val w = FastMath.pow(2.0, (-2 * S).toDouble()) *
            FastMath.pow(-1.0, i.toDouble()) *
            FastMath.pow(cVal, (2 * S + 1).toDouble()) /
            FastMath.exp(Maths.factln(i)) /
            FastMath.exp(Maths.factln(i + cVal.toInt()))

        // Generate all M-element compositions summing to S-i
        val compositions = generateCompositions(M, S - i)
        var Hi = 0.0

        for (comp in compositions) {
            // prod(((2*bvec+1)/c * L).^N)
            // bvec is 1xM, (2*bvec+1)/c is 1xM, matrix-multiply with L (MxR) gives 1xR
            // .^N element-wise, then prod over R gives scalar
            var prodVal = 1.0
            for (r in 0..<R) {
                var sumLr = 0.0
                for (m in 0..<M) {
                    sumLr += (2.0 * comp[m] + 1.0) / cVal * L[m, r]
                }
                prodVal *= FastMath.pow(sumLr, N[r])
            }
            Hi += prodVal
        }
        G += w * Hi
    }

    // Multiply by multinomial coefficient: factorial(sum(N)+M-1) / prod(factorial(N))
    // Use log-space for large factorials
    val logCoeff = Maths.factln(N.elementSum() + M - 1) - Matrix.factln(N).elementSum()
    G = G * FastMath.exp(logCoeff)

    return G
}

/**
 * Generate all compositions of integer k into m non-negative parts.
 * These are all vectors of m non-negative integers summing to k.
 * Equivalent to multichoose(m, k) in MATLAB.
 */
private fun generateCompositions(m: Int, k: Int): List<IntArray> {
    val result = ArrayList<IntArray>()
    if (m == 1) {
        result.add(intArrayOf(k))
        return result
    }
    if (k == 0) {
        result.add(IntArray(m))
        return result
    }
    for (i in 0..k) {
        val subComps = generateCompositions(m - 1, k - i)
        for (sub in subComps) {
            val comp = IntArray(m)
            comp[0] = i
            System.arraycopy(sub, 0, comp, 1, m - 1)
            result.add(comp)
        }
    }
    return result
}

/**
 * PFQN grnmol algorithms
 */
@Suppress("unused")
class PfqnGrnmolAlgo {
    companion object {
        // Class documentation marker for Dokka
    }
}
