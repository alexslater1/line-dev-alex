"""
CTMC State Space Generator for sync-action-based CTMC builder.

Builds per-node state spaces (sn.space) and global state space with hashing.
Port from MATLAB ctmc_ssg.m + spaceGeneratorNodes.m + spaceGenerator.m.
"""

import numpy as np
from typing import Tuple, Optional, Dict, List
from itertools import product as iproduct
from .marginal import fromMarginal, toMarginal
from ...lang.base import NodeType, SchedStrategy, RoutingStrategy
from ...constants import GlobalConstants


def ctmc_ssg(sn, cutoff, options=None):
    """
    Build per-node state spaces and global hashed state space for CTMC.

    Port from MATLAB ctmc_ssg.m.

    Args:
        sn: NetworkStruct
        cutoff: Population cutoff for open classes (scalar or matrix)
        options: Optional solver options dict

    Returns:
        Tuple of (state_space, state_space_aggr, state_space_hashed, sn):
        - state_space: Full state space (rows = global states, cols = concatenated per-node states)
        - state_space_aggr: Aggregated state space (rows = states, cols = M*K per-station per-class counts)
        - state_space_hashed: Hashed state space (rows = states, cols = nstateful, values = row indices in sn.space)
        - sn: Updated NetworkStruct with sn.space populated
    """
    if options is None:
        options = {}

    R = sn.nclasses
    M = sn.nstations

    # Expand cutoff to matrix
    if np.isscalar(cutoff):
        cutoff_matrix = np.full((M, R), cutoff, dtype=float)
    else:
        cutoff_matrix = np.atleast_2d(cutoff).astype(float)
        if cutoff_matrix.shape[0] == 1 and M > 1:
            cutoff_matrix = np.tile(cutoff_matrix, (M, 1))

    # Step 1: Build per-node state spaces
    capacityc = _space_generator_nodes(sn, cutoff_matrix, options)

    # Step 2: Build global state space from per-node spaces
    N = sn.njobs.flatten() if sn.njobs is not None else np.ones(R)
    Np = N.copy()
    is_open_class = np.isinf(Np)
    for r in range(R):
        if is_open_class[r]:
            Np[r] = np.max(capacityc[:, r])

    state_space, state_space_hashed = _build_global_states(sn, Np, is_open_class, capacityc)

    # Step 3: Build aggregated state space (per-station per-class job counts)
    state_space_aggr = _build_state_space_aggr(sn, state_space_hashed)

    return state_space, state_space_aggr, state_space_hashed, sn


