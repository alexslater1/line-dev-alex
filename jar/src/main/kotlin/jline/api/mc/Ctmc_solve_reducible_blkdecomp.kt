package jline.api.mc

import jline.util.graph.DirectedGraph
import jline.util.matrix.Matrix

/**
 * Solve reducible CTMCs via direct block decomposition on the generator matrix.
 *
 * Algorithm (based on SMART's computeInfinityDistribution):
 *   1. Decompose states into transient and recurrent classes via SCC detection
 *   2. For transient states: solve n * Q_tt = -p0_t for expected sojourn
 *   3. Compute hitting probabilities: h = n * Q_ta + p0_r
 *   4. For each recurrent class: solve pi_c * Q_cc = 0, scale by hitting prob
 *
 * @param Q Infinitesimal generator matrix (possibly reducible)
 * @param pi0 Initial state distribution (optional)
 * @param options Solution options (optional)
 * @return Pair of (pi: steady-state distribution, scc: SCC index sets)
 */
fun ctmc_solve_reducible_blkdecomp(
    Q: Matrix,
    pi0: Matrix? = null,
    options: Map<String, Any> = mapOf("tol" to 1e-12)
): Pair<Matrix, List<List<Int>>> {
    val Qmat = ctmc_makeinfgen(Q.copy())
    val N = Qmat.numRows

    // Build adjacency from off-diagonal positive entries
    val Adj = Matrix(N, N)
    for (i in 0 until N) {
        for (j in 0 until N) {
            if (i != j && Qmat[i, j] > 0) Adj[i, j] = 1.0
        }
    }

    // Find SCCs and classify as recurrent/transient
    val graph = DirectedGraph(Adj)
    val sccResult = graph.stronglyconncomp()
    val scc = IntArray(N) { sccResult.I[it] - 1 } // convert to 0-based
    val isrec = sccResult.recurrent
    val numSCC = isrec.size

    // Irreducible case
    if (numSCC == 1) {
        val pi = ctmc_solve(Qmat)
        return Pair(pi, listOf((0 until N).toList()))
    }

    // Build SCC index sets
    val sccIdx = Array(numSCC) { i -> (0 until N).filter { scc[it] == i }.sorted() }

    // Classify SCCs
    val transSccIds = (0 until numSCC).filter { !isrec[it] }
    val recSccIds = (0 until numSCC).filter { isrec[it] }

    // Gather ordered state indices
    val transStates = transSccIds.flatMap { sccIdx[it] }.sorted()
    val recStates = recSccIds.flatMap { sccIdx[it] }.sorted()
    val nt = transStates.size
    val nr = recStates.size

    // Extract Q sub-blocks (only when transient states exist)
    var Q_tt: Matrix? = null
    var Q_ta: Matrix? = null
    if (nt > 0 && nr > 0) {
        val transIdx = Matrix(nt, 1)
        transStates.forEachIndexed { i, v -> transIdx[i, 0] = v.toDouble() }
        val recIdx = Matrix(nr, 1)
        recStates.forEachIndexed { i, v -> recIdx[i, 0] = v.toDouble() }
        Q_tt = Qmat.getSubMatrix(transIdx, transIdx)
        Q_ta = Qmat.getSubMatrix(transIdx, recIdx)
    }

    // Compute per-SCC limiting distributions
    val pis = Array(numSCC) { Matrix.zeros(1, N) }

    for (s in 0 until numSCC) {
        val classStates = sccIdx[s]
        val classSize = classStates.size

        // Compute absorption probabilities into recurrent states
        val hit = DoubleArray(nr)

        if (nt > 0 && Q_tt != null && Q_ta != null) {
            // Build p0_t: initial probability in transient states for this starting SCC
            val p0_t = DoubleArray(nt)
            for ((idx, state) in transStates.withIndex()) {
                if (classStates.contains(state)) {
                    p0_t[idx] = 1.0 / classSize
                }
            }

            if (p0_t.any { it > 0.0 }) {
                // Solve n * Q_tt = -p0_t  =>  Q_tt' * n' = -p0_t'
                // Q_tt is non-singular (Hurwitz) for transient states
                val negP0Col = Matrix(nt, 1)
                for (i in 0 until nt) negP0Col[i, 0] = -p0_t[i]

                val sojournCol = Matrix(nt, 1)
                Matrix.solveDirect(Q_tt.transpose(), negP0Col, sojournCol)

                // hit = sojourn * Q_ta (row vector times matrix)
                val sojournRow = sojournCol.transpose()
                val hitMatrix = sojournRow.mult(Q_ta)
                for (i in 0 until nr) hit[i] = hitMatrix[0, i]
            }
        }

        // Add initial mass already in recurrent states
        for ((idx, state) in recStates.withIndex()) {
            if (classStates.contains(state)) {
                hit[idx] += 1.0 / classSize
            }
        }

        // Solve steady-state per recurrent class, scaled by hitting probability
        for (c in recSccIds) {
            val idxC = sccIdx[c]

            // Sum hitting probabilities for states in this recurrent class
            var reachprob = 0.0
            for (state in idxC) {
                val loc = recStates.indexOf(state)
                if (loc >= 0) reachprob += hit[loc]
            }

            if (reachprob < 1e-15) continue

            if (idxC.size == 1) {
                // Absorbing state: hitting probability IS the final probability
                pis[s][0, idxC[0]] = reachprob
            } else {
                // Solve pi_c * Q_cc = 0 within this recurrent class
                val indices = Matrix(idxC.size, 1)
                idxC.forEachIndexed { i, v -> indices[i, 0] = v.toDouble() }
                val Qcc = Qmat.getSubMatrix(indices, indices)
                val piC = ctmc_solve(Qcc)
                for ((k, stateIdx) in idxC.withIndex()) {
                    pis[s][0, stateIdx] = piC[0, k] * reachprob
                }
            }
        }
    }

    // Compute initial SCC probabilities for weighted average
    val pinl = DoubleArray(numSCC)
    if (pi0 == null) {
        for (i in 0 until numSCC) pinl[i] = 1.0
        // Zero out SCCs containing states with zero column sums (no incoming)
        for (j in 0 until N) {
            var colSum = 0.0
            for (i in 0 until N) colSum += Math.abs(Qmat[i, j])
            if (colSum < 1e-12) pinl[scc[j]] = 0.0
        }
        val totalPinl = pinl.sum()
        if (totalPinl > 0) {
            for (i in 0 until numSCC) pinl[i] /= totalPinl
        } else {
            for (i in 0 until numSCC) pinl[i] = 1.0 / numSCC
        }
    } else {
        for (i in 0 until numSCC) {
            pinl[i] = sccIdx[i].sumOf { pi0[0, it] }
        }
    }

    // Weighted average over starting SCCs
    val pi = Matrix.zeros(1, N)
    for (i in 0 until numSCC) {
        if (pinl[i] > 0) {
            for (k in 0 until N) {
                pi[0, k] += pis[i][0, k] * pinl[i]
            }
        }
    }

    // Special case: single transient SCC without explicit initial distribution
    if (transSccIds.size == 1 && pi0 == null) {
        for (k in 0 until N) {
            pi[0, k] = pis[transSccIds[0]][0, k]
        }
    }

    // Normalize
    var total = 0.0
    for (k in 0 until N) total += pi[0, k]
    if (total > 0) {
        for (k in 0 until N) pi[0, k] /= total
    }

    return Pair(pi, sccIdx.map { it.toList() })
}

/**
 * Alternative signature that returns additional information
 */
data class CtmcSolveReducibleBlkdecompResult(
    val pi: Matrix,
    val pis: List<Matrix>,
    val pi0: Matrix,
    val scc: List<List<Int>>,
    val isrec: List<Boolean>
)

fun ctmc_solve_reducible_blkdecomp_full(
    Q: Matrix,
    pi0: Matrix? = null,
    options: Map<String, Any> = mapOf("tol" to 1e-12)
): CtmcSolveReducibleBlkdecompResult {
    val Qmat = ctmc_makeinfgen(Q.copy())
    val N = Qmat.numRows

    // Build adjacency from off-diagonal positive entries
    val Adj = Matrix(N, N)
    for (i in 0 until N) {
        for (j in 0 until N) {
            if (i != j && Qmat[i, j] > 0) Adj[i, j] = 1.0
        }
    }

    // Find SCCs
    val graph = DirectedGraph(Adj)
    val sccResult = graph.stronglyconncomp()
    val scc = IntArray(N) { sccResult.I[it] - 1 }
    val isrec = sccResult.recurrent
    val numSCC = isrec.size

    // Use the blkdecomp function for the computation
    val (pi, sccLists) = ctmc_solve_reducible_blkdecomp(Q, pi0, options)

    // Build pis by running the algorithm for each starting SCC
    val sccIdx = Array(numSCC) { i -> (0 until N).filter { scc[it] == i }.sorted() }
    val pisList = mutableListOf<Matrix>()
    for (s in 0 until numSCC) {
        val p0s = Matrix.zeros(1, N)
        val classStates = sccIdx[s]
        for (state in classStates) {
            p0s[0, state] = 1.0 / classStates.size
        }
        val (piS, _) = ctmc_solve_reducible_blkdecomp(Q, p0s, options)
        pisList.add(piS)
    }

    return CtmcSolveReducibleBlkdecompResult(
        pi = pi,
        pis = pisList,
        pi0 = pi0 ?: Matrix.zeros(1, N),
        scc = sccLists,
        isrec = isrec.toList()
    )
}
/**
 * CTMC solve reducible algorithms
 */
@Suppress("unused")
class CtmcSolveReducibleBlkdecompAlgo {
    companion object {
        // Class documentation marker for Dokka
    }
}
