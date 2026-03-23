/**
 * @file ProCoMoM algorithm for computing marginal queue-length probabilities
 *
 * Implements the Probabilistic Convolution Method of Moments (ProCoMoM) for computing
 * marginal queue-length probability distributions in multi-station product-form queueing
 * networks. Uses matrix recursion with SVD/QR decomposition for numerical stability,
 * with automatic perturbation on rank deficiency.
 *
 * @since LINE 3.0
 */
package jline.api.pfqn.nc

import jline.io.Ret
import jline.util.Maths
import jline.util.matrix.Matrix
import org.apache.commons.math3.util.FastMath
import java.util.Random

/**
 * Compute marginal queue-length probabilities using ProCoMoM
 *
 * @param L    - service demand matrix (M x R)
 * @param N    - population vector (1 x R)
 * @param Z    - think time vector (1 x R), null or empty for zeros
 * @param atol - absolute numerical tolerance (default: 1e-14)
 * @return Pr - marginal probability matrix (M x sumN+1), Q - mean queue lengths (M x 1)
 */

@JvmOverloads
fun pfqn_procomom(L: Matrix, N: Matrix, Z: Matrix? = null, atol: Double = 1e-14): Ret.pfqnProcomom {
    val M = L.numRows
    val R = L.numCols
    val sumN = N.elementSum().toInt()

    val Zmat: Matrix
    if (Z == null || Z.isEmpty) {
        Zmat = Matrix(1, R)
        Zmat.fill(0.0)
    } else {
        Zmat = Z.copy()
    }

    // Rescale demands per class for numerical stability
    val Lmax = Matrix(1, R)
    for (r in 0..<R) {
        var maxVal = Double.NEGATIVE_INFINITY
        for (i in 0..<M) {
            if (L[i, r] > maxVal) maxVal = L[i, r]
        }
        if (maxVal < atol) maxVal = 1.0
        Lmax[r] = maxVal
    }
    val Ls = L.copy()
    val Zs = Zmat.copy()
    for (i in 0..<M) {
        for (r in 0..<R) {
            Ls[i, r] = Ls[i, r] / Lmax[r]
        }
    }
    for (r in 0..<R) {
        Zs[r] = Zs[r] / Lmax[r]
    }

    // Build Dn basis
    val DnRaw = multichooseP(R, M)
    for (i in 0..<DnRaw.size) {
        DnRaw[i][R - 1] = 0
    }
    val Dn = sortByNnzPosP(DnRaw)
    val numDn = Dn.size
    val basisSize = numDn * M

    // Solve with auto-perturbation on rank deficiency
    var Pr = solveAll(Ls, Zs, Dn, N, M, R, sumN, basisSize, numDn)
    var rankdef = Pr.second

    if (rankdef) {
        val rng = Random(23000)
        var Lscale = 0.0
        for (i in 0..<Ls.numRows) {
            for (j in 0..<Ls.numCols) {
                Lscale = FastMath.max(Lscale, FastMath.abs(Ls[i, j]))
            }
        }
        if (Lscale < atol) Lscale = 1.0

        val deltaExps = doubleArrayOf(-10.0, -8.0, -6.0, -4.0)
        for (deltaExp in deltaExps) {
            val delta = Lscale * FastMath.pow(10.0, deltaExp)
            val rng2 = Random(23000)
            val Lp = Ls.copy()
            val Zp = Zs.copy()
            for (i in 0..<M) {
                for (r in 0..<R) {
                    Lp[i, r] = Lp[i, r] + delta * (1 + rng2.nextDouble())
                }
            }
            for (r in 0..<R) {
                Zp[r] = Zp[r] + delta * (1 + rng2.nextDouble())
            }

            val PrTry = solveAll(Lp, Zp, Dn, N, M, R, sumN, basisSize, numDn)
            if (!PrTry.second) {
                // Check validity
                var allValid = true
                for (i in 0..<M) {
                    for (j in 0..sumN) {
                        if (PrTry.first[i, j] < -1e-6) allValid = false
                    }
                    var rowSum = 0.0
                    for (j in 0..sumN) rowSum += PrTry.first[i, j]
                    if (FastMath.abs(rowSum - 1.0) > 0.01) allValid = false
                }
                if (allValid) {
                    Pr = PrTry
                    break
                }
            }
            Pr = PrTry
        }
    }

    val PrMat = Pr.first
    val Q = Matrix(M, 1)
    for (i in 0..<M) {
        var mean = 0.0
        for (j in 0..sumN) {
            mean += j.toDouble() * PrMat[i, j]
        }
        Q[i, 0] = mean
    }

    return Ret.pfqnProcomom(PrMat, Q)
}

/**
 * Solve for all stations
 */
private fun solveAll(
    Ls: Matrix, Zs: Matrix, Dn: List<IntArray>,
    N: Matrix, M: Int, R: Int, sumN: Int, basisSize: Int, numDn: Int
): Pair<Matrix, Boolean> {
    val Pr = Matrix(M, sumN + 1)
    var rankdef = false

    for (station in 0..<M) {
        // Rotate: swap station with last (M-1)
        val Lrot = Ls.copy()
        for (r in 0..<R) {
            val tmp = Lrot[station, r]
            Lrot[station, r] = Lrot[M - 1, r]
            Lrot[M - 1, r] = tmp
        }

        val result = solveStation(Lrot, Zs, Dn, N, M, R, sumN, basisSize, numDn)
        if (result.second) rankdef = true

        val dist = result.first
        var total = 0.0
        for (v in dist) total += v
        if (FastMath.abs(total) > 0) {
            for (j in 0..sumN) {
                Pr[station, j] = dist[j] / total
            }
        }
    }

    return Pair(Pr, rankdef)
}

/**
 * Solve for a single station (after rotation to position M-1)
 */
private fun solveStation(
    Ls: Matrix, Zs: Matrix, Dn: List<IntArray>,
    N: Matrix, M: Int, R: Int, sumN: Int, basisSize: Int, numDn: Int
): Pair<DoubleArray, Boolean> {
    var rankdef = false

    // pk(:, j+1) holds basis coefficients for queue length n=j
    var pk = Array(basisSize) { DoubleArray(sumN + 1) }

    // Initialize: for empty network, all stations have probability 1 at n=0
    val zeroDn = IntArray(R)
    for (kk in 1..M) {
        val idx = phash(Dn, zeroDn, kk, M)
        if (idx >= 0 && idx < basisSize) {
            pk[idx][0] = 1.0
        }
    }

    val Ncur = IntArray(R)
    for (r in 0..<R) {
        for (Nr in 1..N[r].toInt()) {
            Ncur[r] = Nr
            val pklast = Array(basisSize) { i -> pk[i].clone() }
            pk = Array(basisSize) { DoubleArray(sumN + 1) }

            val matrices = genPMatrix(Ls, Zs, Ncur, r, Dn, M, R, numDn, basisSize)
            val Ag = matrices[0]
            val Bg = matrices[1]
            val DCg = matrices[2]
            val DDg = matrices[3]
            val numRows = Ag.size

            // SVD for rank-revealing decomposition
            val svdResult = computeSVD(Ag, numRows, basisSize)
            val U = svdResult.first
            val sv = svdResult.second
            val V = svdResult.third

            val tol = FastMath.max(numRows, basisSize).toDouble() * FastMath.ulp(1.0) * sv[0]
            var rk = 0
            for (s in sv) {
                if (s > tol) rk++
            }

            val sumNcur = sumArrayP(Ncur)

            if (rk < basisSize) {
                rankdef = true
                // Use minimum-norm solution via truncated SVD
                // pB = Vr * (Si * (Ur' * Bg))
                // pDC = Vr * (Si * (Ur' * DCg))
                // pDD = Vr * (Si * (Ur' * DDg))

                val pB = pseudoInvMult(U, sv, V, rk, Bg, numRows, basisSize)
                val pDC = pseudoInvMult(U, sv, V, rk, DCg, numRows, basisSize)
                val pDD = pseudoInvMult(U, sv, V, rk, DDg, numRows, basisSize)

                // pk(:,1) = pB * pklast(:,1)
                for (i in 0..<basisSize) {
                    var sum = 0.0
                    for (j in 0..<basisSize) {
                        sum += pB[i][j] * pklast[j][0]
                    }
                    pk[i][0] = sum
                }

                for (n in 1..sumNcur) {
                    for (i in 0..<basisSize) {
                        var sum = 0.0
                        for (j in 0..<basisSize) {
                            sum += pB[i][j] * pklast[j][n]
                            sum += n.toDouble() * pDC[i][j] * pk[j][n - 1]  // Use n-1 since pk n is being computed
                            // Wait, the MATLAB says: pk(:,n+1) = pB*pklast(:,n+1) + n*pDC*pk(:,n) + n*pDD*pklast(:,n)
                            // n in MATLAB starts from 1, pk(:,n+1) is the n-th probability
                            // Here index is 0-based in arrays, so pk[i][n] corresponds to pk(:,n+1) in MATLAB
                        }
                        // Fix: need to split the loop since pk[j][n-1] needs to refer to already-computed pk
                        pk[i][n] = sum
                    }
                    // Add pDD * pklast contribution
                    for (i in 0..<basisSize) {
                        var sum = 0.0
                        for (j in 0..<basisSize) {
                            sum += n.toDouble() * pDD[i][j] * pklast[j][n - 1]
                        }
                        pk[i][n] += sum
                    }
                }
            } else {
                // Full rank: use QR for speed and accuracy
                val qr = computeQR(Ag, numRows, basisSize)
                val Q = qr.first
                val Rmat = qr.second

                // QtB = Q' * Bg, etc.
                val QtB = transposeMultiply(Q, Bg, numRows, basisSize)
                val QtDC = transposeMultiply(Q, DCg, numRows, basisSize)
                val QtDD = transposeMultiply(Q, DDg, numRows, basisSize)

                // pk(:,1) = R \ (QtB * pklast(:,1))
                val rhsInit = matVecMult(QtB, pklast, 0, basisSize)
                val solInit = backSubstitute(Rmat, rhsInit, basisSize)
                for (i in 0..<basisSize) pk[i][0] = solInit[i]

                for (n in 1..sumNcur) {
                    val rhs = DoubleArray(basisSize)
                    val pklastVec = matVecMult(QtB, pklast, n, basisSize)
                    val pkPrevVec = matVecMult(QtDC, pk, n - 1, basisSize)
                    val pklastPrevVec = matVecMult(QtDD, pklast, n - 1, basisSize)
                    for (i in 0..<basisSize) {
                        rhs[i] = pklastVec[i] + n.toDouble() * pkPrevVec[i] + n.toDouble() * pklastPrevVec[i]
                    }
                    val sol = backSubstitute(Rmat, rhs, basisSize)
                    for (i in 0..<basisSize) pk[i][n] = sol[i]
                }
            }

            // Rescale to prevent overflow
            var smax = 0.0
            for (i in 0..<basisSize) {
                for (j in 0..sumN) {
                    val v = FastMath.abs(pk[i][j])
                    if (v > smax) smax = v
                }
            }
            if (smax > 0 && java.lang.Double.isFinite(smax)) {
                for (i in 0..<basisSize) {
                    for (j in 0..sumN) {
                        pk[i][j] /= smax
                    }
                }
            }
        }
    }

    // Extract result: first basis element at zero Dn entry
    val zeroDnFinal = IntArray(R)
    val idx = phash(Dn, zeroDnFinal, 1, M)
    val dist = DoubleArray(sumN + 1)
    if (idx >= 0 && idx < basisSize) {
        for (j in 0..sumN) {
            dist[j] = pk[idx][j]
        }
    }

    return Pair(dist, rankdef)
}

/**
 * Generate probability matrices for ProCoMoM recursion.
 * Returns [A, B, DC, DD] matrices.
 */
private fun genPMatrix(
    Ls: Matrix, Zs: Matrix, Ncur: IntArray, r: Int,
    Dn: List<IntArray>, M: Int, R: Int, numDn: Int, basisSize: Int
): Array<Array<DoubleArray>> {
    val numRows = countRows(Dn, Ncur, r, M, R, numDn)
    val A = Array(numRows) { DoubleArray(basisSize) }
    val B = Array(numRows) { DoubleArray(basisSize) }
    val DC = Array(numRows) { DoubleArray(basisSize) }
    val DD = Array(numRows) { DoubleArray(basisSize) }

    var row = 0
    for (d in 0..<numDn) {
        val dn = Dn[d]

        // Check if r <= R-2 and sum(Dn(d, r:R-2)) > 0
        var sumRange = 0
        if (r <= R - 2) {
            for (t in r..<R - 1) sumRange += dn[t]
        }

        if (r <= R - 2 && sumRange > 0) {
            // Branch A: propagation through class boundaries
            for (k in 1..M) {
                if (row >= numRows) break
                val colA = phash(Dn, dn, k, M)
                if (colA >= 0 && colA < basisSize) A[row][colA] = 1.0

                // Check if r+1 <= R-2 and sum(Dn(d, r+1:R-2)) > 0
                var sumRange2 = 0
                if (r + 1 <= R - 2) {
                    for (t in r + 1..<R - 1) sumRange2 += dn[t]
                }

                if (r + 1 <= R - 2 && sumRange2 > 0) {
                    if (colA >= 0 && colA < basisSize) B[row][colA] = 1.0
                } else {
                    val shifted = dn.clone()
                    shifted[r]--
                    val colB = phash(Dn, shifted, k, M)
                    if (colB >= 0 && colB < basisSize) B[row][colB] = 1.0
                }
                row++
            }
        } else {
            // Branch B: CE, PC, and extra PC equations
            var sumDn1ToR = 0
            for (t in 0..r) sumDn1ToR += dn[t]

            if (sumDn1ToR < M) {
                // CE equations: k=1..M-1
                for (k in 1..<M) {
                    if (row >= numRows) break
                    val colKP1 = phash(Dn, dn, k + 1, M)
                    if (colKP1 >= 0 && colKP1 < basisSize) A[row][colKP1] = 1.0
                    val col1 = phash(Dn, dn, 1, M)
                    if (col1 >= 0 && col1 < basisSize) A[row][col1] = -1.0

                    for (s in 0..<r) {
                        val shifted = dn.clone()
                        shifted[s]++ // UP shift
                        val col = phash(Dn, shifted, k + 1, M)
                        if (col >= 0 && col < basisSize) {
                            A[row][col] -= Ls[k - 1, s]
                        }
                    }
                    val colB = phash(Dn, dn, k + 1, M)
                    if (colB >= 0 && colB < basisSize) B[row][colB] = Ls[k - 1, r]
                    row++
                }

                // PC equations: s=0..r-1
                for (s in 0..<r) {
                    if (row >= numRows) break
                    val ndS = Ncur[s] - dn[s]
                    val col1 = phash(Dn, dn, 1, M)
                    if (col1 >= 0 && col1 < basisSize) A[row][col1] = ndS.toDouble()

                    val shifted = dn.clone()
                    shifted[s]++ // UP shift
                    val colBase = phash(Dn, shifted, 1, M)
                    if (colBase >= 0 && colBase < basisSize) {
                        A[row][colBase] -= Zs[s]
                        DC[row][colBase] = Ls[M - 1, s]
                    }
                    for (k in 1..<M) {
                        val colK = phash(Dn, shifted, k + 1, M)
                        if (colK >= 0 && colK < basisSize) {
                            A[row][colK] -= Ls[k - 1, s]
                        }
                    }
                    row++
                }
            }

            // Extra PC for class r (always in Branch B)
            if (row < numRows) {
                val ndR = Ncur[r] - dn[r]
                val col1 = phash(Dn, dn, 1, M)
                if (col1 >= 0 && col1 < basisSize) {
                    A[row][col1] = ndR.toDouble()
                    B[row][col1] = Zs[r]
                }
                for (k in 1..<M) {
                    val colKP1 = phash(Dn, dn, k + 1, M)
                    if (colKP1 >= 0 && colKP1 < basisSize) {
                        B[row][colKP1] = Ls[k - 1, r]
                    }
                }
                DD[row][if (col1 >= 0 && col1 < basisSize) col1 else 0] = Ls[M - 1, r]
                row++
            }
        }
    }

    return arrayOf(A, B, DC, DD)
}

