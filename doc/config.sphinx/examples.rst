Examples
========

This section contains examples demonstrating LINE Solver Python capabilities, organized by complexity and topic. All examples are available as Jupyter notebooks in the ``python/examples/`` directory.

.. contents:: Example Categories
   :local:
   :depth: 2

Gallery Examples
----------------

The gallery contains fundamental queueing models used for validation and benchmarking.

Single Server Models
~~~~~~~~~~~~~~~~~~~~~

**M/M/1 Queue** (``examples/gallery/gallery_mm1.ipynb``)
   Classic single-server queue with Poisson arrivals and exponential service times.

**M/M/1 with Processor Sharing** (``examples/gallery/gallery_mm1_ps.ipynb``)
   Single-server queue with processor sharing scheduling.

**M/M/1 with Priority Classes** (``examples/gallery/gallery_mm1_prio.ipynb``)
   Single-server queue with multiple priority classes.

**M/M/1 with Multiclass** (``examples/gallery/gallery_mm1_multiclass.ipynb``)
   Single-server queue with multiple job classes.

**M/M/1 with Linear Service** (``examples/gallery/gallery_mm1_linear.ipynb``)
   Single-server queue with load-dependent linear service rates.

**M/M/1 with Feedback** (``examples/gallery/gallery_mm1_feedback.ipynb``)
   Single-server queue with probabilistic feedback routing.

**M/M/1 Tandem Network** (``examples/gallery/gallery_mm1_tandem.ipynb``)
   Series of M/M/1 queues in tandem.

**M/M/1 Reentrant Network** (``examples/gallery/gallery_mm1_reentrant.ipynb``)
   Jobs can revisit the same queue multiple times.

Multi-Server and Specialized Models
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**M/M/k Queue** (``examples/gallery/gallery_mmk.ipynb``)
   Multi-server queue with exponential service times.

**HyperExp/M/1 Queue** (``examples/gallery/gallery_hypm1.ipynb``)
   Single-server queue with hyper-exponential arrivals.

**M/HyperExp/1 Queue** (``examples/gallery/gallery_mhyp1.ipynb``)
   Single-server queue with hyper-exponential service times.

**Gamma/M/1 Queue** (``examples/gallery/gallery_gamm1.ipynb``)
   Single-server queue with Gamma arrivals.

**Det/M/1 Queue** (``examples/gallery/gallery_detm1.ipynb``)
   Single-server queue with deterministic arrivals.

**M/Erlang/1 Queue** (``examples/gallery/gallery_merl1.ipynb``)
   Single-server queue with Erlang service times.

**Erlang/M/1 Queue** (``examples/gallery/gallery_erlm1.ipynb``)
   Single-server queue with Erlang arrivals.

Network Models
~~~~~~~~~~~~~~

**Closed Queueing Network** (``examples/gallery/gallery_cqn.ipynb``)
   Basic closed queueing network with fixed population.

**Multiclass Closed Network** (``examples/gallery/gallery_cqn_multiclass.ipynb``)
   Closed network with multiple job classes.

**Repairmen Model** (``examples/gallery/gallery_repairmen.ipynb``)
   Classic machine repair model with limited repairmen.

Basic Examples
--------------

Open Queueing Networks
~~~~~~~~~~~~~~~~~~~~~~~

**Basic Open Network** (``examples/basic/openQN/oqn_basic.ipynb``)
   Simple open queueing network with multiple solvers comparison.

**One-Line Open Model** (``examples/basic/openQN/oqn_oneline.ipynb``)
   Minimal open model created in a single line of code.

**Four-Queue Network** (``examples/basic/openQN/oqn_fourqueues.ipynb``)
   Open network with four interconnected queues.

**Class-Switching Routing** (``examples/basic/openQN/oqn_cs_routing.ipynb``)
   Open network with class-switching routing behavior.

**Trace-Driven Model** (``examples/basic/openQN/oqn_trace_driven.ipynb``)
   Open network driven by empirical trace data.

**Multiple Sinks** (``examples/basic/openQN/oqn_vsinks.ipynb``)
   Open network with multiple sink nodes.

Closed Queueing Networks
~~~~~~~~~~~~~~~~~~~~~~~~~

**One-Line Closed Model** (``examples/basic/closedQN/cqn_oneline.ipynb``)
   Minimal closed model created in a single line of code.

**Two-Queue Network** (``examples/basic/closedQN/cqn_twoqueues.ipynb``)
   Basic two-queue closed network.

**Multi-Queue Network** (``examples/basic/closedQN/cqn_twoqueues_multi.ipynb``)
   Closed network with multiple queues and routing.

