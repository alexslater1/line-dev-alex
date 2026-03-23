package jline.solvers.mam.handlers

import jline.api.qsys.qsys_bmapphnn_retrial
import jline.api.qsys.qsys_is_retrial
import jline.VerboseLevel
import jline.io.line_debug
import jline.io.line_error
import jline.lang.NetworkStruct
import jline.solvers.SolverOptions
import jline.solvers.mam.MAMResult
import jline.util.matrix.Matrix

/**
 * Solves BMAP/PH/N/N bufferless retrial queues using the MAM retrial solver.
 *
 * Extracts arrival process (BMAP), service process (PH), and retrial parameters
 * from the network structure and calls [qsys_bmapphnn_retrial].
 *
 * Mirrors MATLAB solver_mam_retrial.m (retrial branch).
 *
 * @param sn Network structure
 * @param options Solver options
 * @return MAMResult with performance metrics
 */
fun solver_mam_retrial(sn: NetworkStruct, options: SolverOptions): MAMResult {
    val retInfo = qsys_is_retrial(sn)
    if (!retInfo.isRetrial) {
        line_error("solver_mam_retrial", "No valid retrial configuration detected: ${retInfo.errorMsg}")
        throw RuntimeException("No valid retrial configuration detected: ${retInfo.errorMsg}")
    }

    val M = sn.nstations
    val K = sn.nclasses

    val QN = Matrix(M, K, M * K)
    val UN = Matrix(M, K, M * K)
    val RN = Matrix(M, K, M * K)
    val TN = Matrix(M, K, M * K)
    val CN = Matrix(1, K, K)
    val XN = Matrix(1, K, K)

    val stationIdx = retInfo.stationIdx
    val sourceIdx = retInfo.sourceIdx
    val classIdx = retInfo.classIdx
    val N = retInfo.N

    // Extract arrival process (BMAP) from source
    val sourceStation = sn.stations[sourceIdx]
    val jobClass = sn.jobclasses[classIdx]
    val arrivalProc = sn.proc[sourceStation]?.get(jobClass)
        ?: throw RuntimeException("No arrival process at source station")

    // Convert arrival process to BMAP matrices D = {D0, D1, ...}
    // LINE stores arrival processes in MAP format: {D0, D1, D2, ...}
    val numMats = arrivalProc.size()
    if (numMats < 2) {
        throw RuntimeException("Invalid arrival process: need at least D0 and D1 matrices")
    }
    val D = Array<Matrix>(numMats) { k ->
        arrivalProc[k] ?: throw RuntimeException("Null matrix at index $k in arrival process")
    }

    // Extract PH service distribution from queue
    val queueStation = sn.stations[stationIdx]
    val serviceProc = sn.proc[queueStation]?.get(jobClass)
        ?: throw RuntimeException("No service process at queue station")

    // Service process is stored as MAP format: {D0, D1}
    // D0 = T (subgenerator), D1 = S0 * alpha (exit rate * initial prob)
    val S = serviceProc[0]
        ?: throw RuntimeException("No D0 matrix in service process")
    val D1_service = serviceProc[1]
        ?: throw RuntimeException("No D1 matrix in service process")

    val nPhases = S.numRows

    // Extract beta (initial probability vector) from D1 = S0 * alpha
    // S0 = -S * ones (exit rates)
    val S0 = Matrix(nPhases, 1)
    for (i in 0 until nPhases) {
        var rowSum = 0.0
        for (j in 0 until nPhases) {
            rowSum += S[i, j]
        }
        S0[i, 0] = -rowSum
    }

    // Find a row with non-zero exit rate to extract beta
    val beta = Matrix(1, nPhases)
    var foundIdx = -1
    for (i in 0 until nPhases) {
        if (S0[i, 0] > 1e-10) {
            foundIdx = i
            break
        }
    }

    if (foundIdx >= 0) {
        for (j in 0 until nPhases) {
            beta[0, j] = D1_service[foundIdx, j] / S0[foundIdx, 0]
        }
    } else {
        // All rows have zero exit rate - use uniform
        for (j in 0 until nPhases) {
            beta[0, j] = 1.0 / nPhases
        }
    }

    // Normalize beta to sum to 1
    var betaSum = 0.0
    for (j in 0 until nPhases) {
        betaSum += beta[0, j]
    }
    if (Math.abs(betaSum - 1.0) > 1e-6 && betaSum > 0) {
        for (j in 0 until nPhases) {
            beta[0, j] = beta[0, j] / betaSum
        }
    }

    // Extract retrial rate alpha from network configuration
    var alpha = 0.1  // Default retrial rate
    if (sn.retrialProc != null) {
        val retrialDistCell = sn.retrialProc[queueStation]?.get(jobClass)
        if (retrialDistCell != null && retrialDistCell.size() >= 2) {
            // For Exp(alpha), retrialProc stores {D0, D1} where D0 = -alpha
            val retrialD0 = retrialDistCell[0]
            if (retrialD0 != null && retrialD0.numRows >= 1 && retrialD0.numCols >= 1) {
                alpha = -retrialD0[0, 0]
            }
        }
    }

    // Extract orbit impatience gamma (default 0)
    val gamma = retInfo.gamma

    // Extract batch rejection probability p (default 0)
    val p = retInfo.p

    // Extract admission threshold R (from FCR or default N-1)
    val R = retInfo.R

    // Solver parameters
    val maxLevel = if (options.iter_max > 0) options.iter_max else 150
    val tol = if (options.tol > 0) options.tol else 1e-10
    val verbose = options.verbose != VerboseLevel.SILENT

    line_debug(options.verbose, "Calling qsys_bmapphnn_retrial: N=$N, alpha=$alpha, gamma=$gamma, p=$p, R=$R")

    // Call qsys BMAP/PH/N/N retrial solver
    val perf = qsys_bmapphnn_retrial(D, beta, S, N, alpha, gamma, p, R, maxLevel, tol, verbose)

    // Map to LINE output format
    // Queue length includes both orbit and servers
    QN[stationIdx, classIdx] = perf.L_orbit + perf.N_server
    UN[stationIdx, classIdx] = perf.Utilization
    TN[stationIdx, classIdx] = perf.Throughput

    // Response time via Little's law
    if (perf.Throughput > 0) {
        RN[stationIdx, classIdx] = QN[stationIdx, classIdx] / perf.Throughput
    } else {
        RN[stationIdx, classIdx] = Double.POSITIVE_INFINITY
    }

    // System-level metrics
    XN[0, classIdx] = perf.Throughput
    CN[0, classIdx] = RN[stationIdx, classIdx]

    val result = MAMResult()
    result.QN = QN
    result.UN = UN
    result.RN = RN
    result.TN = TN
    result.CN = CN
    result.XN = XN
    result.iter = perf.truncLevel
    result.method = "retrial"

    return result
}
