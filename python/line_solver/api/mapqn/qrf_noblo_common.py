"""
Shared utilities for QRF NLP no-blocking approximation methods.

Provides variable unflattening (sub_qrfvar), objective functions (MMI, MEM),
and constraint construction (sub_qrfcon) used by all qrf_noblo_* methods.

Port of MATLAB qrf_noblo_mmi.m / qrf_noblo_mem.m shared infrastructure.
"""

import numpy as np

LOGTOL = 1e-6


def sub_qrfvar(x, M, N, K, MR):
    """Unflatten decision vector x into 7D tensor p2 and effective utilizations e.

    Args:
        x: Flat decision vector of length M*(N+1)*Kmax*M*(N+1)*Kmax*MR + M*Kmax
        M: Number of queues
        N: Total population
        K: Array of phases per queue [M]
        MR: Number of blocking configurations

    Returns:
        p2: 7D array [M, N+1, Kmax, M, N+1, Kmax, MR] joint probabilities
        e: 2D array [M, Kmax] effective utilizations
    """
    Kmax = int(max(K))
    p2_size = M * (N + 1) * Kmax * M * (N + 1) * Kmax * MR

    # Reshape p2 from flat vector - MATLAB iteration order:
    # j, nj, k, i, ni, h, m (outermost to innermost)
    p2 = np.zeros((M, N + 1, Kmax, M, N + 1, Kmax, MR))
    ctr = 0
    for j in range(M):
        for nj in range(N + 1):
            for k in range(K[j]):
                for i in range(M):
                    for ni in range(N + 1):
                        for h in range(K[i]):
                            for m in range(MR):
                                p2[j, nj, k, i, ni, h, m] = x[ctr]
                                ctr += 1

    # Extract e variables
    e = np.zeros((M, Kmax))
    for i in range(M):
        for k in range(K[i]):
            e[i, k] = x[ctr]
            ctr += 1

    return p2, e


def mmi_objective(x, M, N, K, F, MR):
    """Mutual Information Minimization objective.

    Minimizes: sum p2[i,ni,ki,j,nj,kj,m] *
        (log(p2[i,ni,ki,j,nj,kj,m]) - log(p2[i,ni,ki,i,ni,ki,m]) - log(p2[j,nj,kj,j,nj,kj,m]))

    Port of MATLAB mmi() nested function.
    """
    p2, _ = sub_qrfvar(x, M, N, K, MR)
    fobj = 0.0
    for m in range(MR):
        for i in range(M):
            for ki in range(K[i]):
                for j in range(M):
                    if i != j:
                        for kj in range(K[j]):
                            for ni in range(1, F[i] + 1):
                                for nj in range(1, F[j] + 1):
                                    pij = p2[i, ni, ki, j, nj, kj, m]
                                    pii = p2[i, ni, ki, i, ni, ki, m]
                                    pjj = p2[j, nj, kj, j, nj, kj, m]
                                    fobj += pij * (
                                        np.log(LOGTOL + pij)
                                        - np.log(LOGTOL + pii)
                                        - np.log(LOGTOL + pjj)
                                    )
    return fobj


def mem_objective(x, M, N, K, F, MR):
    """Maximum Entropy objective (minimize negative entropy).

    Minimizes: -sum p2[i,ni,k,i,ni,k,m] * log(p2[i,ni,k,i,ni,k,m])

    Port of MATLAB mem() nested function.
    """
    p2, _ = sub_qrfvar(x, M, N, K, MR)
    fobj = 0.0
    for m in range(MR):
        for i in range(M):
            for k in range(K[i]):
                for ni in range(1, F[i] + 1):
                    pval = p2[i, ni, k, i, ni, k, m]
                    fobj -= pval * np.log(LOGTOL + pval)
    return fobj


