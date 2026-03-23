Markov Chain Utilities
======================

CTMC and DTMC analysis tools.

The ``mc`` module provides general tools for analyzing Markov chains, including
steady-state and transient analysis for both continuous-time and discrete-time
Markov chains.

**Key function categories:**

- **Steady-state analysis:** :func:`~line_solver.api.ctmc.ctmc_solve_reducible`, :func:`~line_solver.api.dtmc.dtmc_solve_reducible`, :func:`~line_solver.api.ctmc.ctmc_stochcomp`
- **Transient analysis:** :func:`~line_solver.api.ctmc.ctmc_transient`, :func:`~line_solver.api.ctmc.ctmc_uniformization`
- **Simulation:** :func:`~line_solver.api.ctmc.ctmc_simulate`, :func:`~line_solver.api.ctmc.ctmc_rand`
- **Aggregation methods:** :func:`~line_solver.api.ctmc.ctmc_courtois`, :func:`~line_solver.api.ctmc.ctmc_takahashi`, :func:`~line_solver.api.ctmc.ctmc_kms`
- **State-space generation:** :func:`~line_solver.api.ctmc.ctmc_ssg`, :func:`~line_solver.api.ctmc.ctmc_ssg_reachability`
- **Generator matrices:** :func:`~line_solver.api.ctmc.ctmc_makeinfgen`, :func:`~line_solver.api.ctmc.ctmc_multi`

Markov Chain Analysis (line_solver.api.mc)
-------------------------------------------

.. automodule:: line_solver.api.mc
   :members:
   :undoc-members:
   :show-inheritance:


Continuous-Time Markov Chains (line_solver.api.ctmc)
-----------------------------------------------------

The ``ctmc`` module contains algorithms specifically for continuous-time Markov
chains (CTMCs), including steady-state solvers and transient analysis.

.. automodule:: line_solver.api.ctmc
   :members:
   :undoc-members:
   :show-inheritance:


Discrete-Time Markov Chains (line_solver.api.dtmc)
---------------------------------------------------

The ``dtmc`` module provides algorithms for discrete-time Markov chains (DTMCs),
including steady-state and transient analysis.

.. automodule:: line_solver.api.dtmc
   :members:
   :undoc-members:
   :show-inheritance:
