/**
 * Normalizing Constant for LCFS Queueing Networks
 *
 * Computes the normalizing constant for LCFS queueing networks using
 * matrix permanent calculations.
 *
 * Reference:
 *   G. Casale, "A family of multiclass LCFS queueing networks with
 *   order-dependent product-form solutions", QUESTA 2026.
 *
 * @since LINE 3.0
 */
package jline.api.pfqn.lcfs

import jline.lib.perm.Permanent
import jline.util.matrix.Matrix

/**
 * Result class for LCFS NC algorithm
 *
 * @property G The normalizing constant
 * @property Ax Cell array of A matrices for each state
 */
class LcfsqnNcResult(
    val G: Double,
    val Ax: Array<Matrix>
)

/**
 * Normalizing constant for multiclass LCFS queueing networks.
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
 * @return LcfsqnNcResult containing normalizing constant G and A matrices
 */
fun pfqn_lcfsqn_nc(alpha: Matrix, beta: Matrix, N: Matrix): LcfsqnNcResult {
    val K = N.elementSum().toInt()
    val R = N.length()
    var G = 0.0

    // Create array of A matrices for each state x=0:K
    val Ax = Array(K + 1) { x -> makeA(alpha, beta, x, K, R) }

    // Sum over all states
    for (x in 0..K) {
        G += perm(Ax[x], N)
    }

    return LcfsqnNcResult(G, Ax)
}

/**
 * Constructs the A matrix for a given state x.
 *
 * @param alpha Vector of inverse service rates at LCFS station
 * @param beta Vector of inverse service rates at LCFS-PR station
 * @param x Current state (number of jobs at station 1)
 * @param K Total population
 * @param R Number of classes
 * @return The A matrix for state x
 */
private fun makeA(alpha: Matrix, beta: Matrix, x: Int, K: Int, R: Int): Matrix {
    val A = Matrix(R, K)

    for (i in 0 until R) {
        // Columns 0 to x-1: alpha(i)^(j+1) for j=0..x-1
        for (j in 0 until x) {
            A.set(i, j, Math.pow(alpha.get(i), (j + 1).toDouble()))
        }
        // Columns x to K-1: alpha(i)^(x+j) * beta(i) for j=0..K-x-1
        for (j in 0 until (K - x)) {
            A.set(i, x + j, Math.pow(alpha.get(i), (x + j).toDouble()) * beta.get(i))
        }
    }

    return A
}

/**
 * Computes the permanent of matrix A with multiplicities from population vector N.
 *
 * Uses the permanent with repeated rows based on the population vector.
 *
 * @param A The matrix
 * @param N Population vector (determines row repetitions)
 * @return The permanent value
 */
private fun perm(A: Matrix, N: Matrix): Double {
    val K = N.elementSum().toInt()
    val R = N.length()

    // Build expanded matrix with rows repeated according to N
    val expandedMatrix = Matrix(K, K)
    var rowIdx = 0
    for (r in 0 until R) {
        val count = N.get(r).toInt()
        for (rep in 0 until count) {
            for (col in 0 until K) {
                expandedMatrix.set(rowIdx, col, A.get(r, col))
            }
            rowIdx++
        }
    }

    // Use the Permanent class to compute the permanent
    val permSolver = Permanent(expandedMatrix, true)
    return permSolver.value
}

/**
 * PFQN LCFS NC algorithm class marker for Dokka documentation.
 */
@Suppress("unused")
class PfqnLcfsqnNcAlgo {
    companion object {
        // Class documentation marker for Dokka
    }
}
