
"""
MAP Queueing Network (MAPQN) Performance Analysis Functions.

This module provides Python wrappers for the JAR MAPQN API, which implements
analytical methods for computing performance bounds and approximations in
closed queueing networks with Markovian Arrival Processes (MAPs).

Components:

**Parameter Classes:**
- MapqnSolution: Result container with objective value and variable map.
- LinearReductionParameters: Parameters for multi-phase networks.
- MVAVersionParameters: Parameters for MVA-based models.
- Mapqn_qr_bounds_bas_parameters: Parameters for BAS blocking models.
- Mapqn_qr_bounds_rsrd_parameters: Parameters for RSRD blocking models.
- PFParameters: Parameters for product-form networks (no phases).
- QuadraticDelayParameters: Parameters for delay models.
- QuadraticLDParameters: Parameters for load-dependent models.
- mapqn_parameters_factory: Factory to create parameters from NetworkStruct.

**Linear Reduction Bounds:**
- mapqn_bnd_lr: General linear reduction bounds (LP).
- mapqn_bnd_lr_mva: Linear reduction via MVA formulation.
- mapqn_bnd_lr_pf: Linear reduction for product-form networks.

**Quadratic Reduction Bounds:**
- mapqn_bnd_qr: General quadratic reduction bounds (LP).
- mapqn_bnd_qr_delay: Quadratic bounds for delay systems.
- mapqn_bnd_qr_ld: Quadratic bounds for load-dependent systems.

**QR Bounds with Blocking:**
- mapqn_qr_bounds_bas: Blocking-After-Service (BAS) LP bounds.
- mapqn_qr_bounds_rsrd: Repetitive-Service Random-Destination (RSRD) LP bounds.

**QRF No-Blocking Approximations (NLP):**
- mapqn_qrf_noblo_mem: Maximum Entropy Method approximation.
- mapqn_qrf_noblo_mmi: Minimize Mutual Information approximation.
- mapqn_qrf_noblo_mmi_ld: Load-dependent MMI approximation.
- mapqn_qrf_noblo_mmi_linear: MMI with linear constraints (MEM objective).

**NLP Solver:**
- mapqn_nlp_solver: Augmented Lagrangian + BOBYQA constrained NLP solver.

**LP Model Builder:**
- MapqnLpmodel: Linear programming model builder for MAPQN problems.

References:
    Casale, G. "QRF: Analytical Bounds for Closed Queueing Networks
    with MAP Service", ACM SIGMETRICS, 2017.
"""

import jpype
import numpy as np
from line_solver import jlineMatrixToArray, jlineMatrixFromArray


# ===========================================================================
# Accessor for the JAR MAPQN package
# ===========================================================================

def _mapqn_pkg():
    """Return the jline.api.mapqn Java package."""
    return jpype.JPackage('jline').api.mapqn


# ===========================================================================
# Helper: convert Java Map<String, Double> to Python dict
# ===========================================================================

def _java_map_to_dict(java_map):
    """Convert a Java Map<String, Double> to a Python dict."""
    result = {}
    for entry in java_map.entrySet():
        result[str(entry.getKey())] = float(entry.getValue())
    return result


# ===========================================================================
# Result container
# ===========================================================================

class MapqnSolution:
    """
    Result container for MAPQN optimization solutions.

    Wraps the Java Mapqn_solution data class, providing access to the
    objective function value and all decision variable values.

    Attributes:
        objective_value (float): The optimal objective function value.
        variables (dict): Mapping of variable names to their optimal values.
    """

    def __init__(self, java_solution):
        """
        Construct from a Java Mapqn_solution object.

        Args:
            java_solution: A jline.api.mapqn.Mapqn_solution instance.
        """
        self._java = java_solution
        self.objective_value = float(java_solution.getObjectiveValue())
        self.variables = _java_map_to_dict(java_solution.getVariables())

    def get_variable(self, name):
        """
        Get the value of a specific decision variable.

        Args:
            name (str): Variable name (e.g., "U_1_1", "Q_2_1").

        Returns:
            float: The variable value, or 0.0 if not found.
        """
        return self.variables.get(name, 0.0)

    def get_utilization(self, i, k):
        """
        Get utilization for queue i, phase k (1-based indices).

        Tries both naming conventions used by different solvers.

        Args:
            i (int): Queue index (1-based).
            k (int): Phase index (1-based).

        Returns:
            float: Utilization value.
        """
        u = self.get_variable("U_{}_{}".format(i, k))
        if u != 0.0:
            return u
        return self.get_variable("e_{}_{}".format(i, k))

    def get_queue_length(self, i, k):
        """
        Get queue length for queue i, phase k (1-based indices).

        Args:
            i (int): Queue index (1-based).
            k (int): Phase index (1-based).

        Returns:
            float: Queue length value.
        """
        return self.get_variable("Q_{}_{}".format(i, k))

    @property
    def obj(self):
        """Access the underlying Java Mapqn_solution object."""
        return self._java


# ===========================================================================
# Parameter classes
# ===========================================================================

