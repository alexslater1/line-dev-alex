
"""
Fork-Join (F/J) queueing system analysis functions.

This module provides analytical methods for Fork-Join queueing systems
where arriving jobs are split into K parallel sub-tasks, each processed
by an independent server, and the job completes when all sub-tasks finish.

Functions include:
- Response time approximations (exact 2-way, Nelson-Tantawi, Varma-Makowski, Varki)
- Bounds (upper/lower) on response time
- Maximum response time (Rmax) for K parallel M/M/1 queues
- Expected maximum of K i.i.d. random variables (exp, Erlang, hyperexp, normal, Pareto)
- Order statistics (CDF, expected max/min)
- Synchronization delay
- Split-Merge throughput
- Characteristic maximum (M_K)
- Quantile approximations
- G(K) bound factors for mean-variance approximation

References:
    A. Thomasian, "Analysis of Fork/Join and Related Queueing Systems",
    ACM Computing Surveys, Vol. 47, No. 2, Article 17, July 2014.
"""

import jpype
import numpy as np
from line_solver import jlineMatrixToArray, jlineMatrixFromArray


# ===========================================================================
# Harmonic number
# ===========================================================================

def fj_harmonic(K):
    """
    Compute the K-th Harmonic number H_K = 1 + 1/2 + ... + 1/K.

    H_K is a fundamental quantity in Fork-Join analysis. For large K,
    H_K approximates ln(K) + gamma (Euler-Mascheroni constant).

    Args:
        K: Number of parallel servers (positive integer).

    Returns:
        float: Harmonic number H_K.
    """
    return float(jpype.JPackage('jline').api.fj.FJ_harmonicKt.fj_harmonic(jpype.JInt(K)))


# ===========================================================================
# Response time bounds
# ===========================================================================

def fj_bounds(K, lambda_, mu):
    """
    Upper and lower bounds for K-way Fork-Join response time.

    Upper bound: R_K^max(rho) = H_K / (mu * (1 - rho))
    Lower bound: R_K^{F/J(opt)}(rho) = (1/mu) * [H_K + S_{K(K-rho)}]

    Args:
        K: Number of parallel servers (positive integer).
        lambda_: Arrival rate.
        mu: Service rate (mu > lambda for stability).

    Returns:
        tuple: (Rmax, Rmin) upper and lower bounds.
    """
    result = jpype.JPackage('jline').api.fj.FJ_boundsKt.fj_bounds(
        jpype.JInt(K), jpype.JDouble(lambda_), jpype.JDouble(mu)
    )
    # Kotlin data class FJBoundsResult: component1()=Rmax, component2()=Rmin
    return float(result.component1()), float(result.component2())


# ===========================================================================
# Response time approximations
# ===========================================================================

def fj_respt_2way(lambda_, mu):
    """
    Exact two-way Fork-Join response time (Flatto-Hahn, 1984).

    R_2^{F/J}(rho) = (12 - rho)/8 * R(rho)
    where R(rho) = 1/(mu - lambda) is the M/M/1 response time.

    Args:
        lambda_: Arrival rate.
        mu: Service rate (mu > lambda for stability).

    Returns:
        float: Exact 2-way F/J response time.
    """
    return float(jpype.JPackage('jline').api.fj.FJ_resptKt.fj_respt_2way(
        jpype.JDouble(lambda_), jpype.JDouble(mu)
    ))


def fj_respt_nt(K, lambda_, mu):
    """
    Nelson-Tantawi approximation for K-way F/J response time.

    Valid for 2 <= K <= 32.

    Args:
        K: Number of parallel servers (2 <= K <= 32).
        lambda_: Arrival rate.
        mu: Service rate (mu > lambda for stability).

    Returns:
        float: Approximate K-way F/J response time.
    """
    return float(jpype.JPackage('jline').api.fj.FJ_resptKt.fj_respt_nt(
        jpype.JInt(K), jpype.JDouble(lambda_), jpype.JDouble(mu)
    ))


def fj_respt_vm(K, lambda_, mu):
    """
    Varma-Makowski approximation for K-way F/J response time.

    Args:
        K: Number of parallel servers (positive integer).
        lambda_: Arrival rate.
        mu: Service rate (mu > lambda for stability).

    Returns:
        float: Approximate K-way F/J response time.
    """
    return float(jpype.JPackage('jline').api.fj.FJ_resptKt.fj_respt_vm(
        jpype.JInt(K), jpype.JDouble(lambda_), jpype.JDouble(mu)
    ))


