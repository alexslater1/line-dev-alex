/*
 * Copyright (c) 2012-2026, Imperial College London
 * All rights reserved.
 */
package jline.inference.api

import jline.lang.NetworkStruct
import jline.lang.constant.SchedStrategy
import jline.util.matrix.Matrix
import org.apache.commons.math3.ode.FirstOrderDifferentialEquations
import org.apache.commons.math3.ode.events.EventHandler
import org.apache.commons.math3.ode.nonstiff.DormandPrince853Integrator

/**
 * Result of building the augmented fluid model for response time likelihood.
 */
data class FluidPsRtResult(
    val ode: FirstOrderDifferentialEquations,
    val qIndices: Matrix,
    val augPhases: Matrix,
    val stateSize: Int,
    val refIdx: Int,
    val newK: Int
)

/**
 * Fluid-based response time likelihood.
 *
 * Builds an augmented model with tagged class K+1 by expanding the
 * NetworkStruct arrays, then constructs a fluid ODE system for
 * computing the response time likelihood.
 *
 * Uses the routing table sn.rt for general M-station routing and
 * builds phase-aware q_indices from service distributions.
 *
 * @param sn NetworkStruct (from model.getStruct())
 * @param taggedClass class index of the tagged job (0-based)
 * @return FluidPsRtResult with ODE handle and indexing info
 */
