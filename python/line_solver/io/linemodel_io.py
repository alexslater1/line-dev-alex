"""
LINE Model JSON Save/Load

Provides save_model() and load_model() functions for serializing LINE
Network and LayeredNetwork models to/from JSON format conforming to
the line-model.schema.json specification.

Copyright (c) 2012-2026, Imperial College London
All rights reserved.
"""

import json
import math
import numpy as np
from typing import Any, Dict, Optional, Union


# ---------------------------------------------------------------------------
# Distribution serialization
# ---------------------------------------------------------------------------

def _dist_to_json(dist) -> Optional[Dict[str, Any]]:
    """Convert a LINE distribution object to JSON-compatible dict."""
    if dist is None:
        return None

    from ..distributions.continuous import (
        Exp, Det, Erlang, HyperExp, Gamma, Lognormal, Uniform, Immediate, Disabled, Pareto,
        Weibull, Normal, Expolynomial
    )
    from ..distributions.discrete import (
        Zipf, DiscreteSampler, Replayer, Geometric, Binomial, Poisson, Bernoulli, DiscreteUniform
    )
    from ..distributions.markovian import PH, APH, Coxian, MAP, MMPP2

    if isinstance(dist, Expolynomial):
        return {"type": "Expolynomial", "expolynomial": {
            "density": dist._density, "eft": dist._eft,
            "lft": dist._lft if math.isfinite(dist._lft) else "Inf"
        }}
    if isinstance(dist, Disabled):
        return {"type": "Disabled"}
    if isinstance(dist, Immediate):
        return {"type": "Immediate"}
    if isinstance(dist, Exp):
        return {"type": "Exp", "params": {"lambda": dist._rate}}
    if isinstance(dist, Det):
        return {"type": "Det", "params": {"value": dist._value}}
    if isinstance(dist, Erlang):
        return {"type": "Erlang", "params": {"lambda": dist._phase_rate, "k": int(dist._phases)}}
    if isinstance(dist, HyperExp):
        return {"type": "HyperExp", "params": {
            "p": _to_list(dist._probs), "lambda": _to_list(dist._rates)
        }}
    if isinstance(dist, Gamma):
        return {"type": "Gamma", "params": {"alpha": dist._shape, "beta": dist._rate}}
    if isinstance(dist, Lognormal):
        return {"type": "Lognormal", "params": {"mu": dist._mu, "sigma": dist._sigma}}
    if isinstance(dist, Uniform):
        return {"type": "Uniform", "params": {"a": dist._a, "b": dist._b}}
    if isinstance(dist, Zipf):
        return {"type": "Zipf", "params": {"s": dist._s, "n": int(dist._n)}}
    if isinstance(dist, Pareto):
        return {"type": "Pareto", "params": {"alpha": dist._alpha, "scale": dist._scale}}
    if isinstance(dist, Weibull):
        return {"type": "Weibull", "params": {"alpha": dist._scale, "beta": dist._shape}}
    if isinstance(dist, Normal):
        return {"type": "Normal", "params": {"mu": dist._mean_val, "sigma": dist._std}}
    if isinstance(dist, Geometric):
        return {"type": "Geometric", "params": {"p": dist._p}}
    if isinstance(dist, Binomial):
        return {"type": "Binomial", "params": {"n": int(dist._n), "p": dist._p}}
    if isinstance(dist, Poisson):
        return {"type": "Poisson", "params": {"lambda": dist._lambda}}
    if isinstance(dist, Bernoulli):
        return {"type": "Bernoulli", "params": {"p": dist._p}}
    if isinstance(dist, DiscreteUniform):
        return {"type": "DiscreteUniform", "params": {"min": dist._a, "max": dist._b}}
    if isinstance(dist, DiscreteSampler):
        return {"type": "DiscreteSampler", "params": {
            "p": _to_list(dist._probs), "x": _to_list(dist._values)
        }}
    # Replayer: save file path and mean as fallback, plus APH fit if possible
    if isinstance(dist, Replayer):
        rj = {"type": "Replayer", "params": {"fileName": dist._file_path}}
        try:
            rj["params"]["mean"] = dist.getMean()
        except Exception:
            pass
        try:
            aph = dist.fit_aph()
            if aph is not None and hasattr(aph, '_alpha') and hasattr(aph, '_T'):
                rj["ph"] = {
                    "alpha": _to_list(aph._alpha),
                    "T": _matrix_to_list(aph._T)
                }
        except Exception:
            pass
        return rj
    if isinstance(dist, MMPP2):
        return {"type": "MMPP2", "params": {
            "lambda0": float(dist._lambda0),
            "lambda1": float(dist._lambda1),
            "sigma0": float(dist._sigma0),
            "sigma1": float(dist._sigma1)
        }}
    if isinstance(dist, MAP):
        return {"type": "MAP", "map": {
            "D0": _matrix_to_list(dist._D0),
            "D1": _matrix_to_list(dist._D1)
        }}
    # Coxian before APH/PH (subclass check)
    if isinstance(dist, Coxian):
        rates = 1.0 / dist._means  # mu = 1/mean
        phi = np.append(dist._probs, 1.0)  # add implicit last phi=1.0
        return {"type": "Coxian", "params": {
            "mu": _to_list(rates),
            "phi": _to_list(phi)
        }}
    if isinstance(dist, APH):
        return {"type": "APH", "ph": {
            "alpha": _to_list(dist._alpha),
            "T": _matrix_to_list(dist._T)
        }}
    if isinstance(dist, PH):
        return {"type": "PH", "ph": {
            "alpha": _to_list(dist._alpha),
            "T": _matrix_to_list(dist._T)
        }}

    # Prior distribution (mixture of alternatives with prior probabilities)
    from ..distributions.continuous import Prior
    if isinstance(dist, Prior):
        alts = []
        for i in range(dist.getNumAlternatives()):
            alt_json = _dist_to_json(dist.getAlternative(i))
            if alt_json is not None:
                alts.append(alt_json)
        return {"type": "Prior", "distributions": alts,
                "probabilities": _to_list(dist.getProbabilities())}

    # Fallback: use fitMean with mean
    try:
        mean = dist.getMean()
        return {"type": "Exp", "fit": {"method": "fitMean", "mean": mean}}
    except Exception:
        return None


def _json_to_dist(d: Dict[str, Any]):
    """Convert a JSON dist dict back to a LINE distribution object."""
    from ..distributions.continuous import (
        Exp, Det, Erlang, HyperExp, Gamma, Lognormal, Uniform, Immediate, Disabled, Pareto,
        Weibull, Normal, Expolynomial
    )
    from ..distributions.discrete import (
        Zipf, DiscreteSampler, Replayer, Geometric, Binomial, Poisson, Bernoulli, DiscreteUniform
    )
    from ..distributions.markovian import PH, APH, MAP, Coxian, MMPP2

    dtype = d.get("type")
    params = d.get("params", {})
    fit = d.get("fit")
    ph = d.get("ph")
    mapspec = d.get("map")

    if dtype == "Expolynomial":
        ep = d.get("expolynomial", {})
        lft = float('inf') if ep.get("lft") == "Inf" else float(ep["lft"])
        return Expolynomial(ep["density"], float(ep["eft"]), lft)

    if dtype == "Disabled":
        return Disabled.getInstance()
    if dtype == "Immediate":
        return Immediate.getInstance()

    # Replayer: try file first, then APH fallback, then Exp fallback
    if dtype == "Replayer":
        file_path = params.get("fileName")
        if file_path:
            import os
            if os.path.isfile(file_path):
                return Replayer(file_path)
        # Fallback to APH if available
        if ph:
            alpha = np.array(ph["alpha"])
            T = np.array(ph["T"])
            return PH(alpha, T)
        # Fallback to Exp with stored mean
        mean = params.get("mean", 1.0)
        return Exp.fit_mean(mean)

    # Direct params
    if params:
        if dtype == "Exp":
            rate = params.get("lambda", params.get("rate"))
            return Exp(rate)
        if dtype == "Det":
            return Det(params["value"])
        if dtype == "Erlang":
            return Erlang(params["lambda"], int(params["k"]))
        if dtype == "HyperExp":
            p = params["p"]
            lam = params["lambda"]
            if len(p) == 2:
                return HyperExp(p[0], lam[0], lam[1])
            return HyperExp(np.array(p), np.array(lam))
        if dtype == "Gamma":
            return Gamma(params["alpha"], params["beta"])
        if dtype == "Lognormal":
            return Lognormal(params["mu"], params["sigma"])
        if dtype == "Uniform":
            return Uniform(params["a"], params["b"])
        if dtype == "Zipf":
            return Zipf(params["s"], int(params["n"]))
        if dtype == "Pareto":
            return Pareto(params["alpha"], params["scale"])
        if dtype == "Weibull":
            return Weibull(params["beta"], params["alpha"])  # constructor: Weibull(shape, scale)
        if dtype == "Normal":
            return Normal(params["mu"], params["sigma"])
        if dtype == "Geometric":
            return Geometric(params["p"])
        if dtype == "Binomial":
            return Binomial(int(params["n"]), params["p"])
        if dtype == "Poisson":
            return Poisson(params["lambda"])
        if dtype == "Bernoulli":
            return Bernoulli(params["p"])
        if dtype == "DiscreteUniform":
            return DiscreteUniform(int(params["min"]), int(params["max"]))
        if dtype == "DiscreteSampler":
            pv = np.array(params["p"])
            xv = np.array(params["x"])
            return DiscreteSampler(xv, pv)
        if dtype == "Coxian":
            mu = np.array(params["mu"])
            phi = np.array(params["phi"])
            return Coxian(mu, phi)

    # Prior distribution (mixture of alternatives with prior probabilities)
    if dtype == "Prior":
        from ..distributions.continuous import Prior
        dist_list = d.get("distributions", [])
        prob_list = d.get("probabilities", [])
        alternatives = [_json_to_dist(dd) for dd in dist_list]
        return Prior(alternatives, prob_list)

    # PH/APH representation
    if ph:
        alpha = np.array(ph["alpha"])
        T = np.array(ph["T"])
        if dtype == "APH":
            return APH(alpha, T)
        return PH(alpha, T)

    # MMPP2 (before generic MAP)
    if dtype == "MMPP2" and params:
        return MMPP2(params["lambda0"], params["lambda1"], params["sigma0"], params["sigma1"])

    # MAP representation
    if mapspec:
        D0 = np.array(mapspec["D0"])
        D1 = np.array(mapspec["D1"])
        return MAP(D0, D1)

    # Fit specification
    if fit:
        method = fit["method"]
        if method == "fitMean":
            mean = fit["mean"]
            if dtype == "Exp":
                return Exp.fit_mean(mean)
            if dtype == "Erlang":
                order = fit.get("order", 1)
                return Erlang.fit_mean_and_order(mean, order)
            if dtype == "Det":
                return Det(mean)
            # Default: Exp with given mean
            return Exp.fit_mean(mean)
        if method == "fitMeanAndSCV":
            mean = fit["mean"]
            scv = fit["scv"]
            if dtype == "Erlang":
                return Erlang.fit_mean_and_scv(mean, scv)
            if dtype == "HyperExp":
                return HyperExp.fit_mean_and_scv(mean, scv)
            if dtype == "Gamma":
                return Gamma.fit_mean_and_scv(mean, scv)
            if dtype == "Lognormal":
                return Lognormal.fit_mean_and_scv(mean, scv)
            return Exp.fit_mean(mean)
        if method == "fitMeanAndOrder":
            mean = fit["mean"]
            order = fit["order"]
            if dtype == "Erlang":
                return Erlang.fit_mean_and_order(mean, order)
            return Exp.fit_mean(mean)

    # Fallback
    return Exp(1.0)


# ---------------------------------------------------------------------------
# Network serialization
# ---------------------------------------------------------------------------

def _network_to_json(model) -> Dict[str, Any]:
    """Convert a Network model to JSON-compatible dict."""
    from ..lang.nodes import Queue, Delay, Source, Sink, Fork, Join, Router, ClassSwitch, Cache, Place, Transition
    from ..lang.classes import OpenClass, ClosedClass, OpenSignal, ClosedSignal, Signal, SignalType, RemovalPolicy
    from ..lang.base import SchedStrategy, NodeType, RoutingStrategy

    nodes = model.get_nodes()
    classes = model.get_classes()

    # Build nodes array
    nodes_json = []
    for node in nodes:
        # Skip implicit ClassSwitch nodes (auto-created by link())
        if isinstance(node, ClassSwitch) and getattr(node, '_auto_added', False):
            continue

        nj = {"name": node.name, "type": _node_type_str(node)}

        # Scheduling
        if isinstance(node, (Queue, Delay)):
            sched = node.get_sched_strategy()
            if sched is not None and sched != SchedStrategy.INF:
                nj["scheduling"] = sched.name if hasattr(sched, 'name') else str(sched).upper()
            elif isinstance(node, Delay):
                nj["scheduling"] = "INF"

        # Servers
        if isinstance(node, Queue) and not isinstance(node, Delay):
            ns = getattr(node, '_number_of_servers', 1)
            if ns is not None and np.isfinite(ns) and int(ns) != 1:
                nj["servers"] = int(ns)

        # Buffer
        if isinstance(node, Queue):
            cap = getattr(node, '_capacity', None)
            if cap is not None and np.isfinite(cap):
                nj["buffer"] = int(cap)

        # Per-class buffer capacity
        if isinstance(node, (Queue, Delay)):
            cc_dict = getattr(node, '_class_capacity', {})
            if cc_dict:
                cc_json = {}
                for jc, ccap in cc_dict.items():
                    if np.isfinite(ccap):
                        cc_json[jc.name] = int(ccap)
                if cc_json:
                    nj["classCap"] = cc_json

        # Drop rule
        if isinstance(node, (Queue, Delay)):
            from ..lang.base import DropStrategy
            dr = getattr(node, '_drop_rule', DropStrategy.DROP)
            dr_str = _drop_strategy_to_str(dr)
            if dr_str and dr_str != "drop":
                nj["dropRule"] = {jc.name: dr_str for jc in classes}

        # Load-dependent scaling
        if isinstance(node, (Queue, Delay)):
            lld = getattr(node, '_load_depend_scaling', None)
            if lld is not None:
                nj["loadDependence"] = {
                    "type": "loadDependent",
                    "scaling": [float(x) for x in lld]
                }

        # Service/arrival distributions
        if isinstance(node, Source):
            svc = {}
            for jc in classes:
                d = node._arrival_process.get(jc)
                if d is not None:
                    dj = _dist_to_json(d)
                    if dj is not None:
                        svc[jc.name] = dj
            if svc:
                nj["service"] = svc
        elif isinstance(node, (Queue, Delay)):
            svc = {}
            for jc in classes:
                d = node._service_process.get(jc)
                if d is not None:
                    dj = _dist_to_json(d)
                    if dj is not None:
                        svc[jc.name] = dj
            if svc:
                nj["service"] = svc

        # Scheduling params (DPS weights, etc.)
        if isinstance(node, Queue) and hasattr(node, '_sched_param') and node._sched_param:
            sp = {}
            for jc, val in node._sched_param.items():
                if val is not None:
                    sp[jc.name] = float(val)
            if sp:
                nj["schedParams"] = sp

        # ClassSwitch matrix
        if isinstance(node, ClassSwitch):
            csm = node.get_class_switching_matrix()
            if csm is not None:
                cs_dict = {}
                for ri, rc in enumerate(classes):
                    row = {}
                    for ci, cc in enumerate(classes):
                        if ri < csm.shape[0] and ci < csm.shape[1] and csm[ri, ci] != 0:
                            row[cc.name] = float(csm[ri, ci])
                    if row:
                        cs_dict[rc.name] = row
                if cs_dict:
                    nj["classSwitchMatrix"] = cs_dict

        # Join strategy and quorum
        if isinstance(node, Join):
            js_dict = getattr(node, '_join_strategy', {})
            for jc, js in js_dict.items():
                if js is not None and hasattr(js, 'name') and js.name != 'STD':
                    # Save as PARTIAL for MATLAB compatibility (QUORUM→PARTIAL)
                    _js_save_map = {'QUORUM': 'PARTIAL'}
                    nj["joinStrategy"] = _js_save_map.get(js.name, js.name)
                    break
            req_dict = getattr(node, '_required', {})
            for jc, req in req_dict.items():
                if req is not None and req > 0:
                    nj["joinQuorum"] = int(req)
                    break

        # Cache config
        if isinstance(node, Cache):
            cc = {}
            cc["items"] = int(node._num_items)
            cap = node._item_level_cap
            if isinstance(cap, np.ndarray):
                cc["capacity"] = [int(x) for x in cap]
            else:
                cc["capacity"] = int(cap)
            rs = node._replacement_strategy
            cc["replacement"] = rs.name if hasattr(rs, 'name') else str(rs)

            # Hit/miss class mappings
            if node._hit_class:
                hit_map = {}
                for in_cls, out_cls in node._hit_class.items():
                    hit_map[in_cls.name] = out_cls.name
                if hit_map:
                    cc["hitClass"] = hit_map
            if node._miss_class:
                miss_map = {}
                for in_cls, out_cls in node._miss_class.items():
                    miss_map[in_cls.name] = out_cls.name
                if miss_map:
                    cc["missClass"] = miss_map

            # Read popularity distributions (set_read)
            if node._read_process:
                pop_map = {}
                for jc, dist in node._read_process.items():
                    dj = _dist_to_json(dist)
                    if dj is not None:
                        pop_map[jc.name] = dj
                if pop_map:
                    cc["popularity"] = pop_map

            nj["cache"] = cc

        # Switchover times
        if isinstance(node, Queue):
            so_dict = getattr(node, '_switchover_times', None) or getattr(node, 'switchoverTimes', None)
            if so_dict:
                so_list = []
                for (from_cls, to_cls), dist in so_dict.items():
                    dj = _dist_to_json(dist)
                    if dj is not None:
                        so_list.append({
                            "from": from_cls.name,
                            "to": to_cls.name,
                            "distribution": dj
                        })
                if so_list:
                    nj["switchoverTimes"] = so_list

        # Heterogeneous server types
        if isinstance(node, Queue) and hasattr(node, 'is_heterogeneous') and node.is_heterogeneous():
            st_arr = []
            for st in node.get_server_types():
                stj = {"name": st.name, "count": st.get_num_of_servers()}
                # Compatible classes
                cc_list = st.get_compatible_classes()
                if cc_list:
                    stj["compatibleClasses"] = [c.name for c in cc_list]
                # Per-class service distributions
                svc = {}
                for jc in classes:
                    dist = node.get_hetero_service(jc, st)
                    if dist is not None:
                        dj = _dist_to_json(dist)
                        if dj is not None:
                            svc[jc.name] = dj
                if svc:
                    stj["service"] = svc
                st_arr.append(stj)
            if st_arr:
                nj["serverTypes"] = st_arr
                hsp = node.get_hetero_sched_policy()
                if hsp is not None:
                    from ..lang.base import HeteroSchedPolicy
                    if hsp != HeteroSchedPolicy.ORDER:
                        nj["heteroSchedPolicy"] = hsp.to_text()

        # Balking
        if isinstance(node, (Queue, Delay)):
            from ..lang.base import BalkingStrategy
            balk_json = {}
            for jc in classes:
                if node.has_balking(jc):
                    strategy, thresholds = node.get_balking(jc)
                    bjc = {"strategy": BalkingStrategy.to_text(strategy)}
                    th_arr = []
                    for th in thresholds:
                        min_j, max_j, prob = th[0], th[1], th[2]
                        tjson = {"minJobs": int(min_j), "probability": float(prob)}
                        if max_j == float('inf') or max_j == float('inf'):
                            tjson["maxJobs"] = -1
                        else:
                            tjson["maxJobs"] = int(max_j)
                        th_arr.append(tjson)
                    bjc["thresholds"] = th_arr
                    balk_json[jc.name] = bjc
            if balk_json:
                nj["balking"] = balk_json

        # Retrial
        if isinstance(node, (Queue, Delay)):
            ret_json = {}
            for jc in classes:
                if node.has_retrial(jc):
                    delay_dist, max_attempts = node.get_retrial(jc)
                    rjc = {
                        "delay": _dist_to_json(delay_dist),
                        "maxAttempts": int(max_attempts)
                    }
                    ret_json[jc.name] = rjc
            if ret_json:
                nj["retrial"] = ret_json

        # Patience
        if isinstance(node, (Queue, Delay)):
            from ..lang.base import ImpatienceType
            pat_json = {}
            for jc in classes:
                if node.has_patience(jc):
                    pat_dist = node.get_patience(jc)
                    pjc = {"distribution": _dist_to_json(pat_dist)}
                    imp_type = node.get_impatience_type(jc)
                    if imp_type is not None:
                        pjc["impatienceType"] = ImpatienceType.to_text(imp_type)
                    pat_json[jc.name] = pjc
            if pat_json:
                nj["patience"] = pat_json

        # Fork tasksPerLink
        if isinstance(node, Fork):
            tpl = getattr(node, '_tasks_per_link', None)
            if tpl is not None and tpl > 1:
                nj["tasksPerLink"] = int(tpl)

        # Join paired fork
        if isinstance(node, Join):
            fork_ref = getattr(node, '_fork', None)
            if fork_ref is not None and hasattr(fork_ref, 'name'):
                nj["forkNode"] = fork_ref.name

        # Transition modes
        if isinstance(node, Transition):
            modes_json = []
            n_modes = node.get_number_of_modes()
            all_nodes = model.get_nodes()
            for mi_idx in range(n_modes):
                mj = {}
                if mi_idx < len(node._mode_names):
                    mj["name"] = node._mode_names[mi_idx]
                else:
                    mj["name"] = f"Mode{mi_idx + 1}"
                # Distribution
                if mi_idx < len(node._distributions) and node._distributions[mi_idx] is not None:
                    dj = _dist_to_json(node._distributions[mi_idx])
                    if dj is not None:
                        mj["distribution"] = dj
                # Timing strategy
                if mi_idx < len(node._timing_strategies):
                    from ..lang.nodes import TimingStrategy
                    if node._timing_strategies[mi_idx] == TimingStrategy.IMMEDIATE:
                        mj["timingStrategy"] = "IMMEDIATE"
                    else:
                        mj["timingStrategy"] = "TIMED"
                # Number of servers
                if mi_idx < len(node._number_of_servers) and node._number_of_servers[mi_idx] > 1:
                    ns_val = node._number_of_servers[mi_idx]
                    if math.isinf(ns_val):
                        mj["numServers"] = "Infinity"
                    else:
                        mj["numServers"] = int(ns_val)
                # Firing priority
                if mi_idx < len(node._firing_priorities) and node._firing_priorities[mi_idx] > 0:
                    mj["firingPriority"] = float(node._firing_priorities[mi_idx])
                # Firing weight
                if mi_idx < len(node._firing_weights) and node._firing_weights[mi_idx] != 1.0:
                    mj["firingWeight"] = float(node._firing_weights[mi_idx])
                # Enabling conditions
                if mi_idx < len(node._enabling_conditions):
                    ec_mat = node._enabling_conditions[mi_idx]
                    ec_list = []
                    for ni in range(ec_mat.shape[0]):
                        for ci in range(ec_mat.shape[1]):
                            if ec_mat[ni, ci] > 0:
                                ec_list.append({
                                    "node": all_nodes[ni].name,
                                    "class": classes[ci].name,
                                    "count": float(ec_mat[ni, ci])
                                })
                    if ec_list:
                        mj["enablingConditions"] = ec_list
                # Inhibiting conditions
                if mi_idx < len(node._inhibiting_conditions):
                    ic_mat = node._inhibiting_conditions[mi_idx]
                    ic_list = []
                    for ni in range(ic_mat.shape[0]):
                        for ci in range(ic_mat.shape[1]):
                            if np.isfinite(ic_mat[ni, ci]):
                                ic_list.append({
                                    "node": all_nodes[ni].name,
                                    "class": classes[ci].name,
                                    "count": float(ic_mat[ni, ci])
                                })
                    if ic_list:
                        mj["inhibitingConditions"] = ic_list
                # Firing outcomes
                if mi_idx < len(node._firing_outcomes):
                    fo_mat = node._firing_outcomes[mi_idx]
                    fo_list = []
                    for ni in range(fo_mat.shape[0]):
                        for ci in range(fo_mat.shape[1]):
                            if fo_mat[ni, ci] != 0:
                                fo_list.append({
                                    "node": all_nodes[ni].name,
                                    "class": classes[ci].name,
                                    "count": float(fo_mat[ni, ci])
                                })
                    if fo_list:
                        mj["firingOutcomes"] = fo_list
                modes_json.append(mj)
            if modes_json:
                nj["modes"] = modes_json

        # Initial state for Place nodes (token counts)
        if isinstance(node, Place):
            state = getattr(node, '_state', None)
            if state is not None:
                nj["initialState"] = [float(x) for x in np.atleast_1d(state)]

        nodes_json.append(nj)

    # Build classes array
    classes_json = []
    for jc in classes:
        cj = {"name": jc.name}
        if isinstance(jc, OpenSignal):
            cj["type"] = "Signal"
            cj["openOrClosed"] = "Open"
            cj["signalType"] = jc._signal_type.value if hasattr(jc._signal_type, 'value') else str(jc._signal_type)
            target = getattr(jc, '_target_job_class', None)
            if target is not None:
                cj["targetClass"] = target.name
            rem_dist = getattr(jc, '_removal_distribution', None)
            if rem_dist is not None:
                cj["removalDistribution"] = _dist_to_json(rem_dist)
            rem_pol = getattr(jc, '_removal_policy', None)
            if rem_pol is not None and rem_pol != RemovalPolicy.RANDOM:
                cj["removalPolicy"] = rem_pol.value if hasattr(rem_pol, 'value') else str(rem_pol)
        elif isinstance(jc, ClosedSignal):
            cj["type"] = "Signal"
            cj["openOrClosed"] = "Closed"
            cj["signalType"] = jc._signal_type.value if hasattr(jc._signal_type, 'value') else str(jc._signal_type)
            ref = jc._refstat
            if ref is not None:
                cj["refNode"] = ref.name
            target = getattr(jc, '_target_job_class', None)
            if target is not None:
                cj["targetClass"] = target.name
            rem_dist = getattr(jc, '_removal_distribution', None)
            if rem_dist is not None:
                cj["removalDistribution"] = _dist_to_json(rem_dist)
            rem_pol = getattr(jc, '_removal_policy', None)
            if rem_pol is not None and rem_pol != RemovalPolicy.RANDOM:
                cj["removalPolicy"] = rem_pol.value if hasattr(rem_pol, 'value') else str(rem_pol)
        elif isinstance(jc, OpenClass):
            cj["type"] = "Open"
        elif isinstance(jc, ClosedClass):
            cj["type"] = "Closed"
            cj["population"] = int(jc._njobs)
            ref = jc._refstat
            if ref is not None:
                cj["refNode"] = ref.name
        else:
            cj["type"] = "Open"

        prio = getattr(jc, '_priority', 0)
        if prio != 0:
            cj["priority"] = int(prio)

        deadline = getattr(jc, '_deadline', float('inf'))
        if np.isfinite(deadline):
            cj["deadline"] = float(deadline)

        classes_json.append(cj)

    # Build routing matrix
    routing_json = _build_routing_json(model, nodes, classes)

    # Build routing strategies (per-node, per-class)
    routing_strategies_json = {}
    for node in nodes:
        strats = getattr(node, '_routing_strategies', {})
        if strats:
            node_strats = {}
            for jc, strat in strats.items():
                if strat is not None and strat != RoutingStrategy.PROB and strat != RoutingStrategy.RAND:
                    node_strats[jc.name] = strat.name
            if node_strats:
                routing_strategies_json[node.name] = node_strats
    # Routing weights (for WRROBIN)
    routing_weights_json = {}
    for node in nodes:
        weights = getattr(node, '_routing_weights', {})
        if weights:
            node_weights = {}
            for jc, dest_weights in weights.items():
                if dest_weights:
                    cls_weights = {}
                    for dest_node, weight in dest_weights.items():
                        dest_name = dest_node.name if hasattr(dest_node, 'name') else str(dest_node)
                        cls_weights[dest_name] = float(weight)
                    if cls_weights:
                        node_weights[jc.name] = cls_weights
            if node_weights:
                routing_weights_json[node.name] = node_weights

    # Build finite capacity regions
    fcr_json = []
    regions = getattr(model, '_regions', None) or getattr(model, 'regions', [])
    if regions:
        for region in regions:
            rj = {"name": getattr(region, 'name', 'FCR')}
            region_nodes = getattr(region, '_nodes', None) or getattr(region, 'nodes', [])
            # Stations with per-class details
            stations_json = []
            if region_nodes:
                for rn in region_nodes:
                    sj = {"node": rn.name if hasattr(rn, 'name') else str(rn)}
                    # Per-class classWeight
                    cw_dict = getattr(region, '_class_weight', {})
                    if cw_dict:
                        cw_json = {}
                        for cls, w in cw_dict.items():
                            if w != 1.0:
                                cw_json[cls.name] = float(w)
                        if cw_json:
                            sj["classWeight"] = cw_json
                    # Per-class classSize
                    cs_dict = getattr(region, '_class_size', {})
                    if cs_dict:
                        cs_json = {}
                        for cls, sz in cs_dict.items():
                            if sz != 1:
                                cs_json[cls.name] = int(sz)
                        if cs_json:
                            sj["classSize"] = cs_json
                    stations_json.append(sj)
            rj["stations"] = stations_json
            max_jobs = getattr(region, '_global_max_jobs', None)
            if max_jobs is not None and max_jobs >= 0:
                rj["globalMaxJobs"] = int(max_jobs)
            max_mem = getattr(region, '_global_max_memory', None)
            if max_mem is not None and max_mem >= 0:
                rj["globalMaxMemory"] = int(max_mem)
            # classMaxJobs
            cmj_dict = getattr(region, '_class_max_jobs', {})
            if cmj_dict:
                cmj_json = {}
                for cls, cmj in cmj_dict.items():
                    if cmj >= 0:
                        cmj_json[cls.name] = int(cmj)
                if cmj_json:
                    rj["classMaxJobs"] = cmj_json
            # dropRule
            dr_dict = getattr(region, '_drop_rules', {})
            if dr_dict:
                dr_json = {}
                for cls, dr in dr_dict.items():
                    dr_str = _drop_strategy_to_str(dr) if hasattr(dr, 'name') else str(dr)
                    if dr_str:
                        dr_json[cls.name] = dr_str
                if dr_json:
                    rj["dropRule"] = dr_json
            fcr_json.append(rj)

    result = {
        "type": "Network",
        "name": model.name,
        "nodes": nodes_json,
        "classes": classes_json,
        "routing": routing_json
    }
    if routing_strategies_json:
        result["routingStrategies"] = routing_strategies_json
    if routing_weights_json:
        result["routingWeights"] = routing_weights_json
    if fcr_json:
        result["finiteCapacityRegions"] = fcr_json

    return result


def _build_routing_json(model, nodes, classes) -> Dict[str, Any]:
    """Build routing JSON from the model's routing matrix."""
    matrix = {}

    # Use sn.rtnodes as primary source - it has computed routing for ALL
    # routing strategies (PROB, RROBIN, WRROBIN, etc.)
    try:
        sn = model.getStruct()
        if sn is not None and hasattr(sn, 'rtnodes') and sn.rtnodes is not None:
            rt = sn.rtnodes
            K = len(classes)
            N = len(nodes)
            # Identify auto-added ClassSwitch nodes to collapse routing through them
            auto_cs_indices = set()
            from ..lang.nodes import ClassSwitch as ClassSwitchNode
            from ..lang.nodes import Source as SourceNode, Sink as SinkNode
            from ..lang.nodes import Cache as CacheNode
            sink_indices = set()
            source_indices = set()
            cache_indices = set()
            for i, nd in enumerate(nodes):
                if isinstance(nd, ClassSwitchNode) and getattr(nd, '_auto_added', False):
                    auto_cs_indices.add(i)
                if isinstance(nd, SinkNode):
                    sink_indices.add(i)
                if isinstance(nd, SourceNode):
                    source_indices.add(i)
                if isinstance(nd, CacheNode):
                    cache_indices.add(i)

            # Build set of (cache_idx, read_class_idx, hit_or_miss_class_idx)
            # tuples representing Cache-internal class-switching that should
            # NOT be saved in routing (Cache reconstructs these from its properties)
            cache_internal_cs = set()
            for ci in cache_indices:
                cnode = nodes[ci]
                for attr in ('_hit_class', '_miss_class'):
                    mapping = getattr(cnode, attr, None)
                    if mapping:
                        for read_cls, hm_cls in mapping.items():
                            r_idx = classes.index(read_cls) if read_cls in classes else -1
                            s_idx = classes.index(hm_cls) if hm_cls in classes else -1
                            if r_idx >= 0 and s_idx >= 0:
                                cache_internal_cs.add((ci, r_idx, s_idx))

            # Build set of (node_idx, class_idx) pairs that are confirmed
            # non-visiting (DTMC artifacts). A pair is non-visiting only when
            # BOTH nodevisits is zero AND the station's procid is DISABLED.
            # Using only nodevisits can be too aggressive because fork-join
            # class-switching models may have zero nodevisits for classes that
            # DO actually visit (nodevisits computation bug).
            import numpy as np
            from ..constants import ProcessType
            non_visiting = set()  # (node_idx, class_idx)
            if hasattr(sn, 'nodevisits') and sn.nodevisits is not None:
                # Collect all (node, class) with zero visits
                zero_visits = set()
                for i_n in range(N):
                    for r_c in range(K):
                        has_visit = False
                        for c in range(sn.nchains):
                            nv = sn.nodevisits[c]
                            if i_n < nv.shape[0] and r_c < nv.shape[1]:
                                if abs(nv[i_n, r_c]) > 1e-14:
                                    has_visit = True
                                    break
                        if not has_visit:
                            zero_visits.add((i_n, r_c))
                # Only mark as non-visiting if ALSO procid==DISABLED
                # (for station nodes) or if the node is a non-station
                # AND has no explicit routing in _routing_matrix._routes.
                # nodevisits alone is unreliable for fork-join class-
                # switching models (nodevisits bug).
                # Build set of (node_name, class_idx) with explicit routes
                _routed_pairs = set()  # (node_name, class_idx)
                rm = getattr(model, '_routing_matrix', None)
                if rm is not None and hasattr(rm, '_routes'):
                    _rt_routes = getattr(rm, '_original_routes', None) or rm._routes
                    for (cs, _cd), rd in _rt_routes.items():
                        cs_idx = classes.index(cs) if cs in classes else -1
                        cd_idx = classes.index(_cd) if _cd in classes else -1
                        for (ns, _nd), prob in rd.items():
                            if prob > 0:
                                _routed_pairs.add((ns.name, cs_idx))
                for (i_n, r_c) in zero_visits:
                    ist = sn.nodeToStation[i_n] if hasattr(sn, 'nodeToStation') else -1
                    if ist >= 0 and ist < sn.nstations:
                        try:
                            pid = sn.procid[ist, r_c]
                            if pid == ProcessType.DISABLED:
                                non_visiting.add((i_n, r_c))
                        except (IndexError, TypeError):
                            pass
                    elif ist < 0:
                        # Non-station node: only mark non-visiting if
                        # it has no explicit route in the original routing
                        nname = nodes[i_n].name if i_n < len(nodes) else ''
                        if (nname, r_c) not in _routed_pairs:
                            non_visiting.add((i_n, r_c))

            if auto_cs_indices:
                # Collapse routing through auto-added CS nodes:
                # For path i→CS→j, effective prob = sum over CS of rt[i,CS] * rt[CS,j]
                # where rt entries include class indices: rt[(i,r),(CS,t)] * rt[(CS,t),(j,s)]
                dim = N * K
                rt_arr = np.zeros((dim, dim))
                for a in range(dim):
                    for b in range(dim):
                        rt_arr[a, b] = rt[a, b]
                # Iteratively eliminate auto-CS nodes
                for cs_idx in auto_cs_indices:
                    for r_in in range(K):
                        cs_col = cs_idx * K + r_in
                        for i in range(N):
                            if i in auto_cs_indices:
                                continue
                            for r_src in range(K):
                                src = i * K + r_src
                                p_to_cs = rt_arr[src, cs_col]
                                if p_to_cs < 1e-14:
                                    continue
                                # Route through CS to all successors
                                for j in range(N):
                                    for s_dst in range(K):
                                        dst = j * K + s_dst
                                        p_from_cs = rt_arr[cs_col, dst]
                                        if p_from_cs < 1e-14:
                                            continue
                                        rt_arr[src, dst] += p_to_cs * p_from_cs
                                rt_arr[src, cs_col] = 0.0
                # Now extract routing, skipping auto-CS nodes,
                # cross-class Sink->Source loopback entries (DTMC artifacts),
                # and cross-class entries from Cache nodes (Cache handles
                # class-switching internally via hitClass/missClass).
                for r in range(K):
                    for s in range(K):
                        from_to = {}
                        for i in range(N):
                            if i in auto_cs_indices:
                                continue
                            # Skip Cache-internal class-switching entries
                            # (read-class → hit/miss class); these are
                            # reconstructed from Cache properties on load
                            if (i, r, s) in cache_internal_cs:
                                continue
                            # Skip entries where source class r has zero
                            # visits at node i (DTMC artifacts)
                            if (i, r) in non_visiting:
                                continue
                            for j in range(N):
                                if j in auto_cs_indices:
                                    continue
                                # Skip cross-class Sink->Source entries (internal DTMC artifacts)
                                if r != s and i in sink_indices and j in source_indices:
                                    continue
                                val = rt_arr[i * K + r, j * K + s]
                                if val > 1e-14:
                                    ni = nodes[i].name
                                    nj = nodes[j].name
                                    if ni not in from_to:
                                        from_to[ni] = {}
                                    from_to[ni][nj] = float(val)
                        if from_to:
                            key = f"{classes[r].name},{classes[s].name}"
                            matrix[key] = from_to
            else:
                # No auto-added CS nodes, use rtnodes directly.
                # For explicit ClassSwitch nodes, _refresh_routing applies the
                # Pcs switch matrix to outgoing routes, producing cross-class
                # entries (e.g. CS/Class1 → Delay/Class2).  We undo this so
                # the JSON stores same-class routing from the CS node and the
                # classSwitchMatrix (already in JSON) captures the switching.
                # Otherwise, loading re-inserts auto-CS nodes that double-switch.
                import numpy as np
                explicit_cs_indices = set()
                explicit_cs_pcs = {}  # cs_idx -> Pcs matrix (K x K)
                for i, nd in enumerate(nodes):
                    if isinstance(nd, ClassSwitchNode) and not getattr(nd, '_auto_added', False):
                        explicit_cs_indices.add(i)
                        explicit_cs_pcs[i] = nd.get_class_switching_matrix()

                # Pre-compute base routing for explicit CS nodes by undoing Pcs.
                # rtnodes[CS*K+r, j*K+s] = Pcs[r,s] * P_route(CS, j, s)
                # so P_route(CS, j, s) = rtnodes[CS*K+r, j*K+s] / Pcs[r,s]
                # for any r where Pcs[r,s] > 0.
                cs_base_route = {}  # (cs_idx, s, j) -> P_route
                for cs_idx in explicit_cs_indices:
                    pcs = explicit_cs_pcs[cs_idx]
                    for s in range(K):
                        # Find any source class r with Pcs[r,s] > 0
                        ref_r = None
                        for r in range(K):
                            if pcs is not None and pcs[r, s] > 1e-14:
                                ref_r = r
                                break
                        if ref_r is None:
                            continue
                        for j in range(N):
                            val = rt[cs_idx * K + ref_r, j * K + s]
                            if val > 1e-14:
                                p_route = float(val / pcs[ref_r, s])
                                cs_base_route[(cs_idx, s, j)] = p_route

                for r in range(K):
                    for s in range(K):
                        from_to = {}
                        for i in range(N):
                            if i in explicit_cs_indices:
                                # Use base routing (same-class only)
                                if r == s:
                                    for j in range(N):
                                        val = cs_base_route.get((i, s, j), 0.0)
                                        if val > 1e-14:
                                            ni = nodes[i].name
                                            nj = nodes[j].name
                                            if ni not in from_to:
                                                from_to[ni] = {}
                                            from_to[ni][nj] = val
                                # Skip cross-class entries for explicit CS
                                continue
                            # Skip cross-class entries from Cache nodes (Cache
                            # handles class-switching internally via hitClass/missClass)
                            if r != s and i in cache_indices:
                                continue
                            # When explicit CS nodes exist, non-CS nodes should
                            # only store same-class entries — cross-class entries
                            # at non-CS nodes are propagated from CS via rtnodes
                            # and would cause spurious auto-CS insertion on reload.
                            if r != s and explicit_cs_indices:
                                continue
                            # Skip entries where source class r has zero
                            # visits at node i (DTMC artifacts)
                            if (i, r) in non_visiting:
                                continue
                            for j in range(N):
                                # Skip cross-class Sink->Source entries (DTMC artifacts)
                                if r != s and i in sink_indices and j in source_indices:
                                    continue
                                val = rt[i * K + r, j * K + s]
                                if val > 1e-14:
                                    ni = nodes[i].name
                                    nj = nodes[j].name
                                    if ni not in from_to:
                                        from_to[ni] = {}
                                    from_to[ni][nj] = float(val)
                        if from_to:
                            key = f"{classes[r].name},{classes[s].name}"
                            matrix[key] = from_to
    except Exception:
        pass

    # Fallback to _routing_matrix._routes if rtnodes approach didn't work
    if not matrix:
        rm = getattr(model, '_routing_matrix', None)
        if rm is not None and hasattr(rm, '_routes'):
            routes = rm._routes
            original_routes = getattr(rm, '_original_routes', None)
            if original_routes is not None:
                routes = original_routes
            for (cs, cd), route_dict in routes.items():
                key = f"{cs.name},{cd.name}"
                from_to = {}
                for (ns, nd), prob in route_dict.items():
                    if prob > 0:
                        if ns.name not in from_to:
                            from_to[ns.name] = {}
                        from_to[ns.name][nd.name] = prob
                if from_to:
                    matrix[key] = from_to

    return {"type": "matrix", "matrix": matrix}


def _json_to_network(data: Dict[str, Any]):
    """Reconstruct a Network from JSON data."""
    from ..lang.network import Network
    from ..lang.nodes import Queue, Delay, Source, Sink, Fork, Join, Router, ClassSwitch, Cache, Place, Transition
    from ..lang.classes import OpenClass, ClosedClass, OpenSignal, ClosedSignal, SignalType, RemovalPolicy
    from ..lang.routing import RoutingMatrix
    from ..lang.base import SchedStrategy, ReplacementStrategy
    from ..distributions.continuous import Disabled

    model = Network(data.get("name", "model"))

    # Create nodes
    node_map = {}
    for nd in data.get("nodes", []):
        name = nd["name"]
        ntype = nd["type"]
        node = _create_node(model, nd)
        node_map[name] = node

    # Deferred Join fork linking
    for nd in data.get("nodes", []):
        if nd.get("type") == "Join" and "forkNode" in nd:
            join_node = node_map.get(nd["name"])
            fork_node = node_map.get(nd["forkNode"])
            if join_node is not None and fork_node is not None:
                join_node._fork = fork_node

    # Create classes
    class_map = {}
    for cd in data.get("classes", []):
        cname = cd["name"]
        ctype = cd["type"]
        if ctype == "Open":
            prio = cd.get("priority", 0)
            jc = OpenClass(model, cname, prio)
        elif ctype == "Closed":
            pop = cd["population"]
            ref_name = cd.get("refNode")
            ref_node = node_map.get(ref_name) if ref_name else None
            prio = cd.get("priority", 0)
            jc = ClosedClass(model, cname, pop, ref_node, prio)
        elif ctype == "Signal":
            prio = cd.get("priority", 0)
            sig_type_str = cd.get("signalType", "negative")
            sig_type = SignalType(sig_type_str)
            open_or_closed = cd.get("openOrClosed", "Open")
            if open_or_closed == "Closed":
                ref_name = cd.get("refNode")
                ref_node = node_map.get(ref_name) if ref_name else None
                jc = ClosedSignal(model, cname, sig_type, ref_node, prio)
            else:
                jc = OpenSignal(model, cname, sig_type, prio)
            # Removal distribution
            rem_dist_json = cd.get("removalDistribution")
            if rem_dist_json is not None:
                rem_dist = _json_to_dist(rem_dist_json)
                if rem_dist is not None:
                    jc.setRemovalDistribution(rem_dist)
            # Removal policy
            rem_pol_str = cd.get("removalPolicy")
            if rem_pol_str is not None:
                jc.setRemovalPolicy(RemovalPolicy(rem_pol_str))
        else:
            jc = OpenClass(model, cname)
        deadline = cd.get("deadline")
        if deadline is not None and np.isfinite(deadline):
            jc._deadline = float(deadline)
        class_map[cname] = jc

    # Resolve signal targetClass associations
    for cd in data.get("classes", []):
        if cd.get("type") != "Signal":
            continue
        target_name = cd.get("targetClass")
        if target_name and cd["name"] in class_map and target_name in class_map:
            sig_cls = class_map[cd["name"]]
            sig_cls.forJobClass(class_map[target_name])

    # Set service/arrival distributions
    for nd in data.get("nodes", []):
        name = nd["name"]
        node = node_map[name]
        svc = nd.get("service", {})
        for cname, dist_json in svc.items():
            jc = class_map.get(cname)
            if jc is None:
                continue
            dist = _json_to_dist(dist_json)
            if dist is not None:
                if isinstance(node, Source):
                    node.set_arrival(jc, dist)
                elif isinstance(node, (Queue, Delay)):
                    node.set_service(jc, dist)

        # Per-class buffer capacity
        cc = nd.get("classCap", {})
        if cc and isinstance(node, (Queue, Delay)):
            for cname, cap_val in cc.items():
                jc = class_map.get(cname)
                if jc is not None and int(cap_val) > 0:
                    node.set_class_capacity(jc, int(cap_val))

        # Drop rule
        dr = nd.get("dropRule", {})
        if dr and isinstance(node, (Queue, Delay)):
            # Python uses station-wide drop rule; pick the first entry
            for cname, dr_str in dr.items():
                node.set_drop_rule(_str_to_drop_strategy(dr_str))
                break

        # Load-dependent scaling
        ld = nd.get("loadDependence")
        if ld and isinstance(node, (Queue, Delay)):
            ld_type = ld.get("type", "loadDependent")
            if ld_type == "loadDependent" and "scaling" in ld:
                node.set_load_dependence(np.array(ld["scaling"]))

        # Join strategy and quorum
        if isinstance(node, Join):
            js_str = nd.get("joinStrategy")
            if js_str is not None:
                from ..lang.base import JoinStrategy
                # Map aliases from other codebases
                _js_map = {'PARTIAL': 'QUORUM', 'Quorum': 'QUORUM', 'Partial': 'QUORUM'}
                js_key = _js_map.get(js_str, js_str)
                js = getattr(JoinStrategy, js_key, JoinStrategy.STD)
                for jc in class_map.values():
                    node.set_strategy(jc, js)
            jq = nd.get("joinQuorum")
            if jq is not None:
                for jc in class_map.values():
                    node.set_required(jc, int(jq))

        # Heterogeneous server types
        st_arr = nd.get("serverTypes")
        if st_arr and isinstance(node, Queue):
            from ..lang.constant.server_type import ServerType
            for stData in st_arr:
                st_name = stData["name"]
                st_count = stData["count"]
                st = ServerType(st_name, st_count)
                # Compatible classes
                for cc_name in stData.get("compatibleClasses", []):
                    jc = class_map.get(cc_name)
                    if jc is not None:
                        st.add_compatible(jc)
                node.add_server_type(st)
                # Per-class service distributions
                for cname, dist_json in stData.get("service", {}).items():
                    jc = class_map.get(cname)
                    if jc is not None:
                        dist = _json_to_dist(dist_json)
                        if dist is not None:
                            node.set_hetero_service(jc, st, dist)
            # Scheduling policy
            hsp_str = nd.get("heteroSchedPolicy")
            if hsp_str:
                from ..lang.base import HeteroSchedPolicy
                hsp = HeteroSchedPolicy.from_text(hsp_str)
                node.set_hetero_sched_policy(hsp)

        # Balking
        balk_data = nd.get("balking")
        if balk_data and isinstance(node, (Queue, Delay)):
            from ..lang.base import BalkingStrategy
            for cname, bjc in balk_data.items():
                jc = class_map.get(cname)
                if jc is None:
                    continue
                strategy = BalkingStrategy.from_text(bjc["strategy"])
                thresholds = []
                for td in bjc.get("thresholds", []):
                    min_j = td["minJobs"]
                    max_j = td["maxJobs"]
                    if max_j < 0:
                        max_j = float('inf')
                    prob = td["probability"]
                    thresholds.append((min_j, max_j, prob))
                node.set_balking(jc, strategy, thresholds)

        # Retrial
        ret_data = nd.get("retrial")
        if ret_data and isinstance(node, (Queue, Delay)):
            for cname, rjc in ret_data.items():
                jc = class_map.get(cname)
                if jc is None:
                    continue
                delay_dist = _json_to_dist(rjc["delay"])
                max_attempts = rjc.get("maxAttempts", -1)
                if delay_dist is not None:
                    node.set_retrial(jc, delay_dist, max_attempts)

        # Patience
        pat_data = nd.get("patience")
        if pat_data and isinstance(node, (Queue, Delay)):
            from ..lang.base import ImpatienceType
            for cname, pjc in pat_data.items():
                jc = class_map.get(cname)
                if jc is None:
                    continue
                pat_dist = _json_to_dist(pjc["distribution"])
                imp_type = ImpatienceType.RENEGING
                imp_str = pjc.get("impatienceType")
                if imp_str:
                    imp_type = ImpatienceType.from_text(imp_str)
                if pat_dist is not None:
                    node.set_patience(jc, pat_dist, imp_type)

        # Set scheduling params
        sp = nd.get("schedParams", {})
        if sp and isinstance(node, Queue):
            for cname, val in sp.items():
                jc = class_map.get(cname)
                if jc is not None:
                    node._sched_param[jc] = val

        # Set class switch matrix (dict format: "classSwitchMatrix")
        csm = nd.get("classSwitchMatrix")
        if csm and isinstance(node, ClassSwitch):
            classes_list = model.get_classes()
            K = len(classes_list)
            mat = np.zeros((K, K))
            class_idx = {c.name: i for i, c in enumerate(classes_list)}
            for from_name, to_dict in csm.items():
                ri = class_idx.get(from_name, -1)
                if ri < 0:
                    continue
                for to_name, prob in to_dict.items():
                    ci = class_idx.get(to_name, -1)
                    if ci >= 0:
                        mat[ri, ci] = prob
            node.set_class_switching_matrix(mat)
        # Legacy 2D array format: "csMatrix" (from older JAR saves)
        elif not csm and isinstance(node, ClassSwitch):
            cs_arr = nd.get("csMatrix")
            if cs_arr and isinstance(cs_arr, list):
                mat = np.array(cs_arr, dtype=float)
                node.set_class_switching_matrix(mat)

        # Cache hit/miss class mappings and popularity distributions
        # Support both Python format (nested "cache" key) and JAR format
        # (flat top-level keys: hitClass, missClass, popularity)
        if isinstance(node, Cache):
            cc = nd.get("cache", {})
            cache_src = cc if cc else nd
            # Hit class mapping
            hc = cache_src.get("hitClass", {})
            for in_name, out_name in hc.items():
                in_cls = class_map.get(in_name)
                out_cls = class_map.get(out_name)
                if in_cls is not None and out_cls is not None:
                    node.set_hit_class(in_cls, out_cls)
            # Miss class mapping
            mc = cache_src.get("missClass", {})
            for in_name, out_name in mc.items():
                in_cls = class_map.get(in_name)
                out_cls = class_map.get(out_name)
                if in_cls is not None and out_cls is not None:
                    node.set_miss_class(in_cls, out_cls)
            # Popularity distributions (set_read)
            pop = cache_src.get("popularity", {})
            for cname, dist_json in pop.items():
                jc = class_map.get(cname)
                if jc is not None:
                    pop_dist = _json_to_dist(dist_json)
                    if pop_dist is not None and not isinstance(pop_dist, Disabled):
                        node.set_read(jc, pop_dist)

    # Configure Transition modes
    for nd in data.get("nodes", []):
        if nd.get("type") != "Transition" or "modes" not in nd:
            continue
        tnode = node_map[nd["name"]]
        for md in nd["modes"]:
            mode_name = md.get("name", "Mode")
            mode = tnode.add_mode(mode_name)
            # Distribution
            dist_data = md.get("distribution")
            if dist_data:
                dist = _json_to_dist(dist_data)
                if dist is not None:
                    tnode.set_distribution(mode, dist)
            # Timing strategy
            ts = md.get("timingStrategy")
            if ts:
                from ..lang.nodes import TimingStrategy
                if ts == "IMMEDIATE":
                    tnode.set_timing_strategy(mode, TimingStrategy.IMMEDIATE)
                else:
                    tnode.set_timing_strategy(mode, TimingStrategy.TIMED)
            # Number of servers
            ns = md.get("numServers")
            if ns is not None:
                if isinstance(ns, str):
                    ns = float('inf') if ns.lower() == 'infinity' else float(ns)
                if ns > 1:
                    tnode.set_number_of_servers(mode, ns)
            # Firing priority
            fp = md.get("firingPriority")
            if fp is not None:
                tnode.set_firing_priorities(mode, fp)
            # Firing weight
            fw = md.get("firingWeight")
            if fw is not None:
                tnode.set_firing_weights(mode, fw)
            # Enabling conditions
            for ec in md.get("enablingConditions", []):
                ec_node = node_map.get(ec["node"])
                ec_cls = class_map.get(ec["class"])
                if ec_node is not None and ec_cls is not None:
                    tnode.set_enabling_conditions(mode, ec_cls, ec_node, ec["count"])
            # Inhibiting conditions
            for ic in md.get("inhibitingConditions", []):
                ic_node = node_map.get(ic["node"])
                ic_cls = class_map.get(ic["class"])
                if ic_node is not None and ic_cls is not None:
                    tnode.set_inhibiting_conditions(mode, ic_cls, ic_node, ic["count"])
            # Firing outcomes
            for fo in md.get("firingOutcomes", []):
                fo_node = node_map.get(fo["node"])
                fo_cls = class_map.get(fo["class"])
                if fo_node is not None and fo_cls is not None:
                    tnode.set_firing_outcome(mode, fo_cls, fo_node, fo["count"])

    # Build routing
    routing_data = data.get("routing", {})
    if routing_data.get("type") == "matrix":
        P = RoutingMatrix(model)
        matrix = routing_data.get("matrix", {})
        # Build set of Cache-internal cross-class routing entries to skip.
        # MATLAB/JAR save Cache self-loop entries (e.g., InitClass→HitClass at
        # Cache→Cache) with actual hit/miss probabilities. Python's Cache node
        # reconstructs these internally from hitClass/missClass properties, so
        # loading them would cause double-counting.
        cache_internal_cs = set()
        sink_names = set()
        source_names = set()
        for nname, nobj in node_map.items():
            if isinstance(nobj, Cache):
                for attr in ('_hit_class', '_miss_class'):
                    mapping = getattr(nobj, attr, None)
                    if mapping:
                        for read_cls, hm_cls in mapping.items():
                            cache_internal_cs.add((nname, read_cls.name, hm_cls.name))
            elif isinstance(nobj, Sink):
                sink_names.add(nname)
            elif isinstance(nobj, Source):
                source_names.add(nname)
        for key, from_to in matrix.items():
            parts = key.split(",")
            if len(parts) != 2:
                continue
            cs_name, cd_name = parts[0].strip(), parts[1].strip()
            cs = class_map.get(cs_name)
            cd = class_map.get(cd_name)
            if cs is None or cd is None:
                continue
            for from_name, to_dict in from_to.items():
                ns = node_map.get(from_name)
                if ns is None:
                    continue
                for to_name, prob in to_dict.items():
                    nd = node_map.get(to_name)
                    if nd is None:
                        continue
                    # Skip Cache-internal class-switching self-loop entries
                    if cs_name != cd_name and from_name == to_name and \
                       (from_name, cs_name, cd_name) in cache_internal_cs:
                        continue
                    # Skip cross-class Sink→Source entries (DTMC artifacts
                    # that Python reconstructs internally)
                    if cs_name != cd_name and from_name in sink_names \
                       and to_name in source_names:
                        continue
                    P.set(cs, cd, ns, nd, prob)
        model.link(P)

    # Restore routing strategies (per-node, per-class)
    routing_strats = data.get("routingStrategies", {})
    if routing_strats:
        from ..lang.base import RoutingStrategy
        for node_name, class_strats in routing_strats.items():
            node = node_map.get(node_name)
            if node is None:
                continue
            for cls_name, strat_name in class_strats.items():
                cls = class_map.get(cls_name)
                if cls is None:
                    continue
                strat = getattr(RoutingStrategy, strat_name, None)
                # Skip RAND and PROB: already handled by routing matrix
                if strat is not None and strat != RoutingStrategy.RAND and strat != RoutingStrategy.PROB:
                    node.set_routing(cls, strat)

    # Restore routing weights (for WRROBIN)
    routing_weights = data.get("routingWeights", {})
    if routing_weights:
        from ..lang.base import RoutingStrategy as RS
        for node_name, class_weights in routing_weights.items():
            node = node_map.get(node_name)
            if node is None:
                continue
            for cls_name, dest_weights in class_weights.items():
                cls = class_map.get(cls_name)
                if cls is None:
                    continue
                for dest_name, weight in dest_weights.items():
                    dest = node_map.get(dest_name)
                    if dest is not None:
                        node.set_routing(cls, RS.WRROBIN, dest, weight)

    # Restore switchover times
    for nd_data in data.get("nodes", []):
        so_list = nd_data.get("switchoverTimes")
        if not so_list:
            continue
        node = node_map.get(nd_data["name"])
        if node is None:
            continue
        for so in so_list:
            from_cls = class_map.get(so.get("from"))
            to_cls = class_map.get(so.get("to"))
            dist_data = so.get("distribution")
            if from_cls is not None and to_cls is not None and dist_data:
                dist = _json_to_dist(dist_data)
                if dist is not None:
                    if hasattr(node, 'setSwitchover'):
                        node.setSwitchover(from_cls, to_cls, dist)
                    elif hasattr(node, 'set_switchover'):
                        node.set_switchover(from_cls, to_cls, dist)

    # Restore finite capacity regions
    fcr_list = data.get("finiteCapacityRegions", [])
    for rj in fcr_list:
        # Support both new "stations" format and old "nodes" format
        region_nodes = []
        if "stations" in rj:
            for sj in rj["stations"]:
                n = node_map.get(sj.get("node"))
                if n is not None:
                    region_nodes.append(n)
        elif "nodes" in rj:
            for nname in rj["nodes"]:
                n = node_map.get(nname)
                if n is not None:
                    region_nodes.append(n)
        max_jobs = rj.get("globalMaxJobs", -1)
        if region_nodes:
            try:
                region = model.addRegion(rj.get("name", "FCR"), region_nodes, max_jobs)
                # globalMaxMemory
                gmm = rj.get("globalMaxMemory")
                if gmm is not None:
                    region.set_global_max_memory(gmm)
                # classMaxJobs
                for cls_name, cmj in rj.get("classMaxJobs", {}).items():
                    cls = class_map.get(cls_name)
                    if cls is not None:
                        region.set_class_max_jobs(cls, cmj)
                # dropRule
                for cls_name, dr_str in rj.get("dropRule", {}).items():
                    cls = class_map.get(cls_name)
                    if cls is not None:
                        region.set_drop_rule(cls, _str_to_drop_strategy(dr_str))
                # Per-station classWeight and classSize
                if "stations" in rj:
                    for sj in rj["stations"]:
                        for cls_name, w in sj.get("classWeight", {}).items():
                            cls = class_map.get(cls_name)
                            if cls is not None:
                                region.set_class_weight(cls, w)
                        for cls_name, sz in sj.get("classSize", {}).items():
                            cls = class_map.get(cls_name)
                            if cls is not None:
                                region.set_class_size(cls, sz)
            except Exception:
                pass

    # Restore initial state for Place nodes (token counts)
    for nd_data in data.get("nodes", []):
        init_st = nd_data.get("initialState")
        if init_st is not None and nd_data.get("type") == "Place":
            node = node_map.get(nd_data.get("name"))
            if node is not None and hasattr(node, 'set_state'):
                node.set_state(init_st)

    return model


