/*
 * Copyright (c) 2012-2026, Imperial College London
 * All rights reserved.
 */
package jline.inference

import jline.lang.Network
import jline.lang.ClosedClass
import jline.lang.Event
import jline.lang.constant.EventType
import jline.lang.constant.MetricType
import jline.lang.constant.SchedStrategy
import jline.lang.nodes.Delay
import jline.lang.nodes.Queue
import jline.lang.processes.Exp
import jline.solvers.SolverOptions
import jline.lang.constant.SolverType
import jline.solvers.mva.SolverMVA
import jline.solvers.ssa.SolverSSA
import jline.inference.lang.ParamEstimator
import jline.inference.lang.SampledMetric
import jline.util.Maths
import org.junit.jupiter.api.*
import org.junit.jupiter.api.Assertions.*

class InferMlpsTest {

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
    fun testMlpsClosed() {
        val trueDemand = 0.5

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

        // Generate trace data from SSA simulation
        val ssaOptions = SolverOptions(SolverType.SSA)
        ssaOptions.seed = 1
        ssaOptions.samples = 10000
        val solverSsa = SolverSSA(model, ssaOptions)
        val samplePath = solverSsa.sample(queue, 10000)

        // Extract arrival and departure times at the queue for class 1
        val arvTimes = ArrayList<Double>()
        val depTimes = ArrayList<Double>()
        for (ev in samplePath.event) {
            if (ev.node == queue.nodeIndex && ev.jobClass == class1.index - 1) {
                when (ev.event) {
                    EventType.ARV -> arvTimes.add(ev.t)
                    EventType.DEP -> depTimes.add(ev.t)
                    else -> {}
                }
            }
        }
        val n = minOf(arvTimes.size, depTimes.size)
        val arrivalTimes = DoubleArray(n) { arvTimes[it] }
        val responseTimes = DoubleArray(n) { depTimes[it] - arvTimes[it] }

        // Reset service for estimation
        queue.setService(class1, Exp(Double.NaN))

        // Create trace-format SampledMetric objects
        val arvData = SampledMetric(MetricType.ArvR, arrivalTimes, arrivalTimes, queue, class1)
        arvData.setTrace()
        val rtData = SampledMetric(MetricType.RespT, arrivalTimes, responseTimes, queue, class1)
        rtData.setTrace()

        val options = ParamEstimator.defaultOptions()
        options.method = "mlps"
        val se = ParamEstimator(model, options)
        se.addSamples(arvData)
        se.addSamples(rtData)
        se.interpolate()
        val estVal = se.estimateAt(listOf(queue))

        val est = estVal.get(0, 0)
        val relErr = Math.abs(est - trueDemand) / trueDemand
        assertTrue(relErr < 0.10,
            "MLPS: estimated $est, relative error ${relErr * 100}% exceeds 10%")

        val solver = SolverMVA(model)
        val avgTable = solver.getAvgTable()
        assertNotNull(avgTable)
    }
}
