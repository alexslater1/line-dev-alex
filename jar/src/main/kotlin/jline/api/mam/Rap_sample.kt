/**
 * @file Rational Arrival Process (RAP) sampling
 *
 * Generates random samples from a RAP distribution by delegating to the
 * Matrix Exponential (ME) sampling algorithm. The marginal distribution of
 * RAP inter-arrival times is an ME distribution.
 *
 * @since LINE 3.0
 */
package jline.api.mam

import jline.util.matrix.MatrixCell
import java.util.Random

/**
 * Generates random samples from a Rational Arrival Process (RAP) distribution.
 *
 * The marginal distribution of RAP inter-arrival times is a Matrix Exponential
 * (ME) distribution, so this function delegates to [me_sample].
 *
 * Note: This generates samples from the marginal distribution only.
 * It does not preserve the correlation structure of the RAP.
 * For full RAP simulation with correlations, use [map_sample] instead.
 *
 * @param RAP the RAP distribution as a MatrixCell {H0, H1}
 * @param n the number of samples to generate
 * @param random the random number generator to use
 * @return array of n samples from the RAP marginal distribution
 */
fun rap_sample(RAP: MatrixCell, n: Long, random: Random): DoubleArray {
    return me_sample(RAP, n, random)
}
