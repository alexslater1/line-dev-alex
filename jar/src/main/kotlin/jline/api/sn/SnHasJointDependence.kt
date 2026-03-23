package jline.api.sn

import jline.lang.NetworkStruct

/**
 * Checks if the network has a station with joint-dependent service process
 *
 * @param sn - NetworkStruct object for the queueing network model
 * @return boolean
 */

fun snHasJointDependence(sn: NetworkStruct): Boolean {
    val hasLjd = sn.ljdscaling != null && sn.ljdscaling.isNotEmpty()
    val hasLjcd = sn.ljcdscaling != null && sn.ljcdscaling.isNotEmpty()
    return hasLjd || hasLjcd
}

/**
 * Stochastic network HasJointDependence algorithms
 */
@Suppress("unused")
class SnhasjointdependenceAlgo {
    companion object {
        // Class documentation marker for Dokka
    }
}
