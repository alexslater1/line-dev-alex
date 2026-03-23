/*
 * Copyright (c) 2012-2026, Imperial College London
 * All rights reserved.
 */
package jline.inference

import jline.lang.Network
import jline.lang.ClosedClass
import jline.lang.constant.EventType
import jline.lang.constant.MetricType
import jline.lang.constant.SchedStrategy
import jline.lang.nodes.Delay
import jline.lang.nodes.Queue
import jline.lang.processes.Exp
import jline.solvers.mva.SolverMVA
import jline.inference.lang.ConditionEvent
import jline.inference.lang.ParamEstimator
import jline.inference.lang.SampledMetric
import jline.util.Maths
import org.junit.jupiter.api.*
import org.junit.jupiter.api.Assertions.*
import java.util.Random

class InferErpsTest {

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
    fun testErpsClosed() {
        val rng = Random(1)

        val model = Network("model")
        val delay = Delay(model, "Delay")
        val queue = Queue(model, "Queue1", SchedStrategy.PS)
        val class1 = ClosedClass(model, "Class1", 1, delay, 0)
        val class2 = ClosedClass(model, "Class2", 3, delay, 0)

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
        val arvr1 = DoubleArray(n) { 1.0 - rng.nextDouble() * 0.15 }
        val arvr2 = DoubleArray(n) { 2.0 - rng.nextDouble() * 0.15 }
        val util = DoubleArray(n) { 0.1 * arvr1[it] + 0.3 * arvr2[it] }
        val respt1 = DoubleArray(n) { 0.1 / (1.0 - util[it]) }
        val respt2 = DoubleArray(n) { 0.3 / (1.0 - util[it]) }
        val aqlen1 = DoubleArray(n) { 1.0 + util[it] / (1.0 - util[it]) }
        val aqlen2 = DoubleArray(n) { 1.0 + util[it] / (1.0 - util[it]) }

        val options = ParamEstimator.defaultOptions()
        options.method = "erps"
        val se = ParamEstimator(model, options)

        val aql1 = SampledMetric(MetricType.QLen, ts, aqlen1, queue)
        aql1.setConditional(ConditionEvent(queue, class1, EventType.ARV))
        val aql2 = SampledMetric(MetricType.QLen, ts, aqlen2, queue)
        aql2.setConditional(ConditionEvent(queue, class2, EventType.ARV))

        se.addSamples(aql1)
        se.addSamples(aql2)
        se.addSamples(SampledMetric(MetricType.RespT, ts, respt1, queue, class1))
        se.addSamples(SampledMetric(MetricType.RespT, ts, respt2, queue, class2))
        se.interpolate()
        val estVal = se.estimateAt(listOf(queue))

        val trueDemands = doubleArrayOf(0.1, 0.3)
        for (r in trueDemands.indices) {
            val relErr = Math.abs(estVal.get(0, r) - trueDemands[r]) / trueDemands[r]
            assertTrue(relErr < 0.10,
                "ERPS: class ${r + 1} estimated ${estVal.get(0, r)}, " +
                "relative error ${relErr * 100}% exceeds 10%")
        }

        val solver = SolverMVA(model)
        val avgTable = solver.getAvgTable()
        assertNotNull(avgTable)
    }
}
