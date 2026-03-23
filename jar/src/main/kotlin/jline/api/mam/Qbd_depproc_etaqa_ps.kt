/**
 * @file QBD departure process ETAQA truncation for PS discipline
 *
 * Constructs a MAP approximation of the departure process for MAP/MAP/1-PS
 * (Processor Sharing) queues using ETAQA truncation at level n.
 * The key difference from FCFS is rate-dependent service completion splitting:
 * at level j, service completions occur at rate B*(1/j).
 *
 * @since LINE 3.0
 */
package jline.api.mam

import jline.util.matrix.Matrix
import jline.util.matrix.MatrixCell
import jline.lib.smc.QBD_CR

/**
 * Compute MAP departure process for MAP/MAP/1-PS via ETAQA truncation
 *
 * Builds a finite MAP {D0, D1} that approximates the departure process under
 * Processor Sharing discipline by truncating the QBD state space at level n.
 * Under PS, the service completion rate at level j is split: fraction 1/j goes
 * to D1 (departure) and fraction (1-1/j) stays in D0 (non-departure transition).
 *
 * The D0 block structure starts as in FCFS, then adds:
 *   - Bbar at block (n, n-1) and Bhat*(n-1)/n at block (n, n) for the tail level
 *   - B*(1-1/j) at block (j, j-1) for levels j=1..n-1 (PS non-departure fraction)
 *
 * The D1 block structure has:
 *   - Bbar at block (n, n-1) and Bhat/n at block (n, n) for the tail level
 *   - B*(1/j) at block (j, j-1) for levels j=1..n-1 (PS departure fraction)
 *
 * @param MAPa Arrival MAP (MatrixCell with D0 and D1)
 * @param MAPs Service MAP (MatrixCell with D0 and D1)
 * @param n    Truncation level (number of QBD levels beyond level 0)
 * @return MatrixCell containing D0 and D1 matrices of the departure MAP
 */
fun qbd_depproc_etaqa_ps(MAPa: MatrixCell, MAPs: MatrixCell, n: Int): MatrixCell {
    val na = MAPa[0].numRows
    val ns = MAPs[0].numRows
    val lvlsz = ns * na

    // Construct QBD transition matrices
    val IA = Matrix.eye(na)
    val IS = Matrix.eye(ns)
    val F = MAPa[1].kron(IS)                                    // Forward (arrivals)
    val L = MAPa[0].kron(IS).add(IA.kron(MAPs[0]))             // Local (Kronecker sum)
    val B = IA.kron(MAPs[1])                                     // Backward (services)
    val L0 = MAPa[0].kron(IS)                                   // Boundary local

    // Solve QBD for R matrix
    val qbdResult = QBD_CR(B, L, F, null, null, null, null)
    val R = qbdResult["R"] ?: throw RuntimeException("QBD_CR failed to compute R matrix")

    // Compute derived matrices
    val G = L.add(R.mult(B)).neg().inv().mult(B)                // G = inv(-L - R*B) * B
    val Lhat = F.add(L)                                          // Lhat = F + L
    val Bbar = B.add(F.mult(G))                                  // Bbar = B + F*G
    val Bhat = F.mult(G)                                         // Bhat = F*G

    // Total dimension: (n+1) levels, each of size lvlsz
    val totalDim = (n + 1) * lvlsz

    // ---------------------------------------------------------------
    // Build D0: same base structure as FCFS
    // ---------------------------------------------------------------
    val D0 = Matrix.zeros(totalDim, totalDim)

    // Level 0: L0 on diagonal, F on superdiagonal
    D0.insertSubMatrix(0, 0, lvlsz, lvlsz, L0)
    if (n >= 1) {
        D0.insertSubMatrix(0, lvlsz, lvlsz, 2 * lvlsz, F)
    }

    // Levels 1..n: L on diagonal, F on superdiagonal
    for (j in 1..n) {
        val rs = j * lvlsz
        D0.insertSubMatrix(rs, rs, rs + lvlsz, rs + lvlsz, L)
        if (j < n) {
            D0.insertSubMatrix(rs, rs + lvlsz, rs + lvlsz, rs + 2 * lvlsz, F)
        }
    }

    // Overwrite block (n-1, n-1) with Lhat = F + L
    if (n >= 1) {
        val rs = (n - 1) * lvlsz
        D0.insertSubMatrix(rs, rs, rs + lvlsz, rs + lvlsz, Lhat)
    }

    // ---------------------------------------------------------------
    // PS-specific additions to D0:
    //   D0 += kron(diag(vn,-1), Bbar) + kron(diag([0,vn]), Bhat)*(n-1)/n
    // This adds Bbar at block (n, n-1) and Bhat*(n-1)/n at block (n, n)
    // ---------------------------------------------------------------
    if (n >= 1) {
        val rsN = n * lvlsz
        val csN1 = (n - 1) * lvlsz
        // Add Bbar at block (n, n-1)
        for (i in 0 until lvlsz) {
            for (j in 0 until lvlsz) {
                D0[rsN + i, csN1 + j] = D0[rsN + i, csN1 + j] + Bbar[i, j]
            }
        }
        // Add Bhat * (n-1)/n at block (n, n)
        val tailScale = (n - 1).toDouble() / n.toDouble()
        for (i in 0 until lvlsz) {
            for (j in 0 until lvlsz) {
                D0[rsN + i, rsN + j] = D0[rsN + i, rsN + j] + Bhat[i, j] * tailScale
            }
        }
    }

    // ---------------------------------------------------------------
    // Build D1
    // ---------------------------------------------------------------
    val D1 = Matrix.zeros(totalDim, totalDim)

    // Tail level n: Bbar at block (n, n-1), Bhat/n at block (n, n)
    if (n >= 1) {
        val rsN = n * lvlsz
        val csN1 = (n - 1) * lvlsz
        D1.insertSubMatrix(rsN, csN1, rsN + lvlsz, csN1 + lvlsz, Bbar)
        D1.insertSubMatrix(rsN, rsN, rsN + lvlsz, rsN + lvlsz, Bhat.scale(1.0 / n.toDouble()))
    }

    // ---------------------------------------------------------------
    // PS rate splitting for levels 1..n-1:
    //   D0 block (j, j-1) += B * (1 - 1/j)  (non-departure fraction)
    //   D1 block (j, j-1)  = B * (1/j)       (departure fraction)
    // MATLAB uses 1-indexed j=1:(n-1), mapping to 0-indexed block rows j, cols j-1
    // ---------------------------------------------------------------
    for (j in 1 until n) {
        val rs = j * lvlsz
        val cs = (j - 1) * lvlsz
        val invJ = 1.0 / j.toDouble()

        // D0: non-departure fraction B*(1-1/j)
        val nonDepartScale = 1.0 - invJ
        for (i in 0 until lvlsz) {
            for (k in 0 until lvlsz) {
                D0[rs + i, cs + k] = D0[rs + i, cs + k] + B[i, k] * nonDepartScale
            }
        }

        // D1: departure fraction B*(1/j)
        D1.insertSubMatrix(rs, cs, rs + lvlsz, cs + lvlsz, B.scale(invJ))
    }

    val result = MatrixCell(2)
    result[0] = D0
    result[1] = D1
    return result
}

/**
 * QBD departure process ETAQA algorithms for PS discipline
 */
@Suppress("unused")
class QbdDepprocEtaqaPs {
    companion object {
        // Class documentation marker for Dokka
    }
}
