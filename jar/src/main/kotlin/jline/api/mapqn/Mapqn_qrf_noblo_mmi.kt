/**
 * QRF No-Blocking MMI (Minimize Mutual Information) Approximation.
 * Port of MATLAB qrf_noblo_mmi.m.
 * @since LINE 3.0
 */
package jline.api.mapqn

import kotlin.math.ln
import kotlin.math.max

object Mapqn_qrf_noblo_mmi {

    private const val LOGTOL = 1e-6

    @JvmStatic
    fun solve(M: Int, MR_input: Int, K: IntArray, N: Int,
              mu: Array<Array<DoubleArray>>, v: Array<Array<DoubleArray>>,
              rt: Array<DoubleArray>): Mapqn_solution {

        val MR = 1
        val BB = Array(1) { DoubleArray(M) }
        val F = IntArray(M) { N }
        val Kmax = K.max()

        // Build transition rates q[i][j][k][h]
        val q = Array(M) { Array(M) { Array(Kmax) { DoubleArray(Kmax) } } }
        for (i in 0 until M) {
            for (j in 0 until M) {
                for (k in 0 until K[i]) {
                    for (h in 0 until K[i]) {
                        q[i][j][k][h] = if (j != i) {
                            rt[i][j] * mu[i][k][h]
                        } else {
                            v[i][k][h] + rt[i][i] * mu[i][k][h]
                        }
                    }
                }
            }
        }

        val numVars = computeNumVars(M, N, Kmax, MR)
        val x0 = DoubleArray(numVars) { 0.0 }
        val lb = DoubleArray(numVars) { 0.0 }
        val ub = DoubleArray(numVars) { 1.0 }

        // Build linear constraints
        val (aeqList, beqList, aubList, bubList) = buildConstraints(q, M, MR, BB, F, N, K, Kmax, numVars)

        val Aeq = aeqList.toTypedArray()
        val beq = beqList.toDoubleArray()
        val Aub = if (aubList.isEmpty()) null else aubList.toTypedArray()
        val bub = if (bubList.isEmpty()) null else bubList.toDoubleArray()

        // MMI objective
        val objective: (DoubleArray) -> Double = { x ->
            val p2 = unflattenP2(x, M, N, Kmax, MR)
            var fobj = 0.0
            for (m in 0 until MR) {
                for (i in 0 until M) {
                    for (ki in 0 until K[i]) {
                        for (j in 0 until M) {
                            if (i != j) {
                                for (kj in 0 until K[j]) {
                                    for (ni in 1..F[i]) {
                                        for (nj in 1..F[j]) {
                                            val pij = p2[i][ni][ki][j][nj][kj][m]
                                            val pii = p2[i][ni][ki][i][ni][ki][m]
                                            val pjj = p2[j][nj][kj][j][nj][kj][m]
                                            fobj += pij * (ln(LOGTOL + pij) - ln(LOGTOL + pii) - ln(LOGTOL + pjj))
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            fobj
        }

        val xOpt = Mapqn_nlp_solver.solve(objective, numVars, Aeq, beq, Aub, bub, lb, ub, x0)

        return extractResults(xOpt, M, N, K, Kmax, F, MR)
    }

    // Shared helper functions

    internal fun computeNumVars(M: Int, N: Int, Kmax: Int, MR: Int): Int {
        return M * (N + 1) * Kmax * M * (N + 1) * Kmax * MR + M * Kmax
    }

    internal fun p2Index(j: Int, nj: Int, k: Int, i: Int, ni: Int, h: Int, m: Int,
                         M: Int, N: Int, Kmax: Int, MR: Int): Int {
        var idx = (N + 1) * Kmax * M * (N + 1) * Kmax * MR * j
        idx += Kmax * M * (N + 1) * Kmax * MR * nj
        idx += M * (N + 1) * Kmax * MR * k
        idx += (N + 1) * Kmax * MR * i
        idx += Kmax * MR * ni
        idx += MR * h
        idx += m
        return idx
    }

    internal fun eIndex(i: Int, k: Int, M: Int, N: Int, Kmax: Int, MR: Int): Int {
        val p2Size = M * (N + 1) * Kmax * M * (N + 1) * Kmax * MR
        return p2Size + Kmax * i + k
    }

    internal fun unflattenP2(x: DoubleArray, M: Int, N: Int, Kmax: Int, MR: Int):
            Array<Array<Array<Array<Array<Array<Array<Double>>>>>>> {
        val p2 = Array(M) { Array(N + 1) { Array(Kmax) { Array(M) { Array(N + 1) { Array(Kmax) { Array(MR) { 0.0 } } } } } } }
        var ctr = 0
        for (j in 0 until M) {
            for (nj in 0..N) {
                for (k in 0 until Kmax) {
                    for (i in 0 until M) {
                        for (ni in 0..N) {
                            for (h in 0 until Kmax) {
                                for (m in 0 until MR) {
                                    p2[j][nj][k][i][ni][h][m] = x[ctr++]
                                }
                            }
                        }
                    }
                }
            }
        }
        return p2
    }

    internal fun extractResults(xOpt: DoubleArray, M: Int, N: Int, K: IntArray, Kmax: Int, F: IntArray, MR: Int): Mapqn_solution {
        val p2 = unflattenP2(xOpt, M, N, Kmax, MR)
        val UN = DoubleArray(M)
        val QN = DoubleArray(M)
        for (ti in 0 until M) {
            for (m in 0 until MR) {
                for (ni in 1..F[ti]) {
                    for (ki in 0 until K[ti]) {
                        UN[ti] += p2[ti][ni][ki][ti][ni][ki][m]
                        QN[ti] += ni * p2[ti][ni][ki][ti][ni][ki][m]
                    }
                }
            }
        }
        val vars = mutableMapOf<String, Double>()
        for (i in 0 until M) {
            vars["UN_${i + 1}"] = UN[i]
            vars["QN_${i + 1}"] = QN[i]
        }
        return Mapqn_solution(0.0, vars)
    }

    internal data class ConstraintSet(
        val aeq: MutableList<DoubleArray>,
        val beq: MutableList<Double>,
        val aub: MutableList<DoubleArray>,
        val bub: MutableList<Double>
    )

    internal fun buildConstraints(
        q: Array<Array<Array<DoubleArray>>>,
        M: Int, MR: Int, BB: Array<DoubleArray>, F: IntArray, N: Int, K: IntArray, Kmax: Int, numVars: Int
    ): ConstraintSet {
        val aeq = mutableListOf<DoubleArray>()
        val beq = mutableListOf<Double>()
        val aub = mutableListOf<DoubleArray>()
        val bub = mutableListOf<Double>()

        // ONE: normalization
        for (j in 0 until M) {
            val row = DoubleArray(numVars)
            for (nj in 0..N) {
                for (k in 0 until K[j]) {
                    for (m in 0 until MR) {
                        row[p2Index(j, nj, k, j, nj, k, m, M, N, Kmax, MR)] += 1.0
                    }
                }
            }
            aeq.add(row); beq.add(1.0)
        }

        // ZERO1: i==j and ni==nj and h!=k
        for (j in 0 until M) for (k in 0 until K[j]) for (nj in 0..N) for (i in 0 until M)
            for (h in 0 until K[i]) for (ni in 0..N) for (m in 0 until MR) {
                if (i == j && nj == ni && h != k) {
                    val row = DoubleArray(numVars)
                    row[p2Index(j, nj, k, i, ni, h, m, M, N, Kmax, MR)] = 1.0
                    aeq.add(row); beq.add(0.0)
                }
            }

        // ZERO2: i==j and nj!=ni
        for (j in 0 until M) for (k in 0 until K[j]) for (nj in 0..N) for (i in 0 until M)
            for (h in 0 until K[i]) for (ni in 0..N) for (m in 0 until MR) {
                if (i == j && nj != ni) {
                    val row = DoubleArray(numVars)
                    row[p2Index(j, nj, k, i, ni, h, m, M, N, Kmax, MR)] = 1.0
                    aeq.add(row); beq.add(0.0)
                }
            }

        // ZERO3: i!=j and nj+ni>N
        for (j in 0 until M) for (k in 0 until K[j]) for (nj in 0..N) for (i in 0 until M)
            for (h in 0 until K[i]) for (ni in 0..N) for (m in 0 until MR) {
                if (i != j && nj + ni > N) {
                    val row = DoubleArray(numVars)
                    row[p2Index(j, nj, k, i, ni, h, m, M, N, Kmax, MR)] = 1.0
                    aeq.add(row); beq.add(0.0)
                }
            }

        // ZERO6: nj > F[j]
        for (j in 0 until M) for (k in 0 until K[j]) for (nj in (F[j] + 1)..N) for (i in 0 until M)
            for (h in 0 until K[i]) for (ni in 0..N) for (m in 0 until MR) {
                val row = DoubleArray(numVars)
                row[p2Index(j, nj, k, i, ni, h, m, M, N, Kmax, MR)] = 1.0
                aeq.add(row); beq.add(0.0)
            }

        // SYMMETRY
        for (j in 0 until M) for (nj in 0..N) for (k in 0 until K[j]) for (i in 0 until M)
            for (ni in 0..N) for (h in 0 until K[i]) for (m in 0 until MR) {
                val row = DoubleArray(numVars)
                row[p2Index(i, ni, h, j, nj, k, m, M, N, Kmax, MR)] += 1.0
                row[p2Index(j, nj, k, i, ni, h, m, M, N, Kmax, MR)] -= 1.0
                aeq.add(row); beq.add(0.0)
            }

        // MARGINALS
        for (j in 0 until M) for (k in 0 until K[j]) for (nj in 0..N) for (i in 0 until M)
            for (m in 0 until MR) {
                if (i != j) {
                    val row = DoubleArray(numVars)
                    row[p2Index(j, nj, k, j, nj, k, m, M, N, Kmax, MR)] += 1.0
                    for (ni in 0..(N - nj)) {
                        for (h in 0 until K[i]) {
                            row[p2Index(j, nj, k, i, ni, h, m, M, N, Kmax, MR)] -= 1.0
                        }
                    }
                    aeq.add(row); beq.add(0.0)
                }
            }

        // UEFF
        for (j in 0 until M) for (i in 0 until M) for (ki in 0 until K[i]) {
            val row = DoubleArray(numVars)
            row[eIndex(i, ki, M, N, Kmax, MR)] += 1.0
            for (nj in 0..N) for (kj in 0 until K[j]) for (m in 0 until MR) for (ni in 1..N) {
                if (BB[m][i] == 0.0) {
                    row[p2Index(j, nj, kj, i, ni, ki, m, M, N, Kmax, MR)] -= 1.0
                }
            }
            aeq.add(row); beq.add(0.0)
        }

        // THM1
        for (i in 0 until M) for (k in 0 until K[i]) {
            val row = DoubleArray(numVars)
            for (j in 0 until M) for (h in 0 until K[i]) {
                row[eIndex(i, k, M, N, Kmax, MR)] += q[i][j][k][h]
            }
            for (j in 0 until M) for (h in 0 until K[i]) {
                row[eIndex(i, h, M, N, Kmax, MR)] -= q[i][j][h][k]
            }
            aeq.add(row); beq.add(0.0)
        }

        // THM2
        for (j in 0 until M) for (k in 0 until K[j]) for (nj in 0..F[j]) for (m in 0 until MR) {
            val row = DoubleArray(numVars)
            for (i in 0 until M) for (ni in 1..F[i]) for (ki in 0 until K[i]) {
                row[p2Index(j, nj, k, i, ni, ki, m, M, N, Kmax, MR)] += ni.toDouble()
            }
            row[p2Index(j, nj, k, j, nj, k, m, M, N, Kmax, MR)] -= N.toDouble()
            aeq.add(row); beq.add(0.0)
        }

        // COR1
        val rowCor = DoubleArray(numVars)
        for (m in 0 until MR) for (i in 0 until M) for (j in 0 until M)
            for (nj in 1..F[j]) for (ni in 1..F[i]) for (ki in 0 until K[i]) for (kj in 0 until K[j]) {
                rowCor[p2Index(j, nj, kj, i, ni, ki, m, M, N, Kmax, MR)] += (ni * nj).toDouble()
            }
        aeq.add(rowCor); beq.add((N * N).toDouble())

        // THM4 (inequality <= 0)
        for (j in 0 until M) for (k in 0 until K[j]) for (i in 0 until M) for (m in 0 until MR) {
            val row = DoubleArray(numVars)
            for (t in 0 until M) for (h in 0 until K[t]) for (njIdx in 0..N) for (nt in 0..N) {
                row[p2Index(j, njIdx, k, t, nt, h, m, M, N, Kmax, MR)] -= nt.toDouble()
            }
            for (h in 0 until K[i]) for (njIdx in 0..N) for (ni in 1..N) {
                row[p2Index(j, njIdx, k, i, ni, h, m, M, N, Kmax, MR)] += N.toDouble()
            }
            aub.add(row); bub.add(0.0)
        }

        return ConstraintSet(aeq, beq, aub, bub)
    }
}
