/*
 * Copyright (c) 2012-2026, Imperial College London
 * All rights reserved.
 */
package jline.inference

import jline.lang.Network
import jline.lang.ClosedClass
import jline.lang.RoutingMatrix
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

class InferUbrTest {

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
    fun testUbrClosed() {
        val rng = Random(1)

        val model = Network("model")
        val delay = Delay(model, "Delay")
        val queue = Queue(model, "Queue1", SchedStrategy.PS)
        val class1 = ClosedClass(model, "Class1", 1, delay, 0)
        val class2 = ClosedClass(model, "Class2", 2, delay, 0)

        delay.setService(class1, Exp.fitMean(1.0))
        delay.setService(class2, Exp.fitMean(1.0))
        queue.setService(class1, Exp(Double.NaN))
        queue.setService(class2, Exp(Double.NaN))

        val P = model.initRoutingMatrix()
        P.set(class1, class1, delay, queue, 1.0)
        P.set(class1, class1, queue, delay, 1.0)
        P.set(class2, class2, delay, queue, 1.0)
        P.set(class2, class2, queue, delay, 1.0)
        model.link(P)

        val n = 1000
        val ts = DoubleArray(n) { (it + 1).toDouble() }
        val arvr1 = DoubleArray(n) { 1.5 + rng.nextDouble() * 0.1 }
        val arvr2 = DoubleArray(n) { 2.0 + rng.nextDouble() * 0.1 }
        val util = DoubleArray(n) { 0.2 * arvr1[it] + 0.4 * arvr2[it] }

        val options = ParamEstimator.defaultOptions()
        options.method = "ubr"
        val se = ParamEstimator(model, options)

        se.addSamples(SampledMetric(MetricType.ArvR, ts, arvr1, queue, class1))
        se.addSamples(SampledMetric(MetricType.ArvR, ts, arvr2, queue, class2))
        se.addSamples(SampledMetric(MetricType.Util, ts, util, queue))
        se.interpolate()
        val estVal = se.estimateAt(listOf(queue))

        val trueDemands = doubleArrayOf(0.2, 0.4)
        for (r in trueDemands.indices) {
            val relErr = Math.abs(estVal.get(0, r) - trueDemands[r]) / trueDemands[r]
            assertTrue(relErr < 0.10,
                "UBR: class ${r + 1} estimated ${estVal.get(0, r)}, " +
                "relative error ${relErr * 100}% exceeds 10%")
        }

        // Verify model solves after estimation
        val solver = SolverMVA(model)
        val avgTable = solver.getAvgTable()
        assertNotNull(avgTable)
    }
}
