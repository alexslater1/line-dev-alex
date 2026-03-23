/**
 * Convolution Algorithm for LCFS Queueing Networks
 *
 * Implements the convolution algorithm for computing normalizing constants
 * in 2-station closed queueing networks with LCFS and LCFS-PR scheduling.
 *
 * Reference:
 *   G. Casale, "A family of multiclass LCFS queueing networks with
 *   order-dependent product-form solutions", QUESTA 2026.
 *
 * @since LINE 3.0
 */
package jline.api.pfqn.lcfs

import jline.io.Ret
import jline.util.PopulationLattice
import jline.util.matrix.Matrix

/**
 * Result class for LCFS convolution algorithm
 *
 * @property G The normalizing constant
 * @property V Auxiliary normalization term
 */
class LcfsqnCaResult(
    val G: Double,
    val V: Double
)

/**
 * Convolution algorithm for multiclass LCFS queueing networks.
 *
 * This function computes the normalizing constant for a 2-station closed
 * queueing network with:
 *   - Station 1: LCFS (Last-Come-First-Served, non-preemptive)
 *   - Station 2: LCFS-PR (LCFS with Preemption-Resume)
 *
 * @param alpha Vector of inverse service rates at station 1 (LCFS).
 *              alpha(r) = 1/mu(1,r) for class r
 * @param beta Vector of inverse service rates at station 2 (LCFS-PR).
 *             beta(r) = 1/mu(2,r) for class r
 * @param N Population vector, N(r) = number of jobs of class r.
 *          Default: ones(1,R) - one job per class
 * @return LcfsqnCaResult containing normalizing constant G and auxiliary term V
 */
fun pfqn_lcfsqn_ca(alpha: Matrix, beta: Matrix, N: Matrix? = null): LcfsqnCaResult {
    val R = alpha.length()
    val populationVector = N ?: Matrix.ones(1, R)

    val K = populationVector.elementSum().toInt()

    if (K == 0) {
        return LcfsqnCaResult(1.0, 1.0)
    }

    // Precompute products for hash computation
    val prods = Matrix(1, R)
    for (r in 0 until R) {
        var prod = 1.0
        for (j in 0 until r) {
            prod *= (populationVector.get(j) + 1)
        }
        prods.set(r, prod)
    }

    // Calculate total size of G and V arrays
    var totalSize = 1
    for (r in 0 until R) {
        totalSize *= (populationVector.get(r).toInt() + 1)
    }

    val G = DoubleArray(totalSize)
    val V = DoubleArray(totalSize)

    // Initialize population iterator
    var n = PopulationLattice.pprod(populationVector)

    while (n.get(0) >= 0) {
        val idx = hashpopLcfs(n, populationVector, R, prods)
        val sumN = n.elementSum().toInt()

        if (sumN == 0) {
            G[idx] = 1.0
            V[idx] = 1.0
        } else {
            V[idx] = 0.0
            G[idx] = 0.0

            for (r in 0 until R) {
                if (n.get(r) > 0) {
                    val n_minus_r = oner(n, r)
                    val idx_r = hashpopLcfs(n_minus_r, populationVector, R, prods)

                    // Recursive definition of V
                    V[idx] = V[idx] + V[idx_r]

                    // Recursive definition of G uses V and beta
                    G[idx] = G[idx] + Math.pow(alpha.get(r), (sumN - 1).toDouble()) * beta.get(r) * G[idx_r]
                }
            }

            // V(idx) = prod(alpha.^n) * V(idx)
            var prodAlphaN = 1.0
            for (r in 0 until R) {
                prodAlphaN *= Math.pow(alpha.get(r), n.get(r))
            }
            V[idx] = prodAlphaN * V[idx]
            G[idx] = G[idx] + V[idx]
        }

        n = PopulationLattice.pprod(n, populationVector)
    }

    return LcfsqnCaResult(G[totalSize - 1], V[totalSize - 1])
}

/**
 * Computes hash index for LCFS population lattice.
 *
 * @param n Current population vector
 * @param N Maximum population vector
 * @param R Number of classes
 * @param prods Precomputed products for hash computation
 * @return Hash index
 */
private fun hashpopLcfs(n: Matrix, N: Matrix, R: Int, prods: Matrix): Int {
    var idx = 0
    for (r in 0 until R) {
        idx += (prods.get(r) * n.get(r)).toInt()
    }
    return idx
}

/**
 * Decrements the r-th component of vector n by 1.
 * Returns a new vector with n(r) - 1.
 *
 * @param n The population vector
 * @param r The index to decrement
 * @return New vector with n(r) decremented
 */
private fun oner(n: Matrix, r: Int): Matrix {
    val result = n.copy()
    result.set(r, result.get(r) - 1)
    return result
}

/**
 * PFQN LCFS convolution algorithm class marker for Dokka documentation.
 */
@Suppress("unused")
class PfqnLcfsqnCaAlgo {
    companion object {
        // Class documentation marker for Dokka
    }
}
