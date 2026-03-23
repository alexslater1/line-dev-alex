"""
Native Python implementation of NC (Normalizing Constant) solver.

This implementation uses pure Python/NumPy algorithms from the api.solvers.nc
module, providing the same functionality as the JPype wrapper without .
"""

import numpy as np
import pandas as pd
from typing import Optional, Dict, Any, List, Tuple
from dataclasses import dataclass

from ...api.sn.transforms import sn_get_residt_from_respt
from ...api.sn.getters import sn_get_arvr_from_tput
from ...api.io.logging import line_debug
from ..base import NetworkSolver


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
class SolverNCOptions:
    """Options for the native NC solver."""
    method: str = 'default'
    tol: float = 1e-6
    iter_max: int = 1000
    iter_tol: float = 1e-4
    verbose: bool = False
    seed: int = 1  # Random seed (for compatibility, NC is deterministic)
    keep: bool = False  # Keep intermediate data (for compatibility)
    cutoff: Optional[int] = None  # State space cutoff (for compatibility)
    samples: Optional[int] = None  # Samples (for compatibility)


class SolverNC(NetworkSolver):
    """
    Native Python NC (Normalizing Constant) solver.

    This solver analyzes product-form queueing networks using normalizing
    constant computation methods in pure Python/NumPy, providing the same
    functionality as the Java wrapper without requiring the JVM.

    Supported methods:
        - 'default': Automatic method selection
        - 'exact': Exact convolution
        - 'comom': Approximate method
        - 'mom': Method of moments

    Args:
        model: Network model (Python wrapper or native structure)
        method: Solution method (default: 'default')
        **kwargs: Additional solver options
    """

    def __init__(self, model, method_or_options=None, **kwargs):
        self.model = model
        self._result = None
        self._result_ld = None
        self._sn = None

        # Handle options passed as second argument (MATLAB-style)
        if method_or_options is None:
            # Check if method was passed as keyword argument
            self.method = kwargs.pop('method', 'default')
            if isinstance(self.method, str):
                self.method = self.method.lower()
        elif isinstance(method_or_options, str):
            self.method = method_or_options.lower()
            # Remove 'method' from kwargs if present to avoid duplicate argument
            kwargs.pop('method', None)
        elif hasattr(method_or_options, 'get'):
            # Dict-like options object
            self.method = method_or_options.get('method', 'default')
            if hasattr(method_or_options, 'verbose'):
                kwargs.setdefault('verbose', method_or_options.verbose)
            if hasattr(method_or_options, 'iter_max'):
                kwargs.setdefault('iter_max', method_or_options.iter_max)
            if hasattr(method_or_options, 'seed'):
                kwargs.setdefault('seed', method_or_options.seed)
            kwargs.pop('method', None)
        elif hasattr(method_or_options, 'method'):
            # SolverOptions-like object
            self.method = getattr(method_or_options, 'method', 'default')
            if hasattr(method_or_options, 'verbose'):
                kwargs.setdefault('verbose', method_or_options.verbose)
            if hasattr(method_or_options, 'iter_max'):
                kwargs.setdefault('iter_max', method_or_options.iter_max)
            if hasattr(method_or_options, 'seed'):
                kwargs.setdefault('seed', method_or_options.seed)
            kwargs.pop('method', None)
        else:
            self.method = kwargs.pop('method', 'default')
            if isinstance(self.method, str):
                self.method = self.method.lower()

        self.options = SolverNCOptions(method=self.method, **kwargs)

        # Extract network structure
        self._extract_network_params()

    def getName(self) -> str:
        """Get the name of this solver."""
        return "NC"

    get_name = getName

    def reset(self):
        """Reset solver state to force recomputation on next getAvg() call.

        This is called by ensemble solvers (like LN) after updating layer
        parameters to ensure the solver recomputes with new values.
        """
        self._result = None
        self._result_ld = None
        # Re-extract network parameters to pick up changes
        self._extract_network_params()

    def _extract_network_params(self):
        """Extract parameters from the model for NC computation."""
        model = self.model

        # Priority 1: Native model with _sn
        if hasattr(model, '_sn') and model._sn is not None:
            self._sn = model._sn
            return

        # Priority 2: Native model with refresh_struct
        if hasattr(model, 'refresh_struct'):
            model.refresh_struct()
            if hasattr(model, '_sn'):
                self._sn = model._sn
                return

        # Priority 3: JPype wrapper
        if hasattr(model, 'obj'):
            try:
                from ...solvers.convert import wrapper_sn_to_native
                sn = model.getStruct()
                self._sn = wrapper_sn_to_native(sn)
                return
            except Exception:
                pass

        # Priority 4: Has getStruct method
        if hasattr(model, 'getStruct'):
            from ...solvers.convert import wrapper_sn_to_native
            sn = model.getStruct()
            self._sn = wrapper_sn_to_native(sn)
            return

        # Priority 5: Already a struct
        if hasattr(model, 'nclasses') and hasattr(model, 'nstations'):
            self._sn = model
            return

        raise ValueError("Cannot extract NetworkStruct from model")

    def runAnalyzer(self) -> 'SolverNC':
        """Run the NC analysis."""
        import numpy as np
        import warnings
        from ...api.solvers.nc.handler import (
            solver_nc, solver_ncld, SolverOptions as HandlerOptions
        )
        from ...api.solvers.nc.analyzers import solver_nc_lossn_analyzer
        from ...api.sn.predicates import sn_has_closed_classes
        from ...lang.base import DropStrategy

        line_debug("NC: using lang=python", options=self.options)

        sn = self._sn

        # Check if model contains Cache nodes - use specialized cache analyzer
        has_cache = False
        if hasattr(self, 'model') and hasattr(self.model, '_nodes'):
            from ...lang.nodes import Cache
            for node in self.model._nodes:
                if isinstance(node, Cache):
                    has_cache = True
                    break

        if has_cache:
            line_debug("Non-reentrant cache (Source-Cache-Sink), routing to nc_cache_analyzer", options=self.options)
            return self._runCacheAnalyzer()

        # Check for open loss network with FCR (MATLAB: lines 138-161 in runAnalyzer.m)
        # Conditions:
        # 1. No closed classes
        # 2. Exactly 1 FCR
        # 3. Single station in FCR with infinite servers (Delay)
        # 4. All classes have DROP policy
        nservers = sn.nservers.flatten().copy() if sn.nservers is not None else np.ones(sn.nstations)
        # Save original nservers and lldscaling so we can restore after the
        # solver completes.  The multi-server -> lldscaling transformation
        # below sets nservers[i]=1 and writes a new lldscaling matrix into
        # sn.  Unlike MATLAB (value semantics) the Python sn is a shared
        # reference, so we must restore these fields to avoid corrupting
        # subsequent solver calls on the same model.
        _orig_nservers = sn.nservers.copy() if sn.nservers is not None else None
        _orig_lldscaling = sn.lldscaling.copy() if (hasattr(sn, 'lldscaling') and sn.lldscaling is not None) else None

        if (not sn_has_closed_classes(sn) and
                hasattr(sn, 'nregions') and sn.nregions == 1 and
                hasattr(sn, 'region') and sn.region is not None and len(sn.region) > 0):

            region_matrix = sn.region[0]
            K = sn.nclasses
            M = sn.nstations

            # Find stations in FCR (those with non-negative constraints)
            stations_in_fcr = []
            for i in range(M):
                has_class_constraint = np.any(region_matrix[i, :K] >= 0)
                has_global_constraint = region_matrix[i, K] >= 0 if region_matrix.shape[1] > K else False
                if has_class_constraint or has_global_constraint:
                    stations_in_fcr.append(i)

            # Check if single Delay node in FCR with DROP policy
            if (len(stations_in_fcr) == 1 and
                    np.isinf(nservers[stations_in_fcr[0]]) and
                    hasattr(sn, 'regionrule') and sn.regionrule is not None):

                # Check if all classes have DROP policy
                all_drop = True
                for r in range(K):
                    if sn.regionrule[0, r] != float(DropStrategy.DROP):
                        all_drop = False
                        break

                if all_drop:
                    line_debug("Open model with single FCR + Delay node (DROP), routing to nc_lossn_analyzer", options=self.options)
                    # Use loss network solver
                    handler_options = HandlerOptions(
                        method=self.options.method,
                        tol=self.options.tol,
                        iter_max=self.options.iter_max,
                        iter_tol=self.options.iter_tol,
                        verbose=1 if self.options.verbose else 0
                    )
                    result = solver_nc_lossn_analyzer(sn, handler_options)
                    # Convert NCResult to handler result format
                    from dataclasses import dataclass

                    @dataclass
                    class LossnResult:
                        Q: np.ndarray
                        U: np.ndarray
                        R: np.ndarray
                        T: np.ndarray
                        X: np.ndarray
                        lG: float
                        STeff: np.ndarray
                        it: int
                        runtime: float
                        method: str

                    self._result = LossnResult(
                        Q=result.QN,
                        U=result.UN,
                        R=result.RN,
                        T=result.TN,
                        X=result.XN.flatten() if result.XN is not None else np.zeros(K),
                        lG=result.lG if result.lG is not None else np.nan,
                        STeff=np.zeros((M, K)),
                        it=result.it,
                        runtime=result.runtime,
                        method=result.method
                    )
                    self._extract_names()
                    return self

        # Check if we should use load-dependent analysis
        # MATLAB behavior:
        # 1. For 'exact' method with multi-server stations: transform to lldscaling and use solver_ncld
        # 2. For 'default' with 2-station (Delay + multi-server Queue): use comomld/solver_ncld
        use_ld_solver = False
        # sn and nservers already computed above

        # Check for multi-server stations
        has_multiserver = any(s > 1 and np.isfinite(s) for s in nservers)

        # Check for open classes
        has_open = False
        if sn.njobs is not None:
            njobs = sn.njobs.flatten()
            has_open = any(np.isinf(njobs))

        method = self.options.method

        if method == 'exact' and has_multiserver and not has_open:
            line_debug("Exact method: converting multiserver stations to load-dependent", options=self.options)
            # Transform multi-server nodes into lldscaling (like MATLAB lines 69-80 in runAnalyzer.m)
            njobs = sn.njobs.flatten() if sn.njobs is not None else np.zeros(sn.nclasses)
            Nt = int(np.sum(njobs[np.isfinite(njobs)]))
            if Nt > 0:
                # Create lldscaling matrix
                lldscaling = np.ones((sn.nstations, Nt))
                for i in range(sn.nstations):
                    if nservers[i] > 1 and np.isfinite(nservers[i]):
                        for j in range(Nt):
                            lldscaling[i, j] = min(j + 1, nservers[i])
                        nservers[i] = 1  # Set to single server after creating lldscaling

                # Update sn with lldscaling
                sn.lldscaling = lldscaling
                sn.nservers = nservers.reshape(-1, 1)
                use_ld_solver = True

        elif method == 'default' and has_multiserver:
            line_debug("Default method: 2-station Delay+multiserver network, using comomld", options=self.options)
            # Check for 2-station model with Delay + multi-server Queue
            # (like MATLAB lines 62-63 in runAnalyzer.m)
            if sn.nstations == 2:
                has_delay = any(np.isinf(nservers))
                if has_delay and not has_open:
                    use_ld_solver = True

        # Check if lldscaling is already set
        if hasattr(sn, 'lldscaling') and sn.lldscaling is not None and sn.lldscaling.size > 0:
            use_ld_solver = True

        # Check for LJCD scaling (FES models) - route to convolution solver
        use_conv_solver = False
        if hasattr(sn, 'ljcdscaling') and sn.ljcdscaling is not None:
            if isinstance(sn.ljcdscaling, list) and any(x is not None for x in sn.ljcdscaling):
                use_conv_solver = True

        # Create handler options
        handler_options = HandlerOptions(
            method=self.options.method,
            tol=self.options.tol,
            iter_max=self.options.iter_max,
            iter_tol=self.options.iter_tol,
            verbose=1 if self.options.verbose else 0
        )

        # Run the appropriate solver
        if use_conv_solver:
            from ...api.pfqn.conv import solver_nc_conv
            line_debug("LJCD scaling detected, routing to convolution solver", options=self.options)
            self._result = solver_nc_conv(sn, handler_options)
        elif use_ld_solver:
            line_debug("Load-dependent scaling detected, routing to ncld_analyzer", options=self.options)
            self._result = solver_ncld(sn, handler_options)
        else:
            line_debug("NC method=%s, routing to nc_analyzer", self.options.method, options=self.options)
            self._result = solver_nc(sn, handler_options)

        # Extract station and class names
        self._extract_names()

        # Print completion message (matches MATLAB verbose guard)
        if self.options.verbose:
            import sys
            py_version = f"{sys.version_info.major}.{sys.version_info.minor}.{sys.version_info.micro}"
            runtime = self._result.runtime if hasattr(self._result, 'runtime') else 0.0
            method = self._result.method if hasattr(self._result, 'method') else 'default'
            print(f"NC analysis [method: {method}, lang: python, env: {py_version}] completed in {runtime:.6f}s.")

        # Restore original nservers and lldscaling to avoid corrupting the
        # shared sn struct (the multi-server -> lldscaling transformation
        # sets nservers[i]=1 and writes a new lldscaling matrix).
        if _orig_nservers is not None:
            sn.nservers = _orig_nservers
        sn.lldscaling = _orig_lldscaling

        return self

    def _runCacheAnalyzer(self) -> 'SolverNC':
        """
        Run the NC cache analyzer for networks with Cache nodes.

        This method distinguishes between:
        1. Standalone cache networks (Source→Cache→Sink with no closed jobs):
           Uses solver_nc_cache_analyzer for direct cache analysis
        2. Cache+queueing networks (Cache nodes with queues and closed classes):
           Uses solver_nc_cacheqn_analyzer for iterative cache-QN analysis

        This is called automatically by runAnalyzer() when the model
        contains Cache nodes.

        Returns:
            Self for method chaining

        References:
            MATLAB: runAnalyzer.m lines 90-91, 127-128
        """
        from ...api.solvers.nc.handler import (
            solver_nc_cache_analyzer, solver_nc_cacheqn_analyzer, SolverOptions as HandlerOptions
        )
        from ...api.sn.network_struct import NodeType

        sn = self._sn

        # Determine if this is a standalone cache network (non-reentrant cache)
        # MATLAB line 90: nclosedjobs == 0 && all(sort(nodetype) == [Source, Cache, Sink])
        is_standalone_cache = False

        # Check for no closed jobs
        nclosedjobs = sn.nclosedjobs if hasattr(sn, 'nclosedjobs') else 0
        if nclosedjobs == 0:
            # Check if node types are exactly Source, Cache, Sink
            if sn.nodetype is not None and len(sn.nodetype) == 3:
                node_types_sorted = sorted(int(nt) for nt in sn.nodetype)
                expected_types = sorted([int(NodeType.SOURCE), int(NodeType.CACHE), int(NodeType.SINK)])
                if node_types_sorted == expected_types:
                    is_standalone_cache = True

        # Create handler options
        handler_options = HandlerOptions(
            method=self.options.method,
            tol=self.options.tol,
            iter_max=self.options.iter_max,
            iter_tol=self.options.iter_tol,
            verbose=1 if self.options.verbose else 0
        )

        # Add samples if available
        if hasattr(self.options, 'samples') and self.options.samples is not None:
            handler_options.samples = self.options.samples

        if is_standalone_cache:
            # Standalone cache network: use direct cache analyzer
            cache_result = solver_nc_cache_analyzer(sn, handler_options)
        else:
            # Cache+queueing network: use iterative cache-QN analyzer
            cache_result = solver_nc_cacheqn_analyzer(sn, handler_options)

        # Convert cache result to standard NC result format
        # Create a result object compatible with the standard NC result
        from dataclasses import dataclass

        @dataclass
        class CacheResultAdapter:
            Q: np.ndarray
            U: np.ndarray
            R: np.ndarray
            T: np.ndarray
            X: np.ndarray
            lG: float
            STeff: np.ndarray
            it: int
            runtime: float
            method: str
            pij: np.ndarray  # Cache-specific: item probabilities
            hitprob: np.ndarray  # Hit probabilities per cache per class
            missprob: np.ndarray  # Miss probabilities per cache per class

        M = self._sn.nstations
        K = self._sn.nclasses

        # Initialize with zeros for stations that don't have metrics
        Q = cache_result.QN if cache_result.QN is not None else np.zeros((M, K))
        U = cache_result.UN if cache_result.UN is not None else np.zeros((M, K))
        R = cache_result.RN if cache_result.RN is not None else np.zeros((M, K))
        T = cache_result.TN if cache_result.TN is not None else np.zeros((M, K))

        # Handle different result types (standalone vs cacheqn)
        pij = getattr(cache_result, 'pij', None)
        hitprob = getattr(cache_result, 'hitprob', None)
        missprob = getattr(cache_result, 'missprob', None)
        it_count = getattr(cache_result, 'it', 1)

        self._result = CacheResultAdapter(
            Q=Q,
            U=U,
            R=R,
            T=T,
            X=cache_result.XN,
            lG=cache_result.lG,
            STeff=np.zeros((M, K)),
            it=it_count,
            runtime=cache_result.runtime,
            method=cache_result.method,
            pij=pij,
            hitprob=hitprob,
            missprob=missprob
        )

        # Store cache-specific results
        self._cache_result = cache_result

        # Copy updated visits from cache analyzer to self._sn
        # (MATLAB: self.model.refreshStruct(true); sn = self.model.sn;)
        if hasattr(cache_result, 'visits') and cache_result.visits is not None:
            sn.visits = cache_result.visits
        if hasattr(cache_result, 'nodevisits') and cache_result.nodevisits is not None:
            sn.nodevisits = cache_result.nodevisits

        # Extract station and class names
        self._extract_names()

        # Set hit/miss probabilities on Cache node in model
        # This is needed for get_hit_ratio()/get_miss_ratio() to work correctly
        for ind in range(sn.nnodes):
            if sn.nodetype is not None and ind < len(sn.nodetype):
                if sn.nodetype[ind] == NodeType.CACHE and ind in sn.nodeparam:
                    cache_param = sn.nodeparam[ind]

                    # Try to get hit/miss probs from nodeparam (standalone cache)
                    actualhitprob = getattr(cache_param, 'actualhitprob', None)
                    actualmissprob = getattr(cache_param, 'actualmissprob', None)

                    # For cacheqn results, always use the hitprob from the NC solver
                    # (a previous solver like SSA may have set actualhitprob, which would be stale)
                    if hitprob is not None:
                        # hitprob shape is (ncaches, K) - find index of this cache
                        cache_indices = []
                        for i in range(sn.nnodes):
                            if i < len(sn.nodetype) and sn.nodetype[i] == NodeType.CACHE:
                                cache_indices.append(i)
                        if ind in cache_indices:
                            cache_idx = cache_indices.index(ind)
                            if cache_idx < hitprob.shape[0]:
                                actualhitprob = hitprob[cache_idx, :]
                                actualmissprob = missprob[cache_idx, :]

                    if actualhitprob is not None:
                        # Update sn.nodeparam with actual hit/miss probs for sn_get_node_tput_from_tput
                        cache_param.actualhitprob = actualhitprob
                        cache_param.actualmissprob = actualmissprob

                        # Also set result on Cache node in model
                        if hasattr(self, 'model') and hasattr(self.model, '_nodes'):
                            cache_node = self.model._nodes[ind]
                            if hasattr(cache_node, 'set_result_hit_prob'):
                                cache_node.set_result_hit_prob(actualhitprob)
                            if actualmissprob is not None and hasattr(cache_node, 'set_result_miss_prob'):
                                cache_node.set_result_miss_prob(actualmissprob)

        return self

    def runAnalyzerLD(self) -> 'SolverNC':
        """
        Run the load-dependent NC analysis.

        Returns:
            Self for method chaining
        """
        from ...api.solvers.nc.handler import (
            solver_ncld, SolverOptions as HandlerOptions
        )

        # Create handler options
        handler_options = HandlerOptions(
            method=self.options.method,
            tol=self.options.tol,
            iter_max=self.options.iter_max,
            iter_tol=self.options.iter_tol,
            verbose=1 if self.options.verbose else 0
        )

        # Run the load-dependent solver
        self._result_ld = solver_ncld(self._sn, handler_options)

        # Also set standard result for metric accessors
        self._result = self._result_ld

        # Extract station and class names
        self._extract_names()

        return self

    def _extract_names(self):
        """Extract station and class names from network struct."""
        if self._sn is not None:
            # Use station names, not node names (NC operates on stations, not nodes)
            # Nodes include non-station elements like ClassSwitch, Router, etc.
            if hasattr(self._sn, 'stationnames') and self._sn.stationnames:
                self.station_names = list(self._sn.stationnames)
            elif hasattr(self._sn, 'nodenames') and self._sn.nodenames and hasattr(self._sn, 'stationToNode'):
                # Map station indices to node names
                self.station_names = []
                for i in range(self._sn.nstations):
                    if i < len(self._sn.stationToNode):
                        node_idx = int(self._sn.stationToNode[i])
                        if node_idx < len(self._sn.nodenames):
                            self.station_names.append(self._sn.nodenames[node_idx])
                        else:
                            self.station_names.append(f'Station{i}')
                    else:
                        self.station_names.append(f'Station{i}')
            else:
                self.station_names = [f'Station{i}' for i in range(self._sn.nstations)]
            self.class_names = list(self._sn.classnames) if hasattr(self._sn, 'classnames') and self._sn.classnames else \
                              [f'Class{i}' for i in range(self._sn.nclasses)]
        else:
            self.station_names = []
            self.class_names = []

    # =========================================================================
    # Table Output
    # =========================================================================

    def getAvgTable(self) -> pd.DataFrame:
        """
        Get comprehensive average performance metrics table.

        Returns:
            pandas.DataFrame with columns: Station, JobClass, QLen, Util, RespT, ResidT, ArvR, Tput
        """
        if self._result is None:
            self.runAnalyzer()

        nstations = self._result.Q.shape[0]
        nclasses = self._result.Q.shape[1]

        # Compute residence times from response times using visit ratios
        if self._sn is not None and self._sn.visits:
            WN = sn_get_residt_from_respt(self._sn, self._result.R, None)
        else:
            WN = self._result.R.copy()

        # Compute proper arrival rates (sets Source ArvR = 0)
        AN = sn_get_arvr_from_tput(self._sn, self._result.T)

        rows = []
        for i in range(nstations):
            for r in range(nclasses):
                station_name = self.station_names[i] if i < len(self.station_names) else f'Station{i}'
                class_name = self.class_names[r] if r < len(self.class_names) else f'Class{r}'

                rows.append({
                    'Station': station_name,
                    'JobClass': class_name,
                    'QLen': self._result.Q[i, r],
                    'Util': self._result.U[i, r],
                    'RespT': self._result.R[i, r],
                    'ResidT': WN[i, r],
                    'ArvR': AN[i, r],
                    'Tput': self._result.T[i, r],
                })

        df = pd.DataFrame(rows)

        # Filter out all-zero rows
        numeric_cols = ['QLen', 'Util', 'RespT', 'ResidT', 'ArvR', 'Tput']
        tokeep = ~(df[numeric_cols] <= 0.0).all(axis=1)
        df = df.loc[tokeep].reset_index(drop=True)

        if not self._table_silent:
            print(df.to_string(index=False))

        return df

    # =========================================================================
    # Individual Metric Accessors
    # =========================================================================

    def getAvgQLen(self) -> np.ndarray:
        """Get average queue lengths (M x K)."""
        if self._result is None:
            self.runAnalyzer()
        return self._result.Q.copy()

    def getAvgUtil(self) -> np.ndarray:
        """Get average utilizations (M x K)."""
        if self._result is None:
            self.runAnalyzer()
        return self._result.U.copy()

    def getAvgRespT(self) -> np.ndarray:
        """Get average response times (M x K)."""
        if self._result is None:
            self.runAnalyzer()
        return self._result.R.copy()

    def getAvgResidT(self) -> np.ndarray:
        """Get average residence times (M x K).

        Residence time is computed from response time using visit ratios:
        WN[ist,k] = RN[ist,k] * V[ist,k] / V[refstat,refclass]
        """
        if self._result is None:
            self.runAnalyzer()

        # Compute ResidT using proper visit ratios from network structure
        if self._sn is not None and self._sn.visits:
            return sn_get_residt_from_respt(self._sn, self._result.R, None)
        else:
            # Fallback: ResidT = RespT (no visit information available)
            return self._result.R.copy()

    def getAvgWaitT(self) -> np.ndarray:
        """Get average waiting times (M x K)."""
        if self._result is None:
            self.runAnalyzer()

        R = self._result.R.copy()
        # W = R - S where S is service time (1/rate)
        if hasattr(self._sn, 'rates') and self._sn.rates is not None:
            rates = np.asarray(self._sn.rates)
            S = np.zeros_like(rates)
            nonzero = rates > 0
            S[nonzero] = 1.0 / rates[nonzero]
            W = R - S
            W = np.maximum(W, 0.0)
            return W
        return R

    def getAvgTput(self) -> np.ndarray:
        """Get average throughputs (M x K)."""
        if self._result is None:
            self.runAnalyzer()
        return self._result.T.copy()

    def getAvgArvR(self) -> np.ndarray:
        """Get average arrival rates (M x K).

        Uses routing matrix to compute proper arrival rates.
        Source stations have arrival rate = 0.
        """
        if self._result is None:
            self.runAnalyzer()
        return sn_get_arvr_from_tput(self._sn, self._result.T)

    def getAvgSysRespT(self) -> np.ndarray:
        """Get system response times (cycle times) per chain (nchains,).

        Returns chain-level response times matching MATLAB/Java implementation.
        Uses the completes flag to determine which classes contribute to chain throughput.

        Note:
            For closed chains: uses Little's Law CNchain = nJobsChain / XNchain
            For open chains: weighted sum of class response times
        """
        CN, XN = self._computeChainMetrics()
        return CN

    def getAvgSysTput(self) -> np.ndarray:
        """Get system throughputs per chain (nchains,).

        Returns chain-level throughputs matching MATLAB/Java implementation.
        Uses the completes flag to determine which classes contribute to chain throughput.
        """
        CN, XN = self._computeChainMetrics()
        return XN

    def _computeChainMetrics(self) -> Tuple[np.ndarray, np.ndarray]:
        """Compute chain-level response times and throughputs.

        This implements the MATLAB getAvgSys.m algorithm:
        1. Get completes flag from each class
        2. Compute CNclass (class-level response times weighted by visits)
        3. Compute alpha coefficients for weighting class contributions
        4. For each chain, compute XNchain and CNchain

        Returns:
            Tuple of (CNchain, XNchain) - chain-level response times and throughputs
        """
        if self._result is None:
            self.runAnalyzer()

        sn = self._sn
        if sn is None:
            sn = self.model.getStruct(True)
            self._sn = sn

        RN = self._result.R  # Station response times (M x K)
        TN = self._result.T  # Station throughputs (M x K)
        njobs = sn.njobs.flatten()
        nstations = sn.nstations
        nclasses = sn.nclasses
        nchains = sn.nchains if hasattr(sn, 'nchains') and sn.nchains > 0 else 1

        # Get completes flag for each class
        completes = np.ones(nclasses, dtype=bool)
        classes = self.model.get_classes()
        for r in range(nclasses):
            if r < len(classes):
                completes[r] = classes[r].completes

        # Compute class-level response times weighted by visits (CNclass)
        CNclass = np.zeros(nclasses)
        for c in range(nchains):
            if c not in sn.inchain:
                continue
            inchain = sn.inchain[c].flatten().astype(int)
            for r in inchain:
                if r >= nclasses:
                    continue
                CNclass[r] = 0.0
                if RN is not None and RN.size > 0:
                    refstat = int(sn.refstat[r]) if hasattr(sn, 'refstat') else 0
                    for i in range(nstations):
                        # Skip if this is the source for an open class
                        if np.isinf(njobs[r]) and i == refstat:
                            continue
                        # Get visits
                        if c in sn.visits and sn.visits[c] is not None:
                            visits_c = sn.visits[c]
                            stateful_i = int(sn.stationToStateful[i]) if hasattr(sn, 'stationToStateful') else i
                            stateful_ref = int(sn.stationToStateful[refstat]) if hasattr(sn, 'stationToStateful') else refstat
                            if stateful_i < visits_c.shape[0] and r < visits_c.shape[1]:
                                visit_i = visits_c[stateful_i, r]
                                visit_ref = visits_c[stateful_ref, r] if stateful_ref < visits_c.shape[0] else 1.0
                                if visit_ref > 0:
                                    CNclass[r] += visit_i * RN[i, r] / visit_ref

        # Compute alpha coefficients
        alpha = np.zeros((nstations, nclasses))
        for c in range(nchains):
            if c not in sn.inchain:
                continue
            inchain = sn.inchain[c].flatten().astype(int)

            # Get completing classes in this chain (intersection of inchain and completes)
            completingclasses = [k for k in inchain if k < nclasses and completes[k]]

            for i in range(nstations):
                for k in inchain:
                    if k >= nclasses:
                        continue
                    refstat = int(sn.refstat[k]) if hasattr(sn, 'refstat') else 0
                    if c in sn.visits and sn.visits[c] is not None:
                        visits_c = sn.visits[c]
                        stateful_i = int(sn.stationToStateful[i]) if hasattr(sn, 'stationToStateful') else i
                        stateful_ref = int(sn.stationToStateful[refstat]) if hasattr(sn, 'stationToStateful') else refstat

                        # Sum visits at refstat for completing classes
                        sum_visits = 0.0
                        for idx in completingclasses:
                            if stateful_ref < visits_c.shape[0] and idx < visits_c.shape[1]:
                                sum_visits += visits_c[stateful_ref, idx]

                        if sum_visits > 0 and stateful_i < visits_c.shape[0] and k < visits_c.shape[1]:
                            alpha[i, k] += visits_c[stateful_i, k] / sum_visits

        # Replace inf/nan with 0
        alpha[~np.isfinite(alpha)] = 0.0

        # Compute chain-level metrics
        CNchain = np.zeros(nchains)
        XNchain = np.zeros(nchains)

        for c in range(nchains):
            if c not in sn.inchain:
                continue
            inchain = sn.inchain[c].flatten().astype(int)

            # Get completing classes in this chain (intersection of inchain and completes)
            completingclasses = [k for k in inchain if k < nclasses and completes[k]]

            # Compute chain throughput from completing classes
            if TN is not None and TN.size > 0:
                ref = int(sn.refstat[inchain[0]]) if hasattr(sn, 'refstat') and len(inchain) > 0 else 0
                for i in range(nstations):
                    for r in completingclasses:
                        if r >= nclasses:
                            continue
                        if not np.isnan(TN[i, r]):
                            # Use routing table to compute throughput contribution
                            if hasattr(sn, 'rt') and sn.rt is not None:
                                for s in inchain:
                                    if s >= nclasses:
                                        continue
                                    rt_idx_src = i * nclasses + r
                                    rt_idx_dst = ref * nclasses + s
                                    if rt_idx_src < sn.rt.shape[0] and rt_idx_dst < sn.rt.shape[1]:
                                        XNchain[c] += sn.rt[rt_idx_src, rt_idx_dst] * TN[i, r]
                            else:
                                # Fallback: sum throughputs at reference station
                                if i == ref:
                                    XNchain[c] += TN[i, r]

            # Compute chain response time using total jobs in chain
            nJobsChain = sum(njobs[k] for k in inchain if k < nclasses)

            if np.isinf(nJobsChain):
                # Open chain: use weighted sum
                refstat = int(sn.refstat[inchain[0]]) if hasattr(sn, 'refstat') and len(inchain) > 0 else 0
                sumfinite = 0.0
                for k in inchain:
                    if k >= nclasses:
                        continue
                    val = alpha[refstat, k] * CNclass[k]
                    if np.isfinite(val):
                        sumfinite += val
                CNchain[c] = sumfinite
            else:
                # Closed chain: use Little's Law
                if XNchain[c] > 0:
                    CNchain[c] = nJobsChain / XNchain[c]
                else:
                    CNchain[c] = np.inf

        return CNchain, XNchain

    # =========================================================================
    # NC-Specific Methods
    # =========================================================================

    def getNormalizingConstant(self) -> float:
        """
        Get the normalizing constant G.

        Returns:
            float: The normalizing constant (not log)
        """
        if self._result is None:
            self.runAnalyzer()

        lG = self._result.lG
        if np.isfinite(lG):
            return np.exp(lG)
        return 0.0

    def getLogNormalizingConstant(self) -> float:
        """
        Get the log normalizing constant log(G).

        Returns:
            float: log(G)
        """
        if self._result is None:
            self.runAnalyzer()
        return self._result.lG

    def getProbNormConstAggr(self) -> float:
        """
        Get the log normalizing constant (alias).

        Returns:
            float: log(G)
        """
        return self.getLogNormalizingConstant()

    def getEffectiveServiceTimes(self) -> np.ndarray:
        """
        Get effective service times.

        Returns:
            np.ndarray: Effective service times (M x K)
        """
        if self._result is None:
            self.runAnalyzer()
        if self._result.STeff is not None:
            return self._result.STeff.copy()
        return np.array([])

    def getIterationCount(self) -> int:
        """Get the number of iterations used."""
        if self._result is None:
            self.runAnalyzer()
        return self._result.it

    def getRuntime(self) -> float:
        """Get the solver runtime in seconds."""
        if self._result is None:
            self.runAnalyzer()
        return self._result.runtime

    def getMethodUsed(self) -> str:
        """Get the method actually used (may differ from requested)."""
        if self._result is None:
            self.runAnalyzer()
        return self._result.method

    # =========================================================================
    # CDF and Percentile Methods
    # =========================================================================

    def getCdfRespT(self, R: Optional[np.ndarray] = None) -> List[Dict]:
        """
        Get response time CDF using exponential approximation.

        Returns:
            List of dicts with 'station', 'class', 't', 'p' keys
        """
        if self._result is None:
            self.runAnalyzer()

        if R is None:
            R = self._result.R

        nstations, nclasses = R.shape
        RD = []

        for i in range(nstations):
            for r in range(nclasses):
                mean_resp_t = R[i, r]
                if mean_resp_t <= 0:
                    continue

                lambda_rate = 1.0 / mean_resp_t
                quantiles = np.linspace(0.001, 0.999, 100)
                times = -np.log(1 - quantiles) / lambda_rate
                cdf_vals = 1 - np.exp(-lambda_rate * times)

                RD.append({
                    'station': i + 1,
                    'class': r + 1,
                    't': times,
                    'p': cdf_vals,
                })

        return RD

    def getPerctRespT(
        self,
        percentiles: Optional[List[float]] = None,
        jobclass: Optional[int] = None
    ) -> Tuple[List[Dict], pd.DataFrame]:
        """
        Extract percentiles from response time distribution.

        Args:
            percentiles: List of percentiles (0-100). Default: [10, 25, 50, 75, 90, 95, 99]
            jobclass: Optional class filter (1-based)

        Returns:
            Tuple of (percentile_list, percentile_table)
        """
        if percentiles is None:
            percentiles = [10, 25, 50, 75, 90, 95, 99]

        percentiles = np.asarray(percentiles)
        percentiles = np.clip(percentiles, 0.01, 99.99)
        percentiles_normalized = percentiles / 100.0

        if self._result is None:
            self.runAnalyzer()

        R = self._result.R
        nstations, nclasses = R.shape

        PercRT = []
        rows = []
        perc_col_names = [f'P{int(p)}' for p in percentiles]

        for i in range(nstations):
            for r in range(nclasses):
                if jobclass is not None and (r + 1) != jobclass:
                    continue

                mean_resp_t = R[i, r]
                if mean_resp_t <= 0:
                    continue

                lambda_rate = 1.0 / mean_resp_t
                perc_values = -np.log(1 - percentiles_normalized) / lambda_rate

                PercRT.append({
                    'station': i + 1,
                    'class': r + 1,
                    'percentiles': percentiles.tolist(),
                    'values': perc_values.tolist(),
                })

                row_data = {
                    'Station': self.station_names[i] if i < len(self.station_names) else f'Station{i}',
                    'Class': self.class_names[r] if r < len(self.class_names) else f'Class{r}',
                }
                for perc_col, perc_val in zip(perc_col_names, perc_values):
                    row_data[perc_col] = perc_val
                rows.append(row_data)

        PercTable = pd.DataFrame(rows) if rows else pd.DataFrame()
        return PercRT, PercTable

    # =========================================================================
    # Introspection Methods
    # =========================================================================

    def listValidMethods(self) -> List[str]:
        """List valid solution methods.

        Returns:
            List of valid method names for NC solver:
            - 'default': Auto-select based on problem size
            - 'exact', 'ca': Exact convolution algorithm
            - 'imci': Importance sampling Monte Carlo integration
            - 'ls': Linearizer method
            - 'le': Leading eigenvalue asymptotic
            - 'mmint2': Gauss-Legendre quadrature
            - 'gleint': Gauss-Legendre integration
            - 'panacea': Hybrid convolution/MVA
            - 'kt': Knessl-Tier expansion
            - 'sampling': Monte Carlo sampling
            - 'propfair': Proportionally fair allocation
            - 'comom': Conditional moments
            - 'cub': Controllable upper bound
            - 'rd': Reduced decomposition
            - 'nrl': Normal Radius-Logistic approximation
            - 'nrp': Normal Radius-Probit approximation
        """
        return [
            'default', 'exact', 'ca',
            'imci', 'ls',
            'le', 'mmint2', 'gleint', 'panacea',
            'kt', 'sampling',
            'propfair', 'comom', 'cub',
            'rd', 'nrl', 'nrp',
        ]

    @staticmethod
    def getFeatureSet():
        """Get supported features as a SolverFeatureSet.

        NC supports limited features - notably not Cache with LRU replacement.
        """
        from ..base import SolverFeatureSet
        feat_supported = SolverFeatureSet()
        feat_supported.set_true([
            'Sink', 'Source', 'Queue', 'Delay',
            'Exp', 'Erlang', 'HyperExp', 'APH', 'PH', 'Cox2',
            'Det', 'Gamma', 'Lognormal', 'Pareto', 'Uniform', 'Weibull',
            'StatelessClassSwitcher', 'ClassSwitch',
            'InfiniteServer', 'SharedServer', 'Buffer', 'Dispatcher',
            'Server', 'ServiceTunnel',
            'RoutingStrategy_PROB', 'RoutingStrategy_RAND',
            'SchedStrategy_INF', 'SchedStrategy_FCFS', 'SchedStrategy_PS',
            'SchedStrategy_LCFS', 'SchedStrategy_SIRO',
            'Cache', 'CacheClassSwitcher',
            # NC only supports RR and FIFO replacement, not LRU
            'ReplacementStrategy_RR', 'ReplacementStrategy_FIFO',
            'OpenClass', 'ClosedClass',
        ])
        return feat_supported

    @staticmethod
    def supports(model) -> bool:
        """Check if model is supported.

        Uses feature set checking to compare supported features against
        features used by the model. Prints warnings for unsupported features.
        """
        from ..base import SolverFeatureSet
        try:
            # Get used features from model
            if hasattr(model, 'get_used_lang_features'):
                feat_used = model.get_used_lang_features()
            elif hasattr(model, 'getUsedLangFeatures'):
                feat_used = model.getUsedLangFeatures()
            else:
                # Fall back to basic check
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

                return nstations > 0 and nclasses > 0

            # Compare features using SolverFeatureSet
            feat_supported = SolverNC.getFeatureSet()
            return SolverFeatureSet.supports(feat_supported, feat_used)
        except Exception:
            return False

    @staticmethod
    def defaultOptions() -> OptionsDict:
        """Get default solver options."""
        return OptionsDict({
            'method': 'default',
            'tol': 1e-6,
            'iter_max': 1000,
            'iter_tol': 1e-4,
            'verbose': False,
        })

    # =========================================================================
    # Probability Methods
    # =========================================================================

    def getProb(self, station: Optional[int] = None) -> np.ndarray:
        """Get state probabilities at station.

        For NC, returns approximate marginal probabilities computed from
        queue lengths using a geometric distribution approximation.

        Args:
            station: Station index (0-based). If None, returns for all stations.

        Returns:
            State probability vector or list of vectors
        """
        if self._result is None:
            self.runAnalyzer()

        Q = self._result.Q
        U = self._result.U

        if station is not None:
            # Single station
            rho = np.mean(U[station, :])
            if rho >= 1.0:
                rho = 0.99
            max_n = max(10, int(Q[station, :].sum() * 3))
            n = np.arange(max_n + 1)
            prob = (1 - rho) * (rho ** n)
            return prob
        else:
            # All stations
            probs = []
            for i in range(Q.shape[0]):
                rho = np.mean(U[i, :])
                if rho >= 1.0:
                    rho = 0.99
                max_n = max(10, int(Q[i, :].sum() * 3))
                n = np.arange(max_n + 1)
                prob = (1 - rho) * (rho ** n)
                probs.append(prob)
            return probs

    def getProbAggr(self, ist) -> float:
        """Get probability of a specific per-class job distribution at a station.

        Returns P(n1 jobs of class 1, n2 jobs of class 2, ...) for the state
        that was set via setState() on the station.

        Args:
            ist: Station index (0-based) or node object

        Returns:
            Probability that station ist is in the specified state.
        """
        from line_solver.api.solvers.nc import solver_nc_margaggr

        # Convert node object to index if needed (like MATLAB)
        if not isinstance(ist, (int, np.integer)):
            ist = ist.get_station_index0()

        if self._result is None:
            self.runAnalyzer()

        sn = self._sn
        if sn is None:
            return 0.0

        # Use solver_nc_margaggr to compute marginal probabilities
        options = self.options
        lG = getattr(getattr(self._result, 'Prob', None), 'logNormConstAggr', None) if self._result else None

        result = solver_nc_margaggr(sn, options, lG)

        # lPr contains log probabilities, convert to probability
        if result.lPr is not None and ist < len(result.lPr):
            return np.exp(result.lPr[ist])

        return 0.0

    def getProbMarg(self, station, jobclass=None) -> np.ndarray:
        """Get marginal queue-length distribution at station.

        When method='comom', uses pfqn_procomom for exact marginal probabilities.

        Args:
            station: Station index (0-based) or station object
            jobclass: Job class index (unused for comom, kept for API compat)

        Returns:
            Marginal probability vector P(n_total = j) for j=0,1,...,sumN
        """
        sn = self.model.getStruct()

        # Convert node object to station index if needed
        if hasattr(station, 'index'):
            ist = sn.nodeToStation[station.index]
        else:
            ist = int(station)

        if self.options.method == 'comom':
            from line_solver.api.sn.demands import sn_get_demands_chain
            from line_solver.api.pfqn.comom import pfqn_procomom

            demands = sn_get_demands_chain(sn)
            Lchain = demands.Lchain.copy()  # (M, C)
            Lchain[~np.isfinite(Lchain)] = 0.0
            Nchain = demands.Nchain.flatten().astype(int)  # (C,)

            M = sn.nstations
            C = sn.nchains

            # Separate queue vs delay stations (matching solver_nc.m lines 97-119)
            # For multiserver queues: Seidmann's approximation splits demand
            # into per-server demand Lms = L/k and residual Zms = L*(k-1)/k
            queue_stations = []
            Z_total = np.zeros(C)
            Zms_total = np.zeros(C)
            Lms = np.zeros_like(Lchain)

            for i in range(M):
                if np.isinf(sn.nservers[i]):
                    # Delay station: accumulate into Z
                    Z_total += Lchain[i, :]
                else:
                    queue_stations.append(i)
                    k = sn.nservers[i]
                    Lms[i, :] = Lchain[i, :] / k
                    Zms_total += Lchain[i, :] * (k - 1) / k

            M_queues = len(queue_stations)
            L_queues = np.zeros((M_queues, C))
            for qi, orig_idx in enumerate(queue_stations):
                L_queues[qi, :] = Lms[orig_idx, :]

            # Call pfqn_procomom
            Pr, Q = pfqn_procomom(L_queues, Nchain, Z_total + Zms_total)
            # Pr is (M_queues x sumN+1)

            # Find queue index for requested station
            if ist in queue_stations:
                queue_idx = queue_stations.index(ist)
                return Pr[queue_idx, :]
            else:
                # Delay station - return zeros
                sumN = int(np.sum(Nchain))
                return np.zeros(sumN + 1)

        # Default fallback: geometric approximation
        if self._result is None:
            self.runAnalyzer()

        Q = self._result.Q
        U = self._result.U

        mean_q = Q[ist, jobclass if jobclass is not None else 0]
        rho = U[ist, jobclass if jobclass is not None else 0]
        if rho >= 1.0:
            rho = 0.99
        if rho <= 0:
            rho = 0.01

        max_n = max(10, int(mean_q * 3))
        n = np.arange(max_n + 1)
        prob = (1 - rho) * (rho ** n)

        return prob

    def getProbSys(self) -> float:
        """Get joint system state probability for the detailed state.

        For closed networks, this computes the probability of the current
        system state using the normalizing constant.

        Returns:
            float: Joint probability of the current system state.
        """
        # NC solver does not support detailed state probabilities,
        # delegate to aggregated version
        return self.getProbSysAggr()

    def getProbSysAggr(self) -> float:
        """Get aggregated system state probability.

        Computes the joint probability of observing the current queue length
        distribution across all stations using normalizing constants.

        Matches MATLAB: SolverNC.getProbSysAggr -> solver_nc_jointaggr

        Returns:
            float: Joint probability of the current system state.
        """
        from line_solver.api.solvers.nc import solver_nc_jointaggr

        if self._result is None:
            self.runAnalyzer()

        # Get a fresh sn from the model - runAnalyzer may have modified
        # self._sn (e.g., converting multiserver nservers to lldscaling),
        # but solver_nc_jointaggr needs the original nservers to build
        # its own mu matrix, matching MATLAB's value-copy semantics.
        self.model.reset_struct()
        sn = self.model.getStruct()
        if sn is None:
            return 0.0

        result = solver_nc_jointaggr(sn, self.options)

        if hasattr(result, 'Pr') and result.Pr is not None:
            return float(result.Pr)

        return 0.0

    # =========================================================================
    # Unified Metrics Method
    # =========================================================================

    def getAvg(self) -> Tuple[np.ndarray, np.ndarray, np.ndarray, np.ndarray, np.ndarray, np.ndarray]:
        """Get all average metrics at once.

        Returns:
            Tuple of (QN, UN, RN, TN, AN, WN) where:
            - QN: Queue lengths
            - UN: Utilizations
            - RN: Response times
            - TN: Throughputs
            - AN: Arrival rates (same as TN)
            - WN: Residence times (RN scaled by visit ratios)
        """
        if self._result is None:
            self.runAnalyzer()

        Q = self._result.Q
        U = self._result.U
        R = self._result.R
        T = self._result.T
        A = T.copy()

        # Compute residence time from response time using visit ratios
        # WN[ist,k] = RN[ist,k] * V[ist,k] / V[refstat,refclass]
        # This matches MATLAB/Java behavior in sn_get_residt_from_respt
        if self._sn is not None and self._sn.visits:
            W = sn_get_residt_from_respt(self._sn, R, None)
        else:
            # Fallback: ResidT = RespT when no visit information available
            W = R.copy()

        return Q, U, R, T, A, W

    # =========================================================================
    # Chain-Level Methods
    # =========================================================================

    def _get_chains(self) -> List[List[int]]:
        """Get chain-to-class mapping from network structure."""
        if hasattr(self._sn, 'chains') and self._sn.chains is not None:
            chains = []
            nchains = getattr(self._sn, 'nchains', 1)
            for c in range(nchains):
                chain_classes = []
                for k in range(self._sn.nclasses):
                    if hasattr(self._sn.chains, '__getitem__'):
                        if self._sn.chains[c, k] > 0:
                            chain_classes.append(k)
                chains.append(chain_classes)
            return chains if chains else [[k for k in range(self._sn.nclasses)]]
        return [[k] for k in range(self._sn.nclasses)]

    def getAvgQLenChain(self) -> np.ndarray:
        """Get average queue lengths aggregated by chain."""
        if self._result is None:
            self.runAnalyzer()
        Q = self._result.Q
        chains = self._get_chains()
        QN = np.zeros((Q.shape[0], len(chains)))
        for c, cc in enumerate(chains):
            if cc:
                QN[:, c] = np.sum(Q[:, cc], axis=1)
        return QN

    def getAvgUtilChain(self) -> np.ndarray:
        """Get average utilizations aggregated by chain."""
        if self._result is None:
            self.runAnalyzer()
        U = self._result.U
        chains = self._get_chains()
        UN = np.zeros((U.shape[0], len(chains)))
        for c, cc in enumerate(chains):
            if cc:
                UN[:, c] = np.sum(U[:, cc], axis=1)
        return UN

    def getAvgRespTChain(self) -> np.ndarray:
        """Get average response times aggregated by chain."""
        if self._result is None:
            self.runAnalyzer()
        R = self._result.R
        chains = self._get_chains()
        RN = np.zeros((R.shape[0], len(chains)))
        for c, cc in enumerate(chains):
            if cc:
                RN[:, c] = np.mean(R[:, cc], axis=1)
        return RN

    def getAvgResidTChain(self) -> np.ndarray:
        """Get average residence times aggregated by chain."""
        return self.getAvgRespTChain()

    def getAvgTputChain(self) -> np.ndarray:
        """Get average throughputs aggregated by chain."""
        if self._result is None:
            self.runAnalyzer()
        T = self._result.T
        chains = self._get_chains()
        TN = np.zeros((T.shape[0], len(chains)))
        for c, cc in enumerate(chains):
            if cc:
                TN[:, c] = np.sum(T[:, cc], axis=1)
        return TN

    def getAvgArvRChain(self) -> np.ndarray:
        """Get average arrival rates aggregated by chain."""
        return self.getAvgTputChain()

    def getAvgChain(self) -> Tuple[np.ndarray, np.ndarray, np.ndarray, np.ndarray, np.ndarray, np.ndarray]:
        """Get all average metrics aggregated by chain."""
        return (self.getAvgQLenChain(), self.getAvgUtilChain(), self.getAvgRespTChain(),
                self.getAvgResidTChain(), self.getAvgArvRChain(), self.getAvgTputChain())

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
                    'QLen': QN[i,c], 'Util': UN[i,c], 'RespT': RN[i,c],
                    'ResidT': WN[i,c], 'ArvR': AN[i,c], 'Tput': TN[i,c]
                })
        return pd.DataFrame(rows)

    def getAvgNode(self) -> Tuple[np.ndarray, np.ndarray, np.ndarray, np.ndarray, np.ndarray, np.ndarray]:
        """
        Get average metrics per node.

        Unlike getAvg() which returns station-level metrics, this method
        returns node-level metrics including non-station nodes (e.g., Router/VSink).

        Returns:
            Tuple of (QNn, UNn, RNn, WNn, ANn, TNn) - node-level metrics
        """
        from ...api.sn.getters import sn_get_node_arvr_from_tput, sn_get_node_tput_from_tput

        if self._result is None:
            self.runAnalyzer()

        QN = self._result.Q
        UN = self._result.U
        RN = self._result.R
        TN = self._result.T
        AN = sn_get_arvr_from_tput(self._sn, TN)  # Compute proper arrival rates from routing

        sn = self._sn
        I = sn.nnodes
        M = sn.nstations
        R = sn.nclasses

        # Create TH (throughput handle) - indicates which station-classes have valid throughput
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

        # Compute residence times from response times using visit ratios
        WN = sn_get_residt_from_respt(sn, RN, None)

        # Copy station metrics to station nodes
        for ist in range(M):
            ind = sn.stationToNode[ist]
            if ind >= 0 and ind < I:
                QNn[ind, :] = QN[ist, :]
                UNn[ind, :] = UN[ist, :]
                RNn[ind, :] = RN[ist, :]
                WNn[ind, :] = WN[ist, :]

        # Fix arrival rates for ClassSwitch and Sink nodes for cache hit/miss classes
        # (MATLAB: getAvgNode.m lines 54-76)
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
        non-station nodes like Router/VSink.

        Returns:
            pandas.DataFrame with columns: Node, JobClass, QLen, Util, RespT, ResidT, ArvR, Tput
        """
        QNn, UNn, RNn, WNn, ANn, TNn = self.getAvgNode()

        sn = self._sn
        nodenames = list(sn.nodenames) if hasattr(sn, 'nodenames') and sn.nodenames else []
        class_names = list(sn.classnames) if hasattr(sn, 'classnames') and sn.classnames else []

        rows = []
        for node_idx in range(sn.nnodes):
            node_name = nodenames[node_idx] if node_idx < len(nodenames) else f'Node{node_idx}'

            for r in range(sn.nclasses):
                class_name = class_names[r] if r < len(class_names) else f'Class{r}'

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

    def getTranAvg(self) -> Tuple[np.ndarray, np.ndarray, np.ndarray]:
        """Get transient average metrics (not supported for NC).

        NC is a steady-state solver. Returns steady-state values.

        Returns:
            Tuple of (Q, U, T) steady-state values
        """
        if self._result is None:
            self.runAnalyzer()
        return self._result.Q, self._result.U, self._result.T

    def getAvgSys(self) -> Tuple[np.ndarray, np.ndarray]:
        """Get system-level average metrics."""
        return self.getAvgSysRespT(), self.getAvgSysTput()

    def getAvgSysTable(self) -> pd.DataFrame:
        """Get system-level metrics as DataFrame.

        Returns chain-level metrics matching MATLAB/Java implementation.
        The table includes:
        - Chain: Chain name (Chain1, Chain2, ...)
        - JobClasses: Class names within each chain
        - SysRespT: Chain response time
        - SysTput: Chain throughput
        """
        CN, XN = self.getAvgSys()

        sn = self._sn
        if sn is None:
            sn = self.model.getStruct(True)
            self._sn = sn

        nchains = sn.nchains if hasattr(sn, 'nchains') and sn.nchains > 0 else 1
        classnames = sn.classnames if hasattr(sn, 'classnames') and sn.classnames else [f'Class{k}' for k in range(sn.nclasses)]

        rows = []
        for c in range(nchains):
            chain_name = f'Chain{c + 1}'

            # Get class names in this chain
            if c in sn.inchain:
                inchain = sn.inchain[c].flatten().astype(int)
                chain_classes = ' '.join([classnames[k] if k < len(classnames) else f'Class{k}' for k in inchain])
                chain_classes = f'({chain_classes})'
            else:
                chain_classes = ''

            rows.append({
                'Chain': chain_name,
                'JobClasses': chain_classes,
                'SysRespT': CN[c] if c < len(CN) else 0.0,
                'SysTput': XN[c] if c < len(XN) else 0.0,
            })

        df = pd.DataFrame(rows)
        if not self._table_silent:
            print(df.to_string(index=False))
        return df

    # =========================================================================
    # Matrix-Exponential Method for Open Networks
    # =========================================================================

    def me_open(self) -> Dict[str, Any]:
        """Matrix-exponential method for open networks.

        Returns dictionary with ME-based analysis results.
        """
        if self._result is None:
            self.runAnalyzer()

        # For open networks, compute ME-based metrics
        Q = self._result.Q
        U = self._result.U
        R = self._result.R
        T = self._result.T

        return {
            'Q': Q,
            'U': U,
            'R': R,
            'T': T,
            'lG': self._result.lG,
            'method': 'me_open',
        }

    # =========================================================================
    # Sampling Methods (Not Supported - Analytical Solver)
    # =========================================================================

    def sample(self, node: int, numEvents: int) -> np.ndarray:
        """Sampling not supported by NC (analytical solver)."""
        raise NotImplementedError(
            "Sampling not supported by SolverNC. "
            "Use SolverSSA or SolverLDES for simulation-based analysis."
        )

    # =========================================================================
    # Aliases
    # =========================================================================

    GetProb = getProb
    GetProbAggr = getProbAggr
    GetProbMarg = getProbMarg
    GetProbSys = getProbSys
    GetProbSysAggr = getProbSysAggr
    GetAvg = getAvg
    GetAvgTable = getAvgTable
    GetAvgQLen = getAvgQLen
    GetAvgUtil = getAvgUtil
    GetAvgRespT = getAvgRespT
    GetAvgResidT = getAvgResidT
    GetAvgWaitT = getAvgWaitT
    GetAvgTput = getAvgTput
    GetAvgArvR = getAvgArvR
    GetAvgSysRespT = getAvgSysRespT
    GetAvgSysTput = getAvgSysTput
    GetAvgChain = getAvgChain
    GetAvgChainTable = getAvgChainTable
    GetAvgNode = getAvgNode
    GetAvgNodeTable = getAvgNodeTable
    GetAvgNodeChain = getAvgNodeChain
    GetAvgNodeChainTable = getAvgNodeChainTable
    GetAvgSys = getAvgSys
    GetAvgSysTable = getAvgSysTable
    GetAvgQLenChain = getAvgQLenChain
    GetAvgUtilChain = getAvgUtilChain
    GetAvgRespTChain = getAvgRespTChain
    GetAvgResidTChain = getAvgResidTChain
    GetAvgTputChain = getAvgTputChain
    GetAvgArvRChain = getAvgArvRChain
    GetNormalizingConstant = getNormalizingConstant
    GetLogNormalizingConstant = getLogNormalizingConstant
    GetProbNormConstAggr = getProbNormConstAggr
    GetCdfRespT = getCdfRespT
    GetPerctRespT = getPerctRespT
    MeOpen = me_open
    ListValidMethods = listValidMethods
    GetFeatureSet = getFeatureSet
    Supports = supports
    DefaultOptions = defaultOptions
    default_options = defaultOptions
    GetTranAvg = getTranAvg

    # Node-chain specific aliases
    GetAvgNodeQLenChain = getAvgNodeQLenChain
    GetAvgNodeUtilChain = getAvgNodeUtilChain
    GetAvgNodeRespTChain = getAvgNodeRespTChain
    GetAvgNodeResidTChain = getAvgNodeResidTChain
    GetAvgNodeTputChain = getAvgNodeTputChain
    GetAvgNodeArvRChain = getAvgNodeArvRChain

    # Short aliases (MATLAB compatibility)
    aT = getAvgTable
    aNT = getAvgNodeTable
    aCT = getAvgChainTable
    aNCT = getAvgNodeChainTable
    aST = getAvgSysTable
    avgT = getAvgTable
    nodeAvgT = getAvgNodeTable
    chainAvgT = getAvgChainTable
    nodeChainAvgT = getAvgNodeChainTable
    sysAvgT = getAvgSysTable
    avg_sys_table = getAvgSysTable
    avg_node_table = getAvgNodeTable
    avg_chain_table = getAvgChainTable
    avg_node_chain_table = getAvgNodeChainTable
    run_analyzer = runAnalyzer
    get_normalizing_constant = getNormalizingConstant
    get_log_normalizing_constant = getLogNormalizingConstant
    prob = getProb
    prob_aggr = getProbAggr
    prob_marg = getProbMarg
    prob_sys = getProbSys
    prob_sys_aggr = getProbSysAggr
    prob_norm_const_aggr = getProbNormConstAggr


__all__ = ['SolverNC', 'SolverNCOptions']