def _space_generator_nodes(sn, cutoff_matrix, options):
    """
    Build per-node state spaces sn.space[isf] for each stateful node.

    Port from MATLAB spaceGeneratorNodes.m.

    Args:
        sn: NetworkStruct
        cutoff_matrix: Cutoff matrix (M x R)
        options: Solver options

    Returns:
        capacityc: Per-node per-class capacity matrix (nnodes x R)
    """
    R = sn.nclasses
    M = sn.nstations
    N = sn.njobs.flatten() if sn.njobs is not None else np.ones(R)

    sn.space = {}
    capacityc = np.zeros((sn.nnodes, R))

    for ind in range(sn.nnodes):
        if sn.isstation[ind]:
            ist = int(sn.nodeToStation[ind])
            isf = int(sn.nodeToStateful[ind])

            for r in range(R):
                c = _find_chain(sn, r)

                # Check visits
                if c is not None and sn.visits is not None and c in sn.visits:
                    v = sn.visits[c]
                    if v is not None and isf < v.shape[0] and v[isf, r] == 0:
                        capacityc[ind, r] = 0
                        continue

                # Check disabled process
                if _is_disabled_proc(sn, ist, r):
                    capacityc[ind, r] = 0
                    continue

                if np.isinf(N[r]):
                    cc = cutoff_matrix[ist, r] if ist < cutoff_matrix.shape[0] else cutoff_matrix[0, r]
                    capacityc[ind, r] = min(cc, sn.classcap[ist, r])
                else:
                    if c is not None:
                        chain_mask = sn.chains[c].astype(bool)
                        capacityc[ind, r] = np.sum(N[chain_mask])
                    else:
                        capacityc[ind, r] = N[r]

            # Generate per-node state space including local variables
            # Keep infinite capacity for Source (EXT) stations
            if np.isfinite(sn.cap[ist]):
                cap_val = int(sn.cap[ist])
            else:
                sched = sn.sched[ist] if hasattr(sn, 'sched') else None
                ext_val = int(SchedStrategy.EXT.value) if hasattr(SchedStrategy.EXT, 'value') else int(SchedStrategy.EXT)
                sched_val = int(sched.value) if hasattr(sched, 'value') else int(sched) if sched is not None else -1
                if sched_val == ext_val:
                    cap_val = float('inf')  # Source: keep infinite capacity
                else:
                    cap_val = int(np.sum(capacityc[ind]))
            sn.space[isf] = _from_marginal_bounds(
                sn, ind, capacityc[ind].astype(int), cap_val, options)

            if np.isinf(sn.nservers[ist]):
                sn.nservers[ist] = int(np.sum(capacityc[ind]))

        elif sn.isstateful[ind]:
            isf = int(sn.nodeToStateful[ind])
            nt_val = _get_nodetype_val(sn, ind)
            cache_val = int(NodeType.CACHE.value) if hasattr(NodeType.CACHE, 'value') else int(NodeType.CACHE)
            router_val = int(NodeType.ROUTER.value) if hasattr(NodeType.ROUTER, 'value') else int(NodeType.ROUTER)

            if nt_val == cache_val:
                capacityc[ind, :] = 1
            elif nt_val == router_val:
                for r in range(R):
                    c = _find_chain(sn, r)
                    if c is not None and sn.nodevisits is not None and c in sn.nodevisits:
                        nv = sn.nodevisits[c]
                        if nv is not None and ind < nv.shape[0] and nv[ind, r] > 0:
                            capacityc[ind, r] = 1
            else:
                capacityc[ind, :] = 1

            state_bufsrv = _from_marginal_bounds(
                sn, ind, capacityc[ind].astype(int), 1, options)
            state_var = _space_local_vars(sn, ind)

            if state_var is not None and state_var.size > 0:
                sn.space[isf] = _cartesian_2d(state_bufsrv, state_var)
            else:
                sn.space[isf] = state_bufsrv

    return capacityc


def _from_marginal_bounds(sn, ind, ub, cap, options=None):
    """
    Generate all states within marginal bounds.

    Port from MATLAB fromMarginalBounds.m.
    Iterates over all marginals n with 0 <= n[r] <= ub[r] for each class r,
    calls fromMarginal for each, and filters by capacity.

    Args:
        sn: NetworkStruct
        ind: Node index (0-based)
        ub: Upper bounds per class (array of length R)
        cap: Total capacity at this node
        options: Solver options

    Returns:
        State space matrix (n_states x n_cols)
    """
    R = sn.nclasses
    ub = np.atleast_1d(ub).astype(int)

    # Generate all marginals within bounds
    ranges = [range(int(ub[r]) + 1) for r in range(R)]
    space_list = []
    max_cols = 0

    for n_tuple in iproduct(*ranges):
        n = np.array(n_tuple, dtype=float)
        state = fromMarginal(sn, ind, n, options)
        if isinstance(state, np.ndarray) and state.size > 0:
            state = np.atleast_2d(state)
            if state.shape[1] > max_cols:
                max_cols = state.shape[1]
            space_list.append(state)

    if not space_list:
        return np.zeros((1, max(R, 1)))

    # Pad to same width (left-pad with zeros, matching MATLAB behavior)
    padded = []
    for s in space_list:
        if s.shape[1] < max_cols:
            pad = np.zeros((s.shape[0], max_cols - s.shape[1]))
            s = np.hstack([pad, s])
        padded.append(s)

    space = np.vstack(padded)
    space = np.unique(space, axis=0)

    # Filter by capacity constraints
    if sn.isstateful[ind] and space.shape[0] > 0:
        keep = []
        ist = int(sn.nodeToStation[ind]) if sn.isstation[ind] else -1
        for s_idx in range(space.shape[0]):
            try:
                ni_nir = toMarginal(sn, ind, space[s_idx:s_idx + 1])
                ni = ni_nir[0]
                nir = ni_nir[1]
                ni_val = float(ni.ravel()[0]) if isinstance(ni, np.ndarray) else float(ni)
                nir_vec = nir.ravel() if isinstance(nir, np.ndarray) else np.atleast_1d(nir)

                if sn.isstation[ind]:
                    if np.all(nir_vec[:R] <= sn.classcap[ist, :R]) and ni_val <= cap:
                        keep.append(s_idx)
                else:
                    if ni_val <= cap:
                        keep.append(s_idx)
            except Exception:
                keep.append(s_idx)  # Keep if toMarginal fails

        if keep:
            space = space[keep]
        else:
            return np.zeros((1, max_cols))

    # Reverse order so states with jobs in phase 1 come first
    space = space[::-1]
    return space


def _space_local_vars(sn, ind):
    """
    Generate state space for local state variables at a node.

    Port from MATLAB spaceLocalVars.m.

    Args:
        sn: NetworkStruct
        ind: Node index (0-based)

    Returns:
        State space matrix for local variables, or None if no local vars
    """
    R = sn.nclasses
    space = None

    # Cache local variables (item positions in cache lists)
    nt_val = _get_nodetype_val(sn, ind)
    cache_val = int(NodeType.CACHE.value) if hasattr(NodeType.CACHE, 'value') else int(NodeType.CACHE)

    if nt_val == cache_val:
        nparam = sn.nodeparam[ind] if sn.nodeparam is not None and ind in sn.nodeparam else None
        if nparam is not None:
            n_items = nparam.get('nitems', 0) if isinstance(nparam, dict) else getattr(nparam, 'nitems', 0)
            m = nparam.get('itemcap', []) if isinstance(nparam, dict) else getattr(nparam, 'itemcap', [])
            m = np.atleast_1d(m).astype(int)
            if n_items > 0 and len(m) > 0:
                space = _space_cache(n_items, m)

    # Round-robin routing variables
    if hasattr(sn, 'routing') and sn.routing is not None:
        for r in range(R):
            routing_val = None
            if isinstance(sn.routing, np.ndarray) and ind < sn.routing.shape[0] and r < sn.routing.shape[1]:
                routing_val = sn.routing[ind, r]
            elif isinstance(sn.routing, dict):
                routing_val = sn.routing.get((ind, r), None)

            if routing_val is not None:
                rv = int(routing_val.value) if hasattr(routing_val, 'value') else int(routing_val)
                rr_val = int(RoutingStrategy.RROBIN.value) if hasattr(RoutingStrategy.RROBIN, 'value') else int(RoutingStrategy.RROBIN)
                wrr_val = int(RoutingStrategy.WRROBIN.value) if hasattr(RoutingStrategy.WRROBIN, 'value') else int(RoutingStrategy.WRROBIN)

                if rv in (rr_val, wrr_val):
                    nparam = sn.nodeparam[ind] if sn.nodeparam is not None and ind in sn.nodeparam else None
                    if nparam is not None:
                        # Get outlinks for this class
                        if isinstance(nparam, dict):
                            outlinks_all = nparam.get('outlinks', None)
                        elif isinstance(nparam, (list, tuple)):
                            outlinks_all = nparam[r] if r < len(nparam) else None
                            if isinstance(outlinks_all, dict):
                                outlinks_all = outlinks_all.get('outlinks', None)
                            else:
                                outlinks_all = getattr(outlinks_all, 'outlinks', None)
                        else:
                            outlinks_all = getattr(nparam, 'outlinks', None)

                        if outlinks_all is not None:
                            if isinstance(outlinks_all, (list, tuple)):
                                ol = np.atleast_1d(outlinks_all[r]) if r < len(outlinks_all) else np.array([])
                            else:
                                ol = np.atleast_1d(outlinks_all)

                            if ol.size > 0:
                                rr_space = ol.reshape(-1, 1)
                                if space is not None:
                                    space = _cartesian_2d(space, rr_space)
                                else:
                                    space = rr_space

    return space


