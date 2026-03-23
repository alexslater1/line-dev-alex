/*
 * Copyright (c) 2012-2026, Imperial College London
 * All rights reserved.
 */
package jline.inference

import jline.lang.Network
import jline.lang.ClosedClass
import jline.lang.constant.MetricType
import jline.lang.constant.SchedStrategy
import jline.lang.nodes.Delay
import jline.lang.nodes.Queue
import jline.lang.processes.Exp
import jline.solvers.mva.SolverMVA
import jline.inference.lang.ParamEstimator
import jline.inference.lang.SampledMetric
import jline.util.Maths
import org.junit.jupiter.api.*
import org.junit.jupiter.api.Assertions.*
import java.util.Random

class InferEkfTest {

    companion object {
        @JvmStatic
        @BeforeAll
        fun setup() {
            Maths.setRandomNumbersMatlab(true)
        }

        @JvmStatic
        @AfterAll
        fun teardown() {
            Maths.setRandomNumbersMatlab(false)
        }
    }

    @Test
    fun testEkfClosed() {
        val rng = Random(1)
        val trueDemand = 0.3

        val model = Network("model")
        val delay = Delay(model, "Delay")
        val queue = Queue(model, "Queue1", SchedStrategy.PS)
        val class1 = ClosedClass(model, "Class1", 5, delay, 0)

        delay.setService(class1, Exp.fitMean(1.0))
        queue.setService(class1, Exp.fitMean(trueDemand))

        val P = model.initRoutingMatrix()
        P.set(class1, class1, delay, queue, 1.0)
        P.set(class1, class1, queue, delay, 1.0)
        model.link(P)

        // Get true steady-state metrics from MVA
        val solverMva = SolverMVA(model)
        val trueRespT = solverMva.getAvgRespT()
        val trueUtil = solverMva.getAvgUtil()
        val trueTput = solverMva.getAvgTput()

        val stIdx = queue.getStationIdx()
        val trueR = trueRespT.get(stIdx, 0)
        val trueU = trueUtil.get(stIdx, 0)
        val trueX = trueTput.get(stIdx, 0)

        // Generate noisy dataset
        val n = 1000
        val ts = DoubleArray(n) { (it + 1).toDouble() }
        val noiseScale = 0.05
        val arvr = DoubleArray(n) { trueX + (rng.nextDouble() - 0.5) * noiseScale * trueX }
        val respt = DoubleArray(n) { trueR + (rng.nextDouble() - 0.5) * noiseScale * trueR }
        val util = DoubleArray(n) { trueU + (rng.nextDouble() - 0.5) * noiseScale * trueU }

        // Reset service for estimation
        queue.setService(class1, Exp(Double.NaN))

        val options = ParamEstimator.defaultOptions()
        options.method = "ekf"
        val se = ParamEstimator(model, options)

        se.addSamples(SampledMetric(MetricType.ArvR, ts, arvr, queue, class1))
        se.addSamples(SampledMetric(MetricType.RespT, ts, respt, queue, class1))
        se.addSamples(SampledMetric(MetricType.Util, ts, util, queue))
        val estVal = se.estimateAt(listOf(queue))

        val est = estVal.get(0, 0)
        val relErr = Math.abs(est - trueDemand) / trueDemand
        assertTrue(relErr < 0.10,
            "EKF: estimated $est, relative error ${relErr * 100}% exceeds 10%")

        val solver = SolverMVA(model)
        val avgTable = solver.getAvgTable()
        assertNotNull(avgTable)
    }
}
