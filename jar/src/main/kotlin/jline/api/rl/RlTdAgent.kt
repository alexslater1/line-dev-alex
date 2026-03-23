/**
 * @file Temporal Difference Learning Agent for Queueing Routing
 *
 * Implements a Temporal Difference (TD) learning agent that learns optimal routing
 * policies for queueing networks. The agent uses average-reward TD(0) learning
 * to estimate a value function V(s) and derives an epsilon-greedy routing policy.
 *
 * The value function is stored as a flat array representing an N-dimensional
 * table indexed by queue lengths (one dimension per queue). The Q-function is
 * stored similarly with an extra dimension for actions.
 *
 * Port of: matlab/src/api/rl/rl_td_agent.m
 *
 * @since LINE 3.0
 */
package jline.api.rl

import jline.GlobalConstants
import java.util.Random

/**
 * TD learning agent for queueing network routing decisions.
 *
 * This agent learns an optimal routing policy using average-reward TD(0) learning.
 * When a new job arrives (departure from source), the agent selects a queue using
 * an epsilon-greedy policy derived from the value function. When a job departs
 * from a queue, the queue length is decremented.
 *
 * The value function is normalized after each update so that V(0,...,0) = 0,
 * ensuring the differential value function interpretation.
 *
 * @param lr learning rate for value function updates
 * @param epsilon initial exploration rate for the epsilon-greedy policy (0 to 1)
 * @param epsDecay decay factor applied to epsilon each episode
 */