def sub_qrfcon_noblo(x, q, M, MR, BB, F, N, K):
    """Build equality and inequality constraints for QRF no-blocking.

    Port of MATLAB sub_qrfcon() for qrf_noblo_mmi.m / qrf_noblo_mem.m.

    Args:
        x: Decision vector
        q: Transition rates [M, M, Kmax, Kmax]
        M, MR, N: Problem dimensions
        BB: Blocking state matrix [MR, M]
        F: Capacity per queue [M]
        K: Phases per queue [M]

    Returns:
        c_ineq: Inequality constraints (c <= 0, MATLAB convention)
        ceq: Equality constraints (ceq = 0)
    """
    p2, e = sub_qrfvar(x, M, N, K, MR)
    ceq = []
    c = []

    # ONE: normalization
    # sum_{nj,k,m} p2[j,nj,k,j,nj,k,m] = 1  for each j
    for j in range(M):
        val = 0.0
        for nj in range(N + 1):
            for k in range(K[j]):
                for m in range(MR):
                    val += p2[j, nj, k, j, nj, k, m]
        ceq.append(val - 1.0)

    # ZERO1: i==j and ni==nj and h!=k => p2=0
    for j in range(M):
        for k in range(K[j]):
            for nj in range(N + 1):
                for i in range(M):
                    for h in range(K[i]):
                        for ni in range(N + 1):
                            for m in range(MR):
                                if i == j and nj == ni and h != k:
                                    ceq.append(p2[j, nj, k, i, ni, h, m])

    # ZERO2: i==j and nj!=ni => p2=0
    for j in range(M):
        for k in range(K[j]):
            for nj in range(N + 1):
                for i in range(M):
                    for h in range(K[i]):
                        for ni in range(N + 1):
                            for m in range(MR):
                                if i == j and nj != ni:
                                    ceq.append(p2[j, nj, k, i, ni, h, m])

    # ZERO3: i!=j and nj+ni>N => p2=0
    for j in range(M):
        for k in range(K[j]):
            for nj in range(N + 1):
                for i in range(M):
                    for h in range(K[i]):
                        for ni in range(N + 1):
                            for m in range(MR):
                                if i != j and nj + ni > N:
                                    ceq.append(p2[j, nj, k, i, ni, h, m])

    # ZERO5: BB[m,j]==1 for m>=1 (0-based) => p2[j,0,...]=0
    for j in range(M):
        for k in range(K[j]):
            for i in range(M):
                for h in range(K[i]):
                    for ni in range(F[i] + 1):
                        for m in range(1, MR):  # m from 1 (2nd config, 0-based)
                            if BB[m, j] == 1:
                                ceq.append(p2[j, 0, k, i, ni, h, m])

    # ZERO6: nj > F[j] => p2=0
    for j in range(M):
        for k in range(K[j]):
            for nj in range(F[j] + 1, N + 1):
                for i in range(M):
                    for h in range(K[i]):
                        for ni in range(N + 1):
                            for m in range(MR):
                                ceq.append(p2[j, nj, k, i, ni, h, m])

    # ZERO7: blocking constraint (only active when MR>1 and BB has nonzeros)
    # Skipped for no-blocking (MR=1) since m range is 1:MR-1 which is empty

    # SYMMETRY: p2[i,ni,h,j,nj,k,m] = p2[j,nj,k,i,ni,h,m]
    for j in range(M):
        for nj in range(N + 1):
            for k in range(K[j]):
                for i in range(M):
                    for ni in range(N + 1):
                        for h in range(K[i]):
                            for m in range(MR):
                                ceq.append(
                                    p2[i, ni, h, j, nj, k, m]
                                    - p2[j, nj, k, i, ni, h, m]
                                )

    # MARGINALS: p2[j,nj,k,j,nj,k,m] = sum_{ni,h} p2[j,nj,k,i,ni,h,m] for i!=j
    for j in range(M):
        for k in range(K[j]):
            for nj in range(N + 1):
                for i in range(M):
                    for m in range(MR):
                        if i != j:
                            val = p2[j, nj, k, j, nj, k, m]
                            for ni in range(N - nj + 1):  # 0 to N-nj
                                for h in range(K[i]):
                                    val -= p2[j, nj, k, i, ni, h, m]
                            ceq.append(val)

    # UEFF: e[i,ki] = sum p2[j,nj,kj,i,ni,ki,m] for ni>=1 and BB[m,i]==0
    for j in range(M):
        for i in range(M):
            for ki in range(K[i]):
                val = e[i, ki]
                for nj in range(N + 1):
                    for kj in range(K[j]):
                        for m in range(MR):
                            for ni in range(1, N + 1):
                                if BB[m, i] == 0:
                                    val -= p2[j, nj, kj, i, ni, ki, m]
                ceq.append(val)

    # THM1: sum_{j,h} q[i,j,k,h]*e[i,k] = sum_{j,h} q[i,j,h,k]*e[i,h]
    for i in range(M):
        for k in range(K[i]):
            val = 0.0
            # LHS
            for j in range(M):
                for h in range(K[i]):
                    val += q[i, j, k, h] * e[i, k]
            # RHS
            for j in range(M):
                for h in range(K[i]):
                    val -= q[i, j, h, k] * e[i, h]
            ceq.append(val)

    # THM2: sum_{i,ni,ki} ni*p2[j,nj,k,i,ni,ki,m] = N*p2[j,nj,k,j,nj,k,m]
    for j in range(M):
        for k in range(K[j]):
            for nj in range(F[j] + 1):  # 0 to F[j]
                for m in range(MR):
                    val = 0.0
                    for i in range(M):
                        for ni in range(1, F[i] + 1):  # 1 to F[i]
                            for ki in range(K[i]):
                                val += ni * p2[j, nj, k, i, ni, ki, m]
                    val -= N * p2[j, nj, k, j, nj, k, m]
                    ceq.append(val)

    # COR1: sum ni*nj*p2 = N^2
    val = 0.0
    for m in range(MR):
        for i in range(M):
            for j in range(M):
                for nj in range(1, F[j] + 1):  # 1 to F[j]
                    for ni in range(1, F[i] + 1):  # 1 to F[i]
                        for ki in range(K[i]):
                            for kj in range(K[j]):
                                val += ni * nj * p2[j, nj, kj, i, ni, ki, m]
    val -= N ** 2
    ceq.append(val)

    # THM4: inequality (>= in MATLAB, stored as <= with sign swap)
    # sum_{t,h,nj,nt} nt*p2[j,nj,k,t,nt,h,m] >= N*sum_{h,nj,ni} p2[j,nj,k,i,ni,h,m]
    for j in range(M):
        for k in range(K[j]):
            for i in range(M):
                for m in range(MR):
                    val = 0.0
                    # -LHS (sign swapped for <= form)
                    for t in range(M):
                        for h in range(K[t]):
                            for nj_idx in range(N + 1):
                                for nt in range(N + 1):
                                    val -= nt * p2[j, nj_idx, k, t, nt, h, m]
                    # +RHS (sign swapped)
                    for h in range(K[i]):
                        for nj_idx in range(N + 1):
                            for ni in range(1, N + 1):
                                val += N * p2[j, nj_idx, k, i, ni, h, m]
                    c.append(val)

    return np.array(c), np.array(ceq)