class LinearReductionParameters:
    """
    Parameters for linear reduction models with multiple MAP phases.

    Represents a closed network of M queues with population N, where each
    queue i has K[i] MAP phases. Service completion rates are given by
    mu[i] (K[i] x K[i] matrix) and background transition rates by v[i].

    Args:
        M (int): Number of queues.
        N (int): Total population (number of jobs).
        K (list of int): Number of phases for each queue, length M.
        mu (list of numpy.ndarray): Service completion rate matrices.
            mu[i] is a K[i] x K[i] matrix where mu[i][k][h] is the
            completion rate from phase k to phase h at queue i.
        r (numpy.ndarray): Routing probability matrix (M x M).
            r[i][j] is the probability of routing from queue i to queue j.
        v (list of numpy.ndarray): Background transition rate matrices.
            v[i] is a K[i] x K[i] matrix of phase transition rates
            that do not cause job routing (internal phase changes).
    """

    def __init__(self, M, N, K, mu, r, v):
        K_arr = jpype.JArray(jpype.JInt)(len(K))
        for i, val in enumerate(K):
            K_arr[i] = int(val)

        mu_arr = jpype.JArray(jpype.JClass('jline.util.matrix.Matrix'))(len(mu))
        for i, mat in enumerate(mu):
            mu_arr[i] = jlineMatrixFromArray(np.asarray(mat))

        r_mat = jlineMatrixFromArray(np.asarray(r))

        v_arr = jpype.JArray(jpype.JClass('jline.util.matrix.Matrix'))(len(v))
        for i, mat in enumerate(v):
            v_arr[i] = jlineMatrixFromArray(np.asarray(mat))

        self._java = _mapqn_pkg().LinearReductionParameters(
            jpype.JInt(M), jpype.JInt(N), K_arr, mu_arr, r_mat, v_arr
        )

    @property
    def obj(self):
        """Access the underlying Java LinearReductionParameters object."""
        return self._java


class MVAVersionParameters:
    """
    Parameters for MVA-based linear reduction models.

    In this formulation, queues 1 to M-1 are single-phase (exponential)
    servers with rates muM[i], while queue M is a MAP queue with K phases.

    Args:
        M (int): Number of queues.
        N (int): Total population.
        K (int): Number of levels/phases for the MAP queue (queue M).
        muM (list or numpy.ndarray): Service rates for queues 1 to M-1,
            length M-1.
        muMAP (numpy.ndarray): K x K service rate matrix for the MAP queue.
        r (numpy.ndarray): M x M routing probability matrix.
        v (numpy.ndarray): K x K level change rate matrix for the MAP queue.
    """

    def __init__(self, M, N, K, muM, muMAP, r, v):
        muM_arr = jpype.JArray(jpype.JDouble)(len(muM))
        for i, val in enumerate(muM):
            muM_arr[i] = float(val)

        self._java = _mapqn_pkg().MVAVersionParameters(
            jpype.JInt(M), jpype.JInt(N), jpype.JInt(K),
            muM_arr,
            jlineMatrixFromArray(np.asarray(muMAP)),
            jlineMatrixFromArray(np.asarray(r)),
            jlineMatrixFromArray(np.asarray(v))
        )

    @property
    def obj(self):
        """Access the underlying Java MVAVersionParameters object."""
        return self._java


class PFParameters:
    """
    Parameters for the product-form linear reduction model (no phases).

    A simplified parameterization for single-phase networks where each
    queue has a scalar service rate and routing is defined by an M x M matrix.

    Args:
        M (int): Number of queues.
        N (int): Total population.
        mu (list or numpy.ndarray): Service rate for each queue, length M.
        r (list of list or numpy.ndarray): M x M routing probability matrix.
    """

    def __init__(self, M, N, mu, r):
        mu_arr = jpype.JArray(jpype.JDouble)(len(mu))
        for i, val in enumerate(mu):
            mu_arr[i] = float(val)

        r_arr = jpype.JArray(jpype.JArray(jpype.JDouble))(len(r))
        r_np = np.asarray(r)
        for i in range(len(r)):
            row = jpype.JArray(jpype.JDouble)(r_np.shape[1])
            for j in range(r_np.shape[1]):
                row[j] = float(r_np[i, j])
            r_arr[i] = row

        self._java = _mapqn_pkg().Mapqn_bnd_lr_pf.PFParameters(
            jpype.JInt(M), jpype.JInt(N), mu_arr, r_arr
        )

    @property
    def obj(self):
        """Access the underlying Java PFParameters object."""
        return self._java


class QuadraticDelayParameters:
    """
    Parameters for the quadratic reduction model with delay stations.

    Extends the basic quadratic model by including a delay (infinite server)
    station and load-dependent service rates.

    Args:
        M (int): Number of queues.
        N (int): Total population.
        K (list of int): Number of phases for each queue, length M.
        Z (float): Think time at the delay station (queue M).
        D1 (float): Service demand at queue 1.
        mu (list of list of list): Completion transition rates.
            mu[i][k][h] is the completion rate from phase k to h at queue i.
        v (list of list of list): Background transition rates.
            v[i][k][h] is the background rate from phase k to h at queue i.
        alpha (list of list): Load-dependent rate scaling factors.
            alpha[i][n-1] is the scaling factor at queue i with n jobs.
        r (list of list or numpy.ndarray): M x M routing probabilities.
    """

    def __init__(self, M, N, K, Z, D1, mu, v, alpha, r):
        K_arr = jpype.JArray(jpype.JInt)(len(K))
        for i, val in enumerate(K):
            K_arr[i] = int(val)

        mu_arr = _to_3d_java_array(mu)
        v_arr = _to_3d_java_array(v)
        alpha_arr = _to_2d_java_array(alpha)
        r_arr = _to_2d_java_array(r)

        self._java = _mapqn_pkg().Mapqn_bnd_qr_delay.QuadraticDelayParameters(
            jpype.JInt(M), jpype.JInt(N), K_arr,
            jpype.JDouble(Z), jpype.JDouble(D1),
            mu_arr, v_arr, alpha_arr, r_arr
        )

    @property
    def obj(self):
        """Access the underlying Java QuadraticDelayParameters object."""
        return self._java


