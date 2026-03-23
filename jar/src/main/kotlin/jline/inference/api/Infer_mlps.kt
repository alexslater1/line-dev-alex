/*
 * Copyright (c) 2012-2026, Imperial College London
 * All rights reserved.
 */
package jline.inference.api

import jline.inference.util.OptimUtils
import jline.lang.*
import jline.lang.constant.EventType
import jline.lang.constant.SchedStrategy
import jline.lang.nodes.Delay
import jline.lang.nodes.Queue
import jline.lang.processes.Exp
import jline.solvers.SolverOptions
import jline.solvers.ctmc.SolverCTMC
import jline.solvers.ctmc.handlers.Solver_ctmc.Companion.solver_ctmc
import jline.util.matrix.Matrix
import jline.util.matrix.MatrixCell
import jline.api.mam.map_pdf

/**
 * Pre-built CTMC model information for a unique (tagClass, arrivalQueue) pair.
 */
private data class PrebuiltCTMC(
    val sn: NetworkStruct,
    val queueStIdx: Int,
    val taggedDepIdx: List<Int>,
    val subset: IntArray,
    val SSqueue: Matrix,
    val N: IntArray,
    val tagClass: Int
)

/**
 * MLPS demand estimation using CTMC-based likelihood.
 *
 * Estimates service demands at a PS queue using the Maximum Likelihood
 * for Processor Sharing method. Pre-builds augmented CTMC models for
 * each unique (tagClass, arrivalQueue) combination once, then uses
 * sn_set_service_coc + solver_ctmc directly inside the optimization loop.
 *
 * @param model LINE Network model with delay rates set and queue rates to estimate
 * @param node PS queue node
 * @param rt response time samples (n x 1)
 * @param classVec class of each sample (0-based)
 * @param ql queue lengths at arrival (n x R matrix)
 * @return estimated demands (1 x R array)
 */
