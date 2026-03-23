"""
Station node event handler for afterEvent dispatch.

Handles ARV, DEP, and PHASE events at station nodes (queues, delays, sources).
This is the largest handler, covering all scheduling strategies.

Port from JAR AfterEventStation.java.
"""

import numpy as np
from ...constants import EventType, GlobalConstants, ProcessType
from ...lang.base import SchedStrategy, NodeType


def after_event_station(sn, ind, inspace, event, job_class,
                        M, R, ist, K, Ks, hasOnlyExp,
                        mu, phi, pie, proc, ismkvmodclass,
                        lldscaling, lldlimit, cdscaling,
                        capacity, classcap, V,
                        space_buf, space_srv, space_var):
    """
    Handle events at a station node.

    Args:
        sn: NetworkStruct
        ind: Node index (0-based)
        inspace: Full input state, shape (n_rows, n_cols)
        event: EventType (ARV, DEP, or PHASE)
        job_class: Job class index (0-based)
        M: Number of stations
        R: Number of classes
        ist: Station index
        K: Phase counts per class, shape (R,)
        Ks: Phase shifts per class, shape (R,)
        hasOnlyExp: True if all processes are exponential (single-phase)
        mu: Service rates, mu[ist][r][k]
        phi: Completion probabilities, phi[ist][r][k]
        pie: Entry probabilities, pie[ist][r][k]
        proc: Process matrices, proc[ist][r] = [D0, D1]
        ismkvmodclass: MAP/MMPP2 flags per class, shape (R,)
        lldscaling: Load-level-dependent scaling, shape (M, lldlimit)
        lldlimit: Max load level for scaling
        cdscaling: Class-dependent scaling functions
        capacity: Station capacities, shape (M,)
        classcap: Per-class capacities, shape (M, R)
        V: Number of state variables
        space_buf: Buffer state
        space_srv: Server state
        space_var: Variable state

    Returns:
        Tuple of (outspace, outrate, outprob)
    """
    S = int(sn.nservers[ist])
    sched = sn.sched[ist]
    K = np.atleast_1d(K).astype(int)
    Ks = np.atleast_1d(Ks).astype(int)

    if event == EventType.ARV:
        return _handle_arv(sn, ind, inspace, job_class, M, R, ist, S, K, Ks,
                           hasOnlyExp, mu, phi, pie, proc, ismkvmodclass,
                           lldscaling, lldlimit, cdscaling, capacity, classcap,
                           V, sched, space_buf, space_srv, space_var)
    elif event == EventType.DEP:
        return _handle_dep(sn, ind, inspace, job_class, M, R, ist, S, K, Ks,
                           hasOnlyExp, mu, phi, pie, proc, ismkvmodclass,
                           lldscaling, lldlimit, cdscaling, capacity, classcap,
                           V, sched, space_buf, space_srv, space_var)
    elif event == EventType.PHASE:
        return _handle_phase(sn, ind, inspace, job_class, M, R, ist, S, K, Ks,
                             hasOnlyExp, mu, phi, pie, proc, ismkvmodclass,
                             lldscaling, lldlimit, cdscaling,
                             V, sched, space_buf, space_srv, space_var)

    return np.zeros((0, 0)), np.zeros((0, 0)), np.zeros((0, 0))


# ---------------------------------------------------------------------------
# ARV handler
# ---------------------------------------------------------------------------

