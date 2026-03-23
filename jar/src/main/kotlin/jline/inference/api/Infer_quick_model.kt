/*
 * Copyright (c) 2012-2026, Imperial College London
 * All rights reserved.
 */
package jline.inference.api

import jline.lang.*
import jline.lang.constant.SchedStrategy
import jline.lang.nodes.*
import jline.lang.processes.Exp
import jline.util.matrix.Matrix

/**
 * Generate simple queueing network based on given parameters.
 *
 * @param open true for open network, false for closed
 * @param stations scheduling strategies for each station
 * @param classes service demands matrix (numClasses x numStations)
 * @param servers number of servers per station (default: 1 each)
 * @param jobs number of jobs per class (default: 1 each, ignored for open)
 * @param routing optional routing matrices per class
 * @return Network model
 */
fun infer_quick_model(
    open: Boolean,
    stations: List<SchedStrategy>,
    classes: Array<DoubleArray>,
    servers: IntArray? = null,
    jobs: IntArray? = null,
    routing: Array<DoubleArray>? = null
): Network {
    val numStations = stations.size
    val numClasses = classes.size
    val actualServers = servers ?: IntArray(numStations) { 1 }
    val actualJobs = jobs ?: IntArray(numClasses) { 1 }

    val model = Network("quickModel")
    val nodes = arrayOfNulls<Node>(numStations + if (open) 2 else 0)

    if (open) {
        nodes[0] = Source(model, "mySource")
        nodes[numStations + 1] = Sink(model, "mySink")
    }

    for (i in 0 until numStations) {
        val idx = if (open) i + 1 else i
        val queue = Queue(model, "QueueStation${i + 1}", stations[i])
        queue.setNumberOfServers(actualServers[i])
        nodes[idx] = queue
    }

    val P = model.initRoutingMatrix()
    val jobClasses = arrayOfNulls<JobClass>(numClasses)

    for (c in 0 until numClasses) {
        if (!open) {
            val refNode = nodes[0] as Station
            jobClasses[c] = ClosedClass(model, "Class${c + 1}", actualJobs[c], refNode)
        } else {
            jobClasses[c] = OpenClass(model, "Class${c + 1}")
        }

        for (i in 0 until numStations) {
            val idx = if (open) i + 1 else i
            val queue = nodes[idx] as Queue
            queue.setService(jobClasses[c], Exp.fitMean(classes[c][i]))
        }
    }

    if (open) {
        val nodeList = (0 until nodes.size).mapNotNull { nodes[it] }
        for (c in 0 until numClasses) {
            P.set(jobClasses[c], Network.serialRouting(nodeList))
        }
    } else {
        for (c in 0 until numClasses) {
            if (routing != null) {
                P.set(jobClasses[c], Matrix(routing[c]))
            } else {
                val nodeList = (0 until numStations).mapNotNull { nodes[it] }
                P.set(jobClasses[c], Network.serialRouting(nodeList))
            }
        }
    }

    model.link(P)
    return model
}
