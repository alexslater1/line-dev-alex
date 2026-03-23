/*
 * Copyright (c) 2012-2026, Imperial College London
 * All rights reserved.
 */
package jline.inference.lang

import jline.inference.api.*
import jline.inference.util.NnlsSolver
import jline.inference.util.OptimUtils
import jline.inference.util.SplineInterpolator
import jline.lang.*
import jline.lang.constant.MetricType
import jline.lang.constant.EventType
import jline.lang.constant.SchedStrategy
import jline.lang.nodes.*
import jline.lang.processes.Det
import jline.lang.processes.Exp
import jline.lang.processes.Markovian
import jline.solvers.NetworkSolver
import jline.solvers.SolverOptions
import jline.solvers.SolverResult
import jline.solvers.mva.SolverMVA
import jline.solvers.mva.analyzers.solver_mva_analyzer
import jline.solvers.ctmc.analyzers.Solver_ctmc_analyzer.Companion.solver_ctmc_analyzer
import jline.solvers.nc.analyzers.solver_nc_analyzer
import jline.util.matrix.Matrix

/**
 * Options for the ParamEstimator.
 */
data class EstimatorOptions(
    var verbose: Int = 1,
    var method: String = "ubr",
    var variant: String = "default",
    var iterMax: Int = 1000,
    var tol: Double = 1e-3,
    var solverFactory: ((Network) -> NetworkSolver)? = null,
    var openPopulation: Int = 100,
    var x0: DoubleArray? = null
) {
    companion object {
        @JvmStatic
        fun defaultOptions(): EstimatorOptions = EstimatorOptions()
    }
}

/**
 * Service demand estimator for queueing network models.
 *
 * Manages sampled data and dispatches to specific estimation methods
 * including UBR, UBO, ERPS, EKF, MCMC, MLE, MLPS, FMLPS, QMLE, and Gibbs.
 *
 * @param model the Network model to estimate parameters for
 * @param options estimator configuration options
 */