def _handle_arv(sn, ind, inspace, job_class, M, R, ist, S, K, Ks,
                hasOnlyExp, mu, phi, pie, proc, ismkvmodclass,
                lldscaling, lldlimit, cdscaling, capacity, classcap,
                V, sched, space_buf, space_srv, space_var):
    """Handle arrival events."""
    from .marginal import toMarginalAggr
    ni, nir = toMarginalAggr(sn, ind, inspace, K, Ks, space_buf, space_srv, space_var)

    # Get phase entry probabilities
    pentry = _get_pie(pie, ist, job_class, K)

    out_states = []
    out_rates = []
    out_probs = []

    for kentry in range(int(K[job_class])):
        space_buf_k = space_buf.copy()
        space_srv_k = space_srv.copy()
        space_var_k = space_var.copy() if space_var.size > 0 else space_var

        n_rows = space_srv_k.shape[0]
        outprob_k = np.full(n_rows, pentry[kentry])
        valid = np.ones(n_rows, dtype=bool)

        if sched == SchedStrategy.EXT:
            # Source: virtual arrival, rate=0, state unchanged
            outrate_k = np.zeros(n_rows)
            # Only one phase iteration needed for EXT
            for i in range(n_rows):
                if valid[i]:
                    state_row = _compose_state(space_buf_k[i] if space_buf_k.ndim >= 2 else space_buf_k,
                                               space_srv_k[i] if space_srv_k.ndim >= 2 else space_srv_k,
                                               space_var_k[i] if space_var_k.ndim >= 2 and space_var_k.size > 0 else space_var_k)
                    out_states.append(state_row)
                    out_rates.append(-1.0)
                    out_probs.append(1.0)
            return _finalize(out_states, out_rates, out_probs, inspace)

        elif sched in (SchedStrategy.INF, SchedStrategy.PS, SchedStrategy.DPS,
                        SchedStrategy.GPS, SchedStrategy.PSPRIO):
            # Jobs enter service immediately
            for i in range(n_rows):
                if classcap is not None and nir[i, job_class] >= classcap[ist, job_class]:
                    valid[i] = False
                    continue
                space_srv_k[i, int(Ks[job_class]) + kentry] += 1

        elif sched in (SchedStrategy.SIRO,):
            # Idle servers: enter service; busy: go to buffer
            for i in range(n_rows):
                if ni[i] < S:
                    space_srv_k[i, int(Ks[job_class]) + kentry] += 1
                else:
                    if space_buf_k.ndim >= 2 and job_class < space_buf_k.shape[1]:
                        space_buf_k[i, job_class] += 1
                    elif space_buf_k.ndim == 1 and job_class < len(space_buf_k):
                        space_buf_k[job_class] += 1

        elif sched in (SchedStrategy.FCFS, SchedStrategy.HOL, SchedStrategy.LCFS):
            for i in range(n_rows):
                ni_val = ni[i]
                if ni_val < S:
                    # Idle server available: enter service directly
                    space_srv_k[i, int(Ks[job_class]) + kentry] += 1
                else:
                    # All servers busy: add to buffer
                    cap_ok = True
                    if capacity is not None and ni_val >= capacity[ist]:
                        cap_ok = False
                    if classcap is not None and nir[i, job_class] >= classcap[ist, job_class]:
                        cap_ok = False
                    if not cap_ok:
                        valid[i] = False
                        continue

                    # Insert into buffer (right-aligned: jobs packed to the right)
                    buf_row = space_buf_k[i] if space_buf_k.ndim >= 2 else space_buf_k
                    # Find leftmost non-zero (first existing job in buffer)
                    first_job = -1
                    for b in range(len(buf_row)):
                        if buf_row[b] > 0:
                            first_job = b
                            break
                    if first_job > 0:
                        # Insert just left of first existing job
                        buf_row[first_job - 1] = job_class + 1
                    elif first_job == 0:
                        # Buffer full: capacity exceeded
                        valid[i] = False
                        continue
                    else:
                        # Empty buffer: insert at rightmost position
                        buf_row[len(buf_row) - 1] = job_class + 1

        elif sched in (SchedStrategy.LCFSPR, SchedStrategy.LCFSPRPRIO, SchedStrategy.FCFSPRPRIO):
            # Preemptive strategies: idle server -> enter service directly;
            # all busy -> preempt a job in service, move it to buffer with (class, phase) pair.
            # Multiple output states are generated (one per preemption possibility),
            # so we compose states inline and continue to skip the default compose loop.
            for i in range(n_rows):
                ni_val = ni[i]
                if ni_val < S:
                    # Idle server: enter service directly
                    space_srv_k[i, int(Ks[job_class]) + kentry] += 1
                    state_row = _compose_state(
                        space_buf_k[i] if space_buf_k.ndim >= 2 else space_buf_k,
                        space_srv_k[i] if space_srv_k.ndim >= 2 else space_srv_k,
                        space_var_k[i] if space_var_k.ndim >= 2 and space_var_k.size > 0 else space_var_k)
                    out_states.append(state_row)
                    out_rates.append(-1.0)
                    out_probs.append(pentry[kentry])
                else:
                    # All servers busy: generate states for each possible preemption
                    srv_total = np.sum(space_srv_k[i])
                    any_preempt = False
                    classprio_arr = sn.classprio if hasattr(sn, 'classprio') and sn.classprio is not None else np.zeros(R)
                    has_diff_prio_arr = not np.all(classprio_arr == classprio_arr[0])
                    for classpreempt in range(R):
                        is_prio_sched = sched in (SchedStrategy.LCFSPRPRIO, SchedStrategy.LCFSPIPRIO, SchedStrategy.FCFSPRPRIO, SchedStrategy.FCFSPIPRIO)
                        is_prio_aware = is_prio_sched or (has_diff_prio_arr and sched in (SchedStrategy.LCFSPR, SchedStrategy.FCFSPR))
                        if is_prio_aware:
                            if classprio_arr[job_class] >= classprio_arr[classpreempt]:
                                continue  # arriving job has same or lower priority
                        for phasepreempt in range(int(K[classpreempt])):
                            col_preempt = int(Ks[classpreempt]) + phasepreempt
                            count_preempt = space_srv_k[i, col_preempt]
                            if count_preempt > 0:
                                srv_k = space_srv_k[i].copy()
                                buf_k = space_buf_k[i].copy() if space_buf_k.ndim >= 2 else space_buf_k.copy()
                                var_k = space_var_k[i].copy() if space_var_k.ndim >= 2 and space_var_k.size > 0 else (space_var_k.copy() if space_var_k.size > 0 else np.array([]))
                                # Remove preempted job from service
                                srv_k[col_preempt] -= 1
                                # Add arriving job to service
                                srv_k[int(Ks[job_class]) + kentry] += 1
                                # Save preempted job in buffer as [class, phase] pair
                                # Buffer stores pairs: [class, phase, class, phase, ...]
                                # Find first empty pair slot (even index where class == 0)
                                for b in range(0, len(buf_k) - 1, 2):
                                    if buf_k[b] == 0:
                                        buf_k[b] = classpreempt + 1      # 1-based class
                                        buf_k[b + 1] = phasepreempt + 1  # 1-based phase
                                        break
                                state_row = _compose_state(buf_k, srv_k, var_k)
                                out_states.append(state_row)
                                out_rates.append(-1.0)
                                out_probs.append(pentry[kentry] * count_preempt / srv_total)
                                any_preempt = True
                    if not any_preempt:
                        # No one to preempt (priority variants: no lower-priority job in service)
                        valid[i] = False
            # Skip the default compose-output loop below
            continue

        else:
            # Default: enter service
            for i in range(n_rows):
                space_srv_k[i, int(Ks[job_class]) + kentry] += 1

        # Compose output states
        for i in range(n_rows):
            if valid[i]:
                state_row = _compose_state(
                    space_buf_k[i] if space_buf_k.ndim >= 2 else space_buf_k,
                    space_srv_k[i] if space_srv_k.ndim >= 2 else space_srv_k,
                    space_var_k[i] if space_var_k.ndim >= 2 and space_var_k.size > 0 else space_var_k)
                out_states.append(state_row)
                out_rates.append(-1.0)  # Passive action
                out_probs.append(outprob_k[i])

    return _finalize(out_states, out_rates, out_probs, inspace)


# ---------------------------------------------------------------------------
# DEP handler
# ---------------------------------------------------------------------------

def _handle_dep(sn, ind, inspace, job_class, M, R, ist, S, K, Ks,
                hasOnlyExp, mu, phi, pie, proc, ismkvmodclass,
                lldscaling, lldlimit, cdscaling, capacity, classcap,
                V, sched, space_buf, space_srv, space_var):
    """Handle departure events."""
    from .marginal import toMarginal
    ni, nir, sir, kir = toMarginal(sn, ind, inspace, K, Ks,
                                    space_buf=space_buf, space_srv=space_srv,
                                    space_var=space_var)
    ni = np.atleast_1d(ni)
    nir = np.atleast_2d(nir)
    sir = np.atleast_2d(sir)

    # Check any job of this class in service
    has_jobs = False
    for i in range(space_srv.shape[0]):
        for k_phase in range(int(K[job_class])):
            col = int(Ks[job_class]) + k_phase
            if col < space_srv.shape[1] and space_srv[i, col] > 0:
                has_jobs = True
                break
        if has_jobs:
            break
    if not has_jobs:
        return np.zeros((0, 0)), np.zeros((0, 0)), np.zeros((0, 0))

    # Update round-robin pointer
    space_var = space_var.copy()
    _maybe_update_rrobin(sn, ind, job_class, R, space_var)

    out_states = []
    out_rates = []
    out_probs = []

    for k_phase in range(int(K[job_class])):
        col = int(Ks[job_class]) + k_phase
        # Find enabled rows
        en = space_srv[:, col] > 0
        if not np.any(en):
            continue

        enabled_rows = np.where(en)[0]

        for row_idx in enabled_rows:
            space_buf_k = space_buf[row_idx].copy() if space_buf.ndim >= 2 else space_buf.copy()
            space_srv_k = space_srv[row_idx].copy()
            space_var_k = space_var[row_idx].copy() if space_var.ndim >= 2 and space_var.size > 0 else (space_var.copy() if space_var.size > 0 else np.array([]))

            # Record departure
            space_srv_k[col] -= 1

            # Get kir for this row
            if kir.ndim == 3:
                kir_val = kir[row_idx, job_class, k_phase]
            else:
                kir_val = sir[row_idx, job_class] if hasOnlyExp else 1.0

            ni_val = ni[row_idx]
            nir_row = nir[row_idx]

            # Compute rate based on scheduling strategy
            rate = _compute_dep_rate(sched, ist, S, job_class, k_phase, K, Ks,
                                      mu, phi, proc, kir_val, ni_val, nir_row, sir[row_idx],
                                      lldscaling, lldlimit, cdscaling, R, sn)

            if rate <= 0:
                continue

            if sched == SchedStrategy.EXT:
                # Source: DEP = generate an arrival. After decrement,
                # re-enter in each possible entry phase (JAR: spaceSrv[kentry]++)
                pentry = _get_pie(pie, ist, job_class, K)
                for kentry in range(int(K[job_class])):
                    space_srv_e = space_srv_k.copy()
                    col_entry = int(Ks[job_class]) + kentry
                    space_srv_e[col_entry] += 1
                    state_row = _compose_state(space_buf_k, space_srv_e, space_var_k)
                    out_states.append(state_row)
                    out_rates.append(rate)
                    out_probs.append(pentry[kentry] if kentry < len(pentry) else 1.0)
            elif sched in (SchedStrategy.FCFS, SchedStrategy.HOL, SchedStrategy.LCFS):
                # Buffer promotion: move head-of-line job to service
                _dep_with_buffer_promotion(sched, sn, space_buf_k, space_srv_k, space_var_k,
                                            K, Ks, pie, ist, S, ni_val, sir[row_idx],
                                            out_states, out_rates, out_probs, rate, R)
            elif sched in (SchedStrategy.LCFSPR, SchedStrategy.LCFSPRPRIO, SchedStrategy.FCFSPRPRIO):
                # Preemptive resume: promote buffered job back to service with saved phase
                _dep_preemptive_resume(sched, sn, space_buf_k, space_srv_k, space_var_k,
                                        K, Ks, pie, ist, S, ni_val,
                                        out_states, out_rates, out_probs, rate, R)
            elif sched == SchedStrategy.SIRO:
                _dep_siro_promotion(space_buf_k, space_srv_k, space_var_k,
                                     K, Ks, pie, ist, ni_val, nir_row, sir[row_idx],
                                     out_states, out_rates, out_probs, rate, R)
            else:
                # No buffer promotion (PS, INF, DPS, GPS, etc.)
                state_row = _compose_state(space_buf_k, space_srv_k, space_var_k)
                out_states.append(state_row)
                out_rates.append(rate)
                out_probs.append(1.0)

    return _finalize(out_states, out_rates, out_probs, inspace)


