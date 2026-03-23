Matrix-Analytic Methods
=======================

QBD processes and matrix-analytic solutions.

The ``mam`` module implements matrix-analytic methods for analyzing queues with
structured Markov chains, including QBD, MAP, RAP, and G/M/1 and M/G/1 type processes.

**Key function categories:**

- **MAP analysis:** :func:`~line_solver.api.mam.map_pie`, :func:`~line_solver.api.mam.map_mean`, :func:`~line_solver.api.mam.map_var`, :func:`~line_solver.api.mam.map_scv`, :func:`~line_solver.api.mam.map_skew`
- **MAP fitting:** :func:`~line_solver.api.mam.map2_fit`, :func:`~line_solver.api.mam.mmpp2_fit`, :func:`~line_solver.api.mam.aph_fit`, :func:`~line_solver.api.mam.aph2_fit`
- **PH distributions:** Phase-type analysis and fitting
- **Transformations:** :func:`~line_solver.api.mam.map_scale`, :func:`~line_solver.api.mam.map_normalize`, :func:`~line_solver.api.mam.map_timereverse`
- **QBD methods:** :func:`~line_solver.api.mam.qbd_R`, :func:`~line_solver.api.mam.qbd_mapmap1`, :func:`~line_solver.api.mam.qbd_raprap1`
- **Compression:** :func:`~line_solver.api.mam.compress_adaptive`, :func:`~line_solver.api.mam.compress_spectral`

.. automodule:: line_solver.api.mam
   :members:
   :undoc-members:
   :show-inheritance:
