
"""
Normalizing Constant (NC) algorithms for queueing networks.

This module provides NC-based algorithms for performance analysis of
queueing networks that complement the product-form algorithms in the
pfqn module.

Functions:
- me_oqn: Maximum Entropy algorithm for Open Queueing Networks (Kouvatsos, 1994)

References:
    Kouvatsos, D. D. (1994). Entropy Maximisation and Queueing Network Models.
    Annals of Operations Research, 48, 63-126.
"""

import jpype
import numpy as np
from line_solver import jlineMatrixToArray, jlineMatrixFromArray


def me_oqn(M, R, lambda0, Ca0, mu, Cs, P, tol=1e-6, max_iter=1000,
           verbose=False):
    """
    Maximum Entropy algorithm for Open Queueing Networks.

    Implements the ME algorithm from Kouvatsos (1994), Section 3.2. This
    approximation computes mean queue lengths, waiting times, and arrival/
    departure squared coefficients of variation (SCV) for general open
    queueing networks with single-server FCFS queues.

    The algorithm iterates:
    1. Solve traffic equations to obtain total arrival rates per queue/class
    2. Apply GE-type formulae for mean queue lengths using arrival and service SCV
    3. Update departure SCV via equation (3.6): Cd = 2L(1-rho) + Ca(1-2*rho)
    4. Update arrival SCV via superposition/splitting (equation 3.7)
    5. Repeat steps 2-4 until convergence

    Feedback correction is automatically applied for queues with self-loops
    (p_{ii,r} > 0), adjusting effective service rates and SCV.

    Args:
        M (int): Number of queues (stations) in the network.
        R (int): Number of job classes.
        lambda0 (numpy.ndarray): External arrival rates, shape (M, R).
            lambda0[i, r] is the external arrival rate of class r to queue i.
            Set to 0 for queues with no external arrivals.
        Ca0 (numpy.ndarray): External arrival SCV, shape (M, R).
            Ca0[i, r] is the squared coefficient of variation of the external
            arrival process of class r at queue i. Use 1.0 for Poisson arrivals.
        mu (numpy.ndarray): Service rates, shape (M, R).
            mu[i, r] is the service rate for class r at queue i.
        Cs (numpy.ndarray): Service SCV, shape (M, R).
            Cs[i, r] is the squared coefficient of variation of the service
            time distribution for class r at queue i. Use 1.0 for exponential.
        P (list of list of numpy.ndarray): Routing probability matrix.
            P[j][i] is a (R, 1) array where P[j][i][r, 0] is the probability
            that a class-r job departing queue j goes to queue i.
            Row sums (over destinations) should be <= 1 for each source queue
            and class; the remainder leaves the network.
        tol (float): Convergence tolerance for the arrival SCV iteration.
            Iteration stops when max|Ca_new - Ca_old| < tol. Default: 1e-6.
        max_iter (int): Maximum number of iterations. Default: 1000.
        verbose (bool): If True, print iteration progress. Default: False.

    Returns:
        dict: Performance metrics containing:
            - L (numpy.ndarray): Mean queue lengths, shape (M, R).
                L[i, r] is the mean number of class-r jobs at queue i
                (including the one in service).
            - W (numpy.ndarray): Mean waiting times, shape (M, R).
                W[i, r] = L[i, r] / lambda[i, r] by Little's law.
            - Ca (numpy.ndarray): Arrival SCV at each queue, shape (M, R).
                Converged squared coefficient of variation of the total
                arrival process.
            - Cd (numpy.ndarray): Departure SCV at each queue, shape (M, R).
            - lambda_ (numpy.ndarray): Total arrival rates, shape (M, R).
                lambda_[i, r] is the total arrival rate of class r at queue i
                (external + internal routing).
            - rho (numpy.ndarray): Utilizations, shape (M, R).
                rho[i, r] = lambda_[i, r] / mu[i, r].

    Raises:
        RuntimeError: If the network is unstable (total utilization >= 1 at
            any queue).

    Example:
        >>> import numpy as np
        >>> M, R = 2, 1
        >>> lambda0 = np.array([[1.0], [0.0]])  # arrivals only at queue 0
        >>> Ca0 = np.array([[1.0], [1.0]])       # Poisson arrivals
        >>> mu = np.array([[2.0], [3.0]])         # service rates
        >>> Cs = np.array([[1.0], [1.0]])         # exponential service
        >>> # Routing: queue 0 -> queue 1 with prob 0.5
        >>> P = [[np.array([[0.0]]), np.array([[0.5]])],
        ...      [np.array([[0.0]]), np.array([[0.0]])]]
        >>> result = me_oqn(M, R, lambda0, Ca0, mu, Cs, P)
        >>> print(result['L'])  # mean queue lengths
        >>> print(result['W'])  # mean waiting times
    """
    Me_oqnKt = jpype.JPackage('jline').api.nc.Me_oqnKt
    MeOqnOptions = jpype.JPackage('jline').api.nc.MeOqnOptions
    MatrixClass = jpype.JPackage('jline').util.matrix.Matrix

    # Convert scalar inputs to Java types
    java_M = jpype.JInt(M)
    java_R = jpype.JInt(R)

    # Convert matrix inputs
    lambda0_mat = jlineMatrixFromArray(np.atleast_2d(lambda0))
    Ca0_mat = jlineMatrixFromArray(np.atleast_2d(Ca0))
    mu_mat = jlineMatrixFromArray(np.atleast_2d(mu))
    Cs_mat = jlineMatrixFromArray(np.atleast_2d(Cs))

    # Build Java Array<Array<Matrix>> for P
    # P[j][i] is an R x 1 Matrix
    # In Kotlin: Array<Array<Matrix>> means Array of size M, each containing Array of size M
    outer_type = jpype.JArray(jpype.JArray(MatrixClass))
    inner_type = jpype.JArray(MatrixClass)

    java_P = outer_type(M)
    for j in range(M):
        java_P[j] = inner_type(M)
        for i in range(M):
            p_arr = np.atleast_2d(P[j][i])
            # Ensure it's R x 1
            if p_arr.shape[1] != 1 and p_arr.shape[0] == 1:
                p_arr = p_arr.T
            java_P[j][i] = jlineMatrixFromArray(p_arr)

    # Build options
    options = MeOqnOptions(
        jpype.JDouble(tol),
        jpype.JInt(max_iter),
        jpype.JBoolean(verbose)
    )

    # Call the Java function
    result = Me_oqnKt.me_oqn(
        java_M, java_R, lambda0_mat, Ca0_mat, mu_mat, Cs_mat, java_P, options
    )

    # Extract results using component accessors (Kotlin data class MeOqnResult)
    L = jlineMatrixToArray(result.component1())       # L
    W = jlineMatrixToArray(result.component2())       # W
    Ca = jlineMatrixToArray(result.component3())      # Ca
    Cd = jlineMatrixToArray(result.component4())      # Cd
    lambda_out = jlineMatrixToArray(result.component5())  # lambda
    rho = jlineMatrixToArray(result.component6())     # rho

    return {
        'L': L,
        'W': W,
        'Ca': Ca,
        'Cd': Cd,
        'lambda_': lambda_out,
        'rho': rho,
    }