def _create_node(model, nd: Dict[str, Any]):
    """Create a node from JSON data (before classes are set up)."""
    from ..lang.nodes import Queue, Delay, Source, Sink, Fork, Join, Router, ClassSwitch, Cache, Place, Transition
    from ..lang.base import SchedStrategy, ReplacementStrategy

    name = nd["name"]
    ntype = nd["type"]

    if ntype == "Source":
        return Source(model, name)
    elif ntype == "Sink":
        return Sink(model, name)
    elif ntype == "Delay":
        return Delay(model, name)
    elif ntype == "Queue":
        sched_str = nd.get("scheduling", "FCFS")
        # Map JAR enum names to Python SchedStrategy names
        _sched_aliases = {"FCFSPRIO": "HOL"}
        sched_str = _sched_aliases.get(sched_str, sched_str)
        sched = getattr(SchedStrategy, sched_str, SchedStrategy.FCFS)
        node = Queue(model, name, sched)
        servers = nd.get("servers", 1)
        if servers > 1:
            node.set_number_of_servers(servers)
        buf = nd.get("buffer")
        if buf is not None:
            node._capacity = buf
        return node
    elif ntype == "Fork":
        node = Fork(model, name)
        tpl = nd.get("tasksPerLink")
        if tpl is not None and tpl > 1:
            node.set_tasks_per_link(tpl)
        return node
    elif ntype == "Join":
        node = Join(model, name)
        # joinStrategy and joinQuorum are applied in post-class phase
        # (set_strategy/set_required require class objects)
        return node
    elif ntype == "Router":
        return Router(model, name)
    elif ntype == "ClassSwitch":
        return ClassSwitch(model, name)
    elif ntype == "Cache":
        # Support both Python format (nested "cache" key) and JAR format
        # (flat top-level with JAR key names)
        cc = nd.get("cache", {})
        if cc:
            nitems = cc.get("items", 10)
            cap = cc.get("capacity", 1)
            repl_str = cc.get("replacement", "LRU")
        else:
            nitems = nd.get("numItems", nd.get("items", 10))
            cap = nd.get("itemLevelCap", nd.get("capacity", 1))
            repl_str = nd.get("replacementStrategy", nd.get("replacement", "LRU"))
        if isinstance(cap, list):
            cap = np.array(cap)
        repl = getattr(ReplacementStrategy, repl_str, ReplacementStrategy.LRU)
        return Cache(model, name, nitems, cap, repl)
    elif ntype == "Place":
        return Place(model, name)
    elif ntype == "Transition":
        return Transition(model, name)
    else:
        # Default to Queue FCFS
        return Queue(model, name, SchedStrategy.FCFS)


def _drop_strategy_to_str(ds) -> Optional[str]:
    """Convert a DropStrategy enum to JSON string."""
    from ..lang.base import DropStrategy
    _map = {
        DropStrategy.DROP: "drop",
        DropStrategy.WaitingQueue: "waitingQueue",
        DropStrategy.BAS: "blockingAfterService",
    }
    return _map.get(ds, None)


def _str_to_drop_strategy(s: str):
    """Convert a JSON drop rule string to DropStrategy enum."""
    from ..lang.base import DropStrategy
    _map = {
        "drop": DropStrategy.DROP,
        "waitingQueue": DropStrategy.WaitingQueue,
        "blockingAfterService": DropStrategy.BAS,
    }
    return _map.get(s, DropStrategy.DROP)


def _node_type_str(node) -> str:
    """Get the schema node type string for a node object."""
    from ..lang.nodes import Queue, Delay, Source, Sink, Fork, Join, Router, ClassSwitch, Cache, Place, Transition
    if isinstance(node, Source):
        return "Source"
    if isinstance(node, Sink):
        return "Sink"
    if isinstance(node, Delay):
        return "Delay"
    if isinstance(node, Cache):
        return "Cache"
    if isinstance(node, Place):
        return "Place"
    if isinstance(node, Transition):
        return "Transition"
    if isinstance(node, Queue):
        return "Queue"
    if isinstance(node, Fork):
        return "Fork"
    if isinstance(node, Join):
        return "Join"
    if isinstance(node, Router):
        return "Router"
    if isinstance(node, ClassSwitch):
        return "ClassSwitch"
    return "Queue"


# ---------------------------------------------------------------------------
# LayeredNetwork serialization
# ---------------------------------------------------------------------------

def _layered_to_json(model) -> Dict[str, Any]:
    """Convert a LayeredNetwork to JSON-compatible dict."""
    from ..layered import (
        LayeredNetwork, Processor, Task, Entry, Activity,
        ActivityPrecedence, PrecedenceType, CallType, Distribution
    )
    from ..constants import SchedStrategy

    result = {
        "type": "LayeredNetwork",
        "name": model.name,
    }

    # Processors
    procs = []
    for p in model.processors:
        pj = {"name": p.name}
        if p.multiplicity is not None and np.isfinite(p.multiplicity) and int(p.multiplicity) != 1:
            pj["multiplicity"] = int(p.multiplicity)
        sched = p.sched_strategy
        if sched is not None:
            sname = sched.name if hasattr(sched, 'name') else str(sched)
            if sname != "INF":
                pj["scheduling"] = sname
        q = p.getQuantum()
        if q > 0:
            pj["quantum"] = q
        sf = p.getSpeedFactor()
        if sf != 1.0:
            pj["speedFactor"] = sf
        repl = p.getReplication()
        if repl > 1:
            pj["replication"] = repl
        procs.append(pj)
    result["processors"] = procs

    # Tasks
    tasks = []
    for t in model.tasks:
        tj = {"name": t.name}
        # Processor
        if t.processor is not None:
            tj["processor"] = t.processor.name
        # Multiplicity
        mult = t.multiplicity
        if mult is not None and np.isfinite(mult) and int(mult) != 1:
            tj["multiplicity"] = int(mult)
        # Scheduling
        sched = t.sched_strategy
        if sched is not None:
            sname = sched.name if hasattr(sched, 'name') else str(sched)
            tj["scheduling"] = sname
        # Think time
        if t.think_time is not None:
            tt_mean = _get_dist_mean_safe(t.think_time)
            if tt_mean > 0:
                tj["thinkTime"] = _layered_dist_to_json(t.think_time)
        # Fan in/out
        if t._fan_in:
            tj["fanIn"] = dict(t._fan_in)
        if t._fan_out:
            tj["fanOut"] = dict(t._fan_out)
        # Replication
        repl = t.getReplication()
        if repl > 1:
            tj["replication"] = repl
        # FunctionTask detection
        from ..layered import FunctionTask as _FunctionTask
        if isinstance(t, _FunctionTask) or getattr(t, '_is_function_task', False):
            tj["taskType"] = "FunctionTask"
        # Setup time / delay-off time (on any Task)
        if t.setup_time is not None:
            st_mean = _get_dist_mean_safe(t.setup_time)
            if st_mean > 0:
                tj["setupTime"] = _layered_dist_to_json(t.setup_time)
        if t.delay_off_time is not None:
            dot_mean = _get_dist_mean_safe(t.delay_off_time)
            if dot_mean > 0:
                tj["delayOffTime"] = _layered_dist_to_json(t.delay_off_time)
        # CacheTask detection
        from ..layered import CacheTask as _CacheTask
        if isinstance(t, _CacheTask):
            tj["taskType"] = "CacheTask"
            tj["totalItems"] = t.total_items
            tj["cacheCapacity"] = t.cache_capacity
            rs = t.replacement_strategy
            tj["replacementStrategy"] = rs.name if hasattr(rs, 'name') else str(rs)
        tasks.append(tj)
    result["tasks"] = tasks

    # Entries
    entries = []
    for e in model.entries:
        ej = {"name": e.name}
        if e.task is not None:
            ej["task"] = e.task.name
        # Entry arrival distribution
        arrival = getattr(e, '_arrival', None)
        if arrival is not None:
            adj = _dist_to_json(arrival)
            if adj is not None:
                ej["arrival"] = adj
        # ItemEntry detection
        from ..layered import ItemEntry as _ItemEntry
        if isinstance(e, _ItemEntry):
            ej["entryType"] = "ItemEntry"
            ej["totalItems"] = e.total_items
            if e.access_prob is not None:
                # Save access probability as a list if it's array-like
                ap = e.access_prob
                if hasattr(ap, 'tolist'):
                    ej["accessProb"] = ap.tolist()
                elif isinstance(ap, (list, tuple)):
                    ej["accessProb"] = list(ap)
                else:
                    # Try to serialize as distribution
                    dj = _dist_to_json(ap)
                    if dj is not None:
                        ej["accessProb"] = dj
        entries.append(ej)
    result["entries"] = entries

    # Activities
    activities = []
    for a in model.activities:
        aj = {"name": a.name}
        if a.task is not None:
            aj["task"] = a.task.name
        # Host demand
        if a.host_demand is not None:
            aj["hostDemand"] = _layered_dist_to_json(a.host_demand)
        # Bound to entry
        if a.bound_entry is not None:
            aj["boundTo"] = a.bound_entry.name
        # Replies to entry
        if a.reply_entry is not None:
            aj["repliesTo"] = a.reply_entry.name
        # Calls
        synch_calls = []
        asynch_calls = []
        for entry, mean_calls, call_type in a.calls:
            call_obj = {"entry": entry.name}
            if mean_calls != 1.0:
                call_obj["mean"] = mean_calls
            if call_type == CallType.SYNC:
                synch_calls.append(call_obj)
            elif call_type == CallType.ASYNC:
                asynch_calls.append(call_obj)
        if synch_calls:
            aj["synchCalls"] = synch_calls
        if asynch_calls:
            aj["asynchCalls"] = asynch_calls
        activities.append(aj)
    result["activities"] = activities

    # Precedences
    precedences = []
    for t in model.tasks:
        for p in t.precedences:
            pj = {"task": t.name}
            if p.prec_type == PrecedenceType.SERIAL:
                pj["type"] = "Serial"
                pj["activities"] = [a.name for a in p.activities]
            elif p.prec_type == PrecedenceType.PARALLEL:
                if len(p.pre_activities) == 1 and len(p.post_activities) > 1:
                    pj["type"] = "AndFork"
                    pj["activities"] = [p.pre_activities[0].name] + [a.name for a in p.post_activities]
                elif len(p.pre_activities) > 1 and len(p.post_activities) == 1:
                    pj["type"] = "AndJoin"
                    pj["activities"] = [a.name for a in p.pre_activities] + [p.post_activities[0].name]
                else:
                    pj["type"] = "Serial"
                    pj["activities"] = [a.name for a in (p.pre_activities + p.post_activities)]
            elif p.prec_type == PrecedenceType.CHOICE:
                if len(p.pre_activities) == 1 and len(p.post_activities) > 1:
                    pj["type"] = "OrFork"
                    pj["activities"] = [p.pre_activities[0].name] + [a.name for a in p.post_activities]
                    if p.probabilities:
                        pj["probabilities"] = list(p.probabilities)
                elif len(p.pre_activities) > 1 and len(p.post_activities) == 1:
                    pj["type"] = "OrJoin"
                    pj["activities"] = [a.name for a in p.pre_activities] + [p.post_activities[0].name]
                else:
                    pj["type"] = "Serial"
                    pj["activities"] = [a.name for a in (p.pre_activities + p.post_activities)]
            elif p.prec_type == PrecedenceType.LOOP:
                pj["type"] = "Loop"
                pj["activities"] = [a.name for a in p.activities]
                if p.pre_activities:
                    pj["preActivity"] = p.pre_activities[0].name
                if p.count != 1.0:
                    pj["loopCount"] = p.count
            elif p.prec_type == PrecedenceType.CACHE_ACCESS:
                pj["type"] = "CacheAccess"
                pj["activities"] = [p.pre_activities[0].name] + [a.name for a in p.post_activities]
            else:
                continue
            precedences.append(pj)
    if precedences:
        result["precedences"] = precedences

    return result


def _json_to_layered(data: Dict[str, Any]):
    """Reconstruct a LayeredNetwork from JSON data."""
    from ..layered import (
        LayeredNetwork, Processor, Task, Entry, Activity,
        ActivityPrecedence, PrecedenceType, CallType, Distribution as LDist,
        FunctionTask, CacheTask, ItemEntry
    )
    from ..constants import SchedStrategy
    from ..lang.base import ReplacementStrategy
    from ..distributions.continuous import Immediate

    model = LayeredNetwork(data.get("name", "model"))

    # Create processors (Python schema: "processors", JAR schema: "hosts")
    proc_map = {}
    for pd in data.get("processors", data.get("hosts", [])):
        name = pd["name"]
        mult = pd.get("multiplicity", 1)
        sched_str = pd.get("scheduling", "INF")
        sched = getattr(SchedStrategy, sched_str, SchedStrategy.INF)
        proc = Processor(model, name, mult, sched)
        q = pd.get("quantum", 0.0)
        if q > 0:
            proc.setQuantum(q)
        sf = pd.get("speedFactor", 1.0)
        if sf != 1.0:
            proc.setSpeedFactor(sf)
        repl = pd.get("replication")
        if repl is not None and repl > 1:
            proc.setReplication(repl)
        proc_map[name] = proc

    # Create tasks
    task_map = {}
    for td in data.get("tasks", []):
        name = td["name"]
        mult = td.get("multiplicity", 1)
        sched_str = td.get("scheduling", "INF")
        sched = getattr(SchedStrategy, sched_str, SchedStrategy.INF)
        proc_name = td.get("processor", td.get("host"))
        proc = proc_map.get(proc_name) if proc_name else None
        task_type = td.get("taskType", "Task")
        if task_type == "FunctionTask":
            task = FunctionTask(model, name, mult, sched)
        elif task_type == "CacheTask":
            total_items = td.get("totalItems", 1)
            cache_cap = td.get("cacheCapacity", 1)
            rs_name = td.get("replacementStrategy", "FIFO")
            rs = getattr(ReplacementStrategy, rs_name, ReplacementStrategy.FIFO)
            task = CacheTask(model, name, total_items, cache_cap, rs, mult)
        else:
            task = Task(model, name, mult, sched)
        if proc is not None:
            task.on(proc)
        # Think time (Python schema: "thinkTime" as dist, JAR schema: "thinkTimeMean"/"thinkTimeSCV")
        tt = td.get("thinkTime")
        if tt is not None:
            task.set_think_time(_json_to_native_or_layered_dist(tt))
        else:
            tt_mean = td.get("thinkTimeMean", 0.0)
            if tt_mean > 0:
                from ..distributions.continuous import Exp
                task.set_think_time(Exp(1.0 / tt_mean))
        # Setup time
        st = td.get("setupTime")
        if st is not None:
            task.set_setup_time(_json_to_native_or_layered_dist(st))
        else:
            st_mean = td.get("setupTimeMean", 0.0)
            if st_mean > 1e-8:
                from ..distributions.continuous import Exp
                task.set_setup_time(Exp(1.0 / st_mean))
        # Delay-off time
        dot = td.get("delayOffTime")
        if dot is not None:
            task.set_delay_off_time(_json_to_native_or_layered_dist(dot))
        else:
            dot_mean = td.get("delayOffTimeMean", 0.0)
            if dot_mean > 1e-8:
                from ..distributions.continuous import Exp
                task.set_delay_off_time(Exp(1.0 / dot_mean))
        # Fan in/out
        fi = td.get("fanIn", {})
        for src, val in fi.items():
            task.setFanIn(src, val)
        fo = td.get("fanOut", {})
        for dst, val in fo.items():
            task.setFanOut(dst, val)
        # Replication
        repl = td.get("replication")
        if repl is not None and repl > 1:
            task.setReplication(repl)
        task_map[name] = task

    # Create entries
    entry_map = {}
    for ed in data.get("entries", []):
        name = ed["name"]
        task_name = ed.get("task")
        task = task_map.get(task_name) if task_name else None
        entry_type = ed.get("entryType", "Entry")
        if entry_type == "ItemEntry":
            total_items = ed.get("totalItems", 1)
            access_prob = ed.get("accessProb")
            # access_prob may be a list or a distribution dict
            if isinstance(access_prob, dict):
                access_prob = _json_to_dist(access_prob)
            entry = ItemEntry(model, name, total_items, access_prob)
        else:
            entry = Entry(model, name)
        if task is not None:
            entry.on(task)
        # Entry arrival distribution
        arv = ed.get("arrival")
        if arv is not None:
            arv_dist = _json_to_dist(arv)
            if arv_dist is not None:
                entry.setArrival(arv_dist)
        entry_map[name] = entry

    # Create activities
    act_map = {}
    for ad in data.get("activities", []):
        name = ad["name"]
        task_name = ad.get("task")
        task = task_map.get(task_name) if task_name else None
        hd = ad.get("hostDemand")
        host_demand = _json_to_native_or_layered_dist(hd) if hd else Immediate.getInstance()
        act = Activity(model, name, host_demand)
        if task is not None:
            act.on(task)
        # Bound to (Python schema: "boundTo", JAR schema: "boundToEntry")
        bt = ad.get("boundTo", ad.get("boundToEntry"))
        if bt and bt in entry_map:
            act.bound_to(entry_map[bt])
        # Replies to
        rt = ad.get("repliesTo")
        if rt and rt in entry_map:
            act.replies_to(entry_map[rt])
        # Calls (Python schema: "entry", JAR schema: "dest")
        for sc in ad.get("synchCalls", []):
            ename = sc.get("entry", sc.get("dest"))
            mean = sc.get("mean", 1.0)
            if ename and ename in entry_map:
                act.synch_call(entry_map[ename], mean)
        for ac in ad.get("asynchCalls", []):
            ename = ac.get("entry", ac.get("dest"))
            mean = ac.get("mean", 1.0)
            if ename and ename in entry_map:
                act.asynch_call(entry_map[ename], mean)
        act_map[name] = act

    # Create precedences (Python schema: "type"/"activities", JAR schema: "preActs"/"postActs"/"preType"/"postType")
    for pd in data.get("precedences", []):
        task_name = pd.get("task")
        task = task_map.get(task_name)
        if task is None:
            continue

        # Detect schema: JAR uses preActs/postActs, Python uses type/activities
        if "preActs" in pd or "postActs" in pd:
            # JAR schema
            pre_names = pd.get("preActs", [])
            post_names = pd.get("postActs", [])
            pre_type = pd.get("preType", "pre")
            post_type = pd.get("postType", "post")
            pre_acts = [act_map[n] for n in pre_names if n in act_map]
            post_acts = [act_map[n] for n in post_names if n in act_map]
            probs = pd.get("probabilities", [])

            if pre_type == "pre" and post_type == "post":
                # Simple serial: pre→post
                if len(pre_acts) == 1 and len(post_acts) == 1:
                    task.add_precedence(ActivityPrecedence.Serial(pre_acts + post_acts))
            elif pre_type == "pre" and post_type in ("and-fork", "post-AND"):
                if len(pre_acts) >= 1 and len(post_acts) >= 1:
                    task.add_precedence(ActivityPrecedence.AndFork(pre_acts[0], post_acts))
            elif pre_type in ("and-join", "pre-AND") and post_type == "post":
                if len(pre_acts) >= 1 and len(post_acts) >= 1:
                    task.add_precedence(ActivityPrecedence.AndJoin(pre_acts, post_acts[0]))
            elif pre_type == "pre" and post_type in ("or-fork", "post-OR"):
                if len(pre_acts) >= 1 and len(post_acts) >= 1:
                    post_params = pd.get("postParams", probs)
                    if not post_params:
                        n = len(post_acts)
                        post_params = [1.0 / n] * n
                    task.add_precedence(ActivityPrecedence.OrFork(pre_acts[0], post_acts, post_params))
            elif pre_type in ("or-join", "pre-OR") and post_type == "post":
                if len(pre_acts) >= 1 and len(post_acts) >= 1:
                    task.add_precedence(ActivityPrecedence.OrJoin(pre_acts, post_acts[0]))
            elif pre_type == "pre" and post_type in ("loop", "post-LOOP"):
                count = pd.get("loopCount", None)
                if count is None:
                    # JAR format uses postParams for loop count
                    post_params = pd.get("postParams", [])
                    count = post_params[0] if post_params else 1.0
                if len(pre_acts) >= 1 and len(post_acts) >= 1:
                    task.add_precedence(ActivityPrecedence.Loop(pre_acts[0], post_acts, count))
            elif pre_type == "pre" and post_type == "post-CACHE":
                if len(pre_acts) >= 1 and len(post_acts) >= 1:
                    task.add_precedence(ActivityPrecedence.CacheAccess(pre_acts[0], post_acts))
        else:
            # Python schema
            ptype = pd.get("type", "Serial")
            act_names = pd.get("activities", [])
            acts = [act_map[n] for n in act_names if n in act_map]

            if ptype == "Serial" and len(acts) >= 2:
                task.add_precedence(ActivityPrecedence.Serial(acts))
            elif ptype == "AndFork" and len(acts) >= 2:
                task.add_precedence(ActivityPrecedence.AndFork(acts[0], acts[1:]))
            elif ptype == "AndJoin" and len(acts) >= 2:
                task.add_precedence(ActivityPrecedence.AndJoin(acts[:-1], acts[-1]))
            elif ptype == "OrFork" and len(acts) >= 2:
                probs = pd.get("probabilities", [])
                if not probs:
                    n = len(acts) - 1
                    probs = [1.0 / n] * n
                task.add_precedence(ActivityPrecedence.OrFork(acts[0], acts[1:], probs))
            elif ptype == "OrJoin" and len(acts) >= 2:
                task.add_precedence(ActivityPrecedence.OrJoin(acts[:-1], acts[-1]))
            elif ptype == "Loop":
                count = pd.get("loopCount", 1.0)
                pre_name = pd.get("preActivity")
                if pre_name and pre_name in act_map:
                    pre_act = act_map[pre_name]
                    task.add_precedence(ActivityPrecedence.Loop(pre_act, acts, count))
                elif len(acts) >= 2:
                    # Legacy format: first activity is pre, rest is loop body
                    task.add_precedence(ActivityPrecedence.Loop(acts[0], acts[1:], count))
            elif ptype == "CacheAccess" and len(acts) >= 2:
                task.add_precedence(ActivityPrecedence.CacheAccess(acts[0], acts[1:]))

    return model


def _layered_dist_to_json(dist) -> Dict[str, Any]:
    """Convert a layered network distribution (dataclass or native) to JSON."""
    from ..layered import Distribution as LDist
    if isinstance(dist, LDist):
        # Dataclass distribution — use Exp with fitMean
        if dist.scv == 0:
            return {"type": "Det", "params": {"value": dist.mean}}
        return {"type": "Exp", "params": {"lambda": 1.0 / dist.mean if dist.mean > 0 else 1e8}}
    # Try native distribution
    return _dist_to_json(dist) or {"type": "Exp", "params": {"lambda": 1.0}}


def _json_to_layered_dist(d: Dict[str, Any]):
    """Convert JSON dist to a layered Distribution (dataclass)."""
    from ..layered import Distribution as LDist
    # Use native distribution conversion, then get mean
    native = _json_to_dist(d)
    if native is not None:
        try:
            mean = native.getMean()
            scv = native.getSCV()
            return LDist(mean=mean, scv=scv)
        except Exception:
            return LDist(mean=1.0, scv=1.0)
    return LDist(mean=1.0, scv=1.0)


def _json_to_native_or_layered_dist(d: Dict[str, Any]):
    """Convert JSON dist to a native distribution if possible, else a layered Distribution dataclass.
    Native distributions have getMean()/getSCV() methods needed by solvers."""
    native = _json_to_dist(d)
    if native is not None:
        return native
    # Fallback to layered Distribution dataclass
    from ..layered import Distribution as LDist
    return LDist(mean=1.0, scv=1.0)


def _get_dist_mean_safe(dist) -> float:
    """Get mean from any distribution safely."""
    if dist is None:
        return 0.0
    if isinstance(dist, (int, float)):
        return float(dist)
    if hasattr(dist, 'mean') and not callable(dist.mean):
        return dist.mean
    if hasattr(dist, 'getMean'):
        return dist.getMean()
    return 0.0


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

def _to_list(arr) -> list:
    """Convert numpy array or list to plain Python list."""
    if isinstance(arr, np.ndarray):
        return arr.tolist()
    if isinstance(arr, (list, tuple)):
        return list(arr)
    return [arr]


def _matrix_to_list(mat) -> list:
    """Convert 2D numpy array to nested list."""
    if isinstance(mat, np.ndarray):
        return mat.tolist()
    return mat


class _JSONEncoder(json.JSONEncoder):
    """Custom JSON encoder that handles numpy types."""
    def default(self, obj):
        if isinstance(obj, np.integer):
            return int(obj)
        if isinstance(obj, np.floating):
            return float(obj)
        if isinstance(obj, np.ndarray):
            return obj.tolist()
        if isinstance(obj, np.bool_):
            return bool(obj)
        return super().default(obj)


# ---------------------------------------------------------------------------
# Workflow serialization
# ---------------------------------------------------------------------------

def _workflow_to_json(model) -> Dict[str, Any]:
    """Convert a Workflow model to a JSON-compatible dict."""
    from ..distributions.continuous import Exp

    result = {
        "type": "Workflow",
        "name": model.name,
    }

    # Activities
    acts_json = []
    for act in model.getActivities():
        act_obj = {"name": act.name}
        if act._distribution is not None:
            act_obj["hostDemand"] = _dist_to_json(act._distribution)
        else:
            # Activity was created from a float mean — serialize as Exp
            act_obj["hostDemand"] = _dist_to_json(Exp(1.0 / act._host_demand_mean))
        acts_json.append(act_obj)
    result["activities"] = acts_json

    # Precedences
    precs_json = []
    for prec in model.getPrecedences():
        prec_obj = {
            "preActs": list(prec.pre_acts),
            "postActs": list(prec.post_acts),
            "preType": prec.pre_type,
            "postType": prec.post_type,
        }
        if prec.pre_params is not None and len(prec.pre_params) > 0:
            prec_obj["preParams"] = _to_list(prec.pre_params)
        if prec.post_params is not None and len(prec.post_params) > 0:
            prec_obj["postParams"] = _to_list(prec.post_params)
        precs_json.append(prec_obj)
    result["precedences"] = precs_json

    return result


def _json_to_workflow(data: Dict[str, Any]):
    """Convert a JSON dict back to a Workflow model."""
    from ..lang.workflow import Workflow, ActivityPrecedence

    name = data.get("name", "workflow")
    wf = Workflow(name)

    # Activities
    if "activities" in data:
        for act_data in data["activities"]:
            act_name = act_data["name"]
            if "hostDemand" in act_data:
                host_demand = _json_to_dist(act_data["hostDemand"])
                wf.addActivity(act_name, host_demand)
            else:
                wf.addActivity(act_name, 1.0)

    # Precedences
    if "precedences" in data:
        for prec_data in data["precedences"]:
            pre_acts = list(prec_data["preActs"])
            post_acts = list(prec_data["postActs"])
            pre_type = prec_data.get("preType", "pre")
            post_type = prec_data.get("postType", "post")
            pre_params = None
            if "preParams" in prec_data:
                pre_params = np.atleast_1d(np.array(prec_data["preParams"], dtype=float))
            post_params = None
            if "postParams" in prec_data:
                post_params = np.atleast_1d(np.array(prec_data["postParams"], dtype=float))
            wf.addPrecedence(ActivityPrecedence(
                pre_acts=pre_acts,
                post_acts=post_acts,
                pre_type=pre_type,
                post_type=post_type,
                pre_params=pre_params,
                post_params=post_params
            ))

    return wf


# ---------------------------------------------------------------------------
# Environment serialization
# ---------------------------------------------------------------------------

def _environment_to_json(model) -> Dict[str, Any]:
    """Convert an Environment model to a JSON-compatible dict."""
    result = {
        "type": "Environment",
        "name": model.name,
        "numStages": model.num_stages,
    }

    # Stages
    stages_json = []
    for i in range(model.num_stages):
        stage_name = model._stage_names[i] if i < len(model._stage_names) else None
        if stage_name is None:
            continue
        stage_obj = {"name": stage_name}
        stage_model = model.get_model(i)
        if stage_model is not None:
            stage_obj["model"] = _network_to_json(stage_model)
        stages_json.append(stage_obj)
    result["stages"] = stages_json

    # Transitions
    trans_json = []
    for i in range(model.num_stages):
        for j in range(model.num_stages):
            if model.env[i][j] is not None:
                trans_obj = {
                    "from": i,
                    "to": j,
                    "distribution": _dist_to_json(model.env[i][j]),
                }
                trans_json.append(trans_obj)
    result["transitions"] = trans_json

    return result


def _json_to_environment(data: Dict[str, Any]):
    """Convert a JSON dict back to an Environment model."""
    from ..environment import Environment

    name = data.get("name", "env")
    num_stages = data.get("numStages", 0)
    env = Environment(name, num_stages)

    # Stages
    if "stages" in data:
        for i, stage_data in enumerate(data["stages"]):
            stage_name = stage_data.get("name", "Stage" + str(i))
            stage_type = stage_data.get("type", "")
            stage_model = None
            if "model" in stage_data:
                stage_model = _json_to_network(stage_data["model"])
            if stage_model is not None:
                env.add_stage(i, stage_name, stage_type, stage_model)

    # Transitions
    if "transitions" in data:
        for trans_data in data["transitions"]:
            from_idx = trans_data["from"]
            to_idx = trans_data["to"]
            dist = _json_to_dist(trans_data["distribution"])
            env.add_transition(from_idx, to_idx, dist)

    env.init()
    return env


# ---------------------------------------------------------------------------
# Public API
# ---------------------------------------------------------------------------

def save_model(model, filename: str) -> None:
    """
    Save a LINE model to a JSON file.

    Supports Network, LayeredNetwork, Workflow, and Environment models.
    The output conforms to the line-model.schema.json specification.

    Args:
        model: A Network, LayeredNetwork, Workflow, or Environment instance.
        filename: Output file path (should end in .json).

    Example:
        >>> model = Network('M/M/1')
        >>> # ... define model ...
        >>> save_model(model, 'mymodel.json')
    """
    from ..lang.network import Network
    from ..layered import LayeredNetwork
    from ..lang.workflow import Workflow
    from ..environment import Environment

    if isinstance(model, LayeredNetwork):
        model_json = _layered_to_json(model)
    elif isinstance(model, Network):
        model_json = _network_to_json(model)
    elif isinstance(model, Workflow):
        model_json = _workflow_to_json(model)
    elif isinstance(model, Environment):
        model_json = _environment_to_json(model)
    else:
        raise TypeError(f"Unsupported model type: {type(model)}")

    doc = {
        "format": "line-model",
        "version": "1.0",
        "model": model_json
    }

    with open(filename, 'w') as f:
        json.dump(doc, f, indent=2, cls=_JSONEncoder)


def load_model(filename: str):
    """
    Load a LINE model from a JSON file.

    Returns a Network, LayeredNetwork, Workflow, or Environment
    depending on the model type in the JSON file.

    Args:
        filename: Path to a .json file conforming to line-model.schema.json.

    Returns:
        Network, LayeredNetwork, Workflow, or Environment instance.

    Example:
        >>> model = load_model('mymodel.json')
        >>> solver = SolverMVA(model)
        >>> print(solver.avg_table())
    """
    with open(filename, 'r') as f:
        doc = json.load(f)

    model_data = doc.get("model", {})
    mtype = model_data.get("type")

    if mtype == "Network":
        return _json_to_network(model_data)
    elif mtype == "LayeredNetwork":
        return _json_to_layered(model_data)
    elif mtype == "Workflow":
        return _json_to_workflow(model_data)
    elif mtype == "Environment":
        return _json_to_environment(model_data)
    else:
        raise ValueError(f"Unsupported model type: {mtype}")
