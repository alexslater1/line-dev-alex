
"""
Age of Information (AoI) analysis functions.

This module provides analytical methods for computing Age of Information
metrics in queueing systems. AoI measures the freshness of information
in status-update systems where a source sends updates through a queue
to a monitor.

Supported queue types:
- M/M/1, M/D/1, D/M/1 (FCFS and LCFS-PR)
- M/GI/1, GI/M/1 (FCFS, LCFS-PR, LCFS-D, LCFS-S)
- General PH/PH distributions via LST functions

Metrics computed:
- Mean AoI (average staleness of information)
- Variance of AoI
- Peak AoI (mean of maximum staleness per cycle)
- LST of AoI distribution (where available)

References:
    Inoue et al., "A General Formula for the Stationary Distribution of
    the Age of Information and Its Application to Single-Server Queues",
    IEEE Transactions on Information Theory, 2019.
"""

import jpype
import numpy as np
from line_solver import jlineMatrixToArray, jlineMatrixFromArray


# ===========================================================================
# AoI topology validation and parameter extraction
# ===========================================================================

def aoi_is_aoi(sn):
    """
    Check if a network structure is a valid AoI topology for analytical analysis.

    Requirements for a valid AoI topology:
    - Single open class
    - Source -> Queue -> Sink topology
    - Queue capacity = 1 (bufferless) or 2 (single-buffer)
    - Single server
    - FCFS, LCFS, or LCFSPR scheduling
    - For capacity=2: arrivals must be exponential (Poisson)

    Args:
        sn: NetworkStruct from model.getStruct().

    Returns:
        dict: Validation result containing:
            - isAoI (bool): True if valid AoI topology
            - sourceIdx (int): Node index of the Source
            - queueIdx (int): Node index of the Queue
            - sinkIdx (int): Node index of the Sink
            - sourceStation (int): Station index of the Source
            - queueStation (int): Station index of the Queue
            - capacity (int): Queue capacity (1=bufferless, 2=single-buffer)
            - schedStrategy (str): Scheduling strategy name
            - systemType (str): "bufferless" or "singlebuffer"
            - errorMsg (str): Error message if not valid
    """
    result = jpype.JPackage('jline').api.aoi.Aoi_is_aoiKt.aoi_is_aoi(sn)
    # Use component functions for reliable cross-language access with Kotlin data classes
    return {
        'isAoI': bool(result.component1()),       # isAoI
        'sourceIdx': int(result.component2()),     # sourceIdx
        'queueIdx': int(result.component3()),      # queueIdx
        'sinkIdx': int(result.component4()),       # sinkIdx
        'sourceStation': int(result.component5()), # sourceStation
        'queueStation': int(result.component6()),  # queueStation
        'capacity': int(result.component7()),      # capacity
        'schedStrategy': str(result.component8()), # schedStrategy
        'systemType': str(result.component9()),    # systemType
        'errorMsg': str(result.component10()),     # errorMsg
    }


