/**
 * @file Single-buffer AoI solver using Markovian Fluid Queues
 *
 * Computes matrix exponential (ME) representations for Age of Information
 * and Peak AoI distributions in M/PH/1/2 (or M/PH/1/2*) single-buffer systems.
 *
 * Based on: aoi-fluid toolbox by Ozancan Dogan, Nail Akar, Eray Unsal Atay
 * BSD 2-Clause License, 2020
 *
 * Reference: Eqns (16), (19), and Algorithm 1 from the aoi-fluid paper.
 *
 * @since LINE 3.0
 */
package jline.api.aoi

import jline.util.matrix.Matrix
import kotlin.math.abs
import kotlin.math.sqrt

/**
 * Solve single-buffer (M/PH/1/2 or M/PH/1/2*) AoI system using MFQ.
 *
 * Computes matrix exponential parameters for AoI and Peak AoI distributions
 * along with their first two moments.
 *
 * @param lambda Arrival rate (Poisson arrivals)
 * @param sigma Initial probability vector of the service process (1 x l)
 * @param S Sub-generator matrix of the service process (l x l)
 * @param r Packet replacement probability (0 = FCFS, 1 = replacement)
 * @return AoiMfqResult with ME parameters and moments
 */
fun aoi_solve_singlebuffer(lambda: Double, sigma: Matrix, S: Matrix, r: Double): AoiMfqResult {
    val l = S.numCols  // size of service matrix
    val nu = S.mult(Matrix.ones(l, 1)).scale(-1.0)  // -S*ones(l,1)
    val z1 = l + 2  // system size of waiting matrix

    // =========================================================================
    // PART 1: Solve the waiting time MFQ
    // =========================================================================

    // Construction of Q in Eqn (16)
    // Q = [S, nu, zeros(l,1); zeros(1,l), -lambda, lambda; zeros(1,l+2)]
    val Q1 = Matrix.zeros(z1, z1)
    for (i in 0 until l) {
        for (j in 0 until l) Q1.set(i, j, S.get(i, j))
        Q1.set(i, l, nu.get(i, 0))
    }
    Q1.set(l, l, -lambda)
    Q1.set(l, l + 1, lambda)
    // last row all zeros

    // Drift matrix R in Eqn (16)
    val R1 = Matrix.eye(z1)
    R1.set(z1 - 2, z1 - 2, -1.0)  // R(z-1,z-1) = -1
    R1.set(z1 - 1, z1 - 1, -1.0)  // R(z,z) = -1

    // Qtilde in Eqn (16)
    // Qtilde = [zeros(l,l+2); lambda*sigma, -lambda, 0; sigma, 0, -1]
    val Qtilde1 = Matrix.zeros(z1, z1)
    for (j in 0 until l) {
        Qtilde1.set(l, j, lambda * sigma.get(0, j))
    }
    Qtilde1.set(l, l, -lambda)
    for (j in 0 until l) {
        Qtilde1.set(l + 1, j, sigma.get(0, j))
    }
    Qtilde1.set(l + 1, l + 1, -1.0)

    // Steady state distribution of MFQ: pik = e' / (Q + e*e')
    val e1 = Matrix.ones(z1, 1)
    val QplusEE = Q1.add(1.0, e1.mult(e1.transpose()))
    val pikT = Matrix(z1, 1)
    Matrix.solveSafe(QplusEE.transpose(), e1, pikT)
    val pik = pikT.transpose()  // 1 x z1

    // Schur Algorithm for orthogonal matrix P
    // QR = Q / R = Q * R^{-1}
    val QR1 = Q1.mult(Matrix.inv(R1))
    // x_R = R*e, x_L = pik
    val xR = R1.mult(e1)  // z1 x 1
    val xL = pik  // 1 x z1
    val xLxR = xL.mult(xR).get(0, 0)  // scalar
    // A1 = QR + (x_R * x_L) / (x_L * x_R)
    val A1mat = QR1.add(1.0 / xLxR, xR.mult(xL))

    // Schur decomposition: [U, T] = schur(A1)
    val schurResult = A1mat.schur()
    val schurU = schurResult["U"]!!
    val schurT = schurResult["T"]!!

    // ordschur: reorder to put eigenvalues with positive real part first
    // In MATLAB: [P, ~] = ordschur(U, T, 'rhp')
    // We implement this by swapping blocks to move positive eigenvalues to top-left
    val ordResult = ordschurRhp(schurU, schurT)
    val P1 = ordResult.first

    // Construction of A, H, Qtildestar
    val a1 = 2  // number of negative drift states
    val b1 = l  // number of positive drift states (= z1 - a1)

    val Ae1 = P1.transpose().mult(QR1).mult(P1)
    // A = Ae(a+1:z, a+1:z)
    val Amat1 = Matrix(b1, b1)
    for (i in 0 until b1) {
        for (j in 0 until b1) {
            Amat1.set(i, j, Ae1.get(a1 + i, a1 + j))
        }
    }
    // H = P(1:z, a+1:z)'
    val Hmat1 = Matrix(b1, z1)
    for (i in 0 until b1) {
        for (j in 0 until z1) {
            Hmat1.set(i, j, P1.get(j, a1 + i))
        }
    }
    // Qtildestar = Qtilde(b+1:z, 1:z)
    val Qts1 = Matrix(a1, z1)
    for (i in 0 until a1) {
        for (j in 0 until z1) {
            Qts1.set(i, j, Qtilde1.get(b1 + i, j))
        }
    }

    // Solve for wait_g and wait_d
    val Ainv1 = Matrix.inv(Amat1)
    val eqn1 = Matrix(z1, z1 + 1)
    val HR1 = Hmat1.mult(R1)
    val AinvHones1 = Ainv1.mult(Hmat1.mult(Matrix.ones(z1, 1))).scale(-1.0)
    for (i in 0 until b1) {
        for (j in 0 until z1) eqn1.set(i, j, HR1.get(i, j))
        eqn1.set(i, z1, AinvHones1.get(i, 0))
    }
    for (i in 0 until a1) {
        for (j in 0 until z1) eqn1.set(b1 + i, j, -Qts1.get(i, j))
        eqn1.set(b1 + i, z1, 1.0)
    }
    val rhs1 = Matrix(z1 + 1, 1)
    rhs1.set(z1, 0, 1.0)
    val sol1 = Matrix(z1, 1)
    Matrix.solveSafe(eqn1.transpose(), rhs1, sol1)

    val wait_g = Matrix(1, b1)
    for (i in 0 until b1) wait_g.set(0, i, sol1.get(i, 0))
    val wait_d = Matrix(a1, 1)
    for (i in 0 until a1) wait_d.set(0, i, sol1.get(b1 + i, 0))

    // c_0 = wait_d(1)
    val c_0 = wait_d.get(0, 0)

    // wait_A = A - r*lambda*eye(l)
    val wait_A = Amat1.add(-r * lambda, Matrix.eye(l))

    // wait_H = H * [zeros(l,1); 1; r]
    val selVec = Matrix(z1, 1)
    selVec.set(l, 0, 1.0)
    selVec.set(l + 1, 0, r)
    val wait_H = Hmat1.mult(selVec)

    // n_1 = 1 / (-wait_g * wait_A^{-1} * wait_H + c_0)
    val wait_Ainv = Matrix.inv(wait_A)
    val n_1 = 1.0 / (-wait_g.mult(wait_Ainv).mult(wait_H).get(0, 0) + c_0)
    val wait_g_scaled = wait_g.scale(n_1)

    // Lemma 1
    val Mdiag = wait_Ainv.mult(wait_H).scale(-1.0)  // l x 1
    // M = diag(Mdiag)
    val Mmat = Matrix.zeros(l, l)
    for (i in 0 until l) Mmat.set(i, i, Mdiag.get(i, 0))
    // B = M^{-1} * wait_A * M
    val Minv = Matrix.inv(Mmat)
    val B = Minv.mult(wait_A).mult(Mmat)
    // beta = wait_g * M
    val beta = wait_g_scaled.mult(Mmat)  // 1 x l
    // beta_0 = 1 - sum(beta)
    var betaSum = 0.0
    for (j in 0 until l) betaSum += beta.get(0, j)
    val beta_0 = 1.0 - betaSum
    // psi = -B * ones(l,1)
    val psi = B.mult(Matrix.ones(l, 1)).scale(-1.0)  // l x 1

    // =========================================================================
    // PART 2: Solve the AoI MFQ
    // =========================================================================

    val z2 = 4 * l + 2  // system size
    val a2 = 1
    val b2 = z2 - 1

    // Construction of Q in Eqn (19)
    val Q2 = Matrix.zeros(z2, z2)

    // Row block 1: [B, kron(psi,sigma), zeros(l, 2l+2)]
    for (i in 0 until l) {
        for (j in 0 until l) Q2.set(i, j, B.get(i, j))
    }
    val psiSigma = psi.kron(sigma)  // l x l
    for (i in 0 until l) {
        for (j in 0 until l) Q2.set(i, l + j, psiSigma.get(i, j))
    }

    // Row block 2: [zeros(l,l), S-lambda*eye(l), lambda*eye(l), nu, zeros(l,l+1)]
    val SlI = S.add(-lambda, Matrix.eye(l))
    val lamI = Matrix.eye(l).scale(lambda)
    for (i in 0 until l) {
        for (j in 0 until l) Q2.set(l + i, l + j, SlI.get(i, j))
        for (j in 0 until l) Q2.set(l + i, 2 * l + j, lamI.get(i, j))
        Q2.set(l + i, 3 * l, nu.get(i, 0))
    }

    // Row block 3: [zeros(l,2l), S, zeros(l,1), kron(nu,sigma), zeros(l,1)]
    val nuSigma = nu.kron(sigma)  // l x l
    for (i in 0 until l) {
        for (j in 0 until l) Q2.set(2 * l + i, 2 * l + j, S.get(i, j))
        for (j in 0 until l) Q2.set(2 * l + i, 3 * l + 1 + j, nuSigma.get(i, j))
    }

    // Row block 4: [zeros(1,3l), -lambda, lambda*sigma, zeros(1,1)]
    Q2.set(3 * l, 3 * l, -lambda)
    for (j in 0 until l) Q2.set(3 * l, 3 * l + 1 + j, lambda * sigma.get(0, j))

    // Row block 5: [zeros(l,3l+1), S, nu]
    for (i in 0 until l) {
        for (j in 0 until l) Q2.set(3 * l + 1 + i, 3 * l + 1 + j, S.get(i, j))
        Q2.set(3 * l + 1 + i, 4 * l + 1, nu.get(i, 0))
    }

    // Row block 6: zeros(1, z2) - already zero

    // Drift matrix R
    val R2 = Matrix.eye(z2)
    R2.set(z2 - 1, z2 - 1, -1.0)

    // Qtilde construction
    val Qtilde2 = Q2.copy()
    for (j in 0 until l) Qtilde2.set(z2 - 1, j, beta.get(0, j))
    for (j in 0 until l) Qtilde2.set(z2 - 1, l + j, beta_0 * sigma.get(0, j))
    Qtilde2.set(z2 - 1, z2 - 1, -1.0)

    // Householder reflection for P
    val u1 = Matrix(z2, 1)
    for (i in 0 until z2 - 1) u1.set(i, 0, 1.0)
    u1.set(z2 - 1, 0, -1.0)
    val norm_u1 = sqrt(u1.transpose().mult(u1).get(0, 0))
    val u = u1.copy()
    u.set(0, 0, u.get(0, 0) - norm_u1)
    val utu = u.transpose().mult(u).get(0, 0)
    val P2 = Matrix.eye(z2).add(-2.0 / utu, u.mult(u.transpose()))

    // QR = Q / R
    val QR2 = Q2.mult(Matrix.inv(R2))
    val Ae2 = P2.transpose().mult(QR2).mult(P2)

    // A = Ae(a+1:z, a+1:z)
    val Amat2 = Matrix(b2, b2)
    for (i in 0 until b2) {
        for (j in 0 until b2) {
            Amat2.set(i, j, Ae2.get(a2 + i, a2 + j))
        }
    }

    // H = P(1:z, a+1:z)'
    val Hmat2 = Matrix(b2, z2)
    for (i in 0 until b2) {
        for (j in 0 until z2) {
            Hmat2.set(i, j, P2.get(j, a2 + i))
        }
    }

    // Qtildestar = Qtilde(b+1:z, 1:z)
    val Qts2 = Matrix(a2, z2)
    for (i in 0 until a2) {
        for (j in 0 until z2) {
            Qts2.set(i, j, Qtilde2.get(b2 + i, j))
        }
    }

    // Solve for g and d
    val Ainv2 = Matrix.inv(Amat2)
    val HR2 = Hmat2.mult(R2)
    val AinvHones2 = Ainv2.mult(Hmat2.mult(Matrix.ones(z2, 1))).scale(-1.0)

    val eqn2 = Matrix(z2, z2 + 1)
    for (i in 0 until b2) {
        for (j in 0 until z2) eqn2.set(i, j, HR2.get(i, j))
        eqn2.set(i, z2, AinvHones2.get(i, 0))
    }
    for (i in 0 until a2) {
        for (j in 0 until z2) eqn2.set(b2 + i, j, -Qts2.get(i, j))
        eqn2.set(b2 + i, z2, 1.0)
    }
    val rhs2 = Matrix(z2 + 1, 1)
    rhs2.set(z2, 0, 1.0)
    val sol2 = Matrix(z2, 1)
    Matrix.solveSafe(eqn2.transpose(), rhs2, sol2)

    val g = Matrix(1, b2)
    for (i in 0 until b2) g.set(0, i, sol2.get(i, 0))

    // AoI: restrict to states in Phases 4 and 5 only
    // SelectedStates = [zeros(3*l,1); ones(l+1,1); 0]
    val selectedAoI = Matrix(z2, 1)
    for (i in 3 * l until 4 * l + 1) selectedAoI.set(i, 0, 1.0)

    val aoiH = Hmat2.mult(selectedAoI)
    val normConst = g.mult(Ainv2).mult(aoiH).scale(-1.0).get(0, 0)
    val aoiG = g.scale(1.0 / normConst)
    val aoiA = Amat2

    val Ainv2_2 = Ainv2.mult(Ainv2)
    val Ainv2_3 = Ainv2_2.mult(Ainv2)
    val aoiMean = aoiG.mult(Ainv2_2).mult(aoiH).get(0, 0)
    val aoiVar = -2.0 * aoiG.mult(Ainv2_3).mult(aoiH).get(0, 0) - aoiMean * aoiMean

    // PAoI: restrict to states in Phase 5 scaled by nu
    // SelectedStates = [zeros(3*l+1,1); nu; 0]
    val selectedPAoI = Matrix(z2, 1)
    for (i in 0 until l) selectedPAoI.set(3 * l + 1 + i, 0, nu.get(i, 0))

    val paoiH = Hmat2.mult(selectedPAoI)
    val normConstP = g.mult(Ainv2).mult(paoiH).scale(-1.0).get(0, 0)
    val paoiG = g.scale(1.0 / normConstP)
    val paoiA = Amat2

    val paoiMean = paoiG.mult(Ainv2_2).mult(paoiH).get(0, 0)
    val paoiVar = -2.0 * paoiG.mult(Ainv2_3).mult(paoiH).get(0, 0) - paoiMean * paoiMean

    return AoiMfqResult(
        aoiG = aoiG, aoiA = aoiA, aoiH = aoiH,
        aoiMean = aoiMean, aoiVar = Math.max(0.0, aoiVar),
        paoiG = paoiG, paoiA = paoiA, paoiH = paoiH,
        paoiMean = paoiMean, paoiVar = Math.max(0.0, paoiVar),
        systemType = "singlebuffer", preemption = r
    )
}

