/**
 * Sample generation for discrete-time MAPs.
 *
 * @since LINE 3.0
 */
package jline.api.mam

import jline.lib.butools.dmap.samplesFromDMAP
import jline.util.matrix.Matrix
import jline.util.matrix.MatrixCell
import java.util.Random

/**
 * Generates samples of inter-arrival times from a discrete-time MAP.
 * Inter-arrival times are integer-valued, returned as doubles.
 */
fun dmap_sample(D0: Matrix, D1: Matrix, n: Int, random: Random?): DoubleArray {
    val rng = random ?: Random()
    val intSamples = samplesFromDMAP(D0, D1, n, null, 1e-14, rng)
    return DoubleArray(intSamples.size) { intSamples[it].toDouble() }
}

fun dmap_sample(DMAP: MatrixCell, n: Long, random: Random?): DoubleArray {
    return dmap_sample(DMAP.get(0), DMAP.get(1), n.toInt(), random)
}
