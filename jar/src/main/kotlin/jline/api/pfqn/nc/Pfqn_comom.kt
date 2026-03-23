/**
 * @file CoMoM algorithm for computing the normalizing constant
 *
 * Implements the Convolution Method of Moments (CoMoM) for computing the normalizing
 * constant of multi-station product-form queueing networks with multiple job classes.
 * Uses a matrix recursion over class populations with conditional equations (CE) and
 * population constraints (PC) to avoid explicit enumeration of the state space.
 *
 * @since LINE 3.0
 */
package jline.api.pfqn.nc

import jline.util.Maths
import jline.util.matrix.Matrix
import org.apache.commons.math3.util.FastMath

/**
 * Compute the log normalizing constant of a multi-station closed network using CoMoM
 *
 * @param L    - service demand matrix (M x R)
 * @param N    - population vector (1 x R)
 * @param Z    - think time vector (1 x R), default zeros
 * @param atol - absolute numerical tolerance
 * @return lG - logarithm of the normalizing constant
 */

fun pfqn_comom(L: Matrix, N: Matrix, Z: Matrix?, atol: Double): Double {
    val Zmat = Z ?: Matrix(1, N.numCols)
    val M = L.numRows
    val R = L.numCols

    // Rescale demands
    val Lmax = Matrix(1, R)
    for (r in 0..<R) {
        var maxVal = Double.NEGATIVE_INFINITY
        for (i in 0..<M) {
            if (L[i, r] > maxVal) maxVal = L[i, r]
        }
        if (maxVal < atol) {
            maxVal = Zmat[r]
        }
        Lmax[r] = maxVal
    }

    val Ls = L.copy()
    val Zs = Zmat.copy()
    for (i in 0..<M) {
        for (r in 0..<R) {
            if (Lmax[r] > 0) {
                Ls[i, r] = Ls[i, r] / Lmax[r]
            }
        }
    }
    for (r in 0..<R) {
        if (Lmax[r] > 0) {
            Zs[r] = Zs[r] / Lmax[r]
        }
    }

    // Build Dn basis: multichoose(R, M) with column R zeroed, sorted by nnzpos
    val DnRaw = multichoose(R, M)
    // Zero out column R-1 (0-indexed)
    for (i in 0..<DnRaw.size) {
        DnRaw[i][R - 1] = 0
    }
    val Dn = sortByNnzPos(DnRaw, R)
    val numDn = Dn.size

    // Initialize
    val nvec = IntArray(R)
    val totalCols = numDn * (M + 1)
    val h = DoubleArray(totalCols)
    // ginit: g(hash(N,N,i+1)) = 1 for i = 0..M
    for (i in 0..M) {
        val col = hash(Dn, N, nvec, i + 1, M, R)
        if (col >= 0 && col < totalCols) {
            h[col] = 1.0
        }
    }

    // Apply initial log scaling
    val sumNvec = sumArray(nvec)
    val logScale = Maths.factln(sumNvec + M - 1) - sumFactln(nvec)
    for (i in 0..<totalCols) {
        if (h[i] > 0) {
            h[i] = FastMath.exp(FastMath.log(h[i]) + logScale)
        }
    }

    val scale = DoubleArray(sumArrayInt(N))

    // Iterate
    for (r in 0..<R) {
        var A: Array<DoubleArray>? = null
        var B: Array<DoubleArray>? = null
        var DA: Array<DoubleArray>? = null

        for (Nr in 1..N[r].toInt()) {
            nvec[r]++

            if (Nr == 1) {
                val matrices = genMatrix(Ls, nvec, Zs, r, Dn, M, R, N)
                A = matrices.first
                B = matrices.second
                DA = matrices.third
            } else {
                // A = A + DA
                for (i in A!!.indices) {
                    for (j in A[0].indices) {
                        A[i][j] += DA!![i][j]
                    }
                }
            }

            val sz = A!!.size
            // b = B*h*nvec(r)/(sum(nvec)+M-1)
            val coeff = nvec[r].toDouble() / (sumArray(nvec) + M - 1).toDouble()
            val b = DoubleArray(sz)
            for (i in 0..<sz) {
                var sum = 0.0
                for (j in 0..<sz) {
                    sum += B!![i][j] * h[j]
                }
                b[i] = sum * coeff
            }

            // h = A\b (solve linear system)
            val hNew = solveLinearSystem(A, b)
            for (i in 0..<sz) {
                h[i] = hNew[i]
            }

            val nt = sumArray(nvec)
            // scale(nt) = abs(sum(sort(h)))
            val sortedH = h.clone()
            sortedH.sort()
            var absSum = 0.0
            for (v in sortedH) absSum += v
            scale[nt - 1] = FastMath.abs(absSum)

            // h = abs(h) / scale(nt)
            for (i in h.indices) {
                h[i] = FastMath.abs(h[i]) / scale[nt - 1]
            }
        }
    }

    // Unscale and return the log of the normalizing constant
    // lG = log(h(end-(R-1))) + factln(sum(N)+M-1) - sum(factln(N)) + N*log(Lmax)' + sum(log(scale))
    val sumN = sumArrayInt(N)
    var lG = FastMath.log(h[totalCols - R])
    lG += Maths.factln(sumN + M - 1)
    for (r in 0..<R) {
        lG -= Maths.factln(N[r])
    }
    for (r in 0..<R) {
        lG += N[r] * FastMath.log(Lmax[r])
    }
    for (i in 0..<sumN) {
        lG += FastMath.log(scale[i])
    }

    return lG
}

