/**
 * NLP Solver Utility for Linearly-Constrained Nonlinear Optimization.
 *
 * Implements an Augmented Lagrangian Penalty Method for solving NLP problems
 * with linear equality/inequality constraints and variable bounds.
 *
 * Used by QRF NLP approximation methods (MMI, MEM).
 *
 * @since LINE 3.0
 */
package jline.api.mapqn

import org.apache.commons.math3.optim.InitialGuess
import org.apache.commons.math3.optim.MaxEval
import org.apache.commons.math3.optim.SimpleBounds
import org.apache.commons.math3.optim.nonlinear.scalar.GoalType
import org.apache.commons.math3.optim.nonlinear.scalar.ObjectiveFunction
import org.apache.commons.math3.optim.nonlinear.scalar.noderiv.BOBYQAOptimizer
import org.apache.commons.math3.analysis.MultivariateFunction
import kotlin.math.max
import kotlin.math.min
import kotlin.math.sqrt

object Mapqn_nlp_solver {

    /**
     * Solve a linearly-constrained NLP problem using Augmented Lagrangian + BOBYQA.
     *
     * Minimizes objective(x) subject to:
     *   Aeq * x = beq  (equality constraints)
     *   Aub * x <= bub  (inequality constraints)
     *   lb <= x <= ub   (variable bounds)
     *
     * @param objective The nonlinear objective function to minimize
     * @param nVars Number of decision variables
     * @param Aeq Equality constraint matrix (numEq x nVars), null if none
     * @param beq Equality RHS vector (numEq), null if none
     * @param Aub Inequality constraint matrix (numIneq x nVars), null if none
     * @param bub Inequality RHS vector (numIneq), null if none
     * @param lb Lower bounds on variables
     * @param ub Upper bounds on variables
     * @param x0 Initial point
     * @param maxIter Maximum outer iterations for augmented Lagrangian
     * @param maxEval Maximum function evaluations per BOBYQA call
     * @return Optimal x vector
     */
    @JvmStatic
    @JvmOverloads
    fun solve(
        objective: (DoubleArray) -> Double,
        nVars: Int,
        Aeq: Array<DoubleArray>? = null,
        beq: DoubleArray? = null,
        Aub: Array<DoubleArray>? = null,
        bub: DoubleArray? = null,
        lb: DoubleArray,
        ub: DoubleArray,
        x0: DoubleArray,
        maxIter: Int = 20,
        maxEval: Int = 50000
    ): DoubleArray {
        val numEq = Aeq?.size ?: 0
        val numIneq = Aub?.size ?: 0

        // Lagrange multipliers
        val lambdaEq = DoubleArray(numEq) { 0.0 }
        val lambdaIneq = DoubleArray(numIneq) { 0.0 }
        var rho = 1.0
        val rhoMax = 1e6
        val rhoMult = 10.0

        var xCurrent = x0.copyOf()

        for (iter in 0 until maxIter) {
            // Create augmented Lagrangian objective
            val augLagrangian = MultivariateFunction { x ->
                var fval = objective(x)

                // Equality constraints: L += lambda_i * h_i + (rho/2) * h_i^2
                if (Aeq != null && beq != null) {
                    for (i in 0 until numEq) {
                        var hi = -beq[i]
                        for (j in 0 until nVars) {
                            hi += Aeq[i][j] * x[j]
                        }
                        fval += lambdaEq[i] * hi + (rho / 2.0) * hi * hi
                    }
                }

                // Inequality constraints: L += (rho/2) * max(0, lambda_i/rho + g_i)^2
                if (Aub != null && bub != null) {
                    for (i in 0 until numIneq) {
                        var gi = -bub[i]
                        for (j in 0 until nVars) {
                            gi += Aub[i][j] * x[j]
                        }
                        val shifted = max(0.0, lambdaIneq[i] / rho + gi)
                        fval += (rho / 2.0) * shifted * shifted
                    }
                }

                fval
            }

            // BOBYQA requires interpolation points: n+2 <= npt <= (n+1)(n+2)/2
            val npt = min(2 * nVars + 1, (nVars + 1) * (nVars + 2) / 2)
            val initialRadius = 0.1
            val stoppingRadius = 1e-8

            try {
                val optimizer = BOBYQAOptimizer(npt, initialRadius, stoppingRadius)
                val result = optimizer.optimize(
                    MaxEval(maxEval),
                    ObjectiveFunction(augLagrangian),
                    GoalType.MINIMIZE,
                    SimpleBounds(lb, ub),
                    InitialGuess(xCurrent)
                )
                xCurrent = result.point
            } catch (e: Exception) {
                // If BOBYQA fails, keep current point and continue
            }

            // Update Lagrange multipliers
            if (Aeq != null && beq != null) {
                for (i in 0 until numEq) {
                    var hi = -beq[i]
                    for (j in 0 until nVars) {
                        hi += Aeq[i][j] * xCurrent[j]
                    }
                    lambdaEq[i] += rho * hi
                }
            }

            if (Aub != null && bub != null) {
                for (i in 0 until numIneq) {
                    var gi = -bub[i]
                    for (j in 0 until nVars) {
                        gi += Aub[i][j] * xCurrent[j]
                    }
                    lambdaIneq[i] = max(0.0, lambdaIneq[i] + rho * gi)
                }
            }

            // Check convergence
            var maxViolation = 0.0
            if (Aeq != null && beq != null) {
                for (i in 0 until numEq) {
                    var hi = -beq[i]
                    for (j in 0 until nVars) {
                        hi += Aeq[i][j] * xCurrent[j]
                    }
                    maxViolation = max(maxViolation, Math.abs(hi))
                }
            }
            if (Aub != null && bub != null) {
                for (i in 0 until numIneq) {
                    var gi = -bub[i]
                    for (j in 0 until nVars) {
                        gi += Aub[i][j] * xCurrent[j]
                    }
                    maxViolation = max(maxViolation, max(0.0, gi))
                }
            }

            if (maxViolation < 1e-6) break

            // Increase penalty
            rho = min(rho * rhoMult, rhoMax)
        }

        return xCurrent
    }
}