def compute_num_vars(M, N, K, MR):
    """Compute total number of decision variables."""
    Kmax = int(max(K))
    return M * (N + 1) * Kmax * M * (N + 1) * Kmax * MR + M * Kmax


def extract_results(p2opt, M, K, F, MR):
    """Extract UN and QN from optimal p2 tensor.

    Args:
        p2opt: Optimal 7D probability tensor
        M: Number of queues
        K: Phases per queue [M]
        F: Capacity per queue [M]
        MR: Number of blocking configurations

    Returns:
        UN: Utilization per queue [M]
        QN: Queue length per queue [M]
    """
    UN = np.zeros(M)
    QN = np.zeros(M)
    for ti in range(M):
        for m in range(MR):
            for ni in range(1, F[ti] + 1):
                for ki in range(K[ti]):
                    UN[ti] += p2opt[ti, ni, ki, ti, ni, ki, m]
                    QN[ti] += ni * p2opt[ti, ni, ki, ti, ni, ki, m]
    return UN, QN


def build_q_from_mu_v_rt(M, K, mu, v, rt):
    """Build 4D transition rate array from mu, v, rt.

    Args:
        M: Number of queues
        K: Phases per queue [M]
        mu: Completion rates [M, Kmax, Kmax]
        v: Background rates [M, Kmax, Kmax]
        rt: Routing matrix [M, M]

    Returns:
        q: Transition rates [M, M, Kmax, Kmax]
    """
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
    return q


def extract_mu_v_from_maps(MAPs, M, K):
    """Extract mu and v arrays from MAPs cell array.

    Args:
        MAPs: List of [D0, D1] pairs per queue
        M: Number of queues
        K: Phases per queue [M]

    Returns:
        mu: Completion rates [M, Kmax, Kmax]
        v: Background rates [M, Kmax, Kmax]
    """
    Kmax = int(max(K))
    mu = np.zeros((M, Kmax, Kmax))
    v = np.zeros((M, Kmax, Kmax))
    for i in range(M):
        D0 = MAPs[i][0]
        D1 = MAPs[i][1]
        for h in range(K[i]):
            for k in range(K[i]):
                mu[i, h, k] = D1[h, k]
                if h == k:
                    v[i, k, h] = 0.0
                else:
                    v[i, k, h] = D0[h, k]
    return mu, v
