"""
Main afterEvent dispatch for CTMC state space generation.

This module provides the core event dispatch mechanism used by the
sync-action-based CTMC solver. Given a stateful node, an event type,
and a current state, it computes all possible successor states with
their rates and probabilities.

Port from JAR State.java:afterEvent/afterEventHashed.
"""

import numpy as np
from typing import Dict, Tuple, Optional
from ...constants import EventType, ProcessType
from ...lang.base import NodeType
from ...lang.base import SchedStrategy


def build_space_hash(sn) -> Dict[int, Dict[tuple, int]]:
    """
    Build hash maps for O(1) state lookup for each stateful node.

    Args:
        sn: NetworkStruct with sn.space populated

    Returns:
        Dict mapping stateful node index -> {state_tuple: row_index}
    """
    hash_maps = {}
    if not hasattr(sn, 'space') or sn.space is None:
        return hash_maps

    for isf in range(sn.nstateful):
        if isf in sn.space and sn.space[isf] is not None:
            space = np.atleast_2d(sn.space[isf])
            row_map = {}
            for row_idx in range(space.shape[0]):
                key = tuple(space[row_idx].astype(float))
                row_map[key] = row_idx
            hash_maps[isf] = row_map
    return hash_maps


def get_hash(sn, ind, outspace, hash_maps=None):
    """
    Find row indices of output states in the precomputed state space.

    Args:
        sn: NetworkStruct
        ind: Node index (0-based)
        outspace: Output state matrix, shape (n_rows, n_cols)
        hash_maps: Optional precomputed hash maps from build_space_hash()

    Returns:
        np.ndarray of shape (n_rows,) with hash indices, or -1 for not found
    """
    isf = int(sn.nodeToStateful[ind])
    outspace = np.atleast_2d(outspace)
    n_rows = outspace.shape[0]
    outhash = -1 * np.ones(n_rows, dtype=int)

    if hash_maps is not None and isf in hash_maps:
        row_map = hash_maps[isf]
        for i in range(n_rows):
            key = tuple(outspace[i].astype(float))
            if key in row_map:
                outhash[i] = row_map[key]
    else:
        # Fallback: linear search
        if isf in sn.space and sn.space[isf] is not None:
            space = np.atleast_2d(sn.space[isf])
            for i in range(n_rows):
                for j in range(space.shape[0]):
                    if np.array_equal(outspace[i], space[j]):
                        outhash[i] = j
                        break

    return outhash


def after_event(sn, ind, inspace, event, job_class):
    """
    Compute successor states after an event at a stateful node.

    Decomposes the state vector, dispatches to the appropriate handler
    based on node type, and returns possible output states with rates
    and probabilities.

    Port from JAR State.java:afterEvent().

    Args:
        sn: NetworkStruct
        ind: Node index (0-based)
        inspace: Input state row(s), shape (n_cols,) or (n_rows, n_cols)
        event: EventType
        job_class: Job class index (0-based)

    Returns:
        Tuple of (outspace, outrate, outprob):
        - outspace: np.ndarray of output state rows
        - outrate: np.ndarray of transition rates
        - outprob: np.ndarray of transition probabilities
    """
    inspace = np.atleast_2d(inspace).astype(float)
    M = sn.nstations
    R = sn.nclasses

    # Determine node type and decompose state
    if sn.isstation[ind]:
        ist = int(sn.nodeToStation[ind])
        K = np.array(sn.phasessz[ist], dtype=int)
        Ks = np.array(sn.phaseshift[ist], dtype=int)

        if K[job_class] == 0:
            return np.zeros((0, 0)), np.zeros((0, 0)), np.ones((1, 1))

        V = int(np.sum(sn.nvars[ind])) if sn.nvars is not None else 0
        sumK = int(np.sum(K))
        n_rows = inspace.shape[0]
        n_cols = inspace.shape[1]

        nt = sn.nodetype[ind]
        nt_val = int(nt.value) if hasattr(nt, 'value') else int(nt)
        place_val = int(NodeType.PLACE.value) if hasattr(NodeType.PLACE, 'value') else int(NodeType.PLACE)

        if nt_val == place_val:
            # Place node: [buffer(R) | server(sumK)]
            space_var = np.zeros((n_rows, 0))
            space_buf = inspace[:, :R]
            space_srv = inspace[:, R:R + sumK] if n_cols >= R + sumK else np.zeros((n_rows, sumK))
        else:
            # Regular station: [buf | srv | var]
            if V > 0:
                space_var = inspace[:, -V:]
            else:
                space_var = np.zeros((n_rows, 0))
            srv_start = n_cols - sumK - V
            srv_end = n_cols - V
            space_srv = inspace[:, srv_start:srv_end]
            buf_end = srv_start
            space_buf = inspace[:, :buf_end] if buf_end > 0 else np.zeros((n_rows, 0))

        hasOnlyExp = int(np.max(K)) == 1

        # Build parameter dicts
        ismkvmodclass = np.zeros(R)
        if hasattr(sn, 'procid') and sn.procid is not None:
            for r in range(R):
                pt = sn.procid.get(ist, {}).get(r, None) if isinstance(sn.procid, dict) else None
                if pt is not None:
                    pt_val = int(pt.value) if hasattr(pt, 'value') else int(pt)
                    map_val = int(ProcessType.MAP.value) if hasattr(ProcessType.MAP, 'value') else int(ProcessType.MAP)
                    if pt_val == map_val:
                        ismkvmodclass[r] = 1

        lldscaling = sn.lldscaling if hasattr(sn, 'lldscaling') and sn.lldscaling is not None else None
        if lldscaling is None or (isinstance(lldscaling, np.ndarray) and lldscaling.size == 0):
            lldlimit = max(int(sn.nclosedjobs), 1) if hasattr(sn, 'nclosedjobs') else 1
            lldscaling = np.ones((M, lldlimit))
        else:
            lldlimit = lldscaling.shape[1] if isinstance(lldscaling, np.ndarray) and lldscaling.ndim >= 2 else 1

        cdscaling = sn.cdscaling if hasattr(sn, 'cdscaling') else None

        from .after_event_station import after_event_station
        return after_event_station(
            sn, ind, inspace, event, job_class,
            M, R, ist, K, Ks, hasOnlyExp,
            sn.mu, sn.phi, sn.pie, sn.proc, ismkvmodclass,
            lldscaling, lldlimit, cdscaling,
            sn.cap if hasattr(sn, 'cap') else None,
            sn.classcap if hasattr(sn, 'classcap') else None,
            V, space_buf, space_srv, space_var)

    elif sn.isstateful[ind]:
        V = int(np.sum(sn.nvars[ind])) if sn.nvars is not None else 0
        n_rows = inspace.shape[0]
        n_cols = inspace.shape[1]

        nt = sn.nodetype[ind]
        nt_val = int(nt.value) if hasattr(nt, 'value') else int(nt)
        router_val = int(NodeType.ROUTER.value) if hasattr(NodeType.ROUTER, 'value') else int(NodeType.ROUTER)
        cache_val = int(NodeType.CACHE.value) if hasattr(NodeType.CACHE, 'value') else int(NodeType.CACHE)
        trans_val = int(NodeType.TRANSITION.value) if hasattr(NodeType.TRANSITION, 'value') else int(NodeType.TRANSITION)

        # State variable extraction (always at the end)
        if V > 0:
            space_var = inspace[:, -V:]
        else:
            space_var = np.zeros((n_rows, 0))

        if nt_val == trans_val:
            # Transition: [idle(nmodes) | enabled(sumK) | var]
            nparam = sn.nodeparam[ind] if sn.nodeparam is not None and ind in sn.nodeparam else None
            nmodes = nparam.get('nmodes', 0) if isinstance(nparam, dict) else getattr(nparam, 'nmodes', 0)

            # Get K (firing phases per mode) from nodeparam
            fp = nparam.get('firingphases', None) if isinstance(nparam, dict) else getattr(nparam, 'firingphases', None)
            if fp is not None:
                K = np.atleast_1d(fp).astype(int)
            else:
                K = np.ones(nmodes, dtype=int)
            Ks = np.concatenate([[0], np.cumsum(K[:-1])]).astype(int) if len(K) > 1 else np.array([0], dtype=int)

            space_buf = inspace[:, :nmodes]
            space_srv = inspace[:, nmodes:nmodes + int(np.sum(K))]

            from .after_event_transition import after_event_transition
            return after_event_transition(
                sn, ind, event, job_class, inspace,
                K, Ks, space_buf, space_srv, space_var)

        else:
            # Router, Cache, other: [srv(R) | var]
            space_buf = np.zeros((n_rows, 0))
            srv_start = max(0, n_cols - R - V)
            srv_end = n_cols - V
            if srv_start >= 0 and srv_end > srv_start:
                space_srv = inspace[:, srv_start:srv_end]
            else:
                space_srv = np.zeros((n_rows, R))

            if nt_val == router_val:
                from .after_event_router import after_event_router
                return after_event_router(
                    sn, ind, event, job_class,
                    space_buf, space_srv, space_var)

            elif nt_val == cache_val:
                from .after_event_cache import after_event_cache
                return after_event_cache(
                    sn, ind, event, job_class, R,
                    space_buf, space_srv, space_var)

    # Stateless node: no state change
    return np.zeros((0, 0)), np.zeros((0, 0)), np.ones((1, 1))


def after_event_hashed(sn, ind, inhash, event, job_class, hash_maps=None):
    """
    Hash-based wrapper for afterEvent.

    Looks up the state from the hash index, calls afterEvent, then
    converts output states back to hash indices.

    Port from JAR State.java:afterEventHashed().

    Args:
        sn: NetworkStruct
        ind: Node index (0-based)
        inhash: Input state hash (row index in sn.space[isf])
        event: EventType
        job_class: Job class index (0-based)
        hash_maps: Optional precomputed hash maps

    Returns:
        Tuple of (outhash, outrate, outprob):
        - outhash: np.ndarray of output hash indices (-1 if not found)
        - outrate: np.ndarray of rates
        - outprob: np.ndarray of probabilities
    """
    if inhash < 0:
        return np.array([-1]), np.array([0.0]), np.array([0.0])

    isf = int(sn.nodeToStateful[ind])
    if isf not in sn.space or sn.space[isf] is None:
        return np.array([-1]), np.array([0.0]), np.array([0.0])

    space = np.atleast_2d(sn.space[isf])
    if inhash >= space.shape[0]:
        return np.array([-1]), np.array([0.0]), np.array([0.0])

    inspace = space[inhash:inhash + 1]

    outspace, outrate, outprob = after_event(sn, ind, inspace, event, job_class)

    if outspace.size == 0:
        return np.array([-1]), np.array([0.0]), np.array([0.0])

    outhash = get_hash(sn, ind, outspace, hash_maps)
    outrate = outrate.ravel() if outrate.size > 0 else np.array([0.0])
    outprob = outprob.ravel() if outprob.size > 0 else np.array([0.0])

    return outhash, outrate, outprob
