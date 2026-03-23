
"""
MAP/M/1-PS sojourn time distribution functions.

This module computes the complementary distribution function (CCDF) of
sojourn time in a MAP/M/1 processor-sharing queue using the algorithm from:

    Masuyama, H., & Takine, T. (2003). Sojourn time distribution in a
    MAP/M/1 processor-sharing queue. Operations Research Letters, 31(6), 406-412.

The MAP (Markovian Arrival Process) is specified by two matrices (C, D):
- C: Transitions without arrivals (the "hidden" transitions)
- D: Transitions with arrivals (the "observable" transitions)
The underlying Markov chain generator is Q = C + D.

Functions:
- mapm1ps_cdf_respt: Compute complementary sojourn time CDF values
"""

import jpype
import numpy as np
from line_solver import jlineMatrixToArray, jlineMatrixFromArray


def mapm1ps_cdf_respt(C, D, mu, x, epsilon=1e-11, epsilon_prime=1e-10):
    """
    Compute complementary sojourn time CDF for MAP/M/1-PS queue.

    Evaluates the complementary CDF W_bar(x) = Pr[W > x] at each time point
    in x, where W is the sojourn time (time in system) for a tagged customer
    in a MAP/M/1 processor-sharing queue.

    The computation uses a matrix-analytic approach:
    1. Compute stationary distribution pi of the MAP (C + D)
    2. Find the rate matrix R (minimal nonneg. solution of D + R(C - mu*I) + mu*R^2 = 0)
    3. Determine queue length truncation N(epsilon)
    4. For each time point, compute W_bar(x) via Poisson uniformization over
       recursively computed vectors h_{n,k}

    Stability requires rho = lambda/mu < 1 where lambda = pi * D * e is the
    mean arrival rate.

    Args:
        C (numpy.ndarray): MAP C matrix (M x M). Transitions without arrivals.
            Off-diagonal elements are non-negative; diagonal elements are
            negative such that row sums of (C + D) are zero.
        D (numpy.ndarray): MAP D matrix (M x M). Transitions with arrivals.
            All elements are non-negative.
        mu (float): Service rate at the PS server. Must be positive.
        x (numpy.ndarray or list): Time points at which to evaluate the
            complementary CDF. Must be non-negative.
        epsilon (float): Queue length truncation parameter. Determines the
            maximum queue length N(epsilon) considered. Smaller values give
            higher accuracy but slower computation. Default: 1e-11.
        epsilon_prime (float): Uniformization truncation parameter. Controls
            the Poisson tail truncation for the time discretization.
            Default: 1e-10.

    Returns:
        numpy.ndarray: Complementary CDF values W_bar(x_i) = Pr[W > x_i]
            for each time point x_i. Shape: (len(x),).
            Values are in [0, 1] with W_bar(0) = 1 and W_bar(inf) -> 0.

    Raises:
        RuntimeError: If the system is unstable (rho >= 1), C and D have
            different dimensions, or C is not square.

    Example:
        >>> import numpy as np
        >>> # Exponential arrivals (Poisson) with rate 0.5
        >>> C = np.array([[-0.5]])
        >>> D = np.array([[0.5]])
        >>> mu = 1.0
        >>> x = np.linspace(0, 10, 100)
        >>> ccdf = mapm1ps_cdf_respt(C, D, mu, x)
        >>> # ccdf[0] is close to 1, ccdf[-1] is close to 0
    """
    MAPM1PSCdfRespT = jpype.JPackage('jline').api.map.MAPM1PSCdfRespT

    # Convert inputs to Java types
    C_mat = jlineMatrixFromArray(np.atleast_2d(C))
    D_mat = jlineMatrixFromArray(np.atleast_2d(D))

    x_arr = np.asarray(x, dtype=float).ravel()
    java_x = jpype.JArray(jpype.JDouble)(len(x_arr))
    for i in range(len(x_arr)):
        java_x[i] = float(x_arr[i])

    # Call the Java method
    java_result = MAPM1PSCdfRespT.computeCdf(
        C_mat, D_mat, jpype.JDouble(mu), java_x,
        jpype.JDouble(epsilon), jpype.JDouble(epsilon_prime)
    )

    # Convert result back to numpy array
    return np.array([float(java_result[i]) for i in range(len(java_result))])