class QuadraticLDParameters:
    """
    Parameters for the quadratic reduction model with load-dependent rates.

    Like the basic quadratic model but with load-dependent service rate
    scaling factors alpha[i][n].

    Args:
        M (int): Number of queues.
        N (int): Total population.
        K (list of int): Number of phases for each queue, length M.
        mu (list of list of list): Completion transition rates.
            mu[i][k][h] is the completion rate from phase k to h at queue i.
        v (list of list of list): Background transition rates.
            v[i][k][h] is the background rate from phase k to h at queue i.
        alpha (list of list): Load-dependent rate scaling factors.
            alpha[i][n-1] is the scaling factor at queue i with n jobs.
        r (list of list or numpy.ndarray): M x M routing probabilities.
    """

    def __init__(self, M, N, K, mu, v, alpha, r):
        K_arr = jpype.JArray(jpype.JInt)(len(K))
        for i, val in enumerate(K):
            K_arr[i] = int(val)

        mu_arr = _to_3d_java_array(mu)
        v_arr = _to_3d_java_array(v)
        alpha_arr = _to_2d_java_array(alpha)
        r_arr = _to_2d_java_array(r)

        self._java = _mapqn_pkg().Mapqn_bnd_qr_ld.QuadraticLDParameters(
            jpype.JInt(M), jpype.JInt(N), K_arr,
            mu_arr, v_arr, alpha_arr, r_arr
        )

    @property
    def obj(self):
        """Access the underlying Java QuadraticLDParameters object."""
        return self._java


class Mapqn_qr_bounds_bas_parameters:
    """
    Parameters for QR Bounds with Blocking-After-Service (BAS).

    Describes a closed MAP network with finite-capacity queues using
    BAS blocking. When a completed job tries to enter a full queue,
    it is blocked until space becomes available.

    Args:
        M (int): Number of queues.
        N (int): Total population.
        MR (int): Number of independent blocking configurations.
        f (int): Finite-capacity queue index (1-based).
        K (list of int): Number of phases for each queue, length M.
        F (list of int): Capacity of each queue, length M.
        MM (numpy.ndarray): MR x 2 blocking order matrix.
        MM1 (numpy.ndarray): MR x M blocking order matrix.
        ZZ (list of int): Number of nonzeros in each blocking configuration.
        BB (numpy.ndarray): MR x M blocking state matrix (0/1 values).
        mu (list of numpy.ndarray): Completion rate matrices.
            mu[i] is a K[i] x K[i] matrix.
        v (list of numpy.ndarray): Background transition rate matrices.
            v[i] is a K[i] x K[i] matrix.
        r (numpy.ndarray): M x M routing probability matrix.
    """

    def __init__(self, M, N, MR, f, K, F, MM, MM1, ZZ, BB, mu, v, r):
        K_arr = jpype.JArray(jpype.JInt)(len(K))
        for i, val in enumerate(K):
            K_arr[i] = int(val)

        F_arr = jpype.JArray(jpype.JInt)(len(F))
        for i, val in enumerate(F):
            F_arr[i] = int(val)

        ZZ_arr = jpype.JArray(jpype.JInt)(len(ZZ))
        for i, val in enumerate(ZZ):
            ZZ_arr[i] = int(val)

        mu_arr = jpype.JArray(jpype.JClass('jline.util.matrix.Matrix'))(len(mu))
        for i, mat in enumerate(mu):
            mu_arr[i] = jlineMatrixFromArray(np.asarray(mat))

        v_arr = jpype.JArray(jpype.JClass('jline.util.matrix.Matrix'))(len(v))
        for i, mat in enumerate(v):
            v_arr[i] = jlineMatrixFromArray(np.asarray(mat))

        self._java = _mapqn_pkg().Mapqn_qr_bounds_bas_parameters(
            jpype.JInt(M), jpype.JInt(N), jpype.JInt(MR), jpype.JInt(f),
            K_arr, F_arr,
            jlineMatrixFromArray(np.asarray(MM)),
            jlineMatrixFromArray(np.asarray(MM1)),
            ZZ_arr,
            jlineMatrixFromArray(np.asarray(BB)),
            mu_arr, v_arr,
            jlineMatrixFromArray(np.asarray(r))
        )

    @property
    def obj(self):
        """Access the underlying Java parameter object."""
        return self._java


