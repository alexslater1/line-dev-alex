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

class InferQmleTest {

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
    fun testQmleClosed() {
        val rng = Random(1)
        val trueDemands = doubleArrayOf(0.2, 0.4)

        val model = Network("model")
        val delay = Delay(model, "Delay")
        val queue = Queue(model, "Queue1", SchedStrategy.PS)
        val class1 = ClosedClass(model, "Class1", 2, delay, 0)
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

        // Get true steady-state queue lengths from MVA
        val solverMva = SolverMVA(model)
        val trueQLen = solverMva.getAvgQLen()
        val trueQLen1 = trueQLen.get(1, 0) // Queue station, Class 1
        val trueQLen2 = trueQLen.get(1, 1) // Queue station, Class 2

        // Generate model-consistent queue length samples
        val n = 5000
        val ts = DoubleArray(n) { (it + 1).toDouble() }
        val qlen1 = DoubleArray(n) { trueQLen1 + rng.nextDouble() * 0.02 - 0.01 }
        val qlen2 = DoubleArray(n) { trueQLen2 + rng.nextDouble() * 0.02 - 0.01 }

        // Reset service for estimation
        queue.setService(class1, Exp(Double.NaN))
        queue.setService(class2, Exp(Double.NaN))

        val options = ParamEstimator.defaultOptions()
        options.method = "qmle"
        val se = ParamEstimator(model, options)

        se.addSamples(SampledMetric(MetricType.QLen, ts, qlen1, queue, class1))
        se.addSamples(SampledMetric(MetricType.QLen, ts, qlen2, queue, class2))
        se.interpolate()
        val estVal = se.estimateAt(listOf(queue))

        for (r in trueDemands.indices) {
            val relErr = Math.abs(estVal.get(0, r) - trueDemands[r]) / trueDemands[r]
            assertTrue(relErr < 0.10,
                "QMLE: class ${r + 1} estimated ${estVal.get(0, r)}, " +
                "relative error ${relErr * 100}% exceeds 10%")
        }

        val solver = SolverMVA(model)
        val avgTable = solver.getAvgTable()
        assertNotNull(avgTable)
    }
}
