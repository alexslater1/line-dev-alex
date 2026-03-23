====================
Solver Reference
====================

This reference describes the main solver methods available in LINE. All solvers inherit from the ``NetworkSolver`` base class and share common methods for obtaining performance metrics. Each solver also provides specialized methods and algorithm options tailored to its solution approach.

Common NetworkSolver Methods
=============================

The following methods are available for **all** LINE solvers (AUTO, CTMC, FLD, JMT, LN, LQNS, MAM, MVA, NC, SSA). These methods provide standard steady-state performance metrics.

.. note::
   Not all solvers support all advanced methods (distributions, transient analysis, state probabilities). See solver-specific sections below for details.

Steady-State Average Metrics
-----------------------------

Basic Performance Metrics
~~~~~~~~~~~~~~~~~~~~~~~~~~

**avg()**
   Returns mean performance metrics (queue length, utilization, response time, throughput) for each station and class.

   **Parameters:** None

   **Returns:**
      Tuple of 5 numpy arrays: ``(QN, UN, RN, TN, AN)``

      - ``QN``: Queue length matrix with shape ``(num_stations, num_classes)``
      - ``UN``: Utilization matrix with shape ``(num_stations, num_classes)``
      - ``RN``: Response time matrix with shape ``(num_stations, num_classes)``
      - ``TN``: Throughput matrix with shape ``(num_stations, num_classes)``
      - ``AN``: Arrival rate matrix with shape ``(num_stations, num_classes)``

   **Example:**
      .. code-block:: python

         solver = MVA(model)
         QN, UN, RN, TN, AN = solver.avg()

         # Access queue length at station 0, class 1
         qlen = QN[0, 1]

**avg_table()**
   Returns the same metrics as ``avg()`` but formatted as a pandas DataFrame with station and class names for easier inspection.

   **Parameters:** None

   **Returns:**
      ``pandas.DataFrame`` with columns: ``['Node', 'JobClass', 'QLen', 'Util', 'RespT', 'ResidT', 'ArvR', 'Tput']``

      - Rows with all-zero metrics are filtered out
      - Each row represents a station-class combination

   **Example:**
      .. code-block:: python

         solver = MVA(model)
         df = solver.avg_table()
         print(df)

         # Filter to specific station
         queue1_metrics = df[df['Node'] == 'Queue1']

**avg_node()** / **avg_node_table()**
   Returns metrics for each node (including sinks) and class.

   **Returns:** ``pandas.DataFrame`` with same structure as ``avg_table()``

**avg_chain()** / **avg_chain_table()**
   Returns metrics aggregated by station and chain (instead of class).

   **Returns:**
      ``pandas.DataFrame`` with columns: ``['Station', 'Chain', 'QLen', 'Util', 'RespT', 'ResidT', 'ArvR', 'Tput']``

**avg_node_chain()** / **avg_node_chain_table()**
   Returns metrics aggregated by node and chain.

   **Returns:**
      ``pandas.DataFrame`` with columns: ``['Node', 'Chain', 'QLen', 'Util', 'RespT', 'ResidT', 'ArvR', 'Tput']``

**avg_sys()** / **avg_sys_table()**
   Returns system-level metrics (system response time and throughput) aggregated by chain.

   **avg_sys() Returns:**
      List ``[response_time, throughput]`` with system-level performance values

   **avg_sys_table() Returns:**
      ``pandas.DataFrame`` with columns: ``['Chain', 'JobClasses', 'SysRespT', 'SysTput']``

   .. note::
      System metrics are only defined by chain to avoid ambiguity when jobs switch classes within a chain.

Specialized Average Methods
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**avg_arv_r()**
   Returns arrival rates for each station and class.

   **Returns:** ``numpy.ndarray`` with shape ``(num_stations, num_classes)``

**avg_qlen()**
   Returns average queue length for each station and class.

   **Returns:** ``numpy.ndarray`` with shape ``(num_stations, num_classes)``

**avg_respt()**
   Returns average response time for each station and class.

   **Returns:** ``numpy.ndarray`` with shape ``(num_stations, num_classes)``

**avg_residt()**
   Returns average residence time for each station and class.

   **Returns:** ``numpy.ndarray`` with shape ``(num_stations, num_classes)``

**avg_tput()**
   Returns average throughput for each station and class.

   **Returns:** ``numpy.ndarray`` with shape ``(num_stations, num_classes)``

**avg_util()**
   Returns average utilization for each station and class.

   **Returns:** ``numpy.ndarray`` with shape ``(num_stations, num_classes)``

**avg_sys_respt()**
   Returns system-level average response time.

   **Returns:** ``numpy.ndarray`` - vector of system response times (one per class)

**avg_sys_tput()**
   Returns system-level average throughput.

   **Returns:** ``numpy.ndarray`` - vector of system throughputs (one per class)

Chain-Based Metrics
~~~~~~~~~~~~~~~~~~~

The following methods return metrics aggregated by routing chains instead of job classes:

**avg_qlen_chain()** / **avg_node_qlen_chain()**
   Returns queue length aggregated by chain.

   **Returns:** ``numpy.ndarray`` with chain-based aggregation

**avg_respt_chain()** / **avg_node_respt_chain()**
   Returns response time aggregated by chain.

   **Returns:** ``numpy.ndarray`` with chain-based aggregation

**avg_tput_chain()** / **avg_node_tput_chain()**
   Returns throughput aggregated by chain.

   **Returns:** ``numpy.ndarray`` with chain-based aggregation

**avg_util_chain()** / **avg_node_util_chain()**
   Returns utilization aggregated by chain.

   **Returns:** ``numpy.ndarray`` with chain-based aggregation

State Probability Methods
--------------------------

**get_prob_aggr(node, state=None)**
   Returns marginal state probabilities for jobs of different classes at a given station.

   **Parameters:**
      - ``node``: Network node (Queue, Delay, etc.) or node index (int)
      - ``state`` (optional): State specification as list/array. Default: None (returns all states)

   **Returns:**
      - ``float``: Single probability value (if result is scalar)
      - ``numpy.ndarray``: Matrix of probabilities (if multi-dimensional)
      - ``None``: If computation fails or is not supported

   **Example:**
      .. code-block:: python

         solver = CTMC(model)
         queue1 = model.nodes[0]

         # Get probability of specific state [2 jobs of class 0, 1 job of class 1]
         queue1.set_state([2, 1])
         prob = solver.get_prob_aggr(queue1)  # Returns float
         print(f"Probability of state [2,1]: {prob}")

         # Get all state probabilities (returns array)
         all_probs = solver.get_prob_aggr(queue1, state=None)

   **Supported by:** CTMC, FLD, JMT, MAM, MVA

**get_prob(node, state=None)**
   Returns state probabilities at equilibrium at a given station, including phase information (full state space).

   **Parameters:**
      - ``node``: Network node (Queue, Delay, etc.) or node index (int)
      - ``state`` (optional): Complete state specification including phase. Default: None

   **Returns:**
      - ``float``: Single probability value (if result is scalar)
      - ``numpy.ndarray``: Array of probabilities (if multi-dimensional)
      - ``None``: If computation fails

   **Example:**
      .. code-block:: python

         solver = CTMC(model)
         queue1 = model.nodes[0]

         # Get probability for a specific state with phase information
         prob = solver.get_prob(queue1, state=[1, 0, 2])  # [class0_jobs, class1_jobs, phase]

   **Note:** This method returns the full state space including phase information, whereas ``get_prob_aggr()`` aggregates over phases.

   **Supported by:** CTMC, SSA

**get_prob_sys()**
   Returns joint probabilities for the entire system state (all stations).

   **Parameters:** None

   **Returns:**
      - ``numpy.ndarray``: Multi-dimensional array of joint state probabilities
      - ``None``: If computation fails

   **Example:**
      .. code-block:: python

         solver = CTMC(model)
         sys_prob = solver.get_prob_sys()

         # Access probability of specific system state
         # prob[i, j, k] = probability of i jobs at station 0, j at station 1, k at station 2

   **Supported by:** CTMC, SSA

**get_prob_sys_aggr()**
   Returns joint probabilities for jobs of different classes across all stations (aggregated over phases).

   **Parameters:** None

   **Returns:**
      - ``ProbabilityResult`` object containing:
         - ``.probability``: numpy.ndarray of joint probabilities
         - ``.log_normalizing_constant``: float
         - ``.is_aggregated``: bool (True for this method)
      - ``None``: If computation fails

   **Example:**
      .. code-block:: python

         solver = JMT(model)
         result = solver.get_prob_sys_aggr()

         if result is not None:
             prob_array = result.probability
             log_norm = result.log_normalizing_constant
             print(f"System probabilities shape: {prob_array.shape}")

   **Supported by:** CTMC, JMT, NC

**get_prob_norm_const_aggr()**
   Returns the normalizing constant of state probabilities.

   **Parameters:** None

   **Returns:**
      - ``float``: The normalizing constant value
      - ``None``: If computation fails

   **Example:**
      .. code-block:: python

         solver = NC(model)
         norm_const = solver.get_prob_norm_const_aggr()
         print(f"Normalizing constant: {norm_const}")

   **Note:** The normalizing constant is used to normalize state probabilities in closed queueing networks.

   **Supported by:** JMT, MVA, NC

Distribution Methods
--------------------

**get_cdf_respt(R=None)**
   Returns the cumulative distribution function (CDF) of response times for each station and class at steady-state.

   **Parameters:**
      - ``R`` (optional): Response time handle from a previous computation. Default: None (computes new CDF)

   **Returns:**
      Nested list with shape ``[num_stations][num_classes]``

      - Each element ``[i][k]`` contains either:
         - ``numpy.ndarray`` with shape ``(n_points, 2)`` where:
            - Column 0: CDF values (cumulative probability from 0 to 1)
            - Column 1: Time points (response time values)
         - ``None`` if not available for that station-class pair

   **Example:**
      .. code-block:: python

         solver = FLD(model)
         cdf_data = solver.get_cdf_respt()

         # Access CDF for station 1, class 0
         if cdf_data[1][0] is not None:
             cdf_values = cdf_data[1][0][:, 0]  # Cumulative probabilities
             time_points = cdf_data[1][0][:, 1]  # Response times

             # Plot complementary CDF (CCDF)
             import matplotlib.pyplot as plt
             plt.semilogx(time_points, 1 - cdf_values)

   **Supported by:** CTMC, FLD, JMT, MAM, SSA

**get_cdf_passt(R=None)**
   Returns the cumulative distribution function (CDF) of passage times between stations at steady-state.

   **Parameters:**
      - ``R`` (optional): Passage time handle from a previous computation. Default: None

   **Returns:**
      Nested list with the same structure as ``get_cdf_respt()``

      - Shape: ``[num_stations][num_classes]``
      - Each element contains ``numpy.ndarray`` with shape ``(n_points, 2)`` or ``None``

   **Example:**
      .. code-block:: python

         solver = CTMC(model)
         passage_cdf = solver.get_cdf_passt()

         # Check if passage time CDF is available for station 0, class 1
         if passage_cdf[0][1] is not None:
             print(f"Passage time CDF has {len(passage_cdf[0][1])} points")

   **Supported by:** CTMC

Transient Analysis Methods
---------------------------

.. note::
   Transient analysis requires specifying a ``timespan`` parameter when creating the solver.
   Example: ``solver = JMT(model, timespan=[0, 100])``

**get_tran_avg(Qt=None, Ut=None, Tt=None)**
   Returns transient mean performance metrics (queue length, utilization, throughput) over time for every station and class.

   **Parameters:**
      - ``Qt`` (optional): Queue length handles from previous computation. Default: None
      - ``Ut`` (optional): Utilization handles from previous computation. Default: None
      - ``Tt`` (optional): Throughput handles from previous computation. Default: None

   **Returns:**
      The return structure varies by solver:

      **FLD/CTMC:** Dictionary with keys ``['QNt', 'UNt', 'TNt']``

      - Each value is a nested list: ``[[station_0_classes], [station_1_classes], ...]``
      - Each element is ``numpy.ndarray`` containing time series data

      **JMT:** Tuple ``(QNclass_t, UNclass_t, TNclass_t)``

      - Each element has shape ``[num_stations][num_classes]``
      - Each ``[i][k]`` is either ``None`` or a dict with:
         - ``'handle'``: tuple of (station_obj, class_obj)
         - ``'t'``: list of time points
         - ``'metric'``: list of metric values at each time point
         - ``'isaggregate'``: bool (True)

   **Example:**
      .. code-block:: python

         # FLD solver - returns dictionary
         solver = FLD(model, timespan=[0, 50])
         result = solver.get_tran_avg()

         QNt = result['QNt']  # Queue length over time
         UNt = result['UNt']  # Utilization over time
         TNt = result['TNt']  # Throughput over time

         # Access queue length time series for station 0, class 1
         qlen_series = QNt[0][1]  # numpy array

         # JMT solver - returns tuple of nested lists
         solver_jmt = JMT(model, timespan=[0, 100], timestep=1.0)
         QN_t, UN_t, TN_t = solver_jmt.get_tran_avg()

         # Access specific station-class time series
         if QN_t[0][1] is not None:
             time_points = QN_t[0][1]['t']
             qlen_values = QN_t[0][1]['metric']

   **Solver Configuration:**
      - ``timespan``: Time range as ``[t_start, t_end]`` (required)
      - ``timestep``: Time resolution (optional, solver-specific default)

   **Supported by:** CTMC, FLD, JMT