class Mapqn_qr_bounds_rsrd_parameters:
    """
    Parameters for QR Bounds with Repetitive-Service Random-Destination (RSRD).

    Describes a closed MAP network with finite-capacity queues using RSRD
    blocking. When a completed job tries to enter a full queue, it returns
    to the server and re-attempts service (going to a random destination).

    Args:
        M (int): Number of queues.
        N (int): Total population.
        F (list of int): Capacity of each queue, length M.
        K (list of int): Number of phases for each queue, length M.
        mu (list of numpy.ndarray): Completion rate matrices.
            mu[i] is a K[i] x K[i] matrix.
        v (list of numpy.ndarray): Background transition rate matrices.
            v[i] is a K[i] x K[i] matrix.
        alpha (list of list): Load-dependent rate scaling factors.
            alpha[i][n-1] is the scaling at queue i with n jobs (length N).
        r (numpy.ndarray): M x M routing probability matrix.
    """

    def __init__(self, M, N, F, K, mu, v, alpha, r):
        F_arr = jpype.JArray(jpype.JInt)(len(F))
        for i, val in enumerate(F):
            F_arr[i] = int(val)

        K_arr = jpype.JArray(jpype.JInt)(len(K))
        for i, val in enumerate(K):
            K_arr[i] = int(val)

        mu_arr = jpype.JArray(jpype.JClass('jline.util.matrix.Matrix'))(len(mu))
        for i, mat in enumerate(mu):
            mu_arr[i] = jlineMatrixFromArray(np.asarray(mat))

        v_arr = jpype.JArray(jpype.JClass('jline.util.matrix.Matrix'))(len(v))
        for i, mat in enumerate(v):
            v_arr[i] = jlineMatrixFromArray(np.asarray(mat))

        alpha_arr = _to_2d_java_array(alpha)

        self._java = _mapqn_pkg().Mapqn_qr_bounds_rsrd_parameters(
            jpype.JInt(M), jpype.JInt(N),
            F_arr, K_arr,
            mu_arr, v_arr, alpha_arr,
            jlineMatrixFromArray(np.asarray(r))
        )

    @property
    def obj(self):
        """Access the underlying Java parameter object."""
        return self._java


# ===========================================================================
# Array conversion helpers
# ===========================================================================

def _to_2d_java_array(arr):
    """Convert a Python 2D array/list to Java double[][]."""
    arr_np = np.asarray(arr, dtype=float)
    outer = jpype.JArray(jpype.JArray(jpype.JDouble))(arr_np.shape[0])
    for i in range(arr_np.shape[0]):
        inner = jpype.JArray(jpype.JDouble)(arr_np.shape[1])
        for j in range(arr_np.shape[1]):
            inner[j] = float(arr_np[i, j])
        outer[i] = inner
    return outer


def _to_3d_java_array(arr):
    """Convert a Python 3D array/list to Java double[][][]."""
    arr_list = list(arr)
    M = len(arr_list)
    outer = jpype.JArray(jpype.JArray(jpype.JArray(jpype.JDouble)))(M)
    for i in range(M):
        mat = np.asarray(arr_list[i], dtype=float)
        mid = jpype.JArray(jpype.JArray(jpype.JDouble))(mat.shape[0])
        for k in range(mat.shape[0]):
            inner = jpype.JArray(jpype.JDouble)(mat.shape[1])
            for h in range(mat.shape[1]):
                inner[h] = float(mat[k, h])
            mid[k] = inner
        outer[i] = mid
    return outer


def _to_4d_java_array(arr):
    """Convert a Python 4D array/list (MAPs) to Java double[][][][].

    MAPs[i] is a list of 2 matrices [D0, D1], each K[i] x K[i].
    So MAPs is Array<Array<Array<DoubleArray>>> in Kotlin.
    """
    arr_list = list(arr)
    M = len(arr_list)
    outer = jpype.JArray(jpype.JArray(jpype.JArray(jpype.JArray(jpype.JDouble))))(M)
    for i in range(M):
        station_maps = list(arr_list[i])
        n_matrices = len(station_maps)
        mid1 = jpype.JArray(jpype.JArray(jpype.JArray(jpype.JDouble)))(n_matrices)
        for d in range(n_matrices):
            mat = np.asarray(station_maps[d], dtype=float)
            mid2 = jpype.JArray(jpype.JArray(jpype.JDouble))(mat.shape[0])
            for k in range(mat.shape[0]):
                inner = jpype.JArray(jpype.JDouble)(mat.shape[1])
                for h in range(mat.shape[1]):
                    inner[h] = float(mat[k, h])
                mid2[k] = inner
            mid1[d] = mid2
        outer[i] = mid1
    return outer


# ===========================================================================
# Parameters Factory
# ===========================================================================

def mapqn_parameters_factory(network_struct):
    """
    Create MAPQN parameters from a NetworkStruct.

    Analyzes the network structure and returns the appropriate parameter
    object (LinearReductionParameters, Mapqn_qr_bounds_rsrd_parameters, etc.)
    based on the network characteristics (phases, finite capacity, etc.).

    Args:
        network_struct: A NetworkStruct object (Java or Python wrapper).
            Obtained via model.getStruct().

    Returns:
        object: The appropriate Java Mapqn_parameters subclass instance.
            The returned object can be passed directly to the solver
            functions (mapqn_bnd_lr, mapqn_bnd_qr, etc.) via its .obj
            attribute or used directly.
    """
    java_sn = network_struct.obj if hasattr(network_struct, 'obj') else network_struct
    return _mapqn_pkg().Mapqn_parameters_factory.INSTANCE.createFromNetworkStruct(java_sn)


def mapqn_parameters(network_struct):
    """
    Create MAPQN parameters from a NetworkStruct.

    Alias for mapqn_parameters_factory that uses the static factory method
    on the Mapqn_parameters base class.

    Args:
        network_struct: A NetworkStruct object (Java or Python wrapper).

    Returns:
        object: The appropriate Java Mapqn_parameters subclass instance.
    """
    java_sn = network_struct.obj if hasattr(network_struct, 'obj') else network_struct
    return _mapqn_pkg().Mapqn_parameters.fromNetworkStruct(java_sn)