def aoi_extract_params(sn, aoi_info, aoi_preemption=float('nan')):
    """
    Extract parameters from a LINE network for AoI analysis.

    For bufferless systems (capacity=1): extracts (tau, T, sigma, S, p)
    For single-buffer systems (capacity=2): extracts (lambda, sigma, S, r)

    Args:
        sn: NetworkStruct from model.getStruct().
        aoi_info: Java AoiValidationResult from aoi_is_aoi (pass the raw Java object).
        aoi_preemption: Optional preemption probability override (0-1), or NaN for auto.

    Returns:
        dict: Extracted parameters containing:
            - tau (numpy.ndarray or None): Arrival initial probability vector (bufferless)
            - T (numpy.ndarray or None): Arrival sub-generator matrix (bufferless)
            - sigma (numpy.ndarray): Service initial probability vector
            - S (numpy.ndarray): Service sub-generator matrix
            - p (float): Preemption probability (bufferless), NaN for single-buffer
            - lambda_ (float): Arrival rate (single-buffer), NaN for bufferless
            - r (float): Replacement probability (single-buffer), NaN for bufferless
            - systemType (str): "bufferless" or "singlebuffer"
            - arrivalType (str): "PH" or "M"
    """
    result = jpype.JPackage('jline').api.aoi.Aoi_extract_paramsKt.aoi_extract_params(
        sn, aoi_info, jpype.JDouble(aoi_preemption)
    )
    # Use component functions for reliable Kotlin data class access
    tau_mat = result.component1()    # tau (Matrix or null)
    T_mat = result.component2()      # T (Matrix or null)
    sigma_mat = result.component3()  # sigma (Matrix)
    S_mat = result.component4()      # S (Matrix)
    p_val = result.component5()      # p (Double)
    lambda_val = result.component6() # lambda (Double)
    r_val = result.component7()      # r (Double)
    sysType = result.component8()    # systemType (String)
    arrType = result.component9()    # arrivalType (String)
    return {
        'tau': jlineMatrixToArray(tau_mat) if tau_mat is not None else None,
        'T': jlineMatrixToArray(T_mat) if T_mat is not None else None,
        'sigma': jlineMatrixToArray(sigma_mat),
        'S': jlineMatrixToArray(S_mat),
        'p': float(p_val),
        'lambda_': float(lambda_val),
        'r': float(r_val),
        'systemType': str(sysType),
        'arrivalType': str(arrType),
    }


def aoi_dist2ph(proc):
    """
    Convert LINE {D0, D1} MAP process to PH (alpha, T) format for AoI analysis.

    The initial probability vector alpha is computed from the stationary
    distribution of the MAP at completion epochs.

    Args:
        proc: LINE process representation as Java MatrixCell {D0, D1}.

    Returns:
        tuple: (alpha, T) where:
            - alpha (numpy.ndarray): Initial probability row vector (1 x n)
            - T (numpy.ndarray): Sub-generator matrix (n x n)
    """
    result = jpype.JPackage('jline').api.aoi.Aoi_dist2phKt.aoi_dist2ph(proc)
    # Kotlin data class Aoi_dist2phResult: component1()=alpha, component2()=T
    alpha = jlineMatrixToArray(result.component1())
    T = jlineMatrixToArray(result.component2())
    return alpha, T


# ===========================================================================
# LST (Laplace-Stieltjes Transform) functions
# ===========================================================================

def aoi_lst_exp(mu):
    """
    Laplace-Stieltjes transform for exponential distribution.

    H*(s) = mu / (mu + s)

    Args:
        mu: Rate parameter (mean = 1/mu), must be positive.

    Returns:
        Java LstFunction object that can evaluate H*(s).
    """
    return jpype.JPackage('jline').api.aoi.Aoi_lstKt.aoi_lst_exp(jpype.JDouble(mu))


def aoi_lst_det(d):
    """
    Laplace-Stieltjes transform for deterministic (constant) distribution.

    H*(s) = exp(-s * d)

    Args:
        d: Constant value, must be positive.

    Returns:
        Java LstFunction object that can evaluate H*(s).
    """
    return jpype.JPackage('jline').api.aoi.Aoi_lstKt.aoi_lst_det(jpype.JDouble(d))


def aoi_lst_erlang(k, mu):
    """
    Laplace-Stieltjes transform for Erlang distribution.

    H*(s) = (mu / (mu + s))^k

    Args:
        k: Shape parameter (number of phases), positive integer.
        mu: Rate parameter per phase (mean = k/mu), must be positive.

    Returns:
        Java LstFunction object that can evaluate H*(s).
    """
    return jpype.JPackage('jline').api.aoi.Aoi_lstKt.aoi_lst_erlang(
        jpype.JInt(k), jpype.JDouble(mu)
    )


def aoi_lst_ph(alpha, T):
    """
    Laplace-Stieltjes transform for phase-type distribution.

    H*(s) = alpha * (s*I - T)^{-1} * (-T * e)

    Args:
        alpha: Initial probability row vector (1 x n), array-like.
        T: Sub-generator matrix (n x n), array-like.

    Returns:
        Java LstFunction object that can evaluate H*(s).
    """
    return jpype.JPackage('jline').api.aoi.Aoi_lstKt.aoi_lst_ph(
        jlineMatrixFromArray(alpha), jlineMatrixFromArray(T)
    )


