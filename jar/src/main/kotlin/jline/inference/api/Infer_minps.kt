/*
 * Copyright (c) 2012-2026, Imperial College London
 * All rights reserved.
 */
package jline.inference.api

import jline.lang.Network
import jline.lang.nodes.Queue
import jline.util.matrix.Matrix

/**
 * MINPS demand estimation method.
 *
 * Runs both MLPS and RPS estimators and selects the one with the
 * smaller mean demand estimate.
 *
 * @param model LINE Network model with delay rates set
 * @param node PS queue node
 * @param rt response time samples (column vector)
 * @param classVec class of each sample (0-based)
 * @param ql queue lengths at arrival (n x R matrix)
 * @return estimated demands (1 x R array)
 */
fun infer_minps(model: Network, node: Queue, rt: DoubleArray, classVec: IntArray, ql: Matrix): DoubleArray {
    val V = node.numberOfServers

    // Run MLPS
    val demandEstMLPS = infer_mlps(model, node, rt, classVec, ql)
    // Run RPS
    val demandEstRPS = infer_rps(rt, classVec, ql, V)

    // Choose the smallest mean result
    return if (demandEstMLPS.average() < demandEstRPS.average()) {
        demandEstMLPS
    } else {
        demandEstRPS
    }
}