# ===========================================================================
# LP Model Builder
# ===========================================================================

class MapqnLpmodel:
    """
    Linear Programming model builder for MAPQN problems.

    Provides methods to define variables, constraints, and objective
    functions for LP-based MAPQN analysis. Used internally by the
    bound computation methods.

    This is a Python wrapper around the Java Mapqn_lpmodel class.
    """

    def __init__(self):
        """Create a new empty LP model."""
        self._java = _mapqn_pkg().Mapqn_lpmodel()

    def add_variable(self, name):
        """
        Register a variable and return its index.

        Args:
            name (str): Variable name (e.g., "U_1_1", "Q_2_1").

        Returns:
            int: The variable index.
        """
        return int(self._java.addVariable(name))

    def get_variable_index(self, name):
        """
        Get the index of a registered variable.

        Args:
            name (str): Variable name.

        Returns:
            int: The variable index.

        Raises:
            Exception: If the variable is not found.
        """
        return int(self._java.getVariableIndex(name))

    def get_num_variables(self):
        """
        Get the total number of registered variables.

        Returns:
            int: Number of variables.
        """
        return int(self._java.getNumVariables())

    @property
    def obj(self):
        """Access the underlying Java Mapqn_lpmodel object."""
        return self._java


# ===========================================================================
# NLP Solver
# ===========================================================================

def mapqn_nlp_solver(objective_fn, n_vars, Aeq=None, beq=None,
                     Aub=None, bub=None, lb=None, ub=None, x0=None,
                     max_iter=20, max_eval=50000):
    """
    Solve a linearly-constrained NLP using Augmented Lagrangian + BOBYQA.

    Minimizes objective_fn(x) subject to:
        Aeq * x = beq   (equality constraints)
        Aub * x <= bub   (inequality constraints)
        lb <= x <= ub    (variable bounds)

    This is a general-purpose constrained NLP solver used internally by
    the QRF approximation methods (MMI, MEM).

    Args:
        objective_fn: Python callable that takes a numpy array (n_vars,)
            and returns a scalar float. Bridged to Java via JPype.
        n_vars (int): Number of decision variables.
        Aeq (numpy.ndarray or None): Equality constraint matrix
            (num_eq x n_vars).
        beq (numpy.ndarray or None): Equality constraint RHS (num_eq,).
        Aub (numpy.ndarray or None): Inequality constraint matrix
            (num_ineq x n_vars).
        bub (numpy.ndarray or None): Inequality constraint RHS (num_ineq,).
        lb (numpy.ndarray): Lower bounds on variables (n_vars,).
        ub (numpy.ndarray): Upper bounds on variables (n_vars,).
        x0 (numpy.ndarray): Initial point (n_vars,).
        max_iter (int): Maximum outer iterations for augmented Lagrangian.
        max_eval (int): Maximum function evaluations per BOBYQA call.

    Returns:
        numpy.ndarray: Optimal x vector (n_vars,).

    Note:
        The Python objective function is bridged to Java via JPype.
        For common use cases, prefer the QRF wrapper functions
        (mapqn_qrf_noblo_mem, mapqn_qrf_noblo_mmi, etc.) which
        construct the objective internally in Java for better performance.
    """
    Aeq_j = _to_2d_java_array(Aeq) if Aeq is not None else None
    beq_j = None
    if beq is not None:
        beq_np = np.asarray(beq, dtype=float).ravel()
        beq_j = jpype.JArray(jpype.JDouble)(len(beq_np))
        for i in range(len(beq_np)):
            beq_j[i] = float(beq_np[i])

    Aub_j = _to_2d_java_array(Aub) if Aub is not None else None
    bub_j = None
    if bub is not None:
        bub_np = np.asarray(bub, dtype=float).ravel()
        bub_j = jpype.JArray(jpype.JDouble)(len(bub_np))
        for i in range(len(bub_np)):
            bub_j[i] = float(bub_np[i])

    lb_np = np.asarray(lb, dtype=float).ravel()
    lb_j = jpype.JArray(jpype.JDouble)(len(lb_np))
    for i in range(len(lb_np)):
        lb_j[i] = float(lb_np[i])

    ub_np = np.asarray(ub, dtype=float).ravel()
    ub_j = jpype.JArray(jpype.JDouble)(len(ub_np))
    for i in range(len(ub_np)):
        ub_j[i] = float(ub_np[i])

    x0_np = np.asarray(x0, dtype=float).ravel()
    x0_j = jpype.JArray(jpype.JDouble)(len(x0_np))
    for i in range(len(x0_np)):
        x0_j[i] = float(x0_np[i])

    # Bridge the Python objective function to the Kotlin (DoubleArray) -> Double type
    # which compiles to kotlin.jvm.functions.Function1 in the JVM
    @jpype.JImplements('kotlin.jvm.functions.Function1')
    class _PythonObjective:
        def __init__(self, py_fn):
            self._fn = py_fn

        @jpype.JOverride
        def invoke(self, x):
            # x is a Java double[], convert to numpy array for the Python callable
            x_np = np.array(x, dtype=float)
            result = self._fn(x_np)
            return jpype.JDouble(float(result))

    java_objective = _PythonObjective(objective_fn)

    result_j = _mapqn_pkg().Mapqn_nlp_solver.solve(
        java_objective, n_vars,
        Aeq_j, beq_j, Aub_j, bub_j,
        lb_j, ub_j, x0_j,
        max_iter, max_eval
    )

    return np.array(result_j, dtype=float)


# ===========================================================================
# Linear Reduction Bounds
# ===========================================================================

def mapqn_bnd_lr(params, objective_queue, objective_phase):
    """
    Compute general linear reduction bounds for MAP queueing networks.

    Solves a linear program based on the linear reduction method to compute
    performance bounds (utilization, queue lengths) for closed MAP networks
    with multi-phase service.

    The LP maximizes the utilization U[objective_queue][objective_phase]
    subject to flow balance, population conservation, and bound constraints.

    Args:
        params (LinearReductionParameters): Model parameters including
            M queues, N population, phase counts K, service rates mu,
            routing r, and background rates v.
        objective_queue (int): Queue index to maximize utilization (1-based).
        objective_phase (int): Phase index to maximize utilization (1-based).

    Returns:
        MapqnSolution: Solution containing:
            - objective_value: Maximum utilization at the specified
              queue/phase.
            - variables: Dict of all LP variable values (U, Q, C, I, p1,
              p1c, UP, QP).

    Raises:
        Exception: If the LP is infeasible or parameters are invalid.

    Example:
        >>> params = LinearReductionParameters(M=2, N=5, K=[2, 1],
        ...     mu=[np.array([[0, 1],[1, 0]]), np.array([[2]])],
        ...     r=np.array([[0, 1],[1, 0]]),
        ...     v=[np.array([[0, 0.5],[0.5, 0]]), np.array([[0]])])
        >>> sol = mapqn_bnd_lr(params, 1, 1)
        >>> print(sol.objective_value)
    """
    java_params = params.obj if hasattr(params, 'obj') else params
    result = _mapqn_pkg().Mapqn_bnd_lr.INSTANCE.solve(
        java_params,
        jpype.JInt(objective_queue),
        jpype.JInt(objective_phase)
    )
    return MapqnSolution(result)


def mapqn_bnd_lr_mva(params, objective_queue, objective_level):
    """
    Compute linear reduction bounds using MVA formulation.

    Uses a Mean Value Analysis (MVA) formulation of the linear reduction
    method, specialized for networks where queues 1..M-1 are exponential
    and queue M is a MAP queue.

    Args:
        params (MVAVersionParameters): MVA model parameters including
            scalar rates muM for exponential queues, matrix muMAP for
            the MAP queue, routing r, and level change rates v.
        objective_queue (int): Queue index to maximize utilization (1-based).
        objective_level (int): Level index to maximize utilization (1-based).

    Returns:
        MapqnSolution: Solution containing the optimal utilization and
            all variable values (UN, QN, B).

    Example:
        >>> params = MVAVersionParameters(M=3, N=10, K=2,
        ...     muM=[1.0, 2.0],
        ...     muMAP=np.array([[1, 0.5],[0.5, 1]]),
        ...     r=np.array([[0, 0.5, 0.5],[0.5, 0, 0.5],[0.5, 0.5, 0]]),
        ...     v=np.array([[0, 0.3],[0.3, 0]]))
        >>> sol = mapqn_bnd_lr_mva(params, 3, 1)
    """
    java_params = params.obj if hasattr(params, 'obj') else params
    result = _mapqn_pkg().Mapqn_bnd_lr_mva.INSTANCE.solve(
        java_params,
        jpype.JInt(objective_queue),
        jpype.JInt(objective_level)
    )
    return MapqnSolution(result)


def mapqn_bnd_lr_pf(params, objective_queue=1):
    """
    Compute linear reduction bounds for product-form networks.

    Specialized linear reduction for single-phase (product-form) networks.
    Minimizes the utilization U[objective_queue] subject to flow balance,
    population conservation, and structural constraints.

    Args:
        params (PFParameters): Product-form model parameters including
            scalar service rates mu and routing matrix r.
        objective_queue (int): Queue index to minimize utilization
            (1-based, default 1).

    Returns:
        MapqnSolution: Solution containing minimum utilization and
            all variable values (U, Q, C, p1, p1c).

    Example:
        >>> params = PFParameters(M=2, N=5,
        ...     mu=[2.0, 3.0],
        ...     r=[[0, 1],[1, 0]])
        >>> sol = mapqn_bnd_lr_pf(params, 1)
    """
    java_params = params.obj if hasattr(params, 'obj') else params
    result = _mapqn_pkg().Mapqn_bnd_lr_pf.INSTANCE.solve(
        java_params,
        jpype.JInt(objective_queue)
    )
    return MapqnSolution(result)


# ===========================================================================
# Quadratic Reduction Bounds
# ===========================================================================

def mapqn_bnd_qr(params, objective_queue, objective_phase):
    """
    Compute general quadratic reduction bounds for MAP queueing networks.

    Extends linear reduction by introducing quadratic (p2) variables for
    joint queue length probabilities. This provides tighter bounds at the
    cost of a larger LP.

    Args:
        params (LinearReductionParameters): Model parameters (same as
            for mapqn_bnd_lr).
        objective_queue (int): Queue index (1-based).
        objective_phase (int): Phase index (1-based).

    Returns:
        MapqnSolution: Solution with objective value and all variables
            including p2 joint probabilities.
    """
    java_params = params.obj if hasattr(params, 'obj') else params
    result = _mapqn_pkg().Mapqn_bnd_qr.INSTANCE.solve(
        java_params,
        jpype.JInt(objective_queue),
        jpype.JInt(objective_phase)
    )
    return MapqnSolution(result)