def _compute_dep_rate(sched, ist, S, job_class, k_phase, K, Ks,
                       mu, phi, proc, kir_val, ni_val, nir_row, sir_row,
                       lldscaling, lldlimit, cdscaling, R, sn):
    """Compute departure rate for a given scheduling strategy."""
    # Get mu and phi values
    mu_val = _get_mu(mu, ist, job_class, k_phase)
    phi_val = _get_phi(phi, ist, job_class, k_phase)

    if mu_val == 0 or np.isnan(mu_val):
        return 0.0

    # Load-dependent scaling
    lld = _get_lld(lldscaling, ist, ni_val, lldlimit)
    cd = _get_cd(cdscaling, ist, nir_row)

    if sched == SchedStrategy.EXT:
        # Source: rate = mu * phi * lld * cd
        return mu_val * phi_val * lld * cd

    elif sched == SchedStrategy.INF:
        return mu_val * phi_val * kir_val * lld * cd

    elif sched in (SchedStrategy.PS,):
        if ni_val > 0:
            return mu_val * phi_val * kir_val * min(ni_val, S) / ni_val * lld * cd
        return 0.0

    elif sched == SchedStrategy.PSPRIO:
        # PS with priority: only serve highest priority jobs
        niprio = _get_niprio(sn, ist, nir_row, job_class, R)
        if niprio > 0:
            return mu_val * phi_val * kir_val * min(niprio, S) / niprio * lld * cd
        return 0.0

    elif sched in (SchedStrategy.FCFS, SchedStrategy.HOL, SchedStrategy.LCFS,
                    SchedStrategy.SIRO, SchedStrategy.LCFSPR,
                    SchedStrategy.LCFSPRPRIO, SchedStrategy.FCFSPRPRIO):
        # For FCFS with MAP processes, use D1 matrix
        if proc is not None:
            p = _get_proc(proc, ist, job_class)
            if p is not None and len(p) > 1:
                d1 = np.atleast_2d(p[1])
                # Sum D1 row k_phase to get total departure rate from this phase
                rate = np.sum(d1[k_phase, :]) * kir_val * lld * cd
                return rate

        return mu_val * phi_val * kir_val * lld * cd

    elif sched == SchedStrategy.DPS:
        # Discriminatory Processor Sharing
        w = _get_sched_weights(sn, ist, R)
        w_sum = np.dot(w, nir_row)
        if w_sum > 0 and nir_row[job_class] > 0:
            return mu_val * phi_val * (kir_val / nir_row[job_class]) * w[job_class] * nir_row[job_class] / w_sum * lld * cd
        return 0.0

    elif sched == SchedStrategy.GPS:
        # Generalized Processor Sharing
        w = _get_sched_weights(sn, ist, R)
        cir = np.minimum(nir_row, 1)
        w_cir = np.dot(w, cir)
        if w_cir > 0 and nir_row[job_class] > 0:
            return mu_val * phi_val * (kir_val / nir_row[job_class]) * w[job_class] / w_cir * lld * cd
        return 0.0

    # Default
    return mu_val * phi_val * kir_val * lld * cd


