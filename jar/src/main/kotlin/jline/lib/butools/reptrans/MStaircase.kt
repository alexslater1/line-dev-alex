/*
 * Copyright (c) 2012-2026, QORE Lab, Imperial College London
 * All rights reserved.
 * Ported from BUTools V2.0
 *
 * Reference:
 * P. Buchholz, M. Telek, "On minimal representation of rational arrival
 * processes." Madrid Conference on Queueing theory (MCQT), June 2010.
 */
package jline.lib.butools.reptrans

import jline.util.matrix.Matrix
import org.apache.commons.math3.linear.MatrixUtils
import org.apache.commons.math3.linear.SingularValueDecomposition
import kotlin.math.abs
import kotlin.math.min

/**
 * Computes a smaller representation using the staircase algorithm.
 *
 * This function should not be called directly. It is used by
 * 'MinimalRepFromME' and 'MinimalRepFromRAP'.
 *
 * @param Y List of matrices (the representation matrices)
 * @param Z The vector used in the staircase procedure (shape m x 1 or m x k)
 * @param precision Numerical precision for rank and norm tests. The default value is 1e-12.
 * @return A Pair of (B, n) where B is the transformation matrix (shape m x m)
 *         and n is the minimal order found.
 */
fun mStaircase(Y: List<Matrix>, Z: Matrix, precision: Double = 1e-12): Pair<Matrix, Int> {
    val MCount = Y.size
    val m = Y[0].numRows

    // Make copies so we don't modify the originals
    val X = ArrayList<Matrix>(MCount)
    for (y in Y) {
        X.add(y.copy())
    }
    var Zwork = Z.copy()

    var U = Matrix.eye(m)
    var ranksum = 0
    var crit = true

    while (crit) {
        val r = matrixRank(Zwork, precision)
        ranksum += r

        // Full SVD of Zwork: Ui is m_current x m_current orthogonal matrix
        val Ui = fullSvdU(Zwork)
        val mCurrent = Ui.numRows

        // Build transformation: Transf = eye(m)
        // with Transf[-mCurrent:, -mCurrent:] = Ui^T
        // Since ranksum - r + mCurrent == m always, Transf is m x m
        val Transf = Matrix.eye(m)
        val offset = m - mCurrent
        for (i in 0 until mCurrent) {
            for (j in 0 until mCurrent) {
                Transf[offset + i, offset + j] = Ui[j, i]  // Ui^T
            }
        }

        // U = (Transf * U^T)^T
        // U_new[i,j] = sum_k( Transf[j,k] * U_old[i,k] )
        val Uold = U.copy()
        val newU = Matrix(m, m)
        for (i in 0 until m) {
            for (j in 0 until m) {
                var sum = 0.0
                for (k in 0 until m) {
                    sum += Uold[i, k] * Transf[j, k]
                }
                newU[i, j] = sum
            }
        }
        U = newU

        // Transform X matrices and build new Z
        var newZ: Matrix? = null
        for (i in 0 until MCount) {
            // TEMP = Ui^T * X[i] * Ui
            val TEMP = Ui.transpose().mult(X[i]).mult(Ui)
            val currentSize = TEMP.numRows

            // X[i] = TEMP[r:, r:]
            val newSize = currentSize - r
            val newXi = Matrix(newSize, newSize)
            for (ri in 0 until newSize) {
                for (ci in 0 until newSize) {
                    newXi[ri, ci] = TEMP[r + ri, r + ci]
                }
            }
            X[i] = newXi

            // Build new Z from TEMP[r:, 0:r] blocks
            val block = Matrix(newSize, r)
            for (ri in 0 until newSize) {
                for (ci in 0 until r) {
                    block[ri, ci] = TEMP[r + ri, ci]
                }
            }

            if (i == 0) {
                newZ = block
            } else {
                // Horizontal concatenation: [newZ, block]
                val oldCols = newZ!!.numCols
                val combined = Matrix(newSize, oldCols + r)
                for (ri in 0 until newSize) {
                    for (ci in 0 until oldCols) {
                        combined[ri, ci] = newZ[ri, ci]
                    }
                    for (ci in 0 until r) {
                        combined[ri, oldCols + ci] = block[ri, ci]
                    }
                }
                newZ = combined
            }
        }
        Zwork = newZ!!

        // Check stopping criteria
        if (Zwork.norm() < precision || matrixRank(Zwork, precision) == m - ranksum) {
            crit = false
        }
    }

    val n = ranksum

    if (Zwork.norm() < precision) {
        // Compute x = sum(U^T, axis=1)[0:n]
        // In Python: x = np.sum(U.T, 1)[0:n]
        // sum(U^T, axis=1) means row sums of U^T = column sums of U
        val xFull = DoubleArray(m)
        for (j in 0 until m) {
            var colSum = 0.0
            for (i in 0 until m) {
                colSum += U[i, j]
            }
            xFull[j] = colSum
        }

        // Check if x has zero values
        var hasZero = false
        val zeroloc = BooleanArray(n)
        var nonzeroIdx = -1
        for (l in 0 until n) {
            if (abs(xFull[l]) < precision) {
                hasZero = true
                zeroloc[l] = true
            } else if (nonzeroIdx == -1) {
                nonzeroIdx = l
            }
        }

        // R = eye(n), with R[l, nonzero] = 1 for zero locations
        val R = Matrix.eye(n)
        if (hasZero && nonzeroIdx >= 0) {
            for (l in 0 until n) {
                if (zeroloc[l]) {
                    R[l, nonzeroIdx] = 1.0
                }
            }
        }

        // y = R * x[0:n]
        val xVec = Matrix(n, 1)
        for (i in 0 until n) {
            xVec[i, 0] = xFull[i]
        }
        val yVec = R.mult(xVec)

        // Gamma = diag(y)
        val gammaVals = DoubleArray(n)
        for (i in 0 until n) {
            gammaVals[i] = yVec[i, 0]
        }
        val Gamma = Matrix.diag(*gammaVals)

        // TEMP1 = eye(m), TEMP1[0:n, 0:n] = inv(Gamma)
        val TEMP1 = Matrix.eye(m)
        val GammaInv = Gamma.inv()
        for (i in 0 until n) {
            for (j in 0 until n) {
                TEMP1[i, j] = GammaInv[i, j]
            }
        }

        // TEMP2 = eye(m), TEMP2[0:n, 0:n] = R
        val TEMP2 = Matrix.eye(m)
        for (i in 0 until n) {
            for (j in 0 until n) {
                TEMP2[i, j] = R[i, j]
            }
        }

        // B = inv(TEMP1 * TEMP2 * U^T)
        val UT = U.transpose()
        val B = TEMP1.mult(TEMP2).mult(UT).inv()

        return Pair(B, n)
    } else {
        return Pair(Matrix.eye(m), m)
    }
}