def fj_respt_varki(K, lambda_, mu):
    """
    Varki et al. approximation for K-way F/J response time.

    Uses the mean of pessimistic and optimistic bounds.

    Args:
        K: Number of parallel servers (positive integer).
        lambda_: Arrival rate.
        mu: Service rate (mu > lambda for stability).

    Returns:
        float: Approximate K-way F/J response time.
    """
    return float(jpype.JPackage('jline').api.fj.FJ_resptKt.fj_respt_varki(
        jpype.JInt(K), jpype.JDouble(lambda_), jpype.JDouble(mu)
    ))


# ===========================================================================
# Maximum response time (Rmax)
# ===========================================================================

def fj_rmax(K, lambda_, mu):
    """
    Maximum response time R_K^max(rho) for K parallel M/M/1 queues.

    R_K^max(rho) = H_K / (mu - lambda), an upper bound to F/J response time.

    Args:
        K: Number of parallel servers (positive integer).
        lambda_: Arrival rate.
        mu: Service rate (mu > lambda for stability).

    Returns:
        float: Expected maximum response time.
    """
    return float(jpype.JPackage('jline').api.fj.FJ_rmaxKt.fj_rmax(
        jpype.JInt(K), jpype.JDouble(lambda_), jpype.JDouble(mu)
    ))


def fj_rmax_erlang(K, k, lambda_, mu):
    """
    Maximum response time for K parallel M/E_k/1 queues.

    Args:
        K: Number of parallel servers (positive integer).
        k: Number of Erlang stages (positive integer).
        lambda_: Arrival rate.
        mu: Service rate per Erlang stage (mean service = k/mu).

    Returns:
        float: Expected maximum response time.
    """
    return float(jpype.JPackage('jline').api.fj.FJ_rmaxKt.fj_rmax_erlang(
        jpype.JInt(K), jpype.JInt(k), jpype.JDouble(lambda_), jpype.JDouble(mu)
    ))


def fj_rmax_evd(K, R, sigmaR, calibrated=False):
    """
    Maximum response time using Extreme Value Distribution approximation.

    R_K^max(rho) = R(rho) + (sqrt(6)*ln(K)/pi) * sigma_R(rho)

    Args:
        K: Number of parallel servers (positive integer).
        R: Mean response time.
        sigmaR: Standard deviation of response time.
        calibrated: Use calibrated formula dividing by 1.27 (default: False).

    Returns:
        float: Approximate maximum response time.
    """
    return float(jpype.JPackage('jline').api.fj.FJ_rmaxKt.fj_rmax_evd(
        jpype.JInt(K), jpype.JDouble(R), jpype.JDouble(sigmaR), jpype.JBoolean(calibrated)
    ))


# ===========================================================================
# Expected maximum of K i.i.d. random variables (Xmax)
# ===========================================================================

def fj_xmax_exp(K, mu):
    """
    Expected maximum of K i.i.d. exponential random variables.

    X_K^max = H_K / mu

    Args:
        K: Number of random variables (positive integer).
        mu: Rate parameter (mean = 1/mu).

    Returns:
        float: Expected maximum.
    """
    return float(jpype.JPackage('jline').api.fj.FJ_xmaxKt.fj_xmax_exp(
        jpype.JInt(K), jpype.JDouble(mu)
    ))


def fj_xmax_2(lambda1, lambda2=None):
    """
    Expected maximum of 2 exponential random variables.

    Y_2^max = 1/lambda1 + 1/lambda2 - 1/(lambda1 + lambda2)
    For identical rates: Y_2^max = 1.5/lambda

    Args:
        lambda1: Rate of first exponential.
        lambda2: Rate of second exponential (default: same as lambda1).

    Returns:
        float: Expected maximum.
    """
    if lambda2 is None:
        return float(jpype.JPackage('jline').api.fj.FJ_xmaxKt.fj_xmax_2(
            jpype.JDouble(lambda1)
        ))
    else:
        return float(jpype.JPackage('jline').api.fj.FJ_xmaxKt.fj_xmax_2(
            jpype.JDouble(lambda1), jpype.JDouble(lambda2)
        ))