# ===========================================================================
# FCFS queue AoI analysis
# ===========================================================================

def aoi_fcfs_mm1(lambda_, mu):
    """
    Mean, variance, and peak AoI for M/M/1 FCFS queue.

    Uses closed-form formulas from Inoue et al. (2019):
        Mean AoI: E[A] = (1/mu) * (1 + 1/rho + rho^2/(1-rho))
        Peak AoI: E[Apeak] = (1/mu) * (1 + 1/rho + rho/(1-rho))

    Args:
        lambda_: Arrival rate (Poisson), must be positive.
        mu: Service rate (exponential), must be positive.

    Returns:
        tuple: (meanAoI, varAoI, peakAoI)
    """
    result = jpype.JPackage('jline').api.aoi.Aoi_fcfs_mm1Kt.aoi_fcfs_mm1(
        jpype.JDouble(lambda_), jpype.JDouble(mu)
    )
    # Kotlin data class AoiResult: component1()=meanAoI, component2()=varAoI, component3()=peakAoI
    return float(result.component1()), float(result.component2()), float(result.component3())


def aoi_fcfs_md1(lambda_, d):
    """
    Mean, variance, and peak AoI for M/D/1 FCFS queue.

    M/D/1: Poisson arrivals with rate lambda, deterministic service time d.

    Args:
        lambda_: Arrival rate (Poisson), must be positive.
        d: Deterministic service time, must be positive.

    Returns:
        tuple: (meanAoI, varAoI, peakAoI)
    """
    result = jpype.JPackage('jline').api.aoi.Aoi_fcfs_md1Kt.aoi_fcfs_md1(
        jpype.JDouble(lambda_), jpype.JDouble(d)
    )
    # Kotlin data class AoiResult: component1()=meanAoI, component2()=varAoI, component3()=peakAoI
    return float(result.component1()), float(result.component2()), float(result.component3())


def aoi_fcfs_dm1(tau, mu):
    """
    Mean, variance, and peak AoI for D/M/1 FCFS queue.

    D/M/1: Deterministic arrivals with interarrival time tau,
    exponential service with rate mu.

    Args:
        tau: Deterministic interarrival time, must be positive.
        mu: Service rate (exponential), must be positive.

    Returns:
        tuple: (meanAoI, varAoI, peakAoI)
    """
    result = jpype.JPackage('jline').api.aoi.Aoi_fcfs_dm1Kt.aoi_fcfs_dm1(
        jpype.JDouble(tau), jpype.JDouble(mu)
    )
    # Kotlin data class AoiResult: component1()=meanAoI, component2()=varAoI, component3()=peakAoI
    return float(result.component1()), float(result.component2()), float(result.component3())


def aoi_fcfs_gim1(Y_lst, mu, E_Y, E_Y2):
    """
    Mean AoI and LST for GI/M/1 FCFS queue.

    GI/M/1: General independent arrivals, exponential service with rate mu.

    Args:
        Y_lst: LST of interarrival time distribution (Java LstFunction).
        mu: Service rate (exponential), must be positive.
        E_Y: Mean interarrival time, must be positive.
        E_Y2: Second moment of interarrival time (E_Y2 >= E_Y^2).

    Returns:
        tuple: (meanAoI, lstAoI, peakAoI) where lstAoI is a Java LstFunction or None.
    """
    result = jpype.JPackage('jline').api.aoi.Aoi_fcfs_gim1Kt.aoi_fcfs_gim1(
        Y_lst, jpype.JDouble(mu), jpype.JDouble(E_Y), jpype.JDouble(E_Y2)
    )
    # Kotlin data class AoiLstResult: component1()=meanAoI, component2()=lstAoI, component3()=peakAoI
    lst_aoi = result.component2() if result.component2() is not None else None
    return float(result.component1()), lst_aoi, float(result.component3())


def aoi_fcfs_mgi1(lambda_, H_lst, E_H, E_H2):
    """
    Mean AoI and LST for M/GI/1 FCFS queue.

    M/GI/1: Poisson arrivals with rate lambda, general independent service.

    Args:
        lambda_: Arrival rate (Poisson), must be positive.
        H_lst: LST of service time distribution (Java LstFunction).
        E_H: Mean service time, must be positive.
        E_H2: Second moment of service time (E_H2 >= E_H^2).

    Returns:
        tuple: (meanAoI, lstAoI, peakAoI) where lstAoI is a Java LstFunction or None.
    """
    result = jpype.JPackage('jline').api.aoi.Aoi_fcfs_mgi1Kt.aoi_fcfs_mgi1(
        jpype.JDouble(lambda_), H_lst, jpype.JDouble(E_H), jpype.JDouble(E_H2)
    )
    # Kotlin data class AoiLstResult: component1()=meanAoI, component2()=lstAoI, component3()=peakAoI
    lst_aoi = result.component2() if result.component2() is not None else None
    return float(result.component1()), lst_aoi, float(result.component3())


# ===========================================================================
# LCFS-PR (preemptive) queue AoI analysis
# ===========================================================================

def aoi_lcfspr_mm1(lambda_, mu):
    """
    Mean, variance, and peak AoI for M/M/1 preemptive LCFS queue.

    In LCFS-PR, a new update arriving preempts the current one in service.
    Mean AoI: E[A] = 1/mu + 1/lambda (always lower than FCFS).

    Args:
        lambda_: Arrival rate (Poisson), must be positive.
        mu: Service rate (exponential), must be positive.

    Returns:
        tuple: (meanAoI, varAoI, peakAoI)
    """
    result = jpype.JPackage('jline').api.aoi.Aoi_lcfspr_mm1Kt.aoi_lcfspr_mm1(
        jpype.JDouble(lambda_), jpype.JDouble(mu)
    )
    # Kotlin data class AoiResult: component1()=meanAoI, component2()=varAoI, component3()=peakAoI
    return float(result.component1()), float(result.component2()), float(result.component3())


def aoi_lcfspr_md1(lambda_, d):
    """
    Mean, variance, and peak AoI for M/D/1 preemptive LCFS queue.

    Args:
        lambda_: Arrival rate (Poisson), must be positive.
        d: Deterministic service time, must be positive.

    Returns:
        tuple: (meanAoI, varAoI, peakAoI)
    """
    result = jpype.JPackage('jline').api.aoi.Aoi_lcfspr_md1Kt.aoi_lcfspr_md1(
        jpype.JDouble(lambda_), jpype.JDouble(d)
    )
    # Kotlin data class AoiResult: component1()=meanAoI, component2()=varAoI, component3()=peakAoI
    return float(result.component1()), float(result.component2()), float(result.component3())


def aoi_lcfspr_dm1(tau, mu):
    """
    Mean, variance, and peak AoI for D/M/1 preemptive LCFS queue.

    Args:
        tau: Deterministic interarrival time, must be positive.
        mu: Service rate (exponential), must be positive.

    Returns:
        tuple: (meanAoI, varAoI, peakAoI)
    """
    result = jpype.JPackage('jline').api.aoi.Aoi_lcfspr_dm1Kt.aoi_lcfspr_dm1(
        jpype.JDouble(tau), jpype.JDouble(mu)
    )
    # Kotlin data class AoiResult: component1()=meanAoI, component2()=varAoI, component3()=peakAoI
    return float(result.component1()), float(result.component2()), float(result.component3())


def aoi_lcfspr_gim1(Y_lst, mu, E_Y, E_Y2):
    """
    Mean AoI and LST for GI/M/1 preemptive LCFS queue.

    For preemptive LCFS: E[A] = E[Y] + 1/mu
    LST: A*(s) = Y*(s) * (mu / (s + mu))

    Args:
        Y_lst: LST of interarrival time distribution (Java LstFunction).
        mu: Service rate (exponential), must be positive.
        E_Y: Mean interarrival time, must be positive.
        E_Y2: Second moment of interarrival time.

    Returns:
        tuple: (meanAoI, lstAoI, peakAoI) where lstAoI is a Java LstFunction or None.
    """
    result = jpype.JPackage('jline').api.aoi.Aoi_lcfspr_gim1Kt.aoi_lcfspr_gim1(
        Y_lst, jpype.JDouble(mu), jpype.JDouble(E_Y), jpype.JDouble(E_Y2)
    )
    # Kotlin data class AoiLstResult: component1()=meanAoI, component2()=lstAoI, component3()=peakAoI
    lst_aoi = result.component2() if result.component2() is not None else None
    return float(result.component1()), lst_aoi, float(result.component3())


def aoi_lcfspr_mgi1(lambda_, H_lst, E_H, E_H2):
    """
    Mean AoI and LST for M/GI/1 preemptive LCFS queue.

    For preemptive LCFS: E[A] = 1/lambda + E[H]
    LST: A*(s) = (lambda / (s + lambda)) * H*(s)

    Args:
        lambda_: Arrival rate (Poisson), must be positive.
        H_lst: LST of service time distribution (Java LstFunction).
        E_H: Mean service time, must be positive.
        E_H2: Second moment of service time.

    Returns:
        tuple: (meanAoI, lstAoI, peakAoI) where lstAoI is a Java LstFunction or None.
    """
    result = jpype.JPackage('jline').api.aoi.Aoi_lcfspr_mgi1Kt.aoi_lcfspr_mgi1(
        jpype.JDouble(lambda_), H_lst, jpype.JDouble(E_H), jpype.JDouble(E_H2)
    )
    # Kotlin data class AoiLstResult: component1()=meanAoI, component2()=lstAoI, component3()=peakAoI
    lst_aoi = result.component2() if result.component2() is not None else None
    return float(result.component1()), lst_aoi, float(result.component3())


# ===========================================================================
# LCFS-D (non-preemptive with discarding) queue AoI analysis
# ===========================================================================

def aoi_lcfsd_gim1(Y_lst, mu, E_Y, E_Y2):
    """
    Mean AoI for GI/M/1 non-preemptive LCFS-D (discarding) queue.

    In LCFS-D, when a new update arrives while the server is busy,
    any waiting update is discarded and replaced by the new one.

    Args:
        Y_lst: LST of interarrival time distribution (Java LstFunction).
        mu: Service rate (exponential), must be positive.
        E_Y: Mean interarrival time, must be positive.
        E_Y2: Second moment of interarrival time.

    Returns:
        tuple: (meanAoI, lstAoI, peakAoI) where lstAoI is None (complex for LCFS-D).
    """
    result = jpype.JPackage('jline').api.aoi.Aoi_lcfsd_gim1Kt.aoi_lcfsd_gim1(
        Y_lst, jpype.JDouble(mu), jpype.JDouble(E_Y), jpype.JDouble(E_Y2)
    )
    # Kotlin data class AoiLstResult: component1()=meanAoI, component2()=lstAoI, component3()=peakAoI
    lst_aoi = result.component2() if result.component2() is not None else None
    return float(result.component1()), lst_aoi, float(result.component3())


def aoi_lcfsd_mgi1(lambda_, H_lst, E_H, E_H2):
    """
    Mean AoI for M/GI/1 non-preemptive LCFS-D (discarding) queue.

    Also known as M/GI/1/2* (buffer size 2 with replacement).

    Args:
        lambda_: Arrival rate (Poisson), must be positive.
        H_lst: LST of service time distribution (Java LstFunction, may be None).
        E_H: Mean service time, must be positive.
        E_H2: Second moment of service time (E_H2 >= E_H^2).

    Returns:
        tuple: (meanAoI, lstAoI, peakAoI) where lstAoI is None (complex for LCFS-D).
    """
    result = jpype.JPackage('jline').api.aoi.Aoi_lcfsd_mgi1Kt.aoi_lcfsd_mgi1(
        jpype.JDouble(lambda_), H_lst, jpype.JDouble(E_H), jpype.JDouble(E_H2)
    )
    # Kotlin data class AoiLstResult: component1()=meanAoI, component2()=lstAoI, component3()=peakAoI
    lst_aoi = result.component2() if result.component2() is not None else None
    return float(result.component1()), lst_aoi, float(result.component3())


# ===========================================================================
# LCFS-S (non-preemptive with set-aside) queue AoI analysis
# ===========================================================================

def aoi_lcfss_gim1(Y_lst, mu, E_Y, E_Y2):
    """
    Mean AoI for GI/M/1 non-preemptive LCFS-S (set-aside) queue.

    In LCFS-S, when a new update arrives while the server is busy,
    the new update waits; when service completes, the most recent
    waiting update is served next (older ones are "set aside").

    Args:
        Y_lst: LST of interarrival time distribution (Java LstFunction).
        mu: Service rate (exponential), must be positive.
        E_Y: Mean interarrival time, must be positive.
        E_Y2: Second moment of interarrival time.

    Returns:
        tuple: (meanAoI, lstAoI, peakAoI) where lstAoI is None (complex for LCFS-S).
    """
    result = jpype.JPackage('jline').api.aoi.Aoi_lcfss_gim1Kt.aoi_lcfss_gim1(
        Y_lst, jpype.JDouble(mu), jpype.JDouble(E_Y), jpype.JDouble(E_Y2)
    )
    # Kotlin data class AoiLstResult: component1()=meanAoI, component2()=lstAoI, component3()=peakAoI
    lst_aoi = result.component2() if result.component2() is not None else None
    return float(result.component1()), lst_aoi, float(result.component3())


def aoi_lcfss_mgi1(lambda_, H_lst, E_H, E_H2):
    """
    Mean AoI for M/GI/1 non-preemptive LCFS-S (set-aside) queue.

    The formula involves the busy period distribution of M/G/1:
        E[A] = 1/lambda + E[H] + lambda * E[H^2] / (2 * (1 - rho)^2)

    Args:
        lambda_: Arrival rate (Poisson), must be positive.
        H_lst: LST of service time distribution (Java LstFunction, may be None).
        E_H: Mean service time, must be positive.
        E_H2: Second moment of service time (E_H2 >= E_H^2).

    Returns:
        tuple: (meanAoI, lstAoI, peakAoI) where lstAoI is None (complex for LCFS-S).
    """
    result = jpype.JPackage('jline').api.aoi.Aoi_lcfss_mgi1Kt.aoi_lcfss_mgi1(
        jpype.JDouble(lambda_), H_lst, jpype.JDouble(E_H), jpype.JDouble(E_H2)
    )
    # Kotlin data class AoiLstResult: component1()=meanAoI, component2()=lstAoI, component3()=peakAoI
    lst_aoi = result.component2() if result.component2() is not None else None
    return float(result.component1()), lst_aoi, float(result.component3())


# ===========================================================================
# MFQ-based AoI solvers (bufferless and single-buffer)
# ===========================================================================

