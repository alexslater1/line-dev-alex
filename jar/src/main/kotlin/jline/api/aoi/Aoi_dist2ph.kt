/**
 * @file Distribution conversion for AoI analysis
 *
 * Converts LINE's {D0, D1} MAP process representation to phase-type (alpha, T)
 * format suitable for AoI analysis. The initial probability vector alpha is
 * computed from the stationary distribution of the MAP at completion epochs.
 *
 * @since LINE 3.0
 */
package jline.api.aoi

import jline.util.matrix.Matrix
import jline.util.matrix.MatrixCell
import kotlin.math.abs
import kotlin.math.max

/**
 * Result of aoi_dist2ph conversion.
 *
 * @property alpha Initial probability row vector (1 x n)
 * @property T Sub-generator matrix (n x n)
 */
data class Aoi_dist2phResult(
    val alpha: Matrix,
    val T: Matrix
)

/**
 * Convert LINE process representation to PH format for AoI analysis.
 *
 * Converts LINE's {D0, D1} MAP representation to (alpha, T) PH format
 * where alpha is the initial probability vector and T is the sub-generator matrix.
 *
 * In PH representation:
 *   - alpha: Row vector of initial probabilities (sums to 1)
 *   - T: Sub-generator matrix (negative diagonal, non-negative off-diagonal)
 *   - Absorption rates: -T * ones(n,1)
 *
 * @param proc LINE process representation as MatrixCell {D0, D1} where:
 *             D0: Sub-generator matrix (like T in PH)
 *             D1: Completion/transition matrix
 * @return Aoi_dist2phResult containing (alpha, T)
 * @throws IllegalArgumentException if proc is invalid
 */
fun aoi_dist2ph(proc: MatrixCell): Aoi_dist2phResult {
    require(proc.size() >= 2) { "proc must be a cell array {D0, D1}" }

    val D0 = proc[0]
    val D1 = proc[1]

    // Check for NaN (disabled class)
    require(!D0.hasNaN() && !D1.hasNaN()) {
        "Process contains NaN - class may be disabled"
    }

    // Get dimensions
    val n = D0.numRows
    require(n == D0.numCols && n == D1.numRows && n == D1.numCols) {
        "D0 and D1 must be square matrices of the same size"
    }

    // T is the sub-generator (D0 in MAP notation)
    val T = D0.copy()

    // Compute stationary distribution of the underlying CTMC
    // The full generator is Q = D0 + D1
    val Q = D0.add(D1)

    // Check if Q is a proper generator (rows should sum to ~0)
    for (i in 0 until n) {
        var rowSum = 0.0
        for (j in 0 until n) {
            rowSum += Q.get(i, j)
        }
        if (abs(rowSum) > 1e-10) {
            Q.set(i, i, Q.get(i, i) - rowSum)
        }
    }

    // Compute stationary distribution pi of Q
    // pi * Q = 0 and sum(pi) = 1
    // Solve [Q'; ones(1,n)] * [pi'; 1] = [zeros(n,1); 1]
    val A = Matrix(n + 1, n)
    for (i in 0 until n) {
        for (j in 0 until n) {
            A.set(j, i, Q.get(i, j)) // Transpose Q
        }
    }
    for (j in 0 until n) {
        A.set(n, j, 1.0) // ones row
    }

    val b = Matrix(n + 1, 1)
    b.set(n, 0, 1.0) // last element is 1

    // Solve overdetermined system using normal equations: A'*A * x = A'*b
    val At = A.transpose()
    val AtA = At.mult(A)
    val Atb = At.mult(b)
    val piCol = Matrix(n, 1)
    Matrix.solveSafe(AtA, Atb, piCol)

    // Extract pi as row vector, ensure non-negative and normalized
    val pi = Matrix(1, n)
    var piSum = 0.0
    for (j in 0 until n) {
        val v = max(piCol.get(j, 0), 0.0)
        pi.set(0, j, v)
        piSum += v
    }
    if (piSum > 0) {
        for (j in 0 until n) {
            pi.set(0, j, pi.get(0, j) / piSum)
        }
    }

    // Initial probability alpha is the stationary distribution weighted by
    // the probability of starting in each phase after a completion
    // For a renewal process: alpha = pi .* completionRates' / sum(pi .* completionRates')
    val ones = Matrix.ones(n, 1)
    val completionRates = D1.mult(ones) // n x 1 column vector

    val alpha = Matrix(1, n)
    var alphaSum = 0.0
    for (j in 0 until n) {
        val v = pi.get(0, j) * completionRates.get(j, 0)
        alpha.set(0, j, v)
        alphaSum += v
    }

    if (alphaSum > 0) {
        for (j in 0 until n) {
            alpha.set(0, j, alpha.get(0, j) / alphaSum)
        }
    } else {
        // Fallback: use stationary distribution
        for (j in 0 until n) {
            alpha.set(0, j, pi.get(0, j))
        }
    }

    // Final normalization
    var finalSum = 0.0
    for (j in 0 until n) {
        finalSum += alpha.get(0, j)
    }
    if (finalSum > 0 && abs(finalSum - 1.0) > 1e-14) {
        for (j in 0 until n) {
            alpha.set(0, j, alpha.get(0, j) / finalSum)
        }
    }

    // Validate T is a proper sub-generator
    for (i in 0 until n) {
        require(T.get(i, i) <= 0) {
            "T($i,$i) = ${T.get(i, i)} > 0, expected non-positive"
        }
    }

    return Aoi_dist2phResult(alpha, T)
}

/**
 * Age of Information dist2ph algorithms
 */
@Suppress("unused")
class AoiDist2phAlgo {
    companion object {
        // Class documentation marker for Dokka
    }
}
