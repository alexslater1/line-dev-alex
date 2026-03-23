Core Modules
============

Networks, nodes, classes, distributions, solvers.

These modules provide the fundamental building blocks for creating and solving queueing models.

Network Models (line_solver.lang)
----------------------------------

The ``lang`` module contains the core classes for building queueing network models.

Main Classes
~~~~~~~~~~~~

.. autoclass:: line_solver.lang.Network
   :members:
   :show-inheritance:

.. autoclass:: line_solver.lang.LayeredNetwork
   :members:
   :show-inheritance:

.. autoclass:: line_solver.lang.Ensemble
   :members:
   :show-inheritance:

Node Classes
~~~~~~~~~~~~

.. autoclass:: line_solver.lang.Node
   :members:
   :show-inheritance:

.. autoclass:: line_solver.lang.Source
   :members:
   :show-inheritance:

.. autoclass:: line_solver.lang.Queue
   :members:
   :show-inheritance:

.. autoclass:: line_solver.lang.QueueingStation
   :members:
   :show-inheritance:

.. autoclass:: line_solver.lang.Delay
   :members:
   :show-inheritance:

.. autoclass:: line_solver.lang.Sink
   :members:
   :show-inheritance:

.. autoclass:: line_solver.lang.Router
   :members:
   :show-inheritance:

.. autoclass:: line_solver.lang.Fork
   :members:
   :show-inheritance:

.. autoclass:: line_solver.lang.Join
   :members:
   :show-inheritance:

.. autoclass:: line_solver.lang.Cache
   :members:
   :show-inheritance:

.. autoclass:: line_solver.lang.Logger
   :members:
   :show-inheritance:

.. autoclass:: line_solver.lang.ClassSwitch
   :members:
   :show-inheritance:

.. autoclass:: line_solver.lang.Place
   :members:
   :show-inheritance:

.. autoclass:: line_solver.lang.Transition
   :members:
   :show-inheritance:

Job Class Classes
~~~~~~~~~~~~~~~~~

.. autoclass:: line_solver.lang.JobClass
   :members:
   :show-inheritance:

.. autoclass:: line_solver.lang.OpenClass
   :members:
   :show-inheritance:

.. autoclass:: line_solver.lang.ClosedClass
   :members:
   :show-inheritance:

.. autoclass:: line_solver.lang.SelfLoopingClass
   :members:
   :show-inheritance:

Routing and Structure
~~~~~~~~~~~~~~~~~~~~~

.. autoclass:: line_solver.lang.RoutingMatrix
   :members:
   :show-inheritance:

.. autoclass:: line_solver.lang.NetworkStruct
   :members:
   :show-inheritance:

State Representation
~~~~~~~~~~~~~~~~~~~~

.. autoclass:: line_solver.lang.State
   :members:
   :show-inheritance:

.. autoclass:: line_solver.lang.Mode
   :members:
   :show-inheritance:


Solvers (line_solver.solvers)
------------------------------

The ``solvers`` module provides various analytical and simulation-based solvers.

Base Solver Classes
~~~~~~~~~~~~~~~~~~~

.. autoclass:: line_solver.solvers.Solver
   :members:
   :show-inheritance:

.. autoclass:: line_solver.solvers.NetworkSolver
   :members:
   :show-inheritance:

.. autoclass:: line_solver.solvers.EnsembleSolver
   :members:
   :show-inheritance:

Analytical Solvers
~~~~~~~~~~~~~~~~~~

.. autoclass:: line_solver.solvers.SolverMVA
   :members:
   :show-inheritance:

.. autoclass:: line_solver.solvers.SolverCTMC
   :members:
   :show-inheritance:

.. autoclass:: line_solver.solvers.SolverFLD
   :members:
   :show-inheritance:

.. autoclass:: line_solver.solvers.SolverMAM
   :members:
   :show-inheritance:

.. autoclass:: line_solver.solvers.SolverNC
   :members:
   :show-inheritance:

.. autoclass:: line_solver.solvers.SolverSSA
   :members:
   :show-inheritance:

External Solver Wrappers
~~~~~~~~~~~~~~~~~~~~~~~~

.. autoclass:: line_solver.solvers.SolverJMT
   :members:
   :show-inheritance:

.. autoclass:: line_solver.solvers.SolverQNS
   :members:
   :show-inheritance:

.. autoclass:: line_solver.solvers.SolverLQNS
   :members:
   :show-inheritance:

.. autoclass:: line_solver.solvers.SolverLN
   :members:
   :show-inheritance:

Specialized Solvers
~~~~~~~~~~~~~~~~~~~

.. autoclass:: line_solver.solvers.SolverBLN
   :members:
   :show-inheritance:

.. autoclass:: line_solver.solvers.SolverAuto
   :members:
   :show-inheritance:

.. autoclass:: line_solver.solvers.LINE
   :members:
   :show-inheritance:

Result Classes
~~~~~~~~~~~~~~

.. autoclass:: line_solver.solvers.SampleResult
   :members:
   :show-inheritance:

.. autoclass:: line_solver.solvers.EventInfo
   :members:
   :show-inheritance:

.. autoclass:: line_solver.solvers.DistributionResult
   :members:
   :show-inheritance:

.. autoclass:: line_solver.solvers.ProbabilityResult
   :members:
   :show-inheritance:

Solver Configuration
~~~~~~~~~~~~~~~~~~~~

.. autoclass:: line_solver.solvers.SolverOptions
   :members:
   :show-inheritance:


Distributions (line_solver.distributions)
------------------------------------------

The ``distributions`` module provides probability distributions for service and arrival processes.

Base Distribution Classes
~~~~~~~~~~~~~~~~~~~~~~~~~

.. autoclass:: line_solver.distributions.Distribution
   :members:
   :show-inheritance:

.. autoclass:: line_solver.distributions.ContinuousDistribution
   :members:
   :show-inheritance:

.. autoclass:: line_solver.distributions.DiscreteDistribution
   :members:
   :show-inheritance:

.. autoclass:: line_solver.distributions.Markovian
   :members:
   :show-inheritance:

Continuous Distributions
~~~~~~~~~~~~~~~~~~~~~~~~

.. autoclass:: line_solver.distributions.Exp
   :members:
   :show-inheritance:

.. autoclass:: line_solver.distributions.Det
   :members:
   :show-inheritance:

.. autoclass:: line_solver.distributions.Erlang
   :members:
   :show-inheritance:

.. autoclass:: line_solver.distributions.Gamma
   :members:
   :show-inheritance:

.. autoclass:: line_solver.distributions.HyperExp
   :members:
   :show-inheritance:

.. autoclass:: line_solver.distributions.Lognormal
   :members:
   :show-inheritance:

.. autoclass:: line_solver.distributions.Pareto
   :members:
   :show-inheritance:

.. autoclass:: line_solver.distributions.Uniform
   :members:
   :show-inheritance:

.. autoclass:: line_solver.distributions.Weibull
   :members:
   :show-inheritance:

Phase-Type Distributions
~~~~~~~~~~~~~~~~~~~~~~~~

.. autoclass:: line_solver.distributions.APH
   :members:
   :show-inheritance:

.. autoclass:: line_solver.distributions.Coxian
   :members:
   :show-inheritance:

.. autoclass:: line_solver.distributions.Cox2
   :members:
   :show-inheritance:

.. autoclass:: line_solver.distributions.PH
   :members:
   :show-inheritance:

Markovian Arrival Processes
~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. autoclass:: line_solver.distributions.MAP
   :members:
   :show-inheritance:

.. autoclass:: line_solver.distributions.MMPP2
   :members:
   :show-inheritance:

Discrete Distributions
~~~~~~~~~~~~~~~~~~~~~~

.. autoclass:: line_solver.distributions.Bernoulli
   :members:
   :show-inheritance:

.. autoclass:: line_solver.distributions.Binomial
   :members:
   :show-inheritance:

.. autoclass:: line_solver.distributions.DiscreteUniform
   :members:
   :show-inheritance:

.. autoclass:: line_solver.distributions.Geometric
   :members:
   :show-inheritance:

.. autoclass:: line_solver.distributions.Poisson
   :members:
   :show-inheritance:

.. autoclass:: line_solver.distributions.Zipf
   :members:
   :show-inheritance:

Special Distributions
~~~~~~~~~~~~~~~~~~~~~

.. autoclass:: line_solver.distributions.Disabled
   :members:
   :show-inheritance:

.. autoclass:: line_solver.distributions.Immediate
   :members:
   :show-inheritance:

.. autoclass:: line_solver.distributions.Replayer
   :members:
   :show-inheritance:


Layered Networks (line_solver.layered)
---------------------------------------

The ``layered`` module provides support for layered queueing networks (LQNs).

Main Classes
~~~~~~~~~~~~

.. autoclass:: line_solver.layered.LayeredNetwork
   :members:
   :show-inheritance:

.. autoclass:: line_solver.layered.Processor
   :members:
   :show-inheritance:

.. autoclass:: line_solver.layered.Task
   :members:
   :show-inheritance:

.. autoclass:: line_solver.layered.Entry
   :members:
   :show-inheritance:

.. autoclass:: line_solver.layered.Activity
   :members:
   :show-inheritance:

.. autoclass:: line_solver.layered.ActivityPrecedence
   :members:
   :show-inheritance:

Cache Tasks
~~~~~~~~~~~

.. autoclass:: line_solver.layered.CacheTask
   :members:
   :show-inheritance:

.. autoclass:: line_solver.layered.ItemEntry
   :members:
   :show-inheritance:
