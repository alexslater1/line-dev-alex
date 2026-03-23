/**
 * @file AoI topology validation
 *
 * Validates that a network structure is suitable for Age of Information
 * analysis using the analytical AoI formulas.
 *
 * Requirements:
 * - Single open class
 * - Single-queue open system: Source -> Queue -> Sink
 * - Queue capacity = 1 (bufferless) or 2 (single-buffer)
 * - Single server (nservers = 1)
 * - Scheduling: FCFS, LCFS, or LCFSPR
 * - For capacity=2: arrivals must be exponential (Poisson)
 *
 * @since LINE 3.0
 */
package jline.api.aoi

import jline.lang.NetworkStruct
import jline.lang.constant.NodeType
import jline.lang.constant.SchedStrategy

/**
 * Check if network is a valid AoI topology for analytical AoI analysis.
 *
 * @param sn Network structure from Network.getStruct()
 * @return AoiValidationResult with isAoI flag and topology information
 */
fun aoi_is_aoi(sn: NetworkStruct): AoiValidationResult {
    val defaultResult = AoiValidationResult(
        isAoI = false,
        sourceIdx = -1,
        queueIdx = -1,
        sinkIdx = -1,
        sourceStation = -1,
        queueStation = -1,
        capacity = -1,
        schedStrategy = "",
        systemType = "",
        errorMsg = ""
    )

    // Check 1: Must have at least one open class
    val njobs = sn.njobs
    var hasOpen = false
    val openClasses = java.util.ArrayList<Int>()
    for (r in 0 until sn.nclasses) {
        if (java.lang.Double.isInfinite(njobs.get(0, r))) {
            hasOpen = true
            openClasses.add(r)
        }
    }
    if (!hasOpen) {
        return defaultResult.copy(errorMsg = "Not an open model - all classes are closed")
    }

    // Check 2: Must have exactly one open class
    if (openClasses.size > 1) {
        return defaultResult.copy(
            errorMsg = "Multiple open classes found (${openClasses.size}) - AoI analysis requires single class"
        )
    }

    // Find Source, Queue, and Sink nodes
    var sourceNodeIdx = -1
    var queueNodeIdx = -1
    var sinkNodeIdx = -1
    var sourceCount = 0
    var queueCount = 0
    var sinkCount = 0

    val nodetype = sn.nodetype
    for (i in 0 until nodetype.size) {
        when (nodetype[i]) {
            NodeType.Source -> {
                sourceNodeIdx = i
                sourceCount++
            }
            NodeType.Queue -> {
                queueNodeIdx = i
                queueCount++
            }
            NodeType.Sink -> {
                sinkNodeIdx = i
                sinkCount++
            }
            else -> { /* ignore other node types */ }
        }
    }

    // Check: exactly one source
    if (sourceCount == 0) {
        return defaultResult.copy(errorMsg = "No source node found")
    }
    if (sourceCount > 1) {
        return defaultResult.copy(errorMsg = "Multiple source nodes found ($sourceCount)")
    }

    // Check: exactly one sink
    if (sinkCount == 0) {
        return defaultResult.copy(errorMsg = "No sink node found")
    }
    if (sinkCount > 1) {
        return defaultResult.copy(errorMsg = "Multiple sink nodes found ($sinkCount)")
    }

    // Check: exactly one queue
    if (queueCount == 0) {
        return defaultResult.copy(errorMsg = "No queue node found")
    }
    if (queueCount > 1) {
        return defaultResult.copy(
            errorMsg = "Multiple queue nodes found ($queueCount) - AoI analysis supports single queue only"
        )
    }

    // Get station indices (nodeToStation is a Matrix in JAR)
    val sourceStation = sn.nodeToStation.get(sourceNodeIdx, 0).toInt()
    val queueStation = sn.nodeToStation.get(queueNodeIdx, 0).toInt()

    // Check: Single server
    val nservers = sn.nservers.get(queueStation, 0).toInt()
    if (nservers != 1) {
        return defaultResult.copy(
            errorMsg = "Queue has $nservers servers - AoI analysis requires single server (c=1)"
        )
    }

    // Check: Queue capacity (1 for bufferless, 2 for single-buffer)
    val cap = sn.cap.get(queueStation, 0)
    if (java.lang.Double.isInfinite(cap) || cap > 2 || cap < 1) {
        return defaultResult.copy(
            errorMsg = "Queue capacity is $cap - AoI analysis requires capacity 1 (bufferless) or 2 (single-buffer)"
        )
    }

    val capacity = cap.toInt()
    val systemType = if (capacity == 1) "bufferless" else "singlebuffer"

    // Check: Scheduling strategy (FCFS, LCFS, or LCFSPR)
    val schedStrategy = sn.sched[sn.stations[queueStation]]
    val schedName = schedStrategy?.toString() ?: "unknown"

    if (schedStrategy != SchedStrategy.FCFS &&
        schedStrategy != SchedStrategy.LCFS &&
        schedStrategy != SchedStrategy.LCFSPR) {
        return defaultResult.copy(
            errorMsg = "Unsupported scheduling strategy ($schedName) - AoI analysis supports FCFS, LCFS, or LCFSPR only"
        )
    }

    // For single-buffer (capacity=2): arrivals must be exponential (Poisson)
    if (capacity == 2) {
        val classIdx = openClasses[0]
        val sourceStationObj = sn.stations[sourceStation]
        val jobClassObj = sn.jobclasses[classIdx]
        val arrivalProc = sn.proc[sourceStationObj]?.get(jobClassObj)
        if (arrivalProc != null && arrivalProc.size() >= 1) {
            val D0 = arrivalProc[0]
            if (D0 != null && D0.numRows > 1) {
                return defaultResult.copy(
                    errorMsg = "Single-buffer (capacity=2) requires exponential arrivals (Poisson process)"
                )
            }
        }
    }

    // Check: No self-loops
    val K = sn.nclasses
    val classIdx = openClasses[0]
    val rtIdx = (queueStation) * K + classIdx
    if (sn.rt != null && rtIdx < sn.rt.numRows && rtIdx < sn.rt.numCols) {
        if (sn.rt.get(rtIdx, rtIdx) > 0) {
            return defaultResult.copy(
                errorMsg = "Self-loop detected at queue - violates AoI model assumptions"
            )
        }
    }

    // All checks passed
    return AoiValidationResult(
        isAoI = true,
        sourceIdx = sourceNodeIdx,
        queueIdx = queueNodeIdx,
        sinkIdx = sinkNodeIdx,
        sourceStation = sourceStation,
        queueStation = queueStation,
        capacity = capacity,
        schedStrategy = schedName,
        systemType = systemType,
        errorMsg = ""
    )
}

/**
 * Age of Information topology validation algorithms
 */
@Suppress("unused")
class AoiIsAoiAlgo {
    companion object {
        // Class documentation marker for Dokka
    }
}
