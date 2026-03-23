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

class InferMcmcTest {

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
    fun testMcmcClosed() {
        val rng = Random(1)
        val trueDemand = 0.1

        val model = Network("model")
        val delay = Delay(model, "Delay")
        val queue = Queue(model, "Queue1", SchedStrategy.PS)
        val class1 = ClosedClass(model, "Class1", 1, delay, 0)

        delay.setService(class1, Exp.fitMean(1.0))
        queue.setService(class1, Exp.fitMean(trueDemand))

        val P = model.initRoutingMatrix()
        P.set(class1, class1, delay, queue, 1.0)
        P.set(class1, class1, queue, delay, 1.0)
        model.link(P)

        // Get true steady-state queue length from MVA
        val solverMva = SolverMVA(model)
        val trueQLen = solverMva.getAvgQLen()
        val trueQLenQueue = trueQLen.get(1, 0) // Queue station index = 1 (0-based)

        // Generate model-consistent queue length samples
        val n = 5000
        val ts = DoubleArray(n) { (it + 1).toDouble() }
        val qlenSamples = DoubleArray(n) { trueQLenQueue + rng.nextDouble() * 0.005 - 0.0025 }

        // Reset service for estimation
        queue.setService(class1, Exp(Double.NaN))

        val options = ParamEstimator.defaultOptions()
        options.method = "mcmc"
        val se = ParamEstimator(model, options)

        se.addSamples(SampledMetric(MetricType.QLen, ts, qlenSamples, queue))
        se.interpolate()
        val estVal = se.estimateAt(listOf(queue))

        val est = estVal.get(0, 0)
        val relErr = Math.abs(est - trueDemand) / trueDemand
        assertTrue(relErr < 0.10,
            "MCMC: estimated $est, relative error ${relErr * 100}% exceeds 10%")

        val solver = SolverMVA(model)
        val avgTable = solver.getAvgTable()
        assertNotNull(avgTable)
    }
}
