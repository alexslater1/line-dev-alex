/**
 * @file Laplace approximation for normalizing constant
 *
 * Implements the Laplace method for approximating the normalizing constant of a
 * product-form queueing network. Uses the saddle-point method to find the dominant
 * contribution to the integral representation and provides a second-order Gaussian
 * approximation around the saddle point.
 *
 * @since LINE 3.0
 */
package jline.api.pfqn.nc

import jline.util.Maths
import jline.util.matrix.Matrix
import org.apache.commons.math3.util.FastMath

/**
 * Compute the Laplace approximation for the log normalizing constant
 *
 * @param L - service demand matrix (M x R, typically 1 x R for repairman model)
 * @param N - population vector (1 x R)
 * @param Z - think time vector (1 x R)
 * @return logI - logarithm of the normalizing constant approximation (NaN if no valid saddle point)
 */

fun pfqn_lap(L: Matrix, N: Matrix, Z: Matrix): Double {
    val Ntot = N.elementSum()
    val R = N.numCols

    // f(x) = 1 - sum(N.*L./(Z+Ntot*L.*x))
    // Find u0 such that f(u0) = 0 using bisection
    var u0 = findZero(L, N, Z, Ntot, R)

    if (u0 < 0) {
        return Double.NaN
    }

    if (!java.lang.Double.isFinite(u0)) {
        // Fallback: scan for sign change
        val initSign = Math.signum(evalF(0.001, L, N, Z, Ntot, R))
        u0 = Double.NaN
        var x = 1e-4
        while (x <= 10.0) {
            val fx = evalF(x, L, N, Z, Ntot, R)
            if (Math.signum(fx) != initSign) {
                u0 = x
                break
            }
            x += 1e-4
        }
        if (!java.lang.Double.isFinite(u0) || u0 < 0) {
            return Double.NaN
        }
    }

    if (u0 < 0) {
        return Double.NaN
    }

    // logI = log(Ntot) - sum(factln(N)) - Ntot*u0 + sum(N.*log(Z+L.*u0*Ntot))
    //        + 0.5*log(2*pi) - 0.5*log(sum((N./Ntot)./(Z./(Ntot.*L)+u0).^2)) - 0.5*log(Ntot)
    var logI = FastMath.log(Ntot)
    for (r in 0..<R) {
        logI -= Maths.factln(N[r])
    }
    logI -= Ntot * u0
    for (r in 0..<R) {
        logI += N[r] * FastMath.log(Z[r] + L[r] * u0 * Ntot)
    }
    logI += 0.5 * FastMath.log(2 * Math.PI)

    // f''(u0) = sum((N./Ntot)./(Z./(Ntot.*L)+u0).^2)
    var f2 = 0.0
    for (r in 0..<R) {
        if (L[r] > 0) {
            val term = Z[r] / (Ntot * L[r]) + u0
            f2 += (N[r] / Ntot) / (term * term)
        }
    }
    logI -= 0.5 * FastMath.log(f2)
    logI -= 0.5 * FastMath.log(Ntot)

    return logI
}

/**
 * Evaluate f(x) = 1 - sum(N.*L./(Z+Ntot*L.*x))
 */
private fun evalF(x: Double, L: Matrix, N: Matrix, Z: Matrix, Ntot: Double, R: Int): Double {
    var sum = 0.0
    for (r in 0..<R) {
        val denom = Z[r] + Ntot * L[r] * x
        if (denom > 0) {
            sum += N[r] * L[r] / denom
        }
    }
    return 1.0 - sum
}

/**
 * Find the zero of f(x) = 1 - sum(N.*L./(Z+Ntot*L.*x)) using bisection method.
 * The function is monotonically increasing from -inf at x=0 to 1 as x->inf.
 */
private fun findZero(L: Matrix, N: Matrix, Z: Matrix, Ntot: Double, R: Int): Double {
    // Use the bisection method to find the root
    var lo = 1e-10
    var hi = 100.0

    val flo = evalF(lo, L, N, Z, Ntot, R)
    var fhi = evalF(hi, L, N, Z, Ntot, R)

    // If f(lo) and f(hi) have the same sign, extend search range
    if (flo * fhi > 0) {
        // f is increasing, so if flo > 0 then root is before lo
        if (flo > 0) {
            lo = 1e-15
            if (evalF(lo, L, N, Z, Ntot, R) > 0) {
                return -1.0 // No valid root
            }
        } else {
            // fhi < 0, extend range
            hi = 10000.0
            fhi = evalF(hi, L, N, Z, Ntot, R)
            if (fhi < 0) {
                return Double.POSITIVE_INFINITY
            }
        }
    }

    // Bisection
    val maxIter = 200
    val tol = 1e-14
    for (iter in 0..<maxIter) {
        val mid = (lo + hi) / 2
        val fmid = evalF(mid, L, N, Z, Ntot, R)
        if (FastMath.abs(fmid) < tol || (hi - lo) / 2 < tol) {
            return mid
        }
        if (fmid * evalF(lo, L, N, Z, Ntot, R) < 0) {
            hi = mid
        } else {
            lo = mid
        }
    }
    return (lo + hi) / 2
}

/**
 * PFQN lap algorithms
 */
@Suppress("unused")
class PfqnLapAlgo {
    companion object {
        // Class documentation marker for Dokka
    }
}
