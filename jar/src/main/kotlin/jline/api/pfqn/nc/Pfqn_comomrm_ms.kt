/**
 * @file CoMoM normalizing constant method for multiserver repairman models
 *
 * Implements the Convolution Method of Moments (CoMoM) for computing normalizing constants
 * in repairman queueing models with multiserver stations. Handles load-dependent service
 * rates arising from multiple servers via the pfqn_mu_ms function.
 *
 * @since LINE 3.0
 */
package jline.api.pfqn.nc

import jline.io.Ret
import jline.GlobalConstants
import jline.api.pfqn.ld.pfqn_mu_ms
import jline.util.Maths
import jline.util.matrix.Matrix
import org.apache.commons.math3.util.FastMath

/**
 * Compute the normalizing constant of a multiserver repairman model using CoMoM
 *
 * @param L    - demands at all stations (1 x R, single station only)
 * @param N    - number of jobs for each class (1 x R)
 * @param Z    - think time for each class (1 x R)
 * @param m    - replication factor (default: 1)
 * @param S    - number of servers at the queueing station
 * @return normalizing constant G, log normalizing constant lG, and state probabilities
 */

@JvmOverloads
fun pfqn_comomrm_ms(L: Matrix, N: Matrix, Z: Matrix, m: Int = 1, S: Int): Ret.pfqnComomrmMs {
    val M = L.numRows
    val R = L.numCols
    if (M != 1) {
        throw RuntimeException("pfqn_comomrm_ms: The solver accepts at most a single queueing station.")
    }

    val atol = GlobalConstants.FineTol
    val lambda = Matrix(1, R)
    lambda.fill(0.0)
    val ret = pfqn_nc_sanitize(lambda, L, N, Z, atol)
    val L_new = ret.L
    val N_new = ret.N
    val Z_new = ret.Z
    val lG0 = ret.lGremaind

    val Nt = N_new.elementSum().toInt()

    // Compute load-dependent service rates
    val mu: Matrix
    if (m > 1) {
        mu = pfqn_mu_ms(Nt, m, S)
    } else {
        mu = Matrix(1, Nt)
        for (i in 0..<Nt) {
            mu[i] = FastMath.min(S.toDouble(), (i + 1).toDouble())
        }
    }

    var h = Matrix(Nt + 1, 1)
    h[Nt] = 1.0
    val scale = Matrix(Nt, 1)
    var nt = 0

    for (r in 0..<R) {
        // Build transition matrix Tr = Z(r)*I + diag(L(r)*(Nt:-1:1)./mu(Nt:-1:1), 1)
        val Tr = Matrix.eye(Nt + 1).scale(Z_new[r])
        for (i in 0..<Nt) {
            Tr[i, i + 1] = L_new[r] * (Nt - i).toDouble() / mu[(Nt - i - 1)]
        }
        for (nr in 1..N_new[r].toInt()) {
            val hT = Tr.copy().scale(1.0 / nr.toDouble())
            h = hT.mult(h)
            scale[nt] = FastMath.abs(h.sort().elementSum())
            h.absEq()
            h.scaleEq(1.0 / scale[nt])
            nt++
        }
    }

    val lG = lG0 + scale.log().elementSum()
    val G = FastMath.exp(lG)
    val prob = h.reverse().scale(1.0 / G)
    prob.divideEq(prob.elementSum())

    return Ret.pfqnComomrmMs(G, lG, prob)
}

/**
 * PFQN comomrm_ms algorithms
 */
@Suppress("unused")
class PfqnComomrmMsAlgo {
    companion object {
        // Class documentation marker for Dokka
    }
}