def _space_cache(n, m):
    """
    Generate all cache state vectors.

    Port from MATLAB spaceCache.m.
    Items are 1-indexed. Each position in the cache holds one item.
    The cache has h lists with sizes m[0], m[1], ..., m[h-1].

    Args:
        n: Total number of items
        m: Array of list capacities

    Returns:
        Matrix where each row is a cache state (item IDs in positions)
    """
    from itertools import combinations, permutations

    m = np.atleast_1d(m).astype(int)
    total_slots = int(np.sum(m))

    if total_slots == 0 or n == 0:
        return np.zeros((1, max(total_slots, 1)))

    items = list(range(1, n + 1))
    states = []

    # Choose which items are in cache, then permute
    for combo in combinations(items, min(total_slots, n)):
        for perm in permutations(combo):
            states.append(list(perm))

    if not states:
        return np.zeros((1, total_slots))

    return np.array(states, dtype=float)


def _build_global_states(sn, Np, is_open_class, capacityc):
    """
    Build global state space from per-node state spaces.

    Port from MATLAB spaceGenerator.m lines 48-150.

    Args:
        sn: NetworkStruct with sn.space populated
        Np: Effective population per class (cutoff for open, actual for closed)
        is_open_class: Boolean array, True for open classes
        capacityc: Per-node capacity matrix (nnodes x R)

    Returns:
        Tuple of (state_space, state_space_hashed)
    """
    R = sn.nclasses
    nstateful = sn.nstateful
    is_closed_class = ~is_open_class

    # Count non-Source stateful nodes
    source_val = int(NodeType.SOURCE.value) if hasattr(NodeType.SOURCE, 'value') else int(NodeType.SOURCE)
    n_sources = 0
    for ind in range(sn.nnodes):
        nt_val = _get_nodetype_val(sn, ind)
        if nt_val == source_val:
            n_sources += 1
    nstateful_p = nstateful - n_sources

    # Generate chain-station positions
    chain_station_pos = _generate_chain_station_positions(sn, Np, is_open_class, is_closed_class, nstateful_p)

    if chain_station_pos is None or len(chain_station_pos) == 0:
        # Fallback: simple cartesian product approach
        return _build_global_states_cartesian(sn, Np, is_open_class)

    # For each chain-station position, find compatible per-node state hashes
    netstates = {}  # netstates[j][isf] = list of hash indices
    for j in range(chain_station_pos.shape[0]):
        netstates[j] = {}
        for ind in range(sn.nnodes):
            if not sn.isstateful[ind]:
                continue
            isf = int(sn.nodeToStateful[ind])
            nt_val = _get_nodetype_val(sn, ind)

            if nt_val == source_val:
                # Source: fixed state
                if isf in sn.space and sn.space[isf] is not None:
                    state_i = fromMarginal(sn, ind, [])
                    hashes = _get_hash_list(sn, ind, state_i)
                    netstates[j][isf] = hashes
                else:
                    netstates[j][isf] = [0]

            elif sn.isstation[ind]:
                # Station: extract marginal from chain_station_pos
                marg = _extract_marginal(sn, ind, j, chain_station_pos, nstateful_p, n_sources)
                if marg is not None and np.any(marg > capacityc[ind, :R]):
                    netstates[j][isf] = []
                else:
                    if marg is not None:
                        state_i = fromMarginal(sn, ind, marg)
                    else:
                        state_i = np.array([])
                    hashes = _get_hash_list(sn, ind, state_i)
                    netstates[j][isf] = hashes

            elif sn.isstateful[ind]:
                # Non-station stateful: match marginal from space
                marg = _extract_marginal(sn, ind, j, chain_station_pos, nstateful_p, n_sources)
                if marg is not None and np.any(marg > capacityc[ind, :R]):
                    netstates[j][isf] = []
                else:
                    if isf in sn.space and sn.space[isf] is not None:
                        space_isf = np.atleast_2d(sn.space[isf])
                        if marg is not None:
                            # Filter: keep rows where first R cols match marginal
                            matching = []
                            for row_idx in range(space_isf.shape[0]):
                                row_marg = space_isf[row_idx, :len(marg)]
                                if np.allclose(row_marg, marg):
                                    matching.append(row_idx)
                            netstates[j][isf] = matching if matching else []
                        else:
                            netstates[j][isf] = list(range(space_isf.shape[0]))
                    else:
                        netstates[j][isf] = [0]

    # Combine: cartesian product of per-node state hash sets
    SS_rows = []
    SSh_rows = []

    for j in range(chain_station_pos.shape[0]):
        # Build list of hash arrays for each stateful node
        hash_lists = []
        for isf in range(nstateful):
            if isf in netstates[j] and netstates[j][isf]:
                hash_lists.append(netstates[j][isf])
            elif isf in sn.space and sn.space[isf] is not None:
                hash_lists.append([0])  # Default to first state
            else:
                hash_lists.append([0])

        # Cartesian product
        for combo in iproduct(*hash_lists):
            skip = False
            h_row = []
            u_row = []
            for isf_idx in range(nstateful):
                h_idx = combo[isf_idx]
                if h_idx < 0:
                    skip = True
                    break
                h_row.append(h_idx)
                if isf_idx in sn.space and sn.space[isf_idx] is not None:
                    space_isf = np.atleast_2d(sn.space[isf_idx])
                    if h_idx < space_isf.shape[0]:
                        u_row.extend(space_isf[h_idx].tolist())
                    else:
                        skip = True
                        break

            if not skip:
                SS_rows.append(u_row)
                SSh_rows.append(h_row)

    if not SS_rows:
        return np.zeros((0, 0)), np.zeros((0, nstateful), dtype=int)

    SS = np.array(SS_rows, dtype=float)
    SSh = np.array(SSh_rows, dtype=int)

    # Remove duplicates
    _, unique_idx = np.unique(SS, axis=0, return_index=True)
    unique_idx = np.sort(unique_idx)
    SS = SS[unique_idx]
    SSh = SSh[unique_idx]

    return SS, SSh


def _build_global_states_cartesian(sn, Np, is_open_class):
    """
    Fallback: build global state space by simple cartesian product with population filtering.

    Args:
        sn: NetworkStruct with sn.space populated
        Np: Population limits per class
        is_open_class: Boolean array

    Returns:
        Tuple of (state_space, state_space_hashed)
    """
    R = sn.nclasses
    nstateful = sn.nstateful
    is_closed = ~is_open_class

    # Build list of per-node state index ranges
    index_ranges = []
    for isf in range(nstateful):
        if isf in sn.space and sn.space[isf] is not None:
            n_states = np.atleast_2d(sn.space[isf]).shape[0]
            index_ranges.append(range(n_states))
        else:
            index_ranges.append(range(1))

    SS_rows = []
    SSh_rows = []

    for combo in iproduct(*index_ranges):
        # Build full state and check population constraints
        u_row = []
        class_totals = np.zeros(R)
        valid = True

        for isf_idx in range(nstateful):
            h_idx = combo[isf_idx]
            if isf_idx in sn.space and sn.space[isf_idx] is not None:
                space_isf = np.atleast_2d(sn.space[isf_idx])
                state_row = space_isf[h_idx]
                u_row.extend(state_row.tolist())

                # Extract marginal to check population
                node_ind = int(sn.statefulToNode[isf_idx]) if hasattr(sn, 'statefulToNode') else -1
                if node_ind >= 0:
                    try:
                        result = toMarginal(sn, node_ind, state_row.reshape(1, -1))
                        nir = result[1]
                        nir_vec = nir.ravel()[:R]
                        class_totals += nir_vec
                    except Exception:
                        pass

        # Check population constraints
        for r in range(R):
            if is_closed[r]:
                if abs(class_totals[r] - Np[r]) > 0.5:
                    valid = False
                    break
            else:
                if class_totals[r] > Np[r] + 0.5:
                    valid = False
                    break

        if valid:
            SS_rows.append(u_row)
            SSh_rows.append(list(combo))

    if not SS_rows:
        return np.zeros((0, 0)), np.zeros((0, nstateful), dtype=int)

    SS = np.array(SS_rows, dtype=float)
    SSh = np.array(SSh_rows, dtype=int)

    # Remove duplicates
    _, unique_idx = np.unique(SS, axis=0, return_index=True)
    unique_idx = np.sort(unique_idx)
    SS = SS[unique_idx]
    SSh = SSh[unique_idx]

    return SS, SSh