**get_tran_cdf_respt(R=None)**
   Returns response time distribution in the transient regime.

   **Parameters:**
      - ``R`` (optional): Response time handle from previous computation. Default: None

   **Returns:**
      Nested list with shape ``[num_stations][num_classes]``

      - Same structure as ``get_cdf_respt()``
      - Each element is ``numpy.ndarray`` with shape ``(n_points, 2)`` or ``None``

   **Example:**
      .. code-block:: python

         solver = JMT(model, timespan=[0, 100])
         tran_cdf = solver.get_tran_cdf_respt()

         # Plot transient response time CDF
         if tran_cdf[0][0] is not None:
             cdf_vals = tran_cdf[0][0][:, 0]
             time_vals = tran_cdf[0][0][:, 1]

   **Supported by:** JMT

**get_tran_cdf_passt(R=None)**
   Returns first passage time distributions in the transient regime.

   **Parameters:**
      - ``R`` (optional): Passage time handle from previous computation. Default: None

   **Returns:**
      - ``DistributionResult`` object or ``None``

   **Example:**
      .. code-block:: python

         solver = FLD(model, timespan=[0, 50])
         passage_dist = solver.get_tran_cdf_passt()

         if passage_dist is not None:
             # Access distribution data
             print(f"Distribution type: {passage_dist.distribution_type}")
             print(f"Is transient: {passage_dist.is_transient}")

   **Supported by:** FLD

**get_tran_prob(node)**
   Returns transient state probabilities for a specific node over time.

   **Parameters:**
      - ``node``: Network node (Queue, Delay, etc.) or node index (int)

   **Returns:**
      - ``ProbabilityResult`` object containing transient probabilities or ``None``

   **Example:**
      .. code-block:: python

         solver = CTMC(model, timespan=[0, 50])
         queue1 = model.nodes[0]

         prob_result = solver.get_tran_prob(queue1)
         if prob_result is not None:
             probs = prob_result.probability  # numpy array

   **Supported by:** CTMC

**get_tran_prob_aggr(node)**
   Returns transient marginal state probabilities aggregated by class for a specific node.

   **Parameters:**
      - ``node``: Network node (Queue, Delay, etc.) or node index (int)

   **Returns:**
      - ``ProbabilityResult`` object containing aggregated transient probabilities or ``None``

   **Example:**
      .. code-block:: python

         solver = CTMC(model, timespan=[0, 50])
         prob_aggr = solver.get_tran_prob_aggr(model.nodes[0])

   **Supported by:** CTMC

**get_tran_prob_sys()**
   Returns transient joint system state probabilities over time.

   **Parameters:** None

   **Returns:**
      - ``ProbabilityResult`` object with system-wide transient probabilities or ``None``

   **Example:**
      .. code-block:: python

         solver = CTMC(model, timespan=[0, 50])
         sys_probs = solver.get_tran_prob_sys()

         if sys_probs is not None:
             prob_matrix = sys_probs.probability

   **Supported by:** CTMC

**get_tran_prob_sys_aggr()**
   Returns transient joint probabilities aggregated by class for the entire system.

   **Parameters:** None

   **Returns:**
      - ``ProbabilityResult`` object with aggregated system transient probabilities or ``None``

   **Example:**
      .. code-block:: python

         solver = CTMC(model, timespan=[0, 50])
         sys_aggr = solver.get_tran_prob_sys_aggr()

   **Supported by:** CTMC

Sample Path Methods
-------------------

.. note::
   Sample path methods generate discrete-event simulation traces showing the evolution of the system state over time.
   These are primarily used for validation, visualization, and detailed analysis of system dynamics.

**sample(node, numSamples, markActivePassive=False)**
   Returns a sample path of the state evolution at a given node, including detailed phase information.

   **Parameters:**
      - ``node``: Network node (Queue, Delay, etc.) to sample
      - ``numSamples``: Number of samples/events to generate (int)
      - ``markActivePassive`` (optional): Mark active/passive jobs (SSA only). Default: False

   **Returns:**
      - ``SampleResult`` object containing:
         - ``.handle``: Unique identifier (str)
         - ``.t``: Array of time points
         - ``.state``: State information at each time point (includes phase details)
         - ``.event``: List of events that occurred
         - ``.isaggregate``: False (not aggregated)
         - ``.nodeIndex``: Index of the sampled node
         - ``.numSamples``: Number of samples collected
      - ``None``: If sampling fails

   **Example:**
      .. code-block:: python

         solver = SSA(model, seed=12345)
         queue1 = model.nodes[0]

         sample_result = solver.sample(queue1, numSamples=1000)
         if sample_result is not None:
             times = sample_result.t
             states = sample_result.state
             events = sample_result.event

             # Plot sample path
             import matplotlib.pyplot as plt
             plt.step(times, states[:, 0], where='post')  # State of first class

   **Supported by:** CTMC, SSA

**sample_aggr(node, numSamples, markActivePassive=False)**
   Returns a sample path with aggregated state (number of jobs per class, without phase information).

   **Parameters:**
      - ``node``: Network node to sample
      - ``numSamples``: Number of samples/events to generate (int)
      - ``markActivePassive`` (optional): Mark active/passive jobs (SSA only). Default: False

   **Returns:**
      - ``SampleResult`` object with:
         - ``.isaggregate``: True (aggregated over phases)
         - Other fields same as ``sample()``
      - ``None``: If sampling fails

   **Example:**
      .. code-block:: python

         solver = JMT(model, seed=23000, samples=5000)
         queue1 = model.nodes[0]

         sample_aggr = solver.sample_aggr(queue1, numSamples=500)
         if sample_aggr is not None:
             # Aggregated state (just job counts per class, no phase info)
             job_counts = sample_aggr.state
             print(f"Collected {sample_aggr.numSamples} samples")

   **Supported by:** CTMC, JMT, SSA

**sample_sys(numEvents)**
   Returns sample paths for all nodes in the system with detailed phase information.

   **Parameters:**
      - ``numEvents``: Number of events to simulate (int) [CTMC] or number of samples (int) [SSA]

   **Returns:**
      - ``SampleResult`` object with system-wide sample paths including phase details
      - ``None``: If sampling fails

   **Example:**
      .. code-block:: python

         solver = SSA(model, seed=54321)
         sys_sample = solver.sample_sys(numEvents=2000)

         if sys_sample is not None:
             # Contains sample paths for all stations
             times = sys_sample.t
             system_states = sys_sample.state  # Multi-dimensional: [time, station, class, phase]

   **Supported by:** CTMC, SSA

**sample_sys_aggr(numEvents)**
   Returns sample paths for all nodes with aggregated state (job counts per class, no phase information).

   **Parameters:**
      - ``numEvents``: Number of events to simulate (int) [CTMC] or number of samples (int) [SSA, JMT]

   **Returns:**
      - ``SampleResult`` object with system-wide aggregated sample paths
      - ``None``: If sampling fails

   **Example:**
      .. code-block:: python

         solver = JMT(model, seed=99999, samples=10000)
         sys_sample = solver.sample_sys_aggr(numEvents=1000)

         if sys_sample is not None:
             # Aggregated system state over time
             times = sys_sample.t
             states = sys_sample.state  # Shape: [time, station, class]

             # Analyze queue lengths at station 0 over time
             queue0_state = states[:, 0, :]  # All classes at station 0

   **Supported by:** CTMC, JMT, SSA

