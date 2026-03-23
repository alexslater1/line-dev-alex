/**
 * @file McKenna-Mitra integral with Gauss-Laguerre quadrature
 *
 * Implements Gauss-Laguerre quadrature integration for computing normalizing constants
 * in multi-class repairman models. Uses precomputed Gauss-Laguerre nodes and weights
 * for high-precision numerical integration of the McKenna-Mitra integral representation.
 *
 * The Gauss-Laguerre quadrature approximates integrals of the form:
 *   int_0^inf f(x) exp(-x) dx ~ sum_i w_i * f(x_i)
 *
 * @since LINE 3.0
 */
package jline.api.pfqn.nc

import jline.io.Ret
import jline.util.Maths
import jline.util.matrix.Matrix
import org.apache.commons.math3.util.FastMath

/**
 * Compute the normalizing constant of a repairmen model using Gauss-Laguerre integration
 *
 * @param L - demands at all stations (1 x R)
 * @param N - number of jobs for each class (1 x R)
 * @param Z - think times (1 x R)
 * @param m - replication factor (default: 1)
 * @return normalizing constant and its logarithm
 */

@JvmOverloads
fun pfqn_mmint2_gausslaguerre(L: Matrix, N: Matrix, Z: Matrix, m: Int = 1): Ret.pfqnNc {
    val nMax = 300
    val nodes = getGaussLaguerreNodes(nMax)
    val weights = getGaussLaguerreWeights(nMax)

    val nonzeroClasses = ArrayList<Int>()
    for (i in 0..<N.length()) {
        if (N[i] > 0) {
            nonzeroClasses.add(i)
        }
    }

    val n = FastMath.min(nMax, 2 * N.elementSum().toInt() + 1)

    // f(u) = N(nonzeroClasses) * log(Z(nonzeroClasses) + L(nonzeroClasses) * u)'
    val F = DoubleArray(n)
    for (i in 0..<n) {
        var sum = 0.0
        for (j in nonzeroClasses) {
            sum += N[j] * FastMath.log(Z[j] + L[j] * nodes[i])
        }
        F[i] = (m - 1).toDouble() * FastMath.log(nodes[i]) + sum
    }

    // g = log(w) + F - sum(factln(N)) - factln(m-1)
    val g = Matrix(1, n)
    var factlnSum = 0.0
    for (i in 0..<N.length()) {
        factlnSum += Maths.factln(N[i])
    }
    val factlnM1 = Maths.factln(m - 1)

    for (i in 0..<n) {
        g[i] = FastMath.log(weights[i]) + F[i] - factlnSum - factlnM1
    }

    // lG = log(sum(exp(g)))
    var lG = 0.0
    var sumExpG = 0.0
    for (i in 0..<n) {
        sumExpG += FastMath.exp(g[i])
    }
    lG = FastMath.log(sumExpG)

    if (!java.lang.Double.isFinite(lG)) {
        // Use logsumexp trick for numerical stability
        lG = Matrix.logsumexp(g)
    }

    val G = FastMath.exp(lG)
    return Ret.pfqnNc(G, lG)
}

/**
 * Compute Gauss-Laguerre quadrature nodes using the Golub-Welsch algorithm.
 * The nodes are the eigenvalues of the symmetric tridiagonal matrix associated
 * with the Laguerre polynomials.
 *
 * @param n number of quadrature points
 * @return array of n nodes
 */
private fun getGaussLaguerreNodes(n: Int): DoubleArray {
    return computeGaussLaguerre(n).first
}

/**
 * Compute Gauss-Laguerre quadrature weights.
 *
 * @param n number of quadrature points
 * @return array of n weights
 */
private fun getGaussLaguerreWeights(n: Int): DoubleArray {
    return computeGaussLaguerre(n).second
}

/**
 * Cache for Gauss-Laguerre nodes and weights
 */
private var cachedN = -1
private var cachedNodes: DoubleArray? = null
private var cachedWeights: DoubleArray? = null

/**
 * Compute Gauss-Laguerre quadrature nodes and weights using the Golub-Welsch algorithm.
 *
 * For the generalized Laguerre weight function w(x) = x^alpha * exp(-x) with alpha=0,
 * the three-term recurrence for Laguerre polynomials gives:
 *   a_k = 2k + 1  (diagonal)
 *   b_k = k       (off-diagonal, after square root)
 *
 * The nodes are eigenvalues and weights are computed from the first components
 * of the eigenvectors.
 *
 * @param n number of quadrature points
 * @return pair of (nodes, weights)
 */