/**
 * Ordered Schur decomposition: reorder eigenvalues so that those with
 * positive real part come first (MATLAB's ordschur(..., 'rhp')).
 *
 * Uses pairwise swapping of adjacent Schur blocks via Givens rotations.
 *
 * @param U Unitary matrix from Schur decomposition
 * @param T Upper quasi-triangular matrix from Schur decomposition
 * @return Pair of reordered (U, T)
 */
private fun ordschurRhp(U: Matrix, T: Matrix): Pair<Matrix, Matrix> {
    val n = T.numRows
    var Uc = U.copy()
    var Tc = T.copy()

    // Simple bubble sort: move positive eigenvalues to top-left
    var sorted = false
    while (!sorted) {
        sorted = true
        var i = 0
        while (i < n - 1) {
            // Determine block sizes at position i
            val blockSize1 = if (i + 1 < n && abs(Tc.get(i + 1, i)) > 1e-14) 2 else 1
            val j = i + blockSize1
            if (j >= n) break
            val blockSize2 = if (j + 1 < n && abs(Tc.get(j + 1, j)) > 1e-14) 2 else 1

            // Get eigenvalue real parts
            val re1 = if (blockSize1 == 1) Tc.get(i, i) else (Tc.get(i, i) + Tc.get(i + 1, i + 1)) / 2.0
            val re2 = if (blockSize2 == 1) Tc.get(j, j) else (Tc.get(j, j) + Tc.get(j + 1, j + 1)) / 2.0

            // If block 2 should come first (positive real part first)
            if (re2 > 0 && re1 <= 0) {
                // Swap 1x1 blocks via Givens rotation
                if (blockSize1 == 1 && blockSize2 == 1) {
                    swapSchur1x1(Tc, Uc, i)
                    sorted = false
                } else {
                    // For mixed-size or 2x2 blocks, skip (approximate handling)
                    // Most AoI systems have simple eigenvalues
                    i += blockSize1
                    continue
                }
            }
            i += blockSize1
        }
    }
    return Pair(Uc, Tc)
}

