package jline.solvers.mam.handlers

import jline.api.mam.map_mean
import jline.api.mam.map_scv
import jline.lang.NetworkStruct
import jline.lang.constant.NodeType
import jline.lang.constant.SchedStrategy
import jline.solvers.SolverOptions
import jline.solvers.mam.MAMResult
import jline.util.matrix.Matrix
import kotlin.math.max

private data class FJSyncGroup(
    val forkIdx: Int,
    val joinIdx: Int,
    val branchNodes: IntArray
)

fun solver_mam_basic_mmap(sn: NetworkStruct, options: SolverOptions): MAMResult {
    val M = sn.nstations
    val K = sn.nclasses
    val C = sn.nchains

    val QN = Matrix(M, K, M * K)
    val UN = Matrix(M, K, M * K)
    val RN = Matrix(M, K, M * K)
    val TN = Matrix(M, K, M * K)
    val CN = Matrix(1, K, K)
    val XN = Matrix(1, K, K)

    val lambda = DoubleArray(K)
    for (c in 0 until C) {
        val inchain = sn.inchain[c] ?: continue
        if (inchain.length() == 0) {
            continue
        }
        val refClass = inchain.get(0).toInt()
        val refStation = sn.refstat.get(refClass).toInt()
        var chainLambda = 0.0
        for (i in 0 until inchain.length()) {
            val jobClass = inchain.get(i).toInt()
            val rate = sn.rates.get(refStation, jobClass)
            if (rate.isFinite()) {
                chainLambda += rate
            }
        }
        for (i in 0 until inchain.length()) {
            lambda[inchain.get(i).toInt()] = chainLambda
        }
    }

    val syncGroups = buildFJSyncGroups(sn)
    val branchGroupByNode = HashMap<Int, FJSyncGroup>()
    for (group in syncGroups) {
        for (branchNode in group.branchNodes) {
            branchGroupByNode[branchNode] = group
        }
    }

    for (ist in 0 until M) {
        val ind = sn.stationToNode.get(ist).toInt()
        when (sn.nodetype[ind]) {
            NodeType.Join -> {
                for (r in 0 until K) {
                    if (lambda[r] <= 0) {
                        continue
                    }
                    val groupsAtJoin = syncGroups.filter { it.joinIdx == ind }
                    var syncDelay = 0.0
                    var joinArrivalRate = 0.0
                    for (group in groupsAtJoin) {
                        val branchRt = mutableListOf<Double>()
                        var branchArrivalRate = 0.0
                        for (branchNode in group.branchNodes) {
                            val branchStation = sn.nodeToStation.get(branchNode).toInt()
                            if (branchStation < 0) {
                                continue
                            }
                            val rt = RN.get(branchStation, r)
                            val tput = TN.get(branchStation, r)
                            if (rt > 0 && tput > 0) {
                                branchRt.add(rt)
                                branchArrivalRate += tput
                            }
                        }
                        if (branchRt.size < 2) {
                            continue
                        }
                        val lambdai = branchRt.map { 1.0 / it }.toDoubleArray()
                        val maxBranchRt = inclusionExclusionMaxMean(lambdai)
                        syncDelay += max(maxBranchRt - branchRt.average(), 0.0)
                        joinArrivalRate += branchArrivalRate
                    }
                    TN.set(ist, r, lambda[r])
                    RN.set(ist, r, syncDelay)
                    QN.set(ist, r, joinArrivalRate * syncDelay)
                    UN.set(ist, r, 0.0)
                    XN.set(0, r, lambda[r])
                }
            }
            NodeType.Queue, NodeType.Delay -> {
                val group = branchGroupByNode[ind]
                for (r in 0 until K) {
                    val arrivalRate = when {
                        lambda[r] <= 0 -> 0.0
                        group != null -> lambda[r] * sn.rtnodes.get(group.forkIdx * K + r, ind * K + r)
                        sn.sched[sn.stations[ist]] == SchedStrategy.INF -> lambda[r]
                        else -> 0.0
                    }
                    if (arrivalRate <= 0) {
                        continue
                    }

                    val station = sn.stations[ist]
                    val proc = sn.proc[station]?.get(sn.jobclasses[r])
                    val rawServiceMean = when {
                        proc != null && proc.size() >= 2 -> map_mean(proc[0], proc[1])
                        sn.rates.get(ist, r).isFinite() && sn.rates.get(ist, r) > 0 -> 1.0 / sn.rates.get(ist, r)
                        else -> 0.0
                    }
                    val serviceScv = when {
                        proc != null && proc.size() >= 2 -> map_scv(proc[0], proc[1])
                        else -> 1.0
                    }
                    val servers = max(sn.nservers.get(ist), 1.0)
                    val serviceMean = rawServiceMean / servers
                    val rho = arrivalRate * serviceMean

                    TN.set(ist, r, arrivalRate)
                    XN.set(0, r, lambda[r])

                    when (sn.sched[station]) {
                        SchedStrategy.INF -> {
                            RN.set(ist, r, rawServiceMean)
                            QN.set(ist, r, arrivalRate * rawServiceMean)
                            UN.set(ist, r, arrivalRate * rawServiceMean)
                        }
                        SchedStrategy.PS -> {
                            if (rho >= 1.0) {
                                RN.set(ist, r, Double.POSITIVE_INFINITY)
                                QN.set(ist, r, Double.POSITIVE_INFINITY)
                                UN.set(ist, r, 1.0)
                            } else {
                                val responseTime = serviceMean / (1.0 - rho)
                                RN.set(ist, r, responseTime)
                                QN.set(ist, r, arrivalRate * responseTime)
                                UN.set(ist, r, rho)
                            }
                        }
                        else -> {
                            if (rho >= 1.0) {
                                RN.set(ist, r, Double.POSITIVE_INFINITY)
                                QN.set(ist, r, Double.POSITIVE_INFINITY)
                                UN.set(ist, r, 1.0)
                            } else {
                                val secondMoment = (1.0 + serviceScv) * serviceMean * serviceMean
                                val responseTime = serviceMean + arrivalRate * secondMoment / (2.0 * (1.0 - rho))
                                RN.set(ist, r, responseTime)
                                QN.set(ist, r, arrivalRate * responseTime)
                                UN.set(ist, r, rho)
                            }
                        }
                    }
                }
            }
            else -> {
                if (sn.sched[sn.stations[ist]] == SchedStrategy.EXT) {
                    for (r in 0 until K) {
                        val tput = sn.rates.get(ist, r)
                        if (!tput.isNaN()) {
                            TN.set(ist, r, tput)
                            XN.set(0, r, lambda[r])
                        }
                    }
                }
            }
        }
    }

    for (r in 0 until K) {
        var classResponse = 0.0
        for (ist in 0 until M) {
            val value = RN.get(ist, r)
            if (value.isFinite()) {
                classResponse += value
            }
        }
        CN.set(0, r, classResponse)
    }

    return MAMResult().apply {
        this.QN = QN
        this.UN = UN
        this.RN = RN
        this.TN = TN
        this.CN = CN
        this.XN = XN
        this.iter = 1
    }
}

