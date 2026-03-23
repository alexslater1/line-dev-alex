package jline.solvers.nc.handlers

import jline.api.pfqn.ld.ljd_linearize
import jline.api.pfqn.ld.pfqn_conv
import jline.lang.NetworkStruct
import jline.solvers.SolverOptions
import jline.solvers.nc.SolverNC
import jline.util.Utils
import jline.util.matrix.Matrix
import kotlin.math.min

/**
 * Exact normalizing constant solver for closed networks with Limited
 * Joint Class Dependence (LJCD) service rates, using the multichain
 * convolution algorithm of Sauer (1983), Section 5.2.
 *
 * Ports MATLAB solver_nc_conv.m
 */
fun solver_nc_conv(sn: NetworkStruct, options: SolverOptions): SolverNC.SolverNCLDReturn {
    val startTime = System.nanoTime()
    val method = "conv"
    val iter = 1

    val M = sn.nstations
    val K = sn.nclasses
    val nservers = sn.nservers

    // Compute V = cellsum(sn.visits)
    var V: Matrix? = null
    for (i in 0 until sn.visits.size) {
        val vi = sn.visits.get(i)
        if (vi != null) {
            V = if (V == null) vi.copy() else V.add(1.0, vi)
        }
    }
    if (V == null) V = Matrix(M, K)

    // ST = 1 / rates
    val ST = Matrix(M, K)
    for (i in 0 until M) {
        for (k in 0 until K) {
            val rate = sn.rates.get(i, k)
            ST.set(i, k, if (java.lang.Double.isNaN(rate) || rate == 0.0) 0.0 else 1.0 / rate)
        }
    }

    // Demands: L(ist,k) = V(ist,k) * ST(ist,k)
    val Ldemand = Matrix(M, K)
    for (i in 0 until M) {
        for (k in 0 until K) {
            Ldemand.set(i, k, V.get(i, k) * ST.get(i, k))
        }
    }

    // Separate delay and queue stations
    val isDelay = BooleanArray(M) { Utils.isInf(nservers.get(it)) }
    val delayIdx = (0 until M).filter { isDelay[it] }
    val queueIdx = (0 until M).filter { !isDelay[it] }
    val nQueues = queueIdx.size

    // Z_conv: total delay demand per class
    val Z_conv = DoubleArray(K)
    for (ist in delayIdx) {
        for (k in 0 until K) {
            Z_conv[k] += Ldemand.get(ist, k)
        }
    }

    // L_conv: demands for queue stations only
    val L_conv = Matrix(nQueues, K)
    for (qi in 0 until nQueues) {
        for (k in 0 until K) {
            L_conv.set(qi, k, Ldemand.get(queueIdx[qi], k))
        }
    }

    // NK: population vector
    val NK = IntArray(K) { sn.njobs.get(0, it).toInt() }
    val Nt = NK.sum()

    // mu_conv: load-dependent rates for queue stations
    val mu_conv: Matrix? = if (!sn.lldscaling.isEmpty) {
        val mu = Matrix(nQueues, Nt)
        mu.fill(1.0)
        for (qi in 0 until nQueues) {
            val ist = queueIdx[qi]
            val nEntries = min(Nt, sn.lldscaling.numCols)
            for (j in 0 until nEntries) {
                mu.set(qi, j, sn.lldscaling.get(ist, j))
            }
        }
        mu
    } else null

    // LJCD scaling tables for queue stations
    val stations = sn.stations
    val jobClasses = sn.jobclasses
    val ljcdscalingConv: List<List<Matrix?>?>? = if (sn.ljcdscaling != null && sn.ljcdscaling.isNotEmpty()) {
        (0 until nQueues).map { qi ->
            val station = stations[queueIdx[qi]]
            val stationMap = sn.ljcdscaling!![station]
            if (stationMap != null) {
                jobClasses.map { jc -> stationMap[jc] }
            } else {
                null
            }
        }
    } else null

    val ljcdcutoffsConv: List<Matrix?>? = if (sn.ljcdcutoffs != null && sn.ljcdcutoffs.isNotEmpty()) {
        (0 until nQueues).map { qi ->
            val station = stations[queueIdx[qi]]
            sn.ljcdcutoffs!![station]
        }
    } else null

    // Compute G(N)
    val result = pfqn_conv(L_conv, NK, Z_conv, mu_conv, ljcdscalingConv, ljcdcutoffsConv)
    val G_N = result[0]
    val lG = result[1]

    // Compute G(N - e_k) for each class -> throughput
    val XN = DoubleArray(K)
    for (k in 0 until K) {
        if (NK[k] > 0) {
            val NKminus = NK.copyOf()
            NKminus[k]--
            val resultK = pfqn_conv(L_conv, NKminus, Z_conv, mu_conv, ljcdscalingConv, ljcdcutoffsConv)
            XN[k] = resultK[0] / G_N
        }
    }

    // Compute per-station throughput TN = V * XN
    val TN = Matrix(M, K)
    for (i in 0 until M) {
        for (k in 0 until K) {
            TN.set(i, k, V.get(i, k) * XN[k])
        }
    }

    // Compute queue lengths QN
    val QN = Matrix(M, K)

    // Delay stations: Q = L * X
    for (ist in delayIdx) {
        for (k in 0 until K) {
            QN.set(ist, k, Ldemand.get(ist, k) * XN[k])
        }
    }

    // Queue stations: use marginal distribution P_m(n|N) = X_m(n) * G_{-m}(N-n) / G(N)
    val stateSpaceSize = NK.fold(1) { acc, n -> acc * (n + 1) }

    for (qi in 0 until nQueues) {
        val ist = queueIdx[qi]

        // Build X_m(n) for this station
        val Xm = DoubleArray(stateSpaceSize)
        Xm[0] = 1.0
        val isLjcdStation = ljcdscalingConv != null && ljcdscalingConv[qi] != null &&
                ljcdscalingConv[qi]!!.any { it != null && it.length() > 0 }

        val n = IntArray(K)
        while (true) {
            val idx = hashpopConv(n, NK)
            if (n.sum() > 0) {
                if (isLjcdStation) {
                    for (r in 0 until K) {
                        if (n[r] > 0) {
                            val cutoffs = ljcdcutoffsConv?.get(qi)
                            val nClamped = Matrix(1, K)
                            val cutoffsMat = Matrix(1, K)
                            for (kk in 0 until K) {
                                val cutVal = cutoffs?.get(kk) ?: NK[kk].toDouble()
                                nClamped.set(kk, min(n[kk].toDouble(), cutVal))
                                cutoffsMat.set(kk, cutVal)
                            }
                            val lidx = ljd_linearize(nClamped, cutoffsMat)
                            val ljcdTable = ljcdscalingConv?.get(qi)?.get(r)
                            val mu_km = if (ljcdTable != null && lidx < ljcdTable.length()) {
                                ljcdTable.get(lidx)
                            } else {
                                1.0
                            }

                            n[r]--
                            val idxPrev = hashpopConv(n, NK)
                            n[r]++
                            if (mu_km > 0) {
                                Xm[idx] = (L_conv.get(qi, r) / mu_km) * Xm[idxPrev]
                            }
                            break
                        }
                    }
                } else {
                    for (r in 0 until K) {
                        if (n[r] > 0) {
                            n[r]--
                            val idxPrev = hashpopConv(n, NK)
                            n[r]++
                            Xm[idx] += L_conv.get(qi, r) * Xm[idxPrev]
                        }
                    }
                }
            }
            if (!pprodNextConv(n, NK)) break
        }

        // Build complement: all stations except qi
        val L_comp = Matrix(nQueues - 1, K)
        val ljcdComp = mutableListOf<List<Matrix?>?>()
        val ljcdcComp = mutableListOf<Matrix?>()
        var ci = 0
        for (qj in 0 until nQueues) {
            if (qj != qi) {
                for (k in 0 until K) {
                    L_comp.set(ci, k, L_conv.get(qj, k))
                }
                ljcdComp.add(ljcdscalingConv?.getOrNull(qj))
                ljcdcComp.add(ljcdcutoffsConv?.getOrNull(qj))
                ci++
            }
        }

        // Compute Q_m_k using marginal
        val nMarg = IntArray(K)
        while (true) {
            if (nMarg.any { it > 0 }) {
                val idx = hashpopConv(nMarg, NK)
                val nmi = IntArray(K) { k -> NK[k] - nMarg[k] }
                if (nmi.all { it >= 0 }) {
                    val resultComp = pfqn_conv(
                        L_comp, nmi, Z_conv, null,
                        if (ljcdComp.isEmpty()) null else ljcdComp,
                        if (ljcdcComp.isEmpty()) null else ljcdcComp
                    )
                    val prob = Xm[idx] * resultComp[0] / G_N
                    for (k in 0 until K) {
                        QN.set(ist, k, QN.get(ist, k) + nMarg[k] * prob)
                    }
                }
            }
            if (!pprodNextConv(nMarg, NK)) break
        }
    }

    // Compute remaining metrics
    val RN = Matrix(M, K)
    for (i in 0 until M) {
        for (k in 0 until K) {
            RN.set(i, k, if (TN.get(i, k) != 0.0) QN.get(i, k) / TN.get(i, k) else 0.0)
        }
    }

    val UN = Matrix(M, K)
    for (i in 0 until M) {
        for (k in 0 until K) {
            UN.set(i, k, TN.get(i, k) * ST.get(i, k))
        }
    }

    val X = Matrix(1, K)
    for (k in 0 until K) X.set(0, k, XN[k])

    val CN = Matrix(1, K)
    for (k in 0 until K) {
        CN.set(0, k, if (XN[k] != 0.0) NK[k].toDouble() / XN[k] else 0.0)
    }

    val runtime = (System.nanoTime() - startTime) / 1e9

    return SolverNC.SolverNCLDReturn(QN, UN, RN, TN, CN, X, lG, runtime, iter, method)
}

private fun hashpopConv(n: IntArray, N: IntArray): Int {
    var idx = 0
    var stride = 1
    for (r in n.indices) {
        idx += stride * n[r]
        stride *= (N[r] + 1)
    }
    return idx
}

private fun pprodNextConv(n: IntArray, N: IntArray): Boolean {
    val R = n.size
    if ((0 until R).all { n[it] == N[it] }) return false
    var s = R - 1
    while (s >= 0 && n[s] == N[s]) {
        n[s] = 0
        s--
    }
    if (s >= 0) n[s]++
    return true
}