@Synchronized
private fun computeGaussLaguerre(n: Int): Pair<DoubleArray, DoubleArray> {
    if (n == cachedN && cachedNodes != null && cachedWeights != null) {
        return Pair(cachedNodes!!, cachedWeights!!)
    }

    // Golub-Welsch: build symmetric tridiagonal matrix
    // For Laguerre: diagonal a_k = 2k+1, off-diagonal sqrt(b_k) = k
    val diag = DoubleArray(n)
    val offdiag = DoubleArray(n - 1)

    for (k in 0..<n) {
        diag[k] = (2.0 * k + 1.0)
    }
    for (k in 0..<n - 1) {
        offdiag[k] = (k + 1).toDouble()
    }

    // Compute eigenvalues and eigenvectors using implicit QR (symmetric tridiagonal)
    val eigenResult = symmetricTridiagonalEigen(diag, offdiag)
    val nodes = eigenResult.first
    val eigenvectors = eigenResult.second

    val weights = DoubleArray(n)
    for (i in 0..<n) {
        weights[i] = eigenvectors[0][i] * eigenvectors[0][i] // w_i = v_{1,i}^2 * Gamma(1) = v_{1,i}^2
    }

    // Sort by nodes
    val indices = (0..<n).toMutableList()
    indices.sortBy { nodes[it] }
    val sortedNodes = DoubleArray(n)
    val sortedWeights = DoubleArray(n)
    for (i in 0..<n) {
        sortedNodes[i] = nodes[indices[i]]
        sortedWeights[i] = weights[indices[i]]
    }

    cachedN = n
    cachedNodes = sortedNodes
    cachedWeights = sortedWeights

    return Pair(sortedNodes, sortedWeights)
}

/**
 * Compute eigenvalues and eigenvectors of a symmetric tridiagonal matrix
 * using the implicit QR algorithm with Wilkinson shift.
 *
 * @param diag diagonal elements (will be modified)
 * @param offdiag off-diagonal elements (will be modified)
 * @return pair of (eigenvalues, eigenvectors[row][col])
 */
private fun symmetricTridiagonalEigen(diag: DoubleArray, offdiag: DoubleArray): Pair<DoubleArray, Array<DoubleArray>> {
    val n = diag.size
    val d = diag.clone()
    val e = DoubleArray(n)
    for (i in 0..<n - 1) {
        e[i] = offdiag[i]
    }
    e[n - 1] = 0.0

    // Initialize eigenvector matrix as identity
    val z = Array(n) { i -> DoubleArray(n) { j -> if (i == j) 1.0 else 0.0 } }

    // Implicit QL algorithm for symmetric tridiagonal matrices
    val maxIter = 300
    for (l in 0..<n) {
        var iter = 0
        while (true) {
            // Find small off-diagonal element
            var m = l
            while (m < n - 1) {
                val dd = FastMath.abs(d[m]) + FastMath.abs(d[m + 1])
                if (FastMath.abs(e[m]) + dd == dd) break
                m++
            }
            if (m == l) break

            if (iter++ >= maxIter) break

            // Wilkinson shift
            var g = (d[l + 1] - d[l]) / (2.0 * e[l])
            var r = FastMath.sqrt(g * g + 1.0)
            g = d[m] - d[l] + e[l] / (g + if (g >= 0) FastMath.abs(r) else -FastMath.abs(r))

            var s = 1.0
            var c = 1.0
            var p = 0.0

            for (i in m - 1 downTo l) {
                var f = s * e[i]
                val b = c * e[i]
                r = FastMath.sqrt(f * f + g * g)
                e[i + 1] = r
                if (r == 0.0) {
                    d[i + 1] -= p
                    e[m] = 0.0
                    break
                }
                s = f / r
                c = g / r
                g = d[i + 1] - p
                r = (d[i] - g) * s + 2.0 * c * b
                p = s * r
                d[i + 1] = g + p
                g = c * r - b

                // Accumulate eigenvector transformation
                for (k in 0..<n) {
                    f = z[k][i + 1]
                    z[k][i + 1] = s * z[k][i] + c * f
                    z[k][i] = c * z[k][i] - s * f
                }
            }

            if (r == 0.0 && m - 1 >= l) continue

            d[l] -= p
            e[l] = g
            e[m] = 0.0
        }
    }

    return Pair(d, z)
}

/**
 * PFQN mmint2 gausslaguerre algorithms
 */
@Suppress("unused")
class PfqnMmint2GausslaguerreAlgo {
    companion object {
        // Class documentation marker for Dokka
    }
}