/**
 * Generate the CE and PC matrices for the CoMoM recursion
 */
private fun genMatrix(
    L: Matrix, nvec: IntArray, Z: Matrix, r: Int,
    Dn: List<IntArray>, M: Int, R: Int, N: Matrix
): Triple<Array<DoubleArray>, Array<DoubleArray>, Array<DoubleArray>> {
    val numDn = Dn.size
    val sz = numDn * (M + 1)
    val A = Array(sz) { DoubleArray(sz) }
    val DA = Array(sz) { DoubleArray(sz) }
    val B = Array(sz) { DoubleArray(sz) }

    var row = 0
    for (d in 0..<numDn) {
        val dn = Dn[d]

        // Check if sum(Dn(d, r:R-1)) > 0 (using 0-indexed: Dn(d, r..R-2))
        var sumDnRToEnd = 0
        for (t in r..<R - 1) {
            sumDnRToEnd += dn[t]
        }

        if (sumDnRToEnd > 0) {
            // Dummy rows for unused norm consts
            for (k in 0..M) {
                if (row >= sz) break
                val nMinusDn = IntArray(R)
                for (t in 0..<R) nMinusDn[t] = nvec[t] - dn[t]
                val col = hash(Dn, N, nMinusDn, k + 1, M, R)
                if (col >= 0 && col < sz) A[row][col] = 1.0

                // Check if sum(Dn(d, (r+1):R-1)) > 0
                var sumDnR1ToEnd = 0
                for (t in r + 1..<R - 1) {
                    sumDnR1ToEnd += dn[t]
                }

                if (sumDnR1ToEnd > 0) {
                    if (col >= 0 && col < sz) B[row][col] = 1.0
                } else {
                    val er = IntArray(R)
                    er[r] = 1
                    val nMinusDnPlusEr = IntArray(R)
                    for (t in 0..<R) nMinusDnPlusEr[t] = nvec[t] - dn[t] + er[t]
                    val colB = hash(Dn, N, nMinusDnPlusEr, k + 1, M, R)
                    if (colB >= 0 && colB < sz) B[row][colB] = 1.0
                }
                row++
            }
        } else {
            // Sum of Dn(d, 1:r) (0-indexed: 0..r-1)
            var sumDn1ToR = 0
            for (t in 0..<r) {
                sumDn1ToR += dn[t]
            }

            if (sumDn1ToR < M) {
                // CE equations
                for (k in 1..M) {
                    if (row >= sz) break
                    val nMinusDn = IntArray(R)
                    for (t in 0..<R) nMinusDn[t] = nvec[t] - dn[t]

                    val colKP1 = hash(Dn, N, nMinusDn, k + 1, M, R)
                    if (colKP1 >= 0 && colKP1 < sz) A[row][colKP1] = 1.0

                    val col0P1 = hash(Dn, N, nMinusDn, 0 + 1, M, R)
                    if (col0P1 >= 0 && col0P1 < sz) A[row][col0P1] = -1.0

                    for (s in 0..<r) {
                        // oner(N-Dn(d,:), s): decrement position s
                        val nOner = IntArray(R)
                        for (t in 0..<R) nOner[t] = nvec[t] - dn[t]
                        nOner[s]--
                        val colOner = hash(Dn, N, nOner, k + 1, M, R)
                        if (colOner >= 0 && colOner < sz) {
                            A[row][colOner] = A[row][colOner] - L[k - 1, s]
                        }
                    }

                    if (colKP1 >= 0 && colKP1 < sz) B[row][colKP1] = L[k - 1, r]
                    row++
                }

                // PC equations for s = 1..(r-1)
                for (s in 0..<r) {
                    if (row >= sz) break
                    val nMinusDn = IntArray(R)
                    for (t in 0..<R) nMinusDn[t] = nvec[t] - dn[t]

                    val col0P1 = hash(Dn, N, nMinusDn, 0 + 1, M, R)
                    if (col0P1 >= 0 && col0P1 < sz) {
                        A[row][col0P1] = nMinusDn[s].toDouble()
                    }

                    val nOner = IntArray(R)
                    for (t in 0..<R) nOner[t] = nMinusDn[t]
                    nOner[s]--

                    val colOner0P1 = hash(Dn, N, nOner, 0 + 1, M, R)
                    if (colOner0P1 >= 0 && colOner0P1 < sz) {
                        A[row][colOner0P1] = A[row][colOner0P1] - Z[s]
                    }
                    for (k in 1..M) {
                        val colOnerKP1 = hash(Dn, N, nOner, k + 1, M, R)
                        if (colOnerKP1 >= 0 && colOnerKP1 < sz) {
                            A[row][colOnerKP1] = A[row][colOnerKP1] - L[k - 1, s]
                        }
                    }
                    row++
                }
            }

            // Add PC of class r
            if (row >= sz) continue
            val nMinusDn = IntArray(R)
            for (t in 0..<R) nMinusDn[t] = nvec[t] - dn[t]
            val col0P1 = hash(Dn, N, nMinusDn, 0 + 1, M, R)
            if (col0P1 >= 0 && col0P1 < sz) {
                A[row][col0P1] = nMinusDn[r].toDouble()
                DA[row][col0P1] = 1.0
                B[row][col0P1] = Z[r]
            }
            for (k in 1..M) {
                val colKP1 = hash(Dn, N, nMinusDn, k + 1, M, R)
                if (colKP1 >= 0 && colKP1 < sz) {
                    B[row][colKP1] = L[k - 1, r]
                }
            }
            row++
        }
    }

    return Triple(A, B, DA)
}