def fj_xmax_erlang(K, k, mu):
    """
    Expected maximum of K i.i.d. Erlang-k random variables.

    For k=2: uses closed-form formula.
    For general k: uses numerical integration.

    Args:
        K: Number of random variables (positive integer).
        k: Number of Erlang stages (positive integer).
        mu: Rate parameter per stage (mean service time = k/mu).

    Returns:
        float: Expected maximum service time.
    """
    return float(jpype.JPackage('jline').api.fj.FJ_xmaxKt.fj_xmax_erlang(
        jpype.JInt(K), jpype.JInt(k), jpype.JDouble(mu)
    ))


def fj_xmax_hyperexp(K, p1, mu1, mu2):
    """
    Expected maximum of K i.i.d. hyperexponential-2 random variables.

    Args:
        K: Number of random variables (positive integer).
        p1: Probability of branch 1 (0 < p1 < 1).
        mu1: Rate of branch 1.
        mu2: Rate of branch 2.

    Returns:
        float: Expected maximum service time.
    """
    return float(jpype.JPackage('jline').api.fj.FJ_xmaxKt.fj_xmax_hyperexp(
        jpype.JInt(K), jpype.JDouble(p1), jpype.JDouble(mu1), jpype.JDouble(mu2)
    ))


def fj_xmax_normal(K, mu, sigma, method='johnson'):
    """
    Expected maximum and variance for K i.i.d. normal random variables.

    Methods:
    - "johnson": Johnson et al. approximation (default)
    - "arnold": Arnold approximation
    - "corrected": With Petzold bias correction

    Args:
        K: Number of random variables (K >= 2).
        mu: Mean of the normal distribution.
        sigma: Standard deviation.
        method: Approximation method ("johnson", "arnold", or "corrected").

    Returns:
        tuple: (Xmax, Vmax) expected maximum and its variance.
    """
    result = jpype.JPackage('jline').api.fj.FJ_xmaxKt.fj_xmax_normal(
        jpype.JInt(K), jpype.JDouble(mu), jpype.JDouble(sigma), str(method)
    )
    # Kotlin data class FJXmaxNormalResult: component1()=Xmax, component2()=Vmax
    return float(result.component1()), float(result.component2())


def fj_xmax_pareto(K, beta, k=None):
    """
    Expected maximum of K i.i.d. Pareto random variables.

    Pareto CDF: F(x) = 1 - (k / (k + x))^beta, x >= 0

    Args:
        K: Number of random variables (positive integer).
        beta: Shape parameter (beta > 2 for finite moments).
        k: Scale parameter (default: beta - 1 for mean = 1).

    Returns:
        float: Expected maximum.
    """
    if k is None:
        return float(jpype.JPackage('jline').api.fj.FJ_xmaxKt.fj_xmax_pareto(
            jpype.JInt(K), jpype.JDouble(beta)
        ))
    else:
        return float(jpype.JPackage('jline').api.fj.FJ_xmaxKt.fj_xmax_pareto(
            jpype.JInt(K), jpype.JDouble(beta), jpype.JDouble(k)
        ))


def fj_xmax_pareto_char_max(K, beta, k):
    """
    Characteristic maximum M_K for Pareto distribution.

    Args:
        K: Number of random variables (positive integer).
        beta: Shape parameter (beta > 1).
        k: Scale parameter.

    Returns:
        float: Characteristic maximum M_K.
    """
    return float(jpype.JPackage('jline').api.fj.FJ_xmaxKt.fj_xmax_pareto_char_max(
        jpype.JInt(K), jpype.JDouble(beta), jpype.JDouble(k)
    ))


def fj_xmax_approx(K, muX, sigmaX, dist_type='exp'):
    """
    Mean-variance approximation for expected maximum of K random variables.

    X_K^max ~ mu_X + sigma_X * G(K)
    where G(K) depends on distribution type.

    Args:
        K: Number of random variables (positive integer).
        muX: Mean of the distribution.
        sigmaX: Standard deviation.
        dist_type: Distribution type: "exp", "uniform", "evd", or "bound".

    Returns:
        float: Expected maximum.
    """
    return float(jpype.JPackage('jline').api.fj.FJ_xmaxKt.fj_xmax_approx(
        jpype.JInt(K), jpype.JDouble(muX), jpype.JDouble(sigmaX), str(dist_type)
    ))


def fj_xmax_emma(K, mu):
    """
    Expected maximum using EMMA (Expected Maximum from Marginal Approximation).

    Based on: E[Y_K] ~ F^{-1}(phi^{1/K}) where phi = exp(-exp(-gamma)) ~ 0.570376
    For exponential: E[Y_K] = -(1/mu) * ln(1 - phi^{1/K})

    Args:
        K: Number of random variables (positive integer).
        mu: Rate parameter for exponential distribution.

    Returns:
        float: Approximate expected maximum.
    """
    return float(jpype.JPackage('jline').api.fj.FJ_xmaxKt.fj_xmax_emma(
        jpype.JInt(K), jpype.JDouble(mu)
    ))