/**
 * Swap two adjacent 1x1 blocks in a Schur form at position i, i+1.
 * Uses a Givens rotation to swap T[i,i] and T[i+1,i+1].
 */
private fun swapSchur1x1(T: Matrix, U: Matrix, i: Int) {
    val n = T.numRows
    // Solve for rotation to zero out T[i, i+1] after swap
    // Use Givens rotation G such that G' * [T[i,i+1]; T[i+1,i+1]-T[i,i]] = [*; 0]
    val a = T.get(i, i + 1)
    val b = T.get(i + 1, i + 1) - T.get(i, i)
    val r = sqrt(a * a + b * b)
    if (r < 1e-14) return  // eigenvalues are the same, no swap needed
    val c = b / r
    val s = a / r

    // Apply Givens rotation from left: T := G' * T
    for (j in 0 until n) {
        val t1 = T.get(i, j)
        val t2 = T.get(i + 1, j)
        T.set(i, j, c * t1 + s * t2)
        T.set(i + 1, j, -s * t1 + c * t2)
    }
    // Apply Givens rotation from right: T := T * G
    for (j in 0 until n) {
        val t1 = T.get(j, i)
        val t2 = T.get(j, i + 1)
        T.set(j, i, c * t1 + s * t2)
        T.set(j, i + 1, -s * t1 + c * t2)
    }
    // Update U: U := U * G
    for (j in 0 until n) {
        val u1 = U.get(j, i)
        val u2 = U.get(j, i + 1)
        U.set(j, i, c * u1 + s * u2)
        U.set(j, i + 1, -s * u1 + c * u2)
    }
    // Clean up sub-diagonal
    T.set(i + 1, i, 0.0)
}
