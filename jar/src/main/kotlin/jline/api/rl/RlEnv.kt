/**
 * @file RL Environment for Queueing Network Routing
 *
 * Implements a Reinforcement Learning environment that wraps a queueing network
 * model, providing an interface for RL agents to interact with it through sampling
 * and state updates. The environment supports state/action space queries and
 * uses the SSA solver to generate event samples.
 *
 * Port of: matlab/src/api/rl/rl_env.m
 *
 * @since LINE 3.0
 */
package jline.api.rl

import jline.lang.Network
import jline.lang.state.FromMarginal
import jline.solvers.ssa.SolverSSA
import jline.util.matrix.Matrix

/**
 * RL environment for queueing network routing decisions.
 *
 * This environment wraps a queueing network model and provides methods for:
 * - Checking if the current state is within defined state/action spaces
 * - Sampling the next event using SSA simulation
 * - Updating the model state after an action
 * - Resetting the environment to its initial state
 *
 * The state is defined by queue lengths at each queue node. The action space
 * corresponds to routing decisions (one per queue).
 *
 * @param model the queueing network model (a copy is stored internally)
 * @param idxOfQueueInNodes indices of queue nodes in model.getNodes()
 * @param idxOfSourceInNodes indices of source nodes in model.getNodes()
 * @param stateSize maximum number of jobs per queue to consider in the state space
 * @param gamma discount factor for future rewards (0 < gamma <= 1)
 */
class RlEnv(
    val model: Network,
    val idxOfQueueInNodes: IntArray,
    val idxOfSourceInNodes: IntArray,
    val stateSize: Int,
    val gamma: Double
) {
    /** Number of possible actions (equal to the number of queues). */
    val actionSize: Int = idxOfQueueInNodes.size

    /**
     * Checks if the current model state is within the defined state space.
     *
     * A state is valid if the total number of jobs at each queue node does not
     * exceed [stateSize].
     *
     * @param nodes the list of nodes from the model (model.getNodes())
     * @return true if all queue lengths are within bounds
     */
    fun isInStateSpace(nodes: List<jline.lang.nodes.Node>): Boolean {
        for (i in idxOfQueueInNodes) {
            val node = nodes[i]
            if (node is jline.lang.nodes.StatefulNode) {
                val stateMatrix = node.state
                if (stateMatrix.elementSum() > stateSize) {
                    return false
                }
            }
        }
        return true
    }

    /**
     * Checks if actions can be taken from the current model state.
     *
     * An action is valid if each queue can accept at least one more job,
     * i.e., the total number of jobs at each queue is strictly less than [stateSize].
     *
     * @param nodes the list of nodes from the model (model.getNodes())
     * @return true if all queues can accept new jobs
     */
    fun isInActionSpace(nodes: List<jline.lang.nodes.Node>): Boolean {
        for (i in idxOfQueueInNodes) {
            val node = nodes[i]
            if (node is jline.lang.nodes.StatefulNode) {
                val stateMatrix = node.state
                if (stateMatrix.elementSum() > stateSize - 1) {
                    return false
                }
            }
        }
        return true
    }

    /**
     * Samples the next event from the environment using the SSA solver.
     *
     * Generates a single system event and returns the elapsed time and
     * the index of the node where a departure occurred.
     *
     * @return a [SampleEvent] containing the time delta and departure node index
     */
    fun sample(): SampleEvent {
        val solver = SolverSSA(model, "verbose", false)
        val sampleResult = solver.sampleSysAggr(1)
        val t = if (sampleResult.t != null && sampleResult.t.length() > 0)
            sampleResult.t.get(0)
        else
            0.0

        // The event matrix has columns: time, node, class
        // Each row pair represents the two events (DEP at source, ARV at destination)
        // We need to find the departure node
        var depNode = -1
        val eventMatrix = sampleResult.event
        if (eventMatrix != null && eventMatrix.getNumRows() > 0) {
            // In the SampleResult, the event data encodes events
            // The MATLAB code checks sample.event{1}.event and sample.event{2}.event
            // In the JAR, events are stored in the event matrix with columns: time, node, class
            // We look for DEP events from the original SampleSysState
            // Since convertToSampleResult stores events as matrix rows,
            // we check the node indices
            for (row in 0 until eventMatrix.getNumRows()) {
                val nodeIdx = eventMatrix.get(row, 1).toInt()
                // Check if this is a source node (departure from source = new arrival)
                // or a queue node (departure from queue = job completion)
                if (idxOfSourceInNodes.contains(nodeIdx) || idxOfQueueInNodes.contains(nodeIdx)) {
                    depNode = nodeIdx
                    break
                }
            }
            // If we didn't find a matching node, use the first event's node
            if (depNode == -1 && eventMatrix.getNumRows() > 0) {
                depNode = eventMatrix.get(0, 1).toInt()
            }
        }

        return SampleEvent(t, depNode)
    }

    /**
     * Updates the model state after an event.
     *
     * Sets the state of each queue node based on the new marginal queue lengths.
     *
     * @param newState array of new queue lengths, one per queue (indexed by position in [idxOfQueueInNodes])
     */
    fun update(newState: IntArray) {
        val sn = model.getStruct(false)
        for (i in idxOfQueueInNodes.indices) {
            val nodeIdx = idxOfQueueInNodes[i]
            val node = model.getNodes()[nodeIdx]
            if (node is jline.lang.nodes.StatefulNode) {
                val marginal = Matrix(1, 1)
                marginal.set(0, 0, newState[i].toDouble())
                val newNodeState = FromMarginal.fromMarginal(sn, nodeIdx, marginal)
                node.state = newNodeState
            }
        }
    }

    /**
     * Resets the environment to its initial state.
     *
     * Calls model.reset() and model.initDefault() to restore the model
     * to its default configuration.
     */
    fun reset() {
        model.reset()
        model.initDefault()
    }
}

/**
 * Container for a sampled event from the RL environment.
 *
 * @param t the elapsed time until the event
 * @param depNode the index of the node where the departure occurred
 */
data class SampleEvent(val t: Double, val depNode: Int)
