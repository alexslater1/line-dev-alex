API Documentation
=================

This section provides detailed API documentation for all LINE Solver Python modules.

.. toctree::
   :maxdepth: 2
   :caption: API Sections:

   core
   cache
   lossn
   lsn
   mc
   mam
   npfqn
   polling
   pfqn
   qsys
   sn
   utilities

Overview
--------

- :doc:`core` - Networks, nodes, classes, distributions, solvers
- :doc:`cache` - Cache performance analysis
- :doc:`lossn` - Analysis of networks with blocking
- :doc:`lsn` - Layered queueing network analysis
- :doc:`mc` - CTMC and DTMC analysis tools
- :doc:`mam` - QBD processes and matrix-analytic solutions
- :doc:`npfqn` - Approximations for intractable queueing networks
- :doc:`polling` - Analysis of gated, exhaustive, and k-limited polling
- :doc:`pfqn` - MVA, convolution, and normalizing constant methods
- :doc:`qsys` - Single-station queueing system analysis
- :doc:`sn` - Network analysis and transformation utilities
- :doc:`utilities` - Helper functions, constants, and enumerations

Module Locations
----------------

.. list-table::
   :widths: 20 40 40
   :header-rows: 1

   * - Category
     - Python Path
     - Description
   * - Cache API
     - line_solver.api.cache
     - Cache analysis algorithms
   * - Constants
     - line_solver.constants
     - Enumerations and global constants
   * - Core
     - line_solver.lang
     - Network, Node, and JobClass definitions
   * - Distributions
     - line_solver.distributions
     - Exp, Erlang, PH, MAP, etc.
   * - Layered Networks
     - line_solver.layered
     - LayeredNetwork, Task, Activity
   * - LossN API
     - line_solver.api.lossn
     - Loss network algorithms
   * - LSN API
     - line_solver.api.lsn
     - Layered stochastic network utilities
   * - MAM API
     - line_solver.api.mam
     - Matrix-analytic methods
   * - MC API
     - line_solver.api.mc
     - Markov chain analysis
   * - NPFQN API
     - line_solver.api.npfqn
     - Non-product-form network approximations
   * - PFQN API
     - line_solver.api.pfqn
     - Product-form queueing network algorithms
   * - Polling API
     - line_solver.api.polling
     - Polling system algorithms
   * - Qsys API
     - line_solver.api.qsys
     - Single queueing system formulas
   * - SN API
     - line_solver.api.sn
     - Stochastic network utilities
   * - Solvers
     - line_solver.solvers
     - MVA, CTMC, FLD, MAM, etc.

License
-------

`BSD-3 License <../../license.html>`_

Copyright (c) 2012-2026, `QORE Lab <https://qore.doc.ic.ac.uk/>`_, Imperial College London
All rights reserved.
