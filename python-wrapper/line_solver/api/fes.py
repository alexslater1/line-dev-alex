
"""
Flow-Equivalent Server (FES) aggregation functions.

This module provides FES aggregation for closed product-form queueing networks.
FES replaces a subset of stations with a single Flow-Equivalent Server whose
state-dependent service rates match the throughputs of the isolated subnetwork.

The aggregated model has fewer stations, enabling faster analysis while
preserving exact product-form performance metrics for the complement stations.

Functions:
- fes_aggregate: Aggregate a station subset into a Flow-Equivalent Server
- fes_aggregate_with_options: Aggregate with custom options (cutoffs, solver, verbose)

References:
    Chandy, K. M., Herzog, U., and Woo, L. S. (1975). Parametric analysis
    of queueing networks. IBM Journal of Research and Development, 19(1), 36-42.

    Casale, G. (2017). LINE: Large-scale Information Networking Environment.
"""

import jpype
import numpy as np
from line_solver import jlineMatrixToArray, jlineMatrixFromArray


def fes_aggregate(model, station_subset):
    """
    Aggregate a station subset into a Flow-Equivalent Server.

    Replaces the specified stations in a closed product-form queueing network
    with a single FES station. The FES has Limited Joint Class Dependence (LJCD)
    service rates computed from MVA throughputs of the isolated subnetwork
    for all reachable population vectors.

    The resulting model has (M - |subset| + 1) stations, where M is the
    original number of stations and |subset| is the size of the station subset.

    Args:
        model: LINE Network model (Python wrapper or Java Network object).
            Must be a closed product-form queueing network.
        station_subset: List of Station objects to aggregate. Each must be
            a Queue or Delay station belonging to the model. Cannot include
            all stations in the model.

    Returns:
        dict: FES aggregation result containing:
            - fes_model: New Network with the FES station replacing the subset.
            - fes_station: Reference to the FES Queue station in the new model.
            - deagg_info (dict): Deaggregation information containing:
                - original_model: Reference to the original model.
                - station_subset: List of original subset stations.
                - subset_indices (numpy.ndarray): Station indices of the subset.
                - complement_indices (numpy.ndarray): Station indices of the complement.
                - throughput_table (list of numpy.ndarray): Per-class throughput
                    tables indexed by linearized population state.
                - cutoffs (numpy.ndarray): Per-class population cutoffs used.
                - stoch_comp_subset (numpy.ndarray): Stochastic complement
                    routing matrix for the subset.
                - stoch_comp_complement (numpy.ndarray): Stochastic complement
                    routing matrix for the complement.
                - isolated_model: Isolated subnetwork model used for throughput
                    computation.
                - fes_node_idx (int): Node index of the FES station in the new model.

    Raises:
        RuntimeError: If the model is not a closed network, the subset is empty,
            the subset includes all stations, or stations are of unsupported type.

    Example:
        >>> model = Network('FES_Example')
        >>> delay = Delay(model, 'Delay')
        >>> queue1 = Queue(model, 'Queue1', SchedStrategy.FCFS)
        >>> queue2 = Queue(model, 'Queue2', SchedStrategy.FCFS)
        >>> # ... set up classes, services, routing ...
        >>> result = fes_aggregate(model, [queue1, queue2])
        >>> fes_model = result['fes_model']
        >>> # Solve the smaller FES model
        >>> solver = SolverMVA(fes_model)
    """
    return _fes_aggregate_impl(model, station_subset, None)


def fes_aggregate_with_options(model, station_subset, solver='mva',
                               cutoffs=None, verbose=False):
    """
    Aggregate a station subset into a Flow-Equivalent Server with custom options.

    This is the extended version of fes_aggregate that allows specifying
    solver type, per-class population cutoffs, and verbose output.

    Args:
        model: LINE Network model (Python wrapper or Java Network object).
            Must be a closed product-form queueing network.
        station_subset: List of Station objects to aggregate.
        solver (str): Solver to use for throughput computation. Default: 'mva'.
        cutoffs (numpy.ndarray or None): Per-class population cutoffs as a
            1 x K array where K is the number of classes. If None, uses the
            total population per class. Smaller cutoffs reduce computation
            time but may reduce accuracy.
        verbose (bool): If True, print progress information. Default: False.

    Returns:
        dict: Same structure as fes_aggregate().

    Example:
        >>> # Aggregate with reduced cutoffs for faster computation
        >>> cutoffs = np.array([[5, 5]])  # limit to 5 jobs per class
        >>> result = fes_aggregate_with_options(model, [q1, q2], cutoffs=cutoffs)
    """
    FESOptions = jpype.JPackage('jline').api.fes.FESOptions

    if cutoffs is not None:
        cutoffs_mat = jlineMatrixFromArray(np.atleast_2d(cutoffs))
        options = FESOptions(str(solver), cutoffs_mat, bool(verbose))
    else:
        options = FESOptions(str(solver), None, bool(verbose))

    return _fes_aggregate_impl(model, station_subset, options)


def _fes_aggregate_impl(model, station_subset, options):
    """
    Internal implementation for FES aggregation.

    Args:
        model: LINE Network model.
        station_subset: List of Station objects.
        options: Java FESOptions object, or None for defaults.

    Returns:
        dict: FES aggregation result.
    """
    FESAggregator = jpype.JPackage('jline').api.fes.FESAggregator
    java_model = model.obj if hasattr(model, 'obj') else model

    # Build Java List<Station> from the Python list
    java_list = jpype.java.util.ArrayList()
    for station in station_subset:
        java_station = station.obj if hasattr(station, 'obj') else station
        java_list.add(java_station)

    # Call the appropriate Java method
    if options is not None:
        result = FESAggregator.aggregateFES(java_model, java_list, options)
    else:
        result = FESAggregator.aggregateFES(java_model, java_list)

    # Extract FESResult fields
    fes_model = result.getFesModel()
    fes_station = result.getFesStation()
    deagg = result.getDeaggInfo()

    # Convert deaggregation info to Python types
    subset_indices = np.array([int(deagg.getSubsetIndices()[i])
                               for i in range(len(deagg.getSubsetIndices()))])
    complement_indices = np.array([int(deagg.getComplementIndices()[i])
                                   for i in range(len(deagg.getComplementIndices()))])

    # Convert throughput tables (Java List<Matrix> to list of numpy arrays)
    java_tput_table = deagg.getThroughputTable()
    throughput_table = []
    for k in range(java_tput_table.size()):
        throughput_table.append(jlineMatrixToArray(java_tput_table.get(k)))

    cutoffs_out = jlineMatrixToArray(deagg.getCutoffs())
    stoch_comp_subset = jlineMatrixToArray(deagg.getStochCompSubset())
    stoch_comp_complement = jlineMatrixToArray(deagg.getStochCompComplement())
    isolated_model = deagg.getIsolatedModel()
    fes_node_idx = int(deagg.getFesNodeIdx())

    deagg_info = {
        'original_model': deagg.getOriginalModel(),
        'station_subset': list(deagg.getStationSubset()),
        'subset_indices': subset_indices,
        'complement_indices': complement_indices,
        'throughput_table': throughput_table,
        'cutoffs': cutoffs_out,
        'stoch_comp_subset': stoch_comp_subset,
        'stoch_comp_complement': stoch_comp_complement,
        'isolated_model': isolated_model,
        'fes_node_idx': fes_node_idx,
    }

    return {
        'fes_model': fes_model,
        'fes_station': fes_station,
        'deagg_info': deagg_info,
    }
