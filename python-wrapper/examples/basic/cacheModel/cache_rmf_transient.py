"""
Cache RMF Transient Analysis (Wrapper)

Demonstrates refined mean field (RMF) transient and steady-state
analysis of a multi-list cache with RANDOM(m) replacement.

Uses JAR CacheRMF via JPype to compute:
  - Steady-state hit/miss probabilities with 1/N correction
  - Transient evolution of hit rates via coupled ODE system
"""

import sys
import os
_wrapper_path = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..', '..'))
if _wrapper_path not in sys.path:
    sys.path.insert(0, _wrapper_path)

import numpy as np
from line_solver import *  # initializes JVM


def cache_rmf_transient():
    # Cache parameters
    n = 10        # number of items
    m_list = [3, 2]    # list capacities (2-list cache)
    alpha = 0.8   # Zipf exponent

    # Zipf popularity distribution
    p = np.array([(i + 1) ** (-alpha) for i in range(n)])
    p = p / p.sum()

    print(f'Cache parameters: n={n}, m=[{", ".join(str(x) for x in m_list)}], Zipf({alpha:.1f})')

    # Access JAR CacheRMF via JPype
    import jpype
    CacheRMF = jpype.JPackage('jline').lib.rmf.CacheRMF

    # Convert to Java arrays
    p_java = jpype.JArray(jpype.JDouble)(p.tolist())
    m_java = jpype.JArray(jpype.JInt)([int(x) for x in m_list])

    # Build DDPP model
    model = CacheRMF(p_java, m_java)

    # Steady-state analysis with 1/N correction
    result = model.meanFieldExpansionSteadyState(1)
    pi = np.array(list(result[0]))
    V = np.array(list(result[1]))
    pi_refined = pi + V / n

    print('\nSteady-state results (refined mean field):')
    total_hit = 0.0
    for k in range(1, len(m_list) + 1):
        hr = model.hitRate(pi_refined.tolist(), k)
        total_hit += hr
        print(f'  Hit rate (list {k}): {hr:.6f}')
    miss_rate = model.hitRate(pi_refined.tolist(), 0)
    print(f'  Miss rate:         {miss_rate:.6f}')
    print(f'  Total hit prob:    {total_hit:.6f}')
    print(f'  Total miss prob:   {miss_rate:.6f}')

    # Transient analysis
    trans_result = model.meanFieldExpansionTransient(50.0, 200, 1)
    T_java = trans_result[0]
    X_java = trans_result[1]
    Vt_java = trans_result[2]

    T = np.array(list(T_java))
    dim = model.getModelDimension()
    X = np.array([[float(X_java[i][j]) for j in range(dim)] for i in range(200)])
    Vt = np.array([[float(Vt_java[i][j]) for j in range(dim)] for i in range(200)])

    print(f'\nTransient hit rates (refined, N={n}):')
    time_indices = [0, 20, 50, 100, 199]  # t=0, ~5, ~12.5, ~25, 50
    for idx in time_indices:
        xt = X[idx, :] + Vt[idx, :] / n
        hr = sum(model.hitRate(xt.tolist(), k) for k in range(1, len(m_list) + 1))
        print(f'  t={T[idx]:7.3f}: hit_rate={hr:.6f}')


if __name__ == "__main__":
    cache_rmf_transient()
