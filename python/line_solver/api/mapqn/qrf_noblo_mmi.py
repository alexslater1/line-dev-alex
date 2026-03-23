"""
QRF No-Blocking MMI (Minimize Mutual Information) Approximation.

Solves a constrained NLP to approximate performance metrics of single-class
closed queueing networks with PH service, using mutual information
minimization as the objective.

Port of MATLAB qrf_noblo_mmi.m.
"""

import numpy as np
from scipy.optimize import minimize

from .qrf_noblo_common import (
    sub_qrfvar,
    sub_qrfcon_noblo,
    mmi_objective,
    compute_num_vars,
    extract_results,
    build_q_from_mu_v_rt,
)


def qrf_noblo_mmi(M, MR, K, N, mu, v, rt):
    """QRF no-blocking MMI approximation.

    Args:
        M: Number of queues
        MR: Number of blocking configurations (typically 1 for no-blocking)
        K: Phases per queue, array of int [M]
        N: Total population
        mu: Completion rates [M, Kmax, Kmax]
        v: Background rates [M, Kmax, Kmax]
        rt: Routing matrix [M, M]

    Returns:
        UN: Utilization per queue [M]
        QN: Queue length per queue [M]
    """
    K = np.asarray(K, dtype=int)

    # No-blocking defaults
    MR = 1
    BB = np.zeros((1, M))
    F = np.full(M, N, dtype=int)

    # Build transition rates
    q = build_q_from_mu_v_rt(M, K, mu, v, rt)

    # Initial point
    num_vars = compute_num_vars(M, N, K, MR)
    x0 = np.zeros(num_vars)

    # Bounds: all variables in [0, 1]
    bounds = [(0.0, 1.0)] * num_vars

    # Constraint functions for scipy
    def ceq_fn(x):
        _, ceq = sub_qrfcon_noblo(x, q, M, MR, BB, F, N, K)
        return ceq

    def c_ineq_fn(x):
        c, _ = sub_qrfcon_noblo(x, q, M, MR, BB, F, N, K)
        return -c  # scipy ineq >= 0, MATLAB c <= 0

    constraints = [
        {'type': 'eq', 'fun': ceq_fn},
        {'type': 'ineq', 'fun': c_ineq_fn},
    ]

    result = minimize(
        lambda x: mmi_objective(x, M, N, K, F, MR),
        x0,
        method='SLSQP',
        bounds=bounds,
        constraints=constraints,
        options={'maxiter': 100, 'disp': False, 'ftol': 1e-8},
    )

    p2opt, _ = sub_qrfvar(result.x, M, N, K, MR)
    UN, QN = extract_results(p2opt, M, K, F, MR)

    return UN, QN
