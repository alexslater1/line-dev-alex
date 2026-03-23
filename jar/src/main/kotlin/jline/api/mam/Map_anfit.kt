/**
 * @file Andersen-Nielsen MAP fitting algorithm
 *
 * Implements the MAP fitting algorithm from A.T. Andersen and B.F. Nielsen,
 * "A Markovian Approach for Modeling Packet Traffic with Long-Range Dependence",
 * IEEE JSAC 16(5), 1998. Constructs a MAP by superposing IPP (Interrupted Poisson
 * Process) components calibrated to match the Hurst parameter of the counting process.
 * Optionally performs least-square optimization to match autocorrelation coefficients
 * of inter-arrival times.
 *
 * @since LINE 3.0
 */
package jline.api.mam

import jline.util.matrix.Matrix
import jline.util.matrix.MatrixCell
import org.apache.commons.math3.analysis.MultivariateFunction
import org.apache.commons.math3.optim.InitialGuess
import org.apache.commons.math3.optim.MaxEval
import org.apache.commons.math3.optim.MaxIter
import org.apache.commons.math3.optim.nonlinear.scalar.GoalType
import org.apache.commons.math3.optim.nonlinear.scalar.ObjectiveFunction
import org.apache.commons.math3.optim.nonlinear.scalar.noderiv.NelderMeadSimplex
import org.apache.commons.math3.optim.nonlinear.scalar.noderiv.SimplexOptimizer
import kotlin.math.sqrt

/**
 * Fits a Markovian Arrival Process (MAP) using the Andersen-Nielsen algorithm.
 *
 * The algorithm constructs a MAP by superposing [ds] Interrupted Poisson Process (IPP)
 * components whose parameters are calibrated from the counting process statistics:
 * mean arrival rate [ls], lag-1 autocorrelation [rho], and Hurst parameter [H].
 * The number of time scales [n] controls the geometric spacing of transition rates.
 *
 * When [SA] and [SAlags] are provided, a least-square optimization step refines the
 * IPP parameters to match the specified autocorrelation coefficients of the inter-arrival
 * times.
 *
 * @param ls mean arrival rate of the counting process
 * @param rho lag-1 autocorrelation of the counting process
 * @param H Hurst coefficient of the counting process (0.5 < H < 1 for long-range dependence)
 * @param n number of time scales to be modeled
 * @param ds number of IPP components to be used in the superposition
 * @param SA autocorrelation coefficients of inter-arrival times to match (null if no LSQ fit)
 * @param SAlags lag indices corresponding to [SA] (null if no LSQ fit)
 * @param iter_max maximum number of iterations for the optimizer (default 100)
 * @param iter_tol convergence tolerance for the optimizer (default 1e-9)
 * @return a MatrixCell containing the fitted MAP transition matrices (D0, D1)
 */
