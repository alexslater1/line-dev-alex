"""
QRF No-Blocking NLP with Linear Constraint Matrices.

Builds sparse Aeq/beq and A/b matrices for linear constraints, then
solves an NLP with nonlinear objective (MEM) subject to these linear
constraints. This is computationally more efficient than callback-based
constraints for larger problems.

Port of MATLAB qrf_noblo_mmi_linear.m.
Note: Despite the filename containing 'mmi', the MATLAB implementation
uses the MEM objective. This port faithfully reproduces that behavior.
"""

import numpy as np
from scipy.optimize import minimize
from scipy.sparse import lil_matrix

from .qrf_noblo_common import (
    sub_qrfvar,
    mem_objective,
    compute_num_vars,
    extract_results,
    extract_mu_v_from_maps,
)


def _deltap2(j, nj, k, i, ni, h, m, M, N, K, MR):
    """Compute flat index for p2[j,nj,k,i,ni,h,m] in sparse constraint matrix.

    Returns the 0-based column index into the flat decision vector.
    All inputs are 0-based.
    """
    Kmax = int(max(K))
    idx = ((N + 1) * Kmax * M * (N + 1) * Kmax * MR) * j
    idx += (Kmax * M * (N + 1) * Kmax * MR) * nj
    idx += (M * (N + 1) * Kmax * MR) * k
    idx += ((N + 1) * Kmax * MR) * i
    idx += (Kmax * MR) * ni
    idx += MR * h
    idx += m
    return idx


def _deltae(i, k, M, N, K, MR):
    """Compute flat index for e[i,k] in sparse constraint matrix.

    Returns the 0-based column index. All inputs are 0-based.
    """
    Kmax = int(max(K))
    p2_size = M * (N + 1) * Kmax * M * (N + 1) * Kmax * MR
    return p2_size + Kmax * i + k


def qrf_noblo_mmi_linear(MAPs, N, rt, alpha=None):
    """QRF no-blocking NLP with linear constraint matrices.

    Builds sparse equality/inequality matrices and uses SLSQP with MEM objective.

    Args:
        MAPs: List of [D0, D1] pairs per queue
        N: Total population
        rt: Routing matrix [M, M]
        alpha: Load-dependent scaling [M, N]. If None, defaults to ones.

    Returns:
        UN: Utilization per queue [M]
        QN: Queue length per queue [M]
    """
    M = len(MAPs)
    K = np.array([MAPs[i][0].shape[0] for i in range(M)], dtype=int)
    Kmax = int(max(K))

    if alpha is None:
        alpha = np.ones((M, N))

    mu, v = extract_mu_v_from_maps(MAPs, M, K)

    MR = 1
    BB = np.zeros((MR, M))
    F = np.full(M, N, dtype=int)

    # Build transition rates (load-dependent version but stored as 4D for constraints)
    q = np.zeros((M, M, Kmax, Kmax))
    for i in range(M):
        for j in range(M):
            for k in range(K[i]):
                for h in range(K[i]):
                    if j != i:
                        q[i, j, k, h] = rt[i, j] * mu[i, k, h]
                    else:
                        q[i, j, k, h] = v[i, k, h] + rt[i, i] * mu[i, k, h]

    num_vars = compute_num_vars(M, N, K, MR)

    # Build sparse constraint matrices
    Aeq, beq, Aub, bub = _build_linear_constraints(q, M, MR, BB, F, N, K, num_vars)

    # Initial point: normalized uniform
    p2_size = M * (N + 1) * Kmax * M * (N + 1) * Kmax * MR
    px = np.ones(M * Kmax)
    px = px / np.sum(px)
    x0 = np.concatenate([np.ones(p2_size), px])
    x0 = x0 / np.sum(x0)

    # Bounds
    bounds = [(0.0, 1.0)] * num_vars

    # Convert sparse matrices to constraint functions for SLSQP
    Aeq_dense = Aeq.toarray()
    beq_arr = np.array(beq)
    Aub_dense = Aub.toarray() if Aub.shape[0] > 0 else np.zeros((0, num_vars))
    bub_arr = np.array(bub) if len(bub) > 0 else np.zeros(0)

    constraints = []
    if Aeq_dense.shape[0] > 0:
        constraints.append({
            'type': 'eq',
            'fun': lambda x: Aeq_dense @ x - beq_arr,
        })
    if Aub_dense.shape[0] > 0:
        constraints.append({
            'type': 'ineq',
            'fun': lambda x: -(Aub_dense @ x - bub_arr),  # A*x <= b => -(A*x - b) >= 0
        })

    result = minimize(
        lambda x: mem_objective(x, M, N, K, F, MR),
        x0,
        method='SLSQP',
        bounds=bounds,
        constraints=constraints,
        options={'maxiter': 100, 'disp': False, 'ftol': 1e-8},
    )

    p2opt, _ = sub_qrfvar(result.x, M, N, K, MR)
    UN, QN = extract_results(p2opt, M, K, F, MR)

    return UN, QN