def _dep_with_buffer_promotion(sched, sn, space_buf_k, space_srv_k, space_var_k,
                                K, Ks, pie, ist, S, ni_val, sir_row,
                                out_states, out_rates, out_probs, rate, R):
    """Handle departure with buffer promotion for FCFS/HOL/LCFS."""
    buf = space_buf_k
    has_buffer_job = False
    if buf.size > 0:
        has_buffer_job = np.any(buf > 0)

    if not has_buffer_job:
        # No buffer jobs: just departure
        state_row = _compose_state(space_buf_k, space_srv_k, space_var_k)
        out_states.append(state_row)
        out_rates.append(rate)
        out_probs.append(1.0)
        return

    # Find the job to promote from buffer
    if sched == SchedStrategy.FCFS:
        # FCFS: rightmost non-zero = head of line
        promote_idx = -1
        for b in range(len(buf) - 1, -1, -1):
            if buf[b] > 0:
                promote_idx = b
                break
    elif sched == SchedStrategy.LCFS:
        # LCFS: leftmost non-zero = most recent, priority-aware when priorities differ
        classprio_dep = sn.classprio if hasattr(sn, 'classprio') and sn.classprio is not None else np.zeros(R)
        has_diff_prio_dep = not np.all(classprio_dep == classprio_dep[0])
        promote_idx = -1
        if has_diff_prio_dep:
            # Priority-aware: find leftmost among highest-priority class
            best_prio = float('inf')
            for b in range(len(buf)):
                if buf[b] > 0:
                    cls = int(buf[b]) - 1  # 0-based class
                    prio = classprio_dep[cls] if cls < len(classprio_dep) else float('inf')
                    if prio < best_prio:
                        best_prio = prio
                        promote_idx = b
        else:
            for b in range(len(buf)):
                if buf[b] > 0:
                    promote_idx = b
                    break
    elif sched == SchedStrategy.HOL:
        # HOL: rightmost with highest priority (lowest classprio value)
        promote_idx = -1
        best_prio = float('inf')
        classprio = sn.classprio if hasattr(sn, 'classprio') and sn.classprio is not None else np.zeros(R)
        for b in range(len(buf) - 1, -1, -1):
            if buf[b] > 0:
                cls = int(buf[b]) - 1
                prio = classprio[cls] if cls < len(classprio) else float('inf')
                if prio < best_prio:
                    best_prio = prio
                    promote_idx = b
        if promote_idx == -1:
            # Fallback to FCFS
            for b in range(len(buf) - 1, -1, -1):
                if buf[b] > 0:
                    promote_idx = b
                    break
    else:
        promote_idx = -1

    if promote_idx < 0:
        state_row = _compose_state(space_buf_k, space_srv_k, space_var_k)
        out_states.append(state_row)
        out_rates.append(rate)
        out_probs.append(1.0)
        return

    start_svc_class = int(buf[promote_idx]) - 1  # Convert from 1-based to 0-based
    buf_new = buf.copy()
    # Remove from buffer: shift remaining left
    if sched == SchedStrategy.FCFS:
        # Shift left from the right end
        buf_new[promote_idx] = 0
        # Shift: move buf_new[0:promote_idx] right by prepending 0
        buf_new[1:promote_idx + 1] = buf[0:promote_idx]
        buf_new[0] = 0
    elif sched == SchedStrategy.LCFS:
        buf_new[promote_idx] = 0
    elif sched == SchedStrategy.HOL:
        buf_new[promote_idx] = 0
        buf_new[1:promote_idx + 1] = buf[0:promote_idx]
        buf_new[0] = 0

    # Enter service: iterate over entry phases
    pentry = _get_pie(pie, ist, start_svc_class, K)
    for kentry in range(int(K[start_svc_class])):
        srv_new = space_srv_k.copy()
        srv_new[int(Ks[start_svc_class]) + kentry] += 1
        state_row = _compose_state(buf_new, srv_new, space_var_k)
        out_states.append(state_row)
        out_rates.append(rate * pentry[kentry])
        out_probs.append(1.0)


def _dep_siro_promotion(space_buf_k, space_srv_k, space_var_k,
                          K, Ks, pie, ist, ni_val, nir_row, sir_row,
                          out_states, out_rates, out_probs, rate, R):
    """Handle departure with SIRO buffer promotion."""
    buf = space_buf_k
    total_buffered = np.sum(buf) if buf.size > 0 else 0

    if total_buffered <= 0:
        state_row = _compose_state(space_buf_k, space_srv_k, space_var_k)
        out_states.append(state_row)
        out_rates.append(rate)
        out_probs.append(1.0)
        return

    # Pick class proportional to buffer occupancy
    for r in range(R):
        buf_r = buf[r] if r < len(buf) else 0
        if buf_r <= 0:
            continue
        pick_prob = buf_r / total_buffered
        buf_new = buf.copy()
        buf_new[r] -= 1

        pentry = _get_pie(pie, ist, r, K)
        for kentry in range(int(K[r])):
            srv_new = space_srv_k.copy()
            srv_new[int(Ks[r]) + kentry] += 1
            state_row = _compose_state(buf_new, srv_new, space_var_k)
            out_states.append(state_row)
            out_rates.append(rate * pick_prob * pentry[kentry])
            out_probs.append(1.0)