def solve_bufferless(tau, T, sigma, S, p=0.0):
    """
    Solve bufferless AoI system (PH/PH/1/1 or PH/PH/1/1*) using MFQ.

    Computes matrix exponential parameters for Age of Information and Peak Age
    of Information distributions in a bufferless (capacity 1) queue where
    arrivals preempt service with probability p.

    Args:
        tau: Arrival process initial probability vector (1 x k), array-like.
        T: Arrival process sub-generator matrix (k x k), array-like.
        sigma: Service process initial probability vector (1 x l), array-like.
        S: Service process sub-generator matrix (l x l), array-like.
        p: Preemption probability (default 0 = FCFS, 1 = LCFSPR).

    Returns:
        dict: Solution dictionary with keys:
            - 'AoI_g', 'AoI_A', 'AoI_h': Matrix exponential parameters for AoI
              CDF: F(t) = 1 - g @ expm(A*t) @ h
            - 'AoI_mean', 'AoI_var': Mean and variance of AoI
            - 'PAoI_g', 'PAoI_A', 'PAoI_h': Matrix exponential for Peak AoI
            - 'PAoI_mean', 'PAoI_var': Mean and variance of Peak AoI
            - 'systemType': "bufferless"
            - 'preemption': Preemption probability used
    """
    result = jpype.JPackage('jline').api.aoi.Aoi_solve_bufferlessKt.aoi_solve_bufferless(
        jlineMatrixFromArray(tau), jlineMatrixFromArray(T),
        jlineMatrixFromArray(sigma), jlineMatrixFromArray(S),
        jpype.JDouble(p)
    )
    # Kotlin data class AoiMfqResult with 12 components
    return {
        'AoI_g': jlineMatrixToArray(result.component1()),      # aoiG
        'AoI_A': jlineMatrixToArray(result.component2()),      # aoiA
        'AoI_h': jlineMatrixToArray(result.component3()),      # aoiH
        'AoI_mean': float(result.component4()),                # aoiMean
        'AoI_var': float(result.component5()),                 # aoiVar
        'PAoI_g': jlineMatrixToArray(result.component6()),     # paoiG
        'PAoI_A': jlineMatrixToArray(result.component7()),     # paoiA
        'PAoI_h': jlineMatrixToArray(result.component8()),     # paoiH
        'PAoI_mean': float(result.component9()),               # paoiMean
        'PAoI_var': float(result.component10()),               # paoiVar
        'systemType': str(result.component11()),               # systemType
        'preemption': float(result.component12()),             # preemption
    }


def solve_singlebuffer(lambda_rate, sigma, S, r=0.0):
    """
    Solve single-buffer AoI system (M/PH/1/2 or M/PH/1/2*) using MFQ.

    Computes matrix exponential parameters for Age of Information and Peak Age
    of Information distributions in a single-buffer queue with exponential
    (Poisson) arrivals and phase-type service.

    Args:
        lambda_rate: Poisson arrival rate (exponential inter-arrivals).
        sigma: Service process initial probability vector (1 x l), array-like.
        S: Service process sub-generator matrix (l x l), array-like.
        r: Replacement probability (default 0 = FCFS, 1 = replacement).

    Returns:
        dict: Solution dictionary with keys:
            - 'AoI_g', 'AoI_A', 'AoI_h': Matrix exponential parameters for AoI
              CDF: F(t) = 1 - g @ expm(A*t) @ h
            - 'AoI_mean', 'AoI_var': Mean and variance of AoI
            - 'PAoI_g', 'PAoI_A', 'PAoI_h': Matrix exponential for Peak AoI
            - 'PAoI_mean', 'PAoI_var': Mean and variance of Peak AoI
            - 'systemType': "singlebuffer"
            - 'preemption': Replacement probability used
    """
    result = jpype.JPackage('jline').api.aoi.Aoi_solve_singlebufferKt.aoi_solve_singlebuffer(
        jpype.JDouble(lambda_rate),
        jlineMatrixFromArray(sigma), jlineMatrixFromArray(S),
        jpype.JDouble(r)
    )
    # Kotlin data class AoiMfqResult with 12 components
    return {
        'AoI_g': jlineMatrixToArray(result.component1()),      # aoiG
        'AoI_A': jlineMatrixToArray(result.component2()),      # aoiA
        'AoI_h': jlineMatrixToArray(result.component3()),      # aoiH
        'AoI_mean': float(result.component4()),                # aoiMean
        'AoI_var': float(result.component5()),                 # aoiVar
        'PAoI_g': jlineMatrixToArray(result.component6()),     # paoiG
        'PAoI_A': jlineMatrixToArray(result.component7()),     # paoiA
        'PAoI_h': jlineMatrixToArray(result.component8()),     # paoiH
        'PAoI_mean': float(result.component9()),               # paoiMean
        'PAoI_var': float(result.component10()),               # paoiVar
        'systemType': str(result.component11()),               # systemType
        'preemption': float(result.component12()),             # preemption
    }