class RlTdAgent(
    val lr: Double = 0.05,
    var epsilon: Double = 1.0,
    val epsDecay: Double = 0.99
) {
    /** Value function stored as a flat array (N-dimensional table). */
    var v: DoubleArray = doubleArrayOf(0.0)

    /** Q-function stored as a flat array (N+1 dimensional table). */
    var q: DoubleArray = doubleArrayOf(0.0)

    /** Shape of the value function array (one entry per dimension). */
    var vSize: IntArray = intArrayOf(0)

    /** Shape of the Q-function array (one entry per dimension, last is actionSize). */
    var qSize: IntArray = intArrayOf(0)

    private val random = Random()

    /**
     * Resets the agent and environment to their initial states.
     *
     * Clears the value function and Q-function, then resets the environment.
     *
     * @param env the RL environment
     */
    fun reset(env: RlEnv) {
        v = doubleArrayOf(0.0)
        q = doubleArrayOf(0.0)
        vSize = intArrayOf(0)
        qSize = intArrayOf(0)
        env.reset()
    }

    /**
     * Returns the learned value function.
     *
     * @return the value function as a flat array
     */
    fun getValueFunction(): DoubleArray {
        return v.copyOf()
    }

    /**
     * Returns the learned Q-function.
     *
     * @return the Q-function as a flat array
     */
    fun getQFunction(): DoubleArray {
        return q.copyOf()
    }

    /**
     * Trains the agent using average-reward TD(0) learning.
     *
     * Runs the TD learning algorithm for 10,000 episodes (matching MATLAB default).
     * In each episode:
     * 1. An event is sampled from the environment
     * 2. If a new job arrives (source departure), the agent selects a queue
     *    using epsilon-greedy policy (or JSQ if outside action space)
     * 3. If a job completes (queue departure), the queue length is decremented
     * 4. If the state is valid, the value function is updated using TD(0) rule
     *
     * The average cost rate is estimated using exponentially weighted sums
     * of costs and times.
     *
     * @param env the RL environment to train on
     */
    fun solve(env: RlEnv) {
        reset(env)

        val actionSize = env.actionSize
        val stateSizePlusPad = env.stateSize + 5

        // Initialize value function: N-dimensional array with shape [stateSizePlusPad, ..., stateSizePlusPad]
        // where N = actionSize
        vSize = IntArray(actionSize) { stateSizePlusPad }
        var totalVSize = 1
        for (dim in vSize) {
            totalVSize *= dim
        }
        v = DoubleArray(totalVSize) // zeros

        // Initialize Q-function: (N+1)-dimensional array with extra dimension for actions
        qSize = IntArray(actionSize + 1)
        for (i in 0 until actionSize) {
            qSize[i] = stateSizePlusPad
        }
        qSize[actionSize] = actionSize
        var totalQSize = 1
        for (dim in qSize) {
            totalQSize *= dim
        }
        q = DoubleArray(totalQSize) { random.nextDouble() } // rand

        // State tracking
        val x = IntArray(actionSize)       // current state (queue lengths)
        var n = IntArray(actionSize)       // previous state
        var t = 0.0                        // time of current event
        var c = 0.0                        // incurred costs between visits
        var bigT = 0.0                     // total discounted elapsed time
        var bigC = 0.0                     // total discounted costs

        val numEpisodes = 10000
        var eps = epsilon
        var j = 0

        while (j < numEpisodes) {
            if (j % 1000 == 0) {
                System.out.printf("[rl_td_agent] running episode #%d.%n", j)
            }

            eps *= epsDecay

            // Sample next event
            val sampleEvent = env.sample()
            val dt = sampleEvent.t
            val depNode = sampleEvent.depNode
            t += dt

            // Accumulate holding cost: sum of all queue lengths * dt
            var sumX = 0
            for (xi in x) sumX += xi
            c += sumX * dt

            if (contains(env.idxOfSourceInNodes, depNode)) {
                // New job arrived (departure from source)
                if (env.isInActionSpace(env.model.getNodes())) {
                    // Create epsilon-greedy policy
                    // Compute next_locs: for each action a, the state would be x+1 with x[a] incremented by 1
                    // In MATLAB: next_locs = zeros(actionSize, actionSize) + x + 1 + eye(actionSize)
                    val nextValues = DoubleArray(actionSize)
                    for (a in 0 until actionSize) {
                        val nextLoc = IntArray(actionSize)
                        for (k in 0 until actionSize) {
                            nextLoc[k] = x[k] + 1  // +1 for 1-based indexing
                        }
                        nextLoc[a] = nextLoc[a] + 1  // +eye: increment the action dimension
                        val idx = getStateFromLoc(vSize, nextLoc)
                        nextValues[a] = if (idx >= 0 && idx < v.size) v[idx] else 0.0
                    }

                    val policy = createGreedyPolicy(nextValues, eps, actionSize)

                    // Sample action from policy using cumulative sum method
                    val r = random.nextDouble()
                    var cumSum = 0.0
                    var action = actionSize - 1 // default to last action
                    for (a in 0 until actionSize) {
                        cumSum += policy[a]
                        if (r < cumSum) {
                            action = a
                            break
                        }
                    }

                    x[action] = x[action] + 1
                    env.update(x)
                } else {
                    // Outside action space - use Join Shortest Queue (JSQ)
                    var minVal = x[0]
                    val minIndices = ArrayList<Int>()
                    minIndices.add(0)
                    for (k in 1 until actionSize) {
                        if (x[k] < minVal) {
                            minVal = x[k]
                            minIndices.clear()
                            minIndices.add(k)
                        } else if (x[k] == minVal) {
                            minIndices.add(k)
                        }
                    }
                    val action = if (minIndices.size > 1)
                        minIndices[random.nextInt(minIndices.size)]
                    else
                        minIndices[0]

                    x[action] = x[action] + 1
                    env.update(x)
                }
            } else if (contains(env.idxOfQueueInNodes, depNode)) {
                // Departure from a queue
                val queueIdx = indexOf(env.idxOfQueueInNodes, depNode)
                if (queueIdx >= 0) {
                    x[queueIdx] = maxOf(0, x[queueIdx] - 1)
                }
                env.update(x)
            }

            // TD update when in valid state space
            if (env.isInStateSpace(env.model.getNodes())) {
                j++
                bigT = env.gamma * bigT + t
                bigC = env.gamma * bigC + c
                val meanCostRate = if (bigT > 0) bigC / bigT else 0.0

                // Convert to 1-based location indices
                val prevLoc = IntArray(actionSize) { n[it] + 1 }
                val curLoc = IntArray(actionSize) { x[it] + 1 }

                val prevState = getStateFromLoc(vSize, prevLoc)
                val curState = getStateFromLoc(vSize, curLoc)

                if (prevState >= 0 && prevState < v.size && curState >= 0 && curState < v.size) {
                    // TD(0) update: V(n) = (1-lr)*V(n) + lr*(c - t*meanCostRate + V(x))
                    v[prevState] = (1 - lr) * v[prevState] + lr * (c - t * meanCostRate + v[curState])

                    // Normalize: V = V - V(1) (subtract value at first element)
                    val v0 = v[0]
                    for (i in v.indices) {
                        v[i] -= v0
                    }
                }

                t = 0.0
                c = 0.0
                n = x.copyOf()
            }
        }
    }

    companion object {
        /**
         * Creates an epsilon-greedy policy from state-action values.
         *
         * Each action gets a base probability of epsilon/nA. The remaining
         * probability mass (1-epsilon) is distributed equally among all actions
         * whose value is within FineTol of the minimum value (cost minimization).
         *
         * @param stateQ array of state-action values (one per action)
         * @param epsilon exploration probability
         * @param nA number of actions
         * @return probability distribution over actions
         */
        @JvmStatic
        fun createGreedyPolicy(stateQ: DoubleArray, epsilon: Double, nA: Int): DoubleArray {
            val policy = DoubleArray(nA) { epsilon / nA }

            // Find minimum value
            var minVal = Double.MAX_VALUE
            for (qVal in stateQ) {
                if (qVal < minVal) minVal = qVal
            }

            // Find all actions within FineTol of the minimum
            val argmin = ArrayList<Int>()
            for (i in stateQ.indices) {
                if (stateQ[i] - minVal < GlobalConstants.FineTol) {
                    argmin.add(i)
                }
            }

            // Add exploitation probability to best actions
            val exploitProb = (1 - epsilon) / argmin.size
            for (idx in argmin) {
                policy[idx] += exploitProb
            }

            return policy
        }

        /**
         * Converts a multi-dimensional location to a linear index (column-major order).
         *
         * This mirrors MATLAB's column-major (Fortran) linear indexing:
         * index = loc[0] + (loc[1]-1)*size[0] + (loc[2]-1)*size[0]*size[1] + ...
         *
         * Note: locations are 1-based (as in MATLAB), converted to 0-based internally.
         *
         * @param objSize shape of the array (size of each dimension)
         * @param loc multi-dimensional location (1-based indices)
         * @return linear index (0-based) into the flat array
         */
        @JvmStatic
        fun getStateFromLoc(objSize: IntArray, loc: IntArray): Int {
            if (objSize.size != loc.size) return 0

            // MATLAB: s = loc(1) for i=1, s = s + (loc(i)-1)*prod(objSize(1:i-1)) for i>1
            // Convert to 0-based: index = (loc[0]-1) + (loc[1]-1)*objSize[0] + ...
            var s = 0
            var stride = 1
            for (i in objSize.indices) {
                val locVal = loc[i] - 1 // convert from 1-based to 0-based
                if (locVal < 0 || locVal >= objSize[i]) {
                    // Clamp to valid range
                    val clamped = maxOf(0, minOf(locVal, objSize[i] - 1))
                    s += clamped * stride
                } else {
                    s += locVal * stride
                }
                stride *= objSize[i]
            }
            return s
        }

        /**
         * Converts multiple multi-dimensional locations to linear indices.
         *
         * @param objSize shape of the array
         * @param locs array of locations (each row is one multi-dimensional location)
         * @return array of linear indices
         */
        @JvmStatic
        fun getStateFromLocs(objSize: IntArray, locs: Array<IntArray>): IntArray {
            val result = IntArray(locs.size)
            for (i in locs.indices) {
                result[i] = getStateFromLoc(objSize, locs[i])
            }
            return result
        }

        /**
         * Checks if an integer array contains a given value.
         */
        private fun contains(arr: IntArray, value: Int): Boolean {
            for (v in arr) {
                if (v == value) return true
            }
            return false
        }

        /**
         * Finds the index of a value in an integer array.
         *
         * @return the index, or -1 if not found
         */
        private fun indexOf(arr: IntArray, value: Int): Int {
            for (i in arr.indices) {
                if (arr[i] == value) return i
            }
            return -1
        }
    }
}