def _dep_preemptive_resume(sched, sn, space_buf_k, space_srv_k, space_var_k,
                            K, Ks, pie, ist, S, ni_val,
                            out_states, out_rates, out_probs, rate, R):
    """Handle departure with preempt-resume buffer promotion for LCFSPR/LCFSPRPRIO/FCFSPRPRIO.

    Buffer stores [class, phase, class, phase, ...] pairs (1-based values).
    On departure, the highest-priority buffered job is promoted back to service
    resuming at its saved phase (preempt-resume semantics).
    """
    buf = space_buf_k
    # Check if any job in buffer (class values at even indices)
    has_buffer_job = False
    if buf.size > 0:
        for b in range(0, len(buf), 2):
            if buf[b] > 0:
                has_buffer_job = True
                break

    if not has_buffer_job:
        # No buffer jobs: just departure
        state_row = _compose_state(space_buf_k, space_srv_k, space_var_k)
        out_states.append(state_row)
        out_rates.append(rate)
        out_probs.append(1.0)
        return

    # Find job to promote from buffer based on scheduling strategy
    classprio = sn.classprio if hasattr(sn, 'classprio') and sn.classprio is not None else np.zeros(R)

    if sched == SchedStrategy.LCFSPR:
        # LCFSPR: leftmost non-zero pair = most recently preempted (LCFS order)
        # Priority-aware when priorities differ
        has_diff_prio_lcfspr = not np.all(classprio == classprio[0])
        target_col = -1
        if has_diff_prio_lcfspr:
            # Find leftmost pair among highest-priority class
            best_prio = float('inf')
            for b in range(0, len(buf), 2):
                if buf[b] > 0:
                    cls = int(buf[b]) - 1  # 0-based class
                    prio = classprio[cls] if cls < len(classprio) else float('inf')
                    if prio < best_prio:
                        best_prio = prio
                        target_col = b
        else:
            for b in range(0, len(buf), 2):
                if buf[b] > 0:
                    target_col = b
                    break
    elif sched == SchedStrategy.LCFSPRPRIO:
        # LCFSPRPRIO: among highest-priority jobs, pick leftmost (LCFS: most recent)
        best_prio = float('inf')
        target_col = -1
        for b in range(0, len(buf), 2):
            if buf[b] > 0:
                cls = int(buf[b]) - 1  # 0-based class
                prio = classprio[cls] if cls < len(classprio) else float('inf')
                if prio < best_prio:
                    best_prio = prio
                    target_col = b  # leftmost wins for LCFS (use < not <=)
    elif sched == SchedStrategy.FCFSPRPRIO:
        # FCFSPRPRIO: among highest-priority jobs, pick rightmost (FCFS: oldest)
        best_prio = float('inf')
        target_col = -1
        for b in range(len(buf) - 2, -1, -2):
            if b >= 0 and buf[b] > 0:
                cls = int(buf[b]) - 1  # 0-based class
                prio = classprio[cls] if cls < len(classprio) else float('inf')
                if prio < best_prio:
                    best_prio = prio
                    target_col = b
    else:
        target_col = -1

    if target_col < 0:
        state_row = _compose_state(space_buf_k, space_srv_k, space_var_k)
        out_states.append(state_row)
        out_rates.append(rate)
        out_probs.append(1.0)
        return

    start_svc_class = int(buf[target_col]) - 1       # 0-based class
    kentry_phase = int(buf[target_col + 1]) - 1       # 0-based phase

    # Remove [class, phase] pair from buffer and shift remaining left
    buf_new = np.concatenate([
        np.array([0.0, 0.0]),
        buf[:target_col],
        buf[target_col + 2:]
    ])

    # Put promoted job into service resuming at its saved phase
    srv_new = space_srv_k.copy()
    srv_new[int(Ks[start_svc_class]) + kentry_phase] += 1

    state_row = _compose_state(buf_new, srv_new, space_var_k)
    out_states.append(state_row)
    out_rates.append(rate)
    out_probs.append(1.0)


# ---------------------------------------------------------------------------
# PHASE handler
# ---------------------------------------------------------------------------