/**
 * Count number of rows in the generated matrix system
 */
private fun countRows(Dn: List<IntArray>, Ncur: IntArray, r: Int, M: Int, R: Int, numDn: Int): Int {
    var numRows = 0
    for (d in 0..<numDn) {
        val dn = Dn[d]
        var sumRange = 0
        if (r <= R - 2) {
            for (t in r..<R - 1) sumRange += dn[t]
        }
        if (r <= R - 2 && sumRange > 0) {
            numRows += M
        } else {
            var sumDn1ToR = 0
            for (t in 0..r) sumDn1ToR += dn[t]
            if (sumDn1ToR < M) {
                numRows += M + r - 1 // (M-1) CE + r PC
            }
            numRows += 1 // extra PC for class r
        }
    }
    return numRows
}

/**
 * Hash function for ProCoMoM: maps (dn, k) to column index.
 * k is 1-based station index.
 */
private fun phash(Dn: List<IntArray>, dn: IntArray, k: Int, M: Int): Int {
    val pos = matchRowP(Dn, dn)
    if (pos < 0) return -1
    return pos * M + k - 1
}

private fun matchRowP(Dn: List<IntArray>, row: IntArray): Int {
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

private fun multichooseP(R: Int, M: Int): MutableList<IntArray> {
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
        val sub = multichooseP(R - 1, M - i)
        for (s in sub) {
            val row = IntArray(R)
            row[0] = i
            System.arraycopy(s, 0, row, 1, R - 1)
            result.add(row)
        }
    }
    return result
}

private fun sortByNnzPosP(list: MutableList<IntArray>): List<IntArray> {
    val result = list.toMutableList()
    for (i in 0..<result.size - 1) {
        for (j in i + 1..<result.size) {
            if (nnzCmpP(result[i], result[j]) == 1) {
                val tmp = result[i]
                result[i] = result[j]
                result[j] = tmp
            }
        }
    }
    return result
}

private fun nnzCmpP(i1: IntArray, i2: IntArray): Int {
    var nnz1 = 0
    var nnz2 = 0
    for (v in i1) if (v != 0) nnz1++
    for (v in i2) if (v != 0) nnz2++
    if (nnz1 > nnz2) return 1
    if (nnz1 < nnz2) return 0
    for (j in i1.indices) {
        if (i1[j] == 0 && i2[j] > 0) return 1
        if (i1[j] > 0 && i2[j] == 0) return 0
    }
    return 0
}

private fun sumArrayP(arr: IntArray): Int {
    var s = 0
    for (v in arr) s += v
    return s
}

/**
 * Compute thin SVD of a matrix A (numRows x numCols)
 * Returns (U, singular values, V)
 */
private fun computeSVD(
    A: Array<DoubleArray>, numRows: Int, numCols: Int
): Triple<Array<DoubleArray>, DoubleArray, Array<DoubleArray>> {
    // Convert to Matrix for SVD computation
    val mat = Matrix(numRows, numCols)
    for (i in 0..<numRows) {
        for (j in 0..<numCols) {
            mat[i, j] = A[i][j]
        }
    }
    val svd = org.ejml.dense.row.factory.DecompositionFactory_DDRM.svd(numRows, numCols, true, true, false)
    val ddrm = org.ejml.data.DMatrixRMaj(numRows, numCols)
    for (i in 0..<numRows) {
        for (j in 0..<numCols) {
            ddrm[i, j] = A[i][j]
        }
    }
    svd.decompose(ddrm)
    val minDim = FastMath.min(numRows, numCols)
    val sv = DoubleArray(minDim)
    val Umat = svd.getU(null, false)
    val Vmat = svd.getV(null, false)
    svd.getSingularValues().copyInto(sv, 0, 0, minDim)

    val U = Array(numRows) { i -> DoubleArray(minDim) { j -> Umat[i, j] } }
    val V = Array(numCols) { i -> DoubleArray(minDim) { j -> Vmat[i, j] } }

    return Triple(U, sv, V)
}