/**
 * Computes the rank of a matrix using SVD with a specified tolerance,
 * matching numpy's matrix_rank(Z, tol=precision) behavior.
 */
private fun matrixRank(A: Matrix, tol: Double): Int {
    val rows = A.numRows
    val cols = A.numCols
    if (rows == 0 || cols == 0) return 0

    val realMatrix = MatrixUtils.createRealMatrix(A.toArray2D())
    val svd = SingularValueDecomposition(realMatrix)
    val singularValues = svd.singularValues
    var rank = 0
    for (sv in singularValues) {
        if (sv > tol) {
            rank++
        }
    }
    return rank
}

/**
 * Computes the full U matrix from SVD of matrix A.
 * numpy.linalg.svd returns U of shape (m, m) (full_matrices=True by default),
 * but Apache Commons Math3 returns compact U of shape (m, min(m, n)).
 * This function extends the compact U to a full m x m orthogonal matrix.
 */
private fun fullSvdU(A: Matrix): Matrix {
    val rows = A.numRows
    val cols = A.numCols
    val p = min(rows, cols)

    val realMatrix = MatrixUtils.createRealMatrix(A.toArray2D())
    val svd = SingularValueDecomposition(realMatrix)
    val uCompact = svd.u  // m x p

    if (p >= rows) {
        // Already full, just convert
        val result = Matrix(rows, rows)
        for (i in 0 until rows) {
            for (j in 0 until rows) {
                result[i, j] = uCompact.getEntry(i, j)
            }
        }
        return result
    }

    // Need to extend from m x p to m x m
    // Strategy: create [U_compact | e_1 ... e_m] and do Gram-Schmidt
    // to find m - p additional orthonormal columns in the null space of A^T
    val result = Matrix(rows, rows)

    // Copy compact U columns
    for (i in 0 until rows) {
        for (j in 0 until p) {
            result[i, j] = uCompact.getEntry(i, j)
        }
    }

    // Use modified Gram-Schmidt to add orthonormal columns from standard basis
    var colIdx = p
    for (candidate in 0 until rows) {
        if (colIdx >= rows) break

        // Start with standard basis vector e_candidate
        val v = DoubleArray(rows)
        v[candidate] = 1.0

        // Subtract projections onto all existing columns
        for (j in 0 until colIdx) {
            var dot = 0.0
            for (i in 0 until rows) {
                dot += v[i] * result[i, j]
            }
            for (i in 0 until rows) {
                v[i] -= dot * result[i, j]
            }
        }

        // Check if remaining vector has significant norm
        var norm = 0.0
        for (i in 0 until rows) {
            norm += v[i] * v[i]
        }
        norm = Math.sqrt(norm)

        if (norm > 1e-14) {
            // Normalize and add as new column
            for (i in 0 until rows) {
                result[i, colIdx] = v[i] / norm
            }
            colIdx++
        }
    }

    return result
}
