/**
 * @file Original CoMoM implementation for finite repairman model
 *
 * Implements the original Convolution Method of Moments (CoMoM) for computing the
 * logarithm of the normalizing constant in single-station repairman models. Uses
 * matrix recursion with population expansion to handle multiple job classes, with
 * demand rescaling and numerical stabilization through absolute-value normalization.
 *
 * @since LINE 3.0
 */
package jline.api.pfqn.nc

import jline.GlobalConstants
import jline.util.Maths
import jline.util.matrix.Matrix
import org.apache.commons.math3.util.FastMath

/**
 * Compute the log normalizing constant of a repairman model using original CoMoM
 *
 * @param L    - demands at all stations (1 x R, single station only)
 * @param N    - number of jobs for each class (1 x R)
 * @param Z    - think time for each class (1 x R)
 * @param atol - absolute numerical tolerance
 * @return logarithm of the normalizing constant
 */

@JvmOverloads
fun pfqn_comomrm_orig(L: Matrix, N: Matrix, Z: Matrix, atol: Double = GlobalConstants.FineTol): Double {
    if (L.numRows != 1) {
        throw RuntimeException("pfqn_comomrm_orig: The solver accepts at most a single queueing station.")
    }

    val m = 1
    val lambda = Matrix(1, N.numCols)
    lambda.fill(0.0)
    val ret = pfqn_nc_sanitize(lambda, L, N, Z, atol)
    var L_new = ret.L
    var N_new = ret.N
    var Z_new = ret.Z
    val lG0 = ret.lGremaind

    val M = L_new.numRows
    val R = L_new.numCols

    // Rescale demands
    val Lmax = Matrix(1, R)
    for (r in 0..<R) {
        var maxVal = L_new[0, r]
        if (maxVal < atol) {
            maxVal = Z_new[r]
        }
        Lmax[r] = maxVal
    }
    for (r in 0..<R) {
        if (Lmax[r] > 0) {
            L_new[0, r] = L_new[0, r] / Lmax[r]
            Z_new[r] = Z_new[r] / Lmax[r]
        }
    }

    // Sort from smallest to largest Z
    val rsort = IntArray(R)
    for (i in 0..<R) rsort[i] = i
    // Simple insertion sort by Z
    for (i in 1..<R) {
        val key = rsort[i]
        val keyVal = Z_new[key]
        var j = i - 1
        while (j >= 0 && Z_new[rsort[j]] > keyVal) {
            rsort[j + 1] = rsort[j]
            j--
        }
        rsort[j + 1] = key
    }
    val L_sorted = Matrix(1, R)
    val Z_sorted = Matrix(1, R)
    val N_sorted = Matrix(1, R)
    val Lmax_sorted = Matrix(1, R)
    for (i in 0..<R) {
        L_sorted[0, i] = L_new[0, rsort[i]]
        Z_sorted[i] = Z_new[rsort[i]]
        N_sorted[i] = N_new[rsort[i]]
        Lmax_sorted[i] = Lmax[rsort[i]]
    }
    L_new = L_sorted
    Z_new = Z_sorted
    N_new = N_sorted

    // Initialize
    val nvec = Matrix(1, R)
    nvec.fill(0.0)
    var h = Matrix(2, 1)
    h[0] = 1.0
    h[1] = 1.0
    // lh = log(h) + factln(sum(nvec)+M-1) - sum(factln(nvec))
    val lh = Matrix(2, 1)
    for (i in 0..<2) {
        lh[i] = FastMath.log(h[i]) + Maths.factln(nvec.elementSum() + M - 1) - Matrix.factln(nvec).elementSum()
    }
    for (i in 0..<2) {
        h[i] = FastMath.exp(lh[i])
    }
    val scale = Matrix(1, N_new.elementSum().toInt())
    scale.fill(0.0)

    var h_1 = h.copy()
    var nt = 0

    // Iterate
    for (r in 0..<R) {
        for (Nr in 1..N_new[r].toInt()) {
            nvec[r] = nvec[r] + 1

            if (Nr == 1) {
                if (r > 0) {
                    // Expand h vector to accommodate new class
                    val r1 = r
                    val P = Matrix(2 * r1, 2 * (r + 1))
                    P.fill(0.0)
                    // P(1:r1,1:r1) = eye(r1)
                    for (i in 0..<r1) P[i, i] = 1.0
                    // P((r1+1):2*r1,(r+1):(2*r-1)) = eye(r1)
                    for (i in 0..<r1) P[r1 + i, r + 1 + i] = 1.0

                    val h1 = Matrix(2 * (r + 1), 1)
                    h1.fill(0.0)
                    // h1 = h' * P transposed = P^T * h
                    for (i in 0..<2 * (r + 1)) {
                        var sum = 0.0
                        for (j in 0..<2 * r1) {
                            sum += P[j, i] * h[j]
                        }
                        h1[i] = sum
                    }

                    val nvecSum = nvec.elementSum()
                    if (nvecSum > 1 && nt > 0) {
                        h1[r] = h_1[0] * nvec[r1 - 1] / (nvecSum - 1) / scale[nt - 1]
                        h1[h1.length() - 1] = h_1[r1] * nvec[r1 - 1] / (nvecSum - 1) / scale[nt - 1]
                    }
                    h = h1
                }

                // Build A, DA, B matrices (2r+2 x 2r+2, but using r+1 as current class count)
                val sz = 2 * (r + 1)
                val A = Matrix(sz, sz)
                A.fill(0.0)
                val DA = Matrix(sz, sz)
                DA.fill(0.0)
                val B = Matrix(sz, sz)
                B.fill(0.0)

                // 1 CE for G+
                A[0, 0] = 1.0
                for (s in 0..<r) {
                    A[0, 1 + s] = -L_new[0, s]
                }
                A[0, r + 1] = -1.0
                B[0, 0] = L_new[0, r]

                // Class-1..(R-1) PCs for G
                for (s in 0..<r) {
                    A[1 + s, r + 1] = N_new[s]
                    A[1 + s, r + 1 + 1 + s] = -Z_new[s]
                    A[1 + s, 1 + s] = -m * L_new[0, s]
                }

                // Class-R PCs for G and Gr
                for (i in 0..r) {
                    A[r + 1 + i, r + 1 + i] = Nr.toDouble()
                    DA[r + 1 + i, r + 1 + i] = 1.0
                    B[r + 1 + i, i] = m * L_new[0, r]
                    B[r + 1 + i, r + 1 + i] = Z_new[r]
                }

                // Compute F1r and F2r via block factorization
                // C = A(1:r+1, 1:r+1), A12 = A(1:r+1, r+1:2*(r+1))
                val rp1 = r + 1
                val C = Matrix(rp1, rp1)
                val A12 = Matrix(rp1, rp1)
                for (i in 0..<rp1) {
                    for (j in 0..<rp1) {
                        C[i, j] = A[i, j]
                        A12[i, j] = A[i, rp1 + j]
                    }
                }

                // B1r = B(1:r+1, :), B2r = B(r+1:2*(r+1), :)
                val B1r = Matrix(rp1, sz)
                val B2r = Matrix(rp1, sz)
                for (i in 0..<rp1) {
                    for (j in 0..<sz) {
                        B1r[i, j] = B[i, j]
                        B2r[i, j] = B[rp1 + i, j]
                    }
                }

                // inv(C) * B1r -> top part of F1r
                val iC = C.inv()
                val iCB1r = iC.mult(B1r)
                val F1r = Matrix(sz, sz)
                F1r.fill(0.0)
                for (i in 0..<rp1) {
                    for (j in 0..<sz) {
                        F1r[i, j] = iCB1r[i, j]
                    }
                }
                // bottom part of F1r is 0*B2r = zeros

                // F2r = [-C\A12*B2r; B2r]
                val iCA12 = iC.mult(A12)
                val iCA12B2r = iCA12.mult(B2r)
                val F2r = Matrix(sz, sz)
                F2r.fill(0.0)
                for (i in 0..<rp1) {
                    for (j in 0..<sz) {
                        F2r[i, j] = -iCA12B2r[i, j]
                    }
                }
                for (i in 0..<rp1) {
                    for (j in 0..<sz) {
                        F2r[rp1 + i, j] = B2r[i, j]
                    }
                }

                // h = (nvec(r)*F1r + F2r) * h_1 / (sum(nvec)+M-1)
                h_1 = h.copy()
                val combined = Matrix(sz, sz)
                for (i in 0..<sz) {
                    for (j in 0..<sz) {
                        combined[i, j] = nvec[r] * F1r[i, j] + F2r[i, j]
                    }
                }
                h = combined.mult(h_1)
                val denom = nvec.elementSum() + M - 1
                for (i in 0..<h.length()) {
                    h[i] = h[i] / denom
                }
            } else {
                // Subsequent iterations use same F1r, F2r
                h_1 = h.copy()
                // Recompute: need F1r and F2r from class r
                // For Nr > 1, the A matrix only changes by DA, so A = A + DA
                // But the MATLAB code uses: h = (nvec(r)*F1r+F2r)*h_1/(sum(nvec)+M-1)
                // where F1r and F2r were computed at Nr=1

                // We need to rebuild F1r and F2r for subsequent Nr
                // Actually re-reading the MATLAB code: F1r and F2r don't change; only nvec(r) and sum(nvec) change
                val sz = 2 * (r + 1)
                val rp1 = r + 1

                val A_loc = Matrix(sz, sz)
                A_loc.fill(0.0)
                val B_loc = Matrix(sz, sz)
                B_loc.fill(0.0)

                A_loc[0, 0] = 1.0
                for (s in 0..<r) {
                    A_loc[0, 1 + s] = -L_new[0, s]
                }
                A_loc[0, r + 1] = -1.0
                B_loc[0, 0] = L_new[0, r]

                for (s in 0..<r) {
                    A_loc[1 + s, r + 1] = N_new[s]
                    A_loc[1 + s, r + 1 + 1 + s] = -Z_new[s]
                    A_loc[1 + s, 1 + s] = -m * L_new[0, s]
                }

                for (i in 0..r) {
                    A_loc[r + 1 + i, r + 1 + i] = nvec[r]
                    B_loc[r + 1 + i, i] = m * L_new[0, r]
                    B_loc[r + 1 + i, r + 1 + i] = Z_new[r]
                }

                val C_loc = Matrix(rp1, rp1)
                val A12_loc = Matrix(rp1, rp1)
                for (i in 0..<rp1) {
                    for (j in 0..<rp1) {
                        C_loc[i, j] = A_loc[i, j]
                        A12_loc[i, j] = A_loc[i, rp1 + j]
                    }
                }

                val B1r_loc = Matrix(rp1, sz)
                val B2r_loc = Matrix(rp1, sz)
                for (i in 0..<rp1) {
                    for (j in 0..<sz) {
                        B1r_loc[i, j] = B_loc[i, j]
                        B2r_loc[i, j] = B_loc[rp1 + i, j]
                    }
                }

                val iC_loc = C_loc.inv()
                val iCB1r_loc = iC_loc.mult(B1r_loc)

                val F1r_loc = Matrix(sz, sz)
                F1r_loc.fill(0.0)
                for (i in 0..<rp1) {
                    for (j in 0..<sz) {
                        F1r_loc[i, j] = iCB1r_loc[i, j]
                    }
                }

                val iCA12_loc = iC_loc.mult(A12_loc)
                val iCA12B2r_loc = iCA12_loc.mult(B2r_loc)
                val F2r_loc = Matrix(sz, sz)
                F2r_loc.fill(0.0)
                for (i in 0..<rp1) {
                    for (j in 0..<sz) {
                        F2r_loc[i, j] = -iCA12B2r_loc[i, j]
                    }
                }
                for (i in 0..<rp1) {
                    for (j in 0..<sz) {
                        F2r_loc[rp1 + i, j] = B2r_loc[i, j]
                    }
                }

                val combined = Matrix(sz, sz)
                for (i in 0..<sz) {
                    for (j in 0..<sz) {
                        combined[i, j] = nvec[r] * F1r_loc[i, j] + F2r_loc[i, j]
                    }
                }
                h = combined.mult(h_1)
                val denom = nvec.elementSum() + M - 1
                for (i in 0..<h.length()) {
                    h[i] = h[i] / denom
                }
            }

            nt = nvec.elementSum().toInt()
            scale[nt - 1] = FastMath.abs(h.sort().elementSum())
            h.absEq()
            h.scaleEq(1.0 / scale[nt - 1])
        }
    }

    // Unscale and return the log of the normalizing constant
    var logScale = 0.0
    for (i in 0..<scale.length()) {
        logScale += FastMath.log(scale[i])
    }

    var NlogLmax = 0.0
    for (i in 0..<R) {
        NlogLmax += N_sorted[i] * FastMath.log(Lmax_sorted[i])
    }

    val lG = lG0 + FastMath.log(h[h.length() - 1 - (R - 1)]) +
        Maths.factln(N_new.elementSum() + M - 1) - Matrix.factln(N_new).elementSum() +
        NlogLmax + logScale

    return lG
}

/**
 * PFQN comomrm_orig algorithms
 */
@Suppress("unused")
class PfqnComomrmOrigAlgo {
    companion object {
        // Class documentation marker for Dokka
    }
}
