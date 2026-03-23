/**
 * @file QBD departure process ETAQA truncation for FCFS discipline
 *
 * Constructs a MAP approximation of the departure process for MAP/MAP/1-FCFS
 * queues using ETAQA truncation at level n.
 *
 * @since LINE 3.0
 */
package jline.api.mam

import jline.util.matrix.Matrix
import jline.util.matrix.MatrixCell
import jline.lib.smc.QBD_CR

/**
 * Compute MAP departure process for MAP/MAP/1-FCFS via ETAQA truncation
 *
 * Builds a finite MAP {D0, D1} that approximates the departure process by
 * truncating the QBD state space at level n. The resulting MAP has dimension
 * (n+1)*na*ns where na, ns are the sizes of the arrival and service MAPs.
 *
 * The block structure of D0 is:
 *   Level 0: L0 on diagonal, F on superdiagonal
 *   Levels 1..n-2: L on diagonal, F on superdiagonal
 *   Level n-1: Lhat (=F+L) on diagonal, F on superdiagonal
 *   Level n: L on diagonal
 *
 * The block structure of D1 is:
 *   Levels 1..n-1: B on subdiagonal
 *   Level n: Bbar on subdiagonal, Bhat on diagonal
 *
 * @param MAPa Arrival MAP (MatrixCell with D0 and D1)
 * @param MAPs Service MAP (MatrixCell with D0 and D1)
 * @param n    Truncation level (number of QBD levels beyond level 0)
 * @return MatrixCell containing D0 and D1 matrices of the departure MAP
 */
fun qbd_depproc_etaqa(MAPa: MatrixCell, MAPs: MatrixCell, n: Int): MatrixCell {
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
    // Build D0: matches MATLAB construction exactly
    //   kron(diag([1,vn1]),L) + kron(diag(vn1,1),F) in levels 1..n,
    //   then prepend level 0 row/col of zeros, set [L0,F] at level 0,
    //   and overwrite block (n-1,n-1) with Lhat.
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
    // MATLAB: D0(((n-1)*lvlsz+1):n*lvlsz, ((n-1)*lvlsz+1):n*lvlsz) = Lhat
    // In 0-indexed: block at row/col index n-1
    if (n >= 1) {
        val rs = (n - 1) * lvlsz
        D0.insertSubMatrix(rs, rs, rs + lvlsz, rs + lvlsz, Lhat)
    }

    // ---------------------------------------------------------------
    // Build D1: MATLAB first sets Bbar at block (n, n-1) and Bhat at
    //   block (n, n), then overwrites blocks (1,0)..(n-1,n-2) with B.
    // ---------------------------------------------------------------
    val D1 = Matrix.zeros(totalDim, totalDim)

    // Tail level n: Bbar on subdiagonal, Bhat on diagonal
    if (n >= 1) {
        val rsN = n * lvlsz
        val csN1 = (n - 1) * lvlsz
        D1.insertSubMatrix(rsN, csN1, rsN + lvlsz, csN1 + lvlsz, Bbar)
        D1.insertSubMatrix(rsN, rsN, rsN + lvlsz, rsN + lvlsz, Bhat)
    }

    // Levels 1..n-1: B on subdiagonal (service completions = departures)
    for (j in 1 until n) {
        val rs = j * lvlsz
        val cs = (j - 1) * lvlsz
        D1.insertSubMatrix(rs, cs, rs + lvlsz, cs + lvlsz, B)
    }

    val result = MatrixCell(2)
    result[0] = D0
    result[1] = D1
    return result
}

/**
 * QBD departure process ETAQA algorithms for FCFS discipline
 */
@Suppress("unused")
class QbdDepprocEtaqa {
    companion object {
        // Class documentation marker for Dokka
    }
}
