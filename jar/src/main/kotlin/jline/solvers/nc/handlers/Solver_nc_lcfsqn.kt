/*
 * Copyright (c) 2012-2026, QORE Lab, Imperial College London
 * All rights reserved.
 */

package jline.solvers.nc.handlers

import jline.api.pfqn.lcfs.pfqn_lcfsqn_ca
import jline.lang.NetworkStruct
import jline.lib.perm.Permanent
import jline.solvers.SolverOptions
import jline.solvers.nc.SolverNC
import jline.util.matrix.Matrix
import kotlin.math.ln
import kotlin.math.pow

/**
 * Specialized NC solver for LCFS + LCFS-PR 2-station networks.
 *
 * This function wraps the pfqn_lcfsqn_ca algorithm and computes performance
 * metrics using the convolution approach with permanent calculations.
 *
 * @param sn Network structure
 * @param options Solver options
 * @param lcfsStat Index of the LCFS station
 * @param lcfsprStat Index of the LCFS-PR station
 * @return SolverNCReturn containing performance metrics
 */
fun solver_nc_lcfsqn(
    sn: NetworkStruct,
    options: SolverOptions,
    lcfsStat: Int,
    lcfsprStat: Int
): SolverNC.SolverNCReturn {
    val startTime = System.nanoTime()
    val M = sn.nstations
    val R = sn.nclasses
    val njobs = sn.njobs

    // Extract service times for each class at each station
    // alpha(r) = mean service time at LCFS station for class r
    // beta(r) = mean service time at LCFS-PR station for class r
    val alpha = Matrix(1, R)
    val beta = Matrix(1, R)

    val rates = sn.rates
    for (r in 0 until R) {
        if (njobs.get(r) > 0) {
            val mu_lcfs = rates.get(lcfsStat, r)
            val mu_lcfspr = rates.get(lcfsprStat, r)

            if (mu_lcfs <= 0 || !mu_lcfs.isFinite()) {
                throw RuntimeException("Invalid service rate at LCFS station for class $r.")
            }
            if (mu_lcfspr <= 0 || !mu_lcfspr.isFinite()) {
                throw RuntimeException("Invalid service rate at LCFS-PR station for class $r.")
            }

            alpha.set(r, 1.0 / mu_lcfs)
            beta.set(r, 1.0 / mu_lcfspr)
        }
    }

    // Get population vector
    val N = njobs
    val K = N.elementSum().toInt()

    // Call the LCFS convolution algorithm to get normalizing constant
    val caResult = pfqn_lcfsqn_ca(alpha, beta, N)
    val G = caResult.G

    // Compute log normalizing constant
    val lG = if (G > 0) ln(G) else Double.NEGATIVE_INFINITY

    // Initialize output matrices for the 2-station LCFS network
    val Q_lcfs = Matrix(2, R)
    val U_lcfs = Matrix(2, R)
    val T_lcfs = Matrix(2, R)

    // Compute throughputs and queue lengths for each class using permanent calculations
    // Based on the approach in MATLAB solver_nc_lcfsqn.m
    for (r in 0 until R) {
        if (njobs.get(r) > 0) {
            // Compute throughput and queue length using permanent calculations
            for (xt in 1..K) {
                // Throughput computation
                val Tx = makeTx(alpha, beta, xt, K, R, r)
                val permT = computePermanent(Tx, N, r)
                val tContrib = alpha.get(r).pow(xt - 1) * permT / G
                T_lcfs.set(0, r, T_lcfs.get(0, r) + tContrib)
                T_lcfs.set(1, r, T_lcfs.get(1, r) + tContrib)

                // Queue length at station 1 (LCFS)
                val Yx = makeYx(alpha, beta, xt, K, R, r)
                val permY = computePermanentFull(Yx, N)
                Q_lcfs.set(0, r, Q_lcfs.get(0, r) + permY / G)
            }
            // Queue length at station 2 (LCFS-PR) by conservation
            Q_lcfs.set(1, r, njobs.get(r) - Q_lcfs.get(0, r))
        }
    }

    // Compute utilizations
    for (r in 0 until R) {
        U_lcfs.set(0, r, T_lcfs.get(0, r) * alpha.get(r))
        U_lcfs.set(1, r, T_lcfs.get(1, r) * beta.get(r))
    }

    // Map results back to LINE format
    val Q = Matrix(M, R)
    val U = Matrix(M, R)
    val T = Matrix(M, R)
    val R_resp = Matrix(M, R)
    val X = Matrix(1, R)
    val C = Matrix(1, R)

    // Map queue lengths
    for (r in 0 until R) {
        Q.set(lcfsStat, r, Q_lcfs.get(0, r))
        Q.set(lcfsprStat, r, Q_lcfs.get(1, r))
    }

    // Map utilizations
    for (r in 0 until R) {
        U.set(lcfsStat, r, U_lcfs.get(0, r))
        U.set(lcfsprStat, r, U_lcfs.get(1, r))
    }

    // Throughput is the same at all stations in a closed network
    for (r in 0 until R) {
        if (njobs.get(r) > 0) {
            X.set(r, T_lcfs.get(0, r))
            T.set(lcfsStat, r, T_lcfs.get(0, r))
            T.set(lcfsprStat, r, T_lcfs.get(1, r))
        }
    }

    // Compute response times: R = Q / T (using Little's Law)
    for (k in listOf(lcfsStat, lcfsprStat)) {
        for (r in 0 until R) {
            if (T.get(k, r) > 0) {
                R_resp.set(k, r, Q.get(k, r) / T.get(k, r))
            }
        }
    }

    // Compute cycle times: C = sum of response times at all stations
    for (r in 0 until R) {
        if (njobs.get(r) > 0) {
            C.set(r, R_resp.get(lcfsStat, r) + R_resp.get(lcfsprStat, r))
        }
    }

    val runtime = (System.nanoTime() - startTime) / 1_000_000_000.0

    return SolverNC.SolverNCReturn(
        Q, U, R_resp, T, sn.nchains, X, lG, null, 1, runtime, "lcfsqn"
    )
}

