/**
 * QRF Analyzer Adapter for SolverCTMC.
 *
 * Bridges the LINE NetworkStruct to QRF (Quadratic Reduction Framework) library
 * functions for approximating performance metrics of single-class closed
 * queueing networks with PH service.
 *
 * Port of MATLAB solver_ctmc_qrf_analyzer.m.
 *
 * Copyright (c) 2012-2026, Imperial College London
 * All rights reserved.
 *
 * @since LINE 3.0
 */
package jline.solvers.ctmc.analyzers

import jline.api.mam.map_mean
import jline.api.mapqn.*
import jline.api.sn.snRefreshVisits
import jline.io.line_error
import jline.io.line_warning
import jline.lang.NetworkStruct
import jline.solvers.SolverOptions
import jline.solvers.ctmc.SolverCTMC
import jline.util.matrix.Matrix
import jline.util.matrix.MatrixCell

class Solver_ctmc_qrf_analyzer {
    companion object {
        @JvmStatic
        fun solver_ctmc_qrf_analyzer(sn: NetworkStruct, options: SolverOptions): SolverCTMC.AnalyzerResult {
            val T0 = System.nanoTime()

            val M = sn.nstations
            val K = sn.nclasses
            val N = sn.njobs.elementSum().toInt()
            val S = sn.nservers

            // QRF only supports single-class closed networks
            if (K != 1) {
                line_error("Solver_ctmc_qrf_analyzer", String.format(
                    "QRF methods only support single-class networks (found %d classes).", K))
            }
            for (r in 0 until K) {
                if (java.lang.Double.isInfinite(sn.njobs.get(0, r))) {
                    line_error("Solver_ctmc_qrf_analyzer", "QRF methods only support closed networks.")
                }
            }

            // Extract MAPs as arrays: MAPs[i] = { D0[h][k], D1[h][k] }
            val KPhases = IntArray(M)
            val MAPs = Array<Array<Array<DoubleArray>>?>(M) { null }
            for (i in 0 until M) {
                val station = sn.stations[i]
                val jobClass = sn.jobclasses[0]
                val procCell = sn.proc[station]?.get(jobClass)
                if (procCell != null && procCell.size() > 0) {
                    val D0 = procCell.get(0)
                    val D1 = procCell.get(1)
                    val nPhases = D0.numRows
                    KPhases[i] = nPhases
                    val d0Arr = Array(nPhases) { h -> DoubleArray(nPhases) { k -> D0.get(h, k) } }
                    val d1Arr = Array(nPhases) { h -> DoubleArray(nPhases) { k -> D1.get(h, k) } }
                    MAPs[i] = arrayOf(d0Arr, d1Arr)
                } else {
                    KPhases[i] = 1
                    MAPs[i] = arrayOf(
                        arrayOf(doubleArrayOf(-1.0)),
                        arrayOf(doubleArrayOf(1.0))
                    )
                }
            }

            // Build routing matrix (M x M) from sn.rt (MK x MK)
            // K=1, so rt index = (i-1)*K+r mapped to 0-based: i*K+0
            val rt = Array(M) { i -> DoubleArray(M) { j -> sn.rt.get(i, j) } }

            // Extract mu and v arrays from MAPs
            val Kmax = KPhases.max()
            val mu = Array(M) { Array(Kmax) { DoubleArray(Kmax) } }
            val v = Array(M) { Array(Kmax) { DoubleArray(Kmax) } }
            for (i in 0 until M) {
                val map = MAPs[i]!!
                val D0 = map[0]
                val D1 = map[1]
                for (h in 0 until KPhases[i]) {
                    for (k in 0 until KPhases[i]) {
                        mu[i][h][k] = D1[h][k]
                        v[i][k][h] = if (h == k) 0.0 else D0[h][k]
                    }
                }
            }

            // Dispatch based on method
            var UN_qrf: DoubleArray
            var QN_qrf: DoubleArray
            val method = options.method

            when (method) {
                "qrf.mmi" -> {
                    val sol = Mapqn_qrf_noblo_mmi.solve(M, 1, KPhases, N, mu, v, rt)
                    UN_qrf = extractUN(sol, M)
                    QN_qrf = extractQN(sol, M)
                }
                "qrf.mem" -> {
                    @Suppress("UNCHECKED_CAST")
                    val mapsNonNull = MAPs as Array<Array<Array<DoubleArray>>>
                    val sol = Mapqn_qrf_noblo_mem.solve(mapsNonNull, N, rt)
                    UN_qrf = extractUN(sol, M)
                    QN_qrf = extractQN(sol, M)
                }
                "qrf.mmi.ld" -> {
                    @Suppress("UNCHECKED_CAST")
                    val mapsNonNull = MAPs as Array<Array<Array<DoubleArray>>>
                    val alpha = options.qrfAlpha
                    val sol = Mapqn_qrf_noblo_mmi_ld.solve(mapsNonNull, N, rt, alpha)
                    UN_qrf = extractUN(sol, M)
                    QN_qrf = extractQN(sol, M)
                }
                "qrf.mmi.linear" -> {
                    @Suppress("UNCHECKED_CAST")
                    val mapsNonNull = MAPs as Array<Array<Array<DoubleArray>>>
                    val alpha = options.qrfAlpha
                    val sol = Mapqn_qrf_noblo_mmi_linear.solve(mapsNonNull, N, rt, alpha)
                    UN_qrf = extractUN(sol, M)
                    QN_qrf = extractQN(sol, M)
                }
                "qrf.bas" -> {
                    val params = snToQrfBasParams(sn, KPhases, N, mu, v, rt, options)
                    val result = deriveQnFromBounds(params, M, N, S, MAPs, sn)
                    UN_qrf = result.first
                    QN_qrf = result.second
                }
                "qrf.rsrd" -> {
                    val params = snToQrfRsrdParams(sn, KPhases, N, mu, v, rt, options)
                    val result = deriveQnFromBounds(params, M, N, S, MAPs, sn)
                    UN_qrf = result.first
                    QN_qrf = result.second
                }
                else -> {
                    line_error("Solver_ctmc_qrf_analyzer", String.format(
                        "Unknown QRF method: %s", method))
                    UN_qrf = DoubleArray(M)
                    QN_qrf = DoubleArray(M)
                }
            }

            // Normalize QN to population constraint
            val qnSum = QN_qrf.sum()
            if (qnSum > 0) {
                for (i in 0 until M) {
                    QN_qrf[i] = QN_qrf[i] / qnSum * N
                }
            }

            // Map 1D QRF results to M x K matrices (K=1)
            val QN = Matrix(M, K)
            val UN = Matrix(M, K)
            val TN = Matrix(M, K)
            val RN = Matrix(M, K)
            val XN = Matrix(1, K)
            val CN = Matrix(1, K)

            for (i in 0 until M) {
                QN.set(i, 0, QN_qrf[i])
            }

            // Compute visit ratios
            val V: Matrix
            if (sn.visits != null && !sn.visits.isEmpty()) {
                V = Matrix.cellsum(sn.visits)
            } else {
                val snUpdated = snRefreshVisits(sn, sn.chains, sn.rt, sn.rtnodes)
                V = Matrix.cellsum(snUpdated.visits)
            }

            val refstat = sn.refstat.get(0, 0).toInt()

            // Find system throughput XN from QN using Little's law at reference station
            var stimeRef = 0.0
            val procRef = sn.proc[sn.stations[refstat]]?.get(sn.jobclasses[0])
            if (procRef != null && procRef.size() > 0) {
                stimeRef = map_mean(procRef.get(0), procRef.get(1))
            }
            if (stimeRef > 0 && V.get(refstat, 0) > 0) {
                XN.set(0, 0, QN.get(refstat, 0) / (V.get(refstat, 0) * stimeRef))
            }

            // Derive per-station metrics from XN and visit ratios
            for (i in 0 until M) {
                val procI = sn.proc[sn.stations[i]]?.get(sn.jobclasses[0])
                if (procI != null && procI.size() > 0) {
                    val stime = map_mean(procI.get(0), procI.get(1))
                    TN.set(i, 0, XN.get(0, 0) * V.get(i, 0))
                    if (java.lang.Double.isInfinite(S.get(i, 0))) {
                        // Delay (infinite server): UN = QN by LINE convention
                        UN.set(i, 0, QN.get(i, 0))
                    } else {
                        // Finite server: UN = TN * stime / S
                        if (stime > 0) {
                            UN.set(i, 0, TN.get(i, 0) * stime / S.get(i, 0))
                        }
                    }
                    // Response time via Little's law
                    if (TN.get(i, 0) > 0) {
                        RN.set(i, 0, QN.get(i, 0) / TN.get(i, 0))
                    }
                }
            }

            // Cycle time
            if (XN.get(0, 0) > 0) {
                CN.set(0, 0, N.toDouble() / XN.get(0, 0))
            }

            // Clean NaN values
            cleanNaN(QN)
            cleanNaN(UN)
            cleanNaN(RN)
            cleanNaN(TN)
            cleanNaN(XN)
            cleanNaN(CN)

            val runtime = (System.nanoTime() - T0) / 1_000_000_000.0

            return SolverCTMC.AnalyzerResult(
                QN, UN, RN, TN, CN, XN,
                Matrix(0, 0),  // InfGen (not applicable for QRF)
                Matrix(0, 0),  // StateSpace
                Matrix(0, 0),  // StateSpaceAggr
                MatrixCell(),  // EventFiltration
                runtime,
                "solver_ctmc_qrf_analyzer",
                sn
            )
        }

        private fun extractUN(sol: Mapqn_solution, M: Int): DoubleArray {
            val UN = DoubleArray(M)
            for (i in 0 until M) {
                UN[i] = sol.getVariable("UN_${i + 1}")
            }
            return UN
        }

        private fun extractQN(sol: Mapqn_solution, M: Int): DoubleArray {
            val QN = DoubleArray(M)
            for (i in 0 until M) {
                QN[i] = sol.getVariable("QN_${i + 1}")
            }
            return QN
        }

        private fun cleanNaN(m: Matrix) {
            for (i in 0 until m.numRows) {
                for (j in 0 until m.numCols) {
                    if (java.lang.Double.isNaN(m.get(i, j))) {
                        m.set(i, j, 0.0)
                    }
                }
            }
        }

        /**
         * Build Mapqn_qr_bounds_bas_parameters from sn struct for qrf.bas method.
         */
        private fun snToQrfBasParams(
            sn: NetworkStruct, KPhases: IntArray, N: Int,
            mu: Array<Array<DoubleArray>>, v: Array<Array<DoubleArray>>,
            rt: Array<DoubleArray>, options: SolverOptions
        ): Mapqn_qr_bounds_bas_parameters {
            val M = sn.nstations

            // Convert mu/v from 3D arrays to Matrix arrays expected by parameter class
            val muMat = Array(M) { i ->
                val Ki = KPhases[i]
                val mat = Matrix(Ki, Ki)
                for (h in 0 until Ki) for (k in 0 until Ki) mat.set(h, k, mu[i][h][k])
                mat
            }
            val vMat = Array(M) { i ->
                val Ki = KPhases[i]
                val mat = Matrix(Ki, Ki)
                for (h in 0 until Ki) for (k in 0 until Ki) mat.set(h, k, v[i][h][k])
                mat
            }

            // Capacity
            val F = IntArray(M) { i ->
                if (sn.cap != null && sn.cap.numRows > 0) {
                    val c = sn.cap.get(i, 0).toInt()
                    if (c == Integer.MAX_VALUE || java.lang.Double.isInfinite(sn.cap.get(i, 0))) N else c
                } else {
                    N
                }
            }

            // Routing matrix as Matrix
            val rMat = Matrix(M, M)
            for (i in 0 until M) for (j in 0 until M) rMat.set(i, j, rt[i][j])

            // Default no-blocking parameters
            val f = 1
            val MR = 1
            val BB = Matrix(1, M)
            val MM = Matrix(1, 2)
            val MM1 = Matrix(1, M)
            val ZZ = intArrayOf(0)

            return Mapqn_qr_bounds_bas_parameters(
                M, N, MR, f, KPhases, F, MM, MM1, ZZ, BB, muMat, vMat, rMat
            )
        }

        /**
         * Build Mapqn_qr_bounds_rsrd_parameters from sn struct for qrf.rsrd method.
         */
        private fun snToQrfRsrdParams(
            sn: NetworkStruct, KPhases: IntArray, N: Int,
            mu: Array<Array<DoubleArray>>, v: Array<Array<DoubleArray>>,
            rt: Array<DoubleArray>, options: SolverOptions
        ): Mapqn_qr_bounds_rsrd_parameters {
            val M = sn.nstations

            // Convert mu/v to Matrix arrays
            val muMat = Array(M) { i ->
                val Ki = KPhases[i]
                val mat = Matrix(Ki, Ki)
                for (h in 0 until Ki) for (k in 0 until Ki) mat.set(h, k, mu[i][h][k])
                mat
            }
            val vMat = Array(M) { i ->
                val Ki = KPhases[i]
                val mat = Matrix(Ki, Ki)
                for (h in 0 until Ki) for (k in 0 until Ki) mat.set(h, k, v[i][h][k])
                mat
            }

            // Capacity
            val F = IntArray(M) { i ->
                if (sn.cap != null && sn.cap.numRows > 0) {
                    val c = sn.cap.get(i, 0).toInt()
                    if (c == Integer.MAX_VALUE || java.lang.Double.isInfinite(sn.cap.get(i, 0))) N else c
                } else {
                    N
                }
            }

            // Load-dependent alpha
            val alpha = if (options.qrfAlpha != null) {
                Array(M) { i -> options.qrfAlpha[i].clone() }
            } else {
                Array(M) { DoubleArray(N) { 1.0 } }
            }

            // Routing matrix as Matrix
            val rMat = Matrix(M, M)
            for (i in 0 until M) for (j in 0 until M) rMat.set(i, j, rt[i][j])

            return Mapqn_qr_bounds_rsrd_parameters(M, N, F, KPhases, muMat, vMat, alpha, rMat)
        }

        /**
         * Derive QN from utilization bounds using visit ratios and Little's law.
         * For bounds methods (qrf.bas, qrf.rsrd), the solver returns per-queue
         * utilization bounds. We solve min and max for each queue, average,
         * then compute system throughput and derive QN at all stations.
         */
        private fun deriveQnFromBounds(
            params: Mapqn_parameters, M: Int, N: Int, S: Matrix,
            MAPs: Array<Array<Array<DoubleArray>>?>, sn: NetworkStruct
        ): Pair<DoubleArray, DoubleArray> {
            // Solve bounds for each queue (average of min and max)
            val UBounds = DoubleArray(M)
            for (i in 0 until M) {
                val queueIdx = i + 1 // 1-based
                val minSol: Mapqn_solution
                val maxSol: Mapqn_solution
                if (params is Mapqn_qr_bounds_bas_parameters) {
                    minSol = Mapqn_qr_bounds_bas.solve(params, queueIdx, "min")
                    maxSol = Mapqn_qr_bounds_bas.solve(params, queueIdx, "max")
                } else {
                    minSol = Mapqn_qr_bounds_rsrd.solve(params as Mapqn_qr_bounds_rsrd_parameters, queueIdx, "min")
                    maxSol = Mapqn_qr_bounds_rsrd.solve(params, queueIdx, "max")
                }
                UBounds[i] = (minSol.objectiveValue + maxSol.objectiveValue) / 2.0
            }

            // Compute visit ratios
            val V: Matrix
            if (sn.visits != null && !sn.visits.isEmpty()) {
                V = Matrix.cellsum(sn.visits)
            } else {
                val snUpdated = snRefreshVisits(sn, sn.chains, sn.rt, sn.rtnodes)
                V = Matrix.cellsum(snUpdated.visits)
            }

            // Find XN from the first finite-server station with nonzero utilization
            var XNEst = 0.0
            for (i in 0 until M) {
                if (!java.lang.Double.isInfinite(S.get(i, 0))) {
                    val procI = sn.proc[sn.stations[i]]?.get(sn.jobclasses[0])
                    if (procI != null && procI.size() > 0 && UBounds[i] > 0 && V.get(i, 0) > 0) {
                        val stime = map_mean(procI.get(0), procI.get(1))
                        if (stime > 0) {
                            XNEst = UBounds[i] * S.get(i, 0) / (V.get(i, 0) * stime)
                            break
                        }
                    }
                }
            }

            // Derive QN at each station
            val QN_qrf = DoubleArray(M)
            val UN_qrf = UBounds.clone()
            for (i in 0 until M) {
                val procI = sn.proc[sn.stations[i]]?.get(sn.jobclasses[0])
                if (procI != null && procI.size() > 0) {
                    val stime = map_mean(procI.get(0), procI.get(1))
                    val TNi = XNEst * V.get(i, 0)
                    if (java.lang.Double.isInfinite(S.get(i, 0))) {
                        // Delay: QN = TN * stime
                        QN_qrf[i] = TNi * stime
                        UN_qrf[i] = QN_qrf[i] // LINE convention for INF server
                    } else {
                        // Queue: QN = TN * stime / (1 - U) for M/G/1-like estimate
                        if (UBounds[i] < 1) {
                            QN_qrf[i] = TNi * stime / (1 - UBounds[i])
                        } else {
                            QN_qrf[i] = N.toDouble() // saturated
                        }
                    }
                }
            }

            // Rescale to population constraint
            val qnSum = QN_qrf.sum()
            if (qnSum > 0) {
                for (i in 0 until M) {
                    QN_qrf[i] = QN_qrf[i] / qnSum * N
                }
            }

            return Pair(UN_qrf, QN_qrf)
        }
    }
}
