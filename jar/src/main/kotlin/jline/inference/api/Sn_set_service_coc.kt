/*
 * Copyright (c) 2012-2026, Imperial College London
 * All rights reserved.
 */
package jline.inference.api

import jline.lang.NetworkStruct
import jline.lang.constant.ProcessType
import jline.lang.nodes.Station
import jline.lang.JobClass
import jline.util.matrix.Matrix
import jline.util.matrix.MatrixCell
import jline.api.mam.map_exponential
import jline.api.mam.map_erlang
import jline.api.mam.map_hyperexp
import jline.api.mam.map_pie

/**
 * Update service rate in NetworkStruct preserving Map-based format.
 *
 * Works like sn_set_service with autoRefresh=true, but writes mu, phi,
 * proc, pie using Map-based indexing. This avoids breaking solvers that
 * expect the standard NetworkStruct format.
 *
 * @param sn NetworkStruct to update (modified in-place)
 * @param stationIdx station index (0-based)
 * @param classIdx class index (0-based)
 * @param rate new service rate (positive scalar)
 * @param scv squared coefficient of variation (default 1.0)
 */
fun sn_set_service_coc(sn: NetworkStruct, stationIdx: Int, classIdx: Int, rate: Double, scv: Double = 1.0) {
    val station = sn.stations[stationIdx]
    val jobclass = sn.jobclasses[classIdx]

    // Update rates and scv matrices
    sn.rates.set(stationIdx, classIdx, rate)
    sn.scv.set(stationIdx, classIdx, scv)

    // Skip if rate is invalid
    if (rate.isNaN() || rate <= 0 || rate.isInfinite()) return

    val meanVal = 1.0 / rate

    // Determine process type and create MAP based on SCV
    val MAP: MatrixCell
    val nPhases: Int
    val procType: ProcessType

    if (scv.isNaN() || Math.abs(scv - 1.0) < 1e-10) {
        MAP = map_exponential(meanVal)
        nPhases = 1
        procType = ProcessType.EXP
    } else if (scv < 1.0) {
        val k = Math.max(1, Math.ceil(1.0 / scv).toInt())
        MAP = map_erlang(meanVal, k)
        nPhases = k
        procType = ProcessType.ERLANG
    } else {
        val hyperMAP = map_hyperexp(meanVal, scv, 0.99)
        if (hyperMAP != null) {
            MAP = hyperMAP
            nPhases = 2
            procType = ProcessType.HYPEREXP
        } else {
            MAP = map_exponential(meanVal)
            nPhases = 1
            procType = ProcessType.EXP
        }
    }

    val D0 = MAP.get(0) as Matrix
    val D1 = MAP.get(1) as Matrix

    // Update proc map
    val stationProcMap = sn.proc.getOrPut(station) { HashMap() } as MutableMap
    stationProcMap[jobclass] = MAP

    // Update procid map
    val stationProcIdMap = sn.procid.getOrPut(station) { HashMap() } as MutableMap
    stationProcIdMap[jobclass] = procType

    // Update phases matrix
    sn.phases.set(stationIdx, classIdx, nPhases.toDouble())
    sn.phasessz.set(stationIdx, classIdx, Math.max(nPhases, 1).toDouble())

    // Recompute phaseshift for this station
    var cumSum = 0.0
    sn.phaseshift.set(stationIdx, 0, 0.0)
    for (c in 0 until sn.nclasses) {
        cumSum += sn.phasessz.get(stationIdx, c)
        if (c + 1 < sn.phaseshift.numCols) {
            sn.phaseshift.set(stationIdx, c + 1, cumSum)
        }
    }

    // Update mu (rates from -diag(D0))
    val muVec = Matrix(nPhases, 1)
    for (i in 0 until nPhases) {
        muVec.set(i, 0, -D0.get(i, i))
    }
    val stationMuMap = sn.mu.getOrPut(station) { HashMap() } as MutableMap
    stationMuMap[jobclass] = muVec

    // Update phi (completion probabilities)
    val phiVec = Matrix(nPhases, 1)
    for (i in 0 until nPhases) {
        val d1RowSum = (0 until D1.numCols).sumOf { j -> D1.get(i, j) }
        val d0Diag = -D0.get(i, i)
        if (d0Diag != 0.0) {
            phiVec.set(i, 0, d1RowSum / d0Diag)
        }
    }
    val stationPhiMap = sn.phi.getOrPut(station) { HashMap() } as MutableMap
    stationPhiMap[jobclass] = phiVec

    // Update pie (initial phase distribution)
    val pieVec = map_pie(MAP)
    val stationPieMap = sn.pie.getOrPut(station) { HashMap() } as MutableMap
    stationPieMap[jobclass] = pieVec
}