def _handle_phase(sn, ind, inspace, job_class, M, R, ist, S, K, Ks,
                  hasOnlyExp, mu, phi, pie, proc, ismkvmodclass,
                  lldscaling, lldlimit, cdscaling,
                  V, sched, space_buf, space_srv, space_var):
    """Handle phase transition events (internal to service)."""
    from .marginal import toMarginal
    ni, nir, sir, kir = toMarginal(sn, ind, inspace, K, Ks,
                                    space_buf=space_buf, space_srv=space_srv,
                                    space_var=space_var)
    ni = np.atleast_1d(ni)
    nir = np.atleast_2d(nir)

    if nir[0, job_class] <= 0:
        return np.zeros((0, 0)), np.zeros((0, 0)), np.zeros((0, 0))

    # Get D0 matrix for phase transitions
    d0 = None
    p = _get_proc(proc, ist, job_class)
    if p is not None and len(p) > 0:
        d0 = np.atleast_2d(p[0])

    lld_val = _get_lld(lldscaling, ist, ni[0], lldlimit)
    cd_val = _get_cd(cdscaling, ist, nir[0])

    out_states = []
    out_rates = []
    out_probs = []

    for k in range(int(K[job_class])):
        col_k = int(Ks[job_class]) + k
        en = space_srv[:, col_k] > 0
        if not np.any(en):
            continue

        enabled_rows = np.where(en)[0]

        for kdest in range(int(K[job_class])):
            if kdest == k:
                continue

            col_kdest = int(Ks[job_class]) + kdest

            for row_idx in enabled_rows:
                srv_k = space_srv[row_idx].copy()
                buf_k = space_buf[row_idx].copy() if space_buf.ndim >= 2 else space_buf.copy()
                var_k = space_var[row_idx].copy() if space_var.ndim >= 2 and space_var.size > 0 else (space_var.copy() if space_var.size > 0 else np.array([]))

                if kir.ndim == 3:
                    kir_val = kir[row_idx, job_class, k]
                else:
                    kir_val = 1.0

                # Move from phase k to kdest
                srv_k[col_k] -= 1
                srv_k[col_kdest] += 1

                # Rate from D0 matrix
                if d0 is not None:
                    d0_rate = d0[k, kdest]
                else:
                    d0_rate = 0.0

                # Apply scheduling-specific scaling
                rate = _compute_phase_rate(sched, d0_rate, kir_val, ni[row_idx],
                                            nir[row_idx], S, job_class, R, sn, ist,
                                            lld_val, cd_val)

                if rate == 0:
                    continue

                state_row = _compose_state(buf_k, srv_k, var_k)
                out_states.append(state_row)
                out_rates.append(rate)
                out_probs.append(1.0)

    return _finalize(out_states, out_rates, out_probs, inspace)


def _compute_phase_rate(sched, d0_rate, kir_val, ni_val, nir_row, S,
                         job_class, R, sn, ist, lld, cd):
    """Compute phase transition rate based on scheduling strategy."""
    if sched == SchedStrategy.EXT:
        return d0_rate * lld * cd

    elif sched == SchedStrategy.INF:
        return d0_rate * kir_val * lld * cd

    elif sched in (SchedStrategy.PS,):
        if ni_val > 0:
            return d0_rate * kir_val * min(ni_val, S) / ni_val * lld * cd
        return 0.0

    elif sched == SchedStrategy.PSPRIO:
        niprio = _get_niprio(sn, ist, nir_row, job_class, R)
        if niprio > 0:
            return d0_rate * kir_val * min(niprio, S) / niprio * lld * cd
        return 0.0

    elif sched == SchedStrategy.DPS:
        w = _get_sched_weights(sn, ist, R)
        w_sum = np.dot(w, nir_row)
        if w_sum > 0:
            return d0_rate * kir_val * w[job_class] / w_sum * lld * cd
        return 0.0

    elif sched == SchedStrategy.GPS:
        w = _get_sched_weights(sn, ist, R)
        cir = np.minimum(nir_row, 1)
        w_cir = np.dot(w, cir)
        if w_cir > 0 and nir_row[job_class] > 0:
            return d0_rate * kir_val / nir_row[job_class] * w[job_class] / w_cir * lld * cd
        return 0.0

    else:
        # FCFS, HOL, LCFS, SIRO, SEPT, LEPT: D0 * kir
        return d0_rate * kir_val * lld * cd


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

def _compose_state(buf, srv, var):
    """Compose full state row from buf, srv, var parts."""
    parts = []
    if isinstance(buf, np.ndarray) and buf.size > 0:
        parts.append(buf.ravel())
    if isinstance(srv, np.ndarray) and srv.size > 0:
        parts.append(srv.ravel())
    if isinstance(var, np.ndarray) and var.size > 0:
        parts.append(var.ravel())
    return np.concatenate(parts) if parts else np.array([])


def _finalize(out_states, out_rates, out_probs, inspace):
    """Finalize output arrays."""
    if len(out_states) == 0:
        return np.zeros((0, 0)), np.zeros((0, 0)), np.zeros((0, 0))
    outspace = np.array(out_states)
    outrate = np.array(out_rates).reshape(-1, 1)
    outprob = np.array(out_probs).reshape(-1, 1)
    return outspace, outrate, outprob


def _get_pie(pie, ist, job_class, K):
    """Get phase entry probabilities."""
    if pie is not None and ist in pie and job_class in pie[ist]:
        pe = pie[ist][job_class]
        if isinstance(pe, np.ndarray):
            return pe.ravel()[:int(K[job_class])]
        elif isinstance(pe, (list, tuple)):
            return np.array(pe[:int(K[job_class])])
    # Default: all probability in phase 0
    pentry = np.zeros(int(K[job_class]))
    if len(pentry) > 0:
        pentry[0] = 1.0
    return pentry


def _get_mu(mu, ist, job_class, k_phase):
    """Get service rate for a given station, class, phase."""
    if mu is not None and ist in mu and job_class in mu[ist]:
        m = mu[ist][job_class]
        if isinstance(m, np.ndarray):
            m = m.ravel()
            if k_phase < len(m):
                return float(m[k_phase])
        elif isinstance(m, (list, tuple)):
            if k_phase < len(m):
                return float(m[k_phase])
        else:
            return float(m)
    return 0.0


def _get_phi(phi, ist, job_class, k_phase):
    """Get completion probability for a given station, class, phase."""
    if phi is not None and ist in phi and job_class in phi[ist]:
        p = phi[ist][job_class]
        if isinstance(p, np.ndarray):
            p = p.ravel()
            if k_phase < len(p):
                return float(p[k_phase])
        elif isinstance(p, (list, tuple)):
            if k_phase < len(p):
                return float(p[k_phase])
        else:
            return float(p)
    return 1.0


def _get_proc(proc, ist, job_class):
    """Get process matrices [D0, D1] for a given station and class."""
    if proc is None:
        return None
    try:
        if isinstance(proc, dict):
            if ist in proc and job_class in proc[ist]:
                return proc[ist][job_class]
        elif isinstance(proc, (list, tuple)):
            if ist < len(proc) and proc[ist] is not None:
                station_proc = proc[ist]
                if isinstance(station_proc, (list, tuple)) and job_class < len(station_proc):
                    return station_proc[job_class]
                elif isinstance(station_proc, dict) and job_class in station_proc:
                    return station_proc[job_class]
    except (IndexError, KeyError, TypeError):
        pass
    return None


def _get_lld(lldscaling, ist, ni_val, lldlimit):
    """Get load-level-dependent scaling factor."""
    if lldscaling is None or lldscaling.size == 0:
        return 1.0
    idx = int(min(max(ni_val, 1), lldlimit)) - 1
    if isinstance(lldscaling, np.ndarray) and lldscaling.ndim >= 2:
        if ist < lldscaling.shape[0] and idx < lldscaling.shape[1]:
            return float(lldscaling[ist, idx])
    return 1.0


def _get_cd(cdscaling, ist, nir_row):
    """Get class-dependent scaling factor."""
    if cdscaling is None:
        return 1.0
    if isinstance(cdscaling, dict) and ist in cdscaling:
        fn = cdscaling[ist]
        if callable(fn):
            return fn(nir_row)
    return 1.0


def _get_sched_weights(sn, ist, R):
    """Get scheduling weights (for DPS, GPS)."""
    w = np.ones(R)
    if hasattr(sn, 'schedparam') and sn.schedparam is not None:
        for r in range(R):
            if sn.schedparam[ist, r] is not None:
                w[r] = float(sn.schedparam[ist, r])
    w_sum = np.sum(w)
    if w_sum > 0:
        w = w / w_sum
    return w


def _get_niprio(sn, ist, nir_row, job_class, R):
    """Get total jobs at the same or higher priority level."""
    if hasattr(sn, 'classprio') and sn.classprio is not None:
        my_prio = sn.classprio[job_class]
        niprio = 0
        for r in range(R):
            if sn.classprio[r] <= my_prio and nir_row[r] > 0:
                niprio += nir_row[r]
        return niprio
    return np.sum(nir_row)


def _maybe_update_rrobin(sn, ind, job_class, R, space_var):
    """Update round-robin pointer if applicable."""
    if space_var.size == 0:
        return
    routing_val = sn.routing[ind, job_class] if hasattr(sn, 'routing') and sn.routing is not None else None
    if routing_val is None:
        return

    rs_val = int(routing_val.value) if hasattr(routing_val, 'value') else int(routing_val)
    rrobin_val = int(SchedStrategy.RROBIN.value) if hasattr(SchedStrategy.RROBIN, 'value') else -999

    # Check RoutingStrategy.RROBIN
    from ...constants import RoutingStrategy
    rr_val = int(RoutingStrategy.RROBIN.value) if hasattr(RoutingStrategy.RROBIN, 'value') else int(RoutingStrategy.RROBIN)
    if rs_val != rr_val:
        return

    nvar_sum = int(np.sum(sn.nvars[ind, :R + job_class + 1]))
    if nvar_sum <= 0:
        return
    space_var_idx = nvar_sum - 1

    nparam = sn.nodeparam[ind] if sn.nodeparam is not None and ind in sn.nodeparam else None
    if nparam is None:
        return

    outlinks = nparam.get('outlinks', None) if isinstance(nparam, dict) else getattr(nparam, 'outlinks', None)
    if outlinks is None or job_class >= len(outlinks):
        return
    ol = np.atleast_1d(outlinks[job_class])
    if len(ol) == 0:
        return

    for row in range(space_var.shape[0] if space_var.ndim >= 2 else 1):
        current_val = space_var[row, space_var_idx] if space_var.ndim >= 2 else space_var[space_var_idx]
        idx = -1
        for i in range(len(ol)):
            if ol[i] == current_val:
                idx = i
                break
        next_val = ol[idx + 1] if idx >= 0 and idx < len(ol) - 1 else ol[0]
        if space_var.ndim >= 2:
            space_var[row, space_var_idx] = next_val
        else:
            space_var[space_var_idx] = next_val
