"""
Native Python implementation of Mean Value Analysis (MVA) solver.

This implementation uses pure Python/NumPy algorithms from the api.pfqn
module, providing the same functionality as the JPype wrapper without .
"""

import numpy as np
import pandas as pd
import sys
import time
from typing import Optional, Dict, Any, List, Tuple
from dataclasses import dataclass
from enum import Enum

from ...api.sn.transforms import sn_get_residt_from_respt, get_chain_for_class
from ...api.sn.network_struct import NodeType
from ...api.io.logging import line_debug
from ..base import NetworkSolver
from ...indexed_table import IndexedTable


class OptionsDict(dict):
    """A dict that supports attribute-style access."""
    def __getattr__(self, name):
        try:
            return self[name]
        except KeyError:
            raise AttributeError(f"'OptionsDict' object has no attribute '{name}'")

    def __setattr__(self, name, value):
        self[name] = value

    def __delattr__(self, name):
        try:
            del self[name]
        except KeyError:
            raise AttributeError(f"'OptionsDict' object has no attribute '{name}'")


@dataclass
class SolverMVAOptions:
    """Options for the native MVA solver."""
    method: str = 'exact'  # Use 'exact' for accurate results (matches CTMC)
    max_iter: int = 1000
    tol: float = 1e-8
    verbose: bool = False
    seed: Optional[int] = None  # Random seed (for compatibility, not used in MVA)
    keep: bool = False  # Keep intermediate data (for compatibility)
    cutoff: Optional[int] = None  # State space cutoff (for compatibility, not used in MVA)
    samples: Optional[int] = None  # Samples (for compatibility, not used in MVA)
    fork_join: str = 'default'  # Fork-join method: 'default'/'ht' (H-T), 'mmt' (experimental)
    config: Optional[dict] = None  # Config dict, e.g. {'multiserver': 'softmin'}