Solver Options
--------------

All solvers accept an ``options`` parameter or keyword arguments. Common options include:

- ``cache`` (bool): Cache solver results (default: True)
- ``cutoff`` (int): Maximum jobs per station/class (required for CTMC with open classes)
- ``force`` (bool): Bypass solver feasibility checks (default: False)
- ``iter_max`` (int): Maximum iterations for iterative solvers (default: 1000)
- ``iter_tol`` (float): Iteration convergence tolerance (default: 1e-6)
- ``keep`` (bool): Store intermediate files for debugging (default: False)
- ``method`` (str): Algorithm selection (solver-specific)
- ``samples`` (int): Number of samples for simulation-based solvers (default: 10000)
- ``seed`` (int): Random seed for stochastic solvers
- ``timespan`` (list): Time range for transient analysis (e.g., [0, 100])
- ``tol`` (float): General numerical tolerance (default: 1e-4)
- ``verbose`` (int): Verbosity level (0=silent, 1=standard, 2=debug)

Individual Solvers
==================

AUTO Solver
-----------

**Purpose:** Automatic solver selection based on model properties. The AUTO solver analyzes the model structure and dynamically selects the most appropriate solution method from available solvers (CTMC, NC, JMT, SSA, etc.).

The AUTO solver prioritizes analytical solvers over simulation and uses heuristics based on scheduling strategies, number of jobs, chains, and classes to make its selection. If the optimal solver does not support a requested function, it selects from feasible solvers in order of expected execution time.

**Typical Usage:**

.. code-block:: python

   from line_solver import Network, Queue, ClosedClass, SchedStrategy, Exp
   from line_solver.solvers import AUTO

   model = Network('mymodel')
   queue = Queue(model, 'Queue1', SchedStrategy.FCFS)
   job_class = ClosedClass(model, 'Class1', 5, queue)
   queue.set_service(job_class, Exp(1.0))

   solver = AUTO(model)
   avg_table = solver.avg_table()
   print(avg_table)

**Available Methods:** All common NetworkSolver methods (the AUTO solver will delegate to an appropriate backend solver).

**Configuration:** No solver-specific options. The AUTO solver accepts standard options which it passes to the selected backend solver.

CTMC Solver
-----------

**Purpose:** Continuous-Time Markov Chain (CTMC) solver providing **exact solutions** via explicit generation of the underlying CTMC state space. This is the only method that guarantees exact results for all Markovian models, but it suffers from state-space explosion for larger models.

The CTMC solver is recommended for small to medium models (heuristically limited to ~6000 states) where exact solutions are required and the model is fully Markovian.

**Typical Usage:**

.. code-block:: python

   from line_solver.solvers import CTMC

   # Steady-state analysis
   solver = CTMC(model)
   avg_table = solver.avg_table()

   # Transient analysis at time t=10
   solver_transient = CTMC(model, timespan=[0, 10])
   qn_t = solver_transient.get_tran_avg()

   # State probabilities
   prob_aggr = solver.get_prob_aggr()
   prob_sys = solver.get_prob_sys()

**Solver-Specific Methods:**

- ``get_cdf_respt()``: Response time distribution at steady-state
- ``get_tran_cdf_passt()``: First passage time distribution in transient regime
- ``get_tran_cdf_respt()``: Response time distribution in transient regime
- ``get_prob()``: State probabilities at equilibrium
- ``get_prob_aggr()``: Marginal state probabilities by class
- ``get_prob_sys()``: Joint system state probabilities
- ``get_prob_sys_aggr()``: Joint probabilities by class
- ``get_tran_avg()``: Transient performance metrics
- ``get_tran_prob()``, ``get_tran_prob_aggr()``, ``get_tran_prob_sys()``, ``get_tran_prob_sys_aggr()``: Various transient probability methods

**Solution Methods** (``method`` option):

- ``'default'``: Global balance solution (default)

*QRF approximation methods* — approximate performance metrics via nonlinear programming
(MMI or MEM objectives). All implemented in ``matlab/lib/qrf/``:

- ``'qrf.mmi'``: MMI objective, no blocking (calls ``qrf_noblo_mmi``)
- ``'qrf.mem'``: MEM objective, no blocking (calls ``qrf_noblo_mem``)
- ``'qrf.mmi.ld'``: MMI with load-dependent service rates, no blocking (calls ``qrf_noblo_mmi_ld``)
- ``'qrf.mmi.linear'``: MMI with Aeq/beq linear constraint form, no blocking (calls ``qrf_noblo_mmi_linear``)
- ``'qrf.bas.mmi'``: MMI for BAS networks, simplified parameters (calls ``qrf_bas_mmi_simple``)
- ``'qrf.bas.mem'``: MEM for BAS networks, full blocking configuration (calls ``qrf_bas_mem``)

*QRF bounds methods* — compute utilization bounds via linear programming:

- ``'qrf.bas'``: Utilization bounds for BAS networks (calls ``qrf_bas``)
- ``'qrf.rsrd'``: Utilization bounds for RS-RD blocking networks (calls ``qrf_rsrd``)

.. note::

   QRF methods require the MATLAB Optimization Toolbox (``fmincon`` / ``linprog``)
   and only support single-class closed queueing networks with PH service distributions.
   The approximation methods (``qrf.mmi``, ``qrf.mem``, etc.) return estimated performance
   metrics, while the bounds methods (``qrf.bas``, ``qrf.rsrd``) return utilization bounds.

**Key Options:**

- ``cutoff`` (int): **Mandatory** for models with open classes. Maximum number of jobs per station and class.
- ``force`` (bool): Bypass state-space size checks (use with caution)
- ``timespan`` (list): Temporal range for transient analysis (e.g., [0, 50])
- ``timestep`` (float): Fixed time interval for transient sampling (adaptive by default)
- ``hide_immediate`` (bool): Hide immediate transitions from CTMC
- ``state_space_gen`` (str): 'reachable' (default) or 'full' state space enumeration

**QRF-specific Options** (under ``config``):

- ``qrf_params`` (struct): Blocking configuration for BAS methods (``qrf.bas.mmi``, ``qrf.bas.mem``, ``qrf.bas``). Fields: ``f`` (finite capacity queue index), ``MR`` (number of blocking configurations), ``BB`` (blocking state matrix), ``F`` (capacity vector), ``MM``, ``MM1``, ``ZZ``, ``ZM`` (blocking order parameters).
- ``qrf_alpha`` (matrix): Load-dependent service rates, ``M x N`` matrix where ``alpha(i,n)`` scales the service rate at station ``i`` when ``n`` jobs are present. Used by ``qrf.mmi.ld`` and ``qrf.mmi.linear``.

**Example:**

.. code-block:: python

   # CTMC with open classes requires cutoff
   solver = CTMC(model, cutoff=10)

   # Transient analysis with specific timestep
   solver = CTMC(model, timespan=[0, 100], timestep=1.0)
   tran_avg = solver.get_tran_avg()

