LINE Solver Python Documentation
=================================

Welcome to the LINE Solver Python documentation. LINE is an open-source solver for queueing models
and queueing networks, supporting both analytical methods and simulation.

.. COMMENTED OUT - Getting Started section
.. .. toctree::
..    :maxdepth: 2
..    :caption: Getting Started:
..
..    installation
..    quickstart
..    examples

.. toctree::
   :maxdepth: 2
   :caption: API Reference:

   api/index
   reference

.. COMMENTED OUT - Non-API items from API Reference section
.. .. toctree::
..    :maxdepth: 2
..    :caption: Additional Reference:
..
..    solver_reference
..    choosing_solver

.. COMMENTED OUT - About LINE Solver section
.. About LINE Solver
.. =================
..
.. LINE (Library for INteractive Evaluation) is a comprehensive performance modeling toolkit
.. for queueing networks. The Python implementation provides:
..
.. - **Multiple solvers:** Analytical methods (MVA, CTMC, Fluid, MAM) and simulation (JMT)
.. - **Rich model types:** Open, closed, and mixed networks; layered queueing networks
.. - **Flexible distributions:** Exponential, phase-type, MAP, and general distributions
.. - **Advanced features:** Fork-join, caching, priority scheduling, state-dependent routing
.. - **Extensive examples:** Gallery of validated models and real-world use cases
..
.. Features
.. --------
..
.. **Network Types**
..    - Open queueing networks (e.g., M/M/1, M/M/k, M/G/1)
..    - Closed queueing networks with fixed populations
..    - Mixed networks with both open and closed classes
..    - Layered queueing networks (LQNs) for client-server systems
..
.. **Solvers**
..    - Mean Value Analysis (MVA)
..    - Continuous-Time Markov Chain (CTMC)
..    - Fluid/Mean-Field ODE Solver
..    - Matrix-Analytic Methods (MAM)
..    - Normalizing Constant Analyzer (NC)
..    - Stochastic Simulation Algorithm (SSA)
..    - Java Modelling Tools (JMT) simulation wrapper
..
.. **Advanced Capabilities**
..    - Fork-join parallel processing models
..    - Cache modeling with replacement policies
..    - Priority and polling scheduling
..    - State-dependent routing
..    - Load-dependent service
..    - Random environment models

Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
