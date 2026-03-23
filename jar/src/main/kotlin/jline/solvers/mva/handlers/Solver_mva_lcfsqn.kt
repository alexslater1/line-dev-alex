/*
 * Copyright (c) 2012-2026, QORE Lab, Imperial College London
 * All rights reserved.
 */

package jline.solvers.mva.handlers

import jline.api.pfqn.lcfs.pfqn_lcfsqn_mva
import jline.lang.NetworkStruct
import jline.solvers.SolverOptions
import jline.solvers.mva.MVAResult
import jline.util.matrix.Matrix

/**
 * Specialized MVA solver for LCFS + LCFS-PR 2-station networks.
 *
 * This function wraps the pfqn_lcfsqn_mva algorithm and maps LINE's data
 * structures to/from the algorithm's expected format.
 *
 * @param sn Network structure
 * @param options Solver options
 * @param lcfsStat Index of the LCFS station
 * @param lcfsprStat Index of the LCFS-PR station
 * @return MVAResult containing performance metrics
 */
fun solver_mva_lcfsqn(
    sn: NetworkStruct,
    options: SolverOptions,
    lcfsStat: Int,
    lcfsprStat: Int
): MVAResult {
    val startTime = System.nanoTime()
    val M = sn.nstations
    val nclasses = sn.nclasses
    val njobs = sn.njobs

    // Extract service times for each class at each station
    // alpha(r) = mean service time at LCFS station for class r
    // beta(r) = mean service time at LCFS-PR station for class r
    val alpha = Matrix(1, nclasses)
    val beta = Matrix(1, nclasses)

    val rates = sn.rates
    for (r in 0 until nclasses) {
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

    // Call the LCFS MVA algorithm
    val mvaResult = pfqn_lcfsqn_mva(alpha, beta, N)
    val T_lcfs = mvaResult.T
    val Q_lcfs = mvaResult.Q
    val U_lcfs = mvaResult.U

    // Map results back to LINE format
    val Q = Matrix(M, nclasses)
    val U = Matrix(M, nclasses)
    val T = Matrix(M, nclasses)
    val R = Matrix(M, nclasses)
    val X = Matrix(1, nclasses)
    val C = Matrix(1, nclasses)

    // Map queue lengths
    for (r in 0 until nclasses) {
        Q.set(lcfsStat, r, Q_lcfs.get(0, r))
        Q.set(lcfsprStat, r, Q_lcfs.get(1, r))
    }

    // Map utilizations
    for (r in 0 until nclasses) {
        U.set(lcfsStat, r, U_lcfs.get(0, r))
        U.set(lcfsprStat, r, U_lcfs.get(1, r))
    }

    // Throughput is the same at all stations in a closed network
    for (r in 0 until nclasses) {
        if (njobs.get(r) > 0) {
            X.set(r, T_lcfs.get(0, r))
            T.set(lcfsStat, r, T_lcfs.get(0, r))
            T.set(lcfsprStat, r, T_lcfs.get(0, r))
        }
    }

    // Compute response times: R = Q / T (using Little's Law)
    for (k in listOf(lcfsStat, lcfsprStat)) {
        for (r in 0 until nclasses) {
            if (T.get(k, r) > 0) {
                R.set(k, r, Q.get(k, r) / T.get(k, r))
            }
        }
    }

    // Compute cycle times: C = sum of response times at all stations
    for (r in 0 until nclasses) {
        if (njobs.get(r) > 0) {
            C.set(r, R.get(lcfsStat, r) + R.get(lcfsprStat, r))
        }
    }

    val runtime = (System.nanoTime() - startTime) / 1_000_000_000.0

    // Create and populate MVAResult
    val result = MVAResult()
    result.QN = Q
    result.UN = U
    result.RN = R
    result.TN = T
    result.CN = C
    result.XN = X
    result.logNormConstAggr = Double.NaN  // Not computed by this method
    result.runtime = runtime

    return result
}