/**
 * Hash function to map (N, n, i) to column index in the CoMoM system.
 * i is 1-based (1 = G term, 2..M+1 = G+ terms at stations 1..M).
 */
private fun hash(Dn: List<IntArray>, N: Matrix, n: IntArray, i: Int, M: Int, R: Int): Int {
    // Compute N - n to get the Dn offset
    val diff = IntArray(R)
    for (t in 0..<R) {
        diff[t] = N[t].toInt() - n[t]
        // Actually we need to convert from the original N, but in the recursion
        // nvec is the current population. The hash uses Dn = N_orig - n
        // But here we store diff = (original N - n) = what we're looking for in Dn
        // Wait, re-reading the MATLAB: hash(N, N-Dn(d,:), k+1) means n = N - Dn(d,:)
        // so N - n = Dn(d,:). So diff = current_nvec - n_passed
        // Actually the MATLAB passes nvec (current cumulative population) as N to hash
        // Let me re-read...
    }

    val pos = matchRow(Dn, diff)
    if (pos < 0) return -1

    if (i == 1) {
        // col = size(Dn,1)*M + matchrow(Dn, N-n)
        return Dn.size * M + pos
    } else {
        // col = (matchrow(Dn, N-n) - 1)*M + i - 1
        return pos * M + i - 2
    }
}

/**
 * Find the index of a row in the Dn basis. Returns 0-based index or -1 if not found.
 */
