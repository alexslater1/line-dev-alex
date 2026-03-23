/*
 * Copyright (c) 2012-2026, Imperial College London
 * All rights reserved.
 */
package jline.inference.api

import jline.inference.util.OptimUtils
import jline.lang.*
import jline.lang.constant.SchedStrategy
import jline.lang.nodes.Queue
import jline.util.matrix.Matrix

/**
 * FMLPS demand estimation using fluid-based likelihood.
 *
 * Estimates service demands at a PS queue using the Fluid Maximum
 * Likelihood for Processor Sharing method. Uses sn_set_service_coc for
 * fast parameter updates, avoiding model rebuild in the optimization loop.
 *
 * Calls infer_fluid_ps_rt_likelihood to build the augmented ODE model,
 * then infer_fluid_ps_rt_solve to compute per-sample likelihoods.
 *
 * @param model LINE Network model with delay rates set and queue rates to estimate
 * @param node PS queue node
 * @param rt response time samples (n x 1)
 * @param classVec class of each sample (0-based)
 * @param ql queue lengths at arrival (n x R matrix)
 * @param W total population (number of threads/jobs)
 * @return estimated demands (1 x R array)
 */
fun infer_fmlps(
    model: Network,
    node: Queue,
    rt: DoubleArray,
    classVec: IntArray,
    ql: Matrix,
    W: Int
): DoubleArray {
    val sn = model.getStruct(false)
    val R = sn.nclasses
    val V = node.numberOfServers
    val stIdx = node.getStationIdx()

    val xLB = DoubleArray(R) { rt.min() / W }
    val xUB = DoubleArray(R) { rt.max() }

    // Initial point estimate
    val meanQL = (0 until ql.numRows).map { i ->
        (0 until ql.numCols).sumOf { j -> ql.get(i, j) }
    }.average()
    val Vtilde = Math.min(meanQL, V.toDouble())
    val x0 = DoubleArray(R) { j ->
        val classRT = (0 until rt.size).filter { classVec[it] == j }.map { rt[it] }
        if (classRT.isNotEmpty()) Vtilde * classRT.average() / meanQL else xLB[j]
    }

    // Cache station indices
    var delayIdx = -1
    var refIdx = -1
    for (ii in 0 until sn.nstations) {
        val station = sn.stations[ii]
        if (sn.sched[station] == SchedStrategy.INF) {
            delayIdx = ii
        } else {
            refIdx = ii
        }
    }

    // Cache delay rates
    val delayRates = DoubleArray(R) { k ->
        val station = sn.stations[delayIdx]
        val jobclass = sn.jobclasses[k]
        sn.mu[station]!![jobclass]!!.get(0, 0)
    }

    // Optimization
    val (demandEst, _) = OptimUtils.fmincon(
        objFun = { x ->
            val TOL = 1e-6

            // Update service rates
            for (r in 0 until R) {
                sn_set_service_coc(sn, stIdx, r, 1.0 / x[r])
            }

            val uniqueTC = classVec.toSet().sorted()
            val ftemp = DoubleArray(rt.size)

            for (tc in uniqueTC) {
                val mask = (0 until classVec.size).filter { classVec[it] == tc }
                val rtTc = mask.map { rt[it] }.toDoubleArray()
                val qlTc = Matrix(mask.size, R)
                for (i in mask.indices) {
                    for (r in 0 until R) {
                        qlTc.set(i, r, ql.get(mask[i], r))
                    }
                }

                // Build augmented model and ODE handle from sn struct
                val fluidResult = infer_fluid_ps_rt_likelihood(sn, tc)

                // Solve each sample reusing the same ODE handle
                for (rr in rtTc.indices) {
                    val aQueue = DoubleArray(R) { r -> qlTc.get(rr, r) }

                    // Compute initial fluid levels
                    val M = fluidResult.augPhases.numRows
                    val y0Levels = Matrix(M, R)

                    // Delay station: distribute remaining fluid proportionally to delay rates
                    val totalDelayRate = delayRates.sum()
                    val delayJobsTotal = W - aQueue.sum()
                    for (k in 0 until R) {
                        val delayJob = if (totalDelayRate > 0) delayJobsTotal * delayRates[k] / totalDelayRate else 0.0
                        y0Levels.set(delayIdx, k, delayJob)
                    }

                    // Queue station: observed queue lengths per class
                    for (k in 0 until R) {
                        y0Levels.set(refIdx, k, aQueue[k])
                    }

                    val like = infer_fluid_ps_rt_solve(fluidResult, y0Levels, rtTc[rr], tc)
                    ftemp[mask[rr]] = Math.log(TOL + like)
                }
            }
            -ftemp.sum()
        },
        x0 = x0,
        lb = xLB,
        ub = xUB,
        maxIter = 10000
    )

    return demandEst
}