class ParamEstimator(
    val model: Network,
    val options: EstimatorOptions = EstimatorOptions.defaultOptions()
) {
    /** Per-class samples indexed by [nodeIndex][classIndex] */
    val samples: Array<Array<MutableList<SampledMetric>>>

    /** Aggregate samples indexed by [nodeIndex] */
    val samplesAggr: Array<MutableList<SampledMetric>>

    init {
        val nNodes = model.numberOfNodes
        val nClasses = model.numberOfClasses
        samples = Array(nNodes) { Array(nClasses) { mutableListOf<SampledMetric>() } }
        samplesAggr = Array(nNodes) { mutableListOf<SampledMetric>() }
    }

    /**
     * Add a sampled metric to the estimator.
     */
    fun addSamples(sampleData: SampledMetric) {
        val i = model.getNodeIndex(sampleData.node)
        if (sampleData.isAggregate()) {
            samplesAggr[i].add(sampleData)
        } else {
            val r = model.getClassIndex(sampleData.jobClass)
            samples[i][r].add(sampleData)
        }
    }

    /**
     * Retrieve per-class sampled data.
     */
    fun getData(): Array<Array<MutableList<SampledMetric>>> = samples

    /**
     * Retrieve aggregate sampled data.
     */
    fun getDataAggr(): Array<MutableList<SampledMetric>> = samplesAggr

    /**
     * Get the first ArvR dataset for a given node and class.
     */
    fun getArvR(node: Node, jobClass: JobClass): SampledMetric? {
        val i = model.getNodeIndex(node)
        val r = model.getClassIndex(jobClass)
        return samples[i][r].firstOrNull { it.type == MetricType.ArvR }
    }

    /**
     * Get the first Util dataset for a given node and class.
     */
    fun getUtil(node: Node, jobClass: JobClass): SampledMetric? {
        val i = model.getNodeIndex(node)
        val r = model.getClassIndex(jobClass)
        return samples[i][r].firstOrNull { it.type == MetricType.Util }
    }

    /**
     * Get the first RespT dataset for a given node and class.
     */
    fun getRespT(node: Node, jobClass: JobClass): SampledMetric? {
        val i = model.getNodeIndex(node)
        val r = model.getClassIndex(jobClass)
        return samples[i][r].firstOrNull { it.type == MetricType.RespT }
    }

    /**
     * Get the first aggregate Util dataset for a given node.
     */
    fun getAggrUtil(node: Node): SampledMetric? {
        val i = model.getNodeIndex(node)
        return samplesAggr[i].firstOrNull { it.type == MetricType.Util }
    }

    /**
     * Get QLen datasets for a given node and class.
     * If ev is provided, returns only the conditional match.
     * Otherwise returns all QLen datasets (or the single one if there's only one).
     */
    fun getQLen(node: Node, jobClass: JobClass, ev: ConditionEvent? = null): List<SampledMetric> {
        val i = model.getNodeIndex(node)
        val r = model.getClassIndex(jobClass)
        val nodeData = samples[i][r]

        return if (ev == null) {
            nodeData.filter { it.type == MetricType.QLen }
        } else {
            nodeData.filter { sm ->
                sm.type == MetricType.QLen && sm.cond != null &&
                        sm.cond!!.node == ev.node &&
                        sm.cond!!.`class` == ev.`class` &&
                        sm.cond!!.event == ev.event
            }
        }
    }

    /**
     * Get the first aggregate QLen dataset for a given node.
     */
    fun getAggrQLen(node: Node, ev: ConditionEvent? = null): SampledMetric? {
        val i = model.getNodeIndex(node)
        val nodeData = samplesAggr[i]

        return if (ev == null) {
            nodeData.firstOrNull { it.type == MetricType.QLen }
        } else {
            nodeData.firstOrNull { sm ->
                sm.type == MetricType.QLen && sm.cond != null &&
                        sm.cond!!.node == ev.node &&
                        sm.cond!!.`class` == ev.`class` &&
                        sm.cond!!.event == ev.event
            }
        }
    }

    /**
     * Get the first Tput dataset for a given node and class.
     */
    fun getTput(node: Node, jobClass: JobClass): SampledMetric? {
        val i = model.getNodeIndex(node)
        val r = model.getClassIndex(jobClass)
        return samples[i][r].firstOrNull { it.type == MetricType.Tput }
    }

    /**
     * Automatically select the best estimation method based on available data.
     */
    fun autoMethod(): String {
        var hasArvR = false; var hasRespT = false; var hasUtil = false
        var hasQLen = false; var hasTput = false; var hasTrace = false
        var hasAggrUtil = false; var hasAggrQLen = false

        for (i in samples.indices) {
            for (r in samples[i].indices) {
                for (sm in samples[i][r]) {
                    when (sm.type) {
                        MetricType.ArvR -> hasArvR = true
                        MetricType.RespT -> hasRespT = true
                        MetricType.Util -> hasUtil = true
                        MetricType.QLen -> hasQLen = true
                        MetricType.Tput -> hasTput = true
                        else -> {}
                    }
                    if (sm.isTrace()) hasTrace = true
                }
            }
        }

        for (i in samplesAggr.indices) {
            for (sm in samplesAggr[i]) {
                when (sm.type) {
                    MetricType.Util -> hasAggrUtil = true
                    MetricType.QLen -> hasAggrQLen = true
                    else -> {}
                }
            }
        }

        val method = when {
            hasTrace && hasRespT && hasAggrQLen -> "erps"
            hasTrace && hasRespT && hasArvR -> "mlps"
            hasArvR && hasRespT && (hasUtil || hasAggrUtil) -> "ubo"
            hasArvR && (hasUtil || hasAggrUtil) -> "ubr"
            hasQLen -> "qmle"
            else -> throw IllegalStateException(
                "Insufficient data to automatically select an estimation method. Please set options.method manually."
            )
        }

        options.method = method
        return method
    }

    /**
     * Interpolate all data across all the available timestamps using spline interpolation.
     */
    fun interpolate() {
        // Collect union of all timestamps
        val tUnion = java.util.TreeSet<Double>()

        for (i in samples.indices) {
            for (r in samples[i].indices) {
                for (sm in samples[i][r]) {
                    for (t in sm.t) tUnion.add(t)
                }
            }
        }
        for (i in samplesAggr.indices) {
            for (sm in samplesAggr[i]) {
                for (t in sm.t) tUnion.add(t)
            }
        }

        val tNew = tUnion.toDoubleArray()
        if (tNew.size < 2) return

        // Interpolate all per-class samples
        for (i in samples.indices) {
            for (r in samples[i].indices) {
                for (sm in samples[i][r]) {
                    if (sm.t.size >= 2) {
                        sm.data = SplineInterpolator.interpolate(sm.t, sm.data, tNew)
                        sm.t = tNew.copyOf()
                    }
                }
            }
        }

        // Interpolate all aggregate samples
        for (i in samplesAggr.indices) {
            for (sm in samplesAggr[i]) {
                if (sm.t.size >= 2) {
                    sm.data = SplineInterpolator.interpolate(sm.t, sm.data, tNew)
                    sm.t = tNew.copyOf()
                }
            }
        }
    }

    /**
     * Run the estimation at the specified nodes and update the model parameters.
     *
     * @param nodes list of stations to estimate
     * @return estimated demands matrix (nNodes x nClasses)
     */
    fun estimateAt(nodes: List<Station>): Matrix {
        val sn = model.getStruct(false)

        val estVal = when (options.method) {
            "ubr" -> estimatorUbr(nodes)
            "ubo" -> estimatorUbo(nodes)
            "erps" -> estimatorErps(nodes)
            "ekf" -> estimatorEkf(nodes)
            "mcmc" -> estimatorMcmc(nodes)
            "mle" -> estimatorMle(nodes)
            "mlps" -> estimatorMlps(nodes)
            "fmlps" -> estimatorFmlps(nodes)
            "qmle" -> estimatorQmle(nodes)
            "gibbs" -> estimatorGibbs(nodes)
            else -> throw IllegalArgumentException("Unknown inference method: ${options.method}")
        }

        // Update model parameters, preserving original distribution type
        val jobClasses = model.jobClasses
        for (n in nodes.indices) {
            val nd = nodes[n]
            if (nd is Source || nd is Sink) continue
            for (r in 0 until sn.nclasses) {
                val v = estVal.get(n, r)
                if (v.isNaN() || v <= 0) continue
                val existingDist = (nd as? Queue)?.getService(jobClasses[r])
                if (existingDist is Markovian) {
                    existingDist.setMean(v)
                } else if (existingDist is Det) {
                    existingDist.setMean(v)
                } else {
                    nd.setService(jobClasses[r], Exp.fitMean(v))
                }
            }
        }
        model.reset()

        return estVal
    }

    // ==================== Estimator Methods ====================

    /**
     * UBR: Utilization-Based Regression.
     */
    private fun estimatorUbr(nodes: List<Station>): Matrix {
        val node = nodes[0]
        val sn = model.getStruct(false)
        val nClasses = sn.nclasses
        val jobClasses = model.jobClasses

        // Get aggregate utilization
        var avgAggrUtil: DoubleArray? = null
        if ((node as? Queue)?.numberOfServers?.let { it > 0 && it < Int.MAX_VALUE } == true) {
            val U = getAggrUtil(node)
            if (U != null) {
                avgAggrUtil = DoubleArray(U.data.size) { U.data[it] * node.numberOfServers }
            }
        }

        // Per-class metrics
        val isUtilKnown = BooleanArray(nClasses)
        val avgUtil = arrayOfNulls<DoubleArray>(nClasses)
        val avgArvR = arrayOfNulls<DoubleArray>(nClasses)

        for (r in 0 until nClasses) {
            val Ur = getUtil(node, jobClasses[r])
            if (Ur != null) {
                val nServers = (node as? Queue)?.numberOfServers ?: 1
                avgUtil[r] = DoubleArray(Ur.data.size) { Ur.data[it] * nServers }
                isUtilKnown[r] = true
            }
            avgArvR[r] = getArvR(node, jobClasses[r])?.data
                ?: throw IllegalStateException("Arrival rate data missing for class $r")
        }

        val nSamples = avgArvR[0]!!.size
        val estVal = Matrix(1, nClasses)

        // For known per-class utilizations, do separate NNLS
        for (r in 0 until nClasses) {
            if (isUtilKnown[r]) {
                val A = Array(nSamples) { doubleArrayOf(avgArvR[r]!![it]) }
                val result = NnlsSolver.lsqnonneg(A, avgUtil[r]!!)
                estVal.set(0, r, result[0])
            }
        }

        // For remaining classes, use aggregate utilization
        val unknownClasses = (0 until nClasses).filter { !isUtilKnown[it] }
        if (unknownClasses.isNotEmpty() && avgAggrUtil != null) {
            // Subtract known utilizations from aggregate
            val residualUtil = avgAggrUtil.copyOf()
            for (r in 0 until nClasses) {
                if (isUtilKnown[r]) {
                    val sumUr = avgUtil[r]!!
                    for (i in residualUtil.indices) {
                        residualUtil[i] -= sumUr[i]
                    }
                }
            }

            val A = Array(nSamples) { i ->
                DoubleArray(unknownClasses.size) { j -> avgArvR[unknownClasses[j]]!![i] }
            }
            val result = NnlsSolver.lsqnonneg(A, residualUtil)
            for (j in unknownClasses.indices) {
                estVal.set(0, unknownClasses[j], result[j])
            }
        }

        return estVal
    }

    /**
     * UBO: Utilization-Based Optimization (Liu et al. 2006).
     */
    private fun estimatorUbo(nodes: List<Station>): Matrix {
        val sn = model.getStruct(false)
        val R = sn.nclasses
        val M = nodes.size
        val MR = M * R
        val jobClasses = model.jobClasses

        // Collect measurements
        val avgU = Array<DoubleArray?>(M) { null }
        val avgArvRData = Array(M) { arrayOfNulls<DoubleArray>(R) }
        val avgRespTData = Array(M) { arrayOfNulls<DoubleArray>(R) }

        for (n in 0 until M) {
            val node = nodes[n]
            val nServers = (node as? Queue)?.numberOfServers ?: 1
            val U = getAggrUtil(node)
            if (U != null) {
                avgU[n] = DoubleArray(U.data.size) { U.data[it] * nServers }
            }
            for (r in 0 until R) {
                avgArvRData[n][r] = getArvR(node, jobClasses[r])?.data
                    ?: throw IllegalStateException("Arrival rate data missing")
                avgRespTData[n][r] = getRespT(node, jobClasses[r])?.data
                    ?: throw IllegalStateException("Response time data missing")
            }
        }

        val N = avgArvRData[0][0]!!.size // number of experiments

        // Build Bundle QP
        val H = Matrix(MR, MR)
        val h = Matrix(MR, 1)

        for (n in 0 until N) {
            val rhoN = DoubleArray(M) { avgU[it]!![n] }
            val betaN = DoubleArray(M) { 1.0 / (1.0 - rhoN[it]) }

            val lambdaN = Array(M) { i -> DoubleArray(R) { r -> avgArvRData[i][r]!![n] } }
            val RN = Array(M) { i -> DoubleArray(R) { r -> avgRespTData[i][r]!![n] } }

            val lambdaR = DoubleArray(R) { r -> (0 until M).sumOf { i -> lambdaN[i][r] } }
            val totalLambda = lambdaR.sum()
            val wN = DoubleArray(R) { if (totalLambda > 0) lambdaR[it] / totalLambda else 1.0 / R }

            val EN = DoubleArray(R) { r -> (0 until M).sumOf { i -> RN[i][r] } }

            // Build matrices and accumulate QP
            // A_delta: R x MR, A_eps: M x MR
            for (r1 in 0 until R) {
                for (r2 in 0 until R) {
                    for (i1 in 0 until M) {
                        for (i2 in 0 until M) {
                            val idx1 = r1 * M + i1
                            val idx2 = r2 * M + i2
                            // From delta term: w_r * beta_i1 * beta_i2 (if r1==r2)
                            if (r1 == r2) {
                                H.set(idx1, idx2, H.get(idx1, idx2) + 2.0 * wN[r1] * betaN[i1] * betaN[i2])
                            }
                            // From epsilon term: lambda_i1r1 * lambda_i2r2 (if i1==i2)
                            if (i1 == i2) {
                                H.set(idx1, idx2, H.get(idx1, idx2) + 2.0 * lambdaN[i1][r1] * lambdaN[i2][r2])
                            }
                        }
                    }
                }
            }

            for (r in 0 until R) {
                for (i in 0 until M) {
                    val idx = r * M + i
                    h.set(idx, 0, h.get(idx, 0) - 2.0 * wN[r] * betaN[i] * EN[r])
                    h.set(idx, 0, h.get(idx, 0) - 2.0 * lambdaN[i][r] * rhoN[i])
                }
            }
        }

        // Solve QP: min 0.5 x'Hx + h'x s.t. x >= 0
        val lb = Matrix(MR, 1)
        val (sVec, _) = OptimUtils.quadprog(H, h, lb)

        // Reshape to M x R
        val estVal = Matrix(M, R)
        for (r in 0 until R) {
            for (i in 0 until M) {
                estVal.set(i, r, sVec.get(r * M + i, 0))
            }
        }
        return estVal
    }

    /**
     * ERPS: Extended Regression for Processor Sharing.
     */
    private fun estimatorErps(nodes: List<Station>): Matrix {
        val node = nodes[0]
        val sn = model.getStruct(false)
        val R = sn.nclasses
        val jobClasses = model.jobClasses

        if ((node as? Queue)?.let { sn.sched[sn.stations[it.getStationIdx()]] } != SchedStrategy.PS) {
            throw IllegalStateException("ERPS method is only available for PS stations.")
        }

        val avgRespT = arrayOfNulls<DoubleArray>(R)
        val avgAQLen = arrayOfNulls<Matrix>(R)

        for (r in 0 until R) {
            val rtData = getRespT(node, jobClasses[r])
                ?: throw IllegalStateException("Response time data missing for class $r")
            avgRespT[r] = rtData.data

            val qlData = getAggrQLen(node, ConditionEvent(node, jobClasses[r], EventType.ARV))
                ?: throw IllegalStateException("Arrival queue-length data missing for class $r")
            avgAQLen[r] = Matrix(qlData.data.size, 1).also { m ->
                for (i in qlData.data.indices) m.set(i, 0, qlData.data[i])
            }
        }

        // Estimate average busy cores
        val busyCoresList = ArrayList<Double>()
        for (r in 0 until R) {
            for (i in 0 until avgAQLen[r]!!.numRows) {
                busyCoresList.add(avgAQLen[r]!!.get(i, 0))
            }
        }
        val nServers = (node as? Queue)?.numberOfServers ?: 1
        val avgBusyCores = Math.min(busyCoresList.average(), nServers.toDouble())

        val estVal = Matrix(1, R)
        for (r in 0 until R) {
            val respTimes = avgRespT[r]!!
            val totalQL = DoubleArray(respTimes.size) { i ->
                avgAQLen[r]!!.get(i, 0) / avgBusyCores
            }

            val A = Array(respTimes.size) { doubleArrayOf(totalQL[it]) }
            val result = NnlsSolver.lsqnonneg(A, respTimes)
            estVal.set(0, r, result[0])
        }
        return estVal
    }

    /**
     * EKF: Extended Kalman Filter.
     */
    private fun estimatorEkf(nodes: List<Station>): Matrix {
        val node = nodes[0]
        val sn = model.getStruct(false)
        val R = sn.nclasses
        val jobClasses = model.jobClasses

        // Get aggregate utilization
        val nServers = (node as? Queue)?.numberOfServers ?: 1
        val avgU = getAggrUtil(node)?.let { u ->
            DoubleArray(u.data.size) { u.data[it] * nServers }
        } ?: throw IllegalStateException("Aggregate utilization data missing")

        // Per-class metrics
        val avgA = Array(R) { r ->
            getArvR(node, jobClasses[r])?.data
                ?: throw IllegalStateException("Arrival rate data missing for class $r")
        }
        val avgR = Array(R) { r ->
            getRespT(node, jobClasses[r])?.data
                ?: throw IllegalStateException("Response time data missing for class $r")
        }

        val N = avgU.size

        // Get solver analyzer
        val solverAnalyzer = getSolverAnalyzer(model, options)

        // Initial state
        val x0 = options.x0
        var x = if (x0 != null) {
            x0.copyOf()
        } else {
            DoubleArray(R) { r -> Math.random() * avgR[r].max() }
        }
        var p = Array(R) { i -> DoubleArray(R) { j -> if (i == j) x[i] * x[i] else 0.0 } }

        val mCovNoise = Array(R + 1) { i -> DoubleArray(R + 1) { j -> if (i == j) 0.01 else 0.0 } }
        val pCovNoise = Array(R) { i -> DoubleArray(R) { j -> if (i == j) 0.001 else 0.0 } }

        val stIdx = (node as? Station)?.getStationIdx() ?: 0
        val stepBound = 0.6

        for (n in 0 until Math.min(N, options.iterMax)) {
            val xN = x.copyOf()
            val pN = Array(R) { i -> DoubleArray(R) { j -> p[i][j] + pCovNoise[i][j] } }

            val stepUtil = avgU[n]
            val stepResponse = DoubleArray(R) { r -> avgR[r][n] }

            // Get predicted measurement using solver
            val zN = getPredictedMeasurement(xN, R, sn, stIdx, solverAnalyzer)
            val HN = getJacobian(xN, R, sn, stIdx, solverAnalyzer, zN)
            val z = DoubleArray(R + 1) { i -> if (i < R) stepResponse[i] else stepUtil }
            val yN = DoubleArray(R + 1) { z[it] - zN[it] }

            // S = H P H' + R_noise
            val S = Array(R + 1) { i ->
                DoubleArray(R + 1) { j ->
                    var sum = mCovNoise[i][j]
                    for (k in 0 until R) {
                        for (l in 0 until R) {
                            sum += HN[i][k] * pN[k][l] * HN[j][l]
                        }
                    }
                    sum
                }
            }

            // K = P H' S^-1
            val SInv = invertMatrix(S)
            val K = Array(R) { i ->
                DoubleArray(R + 1) { j ->
                    var sum = 0.0
                    for (k in 0 until R) {
                        for (l in 0 until R + 1) {
                            sum += pN[i][k] * HN[l][k] * SInv[l][j]
                        }
                    }
                    sum
                }
            }

            // x = xN + K * yN
            x = DoubleArray(R) { i ->
                var sum = xN[i]
                for (j in 0 until R + 1) {
                    sum += K[i][j] * yN[j]
                }
                val lower = stepBound * 0.0 + (1.0 - stepBound) * sum
                val upper = stepBound * Double.MAX_VALUE + (1.0 - stepBound) * sum
                Math.min(upper, Math.max(lower, sum))
            }

            if (x.sum() < 0) {
                for (i in x.indices) x[i] = -x[i]
            }

            // P = (I - K H) P  (proper matrix multiply)
            val IKH = Array(R) { i -> DoubleArray(R) { j ->
                val kh = (0 until R + 1).sumOf { k -> K[i][k] * HN[k][j] }
                (if (i == j) 1.0 else 0.0) - kh
            }}
            p = Array(R) { i ->
                DoubleArray(R) { j ->
                    (0 until R).sumOf { k -> IKH[i][k] * pN[k][j] }
                }
            }
        }

        val estVal = Matrix(1, R)
        for (r in 0 until R) {
            estVal.set(0, r, x[r])
        }
        return estVal
    }

    /**
     * MCMC: Markov Chain Monte Carlo via Gibbs Sampling.
     */
    private fun estimatorMcmc(nodes: List<Station>): Matrix {
        val sn = model.getStruct(false)
        val R = sn.nclasses
        val jobClasses = model.jobClasses

        // Get effective population for open classes
        val Nopen = options.openPopulation

        val P = DoubleArray(R) { r ->
            val njobs = sn.njobs.get(r)
            if (njobs < Double.MAX_VALUE / 2) njobs else Nopen.toDouble()
        }

        // Extract think times
        val Z = DoubleArray(R)
        val allNodes = model.nodes
        for (nd in allNodes) {
            if (nd is Delay) {
                for (r in 0 until R) {
                    if (sn.njobs.get(r) < Double.MAX_VALUE / 2) {
                        val dist = nd.getService(jobClasses[r])
                        if (dist != null) Z[r] += dist.getMean()
                    }
                }
            } else if (nd is Source) {
                for (r in 0 until R) {
                    if (sn.njobs.get(r) >= Double.MAX_VALUE / 2) {
                        val dist = nd.getArrivalProcess(jobClasses[r])
                        if (dist != null) {
                            val lambdaR = 1.0 / dist.getMean()
                            Z[r] = P[r] / lambdaR
                        }
                    }
                }
            }
        }

        // Get queue-length data
        val M = nodes.size
        val avgQLList = ArrayList<DoubleArray>()
        for (n in 0 until M) {
            val qlData = getAggrQLen(nodes[n])
                ?: throw IllegalStateException("Queue-length data missing for node $n")
            avgQLList.add(qlData.data)
        }

        val experiments = avgQLList[0].size
        val avgQL = Matrix(M, R)
        for (i in 0 until M) {
            for (r in 0 until R) {
                avgQL.set(i, r, avgQLList[i].average())
            }
        }

        // Run MCMC
        val visits = sn.visits
        val estVal = mcmcData(avgQL, visits, experiments, options.iterMax, P, Z)
        return estVal
    }

    /**
     * MLE: Maximum Likelihood Estimation.
     */
    private fun estimatorMle(nodes: List<Station>): Matrix {
        val node = nodes[0]
        val sn = model.getStruct(false)
        val R = sn.nclasses
        val jobClasses = model.jobClasses

        val nServers = (node as? Queue)?.numberOfServers ?: 1
        val avgU = getAggrUtil(node)?.let { u ->
            DoubleArray(u.data.size) { u.data[it] * nServers }
        } ?: throw IllegalStateException("Aggregate utilization data missing")

        val avgA = Array(R) { r ->
            getArvR(node, jobClasses[r])?.data
                ?: throw IllegalStateException("Arrival rate data missing for class $r")
        }
        val avgRT = Array(R) { r ->
            getRespT(node, jobClasses[r])?.data
                ?: throw IllegalStateException("Response time data missing for class $r")
        }

        val N = avgU.size
        val solverAnalyzer = getSolverAnalyzer(model, options)
        val stIdx = (node as? Station)?.getStationIdx() ?: 0

        // Initial point
        val maxRT = DoubleArray(R) { r -> avgRT[r].max() }
        val x0 = DoubleArray(R) { r -> Math.random() * maxRT[r] }
        val xLB = DoubleArray(R) { 1e-8 }
        val xUB = maxRT

        // Compute weights
        val w = Array(N) { i ->
            val totalArvR = (0 until R).sumOf { r -> avgA[r][i] }
            DoubleArray(R) { r -> if (totalArvR > 0) avgA[r][i] / totalArvR else 1.0 / R }
        }

        val (demEst, _) = OptimUtils.fmincon(
            objFun = { x ->
                // Update service rates
                for (c in 0 until R) {
                    sn_set_service_coc(sn, stIdx, c, 1.0 / x[c])
                }

                val result = solverAnalyzer(sn)
                val predR = DoubleArray(R) { r -> result.RN.get(stIdx, r) }
                val predU = (0 until R).sumOf { r -> result.UN.get(stIdx, r) }

                var f = 0.0
                for (i in 0 until N) {
                    for (r in 0 until R) {
                        val delta = predR[r] - avgRT[r][i]
                        f += w[i][r] * delta * delta
                    }
                    val eps = predU - avgU[i]
                    f += eps * eps
                }
                f
            },
            x0 = x0,
            lb = xLB,
            ub = xUB,
            maxIter = options.iterMax
        )

        val estVal = Matrix(1, R)
        for (r in 0 until R) estVal.set(0, r, demEst[r])
        return estVal
    }

    /**
     * MLPS: Maximum Likelihood for Processor Sharing.
     */
    private fun estimatorMlps(nodes: List<Station>): Matrix {
        val node = nodes[0] as Queue
        val sn = model.getStruct(false)
        val R = sn.nclasses
        val jobClasses = model.jobClasses

        if (sn.sched[sn.stations[node.getStationIdx()]] != SchedStrategy.PS) {
            throw IllegalStateException("MLPS method is only available for PS stations.")
        }

        val hasOpen = (0 until R).any { sn.njobs.get(it) >= Double.MAX_VALUE / 2 }
        val eqModel: Network
        val eqNode: Queue
        if (hasOpen) {
            val eq = buildClosedEquivalentForPS(node)
            eqModel = eq.first
            eqNode = eq.second
        } else {
            eqModel = model
            eqNode = node
        }

        // Extract trace data
        val rtAll = ArrayList<Double>()
        val classAll = ArrayList<Int>()
        val atAll = ArrayList<Double>()

        for (r in 0 until R) {
            val arvData = getArvR(node, jobClasses[r])
                ?: throw IllegalStateException("Arrival data missing for class $r")
            if (!arvData.isTrace()) throw IllegalStateException("MLPS requires trace-format data.")

            val rtData = getRespT(node, jobClasses[r])
                ?: throw IllegalStateException("Response time data missing for class $r")
            if (!rtData.isTrace()) throw IllegalStateException("MLPS requires trace-format data.")

            for (i in rtData.data.indices) {
                rtAll.add(rtData.data[i])
                atAll.add(arvData.data[i])
                classAll.add(r)
            }
        }

        val rt = rtAll.toDoubleArray()
        val at = atAll.toDoubleArray()
        val classVec = classAll.toIntArray()
        val n = at.size
        val jobid = IntArray(n) { it }

        val ql = infer_compute_ql_at_arrival(at, jobid, rt, jobid, classVec, R)

        // Sort by arrival time
        val sortIdx = (0 until n).sortedBy { at[it] }.toIntArray()
        val rtSorted = DoubleArray(n) { rt[sortIdx[it]] }
        val classSorted = IntArray(n) { classVec[sortIdx[it]] }
        val qlSorted = Matrix(n, R)
        for (i in 0 until n) {
            for (r in 0 until R) qlSorted.set(i, r, ql.get(sortIdx[i], r))
        }

        // Remove zero response times
        val valid = (0 until n).filter { rtSorted[it] > 0 }
        val rtValid = valid.map { rtSorted[it] }.toDoubleArray()
        val classValid = valid.map { classSorted[it] }.toIntArray()
        val qlValid = Matrix(valid.size, R)
        for (i in valid.indices) {
            for (r in 0 until R) qlValid.set(i, r, qlSorted.get(valid[i], r))
        }

        val demandEst = infer_mlps(eqModel, eqNode, rtValid, classValid, qlValid)
        val estVal = Matrix(1, R)
        for (r in 0 until R) estVal.set(0, r, demandEst[r])
        return estVal
    }

    /**
     * FMLPS: Fluid Maximum Likelihood for Processor Sharing.
     */
    private fun estimatorFmlps(nodes: List<Station>): Matrix {
        val node = nodes[0] as Queue
        val sn = model.getStruct(false)
        val R = sn.nclasses
        val jobClasses = model.jobClasses

        if (sn.sched[sn.stations[node.getStationIdx()]] != SchedStrategy.PS) {
            throw IllegalStateException("FMLPS method is only available for PS stations.")
        }

        val hasOpen = (0 until R).any { sn.njobs.get(it) >= Double.MAX_VALUE / 2 }
        val eqModel: Network
        val eqNode: Queue
        val W: Int
        if (hasOpen) {
            val eq = buildClosedEquivalentForPS(node)
            eqModel = eq.first
            eqNode = eq.second
            val eqSn = eqModel.getStruct(false)
            W = eqSn.njobs.elementSum().toInt()
        } else {
            eqModel = model
            eqNode = node
            W = sn.njobs.elementSum().toInt()
        }

        // Extract trace data (same as MLPS)
        val rtAll = ArrayList<Double>()
        val classAll = ArrayList<Int>()
        val atAll = ArrayList<Double>()

        for (r in 0 until R) {
            val arvData = getArvR(node, jobClasses[r])
                ?: throw IllegalStateException("Arrival data missing for class $r")
            val rtData = getRespT(node, jobClasses[r])
                ?: throw IllegalStateException("Response time data missing for class $r")

            for (i in rtData.data.indices) {
                rtAll.add(rtData.data[i])
                atAll.add(arvData.data[i])
                classAll.add(r)
            }
        }

        val rt = rtAll.toDoubleArray()
        val at = atAll.toDoubleArray()
        val classVec = classAll.toIntArray()
        val n = at.size
        val jobid = IntArray(n) { it }

        val ql = infer_compute_ql_at_arrival(at, jobid, rt, jobid, classVec, R)

        val sortIdx = (0 until n).sortedBy { at[it] }.toIntArray()
        val rtSorted = DoubleArray(n) { rt[sortIdx[it]] }
        val classSorted = IntArray(n) { classVec[sortIdx[it]] }
        val qlSorted = Matrix(n, R)
        for (i in 0 until n) {
            for (r in 0 until R) qlSorted.set(i, r, ql.get(sortIdx[i], r))
        }

        val valid = (0 until n).filter { rtSorted[it] > 0 }
        val rtValid = valid.map { rtSorted[it] }.toDoubleArray()
        val classValid = valid.map { classSorted[it] }.toIntArray()
        val qlValid = Matrix(valid.size, R)
        for (i in valid.indices) {
            for (r in 0 until R) qlValid.set(i, r, qlSorted.get(valid[i], r))
        }

        val demandEst = infer_fmlps(eqModel, eqNode, rtValid, classValid, qlValid, W)
        val estVal = Matrix(1, R)
        for (r in 0 until R) estVal.set(0, r, demandEst[r])
        return estVal
    }

    /**
     * QMLE: Quick Maximum Likelihood Estimation.
     */
    private fun estimatorQmle(nodes: List<Station>): Matrix {
        val sn = model.getStruct(false)
        val R = sn.nclasses
        val M = nodes.size
        val jobClasses = model.jobClasses

        val Nopen = options.openPopulation
        val N = DoubleArray(R) { r ->
            val njobs = sn.njobs.get(r)
            if (njobs < Double.MAX_VALUE / 2) njobs else Nopen.toDouble()
        }

        // Extract think times
        val Z = DoubleArray(R)
        val allNodes = model.nodes
        for (nd in allNodes) {
            if (nd is Delay) {
                for (r in 0 until R) {
                    if (sn.njobs.get(r) < Double.MAX_VALUE / 2) {
                        val dist = nd.getService(jobClasses[r])
                        if (dist != null) Z[r] += dist.getMean()
                    }
                }
            } else if (nd is Source) {
                for (r in 0 until R) {
                    if (sn.njobs.get(r) >= Double.MAX_VALUE / 2) {
                        val dist = nd.getArrivalProcess(jobClasses[r])
                        if (dist != null) {
                            val lambdaR = 1.0 / dist.getMean()
                            Z[r] = N[r] / lambdaR
                        }
                    }
                }
            }
        }

        // Extract mean queue-lengths
        val Q = Matrix(M, R)
        for (n in 0 until M) {
            for (r in 0 until R) {
                val qlData = getQLen(nodes[n], jobClasses[r])
                if (qlData.isEmpty()) throw IllegalStateException("Queue-length data missing")
                Q.set(n, r, qlData[0].data.average())
            }
        }

        return infer_qmle(Q, N, Z)
    }

    /**
     * Gibbs: Gibbs Sampling from trace data.
     */
    private fun estimatorGibbs(nodes: List<Station>): Matrix {
        val node = nodes[0]
        val sn = model.getStruct(false)
        val R = sn.nclasses
        val nServers = (node as? Queue)?.numberOfServers ?: 1
        val jobClasses = model.jobClasses

        // Build legacy data cell array
        val data = Array(7) { arrayOfNulls<DoubleArray>(R + 1) }

        for (r in 0 until R) {
            val arvData = getArvR(node, jobClasses[r])
                ?: throw IllegalStateException("Arrival data missing for class $r")
            if (!arvData.isTrace()) throw IllegalStateException("Gibbs requires trace-format data.")
            data[3][r] = DoubleArray(arvData.data.size) { arvData.data[it] * 1000 } // convert to ms

            val rtData = getRespT(node, jobClasses[r])
                ?: throw IllegalStateException("Response time data missing for class $r")
            if (!rtData.isTrace()) throw IllegalStateException("Gibbs requires trace-format data.")
            data[4][r] = rtData.data

            val tputData = getTput(node, jobClasses[r])
                ?: throw IllegalStateException("Throughput data missing for class $r")
            data[6][r] = tputData.data
        }

        val demandEst = infer_gibbs(data, nServers, options.tol)
        val estVal = Matrix(1, R)
        for (r in 0 until Math.min(R, demandEst.size)) {
            estVal.set(0, r, demandEst[r])
        }
        return estVal
    }

    // ==================== Helper Methods ====================

    /**
     * Build a closed equivalent model for open/mixed networks with PS stations.
     */
    fun buildClosedEquivalentForPS(node: Queue): Pair<Network, Queue> {
        val sn = model.getStruct(false)
        val R = sn.nclasses
        val jobClasses = model.jobClasses
        val Nopen = options.openPopulation

        val N = IntArray(R)
        val Z = DoubleArray(R)

        val allNodes = model.nodes
        for (nd in allNodes) {
            if (nd is Delay) {
                for (r in 0 until R) {
                    if (sn.njobs.get(r) < Double.MAX_VALUE / 2) {
                        val dist = nd.getService(jobClasses[r])
                        if (dist != null) Z[r] += dist.getMean()
                    }
                }
            } else if (nd is Source) {
                for (r in 0 until R) {
                    if (sn.njobs.get(r) >= Double.MAX_VALUE / 2) {
                        val dist = nd.getArrivalProcess(jobClasses[r])
                        if (dist != null) {
                            val lambdaR = 1.0 / dist.getMean()
                            Z[r] = Nopen.toDouble() / lambdaR
                        }
                    }
                }
            }
        }

        for (r in 0 until R) {
            N[r] = if (sn.njobs.get(r) < Double.MAX_VALUE / 2) {
                sn.njobs.get(r).toInt()
            } else {
                Nopen
            }
        }

        val delayRate = DoubleArray(R) { 1.0 / Z[it] }

        // Build closed equivalent model
        val eqModel = Network("closed_equiv")
        val eqDelay = Delay(eqModel, "Think")
        val eqQueue = Queue(eqModel, "Queue1", SchedStrategy.PS)
        eqQueue.setNumberOfServers(node.numberOfServers)

        val eqClasses = arrayOfNulls<ClosedClass>(R)
        for (r in 0 until R) {
            eqClasses[r] = ClosedClass(eqModel, "Class${r + 1}", N[r], eqDelay, 0)
            eqDelay.setService(eqClasses[r], Exp(delayRate[r]))
            // Copy queue service from original node
            val origDist = node.getService(jobClasses[r])
            if (origDist != null) {
                eqQueue.setService(eqClasses[r], origDist)
            } else {
                eqQueue.setService(eqClasses[r], Exp(1.0))
            }
        }

        val P = eqModel.initRoutingMatrix()
        val nodeList = listOf(eqDelay, eqQueue)
        for (r in 0 until R) {
            P.set(eqClasses[r], Network.serialRouting(nodeList))
        }
        eqModel.link(P)

        return Pair(eqModel, eqQueue)
    }

    // ==================== Solver Analyzer Utilities ====================

    private fun getSolverAnalyzer(
        model: Network,
        options: EstimatorOptions
    ): (NetworkStruct) -> SolverResult {
        val solverFactory = options.solverFactory
        if (solverFactory != null) {
            val solver = solverFactory(model)
            val solverOpts = solver.options

            return when (solver) {
                is SolverMVA -> { sn -> solver_mva_analyzer(sn, solverOpts) }
                else -> { sn ->
                    // Default: use MVA analyzer
                    val mvaOpts = SolverMVA.defaultOptions()
                    solver_mva_analyzer(sn, mvaOpts)
                }
            }
        }

        // Default: use MVA
        val mvaOpts = SolverMVA.defaultOptions()
        return { sn -> solver_mva_analyzer(sn, mvaOpts) }
    }

    private fun getPredictedMeasurement(
        x: DoubleArray,
        R: Int,
        sn: NetworkStruct,
        stIdx: Int,
        solverAnalyzer: (NetworkStruct) -> SolverResult
    ): DoubleArray {
        for (c in 0 until R) {
            sn_set_service_coc(sn, stIdx, c, 1.0 / x[c])
        }

        val result = solverAnalyzer(sn)
        val h = DoubleArray(R + 1)
        for (c in 0 until R) {
            h[c] = result.RN.get(stIdx, c)
        }
        h[R] = (0 until R).sumOf { result.UN.get(stIdx, it) }
        return h
    }

    private fun getJacobian(
        x: DoubleArray,
        R: Int,
        sn: NetworkStruct,
        stIdx: Int,
        solverAnalyzer: (NetworkStruct) -> SolverResult,
        h0: DoubleArray
    ): Array<DoubleArray> {
        val delta = 1e-6
        val Hx = Array(R + 1) { DoubleArray(R) }

        for (c in 0 until R) {
            val xPert = x.copyOf()
            xPert[c] += delta
            val snPert = model.getStruct(true) // fresh copy to avoid in-place mutation
            val hPert = getPredictedMeasurement(xPert, R, snPert, stIdx, solverAnalyzer)
            for (i in 0..R) {
                Hx[i][c] = (hPert[i] - h0[i]) / delta
            }
        }
        return Hx
    }

    private fun mcmcData(
        avgQL: Matrix,
        visits: Map<Int, Matrix>?,
        experiments: Int,
        iterMax: Int,
        P: DoubleArray,
        Z: DoubleArray
    ): Matrix {
        val M = avgQL.numRows
        val R = avgQL.numCols
        val S = 100 // number of Gibbs samples

        val mciVariant = "imci"
        var maxQL = 0.0
        for (i in 0 until M) {
            for (j in 0 until R) {
                maxQL = Math.max(maxQL, avgQL.get(i, j))
            }
        }
        val thetaStep = maxQL / 400
        val steps = generateSequence(0.0) { it + thetaStep }.takeWhile { it <= maxQL }.toList().toDoubleArray()

        val theta = Array(S + 1) { Array(M) { DoubleArray(R) } }

        val rng = java.util.Random()
        for (s in 0 until S) {
            val sampleTheta = Array(M) { theta[s][it].copyOf() }
            for (i in 0 until M) {
                for (c in 0 until R) {
                    // Compute normalizing constant once (sampleTheta is fixed within the steps loop)
                    val thetaMatrix = Matrix(M, R)
                    for (ii in 0 until M) {
                        for (cc in 0 until R) {
                            thetaMatrix.set(ii, cc, sampleTheta[ii][cc])
                        }
                    }
                    val popMatrix = Matrix(1, R)
                    for (cc in 0 until R) popMatrix.set(0, cc, P[cc])
                    val thinkMatrix = Matrix(1, R)
                    for (cc in 0 until R) thinkMatrix.set(0, cc, Z[cc])

                    val gResult = jline.api.pfqn.nc.pfqn_mci(thetaMatrix, popMatrix, thinkMatrix, experiments, mciVariant)
                    val logG = if (gResult.lG != null && gResult.lG.isFinite()) gResult.lG else Math.log(gResult.G + 1e-15)
                    val logPrior = Math.log(thetaStep / maxQL)

                    val logPosteriors = DoubleArray(steps.size)
                    for (st in steps.indices) {
                        val stepTheta = steps[st]
                        logPosteriors[st] = experiments * avgQL.get(i, c) * Math.log(stepTheta + 1e-15) -
                                experiments * logG + logPrior
                    }

                    val maxLogPost = logPosteriors.max()
                    val probs = DoubleArray(steps.size) { Math.exp(logPosteriors[it] - maxLogPost) }
                    val probSum = probs.sum()
                    for (j in probs.indices) probs[j] /= probSum

                    val cumProb = DoubleArray(probs.size)
                    cumProb[0] = probs[0]
                    for (j in 1 until probs.size) cumProb[j] = cumProb[j - 1] + probs[j]

                    val u = rng.nextDouble()
                    val index = cumProb.indexOfFirst { it > u }.let { if (it < 0) cumProb.size - 1 else it }
                    sampleTheta[i][c] = steps[index]
                }
            }
            for (i in 0 until M) {
                theta[s + 1][i] = sampleTheta[i].copyOf()
            }
        }

        // Divide each theta sample by visit ratios before averaging
        // MATLAB: visitPerClass(r,:) = visits{r}(2:M+1, r), then theta ./ visitPerClass'
        if (visits != null) {
            for (s in 0..S) {
                for (i in 0 until M) {
                    for (c in 0 until R) {
                        val chainVisits = visits[c]
                        if (chainVisits != null) {
                            // In MATLAB visits{chain}(stationRow, classCol), station rows start at 2 (1-based)
                            // In Java, station index i+1 corresponds to MATLAB's i+2 (queue stations)
                            val visitVal = chainVisits.get(i + 1, c)
                            if (visitVal != 0.0) {
                                theta[s][i][c] /= visitVal
                            }
                        }
                    }
                }
            }
        }

        // Average over burn-in period
        val cutoff = S / 2
        val thetaAvg = Matrix(M, R)
        for (i in 0 until M) {
            for (c in 0 until R) {
                var sum = 0.0
                for (s in cutoff..S) {
                    sum += theta[s][i][c]
                }
                thetaAvg.set(i, c, sum / (S - cutoff + 1))
            }
        }

        return thetaAvg
    }

    private fun invertMatrix(A: Array<DoubleArray>): Array<DoubleArray> {
        val n = A.size
        val aug = Array(n) { i -> DoubleArray(2 * n) { j ->
            if (j < n) A[i][j] else if (j - n == i) 1.0 else 0.0
        }}

        for (col in 0 until n) {
            var maxRow = col
            var maxVal = Math.abs(aug[col][col])
            for (row in col + 1 until n) {
                if (Math.abs(aug[row][col]) > maxVal) {
                    maxVal = Math.abs(aug[row][col])
                    maxRow = row
                }
            }
            val temp = aug[col]; aug[col] = aug[maxRow]; aug[maxRow] = temp

            val pivot = aug[col][col]
            if (Math.abs(pivot) < 1e-14) continue
            for (j in 0 until 2 * n) aug[col][j] /= pivot

            for (row in 0 until n) {
                if (row == col) continue
                val factor = aug[row][col]
                for (j in 0 until 2 * n) aug[row][j] -= factor * aug[col][j]
            }
        }

        return Array(n) { i -> DoubleArray(n) { j -> aug[i][j + n] } }
    }

    companion object {
        /**
         * Create default estimator options.
         */
        @JvmStatic
        fun defaultOptions(): EstimatorOptions = EstimatorOptions.defaultOptions()

        /**
         * Return description of metrics required by each estimation method.
         */
        @JvmStatic
        fun getRequiredMetrics(method: String): String = when (method) {
            "ubr" -> "ArvR (per-class) + Util (per-class or aggregate)"
            "ubo" -> "ArvR (per-class) + RespT (per-class) + Util (aggregate)"
            "erps" -> "RespT (per-class) + QLen (aggregate, conditional on class arrivals). PS stations only."
            "ekf" -> "RespT (per-class) + Util (aggregate). Sequential/recursive estimation."
            "mcmc" -> "QLen (aggregate). Gibbs sampling with MCMC. Open/mixed via closed equivalence."
            "mle" -> "ArvR (per-class) + RespT (per-class) + Util (aggregate)"
            "mlps" -> "ArvR (per-class, trace) + RespT (per-class, trace). PS stations only."
            "fmlps" -> "ArvR (per-class, trace) + RespT (per-class, trace). PS stations only."
            "qmle" -> "QLen (per-class). Open/mixed via closed equivalence."
            "gibbs" -> "ArvR (per-class, trace) + RespT (per-class, trace) + Tput (per-class). Gibbs sampling."
            else -> "Unknown method: $method"
        }
    }
}

private fun TreeSet(): java.util.TreeSet<Double> = java.util.TreeSet()
