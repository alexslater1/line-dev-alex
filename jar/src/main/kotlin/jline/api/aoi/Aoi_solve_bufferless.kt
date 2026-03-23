/**
 * @file Bufferless AoI solver using Markovian Fluid Queues
 *
 * Computes matrix exponential (ME) representations for Age of Information
 * and Peak AoI distributions in PH/PH/1/1 (or PH/PH/1/1*) bufferless systems.
 *
 * Based on: aoi-fluid toolbox by Ozancan Dogan, Nail Akar, Eray Unsal Atay
 * BSD 2-Clause License, 2020
 *
 * Reference: Eqns (8), (13), and Algorithm 1 from the aoi-fluid paper.
 *
 * @since LINE 3.0
 */
package jline.api.aoi

import jline.util.matrix.Matrix
import kotlin.math.sqrt

/**
 * Result of MFQ-based AoI solver, containing ME parameters and moments.
 *
 * The AoI/PAoI distributions have CDF: F(t) = 1 - g * expm(A*t) * h
 *
 * @property aoiG Row vector g for AoI distribution
 * @property aoiA Matrix A for AoI distribution
 * @property aoiH Column vector h for AoI distribution
 * @property aoiMean Mean AoI
 * @property aoiVar Variance of AoI
 * @property paoiG Row vector g for Peak AoI distribution
 * @property paoiA Matrix A for Peak AoI distribution
 * @property paoiH Column vector h for Peak AoI distribution
 * @property paoiMean Mean Peak AoI
 * @property paoiVar Variance of Peak AoI
 * @property systemType "bufferless" or "singlebuffer"
 * @property preemption Preemption/replacement probability used
 */
data class AoiMfqResult(
    val aoiG: Matrix,
    val aoiA: Matrix,
    val aoiH: Matrix,
    val aoiMean: Double,
    val aoiVar: Double,
    val paoiG: Matrix,
    val paoiA: Matrix,
    val paoiH: Matrix,
    val paoiMean: Double,
    val paoiVar: Double,
    val systemType: String,
    val preemption: Double
)

/**
 * Solve bufferless (PH/PH/1/1 or PH/PH/1/1*) AoI system using MFQ.
 *
 * Computes matrix exponential parameters for AoI and Peak AoI distributions
 * along with their first two moments.
 *
 * @param tau Initial probability vector of the arrival process (1 x k)
 * @param T Sub-generator matrix of the arrival process (k x k)
 * @param sigma Initial probability vector of the service process (1 x l)
 * @param S Sub-generator matrix of the service process (l x l)
 * @param p Packet preemption probability (0 = FCFS, 1 = preemptive)
 * @return AoiMfqResult with ME parameters and moments
 */
