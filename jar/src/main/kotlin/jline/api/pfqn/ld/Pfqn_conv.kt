/**
 * Multichain convolution algorithm for closed queueing networks with
 * Limited Joint Class Dependence (LJCD) service rates.
 *
 * Implements the convolution algorithm of Sauer (1983), Section 5.2,
 * "Computational Algorithms for State-Dependent Queueing Networks",
 * ACM TOCS, Vol. 1, No. 1, pp. 67-92.
 */
package jline.api.pfqn.ld

import jline.util.matrix.Matrix
import org.apache.commons.math3.special.Gamma
import kotlin.math.exp
import kotlin.math.ln
import kotlin.math.min

/**
 * Multichain convolution for LJCD networks.
 *
 * @param L  Service demand matrix (M x R)
 * @param N  Population vector (1 x R), must be finite
 * @param Z  Think time vector (1 x R), or null
 * @param mu Load-dependent rate matrix (M x sum(N)), or null for LI
 * @param ljcdscaling List of per-station LJCD tables; ljcdscaling[ist][r] is the
 *                    linearized scaling table for station ist and class r
 * @param ljcdcutoffs List of per-station cutoff vectors
 * @return Pair(G, lG) where G = normalizing constant G(N), lG = ln(G)
 */
fun pfqn_conv(
    L: Matrix,
    N: IntArray,
    Z: DoubleArray,
    mu: Matrix?,
    ljcdscaling: List<List<Matrix?>?>?,
    ljcdcutoffs: List<Matrix?>?
): DoubleArray {
    val M = L.numRows
    val R = L.numCols

    val stateSpaceSize = N.fold(1) { acc, n -> acc * (n + 1) }

    // Identify LJCD stations
    val isLjcd = BooleanArray(M) { ist ->
        ljcdscaling != null && ist < ljcdscaling.size && ljcdscaling[ist] != null &&
                ljcdscaling[ist]!!.any { it != null && it.length() > 0 }
    }

    // Precompute X_m(n) tables for LJCD stations (Sauer eq. 40)
    val Xm = arrayOfNulls<DoubleArray>(M)
    for (ist in 0 until M) {
        if (isLjcd[ist]) {
            Xm[ist] = DoubleArray(stateSpaceSize)
            Xm[ist]!![0] = 1.0  // X_m(0) = 1

            val cutoffs = ljcdcutoffs?.get(ist)
            val n = IntArray(R)
            while (true) {
                val idx = hashpop(n, N)
                val sumN = n.sum()
                if (sumN == 0) {
                    Xm[ist]!![idx] = 1.0
                } else {
                    for (r in 0 until R) {
                        if (n[r] > 0) {
                            // Get mu_km(n) from LJCD table
                            val nClamped = Matrix(1, R)
                            val cutoffsMat = Matrix(1, R)
                            for (k in 0 until R) {
                                val cutVal = cutoffs?.get(k) ?: N[k].toDouble()
                                nClamped.set(k, min(n[k].toDouble(), cutVal))
                                cutoffsMat.set(k, cutVal)
                            }
                            val lidx = ljd_linearize(nClamped, cutoffsMat)
                            val ljcdTable = ljcdscaling?.get(ist)?.get(r)
                            val mu_km = if (ljcdTable != null && lidx < ljcdTable.length()) {
                                ljcdTable.get(lidx)
                            } else {
                                1.0
                            }

                            n[r]--
                            val idxPrev = hashpop(n, N)
                            n[r]++

                            if (mu_km > 0) {
                                Xm[ist]!![idx] = (L.get(ist, r) / mu_km) * Xm[ist]!![idxPrev]
                            }
                            break
                        }
                    }
                }
                if (!pprodNext(n, N)) break
            }
        }
    }

    // Convolution: G_0(n) = F_Z(n), then G_m(n) = conv(G_{m-1}, X_m)(n)
    var gCurr = DoubleArray(stateSpaceSize)

    // Initialize G_0(n) = F_Z(n)
    val n = IntArray(R)
    while (true) {
        val idx = hashpop(n, N)
        gCurr[idx] = fz(Z, n)
        if (!pprodNext(n, N)) break
    }

    // Convolve stations
    for (ist in 0 until M) {
        if (isLjcd[ist]) {
            val gOld = gCurr.copyOf()
            gCurr = DoubleArray(stateSpaceSize)

            val nOuter = IntArray(R)
            while (true) {
                val idxN = hashpop(nOuter, N)
                var convSum = 0.0

                // Inner loop: enumerate all i from 0 to nOuter
                val i = IntArray(R)
                while (true) {
                    val idxI = hashpop(i, N)
                    val nmi = IntArray(R) { k -> nOuter[k] - i[k] }
                    val idxNmi = hashpop(nmi, N)
                    convSum += Xm[ist]!![idxI] * gOld[idxNmi]
                    if (!pprodNextBounded(i, nOuter)) break
                }

                gCurr[idxN] = convSum
                if (!pprodNext(nOuter, N)) break
            }
        } else {
            // LI station: efficient recurrence
            val nInner = IntArray(R)
            while (true) {
                val idxN = hashpop(nInner, N)
                for (r in 0 until R) {
                    if (nInner[r] >= 1) {
                        nInner[r]--
                        val idxN1r = hashpop(nInner, N)
                        nInner[r]++
                        gCurr[idxN] += L.get(ist, r) * gCurr[idxN1r]
                    }
                }
                if (!pprodNext(nInner, N)) break
            }
        }
    }

    val G = gCurr[stateSpaceSize - 1]
    val lG = if (G > 0) ln(G) else Double.NEGATIVE_INFINITY
    return doubleArrayOf(G, lG)
}

// Map population vector to linear index (0-based internally, but array is 0-based)
private fun hashpop(n: IntArray, N: IntArray): Int {
    var idx = 0
    var stride = 1
    for (r in n.indices) {
        idx += stride * n[r]
        stride *= (N[r] + 1)
    }
    return idx
}

// Advance population vector n (0 <= n <= N), returns false when done
private fun pprodNext(n: IntArray, N: IntArray): Boolean {
    val R = n.size
    // Check if we're at the maximum
    if ((0 until R).all { n[it] == N[it] }) return false

    var s = R - 1
    while (s >= 0 && n[s] == N[s]) {
        n[s] = 0
        s--
    }
    if (s >= 0) n[s]++
    return true
}

// Advance population vector i bounded by upper (0 <= i <= upper)
private fun pprodNextBounded(i: IntArray, upper: IntArray): Boolean {
    val R = i.size
    if ((0 until R).all { i[it] == upper[it] }) return false

    var s = R - 1
    while (s >= 0 && i[s] == upper[s]) {
        i[s] = 0
        s--
    }
    if (s >= 0) i[s]++
    return true
}

// Delay server unnormalized probability factor
// F = prod_r (Z(r)^n(r) / n(r)!)
private fun fz(Z: DoubleArray, n: IntArray): Double {
    if (n.sum() == 0) return 1.0
    var logF = 0.0
    for (r in n.indices) {
        if (Z[r] > 0) {
            logF += ln(Z[r]) * n[r]
            logF -= Gamma.logGamma((1 + n[r]).toDouble())
        } else if (n[r] > 0) {
            return 0.0
        }
    }
    return exp(logF)
}