def mapqn_bnd_qr_delay(params, objective_queue, objective_phase, objective_n):
    """
    Compute quadratic reduction bounds for delay systems.

    Specialized for networks containing infinite-server (delay/think time)
    stations. Includes Little's Law and delay-specific constraints.

    Args:
        params (QuadraticDelayParameters): Parameters including delay Z,
            service demand D1, and load-dependent rates alpha.
        objective_queue (int): Queue index to maximize p2 marginal (1-based).
        objective_phase (int): Phase index (1-based).
        objective_n (int): Population level at the queue (0 to N).

    Returns:
        MapqnSolution: Solution with p2 marginal probabilities.
    """
    java_params = params.obj if hasattr(params, 'obj') else params
    result = _mapqn_pkg().Mapqn_bnd_qr_delay.INSTANCE.solve(
        java_params,
        jpype.JInt(objective_queue),
        jpype.JInt(objective_phase),
        jpype.JInt(objective_n)
    )
    return MapqnSolution(result)


def mapqn_bnd_qr_ld(params, objective_queue, objective_phase, objective_n):
    """
    Compute quadratic reduction bounds for load-dependent systems.

    Like mapqn_bnd_qr but with load-dependent service rates that scale
    with the number of jobs at each queue.

    Args:
        params (QuadraticLDParameters): Parameters including load-dependent
            scaling factors alpha[i][n].
        objective_queue (int): Queue index to maximize p2 marginal (1-based).
        objective_phase (int): Phase index (1-based).
        objective_n (int): Population level at the queue (0 to N).

    Returns:
        MapqnSolution: Solution with p2 marginal probabilities.
    """
    java_params = params.obj if hasattr(params, 'obj') else params
    result = _mapqn_pkg().Mapqn_bnd_qr_ld.INSTANCE.solve(
        java_params,
        jpype.JInt(objective_queue),
        jpype.JInt(objective_phase),
        jpype.JInt(objective_n)
    )
    return MapqnSolution(result)


# ===========================================================================
# QR Bounds with Blocking
# ===========================================================================

def mapqn_qr_bounds_bas(params, objective_queue, sense="min"):
    """
    Compute QR bounds using Blocking-After-Service (BAS) method.

    Solves an LP for finite-capacity MAP networks with BAS blocking protocol.
    Uses Apache Commons Math SimplexSolver.

    In BAS, a job that completes service at queue i and is routed to a
    full queue j is blocked at queue i until space becomes available at j.

    Args:
        params (Mapqn_qr_bounds_bas_parameters): BAS model parameters
            including capacities F, blocking configurations BB/MM/ZZ.
        objective_queue (int): Queue index to optimize utilization (1-based).
        sense (str): "min" for lower bound or "max" for upper bound
            (default "min").

    Returns:
        MapqnSolution: Solution with the bound value and all LP variables.

    Raises:
        ValueError: If sense is not "min" or "max".
    """
    java_params = params.obj if hasattr(params, 'obj') else params
    result = _mapqn_pkg().Mapqn_qr_bounds_bas.INSTANCE.solve(
        java_params,
        jpype.JInt(objective_queue),
        sense
    )
    return MapqnSolution(result)


def mapqn_qr_bounds_rsrd(params, objective_queue, sense="min"):
    """
    Compute QR bounds using Repetitive-Service Random-Destination (RSRD).

    Solves an LP for finite-capacity MAP networks with RSRD blocking.
    Uses ojAlgo ExpressionsBasedModel for optimization.

    In RSRD, a job that completes service and is routed to a full queue
    returns to the server for re-service and chooses a new random
    destination upon completion.

    Args:
        params (Mapqn_qr_bounds_rsrd_parameters): RSRD model parameters
            including capacities F, load-dependent rates alpha.
        objective_queue (int): Queue index to optimize utilization (1-based).
        sense (str): "min" for lower bound or "max" for upper bound
            (default "min").

    Returns:
        MapqnSolution: Solution with the bound value and all LP variables.

    Raises:
        ValueError: If sense is not "min" or "max".
    """
    java_params = params.obj if hasattr(params, 'obj') else params
    result = _mapqn_pkg().Mapqn_qr_bounds_rsrd.INSTANCE.solve(
        java_params,
        jpype.JInt(objective_queue),
        sense
    )
    return MapqnSolution(result)


# ===========================================================================
# QRF No-Blocking NLP Approximations
# ===========================================================================

def mapqn_qrf_noblo_mem(MAPs, N, rt):
    """
    Maximum Entropy Method (MEM) approximation for MAP queueing networks.

    Solves a constrained NLP to find the joint queue length distribution
    that maximizes entropy subject to flow balance and population
    constraints. Uses Augmented Lagrangian + BOBYQA optimization.

    This is a port of MATLAB qrf_noblo_mem.m.

    Args:
        MAPs (list): List of M MAP representations. Each MAPs[i] is a
            list of two 2D arrays [D0, D1] where D0 is the K[i] x K[i]
            background transition matrix and D1 is the K[i] x K[i]
            service completion matrix.
        N (int): Total population (number of closed jobs).
        rt (numpy.ndarray or list): M x M routing probability matrix.

    Returns:
        MapqnSolution: Approximate solution with queue length distribution
            and utilization values.

    Example:
        >>> # Two queues with 2-phase MAP service
        >>> D0_1 = [[-3, 1], [1, -3]]
        >>> D1_1 = [[1, 1], [1, 1]]
        >>> D0_2 = [[-2, 0.5], [0.5, -2]]
        >>> D1_2 = [[0.75, 0.75], [0.75, 0.75]]
        >>> MAPs = [[D0_1, D1_1], [D0_2, D1_2]]
        >>> rt = [[0, 1], [1, 0]]
        >>> sol = mapqn_qrf_noblo_mem(MAPs, N=5, rt=rt)
    """
    MAPs_j = _to_4d_java_array(MAPs)
    rt_j = _to_2d_java_array(rt)
    result = _mapqn_pkg().Mapqn_qrf_noblo_mem.solve(
        MAPs_j, jpype.JInt(N), rt_j
    )
    return MapqnSolution(result)


