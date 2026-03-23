
"""
Reinforcement Learning (RL) functions for queueing network routing control.

This module provides RL environments and TD learning agents for optimizing
routing decisions in queueing networks.

Components:
- RlEnv: Simple RL environment for source-to-queue routing
- RlEnvGeneral: General RL environment with configurable action nodes
- RlTdAgent: TD(0) learning agent with epsilon-greedy policy
- RlTdAgentGeneral: General TD agent with 5 solving modes
  (fixed policy, tabular, hashmap, linear, quadratic)

References:
    Casale, "Accelerating Performance Inference over Closed Systems by
    Asymptotic Methods", ACM SIGMETRICS, 2017.
"""

import jpype
import numpy as np
from line_solver import jlineMatrixToArray


# ===========================================================================
# RL Environments
# ===========================================================================

class RlEnv:
    """
    RL environment for simple source-to-queue routing decisions.

    Wraps a queueing network model providing an interface for RL agents
    to interact via sampling events and updating state.

    The state is defined by queue lengths at each queue node. The action
    space corresponds to routing decisions (one per queue).

    Args:
        model: LINE Network model (Python wrapper).
        idx_of_queue_in_nodes: Indices of queue nodes in model.getNodes().
        idx_of_source_in_nodes: Indices of source nodes in model.getNodes().
        state_size: Maximum number of jobs per queue to consider.
        gamma: Discount factor for future rewards (0 < gamma <= 1).
    """

    def __init__(self, model, idx_of_queue_in_nodes, idx_of_source_in_nodes,
                 state_size, gamma):
        java_model = model.obj if hasattr(model, 'obj') else model
        queue_arr = jpype.JArray(jpype.JInt)(len(idx_of_queue_in_nodes))
        for i, v in enumerate(idx_of_queue_in_nodes):
            queue_arr[i] = v
        source_arr = jpype.JArray(jpype.JInt)(len(idx_of_source_in_nodes))
        for i, v in enumerate(idx_of_source_in_nodes):
            source_arr[i] = v

        self._java_env = jpype.JPackage('jline').api.rl.RlEnv(
            java_model, queue_arr, source_arr,
            jpype.JInt(state_size), jpype.JDouble(gamma)
        )

    @property
    def action_size(self):
        """Number of possible actions (equal to the number of queues)."""
        return int(self._java_env.getActionSize())

    @property
    def state_size(self):
        """Maximum number of jobs per queue."""
        return int(self._java_env.getStateSize())

    @property
    def gamma(self):
        """Discount factor."""
        return float(self._java_env.getGamma())

    def is_in_state_space(self):
        """Check if the current model state is within the defined state space."""
        nodes = self._java_env.getModel().getNodes()
        return bool(self._java_env.isInStateSpace(nodes))

    def is_in_action_space(self):
        """Check if actions can be taken from the current state."""
        nodes = self._java_env.getModel().getNodes()
        return bool(self._java_env.isInActionSpace(nodes))

    def sample(self):
        """
        Sample the next event from the environment using the SSA solver.

        Returns:
            dict: {'t': elapsed_time, 'dep_node': departure_node_index}
        """
        result = self._java_env.sample()
        return {
            't': float(result.getT()),
            'dep_node': int(result.getDepNode()),
        }

    def update(self, new_state):
        """
        Update the model state after an event.

        Args:
            new_state: Array of new queue lengths, one per queue.
        """
        state_arr = jpype.JArray(jpype.JInt)(len(new_state))
        for i, v in enumerate(new_state):
            state_arr[i] = int(v)
        self._java_env.update(state_arr)

    def reset(self):
        """Reset the environment to its initial state."""
        self._java_env.reset()

    @property
    def obj(self):
        """Access the underlying Java object."""
        return self._java_env


