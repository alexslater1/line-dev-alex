Cache Algorithms
================

Cache performance analysis.

The ``cache`` module provides algorithms for analyzing cache systems with various
replacement policies (LRU, FIFO, RR, etc.) and item popularity distributions.

**Key function categories:**

- **Miss probability:** :func:`~line_solver.api.cache.cache_miss`, :func:`~line_solver.api.cache.cache_miss_fpi`, :func:`~line_solver.api.cache.cache_miss_spm`, :func:`~line_solver.api.cache.cache_mva_miss`
- **Hit probability:** :func:`~line_solver.api.cache.cache_prob_erec`, :func:`~line_solver.api.cache.cache_prob_fpi`, :func:`~line_solver.api.cache.cache_prob_spm`
- **TTL caches:** :func:`~line_solver.api.cache.cache_ttl_hlru`, :func:`~line_solver.api.cache.cache_ttl_lrua`, :func:`~line_solver.api.cache.cache_ttl_lrum`
- **Mean-field methods:** :func:`~line_solver.api.cache.cache_rrm_meanfield`, :func:`~line_solver.api.cache.cache_rrm_meanfield_ode`
- **Characteristic time:** :func:`~line_solver.api.cache.cache_t_hlru`, :func:`~line_solver.api.cache.cache_t_lrum`
- **MVA-based analysis:** :func:`~line_solver.api.cache.cache_mva`, :func:`~line_solver.api.cache.cache_spm`

.. automodule:: line_solver.api.cache
   :members:
   :undoc-members:
   :show-inheritance:
