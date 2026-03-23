/**
 * @file AoI parameter extraction
 *
 * Extracts parameters from LINE network structure for AoI analysis.
 * Maps LINE model representation to AoI function inputs:
 * - For bufferless (capacity=1): extract (tau, T, sigma, S, p)
 * - For single-buffer (capacity=2): extract (lambda, sigma, S, r)
 *
 * @since LINE 3.0
 */
package jline.api.aoi

import jline.lang.NetworkStruct
import jline.lang.constant.SchedStrategy
import jline.util.matrix.Matrix

/**
 * Extract parameters from LINE network for AoI analysis.
 *
 * @param sn Network structure from Network.getStruct()
 * @param aoiInfo Topology information from aoi_is_aoi()
 * @param aoiPreemption Optional preemption probability override (0-1), or NaN to auto-detect
 * @return AoiParams with extracted parameters for the AoI solver
 * @throws IllegalArgumentException if the network is not a valid AoI topology
 */
fun aoi_extract_params(sn: NetworkStruct, aoiInfo: AoiValidationResult, aoiPreemption: Double = Double.NaN): AoiParams {
    require(aoiInfo.isAoI) { "Network is not a valid AoI topology: ${aoiInfo.errorMsg}" }

    val sourceStation = aoiInfo.sourceStation
    val queueStation = aoiInfo.queueStation
    val capacity = aoiInfo.capacity

    // Find the open class
    var classIdx = -1
    for (r in 0 until sn.nclasses) {
        if (java.lang.Double.isInfinite(sn.njobs.get(0, r))) {
            classIdx = r
            break
        }
    }
    require(classIdx >= 0) { "No open class found" }

    // =========================================================================
    // EXTRACT SERVICE PROCESS (same for both bufferless and single-buffer)
    // =========================================================================
    val queueStationObj = sn.stations[queueStation]
    val jobClassObj = sn.jobclasses[classIdx]
    val serviceProc = sn.proc[queueStationObj]?.get(jobClassObj)

    val sigma: Matrix
    val S: Matrix

    if (serviceProc == null || serviceProc.size() < 2 || serviceProc[0].hasNaN()) {
        // Simple exponential service
        val mu = sn.rates.get(queueStation, classIdx)
        sigma = Matrix(1, 1)
        sigma.set(0, 0, 1.0)
        S = Matrix(1, 1)
        S.set(0, 0, -mu)
    } else {
        // Phase-type service: extract from MAP representation
        val result = aoi_dist2ph(serviceProc)
        sigma = result.alpha
        S = result.T
    }

    // =========================================================================
    // EXTRACT ARRIVAL PROCESS AND PREEMPTION PARAMETER
    // =========================================================================

    val schedStrategy = sn.sched[queueStationObj]

    if (capacity == 1) {
        // BUFFERLESS: PH/PH/1/1 or PH/PH/1/1*
        // Need: tau, T, sigma, S, p

        val sourceStationObj = sn.stations[sourceStation]
        val arrivalProc = sn.proc[sourceStationObj]?.get(jobClassObj)

        val tau: Matrix
        val T: Matrix

        if (arrivalProc == null || arrivalProc.size() < 2 || arrivalProc[0].hasNaN()) {
            // Simple exponential arrival (Poisson)
            val lambda = sn.rates.get(sourceStation, classIdx)
            tau = Matrix(1, 1)
            tau.set(0, 0, 1.0)
            T = Matrix(1, 1)
            T.set(0, 0, -lambda)
        } else {
            // Phase-type arrival
            val result = aoi_dist2ph(arrivalProc)
            tau = result.alpha
            T = result.T
        }

        // Determine preemption probability p
        val p: Double
        if (!java.lang.Double.isNaN(aoiPreemption)) {
            p = aoiPreemption
        } else {
            // Automatic based on scheduling strategy
            p = when (schedStrategy) {
                SchedStrategy.FCFS -> 0.0
                SchedStrategy.LCFS -> 0.0
                SchedStrategy.LCFSPR -> 1.0
                else -> 0.0
            }
        }

        return AoiParams(
            tau = tau,
            T = T,
            sigma = sigma,
            S = S,
            p = p,
            lambda = Double.NaN,
            r = Double.NaN,
            systemType = "bufferless",
            arrivalType = "PH"
        )

    } else {
        // SINGLE-BUFFER: M/PH/1/2 or M/PH/1/2*
        // Need: lambda, sigma, S, r

        // Arrival must be Poisson (exponential interarrival times)
        val lambda = sn.rates.get(sourceStation, classIdx)

        // Determine replacement probability r
        val r: Double
        if (!java.lang.Double.isNaN(aoiPreemption)) {
            r = aoiPreemption
        } else {
            r = when (schedStrategy) {
                SchedStrategy.FCFS -> 0.0
                SchedStrategy.LCFS -> 1.0
                SchedStrategy.LCFSPR -> 1.0
                else -> 0.0
            }
        }

        return AoiParams(
            tau = null,
            T = null,
            sigma = sigma,
            S = S,
            p = Double.NaN,
            lambda = lambda,
            r = r,
            systemType = "singlebuffer",
            arrivalType = "M"
        )
    }
}

/**
 * Age of Information parameter extraction algorithms
 */
@Suppress("unused")
class AoiExtractParamsAlgo {
    companion object {
        // Class documentation marker for Dokka
    }
}
