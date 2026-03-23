/**
 * @file Laplace-Stieltjes Transform functions for AoI analysis
 *
 * Provides LST functions for common distributions used in Age of Information
 * analysis: exponential, Erlang, deterministic, and phase-type.
 *
 * @since LINE 3.0
 */
package jline.api.aoi

import jline.util.matrix.Matrix
import kotlin.math.exp
import kotlin.math.pow

/**
 * Laplace-Stieltjes transform for exponential distribution.
 *
 * Returns an LstFunction for the LST of an exponential distribution
 * with rate mu: H*(s) = mu / (mu + s)
 *
 * @param mu Rate parameter (mean = 1/mu), must be positive
 * @return LstFunction computing mu / (mu + s)
 * @throws IllegalArgumentException if mu <= 0
 */
fun aoi_lst_exp(mu: Double): LstFunction {
    require(mu > 0) { "Rate mu must be positive" }
    return object : LstFunction {
        override fun evaluate(s: Double): Double {
            return mu / (mu + s)
        }
    }
}

/**
 * Laplace-Stieltjes transform for deterministic (constant) distribution.
 *
 * Returns an LstFunction for the LST of a deterministic distribution
 * with constant value d: H*(s) = exp(-s * d)
 *
 * @param d Constant value, must be positive
 * @return LstFunction computing exp(-s * d)
 * @throws IllegalArgumentException if d <= 0
 */
fun aoi_lst_det(d: Double): LstFunction {
    require(d > 0) { "Constant d must be positive" }
    return object : LstFunction {
        override fun evaluate(s: Double): Double {
            return exp(-s * d)
        }
    }
}

/**
 * Laplace-Stieltjes transform for Erlang distribution.
 *
 * Returns an LstFunction for the LST of an Erlang-k distribution
 * with rate parameter mu: H*(s) = (mu / (mu + s))^k
 *
 * @param k Shape parameter (number of phases), must be a positive integer
 * @param mu Rate parameter per phase (mean = k/mu), must be positive
 * @return LstFunction computing (mu / (mu + s))^k
 * @throws IllegalArgumentException if k < 1 or k is not integer, or mu <= 0
 */
fun aoi_lst_erlang(k: Int, mu: Double): LstFunction {
    require(k >= 1) { "Shape k must be a positive integer" }
    require(mu > 0) { "Rate mu must be positive" }
    return object : LstFunction {
        override fun evaluate(s: Double): Double {
            return (mu / (mu + s)).pow(k.toDouble())
        }
    }
}

/**
 * Laplace-Stieltjes transform for phase-type distribution.
 *
 * Returns an LstFunction for the LST of a phase-type (PH) distribution
 * with initial probability vector alpha and sub-generator matrix T.
 *
 * H*(s) = alpha * (s*I - T)^{-1} * (-T * e)
 *
 * where e = ones(n,1) is the vector of ones.
 *
 * @param alpha Initial probability row vector (1 x n)
 * @param T Sub-generator matrix (n x n)
 * @return LstFunction computing alpha * inv(s*I - T) * t
 * @throws IllegalArgumentException if dimensions mismatch
 */
fun aoi_lst_ph(alpha: Matrix, T: Matrix): LstFunction {
    val n = alpha.numCols
    require(T.numRows == n && T.numCols == n) {
        "T must be $n x $n to match alpha"
    }

    // Precompute exit rate vector t = -T * ones(n,1)
    val ones = Matrix.ones(n, 1)
    val exitRates = T.mult(ones).scale(-1.0)

    return object : LstFunction {
        override fun evaluate(s: Double): Double {
            // Compute (s*I - T)
            val sIminusT = Matrix.eye(n).scale(s).sub(T)
            // Solve (s*I - T) * x = t for x using static solveSafe
            val x = Matrix(n, 1)
            Matrix.solveSafe(sIminusT, exitRates, x)
            // Compute alpha * x
            return alpha.mult(x).get(0, 0)
        }
    }
}

/**
 * Age of Information LST algorithms
 */
@Suppress("unused")
class AoiLstAlgo {
    companion object {
        // Class documentation marker for Dokka
    }
}