def _build_linear_constraints(q, M, MR, BB, F, N, K, num_vars):
    """Build sparse Aeq, beq, Aub, bub matrices.

    Port of MATLAB sub_qrfcon() linear constraint version.

    Returns:
        Aeq: Sparse equality constraint matrix
        beq: Equality RHS vector
        Aub: Sparse inequality constraint matrix (A*x <= b)
        bub: Inequality RHS vector
    """
    Kmax = int(max(K))

    # Use lists and convert to sparse at end
    aeq_rows = []
    beq_vals = []
    aub_rows = []
    bub_vals = []

    # Temporary dense row accumulator
    def new_eq_row():
        return np.zeros(num_vars)

    # ONE: normalization
    for j in range(M):
        row = new_eq_row()
        for nj in range(N + 1):
            for k in range(K[j]):
                for m in range(MR):
                    idx = _deltap2(j, nj, k, j, nj, k, m, M, N, K, MR)
                    row[idx] += 1.0
        aeq_rows.append(row)
        beq_vals.append(1.0)

    # ZERO1: i==j and ni==nj and h!=k
    for j in range(M):
        for k in range(K[j]):
            for nj in range(N + 1):
                for i in range(M):
                    for h in range(K[i]):
                        for ni in range(N + 1):
                            for m in range(MR):
                                if i == j and nj == ni and h != k:
                                    row = new_eq_row()
                                    idx = _deltap2(j, nj, k, i, ni, h, m, M, N, K, MR)
                                    row[idx] = 1.0
                                    aeq_rows.append(row)
                                    beq_vals.append(0.0)

    # ZERO2: i==j and nj!=ni
    for j in range(M):
        for k in range(K[j]):
            for nj in range(N + 1):
                for i in range(M):
                    for h in range(K[i]):
                        for ni in range(N + 1):
                            for m in range(MR):
                                if i == j and nj != ni:
                                    row = new_eq_row()
                                    idx = _deltap2(j, nj, k, i, ni, h, m, M, N, K, MR)
                                    row[idx] = 1.0
                                    aeq_rows.append(row)
                                    beq_vals.append(0.0)

    # ZERO3: i!=j and nj+ni>N
    for j in range(M):
        for k in range(K[j]):
            for nj in range(N + 1):
                for i in range(M):
                    for h in range(K[i]):
                        for ni in range(N + 1):
                            for m in range(MR):
                                if i != j and nj + ni > N:
                                    row = new_eq_row()
                                    idx = _deltap2(j, nj, k, i, ni, h, m, M, N, K, MR)
                                    row[idx] = 1.0
                                    aeq_rows.append(row)
                                    beq_vals.append(0.0)

    # ZERO5: BB[m,j]==1 for m>=1 (0-based)
    for j in range(M):
        for k in range(K[j]):
            for i in range(M):
                for h in range(K[i]):
                    for ni in range(F[i] + 1):
                        for m in range(1, MR):
                            if BB[m, j] == 1:
                                row = new_eq_row()
                                idx = _deltap2(j, 0, k, i, ni, h, m, M, N, K, MR)
                                row[idx] = 1.0
                                aeq_rows.append(row)
                                beq_vals.append(0.0)

    # ZERO6: nj > F[j]
    for j in range(M):
        for k in range(K[j]):
            for nj in range(F[j] + 1, N + 1):
                for i in range(M):
                    for h in range(K[i]):
                        for ni in range(N + 1):
                            for m in range(MR):
                                row = new_eq_row()
                                idx = _deltap2(j, nj, k, i, ni, h, m, M, N, K, MR)
                                row[idx] = 1.0
                                aeq_rows.append(row)
                                beq_vals.append(0.0)

    # SYMMETRY
    for j in range(M):
        for nj in range(N + 1):
            for k in range(K[j]):
                for i in range(M):
                    for ni in range(N + 1):
                        for h in range(K[i]):
                            for m in range(MR):
                                row = new_eq_row()
                                idx1 = _deltap2(i, ni, h, j, nj, k, m, M, N, K, MR)
                                idx2 = _deltap2(j, nj, k, i, ni, h, m, M, N, K, MR)
                                row[idx1] += 1.0
                                row[idx2] -= 1.0
                                aeq_rows.append(row)
                                beq_vals.append(0.0)

    # MARGINALS
    for j in range(M):
        for k in range(K[j]):
            for nj in range(N + 1):
                for i in range(M):
                    for m in range(MR):
                        if i != j:
                            row = new_eq_row()
                            # LHS: p2[j,nj,k,j,nj,k,m]
                            idx = _deltap2(j, nj, k, j, nj, k, m, M, N, K, MR)
                            row[idx] += 1.0
                            # RHS: -sum p2[j,nj,k,i,ni,h,m]
                            for ni in range(N - nj + 1):
                                for h in range(K[i]):
                                    idx = _deltap2(j, nj, k, i, ni, h, m, M, N, K, MR)
                                    row[idx] -= 1.0
                            aeq_rows.append(row)
                            beq_vals.append(0.0)

    # UEFF
    for j in range(M):
        for i in range(M):
            for ki in range(K[i]):
                row = new_eq_row()
                # LHS: e[i,ki]
                idx = _deltae(i, ki, M, N, K, MR)
                row[idx] += 1.0
                # RHS: -sum p2[j,nj,kj,i,ni,ki,m]
                for nj in range(N + 1):
                    for kj in range(K[j]):
                        for m in range(MR):
                            for ni in range(1, N + 1):
                                if BB[m, i] == 0:
                                    idx = _deltap2(j, nj, kj, i, ni, ki, m, M, N, K, MR)
                                    row[idx] -= 1.0
                aeq_rows.append(row)
                beq_vals.append(0.0)

    # THM1
    for i in range(M):
        for k in range(K[i]):
            row = new_eq_row()
            # LHS: sum_{j,h} q[i,j,k,h]*e[i,k]
            for j in range(M):
                for h in range(K[i]):
                    idx = _deltae(i, k, M, N, K, MR)
                    row[idx] += q[i, j, k, h]
            # RHS: -sum_{j,h} q[i,j,h,k]*e[i,h]
            for j in range(M):
                for h in range(K[i]):
                    idx = _deltae(i, h, M, N, K, MR)
                    row[idx] -= q[i, j, h, k]
            aeq_rows.append(row)
            beq_vals.append(0.0)

    # THM2
    for j in range(M):
        for k in range(K[j]):
            for nj in range(F[j] + 1):
                for m in range(MR):
                    row = new_eq_row()
                    # LHS
                    for i in range(M):
                        for ni in range(1, F[i] + 1):
                            for ki in range(K[i]):
                                idx = _deltap2(j, nj, k, i, ni, ki, m, M, N, K, MR)
                                row[idx] += ni
                    # RHS: -N*p2[j,nj,k,j,nj,k,m]
                    idx = _deltap2(j, nj, k, j, nj, k, m, M, N, K, MR)
                    row[idx] -= N
                    aeq_rows.append(row)
                    beq_vals.append(0.0)

    # COR1
    row = new_eq_row()
    for m in range(MR):
        for i in range(M):
            for j in range(M):
                for nj in range(1, F[j] + 1):
                    for ni in range(1, F[i] + 1):
                        for ki in range(K[i]):
                            for kj in range(K[j]):
                                idx = _deltap2(j, nj, kj, i, ni, ki, m, M, N, K, MR)
                                row[idx] += ni * nj
    aeq_rows.append(row)
    beq_vals.append(N ** 2)

    # THM4 (inequality)
    for j in range(M):
        for k in range(K[j]):
            for i in range(M):
                for m in range(MR):
                    row = new_eq_row()
                    # -LHS (sign swapped: >= becomes <=)
                    for t in range(M):
                        for h in range(K[t]):
                            for nj_idx in range(N + 1):
                                for nt in range(N + 1):
                                    idx = _deltap2(j, nj_idx, k, t, nt, h, m, M, N, K, MR)
                                    row[idx] -= nt
                    # +RHS
                    for h in range(K[i]):
                        for nj_idx in range(N + 1):
                            for ni in range(1, N + 1):
                                idx = _deltap2(j, nj_idx, k, i, ni, h, m, M, N, K, MR)
                                row[idx] += N
                    aub_rows.append(row)
                    bub_vals.append(0.0)

    # Convert to sparse matrices
    if aeq_rows:
        Aeq = lil_matrix((len(aeq_rows), num_vars))
        for r, row in enumerate(aeq_rows):
            nz = np.nonzero(row)[0]
            for c in nz:
                Aeq[r, c] = row[c]
        Aeq = Aeq.tocsr()
    else:
        Aeq = lil_matrix((0, num_vars)).tocsr()

    if aub_rows:
        Aub = lil_matrix((len(aub_rows), num_vars))
        for r, row in enumerate(aub_rows):
            nz = np.nonzero(row)[0]
            for c in nz:
                Aub[r, c] = row[c]
        Aub = Aub.tocsr()
    else:
        Aub = lil_matrix((0, num_vars)).tocsr()

    return Aeq, beq_vals, Aub, bub_vals
