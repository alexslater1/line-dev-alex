/*
 * Copyright (c) 2012-2026, Imperial College London
 * All rights reserved.
 */
package jline.inference.util

import jline.util.matrix.Matrix
import org.apache.commons.math3.optim.*
import org.apache.commons.math3.optim.nonlinear.scalar.GoalType
import org.apache.commons.math3.optim.nonlinear.scalar.ObjectiveFunction
import org.apache.commons.math3.optim.nonlinear.scalar.noderiv.BOBYQAOptimizer

/**
 * Optimization utilities for inference algorithms.
 */
object OptimUtils {

    /**
     * Bound-constrained minimization using BOBYQA (derivative-free).
     * Equivalent to MATLAB's fmincon with box constraints.
     *
     * @param objFun objective function to minimize
     * @param x0 initial point
     * @param lb lower bounds
     * @param ub upper bounds
     * @param maxIter maximum iterations
     * @return pair of (optimal point, objective value)
     */
    @JvmStatic
    fun fmincon(
        objFun: (DoubleArray) -> Double,
        x0: DoubleArray,
        lb: DoubleArray,
        ub: DoubleArray,
        maxIter: Int = 10000
    ): Pair<DoubleArray, Double> {
        val n = x0.size
        val interpPoints = 2 * n + 1

        // Ensure x0 is within bounds
        val x0Safe = DoubleArray(n) { i ->
            Math.max(lb[i] + 1e-10, Math.min(ub[i] - 1e-10, x0[i]))
        }

        // Compute initial trust region radius
        var initialRadius = 0.0
        for (i in 0 until n) {
            val range = ub[i] - lb[i]
            if (range.isFinite()) {
                initialRadius = Math.max(initialRadius, range * 0.1)
            } else {
                initialRadius = Math.max(initialRadius, Math.abs(x0Safe[i]) * 0.1 + 1.0)
            }
        }
        if (initialRadius < 1e-8) initialRadius = 1.0

        val optimizer = BOBYQAOptimizer(interpPoints, initialRadius, 1e-8)

        return try {
            val result = optimizer.optimize(
                MaxEval(maxIter * 10),
                ObjectiveFunction { point -> objFun(point) },
                GoalType.MINIMIZE,
                SimpleBounds(lb, ub),
                InitialGuess(x0Safe)
            )
            Pair(result.point, result.value)
        } catch (e: Exception) {
            // If BOBYQA fails, try coordinate descent
            coordinateDescent(objFun, x0Safe, lb, ub, maxIter)
        }
    }

    /**
     * Simple coordinate descent fallback optimizer.
     */
    private fun coordinateDescent(
        objFun: (DoubleArray) -> Double,
        x0: DoubleArray,
        lb: DoubleArray,
        ub: DoubleArray,
        maxIter: Int
    ): Pair<DoubleArray, Double> {
        val n = x0.size
        val x = x0.copyOf()
        var fBest = objFun(x)

        for (iter in 0 until maxIter) {
            var improved = false
            for (i in 0 until n) {
                val delta = Math.max(1e-6, Math.abs(x[i]) * 0.01)
                for (sign in intArrayOf(-1, 1)) {
                    val xTrial = x.copyOf()
                    xTrial[i] = Math.max(lb[i], Math.min(ub[i], x[i] + sign * delta))
                    val fTrial = objFun(xTrial)
                    if (fTrial < fBest) {
                        x[i] = xTrial[i]
                        fBest = fTrial
                        improved = true
                    }
                }
            }
            if (!improved) break
        }
        return Pair(x, fBest)
    }