**Multi-Server Network** (``examples/basic/closedQN/cqn_multiserver.ipynb``)
   Closed network with multi-server queues.

**Repairmen Model** (``examples/basic/closedQN/cqn_repairmen.ipynb``)
   Machine repair model with multiple repairmen.

**Multi-Repairmen Model** (``examples/basic/closedQN/cqn_repairmen_multi.ipynb``)
   Extended repairmen model with complex routing.

**BCMP Theorem Example** (``examples/basic/closedQN/cqn_bcmp_theorem.ipynb``)
   Example demonstrating BCMP network properties.

**DPS Scheduling** (``examples/basic/closedQN/cqn_scheduling_dps.ipynb``)
   Closed network with Discriminatory Processor Sharing.

**MMPP/2 Service** (``examples/basic/closedQN/cqn_mmpp2_service.ipynb``)
   Closed network with MMPP/2 service process.

**Two-Class Erlang** (``examples/basic/closedQN/cqn_twoclass_erl.ipynb``)
   Two-class closed network with Erlang distributions.

**Two-Class HyperExp** (``examples/basic/closedQN/cqn_twoclass_hyperl.ipynb``)
   Two-class closed network with hyper-exponential distributions.

**Three-Class HyperExp** (``examples/basic/closedQN/cqn_threeclass_hyperl.ipynb``)
   Three-class closed network with hyper-exponential distributions.

Mixed Queueing Networks
~~~~~~~~~~~~~~~~~~~~~~~

**Basic Mixed Network** (``examples/basic/mixedQN/mqn_basic.ipynb``)
   Network with both open and closed job classes.

**Single-Server FCFS** (``examples/basic/mixedQN/mqn_singleserver_fcfs.ipynb``)
   Mixed network with single-server FCFS scheduling.

**Single-Server PS** (``examples/basic/mixedQN/mqn_singleserver_ps.ipynb``)
   Mixed network with single-server processor sharing.

**Multi-Server FCFS** (``examples/basic/mixedQN/mqn_multiserver_fcfs.ipynb``)
   Mixed network with multi-server FCFS scheduling.

**Multi-Server PS** (``examples/basic/mixedQN/mqn_multiserver_ps.ipynb``)
   Mixed network with multi-server processor sharing.

Fork-Join Networks
~~~~~~~~~~~~~~~~~~

**Basic Fork-Join** (``examples/basic/forkJoin/fj_basic.ipynb``)
   Simple fork-join parallel processing model.

**Basic Nesting** (``examples/basic/forkJoin/fj_basic_nesting.ipynb``)
   Fork-join network with nested parallel sections.

**Three Branches** (``examples/basic/forkJoin/fj_threebranches.ipynb``)
   Fork-join with three parallel processing branches.

**Serial Fork-Join (Closed)** (``examples/basic/forkJoin/fj_serialfjs_closed.ipynb``)
   Serial combination of fork-join sections in closed network.

**Route Overlap** (``examples/basic/forkJoin/fj_route_overlap.ipynb``)
   Fork-join with overlapping routing paths.

**With Delays** (``examples/basic/forkJoin/fj_delays.ipynb``)
   Fork-join network including delay nodes.

**Complex Serial** (``examples/basic/forkJoin/fj_complex_serial.ipynb``)
   Complex serial arrangement of fork-join sections.

**Class-Switching Pre-Fork** (``examples/basic/forkJoin/fj_cs_prefork.ipynb``)
   Class-switching before fork operations.

**Class-Switching Multi-Visits** (``examples/basic/forkJoin/fj_cs_multi_visits.ipynb``)
   Class-switching with multiple revisits in fork-join.

Priority and Scheduling Models
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Identical Priority Classes** (``examples/basic/prioModel/prio_identical.ipynb``)
   Multiple job classes with identical priorities.

**FCFS Priority Open** (``examples/basic/prioModel/prio_hol_open.ipynb``)
   Open network with first-come first-served priority scheduling.

**FCFS Priority Closed** (``examples/basic/prioModel/prio_hol_closed.ipynb``)
   Closed network with first-come first-served priority scheduling.

**PS Priority** (``examples/basic/prioModel/prio_psprio.ipynb``)
   Processor sharing with priority classes.

Class Switching Models
~~~~~~~~~~~~~~~~~~~~~~

**Implicit Class Switching** (``examples/basic/classSwitching/cs_implicit.ipynb``)
   Network with implicit class switching behavior.

