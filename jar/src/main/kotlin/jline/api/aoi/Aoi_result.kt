/**
 * @file AoI result types
 *
 * Data classes for Age of Information analysis results, providing
 * mean, variance, and peak AoI metrics.
 *
 * @since LINE 3.0
 */
package jline.api.aoi

import jline.util.matrix.Matrix

/**
 * Result of Age of Information analysis for simple queue types.
 *
 * Contains the three primary AoI metrics: mean, variance, and peak.
 *
 * @property meanAoI Mean (average) Age of Information
 * @property varAoI Variance of Age of Information
 * @property peakAoI Mean Peak Age of Information
 */
data class AoiResult(
    val meanAoI: Double,
    val varAoI: Double,
    val peakAoI: Double
)

/**
 * Result of Age of Information analysis for general queue types.
 *
 * Extends AoiResult with an optional LST (Laplace-Stieltjes Transform)
 * function for the AoI distribution.
 *
 * @property meanAoI Mean (average) Age of Information
 * @property lstAoI LST of AoI distribution as matrix exponential parameters, or null
 * @property peakAoI Mean Peak Age of Information
 */
data class AoiLstResult(
    val meanAoI: Double,
    val lstAoI: LstFunction?,
    val peakAoI: Double
)

/**
 * Result of AoI topology validation.
 *
 * @property isAoI True if the network is a valid AoI topology
 * @property sourceIdx Node index of the Source
 * @property queueIdx Node index of the Queue
 * @property sinkIdx Node index of the Sink
 * @property sourceStation Station index of the Source
 * @property queueStation Station index of the Queue
 * @property capacity Queue capacity (1 = bufferless, 2 = single-buffer)
 * @property schedStrategy Scheduling strategy at the queue
 * @property systemType "bufferless" or "singlebuffer"
 * @property errorMsg Error message if not a valid AoI topology
 */
data class AoiValidationResult(
    val isAoI: Boolean,
    val sourceIdx: Int,
    val queueIdx: Int,
    val sinkIdx: Int,
    val sourceStation: Int,
    val queueStation: Int,
    val capacity: Int,
    val schedStrategy: String,
    val systemType: String,
    val errorMsg: String
)

/**
 * Extracted parameters for AoI analysis.
 *
 * For bufferless systems (capacity=1):
 *   tau, T define the PH arrival process
 *   sigma, S define the PH service process
 *   p is the preemption probability
 *
 * For single-buffer systems (capacity=2):
 *   lambda is the Poisson arrival rate
 *   sigma, S define the PH service process
 *   r is the replacement probability
 *
 * @property tau Arrival initial probability vector (bufferless only)
 * @property T Arrival sub-generator matrix (bufferless only)
 * @property sigma Service initial probability vector
 * @property S Service sub-generator matrix
 * @property p Preemption probability, 0=FCFS, 1=preemptive (bufferless only)
 * @property lambda Arrival rate (single-buffer only)
 * @property r Replacement probability, 0=FCFS, 1=replacement (single-buffer only)
 * @property systemType "bufferless" or "singlebuffer"
 * @property arrivalType "PH" or "M"
 */
data class AoiParams(
    val tau: Matrix?,
    val T: Matrix?,
    val sigma: Matrix,
    val S: Matrix,
    val p: Double,
    val lambda: Double,
    val r: Double,
    val systemType: String,
    val arrivalType: String
)

/**
 * Functional interface for Laplace-Stieltjes Transform evaluation.
 *
 * Evaluates the LST at a given point s in the complex plane.
 */
interface LstFunction {
    /**
     * Evaluate the LST at the given value of s.
     *
     * @param s The point at which to evaluate the LST (real, non-negative)
     * @return The value of the LST at s
     */
    fun evaluate(s: Double): Double
}