    /**
     * Quadratic program solver with non-negative constraints.
     * Solves: min 0.5 x'Hx + f'x  subject to x >= lb
     *
     * Uses Cholesky decomposition to convert to NNLS problem.
     *
     * @param H positive definite Hessian matrix (n x n)
     * @param f linear term (n x 1)
     * @param lb lower bounds (n x 1), typically zeros
     * @return solution vector (n x 1)
     */
    @JvmStatic
    fun quadprog(H: Matrix, f: Matrix, lb: Matrix): Pair<Matrix, Double> {
        val n = H.numRows

        // Shift to handle non-zero lower bounds: y = x - lb
        val fShifted = Matrix(n, 1)
        for (i in 0 until n) {
            var sum = f.get(i, 0)
            for (j in 0 until n) {
                sum += H.get(i, j) * lb.get(j, 0)
            }
            fShifted.set(i, 0, sum)
        }

        // Cholesky decomposition H = L L'
        val L = choleskyDecompose(H)

        if (L != null) {
            // Convert QP to NNLS: min ||L'y - b||^2 s.t. y >= 0, where b = -L^{-1}f
            val LInvF = forwardSolve(L, fShifted)
            val rhs = Matrix(n, 1)
            for (i in 0 until n) {
                rhs.set(i, 0, -LInvF.get(i, 0))
            }
            // Use L transpose (H = LL', objective = 0.5||L'y + L^{-1}f||^2)
            val Lt = Matrix(n, n)
            for (i in 0 until n) {
                for (j in 0 until n) {
                    Lt.set(i, j, L.get(j, i))
                }
            }
            val y = NnlsSolver.lsqnonneg(Lt, rhs)

            // x = y + lb
            val x = Matrix(n, 1)
            for (i in 0 until n) {
                x.set(i, 0, y.get(i, 0) + lb.get(i, 0))
            }

            // Compute objective value
            var objVal = 0.0
            for (i in 0 until n) {
                objVal += f.get(i, 0) * x.get(i, 0)
                for (j in 0 until n) {
                    objVal += 0.5 * x.get(i, 0) * H.get(i, j) * x.get(j, 0)
                }
            }
            return Pair(x, objVal)
        } else {
            // Fallback: use projected gradient descent
            return projectedGradientQP(H, f, lb, n)
        }
    }

    private fun projectedGradientQP(H: Matrix, f: Matrix, lb: Matrix, n: Int): Pair<Matrix, Double> {
        val x = Matrix(n, 1)
        for (i in 0 until n) {
            x.set(i, 0, lb.get(i, 0) + 1.0)
        }

        val maxIter = 1000
        var stepSize = 0.001

        for (iter in 0 until maxIter) {
            // gradient = Hx + f
            val grad = Matrix(n, 1)
            for (i in 0 until n) {
                var sum = f.get(i, 0)
                for (j in 0 until n) {
                    sum += H.get(i, j) * x.get(j, 0)
                }
                grad.set(i, 0, sum)
            }

            // Project: x = max(lb, x - step * grad)
            var changed = false
            for (i in 0 until n) {
                val newVal = Math.max(lb.get(i, 0), x.get(i, 0) - stepSize * grad.get(i, 0))
                if (Math.abs(newVal - x.get(i, 0)) > 1e-12) changed = true
                x.set(i, 0, newVal)
            }
            if (!changed) break
        }

        var objVal = 0.0
        for (i in 0 until n) {
            objVal += f.get(i, 0) * x.get(i, 0)
            for (j in 0 until n) {
                objVal += 0.5 * x.get(i, 0) * H.get(i, j) * x.get(j, 0)
            }
        }
        return Pair(x, objVal)
    }

    private fun choleskyDecompose(A: Matrix): Matrix? {
        val n = A.numRows
        val L = Matrix(n, n)

        for (i in 0 until n) {
            for (j in 0..i) {
                var sum = 0.0
                for (k in 0 until j) {
                    sum += L.get(i, k) * L.get(j, k)
                }
                if (i == j) {
                    val diag = A.get(i, i) - sum
                    if (diag <= 0) return null // Not positive definite
                    L.set(i, j, Math.sqrt(diag))
                } else {
                    val lJJ = L.get(j, j)
                    if (Math.abs(lJJ) < 1e-14) return null
                    L.set(i, j, (A.get(i, j) - sum) / lJJ)
                }
            }
        }
        return L
    }

    private fun forwardSolve(L: Matrix, b: Matrix): Matrix {
        val n = L.numRows
        val x = Matrix(n, 1)
        for (i in 0 until n) {
            var sum = b.get(i, 0)
            for (j in 0 until i) {
                sum -= L.get(i, j) * x.get(j, 0)
            }
            x.set(i, 0, sum / L.get(i, i))
        }
        return x
    }
}