def mapqn_qrf_noblo_mmi(M, MR, K, N, mu, v, rt):
    """
    Minimize Mutual Information (MMI) approximation for MAP queueing networks.

    Solves a constrained NLP to find the joint queue length distribution
    that minimizes mutual information between queue length pairs. This
    produces a distribution that is as close to independent as possible
    while satisfying the network constraints.

    This is a port of MATLAB qrf_noblo_mmi.m.

    Args:
        M (int): Number of queues.
        MR (int): Number of independent blocking configurations
            (typically 1 for no-blocking models).
        K (list of int): Number of phases for each queue, length M.
        N (int): Total population.
        mu (list of list of list): Completion transition rates.
            mu[i][k][h] is the rate from phase k to h at queue i.
        v (list of list of list): Background transition rates.
            v[i][k][h] is the background rate from phase k to h at queue i.
        rt (list of list or numpy.ndarray): M x M routing probabilities.

    Returns:
        MapqnSolution: Approximate solution.
    """
    K_j = jpype.JArray(jpype.JInt)(len(K))
    for i, val in enumerate(K):
        K_j[i] = int(val)

    mu_j = _to_3d_java_array(mu)
    v_j = _to_3d_java_array(v)
    rt_j = _to_2d_java_array(rt)

    result = _mapqn_pkg().Mapqn_qrf_noblo_mmi.solve(
        jpype.JInt(M), jpype.JInt(MR), K_j, jpype.JInt(N),
        mu_j, v_j, rt_j
    )
    return MapqnSolution(result)


def mapqn_qrf_noblo_mmi_ld(MAPs, N, rt, alpha=None):
    """
    Load-dependent MMI approximation for MAP queueing networks.

    Like mapqn_qrf_noblo_mmi but with load-dependent service rate
    scaling factors. The service rates at each queue are multiplied
    by alpha[i][n-1] when there are n jobs at queue i.

    This is a port of MATLAB qrf_noblo_mmi_ld.m.

    Args:
        MAPs (list): List of M MAP representations [D0, D1] for each queue.
        N (int): Total population.
        rt (numpy.ndarray or list): M x M routing probability matrix.
        alpha (list of list or None): Load-dependent scaling factors.
            alpha[i][n-1] scales the rate at queue i with n jobs.
            If None, defaults to 1.0 for all queues and populations.

    Returns:
        MapqnSolution: Approximate solution.
    """
    MAPs_j = _to_4d_java_array(MAPs)
    rt_j = _to_2d_java_array(rt)

    if alpha is not None:
        alpha_j = _to_2d_java_array(alpha)
        result = _mapqn_pkg().Mapqn_qrf_noblo_mmi_ld.solve(
            MAPs_j, jpype.JInt(N), rt_j, alpha_j
        )
    else:
        result = _mapqn_pkg().Mapqn_qrf_noblo_mmi_ld.solve(
            MAPs_j, jpype.JInt(N), rt_j
        )
    return MapqnSolution(result)


def mapqn_qrf_noblo_mmi_linear(MAPs, N, rt, alpha=None):
    """
    MMI with linear constraint matrices for MAP queueing networks.

    Uses the MEM (Maximum Entropy) objective function despite the name,
    matching the MATLAB implementation. The "linear" refers to the linear
    constraint matrix structure, not the objective.

    This is a port of MATLAB qrf_noblo_mmi_linear.m.

    Args:
        MAPs (list): List of M MAP representations [D0, D1] for each queue.
        N (int): Total population.
        rt (numpy.ndarray or list): M x M routing probability matrix.
        alpha (list of list or None): Load-dependent scaling factors.
            If None, defaults to 1.0 for all queues and populations.

    Returns:
        MapqnSolution: Approximate solution.
    """
    MAPs_j = _to_4d_java_array(MAPs)
    rt_j = _to_2d_java_array(rt)

    if alpha is not None:
        alpha_j = _to_2d_java_array(alpha)
        result = _mapqn_pkg().Mapqn_qrf_noblo_mmi_linear.solve(
            MAPs_j, jpype.JInt(N), rt_j, alpha_j
        )
    else:
        result = _mapqn_pkg().Mapqn_qrf_noblo_mmi_linear.solve(
            MAPs_j, jpype.JInt(N), rt_j
        )
    return MapqnSolution(result)


# ===========================================================================
# LP Model wrapper function (for completeness)
# ===========================================================================

def mapqn_lpmodel():
    """
    Create a new MAPQN LP model builder.

    Returns a MapqnLpmodel instance for constructing custom LP formulations
    for MAP queueing network analysis.

    Returns:
        MapqnLpmodel: A new empty LP model builder.
    """
    return MapqnLpmodel()