class RlEnvGeneral:
    """
    General RL environment for queueing network control decisions.

    Supports arbitrary dispatch/routing decisions at designated action nodes.
    The action space at each action node is determined by the network's
    connection matrix.

    Args:
        model: LINE Network model (Python wrapper).
        idx_of_queue_in_nodes: Indices of queue nodes in model.getNodes().
        idx_of_action_nodes: Indices of nodes where routing actions are needed.
        state_size: Maximum number of jobs per queue to consider.
        gamma: Discount factor for future rewards (0 < gamma <= 1).
    """

    def __init__(self, model, idx_of_queue_in_nodes, idx_of_action_nodes,
                 state_size, gamma):
        java_model = model.obj if hasattr(model, 'obj') else model
        queue_arr = jpype.JArray(jpype.JInt)(len(idx_of_queue_in_nodes))
        for i, v in enumerate(idx_of_queue_in_nodes):
            queue_arr[i] = v
        action_arr = jpype.JArray(jpype.JInt)(len(idx_of_action_nodes))
        for i, v in enumerate(idx_of_action_nodes):
            action_arr[i] = v

        self._java_env = jpype.JPackage('jline').api.rl.RlEnvGeneral(
            java_model, queue_arr, action_arr,
            jpype.JInt(state_size), jpype.JDouble(gamma)
        )

    @property
    def nqueues(self):
        """Number of queues in the network."""
        return int(self._java_env.getNqueues())

    @property
    def state_size(self):
        """Maximum number of jobs per queue."""
        return int(self._java_env.getStateSize())

    @property
    def gamma(self):
        """Discount factor."""
        return float(self._java_env.getGamma())

    @property
    def action_space(self):
        """
        Action space map: for each action node index, the array of
        downstream node indices reachable from that node.
        """
        java_map = self._java_env.getActionSpace()
        result = {}
        for entry in java_map.entrySet():
            key = int(entry.getKey())
            arr = entry.getValue()
            result[key] = [int(arr[i]) for i in range(len(arr))]
        return result

    def is_in_state_space(self, state):
        """
        Check if the given state vector is within the defined state space.

        Args:
            state: Array of queue lengths (one per queue).
        """
        state_arr = jpype.JArray(jpype.JInt)(len(state))
        for i, v in enumerate(state):
            state_arr[i] = int(v)
        return bool(self._java_env.isInStateSpace(state_arr))

    def is_in_action_space(self, state):
        """
        Check if actions can be taken from the given state.

        Args:
            state: Array of queue lengths (one per queue).
        """
        state_arr = jpype.JArray(jpype.JInt)(len(state))
        for i, v in enumerate(state):
            state_arr[i] = int(v)
        return bool(self._java_env.isInActionSpace(state_arr))

    def sample(self):
        """
        Sample the next event from the environment.

        Returns:
            dict: {'dt': elapsed_time, 'dep_node': departure_node_index,
                   'arv_node': arrival_node_index, 'sample_result': raw_result}
        """
        result = self._java_env.sample()
        return {
            'dt': float(result.getDt()),
            'dep_node': int(result.getDepNode()),
            'arv_node': int(result.getArvNode()),
            'sample_result': result.getSampleResult(),
        }

    def update(self, sample_result):
        """
        Update the model state using the SSA sample result.

        Args:
            sample_result: Raw SSA sample result from sample().
        """
        self._java_env.update(sample_result)

    def reset(self):
        """Reset the environment to its initial state."""
        self._java_env.reset()

    @property
    def obj(self):
        """Access the underlying Java object."""
        return self._java_env


# ===========================================================================
# TD Learning Agents
# ===========================================================================

class RlTdAgent:
    """
    TD(0) learning agent for queueing network routing decisions.

    Uses average-reward TD(0) learning with epsilon-greedy exploration.
    When a new job arrives, the agent selects a queue. When a job departs,
    the queue length is decremented.

    Args:
        lr: Learning rate for value function updates (default: 0.05).
        epsilon: Initial exploration rate (default: 1.0).
        eps_decay: Decay factor applied to epsilon each episode (default: 0.99).
    """

    def __init__(self, lr=0.05, epsilon=1.0, eps_decay=0.99):
        self._java_agent = jpype.JPackage('jline').api.rl.RlTdAgent(
            jpype.JDouble(lr), jpype.JDouble(epsilon), jpype.JDouble(eps_decay)
        )

    def solve(self, env):
        """
        Train the agent using average-reward TD(0) learning.

        Runs 10,000 episodes of TD learning on the environment.

        Args:
            env: RlEnv environment to train on.
        """
        self._java_agent.solve(env.obj)

    def reset(self, env):
        """Reset the agent and environment."""
        self._java_agent.reset(env.obj)

    def get_value_function(self):
        """
        Returns the learned value function.

        Returns:
            numpy.ndarray: Flat value function array.
        """
        return np.array(list(self._java_agent.getValueFunction()))

    def get_q_function(self):
        """
        Returns the learned Q-function.

        Returns:
            numpy.ndarray: Flat Q-function array.
        """
        return np.array(list(self._java_agent.getQFunction()))

    @staticmethod
    def create_greedy_policy(state_q, epsilon, n_actions):
        """
        Create an epsilon-greedy policy from state-action values.

        Args:
            state_q: Array of state-action values (one per action).
            epsilon: Exploration probability.
            n_actions: Number of actions.

        Returns:
            numpy.ndarray: Probability distribution over actions.
        """
        q_arr = jpype.JArray(jpype.JDouble)(len(state_q))
        for i, v in enumerate(state_q):
            q_arr[i] = v
        result = jpype.JPackage('jline').api.rl.RlTdAgent.createGreedyPolicy(
            q_arr, jpype.JDouble(epsilon), jpype.JInt(n_actions)
        )
        return np.array(list(result))

    @property
    def obj(self):
        """Access the underlying Java object."""
        return self._java_agent


class RlTdAgentGeneral:
    """
    General TD learning agent for queueing network control.

    Supports five solving modes:
    1. Fixed policy evaluation (solveForFixedPolicy)
    2. Tabular TD control (solve)
    3. HashMap-based sparse TD control (solveByHashmap)
    4. Linear value function approximation (solveByLinear)
    5. Quadratic value function approximation (solveByQuad)

    Args:
        lr: Learning rate (default: 0.1).
        epsilon: Initial exploration rate (default: 1.0).
        eps_decay: Decay factor for epsilon (default: 0.9999).
    """

    def __init__(self, lr=0.1, epsilon=1.0, eps_decay=0.9999):
        self._java_agent = jpype.JPackage('jline').api.rl.RlTdAgentGeneral(
            jpype.JDouble(lr), jpype.JDouble(epsilon), jpype.JDouble(eps_decay)
        )

    def solve_for_fixed_policy(self, env, num_episodes=10000):
        """
        Evaluate the value function for the current (fixed) routing policy.

        Args:
            env: RlEnvGeneral environment.
            num_episodes: Number of episodes to run.

        Returns:
            numpy.ndarray: Learned value function.
        """
        result = self._java_agent.solveForFixedPolicy(
            env.obj, jpype.JInt(num_episodes)
        )
        return np.array(list(result))

    def solve(self, env, num_episodes=10000):
        """
        Learn an optimal routing policy using tabular TD control.

        Args:
            env: RlEnvGeneral environment.
            num_episodes: Number of episodes to run.

        Returns:
            numpy.ndarray: Learned value function.
        """
        result = self._java_agent.solve(env.obj, jpype.JInt(num_episodes))
        return np.array(list(result))

    def solve_by_hashmap(self, env, num_episodes=10000):
        """
        Learn a routing policy using HashMap-based sparse value function.

        Args:
            env: RlEnvGeneral environment.
            num_episodes: Number of episodes to run.

        Returns:
            tuple: (X, Y) where X is the feature matrix and Y is the value vector.
        """
        result = self._java_agent.solveByHashmap(
            env.obj, jpype.JInt(num_episodes)
        )
        X = jlineMatrixToArray(result.getX())
        Y = jlineMatrixToArray(result.getY())
        return X, Y

    def solve_by_linear(self, env, num_episodes=10000):
        """
        Learn a policy and fit a linear value function approximator.

        Args:
            env: RlEnvGeneral environment.
            num_episodes: Number of episodes to run.

        Returns:
            tuple: (X, Y, coefficients) feature matrix, values, and regression coefficients.
        """
        result = self._java_agent.solveByLinear(
            env.obj, jpype.JInt(num_episodes)
        )
        X = jlineMatrixToArray(result.getX())
        Y = jlineMatrixToArray(result.getY())
        coeff = jlineMatrixToArray(result.getCoefficients())
        return X, Y, coeff

    def solve_by_quad(self, env, num_episodes=10000):
        """
        Learn a policy and fit a quadratic value function approximator.

        Args:
            env: RlEnvGeneral environment.
            num_episodes: Number of episodes to run.

        Returns:
            tuple: (X, Y, coefficients) augmented feature matrix, values, and coefficients.
        """
        result = self._java_agent.solveByQuad(
            env.obj, jpype.JInt(num_episodes)
        )
        X = jlineMatrixToArray(result.getX())
        Y = jlineMatrixToArray(result.getY())
        coeff = jlineMatrixToArray(result.getCoefficients())
        return X, Y, coeff

    def reset(self, env):
        """Reset the agent and environment."""
        self._java_agent.reset(env.obj)

    def get_value_function(self):
        """
        Returns the learned value function.

        Returns:
            numpy.ndarray: Flat value function array.
        """
        return np.array(list(self._java_agent.getValueFunction()))

    @property
    def obj(self):
        """Access the underlying Java object."""
        return self._java_agent
