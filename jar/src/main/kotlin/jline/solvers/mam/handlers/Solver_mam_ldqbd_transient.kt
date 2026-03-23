/*
 * Copyright (c) 2012-2026, QORE Lab, Imperial College London
 * All rights reserved.
 */

package jline.solvers.mam.handlers

import jline.io.line_error
import jline.io.mfilename
import jline.lang.NetworkStruct
import jline.lang.constant.SchedStrategy
import jline.solvers.SolverOptions
import jline.util.matrix.Matrix
import jline.api.mam.map_pie
import kotlin.math.ceil
import kotlin.math.ln
import kotlin.math.max
import kotlin.math.min
import kotlin.math.round

/**
 * Transient analysis of open queues via standard QBD (matrix exponentiation).
 *
 * Supports single-class open models (Source -> Queue -> Sink).
 * For M/M/c: scalar QBD levels (any number of servers).
 * For M/PH/1: m-phase QBD levels (single server only).
 *
 * Infinite capacity: truncation to finite buffer + expm.
 * Finite capacity: direct matrix exponentiation (expm).
 */
data class TransientResult(
    val Qt: Array<Array<Matrix?>>,
    val Ut: Array<Array<Matrix?>>,
    val Tt: Array<Array<Matrix?>>
)

fun solver_mam_ldqbd_transient(sn: NetworkStruct, options: SolverOptions): TransientResult {
    val M = sn.nstations
    val K = sn.nclasses

    if (K != 1) {
        line_error(mfilename(object {}), "Transient QBD method requires a single-class model.")
        return emptyTransientResult(M, K)
    }

    val N = sn.njobs[0, 0]
    if (!java.lang.Double.isInfinite(N)) {
        line_error(mfilename(object {}), "Transient QBD method requires an open model.")
        return emptyTransientResult(M, K)
    }

    // Identify stations
    var sourceIdx = -1
    var queueIdx = -1
    for (i in 0 until M) {
        val sched = sn.sched[sn.stations[i]]
        if (sched == SchedStrategy.EXT) {
            sourceIdx = i
        } else if (sched == SchedStrategy.FCFS) {
            queueIdx = i
        }
    }

    if (sourceIdx < 0 || queueIdx < 0) {
        line_error(mfilename(object {}), "Transient QBD method requires exactly one Source and one FCFS Queue.")
        return emptyTransientResult(M, K)
    }

    // Extract parameters
    val lambda = sn.rates[sourceIdx, 0]
    val queueStation = sn.stations[queueIdx]
    val jobClass = sn.jobclasses[0]
    val PH_queue = sn.proc[queueStation]?.get(jobClass)
        ?: throw RuntimeException("No service process for queue station")
    val nServers = sn.nservers[queueIdx, 0].toInt()
    val bufCap = sn.cap[queueIdx, 0].toInt()

    val D0 = PH_queue[0] ?: throw RuntimeException("No D0 matrix in service process")
    val nPhases: Int
    val isPH: Boolean
    val mu: Double

    if (D0.numRows == 1 && D0.numCols == 1) {
        mu = -D0[0, 0]
        nPhases = 1
        isPH = false
    } else {
        nPhases = D0.numRows
        isPH = true
        mu = 0.0 // not used in PH path
    }

    if (isPH && nServers > 1) {
        line_error(mfilename(object {}), "Transient QBD with PH service supports single-server only.")
        return emptyTransientResult(M, K)
    }

    // PH-specific variables
    val D1: Matrix?
    val alpha: Matrix?
    val t_exit: Matrix?
    if (isPH) {
        D1 = PH_queue[1] ?: throw RuntimeException("No D1 matrix in PH service process")
        alpha = map_pie(PH_queue)
        t_exit = D0.mult(Matrix.ones(nPhases, 1)).scale(-1.0)
    } else {
        D1 = null
        alpha = null
        t_exit = null
    }

    // Time parameters
    val T_start = options.timespan[0]
    val T_end = options.timespan[1]
    val T_duration = T_end - T_start

    // Determine capacity (truncate if infinite)
    val Cap: Int
    if (java.lang.Double.isInfinite(bufCap.toDouble()) || bufCap > 1000000) {
        // Infinite capacity: truncate based on traffic intensity
        val muEff = if (isPH) {
            val meanSvc = jline.api.mam.map_mean(PH_queue)
            if (meanSvc > 0) 1.0 / meanSvc else 1.0
        } else {
            mu
        }
        val rho = lambda / (nServers * muEff)
        val tol = options.tol
        Cap = if (rho < 1.0) {
            min(10000, max(200, ceil(-ln(tol) / (-ln(rho))).toInt()))
        } else {
            10000
        }
    } else {
        Cap = bufCap
    }

    // Build generator Q
    val Q: Matrix
    val dim: Int

    if (!isPH) {
        // M/M/c/N: scalar levels, (Cap+1) x (Cap+1) generator
        dim = Cap + 1
        Q = Matrix(dim, dim)
        for (n in 0..Cap) {
            val dep = min(n, nServers) * mu
            val arr = if (n < Cap) lambda else 0.0
            if (n > 0) {
                Q[n, n - 1] = dep
            }
            if (n < Cap) {
                Q[n, n + 1] = arr
            }
            Q[n, n] = -(dep + arr)
        }
    } else {
        // M/PH/1/N: level 0 is 1-dim, levels 1..Cap are nPhases-dim
        dim = 1 + Cap * nPhases
        Q = Matrix(dim, dim)

        // Level 0
        Q[0, 0] = -lambda
        for (j in 0 until nPhases) {
            Q[0, 1 + j] = lambda * alpha!![0, j]
        }

        for (n in 1..Cap) {
            val rowStart = 1 + (n - 1) * nPhases

            // Internal transitions (D0) and diagonal
            for (i in 0 until nPhases) {
                for (j in 0 until nPhases) {
                    Q[rowStart + i, rowStart + j] = D0[i, j]
                }
                // Subtract arrival rate from diagonal
                if (n < Cap) {
                    Q[rowStart + i, rowStart + i] = Q[rowStart + i, rowStart + i] - lambda
                }
            }

            // Arrivals: level n -> n+1
            if (n < Cap) {
                val nextStart = rowStart + nPhases
                for (i in 0 until nPhases) {
                    Q[rowStart + i, nextStart + i] = lambda
                }
            }

            // Departures: level n -> n-1
            if (n == 1) {
                // Back to level 0 (scalar)
                for (i in 0 until nPhases) {
                    Q[rowStart + i, 0] = t_exit!![i, 0]
                }
            } else {
                val prevStart = rowStart - nPhases
                for (i in 0 until nPhases) {
                    for (j in 0 until nPhases) {
                        Q[rowStart + i, prevStart + j] = D1!![i, j]
                    }
                }
            }
        }
    }

    // Time grid
    val nTimePoints = min(101, max(11, round(T_duration * 10).toInt()))
    val dt = T_duration / (nTimePoints - 1)
    val times = DoubleArray(nTimePoints) { i -> T_start + i * dt }

    // Initial distribution: empty queue
    val pi_t = Matrix(1, dim)
    pi_t[0, 0] = 1.0

    // Compute eQdt = expm(Q * dt)
    val eQdt = Q.scale(dt).expm()

    val queue_lengths = DoubleArray(nTimePoints)
    val util_values = DoubleArray(nTimePoints)
    val tput_values = DoubleArray(nTimePoints)

    var pi_current = pi_t
    for (t_idx in 0 until nTimePoints) {
        // Extract metrics from pi_current
        var q = 0.0
        var u = 0.0
        var tput = 0.0

        for (n in 0..Cap) {
            val p_n: Double
            if (!isPH) {
                p_n = pi_current[0, n]
            } else {
                if (n == 0) {
                    p_n = pi_current[0, 0]
                } else {
                    val idx_s = 1 + (n - 1) * nPhases
                    var sum = 0.0
                    for (j in 0 until nPhases) {
                        sum += pi_current[0, idx_s + j]
                    }
                    p_n = sum
                }
            }
            q += n * p_n
            if (n >= 1) {
                u += (min(n, nServers).toDouble() / nServers) * p_n
                if (!isPH) {
                    tput += min(n, nServers) * mu * p_n
                } else {
                    val idx_s = 1 + (n - 1) * nPhases
                    for (j in 0 until nPhases) {
                        tput += pi_current[0, idx_s + j] * t_exit!![j, 0]
                    }
                }
            }
        }
        queue_lengths[t_idx] = q
        util_values[t_idx] = u
        tput_values[t_idx] = tput

        // Advance to next time point
        if (t_idx < nTimePoints - 1) {
            pi_current = pi_current.mult(eQdt)
        }
    }

    // Package results in [metric, time] format as M x K cell arrays
    val Qt = Array(M) { arrayOfNulls<Matrix>(K) }
    val Ut = Array(M) { arrayOfNulls<Matrix>(K) }
    val Tt = Array(M) { arrayOfNulls<Matrix>(K) }

    // Queue station results
    val qResult = Matrix(nTimePoints, 2)
    val uResult = Matrix(nTimePoints, 2)
    val tResult = Matrix(nTimePoints, 2)
    for (t in 0 until nTimePoints) {
        qResult[t, 0] = queue_lengths[t]
        qResult[t, 1] = times[t]
        uResult[t, 0] = util_values[t]
        uResult[t, 1] = times[t]
        tResult[t, 0] = tput_values[t]
        tResult[t, 1] = times[t]
    }
    Qt[queueIdx][0] = qResult
    Ut[queueIdx][0] = uResult
    Tt[queueIdx][0] = tResult

    return TransientResult(Qt, Ut, Tt)
}

private fun emptyTransientResult(M: Int, K: Int): TransientResult {
    return TransientResult(
        Array(M) { arrayOfNulls<Matrix>(K) },
        Array(M) { arrayOfNulls<Matrix>(K) },
        Array(M) { arrayOfNulls<Matrix>(K) }
    )
}