# ===========================================================================
# Characteristic maximum
# ===========================================================================

def fj_char_max_exp(K, mu):
    """
    Characteristic maximum M_K for exponential distribution.

    m_K = ln(K) / mu; M_K = H_K / mu (exact for exponential)

    Args:
        K: Number of random variables (positive integer).
        mu: Rate parameter (positive).

    Returns:
        tuple: (MK, mK) characteristic maximum and threshold.
    """
    result = jpype.JPackage('jline').api.fj.FJ_char_maxKt.fj_char_max_exp(
        jpype.JInt(K), jpype.JDouble(mu)
    )
    # Kotlin data class: use component1() for MK and component2() for mK
    return float(result.component1()), float(result.component2())


def fj_char_max_erlang(K, kStages, mu):
    """
    Characteristic maximum M_K for Erlang-k distribution.

    Args:
        K: Number of random variables (positive integer).
        kStages: Number of Erlang stages (positive integer).
        mu: Rate parameter per stage (positive).

    Returns:
        tuple: (MK, mK) characteristic maximum and threshold.
    """
    result = jpype.JPackage('jline').api.fj.FJ_char_maxKt.fj_char_max_erlang(
        jpype.JInt(K), jpype.JInt(kStages), jpype.JDouble(mu)
    )
    # Kotlin data class: use component1() for MK and component2() for mK
    return float(result.component1()), float(result.component2())


# ===========================================================================
# Order statistics
# ===========================================================================

def fj_gk_bound(K, type_='exp'):
    """
    Compute G(K) factor for expected maximum approximation.

    G(K) is used in: X_K^max ~ mu_X + sigma_X * G(K)

    Available types:
    - "exp": G(K) = H_K - 1
    - "uniform": G(K) = sqrt(3) * (K-1) / (K+1)
    - "evd": G(K) = sqrt(6) * ln(K) / pi
    - "bound": G(K) = (K-1) / sqrt(2K-1) (upper bound for any distribution)

    Args:
        K: Number of random variables (positive integer).
        type_: Distribution type: "exp", "uniform", "evd", or "bound".

    Returns:
        float: G(K) value.
    """
    return float(jpype.JPackage('jline').api.fj.FJ_order_statKt.fj_gk_bound(
        jpype.JInt(K), str(type_)
    ))


def fj_gk_bound_all(K):
    """
    Compute all G(K) bound factors.

    Args:
        K: Number of random variables (positive integer).

    Returns:
        dict: All G(K) values:
            - K (int): Number of random variables
            - exponential (float): G(K) for exponential
            - uniform (float): G(K) for uniform
            - evd (float): G(K) for extreme value distribution
            - upperBound (float): Upper bound for any distribution
    """
    result = jpype.JPackage('jline').api.fj.FJ_order_statKt.fj_gk_bound_all(jpype.JInt(K))
    # Kotlin data class GKBoundResult: component1()=K, component2()=exponential,
    #   component3()=uniform, component4()=evd, component5()=upperBound
    return {
        'K': int(result.component1()),
        'exponential': float(result.component2()),
        'uniform': float(result.component3()),
        'evd': float(result.component4()),
        'upperBound': float(result.component5()),
    }


def fj_order_stat_cdf(FXy, k, K):
    """
    CDF of k-th order statistic of K i.i.d. random variables.

    For maximum (k=K): F_{Y_K}(y) = [F_X(y)]^K
    For general k: F_{Y_k}(y) = sum_{j=k}^{K} C(K,j) * F_X(y)^j * (1-F_X(y))^{K-j}

    Args:
        FXy: CDF value F_X(y) at point y (0 <= FXy <= 1).
        k: Order of the statistic (1 = minimum, K = maximum).
        K: Total number of random variables.

    Returns:
        float: CDF of k-th order statistic.
    """
    return float(jpype.JPackage('jline').api.fj.FJ_order_statKt.fj_order_stat_cdf(
        jpype.JDouble(FXy), jpype.JInt(k), jpype.JInt(K)
    ))


# ===========================================================================
# Quantile approximations
# ===========================================================================

def fj_quantile(K, q):
    """
    Quantile approximation for maximum of K random variables using Gumbel EVD.

    x(K,q) ~ ln(K) - ln(ln(1/q))

    Note: This approximation is inaccurate for small K.

    Args:
        K: Number of random variables (positive integer).
        q: Quantile probability (0 < q < 1), or array of probabilities.

    Returns:
        float or numpy.ndarray: q-th quantile(s) of the maximum.
    """
    if isinstance(q, (list, np.ndarray)):
        q_arr = np.asarray(q, dtype=float)
        java_arr = jpype.JArray(jpype.JDouble)(len(q_arr))
        for i in range(len(q_arr)):
            java_arr[i] = q_arr[i]
        result = jpype.JPackage('jline').api.fj.FJ_quantileKt.fj_quantile(
            jpype.JInt(K), java_arr
        )
        return np.array([float(result[i]) for i in range(len(result))])
    else:
        return float(jpype.JPackage('jline').api.fj.FJ_quantileKt.fj_quantile(
            jpype.JInt(K), jpype.JDouble(q)
        ))


# ===========================================================================
# Split-Merge throughput
# ===========================================================================

def fj_sm_tput(K, mu):
    """
    Maximum throughput for Split-Merge queueing system.

    lambda_K^SM = mu / H_K

    In SM, all tasks of a request must complete before the next request
    can be issued. The F/J maximum throughput is lambda_K^FJ = mu, which
    exceeds SM throughput by a factor H_K.

    Args:
        K: Number of parallel servers (positive integer).
        mu: Service rate at each server.

    Returns:
        float: Maximum throughput lambda_K^SM.
    """
    return float(jpype.JPackage('jline').api.fj.FJ_sm_tputKt.fj_sm_tput(
        jpype.JInt(K), jpype.JDouble(mu)
    ))


# ===========================================================================
# Synchronization delay
# ===========================================================================

def fj_synch_delay(lambda_, mu):
    """
    Mean synchronization delay S_2(rho) for two-way Fork-Join.

    S_2(rho) = (1/2) * (1 - rho/4) * R(rho)

    The F/J response time decomposes as:
    R_2^{F/J}(rho) = R(rho) + S_2(rho)

    Args:
        lambda_: Arrival rate.
        mu: Service rate (mu > lambda for stability).

    Returns:
        float: Mean synchronization delay.
    """
    return float(jpype.JPackage('jline').api.fj.FJ_synch_delayKt.fj_synch_delay(
        jpype.JDouble(lambda_), jpype.JDouble(mu)
    ))


# ===========================================================================
# Topology validation and parameter extraction
# ===========================================================================

def fj_is_fj(sn):
    """
    Check if a network has Fork-Join topology suitable for analytical analysis.

    Validates: Source -> Fork -> K Queues -> Join -> Sink topology,
    open classes only, homogeneous service, FCFS or PS scheduling.

    Args:
        sn: NetworkStruct from model.getStruct().

    Returns:
        tuple: (isValid, fjInfo) where fjInfo is a dict or None. The dict contains:
            - K (int): Number of parallel queues
            - forkIdx (int): Fork node index
            - joinIdx (int): Join node index
            - queueIndices (list of int): Parallel queue node indices
            - sourceIdx (int): Source node index
            - sinkIdx (int): Sink node index
            - isValid (bool): Whether topology is valid
    """
    result = jpype.JPackage('jline').api.fj.FJValidationKt.isFJ(sn)
    # Kotlin Pair: getFirst()/getSecond()
    is_valid = bool(result.getFirst())
    fj_info_java = result.getSecond()
    if fj_info_java is None:
        return is_valid, None

    # FJInfo data class: component1()=K, component2()=forkIdx, component3()=joinIdx,
    #   component4()=queueIndices, component5()=sourceIdx, component6()=sinkIdx,
    #   component7()=isValid
    queue_indices = []
    java_indices = fj_info_java.component4()  # queueIndices (IntArray)
    for i in range(len(java_indices)):
        queue_indices.append(int(java_indices[i]))

    fj_info = {
        'K': int(fj_info_java.component1()),
        'forkIdx': int(fj_info_java.component2()),
        'joinIdx': int(fj_info_java.component3()),
        'queueIndices': queue_indices,
        'sourceIdx': int(fj_info_java.component5()),
        'sinkIdx': int(fj_info_java.component6()),
        'isValid': bool(fj_info_java.component7()),
    }
    return is_valid, fj_info
