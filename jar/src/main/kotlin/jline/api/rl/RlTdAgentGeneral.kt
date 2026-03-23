/**
 * @file General Temporal Difference Learning Agent for Queueing Control
 *
 * Implements a general-purpose TD learning agent for queueing network control.
 * Supports three solving modes:
 * 1. TD learning with a fixed (existing) routing policy (value evaluation)
 * 2. TD control with tabular value function (policy optimization)
 * 3. TD control with HashMap-based value function (for sparse state spaces)
 *
 * Additionally provides linear and quadratic value function approximation
 * via regression on the HashMap-collected data.
 *
 * Port of: matlab/src/api/rl/rl_td_agent_general.m
 *
 * @since LINE 3.0
 */
package jline.api.rl

import jline.util.matrix.Matrix
import java.util.ArrayList
import java.util.HashMap
import java.util.Random

/**
 * General TD learning agent for queueing network control.
 *
 * This agent operates with [RlEnvGeneral] environments and supports:
 * - Value function evaluation for a fixed policy ([solveForFixedPolicy])
 * - Policy optimization using tabular TD control ([solve])
 * - Sparse state space exploration using HashMap-based value functions ([solveByHashmap])
 * - Linear and quadratic value function approximation ([solveByLinear], [solveByQuad])
 *
 * The agent uses average-reward TD(0) updates where the cost at each step
 * is the total number of jobs in the system multiplied by the elapsed time.
 *
 * @param lr learning rate for value function updates
 * @param epsilon initial exploration rate (0 to 1)
 * @param epsDecay decay factor applied to epsilon each episode
 */
class RlTdAgentGeneral(
    val lr: Double = 0.1,
    var epsilon: Double = 1.0,
    val epsDecay: Double = 0.9999
) {
    /** Value function stored as a flat array (N-dimensional table). */
    var v: DoubleArray = doubleArrayOf(0.0)

    /** Shape of the value function array. */
    var vSize: IntArray = intArrayOf(0)

    private val random = Random()

    /**
     * Resets the agent and environment.
     *
     * @param env the general RL environment
     */
    fun reset(env: RlEnvGeneral) {
        v = doubleArrayOf(0.0)
        vSize = intArrayOf(0)
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

    // ============================================================================
    // TD Learning for Value Function with Fixed (Heuristic) Routing Policy
    // ============================================================================

    /**
     * Evaluates the value function for the current (fixed) routing policy.
     *
     * This method runs TD(0) learning without modifying routing decisions.
     * Events are sampled from the environment and the model's existing routing
     * is used. The value function V(s) is updated to reflect the average cost
     * under the current policy.
     *
     * This is useful for evaluating heuristic policies (e.g., JSQ, round-robin)
     * before attempting policy improvement.
     *
     * @param env the general RL environment
     * @param numEpisodes number of episodes to run (typically 10^4)
     * @return the learned value function as a flat array
     */
    fun solveForFixedPolicy(env: RlEnvGeneral, numEpisodes: Int): DoubleArray {
        reset(env)

        val nqueues = env.nqueues

        // Initialize value function: N-dimensional array with shape [stateSize+1, ..., stateSize+1]
        val dimSize = env.stateSize + 1
        vSize = IntArray(nqueues) { dimSize }
        var totalSize = 1
        for (dim in vSize) totalSize *= dim
        v = DoubleArray(totalSize) // zeros

        var t = 0.0             // time of current event
        var c = 0.0             // incurred costs between visits
        var bigT = 0.0          // total discounted elapsed time
        var bigC = 0.0          // total discounted costs
        val x = IntArray(nqueues)    // current state (queue lengths)
        var n = IntArray(nqueues)    // previous state

        var j = 0
        while (j < numEpisodes) {
            if (j % 1000 == 0) {
                System.out.printf("running episode #%d%n", j)
            }

            val sampleEvent = env.sample()
            val dt = sampleEvent.dt
            val depNode = sampleEvent.depNode
            val arvNode = sampleEvent.arvNode
            val sampleResult = sampleEvent.sampleResult
            t += dt

            var sumX = 0
            for (xi in x) sumX += xi
            c += sumX * dt

            // Event involves departure from server i
            if (contains(env.idxOfQueueInNodes, depNode)) {
                val depServer = indexOf(env.idxOfQueueInNodes, depNode)
                if (depServer >= 0) {
                    x[depServer] = x[depServer] - 1
                }
            }

            // Event involves arrival at server j
            if (contains(env.idxOfQueueInNodes, arvNode)) {
                val arvServer = indexOf(env.idxOfQueueInNodes, arvNode)
                if (arvServer >= 0) {
                    x[arvServer] = x[arvServer] + 1
                }
            }

            env.update(sampleResult)

            if (env.isInStateSpace(x)) {
                j++
                bigT = env.gamma * bigT + t
                bigC = env.gamma * bigC + c
                val meanCostRate = if (bigT > 0) bigC / bigT else 0.0

                // Convert to 1-based indices for lookup
                val prevLoc = IntArray(nqueues) { n[it] + 1 }
                val curLoc = IntArray(nqueues) { x[it] + 1 }

                val prevState = RlTdAgent.getStateFromLoc(vSize, prevLoc)
                val curState = RlTdAgent.getStateFromLoc(vSize, curLoc)

                if (prevState >= 0 && prevState < v.size && curState >= 0 && curState < v.size) {
                    v[prevState] = (1 - lr) * v[prevState] + lr * (c - t * meanCostRate + v[curState])

                    // Normalize: V = V - V(1)
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

        return v.copyOf()
    }

    // ============================================================================
    // TD Control with Tabular Value Function
    // ============================================================================

    /**
     * Learns an optimal routing policy using tabular TD control.
     *
     * In each episode, the agent:
     * 1. Samples an event from the environment
     * 2. Processes departures from queue nodes
     * 3. If the departure is from an action node and the state is in the action
     *    space, selects a routing action using epsilon-greedy policy based on
     *    the value of successor states
     * 4. Processes arrivals at queue nodes
     * 5. Updates the value function using average-reward TD(0)
     *
     * The epsilon parameter decays by [epsDecay] each episode for gradual
     * exploitation.
     *
     * @param env the general RL environment
     * @param numEpisodes number of episodes to run (typically 10^4)
     * @return the learned value function as a flat array
     */
    fun solve(env: RlEnvGeneral, numEpisodes: Int): DoubleArray {
        reset(env)

        val nqueues = env.nqueues
        val dimSize = env.stateSize + 1
        vSize = IntArray(nqueues) { dimSize }
        var totalSize = 1
        for (dim in vSize) totalSize *= dim
        v = DoubleArray(totalSize) // zeros

        var t = 0.0
        var c = 0.0
        var bigT = 0.0
        var bigC = 0.0
        val x = IntArray(nqueues)
        var n = IntArray(nqueues)

        var eps = epsilon

        var j = 0
        while (j < numEpisodes) {
            if (j % 1000 == 0) {
                System.out.printf("running episode #%d.%n", j)
            }

            eps *= epsDecay

            val sampleEvent = env.sample()
            val dt = sampleEvent.dt
            var depNode = sampleEvent.depNode
            var arvNode = sampleEvent.arvNode
            val sampleResult = sampleEvent.sampleResult
            t += dt

            var sumX = 0
            for (xi in x) sumX += xi
            c += sumX * dt

            // Event involves departure from server i
            if (contains(env.idxOfQueueInNodes, depNode)) {
                val depServer = indexOf(env.idxOfQueueInNodes, depNode)
                if (depServer >= 0) {
                    x[depServer] = maxOf(0, x[depServer] - 1)
                }
            }

            // Actions wanted at depNode, and state is in action space
            if (contains(env.idxOfActionNodes, depNode) && env.isInActionSpace(x)) {
                val actions = env.actionSpace[depNode]
                if (actions != null && actions.isNotEmpty()) {
                    // Create epsilon-greedy policy based on successor state values
                    val nextValues = genNextValues(env, x, actions)
                    val policy = RlTdAgent.createGreedyPolicy(nextValues, eps, actions.size)

                    // Sample action from policy
                    val r = random.nextDouble()
                    var cumSum = 0.0
                    var selectedIdx = actions.size - 1
                    for (a in policy.indices) {
                        cumSum += policy[a]
                        if (r < cumSum) {
                            selectedIdx = a
                            break
                        }
                    }
                    arvNode = actions[selectedIdx]

                    // Update the sample's arrival event node to match the chosen action
                    // In MATLAB, this modifies sample.event{i}.node for ARV events
                    // Since we use the SampleResult's event matrix, we update it
                    val eventMatrix = sampleResult.event
                    if (eventMatrix != null && eventMatrix.getNumRows() > 1) {
                        // The arrival event is typically the second row
                        for (row in 0 until eventMatrix.getNumRows()) {
                            // Check if this row corresponds to an arrival
                            // (second event in the pair)
                            if (row == 1 || (row > 0 && eventMatrix.getNumCols() > 1)) {
                                eventMatrix.set(row, 1, arvNode.toDouble())
                                break
                            }
                        }
                    }
                }
            }

            // Update current state: arrival at arvNode queue
            if (contains(env.idxOfQueueInNodes, arvNode)) {
                val arvServer = indexOf(env.idxOfQueueInNodes, arvNode)
                if (arvServer >= 0) {
                    x[arvServer] = x[arvServer] + 1
                }
            }
            env.update(sampleResult)

            // In state space: update value function
            if (env.isInStateSpace(x)) {
                j++
                bigT = env.gamma * bigT + t
                bigC = env.gamma * bigC + c
                val meanCostRate = if (bigT > 0) bigC / bigT else 0.0

                val prevLoc = IntArray(nqueues) { n[it] + 1 }
                val curLoc = IntArray(nqueues) { x[it] + 1 }

                val prevState = RlTdAgent.getStateFromLoc(vSize, prevLoc)
                val curState = RlTdAgent.getStateFromLoc(vSize, curLoc)

                if (prevState >= 0 && prevState < v.size && curState >= 0 && curState < v.size) {
                    v[prevState] = (1 - lr) * v[prevState] + lr * (c - t * meanCostRate + v[curState])

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

        return v.copyOf()
    }

    // ============================================================================
    // TD Control with HashMap Value Function (Sparse State Space)
    // ============================================================================

    /**
     * Result of the HashMap-based TD control solve.
     *
     * @param X feature matrix where each row is [1, q1, q2, ..., qn] (intercept + queue lengths)
     * @param Y value function values for each visited state
     */
    data class HashmapResult(val X: Matrix, val Y: Matrix)

    /**
     * Learns a routing policy using a HashMap-based sparse value function.
     *
     * Instead of allocating a full N-dimensional table, this method stores value
     * function entries only for states actually visited during learning. States
     * not in the map use an "external" default value.
     *
     * This is efficient for large state spaces where only a fraction of states
     * are reachable.
     *
     * @param env the general RL environment
     * @param numEpisodes number of episodes to run
     * @return [HashmapResult] containing the feature matrix X and value vector Y
     */
    fun solveByHashmap(env: RlEnvGeneral, numEpisodes: Int): HashmapResult {
        reset(env)

        val nqueues = env.nqueues

        // HashMap-based value function
        val pointValues = HashMap<String, Double>()
        pointValues[intArrayToString(IntArray(nqueues))] = 0.0
        pointValues["external"] = 0.0

        var t = 0.0
        var c = 0.0
        var bigT = 0.0
        var bigC = 0.0
        val x = IntArray(nqueues)
        var n = IntArray(nqueues)

        var eps = epsilon

        var j = 0
        while (j < numEpisodes) {
            if (j % 1000 == 0) {
                System.out.printf("running episode #%d.%n", j)
            }

            eps *= epsDecay

            val sampleEvent = env.sample()
            val dt = sampleEvent.dt
            var depNode = sampleEvent.depNode
            var arvNode = sampleEvent.arvNode
            val sampleResult = sampleEvent.sampleResult
            t += dt

            var sumX = 0
            for (xi in x) sumX += xi
            c += sumX * dt

            // Event involves departure from server i
            if (contains(env.idxOfQueueInNodes, depNode)) {
                val depServer = indexOf(env.idxOfQueueInNodes, depNode)
                if (depServer >= 0) {
                    x[depServer] = maxOf(0, x[depServer] - 1)
                }
            }

            // Actions wanted at depNode, and state is in action space
            if (contains(env.idxOfActionNodes, depNode) && env.isInActionSpace(x)) {
                val actions = env.actionSpace[depNode]
                if (actions != null && actions.isNotEmpty()) {
                    // Compute next-state values from the hashmap
                    val nextPointValues = DoubleArray(actions.size)
                    for (actI in actions.indices) {
                        val qIdx = indexOf(env.idxOfQueueInNodes, actions[actI])
                        val tmpNextState = x.copyOf()
                        if (qIdx >= 0) {
                            tmpNextState[qIdx] = tmpNextState[qIdx] + 1
                        }
                        val key = intArrayToString(tmpNextState)
                        nextPointValues[actI] = if (pointValues.containsKey(key))
                            pointValues[key]!!
                        else
                            pointValues["external"]!!
                    }
                    val policy = RlTdAgent.createGreedyPolicy(nextPointValues, eps, actions.size)

                    // Sample action
                    val r = random.nextDouble()
                    var cumSum = 0.0
                    var selectedIdx = actions.size - 1
                    for (a in policy.indices) {
                        cumSum += policy[a]
                        if (r < cumSum) {
                            selectedIdx = a
                            break
                        }
                    }
                    arvNode = actions[selectedIdx]

                    // Update sample's arrival node
                    val eventMatrix = sampleResult.event
                    if (eventMatrix != null && eventMatrix.getNumRows() > 1) {
                        for (row in 0 until eventMatrix.getNumRows()) {
                            if (row == 1 || (row > 0 && eventMatrix.getNumCols() > 1)) {
                                eventMatrix.set(row, 1, arvNode.toDouble())
                                break
                            }
                        }
                    }
                }
            }

            // Update current state
            if (contains(env.idxOfQueueInNodes, arvNode)) {
                val arvServer = indexOf(env.idxOfQueueInNodes, arvNode)
                if (arvServer >= 0) {
                    x[arvServer] = x[arvServer] + 1
                }
            }
            env.update(sampleResult)

            // In state space: update value function
            if (env.isInStateSpace(x)) {
                j++
                bigT = env.gamma * bigT + t
                bigC = env.gamma * bigC + c
                val meanCostRate = if (bigT > 0) bigC / bigT else 0.0

                val nKey = intArrayToString(n)
                val xKey = intArrayToString(x)

                if (!pointValues.containsKey(nKey)) {
                    pointValues[nKey] = pointValues["external"]!!
                }

                val curVal = if (pointValues.containsKey(xKey))
                    pointValues[xKey]!!
                else
                    pointValues["external"]!!

                pointValues[nKey] = (1 - lr) * pointValues[nKey]!! + lr * (c - t * meanCostRate + curVal)

                // Normalize when previous state is zero
                var allZero = true
                for (ni in n) {
                    if (ni != 0) {
                        allZero = false
                        break
                    }
                }
                if (allZero) {
                    val subtractor = pointValues[nKey]!!
                    for (key in ArrayList(pointValues.keys)) {
                        pointValues[key] = pointValues[key]!! - subtractor
                    }
                }

                t = 0.0
                c = 0.0
                n = x.copyOf()
            }
        }

        // Remove the "external" entry
        pointValues.remove("external")

        // Build output matrices X and Y
        val count = pointValues.size
        val resultX = Matrix(count, 1 + nqueues)
        val resultY = Matrix(count, 1)
        var iterator = 0
        for ((key, value) in pointValues) {
            resultX.set(iterator, 0, 1.0) // intercept
            val stateVals = stringToIntArray(key)
            for (k in stateVals.indices) {
                resultX.set(iterator, 1 + k, stateVals[k].toDouble())
            }
            resultY.set(iterator, 0, value)
            iterator++
        }

        return HashmapResult(resultX, resultY)
    }

    /**
     * Result of value function approximation.
     *
     * @param X feature matrix
     * @param Y value function values
     * @param coefficients regression coefficients
     */
    data class ApproximationResult(val X: Matrix, val Y: Matrix, val coefficients: Matrix)

    /**
     * Learns a routing policy and fits a linear value function approximator.
     *
     * Runs HashMap-based TD control, then fits a linear model:
     * V(q1, q2, ..., qn) = w0 + w1*q1 + w2*q2 + ... + wn*qn
     *
     * The regression is performed using ordinary least squares (OLS):
     * coefficients = (X^T X)^{-1} X^T Y
     *
     * @param env the general RL environment
     * @param numEpisodes number of episodes to run
     * @return [ApproximationResult] with feature matrix, values, and regression coefficients
     */
    fun solveByLinear(env: RlEnvGeneral, numEpisodes: Int): ApproximationResult {
        val hashmapResult = solveByHashmap(env, numEpisodes)
        val resultX = hashmapResult.X
        val resultY = hashmapResult.Y

        // Linear regression: coefficients = (X^T * X)^{-1} * X^T * Y
        val xt = resultX.transpose()
        val xtx = xt.mult(resultX)
        val xty = xt.mult(resultY)

        val coeff = Matrix.createLike(xty)
        Matrix.solve(xtx, xty, coeff)

        return ApproximationResult(resultX, resultY, coeff)
    }

    /**
     * Learns a routing policy and fits a quadratic value function approximator.
     *
     * Runs HashMap-based TD control, then fits a quadratic model:
     * V(q1, ..., qn) = sum_{i,j} w_{ij} * q_i * q_j + linear terms + intercept
     *
     * The feature matrix is augmented with all pairwise products of the original
     * features (including self-products q_i^2).
     *
     * @param env the general RL environment
     * @param numEpisodes number of episodes to run
     * @return [ApproximationResult] with augmented feature matrix, values, and regression coefficients
     */
    fun solveByQuad(env: RlEnvGeneral, numEpisodes: Int): ApproximationResult {
        val hashmapResult = solveByHashmap(env, numEpisodes)
        val baseX = hashmapResult.X
        val resultY = hashmapResult.Y

        val numRows = baseX.getNumRows()
        val baseCols = baseX.getNumCols()

        // Count additional quadratic columns: for i=1..baseCols-1, j=i..baseCols-1
        // (skipping column 0 which is the intercept)
        var quadCols = 0
        for (i in 1 until baseCols) {
            for (j2 in i until baseCols) {
                quadCols++
            }
        }

        val totalCols = baseCols + quadCols
        val augX = Matrix(numRows, totalCols)

        // Copy original columns
        for (row in 0 until numRows) {
            for (col in 0 until baseCols) {
                augX.set(row, col, baseX.get(row, col))
            }
        }

        // Add quadratic interaction columns
        var colIdx = baseCols
        for (i in 1 until baseCols) {
            for (j2 in i until baseCols) {
                for (row in 0 until numRows) {
                    augX.set(row, colIdx, baseX.get(row, i) * baseX.get(row, j2))
                }
                colIdx++
            }
        }

        // Quadratic regression: coefficients = (X^T * X)^{-1} * X^T * Y
        val xt = augX.transpose()
        val xtx = xt.mult(augX)
        val xty = xt.mult(resultY)

        val coeff = Matrix.createLike(xty)
        Matrix.solve(xtx, xty, coeff)

        return ApproximationResult(augX, resultY, coeff)
    }

    // ============================================================================
    // Helper methods
    // ============================================================================

    /**
     * Computes successor state values for each possible action.
     *
     * For each action (downstream node), computes the value of the state that
     * would result from routing a job to that node's corresponding queue.
     *
     * @param env the environment
     * @param curState current queue lengths
     * @param actions array of possible action node indices
     * @return array of value function values for each action's successor state
     */
    private fun genNextValues(env: RlEnvGeneral, curState: IntArray, actions: IntArray): DoubleArray {
        val values = DoubleArray(actions.size)
        for (actI in actions.indices) {
            val qIdx = indexOf(env.idxOfQueueInNodes, actions[actI])
            val tmpLoc = IntArray(curState.size) { curState[it] + 1 } // 1-based
            if (qIdx >= 0) {
                tmpLoc[qIdx] = tmpLoc[qIdx] + 1
            }
            val stateIdx = RlTdAgent.getStateFromLoc(vSize, tmpLoc)
            values[actI] = if (stateIdx >= 0 && stateIdx < v.size) v[stateIdx] else 0.0
        }
        return values
    }

    companion object {
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

        /**
         * Converts an integer array to a string key for HashMap storage.
         *
         * Produces a space-separated string matching MATLAB's num2str format
         * for consistent key generation.
         *
         * @param arr the integer array
         * @return string representation
         */
        private fun intArrayToString(arr: IntArray): String {
            val sb = StringBuilder()
            for (i in arr.indices) {
                if (i > 0) sb.append(' ')
                sb.append(arr[i])
            }
            return sb.toString()
        }

        /**
         * Converts a string key back to an integer array.
         *
         * @param s the space-separated string
         * @return the integer array
         */
        private fun stringToIntArray(s: String): IntArray {
            val parts = s.trim().split("\\s+".toRegex())
            val result = IntArray(parts.size)
            for (i in parts.indices) {
                result[i] = parts[i].toInt()
            }
            return result
        }
    }
}