/**
 * Compute Q*R factorization
 */
private fun computeQR(
    A: Array<DoubleArray>, numRows: Int, numCols: Int
): Pair<Array<DoubleArray>, Array<DoubleArray>> {
    val ddrm = org.ejml.data.DMatrixRMaj(numRows, numCols)
    for (i in 0..<numRows) {
        for (j in 0..<numCols) {
            ddrm[i, j] = A[i][j]
        }
    }
    val qr = org.ejml.dense.row.factory.DecompositionFactory_DDRM.qr(numRows, numCols)
    qr.decompose(ddrm)
    val Qmat = qr.getQ(null, true)
    val Rmat = qr.getR(null, true)

    val minDim = FastMath.min(numRows, numCols)
    val Q = Array(numRows) { i -> DoubleArray(minDim) { j -> Qmat[i, j] } }
    val R = Array(minDim) { i -> DoubleArray(numCols) { j -> Rmat[i, j] } }

    return Pair(Q, R)
}

/**
 * Compute pseudoinverse times matrix: V * diag(1/sv) * U' * B
 */
private fun pseudoInvMult(
    U: Array<DoubleArray>, sv: DoubleArray, V: Array<DoubleArray>,
    rk: Int, B: Array<DoubleArray>, numRows: Int, numCols: Int
): Array<DoubleArray> {
    // result = Vr * (Si * (Ur' * B))
    // Ur is numRows x rk, Si is rk x rk, Vr is numCols x rk
    val result = Array(numCols) { DoubleArray(numCols) }

    // Step 1: Ur' * B (rk x numCols)
    val UrTB = Array(rk) { DoubleArray(numCols) }
    for (i in 0..<rk) {
        for (j in 0..<numCols) {
            var sum = 0.0
            for (k in 0..<numRows) {
                sum += U[k][i] * B[k][j]
            }
            UrTB[i][j] = sum
        }
    }

    // Step 2: Si * UrTB (rk x numCols)
    for (i in 0..<rk) {
        for (j in 0..<numCols) {
            UrTB[i][j] /= sv[i]
        }
    }

    // Step 3: Vr * (Si * UrTB) (numCols x numCols)
    for (i in 0..<numCols) {
        for (j in 0..<numCols) {
            var sum = 0.0
            for (k in 0..<rk) {
                sum += V[i][k] * UrTB[k][j]
            }
            result[i][j] = sum
        }
    }

    return result
}

/**
 * Compute Q' * B
 */
private fun transposeMultiply(
    Q: Array<DoubleArray>, B: Array<DoubleArray>, numRows: Int, numCols: Int
): Array<DoubleArray> {
    val result = Array(numCols) { DoubleArray(numCols) }
    for (i in 0..<numCols) {
        for (j in 0..<numCols) {
            var sum = 0.0
            for (k in 0..<numRows) {
                sum += Q[k][i] * B[k][j]
            }
            result[i][j] = sum
        }
    }
    return result
}

/**
 * Compute matrix * vector from column of pk
 */
private fun matVecMult(mat: Array<DoubleArray>, pk: Array<DoubleArray>, col: Int, n: Int): DoubleArray {
    val result = DoubleArray(n)
    for (i in 0..<n) {
        var sum = 0.0
        for (j in 0..<n) {
            sum += mat[i][j] * pk[j][col]
        }
        result[i] = sum
    }
    return result
}

/**
 * Back substitution for upper triangular system R*x = b
 */
private fun backSubstitute(R: Array<DoubleArray>, b: DoubleArray, n: Int): DoubleArray {
    val x = DoubleArray(n)
    for (i in n - 1 downTo 0) {
        var sum = b[i]
        for (j in i + 1..<n) {
            sum -= R[i][j] * x[j]
        }
        if (FastMath.abs(R[i][i]) > 1e-30) {
            x[i] = sum / R[i][i]
        }
    }
    return x
}

/**
 * PFQN procomom algorithms
 */
@Suppress("unused")
class PfqnProcoMomAlgo {
    companion object {
        // Class documentation marker for Dokka
    }
}
