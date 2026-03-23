/**
 * Fork-Join Visit Ratio Calculator via Auxiliary SPN Models
 *
 * Computes fork-join node visit ratios by building, for each class that passes
 * through a fork-join pair, an auxiliary closed Stochastic Petri Net capturing
 * the fork/join synchronization semantics. The SPN is solved with SolverCTMC
 * and the throughput ratios give the per-node visit ratios.
 *
 * @since LINE 3.0
 */
package jline.api.sn

import jline.GlobalConstants
import jline.lang.ClosedClass
import jline.lang.Network
import jline.lang.NetworkStruct
import jline.lang.constant.NodeType
import jline.lang.constant.TimingStrategy
import jline.lang.nodes.Place
import jline.lang.nodes.Transition
import jline.lang.processes.Exp
import jline.solvers.NetworkAvgTable
import jline.solvers.ctmc.SolverCTMC
import jline.util.matrix.Matrix
import jline.io.line_warning
import java.util.ArrayList
import java.util.HashMap

/**
 * Compute fork-join node visit ratios via auxiliary SPN models.
 *
 * For each class that passes through a fork-join pair, builds an auxiliary closed
 * Stochastic Petri Net with population = max leaf count (B), solves it with
 * SolverCTMC, and extracts throughput ratios as visit ratios.
 *
 * @param sn NetworkStruct describing the queueing network
 * @return List of Matrix (one per chain), each (nnodes x nclasses) with visit ratios
 *         normalized so that reference station = 1
 */
fun snFjVisitsSpn(sn: NetworkStruct): List<Matrix> {
    val I = sn.nnodes
    val K = sn.nclasses
    val nchains = sn.nchains
    val inchain = sn.inchain
    val refstat = sn.refstat

    // Initialize nodevisits: one (nnodes x nclasses) matrix per chain
    val nodevisits: MutableList<Matrix> = ArrayList()
    for (c in 0 until nchains) {
        val m = Matrix(I, K)
        m.zero()
        nodevisits.add(m)
    }

    // Quick check: if no fork-join pairs, return zeros
    var hasFJ = false
    for (i in 0 until sn.fj.numRows) {
        for (j in 0 until sn.fj.numCols) {
            if (sn.fj[i, j] > 0) {
                hasFJ = true
                break
            }
        }
        if (hasFJ) break
    }
    if (!hasFJ) {
        return nodevisits
    }

    // For each chain, build and solve an SPN for each class in the chain
    for (c in 0 until nchains) {
        val inchain_c = inchain[c] ?: continue
        for (kidx in 0 until inchain_c.numCols) {
            val r = inchain_c[0, kidx].toInt()

            // Extract the single-class sub-routing from rtnodes
            val P_r = Array(I) { DoubleArray(I) }
            for (i in 0 until I) {
                for (j in 0 until I) {
                    P_r[i][j] = sn.rtnodes[i * K + r, j * K + r]
                }
            }

            // Find nodes visited by this class (reachable from reference station)
            val refnode = sn.stationToNode[refstat[r].toInt()].toInt()
            val visited = BooleanArray(I)
            visited[refnode] = true
            var changed = true
            while (changed) {
                changed = false
                for (i in 0 until I) {
                    if (visited[i]) {
                        for (j in 0 until I) {
                            if (P_r[i][j] > 0 && !visited[j]) {
                                visited[j] = true
                                changed = true
                            }
                        }
                    }
                }
            }

            // Skip if this class doesn't pass through any fork
            var hasFork = false
            for (i in 0 until I) {
                if (visited[i] && sn.nodetype[i] == NodeType.Fork) {
                    hasFork = true
                    break
                }
            }
            if (!hasFork) {
                // Set visits to 1 for all visited nodes
                for (i in 0 until I) {
                    if (visited[i]) {
                        nodevisits[c][i, r] = 1.0
                    }
                }
                continue
            }

            // Build and solve auxiliary SPN
            val visits_r = buildAndSolveSpn(sn, P_r, visited, r, refnode)
            for (i in 0 until I) {
                nodevisits[c][i, r] = visits_r[i]
            }
        }

        // Normalize by reference station
        val refclass0 = inchain_c[0, 0].toInt()
        val refnode_c = sn.stationToNode[refstat[refclass0].toInt()].toInt()
        for (kidx in 0 until inchain_c.numCols) {
            val r = inchain_c[0, kidx].toInt()
            val normVal = nodevisits[c][refnode_c, r]
            if (normVal > GlobalConstants.FineTol) {
                for (i in 0 until I) {
                    nodevisits[c][i, r] = nodevisits[c][i, r] / normVal
                }
            }
        }
    }

    return nodevisits
}

/**
 * Recursively resolve Fork destinations to station nodes.
 */
private fun resolveForkDests(sn: NetworkStruct, P_r: Array<DoubleArray>, visited: BooleanArray, forkNd: Int): List<Int> {
    val stDests: MutableList<Int> = ArrayList()
    for (bd in 0 until sn.nnodes) {
        if (P_r[forkNd][bd] > 0 && visited[bd]) {
            if (sn.nodetype[bd] == NodeType.Fork) {
                stDests.addAll(resolveForkDests(sn, P_r, visited, bd))
            } else if (sn.isstation[bd, 0] > 0) {
                stDests.add(bd)
            }
        }
    }
    return stDests
}

/**
 * Build an auxiliary SPN model for one class and solve it with CTMC.
 *
 * Uses a population-preserving approach:
 *   - Population = total leaf branches (B) across all fork-join pairs
 *   - Pre-fork station: requires B tokens, produces 1 per leaf branch
 *   - Branch stations: normal 1-in-1-out service
 *   - Join: requires 1 from each branch done, produces B in post-join dest
 *   - Throughput ratios give visit ratios
 */
private fun buildAndSolveSpn(
    sn: NetworkStruct,
    P_r: Array<DoubleArray>,
    visited: BooleanArray,
    r: Int,
    refnode: Int
): DoubleArray {
    val I = sn.nnodes
    val visits_r = DoubleArray(I)

    // Collect visited nodes
    val visitedNodes: MutableList<Int> = ArrayList()
    for (i in 0 until I) {
        if (visited[i]) visitedNodes.add(i)
    }
    if (visitedNodes.isEmpty()) return visits_r

    // Classify visited nodes
    val stationNodes: MutableList<Int> = ArrayList()
    val forkNodes: MutableList<Int> = ArrayList()
    val joinNodes: MutableList<Int> = ArrayList()
    for (nd in visitedNodes) {
        if (sn.nodetype[nd] == NodeType.Fork) {
            forkNodes.add(nd)
        } else if (sn.nodetype[nd] == NodeType.Join) {
            joinNodes.add(nd)
        } else if (sn.isstation[nd, 0] > 0 && sn.nodetype[nd] != NodeType.Source && sn.nodetype[nd] != NodeType.Sink) {
            stationNodes.add(nd)
        }
    }

    // Compute leaf count for each Join node (bottom-up, handles arbitrary nesting)
    val joinLeaves: MutableMap<Int, Int> = HashMap()
    for (pass in 0 until joinNodes.size) {
        for (jnd in joinNodes) {
            var lc = 0
            for (srcNd in 0 until I) {
                if (P_r[srcNd][jnd] > 0 && visited[srcNd]) {
                    if (sn.nodetype[srcNd] == NodeType.Join && joinLeaves.containsKey(srcNd)) {
                        lc += joinLeaves[srcNd]!!
                    } else {
                        lc += 1
                    }
                }
            }
            joinLeaves[jnd] = lc
        }
    }

    // Compute total leaf branches B = max leaf count across outermost forks
    var B = 0
    for (fnd in forkNodes) {
        // Check if this Fork is outermost (upstream is a station, not a Join)
        var isOutermost = true
        for (srcNd in 0 until I) {
            if (P_r[srcNd][fnd] > 0 && visited[srcNd]) {
                if (sn.nodetype[srcNd] == NodeType.Join) {
                    isOutermost = false
                    break
                }
            }
        }
        if (isOutermost) {
            val leafs = resolveForkDests(sn, P_r, visited, fnd)
            if (leafs.size > B) {
                B = leafs.size
            }
        }
    }
    if (B == 0) B = 1

    // Build SPN model
    val model = Network("fj_spn_aux")

    // Create Places for station nodes
    val places: Array<Place?> = arrayOfNulls(I)
    for (nd in stationNodes) {
        places[nd] = Place(model, "P_" + sn.nodenames[nd])
    }

    // Create "done" Places for stations that route to a Join
    val preJoin: Array<Place?> = arrayOfNulls(I)
    for (nd in stationNodes) {
        for (dstNd in 0 until I) {
            if (P_r[nd][dstNd] > 0 && visited[dstNd] && sn.nodetype[dstNd] == NodeType.Join) {
                preJoin[nd] = Place(model, "P_" + sn.nodenames[nd] + "_done")
                break
            }
        }
    }

    // For inner Joins that route to outer Joins (nested fork-join)
    for (jnd in joinNodes) {
        for (dstNd in 0 until I) {
            if (P_r[jnd][dstNd] > 0 && visited[dstNd] && sn.nodetype[dstNd] == NodeType.Join) {
                preJoin[jnd] = Place(model, "P_" + sn.nodenames[jnd] + "_done")
                break
            }
        }
    }

    // Intermediate Places for Join -> Fork connections (serial fork-join)
    val interJF: Array<Place?> = arrayOfNulls(I)
    for (jnd in joinNodes) {
        for (dstNd in 0 until I) {
            if (P_r[jnd][dstNd] > 0 && visited[dstNd] && sn.nodetype[dstNd] == NodeType.Fork) {
                interJF[jnd] = Place(model, "P_" + sn.nodenames[jnd] + "_to_" + sn.nodenames[dstNd])
                break
            }
        }
    }

    // ClosedClass with B tokens at reference Place
    val jobclass = ClosedClass(model, "Token", B, places[refnode])

    val transitions: MutableList<Transition> = ArrayList()
    val transInfoInPlaces: MutableList<List<Place>> = ArrayList()
    val transInfoOutPlaces: MutableList<List<Place>> = ArrayList()

    // Create timed service Transitions for each station
    for (nd in stationNodes) {
        val ist = sn.nodeToStation[nd].toInt()

        // Service rate
        var isTimed = false
        var rate = 0.0
        if (ist >= 0 && !sn.rates.isEmpty && ist < sn.rates.numRows && r < sn.rates.numCols) {
            val rateVal = sn.rates[ist, r]
            if (rateVal > 0 && !java.lang.Double.isInfinite(rateVal)) {
                rate = rateVal
                isTimed = true
            }
        }

        // Determine effective output Places by resolving through Fork/Join routing
        val outPlaces: MutableList<Place> = ArrayList()
        var enableCount = 1
        for (dstNd in 0 until I) {
            if (P_r[nd][dstNd] <= 0 || !visited[dstNd]) continue

            if (sn.nodetype[dstNd] == NodeType.Fork) {
                // Resolve Fork to leaf stations; require B tokens (population-preserving)
                val forkDests = resolveForkDests(sn, P_r, visited, dstNd)
                enableCount = B
                for (fd in forkDests) {
                    if (places[fd] != null) {
                        outPlaces.add(places[fd]!!)
                    }
                }
            } else if (sn.nodetype[dstNd] == NodeType.Join) {
                if (preJoin[nd] != null) {
                    outPlaces.add(preJoin[nd]!!)
                }
            } else if (sn.isstation[dstNd, 0] > 0) {
                if (places[dstNd] != null) {
                    outPlaces.add(places[dstNd]!!)
                }
            }
        }

        if (outPlaces.isEmpty()) continue

        val tName = "T_svc_" + sn.nodenames[nd]
        val T = Transition(model, tName)
        val mode = T.addMode("serve")

        if (isTimed) {
            T.setDistribution(mode, Exp(rate))
        } else {
            T.setTimingStrategy(mode, TimingStrategy.IMMEDIATE)
            T.setFiringWeights(mode, 1.0)
        }

        T.setEnablingConditions(mode, jobclass, places[nd]!!, enableCount)
        T.setFiringOutcome(mode, jobclass, places[nd]!!, -enableCount)
        for (op in outPlaces) {
            T.setFiringOutcome(mode, jobclass, op, 1)
        }

        transitions.add(T)
        val inList: MutableList<Place> = ArrayList()
        inList.add(places[nd]!!)
        transInfoInPlaces.add(inList)
        transInfoOutPlaces.add(outPlaces)
    }

    // Create Immediate Join Transitions
    for (jnd in joinNodes) {
        // Collect input "done" Places and their source nodes
        val inPlaces: MutableList<Place> = ArrayList()
        val inSrcNodes: MutableList<Int> = ArrayList()
        for (srcNd in 0 until I) {
            if (P_r[srcNd][jnd] > 0 && visited[srcNd]) {
                if (preJoin[srcNd] != null) {
                    inPlaces.add(preJoin[srcNd]!!)
                    inSrcNodes.add(srcNd)
                }
            }
        }
        if (inPlaces.isEmpty()) continue

        // Compute enabling counts per input: 1 for stations, joinLeaves for inner joins
        val enCounts = IntArray(inPlaces.size) { 1 }
        for (ii in inSrcNodes.indices) {
            val srcNd = inSrcNodes[ii]
            if (sn.nodetype[srcNd] == NodeType.Join && joinLeaves.containsKey(srcNd)) {
                enCounts[ii] = joinLeaves[srcNd]!!
            }
        }

        // produce_count = joinLeaves(jnd) = total leaf tokens through this Join
        val produceCount: Int = if (joinLeaves.containsKey(jnd)) {
            joinLeaves[jnd]!!
        } else {
            var sum = 0
            for (ec in enCounts) sum += ec
            sum
        }

        // Determine post-Join output Places
        val outPlaces: MutableList<Place> = ArrayList()
        for (dstNd in 0 until I) {
            if (P_r[jnd][dstNd] <= 0 || !visited[dstNd]) continue

            if (sn.nodetype[dstNd] == NodeType.Fork) {
                // Serial fork-join
                if (interJF[jnd] != null) {
                    outPlaces.add(interJF[jnd]!!)
                }
            } else if (sn.nodetype[dstNd] == NodeType.Join) {
                // Nested: inner Join -> outer Join
                if (preJoin[jnd] != null) {
                    outPlaces.add(preJoin[jnd]!!)
                }
            } else if (sn.isstation[dstNd, 0] > 0) {
                if (places[dstNd] != null) {
                    outPlaces.add(places[dstNd]!!)
                }
            }
        }
        if (outPlaces.isEmpty()) continue

        val tName = "T_join_" + sn.nodenames[jnd]
        val T = Transition(model, tName)
        val mode = T.addMode("sync")
        T.setTimingStrategy(mode, TimingStrategy.IMMEDIATE)
        T.setFiringWeights(mode, 1.0)

        for (ii in inPlaces.indices) {
            T.setEnablingConditions(mode, jobclass, inPlaces[ii], enCounts[ii])
            T.setFiringOutcome(mode, jobclass, inPlaces[ii], -enCounts[ii])
        }
        for (op in outPlaces) {
            T.setFiringOutcome(mode, jobclass, op, produceCount)
        }

        transitions.add(T)
        transInfoInPlaces.add(inPlaces)
        transInfoOutPlaces.add(outPlaces)
    }

    // Create Immediate Fork Transitions for Join -> Fork connections (serial FJ)
    for (jnd in joinNodes) {
        if (interJF[jnd] == null) continue

        for (dstNd in 0 until I) {
            if (P_r[jnd][dstNd] <= 0 || !visited[dstNd]) continue
            if (sn.nodetype[dstNd] != NodeType.Fork) continue

            val forkDests = resolveForkDests(sn, P_r, visited, dstNd)
            if (forkDests.isEmpty()) continue

            val tName = "T_fork_" + sn.nodenames[jnd] + "_" + sn.nodenames[dstNd]
            val T = Transition(model, tName)
            val mode = T.addMode("fork")
            T.setTimingStrategy(mode, TimingStrategy.IMMEDIATE)
            T.setFiringWeights(mode, 1.0)

            T.setEnablingConditions(mode, jobclass, interJF[jnd]!!, B)
            T.setFiringOutcome(mode, jobclass, interJF[jnd]!!, -B)
            val forkOut: MutableList<Place> = ArrayList()
            for (fd in forkDests) {
                if (places[fd] != null) {
                    T.setFiringOutcome(mode, jobclass, places[fd]!!, 1)
                    forkOut.add(places[fd]!!)
                }
            }

            transitions.add(T)
            val inList: MutableList<Place> = ArrayList()
            inList.add(interJF[jnd]!!)
            transInfoInPlaces.add(inList)
            transInfoOutPlaces.add(forkOut)
        }
    }

    // Set up routing matrix
    val R = model.initRoutingMatrix()
    for (tidx in transitions.indices) {
        val T = transitions[tidx]
        val inPl = transInfoInPlaces[tidx]
        val outPl = transInfoOutPlaces[tidx]
        for (ip in inPl) {
            R.set(jobclass, jobclass, ip, T, 1.0)
        }
        for (op in outPl) {
            R.set(jobclass, jobclass, T, op, 1.0)
        }
    }
    model.link(R)

    // Set initial state: B tokens at reference, 0 everywhere else
    places[refnode]!!.setState(B)
    for (i in 0 until I) {
        if (places[i] != null && i != refnode) {
            places[i]!!.setState(0)
        }
        if (preJoin[i] != null) {
            preJoin[i]!!.setState(0)
        }
        if (interJF[i] != null) {
            interJF[i]!!.setState(0)
        }
    }

    // Solve with CTMC
    try {
        val solver = SolverCTMC(model)
        val avg = solver.getAvgTable() as NetworkAvgTable

        // Extract throughput ratios as visit ratios
        val stationNames = avg.stationNames
        val tputVals = avg.tput

        for (nd in visitedNodes) {
            if (places[nd] != null) {
                val placeName = places[nd]!!.name
                for (row in stationNames.indices) {
                    if (stationNames[row] == placeName) {
                        visits_r[nd] = tputVals[row]
                        break
                    }
                }
            }
        }
    } catch (e: Exception) {
        line_warning("snFjVisitsSpn", "SPN CTMC solve failed for class %d: %s", r, e.message)
    }

    return visits_r
}

/**
 * Stochastic network FjVisitsSpn algorithms
 */
@Suppress("unused")
class SnfjvisitsspnAlgo {
    companion object {
        // Class documentation marker for Dokka
    }
}
