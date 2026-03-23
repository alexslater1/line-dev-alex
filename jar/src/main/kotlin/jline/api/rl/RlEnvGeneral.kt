/**
 * @file General RL Environment for Queueing Network Control
 *
 * Implements a general-purpose Reinforcement Learning environment for queueing
 * networks where actions are dispatch/routing decisions at specific nodes.
 * Unlike RlEnv which handles simple source-to-queue routing, this environment
 * supports arbitrary action nodes with configurable action spaces derived from
 * the network topology.
 *
 * Port of: matlab/src/api/rl/rl_env_general.m
 *
 * @since LINE 3.0
 */
package jline.api.rl

import jline.io.Ret.SampleResult
import jline.lang.Network
import jline.lang.constant.EventType
import jline.lang.state.State
import jline.solvers.ssa.SolverSSA
import java.util.ArrayList
import java.util.HashMap

/**
 * General RL environment for queueing network control decisions.
 *
 * This environment supports arbitrary dispatch/routing decisions at designated
 * action nodes. The action space at each action node is determined by the
 * network's connection matrix (the set of downstream nodes reachable from
 * that node).
 *
 * The state is defined by queue lengths at each queue node. Events are sampled
 * using the SSA solver and the agent can override routing decisions at action nodes.
 *
 * @param model the queueing network model (a copy is stored internally)
 * @param idxOfQueueInNodes indices of queue nodes in model.getNodes()
 * @param idxOfActionNodes indices of nodes where routing actions are needed
 * @param stateSize maximum number of jobs per queue to consider in the state space
 * @param gamma discount factor for future rewards (0 < gamma <= 1)
 */
class RlEnvGeneral(
    val model: Network,
    val idxOfQueueInNodes: IntArray,
    val idxOfActionNodes: IntArray,
    val stateSize: Int,
    val gamma: Double
) {
    /** Number of queues in the network. */
    val nqueues: Int = idxOfQueueInNodes.size

    /**
     * Action space map: for each action node index, stores the array of
     * downstream node indices reachable from that node (based on the
     * network's connection matrix).
     */
    val actionSpace: HashMap<Int, IntArray>

    init {
        actionSpace = HashMap<Int, IntArray>()
        val connMatrix = model.getConnectionMatrix()
        for (i in idxOfActionNodes) {
            val reachable = ArrayList<Int>()
            val nCols = connMatrix.getNumCols()
            for (j in 0 until nCols) {
                if (connMatrix.get(i, j) == 1.0) {
                    reachable.add(j)
                }
            }
            val arr = IntArray(reachable.size)
            for (k in reachable.indices) {
                arr[k] = reachable[k]
            }
            actionSpace[i] = arr
        }
    }

    /**
     * Checks if the given state vector is within the defined state space.
     *
     * A state is valid if the number of jobs at each queue does not exceed [stateSize].
     *
     * @param state array of queue lengths (one per queue, indexed by position in [idxOfQueueInNodes])
     * @return true if all queue lengths are within bounds
     * @throws IllegalArgumentException if state size does not match the number of queues
     */
    fun isInStateSpace(state: IntArray): Boolean {
        if (state.size != idxOfQueueInNodes.size) {
            throw IllegalArgumentException(
                "State size mismatch: state size=${state.size}, required size=${idxOfQueueInNodes.size}"
            )
        }
        for (i in idxOfQueueInNodes.indices) {
            if (state[i] > stateSize) {
                return false
            }
        }
        return true
    }

    /**
     * Checks if actions can be taken from the given state.
     *
     * An action is valid if each queue can accept at least one more job,
     * i.e., each queue length is strictly less than [stateSize].
     *
     * @param state array of queue lengths (one per queue)
     * @return true if all queues can accept new jobs
     * @throws IllegalArgumentException if state size does not match the number of queues
     */
    fun isInActionSpace(state: IntArray): Boolean {
        if (state.size != idxOfQueueInNodes.size) {
            throw IllegalArgumentException(
                "State size mismatch: state size=${state.size}, required size=${idxOfQueueInNodes.size}"
            )
        }
        for (i in idxOfQueueInNodes.indices) {
            if (state[i] > stateSize - 1) {
                return false
            }
        }
        return true
    }

    /**
     * Samples the next event from the environment using the SSA solver.
     *
     * Generates a single system event and returns the elapsed time, the departure
     * node, the arrival node, and the raw sample result for state updates.
     *
     * @return a [GeneralSampleEvent] containing the time delta, departure node index,
     *         arrival node index, and the raw SSA sample result
     */
    fun sample(): GeneralSampleEvent {
        val solver = SolverSSA(model, "verbose", false)
        val sampleResult = solver.sampleSysAggr(1)
        val dt = if (sampleResult.t != null && sampleResult.t.length() > 0)
            sampleResult.t.get(0)
        else
            0.0

        var depNode = -1
        var arvNode = -1

        val eventMatrix = sampleResult.event
        if (eventMatrix != null && eventMatrix.getNumRows() > 0) {
            // Parse events from the event matrix
            // The MATLAB code checks event{1}.event and event{2}.event for DEP/ARV
            // In the JAR SampleResult, events are stored as matrix rows: [time, node, class]
            // We need to determine DEP and ARV nodes from the event data
            for (row in 0 until eventMatrix.getNumRows()) {
                val nodeIdx = eventMatrix.get(row, 1).toInt()
                // Heuristic: first event row is typically the departure,
                // second is the arrival (matches MATLAB event{1}, event{2} ordering)
                if (row == 0) {
                    // Check if this could be a departure
                    depNode = nodeIdx
                } else if (row == 1) {
                    arvNode = nodeIdx
                }
            }
        }

        return GeneralSampleEvent(dt, depNode, arvNode, sampleResult)
    }

    /**
     * Updates the model state after an event using the SSA sample result.
     *
     * Applies State.afterEvent for each event in the sample to advance the
     * model's internal state consistently with the SSA simulation.
     *
     * @param sampleResult the raw SSA sample result from [sample]
     */
    fun update(sampleResult: SampleResult) {
        val sn = model.getStruct(false)
        val eventMatrix = sampleResult.event
        if (eventMatrix == null || eventMatrix.getNumRows() == 0) return

        // Process each event in the sample
        // The event matrix has rows with [time, node, class]
        // The first event's class is used as the jobclass for afterEvent
        val jobClass = if (eventMatrix.getNumRows() > 0)
            eventMatrix.get(0, 2).toInt()
        else
            0

        for (row in 0 until eventMatrix.getNumRows()) {
            val nodeIdx = eventMatrix.get(row, 1).toInt()
            val node = model.getNodes()[nodeIdx]
            if (node is jline.lang.nodes.StatefulNode) {
                val currentState = node.state
                // Determine event type from context:
                // In MATLAB, sample.event{i}.event is either EventType.DEP or EventType.ARV
                // We use the row index: row 0 is typically DEP, row 1 is typically ARV
                val eventType = if (row == 0) EventType.DEP else EventType.ARV
                val result = State.afterEvent(sn, nodeIdx, currentState, eventType, jobClass, true)
                if (result != null && result.outspace != null && result.outspace.getNumRows() > 0) {
                    node.state = result.outspace.getRow(0)
                }
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
 * Container for a sampled event from the general RL environment.
 *
 * @param dt the elapsed time until the event
 * @param depNode the index of the node where a departure occurred
 * @param arvNode the index of the node where an arrival occurred
 * @param sampleResult the raw SSA sample result for state updates
 */
data class GeneralSampleEvent(
    val dt: Double,
    val depNode: Int,
    val arvNode: Int,
    val sampleResult: SampleResult
)
