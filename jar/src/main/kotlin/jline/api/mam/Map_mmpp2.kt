/**
 * @file MMPP(2) fitting from moments and autocorrelation
 *
 * Fits a 2-state Markov Modulated Poisson Process (MMPP) as a MAP from four
 * descriptive parameters: mean inter-arrival time, squared coefficient of
 * variation, skewness, and lag-1 autocorrelation coefficient.
 *
 * @since LINE 3.0
 */
package jline.api.mam

import jline.util.matrix.Matrix
import jline.util.matrix.MatrixCell
import kotlin.math.sqrt

/**
 * Fits an MMPP(2) as a MAP from four descriptive parameters.
 *
 * @param MEAN   mean inter-arrival time of the process
 * @param SCV_param squared coefficient of variation of inter-arrival times
 * @param SKEW   skewness of inter-arrival times (-1 for automatic minimization, applies only to SCV>1)
 * @param ACF1   lag-1 autocorrelation coefficient (-1 for maximum feasible autocorrelation)
 * @return a MatrixCell containing the MAP transition matrices {D0, D1}
 */
fun map_mmpp2(MEAN: Double, SCV_param: Double, SKEW: Double, ACF1: Double): MatrixCell {
    val E1 = MEAN
    val E2 = (1.0 + SCV_param) * E1 * E1
    val E1_2 = E1 * E1
    val E1_3 = E1_2 * E1
    var E3 = -(2.0 * E1_3 - 3.0 * E1 * E2 - SKEW * Math.pow(E2 - E1_2, 1.5))

    val G2: Double
    if (ACF1 == -1.0) {
        G2 = 1.0 - 10.0 * Math.pow(10.0, (-map_feastol()).toDouble())
    } else {
        G2 = ACF1 / (1.0 - 1.0 / SCV_param) / 0.5
    }

    var SCV = SCV_param
    if (SKEW == -1.0 && SCV > 1.0) {
        E3 = (3.0 / 2.0 + 0.001) * E2 * E2 / E1
    }
    SCV = (E2 - E1_2) / E1_2

    val mu00: Double
    val mu11: Double
    val q01: Double
    val q10: Double

    if (G2 < 1e-6) {
        // Simplified expressions for small G2
        val SCV2 = SCV * SCV

        mu00 = 2.0 * (6.0 * E1_3 * SCV - E3) / E1 / (6.0 * E1_3 * SCV + 3.0 * E1_3 * SCV2 + 3.0 * E1_3 - 2.0 * E3)
        mu11 = 0.0
        val E1_5 = E1_3 * E1_2
        q01 = 9.0 * E1_5 * (SCV - 1.0) * (SCV2 - 2.0 * SCV + 1.0) / (6.0 * E1_3 * SCV - E3) / (6.0 * E1_3 * SCV + 3.0 * E1_3 * SCV2 + 3.0 * E1_3 - 2.0 * E3)
        q10 = -3.0 * (SCV - 1.0) * E1_2 / (6.0 * E1_3 * SCV - E3)
    } else {
        // Full CAS-derived closed-form expressions
        val E1_5 = E1_3 * E1_2
        val E1_6 = E1_3 * E1_3
        val SCV2 = SCV * SCV
        val SCV3 = SCV2 * SCV
        val G2_2 = G2 * G2
        val G2_3 = G2_2 * G2

        // Common discriminant (appears inside every sqrt)
        val DISC = E3 * E3 - 12.0 * E1_3 * SCV * E3 + 6.0 * E1_3 * G2 * E3 - 6.0 * G2 * SCV * E1_3 * E3 + 18.0 * G2 * SCV3 * E1_6 - 18.0 * E1_6 * G2 * SCV2 + 9.0 * E1_6 * G2_2 + 36.0 * E1_6 * SCV2 + 18.0 * E1_6 * G2 * SCV - 18.0 * E1_6 * SCV * G2_2 + 9.0 * E1_6 * SCV2 * G2_2 - 18.0 * E1_6 * G2

        // Common subexpression F = A / B
        val A = -3.0 * E1_3 * G2 + 3.0 * E1_3 * G2 * SCV - 6.0 * E1_3 * SCV + E3 + sqrt(DISC)
        val B = -3.0 * E1_3 * SCV2 - 6.0 * E1_3 * SCV - 3.0 * E1_3 + 2.0 * E3
        val F = A / B

        // mu11 = F / E1  (line 40)
        mu11 = F / E1

        // mu00 (line 39): mu00 = G2 * NUMER / DENOM / E1
        val mu00_numer = -4.0 * E3 * G2 + 4.0 * F * E3 * G2 - 18.0 * E1_3 * F * G2 - 18.0 * E1_3 * F * G2 * SCV2 - 12.0 * E1_3 * G2_2 - 12.0 * E1_3 * F * G2_2 * SCV + 12.0 * E1_3 * F * G2 * SCV + 12.0 * E1_3 * G2 * SCV2 - 9.0 * E1_3 * F * SCV + 3.0 * E1_3 * F + 12.0 * E1_3 * G2_2 * SCV + 9.0 * E1_3 * F * SCV2 + 12.0 * E1_3 * G2 + 12.0 * E1_3 * F * G2_2 - 3.0 * E1_3 * F * SCV3

        val mu00_denom = 12.0 * E1_3 * G2_3 * SCV + 3.0 * E1_3 * SCV3 * G2 - 12.0 * E1_3 * G2_3 + 18.0 * E1_3 * G2_2 * SCV2 - 3.0 * E1_3 * G2 + 27.0 * E1_3 * F * G2 * SCV2 - 9.0 * E1_3 * G2 * SCV2 + 18.0 * E1_3 * G2_2 - 12.0 * E1_3 * G2_2 * SCV + 9.0 * E1_3 * G2 * SCV - 12.0 * E1_3 * F * G2_3 * SCV - 9.0 * E1_3 * F * SCV3 * G2 - 24.0 * E1_3 * F * G2_2 * SCV2 - F * E3 * SCV2 + 4.0 * F * E3 * G2_2 + 12.0 * E1_3 * F * G2_3 - F * E3 + 2.0 * F * E3 * SCV + 9.0 * E1_3 * F * G2 + 24.0 * E1_3 * F * G2_2 * SCV - 27.0 * E1_3 * F * G2 * SCV + 6.0 * E1_3 * F * SCV - 12.0 * E1_3 * F * SCV2 - 24.0 * E1_3 * F * G2_2 + 6.0 * E1_3 * F * SCV3 - 4.0 * E3 * G2_2

        mu00 = G2 * mu00_numer / mu00_denom / E1

        // q01 (line 41): q01 = -3*E1_2 * NUMER / DENOM
        // In the NUMER, F_OVER_E1 = F/E1 = mu11
        val q01_numer = -6.0 * F * E1_2 * SCV + 12.0 * F * E1_2 * G2 * SCV - 6.0 * G2 * SCV * E1_2 - 3.0 * F * E1_2 * G2 + mu11 * E3 + 3.0 * E1_2 * G2 + 6.0 * F * E1_2 * SCV2 - 9.0 * F * E1_2 * SCV2 * G2 + 3.0 * E1_2 * G2 * SCV2 - E3 * mu11 * SCV - 6.0 * F * E1_2 * G2_2 * SCV + 6.0 * E1_2 * G2_2 * SCV + 3.0 * F * E1_2 * G2_2 - G2 * mu11 * E3 - 3.0 * E1_2 * G2_2 + 3.0 * F * E1_2 * SCV2 * G2_2 - 3.0 * E1_2 * SCV2 * G2_2 + G2 * SCV * mu11 * E3

        val q01_denom = -45.0 * F * E1_5 * G2 * SCV2 + 18.0 * G2_2 * E1_5 * SCV + 18.0 * E1_5 * G2_3 - 27.0 * E1_5 * G2_2 * SCV2 + 6.0 * E1_2 * G2_2 * E3 - 27.0 * E1_5 * G2_2 - 18.0 * E1_5 * G2_3 * SCV - 18.0 * E1_5 * G2 * SCV + 18.0 * E1_5 * G2 * SCV2 + 3.0 * E1_2 * G2 * E3 - 3.0 * E1_2 * G2 * E3 * SCV + mu11 * E3 * E3 + 3.0 * F * E1_2 * G2 * SCV * E3 - 36.0 * F * E1_5 * G2_2 * SCV + 36.0 * F * E1_5 * G2_2 + 36.0 * F * E1_5 * SCV2 + 45.0 * F * E1_5 * G2 * SCV - 12.0 * F * E1_2 * SCV * E3 - 3.0 * F * E1_2 * G2 * E3 + 9.0 * F * E1_5 * G2 * SCV3 + 36.0 * F * E1_5 * G2_2 * SCV2 - 6.0 * F * E1_2 * G2_2 * E3 + 18.0 * F * E1_5 * G2_3 * SCV - 18.0 * F * E1_5 * G2_3 - 9.0 * F * E1_5 * G2

        q01 = -3.0 * E1_2 * q01_numer / q01_denom

        // q10 (line 42): q10 = 3 * INNER * E1_2 * (-1+G2) / DISC
        val q10_inner = -3.0 * E1_3 * F * SCV3 - 3.0 * E1_3 * F * G2 * SCV2 + 6.0 * E1_3 * SCV2 + 3.0 * E1_3 * G2 * SCV2 + 3.0 * E1_3 * F * SCV2 + 6.0 * E1_3 * F * G2 * SCV - E3 * SCV - 6.0 * E1_3 * SCV + F * E3 * SCV - 6.0 * E1_3 * G2 * SCV - 3.0 * E1_3 * F * SCV - F * E3 + 3.0 * E1_3 * G2 - 3.0 * E1_3 * F * G2 + 3.0 * E1_3 * F + E3

        q10 = 3.0 * q10_inner * E1_2 * (-1.0 + G2) / DISC
    }

    val D0 = Matrix(2, 2, 2)
    D0[0, 0] = -mu00 - q01
    D0[0, 1] = q01
    D0[1, 0] = q10
    D0[1, 1] = -mu11 - q10

    val D1 = Matrix(2, 2, 2)
    D1[0, 0] = mu00
    D1[0, 1] = 0.0
    D1[1, 0] = 0.0
    D1[1, 1] = mu11

    val MAP = MatrixCell()
    MAP[0] = D0
    MAP[1] = D1
    return MAP
}

/**
 * MAP MMPP2 algorithms
 */
@Suppress("unused")
class MapMmpp2Algo {
    companion object {
        // Class documentation marker for Dokka
    }
}