fun map_anfit(
    ls: Double,
    rho: Double,
    H: Double,
    n: Int,
    ds: Int,
    SA: DoubleArray? = null,
    SAlags: IntArray? = null,
    iter_max: Int = 100,
    iter_tol: Double = 1e-9
): MatrixCell {
    val LSQFIT = SA != null && SAlags != null

    // If SA and SAlags are given, extract the autocorrelation values at the specified lags
    // MATLAB: SA = SA(SAlags) — index SA by SAlags (MATLAB 1-based), here 0-based
    val SAvals: DoubleArray?
    if (LSQFIT) {
        SAvals = DoubleArray(SAlags!!.size) { i -> SA!![SAlags[i]] }
    } else {
        SAvals = null
    }

    val beta = 2.0 - 2.0 * H

    // phi coefficients indexed 1..d (MATLAB 1-based), stored in a map
    val phi = HashMap<Int, Double>()

    // d may grow if we run out of non-zero phi components
    var d = ds
    var d0 = 0
    var a = Math.pow(10.0, n.toDouble() / (d - 1).toDouble())
    phi[d] = 1.0
    var i = 1

    // Main loop to compute phi coefficients (MATLAB lines 34-69)
    outer@ while (true) {
        // S = sum_{j=0}^{i-1} phi(d-j)^2 * exp(1 - a^(i-j))
        var S = 0.0
        for (j in 0 until i) {
            val phiVal = phi.getOrDefault(d - j, 0.0)
            S += phiVal * phiVal * Math.exp(1.0 - Math.pow(a, (i - j).toDouble()))
        }

        val D = Math.pow(a, i.toDouble() * beta) - S

        if (D < 0) {
            phi[d - i] = 0.0
            d0++
            if (ds > d - d0) {
                // Increase d and restart
                d++
                d0 = 0
                a = Math.pow(10.0, n.toDouble() / (d - 1).toDouble())
                phi.clear()
                phi[d] = 1.0
                i = 1
                continue@outer
            } else {
                i++
                if (i == d) {
                    break@outer
                } else {
                    continue@outer
                }
            }
        } else {
            phi[d - i] = sqrt(D)
            i++
            if (i == d) {
                break@outer
            } else {
                continue@outer
            }
        }
    }

    // Build k(2,:) transition rates (MATLAB lines 71-73)
    // k(2,1) = 0.8, k(2,i) = a^(1-i) * k(2,1) for i=2..d
    val k1 = DoubleArray(d) // k(2,i) in MATLAB, 0-indexed
    k1[0] = 0.8
    for (idx in 2..d) {
        k1[idx - 1] = Math.pow(a, 1.0 - idx.toDouble()) * k1[0]
    }

    // Warning check (MATLAB line 74)
    if (!(k1[0] < 1.0 && rho < 0.5)) {
        System.err.println("warning: if necessary adjust k(2,1) and/or rho")
    }

    // Compute eta (MATLAB lines 77-83)
    var Seta = 0.0
    for (idx in 1..d) {
        val kappa = k1[idx - 1]
        val e = Math.exp(-kappa)
        val phiVal = phi.getOrDefault(idx, 0.0)
        Seta += phiVal * phiVal * Math.pow(kappa, -2.0) *
                ((1.0 - e) * (1.0 - e) - 2.0 * rho * (kappa - (1.0 - e)))
    }
    val eta = sqrt(4.0 * rho * ls) / sqrt(Seta)

    // L = eta * sum(phi(1:d)) / 2 (MATLAB line 84)
    var sumPhi = 0.0
    for (idx in 1..d) {
        sumPhi += phi.getOrDefault(idx, 0.0)
    }
    val L = eta * sumPhi / 2.0

    // c(1,:), c(2,:), l(:) arrays (MATLAB lines 85-99)
    val c1 = DoubleArray(d)
    val c2 = DoubleArray(d)
    val l = DoubleArray(d)

    val lP: Double
    if (ls < L) {
        lP = 0.0
        for (idx in 1..d) {
            val phiVal = phi.getOrDefault(idx, 0.0)
            c1[idx - 1] = L * L / (ls * ls + L * L) * k1[idx - 1]
            c2[idx - 1] = k1[idx - 1] - c1[idx - 1]
            l[idx - 1] = phiVal * (ls * ls + L * L) / (ls * sumPhi)
        }
    } else {
        lP = ls - L
        for (idx in 1..d) {
            val phiVal = phi.getOrDefault(idx, 0.0)
            c2[idx - 1] = 0.5 * k1[idx - 1]
            c1[idx - 1] = c2[idx - 1]
            l[idx - 1] = eta * phiVal
        }
    }

    if (!LSQFIT) {
        // No Least Square Fit: compose and return (MATLAB lines 102-112)
        var MAP = buildPoissonMAP(lP)
        for (idx in 0 until d) {
            val IPP = buildIPP(c1[idx], c2[idx], l[idx])
            MAP = map_super(MAP, IPP)
        }
        return map_normalize(MAP)
    }

    // Least Square Fit (MATLAB lines 113-127)
    val SAv = SAvals!!
    val SAl = SAlags!!

    // k(1,i) and local ls(i) (MATLAB lines 116-119)
    val k0 = DoubleArray(d)
    val lsLocal = DoubleArray(d) // local ls(i) in MATLAB (shadows parameter ls)
    for (idx in 0 until d) {
        val cSum = c1[idx] + c2[idx]
        k0[idx] = l[idx] * l[idx] * c1[idx] * c2[idx] / (cSum * cSum * cSum)
        lsLocal[idx] = c2[idx] * l[idx] / cSum
    }

    // Initial r0 (MATLAB lines 122-124)
    val r0 = DoubleArray(d)
    for (idx in 0 until d) {
        r0[idx] = 1.0 + k0[idx] * k1[idx] / (lsLocal[idx] * lsLocal[idx])
    }

    // Convert SAlags to a Matrix for map_acf
    val lagsMatrix = Matrix(1, SAl.size, SAl.size)
    for (idx in SAl.indices) {
        lagsMatrix[idx] = SAl[idx].toDouble()
    }

    // Penalized objective function (MATLAB lines 128-144)
    // Constraint: -(ls(i) - sqrt(k(1,i)*k(2,i)/r(i))) <= 0
    // Lower bound: r(i) > EPSTOL
    val EPSTOL = 100.0 * iter_tol
    val PENALTY = 1e10

    val objective = MultivariateFunction { r ->
        // Check lower bounds
        for (idx in 0 until d) {
            if (r[idx] <= EPSTOL) return@MultivariateFunction PENALTY
        }

        // Check nonlinear constraints (penalty method)
        for (idx in 0 until d) {
            val cv = -(lsLocal[idx] - sqrt(k0[idx] * k1[idx] / r[idx]))
            if (cv > 0) {
                return@MultivariateFunction PENALTY + cv * PENALTY
            }
        }

        // Build MAP from current r (MATLAB objfun lines 136-142)
        var MAP0 = buildPoissonMAP(lP)
        for (idx in 0 until d) {
            val D0 = Matrix(2, 2, 4)
            D0[0, 0] = 0.0
            D0[0, 1] = k1[idx] * r[idx] / (1.0 + r[idx])
            D0[1, 0] = k1[idx] / (1.0 + r[idx])
            D0[1, 1] = 0.0
            val D1 = Matrix(2, 2, 4)
            D1[0, 0] = lsLocal[idx] + sqrt(k0[idx] * k1[idx] * r[idx])
            D1[0, 1] = 0.0
            D1[1, 0] = 0.0
            D1[1, 1] = lsLocal[idx] - sqrt(k0[idx] * k1[idx] / r[idx])
            val IPP = MatrixCell()
            IPP[0] = D0
            IPP[1] = D1
            MAP0 = map_super(MAP0, map_normalize(IPP))
        }

        // f = norm(map_acf(MAP0, SAlags) - SA', 2) (MATLAB line 143)
        val acfVals = map_acf(MAP0, lagsMatrix)
        var sumSq = 0.0
        for (idx in SAv.indices) {
            val diff = acfVals[idx] - SAv[idx]
            sumSq += diff * diff
        }
        sqrt(sumSq)
    }

    // Nelder-Mead optimization (MATLAB minimize lines 155-161)
    var bestMAP: MatrixCell? = null
    var bestF = Double.MAX_VALUE

    try {
        val optimizer = SimplexOptimizer(iter_tol, iter_tol)
        val simplex = NelderMeadSimplex(d, 0.1)

        val result = optimizer.optimize(
            MaxEval(iter_max * 200),
            MaxIter(iter_max),
            ObjectiveFunction(objective),
            GoalType.MINIMIZE,
            InitialGuess(r0),
            simplex
        )

        bestF = result.value
        val bestR = result.point

        // Rebuild the MAP from the best r
        var MAP0 = buildPoissonMAP(lP)
        for (idx in 0 until d) {
            val D0 = Matrix(2, 2, 4)
            D0[0, 0] = 0.0
            D0[0, 1] = k1[idx] * bestR[idx] / (1.0 + bestR[idx])
            D0[1, 0] = k1[idx] / (1.0 + bestR[idx])
            D0[1, 1] = 0.0
            val D1 = Matrix(2, 2, 4)
            D1[0, 0] = lsLocal[idx] + sqrt(k0[idx] * k1[idx] * bestR[idx])
            D1[0, 1] = 0.0
            D1[1, 0] = 0.0
            D1[1, 1] = lsLocal[idx] - sqrt(k0[idx] * k1[idx] / bestR[idx])
            val IPP = MatrixCell()
            IPP[0] = D0
            IPP[1] = D1
            MAP0 = map_super(MAP0, map_normalize(IPP))
        }
        bestMAP = MAP0
    } catch (e: Exception) {
        // If optimization fails, fall back to the non-LSQ MAP
    }

    if (bestMAP != null) {
        return map_normalize(bestMAP)
    }

    // Fallback: build without LSQ
    var MAP = buildPoissonMAP(lP)
    for (idx in 0 until d) {
        val IPP = buildIPP(c1[idx], c2[idx], l[idx])
        MAP = map_super(MAP, IPP)
    }
    return map_normalize(MAP)
}

/**
 * Builds a 1-state Poisson MAP with the given rate.
 *
 * Creates a MAP with D0 = [-rate] and D1 = [rate]. When rate is 0, this represents
 * a null process (no arrivals).
 *
 * @param rate the Poisson arrival rate
 * @return a MatrixCell containing the (D0, D1) matrices
 */
private fun buildPoissonMAP(rate: Double): MatrixCell {
    val MAP = MatrixCell()
    val D0 = Matrix(1, 1, 1)
    D0[0, 0] = -rate
    val D1 = Matrix(1, 1, 1)
    D1[0, 0] = rate
    MAP[0] = D0
    MAP[1] = D1
    return MAP
}

/**
 * Builds a 2-state IPP (Interrupted Poisson Process) from on/off rates and arrival rate.
 *
 * Creates a MAP with:
 * - D0 = [0, c1; c2, 0] (transition rates between on and off states)
 * - D1 = [arrivalRate, 0; 0, 0] (arrivals only in on state)
 *
 * The diagonal of D0 is set by [map_normalize] to ensure row sums are zero.
 *
 * @param c1 transition rate from on-state to off-state
 * @param c2 transition rate from off-state to on-state
 * @param arrivalRate arrival rate in the on-state
 * @return a normalized MatrixCell containing the IPP (D0, D1) matrices
 */
private fun buildIPP(c1: Double, c2: Double, arrivalRate: Double): MatrixCell {
    val D0 = Matrix(2, 2, 4)
    D0[0, 0] = 0.0
    D0[0, 1] = c1
    D0[1, 0] = c2
    D0[1, 1] = 0.0
    val D1 = Matrix(2, 2, 4)
    D1[0, 0] = arrivalRate
    D1[0, 1] = 0.0
    D1[1, 0] = 0.0
    D1[1, 1] = 0.0
    val IPP = MatrixCell()
    IPP[0] = D0
    IPP[1] = D1
    return map_normalize(IPP)
}

/**
 * MAP Andersen-Nielsen fitting algorithms
 */
@Suppress("unused")
class MapAnfitAlgo {
    companion object {
        // Class documentation marker for Dokka
    }
}
