"""
Cache node event handler for afterEvent dispatch.

Handles ARV, DEP, and READ events at Cache nodes. Cache nodes are
stateful non-station nodes that implement item caching with various
replacement policies (FIFO, LRU, SFIFO, RR).

Port from JAR AfterEventCache.java.
"""

import numpy as np
from ...constants import EventType, RoutingStrategy, GlobalConstants


def _cpos(m, i, j):
    """
    Compute absolute position in cache state vector for position j in list i.

    Args:
        m: Array of list capacities
        i: List index (0-based)
        j: Position within list (0-based)

    Returns:
        Absolute column index in spaceVar
    """
    pos = int(np.sum(m[:i])) if i > 0 else 0
    return pos + j


def after_event_cache(sn, ind, event, job_class, R, space_buf, space_srv, space_var):
    """
    Handle events at a Cache node.

    Args:
        sn: NetworkStruct
        ind: Node index (0-based)
        event: EventType (ARV, DEP, or READ)
        job_class: Job class index (0-based)
        R: Number of classes
        space_buf: Buffer state (empty for Cache)
        space_srv: Server state, shape (n_rows, R) - job counts per class
        space_var: Variable state - cache list contents

    Returns:
        Tuple of (outspace, outrate, outprob)
    """
    space_srv = space_srv.copy()
    space_var = space_var.copy()
    n_rows = space_srv.shape[0]

    if event == EventType.ARV:
        # Increment job count for arriving class
        space_srv[:, job_class] += 1
        outspace = np.hstack([space_srv, space_var]) if space_var.size > 0 else space_srv.copy()
        outrate = -1.0 * np.ones((n_rows, 1))
        outprob = np.ones((n_rows, 1))
        return outspace, outrate, outprob

    elif event == EventType.DEP:
        if space_srv[0, job_class] > 0:
            space_srv[:, job_class] -= 1

            # Update round-robin pointer if applicable
            routing_val = sn.routing[ind, job_class] if hasattr(sn, 'routing') and sn.routing is not None else None
            rs_val = int(routing_val.value) if hasattr(routing_val, 'value') else int(routing_val) if routing_val is not None else -1
            rrobin_val = int(RoutingStrategy.RROBIN.value) if hasattr(RoutingStrategy.RROBIN, 'value') else int(RoutingStrategy.RROBIN)

            if rs_val == rrobin_val:
                nvar_sum = int(np.sum(sn.nvars[ind, :R + job_class + 1]))
                space_var_idx = nvar_sum - 1
                nparam = sn.nodeparam.get(ind, None) if isinstance(sn.nodeparam, dict) else sn.nodeparam[ind] if sn.nodeparam is not None else None
                if nparam is not None:
                    outlinks = nparam.get('outlinks', None) if isinstance(nparam, dict) else getattr(nparam, 'outlinks', None)
                    if outlinks is not None and job_class < len(outlinks):
                        ol = np.atleast_1d(outlinks[job_class])
                        current_val = space_var[0, space_var_idx] if space_var.ndim >= 2 else space_var[space_var_idx]
                        idx = -1
                        for i in range(len(ol)):
                            if ol[i] == current_val:
                                idx = i
                                break
                        next_val = ol[idx + 1] if idx < len(ol) - 1 else ol[0]
                        if space_var.ndim >= 2:
                            space_var[:, space_var_idx] = next_val
                        else:
                            space_var[space_var_idx] = next_val

            outspace = np.hstack([space_srv, space_var]) if space_var.size > 0 else space_srv.copy()
            outrate = GlobalConstants.Immediate * np.ones((n_rows, 1))
            outprob = np.ones((n_rows, 1))
            return outspace, outrate, outprob

        # No job to depart
        return np.zeros((0, 0)), np.zeros((0, 0)), np.zeros((0, 0))

    elif event == EventType.READ:
        return _handle_read(sn, ind, job_class, R, space_srv, space_var)

    return np.zeros((0, 0)), np.zeros((0, 0)), np.zeros((0, 0))