.. code-block:: matlab

   % QRF approximation for a closed network (MATLAB)
   solver = SolverCTMC(model, 'method', 'qrf.mmi');
   [Q, U, R, T] = solver.getAvg();

   % QRF with load-dependent rates
   opts = SolverCTMC.defaultOptions();
   opts.method = 'qrf.mmi.ld';
   opts.config.qrf_alpha = ones(M, N);  % uniform rates (default)
   solver = SolverCTMC(model, opts);

   % QRF utilization bounds for a BAS network
   opts = SolverCTMC.defaultOptions();
   opts.method = 'qrf.bas';
   opts.config.qrf_params = struct('f',1, 'MR',1, 'BB',zeros(1,M), ...
       'MM',zeros(1,2), 'MM1',zeros(1,M), 'ZZ',0, 'ZM',0);
   solver = SolverCTMC(model, opts);

   % QRF via LINE.load dispatch
   solver = LINE.load('ctmc.qrf.mmi', model);

FLD Solver
----------

**Purpose:** Fluid/Mean-Field Approximation solver. Approximates model behavior using Ordinary Differential Equation (ODE)-based mean-field approximations. The FLD solver analyzes models via a system of ODEs, providing faster solutions than CTMC but with some approximation error.

The FLD approximation becomes exact in the limit for processor-sharing (PS) and infinite-server (INF) scheduling. It scales better than CTMC for larger models and is particularly effective for models with many jobs.

**Typical Usage:**

.. code-block:: python

   from line_solver.solvers import FLD

   # Steady-state analysis
   solver = FLD(model)
   avg_table = solver.avg_table()

   # Response time distribution
   cdf_respt = solver.get_cdf_respt()

   # Transient analysis
   solver_transient = FLD(model, timespan=[0, 50])
   tran_avg = solver_transient.get_tran_avg()

**Solver-Specific Methods:**

- ``get_cdf_respt()``: Response time distribution
- ``get_prob_aggr()``: Marginal state probabilities by class
- ``get_tran_avg()``: Transient performance metrics
- ``get_tran_cdf_passt()``: First passage time distributions

**Solution Methods** (``method`` option):

- ``'default'`` or ``'matrix'``: ODE-based mean field approximations (default)
- ``'closing'``: FLD with closing method for open classes
- ``'statedep'``: Kurtz's mean field ODEs for closed models
- ``'softmin'``: Smoothed statedep with softmin replacing min functions

**Key Options:**

- ``stiff`` (bool): Use stiff ODE solver (default: True)
- ``iter_tol`` (float): ODE solver tolerance (default: 1e-4)
- ``iter_max`` (int): Maximum iterations (default: 10)
- ``timespan`` (list): Temporal range (default: [0, inf] for steady-state)

**Example:**

.. code-block:: python

   # FLD with specific method and tolerance
   solver = FLD(model, method='statedep', iter_tol=1e-6)
   avg_table = solver.avg_table()

JMT Solver
----------

**Purpose:** Wrapper for the Java Modelling Tools (JMT) simulation (JSIM) and analytical solver (JMVA). The JMT solver supports the widest range of model features including non-Markovian distributions (Pareto, deterministic, empirical traces).

JMT is the most comprehensive solver in LINE, supporting virtually all model features through discrete-event simulation. It can also invoke JMT's analytical MVA solver for product-form models.

**Typical Usage:**

.. code-block:: python

   from line_solver.solvers import JMT

   # Simulation with specific seed and samples
   solver = JMT(model, seed=23000, samples=50000)
   avg_table = solver.avg_table()

   # Sample path generation
   sample_path = solver.sample_aggr(queue, num_samples=100)

   # Transient response time distribution
   tran_cdf = solver.get_tran_cdf_respt()

**Solver-Specific Methods:**

- ``get_cdf_respt()``: Response time distribution
- ``get_prob_aggr()``: Marginal state probabilities
- ``get_prob_sys_aggr()``: Joint probabilities by class
- ``sample_aggr()``: Sample path for a station (aggregated by class)
- ``sample_sys_aggr()``: Sample path for system (aggregated by class)
- ``get_tran_cdf_respt()``: Transient response time distribution
- ``get_tran_avg()``: Transient performance metrics

.. note::
   JMT does NOT support ``sample()`` or ``sample_sys()`` with detailed phase information due to simulator limitations.

**Solution Methods** (``method`` option):

Simulation methods:
   - ``'jsim'`` or ``'default'``: Discrete-event simulation in JSIM (default)

Analytical methods (product-form models only):
   - ``'jmva'`` or ``'jmva.mva'``: Exact MVA in JMVA
   - ``'jmva.recal'``: Exact RECAL algorithm
   - ``'jmva.comom'``: Exact CoMoM algorithm
   - ``'jmva.amva'`` or ``'jmva.bs'``: Bard-Schweitzer approximate MVA
   - ``'jmva.aql'``: AQL algorithm
   - ``'jmva.chow'``: Chow algorithm
   - ``'jmva.dmlin'``: De Souza-Muntz Linearizer
   - ``'jmva.lin'``: Linearizer algorithm
   - ``'jmva.ls'``: Logistic sampling

**Key Options:**

- ``seed`` (int): Random number generator seed for simulation (default: random)
- ``samples`` (int): Number of simulation samples per metric (minimum 5000, default: 10000)
- ``para`` (bool): Enable parallel simulation with multiple independent replicas
- ``keep`` (bool): Store intermediate .jsimg/.jsimw files for debugging

**Example:**

.. code-block:: python

   # High-precision simulation
   solver = JMT(model, seed=12345, samples=100000, verbose=1)
   avg_table = solver.avg_table()

   # Use analytical JMVA solver
   solver_analytical = JMT(model, method='jmva.mva')
   avg_table = solver_analytical.avg_table()

LN Solver
---------

**Purpose:** LINE's native solver for Layered Queueing Networks (LQNs) with task-entry activity graphs and inter-task calls. Supports decomposition of layers with iterative analysis and specialized cache modeling.

The LN solver iteratively analyzes each layer of the network until convergence of steady-state measures, allowing flexible composition of different solver types for individual layers.

**Typical Usage:**

.. code-block:: python

   from line_solver.solvers import LN, MVA

   # LN solver with MVA for layer analysis
   solver = LN(layered_model, layer_solver_fn=lambda layer: MVA(layer))
   avg_table = solver.avg_table()

**Available Methods:** All standard steady-state average methods (avg, avg_table, etc.)

**Solution Methods** (``method`` option):

- ``'default'``: Default recursive solution based on mean values
- ``'moment3'``: Solution by recursive 3-moment approximation of response time distributions

**Key Options:**

- ``iter_tol`` (float): Convergence tolerance for layer iterations (default: 1e-6)

LQNS Solver
-----------

**Purpose:** Wrapper around the external LQNS solver for layered queueing networks. Transforms LINE models into LQNS XML format, invokes the external solver, and parses results back into LINE format.