private fun buildFJSyncGroups(sn: NetworkStruct): List<FJSyncGroup> {
    val groups = mutableListOf<FJSyncGroup>()
    for (forkIdx in 0 until sn.fj.numRows) {
        for (joinIdx in 0 until sn.fj.numCols) {
            if (sn.fj.get(forkIdx, joinIdx) <= 0) {
                continue
            }
            val branchNodes = mutableListOf<Int>()
            for (nodeIdx in 0 until sn.nnodes) {
                if (nodeIdx == forkIdx || nodeIdx == joinIdx) {
                    continue
                }
                var forkRoutesToNode = false
                var nodeRoutesToJoin = false
                for (r in 0 until sn.nclasses) {
                    if (sn.rtnodes.get(forkIdx * sn.nclasses + r, nodeIdx * sn.nclasses + r) > 0) {
                        forkRoutesToNode = true
                    }
                    if (sn.rtnodes.get(nodeIdx * sn.nclasses + r, joinIdx * sn.nclasses + r) > 0) {
                        nodeRoutesToJoin = true
                    }
                }
                if (forkRoutesToNode && nodeRoutesToJoin) {
                    branchNodes.add(nodeIdx)
                }
            }
            if (branchNodes.isNotEmpty()) {
                groups.add(FJSyncGroup(forkIdx, joinIdx, branchNodes.toIntArray()))
            }
        }
    }
    return groups
}

private fun inclusionExclusionMaxMean(lambdai: DoubleArray): Double {
    var total = 0.0
    val n = lambdai.size
    for (mask in 1 until (1 shl n)) {
        var subsetSum = 0.0
        var bits = 0
        for (i in 0 until n) {
            if ((mask and (1 shl i)) != 0) {
                subsetSum += lambdai[i]
                bits++
            }
        }
        if (subsetSum <= 0) {
            continue
        }
        val term = 1.0 / subsetSum
        total += if (bits % 2 == 1) term else -term
    }
    return total
}
