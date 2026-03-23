/*
 * Copyright (c) 2012-2026, Imperial College London
 * All rights reserved.
 */
package jline.inference.util

import jline.util.matrix.Matrix

/**
 * Non-negative least squares solver implementing the Lawson-Hanson algorithm.
 *
 * Solves: min ||Ax - b||^2  subject to x >= 0
 */
object NnlsSolver {

    /**
     * Solve the NNLS problem: min ||Ax - b||^2 subject to x >= 0
     *
     * @param A coefficient matrix (m x n)
     * @param b right-hand side vector (m x 1)
     * @return solution vector x (n x 1)
     */
    @JvmStatic
    fun lsqnonneg(A: Matrix, b: Matrix): Matrix {
        val m = A.numRows
        val n = A.numCols
        val x = Matrix(n, 1)

        // Passive and active sets
        val passive = BooleanArray(n) { false }
        val maxIter = 3 * n

        // Compute initial gradient w = A'(b - Ax)
        var w = computeGradient(A, b, x, m, n)

        var iter = 0
        while (iter < maxIter) {
            // Find index of largest w among active (non-passive) variables
            var tMax = -1
            var wMax = 0.0
            for (j in 0 until n) {
                if (!passive[j] && w.get(j, 0) > wMax) {
                    wMax = w.get(j, 0)
                    tMax = j
                }
            }

            // If no positive gradient found, we're done
            if (tMax < 0 || wMax <= 1e-12) break

            // Move variable tMax to passive set
            passive[tMax] = true

            // Solve unconstrained LS on passive set
            var innerIter = 0
            while (innerIter < maxIter) {
                val passiveIdx = (0 until n).filter { passive[it] }
                if (passiveIdx.isEmpty()) break

                val Ap = extractColumns(A, passiveIdx, m)
                val sp = solveLeastSquares(Ap, b)

                // Check if all passive variables are positive
                var allPositive = true
                for (i in passiveIdx.indices) {
                    if (sp.get(i, 0) <= 0.0) {
                        allPositive = false
                        break
                    }
                }

                if (allPositive) {
                    for (i in passiveIdx.indices) {
                        x.set(passiveIdx[i], 0, sp.get(i, 0))
                    }
                    break
                }

                // Find alpha: min ratio x[j]/(x[j]-s[j]) for s[j] <= 0
                var alpha = Double.MAX_VALUE
                for (i in passiveIdx.indices) {
                    if (sp.get(i, 0) <= 0.0) {
                        val ratio = x.get(passiveIdx[i], 0) /
                                (x.get(passiveIdx[i], 0) - sp.get(i, 0))
                        if (ratio < alpha) {
                            alpha = ratio
                        }
                    }
                }

                // Update x = x + alpha * (s - x)
                for (i in passiveIdx.indices) {
                    val j = passiveIdx[i]
                    x.set(j, 0, x.get(j, 0) + alpha * (sp.get(i, 0) - x.get(j, 0)))
                }

                // Move variables with zero x back to active set
                for (i in passiveIdx.indices) {
                    if (Math.abs(x.get(passiveIdx[i], 0)) < 1e-12) {
                        passive[passiveIdx[i]] = false
                        x.set(passiveIdx[i], 0, 0.0)
                    }
                }

                innerIter++
            }

            w = computeGradient(A, b, x, m, n)
            iter++
        }

        return x
    }

    /**
     * Solve NNLS with double array inputs: min ||Ax - b||^2 subject to x >= 0
     *
     * @param A coefficient matrix as 2D double array [m][n]
     * @param b right-hand side as 1D double array [m]
     * @return solution as 1D double array [n]
     */
    @JvmStatic
    fun lsqnonneg(A: Array<DoubleArray>, b: DoubleArray): DoubleArray {
        val m = A.size
        val n = if (m > 0) A[0].size else 0
        val matA = Matrix(m, n)
        val vecB = Matrix(m, 1)
        for (i in 0 until m) {
            vecB.set(i, 0, b[i])
            for (j in 0 until n) {
                matA.set(i, j, A[i][j])
            }
        }
        val result = lsqnonneg(matA, vecB)
        return DoubleArray(n) { result.get(it, 0) }
    }

    private fun computeGradient(A: Matrix, b: Matrix, x: Matrix, m: Int, n: Int): Matrix {
        // w = A'(b - Ax)
        val residual = Matrix(m, 1)
        for (i in 0 until m) {
            var ax = 0.0
            for (j in 0 until n) {
                ax += A.get(i, j) * x.get(j, 0)
            }
            residual.set(i, 0, b.get(i, 0) - ax)
        }
        val w = Matrix(n, 1)
        for (j in 0 until n) {
            var sum = 0.0
            for (i in 0 until m) {
                sum += A.get(i, j) * residual.get(i, 0)
            }
            w.set(j, 0, sum)
        }
        return w
    }

    private fun extractColumns(A: Matrix, cols: List<Int>, m: Int): Matrix {
        val result = Matrix(m, cols.size)
        for (i in 0 until m) {
            for (j in cols.indices) {
                result.set(i, j, A.get(i, cols[j]))
            }
        }
        return result
    }

    private fun solveLeastSquares(A: Matrix, b: Matrix): Matrix {
        // Solve A'Ax = A'b using Cholesky or direct method
        val m = A.numRows
        val n = A.numCols
        val AtA = Matrix(n, n)
        val Atb = Matrix(n, 1)

        for (i in 0 until n) {
            for (j in 0 until n) {
                var sum = 0.0
                for (k in 0 until m) {
                    sum += A.get(k, i) * A.get(k, j)
                }
                AtA.set(i, j, sum)
            }
            var sum = 0.0
            for (k in 0 until m) {
                sum += A.get(k, i) * b.get(k, 0)
            }
            Atb.set(i, 0, sum)
        }

        // Solve using Gaussian elimination with partial pivoting
        return solveLinear(AtA, Atb)
    }

    private fun solveLinear(A: Matrix, b: Matrix): Matrix {
        val n = A.numRows
        val aug = Array(n) { i -> DoubleArray(n + 1) { j ->
            if (j < n) A.get(i, j) else b.get(i, 0)
        }}

        // Forward elimination with partial pivoting
        for (col in 0 until n) {
            var maxRow = col
            var maxVal = Math.abs(aug[col][col])
            for (row in col + 1 until n) {
                if (Math.abs(aug[row][col]) > maxVal) {
                    maxVal = Math.abs(aug[row][col])
                    maxRow = row
                }
            }
            val temp = aug[col]
            aug[col] = aug[maxRow]
            aug[maxRow] = temp

            val pivot = aug[col][col]
            if (Math.abs(pivot) < 1e-14) continue

            for (row in col + 1 until n) {
                val factor = aug[row][col] / pivot
                for (j in col until n + 1) {
                    aug[row][j] -= factor * aug[col][j]
                }
            }
        }

        // Back substitution
        val x = Matrix(n, 1)
        for (i in n - 1 downTo 0) {
            var sum = aug[i][n]
            for (j in i + 1 until n) {
                sum -= aug[i][j] * x.get(j, 0)
            }
            if (Math.abs(aug[i][i]) > 1e-14) {
                x.set(i, 0, sum / aug[i][i])
            }
        }
        return x
    }
}
