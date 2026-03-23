/**
 * @file Retrial queue detection
 *
 * Checks if a network structure represents a valid BMAP/PH/N/N bufferless
 * retrial queue suitable for the retrial queue solver.
 *
 * Based on: Dudin et al., "Analysis of BMAP/PH/N-Type Queueing System with
 * Flexible Retrials Admission Control", Mathematics 2025, 13(9), 1434.
 *
 * @since LINE 3.1.0
 */
package jline.api.qsys

import jline.api.sn.snIsOpenModel
import jline.lang.NetworkStruct
import jline.lang.constant.DropStrategy
import jline.lang.constant.NodeType

/**
 * Information about a valid retrial queue topology.
 *
 * @property isRetrial True if network is valid BMAP/PH/N/N retrial topology
 * @property stationIdx Station index of the retrial queue (-1 if not found)
 * @property nodeIdx Node index of the retrial queue (-1 if not found)
 * @property sourceIdx Station index of the source (-1 if not found)
 * @property classIdx Class index (0-based, -1 if not found)
 * @property errorMsg Error message if not valid
 * @property N Number of servers
 * @property alpha Retrial rate per customer in orbit
 * @property gamma Orbit impatience rate (default 0)
 * @property p Batch rejection probability (default 0)
 * @property R Admission threshold (from FCR or default N-1)
 */
data class RetrialInfo(
    var isRetrial: Boolean = false,
    var stationIdx: Int = -1,
    var nodeIdx: Int = -1,
    var sourceIdx: Int = -1,
    var classIdx: Int = -1,
    var errorMsg: String = "",
    var N: Int = 0,
    var alpha: Double = 0.1,
    var gamma: Double = 0.0,
    var p: Double = 0.0,
    var R: Int = 0
)

/**
 * Checks if network is a valid BMAP/PH/N/N bufferless retrial queue.
 *
 * Validates that the network structure matches the requirements for
 * the BMAP/PH/N/N retrial queue solver:
 * - Single bufferless queue (capacity == number of servers)
 * - Retrial drop strategy configured
 * - BMAP/MAP arrival process at source
 * - PH/Exp service at queue
 * - Open class model
 *
 * @param sn Network structure
 * @return RetrialInfo with validation results and extracted parameters
 */
@JvmName("qsys_is_retrial")
fun qsys_is_retrial(sn: NetworkStruct): RetrialInfo {
    val retInfo = RetrialInfo()

    // Check if model is open
    if (!snIsOpenModel(sn)) {
        retInfo.errorMsg = "BMAP/PH/N/N retrial solver requires open queueing model."
        return retInfo
    }

    // Check for single class (current limitation)
    if (sn.nclasses > 1) {
        retInfo.errorMsg = "BMAP/PH/N/N retrial solver currently supports single class only."
        return retInfo
    }

    retInfo.classIdx = 0  // 0-based in JAR

    // Find bufferless queue stations (capacity == nservers, finite capacity)
    val bufferlessStations = ArrayList<Int>()
    if (sn.stationToNode == null || sn.cap == null || sn.nservers == null) {
        retInfo.errorMsg = "Missing network structure fields."
        return retInfo
    }
    for (ist in 0 until sn.nstations) {
        if (ist >= sn.stationToNode.numRows) continue
        val nodeIdx = sn.stationToNode.get(ist, 0).toInt()
        if (nodeIdx >= 0 && nodeIdx < sn.nodetype.size && sn.nodetype[nodeIdx] == NodeType.Queue) {
            if (ist >= sn.cap.numRows || ist >= sn.nservers.numRows) continue
            val cap = sn.cap.get(ist, 0)
            val nservers = sn.nservers.get(ist, 0)
            if (java.lang.Double.isFinite(cap) && Math.abs(cap - nservers) < 1e-10) {
                bufferlessStations.add(ist)
            }
        }
    }

    if (bufferlessStations.isEmpty()) {
        retInfo.errorMsg = "No bufferless queue found (capacity must equal number of servers)."
        return retInfo
    }

    // Check retrial drop strategy
    var retrialStation = -1
    for (ist in bufferlessStations) {
        if (sn.droprule != null && ist < sn.stations.size) {
            val station = sn.stations[ist]
            val classDropRules = sn.droprule[station]
            if (classDropRules != null) {
                for (dropEntry in classDropRules.values) {
                    if (dropEntry == DropStrategy.Retrial || dropEntry == DropStrategy.RetrialWithLimit) {
                        retrialStation = ist
                        break
                    }
                }
            }
        }
        if (retrialStation >= 0) break
    }

    if (retrialStation < 0) {
        retInfo.errorMsg = "No retrial drop strategy configured on bufferless queue."
        return retInfo
    }

    retInfo.stationIdx = retrialStation
    retInfo.nodeIdx = sn.stationToNode.get(retrialStation, 0).toInt()
    retInfo.N = sn.nservers.get(retrialStation, 0).toInt()

    // Find source station
    var sourceStation = -1
    for (ist in 0 until sn.nstations) {
        val nodeIdx = sn.stationToNode.get(ist, 0).toInt()
        if (nodeIdx >= 0 && nodeIdx < sn.nodetype.size && sn.nodetype[nodeIdx] == NodeType.Source) {
            sourceStation = ist
            break
        }
    }

    if (sourceStation < 0) {
        retInfo.errorMsg = "No Source node found."
        return retInfo
    }

    retInfo.sourceIdx = sourceStation

    // Default retrial rate (will be extracted from sn in solver)
    retInfo.alpha = 0.1

    // Default admission threshold
    retInfo.R = retInfo.N - 1

    // Check if region field exists and extract admission threshold from FCR
    if (sn.nregions > 0 && sn.region != null) {
        for (f in 0 until sn.nregions) {
            val regionMatrix = sn.region.get(f)
            if (regionMatrix != null && regionMatrix.numRows > retrialStation) {
                val globalCapCol = regionMatrix.numCols - 1
                val R_fcr = regionMatrix.get(retrialStation, globalCapCol)
                if (R_fcr > 0 && R_fcr <= (retInfo.N - 1).toDouble()) {
                    retInfo.R = R_fcr.toInt()
                    break
                }
            }
        }
    }

    // All validations passed
    retInfo.isRetrial = true
    return retInfo
}

/**
 * Queueing system is_retrial algorithms
 */
@Suppress("unused")
class QsysIsRetrialAlgo {
    companion object {
        // Class documentation marker for Dokka
    }
}