def _handle_read(sn, ind, job_class, R, space_srv, space_var):
    """Handle READ event at a Cache node (CTMC mode only)."""
    # Extract cache parameters
    nparam = sn.nodeparam[ind] if sn.nodeparam is not None and ind in sn.nodeparam else None
    if nparam is None:
        return np.zeros((0, 0)), np.zeros((0, 0)), np.zeros((0, 0))

    if isinstance(nparam, dict):
        n = nparam.get('nitems', 0)
        m = np.atleast_1d(nparam.get('itemcap', []))
        ac = nparam.get('accost', None)
        hitclass = np.atleast_1d(nparam.get('hitclass', []))
        missclass = np.atleast_1d(nparam.get('missclass', []))
        pread = nparam.get('pread', None)
        replacement = nparam.get('replacement', 'FIFO')
    else:
        n = getattr(nparam, 'nitems', 0)
        m = np.atleast_1d(getattr(nparam, 'itemcap', []))
        ac = getattr(nparam, 'accost', None)
        hitclass = np.atleast_1d(getattr(nparam, 'hitclass', []))
        missclass = np.atleast_1d(getattr(nparam, 'missclass', []))
        pread = getattr(nparam, 'pread', None)
        replacement = getattr(nparam, 'replacement', 'FIFO')

    m = m.astype(int)
    h = len(m)  # number of cache lists
    hitclass = hitclass.astype(int)
    missclass = missclass.astype(int)

    # Get replacement strategy name
    if hasattr(replacement, 'name'):
        repl_name = replacement.name
    elif hasattr(replacement, 'value'):
        repl_name = str(replacement.value)
    else:
        repl_name = str(replacement)

    # Check precondition: exactly one job at cache, and it's of this class
    srv_sum = np.sum(space_srv[0])
    if space_srv[0, job_class] <= 0 or int(srv_sum) != 1:
        return np.zeros((0, 0)), np.zeros((0, 0)), np.zeros((0, 0))

    # Get read probabilities for this class
    p = pread[job_class] if pread is not None and job_class < len(pread) else None
    if p is None:
        return np.zeros((0, 0)), np.zeros((0, 0)), np.zeros((0, 0))
    p = list(p)

    out_srv_list = []
    out_var_list = []
    out_rate_list = []
    Immediate = GlobalConstants.Immediate

    # Find enabled rows
    for e in range(space_srv.shape[0]):
        if space_srv[e, job_class] <= 0:
            continue

        # CTMC mode: iterate over ALL items
        for k in range(n):
            space_srv_e = space_srv[e].copy()
            space_srv_e[job_class] -= 1  # Remove the reading job
            var = space_var[e].copy()

            # Find item k+1 in cache state
            posk = -1
            for col in range(var.shape[0] if var.ndim == 1 else var.shape[1] if var.ndim == 2 else 0):
                v = var[col] if var.ndim == 1 else var[0, col]
                if v == k + 1:  # Items are 1-indexed
                    posk = col
                    break

            if posk == -1:
                # CACHE MISS
                space_srv_e[int(missclass[job_class])] += 1
                _handle_miss(m, h, k, p, ac, job_class, repl_name, space_srv_e, var,
                             out_srv_list, out_var_list, out_rate_list, Immediate)

            elif posk < int(np.sum(m) - m[h - 1]):
                # CACHE HIT in list i < h (not the last list)
                space_srv_e[int(hitclass[job_class])] += 1
                # Find which list i the item is in
                mcumsum = np.cumsum(m)
                i_list = 0
                for col in range(len(mcumsum)):
                    if posk < mcumsum[col]:
                        i_list = col
                        break
                # Position within list i
                j_pos = posk - (int(np.sum(m[:i_list])) if i_list > 0 else 0)

                _handle_hit_nonterminal(m, h, k, p, ac, job_class, repl_name, i_list, j_pos,
                                        space_srv_e, var,
                                        out_srv_list, out_var_list, out_rate_list, Immediate)

            else:
                # CACHE HIT in last list h
                space_srv_e[int(hitclass[job_class])] += 1
                j_pos = posk - (int(np.sum(m[:h - 1])) if h > 1 else 0)

                _handle_hit_terminal(m, h, k, p, repl_name, j_pos,
                                     space_srv_e, var,
                                     out_srv_list, out_var_list, out_rate_list, Immediate)

    if len(out_srv_list) == 0:
        return np.zeros((0, 0)), np.zeros((0, 0)), np.zeros((0, 0))

    outspace = np.hstack([np.array(out_srv_list), np.array(out_var_list)])
    outrate = np.array(out_rate_list).reshape(-1, 1)
    outprob = np.ones((len(out_rate_list), 1))
    return outspace, outrate, outprob


def _handle_miss(m, h, k, p, ac, job_class, repl_name, space_srv_e, var,
                 out_srv_list, out_var_list, out_rate_list, Immediate):
    """Handle cache miss: insert item k+1 into cache."""
    for l in range(h):
        # l is the target list index (0-based)
        listidx = l
        head_pos = _cpos(m, listidx, 0)
        tail_pos = _cpos(m, listidx, int(m[listidx]) - 1)

        if repl_name in ('FIFO', 'LRU', 'SFIFO'):
            varp = var.copy()
            # Shift items right by one (evict tail)
            if m[listidx] > 1:
                varp[head_pos + 1:tail_pos + 1] = var[head_pos:tail_pos]
            varp[head_pos] = k + 1  # Insert at head

            out_srv_list.append(space_srv_e.copy())
            out_var_list.append(varp)
            # Rate = ac[class][k](0, l+1) * p[k] * Immediate
            # ac is indexed ac[class][item] with rows: 0=miss, 1+i=hit-in-list-i
            ac_val = _get_ac(ac, job_class, k, 0, l + 1)
            out_rate_list.append(ac_val * p[k] * Immediate)

        elif repl_name == 'RR':
            # Random replacement: place at each position
            for r_pos in range(int(m[listidx])):
                varp = var.copy()
                varp[head_pos + r_pos] = k + 1

                out_srv_list.append(space_srv_e.copy())
                out_var_list.append(varp)
                ac_val = _get_ac(ac, job_class, k, 0, l + 1)
                out_rate_list.append(ac_val * p[k] / m[listidx] * Immediate)