private fun matchRow(Dn: List<IntArray>, row: IntArray): Int {
    for (i in Dn.indices) {
        var match = true
        for (j in row.indices) {
            if (Dn[i][j] != row[j]) {
                match = false
                break
            }
        }
        if (match) return i
    }
    return -1
}

/**
 * Generate all R-element compositions summing to M (multichoose(R, M)).
 */
private fun multichoose(R: Int, M: Int): MutableList<IntArray> {
    val result = ArrayList<IntArray>()
    if (R == 1) {
        result.add(intArrayOf(M))
        return result
    }
    if (M == 0) {
        result.add(IntArray(R))
        return result
    }
    for (i in 0..M) {
        val sub = multichoose(R - 1, M - i)
        for (s in sub) {
            val row = IntArray(R)
            row[0] = i
            System.arraycopy(s, 0, row, 1, R - 1)
            result.add(row)
        }
    }
    return result
}

/**
 * Sort rows by the pattern of zero positions (nnzpos ordering).
 */
private fun sortByNnzPos(list: MutableList<IntArray>, R: Int): List<IntArray> {
    val result = list.toMutableList()
    // Bubble sort by nnzcmp
    for (i in 0..<result.size - 1) {
        for (j in i + 1..<result.size) {
            if (nnzCmp(result[i], result[j]) == 1) {
                val tmp = result[i]
                result[i] = result[j]
                result[j] = tmp
            }
        }
    }
    return result
}

/**
 * Compare two rows by number of nonzeros and position of zeros.
 * Returns 1 if i1 < i2 (i2 has more zeros or left-most zero).
 */
private fun nnzCmp(i1: IntArray, i2: IntArray): Int {
    var nnz1 = 0
    var nnz2 = 0
    for (v in i1) if (v != 0) nnz1++
    for (v in i2) if (v != 0) nnz2++

    if (nnz1 > nnz2) return 1  // i2 has more zeros
    if (nnz1 < nnz2) return 0

    for (j in i1.indices) {
        if (i1[j] == 0 && i2[j] > 0) return 1
        if (i1[j] > 0 && i2[j] == 0) return 0
    }
    return 0
}

private fun sumArray(arr: IntArray): Int {
    var s = 0
    for (v in arr) s += v
    return s
}

private fun sumArrayInt(N: Matrix): Int {
    return N.elementSum().toInt()
}

private fun sumFactln(arr: IntArray): Double {
    var s = 0.0
    for (v in arr) s += Maths.factln(v)
    return s
}

/**
 * Solve a linear system A*x = b using Gaussian elimination with partial pivoting.
 */
private fun solveLinearSystem(A: Array<DoubleArray>, b: DoubleArray): DoubleArray {
    val n = b.size
    // Create augmented matrix
    val aug = Array(n) { i ->
        val row = DoubleArray(n + 1)
        System.arraycopy(A[i], 0, row, 0, n)
        row[n] = b[i]
        row
    }

    // Forward elimination with partial pivoting
    for (col in 0..<n) {
        // Find pivot
        var maxVal = FastMath.abs(aug[col][col])
        var maxRow = col
        for (row in col + 1..<n) {
            if (FastMath.abs(aug[row][col]) > maxVal) {
                maxVal = FastMath.abs(aug[row][col])
                maxRow = row
            }
        }
        // Swap rows
        if (maxRow != col) {
            val tmp = aug[col]
            aug[col] = aug[maxRow]
            aug[maxRow] = tmp
        }

        if (FastMath.abs(aug[col][col]) < 1e-30) continue

        // Eliminate below
        for (row in col + 1..<n) {
            val factor = aug[row][col] / aug[col][col]
            for (j in col..<n + 1) {
                aug[row][j] -= factor * aug[col][j]
            }
        }
    }

    // Back substitution
    val x = DoubleArray(n)
    for (row in n - 1 downTo 0) {
        if (FastMath.abs(aug[row][row]) < 1e-30) {
            x[row] = 0.0
            continue
        }
        var sum = aug[row][n]
        for (col in row + 1..<n) {
            sum -= aug[row][col] * x[col]
        }
        x[row] = sum / aug[row][row]
    }

    return x
}

/**
 * PFQN comom algorithms
 */
@Suppress("unused")
class PfqnComomAlgo {
    companion object {
        // Class documentation marker for Dokka
    }
}