def _build_state_space_aggr(sn, state_space_hashed):
    """
    Build aggregated state space (per-station per-class job counts).

    Port from MATLAB/JAR ctmc_ssg aggregation loop.

    Args:
        sn: NetworkStruct
        state_space_hashed: Hashed state space (n_states x nstateful)

    Returns:
        state_space_aggr: Aggregated matrix (n_states x M*R)
    """
    if state_space_hashed.size == 0:
        return np.zeros((0, sn.nstations * sn.nclasses))

    M = sn.nstations
    R = sn.nclasses
    n_states = state_space_hashed.shape[0]
    aggr = np.zeros((n_states, M * R))

    for s in range(n_states):
        for ind in range(sn.nnodes):
            if not sn.isstateful[ind] or not sn.isstation[ind]:
                continue
            isf = int(sn.nodeToStateful[ind])
            ist = int(sn.nodeToStation[ind])

            if isf not in sn.space or sn.space[isf] is None:
                continue

            h_idx = int(state_space_hashed[s, isf])
            space_isf = np.atleast_2d(sn.space[isf])
            if h_idx >= space_isf.shape[0]:
                continue

            state_row = space_isf[h_idx:h_idx + 1]
            try:
                result = toMarginal(sn, ind, state_row)
                nir = result[1]
                nir_vec = nir.ravel()[:R]
                aggr[s, ist * R:(ist + 1) * R] = nir_vec
            except Exception:
                pass

    return aggr


def _generate_chain_station_positions(sn, Np, is_open, is_closed, nstateful_p):
    """
    Generate all valid job distributions across stateful nodes.

    Port from MATLAB spaceGenerator.m lines 48-82.

    Args:
        sn: NetworkStruct
        Np: Population per class
        is_open: Boolean array for open classes
        is_closed: Boolean array for closed classes
        nstateful_p: Number of stateful nodes excluding sources

    Returns:
        Matrix of chain-station positions (n_positions x nstateful_p * R)
    """
    R = sn.nclasses

    if nstateful_p == 0:
        return None

    # Generate all valid distributions of closed class populations
    positions = []
    Np_int = Np.astype(int)

    # Iterate over all possible class populations (for open classes)
    ranges = []
    for r in range(R):
        ranges.append(range(int(Np_int[r]) + 1))

    for n_tuple in iproduct(*ranges):
        n = np.array(n_tuple, dtype=int)
        # Check closed class constraint: closed classes must have exact population
        if not np.all(n[is_closed] == Np_int[is_closed]):
            continue

        # Distribute n across nstateful_p nodes
        # Use spaceClosedMultiCS equivalent
        node_dists = _distribute_across_nodes(n, nstateful_p, sn.chains if hasattr(sn, 'chains') else None)
        for dist in node_dists:
            positions.append(dist)

    if not positions:
        return None

    pos_array = np.array(positions, dtype=float)
    # Remove duplicates
    pos_array = np.unique(pos_array, axis=0)
    return pos_array


def _distribute_across_nodes(n, nstateful_p, chains):
    """
    Distribute n jobs across nstateful_p nodes respecting chain constraints.

    Simplified port of MATLAB spaceClosedMultiCS.

    Args:
        n: Total jobs per class (array of length R)
        nstateful_p: Number of non-source stateful nodes
        chains: Chain membership matrix (nchains x R)

    Returns:
        List of distribution vectors, each of length nstateful_p * R
    """
    R = len(n)
    if nstateful_p == 0:
        return []

    # Simple approach: distribute each class independently across nodes
    # For class r with n[r] jobs, distribute across nstateful_p nodes
    per_class_dists = []
    for r in range(R):
        nr = int(n[r])
        # All ways to distribute nr jobs across nstateful_p nodes
        class_dists = _multichoose_constrained(nr, nstateful_p)
        per_class_dists.append(class_dists)

    # Combine class distributions
    results = []
    for combo in iproduct(*per_class_dists):
        # combo[r] is the distribution of class r across nodes
        # Build interleaved format: [node0_class0, node1_class0, ..., node0_class1, ...]
        # Actually MATLAB format: position (isf, r) = combo[r][isf]
        row = np.zeros(nstateful_p * R)
        for r in range(R):
            dist = combo[r]
            for isf in range(nstateful_p):
                # MATLAB indexing: chainStationPos(j, isf:nstateful_p:end) gives class marginals
                # So format is: [isf0_r0, isf1_r0, ..., isf0_r1, isf1_r1, ...]
                # Wait, MATLAB: stateMarg_i = chainStationPos(j, (isf-...):nstatefulp:end)
                # This picks every nstateful_p-th element starting from isf
                # So format is: [node0_class0, node0_class1, ..., node1_class0, node1_class1, ...]
                # No wait: chainStationPos(j, isf:nstatefulp:end) with nstatefulp columns apart
                # means columns isf, isf+nstatefulp, isf+2*nstatefulp, ...
                # So column c corresponds to node (c % nstatefulp), class (c // nstatefulp)
                row[r * nstateful_p + isf] = dist[isf]
        results.append(row)

    return results


def _multichoose_constrained(n, k):
    """
    Generate all ways to distribute n identical items across k bins.

    Returns list of arrays, each of length k, summing to n.
    """
    if k == 0:
        return [np.array([])] if n == 0 else []
    if k == 1:
        return [np.array([n])]
    if n == 0:
        return [np.zeros(k, dtype=int)]

    results = []
    for i in range(n + 1):
        for rest in _multichoose_constrained(n - i, k - 1):
            results.append(np.concatenate([[i], rest]).astype(int))
    return results