fun infer_mlps(model: Network, node: Queue, rt: DoubleArray, classVec: IntArray, ql: Matrix): DoubleArray {
    val sn = model.getStruct(false)
    val R = sn.nclasses
    val nCores = node.numberOfServers

    // Get delay rates from model
    var delayIdx = -1
    for (i in 0 until sn.nstations) {
        val station = sn.stations[i]
        if (sn.sched[station] == SchedStrategy.INF) {
            delayIdx = i
            break
        }
    }
    val muZ = DoubleArray(R) { k ->
        val station = sn.stations[delayIdx]
        val jobclass = sn.jobclasses[k]
        sn.mu[station]!![jobclass]!!.get(0, 0)
    }

    // Initial point estimate
    val meanQL = (0 until ql.numRows).map { i ->
        (0 until ql.numCols).sumOf { j -> ql.get(i, j) }
    }.average()
    val Vtilde = Math.min(meanQL, nCores.toDouble())
    val x0 = DoubleArray(R) { j ->
        val classRT = (0 until rt.size).filter { classVec[it] == j }.map { rt[it] }
        if (classRT.isNotEmpty()) Vtilde * classRT.average() / meanQL else 1e-3
    }

    val xLB = DoubleArray(R) { 1e-10 }
    val xUB = DoubleArray(R) { rt.max() }

    val newR = R + 1
    val augMuZ = DoubleArray(newR)
    for (k in 0 until R) augMuZ[k] = muZ[k]

    // Collect unique (tagClass, arrivalQueue) pairs
    val uniqueTC = classVec.toSet().sorted()
    val uniqueQLRows = LinkedHashSet<String>()
    for (i in 0 until ql.numRows) {
        val row = IntArray(R) { j -> ql.get(i, j).toInt() }
        uniqueQLRows.add(row.joinToString(","))
    }

    var ctmcOpts: SolverOptions? = null
    val prebuilt = HashMap<String, PrebuiltCTMC>()

    for (tc in uniqueTC) {
        augMuZ[newR - 1] = muZ[tc]
        for (qlRowStr in uniqueQLRows) {
            val aq = qlRowStr.split(",").map { it.toInt() }.toIntArray()

            // Population: move 1 job from tagClass to tagged class
            val N = IntArray(newR)
            for (k in 0 until R) N[k] = aq[k]
            N[tc] = N[tc] - 1
            N[newR - 1] = 1

            if (N[tc] < 0) continue // skip invalid combinations

            // Build augmented Network model
            val augModel = Network("mlps_aug")
            val augDelay = Delay(augModel, "Think")
            val augQueue = Queue(augModel, "Queue1", SchedStrategy.PS)
            augQueue.setNumberOfServers(nCores)

            val augClasses = arrayOfNulls<ClosedClass>(newR)
            for (r in 0 until newR) {
                augClasses[r] = ClosedClass(augModel, "Class${r + 1}", N[r], augDelay, 0)
                augDelay.setService(augClasses[r], Exp(augMuZ[r]))
                augQueue.setService(augClasses[r], Exp(1.0)) // placeholder
            }

            val P = augModel.initRoutingMatrix()
            val nodeList = listOf(augDelay, augQueue)
            for (r in 0 until newR) {
                P.set(augClasses[r], Network.serialRouting(nodeList))
            }
            augModel.link(P)

            // Use SolverCTMC to get proper options
            val solver = SolverCTMC(augModel)
            if (ctmcOpts == null) ctmcOpts = solver.options

            val augSn = augModel.getStruct(false)

            // Run initial solver_ctmc to get state space structure
            val ctmcResult = solver_ctmc(augSn, ctmcOpts!!)
            val stateSpaceAggr = ctmcResult.stateSpaceAggr

            // Find tagged departure event indices from sync structure
            // MATLAB: ev{e}.active{1}.node == queueNodeIdx && ev{e}.active{1}.class == newR && ev{e}.active{1}.event == EventType.DEP
            val queueStIdx = augQueue.getStationIdx()
            val queueNodeIdx = augQueue.nodeIndex
            val taggedDepIdx = ArrayList<Int>()
            val sync = augSn.sync
            if (sync != null) {
                for (e in 0 until sync.size) {
                    val activeEvent = sync[e]?.active?.get(0) ?: continue
                    if (activeEvent.node == queueNodeIdx &&
                        activeEvent.jobClass == newR - 1 &&
                        activeEvent.event == EventType.DEP) {
                        taggedDepIdx.add(e)
                    }
                }
            }

            // Find subset where tagged job is at Queue
            val taggedColAtQueue = queueStIdx * newR + (newR - 1)
            val subsetList = ArrayList<Int>()
            if (stateSpaceAggr != null) {
                for (row in 0 until stateSpaceAggr.numRows) {
                    if (taggedColAtQueue < stateSpaceAggr.numCols &&
                        stateSpaceAggr.get(row, taggedColAtQueue).toInt() == 1
                    ) {
                        subsetList.add(row)
                    }
                }
            }
            val subset = subsetList.toIntArray()

            // Extract queue state space for subset
            val queueCols = IntArray(newR) { r -> queueStIdx * newR + r }
            val SSqueue = Matrix(subset.size, newR)
            for (si in subset.indices) {
                for (r in 0 until newR) {
                    if (queueCols[r] < stateSpaceAggr!!.numCols) {
                        SSqueue.set(si, r, stateSpaceAggr.get(subset[si], queueCols[r]))
                    }
                }
            }

            val cacheKey = "$tc,${aq.joinToString(",")}"
            prebuilt[cacheKey] = PrebuiltCTMC(augSn, queueStIdx, taggedDepIdx, subset, SSqueue, N, tc)
        }
    }

    val finalCtmcOpts = ctmcOpts ?: SolverCTMC.defaultOptions()

    // Optimization
    val (demandEst, _) = OptimUtils.fmincon(
        objFun = { x ->
            val TOL = 1e-6
            val rates = DoubleArray(R) { 1.0 / x[it] }

            // Update rates and re-solve for each cached configuration
            data class CachedResult(val A: Matrix, val SSqueue: Matrix, val N: IntArray)
            val cache = HashMap<String, CachedResult>()

            for ((key, pb) in prebuilt) {
                val augRates = DoubleArray(newR)
                for (k in 0 until R) augRates[k] = rates[k]
                augRates[newR - 1] = rates[pb.tagClass]

                // Update service rates in cached sn struct
                val snUpd = pb.sn // Note: modifies in-place
                for (cc in 0 until newR) {
                    sn_set_service_coc(snUpd, pb.queueStIdx, cc, augRates[cc])
                }

                // Re-run solver_ctmc with updated rates
                val ctmcResult = solver_ctmc(snUpd, finalCtmcOpts)
                val infGen = ctmcResult.q

                // Build D1 from departure event filters
                val Dfilt = ctmcResult.dfilt
                val nStates = infGen.numRows
                val D1 = Matrix(nStates, nStates)
                if (Dfilt != null) {
                    for (di in pb.taggedDepIdx) {
                        if (di < Dfilt.size()) {
                            val dfiltMat = Dfilt.get(di) as? Matrix ?: continue
                            for (ii in 0 until Math.min(nStates, dfiltMat.numRows)) {
                                for (jj in 0 until Math.min(nStates, dfiltMat.numCols)) {
                                    D1.set(ii, jj, D1.get(ii, jj) + dfiltMat.get(ii, jj))
                                }
                            }
                        }
                    }
                }

                // Extract sub-generator using cached subset
                val subSize = pb.subset.size
                if (subSize > 0) {
                    val MAPQ1 = Matrix(nStates, nStates)
                    for (ii in 0 until nStates) {
                        for (jj in 0 until nStates) {
                            MAPQ1.set(ii, jj, infGen.get(ii, jj) - D1.get(ii, jj))
                        }
                    }
                    val A = Matrix(subSize, subSize)
                    for (ii in 0 until subSize) {
                        for (jj in 0 until subSize) {
                            A.set(ii, jj, MAPQ1.get(pb.subset[ii], pb.subset[jj]))
                        }
                    }
                    cache[key] = CachedResult(A, pb.SSqueue, pb.N)
                }
            }

            // Compute likelihoods
            var totalLogLike = 0.0
            for (i in rt.indices) {
                val tc = classVec[i]
                val aqRow = IntArray(R) { j -> ql.get(i, j).toInt() }
                val cacheKey = "$tc,${aqRow.joinToString(",")}"
                val cached = cache[cacheKey]
                if (cached != null) {
                    val like = evalMlpsLikelihood(cached.A, cached.SSqueue, cached.N, rt[i])
                    totalLogLike += Math.log(TOL + like)
                } else {
                    totalLogLike += Math.log(TOL)
                }
            }
            -totalLogLike
        },
        x0 = x0,
        lb = xLB,
        ub = xUB,
        maxIter = 10000
    )

    return demandEst
}

