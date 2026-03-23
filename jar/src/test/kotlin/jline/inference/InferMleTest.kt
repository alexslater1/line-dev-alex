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

class InferMleTest {

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
    fun testMleClosed() {
        val rng = Random(1)
        val trueDemands = doubleArrayOf(0.1, 0.3)

        val model = Network("model")
        val delay = Delay(model, "Delay")
        val queue = Queue(model, "Queue1", SchedStrategy.PS)
        val class1 = ClosedClass(model, "Class1", 1, delay, 0)
        val class2 = ClosedClass(model, "Class2", 3, delay, 0)

        delay.setService(class1, Exp.fitMean(1.0))
        delay.setService(class2, Exp.fitMean(1.0))
        queue.setService(class1, Exp.fitMean(trueDemands[0]))
        queue.setService(class2, Exp.fitMean(trueDemands[1]))

        val P = model.initRoutingMatrix()
        P.set(class1, class1, delay, queue, 1.0)
        P.set(class1, class1, queue, delay, 1.0)
        P.set(class2, class2, delay, queue, 1.0)
        P.set(class2, class2, queue, delay, 1.0)
        model.link(P)

        // Get true steady-state metrics from MVA
        val solverMva = SolverMVA(model)
        val trueRespT = solverMva.getAvgRespT()
        val trueUtil = solverMva.getAvgUtil()
        val trueTput = solverMva.getAvgTput()

        val stIdx = queue.getStationIdx()
        val trueR1 = trueRespT.get(stIdx, 0)
        val trueR2 = trueRespT.get(stIdx, 1)
        val trueU = trueUtil.get(stIdx, 0) + trueUtil.get(stIdx, 1)
        val trueX1 = trueTput.get(stIdx, 0)
        val trueX2 = trueTput.get(stIdx, 1)

        // Generate noisy dataset
        val n = 1000
        val ts = DoubleArray(n) { (it + 1).toDouble() }
        val noiseScale = 0.05
        val arvr1 = DoubleArray(n) { trueX1 + (rng.nextDouble() - 0.5) * noiseScale * trueX1 }
        val arvr2 = DoubleArray(n) { trueX2 + (rng.nextDouble() - 0.5) * noiseScale * trueX2 }
        val respt1 = DoubleArray(n) { trueR1 + (rng.nextDouble() - 0.5) * noiseScale * trueR1 }
        val respt2 = DoubleArray(n) { trueR2 + (rng.nextDouble() - 0.5) * noiseScale * trueR2 }
        val utilSamples = DoubleArray(n) { trueU + (rng.nextDouble() - 0.5) * noiseScale * trueU }

        // Reset service for estimation
        queue.setService(class1, Exp(Double.NaN))
        queue.setService(class2, Exp(Double.NaN))

        val options = ParamEstimator.defaultOptions()
        options.method = "mle"
        val se = ParamEstimator(model, options)

        se.addSamples(SampledMetric(MetricType.ArvR, ts, arvr1, queue, class1))
        se.addSamples(SampledMetric(MetricType.ArvR, ts, arvr2, queue, class2))
        se.addSamples(SampledMetric(MetricType.RespT, ts, respt1, queue, class1))
        se.addSamples(SampledMetric(MetricType.RespT, ts, respt2, queue, class2))
        se.addSamples(SampledMetric(MetricType.Util, ts, utilSamples, queue))
        se.interpolate()
        val estVal = se.estimateAt(listOf(queue))

        for (r in trueDemands.indices) {
            val relErr = Math.abs(estVal.get(0, r) - trueDemands[r]) / trueDemands[r]
            assertTrue(relErr < 0.10,
                "MLE: class ${r + 1} estimated ${estVal.get(0, r)}, " +
                "relative error ${relErr * 100}% exceeds 10%")
        }

        val solver = SolverMVA(model)
        val avgTable = solver.getAvgTable()
        assertNotNull(avgTable)
    }
}