fun aoi_solve_bufferless(tau: Matrix, T: Matrix, sigma: Matrix, S: Matrix, p: Double): AoiMfqResult {
    val k = T.numCols  // size of arrival matrix
    val l = S.numCols  // size of service matrix
    val kappa = T.mult(Matrix.ones(k, 1)).scale(-1.0)  // -T*ones(k,1)
    val nu = S.mult(Matrix.ones(l, 1)).scale(-1.0)     // -S*ones(l,1)
    val z = 2 * k * l + k + 1  // system size
    val a = 1   // number of negative drift states
    val b = z - 1  // number of positive drift states

    // Construction of the matrix Q in Eqn (13)
    // Q11 = kron(eye(k),S) + kron(T,eye(l)) + (1-p)*kron(kron(kappa,tau),eye(l))
    val Q11 = Matrix.eye(k).kron(S)
        .add(1.0, T.kron(Matrix.eye(l)))
        .add(1.0 - p, kappa.kron(tau).kron(Matrix.eye(l)))

    // Q33 = Q11 + p*kron(kappa, kron(ones(l,1), kron(tau, sigma)))
    val Q33 = Q11.copy().add(p, kappa.kron(Matrix.ones(l, 1).kron(tau.kron(sigma))))

    // Construct full Q matrix (z x z)
    val Q = Matrix.zeros(z, z)

    // Row block 1: [Q11, kron(eye(k),nu), zeros(kl,kl), p*kron(kappa,ones(l,1))]
    val block12 = Matrix.eye(k).kron(nu)  // k*l x k
    val block14 = kappa.kron(Matrix.ones(l, 1))  // k*l x 1
    for (i in 0 until k * l) {
        for (j in 0 until k * l) Q.set(i, j, Q11.get(i, j))
        for (j in 0 until k) Q.set(i, k * l + j, block12.get(i, j))
        // zeros for block 1,3
        Q.set(i, 2 * k * l + k, p * block14.get(i, 0))
    }

    // Row block 2: [zeros(k,kl), T, kron(kappa,kron(tau,sigma)), zeros(k,1)]
    val block23 = kappa.kron(tau.kron(sigma))  // k x k*l
    for (i in 0 until k) {
        for (j in 0 until k) Q.set(k * l + i, k * l + j, T.get(i, j))
        for (j in 0 until k * l) Q.set(k * l + i, k * l + k + j, block23.get(i, j))
    }

    // Row block 3: [zeros(kl,kl), zeros(kl,k), Q33, kron(ones(k,1),nu)]
    val block34 = Matrix.ones(k, 1).kron(nu)  // k*l x 1
    for (i in 0 until k * l) {
        for (j in 0 until k * l) Q.set(k * l + k + i, k * l + k + j, Q33.get(i, j))
        Q.set(k * l + k + i, 2 * k * l + k, block34.get(i, 0))
    }
    // Row block 4: zeros(1, z) - already zero

    // Construction of the drift matrix R in Eqn (13)
    val R = Matrix.eye(z)
    R.set(z - 1, z - 1, -1.0)

    // Qtilde Construction in Eqn (13)
    val Qtilde = Q.copy()
    val tauSigma = tau.kron(sigma)  // 1 x k*l
    for (j in 0 until k * l) {
        Qtilde.set(z - 1, j, tauSigma.get(0, j))
    }
    Qtilde.set(z - 1, z - 1, -1.0)

    // Construction of orthogonal matrix P (Householder reflection)
    // u1 = [ones(z-1,1); -1], u2 = [1; zeros(z-1,1)]
    // u = u1 - norm(u1,2) * u2
    // P = I - 2*u*u'/(u'*u)
    val u1 = Matrix(z, 1)
    for (i in 0 until z - 1) u1.set(i, 0, 1.0)
    u1.set(z - 1, 0, -1.0)
    val norm_u1 = sqrt(u1.transpose().mult(u1).get(0, 0))

    val u = u1.copy()
    u.set(0, 0, u.get(0, 0) - norm_u1)  // u = u1 - norm(u1)*u2

    val utu = u.transpose().mult(u).get(0, 0)  // u'*u
    val uut = u.mult(u.transpose())  // u*u'
    val P = Matrix.eye(z).add(-2.0 / utu, uut)  // I - 2*u*u'/(u'*u)

    // Construction of A, H, Qtildestar (Line 3 of Alg. 1)
    // QR = Q / R  (= Q * R^{-1})
    val QR = Q.mult(Matrix.inv(R))
    // Ae = P' * QR * P
    val Ae = P.transpose().mult(QR).mult(P)

    // A = Ae(a+1:z, a+1:z) — submatrix of size b x b
    val Amat = Matrix(b, b)
    for (i in 0 until b) {
        for (j in 0 until b) {
            Amat.set(i, j, Ae.get(a + i, a + j))
        }
    }

    // H = P(1:z, a+1:z)' — first extract columns a+1:z of P, then transpose
    val Hmat = Matrix(b, z)
    for (i in 0 until b) {
        for (j in 0 until z) {
            Hmat.set(i, j, P.get(j, a + i))
        }
    }

    // Qtildestar = Qtilde(b+1:z, 1:z) — last 'a' rows of Qtilde
    val Qtildestar = Matrix(a, z)
    for (i in 0 until a) {
        for (j in 0 until z) {
            Qtildestar.set(i, j, Qtilde.get(b + i, j))
        }
    }

    // Solve for g and d (Line 4 of Alg. 1)
    // EqnMatrix = [H*R, -inv(A)*H*ones(z,1); -Qtildestar, ones(a,1)]
    val HR = Hmat.mult(R)  // b x z
    val Ainv = Matrix.inv(Amat)
    val AinvHones = Ainv.mult(Hmat.mult(Matrix.ones(z, 1))).scale(-1.0)  // b x 1

    val eqnRows = b + a
    val eqnCols = z
    val EqnMatrix = Matrix(eqnRows, eqnCols)
    for (i in 0 until b) {
        for (j in 0 until z) EqnMatrix.set(i, j, HR.get(i, j))
    }
    for (i in 0 until a) {
        for (j in 0 until z) EqnMatrix.set(b + i, j, -Qtildestar.get(i, j))
    }

    // Augment with the last column: [-inv(A)*H*ones(z,1); ones(a,1)]
    // Actually the equation system is: EqnMatrix' * [g d]' = [zeros(1,z) 1]'
    // which means EqnMatrix is (b+a) x z and we have b+a unknowns
    // The RHS is [0...0, 1] with z entries... wait.

    // Looking at MATLAB more carefully:
    // EqnMatrix = [H*R, -inv(A)*H*ones(z,1); -Qtildestar, ones(a,1)]
    // This is (b+a) x (z+1) = z x (z+1)
    // RightHandSide = [zeros(1,z) 1] — size 1 x (z+1)
    // Solution = mldivide(EqnMatrix', RightHandSide')' — solve EqnMatrix' * x = RHS'
    // This solves for Solution of size 1 x z
    val eqnMatrix2 = Matrix(z, z + 1)
    for (i in 0 until b) {
        for (j in 0 until z) eqnMatrix2.set(i, j, HR.get(i, j))
        eqnMatrix2.set(i, z, AinvHones.get(i, 0))
    }
    for (i in 0 until a) {
        for (j in 0 until z) eqnMatrix2.set(b + i, j, -Qtildestar.get(i, j))
        eqnMatrix2.set(b + i, z, 1.0)
    }

    // RHS = [0 0 ... 0 1]' of size (z+1) x 1
    val rhs = Matrix(z + 1, 1)
    rhs.set(z, 0, 1.0)

    // Solve: EqnMatrix' * solution = RHS  =>  solution = (EqnMatrix')^{-1} * RHS
    val eqnMatrixT = eqnMatrix2.transpose()  // (z+1) x z
    val solution = Matrix(z, 1)
    Matrix.solveSafe(eqnMatrixT, rhs, solution)

    // g = Solution(1:b), d = Solution(b+1:z)
    val g = Matrix(1, b)
    for (i in 0 until b) g.set(0, i, solution.get(i, 0))

    // AoI: restrict to states in Phases 2 and 3 only
    // SelectedStates = [zeros(kl,1); ones(kl+k,1); 0]
    val selectedAoI = Matrix(z, 1)
    for (i in k * l until 2 * k * l + k) selectedAoI.set(i, 0, 1.0)

    val aoiH = Hmat.mult(selectedAoI)  // b x 1
    // NormConstant = -g * A^{-1} * aoiH
    val normConst = g.mult(Ainv).mult(aoiH).scale(-1.0).get(0, 0)
    val aoiG = g.scale(1.0 / normConst)  // 1 x b
    val aoiA = Amat  // b x b

    // AoI moments: mean = g * A^{-2} * h, var = -2*g*A^{-3}*h - mean^2
    val Ainv2 = Ainv.mult(Ainv)
    val Ainv3 = Ainv2.mult(Ainv)
    val aoiMean = aoiG.mult(Ainv2).mult(aoiH).get(0, 0)
    val aoiVar = -2.0 * aoiG.mult(Ainv3).mult(aoiH).get(0, 0) - aoiMean * aoiMean

    // PAoI: restrict to states in Phase 3 scaled by nu
    // SelectedStates = [zeros(kl+k,1); kron(ones(k,1),nu); 0]
    val selectedPAoI = Matrix(z, 1)
    val kronOnesNu = Matrix.ones(k, 1).kron(nu)  // k*l x 1
    for (i in 0 until k * l) selectedPAoI.set(k * l + k + i, 0, kronOnesNu.get(i, 0))

    val paoiH = Hmat.mult(selectedPAoI)
    val normConstP = g.mult(Ainv).mult(paoiH).scale(-1.0).get(0, 0)
    val paoiG = g.scale(1.0 / normConstP)
    val paoiA = Amat

    val paoiMean = paoiG.mult(Ainv2).mult(paoiH).get(0, 0)
    val paoiVar = -2.0 * paoiG.mult(Ainv3).mult(paoiH).get(0, 0) - paoiMean * paoiMean

    return AoiMfqResult(
        aoiG = aoiG, aoiA = aoiA, aoiH = aoiH,
        aoiMean = aoiMean, aoiVar = Math.max(0.0, aoiVar),
        paoiG = paoiG, paoiA = paoiA, paoiH = paoiH,
        paoiMean = paoiMean, paoiVar = Math.max(0.0, paoiVar),
        systemType = "bufferless", preemption = p
    )
}