def _handle_hit_nonterminal(m, h, k, p, ac, job_class, repl_name, i_list, j_pos,
                             space_srv_e, var,
                             out_srv_list, out_var_list, out_rate_list, Immediate):
    """Handle cache hit in non-terminal list i < h: promote item."""
    for inew in range(i_list, h):
        varp = var.copy()

        if repl_name == 'FIFO':
            # Replace position (i, j) with evicted item from target list tail
            varp[_cpos(m, i_list, j_pos)] = var[_cpos(m, inew, int(m[inew]) - 1)]
            # Shift target list right, insert item at head
            if m[inew] > 1:
                head_inew = _cpos(m, inew, 0)
                tail_inew = _cpos(m, inew, int(m[inew]) - 1)
                varp[head_inew + 1:tail_inew + 1] = var[head_inew:tail_inew]
            varp[_cpos(m, inew, 0)] = k + 1

        elif repl_name == 'RR':
            # Random replacement: generate all swap possibilities
            for r_pos in range(int(m[inew])):
                varp2 = var.copy()
                varp2[_cpos(m, i_list, j_pos)] = var[_cpos(m, inew, r_pos)]
                varp2[_cpos(m, inew, r_pos)] = k + 1
                out_srv_list.append(space_srv_e.copy())
                out_var_list.append(varp2)
                ac_val = _get_ac(ac, job_class, k, 1 + i_list, 1 + inew)
                out_rate_list.append(ac_val * p[k] / m[inew] * Immediate)
            continue  # Already appended all states for this inew

        elif repl_name in ('LRU', 'SFIFO'):
            # Shift items in current list i to free position j
            if j_pos > 0:
                head_i = _cpos(m, i_list, 0)
                varp[head_i + 1:head_i + j_pos + 1] = var[head_i:head_i + j_pos]
            # Fill freed slot with evicted item from target list tail
            varp[_cpos(m, i_list, 0)] = var[_cpos(m, inew, int(m[inew]) - 1)]
            # Shift target list right, insert item at head
            if m[inew] > 1:
                head_inew = _cpos(m, inew, 0)
                tail_inew = _cpos(m, inew, int(m[inew]) - 1)
                varp[head_inew + 1:tail_inew + 1] = var[head_inew:tail_inew]
            varp[_cpos(m, inew, 0)] = k + 1

        else:
            continue

        out_srv_list.append(space_srv_e.copy())
        out_var_list.append(varp)
        ac_val = _get_ac(ac, job_class, k, 1 + i_list, 1 + inew)
        out_rate_list.append(ac_val * p[k] * Immediate)


def _handle_hit_terminal(m, h, k, p, repl_name, j_pos,
                          space_srv_e, var,
                          out_srv_list, out_var_list, out_rate_list, Immediate):
    """Handle cache hit in terminal list h."""
    if repl_name in ('FIFO', 'SFIFO', 'RR'):
        # No cache state change
        out_srv_list.append(space_srv_e.copy())
        out_var_list.append(var.copy())
        out_rate_list.append(p[k] * Immediate)

    elif repl_name == 'LRU':
        # Promote item to head of last list
        varp = var.copy()
        head_h = _cpos(m, h - 1, 0)
        if j_pos > 0:
            varp[head_h + 1:head_h + j_pos + 1] = var[head_h:head_h + j_pos]
        varp[head_h] = var[_cpos(m, h - 1, j_pos)]

        out_srv_list.append(space_srv_e.copy())
        out_var_list.append(varp)
        out_rate_list.append(p[k] * Immediate)


def _get_ac(ac, job_class, item, row, col):
    """
    Get access cost matrix entry.

    ac[class][item] is an (h+1) x (h+1) matrix.
    Row 0 = miss-to-list probs; row 1+i = hit-in-list-i-to-list probs.
    """
    if ac is None:
        return 1.0
    try:
        if isinstance(ac, (list, tuple)):
            if job_class < len(ac) and item < len(ac[job_class]):
                mat = ac[job_class][item]
                if isinstance(mat, np.ndarray):
                    return float(mat[row, col]) if mat.ndim == 2 else float(mat[col])
                elif isinstance(mat, (list, tuple)):
                    return float(mat[row][col]) if isinstance(mat[0], (list, tuple)) else float(mat[col])
        elif isinstance(ac, np.ndarray):
            return float(ac[job_class, item, row, col]) if ac.ndim == 4 else float(ac[row, col])
    except (IndexError, TypeError):
        pass
    return 1.0
