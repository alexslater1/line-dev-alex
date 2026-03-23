"""
Base class for native LINE solvers.

Provides common functionality shared across all native solver implementations.
The class hierarchy mirrors the JAR implementation:
- Solver: Base class for all solvers
- NetworkSolver(Solver): For single-network solvers (MVA, NC, CTMC, SSA, etc.)
- EnsembleSolver(Solver): For multi-model solvers (LN, Posterior, ENV)
"""

from typing import Dict, List, Optional
from ..api.io.logging import line_warning


class SolverFeatureSet:
    """
    A class to specify and check features supported by a solver.

    Mirrors MATLAB's SolverFeatureSet class. Used to track which model features
    are supported by each solver and to check compatibility.

    Attributes:
        list: Dict mapping feature names to boolean support status.
    """

    # All possible features that can be used/supported
    FIELDS = [
        'ClassSwitch',
        'Cache',
        'Delay',
        'DelayStation',
        'Fork',
        'Join',
        'Logger',
        'Place',
        'Queue',
        'Sink',
        'Source',
        'Router',
        'Transition',
        'Cox2',
        'APH',
        'Det',
        'Erlang',
        'Exp',
        'Gamma',
        'HyperExp',
        'Lognormal',
        'MAP',
        'MMPP2',
        'Normal',
        'Pareto',
        'PH',
        'Replayer',
        'Trace',
        'Uniform',
        'Weibull',
        'StatelessClassSwitcher',
        'CacheClassSwitcher',
        'InfiniteServer',
        'Forker',
        'Joiner',
        'LogTunnel',
        'SharedServer',
        'Buffer',
        'Linkage',
        'Enabling',
        'Timing',
        'Firing',
        'Storage',
        'RandomSource',
        'Dispatcher',
        'Server',
        'ServiceTunnel',
        'RoutingStrategy_PROB',
        'RoutingStrategy_RAND',
        'RoutingStrategy_RROBIN',
        'RoutingStrategy_WRROBIN',
        'RoutingStrategy_KCHOICES',
        'SchedStrategy_INF',
        'SchedStrategy_FCFS',
        'SchedStrategy_LCFS',
        'SchedStrategy_LCFSPI',
        'SchedStrategy_LCFSPR',
        'SchedStrategy_SEPT',
        'SchedStrategy_LEPT',
        'SchedStrategy_DPS',
        'SchedStrategy_GPS',
        'SchedStrategy_LJF',
        'SchedStrategy_LPS',
        'SchedStrategy_SJF',
        'SchedStrategy_SRPT',
        'SchedStrategy_SRPTPRIO',
        'SchedStrategy_PS',
        'SchedStrategy_SIRO',
        'SchedStrategy_HOL',
        'SchedStrategy_EXT',
        'SchedStrategy_POLLING',
        'ReplacementStrategy_RR',
        'ReplacementStrategy_FIFO',
        'ReplacementStrategy_SFIFO',
        'ReplacementStrategy_LRU',
        'ClosedClass',
        'OpenClass',
    ]

    def __init__(self):
        """Initialize with all features set to False."""
        self.list: Dict[str, bool] = {f: False for f in self.FIELDS}

    def set_true(self, features):
        """
        Set one or more features to True.

        Args:
            features: A single feature name (str) or list of feature names.
        """
        if isinstance(features, str):
            features = [features]
        for feat in features:
            if feat in self.list:
                self.list[feat] = True

    def set_false(self, features):
        """
        Set one or more features to False.

        Args:
            features: A single feature name (str) or list of feature names.
        """
        if isinstance(features, str):
            features = [features]
        for feat in features:
            if feat in self.list:
                self.list[feat] = False

    @staticmethod
    def supports(feat_supported: 'SolverFeatureSet', feat_used: 'SolverFeatureSet') -> bool:
        """
        Check if supported features cover all used features.

        Prints a warning if any used features are not supported.

        Args:
            feat_supported: FeatureSet of features the solver supports.
            feat_used: FeatureSet of features the model uses.

        Returns:
            True if all used features are supported, False otherwise.
        """
        unsupported = []

        for field in SolverFeatureSet.FIELDS:
            if feat_used.list.get(field, False) and not feat_supported.list.get(field, False):
                unsupported.append(field)

        if unsupported:
            feat_str = ', '.join(unsupported)
            line_warning('SolverFeatureSet',
                         f'Some features are not supported by the chosen solver (feature: {feat_str}).')
            return False

        return True

    # MATLAB-compatible aliases
    setTrue = set_true
    setFalse = set_false


class Solver:
    """Base class for all LINE solvers.

    Provides common attributes and default behaviors for all solvers.

    Attributes:
        _table_silent (bool): If True, suppress automatic table printing in
            getAvgTable() and similar methods. Defaults to True so that
            tables are only printed when explicitly requested by the user.
    """

    # Suppress automatic table printing by default
    _table_silent = True


class NetworkSolver(Solver):
    """Base class for single-network LINE solvers.

    Used by: SolverMVA, SolverNC, SolverCTMC, SolverSSA, SolverMAM, SolverJMT,
             SolverLDES, SolverQNS, SolverAuto, SolverFLD
    """

    def avg_table(self):
        """Get average performance metrics as an IndexedTable with proper formatting.

        This method wraps the raw DataFrame from getAvgTable() in an IndexedTable
        to provide MATLAB-style number formatting (e.g., 0 instead of 0.00000).

        Returns:
            IndexedTable: Wrapped DataFrame with MATLAB-style formatting.
        """
        import pandas as pd
        from ..indexed_table import IndexedTable
        result = self.getAvgTable()
        if result is None:
            return None
        # If already an IndexedTable, return as-is
        if isinstance(result, IndexedTable):
            return result
        # If a DataFrame, wrap it
        if isinstance(result, pd.DataFrame):
            return IndexedTable(result)
        return result

    get_avg_table = avg_table


class EnsembleSolver(Solver):
    """Base class for ensemble/multi-model LINE solvers.

    Used by: SolverLN, SolverPosterior, SolverENV
    """
    pass