/**
 * Compute MLPS likelihood from pre-built CTMC components.
 */
private fun evalMlpsLikelihood(A: Matrix, SSqueue: Matrix, N: IntArray, Rsam: Double): Double {
    val nStates = A.numRows
    if (nStates == 0) return 0.0

    // Find initial state: row in SSqueue matching N
    val pie = Matrix(1, nStates)
    for (row in 0 until SSqueue.numRows) {
        var matches = true
        for (col in 0 until Math.min(SSqueue.numCols, N.size)) {
            if (SSqueue.get(row, col).toInt() != N[col]) {
                matches = false
                break
            }
        }
        if (matches) {
            pie.set(0, row, 1.0)
            break
        }
    }

    // Compute MAP PDF: f(t) = pie * exp(A*t) * (-A*1) * pie
    // Using the MAP representation {A, -A*ones*pie}
    val D0 = A
    val rowSums = Matrix(nStates, 1)
    for (i in 0 until nStates) {
        var sum = 0.0
        for (j in 0 until nStates) {
            sum += A.get(i, j)
        }
        rowSums.set(i, 0, -sum)
    }

    // D1 = (-A*ones) * pie
    val D1 = Matrix(nStates, nStates)
    for (i in 0 until nStates) {
        for (j in 0 until nStates) {
            D1.set(i, j, rowSums.get(i, 0) * pie.get(0, j))
        }
    }

    // Use map_pdf to compute the PDF
    val MAP = MatrixCell(2)
    MAP.set(0, D0)
    MAP.set(1, D1)
    return map_pdf(MAP, Rsam)
}