/**
 * Make Tx matrix for throughput computation.
 * This creates a (K-1) x (K-1) matrix excluding class r.
 *
 * @param alpha service times at LCFS station
 * @param beta service times at LCFS-PR station
 * @param xt current state
 * @param K total population
 * @param R number of classes
 * @param r class index to exclude
 */
private fun makeTx(alpha: Matrix, beta: Matrix, xt: Int, K: Int, R: Int, r: Int): Matrix {
    val Tx = Matrix(K - 1, K - 1)

    var idx = 0
    for (i in 0 until R) {
        if (i != r) {
            // Columns 0 to xt-2: alpha(i)^(j+1) for j=0..xt-2
            for (j in 0 until (xt - 1)) {
                Tx.set(idx, j, alpha.get(i).pow(j + 1))
            }
            // Columns xt-1 to K-2: alpha(i)^(xt+j) * beta(i) for j=0..K-xt-1
            for (j in 0 until (K - xt)) {
                Tx.set(idx, xt - 1 + j, alpha.get(i).pow(xt + j) * beta.get(i))
            }
            idx++
        }
    }

    return Tx
}

/**
 * Make Yx matrix for queue length computation.
 * This creates a K x K matrix.
 *
 * @param alpha service times at LCFS station
 * @param beta service times at LCFS-PR station
 * @param xt current state
 * @param K total population
 * @param R number of classes
 * @param r class index
 */
private fun makeYx(alpha: Matrix, beta: Matrix, xt: Int, K: Int, R: Int, r: Int): Matrix {
    val Y = Matrix(R, K)

    for (i in 0 until R) {
        // Columns 0 to xt-1: alpha(i)^(j+1) for j=0..xt-1
        for (j in 0 until xt) {
            Y.set(i, j, alpha.get(i).pow(j + 1))
        }
        // Columns xt to K-1: alpha(i)^(xt+j) * beta(i) for j=0..K-xt-1, but only if i != r
        for (j in 0 until (K - xt)) {
            if (i != r) {
                Y.set(i, xt + j, alpha.get(i).pow(xt + j) * beta.get(i))
            }
        }
    }

    return Y
}

/**
 * Compute permanent of matrix with rows repeated according to population,
 * excluding class r.
 */
private fun computePermanent(A: Matrix, N: Matrix, excludeClass: Int): Double {
    val K = N.elementSum().toInt() - N.get(excludeClass).toInt()
    val R = N.length()

    if (K <= 0) return 1.0

    // Build expanded matrix with rows repeated according to N (excluding class r)
    val expandedMatrix = Matrix(K, K)
    var rowIdx = 0
    for (classR in 0 until R) {
        if (classR != excludeClass) {
            val count = N.get(classR).toInt()
            for (rep in 0 until count) {
                for (col in 0 until K) {
                    expandedMatrix.set(rowIdx, col, A.get(rowIdx / maxOf(1, count), col))
                }
                rowIdx++
            }
        }
    }

    // Rebuild properly - each class contributes N(class) rows
    rowIdx = 0
    var sourceRow = 0
    for (classR in 0 until R) {
        if (classR != excludeClass) {
            val count = N.get(classR).toInt()
            for (rep in 0 until count) {
                for (col in 0 until K) {
                    expandedMatrix.set(rowIdx, col, A.get(sourceRow, col))
                }
                rowIdx++
            }
            sourceRow++
        }
    }

    val permSolver = Permanent(expandedMatrix, true)
    return permSolver.value
}

/**
 * Compute permanent of matrix with rows repeated according to full population.
 */
private fun computePermanentFull(A: Matrix, N: Matrix): Double {
    val K = N.elementSum().toInt()
    val R = N.length()

    if (K <= 0) return 1.0

    // Build expanded matrix with rows repeated according to N
    val expandedMatrix = Matrix(K, K)
    var rowIdx = 0
    for (classR in 0 until R) {
        val count = N.get(classR).toInt()
        for (rep in 0 until count) {
            for (col in 0 until K) {
                expandedMatrix.set(rowIdx, col, A.get(classR, col))
            }
            rowIdx++
        }
    }

    val permSolver = Permanent(expandedMatrix, true)
    return permSolver.value
}