**Single Diamond** (``examples/basic/classSwitching/cs_single_diamond.ipynb``)
   Diamond-shaped routing with class switching.

**Multi Diamond** (``examples/basic/classSwitching/cs_multi_diamond.ipynb``)
   Multiple diamond routing patterns with class switching.

**Transient Class** (``examples/basic/classSwitching/cs_transient_class.ipynb``)
   Class switching with transient job classes.

Cache Models
~~~~~~~~~~~~

**FIFO Replacement** (``examples/basic/cacheModel/cache_replc_fifo.ipynb``)
   Cache model with FIFO replacement policy.

**Round-Robin Replacement** (``examples/basic/cacheModel/cache_replc_rr.ipynb``)
   Cache model with round-robin replacement policy.

**Routing-Based Cache** (``examples/basic/cacheModel/cache_replc_routing.ipynb``)
   Cache model with routing-based replacement.

**Replacement Policy Comparison** (``examples/basic/cacheModel/cache_compare_replc.ipynb``)
   Comparison of different cache replacement policies.

Layered Queueing Networks
~~~~~~~~~~~~~~~~~~~~~~~~~

**Basic LQN** (``examples/basic/layeredModel/lqn_basic.ipynb``)
   Basic layered queueing network model.

**Serial LQN** (``examples/basic/layeredModel/lqn_serial.ipynb``)
   Serial arrangement of layered stochastic services.

**Two Tasks** (``examples/basic/layeredModel/lqn_twotasks.ipynb``)
   Layered model with two interacting tasks.

**Function Calls** (``examples/basic/layeredModel/lqn_function.ipynb``)
   Layered model representing function call patterns.

**BPMN Workflow** (``examples/basic/layeredModel/lqn_bpmn.ipynb``)
   BPMN workflow modeled as layered queueing network.

**Multi-Solver LQN** (``examples/basic/layeredModel/lqn_multi_solvers.ipynb``)
   Layered model solved with multiple solvers.

**Workflow Example** (``examples/basic/layeredModel/lqn_workflows.ipynb``)
   Complex workflow modeled with layered networks.

**OFBiz Example** (``examples/basic/layeredModel/lqn_ofbiz.ipynb``)
   Real-world OFBiz application modeled as LQN.

Stochastic Petri Nets
~~~~~~~~~~~~~~~~~~~~~~

**Basic Open SPN** (``examples/basic/stochPetriNet/spn_basic_open.ipynb``)
   Open stochastic Petri net model.

**Basic Closed SPN** (``examples/basic/stochPetriNet/spn_basic_closed.ipynb``)
   Closed stochastic Petri net model.

**Two Places Closed** (``examples/basic/stochPetriNet/spn_closed_twoplaces.ipynb``)
   Closed SPN with two places.

**Four Places Closed** (``examples/basic/stochPetriNet/spn_closed_fourplaces.ipynb``)
   Closed SPN with four places.

**Seven Places Open** (``examples/basic/stochPetriNet/spn_open_sevenplaces.ipynb``)
   Open SPN with seven places.

**Two Modes** (``examples/basic/stochPetriNet/spn_twomodes.ipynb``)
   SPN with two operational modes.

**Four Modes** (``examples/basic/stochPetriNet/spn_fourmodes.ipynb``)
   SPN with four operational modes.

**Inhibiting Arcs** (``examples/basic/stochPetriNet/spn_inhibiting.ipynb``)
   SPN with inhibiting arc constraints.

Advanced Examples
-----------------

State Probabilities
~~~~~~~~~~~~~~~~~~~

**Aggregated State Probabilities** (``examples/advanced/stateProbabilities/statepr_aggr.ipynb``)
   Computing aggregated state probabilities for network analysis.

**Large Model Aggregation** (``examples/advanced/stateProbabilities/statepr_aggr_large.ipynb``)
   Aggregated state probabilities for large-scale models.

**System Aggregation** (``examples/advanced/stateProbabilities/statepr_sys_aggr.ipynb``)
   System-level aggregated state probability analysis.

**Large System Aggregation** (``examples/advanced/stateProbabilities/statepr_sys_aggr_large.ipynb``)
   System aggregation for large-scale network models.

**All Probabilities FCFS** (``examples/advanced/stateProbabilities/statepr_allprobs_fcfs.ipynb``)
   Complete state probability enumeration for FCFS scheduling.

**All Probabilities PS** (``examples/advanced/stateProbabilities/statepr_allprobs_ps.ipynb``)
   Complete state probability enumeration for processor sharing.

State-Dependent Routing
~~~~~~~~~~~~~~~~~~~~~~~