.. note::
   Requires ``lqns`` and ``lqsim`` executables to be available on the system PATH.

**Typical Usage:**

.. code-block:: python

   from line_solver.solvers import LQNS

   solver = LQNS(layered_model)
   avg_table = solver.avg_table()

**Solution Methods** (``method`` option):

- ``'std'`` or ``'lqns'``: LQNS analytical solver with default settings (default)
- ``'exact'``: LQNS analytical solver with exact MVA method
- ``'srvn'``: LQNS analytical solver with SRVN layering
- ``'srvnexact'``: LQNS with SRVN layering and exact MVA
- ``'lqsim'``: LQSIM simulator (specify simulation length via ``samples`` option)

**Key Options:**

- ``samples`` (int): Simulation length for lqsim method

MAM Solver
----------

**Purpose:** Matrix-Analytic Methods (MAM) solver providing **exact solutions** for open Markovian systems via quasi-birth-death (QBD) process analysis. Solves infinite state-space models exactly by exploiting repetitive CTMC structure.

The MAM solver is particularly effective for open queueing models with MAP/PH arrivals and services, providing exact results without state-space truncation.

**Typical Usage:**

.. code-block:: python

   from line_solver.solvers import MAM

   solver = MAM(model)
   avg_table = solver.avg_table()

   # Response time distribution
   cdf_respt = solver.get_cdf_respt()

**Solver-Specific Methods:**

- ``get_cdf_respt()``: Response time distribution
- ``get_prob_aggr()``: Marginal state probabilities
- ``me_open()``: Maximum Entropy Method for open queueing networks (approximation)

**Solution Methods** (``method`` option):

- ``'default'``: Matrix-analytic solution of structured QBDs (default)
- ``'dec.source'``: Decomposition with arrivals as from the source
- ``'dec.poisson'``: Decomposition based on Poisson arrival flows
- ``'dec.mna'``: Decomposition based on MNA method

**Maximum Entropy Method (MEM):**

The ``me_open()`` method provides an approximate solution for open queueing networks using the Maximum Entropy principle from Kouvatsos (1994). This method is particularly useful for:

- Open networks with general (non-exponential) service distributions
- Networks with feedback routing
- Multiclass queueing systems
- Analysis requiring SCV (squared coefficient of variation) computation

The MEM algorithm iteratively computes:
   - Queue lengths (L)
   - Response times (W)
   - Arrival/departure SCVs (Ca/Cd)
   - Utilizations (rho)
   - Throughputs (lambda)

**Configuration options** (via ``config`` sub-options):
   - ``config.mem_tol`` (float): Convergence tolerance (default: 1e-6)
   - ``config.mem_maxiter`` (int): Maximum iterations (default: 1000)
   - ``config.mem_verbose`` (bool): Print iteration details (default: False)

**Example:**

.. code-block:: python

   from line_solver.solvers import SolverMAM

   solver = SolverMAM(model)
   # Configure MEM options
   solver.options.config.mem_tol = 1e-7
   solver.options.config.mem_maxiter = 2000

   # Run Maximum Entropy analysis
   result = solver.me_open()

   # Access results
   queue_lengths = result['L']
   response_times = result['W']
   utilizations = result['rho']

**Reference:** D.D. Kouvatsos, "Entropy Maximisation and Queueing Network Models", Annals of Operations Research, 48:63-126, 1994.

MVA Solver
----------

**Purpose:** Mean Value Analysis (MVA) solver providing fast approximate and exact solutions for product-form queueing networks. Typically the fastest solver with good accuracy for single-server stations.

The MVA solver is ideal for closed and mixed queueing networks with product-form properties. It provides various approximation algorithms trading speed for accuracy, as well as exact methods for smaller models.

**Typical Usage:**

.. code-block:: python

   from line_solver.solvers import MVA

   # Exact MVA
   solver = MVA(model, method='exact')
   avg_table = solver.avg_table()

   # Approximate MVA (faster for large models)
   solver_approx = MVA(model, method='qd')
   avg_table = solver_approx.avg_table()

**Solver-Specific Methods:**

All standard steady-state average methods are supported.

.. note::
   MVA **cannot** produce response time distributions due to the nature of mean-value analysis.

**Solution Methods** (``method`` option):

Approximation algorithms:
   - ``'default'``, ``'amva'``, or ``'qd'``: Queue-dependent approximate MVA (default)
   - ``'bs'``: Bard-Schweitzer approximate MVA
   - ``'lin'``: Linearizer approximate MVA
   - ``'qdlin'``: Queue-dependent Linearizer approximate MVA
   - ``'exact'``: Exact solution (method depends on model features)

Bounds methods:
   - ``'aba.upper'`` / ``'aba.lower'``: Asymptotic bound analysis
   - ``'bjb.upper'`` / ``'bjb.lower'``: Balanced job bounds
   - ``'gb.upper'`` / ``'gb.lower'``: Geometric square-root bounds
   - ``'pb.upper'`` / ``'pb.lower'``: Proportional bounds
   - ``'sb.upper'`` / ``'sb.lower'``: Simple bounds

Single-station formulas (for tandem/cyclic networks):
   - ``'mm1'``: Exact M/M/1 formula
   - ``'mmk'``: Exact M/M/k (Erlang-C) formula
   - ``'mg1'``: M/G/1 (Pollaczek-Khinchine) formula
   - ``'gig1.klb'``, ``'gig1.allen'``, ``'gig1.heyman'``, ``'gig1.kingman'``, ``'gig1.kobayashi'``, ``'gig1.marchal'``: Various GI/G/1 approximations
   - ``'gigk'``: Kingman approximation for GI/G/k

**Key Options:**

- ``method`` (str): Algorithm selection (see above)
- ``iter_max`` (int): Maximum iterations (default: 1000)
- ``iter_tol`` (float): Convergence tolerance (default: 1e-6)

Advanced configuration (via ``config`` sub-options):
   - ``config.multiserver`` (str): Multiserver handling ('default', 'seidmann', 'softmin')
   - ``config.np_priority`` (str): Non-preemptive priority ('default'/'cl', 'shadow')
   - ``config.highvar`` (str): High variance handling ('default', 'interp', 'hvmva')
   - ``config.fork_join`` (str): Fork-join handling ('default'/'mmt', 'ht')

**Example:**

.. code-block:: python

   # MVA with specific approximation and tolerance
   solver = MVA(model, method='lin', iter_tol=1e-8, iter_max=5000)
   avg_table = solver.avg_table()

   # MVA with advanced multiserver configuration
   options = MVA.default_options()
   options.method = 'qd'
   options.config.multiserver = 'seidmann'
   solver = MVA(model, options)

NC Solver
---------

**Purpose:** Normalizing Constant Analyzer (NC) providing exact and approximate solutions based on the normalizing constant of state probability distributions. Maps the problem to multidimensional integrals solved with Monte Carlo sampling and asymptotic expansions.