fun infer_fluid_ps_rt_likelihood(sn: NetworkStruct, taggedClass: Int): FluidPsRtResult {
    val M = sn.nstations
    val K = sn.nclasses
    val N = sn.nclosedjobs
    val Kc = K + 1

    // Find reference station (PS queue) and get server counts
    var refIdx = -1
    val S = IntArray(M)
    for (i in 0 until M) {
        val station = sn.stations[i]
        if (sn.sched[station] != SchedStrategy.INF) {
            refIdx = i
        }
        S[i] = sn.nservers.get(i, 0).toInt()
        if (S[i] == Int.MAX_VALUE || S[i] < 0) {
            S[i] = N
        }
    }

    // Build augmented service rates (mu) and completion probs (phi)
    // new_mu[i][k] and new_pi[i][k] are DoubleArrays (phase vectors)
    val new_mu = Array<Array<DoubleArray?>>(M) { arrayOfNulls(Kc) }
    val new_pi = Array<Array<DoubleArray?>>(M) { arrayOfNulls(Kc) }
    for (j in 0 until M) {
        val station = sn.stations[j]
        for (k in 0 until K) {
            val jobclass = sn.jobclasses[k]
            val muMatrix = sn.mu[station]?.get(jobclass)
            val piMatrix = sn.phi[station]?.get(jobclass)
            if (muMatrix != null && muMatrix.numRows > 0) {
                val muArr = DoubleArray(muMatrix.numRows) { r -> muMatrix.get(r, 0) }
                val piArr = if (piMatrix != null && piMatrix.numRows > 0) {
                    DoubleArray(piMatrix.numRows) { r -> piMatrix.get(r, 0) }
                } else {
                    DoubleArray(muMatrix.numRows) { 1.0 }
                }
                // Check for NaN
                if (muArr.size == 1 && muArr[0].isNaN()) {
                    new_mu[j][k] = null
                    new_pi[j][k] = null
                } else {
                    new_mu[j][k] = muArr
                    new_pi[j][k] = piArr
                }
            }
        }
        // Tagged class Kc-1: copy from taggedClass
        val tcJobclass = sn.jobclasses[taggedClass]
        val tcMu = sn.mu[station]?.get(tcJobclass)
        val tcPi = sn.phi[station]?.get(tcJobclass)
        if (tcMu != null && tcMu.numRows > 0) {
            val muArr = DoubleArray(tcMu.numRows) { r -> tcMu.get(r, 0) }
            val piArr = if (tcPi != null && tcPi.numRows > 0) {
                DoubleArray(tcPi.numRows) { r -> tcPi.get(r, 0) }
            } else {
                DoubleArray(tcMu.numRows) { 1.0 }
            }
            if (muArr.size == 1 && muArr[0].isNaN()) {
                new_mu[j][Kc - 1] = null
                new_pi[j][Kc - 1] = null
            } else {
                new_mu[j][Kc - 1] = muArr
                new_pi[j][Kc - 1] = piArr
            }
        }
    }

    // Compute augmented phases from new_mu
    val augPhases = Matrix(M, Kc)
    for (i in 0 until M) {
        for (c in 0 until Kc) {
            val mu = new_mu[i][c]
            if (mu != null) {
                augPhases.set(i, c, mu.size.toDouble())
            }
        }
    }

    // Compute phase-aware q_indices: maps (station, class) -> first index in state vector
    val qIndices = Matrix(M, Kc)
    var idx = 0
    for (i in 0 until M) {
        for (c in 0 until Kc) {
            qIndices.set(i, c, idx.toDouble())
            val phases = augPhases.get(i, c).toInt()
            if (phases > 0) {
                idx += phases
            }
        }
    }
    val totalPhases = idx

    // Build expanded routing table from M*K to M*Kc
    val rt = sn.rt
    val newRt = Array(M * Kc) { DoubleArray(M * Kc) }

    // Copy original routing among base classes
    for (l in 0 until K) {
        for (m in 0 until K) {
            for (i in 0 until M) {
                for (j in 0 until M) {
                    newRt[i * Kc + l][j * Kc + m] = rt.get(i * K + l, j * K + m)
                }
            }
        }
    }

    // Tagged class routes like taggedClass at all stations
    for (i in 0 until M) {
        for (j in 0 until M) {
            newRt[i * Kc + (Kc - 1)][j * Kc + (Kc - 1)] = rt.get(i * K + taggedClass, j * K + taggedClass)
        }
    }

    // Absorption at refIdx: tagged class switches back to original classes in chain
    var chainIdx = -1
    if (sn.chains != null) {
        for (ch in 0 until sn.chains.numRows) {
            if (sn.chains.get(ch, taggedClass) == 1.0) {
                chainIdx = ch
                break
            }
        }
    }
    if (chainIdx >= 0) {
        val classesInChain = mutableListOf<Int>()
        for (c in 0 until K) {
            if (sn.chains.get(chainIdx, c) == 1.0) {
                classesInChain.add(c)
            }
        }
        for (l in classesInChain) {
            for (j in 0 until M) {
                newRt[refIdx * Kc + (Kc - 1)][j * Kc + l] = rt.get(refIdx * K + taggedClass, j * K + l)
            }
        }
    }
    // Zero out tagged->tagged transitions at refIdx
    for (j in 0 until M) {
        newRt[refIdx * Kc + (Kc - 1)][j * Kc + (Kc - 1)] = 0.0
    }

    // Build ODE using routing table
    val stateSize = totalPhases
    val ode = object : FirstOrderDifferentialEquations {
        override fun getDimension(): Int = stateSize

        override fun computeDerivatives(t: Double, y: DoubleArray, yDot: DoubleArray) {
            for (i in yDot.indices) yDot[i] = 0.0

            for (i in 0 until M) {
                val station = sn.stations[i]
                val isPS = sn.sched[station] != SchedStrategy.INF

                // Compute total fluid at station i
                var nAtI = 0.0
                for (c in 0 until Kc) {
                    if (augPhases.get(i, c).toInt() > 0) {
                        nAtI += Math.max(0.0, y[qIndices.get(i, c).toInt()])
                    }
                }

                val capacity = S[i].toDouble()

                for (c in 0 until Kc) {
                    val phases = augPhases.get(i, c).toInt()
                    if (phases > 0) {
                        val qi = qIndices.get(i, c).toInt()
                        val muVal = new_mu[i][c]?.get(0) ?: 0.0
                        val phiVal = new_pi[i][c]?.get(0) ?: 1.0

                        if (nAtI > 0 && muVal > 0) {
                            // PS/INF rate
                            val share = y[qi] / Math.max(nAtI, 1e-10)
                            val effectiveRate = if (isPS) {
                                muVal * Math.min(nAtI, capacity) * share
                            } else {
                                // INF (delay): each job served independently
                                muVal * Math.max(0.0, y[qi])
                            }

                            // Departures
                            yDot[qi] -= effectiveRate * phiVal

                            // Route to other stations/classes via routing table
                            for (j in 0 until M) {
                                for (d in 0 until Kc) {
                                    val pcd = newRt[i * Kc + c][j * Kc + d]
                                    if (pcd > 0 && augPhases.get(j, d).toInt() > 0) {
                                        val qj = qIndices.get(j, d).toInt()
                                        yDot[qj] += effectiveRate * phiVal * pcd
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return FluidPsRtResult(ode, qIndices, augPhases, stateSize, refIdx, Kc)
}

/**
 * Solve the fluid ODE and compute response time likelihood.
 *
 * @param result pre-built FluidPsRtResult
 * @param y0Levels M x K matrix of fluid levels per station per class
 * @param Rsampled observed response time
 * @param taggedClass tagged class index (0-based)
 * @return likelihood value
 */
fun infer_fluid_ps_rt_solve(
    result: FluidPsRtResult,
    y0Levels: Matrix,
    Rsampled: Double,
    taggedClass: Int
): Double {
    val newFluid = 1.0
    val M = y0Levels.numRows
    val K = result.newK - 1

    // Build initial condition from fluid levels
    val y0 = DoubleArray(result.stateSize)
    for (i in 0 until M) {
        for (k in 0 until K) {
            if (result.augPhases.get(i, k).toInt() > 0) {
                val idx = result.qIndices.get(i, k).toInt()
                y0[idx] = y0Levels.get(i, k)
            }
        }
    }

    // Move fluid from taggedClass to Tagged at refNode
    val refTaggedOrigIdx = result.qIndices.get(result.refIdx, taggedClass).toInt()
    val refTaggedNewIdx = result.qIndices.get(result.refIdx, K).toInt()
    y0[refTaggedOrigIdx] -= newFluid
    y0[refTaggedNewIdx] = newFluid

    // Ensure non-negative
    for (i in y0.indices) y0[i] = Math.max(0.0, y0[i])

    try {
        val integrator = DormandPrince853Integrator(1e-10, Rsampled, 1e-8, 1e-5)

        integrator.addEventHandler(object : EventHandler {
            override fun init(t0: Double, y0: DoubleArray, t: Double) {}
            override fun g(t: Double, y: DoubleArray): Double = y[refTaggedNewIdx]
            override fun eventOccurred(t: Double, y: DoubleArray, increasing: Boolean): EventHandler.Action =
                EventHandler.Action.STOP
            override fun resetState(t: Double, y: DoubleArray) {}
        }, 1e-6, 1e-8, 100)

        val yFinal = y0.copyOf()
        val tFinal = integrator.integrate(result.ode, 0.0, y0, Rsampled, yFinal)

        if (Rsampled <= tFinal + 1e-10) {
            val lastRates = DoubleArray(result.stateSize)
            result.ode.computeDerivatives(tFinal, yFinal, lastRates)
            return Math.max(0.0, -lastRates[refTaggedNewIdx] / newFluid)
        }
    } catch (e: Exception) {
        // ODE solver failed
    }
    return 0.0
}
