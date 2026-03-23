/**
 * @file MAP to Phase-Type distribution conversion
 *
 * Converts a Markovian Arrival Process (MAP) to a Phase-Type (PH) distribution
 * and its associated PH-renewal process representation.
 *
 * @since LINE 3.0
 */
package jline.api.mam

import jline.util.matrix.Matrix
import jline.util.matrix.MatrixCell

/**
 * Converts a MAP to a Phase-Type (PH) distribution.
 *
 * Given a MAP {D0, D1}, extracts the PH distribution (alpha, T) where:
 * - T = D0 (the subgenerator)
 * - alpha = map_pie(MAP) (the embedded steady-state probability vector)
 *
 * Also returns the PH-renewal process PHR = {D0, D1'} where
 * D1' = D1 * ones * alpha, which is a MAP whose inter-arrival times
 * are i.i.d. with the extracted PH distribution.
 *
 * @param MAP a MatrixCell containing {D0, D1}
 * @return Triple of (alpha, T, PHR) where alpha is the initial probability vector,
 *         T is the subgenerator matrix, and PHR is the PH-renewal process as a MatrixCell
 */
fun map2ph(MAP: MatrixCell): Triple<Matrix, Matrix, MatrixCell> {
    val T = MAP[0]
    val alpha = map_pie(MAP)
    val n = MAP[1].numRows
    val PHR_D1 = MAP[1].mult(Matrix.ones(n, 1)).mult(alpha)
    val PHR = MatrixCell()
    PHR[0] = T
    PHR[1] = PHR_D1
    return Triple(alpha, T, PHR)
}