The NC solver is particularly useful for obtaining marginal and joint state probabilities in closed queueing networks, offering multiple exact and approximate algorithms.

**Typical Usage:**

.. code-block:: python

   from line_solver.solvers import NC

   # Exact convolution algorithm
   solver = NC(model, method='ca')
   avg_table = solver.avg_table()

   # State probabilities
   prob_aggr = solver.get_prob_aggr()
   prob_sys_aggr = solver.get_prob_sys_aggr()
   norm_const = solver.get_prob_norm_const_aggr()

**Solver-Specific Methods:**

- ``get_prob_aggr()``: Marginal state probabilities by class
- ``get_prob_sys_aggr()``: Joint probabilities by class
- ``get_prob_norm_const_aggr()``: Normalizing constant

**Solution Methods** (``method`` option):

Automatic selection:
   - ``'default'`` or ``'adaptive'``: Automated choice of deterministic method (default)
   - ``'exact'``: Automated choice of exact solution method
   - ``'sampling'``: Automated selection of sampling method

Exact methods:
   - ``'ca'``: Multiclass convolution algorithm
   - ``'comom'``: Class-oriented method of moments (for homogeneous models)
   - ``'mva'``: Product of throughputs on MVA lattice

Approximation methods:
   - ``'cub'``: Grundmann-Moeller cubature rules
   - ``'kt'``: Knessl-Tier asymptotic expansion
   - ``'le'``: Logistic asymptotic expansion
   - ``'ls'``: Logistic sampling
   - ``'imci'``: Improved Monte Carlo integration
   - ``'nr.logit'``: Norlund-Rice integral with logit transformation
   - ``'nr.probit'``: Norlund-Rice integral with probit transformation
   - ``'panacea'``: Panacea asymptotic expansion
   - ``'rd'``: Reduction heuristic

**Example:**

.. code-block:: python

   # NC with specific exact method
   solver = NC(model, method='comom')
   avg_table = solver.avg_table()

   # NC with sampling for large models
   solver_sampling = NC(model, method='ls', samples=50000)
   avg_table = solver_sampling.avg_table()

SSA Solver
----------

**Purpose:** Stochastic Simulation Algorithm (SSA) solver based on CTMC stochastic simulation. Estimates the probability distribution of system states by generating sample paths, deriving performance metrics indirectly.

The SSA solver offers more efficient parallelization than JMT and can retrieve detailed node state evolution including active phases of service distributions.

**Typical Usage:**

.. code-block:: python

   from line_solver.solvers import SSA

   # SSA simulation
   solver = SSA(model, samples=50000, seed=12345)
   avg_table = solver.avg_table()

   # Detailed sample path with phase information
   sample_path = solver.sample(queue, num_samples=100)
   sample_sys = solver.sample_sys(num_samples=100)

**Solver-Specific Methods:**

- ``get_cdf_respt()``: Response time distribution (via simulation)
- ``get_prob()``: State probabilities at equilibrium
- ``get_prob_sys()``: Joint system state probabilities
- ``sample()``: Sample path for a station (with phase information)
- ``sample_aggr()``: Sample path for a station (aggregated by class)
- ``sample_sys()``: Sample paths for all stations (with phase information)
- ``sample_sys_aggr()``: Sample paths for all stations (aggregated by class)

**Solution Methods** (``method`` option):

- ``'default'``: Alias for 'nrm' if model supports it, otherwise 'serial' (default)
- ``'nrm'``: Next reaction method for population models (PS/INF scheduling only)
- ``'serial'``: CTMC stochastic simulation on single core
- ``'para'``: Parallel simulations with independent replicas on multiple cores

**Key Options:**

- ``samples`` (int): Number of simulation samples (default: 10000)
- ``seed`` (int): Random number generator seed
- ``para`` (bool): Enable parallel execution
- ``state_space_gen`` (str): 'reachable' (default) or 'full' state space generation

**Example:**

.. code-block:: python

   # Parallel SSA with many samples
   solver = SSA(model, method='para', samples=100000, seed=42)
   avg_table = solver.avg_table()

   # SSA with full state space generation
   solver_full = SSA(model, state_space_gen='full')
   prob_sys = solver_full.get_prob_sys()

LDES Solver
----------

**Purpose:** LINE Discrete Event Simulator (LDES) solver using the SSJ (Stochastic Simulation in Java) library. The LDES solver provides comprehensive simulation support for queueing networks and stochastic Petri nets with advanced scheduling disciplines and routing strategies.

The LDES solver supports the widest range of scheduling disciplines (FCFS, LCFS variants, PS variants, priority scheduling) and advanced features like fork-join nodes, class switching, and Petri net modeling.

**Typical Usage:**

.. code-block:: python

   from line_solver.solvers import LDES

   # Basic simulation
   solver = LDES(model, seed=23000, samples=100000)
   avg_table = solver.avg_table()

   # Transient analysis
   solver_transient = LDES(model, timespan=[0, 50], seed=12345)
   tran_avg = solver_transient.get_tran_avg()

**Supported Features:**

Node Types:
   - **Source/Sink**: External arrival and departure points
   - **Queue**: Service stations with various scheduling disciplines
   - **Delay**: Infinite-server stations (think time)
   - **Fork/Join**: Parallel processing with synchronization (including quorum)
   - **Router**: Multiple routing strategies
   - **ClassSwitch**: Dynamic job class transformation
   - **Logger/LogTunnel**: Job passage logging for trace collection
   - **Place/Transition**: Stochastic Petri net modeling

Scheduling Strategies:
   - **FCFS**: First-Come-First-Served
   - **LCFS/LCFSPR/LCFSPI**: Last-Come-First-Served variants (non-preemptive, preemptive-resume, preemptive-independent)
   - **PS/DPS/GPS**: Processor Sharing variants (standard, discriminatory, generalized)
   - **HOL**: Head-of-Line priority scheduling
   - **SIRO**: Service In Random Order
   - **SJF/LJF**: Shortest/Longest Job First
   - **SEPT/LEPT**: Shortest/Longest Expected Processing Time
   - Priority variants: FCFSPRIO, LCFSPRIO, LCFSPRPRIO, LCFSPIPRIO, PSPRIO, DPSPRIO, GPSPRIO

Routing Strategies:
   - **PROB**: Probabilistic routing based on routing matrix
   - **RAND**: Uniform random selection among destinations
   - **RROBIN**: Round-robin cycling through destinations
   - **WRROBIN**: Weighted round-robin
   - **KCHOICES**: Power-of-K-choices (select shortest queue among K random samples)

Service Time Distributions:
   - **Exp**: Exponential distribution
   - **Erlang**: Erlang distribution (sum of exponentials)
   - **HyperExp**: Hyperexponential (mixture of exponentials)
   - **PH/APH**: Phase-type distributions (general and acyclic)
   - **Coxian**: Coxian distribution
   - **Immediate**: Zero service time (instantaneous)
   - **Disabled**: No service (jobs bypass station)
   - **Replayer**: Trace-driven service times from file