class SolverMVA(NetworkSolver):
    """
    Native Python Mean Value Analysis (MVA) solver.

    This solver implements MVA algorithms using pure Python/NumPy,
    providing the same functionality as the Java wrapper without
    requiring the JVM.

    Supported methods:
        - 'exact': Exact MVA (pfqn_mva)
        - 'mva': Same as exact
        - 'amva': Approximate MVA using Schweitzer approximation
        - 'qna': Queueing Network Analyzer (for open networks)
        - 'aba.lower', 'aba.upper': Asymptotic Bound Analysis
        - 'pb.lower', 'pb.upper': Performance bounds

    Args:
        model: Network model (Python wrapper or native structure)
        method: Solution method (default: 'default', which auto-selects exact/amva based on model)
        **kwargs: Additional solver options
    """

    def __init__(self, model, method_or_options=None, **kwargs):
        self.model = model
        self._result = None

        # Handle options passed as second argument (MATLAB-style)
        if method_or_options is None:
            # Check if 'method' was passed as a keyword argument
            # Default to 'default' to enable auto-selection of amva for non-product-form models
            # (matches MATLAB behavior where SolverMVA auto-selects method)
            self.method = kwargs.get('method', 'default')
        elif isinstance(method_or_options, str):
            self.method = method_or_options.lower()
        elif hasattr(method_or_options, 'get'):
            # Dict-like options object
            self.method = method_or_options.get('method', 'default')
            if hasattr(method_or_options, 'verbose'):
                kwargs.setdefault('verbose', method_or_options.verbose)
            if hasattr(method_or_options, 'max_iter'):
                kwargs.setdefault('max_iter', method_or_options.max_iter)
            if hasattr(method_or_options, 'seed'):
                kwargs.setdefault('seed', method_or_options.seed)
        elif hasattr(method_or_options, 'method'):
            # SolverOptions-like object (e.g., from SolverLN)
            self.method = getattr(method_or_options, 'method', 'default')
            if hasattr(method_or_options, 'verbose'):
                kwargs.setdefault('verbose', method_or_options.verbose)
            if hasattr(method_or_options, 'max_iter'):
                kwargs.setdefault('max_iter', method_or_options.max_iter)
            if hasattr(method_or_options, 'iter_max'):  # LN uses iter_max
                kwargs.setdefault('max_iter', method_or_options.iter_max)
            if hasattr(method_or_options, 'iter_tol'):  # LN uses iter_tol
                kwargs.setdefault('tol', method_or_options.iter_tol)
            if hasattr(method_or_options, 'seed'):
                kwargs.setdefault('seed', method_or_options.seed)
        else:
            self.method = 'default'

        # Remove 'method' from kwargs if present to avoid duplicate argument
        kwargs.pop('method', None)
        self.options = SolverMVAOptions(method=self.method, **kwargs)

        # Extract network structure
        self._extract_network_params()

    def getName(self) -> str:
        """Get the name of this solver."""
        return "MVA"

    get_name = getName

    def reset(self):
        """Reset the solver to force recomputation on next getAvg call."""
        self._result = None
        self._sn = None
        # Re-extract network parameters since the model may have changed
        self._extract_network_params()

    def _extract_network_params(self):
        """Extract parameters from the model for MVA computation."""
        model = self.model

        # Priority 1: Native model with _sn attribute
        if hasattr(model, '_sn') and model._sn is not None:
            self._from_network_struct(model._sn)
            return

        # Priority 2: Native model with refresh_struct()
        if hasattr(model, 'refresh_struct'):
            model.refresh_struct()
            if hasattr(model, '_sn') and model._sn is not None:
                self._from_network_struct(model._sn)
                return

        # Priority 3: JPype wrapper with getStruct()
        if hasattr(model, 'getStruct'):
            try:
                sn = model.getStruct()
                self._from_network_struct(sn)
                return
            except Exception:
                pass

        # Priority 4: JPype wrapper with obj attribute
        if hasattr(model, 'obj'):
            try:
                sn = model.getStruct()
                self._from_network_struct(sn)
                return
            except Exception:
                pass

        # Priority 5: Direct model extraction
        self._from_model_direct(model)

    def _from_network_struct(self, sn):
        """Extract parameters from NetworkStruct."""
        self._sn = sn  # Save reference for chain information
        self.nstations = int(sn.nstations)
        self.nclasses = int(sn.nclasses)

        # Rates matrix (service rates)
        self.rates = np.asarray(sn.rates, dtype=np.float64)

        # Service demands (D = visits * 1/mu if rate > 0)
        # For MVA, demand includes visit ratio: D[i,r] = V[i,r] / mu[i,r]
        self.demands = np.zeros_like(self.rates)
        nonzero = self.rates > 0
        self.demands[nonzero] = 1.0 / self.rates[nonzero]

        # Apply visits to demands (D = V / mu)
        if hasattr(sn, 'visits') and sn.visits is not None:
            # Build combined visit matrix from per-chain visits
            # Note: sn.visits is indexed by stateful node, but demands is indexed by station
            # Use stationToStateful to convert from stateful visits to station visits
            visits_combined = np.zeros_like(self.demands)
            stationToStateful = np.asarray(sn.stationToStateful, dtype=int).flatten()
            for chain_id, visits_chain in sn.visits.items():
                if isinstance(visits_chain, np.ndarray):
                    # Convert from stateful visits to station visits
                    for station_idx in range(self.nstations):
                        stateful_idx = stationToStateful[station_idx]
                        if stateful_idx < visits_chain.shape[0]:
                            visits_combined[station_idx, :] += visits_chain[stateful_idx, :]
            # Apply visits: D = V / mu = V * (1/mu)
            self.demands = self.demands * visits_combined

        # Population vector
        self.njobs = np.asarray(sn.njobs, dtype=np.float64).flatten()

        # Number of servers
        self.nservers = np.asarray(sn.nservers, dtype=np.float64).flatten()

        # Reference station (for think times)
        self.refstat = np.asarray(sn.refstat, dtype=int).flatten()

        # Station to node mapping
        self.stationToNode = np.asarray(sn.stationToNode, dtype=int).flatten() \
                            if hasattr(sn, 'stationToNode') else np.arange(self.nstations)

        # Node types (to identify delays/think times)
        self.nodetype = sn.nodetype if hasattr(sn, 'nodetype') else None

        # Station types - map from station index to node type
        self.station_types = []
        if self.nodetype is not None:
            for i in range(self.nstations):
                node_idx = self.stationToNode[i]
                if node_idx < len(self.nodetype):
                    self.station_types.append(self.nodetype[node_idx])
                else:
                    self.station_types.append(None)

        # Names - use station node names, not all node names
        nodenames = list(sn.nodenames) if hasattr(sn, 'nodenames') else []
        if nodenames and len(self.stationToNode) > 0:
            self.station_names = [nodenames[self.stationToNode[i]] if self.stationToNode[i] < len(nodenames)
                                  else f'Station{i}' for i in range(self.nstations)]
        else:
            self.station_names = [f'Station{i}' for i in range(self.nstations)]

        self.class_names = list(sn.classnames) if hasattr(sn, 'classnames') else \
                          [f'Class{i}' for i in range(self.nclasses)]

        # Scheduling strategies
        self.sched = sn.sched if hasattr(sn, 'sched') else None

        # Scheduling parameters (weights for DPS/GPS)
        self.schedparam = np.asarray(sn.schedparam) if hasattr(sn, 'schedparam') and sn.schedparam is not None else None

        # Visits (routing)
        self.visits = sn.visits if hasattr(sn, 'visits') else None

        # Load-dependent scaling
        self.lldscaling = getattr(sn, 'lldscaling', None)

        # Determine network type
        self._determine_network_type()

    def _from_model_direct(self, model):
        """Extract parameters directly from model."""
        # Support both native (snake_case) and wrapper (PascalCase) APIs
        if hasattr(model, 'get_number_of_stations'):
            self.nstations = model.get_number_of_stations()
            self.nclasses = model.get_number_of_classes()
        else:
            self.nstations = model.getNumberOfStations()
            self.nclasses = model.getNumberOfClasses()

        # Initialize arrays
        self.rates = np.zeros((self.nstations, self.nclasses))
        self.demands = np.zeros((self.nstations, self.nclasses))
        self.njobs = np.zeros(self.nclasses)
        self.nservers = np.ones(self.nstations)
        self.refstat = np.zeros(self.nclasses, dtype=int)

        self.station_names = []
        self.class_names = []

        # Get class names and populations
        if hasattr(model, 'get_classes'):
            classes = list(model.get_classes())
        else:
            classes = list(model.getClasses())

        for c, cobj in enumerate(classes):
            if c < self.nclasses:
                # Get class name
                if hasattr(cobj, 'get_name'):
                    name = cobj.get_name()
                elif hasattr(cobj, 'getName'):
                    name = cobj.getName()
                else:
                    name = getattr(cobj, 'name', f'Class{c}')
                self.class_names.append(str(name))

                # Get number of jobs (for closed classes)
                if hasattr(cobj, 'get_number_of_jobs'):
                    self.njobs[c] = cobj.get_number_of_jobs()
                elif hasattr(cobj, 'getNumberOfJobs'):
                    self.njobs[c] = cobj.getNumberOfJobs()
                else:
                    self.njobs[c] = 0

        # Get station info
        if hasattr(model, 'get_stations'):
            nodes = model.get_stations()
        else:
            nodes = list(model.getNodes())

        station_idx = 0
        for node in nodes:
            node_type = str(type(node).__name__)
            # Filter to stations only (Queue, Delay, Router, ClassSwitch, Fork, Join)
            if any(t in node_type for t in ['Queue', 'Delay', 'Router', 'ClassSwitch', 'Fork', 'Join']):
                # Get station name
                if hasattr(node, 'get_name'):
                    sname = node.get_name()
                elif hasattr(node, 'getName'):
                    sname = node.getName()
                else:
                    sname = getattr(node, 'name', f'Station{station_idx}')
                self.station_names.append(str(sname))

                # Get service rates for each class
                for c in range(self.nclasses):
                    try:
                        if hasattr(node, 'get_service'):
                            service = node.get_service(classes[c])
                        else:
                            service = node.getServiceProcess(classes[c])

                        if service is not None and hasattr(service, 'getMean'):
                            mean_val = service.getMean()
                            if mean_val > 0:
                                self.rates[station_idx, c] = 1.0 / mean_val
                                self.demands[station_idx, c] = mean_val
                    except:
                        pass

                # Get number of servers
                if hasattr(node, 'number_of_servers'):
                    self.nservers[station_idx] = node.number_of_servers
                elif hasattr(node, 'getNumberOfServers'):
                    self.nservers[station_idx] = node.getNumberOfServers()

                station_idx += 1

        self._determine_network_type()

    def _determine_network_type(self):
        """Determine if network is open, closed, or mixed."""
        has_open = False
        has_closed = False

        for c in range(self.nclasses):
            # Open classes have njobs = inf, closed classes have finite njobs >= 0
            if np.isinf(self.njobs[c]):
                has_open = True
            else:
                has_closed = True

        if has_open and has_closed:
            self.network_type = 'mixed'
        elif has_closed:
            self.network_type = 'closed'
        else:
            self.network_type = 'open'

    def _get_think_times(self) -> np.ndarray:
        """Extract think times from delay stations and INF-scheduled stations.

        Think time Z[r] is the sum of demands at all Delay stations and
        stations with SchedStrategy.INF for class r.

        Matches MATLAB solver_mva.m lines 51-66 which separates:
        - infSET: stations with SchedStrategy.INF (treated as delays)
        - qSET: other product-form stations (treated as queues)
        """
        from ...api.sn.network_struct import NodeType
        from ...lang.base import SchedStrategy

        Z = np.zeros(self.nclasses)

        # Sum demands from all Delay stations AND INF-scheduled stations
        for i in range(self.nstations):
            is_delay_or_inf = False

            # Check if station type is DELAY
            if self.station_types and i < len(self.station_types):
                st = self.station_types[i]
                if st is not None:
                    st_val = st.value if hasattr(st, 'value') else int(st)
                    if st_val == NodeType.DELAY.value:
                        is_delay_or_inf = True

            # Also check if scheduling strategy is INF (infinite servers)
            # MATLAB: case SchedStrategy.INF -> infSET (treated as delay)
            if not is_delay_or_inf and self.sched is not None:
                sched_val = self.sched.get(i) if isinstance(self.sched, dict) else (
                    self.sched[i] if i < len(self.sched) else None)
                if sched_val is not None:
                    is_inf = (sched_val == SchedStrategy.INF or
                             (hasattr(sched_val, 'value') and sched_val.value == SchedStrategy.INF.value) or
                             (isinstance(sched_val, int) and sched_val == SchedStrategy.INF.value))
                    if is_inf:
                        is_delay_or_inf = True

            if is_delay_or_inf:
                for c in range(self.nclasses):
                    if self.demands[i, c] > 0:
                        Z[c] += self.demands[i, c]

        return Z

    def _get_queueing_demands(self) -> Tuple[np.ndarray, List[int]]:
        """
        Get demand matrix for queueing stations only (excluding sources, sinks, delays, and INF).

        Delay stations and INF-scheduled stations contribute to think times,
        not queueing demands.

        Matches MATLAB solver_mva.m lines 51-66 which separates:
        - infSET: stations with SchedStrategy.INF (treated as delays)
        - qSET: other product-form stations (treated as queues)

        Returns:
            Tuple of (demand matrix, list of queueing station indices)
        """
        from ...api.sn.network_struct import NodeType
        from ...lang.base import SchedStrategy

        # Get queueing stations (exclude Source, Sink, Delay, Fork, Join, and INF)
        queue_indices = []
        for i in range(self.nstations):
            # Check if this station should be excluded from queueing demands
            is_excluded = False

            # Check node type
            if self.station_types and i < len(self.station_types):
                st = self.station_types[i]
                if st is not None:
                    # Handle both enum objects and integer values
                    st_val = st.value if hasattr(st, 'value') else int(st)
                    # Exclude Source, Sink, Delay, Fork, and Join stations
                    # (Fork/Join don't do queueing work, they handle synchronization)
                    if st_val in (NodeType.SOURCE.value, NodeType.SINK.value, NodeType.DELAY.value,
                                  NodeType.FORK.value, NodeType.JOIN.value):
                        is_excluded = True

            # Also exclude stations with SchedStrategy.INF (infinite servers)
            # MATLAB: case SchedStrategy.INF -> infSET (NOT in qSET)
            if not is_excluded and self.sched is not None:
                sched_val = self.sched.get(i) if isinstance(self.sched, dict) else (
                    self.sched[i] if i < len(self.sched) else None)
                if sched_val is not None:
                    is_inf = (sched_val == SchedStrategy.INF or
                             (hasattr(sched_val, 'value') and sched_val.value == SchedStrategy.INF.value) or
                             (isinstance(sched_val, int) and sched_val == SchedStrategy.INF.value))
                    if is_inf:
                        is_excluded = True

            # Include station if it has non-zero demand and is not excluded
            if not is_excluded and np.any(self.demands[i, :] > 0):
                queue_indices.append(i)

        if not queue_indices:
            # No queueing stations - return empty demands and empty indices
            # This happens when all stations are delays or INF (infinite server)
            return np.zeros((0, self.nclasses)), []

        L = self.demands[queue_indices, :].copy()

        # Apply DPS weight scaling: for DPS stations, effective demand = D / weight
        from ...lang.base import SchedStrategy
        for idx, i in enumerate(queue_indices):
            if self.sched is not None and i in self.sched:
                sched_val = self.sched[i]
                # Check if this is a DPS station
                is_dps = (sched_val == SchedStrategy.DPS or
                         (hasattr(sched_val, 'value') and sched_val.value == SchedStrategy.DPS) or
                         (isinstance(sched_val, int) and sched_val == 5))  # DPS enum value
                if is_dps and self.schedparam is not None:
                    for k in range(self.nclasses):
                        if i < self.schedparam.shape[0] and k < self.schedparam.shape[1]:
                            w_k = self.schedparam[i, k]
                            if w_k > 0:
                                # Scale demand by weight (higher weight = faster service = lower effective demand)
                                L[idx, k] = L[idx, k] / w_k

        return L, queue_indices

    def _get_source_stations(self) -> List[int]:
        """Get list of source station indices."""
        from ...api.sn.network_struct import NodeType

        source_indices = []
        for i in range(self.nstations):
            if self.station_types and i < len(self.station_types):
                st = self.station_types[i]
                if st is not None:
                    st_val = st.value if hasattr(st, 'value') else int(st)
                    if st_val == NodeType.SOURCE:
                        source_indices.append(i)
        return source_indices

    def _get_delay_stations(self) -> List[int]:
        """Get list of delay (infinite server) station indices."""
        from ...api.sn.network_struct import NodeType

        delay_indices = []
        for i in range(self.nstations):
            if self.station_types and i < len(self.station_types):
                st = self.station_types[i]
                if st is not None:
                    st_val = st.value if hasattr(st, 'value') else int(st)
                    if st_val == NodeType.DELAY:
                        delay_indices.append(i)
        return delay_indices

    def _has_fork_join(self) -> bool:
        """Check if the model contains fork-join nodes."""
        if hasattr(self.model, 'has_fork') and callable(self.model.has_fork):
            return self.model.has_fork()
        # Fallback: check node types
        if self._sn is not None:
            from ...api.sn.network_struct import NodeType
            for nt in self._sn.nodetype:
                nt_val = nt.value if hasattr(nt, 'value') else int(nt)
                if nt_val == NodeType.FORK:
                    return True
        return False

    def _compute_cache_hit_miss_probs(self, XN: np.ndarray) -> None:
        """
        Compute and store cache hit/miss probabilities after MVA analysis.

        This method computes hit probabilities using cache analysis (cache_mva)
        based on the cache's gamma matrix and capacity. The probabilities are
        stored in both the Cache node (via set_result_hit_prob) and in the
        nodeparam structure for use by getAvgNode.

        Args:
            XN: System throughput per class

        References:
            MATLAB: solver_mva/@SolverMVA/runAnalyzer.m lines 150-167
        """
        from ...api.sn.network_struct import NodeType
        from ...api.cache import cache_xi_fp

        if self._sn is None or self._sn.nodetype is None:
            return

        # Find Cache nodes
        cache_indices = []
        for ind in range(self._sn.nnodes):
            if ind < len(self._sn.nodetype):
                node_type = self._sn.nodetype[ind]
                if node_type == NodeType.CACHE:
                    cache_indices.append(ind)

        if not cache_indices:
            return

        # Initialize nodeparam if needed
        if self._sn.nodeparam is None:
            self._sn.nodeparam = {}

        # Process each cache node
        model_nodes = self.model.get_nodes() if hasattr(self.model, 'get_nodes') else []
        model_classes = self.model.get_classes() if hasattr(self.model, 'get_classes') else []

        for ind in cache_indices:
            # Get the Cache node from the model
            cache_node = None
            if ind < len(model_nodes):
                cache_node = model_nodes[ind]

            # Get hit/miss class mappings from cache node
            hitclass = []
            missclass = []

            if cache_node is not None:
                # Get hit/miss class indices for all classes
                for k in range(self.nclasses):
                    job_class = model_classes[k] if k < len(model_classes) else None
                    if job_class is not None:
                        h = cache_node.get_hit_class(job_class)
                        m = cache_node.get_miss_class(job_class)
                        if h is not None:
                            h_idx = model_classes.index(h) if h in model_classes else -1
                            hitclass.append(h_idx)
                        else:
                            hitclass.append(-1)
                        if m is not None:
                            m_idx = model_classes.index(m) if m in model_classes else -1
                            missclass.append(m_idx)
                        else:
                            missclass.append(-1)
                    else:
                        hitclass.append(-1)
                        missclass.append(-1)

            hitclass = np.array(hitclass)
            missclass = np.array(missclass)

            # Compute hit probability using cache analysis
            hitprob = np.zeros(len(hitclass))
            missprob = np.zeros(len(missclass))

            if cache_node is not None and hasattr(cache_node, 'get_gamma_matrix'):
                try:
                    # Get gamma matrix and cache parameters
                    gamma = cache_node.get_gamma_matrix(self.nclasses)
                    # Use item_level_cap which contains the capacity per cache level
                    m_levels = cache_node._item_level_cap if hasattr(cache_node, '_item_level_cap') else np.array([1])

                    # Run cache FPI to get miss probabilities (works for large caches)
                    xi, pi0, pij, it = cache_xi_fp(gamma, m_levels)

                    # Total hit probability = 1 - total miss probability
                    # For each item i, miss prob is pi0[i], hit prob is 1 - pi0[i]
                    # The overall hit rate is weighted by access probabilities
                    access_probs = np.sum(gamma, axis=1)
                    access_probs = access_probs / np.sum(access_probs)  # Normalize

                    # Overall hit rate
                    overall_hit_rate = np.sum(access_probs * (1 - pi0))

                    # For each requesting class that has hit/miss classes, set the probability
                    for k in range(len(hitclass)):
                        h = hitclass[k]
                        m = missclass[k]
                        if h >= 0 and m >= 0:
                            hitprob[k] = overall_hit_rate
                            missprob[k] = 1 - overall_hit_rate

                except Exception as e:
                    # If cache analysis fails, fall back to default
                    pass

            # Store probabilities in cache node
            if cache_node is not None and hasattr(cache_node, 'set_result_hit_prob'):
                cache_node.set_result_hit_prob(hitprob)
                cache_node.set_result_miss_prob(missprob)

            # Store in nodeparam for getAvgNode helper functions
            class NodeParam:
                pass
            node_param = NodeParam()
            node_param.hitclass = hitclass
            node_param.missclass = missclass
            node_param.actualhitprob = hitprob
            node_param.actualmissprob = missprob
            self._sn.nodeparam[ind] = node_param

    def _run_fork_join_analysis(self):
        """
        Run MVA analysis for fork-join networks.

        Uses the method specified in options.fork_join:
        - 'default' or 'mmt': MMT method (matches MATLAB default)
        - 'ht': Heidelberger-Trivedi method

        For closed fork-join networks:
        1. Transforms model to create auxiliary classes
        2. Solves transformed model with standard MVA
        3. Iteratively updates sync delays until convergence
        4. Merges auxiliary class results back into original classes

        For open fork-join networks:
        Returns None to fall back to standard analysis which then gets post-processed.
        """
        from ...api.sn.network_struct import NodeType
        from itertools import combinations

        sn = self._sn
        if sn is None:
            return None

        # MATLAB runAnalyzer.m line 68: checks self.model.hasFork, NOT sn.fj.
        # Fork-without-Join models (e.g., fj_nojoin) still need MMT transformation
        # to correctly compute throughputs with fork fanout.
        has_fork = any(int(nt) == int(NodeType.FORK) for nt in sn.nodetype)
        if not has_fork:
            return None

        # Get fork-join method from options
        # Default is 'mmt' to match MATLAB's default behavior
        fork_join_method = getattr(self.options, 'fork_join', 'default')
        if fork_join_method in ('default', 'mmt', 'fjt'):
            # Use MMT method (MATLAB default)
            return self._run_mmt_transformation()
        elif fork_join_method in ('ht', 'heidelberger-trivedi'):
            # Use Heidelberger-Trivedi method
            if self.network_type != 'closed':
                # H-T only works for closed networks
                return None
            return self._run_ht_transformation()
        else:
            # Unknown method, fall back to MMT
            return self._run_mmt_transformation()

    def _run_mmt_transformation(self):
        """
        Run MMT transformation for fork-join networks (MATLAB default method).

        For closed fork-join networks, MMT uses the H-T transformation but with
        a different sync delay formula: d0*fanOut - mean(ri) instead of d0*fanOut.

        This provides results consistent with MATLAB's default fork-join handling.

        Returns:
            Result dictionary or None if transformation fails
        """
        from ...io.model_adapter import ModelAdapter
        from ...api.sn.network_struct import NodeType
        from ...distributions.continuous import Exp
        from ...constants import GlobalConstants
        from itertools import combinations

        sn = self._sn
        if sn is None:
            return None

        # Check for Fork nodes (not sn.fj which only maps Fork→Join pairs)
        # Fork-without-Join models still need MMT transformation
        has_fork = any(int(nt) == int(NodeType.FORK) for nt in sn.nodetype)
        if not has_fork:
            return None

        # For closed networks, use H-T transformation with MMT sync delay formula
        if self.network_type == 'closed':
            return self._run_mmt_closed()
        else:
            # For open networks, MMT creates auxiliary open classes
            return self._run_mmt_open()

    def _run_mmt_closed(self):
        """
        Run MMT (Modified Method T) for closed fork-join networks.

        Matches MATLAB runAnalyzer.m lines 60-336 exactly:
        1. While forkLoop:
           a. If iter==1: call mmt() and sortForks()
           b. Else: update arrival rates, refreshRates()
           c. sn = nonfjmodel.getStruct(false)
           d. Check convergence on MERGED QN (from previous iteration)
           e. Run MVA on transformed model
           f. Fork handling: compute TNfork, update TN at join, compute sync delays
           g. Merge: save TN_orig, merge aux into original, restore TN, delete aux cols
        2. Return merged results
        """
        from ...io.model_adapter import ModelAdapter
        from ...api.sn.network_struct import NodeType
        from ...distributions.continuous import Exp
        from ...constants import GlobalConstants
        from ...api.sn.transforms import sn_get_residt_from_respt
        from ...api.sn.getters import sn_get_arvr_from_tput
        from itertools import combinations

        sn = self._sn
        orig_nclasses = sn.nclasses
        orig_nstations = sn.nstations

        # Initialize forkLambda for auxiliary classes (MATLAB line 63)
        num_forks = sum(1 for nt in sn.nodetype if int(nt) == int(NodeType.FORK))
        fork_lambda = GlobalConstants.FineTol * np.ones(2 * orig_nclasses * max(num_forks, 1))

        # Initialize QN and QN_1 for convergence check (MATLAB lines 64-65)
        QN = GlobalConstants.Immediate * np.ones((1, orig_nclasses))
        QN_1 = np.zeros_like(QN)
        UN = np.zeros_like(QN)

        max_iter = getattr(self.options, 'max_iter', 1000)
        tol = GlobalConstants.CoarseTol
        fork_loop = True
        fork_iter = 0
        total_iter = 0
        line_debug("Fork-join method: mmt", options=self.options)

        nonfjmodel = None
        fjclassmap = None
        fjforkmap = None
        fanout = None
        outer_forks = None
        parent_forks = None

        import os
        debug_mmt = os.environ.get('DEBUG_MMT', '0') == '1'

        while fork_loop and fork_iter < max_iter:
            fork_iter += 1
            line_debug("Fork-join iteration %d", fork_iter, options=self.options)

            # MATLAB lines 70-94: First iteration: transform model; else: update arrival rates
            if fork_iter == 1:
                try:
                    mmt_result = ModelAdapter.mmt(self.model, fork_lambda[:orig_nclasses])
                except Exception as e:
                    import warnings
                    warnings.warn(f"MMT transformation failed: {e}")
                    return None

                nonfjmodel = mmt_result.nonfjmodel
                fjclassmap = mmt_result.fjclassmap
                fjforkmap = mmt_result.fjforkmap
                fanout = mmt_result.fanout

                if nonfjmodel is None or len(fjclassmap) == 0:
                    return None

                # Sort forks using ModelAdapter (MATLAB line 76)
                outer_forks, parent_forks = ModelAdapter.sort_forks(sn, fjforkmap, fjclassmap, nonfjmodel)
            else:
                # Update arrival rates for auxiliary classes (MATLAB lines 78-94)
                source = nonfjmodel.get_source()
                if source is not None:
                    for r_aux in range(len(fjclassmap)):
                        s_orig = int(fjclassmap[r_aux])
                        if s_orig >= 0 and r_aux < len(fanout) and fanout[r_aux] > 0:
                            aux_class_idx = orig_nclasses + r_aux
                            if aux_class_idx < len(nonfjmodel._classes):
                                aux_class = nonfjmodel._classes[aux_class_idx]
                                arrival = source.get_arrival(aux_class)
                                if arrival is not None and not getattr(arrival, '_disabled', False):
                                    new_rate = (fanout[r_aux] - 1) * fork_lambda[r_aux]
                                    if new_rate > 0:
                                        source.set_arrival(aux_class, Exp(new_rate))
                # Only refresh rates, not full struct rebuild (MATLAB uses refreshRates())
                nonfjmodel.refresh_rates()

            # MATLAB line 96: sn = nonfjmodel.getStruct(false)
            # Struct already built by mmt()->relink()->link() on iter 1,
            # and only rates change on iter > 1, so just read cached struct.
            nonfjstruct = nonfjmodel._sn
            line_debug("Fork-join iter %d: rebuilt nonfjmodel struct (nstations=%d, nclasses=%d)",
                       fork_iter, nonfjstruct.nstations, nonfjstruct.nclasses, options=self.options)

            # MATLAB lines 97-108: Convergence check on MERGED QN from previous iteration
            with np.errstate(divide='ignore', invalid='ignore'):
                max_change = np.nanmax(np.abs(1 - QN_1 / QN))
            if debug_mmt and fork_iter <= 10:
                print(f"  [conv] iter={fork_iter}, max_change={max_change}, QN={QN.flatten()[:4]}, QN_1={QN_1.flatten()[:4]}", flush=True)
            if (np.isnan(max_change) or max_change < tol) and fork_iter > 2:
                line_debug("Fork-join iter %d: converged (max ratio error < CoarseTol)", fork_iter, options=self.options)
                fork_loop = False
                if debug_mmt:
                    print(f"Converged at iteration {fork_iter}, max_change={max_change:.6e}")
            else:
                QN_1 = QN.copy()

            # MATLAB lines 181-193: Run MVA on transformed model
            nonfj_solver = SolverMVA(nonfjmodel, method='egflin')
            nonfj_solver._skip_fork_join = True

            try:
                nonfj_result = nonfj_solver.getAvg()
                if nonfj_result is None:
                    return None
                QN_new, UN_new, RN, TN, AN, WN = nonfj_result
                XN = nonfj_solver.getAvgSysTput()
            except Exception as e:
                import warnings
                warnings.warn(f"MVA failed: {e}")
                return None

            if debug_mmt and (fork_iter <= 5 or fork_iter % 50 == 0):
                print(f"\n=== MMT Iteration {fork_iter} ===")
                print(f"QN sum orig: {np.sum(QN_new[:, :orig_nclasses]):.4f}")

            # MATLAB lines 194-273: Fork handling - compute sync delays
            line_debug("Fork-join post-processing: method=%s, computing sync delays", "mmt", options=self.options)
            fork_indices = [i for i in range(sn.nnodes) if int(sn.nodetype[i]) == int(NodeType.FORK)]

            # Pre-compute linked routing matrix once (loop-invariant)
            P_precomputed = nonfjmodel.get_linked_routing_matrix()

            for f in fork_indices:
                # MATLAB lines 200-206: Compute TNfork
                TNfork = np.zeros(orig_nclasses)
                for c in range(sn.nchains):
                    inchain = np.where(sn.chains[c, :] > 0)[0]
                    for r in inchain:
                        if parent_forks is not None and f < len(parent_forks):
                            pf = int(parent_forks[f])
                            if hasattr(sn, 'nodevisits') and sn.nodevisits is not None:
                                if c < len(sn.nodevisits) and sn.nodevisits[c] is not None:
                                    nv = sn.nodevisits[c]
                                    if pf < nv.shape[0] and r < nv.shape[1]:
                                        refstat = int(sn.refstat[r]) if r < len(sn.refstat) else 0
                                        visits_sum = 0
                                        if hasattr(sn, 'visits') and sn.visits is not None and c < len(sn.visits):
                                            v = sn.visits[c]
                                            stateful_idx = int(sn.stationToStateful[refstat]) if refstat < len(sn.stationToStateful) else refstat
                                            if v is not None and stateful_idx < v.shape[0]:
                                                visits_sum = np.sum(v[stateful_idx, inchain])
                                        if visits_sum > 0 and refstat < TN.shape[0]:
                                            TNfork[r] = (nv[pf, r] / visits_sum) * np.sum(TN[refstat, inchain])

                # MATLAB lines 208-209: Find join and auxiliary classes for this fork
                join_idx_arr = np.where(sn.fj[f, :] > 0)[0]
                if len(join_idx_arr) == 0:
                    continue
                join_idx = join_idx_arr[0]

                fork_aux_indices = np.where(fjforkmap == f)[0]

                for aux_idx in fork_aux_indices:
                    r = int(fjclassmap[aux_idx])
                    if r < 0:
                        continue

                    aux_class_idx = orig_nclasses + aux_idx

                    # MATLAB lines 212-217: Update forkLambda and TN at join
                    join_station = -1
                    if join_idx < len(nonfjstruct.nodeToStation):
                        join_station = int(nonfjstruct.nodeToStation[join_idx])

                    if join_station >= 0 and join_station < TN.shape[0] and r < TN.shape[1]:
                        # MATLAB line 215: TN(join,r) = TN(join,r) + sum(TN(join, find(fjclassmap==r))) - TN(join,s)
                        aux_indices_for_r = np.where(fjclassmap == r)[0]
                        aux_class_indices_for_r = orig_nclasses + aux_indices_for_r
                        tn_sum = TN[join_station, r] + np.sum(TN[join_station, aux_class_indices_for_r])
                        if aux_class_idx < TN.shape[1]:
                            tn_sum -= TN[join_station, aux_class_idx]
                        TN[join_station, r] = tn_sum
                        # MATLAB line 216: forkLambda(s) = mean([forkLambda(s); TN(join,r)])
                        fork_lambda[aux_idx] = np.mean([fork_lambda[aux_idx], tn_sum])

                    # MATLAB lines 218-220: Check if outer fork for class r
                    if f < outer_forks.shape[0] and r < outer_forks.shape[1]:
                        if outer_forks[f, r] == 0:
                            continue

                    # MATLAB lines 222-225: Find paths using findPathsCS
                    try:
                        P = P_precomputed
                        if P is not None:
                            P_combined = self._get_combined_routing_matrix(P, nonfjstruct, nonfjmodel)
                            to_merge = [r, aux_class_idx]
                            ri = ModelAdapter.find_paths_cs(
                                sn, P_combined, f, join_idx, r,
                                to_merge, QN_new, TN, 0.0,
                                fjclassmap, fjforkmap, nonfjmodel
                            )
                        else:
                            ri = np.array([GlobalConstants.FineTol, GlobalConstants.FineTol])
                    except Exception as e:
                        if debug_mmt:
                            print(f"    find_paths_cs exception: {e}")
                            import traceback
                            traceback.print_exc()
                        ri = np.array([GlobalConstants.FineTol, GlobalConstants.FineTol])

                    ri = np.maximum(ri, GlobalConstants.FineTol)

                    if len(ri) == 0:
                        continue

                    # MATLAB lines 226-232: Compute E[max] using inclusion-exclusion
                    lambdai = 1.0 / ri
                    d0 = 0.0
                    parallel_branches = len(ri)
                    for pow_val in range(parallel_branches):
                        combos = list(combinations(lambdai, pow_val + 1))
                        current_sum = sum(1.0 / sum(combo) for combo in combos)
                        d0 += ((-1) ** pow_val) * current_sum

                    # Get fanout (tasksPerLink) - MATLAB: sn.nodeparam{f}.fanOut
                    f_fanout = self._get_fanout(sn, f)

                    # MATLAB lines 234-235: Set sync delay
                    mean_ri = np.mean(ri)
                    sync_delay = max(d0 * f_fanout - mean_ri, GlobalConstants.FineTol)

                    if debug_mmt and (fork_iter <= 5 or fork_iter % 50 == 0):
                        print(f"  Fork {f}, r={r}: ri={ri}, d0={d0:.6f}, sync_delay={sync_delay:.6f}")

                    # Set sync delay for both auxiliary and original class at join
                    if aux_class_idx < len(nonfjmodel._classes):
                        nonfjmodel._nodes[join_idx].set_service(
                            nonfjmodel._classes[aux_class_idx], Exp.fit_mean(sync_delay)
                        )
                    if r < len(nonfjmodel._classes):
                        nonfjmodel._nodes[join_idx].set_service(
                            nonfjmodel._classes[r], Exp.fit_mean(sync_delay)
                        )
                    # MATLAB line 236: nonfjmodel.refreshRates()
                    nonfjmodel.refresh_rates()

            # MATLAB lines 303-328: Merge auxiliary classes into original classes
            # This happens INSIDE the while loop, so convergence is checked on merged QN
            # Save TN at Join and Source stations (MATLAB line 304)
            join_indices_all = [i for i in range(sn.nnodes) if int(sn.nodetype[i]) == int(NodeType.JOIN)]
            source_indices_all = [i for i in range(sn.nnodes) if int(sn.nodetype[i]) == int(NodeType.SOURCE)]

            preserve_stations = []
            for j_idx in join_indices_all:
                if j_idx < len(nonfjstruct.nodeToStation):
                    preserve_stations.append(int(nonfjstruct.nodeToStation[j_idx]))
            for s_idx in source_indices_all:
                if s_idx < len(nonfjstruct.nodeToStation):
                    preserve_stations.append(int(nonfjstruct.nodeToStation[s_idx]))

            orig_class_set = sorted(set(int(c) for c in fjclassmap if c >= 0))

            TN_orig = {}
            for ist in preserve_stations:
                for oc in orig_class_set:
                    if ist < TN.shape[0] and oc < TN.shape[1]:
                        TN_orig[(ist, oc)] = TN[ist, oc]

            # MATLAB lines 306-319: merge back artificial classes
            for r_aux in range(len(fjclassmap)):
                s_orig = int(fjclassmap[r_aux])
                if s_orig >= 0:
                    aux_col = orig_nclasses + r_aux
                    if aux_col < QN_new.shape[1] and s_orig < QN_new.shape[1]:
                        QN_new[:, s_orig] += QN_new[:, aux_col]
                        UN_new[:, s_orig] += UN_new[:, aux_col]
                        TN[:, s_orig] += TN[:, aux_col]
                        with np.errstate(divide='ignore', invalid='ignore'):
                            RN[:, s_orig] = np.where(TN[:, s_orig] > 0,
                                                      QN_new[:, s_orig] / TN[:, s_orig], 0)

            # MATLAB line 321: Restore TN at Join and Source
            for (ist, oc), val in TN_orig.items():
                if ist < TN.shape[0] and oc < TN.shape[1]:
                    TN[ist, oc] = val

            # Update QN for convergence check: use merged original classes only
            QN = QN_new[:, :orig_nclasses].copy()
            UN = UN_new[:, :orig_nclasses].copy()

            total_iter += nonfj_solver._result.get('iter', 0) if nonfj_solver._result else 0
            if total_iter > 10000:
                break

        # Extract final results: delete auxiliary class columns (MATLAB lines 323-328)
        QN_final = QN_new[:orig_nstations, :orig_nclasses].copy()
        UN_final = UN_new[:orig_nstations, :orig_nclasses].copy()
        RN_final = RN[:orig_nstations, :orig_nclasses].copy()
        TN_final = TN[:orig_nstations, :orig_nclasses].copy()
        XN_final = XN[:orig_nclasses].copy() if len(XN) >= orig_nclasses else XN.copy()

        # Zero out UN at Join stations
        for j_idx in join_indices_all:
            if j_idx < len(nonfjstruct.nodeToStation):
                join_ist = int(nonfjstruct.nodeToStation[j_idx])
                if join_ist < UN_final.shape[0]:
                    UN_final[join_ist, :] = 0

        # Compute arrival rates and residence times (MATLAB lines 341-342)
        AN_final = sn_get_arvr_from_tput(self._sn, TN_final, None)
        WN_final = sn_get_residt_from_respt(self._sn, RN_final, None)

        self._result = {
            'QN': QN_final,
            'UN': UN_final,
            'RN': RN_final,
            'TN': TN_final,
            'AN': AN_final,
            'XN': XN_final,
            'WN': WN_final,
            'CN': np.sum(RN_final, axis=0),
            'runtime': 0.0,
            'method': 'mmt',
            'iter': total_iter
        }

        return self._result

    def _sort_forks(self, sn, fjforkmap, fjclassmap, nonfjmodel):
        """
        Sort forks to identify outer forks and their parents.
        Delegates to ModelAdapter.sort_forks which implements the full
        recursive nested fork detection matching MATLAB's sortForks.m.
        """
        from ...io.model_adapter import ModelAdapter
        return ModelAdapter.sort_forks(sn, fjforkmap, fjclassmap, nonfjmodel)

    @staticmethod
    def _get_fanout(sn, f):
        """Get fanOut (tasksPerLink) for fork node f from sn.nodeparam."""
        f_fanout = 1  # Default: 1 task per link
        if hasattr(sn, 'nodeparam') and sn.nodeparam is not None:
            fp = None
            if isinstance(sn.nodeparam, dict) and f in sn.nodeparam:
                fp = sn.nodeparam[f]
            elif isinstance(sn.nodeparam, list) and f < len(sn.nodeparam):
                fp = sn.nodeparam[f]
            if fp is not None:
                if isinstance(fp, dict) and 'fanOut' in fp:
                    f_fanout = int(fp['fanOut'])
                elif hasattr(fp, 'fanOut'):
                    f_fanout = int(fp.fanOut)
        return f_fanout

    def _get_combined_routing_matrix(self, P, sn, nonfjmodel=None):
        """
        Convert cell-based routing matrix P to combined format for find_paths_cs.

        The combined matrix has dimensions (nclasses * nnodes) x (nclasses * nnodes).
        Entry at (r*nnodes + i, s*nnodes + j) represents routing probability
        from (class r, node i) to (class s, node j).

        ClassSwitch nodes are resolved: their class-switching behavior is folded
        into the routing probabilities so that routing bypasses CS nodes. This
        matches MATLAB's behavior where cell2mat(getLinkedRoutingMatrix) produces
        a combined matrix with CS nodes already resolved.

        Args:
            P: Cell-based routing matrix P[r][s] = nnodes x nnodes matrix
            sn: Network structure (for extracting node count)
            nonfjmodel: Optional transformed model for ClassSwitch resolution

        Returns:
            Combined routing matrix as numpy array
        """
        from ...lang.nodes import ClassSwitch as CSNode

        if P is None:
            return None

        nclasses = len(P)
        nnodes = P[0][0].shape[0] if nclasses > 0 and len(P[0]) > 0 else 0

        # Create combined matrix of size (nclasses*nnodes, nclasses*nnodes)
        P_combined = np.zeros((nclasses * nnodes, nclasses * nnodes))
        for r in range(nclasses):
            for s in range(nclasses):
                if r < len(P) and s < len(P[r]):
                    P_combined[r*nnodes:(r+1)*nnodes, s*nnodes:(s+1)*nnodes] = P[r][s]

        # Resolve ClassSwitch nodes: fold class-switching into routing
        # probabilities so that find_paths_cs can traverse paths correctly.
        # In Python, the linked routing matrix routes (class_r, predecessor) ->
        # (class_r, CS_node) and then (switched_class, CS_node) -> (switched_class,
        # successor). MATLAB resolves this into direct (class_r, predecessor) ->
        # (switched_class, successor) routing.
        if nonfjmodel is None:
            nonfjmodel = self.model
        cs_nodes = []
        if nonfjmodel is not None and hasattr(nonfjmodel, '_nodes'):
            for node_idx, node in enumerate(nonfjmodel._nodes):
                if isinstance(node, CSNode) and node_idx < nnodes:
                    switch_matrix = getattr(node, '_switch_matrix', None)
                    if switch_matrix is not None:
                        cs_nodes.append((node_idx, switch_matrix))

        for cs_idx, switch_matrix in cs_nodes:
            # For each CS node, resolve: incoming * switch * outgoing
            for r_in in range(nclasses):
                # Incoming: P_combined[r_in*n + i, r_in*n + cs] for all predecessor i
                in_col = r_in * nnodes + cs_idx
                incoming = P_combined[:, in_col].copy()
                if np.sum(incoming) == 0:
                    continue

                # Determine switched class using switch matrix
                for r_out in range(nclasses):
                    if r_out >= switch_matrix.shape[0] or r_in >= switch_matrix.shape[1]:
                        continue
                    # switch_matrix[r_in, r_out] = prob of switching from r_in to r_out
                    sw_prob = switch_matrix[r_in, r_out]
                    if sw_prob <= 0:
                        continue

                    # Outgoing: P_combined[r_out*n + cs, r_out*n + j] for all successor j
                    out_row = r_out * nnodes + cs_idx
                    outgoing = P_combined[out_row, :].copy()
                    if np.sum(outgoing) == 0:
                        continue

                    # Add resolved routing: predecessor -> successor with class switch
                    for pred_idx in np.where(incoming > 0)[0]:
                        for succ_idx in np.where(outgoing > 0)[0]:
                            P_combined[pred_idx, succ_idx] += (
                                incoming[pred_idx] * sw_prob * outgoing[succ_idx]
                            )

            # Zero out CS node rows and columns in the combined matrix
            for r in range(nclasses):
                P_combined[r * nnodes + cs_idx, :] = 0
                P_combined[:, r * nnodes + cs_idx] = 0

        return P_combined

    def _merge_mmt_results(self, QN, UN, RN, TN, XN, orig_sn, nonfjstruct, fjclassmap, fjforkmap, orig_nclasses, orig_nstations):
        """
        Merge results from MMT transformed model back to original model structure.
        """
        from ...api.sn.network_struct import NodeType
        from ...api.sn.transforms import sn_get_residt_from_respt

        # Find join station indices
        join_stations = []
        fork_indices = [i for i in range(orig_sn.nnodes) if int(orig_sn.nodetype[i]) == int(NodeType.FORK)]
        for f in fork_indices:
            join_idx_arr = np.where(orig_sn.fj[f, :] > 0)[0]
            if len(join_idx_arr) > 0:
                join_idx = join_idx_arr[0]
                if join_idx < len(nonfjstruct.nodeToStation):
                    join_stations.append(int(nonfjstruct.nodeToStation[join_idx]))

        # Find Source station index
        source_stations = []
        source_indices = [i for i in range(orig_sn.nnodes) if int(orig_sn.nodetype[i]) == int(NodeType.SOURCE)]
        for src_idx in source_indices:
            if src_idx < len(nonfjstruct.nodeToStation):
                source_stations.append(int(nonfjstruct.nodeToStation[src_idx]))

        # Merge auxiliary class metrics into original classes
        QN_merged = QN.copy()
        UN_merged = UN.copy()
        RN_merged = RN.copy()
        TN_merged = TN.copy()

        # Save TN at Join and Source stations before merging (MATLAB lines 304, 321)
        # These throughputs should NOT be summed - restore them after merge
        special_stations = join_stations + source_stations
        orig_class_indices = np.unique([int(fjclassmap[i]) for i in range(len(fjclassmap)) if fjclassmap[i] >= 0])
        TN_orig = {}
        for st in special_stations:
            if st < TN.shape[0]:
                for oc in orig_class_indices:
                    if oc < TN.shape[1]:
                        TN_orig[(st, oc)] = TN[st, oc]

        # Merge auxiliary class metrics into original classes
        # Auxiliary classes are at indices orig_nclasses + aux_idx in the transformed model
        # MATLAB runAnalyzer.m lines 306-316: merge and recompute RN = QN / TN
        for aux_idx in range(len(fjclassmap)):
            orig_idx = fjclassmap[aux_idx]
            aux_class_idx = orig_nclasses + aux_idx  # Actual index of auxiliary class
            if orig_idx >= 0 and aux_class_idx < QN_merged.shape[1] and orig_idx < QN_merged.shape[1]:
                QN_merged[:, orig_idx] += QN_merged[:, aux_class_idx]
                UN_merged[:, orig_idx] += UN_merged[:, aux_class_idx]
                TN_merged[:, orig_idx] += TN_merged[:, aux_class_idx]
                # Recompute RN = QN / TN after merging (MATLAB line 316)
                with np.errstate(divide='ignore', invalid='ignore'):
                    RN_merged[:, orig_idx] = np.where(TN_merged[:, orig_idx] > 0,
                                                       QN_merged[:, orig_idx] / TN_merged[:, orig_idx], 0)

        # Restore TN at Join and Source stations (MATLAB line 321)
        # These throughputs should NOT be summed - restore them after merge
        for st, oc in TN_orig.keys():
            if st < TN_merged.shape[0] and oc < TN_merged.shape[1]:
                TN_merged[st, oc] = TN_orig[(st, oc)]

        # Zero out UN at Join stations
        for js in join_stations:
            if js < UN_merged.shape[0]:
                UN_merged[js, :orig_nclasses] = 0

        # Extract only original classes and original stations
        # Remove auxiliary delay stations if any
        stations_to_keep = list(range(min(orig_nstations, QN_merged.shape[0])))

        QN_final = QN_merged[stations_to_keep, :][:, :orig_nclasses]
        UN_final = UN_merged[stations_to_keep, :][:, :orig_nclasses]
        RN_final = RN_merged[stations_to_keep, :][:, :orig_nclasses]
        TN_final = TN_merged[stations_to_keep, :][:, :orig_nclasses]
        XN_final = XN[:orig_nclasses].copy() if len(XN) >= orig_nclasses else XN.copy()

        # Compute arrival rates using sn_get_arvr_from_tput (MATLAB runAnalyzer.m line 341)
        from ...api.sn.getters import sn_get_arvr_from_tput
        AN_final = sn_get_arvr_from_tput(self._sn, TN_final, None)

        # Compute residence times
        WN_final = sn_get_residt_from_respt(self._sn, RN_final, None)

        # Store results
        self._result = {
            'QN': QN_final,
            'UN': UN_final,
            'RN': RN_final,
            'TN': TN_final,
            'AN': AN_final,
            'XN': XN_final,
            'WN': WN_final,
            'CN': np.sum(RN_final, axis=0),
            'runtime': 0.0,
            'method': 'mmt',
            'iter': iter_count if 'iter_count' in dir() else 0
        }

        return self._result

    def _merge_ht_results(self, QN, UN, RN, TN, XN, orig_sn, nonfjstruct, fjclassmap, fj_auxiliary_delays, orig_nclasses, orig_nstations):
        """
        Merge results from H-T/MMT transformed model back to original model structure.

        This function:
        1. Finds join and auxiliary delay station indices
        2. Zeros out original class metrics at join stations
        3. Removes auxiliary delay station rows
        4. Merges auxiliary class metrics into original classes
        5. Computes final metrics and stores result
        """
        from ...api.sn.network_struct import NodeType
        from ...api.sn.transforms import sn_get_residt_from_respt

        # Find fork indices in original model
        fork_indices = []
        for i, nt in enumerate(orig_sn.nodetype):
            nt_val = nt.value if hasattr(nt, 'value') else int(nt)
            if nt_val == NodeType.FORK.value if hasattr(NodeType.FORK, 'value') else NodeType.FORK:
                fork_indices.append(i)

        # Find join and aux delay station indices
        join_stations = []
        aux_delay_stations = []
        for f in fork_indices:
            join_idx_arr = np.where(orig_sn.fj[f, :] > 0)[0]
            if len(join_idx_arr) > 0:
                join_idx = join_idx_arr[0]
                if join_idx < len(nonfjstruct.nodeToStation):
                    join_stations.append(nonfjstruct.nodeToStation[join_idx])
                aux_delay_idx = fj_auxiliary_delays.get(join_idx, None)
                if aux_delay_idx is not None and aux_delay_idx < len(nonfjstruct.nodeToStation):
                    aux_delay_stations.append(nonfjstruct.nodeToStation[aux_delay_idx])

        # Get original class indices that have auxiliary classes
        orig_classes_with_aux = sorted(set(fjclassmap[fjclassmap >= 0]))

        # Save original class throughputs at join stations before zeroing
        TN_orig_join = {}
        for js in join_stations:
            for oc in orig_classes_with_aux:
                if js < TN.shape[0] and oc < TN.shape[1]:
                    TN_orig_join[(js, oc)] = TN[js, oc]

        # Zero out original class metrics at join stations
        for js in join_stations:
            for oc in orig_classes_with_aux:
                if js < QN.shape[0] and oc < QN.shape[1]:
                    QN[js, oc] = 0
                    RN[js, oc] = 0
                    TN[js, oc] = 0
                    UN[js, oc] = 0

        # Remove auxiliary delay station rows (create mask for stations to keep)
        stations_to_keep = [i for i in range(QN.shape[0]) if i not in aux_delay_stations]

        QN_merged = QN[stations_to_keep, :]
        UN_merged = UN[stations_to_keep, :]
        RN_merged = RN[stations_to_keep, :]
        TN_merged = TN[stations_to_keep, :]

        # Merge auxiliary classes into original classes
        # fjclassmap[class_idx] = -1 for original classes, = orig_class_idx for auxiliary classes
        # Auxiliary classes are stored at their actual indices in the fjclassmap
        for aux_class_idx in range(len(fjclassmap)):
            orig_idx = fjclassmap[aux_class_idx]
            # Only process auxiliary classes (those that map to an original class)
            if orig_idx >= 0 and aux_class_idx < QN_merged.shape[1] and orig_idx < QN_merged.shape[1]:
                QN_merged[:, orig_idx] += QN_merged[:, aux_class_idx]
                UN_merged[:, orig_idx] += UN_merged[:, aux_class_idx]
                # Add all throughputs of the auxiliary classes to facilitate the computation of the response times
                TN_merged[:, orig_idx] += TN_merged[:, aux_class_idx]
                # Recompute RN = QN / TN after merging (matching JAR behavior)
                for i in range(RN_merged.shape[0]):
                    if TN_merged[i, orig_idx] > 0:
                        RN_merged[i, orig_idx] = QN_merged[i, orig_idx] / TN_merged[i, orig_idx]

        # Restore original class throughputs at join stations
        for (old_js, oc), tput in TN_orig_join.items():
            if old_js in stations_to_keep:
                new_js_idx = stations_to_keep.index(old_js)
                if new_js_idx < TN_merged.shape[0] and oc < TN_merged.shape[1]:
                    TN_merged[new_js_idx, oc] = tput

        # Zero out UN at Join stations for all original classes
        # Join has Immediate service in the original model, so utilization must be 0
        for js in join_stations:
            if js in stations_to_keep:
                new_js_idx = stations_to_keep.index(js)
                if new_js_idx < UN_merged.shape[0]:
                    UN_merged[new_js_idx, :orig_nclasses] = 0

        # Extract only original classes
        QN_final = QN_merged[:, :orig_nclasses]
        UN_final = UN_merged[:, :orig_nclasses]
        RN_final = RN_merged[:, :orig_nclasses]
        TN_final = TN_merged[:, :orig_nclasses]
        XN_final = XN[:orig_nclasses].copy()

        # Compute arrival rates using sn_get_arvr_from_tput (MATLAB runAnalyzer.m line 341)
        from ...api.sn.getters import sn_get_arvr_from_tput
        AN_final = sn_get_arvr_from_tput(self._sn, TN_final, None)

        # Compute residence times from response times (WN = RN * V)
        WN_final = sn_get_residt_from_respt(self._sn, RN_final, None)

        # Store results
        self._result = {
            'QN': QN_final,
            'UN': UN_final,
            'RN': RN_final,
            'TN': TN_final,
            'AN': AN_final,
            'XN': XN_final,
            'WN': WN_final,
            'CN': np.sum(RN_final, axis=0),
            'runtime': 0.0,
            'method': 'mmt',
            'iter': 0
        }

        return self._result

    def _run_mmt_open(self):
        """
        Run MMT for open fork-join networks.

        1-to-1 port of MATLAB runAnalyzer.m MMT implementation:
        1. Transform model using ModelAdapter.mmt() to create auxiliary classes
        2. Iterate until convergence:
           - Update auxiliary class arrival rates based on fanout
           - Solve transformed model using standard MVA
           - Compute TNfork and update forkLambda
           - Call findPathsCS to get response times on parallel paths
           - Compute sync delay d0 and update Join service times
        3. Merge auxiliary class results back into original classes

        Returns:
            Result dictionary or None if transformation fails
        """
        from ...io.model_adapter import ModelAdapter
        from ...api.sn.network_struct import NodeType
        from ...api.sn.transforms import sn_get_residt_from_respt
        from ...distributions.continuous import Exp
        from ...constants import GlobalConstants
        from itertools import combinations

        sn = self._sn
        if sn is None:
            return None

        orig_nclasses = sn.nclasses
        orig_nstations = sn.nstations
        orig_sn = sn  # Save original sn

        # Find fork indices
        fork_indices = [i for i in range(sn.nnodes) if int(sn.nodetype[i]) == int(NodeType.FORK)]
        num_forks = len(fork_indices)
        if num_forks == 0:
            return None

        # Initialize forkLambda (MATLAB line 63)
        fork_lambda = GlobalConstants.FineTol * np.ones(2 * orig_nclasses * num_forks)

        # Initialize QN and QN_1 for convergence check (MATLAB lines 64-65)
        QN = GlobalConstants.Immediate * np.ones(orig_nclasses)
        QN_1 = np.zeros(orig_nclasses)
        UN = np.zeros(orig_nclasses)

        fork_loop = True
        fork_iter = 0
        max_iter = 500  # options.iter_max

        nonfjmodel = None
        fjclassmap = None
        fjforkmap = None
        fanout = None
        outer_forks = None
        parent_forks = None
        total_iter = 0

        DEBUG_MMT = False  # TEMP DEBUG (set to True for debugging)
        while fork_loop and fork_iter < max_iter:
            fork_iter += 1
            total_iter = fork_iter

            if DEBUG_MMT and fork_iter <= 3:
                print(f"DEBUG: Iteration {fork_iter}, fork_lambda[:5] = {fork_lambda[:5]}")

            # First iteration: transform model (MATLAB lines 70-77)
            if fork_iter == 1:
                try:
                    mmt_result = ModelAdapter.mmt(self.model, fork_lambda[:orig_nclasses])
                    nonfjmodel = mmt_result.nonfjmodel
                    fjclassmap = mmt_result.fjclassmap
                    fjforkmap = mmt_result.fjforkmap
                    fanout = mmt_result.fanout

                    if len(fjclassmap) == 0:
                        return None

                    # Sort forks (MATLAB line 76)
                    outer_forks, parent_forks = ModelAdapter.sort_forks(
                        sn, fjforkmap, fjclassmap, nonfjmodel
                    )
                except Exception as e:
                    import warnings
                    warnings.warn(f"MMT transformation failed: {e}. Falling back to standard MVA.")
                    return None
            else:
                # Subsequent iterations: update auxiliary class arrival rates (MATLAB lines 78-94)
                try:
                    from ...distributions.continuous import Disabled
                    nonfj_source = nonfjmodel.get_source()
                    nonfj_classes = nonfjmodel._classes
                    if DEBUG_MMT and fork_iter <= 3:
                        print(f"DEBUG: len(fjclassmap)={len(fjclassmap)}, fjclassmap={fjclassmap}, fanout={fanout}")
                        print(f"DEBUG: nonfj_classes count={len(nonfj_classes)}")
                    for r in range(len(fjclassmap)):  # r is auxiliary class index
                        s = int(fjclassmap[r])  # s is original class
                        if DEBUG_MMT and fork_iter <= 3:
                            print(f"DEBUG: r={r}, s={s}, check s>=0:{s>=0}, r<len(fanout):{r<len(fanout)}, fanout[r]>0:{fanout[r]>0 if r<len(fanout) else 'N/A'}")
                        if s >= 0 and r < len(fanout) and fanout[r] > 0:
                            # MATLAB: nonfjSource.arrivalProcess{r}.setRate((fanout(r)-1)*forkLambda(r))
                            # In Python, use set_arrival with the aux class object
                            aux_class_idx = orig_nclasses + r
                            if aux_class_idx < len(nonfj_classes):
                                aux_class = nonfj_classes[aux_class_idx]
                                arr_proc = nonfj_source.get_arrival(aux_class)
                                is_disabled = isinstance(arr_proc, Disabled) if arr_proc is not None else True
                                if DEBUG_MMT and fork_iter <= 3:
                                    print(f"DEBUG: aux_class={aux_class.name}, arr_proc={arr_proc}, is_disabled={is_disabled}")
                                if arr_proc is not None and not is_disabled:
                                    new_rate = (fanout[r] - 1) * fork_lambda[r]
                                    if DEBUG_MMT and fork_iter <= 3:
                                        print(f"DEBUG: Updating aux class {aux_class.name} rate to {new_rate}, fanout={fanout[r]}, forkLambda={fork_lambda[r]}")
                                    if new_rate > 0:
                                        # Update by setting a new Exp distribution with the updated rate
                                        nonfj_source.set_arrival(aux_class, Exp(new_rate))
                    # Refresh the model struct after updating arrival rates
                    nonfjmodel.refresh_struct()
                except Exception as ex:
                    if DEBUG_MMT:
                        print(f"DEBUG: Exception in arrival rate update: {ex}")
                        import traceback
                        traceback.print_exc()

            # Solve nonfjmodel (MATLAB line 96, then lines 181-190)
            nonfjmodel.refresh_struct()
            nonfj_sn = nonfjmodel._sn

            from . import SolverMVA
            transformed_solver = SolverMVA(nonfjmodel)
            transformed_solver._skip_fork_join = True  # Avoid recursion
            transformed_solver._force_method = 'amva'

            try:
                transformed_solver.runAnalyzer()
                trans_result = transformed_solver._result
            except Exception as e:
                import warnings
                warnings.warn(f"MVA on transformed model failed: {e}.")
                break

            if trans_result is None:
                break

            # Get results from transformed model
            QN_full = trans_result['QN'].copy()
            UN_full = trans_result['UN'].copy()
            RN_full = trans_result['RN'].copy()
            TN_full = trans_result['TN'].copy()
            CN_full = trans_result.get('CN', np.zeros(QN_full.shape[1])).copy()
            XN_full = trans_result['XN'].copy()

            if DEBUG_MMT and fork_iter <= 3:
                print(f"DEBUG: After solve iter {fork_iter}, TN_full shape={TN_full.shape}")
                print(f"DEBUG: TN_full =\n{TN_full}")

            # MATLAB lines 97-108: Convergence check
            # Use sum of QN over stations for each original class
            QN_check = np.sum(QN_full[:, :orig_nclasses], axis=0)
            if np.max(np.abs(1 - QN_1 / (QN_check + 1e-14))) < GlobalConstants.CoarseTol and fork_iter > 2:
                fork_loop = False
            else:
                QN_1 = QN_check.copy()

            # MATLAB lines 194-237: Compute sync delays for each fork
            # nonfjstruct = nonfj_sn (the transformed model's sn)
            nonfjstruct = nonfj_sn

            # Pre-compute linked routing matrix once (loop-invariant)
            P_precomputed = nonfjmodel.get_linked_routing_matrix()

            for f in fork_indices:
                # MATLAB lines 200-206: Compute TNfork
                TNfork = np.zeros(orig_nclasses)
                for c in range(sn.nchains):
                    inchain = np.where(sn.chains[c, :] > 0)[0]
                    for r in inchain:
                        if parent_forks is not None and f < len(parent_forks):
                            pf = int(parent_forks[f])
                            if hasattr(sn, 'nodevisits') and sn.nodevisits is not None:
                                if c < len(sn.nodevisits) and sn.nodevisits[c] is not None:
                                    nv = sn.nodevisits[c]
                                    if pf < nv.shape[0] and r < nv.shape[1]:
                                        visits_sum = 0
                                        refstat = int(sn.refstat[r]) if r < len(sn.refstat) else 0
                                        if hasattr(sn, 'visits') and sn.visits is not None and c < len(sn.visits):
                                            v = sn.visits[c]
                                            stateful_idx = int(sn.stationToStateful[refstat]) if refstat < len(sn.stationToStateful) else refstat
                                            if v is not None and stateful_idx < v.shape[0]:
                                                visits_sum = np.sum(v[stateful_idx, inchain])
                                        if visits_sum > 0 and refstat < TN_full.shape[0]:
                                            TNfork[r] = (nv[pf, r] / visits_sum) * np.sum(TN_full[refstat, inchain])

                # MATLAB lines 208-209: Find join and auxiliary classes for this fork
                join_idx_arr = np.where(sn.fj[f, :] > 0)[0]
                has_join = len(join_idx_arr) > 0
                join_idx = join_idx_arr[0] if has_join else None

                fork_aux_classes = np.where(fjforkmap == f)[0]

                for s_idx in fork_aux_classes:
                    r = int(fjclassmap[s_idx])  # original class
                    if r < 0 or r >= orig_nclasses:
                        continue

                    # MATLAB lines 212-217: Update forkLambda
                    if not has_join:
                        # No join: use TNfork (MATLAB line 213)
                        fork_lambda[s_idx] = 0.5 * (fork_lambda[s_idx] + TNfork[r])
                    else:
                        join_ist = int(sn.nodeToStation[join_idx]) if join_idx < len(sn.nodeToStation) else -1
                        if join_ist >= 0 and join_ist < TN_full.shape[0]:
                            # TN at join for class r plus auxiliary classes mapped to r
                            aux_for_r = np.where(fjclassmap == r)[0]
                            tn_join_r = TN_full[join_ist, r] + np.sum(TN_full[join_ist, orig_nclasses + aux_for_r]) - TN_full[join_ist, orig_nclasses + s_idx]
                            fork_lambda[s_idx] = 0.5 * (fork_lambda[s_idx] + tn_join_r)

                    # MATLAB line 218: if isempty(joinIdx) || ~outer_forks(f, r) -> skip sync delay
                    if DEBUG_MMT and fork_iter <= 3:
                        print(f"DEBUG sync: f={f}, r={r}, has_join={has_join}, outer_forks shape={outer_forks.shape if outer_forks is not None else None}")
                        if outer_forks is not None and f < outer_forks.shape[0] and r < outer_forks.shape[1]:
                            print(f"DEBUG sync: outer_forks[{f},{r}]={outer_forks[f, r]}")
                    if not has_join or outer_forks is None or (f < outer_forks.shape[0] and r < outer_forks.shape[1] and not outer_forks[f, r]):
                        if DEBUG_MMT and fork_iter <= 3:
                            print(f"DEBUG sync: SKIPPING sync delay for f={f}, r={r}")
                        continue

                    # MATLAB lines 222-225: Find paths using findPathsCS
                    try:
                        P = P_precomputed
                        if DEBUG_MMT and fork_iter <= 3:
                            print(f"DEBUG findPaths: P is None: {P is None}")
                        if P is not None:
                            # Convert cell array to combined matrix
                            P_combined = SolverMVA._combine_routing_matrix(P)
                            if DEBUG_MMT and fork_iter <= 3:
                                print(f"DEBUG findPaths: P_combined shape={P_combined.shape if P_combined is not None else None}")
                            ri = ModelAdapter.find_paths_cs(
                                sn, P_combined, f, join_idx, r,
                                [r, orig_nclasses + s_idx], QN_full, TN_full, 0,
                                fjclassmap, fjforkmap, nonfjmodel
                            )
                            if DEBUG_MMT and fork_iter <= 3:
                                print(f"DEBUG findPaths: ri={ri}, len(ri)={len(ri)}")
                        else:
                            ri = np.array([])
                    except Exception as ex:
                        if DEBUG_MMT and fork_iter <= 3:
                            print(f"DEBUG findPaths: Exception {ex}")
                            import traceback
                            traceback.print_exc()
                        ri = np.array([])

                    if len(ri) < 2:
                        if DEBUG_MMT and fork_iter <= 3:
                            print(f"DEBUG findPaths: SKIPPING sync delay, len(ri)={len(ri)} < 2")
                        continue

                    # MATLAB lines 226-232: Compute d0 using order statistics
                    lambdai = 1.0 / ri
                    d0 = 0.0
                    parallel_branches = len(ri)
                    for pow_k in range(parallel_branches):
                        current_sum = 0.0
                        for subset in combinations(range(parallel_branches), pow_k + 1):
                            lambda_sum = np.sum(lambdai[list(subset)])
                            if lambda_sum > 0:
                                current_sum += 1.0 / lambda_sum
                        d0 += ((-1) ** pow_k) * current_sum

                    # MATLAB lines 234-236: Set sync delay at Join
                    fan_out = SolverMVA._get_fanout(sn, f)

                    sync_delay = max(0, d0 * fan_out - np.mean(ri))
                    if DEBUG_MMT and fork_iter <= 3:
                        print(f"DEBUG sync_delay: d0={d0}, fan_out={fan_out}, mean(ri)={np.mean(ri)}, sync_delay={sync_delay}")

                    try:
                        nonfj_nodes = nonfjmodel._nodes
                        nonfj_classes = nonfjmodel._classes
                        if join_idx < len(nonfj_nodes):
                            join_node = nonfj_nodes[join_idx]
                            if DEBUG_MMT and fork_iter <= 3:
                                print(f"DEBUG sync_delay: join_idx={join_idx}, join_node type={type(join_node)}, has set_service={hasattr(join_node, 'set_service')}")
                            if sync_delay > 0 and hasattr(join_node, 'set_service'):
                                # Set for auxiliary class
                                if orig_nclasses + s_idx < len(nonfj_classes):
                                    if DEBUG_MMT and fork_iter <= 3:
                                        print(f"DEBUG sync_delay: Setting aux class {nonfj_classes[orig_nclasses + s_idx].name} sync delay={sync_delay}")
                                    join_node.set_service(nonfj_classes[orig_nclasses + s_idx], Exp.fit_mean(sync_delay))
                                # Set for original class
                                if r < len(nonfj_classes):
                                    if DEBUG_MMT and fork_iter <= 3:
                                        print(f"DEBUG sync_delay: Setting orig class {nonfj_classes[r].name} sync delay={sync_delay}")
                                    join_node.set_service(nonfj_classes[r], Exp.fit_mean(sync_delay))
                                nonfjmodel.refresh_struct()
                    except Exception as ex:
                        if DEBUG_MMT:
                            print(f"DEBUG sync_delay: Exception {ex}")
                            import traceback
                            traceback.print_exc()

        # After loop: merge results (MATLAB lines 303-328)
        QN = QN_full.copy()
        UN = UN_full.copy()
        RN = RN_full.copy()
        TN = TN_full.copy()
        CN = CN_full.copy()
        XN = XN_full.copy()

        # MATLAB line 304: Save TN_orig for Join and Source
        join_indices = [i for i in range(sn.nnodes) if int(sn.nodetype[i]) == int(NodeType.JOIN)]
        source_indices = [i for i in range(sn.nnodes) if int(sn.nodetype[i]) == int(NodeType.SOURCE)]

        preserve_ist_list = []
        for j_idx in join_indices:
            if j_idx < len(nonfj_sn.nodeToStation):
                preserve_ist_list.append(int(nonfj_sn.nodeToStation[j_idx]))
        for s_idx in source_indices:
            if s_idx < len(nonfj_sn.nodeToStation):
                preserve_ist_list.append(int(nonfj_sn.nodeToStation[s_idx]))

        orig_classes_with_aux = [int(s) for s in set(fjclassmap) if s >= 0]

        TN_orig = np.zeros((len(preserve_ist_list), len(orig_classes_with_aux)))
        for pi, ist in enumerate(preserve_ist_list):
            if ist < TN.shape[0]:
                for oi, orig_class in enumerate(orig_classes_with_aux):
                    if orig_class < TN.shape[1]:
                        TN_orig[pi, oi] = TN[ist, orig_class]

        # MATLAB lines 306-319: Merge auxiliary classes
        for r_idx in range(len(fjclassmap)):
            s = int(fjclassmap[r_idx])
            if s >= 0 and s < orig_nclasses:
                aux_col = orig_nclasses + r_idx
                if aux_col < QN.shape[1]:
                    QN[:, s] = QN[:, s] + QN[:, aux_col]
                    UN[:, s] = UN[:, s] + UN[:, aux_col]
                    TN[:, s] = TN[:, s] + TN[:, aux_col]
                    # RN = QN / TN (MATLAB line 316)
                    with np.errstate(divide='ignore', invalid='ignore'):
                        RN[:, s] = np.where(TN[:, s] > 0, QN[:, s] / TN[:, s], 0)

        # MATLAB line 321: Restore TN at Join and Source
        for pi, ist in enumerate(preserve_ist_list):
            if ist < TN.shape[0]:
                for oi, orig_class in enumerate(orig_classes_with_aux):
                    if orig_class < TN.shape[1]:
                        TN[ist, orig_class] = TN_orig[pi, oi]

        # Zero out UN at Join stations (Join has Immediate service, so utilization must be 0)
        for j_idx in join_indices:
            if j_idx < len(nonfj_sn.nodeToStation):
                join_ist = int(nonfj_sn.nodeToStation[j_idx])
                if join_ist < UN.shape[0]:
                    UN[join_ist, :orig_nclasses] = 0

        # MATLAB lines 323-328: Delete auxiliary class columns
        QN_final = QN[:orig_nstations, :orig_nclasses].copy()
        UN_final = UN[:orig_nstations, :orig_nclasses].copy()
        RN_final = RN[:orig_nstations, :orig_nclasses].copy()
        TN_final = TN[:orig_nstations, :orig_nclasses].copy()
        CN_final = CN[:orig_nclasses].copy() if len(CN) >= orig_nclasses else np.sum(RN_final, axis=0)
        XN_final = XN[:orig_nclasses].copy() if len(XN) >= orig_nclasses else np.zeros(orig_nclasses)

        # Compute arrival rates using original sn (MATLAB line 336)
        from ...api.sn.getters import sn_get_arvr_from_tput
        AN_final = sn_get_arvr_from_tput(orig_sn, TN_final, None)

        # Compute residence times (MATLAB line 337)
        WN_final = sn_get_residt_from_respt(orig_sn, RN_final, None)

        self._result = {
            'QN': QN_final,
            'UN': UN_final,
            'RN': RN_final,
            'TN': TN_final,
            'AN': AN_final,
            'XN': XN_final,
            'WN': WN_final,
            'CN': np.sum(RN_final, axis=0),
            'runtime': 0.0,
            'method': 'mmt_open',
            'iter': total_iter
        }

        return self._result

    @staticmethod
    def _combine_routing_matrix(P):
        """Combine cell array routing matrix into single matrix."""
        if P is None:
            return None
        if isinstance(P, np.ndarray):
            return P
        # P is list of lists
        nclasses = len(P)
        if nclasses == 0:
            return None
        nnodes = P[0][0].shape[0] if P[0][0] is not None else 0
        combined = np.zeros((nclasses * nnodes, nclasses * nnodes))
        for r in range(nclasses):
            for s in range(nclasses):
                if r < len(P) and s < len(P[r]) and P[r][s] is not None:
                    combined[r*nnodes:(r+1)*nnodes, s*nnodes:(s+1)*nnodes] = P[r][s]
        return combined

    def _run_ht_transformation(self):
        """
        Run full Heidelberger-Trivedi transformation for closed fork-join networks.

        Implements the full H-T method from MATLAB:
        1. Transform model ONCE using ModelAdapter.ht() to create auxiliary classes
        2. Iterate:
           a. Run standard MVA on the transformed model
           b. Check convergence on QN
           c. Update sync delays at Join and AuxDelay nodes
        3. Merge auxiliary class results back into original classes

        Returns:
            Result dictionary or None if transformation fails
        """
        from ...io.model_adapter import ModelAdapter
        from ...api.sn.network_struct import NodeType
        from ...distributions.continuous import Exp
        from itertools import combinations

        sn = self._sn
        if sn is None or sn.fj is None or not np.any(sn.fj):
            return None

        # Step 1: Transform model using ModelAdapter.ht() (ONCE at the start)
        try:
            ht_result = ModelAdapter.ht(self.model)
        except Exception as e:
            import warnings
            warnings.warn(f"H-T transformation failed: {e}. Falling back to approximate method.")
            return self._run_approximate_ht()

        nonfjmodel = ht_result.nonfjmodel
        fjclassmap = ht_result.fjclassmap
        fjforkmap = ht_result.fjforkmap
        fj_auxiliary_delays = ht_result.fj_auxiliary_delays

        import os
        debug_ht = os.environ.get('DEBUG_HT', '0') == '1'

        if debug_ht:
            print("\n=== H-T Transformation Results ===")
            print(f"Original model: {len(self.model._classes)} classes, {len(self.model._nodes)} nodes")
            print(f"Transformed model: {len(nonfjmodel._classes)} classes, {len(nonfjmodel._nodes)} nodes")
            print(f"fjclassmap: {fjclassmap}")
            print(f"fjforkmap: {fjforkmap}")
            print(f"fj_auxiliary_delays: {fj_auxiliary_delays}")

        # Check if transformation actually created auxiliary classes
        if len(nonfjmodel._classes) == len(self.model._classes):
            if debug_ht:
                print("No auxiliary classes created, falling back to approximate H-T")
            return self._run_approximate_ht()

        orig_sn = sn
        orig_nclasses = len(self.model._classes)

        # Find fork indices in original model
        fork_indices = []
        for i, nt in enumerate(orig_sn.nodetype):
            nt_val = nt.value if hasattr(nt, 'value') else int(nt)
            if nt_val == NodeType.FORK.value if hasattr(NodeType.FORK, 'value') else NodeType.FORK:
                fork_indices.append(i)

        # Iteration parameters
        max_iter = self.options.max_iter if hasattr(self.options, 'max_iter') else 1000
        coarse_tol = 1e-3

        # Initialize for convergence check
        QN_prev = None
        converged = False
        iter_count = 0

        if debug_ht:
            print(f"\nFork indices: {fork_indices}")
            print(f"Max iterations: {max_iter}, Tolerance: {coarse_tol}")

        while not converged and iter_count < max_iter:
            iter_count += 1

            # Refresh and run MVA on transformed model
            nonfjmodel.refresh_struct()
            nonfjstruct = nonfjmodel._sn

            if debug_ht and iter_count == 1:
                print("\n=== Transformed Model Structure ===")
                print(f"Stations: {nonfjstruct.nstations}, Classes: {nonfjstruct.nclasses}, Chains: {nonfjstruct.nchains}")
                print(f"stationToNode: {nonfjstruct.stationToNode}")
                print(f"nodeToStation: {nonfjstruct.nodeToStation}")
                print(f"refstat: {nonfjstruct.refstat}")
                print(f"njobs (populations): {nonfjstruct.njobs}")
                print(f"chains: {nonfjstruct.chains}")
                print("Service rates (rates matrix):")
                print(nonfjstruct.rates)
                print("Visits per chain:")
                for c in range(len(nonfjstruct.visits)):
                    print(f"  Chain {c}: {nonfjstruct.visits[c]}")

            nonfj_solver = SolverMVA(nonfjmodel, method=self.method)
            nonfj_solver._skip_fork_join = True

            try:
                nonfj_solver.runAnalyzer()
            except Exception as e:
                import warnings
                warnings.warn(f"MVA on transformed model failed: {e}. Falling back to approximate method.")
                return self._run_approximate_ht()

            if nonfj_solver._result is None:
                return self._run_approximate_ht()

            result = nonfj_solver._result
            QN = result.get('QN', np.zeros((nonfj_solver.nstations, nonfj_solver.nclasses)))
            RN = result.get('RN', np.zeros((nonfj_solver.nstations, nonfj_solver.nclasses)))
            TN = result.get('TN', np.zeros((nonfj_solver.nstations, nonfj_solver.nclasses)))
            UN = result.get('UN', np.zeros((nonfj_solver.nstations, nonfj_solver.nclasses)))
            XN = result.get('XN', np.zeros(nonfj_solver.nclasses))

            if debug_ht:
                print(f"\n=== Iteration {iter_count} ===")
                qn_per_class = np.nansum(QN, axis=0)
                print(f"QN sum per class: {qn_per_class}")
                print(f"XN: {XN}")
                print(f"RN matrix:\n{RN}")

            # Check convergence (MATLAB: max(abs(1-QN_1./QN)) < CoarseTol && forkIter > 2)
            max_diff = float('inf')
            if QN_prev is not None and iter_count > 2:
                # Use full QN matrix for convergence check (like MATLAB)
                # Only consider entries where QN is non-negligible (to avoid 0/0 issues)
                from ...constants import GlobalConstants
                # Create mask for meaningful entries (both current and previous are > Zero)
                meaningful = (np.abs(QN) > GlobalConstants.Zero) & (np.abs(QN_prev) > GlobalConstants.Zero)
                if np.any(meaningful):
                    ratio = np.divide(QN_prev, QN, out=np.ones_like(QN), where=meaningful)
                    diff = np.abs(1 - ratio)
                    diff = np.where(meaningful, diff, 0.0)  # Ignore non-meaningful entries
                    max_diff = np.max(diff)
                else:
                    max_diff = 0.0  # All zeros, consider converged
                if debug_ht:
                    print(f"  Convergence check: max_diff={max_diff:.6e}, tol={coarse_tol}")
                if max_diff < coarse_tol:
                    converged = True
                    if debug_ht:
                        print(f"  CONVERGED at iteration {iter_count}")

            QN_prev = QN.copy()

            if converged:
                break

            # Update sync delays (MATLAB lines 238-272)
            from ...constants import GlobalConstants

            if debug_ht:
                print(f"\n--- Sync Delay Update (iter {iter_count}) ---")

            for f in fork_indices:
                join_idx_arr = np.where(orig_sn.fj[f, :] > 0)[0]
                if len(join_idx_arr) == 0:
                    continue
                join_idx = join_idx_arr[0]

                aux_delay_idx = fj_auxiliary_delays.get(join_idx, None)
                if aux_delay_idx is None:
                    if debug_ht:
                        print(f"  Fork {f}: No aux_delay for join_idx={join_idx}")
                    continue

                # Get station indices in transformed model
                join_station = nonfjstruct.nodeToStation[join_idx] if join_idx < len(nonfjstruct.nodeToStation) else -1
                aux_delay_station = nonfjstruct.nodeToStation[aux_delay_idx] if aux_delay_idx < len(nonfjstruct.nodeToStation) else -1

                if debug_ht:
                    print(f"  Fork {f}: join_idx={join_idx}, join_station={join_station}, aux_delay_idx={aux_delay_idx}, aux_delay_station={aux_delay_station}")

                if join_station < 0 or aux_delay_station < 0:
                    continue

                # Get tasksPerLink (fanout) from fork node
                # MATLAB: sn.nodeparam{f}.fanOut (tasksPerLink, typically 1)
                fork_node = self.model._nodes[f]
                fanout = 1  # Default: 1 task per forked branch
                if hasattr(fork_node, 'get_tasks_per_link'):
                    tpl = fork_node.get_tasks_per_link()
                    if tpl is not None and not np.any(np.isnan(tpl)):
                        fanout = int(np.max(tpl)) if hasattr(tpl, '__len__') else int(tpl)
                        if fanout < 1:
                            fanout = 1

                if debug_ht:
                    print(f"    fanout (tasksPerLink): {fanout}")

                # For each original class
                for r in range(orig_nclasses):
                    # Check if this class visits the fork (MATLAB line 243: if sn.nodevisits{c}(f,r) == 0 continue)
                    # We need to check nodevisits to skip classes that don't use this fork
                    skip_class = False
                    if hasattr(orig_sn, 'nodevisits') and orig_sn.nodevisits is not None:
                        for c in range(orig_sn.nchains):
                            if hasattr(orig_sn, 'chains') and orig_sn.chains is not None:
                                chains_arr = np.asarray(orig_sn.chains)
                                if chains_arr.ndim == 2 and c < chains_arr.shape[0] and r < chains_arr.shape[1]:
                                    if chains_arr[c, r] > 0:  # Class r is in chain c
                                        nodevisits = orig_sn.nodevisits[c]
                                        if nodevisits is not None and f < nodevisits.shape[0] and r < nodevisits.shape[1]:
                                            if nodevisits[f, r] == 0:
                                                skip_class = True
                                                break

                    if skip_class:
                        if debug_ht:
                            print(f"    Class {r}: Skipping (nodevisits[{f},{r}] == 0)")
                        continue

                    # Get auxiliary classes for this original class and fork
                    aux_class_indices = np.where((fjclassmap == r) & (fjforkmap == f))[0]
                    if len(aux_class_indices) == 0:
                        if debug_ht:
                            print(f"    Class {r}: No auxiliary classes")
                        continue

                    if debug_ht:
                        print(f"    Class {r}: aux_class_indices={aux_class_indices}")

                    # MATLAB: ri = RN(:, find(fjclassmap == r));
                    # MATLAB: ri = sum(ri, 1, "omitnan") - RN(aux_delay, aux_classes) - RN(join, aux_classes)
                    ri = []
                    for aux_idx in aux_class_indices:
                        # Sum RN across all stations for this aux class
                        total_rn = np.nansum(RN[:, aux_idx])
                        # Subtract join and aux delay
                        rn_join = RN[join_station, aux_idx] if not np.isnan(RN[join_station, aux_idx]) else 0
                        rn_auxdelay = RN[aux_delay_station, aux_idx] if not np.isnan(RN[aux_delay_station, aux_idx]) else 0
                        branch_rt = total_rn - rn_join - rn_auxdelay
                        if debug_ht:
                            print(f"      aux_idx={aux_idx}: sum(RN[:,aux])={total_rn:.6f}, RN[join,aux]={rn_join:.6f}, RN[auxdelay,aux]={rn_auxdelay:.6f} => ri={branch_rt:.6f}")
                        # Handle NaN/Inf
                        if np.isnan(branch_rt) or np.isinf(branch_rt):
                            branch_rt = 0
                        ri.append(max(branch_rt, 1e-10))  # Avoid division by zero

                    ri_arr = np.array(ri)
                    num_branches = len(ri_arr)

                    if debug_ht:
                        print(f"    ri_arr={ri_arr}, num_branches={num_branches}")

                    if num_branches < 2:
                        if debug_ht:
                            print(f"    Skipping: num_branches < 2")
                        continue

                    # MATLAB line 252: parallel_branches = length(self.model.nodes{f}.output.outputStrategy{r}{3});
                    # Get the actual number of parallel branches from fork output strategy
                    parallel_branches = num_branches
                    if hasattr(fork_node, 'output') and hasattr(fork_node.output, 'output_strategy'):
                        strategy = fork_node.output.output_strategy
                        if isinstance(strategy, dict) and r in strategy:
                            strat_r = strategy[r]
                            if isinstance(strat_r, (list, tuple)) and len(strat_r) >= 3:
                                parallel_branches = len(strat_r[2]) if hasattr(strat_r[2], '__len__') else num_branches
                    if debug_ht:
                        print(f"    parallel_branches={parallel_branches}")

                    # Compute E[max] using H-T inclusion-exclusion formula
                    lambdai = 1.0 / ri_arr
                    d0 = 0.0
                    for pow_val in range(parallel_branches):
                        combos = list(combinations(lambdai, pow_val + 1))
                        current_sum = sum(1.0 / sum(combo) for combo in combos)
                        d0 += ((-1) ** pow_val) * current_sum

                    if debug_ht:
                        print(f"    lambdai={lambdai}, d0={d0:.6f}")

                    # Individual sync delays: di = d0 * fanout - ri
                    di = d0 * fanout - ri_arr

                    if debug_ht:
                        print(f"    di = d0*fanout - ri = {d0:.6f}*{fanout} - {ri_arr} = {di}")

                    # r0 = sum of response times for all inchain classes, minus join
                    # MATLAB: r0 = sum(RN(:, inchain), 2); r0 = sum(r0, 1, "omitnan") - RN(joinIdx, r)
                    # Find inchain classes (all classes in the same chain as r)
                    inchain = [r]  # Start with original class
                    if hasattr(nonfjstruct, 'inchain') and nonfjstruct.inchain is not None:
                        chain_id = get_chain_for_class(nonfjstruct.chains, r)
                        if chain_id >= 0 and chain_id in nonfjstruct.inchain:
                            inchain = list(nonfjstruct.inchain[chain_id])
                    elif hasattr(nonfjstruct, 'chains') and nonfjstruct.chains is not None:
                        chain_id = get_chain_for_class(nonfjstruct.chains, r)
                        if chain_id >= 0:
                            # Fallback: find classes by searching 2D chains matrix
                            chains_arr = np.asarray(nonfjstruct.chains)
                            if chains_arr.ndim == 2 and chain_id < chains_arr.shape[0]:
                                inchain = list(np.where(chains_arr[chain_id, :] > 0)[0])

                    if debug_ht:
                        print(f"    inchain={inchain}")

                    # Sum RN across all inchain classes
                    # MATLAB: r0 = sum(RN(:, inchain), 2) gives a column vector, then sum(..., 1) sums it
                    r0_matrix = RN[:, inchain]  # M x len(inchain)
                    r0_matrix = np.where(np.isnan(r0_matrix) | np.isinf(r0_matrix), 0, r0_matrix)
                    r0 = np.sum(r0_matrix) - RN[join_station, r]
                    if np.isnan(r0) or np.isinf(r0):
                        r0 = 0
                    r0 = max(r0, GlobalConstants.FineTol)

                    if debug_ht:
                        print(f"    r0 = sum(RN[:, inchain]) - RN[join, r] = {np.sum(r0_matrix):.6f} - {RN[join_station, r]:.6f} = {r0:.6f}")

                    # Update service times
                    # MATLAB line 262: Join service for original class r: d0 * fanout
                    sync_delay_orig = d0 * fanout
                    if debug_ht:
                        print(f"    Setting join[{join_idx}].service(class {r}) = d0*fanout = {sync_delay_orig:.6f}")
                    if sync_delay_orig > 1e-10:
                        nonfjmodel._nodes[join_idx].set_service(nonfjmodel._classes[r], Exp.fit_mean(sync_delay_orig))

                    # For each auxiliary class (MATLAB lines 264-268)
                    for idx, aux_idx in enumerate(aux_class_indices):
                        # Join service for aux class: di[idx]
                        aux_sync_delay = max(di[idx], 1e-10)
                        if debug_ht:
                            print(f"    Setting join[{join_idx}].service(aux_class {aux_idx}) = di[{idx}] = {aux_sync_delay:.6f}")
                            print(f"    Setting aux_delay[{aux_delay_idx}].service(aux_class {aux_idx}) = r0 = {r0:.6f}")
                        nonfjmodel._nodes[join_idx].set_service(nonfjmodel._classes[aux_idx], Exp.fit_mean(aux_sync_delay))
                        # Aux delay service for aux class: r0
                        nonfjmodel._nodes[aux_delay_idx].set_service(nonfjmodel._classes[aux_idx], Exp.fit_mean(r0))

        # Step 3: Merge results (MATLAB lines 274-302)
        if debug_ht:
            print(f"\n=== Step 3: Merge Results ===")
            print(f"Final QN before merge:\n{QN}")
            print(f"Final RN before merge:\n{RN}")
            print(f"Final TN before merge:\n{TN}")
            print(f"Final XN before merge: {XN}")

        # Find join and aux delay station indices
        join_stations = []
        aux_delay_stations = []
        for f in fork_indices:
            join_idx_arr = np.where(orig_sn.fj[f, :] > 0)[0]
            if len(join_idx_arr) > 0:
                join_idx = join_idx_arr[0]
                if join_idx < len(nonfjstruct.nodeToStation):
                    join_stations.append(nonfjstruct.nodeToStation[join_idx])
                aux_delay_idx = fj_auxiliary_delays.get(join_idx, None)
                if aux_delay_idx is not None and aux_delay_idx < len(nonfjstruct.nodeToStation):
                    aux_delay_stations.append(nonfjstruct.nodeToStation[aux_delay_idx])

        # Get original class indices that have auxiliary classes
        orig_classes_with_aux = sorted(set(fjclassmap[fjclassmap >= 0]))

        if debug_ht:
            print(f"join_stations: {join_stations}")
            print(f"aux_delay_stations: {aux_delay_stations}")
            print(f"orig_classes_with_aux: {orig_classes_with_aux}")
            print(f"fjclassmap: {fjclassmap}")
            print(f"stations_to_keep will be: {[i for i in range(QN.shape[0]) if i not in aux_delay_stations]}")

        # Save original class throughputs at join stations before zeroing
        TN_orig_join = {}
        for js in join_stations:
            for oc in orig_classes_with_aux:
                if js < TN.shape[0] and oc < TN.shape[1]:
                    TN_orig_join[(js, oc)] = TN[js, oc]

        if debug_ht:
            print(f"TN_orig_join: {TN_orig_join}")

        # Zero out original class metrics at join stations
        for js in join_stations:
            for oc in orig_classes_with_aux:
                if js < QN.shape[0] and oc < QN.shape[1]:
                    QN[js, oc] = 0
                    RN[js, oc] = 0
                    TN[js, oc] = 0
                    UN[js, oc] = 0

        # Remove auxiliary delay station rows (create mask for stations to keep)
        stations_to_keep = [i for i in range(QN.shape[0]) if i not in aux_delay_stations]

        QN_merged = QN[stations_to_keep, :]
        UN_merged = UN[stations_to_keep, :]
        RN_merged = RN[stations_to_keep, :]
        TN_merged = TN[stations_to_keep, :]

        if debug_ht:
            print(f"After removing aux delay stations:")
            print(f"QN_merged shape: {QN_merged.shape}")
            print(f"QN_merged:\n{QN_merged}")
            print(f"RN_merged:\n{RN_merged}")
            print(f"TN_merged:\n{TN_merged}")

        # Merge auxiliary classes into original classes
        # fjclassmap[class_idx] = -1 for original classes, = orig_class_idx for auxiliary classes
        # Auxiliary classes are stored at their actual indices in the fjclassmap
        for aux_class_idx in range(len(fjclassmap)):
            orig_idx = fjclassmap[aux_class_idx]
            # Only process auxiliary classes (those that map to an original class)
            if orig_idx >= 0 and aux_class_idx < QN_merged.shape[1] and orig_idx < QN_merged.shape[1]:
                if debug_ht:
                    print(f"Merging aux_class {aux_class_idx} into orig_class {orig_idx}:")
                    print(f"  QN_merged[:,{orig_idx}] before: {QN_merged[:, orig_idx]}")
                    print(f"  QN_merged[:,{aux_class_idx}]: {QN_merged[:, aux_class_idx]}")
                QN_merged[:, orig_idx] += QN_merged[:, aux_class_idx]
                UN_merged[:, orig_idx] += UN_merged[:, aux_class_idx]
                # Add all throughputs of the auxiliary classes to facilitate the computation of the response times
                TN_merged[:, orig_idx] += TN_merged[:, aux_class_idx]
                if debug_ht:
                    print(f"  QN_merged[:,{orig_idx}] after: {QN_merged[:, orig_idx]}")
                # Recompute RN = QN / TN after merging (matching JAR behavior)
                for i in range(RN_merged.shape[0]):
                    if TN_merged[i, orig_idx] > 0:
                        RN_merged[i, orig_idx] = QN_merged[i, orig_idx] / TN_merged[i, orig_idx]
                if debug_ht:
                    print(f"  RN_merged[:,{orig_idx}] after: {RN_merged[:, orig_idx]}")

        if debug_ht:
            print(f"After merging auxiliary classes:")
            print(f"QN_merged:\n{QN_merged}")
            print(f"RN_merged:\n{RN_merged}")
            print(f"TN_merged:\n{TN_merged}")

        # Restore original class throughputs at join stations
        for (old_js, oc), tput in TN_orig_join.items():
            if old_js in stations_to_keep:
                new_js_idx = stations_to_keep.index(old_js)
                if new_js_idx < TN_merged.shape[0] and oc < TN_merged.shape[1]:
                    TN_merged[new_js_idx, oc] = tput

        # Zero out UN at Join stations for all original classes
        # Join has Immediate service in the original model, so utilization must be 0
        for js in join_stations:
            if js in stations_to_keep:
                new_js_idx = stations_to_keep.index(js)
                if new_js_idx < UN_merged.shape[0]:
                    UN_merged[new_js_idx, :orig_nclasses] = 0

        # Extract only original classes
        QN_final = QN_merged[:, :orig_nclasses]
        UN_final = UN_merged[:, :orig_nclasses]
        RN_final = RN_merged[:, :orig_nclasses]
        TN_final = TN_merged[:, :orig_nclasses]
        XN_final = XN[:orig_nclasses].copy()

        if debug_ht:
            print(f"\n=== Final Results (orig_nclasses={orig_nclasses}) ===")
            print(f"QN_final:\n{QN_final}")
            print(f"RN_final:\n{RN_final}")
            print(f"TN_final:\n{TN_final}")
            print(f"XN_final: {XN_final}")
            print(f"CN (sum RN): {np.sum(RN_final, axis=0)}")

        # Compute arrival rates using sn_get_arvr_from_tput (MATLAB runAnalyzer.m line 341)
        from ...api.sn.getters import sn_get_arvr_from_tput
        AN_final = sn_get_arvr_from_tput(self._sn, TN_final, None)

        # Compute residence times from response times (WN = RN * V)
        from ...api.sn.transforms import sn_get_residt_from_respt
        WN_final = sn_get_residt_from_respt(self._sn, RN_final, None)

        # Store results
        self._result = {
            'QN': QN_final,
            'UN': UN_final,
            'RN': RN_final,
            'TN': TN_final,
            'AN': AN_final,
            'XN': XN_final,
            'WN': WN_final,
            'CN': np.sum(RN_final, axis=0),
            'runtime': 0.0,
            'method': 'ht',
            'iter': iter_count
        }

        return self._result

    def _run_approximate_ht(self):
        """
        Run Heidelberger-Trivedi MVA for closed fork-join networks.

        Implements the H-T approximation which:
        1. Analyzes each parallel branch independently with full population N
        2. Computes E[max(R_1, ..., R_K)] for synchronization delay
        3. Uses cycle time C = Z + E[max] to compute throughput X = N/C
        4. Iterates until queue lengths converge (matching MATLAB's approach)

        The H-T formula uses exponential approximation for the maximum:
        E[max] = sum_{k=1}^{K} (-1)^{k+1} * sum_{combos of k} 1/sum(rates)

        Note: Results typically differ from exact solutions by ~4% due to the
        exponential assumption in the sync delay formula.

        Returns:
            Result dictionary or None if analysis fails
        """
        from ...api.sn.network_struct import NodeType

        sn = self._sn
        if sn is None or sn.fj is None or not np.any(sn.fj):
            return None

        # Find fork and join indices
        fork_indices = []
        join_indices = []
        for i, nt in enumerate(sn.nodetype):
            nt_val = nt.value if hasattr(nt, 'value') else int(nt)
            if nt_val == NodeType.FORK.value if hasattr(NodeType.FORK, 'value') else NodeType.FORK:
                fork_indices.append(i)
            elif nt_val == NodeType.JOIN.value if hasattr(NodeType.JOIN, 'value') else NodeType.JOIN:
                join_indices.append(i)

        if len(fork_indices) == 0:
            return None

        # Get queueing demands (service times at queues) - also returns queue indices
        L, queue_indices = self._get_queueing_demands()

        # Get delay station indices
        delay_indices = self._get_delay_stations()

        # Get think times (delay station service times)
        Z_base = self._get_think_times()

        # Handle pure delay network (no queueing stations)
        # This happens when all stations are delays or INF (infinite servers)
        if L.shape[0] == 0:
            R = self.nclasses
            QN = np.zeros((self.nstations, R))
            UN = np.zeros((self.nstations, R))
            RN = np.zeros((self.nstations, R))
            TN = np.zeros((self.nstations, R))
            AN = np.zeros((self.nstations, R))
            XN = np.zeros(R)

            # For pure delay network:
            # X[r] = N[r] / Z[r] (throughput = population / think time)
            # R[i,r] = D[i,r] (response time = service time, no queueing)
            # Q[i,r] = X[r] * D[i,r] (using Little's law)
            N = self.njobs.copy()
            for r in range(R):
                if np.isfinite(N[r]) and N[r] > 0 and Z_base[r] > 0:
                    XN[r] = N[r] / Z_base[r]
                    for i in range(self.nstations):
                        if self.demands[i, r] > 0:
                            RN[i, r] = self.demands[i, r]  # Response = service time
                            TN[i, r] = XN[r]  # Throughput at each station
                            QN[i, r] = XN[r] * self.demands[i, r]  # Queue length (Little's law)
                            UN[i, r] = QN[i, r]  # Utilization = queue length for delay stations

            return QN, UN, RN, TN, AN, XN

        # Get population
        N = self.njobs.copy()

        # Initialize result arrays
        R = self.nclasses
        QN = np.zeros((self.nstations, R))
        UN = np.zeros((self.nstations, R))
        RN = np.zeros((self.nstations, R))
        TN = np.zeros((self.nstations, R))
        AN = np.zeros((self.nstations, R))
        XN = np.zeros(R)

        # Identify parallel branches for each fork-join pair
        # Use branch structure to properly handle asymmetric branches (serial stations within a branch)
        fj_branches = {}
        for f in fork_indices:
            join_idx_arr = np.where(sn.fj[f, :])[0]
            if len(join_idx_arr) > 0:
                join_idx = join_idx_arr[0]
                join_station = sn.nodeToStation[join_idx] if join_idx < len(sn.nodeToStation) else -1
                # Get branch structure (list of branches, each branch is list of stations)
                branches = self._find_branch_structure(f, join_idx)
                # Also keep flat list for backward compatibility
                parallel_stations = self._find_parallel_branches(f, join_idx)
                fj_branches[f] = {
                    'join_node': join_idx,
                    'join_station': join_station,
                    'parallel_stations': parallel_stations,
                    'branches': branches  # Each branch is a list of serial stations
                }

        # Initial response times (just service times)
        R_branch = {}  # Response times at parallel branch queues
        for f, fj_info in fj_branches.items():
            R_branch[f] = {}
            for r in range(R):
                if np.isfinite(N[r]) and N[r] > 0:
                    branch_ri = []
                    for station_idx in fj_info['parallel_stations']:
                        if station_idx in queue_indices:
                            q_idx = queue_indices.index(station_idx)
                            branch_ri.append(L[q_idx, r])  # Initial: service time
                        else:
                            branch_ri.append(self.demands[station_idx, r])
                    R_branch[f][r] = np.array(branch_ri) if branch_ri else np.array([1.0])

        # H-T MVA for closed fork-join with outer convergence loop
        # This matches MATLAB's approach which iterates until QN converges
        # Key H-T insight: Each branch is analyzed INDEPENDENTLY with full population N
        max_outer_iter = self.options.max_iter if hasattr(self.options, 'max_iter') else 1000
        coarse_tol = 1e-3  # Convergence tolerance (MATLAB's CoarseTol)

        # Initialize sync delays to 0
        sync_delays = {r: 0.0 for r in range(R)}
        QN_prev = np.ones((self.nstations, R)) * 1e10  # Large initial value for convergence check

        outer_iter = 0
        converged = False

        while not converged and outer_iter < max_outer_iter:
            outer_iter += 1

            # For each class, run H-T analysis
            for r in range(R):
                if not (np.isfinite(N[r]) and N[r] > 0):
                    continue

                n_jobs = int(N[r])

                for f, fj_info in fj_branches.items():
                    branches = fj_info['branches']
                    num_branches = len(branches)

                    if num_branches == 0:
                        continue

                    # H-T approach: analyze each branch INDEPENDENTLY with full population N
                    # Each branch is treated as a separate closed network
                    branch_R = np.zeros(num_branches)  # Branch response times
                    branch_Q = {}  # Per-station queue lengths

                    for b, branch in enumerate(branches):
                        # Get demands for stations in this branch
                        branch_demands = []
                        branch_stations = []
                        for station_idx in branch:
                            if self.rates[station_idx, r] > 0:
                                D_s = 1.0 / self.rates[station_idx, r]
                                branch_demands.append(D_s)
                                branch_stations.append(station_idx)

                        if len(branch_stations) == 0:
                            branch_R[b] = 1e-10
                            continue

                        # Run MVA for this branch alone with population N
                        # Initial queue lengths are 0
                        station_Q = {s: 0.0 for s in branch_stations}
                        station_R = {s: branch_demands[i] for i, s in enumerate(branch_stations)}

                        for k in range(1, n_jobs + 1):
                            # Response time at each station: R_i = D_i * (1 + Q_i)
                            for i, s in enumerate(branch_stations):
                                D_s = branch_demands[i]
                                station_R[s] = D_s * (1 + station_Q[s])

                            # Total branch response time
                            R_branch_k = sum(station_R[s] for s in branch_stations)

                            # Cycle time for this branch = Z + R_branch + sync_delay
                            # (sync_delay accounts for waiting for other branches from previous iter)
                            C_k = Z_base[r] + R_branch_k + sync_delays[r]

                            # Throughput for k jobs
                            X_k = k / C_k

                            # Update queue lengths
                            for s in branch_stations:
                                station_Q[s] = X_k * station_R[s]

                        # Store final branch response time and queue lengths
                        branch_R[b] = sum(station_R[s] for s in branch_stations)
                        for s in branch_stations:
                            branch_Q[s] = station_Q[s]

                    # Compute E[max] of branch response times
                    if num_branches >= 2:
                        d0 = self._compute_sync_delay(branch_R)
                    else:
                        d0 = branch_R[0]

                    # Final cycle time and throughput
                    C_final = Z_base[r] + d0
                    X_final = n_jobs / C_final

                    # Store final results
                    XN[r] = X_final
                    R_branch[f][r] = branch_R

                    # Store individual station results
                    # In H-T, each auxiliary class visits only one branch with full population N
                    # The queue length at each station is from its auxiliary class
                    # Response time RN = QN / TN (Little's Law)
                    for b, branch in enumerate(branches):
                        for station_idx in branch:
                            if station_idx in branch_Q:
                                D_s = 1.0 / self.rates[station_idx, r] if self.rates[station_idx, r] > 0 else 0
                                # Queue length is from the auxiliary class (not divided by branches)
                                QN[station_idx, r] = branch_Q[station_idx]
                                # Utilization = X * D for the actual system throughput
                                UN[station_idx, r] = X_final * D_s
                                TN[station_idx, r] = X_final
                                # Response time = QN / TN (Little's Law)
                                RN[station_idx, r] = QN[station_idx, r] / TN[station_idx, r] if TN[station_idx, r] > 0 else 0
                                # Arrival rate at parallel stations = throughput / num_branches
                                AN[station_idx, r] = X_final / num_branches

                    # Update sync delay for next iteration
                    sync_delays[r] = max(0, d0 - np.mean(branch_R))

            # Set delay station metrics - use individual station demands, not total Z
            for station_idx in delay_indices:
                for r in range(R):
                    D_i = self.demands[station_idx, r]
                    if D_i > 0:
                        RN[station_idx, r] = D_i
                        QN[station_idx, r] = XN[r] * D_i
                        UN[station_idx, r] = XN[r] * D_i  # Utilization at delay
                        TN[station_idx, r] = XN[r]
                        AN[station_idx, r] = XN[r]

            # Set join station metrics
            for f, fj_info in fj_branches.items():
                join_station = fj_info['join_station']
                if join_station >= 0 and join_station < self.nstations:
                    for r in range(R):
                        if np.isfinite(N[r]) and N[r] > 0:
                            ri = R_branch[f].get(r, np.array([1.0]))
                            if len(ri) >= 2:
                                d0 = self._compute_sync_delay(ri)
                                sync_d = max(0, d0 - np.mean(ri))
                                num_branches = len(ri)
                            else:
                                sync_d = 0
                                num_branches = 1

                            RN[join_station, r] = sync_d
                            QN[join_station, r] = XN[r] * sync_d * num_branches
                            TN[join_station, r] = XN[r]
                            # Arrival rate at join = throughput * num_branches (jobs from all branches merge)
                            AN[join_station, r] = XN[r] * num_branches
                            UN[join_station, r] = 0  # Join has no service

            # Check convergence: max(abs(1 - QN_prev / QN)) < tol
            # Avoid division by zero
            QN_safe = np.where(QN > 0, QN, 1e-10)
            QN_prev_safe = np.where(QN_prev > 0, QN_prev, 1e-10)
            rel_change = np.max(np.abs(1 - QN_prev_safe / QN_safe))

            if outer_iter >= 2 and rel_change < coarse_tol:
                converged = True

            QN_prev = QN.copy()

        # Compute final cycle times
        CN = np.zeros(R)
        for r in range(R):
            if np.isfinite(N[r]) and N[r] > 0 and XN[r] > 0:
                CN[r] = N[r] / XN[r]
            else:
                CN[r] = 0

        # Compute residence times from response times (WN = RN * V)
        from ...api.sn.transforms import sn_get_residt_from_respt
        WN = sn_get_residt_from_respt(self._sn, RN, None)

        # Store results
        self._result = {
            'QN': QN,
            'UN': UN,
            'RN': RN,
            'TN': TN,
            'AN': AN,
            'XN': XN,
            'WN': WN,
            'CN': CN,
            'method': 'approximate_ht',
            'iter': outer_iter
        }

        return self._result

    def _compute_fork_join_sync_delays(self, QN, UN, RN, TN, AN, XN):
        """
        Compute and add synchronization delays at Join nodes.

        For each fork-join pair, computes the expected synchronization delay
        using the Heidelberger-Trivedi formula.

        Note: For closed fork-join networks, this post-processing approach gives
        approximate results. The MATLAB implementation uses a full H-T transformation
        that creates auxiliary classes and modifies the model structure, which is
        not yet implemented in Python. Results for closed fork-join networks may
        differ from MATLAB by up to 40%.

        Args:
            QN, UN, RN, TN, AN, XN: Performance metric arrays (modified in place)
        """
        from ...api.sn.network_struct import NodeType
        from itertools import combinations

        sn = self._sn
        if sn.fj is None or not np.any(sn.fj):
            return

        # Find fork indices
        fork_indices = []
        for i, nt in enumerate(sn.nodetype):
            nt_val = nt.value if hasattr(nt, 'value') else int(nt)
            if nt_val == NodeType.FORK:
                fork_indices.append(i)

        for f in fork_indices:
            # Find join associated with this fork
            join_idx_arr = np.where(sn.fj[f, :])[0]
            if len(join_idx_arr) == 0:
                continue
            join_idx = join_idx_arr[0]

            # Get join station index
            if join_idx >= len(sn.nodeToStation):
                continue
            join_station = sn.nodeToStation[join_idx]
            if join_station < 0 or join_station >= self.nstations:
                continue

            # Find the branch structure (which stations are on each branch)
            branches = self._find_branch_structure(f, join_idx)
            parallel_stations = self._find_parallel_branches(f, join_idx)

            if len(branches) < 2:
                continue

            # Get branch response times for each class
            # Sum response times for serial stations within each branch
            for r in range(self.nclasses):
                ri = []
                for branch in branches:
                    branch_resp_time = 0.0
                    for station_idx in branch:
                        if RN[station_idx, r] > 0:
                            branch_resp_time += RN[station_idx, r]
                    if branch_resp_time > 0:
                        ri.append(branch_resp_time)

                if len(ri) >= 2:
                    ri_arr = np.array(ri)
                    num_branches = len(ri_arr)

                    # Compute expected max using H-T formula with response times
                    d0 = self._compute_sync_delay(ri_arr)
                    # Sync delay at join = E[max] - mean (waiting for slowest)
                    sync_delay = d0 - np.mean(ri_arr)
                    sync_delay = max(0, sync_delay)

                    # Get throughput at join
                    # For open classes, use arrival rate from source or TN from parallel stations
                    if XN[r] > 0:
                        tput = XN[r]
                    else:
                        # For open networks, XN may be 0 - use TN from parallel stations
                        tput = 0.0
                        for station_idx in parallel_stations:
                            if TN[station_idx, r] > 0:
                                tput = TN[station_idx, r]
                                break
                        # If still 0, try to get arrival rate from source
                        if tput == 0:
                            source_stations = self._get_source_stations()
                            for src_idx in source_stations:
                                if self.rates[src_idx, r] > 0:
                                    tput = self.rates[src_idx, r]
                                    break

                    # Set join metrics
                    # RespT at join = synchronization delay
                    RN[join_station, r] = sync_delay
                    # QLen = tput * respT * num_branches (jobs from each branch waiting)
                    QN[join_station, r] = tput * sync_delay * num_branches
                    # Throughput at join = same as system throughput
                    TN[join_station, r] = tput
                    # Arrival rate at join = throughput * num_branches (jobs from all branches merge)
                    AN[join_station, r] = tput * num_branches
                    # Utilization stays 0 for join

                    # Update parallel station arrival rates
                    # Arrival rate = throughput / num_branches (visit ratio semantics)
                    for station_idx in parallel_stations:
                        AN[station_idx, r] = tput / num_branches

    def _find_parallel_branches(self, fork_idx: int, join_idx: int) -> List[int]:
        """Find stations on parallel branches between a fork and join.

        Uses the connection matrix to find nodes that are direct successors
        of the fork and eventually lead to the join.

        Returns a flat list of all station indices for backward compatibility.
        """
        branches = self._find_branch_structure(fork_idx, join_idx)
        # Flatten all branches into a single list
        parallel_stations = []
        for branch in branches:
            for station in branch:
                if station not in parallel_stations:
                    parallel_stations.append(station)
        return parallel_stations

    def _find_branch_structure(self, fork_idx: int, join_idx: int) -> List[List[int]]:
        """Find the branch structure between a fork and join.

        Returns a list of branches, where each branch is a list of station indices.
        For asymmetric fork-join, each branch may contain multiple serial stations.
        """
        from ...api.sn.network_struct import NodeType
        import numpy as np

        branches = []
        sn = self._sn

        # Use connection matrix to find direct successors of the fork
        if hasattr(sn, 'connmatrix') and sn.connmatrix is not None:
            connmatrix = np.array(sn.connmatrix)

            # Get direct successors of the fork node
            fork_successors = np.where(connmatrix[fork_idx, :] > 0)[0]

            # For each successor, trace the path to see if it reaches the join
            for succ_node in fork_successors:
                # BFS to find all nodes on this branch until we hit the join
                visited = set()
                queue = [succ_node]
                branch_nodes = []

                while queue:
                    current = queue.pop(0)
                    if current in visited:
                        continue
                    visited.add(current)

                    if current == join_idx:
                        # Reached the join, this is a valid branch
                        break

                    branch_nodes.append(current)

                    # Add successors to queue
                    successors = np.where(connmatrix[current, :] > 0)[0]
                    for s in successors:
                        if s not in visited:
                            queue.append(s)

                # Convert branch nodes to station indices
                branch_stations = []
                for node in branch_nodes:
                    if node < len(sn.nodeToStation):
                        station = sn.nodeToStation[node]
                        if station >= 0 and station not in branch_stations:
                            # Check it's a queue/delay station (has service)
                            if station < len(self.station_types):
                                st = self.station_types[station]
                                if st is not None:
                                    st_val = st.value if hasattr(st, 'value') else int(st)
                                    if st_val in (NodeType.QUEUE, NodeType.DELAY):
                                        branch_stations.append(station)

                if branch_stations:
                    branches.append(branch_stations)
        else:
            # Fallback: treat all queues as single-station branches
            for i in range(self.nstations):
                if self.station_types and i < len(self.station_types):
                    st = self.station_types[i]
                    if st is not None:
                        st_val = st.value if hasattr(st, 'value') else int(st)
                        if st_val == NodeType.QUEUE:
                            branches.append([i])

        return branches

    def _compute_sync_delay(self, path_times: np.ndarray) -> float:
        """
        Compute synchronization delay using Heidelberger-Trivedi formula.

        For K parallel branches with response times r_1, ..., r_K,
        the expected maximum E[max(r_1, ..., r_K)] is computed using
        inclusion-exclusion with exponential approximation.

        Args:
            path_times: Array of response times for parallel paths

        Returns:
            Expected maximum (synchronization point) time
        """
        from itertools import combinations

        if len(path_times) == 0:
            return 0.0
        if len(path_times) == 1:
            return path_times[0]

        # Convert to rates (1/response_time)
        path_times = np.asarray(path_times)
        # Avoid division by zero
        path_times = np.maximum(path_times, 1e-10)
        lambdai = 1.0 / path_times

        d0 = 0.0
        parallel_branches = len(lambdai)

        for pow_val in range(parallel_branches):
            # Get all combinations of (pow_val + 1) elements
            for combo in combinations(range(parallel_branches), pow_val + 1):
                combo_sum = np.sum(lambdai[list(combo)])
                if combo_sum > 0:
                    d0 += ((-1) ** pow_val) * (1.0 / combo_sum)

        return d0

    def _find_node_type_indices(self, nodetype_list, target_type) -> np.ndarray:
        """
        Find indices of nodes with a specific type.

        Properly handles enum comparisons with lists of NodeType values.

        Args:
            nodetype_list: List or array of NodeType values
            target_type: The NodeType to search for

        Returns:
            NumPy array of indices where nodetype matches target_type
        """
        indices = []
        target_val = target_type.value if hasattr(target_type, 'value') else target_type
        for i, nt in enumerate(nodetype_list):
            nt_val = nt.value if hasattr(nt, 'value') else nt
            if nt_val == target_val:
                indices.append(i)
        return np.array(indices, dtype=int)

    def _is_polling_system(self) -> bool:
        """Check if this is a polling queueing system."""
        # Conditions from MATLAB: solver_mva.m line 134
        # - Multiple classes (nclasses > 1)
        # - No closed jobs (nclosedjobs == 0)
        # - Has exactly 3 node types: Source, Queue, Sink
        # - The queue's scheduling strategy is POLLING
        if self.nclasses <= 1:
            return False

        if self.network_type != 'open':
            return False

        # Check for Source-Queue-Sink topology
        from ...lang.base import NodeType, SchedStrategy
        if not hasattr(self, '_sn') or self._sn is None:
            return False

        nodetype = self._sn.nodetype
        if len(nodetype) != 3:
            return False

        # Check node types
        has_source = any(nt == NodeType.SOURCE for nt in nodetype)
        has_queue = any(nt == NodeType.QUEUE for nt in nodetype)
        has_sink = any(nt == NodeType.SINK for nt in nodetype)
        if not (has_source and has_queue and has_sink):
            return False

        # Check if queue uses POLLING scheduling
        sched = self._sn.sched
        if sched is None:
            return False

        for station_idx, sched_strategy in sched.items():
            sched_val = sched_strategy.value if hasattr(sched_strategy, 'value') else sched_strategy
            if sched_val == SchedStrategy.POLLING or sched_val == SchedStrategy.POLLING.value:
                return True

        return False

    def _run_polling_analysis(self):
        """Run polling system analysis."""
        from ...api.polling import polling_qsys_exhaustive, polling_qsys_gated, polling_qsys_1limited
        from ...api.mam import map_erlang
        from ...lang.base import NodeType
        from ...constants import PollingType, GlobalConstants

        R = self.nclasses

        # Get source, queue, and sink station indices
        nodetype = self._sn.nodetype
        source_ist = None
        queue_ist = None

        for i, nt in enumerate(nodetype):
            nt_val = nt.value if hasattr(nt, 'value') else nt
            if nt_val == NodeType.SOURCE or nt_val == NodeType.SOURCE.value:
                source_ist = self._sn.nodeToStation[i]
            elif nt_val == NodeType.QUEUE or nt_val == NodeType.QUEUE.value:
                queue_ist = self._sn.nodeToStation[i]

        if source_ist is None or queue_ist is None:
            return None

        # Get arrival rates
        lambda_arr = np.zeros(R)
        for r in range(R):
            if self.rates[source_ist, r] > 0:
                lambda_arr[r] = self.rates[source_ist, r]

        # Get service rates
        mu = np.zeros(R)
        for r in range(R):
            if self.rates[queue_ist, r] > 0:
                mu[r] = self.rates[queue_ist, r]

        # Get polling type and switchover times from the model nodes
        polling_type = None
        polling_par = 1  # K value for K-limited
        switchover_dists = []

        # Find the queue node in the model
        queue_node = None
        for node in self.model.get_nodes():
            if hasattr(node, 'get_polling_type') and node.get_polling_type() is not None:
                queue_node = node
                pt = node.get_polling_type()
                polling_type = pt.value if hasattr(pt, 'value') else pt
                if hasattr(node, '_polling_k'):
                    polling_par = node._polling_k
                break

        if queue_node is None:
            return None

        # Get switchover time distributions
        for r in range(R):
            jobclass = self.model.get_classes()[r]
            if hasattr(queue_node, '_switchover') and jobclass in queue_node._switchover:
                switchover_dists.append(queue_node._switchover[jobclass])
            else:
                # Default: Immediate (zero switchover time)
                from ...distributions import Immediate
                switchover_dists.append(Immediate())

        # Convert distributions to MAP representations
        arvMAPs = []
        svcMAPs = []
        switchMAPs = []

        for r in range(R):
            # Arrival MAP - simple Poisson with rate lambda
            if lambda_arr[r] > 0:
                D0 = np.array([[-lambda_arr[r]]])
                D1 = np.array([[lambda_arr[r]]])
                arvMAPs.append((D0, D1))
            else:
                D0 = np.array([[-1e-10]])
                D1 = np.array([[1e-10]])
                arvMAPs.append((D0, D1))

            # Service MAP - use Erlang approximation to match SCV (mirrors JAR behavior)
            # For deterministic distributions (SCV ≈ 0), use 20-phase Erlang
            # For other distributions, use Erlang with n = ceil(1/SCV) phases
            if mu[r] > 0:
                mean_svc = 1.0 / mu[r]
                scv = self._sn.scv[queue_ist, r] if self._sn.scv is not None else 1.0
                if scv < GlobalConstants.CoarseTol:
                    # Deterministic or near-deterministic: use 20 phases
                    n_phases = 20
                else:
                    # Match SCV: for Erlang, SCV = 1/n, so n = ceil(1/SCV)
                    n_phases = max(1, int(np.ceil(1.0 / scv)))
                    n_phases = min(n_phases, 100)  # Cap at 100 phases
                D0, D1 = map_erlang(mean_svc, n_phases)
                svcMAPs.append((D0, D1))
            else:
                D0 = np.array([[-1.0]])
                D1 = np.array([[1.0]])
                svcMAPs.append((D0, D1))

            # Switchover MAP
            sw_dist = switchover_dists[r]
            if hasattr(sw_dist, 'isImmediate') and sw_dist.isImmediate():
                # Immediate switchover - use very high rate
                D0 = np.array([[-1e10]])
                D1 = np.array([[1e10]])
            elif hasattr(sw_dist, '_rate'):
                # Exponential
                rate = sw_dist._rate
                D0 = np.array([[-rate]])
                D1 = np.array([[rate]])
            elif hasattr(sw_dist, 'getMean'):
                # Use mean to create exponential approximation
                mean_sw = sw_dist.getMean()
                if mean_sw > 0:
                    rate = 1.0 / mean_sw
                    D0 = np.array([[-rate]])
                    D1 = np.array([[rate]])
                else:
                    D0 = np.array([[-1e10]])
                    D1 = np.array([[1e10]])
            else:
                # Default to immediate
                D0 = np.array([[-1e10]])
                D1 = np.array([[1e10]])
            switchMAPs.append((D0, D1))

        # Call appropriate polling analysis function
        polling_type_val = PollingType.EXHAUSTIVE.value if polling_type is None else polling_type

        if polling_type_val == PollingType.EXHAUSTIVE or polling_type_val == PollingType.EXHAUSTIVE.value:
            W = polling_qsys_exhaustive(arvMAPs, svcMAPs, switchMAPs)
        elif polling_type_val == PollingType.GATED or polling_type_val == PollingType.GATED.value:
            W = polling_qsys_gated(arvMAPs, svcMAPs, switchMAPs)
        elif polling_type_val == PollingType.KLIMITED or polling_type_val == PollingType.KLIMITED.value:
            if polling_par == 1:
                W = polling_qsys_1limited(arvMAPs, svcMAPs, switchMAPs)
            else:
                # For K > 1, fall back to approximation
                return None
        else:
            return None

        # Compute response times: R = W + 1/mu
        R_queue = np.zeros(R)
        for r in range(R):
            if mu[r] > 0:
                R_queue[r] = W[r] + 1.0 / mu[r]
            else:
                R_queue[r] = W[r]

        # Build result arrays
        QN = np.zeros((self.nstations, R))
        UN = np.zeros((self.nstations, R))
        RN = np.zeros((self.nstations, R))
        TN = np.zeros((self.nstations, R))
        AN = np.zeros((self.nstations, R))
        XN = np.zeros(R)

        k = 1  # Number of servers (polling is single server)

        # Set metrics for source station
        TN[source_ist, :] = lambda_arr

        # Set metrics for queue station
        RN[queue_ist, :] = R_queue
        TN[queue_ist, :] = lambda_arr
        AN[queue_ist, :] = lambda_arr
        UN[queue_ist, :] = lambda_arr / mu / k
        QN[queue_ist, :] = lambda_arr * R_queue  # Little's law
        XN = lambda_arr.copy()

        # Compute residence times from response times (WN = RN * V)
        from ...api.sn.transforms import sn_get_residt_from_respt
        WN = sn_get_residt_from_respt(self._sn, RN, None)

        # Store results
        self._result = {
            'QN': QN,
            'UN': UN,
            'RN': RN,
            'TN': TN,
            'AN': AN,
            'XN': XN,
            'WN': WN,
            'lG': 0,
            'runtime': 0,
            'lastiter': 1,
        }

        return self._result

    def _is_cache_only_network(self) -> bool:
        """Check if this is a cache-only network (Source-Cache-Sink)."""
        if self._sn is None:
            return False

        from ...api.sn.network_struct import NodeType

        # Check for open network only (no closed jobs)
        if hasattr(self._sn, 'nclosedjobs') and self._sn.nclosedjobs > 0:
            return False

        # Check if all jobs are open (infinite population)
        if self._sn.njobs is not None:
            if not np.all(np.isinf(self._sn.njobs)):
                return False

        # Check node types - must have exactly Source, Cache, Sink
        if self._sn.nodetype is None:
            return False

        node_types = list(self._sn.nodetype)
        has_source = NodeType.SOURCE in node_types
        has_cache = NodeType.CACHE in node_types
        has_sink = NodeType.SINK in node_types

        # Count each type
        num_sources = node_types.count(NodeType.SOURCE)
        num_caches = node_types.count(NodeType.CACHE)
        num_sinks = node_types.count(NodeType.SINK)

        # Must have exactly 1 source, 1 cache, 1 sink
        if num_sources != 1 or num_caches != 1 or num_sinks != 1:
            return False

        # Total nodes must be 3
        if len(node_types) != 3:
            return False

        return has_source and has_cache and has_sink

    def _has_cache_with_class_switching(self) -> bool:
        """Check if network has cache nodes with class switching (hit/miss classes)."""
        if self._sn is None:
            return False

        from ...api.sn.network_struct import NodeType

        if self._sn.nodetype is None:
            return False

        # Check for cache nodes
        has_cache = False
        for nt in self._sn.nodetype:
            if nt == NodeType.CACHE:
                has_cache = True
                break

        if not has_cache:
            return False

        # Check if cache has hit/miss class switching configured
        model_nodes = self.model.get_nodes() if hasattr(self.model, 'get_nodes') else []
        model_classes = self.model.get_classes() if hasattr(self.model, 'get_classes') else []

        for node in model_nodes:
            if hasattr(node, '_hit_class') and hasattr(node, '_miss_class'):
                # Check if any class has hit/miss configured
                for job_class in model_classes:
                    h = node._hit_class.get(job_class)
                    m = node._miss_class.get(job_class)
                    if h is not None and m is not None:
                        return True

        return False

    def _update_cache_routing_and_visits(self):
        """
        Update routing matrix with cache hit/miss probabilities and refresh visits.

        This implements the routing update logic from MATLAB's solver_mva_cacheqn_analyzer.
        The key steps are:
        1. Compute cache hit/miss probabilities
        2. Update sn.rtnodes to route input class to hit/miss classes with computed probabilities
        3. Recompute sn.rt using stochastic complement
        4. Refresh sn.visits with the updated routing

        References:
            MATLAB: solver_mva_cacheqn_analyzer.m lines 77-91
        """
        from ...api.sn.network_struct import NodeType
        from ...api.cache import cache_xi_fp, cache_gamma_lp, cache_miss_fpi
        from ...api.mc.dtmc import dtmc_stochcomp
        from ...api.sn.transforms import sn_refresh_visits

        sn = self._sn
        if sn is None:
            return

        I = sn.nnodes
        K = sn.nclasses

        # Find cache nodes
        cache_indices = []
        for ind in range(I):
            if ind < len(sn.nodetype) and sn.nodetype[ind] == NodeType.CACHE:
                cache_indices.append(ind)

        if not cache_indices:
            return

        # Get model nodes and classes
        model_nodes = self.model.get_nodes() if hasattr(self.model, 'get_nodes') else []
        model_classes = self.model.get_classes() if hasattr(self.model, 'get_classes') else []

        # Make a copy of rtnodes to modify
        rtnodes = sn.rtnodes.copy() if sn.rtnodes is not None else None
        if rtnodes is None:
            return

        # Compute hit/miss probabilities for each cache and update routing
        for ind in cache_indices:
            cache_node = model_nodes[ind] if ind < len(model_nodes) else None
            if cache_node is None:
                continue

            ch = sn.nodeparam.get(ind) if sn.nodeparam else None
            if ch is None:
                continue

            hitclass = getattr(ch, 'hitclass', None)
            missclass = getattr(ch, 'missclass', None)
            if hitclass is None or missclass is None:
                continue

            # Find input classes (classes that have hit/miss mappings)
            input_classes = []
            for r in range(K):
                if r < len(hitclass) and r < len(missclass):
                    hc = int(hitclass[r]) if hitclass[r] >= 0 else -1
                    mc = int(missclass[r]) if missclass[r] >= 0 else -1
                    if hc >= 0 and mc >= 0:
                        input_classes.append(r)

            if not input_classes:
                continue

            # Compute hit/miss probabilities using FPI (like MATLAB lines 68-71)
            try:
                if hasattr(cache_node, 'get_gamma_matrix'):
                    gamma = cache_node.get_gamma_matrix(K)
                    m_levels = cache_node._item_level_cap if hasattr(cache_node, '_item_level_cap') else np.array([1])

                    # Use cache_miss_fpi to compute miss rates (MATLAB line 70)
                    # For now, use cache_xi_fp and derive hit/miss from pi0
                    xi, pi0, pij, it_fp = cache_xi_fp(gamma, m_levels)
                    access_probs = np.sum(gamma, axis=1)
                    total = np.sum(access_probs)
                    if total > 0:
                        access_probs = access_probs / total
                    overall_hit_rate = np.sum(access_probs * (1 - pi0))
                    overall_miss_rate = 1 - overall_hit_rate
                else:
                    # Default probabilities
                    overall_hit_rate = 0.5
                    overall_miss_rate = 0.5
            except Exception:
                overall_hit_rate = 0.5
                overall_miss_rate = 0.5

            hitprob = np.zeros(K)
            missprob = np.zeros(K)
            for r in input_classes:
                hitprob[r] = overall_hit_rate
                missprob[r] = overall_miss_rate

            # Update routing matrix (MATLAB lines 78-86)
            # For each input class, route from Cache to connected nodes with hit/miss probabilities
            for r in input_classes:
                hc = int(hitclass[r])
                mc = int(missclass[r])

                # Zero out the row for input class at cache
                rtnodes[(ind) * K + r, :] = 0

                # Find connected nodes
                for jnd in range(I):
                    if sn.connmatrix is not None and ind < sn.connmatrix.shape[0] and jnd < sn.connmatrix.shape[1]:
                        if sn.connmatrix[ind, jnd]:
                            # Route to hit class with hit probability
                            rtnodes[(ind) * K + r, (jnd) * K + hc] = hitprob[r]
                            # Route to miss class with miss probability
                            rtnodes[(ind) * K + r, (jnd) * K + mc] = missprob[r]

            # Set hit/miss probs on cache node for result reporting
            if hasattr(cache_node, 'set_result_hit_prob'):
                cache_node.set_result_hit_prob(hitprob)
                cache_node.set_result_miss_prob(missprob)

            # Store exact probs in sn.nodeparam so getAvgNode uses them
            # instead of stale values from a previous solver run (e.g., SSA)
            # MATLAB avoids this because sn is a value-type struct (deep copy)
            ch.actualhitprob = hitprob.copy()
            ch.actualmissprob = missprob.copy()

        # Update sn.rtnodes
        sn.rtnodes = rtnodes

        # Recompute sn.rt using stochastic complement (MATLAB line 87)
        stateful_nodes = np.where(sn.isstateful)[0] if sn.isstateful is not None else np.arange(I)
        stateful_nodes_classes = []
        for sf_idx in stateful_nodes:
            for k in range(K):
                stateful_nodes_classes.append(sf_idx * K + k)
        stateful_nodes_classes = np.array(stateful_nodes_classes, dtype=int)

        try:
            new_rt = dtmc_stochcomp(rtnodes, stateful_nodes_classes)
            sn.rt = new_rt
            # CRITICAL: Also update rt_visits since sn_refresh_visits uses it
            # (if it exists) instead of sn.rt
            if hasattr(sn, 'rt_visits') and sn.rt_visits is not None:
                sn.rt_visits = new_rt.copy()
        except Exception:
            pass

        # Refresh visits (MATLAB line 89)
        try:
            sn_refresh_visits(sn)
        except Exception:
            pass

        # Recompute self.demands with updated visits
        # This replicates the logic from __init__ lines 185-200
        try:
            # Reset demands to 1/rates
            nonzero = self.rates > 0
            self.demands = np.zeros_like(self.rates)
            self.demands[nonzero] = 1.0 / self.rates[nonzero]

            # Apply visits to demands (D = V / mu)
            if hasattr(sn, 'visits') and sn.visits is not None:
                visits_combined = np.zeros_like(self.demands)
                stationToStateful = np.asarray(sn.stationToStateful, dtype=int).flatten()
                for chain_id, visits_chain in sn.visits.items():
                    if isinstance(visits_chain, np.ndarray):
                        for station_idx in range(self.nstations):
                            stateful_idx = stationToStateful[station_idx]
                            if stateful_idx < visits_chain.shape[0]:
                                visits_combined[station_idx, :] += visits_chain[stateful_idx, :]
                self.demands = self.demands * visits_combined
        except Exception:
            pass

    def _run_cache_qn_analysis(self):
        """
        Run MVA analysis for queueing networks with cache nodes.

        This implements the cache QN analyzer similar to MATLAB's
        solver_mva_cacheqn_analyzer. It computes cache hit/miss probabilities,
        updates the routing matrix, converts Cache nodes to ClassSwitch, and
        returns None to let the standard MVA analyzer handle the network with
        the updated routing.

        References:
            MATLAB: solver_mva_cacheqn_analyzer.m
        """
        from ...api.sn.network_struct import NodeType

        sn = self._sn
        if sn is None:
            return None

        I = sn.nnodes

        # Find cache nodes
        cache_indices = []
        for ind in range(I):
            if ind < len(sn.nodetype) and sn.nodetype[ind] == NodeType.CACHE:
                cache_indices.append(ind)

        if not cache_indices:
            return None

        # Update routing matrix with cache hit/miss probabilities
        # This computes hit/miss probabilities, updates rtnodes, refreshes visits
        self._update_cache_routing_and_visits()

        # Convert Cache nodes to ClassSwitch (MATLAB line 38)
        # This allows standard MVA to handle the network topology correctly
        # Save cache indices so we can restore nodetype after solving
        self._cache_indices = cache_indices
        for ind in cache_indices:
            sn.nodetype[ind] = NodeType.CLASSSWITCH

        # Return None to continue with standard MVA using updated routing
        # The standard MVA will properly handle the full topology including
        # downstream calls (like E2 in LN sublayers with synch calls)
        return None

    def _run_cache_analysis(self):
        """Run specialized cache analysis for Source-Cache-Sink models."""
        from ...api.sn.network_struct import NodeType
        from ...api.cache import cache_prob_fpi, cache_ttl_lrua, cache_gamma_lp
        from ...lang.base import ReplacementStrategy

        sn = self._sn
        R = self.nclasses

        # Find source station and get arrival rates
        source_ist = None
        for ist in range(sn.nstations):
            ind = sn.stationToNode[ist]
            if ind < len(sn.nodetype) and sn.nodetype[ind] == NodeType.SOURCE:
                source_ist = ist
                break

        if source_ist is None:
            return None

        source_rate = sn.rates[source_ist, :].copy()
        source_rate = np.nan_to_num(source_rate, nan=0.0)

        # Find cache node
        cache_ind = None
        cache_param = None
        for ind in range(sn.nnodes):
            if ind < len(sn.nodetype) and sn.nodetype[ind] == NodeType.CACHE:
                cache_ind = ind
                if sn.nodeparam is not None and ind in sn.nodeparam:
                    cache_param = sn.nodeparam[ind]
                break

        if cache_ind is None or cache_param is None:
            return None

        # Get cache parameters
        # Use itemcap for full capacity vector if available, otherwise fall back to cap
        cap_raw = getattr(cache_param, 'itemcap', None)
        if cap_raw is None:
            cap_raw = getattr(cache_param, 'cap', [1])
        if np.isscalar(cap_raw) or (isinstance(cap_raw, np.ndarray) and cap_raw.ndim == 0):
            m = np.array([int(cap_raw)])
        else:
            m = np.asarray(cap_raw).ravel()
        n = int(getattr(cache_param, 'nitems', 1))
        h = len(m)  # number of cache levels

        # Get replacement strategy
        replacestrat = getattr(cache_param, 'replacestrat', ReplacementStrategy.RR)

        # Get read probabilities (pread)
        pread = getattr(cache_param, 'pread', None)

        # Build lambda matrix (u x n x h+1)
        # u = number of classes, n = number of items, h+1 = levels (including miss level)
        lambd = np.zeros((R, n, h + 1))
        for v in range(R):
            if pread is not None and v < len(pread) and pread[v] is not None:
                pread_v = np.asarray(pread[v]).ravel()
                for k in range(min(n, len(pread_v))):
                    for l in range(h + 1):
                        lambd[v, k, l] = source_rate[v] * pread_v[k]

        # Get access cost (Rcost)
        Rcost = getattr(cache_param, 'accost', None)
        if Rcost is None:
            # Default routing: linear cache hierarchy
            # MATLAB creates: diag(ones(1,nLevels),1) with last diagonal element set to 1
            # This means: level 0 → 1 → 2 → ... → h, with level h staying at h
            # Rcost should be a 2D structure: Rcost[v][i] is the routing matrix for user v and item i
            def create_default_routing(h):
                """Create default linear cache routing matrix (h+1 x h+1)."""
                mat = np.diag(np.ones(h), 1)  # Super-diagonal with 1s
                mat[h, h] = 1.0  # Last level stays at last level
                return mat
            Rcost = [[create_default_routing(h) for _ in range(n)] for _ in range(R)]

        # Compute gamma using cache_gamma_lp
        try:
            gamma, _, _, _ = cache_gamma_lp(lambd, Rcost)
        except Exception:
            # Fall back to simple gamma computation
            gamma = np.zeros((n, h))
            for k in range(n):
                for l in range(h):
                    gamma[k, l] = np.sum(lambd[:, k, l])

        # Choose algorithm based on replacement strategy
        pij = None
        try:
            if replacestrat in (ReplacementStrategy.RR, ReplacementStrategy.FIFO):
                # Use Fixed Point Iteration method
                pij = cache_prob_fpi(gamma, m)
            elif replacestrat == ReplacementStrategy.LRU:
                # Use TTL-based LRU approximation
                pij = cache_ttl_lrua(lambd, Rcost, m)
            else:
                # Default to FPI
                pij = cache_prob_fpi(gamma, m)
        except Exception:
            # Fall back to simple approximation
            total_cap = np.sum(m)
            hit_prob = min(total_cap / n, 1.0) if n > 0 else 0.0
            pij = np.zeros((n, h + 1))
            pij[:, 0] = 1 - hit_prob  # miss probability
            pij[:, 1:] = hit_prob / h if h > 0 else 0

        # Compute miss rates per class
        miss_rate = np.zeros(R)
        for v in range(R):
            if pread is not None and v < len(pread) and pread[v] is not None:
                pread_v = np.asarray(pread[v]).ravel()
                for k in range(min(n, len(pread_v), pij.shape[0])):
                    miss_rate[v] += source_rate[v] * pread_v[k] * pij[k, 0]

        # Get hit/miss class mappings
        hitclass = np.asarray(getattr(cache_param, 'hitclass', [])).astype(int)
        missclass = np.asarray(getattr(cache_param, 'missclass', [])).astype(int)

        # Initialize throughput array
        XN = np.zeros(R)

        # Set throughputs for hit/miss classes
        for r in range(min(len(hitclass), len(missclass))):
            h_idx = hitclass[r] if r < len(hitclass) else -1
            m_idx = missclass[r] if r < len(missclass) else -1

            if h_idx >= 0 and h_idx < R and m_idx >= 0 and m_idx < R:
                XN[m_idx] = miss_rate[r]
                XN[h_idx] = source_rate[r] - miss_rate[r]

        # Set hit/miss probabilities on cache node
        hit_prob_arr = np.zeros(R)
        miss_prob_arr = np.zeros(R)
        for r in range(R):
            if source_rate[r] > 0:
                miss_prob_arr[r] = miss_rate[r] / source_rate[r]
                hit_prob_arr[r] = 1 - miss_prob_arr[r]

        # Store in nodeparam
        cache_param.actualhitprob = hit_prob_arr
        cache_param.actualmissprob = miss_prob_arr

        # Set on cache node in model
        if hasattr(self, 'model') and hasattr(self.model, '_nodes'):
            cache_node_obj = self.model._nodes[cache_ind]
            if hasattr(cache_node_obj, 'set_result_hit_prob'):
                cache_node_obj.set_result_hit_prob(hit_prob_arr)
            if hasattr(cache_node_obj, 'set_result_miss_prob'):
                cache_node_obj.set_result_miss_prob(miss_prob_arr)

        # Build result arrays
        M = self.nstations
        QN = np.zeros((M, R))
        UN = np.zeros((M, R))
        RN = np.zeros((M, R))
        TN = np.zeros((M, R))
        AN = np.zeros((M, R))

        # Set source throughput
        TN[source_ist, :] = source_rate

        # Set cache station throughput using XN
        cache_ist = sn.nodeToStation[cache_ind]
        if cache_ist >= 0 and cache_ist < M:
            for r in range(R):
                if XN[r] > 0:
                    TN[cache_ist, r] = XN[r]
                elif source_rate[r] > 0:
                    TN[cache_ist, r] = source_rate[r]

        # Compute residence times from response times (WN = RN * V)
        from ...api.sn.transforms import sn_get_residt_from_respt
        WN = sn_get_residt_from_respt(self._sn, RN, None)

        # Store result
        self._result = {
            'QN': QN,
            'UN': UN,
            'RN': RN,
            'TN': TN,
            'CN': RN.copy(),
            'XN': XN,
            'AN': AN,
            'WN': WN,
            'lG': np.nan,
            'runtime': 0.0,
            'iter': 1,
            'method': 'cache'
        }

        return self._result

    def runAnalyzer(self):
        """Run the MVA analysis."""
        start_time = time.time()
        line_debug("MVA: using lang=python", options=self.options)

        # Check for cache-only networks and handle specially
        if self._is_cache_only_network():
            line_debug("Non-reentrant cache (Source-Cache-Sink), routing to cache_analyzer", options=self.options)
            result = self._run_cache_analysis()
            if result is not None:
                return result

        # Check for cache networks with class switching (hit/miss classes)
        # Skip if already in cache QN analysis to avoid recursion
        if not getattr(self, '_skip_cache_qn', False) and self._has_cache_with_class_switching():
            line_debug("Integrated caching-queueing network, routing to cacheqn_analyzer", options=self.options)
            result = self._run_cache_qn_analysis()
            if result is not None:
                return result

        # Check for polling systems and handle specially
        if self._is_polling_system():
            line_debug("Multiclass open polling system, routing to polling_analyzer", options=self.options)
            result = self._run_polling_analysis()
            if result is not None:
                return result
            # Fall through to standard analysis if polling handling failed

        # Check for fork-join networks and handle specially
        # Skip if _skip_fork_join is set (to avoid recursion during H-T transformation)
        if self._has_fork_join() and not getattr(self, '_skip_fork_join', False):
            line_debug("Fork-join network detected, routing to fork_join_analysis", options=self.options)
            result = self._run_fork_join_analysis()
            if result is not None:
                return result
            # Fall through to standard analysis if fork-join handling failed

        from ...api.pfqn import (
            pfqn_mva, pfqn_aql, pfqn_linearizer, pfqn_gflinearizer,
            pfqn_egflinearizer, pfqn_mvald, pfqn_mvams, pfqn_bs, pfqn_sqni,
            pfqn_schmidt, pfqn_ab_amva,
            pfqn_qd, pfqn_qdlin, pfqn_qli, pfqn_fli,
            pfqn_linearizermx,
        )
        from ...api.pfqn.bounds import (
            pfqn_xzabalow, pfqn_xzabaup, pfqn_qzgblow, pfqn_qzgbup,
            pfqn_xzgsblow, pfqn_xzgsbup,
        )

        method = self.method.lower()

        # Normalize AMVA method aliases
        method = method.replace('amva.', '')

        # Get parameters
        L, queue_indices = self._get_queueing_demands()
        N = self.njobs.copy()
        Z = self._get_think_times()
        mi = self.nservers[queue_indices] if len(queue_indices) > 0 else np.ones(1)

        M = L.shape[0]  # Number of queueing stations
        R = self.nclasses  # Number of classes

        # Compute arrival rates for open classes
        lambda_arr = np.zeros(R)
        source_indices = self._get_source_stations()
        for r in range(R):
            if np.isinf(N[r]):  # Open class
                # Get arrival rate from source station
                for src_idx in source_indices:
                    if self.rates[src_idx, r] > 0:
                        lambda_arr[r] = self.rates[src_idx, r]
                        break

        # Initialize result arrays
        QN = np.zeros((self.nstations, R))
        UN = np.zeros((self.nstations, R))
        RN = np.zeros((self.nstations, R))
        TN = np.zeros((self.nstations, R))
        AN = np.zeros((self.nstations, R))
        XN = np.zeros(R)

        # Choose algorithm based on method
        # IMPORTANT: Open networks with feedback also need chain-level MVA to compute visit ratios
        # correctly. Without this, the effective arrival rate at feedback queues is underestimated.
        if self.network_type in ('closed', 'mixed', 'open'):
            # Check if load-dependent MVA should be used (MATLAB: solver_mvald_analyzer)
            # Must check BEFORE converting 'default' method to 'exact'/'amva'
            use_ld_mva = False
            if self.lldscaling is not None:
                use_ld_mva = True
            # Check for class-dependent scaling (MATLAB: ~isempty(sn.cdscaling))
            if hasattr(self._sn, 'cdscaling') and self._sn.cdscaling is not None:
                use_ld_mva = True
            # Mixed networks require chain-level MVA (pfqn_mvaldmx) which handles open classes
            if self.network_type == 'mixed':
                use_ld_mva = True
            # Check for per-class joint dependence (LJCD) scaling (e.g., from FES aggregation)
            if hasattr(self._sn, 'ljcdscaling') and self._sn.ljcdscaling is not None:
                use_ld_mva = True

            # Check for class switching (multiple classes in same chain) BEFORE method selection
            # This is needed because 'amva' doesn't handle class switching properly
            has_class_switching_early = False
            if hasattr(self._sn, 'nchains') and self._sn.nchains > 0:
                chains = self._get_chains()
                for chain in chains:
                    if len(chain) > 1:
                        has_class_switching_early = True
                        break

            # Track if chain-level deaggregation was used (skip delay recompute if so)
            used_chain_deaggregation = False

            # Check product form early for class switching decision
            # Use relaxed check (not_het_fcfs) matching MATLAB solver_amva.m line 90
            from ...api.sn import sn_has_product_form_not_het_fcfs
            has_product_form_early = sn_has_product_form_not_het_fcfs(self._sn) if self._sn is not None else True
            line_debug("Product-form check: hasProductForm=%s (exact method requested)", has_product_form_early, options=self.options)

            # Handle 'default' method with MATLAB-compatible heuristic
            if method == 'default':
                # For mixed networks without actual load-dependent service (no lldscaling),
                # use linearizer which properly handles both open and closed classes
                is_mixed_only = self.network_type == 'mixed' and self.lldscaling is None

                # For open networks with product form and single servers, MATLAB uses egflin
                # (solver_amva.m lines 49-55: if max(nservers)==1, method='egflin')
                is_open_product_form = (self.network_type == 'open' and
                                        has_product_form_early and
                                        self.lldscaling is None)
                max_servers = 1
                if is_open_product_form and self.nservers is not None:
                    finite_servers = self.nservers[np.isfinite(self.nservers)]
                    if len(finite_servers) > 0:
                        max_servers = int(np.max(finite_servers))

                if is_mixed_only:
                    # Use egflin linearizer for mixed networks - handles open+closed properly
                    method = 'egflin'
                elif is_open_product_form and max_servers == 1:
                    # Open network with product form and single servers - use egflin
                    method = 'egflin'
                elif use_ld_mva:
                    # For load-dependent models, MATLAB defaults to approximate MVA (solver_amvald)
                    # even for small models, NOT exact MVA (pfqn_mvaldmx)
                    method = 'amva'
                elif not has_product_form_early:
                    # Non-product-form networks (e.g., heterogeneous FCFS, HOL priority)
                    # Use AMVA which calls solver_amvald with proper priority handling
                    # MATLAB's solver_mva_analyzer also uses solver_amva->solver_amvald for these
                    method = 'amva'
                else:
                    # Match MATLAB's solver_mva_analyzer.m logic for non-LD models:
                    # Use exact MVA if: nchains <= 4 && sum(njobs) <= 20 && product_form && no fractional populations
                    nchains = self._sn.nchains if hasattr(self._sn, 'nchains') and self._sn is not None else R
                    # IMPORTANT: For open networks, total_jobs should be infinity (matches MATLAB's sum(sn.njobs))
                    # This ensures open networks use 'amva' path which handles saturation correctly
                    if np.any(np.isinf(N)):
                        total_jobs = np.inf
                    else:
                        total_jobs = int(np.sum(N[np.isfinite(N)]))
                    from ...api.sn import sn_has_product_form
                    has_product_form = sn_has_product_form(self._sn) if self._sn is not None else True
                    has_fractional = np.any(N != np.floor(N))

                    if nchains <= 4 and total_jobs <= 20 and has_product_form and not has_fractional:
                        method = 'exact'
                    else:
                        method = 'amva'


            # Also check for INF servers which need load-dependent treatment
            # NOTE: Delay stations naturally have INF scheduling and should NOT trigger this
            # Only Queue stations (NodeType.QUEUE) with INF scheduling require load-dependent MVA
            from ...lang.base import SchedStrategy
            from ...api.sn.network_struct import NodeType
            has_inf_server = False
            for idx, q_idx in enumerate(queue_indices):
                if self.sched is not None and q_idx in self.sched:
                    sched_val = self.sched[q_idx]
                    # Check if this is a Queue station (not Delay) with INF servers
                    is_queue_station = False
                    if self.station_types is not None and q_idx < len(self.station_types):
                        st = self.station_types[q_idx]
                        if st is not None:
                            st_val = st.value if hasattr(st, 'value') else int(st)
                            is_queue_station = (st_val == NodeType.QUEUE.value)
                    if is_queue_station and sched_val == SchedStrategy.INF:
                        has_inf_server = True
                        break

            # INF servers require load-dependent MVA (mu scales with population)
            if has_inf_server:
                use_ld_mva = True

            # Check for class switching (multiple classes in same chain)
            has_class_switching = False
            if hasattr(self._sn, 'nchains') and self._sn.nchains > 0:
                chains = self._get_chains()
                for chain in chains:
                    if len(chain) > 1:
                        has_class_switching = True
                        break

            # For simple class switching WITHOUT actual load-dependent service,
            # use the handler's chain-based MVA (pfqn_mvams) which is more accurate
            # Only use pfqn_mvaldmx for actual load-dependent scenarios
            needs_mvaldmx = use_ld_mva and (self.lldscaling is not None or has_inf_server)

            if needs_mvaldmx and method in ['exact', 'mva']:
                line_debug("Load-dependent scaling detected (lldscaling=%s, cdscaling=%s), routing to mvald_analyzer",
                           self.lldscaling is not None, hasattr(self._sn, 'cdscaling') and self._sn.cdscaling is not None,
                           options=self.options)
                # Use EXACT load-dependent MVA (MATLAB: solver_mvald -> pfqn_mvaldmx)
                # IMPORTANT: MATLAB's solver_mvald works at chain level, then disaggregates
                from ...api.pfqn import pfqn_mvaldmx
                from ...api.sn import sn_get_demands_chain, sn_deaggregate_chain_results

                # Get chain-level parameters (matching MATLAB's solver_mvald.m)
                chain_result = sn_get_demands_chain(self._sn)
                Lchain = chain_result.Lchain
                STchain = chain_result.STchain
                Vchain = chain_result.Vchain
                alpha = chain_result.alpha
                Nchain = chain_result.Nchain.flatten()
                refstatchain = chain_result.refstatchain

                C = self._sn.nchains
                total_pop = int(np.sum(Nchain[np.isfinite(Nchain)]))

                # Build mu_chain matrix like MATLAB's solver_mvald.m
                # MUST use full station count M_full (same as Lchain rows), not just queueing stations
                # MATLAB: M = size(STchain,1); mu_chain = ones(M, sum(Nchain(isfinite(Nchain))))
                M_full = Lchain.shape[0]  # Number of ALL stations
                S = self.nservers  # Server counts per station
                mu_chain = np.ones((M_full, total_pop))
                for ist in range(M_full):
                    if ist < len(S) and np.isinf(S[ist]):
                        # INF server: mu[ist,:] = 1:N (linear scaling)
                        for n in range(total_pop):
                            mu_chain[ist, n] = n + 1
                    elif self.lldscaling is not None and ist < self.lldscaling.shape[0]:
                        # Load-dependent: use lldscaling
                        for n in range(total_pop):
                            if n < self.lldscaling.shape[1]:
                                mu_chain[ist, n] = self.lldscaling[ist, n]
                            else:
                                mu_chain[ist, n] = self.lldscaling[ist, -1]
                    elif ist < len(S) and S[ist] > 1:
                        # Finite multiserver queue: mu scales up to number of servers
                        # For c servers: mu = [1, 2, ..., c, c, c, ...]
                        c = int(S[ist])
                        for n in range(total_pop):
                            mu_chain[ist, n] = min(n + 1, c)

                # Build arrival rates for open chains
                # For open classes, arrival rate comes from the Source station, not refstat
                from ...api.sn.network_struct import NodeType
                source_station = None
                for ist in range(self.nstations):
                    if self.station_types and ist < len(self.station_types):
                        st = self.station_types[ist]
                        if st is not None:
                            st_val = st.value if hasattr(st, 'value') else int(st)
                            if st_val == NodeType.SOURCE.value:
                                source_station = ist
                                break

                lambda_chain = np.zeros(C)
                for c in range(C):
                    if np.isinf(Nchain[c]):
                        # Sum arrival rates for all open classes in this chain
                        if c in self._sn.inchain:
                            inchain = self._sn.inchain[c].flatten().astype(int)
                            for r in inchain:
                                if r < R and np.isinf(N[r]):
                                    # For open classes, get arrival rate from Source station
                                    if source_station is not None and source_station < self.nstations:
                                        arr_rate = self.rates[source_station, r]
                                        if arr_rate > 0:
                                            lambda_chain[c] += arr_rate

                # Call pfqn_mvaldmx with chain-level parameters
                # MATLAB: [Xchain,Qchain,Uchain] = pfqn_mvaldmx(lambda,Lchain,Nchain,0*Nchain,mu_chain,S)
                # where S = sn.nservers (all stations)
                Xchain, Qchain, Uchain, _, lGN, Pc = pfqn_mvaldmx(
                    lambda_chain, Lchain, Nchain, np.zeros(C), mu_chain, S
                )

                # Compute Tchain and Rchain like MATLAB solver_mva.m
                # MATLAB: Tchain(k,r) = Xchain(r) * Vchain(k,r)  (line 120)
                #         Rchain = Qchain./Tchain                  (line 153)
                Tchain = np.outer(np.ones(M_full), Xchain) * Vchain
                Rchain = np.zeros((M_full, C))
                for c in range(C):
                    for i in range(M_full):
                        if Tchain[i, c] > 0:
                            Rchain[i, c] = Qchain[i, c] / Tchain[i, c]

                # Disaggregate chain results to class level (matching MATLAB's solver_mvald.m)
                deagg = sn_deaggregate_chain_results(
                    self._sn, Lchain, None, STchain, Vchain, alpha,
                    None, Uchain, Rchain, Tchain, None, Xchain.reshape(1, -1)
                )

                # Copy disaggregated results
                QN = deagg.Q
                UN = deagg.U
                RN = deagg.R
                TN = deagg.T
                XN = deagg.X.flatten()
                AN = TN.copy()
                used_chain_deaggregation = True
            elif use_ld_mva and method in ['default', 'amva']:
                line_debug("Load-dependent scaling detected, routing to approximate mvald (solver_amvald)", options=self.options)
                # Use APPROXIMATE load-dependent MVA (MATLAB: solver_amvald)
                # Note: This branch is for compatibility; the approximate implementation
                # needs more work to match MATLAB exactly
                from ...api.solvers.mva.amvald import solver_amvald, AmvaldOptions

                # Get chain-level parameters
                from ...api.sn import sn_get_demands_chain
                chain_result = sn_get_demands_chain(self._sn)
                Lchain = chain_result.Lchain
                STchain = chain_result.STchain
                Vchain = chain_result.Vchain
                alpha = chain_result.alpha
                Nchain = chain_result.Nchain.flatten()
                refstatchain = chain_result.refstatchain
                SCVchain = np.ones((self._sn.nstations, self._sn.nchains))

                # Set up options
                amvald_options = AmvaldOptions(
                    method='default',
                    iter_tol=self.options.tol,
                    iter_max=self.options.max_iter
                )

                # Call solver_amvald
                result = solver_amvald(
                    self._sn, Lchain, STchain, Vchain, alpha,
                    Nchain, SCVchain, refstatchain, amvald_options
                )

                # Disaggregate chain results to class level
                # MATLAB: pass Uchain only if there's load/class-dependent scaling
                from ...api.sn import sn_deaggregate_chain_results
                has_ld_scaling = (hasattr(self._sn, 'lldscaling') and self._sn.lldscaling is not None and
                                  np.any(self._sn.lldscaling != 0))
                has_cd_scaling = (hasattr(self._sn, 'cdscaling') and self._sn.cdscaling is not None and
                                  np.any(self._sn.cdscaling != 0))
                has_ljd_scaling = (hasattr(self._sn, 'ljdscaling') and self._sn.ljdscaling is not None)
                has_ljcd_scaling = (hasattr(self._sn, 'ljcdscaling') and self._sn.ljcdscaling is not None)
                Uchain_for_deagg = result.U if (has_ld_scaling or has_cd_scaling or has_ljd_scaling or has_ljcd_scaling) else None

                deagg = sn_deaggregate_chain_results(
                    self._sn, Lchain, None, STchain, Vchain, alpha,
                    None, Uchain_for_deagg, result.R, result.T, None, result.X
                )

                # Copy results
                QN = deagg.Q
                UN = deagg.U
                RN = deagg.R
                TN = deagg.T
                XN = deagg.X.flatten()
                AN = TN.copy()
                used_chain_deaggregation = True

            elif method in ['exact', 'mva']:
                line_debug("Standard queueing network, routing to mva_analyzer (method=%s)", method, options=self.options)
                # Use the handler's solver_mva for chain-level aggregation and post-processing
                # This matches MATLAB's solver_mva which does:
                # 1. Chain aggregation via sn_get_demands_chain
                # 2. MVA at chain level via pfqn_mvams
                # 3. Post-processing to recompute Q and X from waiting times
                # 4. Result disaggregation

                # Check for LCFS + LCFS-PR 2-station network BEFORE product-form check
                # because this configuration has a product-form solution but
                # sn_has_product_form_not_het_fcfs doesn't recognize LCFS scheduling
                # (matches MATLAB solver_mva.m lines 22-44 and JAR Solver_mva.kt lines 42-80)
                has_lcfs_network = False
                if self._sn is not None and self._sn.sched:
                    from ...lang.base import SchedStrategy as _SS
                    _lcfs_found = any(
                        _sched_val == _SS.LCFS or
                        (hasattr(_sched_val, 'value') and _sched_val.value == _SS.LCFS.value)
                        for _sched_val in self._sn.sched.values()
                    )
                    _lcfspr_found = any(
                        _sched_val == _SS.LCFSPR or
                        (hasattr(_sched_val, 'value') and _sched_val.value == _SS.LCFSPR.value)
                        for _sched_val in self._sn.sched.values()
                    )
                    has_lcfs_network = _lcfs_found and _lcfspr_found

                if has_lcfs_network:
                    # Dispatch directly to the handler which has LCFS detection and
                    # routes to _solver_mva_lcfsqn (specialized LCFS MVA)
                    from ...api.solvers.mva.handler import solver_mva as mva_handler
                    from ...api.solvers.mva.handler import SolverMVAOptions as MVAHandlerOptions

                    handler_options = MVAHandlerOptions(method='exact', tol=1e-8)
                    result = mva_handler(self._sn, handler_options)

                    QN = result.Q if result.Q is not None else np.zeros((self.nstations, R))
                    UN = result.U if result.U is not None else np.zeros((self.nstations, R))
                    RN = result.R if result.R is not None else np.zeros((self.nstations, R))
                    TN = result.T if result.T is not None else np.zeros((self.nstations, R))
                    XN = result.X.flatten() if result.X is not None else np.zeros(R)
                    AN = TN.copy()

                # Check for product form - if not, fall back to AMVA (MATLAB behavior)
                # Non-product-form open networks (e.g., heterogeneous FCFS) use solver_amvald
                elif not has_product_form_early:
                    # Use solver_amvald for non-product-form models
                    from ...api.solvers.mva.amvald import solver_amvald, AmvaldOptions
                    from ...api.sn import sn_get_demands_chain, sn_deaggregate_chain_results

                    chain_result = sn_get_demands_chain(self._sn)
                    Lchain = chain_result.Lchain
                    STchain = chain_result.STchain
                    Vchain = chain_result.Vchain
                    alpha = chain_result.alpha
                    Nchain = chain_result.Nchain.flatten()
                    refstatchain = chain_result.refstatchain
                    SCVchain = np.ones((self._sn.nstations, self._sn.nchains))

                    amvald_options = AmvaldOptions(method='default', iter_tol=self.options.tol, iter_max=self.options.max_iter)
                    result = solver_amvald(
                        self._sn, Lchain, STchain, Vchain, alpha,
                        Nchain, SCVchain, refstatchain, amvald_options
                    )

                    # Disaggregate chain results to class level
                    # MATLAB: pass Uchain only if there's load/class-dependent scaling
                    # Otherwise, let deaggregation compute U = T * S (MATLAB solver_amvald.m lines 216-220)
                    has_ld_scaling = (hasattr(self._sn, 'lldscaling') and self._sn.lldscaling is not None and
                                      np.any(self._sn.lldscaling != 0))
                    has_cd_scaling = (hasattr(self._sn, 'cdscaling') and self._sn.cdscaling is not None and
                                      np.any(self._sn.cdscaling != 0))
                    Uchain_for_deagg = result.U if (has_ld_scaling or has_cd_scaling) else None

                    deagg = sn_deaggregate_chain_results(
                        self._sn, Lchain, None, STchain, Vchain, alpha,
                        None, Uchain_for_deagg, result.R, result.T, None, result.X
                    )

                    QN = deagg.Q
                    UN = deagg.U
                    RN = deagg.R
                    TN = deagg.T
                    XN = deagg.X.flatten()
                    AN = TN.copy()
                    used_chain_deaggregation = True
                else:
                    from ...api.solvers.mva.handler import solver_mva as mva_handler
                    from ...api.solvers.mva.handler import SolverMVAOptions as MVAHandlerOptions

                    handler_options = MVAHandlerOptions(method='exact', tol=1e-8)
                    result = mva_handler(self._sn, handler_options)

                    # Copy results from handler
                    QN = result.Q if result.Q is not None else np.zeros((self.nstations, R))
                    UN = result.U if result.U is not None else np.zeros((self.nstations, R))
                    RN = result.R if result.R is not None else np.zeros((self.nstations, R))
                    TN = result.T if result.T is not None else np.zeros((self.nstations, R))
                    XN = result.X.flatten() if result.X is not None else np.zeros(R)
                    AN = TN.copy()

            elif method == 'amva':
                line_debug("AMVA method selected, checking multiserver/class-switching/product-form", options=self.options)
                # Check if there are multi-server queues (servers > 1, but not delay stations with inf servers)
                # Delay stations have inf servers but are NOT multiserver queues
                has_multiserver = np.any((mi > 1) & np.isfinite(mi)) if mi is not None else False

                # For class switching with CLOSED models, MATLAB's solver_amva.m:
                # - product-form-not-het-fcfs, no load-dep, single-server: pfqn_linearizermx with egflin
                # - product-form-not-het-fcfs, no load-dep, multi-server: solver_amvald
                # - otherwise: solver_amvald with lin
                is_closed_network = not np.any(np.isinf(N))

                # Check load dependence
                has_load_dep = (hasattr(self._sn, 'lldscaling') and self._sn.lldscaling is not None) or \
                               (hasattr(self._sn, 'cdscaling') and self._sn.cdscaling is not None)

                if has_class_switching_early and is_closed_network and has_product_form_early and not has_load_dep:
                    # MATLAB solver_amva.m lines 90-208: product-form (not-het-fcfs) path
                    # Uses sn_get_product_form_chain_params and pfqn_linearizermx
                    from ...api.sn import sn_get_demands_chain, sn_deaggregate_chain_results, sn_get_product_form_chain_params
                    from ...api.pfqn import pfqn_linearizermx

                    chain_result = sn_get_demands_chain(self._sn)
                    Lchain = chain_result.Lchain
                    STchain = chain_result.STchain
                    Vchain = chain_result.Vchain
                    alpha = chain_result.alpha
                    Nchain = chain_result.Nchain.flatten()

                    nservers = self._sn.nservers
                    if nservers is None:
                        nservers = np.ones(self.nstations)
                    else:
                        nservers = nservers.flatten()

                    # Check max servers for queue stations
                    max_servers = 1
                    for q_idx in queue_indices:
                        if nservers[q_idx] < np.inf:
                            max_servers = max(max_servers, int(nservers[q_idx]))

                    C_chains = self._sn.nchains

                    if max_servers == 1:
                        # Single-server: use pfqn_linearizermx with 'egflin'
                        # (MATLAB solver_amva.m line 208)
                        pf_params = sn_get_product_form_chain_params(self._sn)
                        L_pf = pf_params.D  # Queue demands only (Mq x C)
                        N_pf = pf_params.N.flatten()  # Chain populations
                        Z_pf = pf_params.Z.flatten() if pf_params.Z.ndim > 1 else pf_params.Z  # Delay demands
                        S_pf = pf_params.S.flatten()  # Servers at queues
                        lambda_pf = pf_params.lambda_vec.flatten()

                        # Get scheduling for queueing stations
                        sched_list = []
                        for q_idx in queue_indices:
                            sched_val = self.sched.get(q_idx, SchedStrategy.FCFS) if self.sched else SchedStrategy.FCFS
                            sched_list.append(sched_val)

                        # Handle all-delay case: no non-INF queue stations
                        if not queue_indices:
                            # Pure delay network: X_c = N_c / Z_c
                            Xchain_out = np.zeros(C_chains)
                            for c in range(C_chains):
                                if Z_pf[c] > 0 and N_pf[c] > 0 and not np.isinf(N_pf[c]):
                                    Xchain_out[c] = N_pf[c] / Z_pf[c]
                            Qchain_out = np.zeros((0, C_chains))
                            Uchain_out = np.zeros((0, C_chains))
                        else:
                            Qchain_out, Uchain_out, Wchain_out, Tchain_out, Cchain_out, Xchain_out, iters = pfqn_linearizermx(
                                lambda_pf, L_pf, N_pf, Z_pf, S_pf, sched_list,
                                tol=1e-4, maxiter=1000, method='egflin'
                            )

                        # Build full station chain results
                        Qchain = np.zeros((self.nstations, C_chains))
                        Uchain = np.zeros((self.nstations, C_chains))
                        Rchain = np.zeros((self.nstations, C_chains))
                        Tchain = np.zeros((self.nstations, C_chains))
                        Xchain = Xchain_out.reshape(1, -1) if Xchain_out.ndim == 1 else Xchain_out

                        # Map queue results back to station indices
                        for idx, q_idx in enumerate(queue_indices):
                            for c in range(C_chains):
                                Qchain[q_idx, c] = Qchain_out[idx, c] if Qchain_out.ndim > 1 else Qchain_out[idx]
                                Uchain[q_idx, c] = Uchain_out[idx, c] if Uchain_out.ndim > 1 else Uchain_out[idx]

                        # Compute delay station metrics
                        delay_indices = [i for i in range(self.nstations) if i not in queue_indices]
                        for d_idx in delay_indices:
                            for c in range(C_chains):
                                Qchain[d_idx, c] = Xchain[0, c] * STchain[d_idx, c] * Vchain[d_idx, c]
                                Uchain[d_idx, c] = Qchain[d_idx, c]

                        # Compute throughputs and response times
                        for c in range(C_chains):
                            for i in range(self.nstations):
                                if Vchain[i, c] > 0 and Xchain[0, c] > 0:
                                    Tchain[i, c] = Xchain[0, c] * Vchain[i, c]
                                    if Tchain[i, c] > 0:
                                        Rchain[i, c] = Qchain[i, c] / Tchain[i, c]

                        # Disaggregate chain results to class level
                        deagg = sn_deaggregate_chain_results(
                            self._sn, Lchain, None, STchain, Vchain, alpha,
                            None, None, Rchain, Tchain, None, Xchain
                        )

                        QN = deagg.Q
                        UN = deagg.U
                        RN = deagg.R
                        TN = deagg.T
                        XN = deagg.X.flatten()
                        AN = TN.copy()
                        used_chain_deaggregation = True
                    else:
                        # Multi-server: use solver_amvald (MATLAB solver_amva.m line 216)
                        from ...api.solvers.mva.amvald import solver_amvald, AmvaldOptions

                        SCVchain = np.ones((self._sn.nstations, self._sn.nchains))
                        refstatchain = chain_result.refstatchain

                        amvald_options = AmvaldOptions(
                            method='lin',
                            iter_tol=1e-6,
                            iter_max=1000
                        )

                        result = solver_amvald(
                            self._sn, Lchain, STchain, Vchain, alpha,
                            Nchain, SCVchain, refstatchain, amvald_options
                        )

                        has_ld_scaling = (hasattr(self._sn, 'lldscaling') and self._sn.lldscaling is not None and
                                          np.any(self._sn.lldscaling != 0))
                        has_cd_scaling = (hasattr(self._sn, 'cdscaling') and self._sn.cdscaling is not None and
                                          np.any(self._sn.cdscaling != 0))
                        Uchain_for_deagg = result.U if (has_ld_scaling or has_cd_scaling) else None

                        deagg = sn_deaggregate_chain_results(
                            self._sn, Lchain, None, STchain, Vchain, alpha,
                            None, Uchain_for_deagg, result.R, result.T, None, result.X
                        )

                        QN = deagg.Q
                        UN = deagg.U
                        RN = deagg.R
                        TN = deagg.T
                        XN = deagg.X.flatten()
                        AN = TN.copy()
                        used_chain_deaggregation = True

                elif has_class_switching_early and is_closed_network:
                    # Non-product-form closed model with class switching
                    # Uses solver_amvald with 'lin'
                    from ...api.sn import sn_get_demands_chain, sn_deaggregate_chain_results
                    from ...api.solvers.mva.amvald import solver_amvald, AmvaldOptions

                    chain_result = sn_get_demands_chain(self._sn)
                    Lchain = chain_result.Lchain
                    STchain = chain_result.STchain
                    Vchain = chain_result.Vchain
                    alpha = chain_result.alpha
                    Nchain = chain_result.Nchain.flatten()
                    refstatchain = chain_result.refstatchain
                    SCVchain = np.ones((self._sn.nstations, self._sn.nchains))

                    amvald_options = AmvaldOptions(
                        method='lin',
                        iter_tol=1e-6,
                        iter_max=1000
                    )

                    result = solver_amvald(
                        self._sn, Lchain, STchain, Vchain, alpha,
                        Nchain, SCVchain, refstatchain, amvald_options
                    )

                    has_ld_scaling = (hasattr(self._sn, 'lldscaling') and self._sn.lldscaling is not None and
                                      np.any(self._sn.lldscaling != 0))
                    has_cd_scaling = (hasattr(self._sn, 'cdscaling') and self._sn.cdscaling is not None and
                                      np.any(self._sn.cdscaling != 0))
                    Uchain_for_deagg = result.U if (has_ld_scaling or has_cd_scaling) else None

                    deagg = sn_deaggregate_chain_results(
                        self._sn, Lchain, None, STchain, Vchain, alpha,
                        None, Uchain_for_deagg, result.R, result.T, None, result.X
                    )

                    QN = deagg.Q
                    UN = deagg.U
                    RN = deagg.R
                    TN = deagg.T
                    XN = deagg.X.flatten()
                    AN = TN.copy()
                    used_chain_deaggregation = True

                elif has_class_switching_early and not has_product_form_early and not np.any(np.isinf(N)):
                    # Class switching with non-product-form CLOSED model (e.g., heterogeneous FCFS)
                    # MATLAB: For single-server models, use pfqn_linearizermx with 'egflin' at CHAIN level
                    # For multiserver models, use solver_amvald
                    # NOTE: Open networks (np.any(np.isinf(N))) should fall through to line 3879
                    from ...api.sn import sn_get_demands_chain, sn_deaggregate_chain_results

                    chain_result = sn_get_demands_chain(self._sn)
                    Lchain = chain_result.Lchain
                    STchain = chain_result.STchain
                    Vchain = chain_result.Vchain
                    alpha = chain_result.alpha
                    Nchain = chain_result.Nchain.flatten()
                    refstatchain = chain_result.refstatchain

                    # Check for single-server model (MATLAB: max(nservers)==1)
                    nservers_full = self._sn.nservers.flatten() if self._sn.nservers is not None else np.ones(self.nstations)
                    max_servers = 1
                    finite_servers = nservers_full[np.isfinite(nservers_full)]
                    if len(finite_servers) > 0:
                        max_servers = int(np.max(finite_servers))

                    C_chains = self._sn.nchains

                    if max_servers == 1:
                        # Single-server model: use pfqn_linearizermx with 'egflin' at chain level
                        # (matching MATLAB solver_amva.m lines 199-201)

                        # Build chain-level demands for queueing stations
                        # MATLAB: L = STchain .* Vchain (at chain level)
                        L_chain = STchain * Vchain  # M x C

                        # Extract queueing stations only
                        L_chain_queues = L_chain[queue_indices, :]  # Mq x C

                        # Think times from delay stations (sum of service times at delay stations)
                        delay_indices = [i for i in range(self.nstations) if i not in queue_indices]
                        Z_chain = np.zeros(C_chains)
                        for d_idx in delay_indices:
                            Z_chain += STchain[d_idx, :] * Vchain[d_idx, :]

                        # Get scheduling for queueing stations
                        sched_list = []
                        for q_idx in queue_indices:
                            sched_val = self.sched.get(q_idx, SchedStrategy.FCFS) if self.sched else SchedStrategy.FCFS
                            sched_list.append(sched_val)

                        # Server counts for queueing stations
                        S_queue = nservers_full[queue_indices]

                        # Call pfqn_linearizermx at chain level
                        lambda_chain = np.zeros(C_chains)
                        Qchain_out, Uchain_out, Wchain_out, Tchain_out, Cchain_out, Xchain_out, iters = pfqn_linearizermx(
                            lambda_chain, L_chain_queues, Nchain, Z_chain, S_queue, sched_list,
                            tol=1e-4, maxiter=1000, method='egflin'
                        )

                        # Build full station chain results
                        Qchain = np.zeros((self.nstations, C_chains))
                        Uchain = np.zeros((self.nstations, C_chains))
                        Rchain = np.zeros((self.nstations, C_chains))
                        Tchain = np.zeros((self.nstations, C_chains))
                        Xchain = Xchain_out.reshape(1, -1) if Xchain_out.ndim == 1 else Xchain_out

                        # Map queue results back to station indices
                        for idx, q_idx in enumerate(queue_indices):
                            for c in range(C_chains):
                                Qchain[q_idx, c] = Qchain_out[idx, c] if Qchain_out.ndim > 1 else Qchain_out[idx]
                                Uchain[q_idx, c] = Uchain_out[idx, c] if Uchain_out.ndim > 1 else Uchain_out[idx]

                        # Compute delay station metrics
                        for d_idx in delay_indices:
                            for c in range(C_chains):
                                Qchain[d_idx, c] = Xchain[0, c] * STchain[d_idx, c] * Vchain[d_idx, c]
                                Uchain[d_idx, c] = Qchain[d_idx, c]

                        # Compute throughputs and response times
                        for c in range(C_chains):
                            for i in range(self.nstations):
                                if Vchain[i, c] > 0 and Xchain[0, c] > 0:
                                    Tchain[i, c] = Xchain[0, c] * Vchain[i, c]
                                    if Tchain[i, c] > 0:
                                        Rchain[i, c] = Qchain[i, c] / Tchain[i, c]

                        # Disaggregate chain results to class level
                        deagg = sn_deaggregate_chain_results(
                            self._sn, Lchain, None, STchain, Vchain, alpha,
                            None, None, Rchain, Tchain, None, Xchain
                        )

                        QN = deagg.Q
                        UN = deagg.U
                        RN = deagg.R
                        TN = deagg.T
                        XN = deagg.X.flatten()
                        AN = TN.copy()
                        used_chain_deaggregation = True
                    else:
                        # Multiserver model: use solver_amvald
                        from ...api.solvers.mva.amvald import solver_amvald, AmvaldOptions

                        SCVchain = np.ones((self._sn.nstations, self._sn.nchains))
                        amvald_options = AmvaldOptions(method='default', iter_tol=self.options.tol, iter_max=self.options.max_iter)
                        result = solver_amvald(
                            self._sn, Lchain, STchain, Vchain, alpha,
                            Nchain, SCVchain, refstatchain, amvald_options
                        )

                        # Disaggregate chain results to class level
                        has_ld_scaling = (hasattr(self._sn, 'lldscaling') and self._sn.lldscaling is not None and
                                          np.any(self._sn.lldscaling != 0))
                        has_cd_scaling = (hasattr(self._sn, 'cdscaling') and self._sn.cdscaling is not None and
                                          np.any(self._sn.cdscaling != 0))
                        Uchain_for_deagg = result.U if (has_ld_scaling or has_cd_scaling) else None

                        deagg = sn_deaggregate_chain_results(
                            self._sn, Lchain, None, STchain, Vchain, alpha,
                            None, Uchain_for_deagg, result.R, result.T, None, result.X
                        )

                        QN = deagg.Q
                        UN = deagg.U
                        RN = deagg.R
                        TN = deagg.T
                        XN = deagg.X.flatten()
                        AN = TN.copy()
                        used_chain_deaggregation = True

                elif np.any(np.isinf(N)):
                    # Open or mixed network - use chain-based AMVA (solver_amvald)
                    # This handles open classes correctly with visit ratios
                    from ...api.solvers.mva.amvald import solver_amvald, AmvaldOptions
                    from ...api.sn import sn_get_demands_chain, sn_deaggregate_chain_results

                    chain_result = sn_get_demands_chain(self._sn)
                    Lchain = chain_result.Lchain
                    STchain = chain_result.STchain
                    Vchain = chain_result.Vchain
                    alpha = chain_result.alpha
                    Nchain = chain_result.Nchain.flatten()
                    refstatchain = chain_result.refstatchain
                    SCVchain = np.ones((self._sn.nstations, self._sn.nchains))

                    amvald_options = AmvaldOptions(method='default', iter_tol=self.options.tol, iter_max=self.options.max_iter)
                    result = solver_amvald(
                        self._sn, Lchain, STchain, Vchain, alpha,
                        Nchain, SCVchain, refstatchain, amvald_options
                    )

                    # Disaggregate chain results to class level
                    # MATLAB: pass Uchain only if there's load/class-dependent scaling
                    # Otherwise, let deaggregation compute U = T * S (MATLAB solver_amvald.m lines 216-220)
                    has_ld_scaling = (hasattr(self._sn, 'lldscaling') and self._sn.lldscaling is not None and
                                      np.any(self._sn.lldscaling != 0))
                    has_cd_scaling = (hasattr(self._sn, 'cdscaling') and self._sn.cdscaling is not None and
                                      np.any(self._sn.cdscaling != 0))
                    Uchain_for_deagg = result.U if (has_ld_scaling or has_cd_scaling) else None

                    deagg = sn_deaggregate_chain_results(
                        self._sn, Lchain, None, STchain, Vchain, alpha,
                        None, Uchain_for_deagg, result.R, result.T, None, result.X
                    )

                    QN = deagg.Q
                    UN = deagg.U
                    RN = deagg.R
                    TN = deagg.T
                    XN = deagg.X.flatten()
                    AN = TN.copy()
                    used_chain_deaggregation = True
                elif has_multiserver:
                    # Use solver_amvald for multi-server CLOSED networks
                    # MATLAB solver_amva.m lines 209-217: for multiserver with default/softmin/seidmann
                    # config, calls solver_amvald which handles chain aggregation, Seidmann
                    # transformation, and deaggregation internally
                    from ...api.solvers.mva.amvald import solver_amvald, AmvaldOptions
                    from ...api.sn import sn_get_demands_chain, sn_deaggregate_chain_results

                    chain_result = sn_get_demands_chain(self._sn)
                    Lchain = chain_result.Lchain
                    STchain = chain_result.STchain
                    Vchain = chain_result.Vchain
                    alpha = chain_result.alpha
                    Nchain = chain_result.Nchain.flatten()
                    refstatchain = chain_result.refstatchain
                    SCVchain = np.ones((self._sn.nstations, self._sn.nchains))

                    amvald_options = AmvaldOptions(
                        method='lin',
                        iter_tol=1e-6,
                        iter_max=1000
                    )
                    if self.options.config and 'multiserver' in self.options.config:
                        amvald_options.config = AmvaldOptions.Config(
                            multiserver=self.options.config['multiserver']
                        )

                    result = solver_amvald(
                        self._sn, Lchain, STchain, Vchain, alpha,
                        Nchain, SCVchain, refstatchain, amvald_options
                    )

                    # Disaggregate chain results to class level
                    has_ld_scaling = (hasattr(self._sn, 'lldscaling') and self._sn.lldscaling is not None and
                                      np.any(self._sn.lldscaling != 0))
                    has_cd_scaling = (hasattr(self._sn, 'cdscaling') and self._sn.cdscaling is not None and
                                      np.any(self._sn.cdscaling != 0))
                    Uchain_for_deagg = result.U if (has_ld_scaling or has_cd_scaling) else None

                    deagg = sn_deaggregate_chain_results(
                        self._sn, Lchain, None, STchain, Vchain, alpha,
                        None, Uchain_for_deagg, result.R, result.T, None, result.X
                    )

                    QN = deagg.Q
                    UN = deagg.U
                    RN = deagg.R
                    TN = deagg.T
                    XN = deagg.X.flatten()
                    AN = TN.copy()
                    used_chain_deaggregation = True
                elif not has_product_form_early:
                    # Non-product-form closed network (e.g., HOL priority)
                    # Use solver_amvald which handles priority scheduling correctly
                    from ...api.solvers.mva.amvald import solver_amvald, AmvaldOptions
                    from ...api.sn import sn_get_demands_chain, sn_deaggregate_chain_results

                    chain_result = sn_get_demands_chain(self._sn)
                    Lchain = chain_result.Lchain
                    STchain = chain_result.STchain
                    Vchain = chain_result.Vchain
                    alpha = chain_result.alpha
                    Nchain = chain_result.Nchain.flatten()
                    refstatchain = chain_result.refstatchain
                    SCVchain = np.ones((self._sn.nstations, self._sn.nchains))

                    amvald_options = AmvaldOptions(method='default', iter_tol=self.options.tol, iter_max=self.options.max_iter)
                    result = solver_amvald(
                        self._sn, Lchain, STchain, Vchain, alpha,
                        Nchain, SCVchain, refstatchain, amvald_options
                    )

                    # Disaggregate chain results to class level
                    # MATLAB: pass Uchain only if there's load/class-dependent scaling
                    # Otherwise, let deaggregation compute U = T * S (MATLAB solver_amvald.m lines 216-220)
                    has_ld_scaling = (hasattr(self._sn, 'lldscaling') and self._sn.lldscaling is not None and
                                      np.any(self._sn.lldscaling != 0))
                    has_cd_scaling = (hasattr(self._sn, 'cdscaling') and self._sn.cdscaling is not None and
                                      np.any(self._sn.cdscaling != 0))
                    Uchain_for_deagg = result.U if (has_ld_scaling or has_cd_scaling) else None

                    deagg = sn_deaggregate_chain_results(
                        self._sn, Lchain, None, STchain, Vchain, alpha,
                        None, Uchain_for_deagg, result.R, result.T, None, result.X
                    )

                    QN = deagg.Q
                    UN = deagg.U
                    RN = deagg.R
                    TN = deagg.T
                    XN = deagg.X.flatten()
                    AN = TN.copy()
                    used_chain_deaggregation = True
                else:
                    # MATLAB: for single-server closed product-form models, use egflin (linearizer)
                    # instead of AQL (Schweitzer) - see solver_amva.m lines 50-54
                    total_pop = np.sum(N[np.isfinite(N)])
                    max_servers = 1
                    if mi is not None:
                        finite_servers = mi[np.isfinite(mi)]
                        if len(finite_servers) > 0:
                            max_servers = int(np.max(finite_servers))

                    # MATLAB: if single server model, use egflin; otherwise use lin
                    if total_pop > 2 and np.all(N >= 1) and max_servers == 1:
                        # Switch to egflin method - matches MATLAB's solver_amva.m behavior
                        from ...api.pfqn import pfqn_egflinearizer

                        sched_type = []
                        for q_idx in queue_indices:
                            if self.sched is not None and q_idx in self.sched:
                                sched_type.append(str(self.sched[q_idx]))
                            else:
                                sched_type.append('FCFS')

                        # L is already indexed by queue stations (L = self.demands[queue_indices, :])
                        # So just use L directly with pfqn_egflinearizer
                        QN_out, UN_out, WN_out, TN_out, CN_out, XN_out, iter_count = pfqn_egflinearizer(
                            L, N, Z, sched_type, tol=1e-4, maxiter=1000
                        )

                        # XN_out is (1, R) or (R,), flatten to (R,)
                        XN_out = XN_out.flatten()

                        # Compute response metrics
                        RN_out = np.zeros((len(queue_indices), R))
                        AN_out = np.zeros((len(queue_indices), R))
                        for r in range(R):
                            if XN_out[r] > 0:
                                for idx in range(len(queue_indices)):
                                    if L[idx, r] > 0:
                                        RN_out[idx, r] = QN_out[idx, r] / XN_out[r]
                                        AN_out[idx, r] = XN_out[r]

                        for idx, q_idx in enumerate(queue_indices):
                            QN[q_idx, :] = QN_out[idx, :]
                            UN[q_idx, :] = UN_out[idx, :]
                            RN[q_idx, :] = RN_out[idx, :]
                            TN[q_idx, :] = TN_out[idx, :]
                            AN[q_idx, :] = AN_out[idx, :]

                        XN = XN_out.flatten()
                    else:
                        # Fall back to AQL for small populations or multiserver
                        result = pfqn_aql(L, N, Z)
                        XN_out, CN_out, QN_out, UN_out, RN_out, TN_out, AN_out = result

                        for idx, q_idx in enumerate(queue_indices):
                            QN[q_idx, :] = QN_out[idx, :]
                            UN[q_idx, :] = UN_out[idx, :]
                            RN[q_idx, :] = RN_out[idx, :]
                            TN[q_idx, :] = TN_out[idx, :]
                            AN[q_idx, :] = AN_out[idx, :]

                        XN = XN_out.flatten()

            elif method == 'bs':
                line_debug("Using bound method: %s", method, options=self.options)
                # Balanced System approximation
                XN_out, QN_out, UN_out, RN_out, _ = pfqn_bs(L, N, Z)
                TN_out = np.tile(XN_out, (QN_out.shape[0], 1))
                AN_out = TN_out.copy()

                for idx, q_idx in enumerate(queue_indices):
                    QN[q_idx, :] = QN_out[idx, :]
                    UN[q_idx, :] = UN_out[idx, :]
                    RN[q_idx, :] = RN_out[idx, :]
                    TN[q_idx, :] = TN_out[idx, :]
                    AN[q_idx, :] = AN_out[idx, :]

                XN = XN_out.flatten()

            elif method == 'sqni':
                # Single Queue Network Interpolation
                QN_out, UN_out, XN_out = pfqn_sqni(L, N, Z)
                for idx, q_idx in enumerate(queue_indices):
                    QN[q_idx, :] = QN_out[idx, :] if QN_out.ndim > 1 else QN_out
                    UN[q_idx, :] = UN_out[idx, :] if UN_out.ndim > 1 else UN_out
                XN = XN_out.flatten()

            elif method in ['lin', 'gflin', 'egflin']:
                line_debug("Standard queueing network, routing to mva_analyzer (method=%s)", method, options=self.options)
                # Linearizer family of algorithms
                # Build scheduling strategy list
                sched_type = []
                for q_idx in queue_indices:
                    if self.sched is not None and q_idx in self.sched:
                        sched_type.append(str(self.sched[q_idx]))
                    else:
                        sched_type.append('FCFS')

                # Check if this network has open classes (pure open or mixed)
                has_open_classes = np.any(np.isinf(N))
                is_mixed = has_open_classes and np.any(np.isfinite(N) & (N > 0))

                if has_open_classes:
                    # Get number of servers from model nodes
                    nservers = np.ones(len(queue_indices))
                    model_nodes = self.model.get_nodes() if hasattr(self.model, 'get_nodes') else []
                    for idx, q_idx in enumerate(queue_indices):
                        # Try to get nservers from model nodes first
                        if q_idx < len(model_nodes):
                            node = model_nodes[q_idx]
                            if hasattr(node, 'get_number_of_servers'):
                                ns = node.get_number_of_servers()
                                if ns is not None:
                                    nservers[idx] = ns
                        # Fallback to _sn.nservers if available
                        elif hasattr(self, '_sn') and self._sn is not None:
                            if hasattr(self._sn, 'nservers') and self._sn.nservers is not None:
                                if q_idx < len(self._sn.nservers):
                                    nservers[idx] = self._sn.nservers[q_idx]

                    finite_servers = nservers[np.isfinite(nservers)]
                    max_servers = int(np.max(finite_servers)) if len(finite_servers) > 0 else 1

                    # For open or mixed networks, MATLAB uses solver_amvald + sn_deaggregate
                    # which correctly computes TN with visits (TN = XN * V).
                    # pfqn_linearizermx doesn't have access to visits, so it computes TN = XN.
                    # Reference: MATLAB solver_amva.m lines 208-210, 264
                    if has_open_classes:
                        # Pure open network OR multiserver mixed network: use solver_amvald
                        # Reference: MATLAB solver_amva.m lines 208-210, 264
                        from ...api.solvers.mva.amvald import solver_amvald, AmvaldOptions
                        from ...api.sn import sn_get_demands_chain, sn_deaggregate_chain_results

                        # Get chain-level parameters
                        chain_result = sn_get_demands_chain(self._sn)
                        Lchain = chain_result.Lchain
                        STchain = chain_result.STchain
                        Vchain = chain_result.Vchain
                        alpha = chain_result.alpha
                        Nchain = chain_result.Nchain.flatten()
                        refstatchain = chain_result.refstatchain
                        SCVchain = np.ones((self._sn.nstations, self._sn.nchains))

                        # Set up options with 'lin' method
                        amvald_options = AmvaldOptions(
                            method=method,
                            iter_tol=1e-6,
                            iter_max=1000
                        )
                        if self.options.config and 'multiserver' in self.options.config:
                            amvald_options.config = AmvaldOptions.Config(
                                multiserver=self.options.config['multiserver']
                            )

                        # Call solver_amvald
                        result = solver_amvald(
                            self._sn, Lchain, STchain, Vchain, alpha,
                            Nchain, SCVchain, refstatchain, amvald_options
                        )

                        # Disaggregate chain results to class level
                        # Note: MATLAB passes empty Uchain when lldscaling/cdscaling are not set
                        # so that deaggregate computes U with /nservers division
                        has_lldscaling = hasattr(self._sn, 'lldscaling') and self._sn.lldscaling is not None
                        has_cdscaling = hasattr(self._sn, 'cdscaling') and self._sn.cdscaling is not None
                        Uchain_for_deagg = result.U if (has_lldscaling or has_cdscaling) else None
                        deagg = sn_deaggregate_chain_results(
                            self._sn, Lchain, None, STchain, Vchain, alpha,
                            None, Uchain_for_deagg, result.R, result.T, None, result.X
                        )

                        # Copy disaggregated results
                        QN = deagg.Q
                        UN = deagg.U
                        RN = deagg.R
                        TN = deagg.T
                        XN = deagg.X.flatten()
                        AN = TN.copy()
                        used_chain_deaggregation = True
                    else:
                        # Single-server mixed network: use pfqn_linearizermx directly
                        # Reference: MATLAB solver_amva.m lines 199-201

                        # Compute arrival rates for open classes
                        lambda_arr = np.zeros(R)
                        source_indices = self._get_source_stations()

                        for r in range(R):
                            if np.isinf(N[r]):
                                # Open class - get arrival rate from Source station
                                if hasattr(self, '_sn') and self._sn is not None:
                                    rates = self._sn.rates
                                    if rates is not None:
                                        # Look at Source station indices to get arrival rate
                                        for src_idx in source_indices:
                                            if src_idx < rates.shape[0] and r < rates.shape[1]:
                                                if rates[src_idx, r] > 0 and not np.isinf(rates[src_idx, r]):
                                                    lambda_arr[r] = rates[src_idx, r]
                                                    break

                                # Fallback: try to get arrival rate from model Source node
                                if lambda_arr[r] == 0 and np.any(L[:, r] > 0):
                                    if hasattr(self, 'model') and hasattr(self.model, 'get_classes'):
                                        classes = self.model.get_classes()
                                        if r < len(classes):
                                            job_class = classes[r]
                                            for node in self.model.get_nodes():
                                                if hasattr(node, 'get_arrival') and hasattr(node, '__class__'):
                                                    if 'Source' in node.__class__.__name__:
                                                        arr_dist = node.get_arrival(job_class)
                                                        if arr_dist is not None and hasattr(arr_dist, 'get_rate'):
                                                            lambda_arr[r] = arr_dist.get_rate()
                                                            break

                        QN_out, UN_out, WN_out, TN_out, CN_out, XN_out, iters = pfqn_linearizermx(
                            lambda_arr, L, N, Z, nservers, sched_type, method=method
                        )

                        for idx, q_idx in enumerate(queue_indices):
                            QN[q_idx, :] = QN_out[idx, :]
                            UN[q_idx, :] = UN_out[idx, :]
                            RN[q_idx, :] = WN_out[idx, :]  # Response times
                            TN[q_idx, :] = TN_out[idx, :]
                            AN[q_idx, :] = TN_out[idx, :]  # Arrival rate = throughput

                        XN = XN_out.flatten()
                else:
                    # Check for class switching - requires chain-level approach
                    has_class_switching_lin = False
                    if hasattr(self._sn, 'nchains') and self._sn.nchains > 0:
                        chains = self._get_chains()
                        for chain in chains:
                            if len(chain) > 1:
                                has_class_switching_lin = True
                                break

                    if has_class_switching_lin:
                        # For class-switching networks, use chain-level approach
                        # MATLAB uses chain aggregation + solver_amvald + disaggregation
                        from ...api.solvers.mva.amvald import solver_amvald, AmvaldOptions
                        from ...api.sn import sn_get_demands_chain, sn_deaggregate_chain_results

                        # Get chain-level parameters
                        chain_result = sn_get_demands_chain(self._sn)
                        Lchain = chain_result.Lchain
                        STchain = chain_result.STchain
                        Vchain = chain_result.Vchain
                        alpha = chain_result.alpha
                        Nchain = chain_result.Nchain.flatten()
                        refstatchain = chain_result.refstatchain
                        SCVchain = np.ones((self._sn.nstations, self._sn.nchains))

                        # Set up options
                        amvald_options = AmvaldOptions(
                            method=method,
                            iter_tol=1e-6,
                            iter_max=1000
                        )

                        # Call solver_amvald
                        result = solver_amvald(
                            self._sn, Lchain, STchain, Vchain, alpha,
                            Nchain, SCVchain, refstatchain, amvald_options
                        )

                        # Disaggregate chain results to class level
                        # MATLAB: pass Uchain only if there's load/class-dependent scaling
                        has_ld_scaling = (hasattr(self._sn, 'lldscaling') and self._sn.lldscaling is not None and
                                          np.any(self._sn.lldscaling != 0))
                        has_cd_scaling = (hasattr(self._sn, 'cdscaling') and self._sn.cdscaling is not None and
                                          np.any(self._sn.cdscaling != 0))
                        Uchain_for_deagg = result.U if (has_ld_scaling or has_cd_scaling) else None

                        deagg = sn_deaggregate_chain_results(
                            self._sn, Lchain, None, STchain, Vchain, alpha,
                            None, Uchain_for_deagg, result.R, result.T, None, result.X
                        )

                        # Copy disaggregated results
                        QN = deagg.Q
                        UN = deagg.U
                        RN = deagg.R
                        TN = deagg.T
                        XN = deagg.X.flatten()
                        AN = TN.copy()
                        used_chain_deaggregation = True
                    else:
                        # Use standard linearizer for closed-only networks without class switching
                        if method == 'egflin':
                            result = pfqn_egflinearizer(L, N, Z, sched_type)
                        elif method == 'gflin':
                            result = pfqn_gflinearizer(L, N, Z, sched_type)
                        else:  # lin
                            result = pfqn_linearizer(L, N, Z, sched_type)

                        QN_out, UN_out, WN_out, TN_out, CN_out, XN_out, iters = result

                        for idx, q_idx in enumerate(queue_indices):
                            QN[q_idx, :] = QN_out[idx, :]
                            UN[q_idx, :] = UN_out[idx, :]
                            RN[q_idx, :] = WN_out[idx, :]  # Response times
                            TN[q_idx, :] = TN_out[idx, :]
                            AN[q_idx, :] = TN_out[idx, :]  # Arrival rate = throughput

                        XN = XN_out.flatten()

            elif method in ['schmidt', 'schmidt-ext']:
                # Schmidt's exact MVA for multi-class networks
                # Build scheduling strategy array
                sched_arr = np.zeros(M, dtype=int)
                for idx, q_idx in enumerate(queue_indices):
                    if self.sched is not None and q_idx in self.sched:
                        sched_str = str(self.sched[q_idx])
                        if 'INF' in sched_str:
                            sched_arr[idx] = 2  # INF
                        elif 'PS' in sched_str:
                            sched_arr[idx] = 3  # PS
                        else:
                            sched_arr[idx] = 0  # FCFS
                    else:
                        sched_arr[idx] = 0  # Default FCFS

                result = pfqn_schmidt(L, N, mi, sched_arr)
                XN_out = result.XN
                QN_out = result.QN
                UN_out = result.UN

                for idx, q_idx in enumerate(queue_indices):
                    QN[q_idx, :] = QN_out[idx, :]
                    UN[q_idx, :] = UN_out[idx, :]
                    if XN_out.size > 0:
                        # Only set TN/AN for classes with demand at this station
                        XN_flat = XN_out.flatten() if hasattr(XN_out, 'flatten') else np.array([XN_out])
                        for r in range(R):
                            if idx < L.shape[0] and r < L.shape[1] and L[idx, r] > 0:
                                TN[q_idx, r] = XN_flat[r] if r < len(XN_flat) else 0
                                AN[q_idx, r] = XN_flat[r] if r < len(XN_flat) else 0

                XN = XN_out.flatten() if hasattr(XN_out, 'flatten') else np.array([XN_out])

            elif method == 'ab':
                # Akyildiz-Bolch AMVA for multi-server networks
                V = np.ones_like(L)  # Visit ratios (default 1)
                sched_arr = np.zeros(M, dtype=int)
                for idx, q_idx in enumerate(queue_indices):
                    if self.sched is not None and q_idx in self.sched:
                        sched_str = str(self.sched[q_idx])
                        if 'INF' in sched_str:
                            sched_arr[idx] = 2  # INF
                        elif 'PS' in sched_str:
                            sched_arr[idx] = 3  # PS
                        else:
                            sched_arr[idx] = 0  # FCFS
                    else:
                        sched_arr[idx] = 0  # Default FCFS

                result = pfqn_ab_amva(L, N, V, mi, sched_arr)
                QN_out = result.QN
                UN_out = result.UN
                XN_out = result.XN

                for idx, q_idx in enumerate(queue_indices):
                    QN[q_idx, :] = QN_out[idx, :]
                    UN[q_idx, :] = UN_out[idx, :]
                    RN[q_idx, :] = result.RN[idx, :]
                    # Only set TN/AN for classes with demand at this station
                    XN_flat = XN_out.flatten() if hasattr(XN_out, 'flatten') else np.array([XN_out])
                    for r in range(R):
                        if idx < L.shape[0] and r < L.shape[1] and L[idx, r] > 0:
                            TN[q_idx, r] = XN_flat[r] if r < len(XN_flat) else 0
                            AN[q_idx, r] = XN_flat[r] if r < len(XN_flat) else 0

                XN = XN_out.flatten()

            elif method == 'qd':
                # Queue-Dependent (QD) AMVA
                # Check for class/load/joint dependence - must use solver_amvald (MATLAB: solver_mvald_analyzer.m)
                has_cdscaling = hasattr(self._sn, 'cdscaling') and self._sn.cdscaling is not None
                has_lldscaling = hasattr(self._sn, 'lldscaling') and self._sn.lldscaling is not None
                has_ljdscaling = hasattr(self._sn, 'ljdscaling') and self._sn.ljdscaling is not None
                has_ljcdscaling = hasattr(self._sn, 'ljcdscaling') and self._sn.ljcdscaling is not None

                if has_cdscaling or has_lldscaling or has_ljdscaling or has_ljcdscaling:
                    # Use solver_amvald for class-dependent or load-dependent models
                    from ...api.solvers.mva.amvald import solver_amvald, AmvaldOptions
                    from ...api.sn import sn_get_demands_chain, sn_deaggregate_chain_results

                    chain_result = sn_get_demands_chain(self._sn)
                    Lchain = chain_result.Lchain
                    STchain = chain_result.STchain
                    Vchain = chain_result.Vchain
                    alpha = chain_result.alpha
                    Nchain = chain_result.Nchain.flatten()
                    refstatchain = chain_result.refstatchain
                    SCVchain = np.ones((self._sn.nstations, self._sn.nchains))

                    amvald_options = AmvaldOptions(
                        method='qd',
                        iter_tol=1e-6,
                        iter_max=1000
                    )

                    result = solver_amvald(
                        self._sn, Lchain, STchain, Vchain, alpha,
                        Nchain, SCVchain, refstatchain, amvald_options
                    )

                    # Disaggregate chain results to class level
                    Uchain_for_deagg = result.U if (has_lldscaling or has_cdscaling or has_ljdscaling or has_ljcdscaling) else None
                    deagg = sn_deaggregate_chain_results(
                        self._sn, Lchain, None, STchain, Vchain, alpha,
                        None, Uchain_for_deagg, result.R, result.T, None, result.X
                    )

                    QN = deagg.Q
                    UN = deagg.U
                    RN = deagg.R
                    TN = deagg.T
                    XN = deagg.X.flatten()
                    AN = TN.copy()
                    used_chain_deaggregation = True
                else:
                    # Standard QD for product-form networks
                    QN_out, XN_out, UN_out, iters = pfqn_qd(L, N)

                    for idx, q_idx in enumerate(queue_indices):
                        QN[q_idx, :] = QN_out[idx, :]
                        UN[q_idx, :] = UN_out[idx, :]
                        # Compute residence times from queue lengths
                        # XN_out is per-class throughput (R elements), not per-queue
                        if np.any(XN_out > 0):
                            RN[q_idx, :] = QN_out[idx, :] / np.maximum(XN_out, 1e-10)
                        else:
                            RN[q_idx, :] = L[idx, :]
                        # Only set TN/AN for classes with demand at this station
                        XN_flat = XN_out.flatten() if hasattr(XN_out, 'flatten') else np.array([XN_out])
                        for r in range(R):
                            if idx < L.shape[0] and r < L.shape[1] and L[idx, r] > 0:
                                TN[q_idx, r] = XN_flat[r] if r < len(XN_flat) else 0
                                AN[q_idx, r] = XN_flat[r] if r < len(XN_flat) else 0

                    XN = XN_out.flatten()

            elif method == 'qdlin':
                # QD-Linearizer (QDLIN) AMVA
                QN_out, UN_out, RN_out, XN_out, CN_out, iters = pfqn_qdlin(L, N, Z)

                for idx, q_idx in enumerate(queue_indices):
                    QN[q_idx, :] = QN_out[idx, :]
                    UN[q_idx, :] = UN_out[idx, :]
                    RN[q_idx, :] = RN_out[idx, :]
                    # Only set TN/AN for classes with demand at this station
                    XN_flat = XN_out.flatten() if hasattr(XN_out, 'flatten') else np.array([XN_out])
                    for r in range(R):
                        if idx < L.shape[0] and r < L.shape[1] and L[idx, r] > 0:
                            TN[q_idx, r] = XN_flat[r] if r < len(XN_flat) else 0
                            AN[q_idx, r] = XN_flat[r] if r < len(XN_flat) else 0

                XN = XN_out.flatten()

            elif method == 'qli':
                # Queue-Line (QLI) AMVA (Wang-Sevcik)
                QN_out, UN_out, RN_out, XN_out, CN_out, iters = pfqn_qli(L, N, Z)

                for idx, q_idx in enumerate(queue_indices):
                    QN[q_idx, :] = QN_out[idx, :]
                    UN[q_idx, :] = UN_out[idx, :]
                    RN[q_idx, :] = RN_out[idx, :]
                    # Only set TN/AN for classes with demand at this station
                    XN_flat = XN_out.flatten() if hasattr(XN_out, 'flatten') else np.array([XN_out])
                    for r in range(R):
                        if idx < L.shape[0] and r < L.shape[1] and L[idx, r] > 0:
                            TN[q_idx, r] = XN_flat[r] if r < len(XN_flat) else 0
                            AN[q_idx, r] = XN_flat[r] if r < len(XN_flat) else 0

                XN = XN_out.flatten()

            elif method == 'fli':
                # Fraction-Line (FLI) AMVA (Wang-Sevcik)
                QN_out, UN_out, RN_out, XN_out, CN_out, iters = pfqn_fli(L, N, Z)

                for idx, q_idx in enumerate(queue_indices):
                    QN[q_idx, :] = QN_out[idx, :]
                    UN[q_idx, :] = UN_out[idx, :]
                    RN[q_idx, :] = RN_out[idx, :]
                    # Only set TN/AN for classes with demand at this station
                    XN_flat = XN_out.flatten() if hasattr(XN_out, 'flatten') else np.array([XN_out])
                    for r in range(R):
                        if idx < L.shape[0] and r < L.shape[1] and L[idx, r] > 0:
                            TN[q_idx, r] = XN_flat[r] if r < len(XN_flat) else 0
                            AN[q_idx, r] = XN_flat[r] if r < len(XN_flat) else 0

                XN = XN_out.flatten()

            elif method == 'aba.lower':
                line_debug("Using bound method: %s", method, options=self.options)
                # Asymptotic Bound Analysis - lower bound
                X_low, Q_low = pfqn_xzabalow(L, N, Z)
                XN = X_low.flatten()
                for idx, q_idx in enumerate(queue_indices):
                    QN[q_idx, :] = Q_low[idx, :] if Q_low.ndim > 1 else Q_low
                    UN[q_idx, :] = XN * L[idx, :]

            elif method == 'aba.upper':
                line_debug("Using bound method: %s", method, options=self.options)
                # Asymptotic Bound Analysis - upper bound
                X_up, Q_up = pfqn_xzabaup(L, N, Z)
                XN = X_up.flatten()
                for idx, q_idx in enumerate(queue_indices):
                    QN[q_idx, :] = Q_up[idx, :] if Q_up.ndim > 1 else Q_up
                    UN[q_idx, :] = XN * L[idx, :]

            elif method in ['pb.lower', 'pb.upper']:
                line_debug("Using bound method: %s", method, options=self.options)
                # Performance bounds - use ABA as fallback
                if 'lower' in method:
                    X_low, Q_low = pfqn_xzabalow(L, N, Z)
                    XN = X_low.flatten()
                else:
                    X_up, Q_up = pfqn_xzabaup(L, N, Z)
                    XN = X_up.flatten()

                for idx, q_idx in enumerate(queue_indices):
                    UN[q_idx, :] = XN * L[idx, :]
                    QN[q_idx, :] = UN[q_idx, :]  # Approximate

            elif method in ['gb.lower', 'gb.upper']:
                line_debug("Using bound method: %s", method, options=self.options)
                # Geometric Bounds (single-class closed networks)
                if 'lower' in method:
                    X_bound, Q_bound = pfqn_qzgblow(L, N, Z)
                else:
                    X_bound, Q_bound = pfqn_qzgbup(L, N, Z)
                XN = X_bound.flatten() if hasattr(X_bound, 'flatten') else np.array([X_bound])
                for idx, q_idx in enumerate(queue_indices):
                    if Q_bound.ndim > 1:
                        QN[q_idx, :] = Q_bound[idx, :]
                    else:
                        QN[q_idx, :] = Q_bound
                    UN[q_idx, :] = XN * L[idx, :]

            elif method in ['sb.lower', 'sb.upper']:
                line_debug("Using bound method: %s", method, options=self.options)
                # Schweitzer Bounds (single-class closed networks)
                if 'lower' in method:
                    X_bound, Q_bound = pfqn_xzgsblow(L, N, Z)
                else:
                    X_bound, Q_bound = pfqn_xzgsbup(L, N, Z)
                XN = X_bound.flatten() if hasattr(X_bound, 'flatten') else np.array([X_bound])
                for idx, q_idx in enumerate(queue_indices):
                    if Q_bound.ndim > 1:
                        QN[q_idx, :] = Q_bound[idx, :]
                    else:
                        QN[q_idx, :] = Q_bound
                    UN[q_idx, :] = XN * L[idx, :]

            else:
                # Default to exact MVA
                result = pfqn_mva(L, N, Z, mi)
                XN_out, CN_out, QN_out, UN_out, RN_out, TN_out, AN_out = result

                for idx, q_idx in enumerate(queue_indices):
                    QN[q_idx, :] = QN_out[idx, :]
                    UN[q_idx, :] = UN_out[idx, :]
                    RN[q_idx, :] = RN_out[idx, :]
                    TN[q_idx, :] = TN_out[idx, :]
                    AN[q_idx, :] = AN_out[idx, :]

                XN = XN_out.flatten()

        elif self.network_type == 'open':
            line_debug("Single-class open queueing system (Source-Queue-Sink), routing to qsys_analyzer", options=self.options)
            # Open network - use QNA or queueing system formulas
            from ...api.qsys import (
                qsys_mm1, qsys_mmk, qsys_mg1, qsys_gm1,
                qsys_gig1_approx_kingman, qsys_gig1_approx_gelenbe,
                qsys_gig1_approx_heyman, qsys_gig1_approx_kimura,
                qsys_gig1_approx_kobayashi, qsys_gig1_approx_klb,
                qsys_gig1_approx_marchal,
                qsys_gig1_approx_allencunneen, qsys_gigk_approx,
                qsys_gig1_ubnd_kingman, qsys_gigk_approx_kingman,
            )

            # Get arrival rate and service rate for single queue
            lambda_r = 1.0  # Default arrival rate
            mu = 1.0  # Default service rate
            k = 1  # Number of servers

            for r in range(R):
                if not np.isfinite(self.njobs[r]):  # Open class
                    # Get arrival rate from source station (use source_indices, not just "not in queue_indices")
                    for src_idx in source_indices:
                        if self.rates[src_idx, r] > 0:
                            lambda_r = self.rates[src_idx, r]
                            break

                    for idx, q_idx in enumerate(queue_indices):
                        if L[idx, r] > 0:
                            mu = 1.0 / L[idx, r]
                            # Handle infinity (infinite servers) - treat as single server for analysis
                            nserv = mi[idx] if idx < len(mi) else 1.0
                            k = 1 if not np.isfinite(nserv) else int(nserv)

            # Handle specific queueing system methods
            qsys_methods = {
                'mm1', 'mmk', 'mg1', 'mgi1', 'gm1', 'gig1', 'gim1',
                'gig1.kingman', 'gigk', 'gigk.kingman_approx',
                'gig1.gelenbe', 'gig1.heyman', 'gig1.kimura',
                'gig1.allen', 'gig1.kobayashi', 'gig1.klb', 'gig1.marchal',
            }

            if method in qsys_methods and M == 1 and R == 1:
                # Single queue, single class - use exact formulas
                # Matches MATLAB solver_mva_qsys_analyzer.m pattern:
                #   ca = sqrt(sn.scv(source_ist)); cs = sqrt(sn.scv(queue_ist));
                #   R = qsys_*(...); then Q = X*R; U = lambda/mu/k
                sn_scv = self._sn.scv if self._sn.scv is not None else np.ones((self.nstations, R))
                ca = np.sqrt(sn_scv[source_indices[0], 0]) if len(source_indices) > 0 and sn_scv[source_indices[0], 0] > 0 else 1.0
                cs = np.sqrt(sn_scv[queue_indices[0], 0]) if sn_scv[queue_indices[0], 0] > 0 else 1.0

                # Method selection with default resolution (matches MATLAB)
                if method == 'default':
                    if ca == 1.0 and cs == 1.0 and k == 1:
                        method = 'mm1'
                    elif ca == 1.0 and cs == 1.0 and k > 1:
                        method = 'mmk'
                    elif ca == 1.0 and k == 1:
                        method = 'mg1'
                    elif cs == 1.0 and k == 1:
                        method = 'gm1'
                    elif k > 1:
                        method = 'gigk'
                    else:
                        method = 'gig1.klb'

                if method == 'exact':
                    if ca == 1.0 and cs == 1.0 and k == 1:
                        method = 'mm1'
                    elif ca == 1.0 and cs == 1.0 and k > 1:
                        method = 'mmk'
                    elif ca == 1.0 and k == 1:
                        method = 'mg1'
                    elif cs == 1.0 and k == 1:
                        method = 'gm1'

                Rscalar = None

                if method == 'mm1':
                    result = qsys_mm1(lambda_r, mu)
                    Rscalar = result['W']

                elif method == 'mmk':
                    result = qsys_mmk(lambda_r, mu, k)
                    Rscalar = result['W']

                elif method in ['mg1', 'mgi1']:
                    Rscalar = qsys_mg1(lambda_r, mu, cs)['W']

                elif method in ['gm1', 'gim1']:
                    # MATLAB: sigma = fzero(@(x) LA(mu-mu*x)-x, 0.5) using LST
                    # Simplified: use rho as sigma approximation
                    sigma = lambda_r / mu
                    Rscalar = 1.0 / ((1 - sigma) * mu)

                elif method in ['gigk']:
                    Rscalar, _ = qsys_gigk_approx(lambda_r, mu, ca, cs, k)

                elif method in ['gigk.kingman_approx']:
                    Rscalar, _ = qsys_gigk_approx_kingman(lambda_r, mu, ca, cs, k)

                elif method in ['gig1', 'gig1.kingman']:
                    Rscalar, _ = qsys_gig1_ubnd_kingman(lambda_r, mu, ca, cs)

                elif method == 'gig1.heyman':
                    Rscalar, _ = qsys_gig1_approx_heyman(lambda_r, mu, ca, cs)

                elif method == 'gig1.allen':
                    Rscalar, _ = qsys_gig1_approx_allencunneen(lambda_r, mu, ca, cs)

                elif method == 'gig1.kobayashi':
                    Rscalar, _ = qsys_gig1_approx_kobayashi(lambda_r, mu, ca, cs)

                elif method == 'gig1.klb':
                    Rscalar, _ = qsys_gig1_approx_klb(lambda_r, mu, ca, cs)

                elif method == 'gig1.marchal':
                    Rscalar, _ = qsys_gig1_approx_marchal(lambda_r, mu, ca, cs)

                elif method == 'gig1.gelenbe':
                    Rscalar = qsys_gig1_approx_gelenbe(lambda_r, mu, ca, cs)

                elif method == 'gig1.kimura':
                    Rscalar = qsys_gig1_approx_kimura(lambda_r, mu, ca, cs)

                # Compute Q, U, T, X from R (matches MATLAB pattern)
                if Rscalar is not None:
                    qi = queue_indices[0]
                    RN[qi, 0] = Rscalar
                    XN[0] = lambda_r
                    UN[qi, 0] = lambda_r / mu / k
                    TN[qi, 0] = lambda_r
                    AN[qi, 0] = lambda_r
                    QN[qi, 0] = XN[0] * RN[qi, 0]
                    if len(source_indices) > 0:
                        TN[source_indices[0], 0] = lambda_r

            else:
                # Default: use M/M/k formulas for each queue (multiserver support)
                for r in range(R):
                    if not np.isfinite(self.njobs[r]):  # Open class (njobs = inf)
                        # Get arrival rate from source station (use source_indices, not just "not in queue_indices")
                        lambda_r = 1.0
                        for src_idx in source_indices:
                            if self.rates[src_idx, r] > 0:
                                lambda_r = self.rates[src_idx, r]
                                break

                        for idx, q_idx in enumerate(queue_indices):
                            mu = 1.0 / L[idx, r] if L[idx, r] > 0 else float('inf')
                            # Get number of servers for this queue
                            k = int(mi[idx]) if idx < len(mi) and np.isfinite(mi[idx]) and mi[idx] > 0 else 1
                            # For M/M/k: rho_total = lambda/(k*mu), utilization per server
                            rho_k = lambda_r / (k * mu) if mu > 0 else 0  # Utilization per server

                            if rho_k < 1:
                                # M/M/k queue length approximation
                                # For k=1: reduces to M/M/1 formula rho/(1-rho)
                                # For k>1: use approximation Q = rho_k/(1-rho_k) + rho_total
                                # where rho_total = k * rho_k = lambda/mu
                                rho_total = k * rho_k  # Total offered load
                                if k == 1:
                                    QN[q_idx, r] = rho_k / (1 - rho_k)  # M/M/1 queue length
                                else:
                                    # M/M/k approximation: jobs in queue + jobs in service
                                    # Using simple approximation: Q ≈ rho_k/(1-rho_k) * Pk + rho_total
                                    # where Pk is probability all servers busy (Erlang-C)
                                    # Simplified: Q ≈ rho_total + rho_k/(1-rho_k) for moderate loads
                                    from ...api.qsys import qsys_mmk
                                    result = qsys_mmk(lambda_r, mu, k)
                                    QN[q_idx, r] = result.get('L', rho_total / (1 - rho_k))
                                    RN[q_idx, r] = result.get('W', L[idx, r] / (1 - rho_k))

                                UN[q_idx, r] = rho_k  # Per-server utilization
                                if RN[q_idx, r] == 0:
                                    RN[q_idx, r] = L[idx, r] / (1 - rho_k) if rho_k < 1 else float('inf')
                                TN[q_idx, r] = lambda_r
                                AN[q_idx, r] = lambda_r

                            XN[r] = lambda_r

        # Compute Delay station metrics from throughput and individual station demands
        # For infinite server (delay) stations: QN = X*D, UN = QN, RN = ST (service time)
        # NOTE: Skip this for methods that already computed Delay metrics correctly via handler
        # (e.g., 'exact'/'mva' with chain-based approach, or 'amva' with class-switching)
        skip_delay_recompute = (method in ['exact', 'mva']) and self.network_type != 'open'
        # Also skip for amva with class switching since it uses chain-level disaggregation
        # which already computes correct class-level response times
        if method == 'amva' and has_class_switching_early:
            skip_delay_recompute = True
        # Skip delay recompute if chain-level deaggregation was used (TN already correct)
        if used_chain_deaggregation:
            skip_delay_recompute = True
        if not skip_delay_recompute:
            delay_stations = self._get_delay_stations()
            for d_idx in delay_stations:
                for r in range(R):
                    d_demand = self.demands[d_idx, r]  # Individual station's demand = visits * service_time
                    # Get service time (1/rate), not demand
                    d_service_time = 1.0 / self.rates[d_idx, r] if self.rates[d_idx, r] > 0 else 0.0
                    if d_demand > 0 and XN[r] > 0:
                        QN[d_idx, r] = XN[r] * d_demand  # Little's law: jobs = throughput * demand
                        UN[d_idx, r] = QN[d_idx, r]  # For infinite servers, utilization = queue length
                        RN[d_idx, r] = d_service_time  # Response time = service time (NOT demand)
                        TN[d_idx, r] = XN[r]  # Throughput
                        AN[d_idx, r] = XN[r]  # Arrival rate

        # Compute residence times if not set
        source_stations = self._get_source_stations()
        for r in range(R):
            if XN[r] > 0:
                for i in range(self.nstations):
                    if RN[i, r] == 0 and QN[i, r] > 0:
                        RN[i, r] = QN[i, r] / XN[r]
                    # Only set TN/AN for stations with non-zero demand (not Disabled services)
                    if self.demands[i, r] > 0:
                        if TN[i, r] == 0:
                            TN[i, r] = XN[r]
                        # Don't set AN for Source stations (jobs originate there, not arrive)
                        if AN[i, r] == 0 and i not in source_stations:
                            AN[i, r] = XN[r]

        # Set TN for Source stations to arrival rate
        # AN (arrival rate) at Source is 0 because jobs originate there, not arrive
        for src_idx in source_stations:
            for r in range(R):
                if self.rates[src_idx, r] > 0:
                    TN[src_idx, r] = self.rates[src_idx, r]
                    AN[src_idx, r] = 0.0  # Jobs originate at source, not arrive

        # Compute fork-join synchronization delays if model has fork-join
        if self._has_fork_join():
            line_debug("Fork-join post-processing: computing sync delays", options=self.options)
            self._compute_fork_join_sync_delays(QN, UN, RN, TN, AN, XN)

        # Compute and store cache hit/miss probabilities
        self._compute_cache_hit_miss_probs(XN)

        # Filter TN to set 0 where class doesn't visit station (visit ratio = 0)
        # This fixes cases where TN is incorrectly set at stations not visited by a class
        from ...constants import GlobalConstants
        if hasattr(self._sn, 'visits') and self._sn.visits:
            for chain_id, visits in self._sn.visits.items():
                if isinstance(visits, np.ndarray):
                    for ist in range(min(TN.shape[0], self._sn.nstations)):
                        sf_idx = int(self._sn.stationToStateful[ist]) if ist < len(self._sn.stationToStateful) else -1
                        if sf_idx >= 0 and sf_idx < visits.shape[0]:
                            for r in range(min(TN.shape[1], visits.shape[1])):
                                # Check chain membership - only filter if class r is in this chain
                                if hasattr(self._sn, 'chains') and self._sn.chains is not None:
                                    r_chain = get_chain_for_class(self._sn.chains, r)
                                    if r_chain == chain_id:
                                        if visits[sf_idx, r] < GlobalConstants.Zero:
                                            TN[ist, r] = 0.0
                                            AN[ist, r] = 0.0

        # Compute arrival rates from throughputs using routing matrix
        # MATLAB runAnalyzer.m line 341: AN = sn_get_arvr_from_tput(sn, TN, self.getAvgTputHandles())
        from ...api.sn.getters import sn_get_arvr_from_tput
        AN = sn_get_arvr_from_tput(self._sn, TN, TN)  # Pass TN as TH (non-empty array)

        # Compute residence times from response times (WN = RN * V)
        from ...api.sn.transforms import sn_get_residt_from_respt
        WN = sn_get_residt_from_respt(self._sn, RN, None)

        # Store results
        runtime = time.time() - start_time
        self._result = {
            'QN': QN,
            'UN': UN,
            'RN': RN,
            'TN': TN,
            'AN': AN,
            'XN': XN,
            'WN': WN,
            'runtime': runtime,
            'method': method,
        }

        # Restore Cache nodetype if it was converted to ClassSwitch during solving
        # (MATLAB restores via getStruct() which returns fresh sn; Python needs explicit restore)
        if hasattr(self, '_cache_indices') and self._cache_indices:
            from ...api.sn.network_struct import NodeType
            for ind in self._cache_indices:
                self._sn.nodetype[ind] = NodeType.CACHE

        # Print completion message (matches MATLAB verbose guard)
        if self.options.verbose:
            py_version = f"{sys.version_info.major}.{sys.version_info.minor}.{sys.version_info.micro}"
            print(f"MVA analysis [method: {method}, lang: python, env: {py_version}] completed in {runtime:.6f}s.")

        return self

    def getAvgTable(self) -> pd.DataFrame:
        """
        Get comprehensive average performance metrics table.

        Returns node-based results (one row per node per class) to match MATLAB output format.
        Non-station nodes (e.g., Fork) are included with zero metrics.

        Returns:
            pandas.DataFrame with columns: Station, JobClass, QLen, Util, RespT, ResidT, ArvR, Tput
        """
        if self._result is None:
            self.runAnalyzer()

        # Compute residence times from response times using visit ratios
        from ...api.sn.transforms import sn_get_residt_from_respt
        from ...api.sn.network_struct import NodeType
        WN = sn_get_residt_from_respt(self._sn, self._result['RN'], None)

        # NOTE: WN is correctly computed by sn_get_residt_from_respt using visit=1
        # for fork branches. A Fork with arrival rate lambda sends lambda to EACH
        # branch, so the visit ratio at each branch is 1, not 1/num_branches.
        # No post-processing is needed for fork-join networks.

        # Get node-based dimensions and mappings from NetworkStruct
        nnodes = self._sn.nnodes if hasattr(self._sn, 'nnodes') else self.nstations
        nodeToStation = np.asarray(self._sn.nodeToStation).flatten() if hasattr(self._sn, 'nodeToStation') else np.arange(self.nstations)
        nodenames = list(self._sn.nodenames) if hasattr(self._sn, 'nodenames') and self._sn.nodenames else []

        # Build table data - iterate over nodes (not stations) to match MATLAB format
        rows = []
        for node_idx in range(nnodes):
            station_idx = int(nodeToStation[node_idx]) if node_idx < len(nodeToStation) else -1
            node_name = nodenames[node_idx] if node_idx < len(nodenames) else f'Node{node_idx}'

            for r in range(self.nclasses):
                class_name = self.class_names[r] if r < len(self.class_names) else f'Class{r}'

                # If this node is a station, use station results; otherwise use zeros
                if station_idx >= 0 and station_idx < self.nstations:
                    rows.append({
                        'Station': node_name,
                        'JobClass': class_name,
                        'QLen': self._result['QN'][station_idx, r],
                        'Util': self._result['UN'][station_idx, r],
                        'RespT': self._result['RN'][station_idx, r],
                        'ResidT': WN[station_idx, r],
                        'ArvR': self._result['AN'][station_idx, r],
                        'Tput': self._result['TN'][station_idx, r],
                    })
                else:
                    # Non-station node (e.g., Fork) - include with zeros
                    rows.append({
                        'Station': node_name,
                        'JobClass': class_name,
                        'QLen': 0.0,
                        'Util': 0.0,
                        'RespT': 0.0,
                        'ResidT': 0.0,
                        'ArvR': 0.0,
                        'Tput': 0.0,
                    })

        df = pd.DataFrame(rows)

        # Filter out near-zero rows (MATLAB: any(sum([QN,UN,RN,TN,AN,WN])>0))
        # Use tolerance to avoid keeping rows with only numerical noise (e.g., 8e-18)
        numeric_cols = ['QLen', 'Util', 'RespT', 'ResidT', 'ArvR', 'Tput']
        row_sums = df[numeric_cols].abs().sum(axis=1)
        tokeep = row_sums > 1e-14
        df = df.loc[tokeep].reset_index(drop=True)

        # Wrap in IndexedTable for consistent formatting
        result = IndexedTable(df)

        if not self._table_silent:
            print(result)

        return result

    # Python-style alias

    # ============================================================================
    # Probability Methods (Phase 2)
    # ============================================================================

    def getProbAggr(self, ist: int) -> Tuple[float, float]:
        """
        Get probability of current per-class job distribution at a station.

        Returns P(n_1, ..., n_K at station i) using binomial approximation.

        Args:
            ist: Station index (1-based, MATLAB style)

        Returns:
            (log_prob, prob): Tuple of log probability and probability value

        Raises:
            ValueError: If station index invalid or analysis not run

        Example:
            >>> solver = SolverMVA(model)
            >>> solver.runAnalyzer()
            >>> log_p, p = solver.getProbAggr(1)  # Station 1
        """
        from ...api.solvers.mva.prob_methods import get_prob_aggr

        if self._result is None:
            self.runAnalyzer()

        # Create minimal SolverResults compatible object
        class ResultAdapter:
            def __init__(self, result_dict):
                self.Q = result_dict.get('QN')
                self.U = result_dict.get('UN')
                self.R = result_dict.get('RN')
                self.prob = None

        result_adapter = ResultAdapter(self._result)
        return get_prob_aggr(self._get_network_struct(), result_adapter, ist)

    def getProbMarg(
        self, ist: int, jobclass: int, state_m: Optional[np.ndarray] = None
    ) -> Tuple[np.ndarray, np.ndarray]:
        """
        Get marginal queue-length distribution for a class at a station.

        Returns P(n | station i, class r) for n = 0, 1, ..., N[r].

        Args:
            ist: Station index (1-based)
            jobclass: Job class index (1-based)
            state_m: Optional state vector (for future use)

        Returns:
            (states, probs): Array of state indices and probabilities
                - states: [0, 1, ..., N[jobclass]]
                - probs: Probability distribution summing to 1.0

        Example:
            >>> states, probs = solver.getProbMarg(1, 1)
            >>> print(f"P(n=2 at station 1, class 1) = {probs[2]}")
        """
        from ...api.solvers.mva.prob_methods import get_prob_marg

        if self._result is None:
            self.runAnalyzer()

        class ResultAdapter:
            def __init__(self, result_dict):
                self.Q = result_dict.get('QN')
                self.prob = None

        result_adapter = ResultAdapter(self._result)
        return get_prob_marg(self._get_network_struct(), result_adapter, ist, jobclass)

    def getProbSysAggr(self) -> Tuple[float, float]:
        """
        Get joint probability of current system state.

        Returns P(full system state) using product of station marginals.

        Returns:
            (log_prob, prob): Log probability and probability value

        Notes:
            - Assumes station independence (valid for product-form networks)
            - Requires network state to be set

        Example:
            >>> log_p, p = solver.getProbSysAggr()
            >>> print(f"System state probability: {p:.6e}")
        """
        from ...api.solvers.mva.prob_methods import get_prob_sys_aggr

        if self._result is None:
            self.runAnalyzer()

        class ResultAdapter:
            def __init__(self, result_dict):
                self.Q = result_dict.get('QN')
                self.prob = None

        result_adapter = ResultAdapter(self._result)
        return get_prob_sys_aggr(self._get_network_struct(), result_adapter)

    def getProbNormConstAggr(self) -> float:
        """
        Get log normalizing constant for closed queueing network.

        Returns log(G) where G = ∑_state P(state).

        Returns:
            log_G: Natural logarithm of normalizing constant
                - For open networks: returns inf
                - For closed networks: exact value from MVA or approximation

        Notes:
            - Only applies to closed queueing networks
            - Returns inf for open networks (normalizing constant is infinite)

        Example:
            >>> log_G = solver.getProbNormConstAggr()
            >>> G = np.exp(log_G)  # Reconstruct if needed
        """
        from ...api.solvers.mva.prob_methods import get_prob_norm_const_aggr

        if self._result is None:
            self.runAnalyzer()

        class ResultAdapter:
            def __init__(self, result_dict):
                self.Q = result_dict.get('QN')
                self.prob = None

        result_adapter = ResultAdapter(self._result)
        return get_prob_norm_const_aggr(self._get_network_struct(), result_adapter)

    def _get_network_struct(self):
        """Get or create network structure for probability computations."""
        class NetworkStructAdapter:
            def __init__(self, solver):
                self.nstations = solver.nstations
                self.nclasses = solver.nclasses
                self.njobs = solver.njobs
                self.nservers = solver.nservers
                self.nodetype = getattr(solver, 'nodetype', None)
                self.refstat = getattr(solver, 'refstat', None)
                self.state = None  # Will be set if needed
                self.sched = getattr(solver, 'sched', None)

        return NetworkStructAdapter(self)

    # ============================================================================
    # Individual Metric Accessors (Phase 3)
    # ============================================================================

    def getAvgQLen(self) -> np.ndarray:
        """
        Get average queue lengths.

        Returns:
            Q: Queue lengths matrix (M x K)
               M = number of stations
               K = number of classes
               Q[i,r] = average number of jobs of class r at station i

        Raises:
            RuntimeError: If solver not run yet

        Example:
            >>> Q = solver.getAvgQLen()
            >>> print(f"Queue length at station 1, class 1: {Q[0,0]}")
        """
        if self._result is None:
            self.runAnalyzer()
        return self._result['QN'].copy()

    def getAvgUtil(self) -> np.ndarray:
        """
        Get average utilizations.

        Returns:
            U: Utilization matrix (M x K)
               U[i,r] = utilization of station i by class r
               Range: [0, 1] for single-server, [0, ∞) for multi-server

        Example:
            >>> U = solver.getAvgUtil()
            >>> print(f"Utilization at station 1: {U[0,:].sum()}")
        """
        if self._result is None:
            self.runAnalyzer()
        return self._result['UN'].copy()

    def getAvgRespT(self) -> np.ndarray:
        """
        Get average response times.

        Returns:
            R: Response times matrix (M x K)
               R[i,r] = average time spent at station i for class r
               Includes both service and waiting time

        Example:
            >>> R = solver.getAvgRespT()
            >>> print(f"Response time at station 1, class 1: {R[0,0]}")
        """
        if self._result is None:
            self.runAnalyzer()
        return self._result['RN'].copy()

    def getAvgResidT(self) -> np.ndarray:
        """
        Get average residence times (M x K).

        Residence time is computed from response time using visit ratios:
        WN[ist,k] = RN[ist,k] * V[ist,k] / V[refstat,refclass]

        Returns:
            ResidT: Residence times matrix (M x K)
        """
        if self._result is None:
            self.runAnalyzer()

        # Compute ResidT using proper visit ratios from network structure
        if self._sn is not None and self._sn.visits:
            return sn_get_residt_from_respt(self._sn, self._result['RN'], None)
        else:
            # Fallback: ResidT = RespT (no visit information available)
            return self._result['RN'].copy()

    def getAvgWaitT(self) -> np.ndarray:
        """
        Get average waiting times.

        Returns:
            W: Waiting times matrix (M x K)
               W[i,r] = R[i,r] - S[i,r]
               where S[i,r] is the mean service time
               W[i,r] = 0 for Delay (think time) stations

        Example:
            >>> W = solver.getAvgWaitT()
            >>> print(f"Waiting time at station 1: {W[0,:].sum()}")
        """
        if self._result is None:
            self.runAnalyzer()

        # W = R - S, where S is the service demand (1/service_rate)
        R = self._result['RN'].copy()
        S = self.demands.copy()  # Service demands already computed in __init__

        W = R - S
        # Ensure non-negative (numerical precision)
        W = np.maximum(W, 0.0)
        return W

    def getAvgTput(self) -> np.ndarray:
        """
        Get average throughputs.

        Returns:
            T: Throughput matrix (M x K)
               T[i,r] = average throughput at station i for class r
               jobs/time unit

        Example:
            >>> T = solver.getAvgTput()
            >>> print(f"Throughput at station 1, class 1: {T[0,0]}")
        """
        if self._result is None:
            self.runAnalyzer()
        return self._result['TN'].copy()

    def getAvgArvR(self) -> np.ndarray:
        """
        Get average arrival rates.

        Returns:
            A: Arrival rates matrix (M x K)
               A[i,r] = arrival rate to station i for class r

        Note:
            For closed networks, arrival rates are derived from throughputs
            and visit ratios
        """
        if self._result is None:
            self.runAnalyzer()
        return self._result['AN'].copy()

    def getAvgSysRespT(self) -> np.ndarray:
        """
        Get system response times (cycle times).

        Returns:
            C: Cycle times vector (K,)
               C[r] = average time for one cycle for class r
               (in closed networks: time between successive visits to reference station)

        Note:
            For closed networks: uses Little's Law C = N/X
            For open networks: sum of response times across all stations
        """
        if self._result is None:
            self.runAnalyzer()

        R = self._result['RN']
        X = self._result['XN']
        nclasses = R.shape[1]
        C = np.zeros(nclasses)

        for r in range(nclasses):
            if np.isfinite(self.njobs[r]):
                # Closed class: use Little's Law (matching MATLAB getAvgSys.m line 135)
                if X[r] > 0:
                    C[r] = self.njobs[r] / X[r]
                else:
                    C[r] = np.inf
            else:
                # Open class: sum of response times across all stations
                C[r] = np.sum(R[:, r])

        return C

    def getAvgSysTput(self) -> np.ndarray:
        """
        Get system throughputs.

        Returns:
            X: System throughput vector (K,)
               X[r] = overall throughput of the system for class r

        Note:
            This is the throughput at any single bottleneck station
        """
        if self._result is None:
            self.runAnalyzer()
        return self._result['XN'].copy()

    # ============================================================================
    # Unified Metrics and Chain/Node/System Methods
    # ============================================================================

    def getAvg(self) -> Tuple[np.ndarray, np.ndarray, np.ndarray, np.ndarray, np.ndarray, np.ndarray]:
        """Get all average metrics at once.

        Returns:
            Tuple of (Q, U, R, T, A, W) where:
            - Q: Queue lengths (M x K)
            - U: Utilizations (M x K)
            - R: Response times (M x K)
            - T: Throughputs (M x K)
            - A: Arrival rates (M x K)
            - W: Waiting times (M x K)
        """
        if self._result is None:
            self.runAnalyzer()

        Q = self._result['QN']
        U = self._result['UN']
        R = self._result['RN']
        T = self._result['TN']  # Use station throughputs, not broadcast system throughput
        A = T.copy()
        W = self._result.get('WN', R.copy())  # Use WN (residence times), not RN (response times)

        return Q, U, R, T, A, W

    def _get_chains(self) -> List[List[int]]:
        """Get chain-to-class mapping from network structure."""
        if hasattr(self, '_sn') and self._sn is not None and hasattr(self._sn, 'chains') and self._sn.chains is not None:
            chains_arr = np.asarray(self._sn.chains)
            if chains_arr.size == 0:
                return [[k] for k in range(self.nclasses)]

            nchains = self._sn.nchains if hasattr(self._sn, 'nchains') else 1

            # Check if chains is 1D (class->chain mapping) or 2D (chain,class membership)
            if chains_arr.ndim == 1:
                # 1D format: chains[k] = c means class k belongs to chain c
                chains = [[] for _ in range(nchains)]
                for k in range(self.nclasses):
                    if k < len(chains_arr):
                        c = int(chains_arr[k])
                        if 0 <= c < nchains:
                            chains[c].append(k)
                return chains if any(chains) else [[k for k in range(self.nclasses)]]
            else:
                # 2D format: chains[c, k] > 0 means class k is in chain c
                chains = []
                for c in range(nchains):
                    chain_classes = []
                    for k in range(self.nclasses):
                        if c < chains_arr.shape[0] and k < chains_arr.shape[1] and chains_arr[c, k] > 0:
                            chain_classes.append(k)
                    chains.append(chain_classes)
                return chains if chains else [[k for k in range(self.nclasses)]]
        else:
            return [[k] for k in range(self.nclasses)]

    def getAvgQLenChain(self) -> np.ndarray:
        """Get average queue lengths aggregated by chain."""
        if self._result is None:
            self.runAnalyzer()

        Q = self._result['QN']
        chains = self._get_chains()
        nchains = len(chains)

        QN_chain = np.zeros((self.nstations, nchains))
        for c, chain_classes in enumerate(chains):
            if chain_classes:
                QN_chain[:, c] = np.sum(Q[:, chain_classes], axis=1)

        # Clean up tiny values (numerical noise) to exactly 0
        QN_chain[np.abs(QN_chain) < 1e-10] = 0.0
        return QN_chain

    def getAvgUtilChain(self) -> np.ndarray:
        """Get average utilizations aggregated by chain.

        Cleans up tiny numerical values (< 1e-10) to exactly 0.
        """
        if self._result is None:
            self.runAnalyzer()

        U = self._result['UN']
        chains = self._get_chains()
        nchains = len(chains)

        UN_chain = np.zeros((self.nstations, nchains))
        for c, chain_classes in enumerate(chains):
            if chain_classes:
                UN_chain[:, c] = np.sum(U[:, chain_classes], axis=1)

        # Clean up tiny values (numerical noise) to exactly 0
        UN_chain[np.abs(UN_chain) < 1e-10] = 0.0
        return UN_chain

    def getAvgRespTChain(self) -> np.ndarray:
        """Get average response times aggregated by chain.

        Uses alpha-weighted sum matching MATLAB: RN(:,c) = sum(RNclass(:,inchain).*alpha(:,inchain),2)
        """
        if self._result is None:
            self.runAnalyzer()

        R = self._result['RN']
        chains = self._get_chains()
        nchains = len(chains)

        RN_chain = np.zeros((self.nstations, nchains))

        # Get alpha weights from sn_get_demands_chain
        if hasattr(self, '_sn') and self._sn is not None:
            from ...api.sn.demands import sn_get_demands_chain
            try:
                demands = sn_get_demands_chain(self._sn)
                alpha = demands.alpha

                for c, chain_classes in enumerate(chains):
                    if chain_classes:
                        # Weighted sum: sum(R[:, inchain] * alpha[:, inchain], axis=1)
                        RN_chain[:, c] = np.sum(R[:, chain_classes] * alpha[:, chain_classes], axis=1)
                # Clean up tiny values (numerical noise) to exactly 0
                RN_chain[np.abs(RN_chain) < 1e-10] = 0.0
                return RN_chain
            except Exception:
                pass

        # Fallback: use simple mean if alpha computation fails
        for c, chain_classes in enumerate(chains):
            if chain_classes:
                RN_chain[:, c] = np.mean(R[:, chain_classes], axis=1)

        # Clean up tiny values (numerical noise) to exactly 0
        RN_chain[np.abs(RN_chain) < 1e-10] = 0.0
        return RN_chain

    def getAvgResidTChain(self) -> np.ndarray:
        """Get average residence times aggregated by chain.

        Residence time accounts for visit ratios, computed as:
        WN(i,c) = sum(WNclass(i, inchain))
        where WNclass = sn_get_residt_from_respt converts response times to residence times.
        """
        if self._result is None:
            self.runAnalyzer()

        R = self._result['RN']  # Per-class response times
        chains = self._get_chains()
        nchains = len(chains)

        WN_chain = np.zeros((self.nstations, nchains))

        # Try to compute proper residence times using visit ratios
        if hasattr(self, '_sn') and self._sn is not None:
            from ...api.sn.transforms import sn_get_residt_from_respt
            try:
                # Compute per-class residence times from response times
                WN = sn_get_residt_from_respt(self._sn, R, None)

                # Aggregate by chain (sum per-class residence times)
                for c, chain_classes in enumerate(chains):
                    if chain_classes:
                        WN_chain[:, c] = np.sum(WN[:, chain_classes], axis=1)
                # Clean up tiny values (numerical noise) to exactly 0
                WN_chain[np.abs(WN_chain) < 1e-10] = 0.0
                return WN_chain
            except Exception:
                pass

        # Fallback: return response times if residence time computation fails
        # (getAvgRespTChain already has cleanup)
        return self.getAvgRespTChain()

    def getAvgTputChain(self) -> np.ndarray:
        """Get average throughputs aggregated by chain.

        Sums per-station throughputs for all classes in chain:
        TN(:,c) = sum(TNclass(:, inchain), 2)
        """
        if self._result is None:
            self.runAnalyzer()

        # Use per-station throughputs TN, not system throughput XN
        TN = self._result['TN']
        chains = self._get_chains()
        nchains = len(chains)

        TN_chain = np.zeros((self.nstations, nchains))
        for c, chain_classes in enumerate(chains):
            if chain_classes:
                # Sum per-station throughputs for classes in this chain
                TN_chain[:, c] = np.sum(TN[:, chain_classes], axis=1)

        # Clean up tiny values (numerical noise) to exactly 0
        TN_chain[np.abs(TN_chain) < 1e-10] = 0.0
        return TN_chain

    def getAvgArvRChain(self) -> np.ndarray:
        """Get average arrival rates aggregated by chain.

        For most stations, arrival rate equals throughput at steady state.
        For Source nodes, arrival rate is 0 (jobs don't arrive TO a source,
        they depart FROM it).
        """
        AN = self.getAvgTputChain()

        # Set arrival rate to 0 for Source nodes
        if hasattr(self, 'station_types') and self.station_types:
            for i, node_type in enumerate(self.station_types):
                if node_type is not None and node_type == NodeType.SOURCE:
                    AN[i, :] = 0.0

        return AN

    def getAvgChain(self) -> Tuple[np.ndarray, np.ndarray, np.ndarray, np.ndarray, np.ndarray, np.ndarray]:
        """Get all average metrics aggregated by chain."""
        QN = self.getAvgQLenChain()
        UN = self.getAvgUtilChain()
        RN = self.getAvgRespTChain()
        WN = self.getAvgResidTChain()
        AN = self.getAvgArvRChain()
        TN = self.getAvgTputChain()
        return QN, UN, RN, WN, AN, TN

    def getAvgChainTable(self) -> pd.DataFrame:
        """Get average metrics by chain as DataFrame."""
        QN, UN, RN, WN, AN, TN = self.getAvgChain()

        nstations, nchains = QN.shape
        rows = []

        # Get station names (use actual names if available)
        station_names = getattr(self, 'station_names', None)
        if station_names is None or len(station_names) != nstations:
            station_names = [f'Station{i}' for i in range(nstations)]

        for i in range(nstations):
            for c in range(nchains):
                rows.append({
                    'Station': station_names[i],
                    'Chain': f'Chain{c + 1}',  # 1-based to match MATLAB
                    'QLen': QN[i, c],
                    'Util': UN[i, c],
                    'RespT': RN[i, c],
                    'ResidT': WN[i, c],
                    'ArvR': AN[i, c],
                    'Tput': TN[i, c],
                })

        return pd.DataFrame(rows)

    def getAvgNode(self) -> Tuple[np.ndarray, np.ndarray, np.ndarray, np.ndarray, np.ndarray, np.ndarray]:
        """
        Get average metrics per node.

        Unlike getAvg() which returns station-level metrics, this method
        returns node-level metrics including non-station nodes (e.g., Cache).
        For Cache nodes, hit/miss class throughputs are computed using
        actual hit/miss probabilities.

        Returns:
            Tuple of (QNn, UNn, RNn, WNn, ANn, TNn) - node-level metrics
        """
        from ...api.sn.getters import sn_get_node_arvr_from_tput, sn_get_node_tput_from_tput
        from ...api.sn import NodeType

        if self._result is None:
            self.runAnalyzer()

        TN = self._result['TN']
        AN = self._result['AN']
        XN = self._result['XN']

        sn = self._sn
        I = sn.nnodes
        M = sn.nstations
        R = sn.nclasses

        # Compute actual hit/miss probabilities for Cache nodes
        # Skip if already computed by _run_cache_analysis (which handles replacement strategy properly)
        if sn.nodeparam is not None:
            for ind in range(I):
                if sn.nodetype is not None and ind < len(sn.nodetype):
                    if sn.nodetype[ind] == NodeType.CACHE and ind in sn.nodeparam:
                        cache_param = sn.nodeparam[ind]

                        # Check if hit/miss probs already computed by _run_cache_analysis
                        existing_hit = getattr(cache_param, 'actualhitprob', None)
                        if existing_hit is not None and np.any(existing_hit > 0):
                            # Already computed by _run_cache_analysis, skip
                            continue

                        nitems = getattr(cache_param, 'nitems', 0)
                        cap = getattr(cache_param, 'cap', 0)

                        # Get nitems and cap from model if not in param
                        if nitems == 0 or cap == 0:
                            if hasattr(self, 'model') and hasattr(self.model, '_nodes'):
                                for node in self.model._nodes:
                                    if hasattr(node, '_num_items') and hasattr(node, '_item_level_cap'):
                                        nitems = node._num_items if node._num_items else 0
                                        item_cap = node._item_level_cap
                                        if item_cap is not None and (isinstance(item_cap, (list, np.ndarray)) and len(item_cap) > 0):
                                            cap = item_cap[0]
                                        elif item_cap is not None:
                                            cap = item_cap
                                        else:
                                            cap = 0
                                        break

                        if nitems > 0 and cap > 0:
                            # Get cache node for proper cache analysis
                            cache_node = None
                            if hasattr(self, 'model') and hasattr(self.model, '_nodes'):
                                cache_node = self.model._nodes[ind]

                            # Default: uniform access formula
                            hit_prob = min(cap / nitems, 1.0)
                            miss_prob = 1.0 - hit_prob

                            # Use proper cache analysis if gamma matrix available
                            if cache_node is not None and hasattr(cache_node, 'get_gamma_matrix'):
                                try:
                                    from ...api.cache import cache_xi_fp
                                    gamma = cache_node.get_gamma_matrix(R)
                                    m_levels = cache_node._item_level_cap if hasattr(cache_node, '_item_level_cap') else np.array([cap])

                                    # Use FPI method which works for large caches
                                    xi, pi0, pij, it = cache_xi_fp(gamma, m_levels)

                                    access_probs = np.sum(gamma, axis=1)
                                    access_probs = access_probs / np.sum(access_probs) if np.sum(access_probs) > 0 else access_probs
                                    hit_prob = np.sum(access_probs * (1 - pi0))
                                    miss_prob = 1.0 - hit_prob
                                except Exception:
                                    pass  # Fall back to uniform

                            hitclass = getattr(cache_param, 'hitclass', np.array([]))
                            nclasses = len(hitclass) if hasattr(hitclass, '__len__') else R
                            cache_param.actualhitprob = np.zeros(nclasses)
                            cache_param.actualmissprob = np.zeros(nclasses)

                            for k in range(nclasses):
                                h = hitclass[k] if k < len(hitclass) else -1
                                missclass = getattr(cache_param, 'missclass', np.array([]))
                                m = missclass[k] if k < len(missclass) else -1
                                if h >= 0 and m >= 0:
                                    cache_param.actualhitprob[k] = hit_prob
                                    cache_param.actualmissprob[k] = miss_prob

                            # Set result on Cache node in model
                            if cache_node is not None:
                                if hasattr(cache_node, 'set_result_hit_prob'):
                                    cache_node.set_result_hit_prob(cache_param.actualhitprob)
                                if hasattr(cache_node, 'set_result_miss_prob'):
                                    cache_node.set_result_miss_prob(cache_param.actualmissprob)

        # Create TH (throughput handle) - indicates which station-classes have valid throughput
        # TH > 0 means the station-class has a valid throughput value
        TH = np.zeros_like(TN)
        TH[TN > 0] = 1.0

        # Compute node arrival rates and throughputs using helper functions
        ANn = sn_get_node_arvr_from_tput(sn, TN, TH, AN)
        TNn = sn_get_node_tput_from_tput(sn, TN, TH, ANn)

        # Initialize other node-level metrics
        QNn = np.zeros((I, R))
        UNn = np.zeros((I, R))
        RNn = np.zeros((I, R))
        WNn = np.zeros((I, R))

        # Copy station metrics to station nodes
        QN = self._result['QN']
        UN = self._result['UN']
        RN = self._result['RN']

        # Compute residence times from response times using visit ratios
        from ...api.sn.transforms import sn_get_residt_from_respt
        WN = sn_get_residt_from_respt(sn, RN, None)

        for ist in range(M):
            ind = sn.stationToNode[ist]
            if ind >= 0 and ind < I:
                QNn[ind, :] = QN[ist, :]
                UNn[ind, :] = UN[ist, :]
                RNn[ind, :] = RN[ist, :]
                WNn[ind, :] = WN[ist, :]

        # Fix arrival rates for ClassSwitch and Sink nodes for cache hit/miss classes
        # (matches MATLAB getAvgNode.m lines 54-76)
        from ...api.sn.network_struct import NodeType
        for cacheInd in range(I):
            if sn.nodetype is not None and cacheInd < len(sn.nodetype) and sn.nodetype[cacheInd] == NodeType.CACHE:
                if sn.nodeparam is not None and cacheInd in sn.nodeparam:
                    cache_param = sn.nodeparam[cacheInd]
                    hitclass = np.atleast_1d(getattr(cache_param, 'hitclass', np.array([]))).flatten().astype(int)
                    missclass = np.atleast_1d(getattr(cache_param, 'missclass', np.array([]))).flatten().astype(int)
                    for ind in range(I):
                        if sn.nodetype[ind] == NodeType.CLASSSWITCH or sn.nodetype[ind] == NodeType.SINK:
                            for classIdx in range(R):
                                if np.any(hitclass[hitclass >= 0] == classIdx):
                                    ANn[ind, classIdx] = TNn[cacheInd, classIdx]
                                if np.any(missclass[missclass >= 0] == classIdx):
                                    ANn[ind, classIdx] = TNn[cacheInd, classIdx]

        return QNn, UNn, RNn, WNn, ANn, TNn

    def getAvgNodeTable(self) -> pd.DataFrame:
        """
        Get average metrics by node as DataFrame.

        Returns node-based results (one row per node per class) including
        non-station nodes like Cache. For Cache nodes, hit/miss class
        throughputs are computed using actual hit/miss probabilities.

        Returns:
            pandas.DataFrame with columns: Node, JobClass, QLen, Util, RespT, ResidT, ArvR, Tput
        """
        QNn, UNn, RNn, WNn, ANn, TNn = self.getAvgNode()

        sn = self._sn
        nodenames = list(sn.nodenames) if hasattr(sn, 'nodenames') and sn.nodenames else []

        rows = []
        for node_idx in range(sn.nnodes):
            node_name = nodenames[node_idx] if node_idx < len(nodenames) else f'Node{node_idx}'

            for r in range(sn.nclasses):
                class_name = self.class_names[r] if r < len(self.class_names) else f'Class{r}'

                # Filter out all-zero rows
                if abs(QNn[node_idx, r]) < 1e-10 and abs(UNn[node_idx, r]) < 1e-10 and \
                   abs(RNn[node_idx, r]) < 1e-10 and abs(ANn[node_idx, r]) < 1e-10 and abs(TNn[node_idx, r]) < 1e-10:
                    continue

                rows.append({
                    'Node': node_name,
                    'JobClass': class_name,
                    'QLen': QNn[node_idx, r],
                    'Util': UNn[node_idx, r],
                    'RespT': RNn[node_idx, r],
                    'ResidT': WNn[node_idx, r],
                    'ArvR': ANn[node_idx, r],
                    'Tput': TNn[node_idx, r],
                })

        df = pd.DataFrame(rows)

        if not self._table_silent:
            print(df.to_string(index=False))

        return df

    def getAvgNodeChain(self) -> Tuple[np.ndarray, np.ndarray, np.ndarray, np.ndarray, np.ndarray, np.ndarray]:
        """Get average metrics by node and chain."""
        return self.getAvgChain()

    def getAvgNodeChainTable(self) -> pd.DataFrame:
        """Get average metrics by node and chain as DataFrame."""
        return self.getAvgChainTable()

    def getAvgNodeQLenChain(self) -> np.ndarray:
        """Get average queue lengths by node aggregated by chain."""
        return self.getAvgQLenChain()

    def getAvgNodeUtilChain(self) -> np.ndarray:
        """Get average utilizations by node aggregated by chain."""
        return self.getAvgUtilChain()

    def getAvgNodeRespTChain(self) -> np.ndarray:
        """Get average response times by node aggregated by chain."""
        return self.getAvgRespTChain()

    def getAvgNodeResidTChain(self) -> np.ndarray:
        """Get average residence times by node aggregated by chain."""
        return self.getAvgResidTChain()

    def getAvgNodeTputChain(self) -> np.ndarray:
        """Get average throughputs by node aggregated by chain."""
        return self.getAvgTputChain()

    def getAvgNodeArvRChain(self) -> np.ndarray:
        """Get average arrival rates by node aggregated by chain."""
        return self.getAvgArvRChain()

    def getAvgSys(self) -> Tuple[np.ndarray, np.ndarray]:
        """Get system-level average metrics."""
        R = self.getAvgSysRespT()
        T = self.getAvgSysTput()
        return R, T

    def getAvgSysTable(self) -> pd.DataFrame:
        """Get system-level metrics as DataFrame."""
        R, T = self.getAvgSys()
        nclasses = len(R) if hasattr(R, '__len__') else 1

        rows = []
        for k in range(nclasses):
            rows.append({
                'Class': f'Class{k}',
                'SysRespT': R[k] if hasattr(R, '__getitem__') else R,
                'SysTput': T[k] if k < len(T) else 0,
            })

        return pd.DataFrame(rows)

    # ============================================================================
    # Metric Accessor Aliases
    # ============================================================================

    # PascalCase aliases for MATLAB compatibility
    GetAvgQLen = getAvgQLen
    GetAvgUtil = getAvgUtil
    GetAvgRespT = getAvgRespT
    GetAvgResidT = getAvgResidT
    GetAvgWaitT = getAvgWaitT
    GetAvgTput = getAvgTput
    GetAvgArvR = getAvgArvR
    GetAvgSysRespT = getAvgSysRespT
    GetAvgSysTput = getAvgSysTput

    # ============================================================================
    # CDF and Percentile Analysis (Phase 4)
    # ============================================================================

    def getCdfRespT(self, R: Optional[np.ndarray] = None) -> List[Dict]:
        """
        Get response time cumulative distribution function (CDF).

        Uses exponential approximation: CDF(t) = 1 - exp(-t / E[R])
        where E[R] is the mean response time from MVA.

        Args:
            R: Optional response times matrix (M x K)
               If None, uses results from runAnalyzer()

        Returns:
            RD: List of dicts, one per (station, class) pair with service
                Each dict contains:
                - 'station': Station index (1-based)
                - 'class': Job class (1-based)
                - 't': Time points (100 points from 0.001 to 0.999 quantile)
                - 'p': CDF values at each time point

        Notes:
            - Uses exponential approximation for single-class exponential service
            - For multi-phase or non-exponential, this is approximate
            - Returns empty list for stations with zero response time

        Example:
            >>> cdf_list = solver.getCdfRespT()
            >>> for cdf_data in cdf_list:
            ...     station = cdf_data['station']
            ...     class_id = cdf_data['class']
            ...     print(f"Station {station}, Class {class_id}")
            ...     # Access t and p arrays for plotting
            ...     t = cdf_data['t']
            ...     p = cdf_data['p']
        """
        if self._result is None:
            self.runAnalyzer()

        if R is None:
            R = self._result['RN']

        RD = []  # Result list

        for i in range(self.nstations):
            for r in range(self.nclasses):
                mean_resp_t = R[i, r]

                # Skip if no response time (no service at this station)
                if mean_resp_t <= 0:
                    continue

                # Exponential CDF: F(t) = 1 - exp(-t/mean)
                # Rate parameter: lambda = 1 / mean_resp_t
                lambda_rate = 1.0 / mean_resp_t

                # Generate time points from quantiles
                # Use quantiles 0.001 to 0.999 to avoid singularities
                quantiles = np.linspace(0.001, 0.999, 100)
                times = -np.log(1 - quantiles) / lambda_rate

                # Compute CDF values
                cdf_vals = 1 - np.exp(-lambda_rate * times)

                # Store result
                RD.append({
                    'station': i + 1,  # 1-based indexing
                    'class': r + 1,    # 1-based indexing
                    't': times,
                    'p': cdf_vals,
                })

        return RD

    def getPerctRespT(
        self,
        percentiles: Optional[List[float]] = None,
        jobclass: Optional[int] = None,
    ) -> Tuple[List[Dict], pd.DataFrame]:
        """
        Extract percentiles from response time distribution.

        Computes percentile points from the exponential CDF approximation.

        Args:
            percentiles: List of percentiles to extract (0-100)
                        Default: [10, 25, 50, 75, 90, 95, 99]
            jobclass: Optional class index to filter results (1-based)
                     If None, returns all classes

        Returns:
            (PercRT, PercTable): Tuple of:
            - PercRT: List of dicts with percentile data for each (station, class)
              Each dict contains:
                - 'station': Station index
                - 'class': Job class
                - 'percentiles': Input percentile values
                - 'values': Percentile response times
            - PercTable: pandas DataFrame with columns:
              Station, Class, P10, P25, P50, P75, P90, P95, P99, ...

        Algorithm:
            For exponential CDF with rate λ = 1/E[R]:
            Percentile p: t_p = -ln(1-p) * E[R]
            where p is in [0,1]

        Notes:
            - Percentiles outside (0,100) are clipped
            - Returns empty lists for stations with zero response time
            - Results match exponential percentile formula

        Example:
            >>> perc_list, perc_table = solver.getPerctRespT([90, 95, 99])
            >>> print(perc_table)
            >>> # Extract 90th percentile response time
            >>> p90_values = perc_list[0]['values']
        """
        if percentiles is None:
            percentiles = [10, 25, 50, 75, 90, 95, 99]

        # Normalize percentiles to [0, 100]
        percentiles = np.asarray(percentiles)
        percentiles = np.clip(percentiles, 0.01, 99.99)
        percentiles_normalized = percentiles / 100.0  # Convert to [0, 1]

        # Get CDFs
        cdf_list = self.getCdfRespT()

        PercRT = []
        rows = []

        # Create column names for DataFrame
        perc_col_names = [f'P{int(p)}' for p in percentiles]

        for cdf_data in cdf_list:
            station = cdf_data['station']
            class_id = cdf_data['class']

            # Only include specified class if filter is set
            if jobclass is not None and class_id != jobclass:
                continue

            # Extract mean response time
            mean_resp_t = self._result['RN'][station - 1, class_id - 1]

            if mean_resp_t <= 0:
                continue

            # Compute percentile values using exponential formula
            # For exponential: t_p = -ln(1-p) * E[R]
            lambda_rate = 1.0 / mean_resp_t
            perc_values = -np.log(1 - percentiles_normalized) / lambda_rate

            # Store in result list
            PercRT.append({
                'station': station,
                'class': class_id,
                'percentiles': percentiles.tolist(),
                'values': perc_values.tolist(),
            })

            # Add row to DataFrame
            row_data = {
                'Station': self.station_names[station - 1] if station - 1 < len(self.station_names) else f'Station{station}',
                'Class': self.class_names[class_id - 1] if class_id - 1 < len(self.class_names) else f'Class{class_id}',
            }
            for perc_col, perc_val in zip(perc_col_names, perc_values):
                row_data[perc_col] = perc_val

            rows.append(row_data)

        # Create DataFrame
        if rows:
            PercTable = pd.DataFrame(rows)
        else:
            PercTable = pd.DataFrame()

        return PercRT, PercTable

    # ============================================================================
    # Method Introspection (Phase 5)
    # ============================================================================

    def listValidMethods(self) -> List[str]:
        """
        List all valid solution methods for this network model.

        Returns the set of applicable methods based on network characteristics
        (single-class/multi-class, open/closed, etc.).

        Returns:
            List of method names available for this model:
            - Base methods: 'default', 'mva', 'exact', 'amva', 'qna'
            - AMVA variants: 'bs', 'sqni', 'lin', 'gflin', 'egflin', 'schmidt', 'schmidt-ext', 'ab'
            - Bounds (closed networks): 'aba.lower', 'aba.upper', 'pb.lower', 'pb.upper'
            - Additional bounds (single-class closed): 'bjb.upper', 'bjb.lower', 'gb.upper', 'gb.lower', 'sb.upper', 'sb.lower'
            - Queueing formulas (2-station open): 'mm1', 'mmk', 'mg1', 'mgi1', 'gm1', 'gig1', etc.

        Notes:
            - MVA solver always supports 'default' and 'exact' methods
            - AMVA (approximate MVA) available for most networks
            - Bounds methods useful for single-class networks
            - All methods also available with 'amva.' prefix (e.g., 'amva.lin')

        Example:
            >>> methods = solver.listValidMethods()
            >>> print(f"Available methods: {methods}")
            >>> # Can then call: solver.method = 'amva.lin'
        """
        methods = []

        # Base methods - always available
        methods.extend(['default', 'mva', 'exact'])

        # AMVA and variants - available for closed/mixed networks
        methods.extend([
            'amva',
            'bs', 'amva.bs',
            'sqni',
            'lin', 'amva.lin',
            'gflin',
            'egflin',
            'schmidt', 'amva.schmidt',
            'schmidt-ext', 'amva.schmidt-ext',
            'ab', 'amva.ab',
            'qd', 'amva.qd',
            'qdlin', 'amva.qdlin',
            'qli', 'amva.qli',
            'fli', 'amva.fli',
        ])

        # QNA for open networks
        if self.network_type == 'open':
            methods.append('qna')

        # Bounds for closed networks
        if self.network_type == 'closed':
            methods.extend(['aba.lower', 'aba.upper'])

            # Single-class only bounds
            if self.nclasses == 1:
                methods.extend([
                    'bjb.upper', 'bjb.lower',
                    'gb.upper', 'gb.lower',
                    'sb.upper', 'sb.lower',
                ])

        # Performance bounds - always available
        methods.extend(['pb.lower', 'pb.upper'])

        # Queueing system formulas for 2-station open networks
        if self.network_type == 'open' and self.nstations == 2 and self.nclasses == 1:
            methods.extend([
                'mm1', 'mmk', 'mg1', 'mgi1', 'gm1', 'gig1', 'gim1',
                'gig1.kingman', 'gigk', 'gigk.kingman_approx',
                'gig1.gelenbe', 'gig1.heyman', 'gig1.kimura',
                'gig1.allen', 'gig1.kobayashi', 'gig1.klb', 'gig1.marchal',
            ])

        return methods

    @staticmethod
    def getFeatureSet() -> set:
        """
        Get set of features supported by MVA solver.

        Returns:
            Set of feature strings indicating what the solver supports:
            - Node types: 'Sink', 'Source', 'Queue', 'Delay', 'Fork', 'Join'
            - Distributions: 'Exp', 'Det', 'Erlang', 'HyperExp', 'PH', 'MAP', etc.
            - Classes: 'OpenClass', 'ClosedClass'
            - Scheduling: 'FCFS', 'PS', 'LCFS', 'SIRO'
            - Special: 'MultiServer', 'LoadDependent', 'ThinkTime'

        Example:
            >>> features = SolverMVA.getFeatureSet()
            >>> if 'MultiServer' in features:
            ...     print("Multi-server queues supported")
        """
        features = {
            # Node types
            'Sink', 'Source', 'Queue', 'Delay',
            'Fork', 'Join', 'Router', 'ClassSwitch',
            # Distributions
            'Exp', 'Det', 'Erlang', 'HyperExp',
            'Gamma', 'Lognormal', 'Pareto', 'Uniform', 'Weibull',
            'PH', 'APH', 'Cox', 'MAP', 'MMPP',
            'Poisson', 'Geometric', 'Binomial', 'NegBinomial',
            # Classes
            'OpenClass', 'ClosedClass',
            # Scheduling
            'FCFS', 'PS', 'LCFS', 'SIRO', 'LIFO',
            # Special features
            'MultiServer', 'LoadDependent', 'ThinkTime',
            'Multiclass', 'ProductForm',
        }
        return features

    @staticmethod
    def supports(model) -> bool:
        """
        Check if MVA solver supports the given network model.

        Performs basic model validation to ensure compatibility.

        Args:
            model: Network model to check (native or wrapper)

        Returns:
            True if model is supported, False otherwise

        Notes:
            - Checks for product-form network structure
            - Verifies presence of required network components
            - Returns True for most standard queueing networks

        Example:
            >>> if SolverMVA.supports(model):
            ...     solver = SolverMVA(model)
            ... else:
            ...     print("Model not supported by MVA")
        """
        try:
            # Try to extract basic network properties
            if hasattr(model, 'nstations'):
                nstations = model.nstations
            elif hasattr(model, 'getNumberOfStations'):
                nstations = model.getNumberOfStations()
            else:
                return False

            if hasattr(model, 'nclasses'):
                nclasses = model.nclasses
            elif hasattr(model, 'getNumberOfClasses'):
                nclasses = model.getNumberOfClasses()
            else:
                return False

            # Basic validation
            return nstations > 0 and nclasses > 0

        except Exception:
            return False

    @staticmethod
    def defaultOptions() -> Dict[str, Any]:
        """
        Get default solver options.

        Returns:
            Dictionary with default option values:
            - 'method': 'exact' (exact MVA algorithm)
            - 'tol': 1e-8 (convergence tolerance)
            - 'max_iter': 1000 (maximum iterations)
            - 'verbose': False (quiet by default)

        Example:
            >>> opts = SolverMVA.defaultOptions()
            >>> opts['method'] = 'amva'  # Override for approximate MVA
            >>> solver = SolverMVA(model, **opts)
        """
        return OptionsDict({
            'method': 'exact',
            'tol': 1e-8,
            'max_iter': 1000,
            'verbose': False,
        })

    # ============================================================================
    # Sampling and Transient Methods (Phase 6) - Placeholders
    # ============================================================================

    def sample(self, node: int, numEvents: int) -> np.ndarray:
        """
        Sample from the response time distribution.

        **Not supported by MVA solver** - MVA is an analytical solver.
        For sampling, use simulation-based solvers.

        Args:
            node: Node/station index (1-based)
            numEvents: Number of samples to generate

        Returns:
            NotImplementedError (sampling not supported)

        Raises:
            NotImplementedError: Always - MVA does not support sampling

        Recommendation:
            Use SolverSSA (Stochastic State-space Analysis) or SolverJMT
            (JMT simulator) for sampling-based analysis.

        Example:
            >>> # Instead of sampling from MVA:
            >>> # solver = SolverMVA(model)
            >>> # This will raise NotImplementedError
            >>> solver.sample(1, 1000)
        """
        raise NotImplementedError(
            "Sampling not supported by SolverMVA (analytical solver). "
            "Use SolverSSA, SolverLDES, or SolverJMT for sampling-based analysis."
        )

    def sampleAggr(self, node: int, numEvents: int) -> np.ndarray:
        """Aggregate sampling (not supported by MVA)."""
        raise NotImplementedError(
            "sampleAggr() not supported by SolverMVA. "
            "Use simulation-based solvers (SSA, LDES, JMT)."
        )

    def sampleSys(self, numEvents: int) -> np.ndarray:
        """System-level sampling (not supported by MVA)."""
        raise NotImplementedError(
            "sampleSys() not supported by SolverMVA. "
            "Use simulation-based solvers (SSA, LDES, JMT)."
        )

    def sampleSysAggr(self, numEvents: int) -> np.ndarray:
        """Aggregate system-level sampling (not supported by MVA)."""
        raise NotImplementedError(
            "sampleSysAggr() not supported by SolverMVA. "
            "Use simulation-based solvers (SSA, LDES, JMT)."
        )

    def getCdfPassT(self, R: Optional[np.ndarray] = None) -> List[Dict]:
        """
        Get passage time CDF (not supported by MVA).

        Passage time = time to reach target station from source.
        Not computed by analytical MVA solver.

        Args:
            R: Optional response times (ignored)

        Raises:
            NotImplementedError: Passage time analysis not available

        Recommendation:
            Use simulation-based solvers for detailed path analysis.
        """
        raise NotImplementedError(
            "getCdfPassT() not supported by SolverMVA. "
            "Passage time analysis requires simulation-based solvers."
        )

    def getTranCdfRespT(self, R: Optional[np.ndarray] = None) -> List[Dict]:
        """
        Get transient response time CDF (not supported by MVA).

        Transient analysis (time-dependent) not available from steady-state MVA.

        Args:
            R: Optional response times (ignored)

        Raises:
            NotImplementedError: Transient analysis not available

        Recommendation:
            Use SolverCTMC (Markov chain) or simulation solvers for transient.
        """
        raise NotImplementedError(
            "getTranCdfRespT() not supported by SolverMVA. "
            "Transient analysis available via SolverCTMC or SolverLDES."
        )

    def getTranCdfPassT(self, R: Optional[np.ndarray] = None) -> List[Dict]:
        """Transient passage time CDF (not supported by MVA)."""
        raise NotImplementedError(
            "getTranCdfPassT() not supported by SolverMVA. "
            "Use simulation-based or CTMC solvers for transient analysis."
        )

    def getTranAvg(self) -> np.ndarray:
        """
        Get transient average metrics (not supported by MVA).

        MVA computes only steady-state metrics.

        Raises:
            NotImplementedError: Transient analysis not available
        """
        raise NotImplementedError(
            "getTranAvg() not supported by SolverMVA. "
            "MVA computes steady-state metrics only. "
            "Use SolverCTMC for transient analysis."
        )

    # ============================================================================
    # Introspection and Sampling Aliases
    # ============================================================================

    # PascalCase aliases for MATLAB compatibility
    ListValidMethods = listValidMethods
    GetFeatureSet = getFeatureSet
    Supports = supports
    DefaultOptions = defaultOptions
    Sample = sample
    SampleAggr = sampleAggr
    SampleSys = sampleSys
    SampleSysAggr = sampleSysAggr
    GetCdfPassT = getCdfPassT
    GetTranCdfRespT = getTranCdfRespT
    GetTranCdfPassT = getTranCdfPassT
    GetTranAvg = getTranAvg

    # ============================================================================
    # CDF and Percentile Aliases
    # ============================================================================

    # PascalCase aliases for MATLAB compatibility
    GetCdfRespT = getCdfRespT
    GetPerctRespT = getPerctRespT

    # ============================================================================
    # Aliases and Compatibility
    # ============================================================================

    # PascalCase aliases for MATLAB compatibility
    GetProbAggr = getProbAggr
    GetProbMarg = getProbMarg
    GetProbSysAggr = getProbSysAggr
    GetProbNormConstAggr = getProbNormConstAggr

    # Table aliases
    getAvgT = getAvgTable
    avgT = getAvgTable
    aT = getAvgTable
    default_options = defaultOptions

    # Chain-level aliases
    GetAvg = getAvg
    GetAvgChain = getAvgChain
    GetAvgChainTable = getAvgChainTable
    GetAvgQLenChain = getAvgQLenChain
    GetAvgUtilChain = getAvgUtilChain
    GetAvgRespTChain = getAvgRespTChain
    GetAvgResidTChain = getAvgResidTChain
    GetAvgTputChain = getAvgTputChain
    GetAvgArvRChain = getAvgArvRChain

    # Node-level aliases
    GetAvgNode = getAvgNode
    GetAvgNodeTable = getAvgNodeTable
    GetAvgNodeChain = getAvgNodeChain
    GetAvgNodeChainTable = getAvgNodeChainTable
    GetAvgNodeQLenChain = getAvgNodeQLenChain
    GetAvgNodeUtilChain = getAvgNodeUtilChain
    GetAvgNodeRespTChain = getAvgNodeRespTChain
    GetAvgNodeResidTChain = getAvgNodeResidTChain
    GetAvgNodeTputChain = getAvgNodeTputChain
    GetAvgNodeArvRChain = getAvgNodeArvRChain
    GetAvgSys = getAvgSys
    GetAvgSysTable = getAvgSysTable

    # Short aliases (MATLAB compatibility)
    aNT = getAvgNodeTable
    aCT = getAvgChainTable
    aNCT = getAvgNodeChainTable
    aST = getAvgSysTable
    nodeAvgT = getAvgNodeTable
    chainAvgT = getAvgChainTable
    nodeChainAvgT = getAvgNodeChainTable
    sysAvgT = getAvgSysTable

    # Snake case aliases
    avg_node_table = getAvgNodeTable
    avg_chain_table = getAvgChainTable
    avg_node_chain_table = getAvgNodeChainTable
    avg_sys_table = getAvgSysTable
    avg_qlen = getAvgQLen
    avg_util = getAvgUtil
    avg_respt = getAvgRespT
    avg_resid_t = getAvgResidT
    avg_wait_t = getAvgWaitT
    avg_tput = getAvgTput
    avg_arv_r = getAvgArvR
    avg_sys_resp_t = getAvgSysRespT
    avg_sys_tput = getAvgSysTput
    run_analyzer = runAnalyzer
    cdf_resp_t = getCdfRespT
    perct_resp_t = getPerctRespT


__all__ = ['SolverMVA', 'SolverMVAOptions']
