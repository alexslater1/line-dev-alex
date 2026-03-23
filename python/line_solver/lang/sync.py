"""
Synchronization actions for CTMC state space generation.

Each sync action pairs an active event (e.g., DEP at node i, class r)
with a passive event (e.g., ARV at node j, class s) with a routing probability.

Ported from MATLAB refreshSync.m and JAR Network.java:refreshSync().
"""

from dataclasses import dataclass
from typing import Union, Callable, Optional, List
from ..constants import EventType, RoutingStrategy
from .base import NodeType


@dataclass
class SyncEvent:
    """A single event in a synchronization pair."""
    event: EventType
    node: int       # 0-based node index
    job_class: int  # 0-based class index (or mode index for Transitions)
    prob: Union[float, Callable, None] = None  # routing probability or state-dep function


@dataclass
class SyncAction:
    """A pair of active + passive events that define an atomic state transition."""
    active: SyncEvent
    passive: SyncEvent


def refresh_sync(sn) -> List[SyncAction]:
    """
    Build sync actions from network structure.

    Each sync action encodes one atomic transition:
    - PHASE/LOCAL: internal phase change at a station or transition
    - READ/READ: cache read at a cache node
    - DEP/ARV: departure from one node paired with arrival at another

    Port from MATLAB refreshSync.m / JAR Network.java:refreshSync().

    Args:
        sn: NetworkStruct

    Returns:
        List of SyncAction objects
    """
    import numpy as np

    nclasses = sn.nclasses
    nnodes = sn.nnodes
    local = nnodes  # sentinel for "no passive node" (0-based: nnodes)

    sync = []

    # Build routing mask
    has_state_dep = False
    if hasattr(sn, 'isstatedep') and sn.isstatedep is not None:
        isstatedep = np.asarray(sn.isstatedep)
        if isstatedep.any():
            has_state_dep = True

    if has_state_dep and hasattr(sn, 'rtfun') and sn.rtfun is not None:
        # State-dependent routing: evaluate rtfun with empty states
        empty_state = [np.zeros((1, 0)) for _ in range(nnodes)]
        rtmask = sn.rtfun(empty_state, empty_state)
    else:
        rt = np.asarray(sn.rt)
        rtmask = np.ceil(np.abs(rt))

    for ind in range(nnodes):
        for r in range(nclasses):
            # Phase-change actions for stations with multi-phase service
            if sn.isstation[ind]:
                ist = int(sn.nodeToStation[ind])
                if hasattr(sn, 'phasessz') and sn.phasessz is not None:
                    if sn.phasessz[ist, r] > 1:
                        sync.append(SyncAction(
                            active=SyncEvent(EventType.PHASE, ind, r, float('nan')),
                            passive=SyncEvent(EventType.LOCAL, local, r, 1.0)
                        ))

            # Cache READ actions and Transition PHASE actions
            if sn.isstateful[ind]:
                nt = sn.nodetype[ind]
                nt_val = int(nt.value) if hasattr(nt, 'value') else int(nt)

                if nt_val == int(NodeType.CACHE.value if hasattr(NodeType.CACHE, 'value') else NodeType.CACHE):
                    # Cache node: check if class can read
                    if (hasattr(sn, 'nodeparam') and sn.nodeparam is not None
                            and ind in sn.nodeparam):
                        nparam = sn.nodeparam[ind]
                        pread = nparam.get('pread', None) if isinstance(nparam, dict) else getattr(nparam, 'pread', None)
                        if pread is not None and r < len(pread):
                            pr = pread[r]
                            if pr is not None and not (isinstance(pr, float) and np.isnan(pr)):
                                sync.append(SyncAction(
                                    active=SyncEvent(EventType.READ, ind, r, float('nan')),
                                    passive=SyncEvent(EventType.READ, local, r, 1.0)
                                ))

                elif nt_val == int(NodeType.TRANSITION.value if hasattr(NodeType.TRANSITION, 'value') else NodeType.TRANSITION):
                    # Transition node: phase changes per mode (only iterate once at r==0)
                    if r == 0:
                        if (hasattr(sn, 'nodeparam') and sn.nodeparam is not None
                                and ind in sn.nodeparam):
                            nparam = sn.nodeparam[ind]
                            nmodes = nparam.get('nmodes', 0) if isinstance(nparam, dict) else getattr(nparam, 'nmodes', 0)
                            for m in range(nmodes):
                                sync.append(SyncAction(
                                    active=SyncEvent(EventType.PHASE, ind, m, float('nan')),
                                    passive=SyncEvent(EventType.LOCAL, local, m, 1.0)
                                ))

                # DEP/ARV routing actions
                isf = int(sn.nodeToStateful[ind])
                for jnd in range(nnodes):
                    if sn.isstateful[jnd]:
                        jsf = int(sn.nodeToStateful[jnd])
                        for s in range(nclasses):
                            row_idx = isf * nclasses + r
                            col_idx = jsf * nclasses + s
                            if row_idx < rtmask.shape[0] and col_idx < rtmask.shape[1]:
                                p = rtmask[row_idx, col_idx]
                                if p > 0:
                                    # Determine routing probability
                                    routing_s = sn.routing[ind, s] if hasattr(sn, 'routing') and sn.routing is not None else None
                                    rs_val = int(routing_s.value) if hasattr(routing_s, 'value') else int(routing_s) if routing_s is not None else -1

                                    rrobin_val = int(RoutingStrategy.RROBIN.value) if hasattr(RoutingStrategy.RROBIN, 'value') else int(RoutingStrategy.RROBIN)
                                    wrrobin_val = int(RoutingStrategy.WRROBIN.value) if hasattr(RoutingStrategy.WRROBIN, 'value') else int(RoutingStrategy.WRROBIN)

                                    if rs_val in (rrobin_val, wrrobin_val):
                                        # State-dependent routing: store lambda
                                        def _make_rtfun_prob(sn_ref, ri, ci):
                                            def prob_fn(state_before, state_after):
                                                rt_eval = sn_ref.rtfun(state_before, state_after)
                                                return rt_eval[ri, ci]
                                            return prob_fn
                                        prob = _make_rtfun_prob(sn, row_idx, col_idx)
                                    else:
                                        # Static routing probability
                                        rt = np.asarray(sn.rt)
                                        prob = float(rt[row_idx, col_idx])

                                    sync.append(SyncAction(
                                        active=SyncEvent(EventType.DEP, ind, r, float('nan')),
                                        passive=SyncEvent(EventType.ARV, jnd, s, prob)
                                    ))

    return sync
