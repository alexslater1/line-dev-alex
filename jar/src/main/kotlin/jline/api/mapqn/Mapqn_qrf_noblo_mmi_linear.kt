/**
 * QRF No-Blocking NLP with Linear Constraint Matrices.
 * Port of MATLAB qrf_noblo_mmi_linear.m.
 * Note: Despite the name, this uses the MEM objective (matching MATLAB).
 * @since LINE 3.0
 */
package jline.api.mapqn

import kotlin.math.ln

object Mapqn_qrf_noblo_mmi_linear {

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

        val q = Array(M) { Array(M) { Array(Kmax) { DoubleArray(Kmax) } } }
        for (i in 0 until M) for (j in 0 until M) for (k in 0 until K[i]) for (h in 0 until K[i]) {
            q[i][j][k][h] = if (j != i) rt[i][j] * mu[i][k][h] else v[i][k][h] + rt[i][i] * mu[i][k][h]
        }

        val numVars = Mapqn_qrf_noblo_mmi.computeNumVars(M, N, Kmax, MR)
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

        // MEM objective (matching MATLAB which uses mem despite name)
        val objective: (DoubleArray) -> Double = { x ->
            val p2 = Mapqn_qrf_noblo_mmi.unflattenP2(x, M, N, Kmax, MR)
            var fobj = 0.0
            for (m in 0 until MR) for (i in 0 until M) for (k in 0 until K[i]) for (ni in 1..F[i]) {
                val pval = p2[i][ni][k][i][ni][k][m]
                fobj -= pval * ln(LOGTOL + pval)
            }
            fobj
        }

        val xOpt = Mapqn_nlp_solver.solve(objective, numVars, Aeq, beq, Aub, bub, lb, ub, x0)
        return Mapqn_qrf_noblo_mmi.extractResults(xOpt, M, N, K, Kmax, F, MR)
    }
}
