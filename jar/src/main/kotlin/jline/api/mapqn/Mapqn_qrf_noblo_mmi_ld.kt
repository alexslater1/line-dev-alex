/**
 * QRF No-Blocking MMI with Load-Dependent Rates.
 * Port of MATLAB qrf_noblo_mmi_ld.m.
 * @since LINE 3.0
 */
package jline.api.mapqn

import kotlin.math.ln

object Mapqn_qrf_noblo_mmi_ld {

    private const val LOGTOL = 1e-6

    @JvmStatic
    @JvmOverloads
    fun solve(MAPs: Array<Array<Array<DoubleArray>>>, N: Int, rt: Array<DoubleArray>,
              alpha: Array<DoubleArray>? = null): Mapqn_solution {
        val M = MAPs.size
        val K = IntArray(M) { MAPs[it][0].size }
        val Kmax = K.max()
        val alphaEff = alpha ?: Array(M) { DoubleArray(N) { 1.0 } }

        val mu = Array(M) { Array(Kmax) { DoubleArray(Kmax) } }
        val v = Array(M) { Array(Kmax) { DoubleArray(Kmax) } }
        for (i in 0 until M) {
            val D0 = MAPs[i][0]
            val D1 = MAPs[i][1]
            for (h in 0 until K[i]) for (k in 0 until K[i]) {
                mu[i][h][k] = D1[h][k]
                v[i][k][h] = if (h == k) 0.0 else D0[h][k]
            }
        }

        val MR = 1
        val BB = Array(1) { DoubleArray(M) }
        val F = IntArray(M) { N }

        // Build 4D q (non-load-dependent for constraints)
        val q = Array(M) { Array(M) { Array(Kmax) { DoubleArray(Kmax) } } }
        for (i in 0 until M) for (j in 0 until M) for (k in 0 until K[i]) for (h in 0 until K[i]) {
            q[i][j][k][h] = if (j != i) rt[i][j] * mu[i][k][h] else v[i][k][h] + rt[i][i] * mu[i][k][h]
        }

        val numVars = Mapqn_qrf_noblo_mmi.computeNumVars(M, N, Kmax, MR)
        // Normalized uniform initial point
        val x0 = DoubleArray(numVars) { 1.0 }
        val totalSum = x0.sum()
        for (i in x0.indices) x0[i] /= totalSum
        val lb = DoubleArray(numVars) { 0.0 }
        val ub = DoubleArray(numVars) { 1.0 }

        val (aeqList, beqList, aubList, bubList) = Mapqn_qrf_noblo_mmi.buildConstraints(q, M, MR, BB, F, N, K, Kmax, numVars)

        val Aeq = aeqList.toTypedArray()
        val beq = beqList.toDoubleArray()
        val Aub = if (aubList.isEmpty()) null else aubList.toTypedArray()
        val bub = if (bubList.isEmpty()) null else bubList.toDoubleArray()

        // MMI objective
        val objective: (DoubleArray) -> Double = { x ->
            val p2 = Mapqn_qrf_noblo_mmi.unflattenP2(x, M, N, Kmax, MR)
            var fobj = 0.0
            for (m in 0 until MR) for (i in 0 until M) for (ki in 0 until K[i]) for (j in 0 until M) {
                if (i != j) {
                    for (kj in 0 until K[j]) for (ni in 1..F[i]) for (nj in 1..F[j]) {
                        val pij = p2[i][ni][ki][j][nj][kj][m]
                        val pii = p2[i][ni][ki][i][ni][ki][m]
                        val pjj = p2[j][nj][kj][j][nj][kj][m]
                        fobj += pij * (ln(LOGTOL + pij) - ln(LOGTOL + pii) - ln(LOGTOL + pjj))
                    }
                }
            }
            fobj
        }

        val xOpt = Mapqn_nlp_solver.solve(objective, numVars, Aeq, beq, Aub, bub, lb, ub, x0)
        return Mapqn_qrf_noblo_mmi.extractResults(xOpt, M, N, K, Kmax, F, MR)
    }
}
