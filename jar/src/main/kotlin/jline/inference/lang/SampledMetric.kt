/*
 * Copyright (c) 2012-2026, Imperial College London
 * All rights reserved.
 */
package jline.inference.lang

import jline.lang.Copyable
import jline.lang.constant.MetricType
import jline.lang.nodes.Node
import jline.lang.JobClass

/**
 * Conditioning event for sampled metrics.
 */
data class ConditionEvent(
    val node: Node?,
    val `class`: JobClass?,
    val event: Any?
)

/**
 * Format of the sampled data.
 */
enum class SampledFormat {
    TIMESERIES,
    TRACE
}

/**
 * Observed data for a metric.
 *
 * Represents observed measurement data that can be used for parameter inference.
 * Supports both time-series format (periodic observations) and trace format
 * (per-request observations).
 *
 * @property type the metric type (e.g., ArvR, RespT, Util, QLen, Tput)
 * @property t timestamps for the observations
 * @property data observed values corresponding to each timestamp
 * @property node the network node where the observation was taken
 * @property jobClass the job class for per-class metrics, or null for aggregate
 * @property cond conditioning event for conditional metrics
 * @property format data format: TIMESERIES or TRACE
 */
class SampledMetric(
    val type: MetricType,
    var t: DoubleArray,
    var data: DoubleArray,
    val node: Node,
    val jobClass: JobClass? = null
) : Copyable {

    var cond: ConditionEvent? = null
        private set

    var format: SampledFormat = SampledFormat.TIMESERIES
        private set

    /**
     * Set a conditioning event on this metric.
     */
    fun setConditional(event: ConditionEvent) {
        cond = event
    }

    /**
     * Switch the data format to trace (per-request).
     */
    fun setTrace() {
        format = SampledFormat.TRACE
    }

    /**
     * Check if this metric applies to all classes (aggregate).
     */
    fun isAggregate(): Boolean = jobClass == null

    /**
     * Check if this metric has a conditioning event.
     */
    fun isConditional(): Boolean = cond != null

    /**
     * Check if this metric is in trace format.
     */
    fun isTrace(): Boolean = format == SampledFormat.TRACE

    override fun <T : Copyable> copy(): T {
        val c = SampledMetric(type, t.copyOf(), data.copyOf(), node, jobClass)
        c.cond = cond
        c.format = format
        @Suppress("UNCHECKED_CAST")
        return c as T
    }
}
