/*
 * Copyright (c) 2012-2026, QORE Lab, Imperial College London
 * All rights reserved.
 * Ported from BUTools V2.0
 */
package jline.lib.butools.map

import jline.util.matrix.Matrix
import jline.util.matrix.MatrixCell
import java.util.Random
import kotlin.math.ln

/**
 * Generates random samples from a continuous marked Markovian arrival process.
 *
 * @param D The D0...DN matrices of the MMAP (as MatrixCell)
 * @param K The number of samples to generate
 * @param initial Optional initial state (1-indexed). If not provided, drawn from stationary distribution.
 * @param prec Numerical precision for validation
 * @param random Random number generator
 * @return For single type (2 matrices): DoubleArray of inter-arrival times
 *         For multiple types: Array of DoubleArray where each element is [time, type]
 */
@JvmOverloads
fun samplesFromMMAP(
    D: MatrixCell,
    K: Int,
    initial: Int? = null,
    prec: Double = 1e-14,
    random: Random = Random()
): Any {
    if (!checkMMAPRepresentation(D, prec)) {
        throw IllegalArgumentException("SamplesFromMMAP: Input isn't a valid MMAP representation!")
    }

    val N = D[0].numRows
    val numTypes = D.size()

    // Determine initial state
    var state = if (initial != null) {
        initial - 1 // Convert to 0-indexed
    } else {
        // Draw initial state according to stationary distribution
        val margDist = marginalDistributionFromMMAP(D, prec)
        val stst = margDist.alpha

        // Compute cumulative distribution
        val cummInitial = DoubleArray(N)
        cummInitial[0] = stst[0, 0]
        for (i in 1 until N) {
            cummInitial[i] = cummInitial[i - 1] + stst[0, i]
        }

        // Draw initial state
        val r = random.nextDouble()
        var s = 0
        while (s < N - 1 && cummInitial[s] <= r) {
            s++
        }
        s
    }

    // Build next state probability matrix for continuous-time MMAP
    // For state i, the total departure rate is -D[0][i,i]
    // Next state probabilities are:
    //   D[0][i,j] / (-D[0][i,i]) for j != i (hidden transitions, within D0)
    //   D[k][i,j] / (-D[0][i,i]) for each type k>=1 (arrival transitions)
    val totalCols = N * numTypes
    val nextpr = Matrix(N, totalCols)

    for (i in 0 until N) {
        val rate = -D[0][i, i]
        if (rate <= 0) continue
        val invRate = 1.0 / rate

        var colOffset = 0
        for (dIdx in 0 until numTypes) {
            for (j in 0 until N) {
                val value = if (dIdx == 0 && i == j) {
                    0.0 // Remove diagonal from D0 (self-transition)
                } else {
                    D[dIdx][i, j] * invRate
                }
                nextpr[i, colOffset + j] = value
            }
            colOffset += N
        }
    }

    // Compute cumulative sum along rows
    for (i in 0 until N) {
        for (j in 1 until totalCols) {
            nextpr[i, j] = nextpr[i, j] + nextpr[i, j - 1]
        }
    }

    // Generate samples
    return if (numTypes > 2) {
        // Multiple types: return array of [time, type] pairs
        val samples = Array(K) { DoubleArray(2) }
        for (n in 0 until K) {
            var time = 0.0

            // Play state transitions until an arrival occurs (state >= N)
            while (state < N) {
                // Exponential sojourn time: -ln(U) / rate
                val rate = -D[0][state, state]
                time += -ln(random.nextDouble()) / rate

                // Choose next state
                val r = random.nextDouble()
                var nstate = 0
                while (nstate < totalCols - 1 && nextpr[state, nstate] <= r) {
                    nstate++
                }
                state = nstate
            }

            samples[n][0] = time
            samples[n][1] = (state / N).toDouble() // Type (0-indexed)
            state = state % N
        }
        samples
    } else {
        // Single type: return just inter-arrival times
        val samples = DoubleArray(K)
        for (n in 0 until K) {
            var time = 0.0

            // Play state transitions until an arrival occurs (state >= N)
            while (state < N) {
                // Exponential sojourn time: -ln(U) / rate
                val rate = -D[0][state, state]
                time += -ln(random.nextDouble()) / rate

                // Choose next state
                val r = random.nextDouble()
                var nstate = 0
                while (nstate < totalCols - 1 && nextpr[state, nstate] <= r) {
                    nstate++
                }
                state = nstate
            }

            samples[n] = time
            state = state % N
        }
        samples
    }
}

/**
 * Overload for Array<Matrix>.
 */
@JvmOverloads
fun samplesFromMMAP(
    D: Array<Matrix>,
    K: Int,
    initial: Int? = null,
    prec: Double = 1e-14,
    random: Random = Random()
): Any {
    val cell = MatrixCell(D.size)
    for (i in D.indices) {
        cell[i] = D[i]
    }
    return samplesFromMMAP(cell, K, initial, prec, random)
}