Job Class Types:
   - **OpenClass**: Jobs arrive from external source, depart to sink
   - **ClosedClass**: Fixed population circulating in the network
   - **SelfLoopingClass**: Jobs remain at reference station

Advanced Features:
   - **Load Dependence**: Service rates varying with queue population
   - **Finite Capacity**: Buffer limits with blocking
   - **Multiserver**: Multiple parallel servers at a station
   - **Class Priorities**: Priority-based scheduling across classes

Petri Net Support:
   - **Place nodes**: Token storage with capacity constraints
   - **Transition nodes**: Token firing with enabling/inhibiting conditions
   - **Timed transitions**: Exponential or phase-type firing delays
   - **Immediate transitions**: Zero-delay firing with priority and weight

**Solver-Specific Methods:**

- ``avg()`` / ``avg_table()``: Steady-state average metrics
- ``get_tran_avg()``: Transient performance metrics (queue length, utilization, throughput over time)

**Solution Methods** (``method`` option):

- ``'default'`` or ``'ssj'``: SSJ-based discrete event simulation (default)

**Key Options:**

- ``seed`` (int): Random number generator seed for reproducibility
- ``samples`` (int): Simulation length in time units (default: 10000)
- ``timespan`` (list): Time range for transient analysis (e.g., [0, 100])
- ``verbose`` (int): Verbosity level (0=silent, 1=standard, 2=debug with trace output)

**Example:**

.. code-block:: python

   from line_solver import Network, Source, Queue, Sink, OpenClass, Exp, SchedStrategy
   from line_solver.solvers import LDES

   # Create M/M/1 queue
   model = Network('MM1')
   source = Source(model, 'Source')
   queue = Queue(model, 'Queue', SchedStrategy.FCFS)
   sink = Sink(model, 'Sink')

   job_class = OpenClass(model, 'Jobs')
   source.set_arrival(job_class, Exp(1.0))
   queue.set_service(job_class, Exp(2.0))

   model.link(model.serial_routing(source, queue, sink))

   # Steady-state simulation
   solver = LDES(model, seed=12345, samples=100000)
   avg_table = solver.avg_table()
   print(avg_table)

   # Transient analysis
   solver_tran = LDES(model, seed=12345, timespan=[0, 50])
   tran_result = solver_tran.get_tran_avg()

**Fork-Join Example:**

.. code-block:: python

   from line_solver import Network, Source, Queue, Sink, Fork, Join, OpenClass, Exp, SchedStrategy
   from line_solver.solvers import LDES

   model = Network('ForkJoin')
   source = Source(model, 'Source')
   fork = Fork(model, 'Fork')
   queue1 = Queue(model, 'Queue1', SchedStrategy.FCFS)
   queue2 = Queue(model, 'Queue2', SchedStrategy.FCFS)
   join = Join(model, 'Join', fork)
   sink = Sink(model, 'Sink')

   job_class = OpenClass(model, 'Jobs')
   source.set_arrival(job_class, Exp(0.5))
   queue1.set_service(job_class, Exp(1.0))
   queue2.set_service(job_class, Exp(1.5))

   # Fork to parallel queues, join before sink
   P = [[0, 1, 0, 0, 0, 0],   # Source -> Fork
        [0, 0, 1, 1, 0, 0],   # Fork -> Queue1, Queue2
        [0, 0, 0, 0, 1, 0],   # Queue1 -> Join
        [0, 0, 0, 0, 1, 0],   # Queue2 -> Join
        [0, 0, 0, 0, 0, 1],   # Join -> Sink
        [0, 0, 0, 0, 0, 0]]   # Sink
   model.link(P)

   solver = LDES(model, seed=23000, samples=50000)
   avg_table = solver.avg_table()

**Petri Net Example:**

.. code-block:: python

   from line_solver import Network, Place, Transition, ClosedClass, Exp
   from line_solver.solvers import LDES

   model = Network('PetriNet')
   place1 = Place(model, 'P1')
   place2 = Place(model, 'P2')
   trans1 = Transition(model, 'T1')
   trans2 = Transition(model, 'T2')

   job_class = ClosedClass(model, 'Tokens', 10, place1)

   place1.set_number_of_servers(float('inf'))
   place2.set_number_of_servers(float('inf'))

   trans1.set_firing_distribution(job_class, Exp(1.0))
   trans2.set_firing_distribution(job_class, Exp(0.5))
   trans1.set_enabling_conditions(job_class, place1, 1)
   trans2.set_enabling_conditions(job_class, place2, 1)
   trans1.add_firing_outcome(job_class, place1, -1)
   trans1.add_firing_outcome(job_class, place2, 1)
   trans2.add_firing_outcome(job_class, place2, -1)
   trans2.add_firing_outcome(job_class, place1, 1)

   P = [[0, 1, 0, 0],
        [0, 0, 1, 0],
        [0, 0, 0, 1],
        [1, 0, 0, 0]]
   model.link(P)

   solver = LDES(model, seed=23000, samples=100000)
   avg_table = solver.avg_table()

Solver Compatibility Matrix
============================

This table summarizes which methods are supported by each solver.

.. list-table:: Solver Method Support
   :header-rows: 1
   :widths: 24 8 8 8 8 8 8 8 8 8

   * - Method
     - AUTO
     - CTMC
     - LDES
     - FLD
     - JMT
     - MAM
     - MVA
     - NC
     - SSA
   * - avg() / avg_table()
     - ✓
     - ✓
     - ✓
     - ✓
     - ✓
     - ✓
     - ✓
     - ✓
     - ✓
   * - get_cdf_respt()
     - ✓
     - ✓
     - ✗
     - ✓
     - ✓
     - ✓
     - ✗
     - ✗
     - ✓
   * - get_prob_aggr()
     - ✓
     - ✓
     - ✗
     - ✓
     - ✓
     - ✓
     - ✓
     - ✓
     - ✗
   * - get_prob()
     - ✓
     - ✓
     - ✗
     - ✗
     - ✗
     - ✗
     - ✗
     - ✗
     - ✓
   * - get_prob_sys_aggr()
     - ✓
     - ✓
     - ✗
     - ✗
     - ✓
     - ✗
     - ✗
     - ✓
     - ✗
   * - get_tran_avg()
     - ✓
     - ✓
     - ✓
     - ✓
     - ✓
     - ✗
     - ✗
     - ✗
     - ✗
   * - sample() / sample_sys()
     - ✓
     - ✗
     - ✗
     - ✗
     - ✗
     - ✗
     - ✗
     - ✗
     - ✓
   * - sample_aggr()
     - ✓
     - ✗
     - ✗
     - ✗
     - ✓
     - ✗
     - ✗
     - ✗
     - ✓

See Also
========

- :doc:`choosing_solver` - Guide to choosing the right solver for your model
- :doc:`quickstart` - Getting started with LINE solvers
- :doc:`examples` - Example models using different solvers
- :doc:`api/index` - Complete API reference
