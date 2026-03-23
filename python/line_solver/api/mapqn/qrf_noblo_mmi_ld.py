"""
QRF No-Blocking MMI with Load-Dependent Rates Approximation.

Solves a constrained NLP with load-dependent transition rates.
Uses mutual information minimization as the objective.

Port of MATLAB qrf_noblo_mmi_ld.m.
"""

import numpy as np
from scipy.optimize import minimize

from .qrf_noblo_common import (
    sub_qrfvar,
    sub_qrfcon_noblo,
    mmi_objective,
    compute_num_vars,
    extract_results,
    extract_mu_v_from_maps,
)


def qrf_noblo_mmi_ld(MAPs, N, rt, alpha=None):
    """QRF no-blocking MMI approximation with load-dependent rates.

    Args:
        MAPs: List of [D0, D1] pairs per queue
        N: Total population
        rt: Routing matrix [M, M]
        alpha: Load-dependent scaling factors [M, N]. If None, defaults to ones.

    Returns:
        UN: Utilization per queue [M]
        QN: Queue length per queue [M]
    """
    M = len(MAPs)
    K = np.array([MAPs[i][0].shape[0] for i in range(M)], dtype=int)

    if alpha is None:
        alpha = np.ones((M, N))

    mu, v = extract_mu_v_from_maps(MAPs, M, K)

    MR = 1
    BB = np.zeros((1, M))
    F = np.full(M, N, dtype=int)

    # Build 4D transition rates (non-load-dependent, used in constraints)
    Kmax = int(max(K))
    q = np.zeros((M, M, Kmax, Kmax))
    for i in range(M):
        for j in range(M):
            for k in range(K[i]):
                for h in range(K[i]):
                    if j != i:
                        q[i, j, k, h] = rt[i, j] * mu[i, k, h]
                    else:
                        q[i, j, k, h] = v[i, k, h] + rt[i, i] * mu[i, k, h]

    # Initial point: normalized uniform
    num_vars = compute_num_vars(M, N, K, MR)
    p2_size = M * (N + 1) * Kmax * M * (N + 1) * Kmax * MR
    px = np.ones(M * Kmax)
    px = px / np.sum(px)
    x0 = np.concatenate([np.ones(p2_size), px])
    x0 = x0 / np.sum(x0)

    # Bounds
    bounds = [(0.0, 1.0)] * num_vars

    # Constraint functions
    def ceq_fn(x):
        _, ceq = sub_qrfcon_noblo(x, q, M, MR, BB, F, N, K)
        return ceq

    def c_ineq_fn(x):
        c, _ = sub_qrfcon_noblo(x, q, M, MR, BB, F, N, K)
        return -c

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