def _extract_marginal(sn, ind, j, chain_station_pos, nstateful_p, n_sources):
    """
    Extract per-class job counts for a node from chain_station_pos.

    Port from MATLAB spaceGenerator.m line 93:
    stateMarg_i = chainStationPos(j, (isf-n_preceding_sources):nstatefulp:end)

    Args:
        sn: NetworkStruct
        ind: Node index
        j: Position index
        chain_station_pos: Position matrix
        nstateful_p: Number of non-source stateful nodes
        n_sources: Number of source nodes

    Returns:
        Marginal array (R,) or None
    """
    isf = int(sn.nodeToStateful[ind])
    R = sn.nclasses

    # Count sources before this node
    source_val = int(NodeType.SOURCE.value) if hasattr(NodeType.SOURCE, 'value') else int(NodeType.SOURCE)
    n_preceding_sources = 0
    for i in range(ind):
        nt_val = _get_nodetype_val(sn, i)
        if nt_val == source_val:
            n_preceding_sources += 1

    # MATLAB: (isf - n_preceding_sources) is 0-based offset for non-source stateful nodes
    # In the position vector, columns are [node0_class0, node1_class0, ..., node0_class1, ...]
    # We pick every nstateful_p-th element starting from (isf - n_preceding_sources)
    col_start = isf - n_preceding_sources
    if col_start < 0 or col_start >= nstateful_p:
        return None

    try:
        marg = np.zeros(R)
        for r in range(R):
            col = r * nstateful_p + col_start
            if col < chain_station_pos.shape[1]:
                marg[r] = chain_station_pos[j, col]
        return marg
    except (IndexError, ValueError):
        return None


def _get_hash_list(sn, ind, state_i):
    """
    Find hash indices for states in sn.space[isf].

    Port from MATLAB State.getHash.

    Args:
        sn: NetworkStruct
        ind: Node index
        state_i: State rows to look up

    Returns:
        List of hash (row) indices
    """
    isf = int(sn.nodeToStateful[ind])
    if isf not in sn.space or sn.space[isf] is None:
        return [0]

    if not isinstance(state_i, np.ndarray) or state_i.size == 0:
        return []

    state_i = np.atleast_2d(state_i)
    space = np.atleast_2d(sn.space[isf])

    hashes = []
    for i in range(state_i.shape[0]):
        row = state_i[i]
        # Pad to match space width
        if len(row) < space.shape[1]:
            row = np.concatenate([np.zeros(space.shape[1] - len(row)), row])
        elif len(row) > space.shape[1]:
            row = row[-space.shape[1]:]

        found = False
        for j in range(space.shape[0]):
            if np.allclose(row, space[j], atol=1e-10):
                hashes.append(j)
                found = True
                break
        if not found:
            hashes.append(-1)

    return hashes


def _cartesian_2d(a, b):
    """
    Cartesian product of two 2D matrices (row-wise).

    For each pair of rows (a_row, b_row), produces [a_row, b_row].

    Args:
        a: Matrix (na x ca)
        b: Matrix (nb x cb)

    Returns:
        Matrix (na*nb x ca+cb)
    """
    a = np.atleast_2d(a)
    b = np.atleast_2d(b)
    na, ca = a.shape
    nb, cb = b.shape

    result = np.zeros((na * nb, ca + cb))
    idx = 0
    for i in range(na):
        for j in range(nb):
            result[idx, :ca] = a[i]
            result[idx, ca:] = b[j]
            idx += 1

    return result


# --- Helper functions ---

def _find_chain(sn, r):
    """Find chain index containing class r."""
    if not hasattr(sn, 'chains') or sn.chains is None:
        return None
    chains = np.atleast_2d(sn.chains)
    for c in range(chains.shape[0]):
        if chains[c, r]:
            return c
    return None


def _is_disabled_proc(sn, ist, r):
    """Check if service process at (ist, r) is disabled (NaN)."""
    if sn.proc is None:
        return False
    try:
        proc_ir = sn.proc[ist][r]
        if proc_ir is None:
            return False
        if isinstance(proc_ir, (list, tuple)) and len(proc_ir) > 0:
            D0 = np.atleast_2d(proc_ir[0])
            return bool(np.any(np.isnan(D0)))
        return False
    except (IndexError, KeyError, TypeError):
        return False


def _get_nodetype_val(sn, ind):
    """Get integer nodetype value for a node."""
    if not hasattr(sn, 'nodetype') or sn.nodetype is None or ind >= len(sn.nodetype):
        return -1
    nt = sn.nodetype[ind]
    return int(nt.value) if hasattr(nt, 'value') else int(nt)