**Open State-Dependent Routing** (``examples/advanced/stateDepRouting/sdroute_open.ipynb``)
   Open network with routing decisions based on system state.

**Closed State-Dependent Routing** (``examples/advanced/stateDepRouting/sdroute_closed.ipynb``)
   Closed network with state-dependent routing policies.

**Two-Class State-Dependent** (``examples/advanced/stateDepRouting/sdroute_twoclasses_closed.ipynb``)
   Two-class closed network with state-dependent routing.

Initial State Specification
~~~~~~~~~~~~~~~~~~~~~~~~~~~

**FCFS Exponential Init** (``examples/advanced/initState/init_state_fcfs_exp.ipynb``)
   FCFS network with exponential service and specified initial state.

**FCFS Non-Exponential Init** (``examples/advanced/initState/init_state_fcfs_nonexp.ipynb``)
   FCFS network with non-exponential service and initial state.

**PS Initial State** (``examples/advanced/initState/init_state_ps.ipynb``)
   Processor sharing network with specified initial state.

Load-Dependent Services
~~~~~~~~~~~~~~~~~~~~~~~

**Multi-Server PS** (``examples/advanced/loadDependent/ld_multiserver_ps.ipynb``)
   Multi-server processor sharing with load-dependent rates.

**Multi-Server PS Two-Class** (``examples/advanced/loadDependent/ld_multiserver_ps_twoclasses.ipynb``)
   Two-class multi-server PS with load dependence.

**Multi-Server FCFS** (``examples/advanced/loadDependent/ld_multiserver_fcfs.ipynb``)
   Multi-server FCFS with load-dependent service rates.

**Class Dependence** (``examples/advanced/loadDependent/ld_class_dependence.ipynb``)
   Load dependence based on specific job classes.

Random Environment Models
~~~~~~~~~~~~~~~~~~~~~~~~~~

**Two-Stage Repairmen** (``examples/advanced/randomEnv/renv_twostages_repairmen.ipynb``)
   Two-stage random environment with repairmen model.

**Three-Stage Repairmen** (``examples/advanced/randomEnv/renv_threestages_repairmen.ipynb``)
   Three-stage random environment with repairmen model.

**Four-Stage Repairmen** (``examples/advanced/randomEnv/renv_fourstages_repairmen.ipynb``)
   Four-stage random environment with repairmen model.

Switchover Times
~~~~~~~~~~~~~~~~

**Basic Switchover** (``examples/advanced/switchoverTimes/switchover_basic.ipynb``)
   Queueing model with switchover times between service modes.

Reward Models
~~~~~~~~~~~~~

**Reward-Based CTMC Analysis** (``examples/advanced/rewardModel/example_rewardModel_1.ipynb``)
   Demonstrates custom reward functions for CTMC analysis using setReward, getReward, and getAvgReward methods.

Layered Cache-Queueing Models
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Single Host** (``examples/advanced/layeredCQ/lcq_singlehost.ipynb``)
   Layered cache-queueing model with single host.

**Three Hosts** (``examples/advanced/layeredCQ/lcq_threehosts.ipynb``)
   Layered cache-queueing model with three hosts.

Getting Started
---------------

For new users, we recommend starting with these examples in order:

1. **Basic M/M/1** - ``examples/gallery/gallery_mm1.ipynb``
2. **Open Network** - ``examples/basic/openQN/oqn_basic.ipynb``
3. **Closed Network** - ``examples/basic/closedQN/cqn_twoqueues.ipynb``
4. **Mixed Network** - ``examples/basic/mixedQN/mqn_basic.ipynb``

Example Code Pattern
--------------------

Most examples follow this general pattern:

.. code-block:: python

   from line_solver import *
   import numpy as np
   
   # Enable verbose output
   GlobalConstants.set_verbose(VerboseLevel.STD)
   
   # Create model
   model = Network('MyModel')
   
   # Define nodes
   source = Source(model, 'Source')
   queue = Queue(model, 'Queue', SchedStrategy.FCFS)
   sink = Sink(model, 'Sink')
   
   # Define job classes and service processes
   jobclass = OpenClass(model, 'Jobs')
   source.set_arrival(jobclass, Exp(1.0))
   queue.set_service(jobclass, Exp(2.0))
   
   # Define routing
   P = model.init_routing_matrix()
   P.add_route(jobclass, source, queue, 1.0)
   P.add_route(jobclass, queue, sink, 1.0)
   model.link(P)
   
   # Solve with preferred solver
   solver = MVA(model)
   avg_table = solver.avg_table()
   print(avg_table)