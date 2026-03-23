"""
Native Python LDES solver using subprocess to call ldes.jar CLI.

This module provides the SolverLDES class that runs discrete event
simulation by invoking `java -jar ldes.jar solve model.json` and
parsing the JSON result.
"""

import json
import os
import subprocess
import tempfile
import time
import urllib.request
import xml.etree.ElementTree as ET

import numpy as np
import pandas as pd
from typing import Optional, Any, List, Tuple

from .ldes_options import LDESOptions, LDESResult
from ..base import NetworkSolver
from ...api.sn.transforms import sn_get_residt_from_respt
from ...api.sn.network_struct import NodeType
from ...constants import GlobalConstants
from ...io.linemodel_io import save_model


# ---------------------------------------------------------------------------
# JSIMG/JSIM/JSIMW → Python native Network conversion
# ---------------------------------------------------------------------------

def _jsim_file_to_network(filename: str):
    """
    Parse a JMT jsimg/jsim/jsimw file and construct a Python native Network.

    Mirrors the logic of M2M.JSIM2LINE() in the JAR implementation, using
    Python-native Network/node/class/distribution APIs.

    Args:
        filename: Path to a JMT file (.jsimg, .jsim, .jsimw, or .jmva).

    Returns:
        A fully constructed Network object ready for simulation.
    """
    from ...lang.network import Network
    from ...lang.nodes import Source, Queue, Delay, Sink, Router, Fork, Join
    from ...lang.classes import OpenClass, ClosedClass
    from ...lang.base import SchedStrategy, RoutingStrategy
    from ...distributions.continuous import (
        Exp, Erlang, HyperExp, Det, Gamma, Pareto, Lognormal, Uniform, Weibull, Disabled
    )
    from ...distributions.markovian import Coxian, APH, MAP, MMPP2

    ext = os.path.splitext(filename)[1].lower()
    if ext == '.jmva':
        raise ValueError(
            f"JMVA file format is not yet supported for direct LDES loading: {filename}. "
            "Convert to JSIMG first using JMT."
        )

    tree = ET.parse(filename)
    root = tree.getroot()

    # Get <sim> element (may be direct root or child of <archive>)
    sim = root.find('sim')
    if sim is None:
        sim = root

    # Derive model name from archive name attribute
    arch_name = root.get('name', os.path.basename(filename))
    model_name = os.path.splitext(os.path.basename(arch_name))[0]
    model = Network(model_name)

    # -------------------------------------------------------------------------
    # Step 1: Collect node elements and their sections
    # -------------------------------------------------------------------------
    node_elems = sim.findall('node')
    orig_names: List[str] = [ne.get('name', f'Node{i}') for i, ne in enumerate(node_elems)]
    san_names: List[str] = [n.replace('/', '_').replace('\\', '_') for n in orig_names]
    sections_list: List[List[ET.Element]] = [ne.findall('section') for ne in node_elems]

    # -------------------------------------------------------------------------
    # Step 2: Create node objects (first pass)
    # -------------------------------------------------------------------------
    node_objs: List[Optional[object]] = [None] * len(orig_names)
    fork_stack: List[Fork] = []

    for i, (name, sects) in enumerate(zip(san_names, sections_list)):
        if not sects:
            continue
        first_cls = sects[0].get('className', '')

        if first_cls == 'JobSink':
            node_objs[i] = Sink(model, name)

        elif first_cls == 'RandomSource':
            node_objs[i] = Source(model, name)

        elif first_cls == 'Join':
            fork_obj = fork_stack[-1] if fork_stack else None
            node_objs[i] = Join(model, name, fork_obj)

        elif first_cls == 'Storage':
            # Place nodes (Petri nets) — not parsed here
            pass

        elif first_cls == 'Enabling':
            # Transition nodes (Petri nets) — not parsed here
            pass

        elif first_cls == 'Queue':
            second_cls = sects[1].get('className', '') if len(sects) > 1 else ''
            third_cls = sects[2].get('className', '') if len(sects) > 2 else ''

            if third_cls == 'Fork':
                f = Fork(model, name)
                fork_stack.append(f)
                node_objs[i] = f

            elif second_cls == 'ServiceTunnel':
                node_objs[i] = Router(model, name)

            elif second_cls == 'Delay':
                d = Delay(model, name)
                cap = _jsim_parse_queue_size(sects[0])
                if cap is not None and cap > 0:
                    d.set_capacity(cap)
                node_objs[i] = d

            elif second_cls in ('Server', 'PSServer'):
                strat, nservers = _jsim_parse_server_info(sects, second_cls)
                q = Queue(model, name, strat)
                cap = _jsim_parse_queue_size(sects[0])
                if cap is not None and cap > 0:
                    q.set_capacity(cap)
                if nservers > 1:
                    q.set_number_of_servers(nservers)
                node_objs[i] = q

            else:
                # Fallback: generic FCFS queue
                node_objs[i] = Queue(model, name, SchedStrategy.FCFS)

    # -------------------------------------------------------------------------
    # Step 3: Create job classes
    # -------------------------------------------------------------------------
    class_elems = sim.findall('userClass')
    jmt_prios = [int(ce.get('priority', '0')) for ce in class_elems]
    max_prio = max(jmt_prios) if jmt_prios else 0

    job_classes: List[object] = []
    for ce, jmt_prio in zip(class_elems, jmt_prios):
        cname = ce.get('name', '')
        ctype = ce.get('type', 'open').lower()
        line_prio = max_prio - jmt_prio  # invert JMT convention

        if ctype == 'open':
            c = OpenClass(model, cname, line_prio)
        else:
            ref_src = ce.get('referenceSource', '')
            customers_str = ce.get('customers', ce.get('population', '1'))
            customers = int(customers_str) if customers_str else 1
            ref_idx = orig_names.index(ref_src) if ref_src in orig_names else 0
            ref_node = node_objs[ref_idx] if 0 <= ref_idx < len(node_objs) else None
            c = ClosedClass(model, cname, customers, ref_node, line_prio)
        job_classes.append(c)

    # -------------------------------------------------------------------------
    # Step 4: Set service / arrival distributions
    # -------------------------------------------------------------------------
    for i, (node, sects) in enumerate(zip(node_objs, sections_list)):
        if node is None or not sects:
            continue

        first_cls = sects[0].get('className', '')

        if isinstance(node, Source):
            # RandomSource section: ServiceStrategy parameter contains arrivals
            _jsim_set_source_arrivals(node, sects[0], job_classes, Disabled)

        elif isinstance(node, (Queue, Delay)):
            # Server/PSServer/Delay section at index 1
            if len(sects) > 1:
                _jsim_set_service_distributions(node, sects[1], job_classes, Disabled)

    # -------------------------------------------------------------------------
    # Step 5: Create connections (topology)
    # -------------------------------------------------------------------------
    for ce in sim.findall('connection'):
        src_name = ce.get('source', '')
        tgt_name = ce.get('target', '')
        si = orig_names.index(src_name) if src_name in orig_names else -1
        ti = orig_names.index(tgt_name) if tgt_name in orig_names else -1
        if si >= 0 and ti >= 0 and node_objs[si] is not None and node_objs[ti] is not None:
            model.add_link(node_objs[si], node_objs[ti])

    # -------------------------------------------------------------------------
    # Step 6: Set routing strategies
    # -------------------------------------------------------------------------
    for i, (node, sects) in enumerate(zip(node_objs, sections_list)):
        if node is None or isinstance(node, Sink) or not sects:
            continue

        first_cls = sects[0].get('className', '')

        # Routing section is the last section for Source/Queue nodes
        if first_cls == 'RandomSource' and len(sects) > 2:
            routing_sect = sects[2]
        elif first_cls == 'Queue' and len(sects) > 2:
            routing_sect = sects[2]
        else:
            continue

        _jsim_set_node_routing(node, routing_sect, job_classes, orig_names, node_objs)

    return model


def _jsim_parse_queue_size(queue_sect: ET.Element) -> Optional[int]:
    """Parse the queue buffer capacity from a Queue section element."""
    for param in queue_sect.findall('parameter'):
        if param.get('name', '') == 'size':
            val_elem = param.find('value')
            if val_elem is not None and val_elem.text:
                try:
                    v = int(val_elem.text)
                    return v if v > 0 else None
                except ValueError:
                    pass
    return None


def _jsim_parse_server_info(sects: List[ET.Element], second_cls: str) -> Tuple[Any, int]:
    """
    Determine scheduling strategy and number of servers for a Queue node.

    Returns:
        (SchedStrategy, nservers)
    """
    from ...lang.base import SchedStrategy

    strategy = SchedStrategy.FCFS
    nservers = 1

    if second_cls == 'Server':
        # Scheduling from QueuePutStrategy in section[0]
        for param in sects[0].findall('parameter'):
            if param.get('name', '') == 'QueuePutStrategy':
                subs = param.findall('subParameter')
                if subs:
                    strategy = _jsim_put_strategy_to_sched(subs[0].get('name', 'TailStrategy'))
                break
        # Number of servers from maxJobs in section[1] (Server)
        for param in sects[1].findall('parameter'):
            if param.get('name', '') == 'maxJobs':
                val = param.find('value')
                if val is not None and val.text:
                    try:
                        nservers = int(val.text)
                    except ValueError:
                        pass
                break

    elif second_cls == 'PSServer':
        strategy = _jsim_parse_ps_strategy(sects[1])
        for param in sects[1].findall('parameter'):
            if param.get('name', '') == 'maxJobs':
                val = param.find('value')
                if val is not None and val.text:
                    try:
                        nservers = int(val.text)
                    except ValueError:
                        pass
                break

    return strategy, nservers


def _jsim_put_strategy_to_sched(name: str) -> Any:
    """Map a JMT QueuePutStrategy name to a LINE SchedStrategy."""
    from ...lang.base import SchedStrategy
    mapping = {
        'TailStrategy': SchedStrategy.FCFS,
        'TailStrategyPriority': SchedStrategy.HOL,
        'HeadStrategy': SchedStrategy.LCFS,
        'RandStrategy': SchedStrategy.SIRO,
        'SJFStrategy': SchedStrategy.SJF,
        'SEPTStrategy': SchedStrategy.SEPT,
        'LJFStrategy': SchedStrategy.LJF,
        'LEPTStrategy': SchedStrategy.LEPT,
    }
    return mapping.get(name, SchedStrategy.FCFS)


def _jsim_parse_ps_strategy(server_sect: ET.Element) -> Any:
    """Parse PS/DPS/GPS strategy from a PSServer section."""
    from ...lang.base import SchedStrategy
    for sp in server_sect.iter('subParameter'):
        name = sp.get('name', '')
        if name == 'EPSStrategy':
            return SchedStrategy.PS
        elif name == 'DPSStrategy':
            return SchedStrategy.DPS
        elif name == 'GPSStrategy':
            return SchedStrategy.GPS
    return SchedStrategy.PS


def _jsim_parse_distribution(subs: List[ET.Element]) -> Any:
    """
    Parse a service/arrival distribution from the two subParameter children of a
    ServiceTimeStrategy element.

    Args:
        subs: List of subParameter elements — [distribution_type_elem, distrPar_elem].

    Returns:
        A distribution object or None if not parsable (caller should use Disabled).
    """
    from ...distributions.continuous import (
        Exp, Erlang, HyperExp, Det, Gamma, Pareto, Lognormal, Uniform, Weibull, Disabled
    )
    from ...distributions.markovian import Coxian

    if not subs:
        return None

    distr_name = subs[0].get('name', '')

    if distr_name in ('ZeroServiceTimeStrategy', 'Disabled', ''):
        return None  # caller sets Disabled

    if len(subs) < 2:
        return None

    distr_params = subs[1].findall('subParameter')

    def _val(idx: int) -> Optional[float]:
        if idx < len(distr_params):
            v = distr_params[idx].find('value')
            if v is not None and v.text:
                try:
                    return float(v.text)
                except ValueError:
                    pass
        return None

    if distr_name == 'Exponential':
        lam = _val(0)
        return Exp(lam) if lam is not None else None

    elif distr_name == 'Erlang':
        lam = _val(0)
        phases = _val(1)
        return Erlang(lam, int(phases)) if lam is not None and phases is not None else None

    elif distr_name == 'Hyperexponential':
        p = _val(0)
        r1 = _val(1)
        r2 = _val(2)
        return HyperExp(p, r1, r2) if None not in (p, r1, r2) else None

    elif distr_name == 'Coxian':
        lam = _val(0)
        lam1 = _val(1)
        phi = _val(2)
        return Coxian([lam, lam1], [phi, 1.0]) if None not in (lam, lam1, phi) else None

    elif distr_name == 'Deterministic':
        val = _val(0)
        return Det(val) if val is not None else None

    elif distr_name == 'Pareto':
        alpha = _val(0)
        k = _val(1)
        return Pareto(alpha, k) if None not in (alpha, k) else None

    elif distr_name == 'Gamma':
        shape = _val(0)
        scale = _val(1)
        return Gamma(shape, scale) if None not in (shape, scale) else None

    elif distr_name == 'Weibull':
        # Java M2M note: "scale and shape are inverted in the constructor"
        # JMT par[0]=shape(lambda), par[1]=scale(lambda1) → Weibull(scale, shape)
        shape = _val(0)
        scale = _val(1)
        return Weibull(scale, shape) if None not in (shape, scale) else None

    elif distr_name == 'Lognormal':
        mu = _val(0)
        sigma = _val(1)
        return Lognormal(mu, sigma) if None not in (mu, sigma) else None

    elif distr_name == 'Uniform':
        a = _val(0)
        b = _val(1)
        return Uniform(a, b) if None not in (a, b) else None

    return None


def _jsim_set_source_arrivals(source_node, source_sect: ET.Element,
                               job_classes: list, Disabled) -> None:
    """Set arrival distributions for a Source node from its RandomSource section."""
    for param in source_sect.findall('parameter'):
        if param.get('name', '') == 'ServiceStrategy':
            subparams = param.findall('subParameter')  # one ServiceTimeStrategy per class
            for r, cls in enumerate(job_classes):
                if r < len(subparams):
                    sp = subparams[r]
                    subs = sp.findall('subParameter')  # [dist_type, distrPar]
                    dist = _jsim_parse_distribution(subs)
                    source_node.set_arrival(cls, dist if dist is not None else Disabled())
            break


def _jsim_set_service_distributions(queue_node, srv_sect: ET.Element,
                                     job_classes: list, Disabled) -> None:
    """Set service distributions for a Queue or Delay node from its Server/Delay section."""
    for param in srv_sect.findall('parameter'):
        if param.get('name', '') == 'ServiceStrategy':
            subparams = param.findall('subParameter')  # one ServiceTimeStrategy per class
            for r, cls in enumerate(job_classes):
                if r < len(subparams):
                    sp = subparams[r]
                    subs = sp.findall('subParameter')  # [dist_type, distrPar]
                    dist = _jsim_parse_distribution(subs)
                    queue_node.set_service(cls, dist if dist is not None else Disabled())
            break


def _jsim_set_node_routing(node, routing_sect: ET.Element, job_classes: list,
                            orig_names: List[str], node_objs: list) -> None:
    """Set routing strategies for all classes on a node from its Router section."""
    from ...lang.base import RoutingStrategy

    for param in routing_sect.findall('parameter'):
        if param.get('name', '') == 'RoutingStrategy':
            subparams = param.findall('subParameter')  # one per class
            for r, cls in enumerate(job_classes):
                if r < len(subparams):
                    sp = subparams[r]
                    strat_name = sp.get('name', 'Random')
                    if strat_name == 'Random':
                        node.set_routing(cls, RoutingStrategy.RAND)
                    elif strat_name == 'Probabilities':
                        node.set_routing(cls, RoutingStrategy.PROB)
                        _jsim_set_prob_routing(node, cls, sp, orig_names, node_objs)
                    elif strat_name == 'Round Robin':
                        node.set_routing(cls, RoutingStrategy.RROBIN)
                    elif strat_name == 'Weighted Round Robin':
                        node.set_routing(cls, RoutingStrategy.WRROBIN)
                        _jsim_set_wrrobin_routing(node, cls, sp, orig_names, node_objs)
                    elif strat_name in ('Join the Shortest Queue (JSQ)',
                                        'Shortest Response Time', 'Fastest Service'):
                        node.set_routing(cls, RoutingStrategy.JSQ)
                    elif strat_name == 'Disabled':
                        node.set_routing(cls, RoutingStrategy.DISABLED)
                    else:
                        node.set_routing(cls, RoutingStrategy.RAND)
            break


def _jsim_set_prob_routing(node, cls, prob_param: ET.Element,
                            orig_names: List[str], node_objs: list) -> None:
    """Parse probabilistic routing destinations and set them on the node."""
    # XML structure under the Probabilities subParameter:
    # <subParameter name="Probabilities">
    #   <subParameter classPath="Probabilities_class">
    #     <subParameter name="dest1">
    #       <value>dest1_name</value>
    #       <value>probability</value>
    #     </subParameter>
    #     ...
    #   </subParameter>
    # </subParameter>
    outer = prob_param.find('subParameter')
    if outer is None:
        return
    for dest_sp in outer.findall('subParameter'):
        values = dest_sp.findall('value')
        if len(values) >= 2:
            dest_name = values[0].text or ''
            try:
                prob = float(values[1].text or '0')
            except ValueError:
                continue
            # Sanitize name
            dest_san = dest_name.replace('/', '_').replace('\\', '_')
            dest_idx = -1
            if dest_name in orig_names:
                dest_idx = orig_names.index(dest_name)
            elif dest_san in [n.replace('/', '_').replace('\\', '_') for n in orig_names]:
                for j, n in enumerate(orig_names):
                    if n.replace('/', '_').replace('\\', '_') == dest_san:
                        dest_idx = j
                        break
            if dest_idx >= 0 and node_objs[dest_idx] is not None:
                node.set_prob_routing(cls, node_objs[dest_idx], prob)


def _jsim_set_wrrobin_routing(node, cls, wrr_param: ET.Element,
                               orig_names: List[str], node_objs: list) -> None:
    """Parse weighted round-robin weights and set them on the node."""
    from ...lang.base import RoutingStrategy
    outer = wrr_param.find('subParameter')
    if outer is None:
        return
    for dest_sp in outer.findall('subParameter'):
        values = dest_sp.findall('value')
        if len(values) >= 2:
            dest_name = values[0].text or ''
            try:
                weight = float(values[1].text or '1')
            except ValueError:
                continue
            dest_idx = orig_names.index(dest_name) if dest_name in orig_names else -1
            if dest_idx >= 0 and node_objs[dest_idx] is not None:
                node.set_routing(cls, RoutingStrategy.WRROBIN,
                                 node_objs[dest_idx], weight)


# ---------------------------------------------------------------------------

class SolverLDES(NetworkSolver):
    """
    Native Python LINE Discrete Event Simulator (LDES) solver.

    Runs simulation by calling ldes.jar via subprocess and parsing
    the JSON result. This ensures Python native uses the same
    simulation engine as MATLAB and python-wrapper.

    Args:
        model: Network model (Python native)
        options: LDESOptions configuration (optional)
        **kwargs: Additional solver options (seed, samples, verbose, etc.)

    Example:
        >>> from line_solver.solvers.solver_ldes import SolverLDES, LDESOptions
        >>> options = LDESOptions(seed=23000, samples=200000)
        >>> solver = SolverLDES(model, options)
        >>> solver.runAnalyzer()
        >>> table = solver.getAvgTable()
    """

    def __init__(
        self,
        model: Any,
        options: Optional[LDESOptions] = None,
        **kwargs
    ):
        # Accept a JMT file path (.jsimg/.jsim/.jsimw) in place of a Network object
        if isinstance(model, str):
            model = _jsim_file_to_network(model)

        self.model = model
        self.options = options or LDESOptions()

        # Apply kwargs to options
        for key, value in kwargs.items():
            if hasattr(self.options, key):
                setattr(self.options, key, value)

        self._result: Optional[LDESResult] = None

        # Extract network structure
        self._sn = self._get_network_struct()

        # Station and class names for table output
        self._station_names: list = []
        self._class_names: list = []
        self._extract_names()

    def _get_network_struct(self) -> Any:
        """Get NetworkStruct from model."""
        model = self.model

        if hasattr(model, '_sn') and model._sn is not None:
            return model._sn

        if hasattr(model, 'refresh_struct'):
            model.refresh_struct()
            if hasattr(model, '_sn') and model._sn is not None:
                return model._sn

        if hasattr(model, 'getStruct'):
            try:
                return model.getStruct()
            except Exception:
                pass

        if hasattr(model, 'obj'):
            try:
                return model.getStruct()
            except Exception:
                pass

        if hasattr(model, 'nclasses') and hasattr(model, 'nstations'):
            return model

        raise ValueError("Cannot extract network structure from model")

    def _extract_names(self) -> None:
        """Extract station and class names from network structure."""
        sn = self._sn
        num_stations = int(sn.nstations) if hasattr(sn, 'nstations') else 1

        nodenames = list(sn.nodenames) if hasattr(sn, 'nodenames') and sn.nodenames is not None else []
        stationToNode = sn.stationToNode if hasattr(sn, 'stationToNode') and sn.stationToNode is not None else None

        if stationToNode is not None and nodenames:
            stationToNode = np.asarray(stationToNode).flatten()
            self._station_names = []
            for i in range(num_stations):
                if i < len(stationToNode):
                    node_idx = int(stationToNode[i])
                    if node_idx < len(nodenames):
                        self._station_names.append(nodenames[node_idx])
                    else:
                        self._station_names.append(f'Station{i}')
                else:
                    self._station_names.append(f'Station{i}')
        elif nodenames:
            self._station_names = nodenames[:num_stations]
            while len(self._station_names) < num_stations:
                self._station_names.append(f'Station{len(self._station_names)}')
        else:
            self._station_names = [f'Station{i}' for i in range(num_stations)]

        if hasattr(sn, 'classnames') and sn.classnames is not None:
            try:
                self._class_names = list(sn.classnames)
            except Exception:
                self._class_names = [f'Class{i}' for i in range(sn.nclasses)]
        else:
            num_classes = int(sn.nclasses) if hasattr(sn, 'nclasses') else 1
            self._class_names = [f'Class{i}' for i in range(num_classes)]

    @staticmethod
    def _get_package_bin_dir() -> str:
        """Get path to bin/ directory bundled inside the installed package."""
        package_dir = os.path.dirname(os.path.abspath(__file__))
        # Navigate: solver_ldes -> solvers -> line_solver
        line_solver_dir = os.path.dirname(os.path.dirname(package_dir))
        return os.path.join(line_solver_dir, 'bin')

    @staticmethod
    def _get_common_dir() -> str:
        """Get path to common/ directory in the repository root."""
        package_dir = os.path.dirname(os.path.abspath(__file__))
        # Navigate: solver_ldes -> solvers -> line_solver -> python -> repo root
        python_dir = os.path.dirname(os.path.dirname(os.path.dirname(package_dir)))
        root_dir = os.path.dirname(python_dir)
        return os.path.join(root_dir, 'common')

    @staticmethod
    def _get_ldes_native_path() -> Optional[str]:
        """Get path to native LDES binary if available locally.

        Lookup order:
        1. Bundled in package (line_solver/bin/ldes)
        2. Repository common/ directory

        Native binary is only used on Linux. On macOS and Windows,
        LDES runs via java -jar ldes.jar.
        """
        import platform
        if platform.system() != 'Linux':
            return None

        # 1. Check package bin/ directory (pip-installed wheel)
        bin_dir = SolverLDES._get_package_bin_dir()
        native_path = os.path.join(bin_dir, 'ldes')
        if os.path.isfile(native_path) and os.access(native_path, os.X_OK):
            return native_path

        # 2. Check common/ directory (development/repo checkout)
        common_dir = SolverLDES._get_common_dir()
        native_path = os.path.join(common_dir, 'ldes')
        if os.path.isfile(native_path) and os.access(native_path, os.X_OK):
            return native_path

        return None

    @staticmethod
    def _get_ldes_jar_path() -> str:
        """Get path to ldes.jar, downloading if necessary.

        Lookup order:
        1. Bundled in package (line_solver/bin/ldes.jar)
        2. Repository common/ directory
        3. Auto-download from SourceForge
        """
        # 1. Check package bin/ directory (pip-installed wheel)
        bin_dir = SolverLDES._get_package_bin_dir()
        jar_path = os.path.join(bin_dir, 'ldes.jar')
        if os.path.isfile(jar_path):
            return jar_path

        # 2. Check common/ directory (development/repo checkout)
        common_dir = SolverLDES._get_common_dir()
        ldes_path = os.path.join(common_dir, 'ldes.jar')
        if os.path.isfile(ldes_path):
            return ldes_path

        # 3. Try to download to common/
        os.makedirs(common_dir, exist_ok=True)
        ldes_url = 'https://line-solver.sourceforge.net/latest/ldes.jar'
        try:
            print(f"ldes.jar not found in {common_dir}")
            print("Attempting to download ldes.jar...")
            urllib.request.urlretrieve(ldes_url, ldes_path)
            print(f"Successfully downloaded ldes.jar to {common_dir}")
            return ldes_path
        except Exception as e:
            raise RuntimeError(
                f"ldes.jar not found and download failed: {e}\n"
                f"Please manually download from {ldes_url} and place in {common_dir}"
            )

    def _build_cli_args(self, model_path: str, result_path: str,
                         trajectory: bool = False) -> list:
        """Build CLI command arguments for LDES solver.

        Prefers native binary (common/ldes) for faster startup;
        falls back to java -jar ldes.jar if native binary not found.
        """
        opts = self.options

        native_path = self._get_ldes_native_path()
        if native_path is not None:
            cmd = [native_path, 'solve', model_path, '-o', result_path]
        else:
            ldes_jar = self._get_ldes_jar_path()
            cmd = ['java', '-jar', ldes_jar, 'solve', model_path, '-o', result_path]

        if opts.samples != 200_000:
            cmd.extend(['-s', str(opts.samples)])
        if opts.seed != 23000:
            cmd.extend(['--seed', str(opts.seed)])
        if opts.method != 'default':
            cmd.extend(['--method', opts.method])
        if opts.cnvgon:
            cmd.append('--cnvgon')
        if opts.cnvgtol != 0.05:
            cmd.extend(['--cnvgtol', str(opts.cnvgtol)])
        if opts.tranfilter != 'mser5':
            cmd.extend(['--tranfilter', opts.tranfilter])
        if opts.warmupfrac != 0.2:
            cmd.extend(['--warmupfrac', str(opts.warmupfrac)])
        if opts.cimethod != 'obm':
            cmd.extend(['--cimethod', opts.cimethod])
        if hasattr(opts, 'replications') and opts.replications is not None:
            cmd.extend(['--replications', str(opts.replications)])
        if hasattr(opts, 'numthreads') and opts.numthreads is not None:
            cmd.extend(['--numthreads', str(opts.numthreads)])
        if opts.timespan is not None:
            cmd.extend(['--timespan', f'{opts.timespan[0]},{opts.timespan[1]}'])
        if trajectory:
            cmd.append('--trajectory')

        return cmd

    def _parse_result_json(self, path: str) -> LDESResult:
        """Parse LDES result JSON into LDESResult dataclass."""
        with open(path, 'r') as f:
            data = json.load(f)

        # Check for error
        if 'error' in data:
            raise RuntimeError(f"LDES solver error: {data['error']}")

        result = LDESResult()

        # Parse dimensions
        dims = data.get('dimensions', {})
        nstations = dims.get('nstations', 0)
        nclasses = dims.get('nclasses', 0)

        # Update names from result if available
        if 'stationNames' in dims:
            self._station_names = dims['stationNames']
        if 'classNames' in dims:
            self._class_names = dims['classNames']

        # Parse metrics
        metrics = data.get('metrics', {})
        for key in ('QN', 'UN', 'RN', 'TN', 'AN', 'WN', 'CN', 'XN'):
            val = metrics.get(key)
            if val is not None:
                arr = self._json_array_to_numpy(val)
                setattr(result, key, arr)

        # Parse confidence intervals
        ci = data.get('confidenceIntervals', {})
        for key in ('QNCI', 'UNCI', 'RNCI', 'TNCI', 'ANCI', 'WNCI'):
            val = ci.get(key)
            if val is not None:
                arr = self._json_array_to_numpy(val)
                setattr(result, key, arr)

        # Parse relative precision
        rp = data.get('relativePrecision', {})
        for key in ('QNRelPrec', 'UNRelPrec', 'RNRelPrec', 'TNRelPrec'):
            val = rp.get(key)
            if val is not None:
                arr = self._json_array_to_numpy(val)
                setattr(result, key, arr)

        # Parse metadata
        result.method = data.get('method', 'default')
        result.runtime = data.get('runtime', 0.0)
        result.converged = data.get('converged', False)
        result.stopping_reason = data.get('stoppingReason', '')
        result.convergence_batches = data.get('convergenceBatches', 0)

        # Parse transient trajectory data
        tran = data.get('transient', {})
        if tran:
            t_arr = tran.get('t')
            if t_arr is not None:
                result.t = np.array(t_arr).reshape(-1, 1)

            for key in ('QNt', 'UNt', 'TNt'):
                val = tran.get(key)
                if val is not None:
                    parsed = []
                    for station_arr in val:
                        station_list = []
                        for class_data in station_arr:
                            if class_data is None:
                                station_list.append(None)
                            else:
                                station_list.append(self._json_array_to_numpy(class_data))
                        parsed.append(station_list)
                    setattr(result, key, parsed)

            rts = tran.get('respTimeSamples')
            if rts is not None:
                parsed_rts = []
                for station_arr in rts:
                    station_list = []
                    for class_samples in station_arr:
                        if class_samples is None:
                            station_list.append(None)
                        else:
                            station_list.append([float(v) for v in class_samples])
                    parsed_rts.append(station_list)
                result.respTimeSamples = parsed_rts

        return result

    @staticmethod
    def _json_array_to_numpy(val) -> np.ndarray:
        """Convert a JSON 2D array (with possible null) to numpy array."""
        if isinstance(val, list) and len(val) > 0 and isinstance(val[0], list):
            # 2D array
            rows = []
            for row in val:
                rows.append([float('nan') if v is None else float(v) for v in row])
            return np.array(rows)
        elif isinstance(val, list):
            # 1D array
            return np.array([float('nan') if v is None else float(v) for v in val])
        else:
            return np.array([[float('nan') if val is None else float(val)]])

    def runAnalyzer(self, trajectory: bool = False) -> LDESResult:
        """
        Run the LDES simulation via subprocess.

        Args:
            trajectory: If True, request trajectory data (QNt, UNt, TNt, t) from ldes.jar.

        Returns:
            LDESResult containing performance metrics
        """
        start_time = time.time()

        with tempfile.TemporaryDirectory(prefix='line_ldes_') as temp_dir:
            model_path = os.path.join(temp_dir, 'model.json')
            result_path = os.path.join(temp_dir, 'result.json')

            # Save model to JSON
            save_model(self.model, model_path)

            # Build and run CLI command
            cmd = self._build_cli_args(model_path, result_path, trajectory=trajectory)

            if self.options.verbose not in ('silent',):
                print(f"SolverLDES command: {' '.join(cmd)}")

            proc = subprocess.run(
                cmd,
                capture_output=True,
                cwd=temp_dir,
                timeout=600,
            )

            if proc.returncode != 0:
                stderr = proc.stderr.decode('utf-8', errors='ignore')
                raise RuntimeError(f"LDES simulation failed (exit code {proc.returncode}): {stderr}")

            if not os.path.isfile(result_path):
                stdout = proc.stdout.decode('utf-8', errors='ignore')
                stderr = proc.stderr.decode('utf-8', errors='ignore')
                raise RuntimeError(
                    f"LDES result file not produced.\nstdout: {stdout}\nstderr: {stderr}"
                )

            # Parse result JSON
            self._result = self._parse_result_json(result_path)

        self._result.runtime = time.time() - start_time
        return self._result

    def run_analyzer(self) -> LDESResult:
        """Alias for runAnalyzer (Python convention)."""
        return self.runAnalyzer()

    def getAvgTable(self) -> pd.DataFrame:
        """
        Get average performance metrics as a DataFrame.

        Returns:
            DataFrame with columns: Station, JobClass, QLen, Util, RespT, ResidT, ArvR, Tput
        """
        if self._result is None:
            self.runAnalyzer()

        result = self._result
        M = len(self._station_names)
        K = len(self._class_names)
        sn = self._sn

        # Make copies of station-level metrics (to avoid modifying originals)
        QN = result.QN.copy() if result.QN is not None else np.zeros((M, K))
        UN = result.UN.copy() if result.UN is not None else np.zeros((M, K))
        RN = result.RN.copy() if result.RN is not None else np.zeros((M, K))
        TN = result.TN.copy() if result.TN is not None else np.zeros((M, K))
        AN = result.AN.copy() if result.AN is not None else np.zeros((M, K))

        # Zero out metrics for classes that don't visit stations based on visit ratios
        hasForkJoin = False
        hasSPN = False
        if hasattr(sn, 'nodetype') and sn.nodetype is not None:
            hasForkJoin = np.any(sn.nodetype == NodeType.FORK) and np.any(sn.nodetype == NodeType.JOIN)
            hasSPN = np.any(sn.nodetype == NodeType.PLACE) or np.any(sn.nodetype == NodeType.TRANSITION)

        if sn is not None and hasattr(sn, 'nchains') and sn.nchains > 0 and not hasSPN:
            if hasattr(sn, 'chains') and sn.chains is not None and hasattr(sn, 'visits') and sn.visits:
                chains_arr = np.asarray(sn.chains)
                for k in range(K):
                    chains_with_class = np.where(chains_arr[:, k] > 0)[0] if k < chains_arr.shape[1] else []
                    if len(chains_with_class) > 0:
                        c = chains_with_class[0]
                        if c in sn.visits and sn.visits[c] is not None:
                            visits_c = np.asarray(sn.visits[c])
                            for i in range(M):
                                if i < visits_c.shape[0] and k < visits_c.shape[1]:
                                    if visits_c[i, k] == 0:
                                        if hasForkJoin and (QN[i, k] > GlobalConstants.FineTol or
                                                           UN[i, k] > GlobalConstants.FineTol or
                                                           TN[i, k] > GlobalConstants.FineTol):
                                            continue
                                        QN[i, k] = 0
                                        UN[i, k] = 0
                                        RN[i, k] = 0
                                        TN[i, k] = 0
                                        AN[i, k] = 0

        # Compute residence times from response times using visit ratios
        WN = None
        if RN is not None and sn is not None and hasattr(sn, 'visits') and sn.visits:
            try:
                WN = sn_get_residt_from_respt(sn, RN, None)
            except Exception:
                WN = RN.copy() if RN is not None else None

        # Identify source stations
        source_stations = set()
        nodetype = self._sn.nodetype if hasattr(self._sn, 'nodetype') else None
        if nodetype is not None:
            stationToNode = self._sn.stationToNode
            if stationToNode is not None:
                stationToNode = np.asarray(stationToNode).flatten()
                nodetype = np.asarray(nodetype).flatten()
                for i in range(M):
                    if i < len(stationToNode):
                        node_idx = int(stationToNode[i])
                        if node_idx < len(nodetype):
                            if int(nodetype[node_idx]) == 0:  # SOURCE = 0
                                source_stations.add(i)

        rows = []
        for i in range(M):
            for r in range(K):
                qlen = QN[i, r] if i < QN.shape[0] and r < QN.shape[1] else 0
                util = UN[i, r] if i < UN.shape[0] and r < UN.shape[1] else 0
                respt = RN[i, r] if i < RN.shape[0] and r < RN.shape[1] else 0
                tput = TN[i, r] if i < TN.shape[0] and r < TN.shape[1] else 0
                arvr = AN[i, r] if i < AN.shape[0] and r < AN.shape[1] else 0

                is_source = i in source_stations

                if is_source:
                    if hasattr(self._sn, 'rates') and self._sn.rates is not None:
                        rates = np.asarray(self._sn.rates)
                        stationToNode = np.asarray(self._sn.stationToNode).flatten()
                        node_idx = int(stationToNode[i])
                        if node_idx < rates.shape[0] and r < rates.shape[1]:
                            tput = rates[node_idx, r]
                    arvr = 0.0

                if abs(qlen) < 1e-12 and abs(util) < 1e-12 and abs(tput) < 1e-12:
                    continue

                residt = respt
                if WN is not None and i < WN.shape[0] and r < WN.shape[1]:
                    residt_val = WN[i, r]
                    if not np.isnan(residt_val) and residt_val >= 0:
                        residt = residt_val

                rows.append({
                    'Station': self._station_names[i],
                    'JobClass': self._class_names[r],
                    'QLen': qlen,
                    'Util': util,
                    'RespT': respt,
                    'ResidT': residt,
                    'ArvR': arvr,
                    'Tput': tput,
                })

        df = pd.DataFrame(rows)

        if not self._table_silent and len(df) > 0:
            print(df.to_string(index=False))

        return df

    def getAvgChainTable(self) -> pd.DataFrame:
        """
        Get average performance metrics aggregated by chain.

        Returns:
            DataFrame with columns: Chain, QLen, Util, RespT, Tput
        """
        if self._result is None:
            self.runAnalyzer()

        result = self._result
        nchains = self._sn.nchains if hasattr(self._sn, 'nchains') else self._sn.nclasses
        inchain = self._sn.inchain if hasattr(self._sn, 'inchain') else None

        rows = []
        for c in range(nchains):
            chain_name = f'Chain{c+1}'

            if inchain is not None and c in inchain:
                chain_classes = np.asarray(inchain[c]).flatten().astype(int)
            else:
                chain_classes = [c]

            total_qlen = 0.0
            total_util = 0.0
            total_respt = 0.0
            total_tput = 0.0

            M = self._sn.nstations
            for i in range(M):
                for k in chain_classes:
                    if k < result.QN.shape[1] if result.QN is not None else 0:
                        total_qlen += result.QN[i, k] if result.QN is not None and not np.isnan(result.QN[i, k]) else 0.0
                        total_util += result.UN[i, k] if result.UN is not None and not np.isnan(result.UN[i, k]) else 0.0
                        total_respt += result.RN[i, k] if result.RN is not None and not np.isnan(result.RN[i, k]) else 0.0
                        total_tput = max(total_tput, result.TN[i, k] if result.TN is not None and not np.isnan(result.TN[i, k]) else 0.0)

            rows.append({
                'Chain': chain_name,
                'QLen': total_qlen,
                'Util': total_util,
                'RespT': total_respt,
                'Tput': total_tput,
            })

        return pd.DataFrame(rows)

    def getAvgSysTable(self) -> pd.DataFrame:
        """
        Get system-level average performance metrics.

        Returns:
            DataFrame with columns: Chain, SysRespT, SysTput
        """
        if self._result is None:
            self.runAnalyzer()

        chain_table = self.getAvgChainTable()
        rows = []
        for _, row in chain_table.iterrows():
            rows.append({
                'Chain': row['Chain'],
                'SysRespT': row['RespT'],
                'SysTput': row['Tput'],
            })

        return pd.DataFrame(rows)

    # =========================================================================
    # Transient analysis
    # =========================================================================

    def getTranAvg(self) -> LDESResult:
        """
        Run transient analysis and return result with time-series metrics.

        If options.timespan is not set, defaults to [0, 30/min_rate].
        The result will contain QNt, UNt, TNt, t fields with trajectory data.

        Returns:
            LDESResult with transient time-series data populated.
        """
        if self.options.timespan is None:
            sn = self._sn
            min_rate = float('inf')
            if hasattr(sn, 'rates') and sn.rates is not None:
                rates = np.asarray(sn.rates).flatten()
                positive_rates = rates[rates > 0]
                if len(positive_rates) > 0:
                    min_rate = float(np.min(positive_rates))
            if min_rate == float('inf'):
                min_rate = 1.0
            self.options.timespan = [0.0, 30.0 / min_rate]

        return self.runAnalyzer(trajectory=True)

    # =========================================================================
    # Sampling methods
    # =========================================================================

    def _run_transient(self, num_events: int) -> LDESResult:
        """Run transient simulation with the given number of events as time horizon."""
        original_timespan = self.options.timespan
        original_samples = self.options.samples
        if num_events > 0:
            self.options.samples = num_events
        self.options.timespan = [0.0, float(self.options.samples)]

        try:
            result = self.runAnalyzer(trajectory=True)
        finally:
            self.options.timespan = original_timespan
            self.options.samples = original_samples

        return result

    def sample(self, node, num_events: int = 0) -> Optional[dict]:
        """
        Generate a sample path (state trajectory) for a specific node.

        Args:
            node: The stateful node to sample (or node index).
            num_events: Number of events for the simulation time horizon.
                        If 0, uses options.samples.

        Returns:
            Dict with keys 't' (time vector), 'state' (numpy array [timepoints x classes]),
            or None if simulation produced no trajectory data.
        """
        result = self._run_transient(num_events)

        if result.t is None or result.QNt is None:
            return None

        sn = self._sn
        node_idx = node if isinstance(node, int) else getattr(node, '_node_idx', 0)

        isf = int(np.asarray(sn.nodeToStateful).flatten()[node_idx])
        num_time_points = result.t.shape[0]
        num_classes = int(sn.nclasses)
        state = np.zeros((num_time_points, num_classes))

        if isf < len(result.QNt):
            for k in range(min(num_classes, len(result.QNt[isf]))):
                class_data = result.QNt[isf][k]
                if class_data is not None:
                    n = min(num_time_points, class_data.shape[0])
                    state[:n, k] = class_data[:n, 0]

        return {'t': result.t.flatten(), 'state': state}

    def sampleAggr(self, node, num_events: int = 0) -> Optional[dict]:
        """
        Generate an aggregated sample path for a specific node.

        For LDES, sample() already returns per-class queue lengths, so this
        is the same as sample() but marked as aggregated.

        Returns:
            Dict with keys 't', 'state', 'aggregated'=True.
        """
        result = self.sample(node, num_events)
        if result is not None:
            result['aggregated'] = True
        return result

    def sampleSys(self, num_events: int = 0) -> Optional[dict]:
        """
        Generate a system-wide sample path for all stateful nodes.

        Returns:
            Dict with keys 't' (time vector), 'states' (list of numpy arrays,
            one per stateful node), or None if no trajectory data.
        """
        result = self._run_transient(num_events)

        if result.t is None or result.QNt is None:
            return None

        sn = self._sn
        num_time_points = result.t.shape[0]
        num_classes = int(sn.nclasses)

        states = []
        for isf in range(int(sn.nstateful)):
            node_state = np.zeros((num_time_points, num_classes))
            if isf < len(result.QNt):
                for k in range(min(num_classes, len(result.QNt[isf]))):
                    class_data = result.QNt[isf][k]
                    if class_data is not None:
                        n = min(num_time_points, class_data.shape[0])
                        node_state[:n, k] = class_data[:n, 0]
            states.append(node_state)

        return {'t': result.t.flatten(), 'states': states}

    def sampleSysAggr(self, num_events: int = 0) -> Optional[dict]:
        """
        Generate an aggregated system-wide sample path.

        Returns:
            Dict with keys 't', 'states', 'aggregated'=True.
        """
        result = self.sampleSys(num_events)
        if result is not None:
            result['aggregated'] = True
        return result

    # =========================================================================
    # Probability estimation methods
    # =========================================================================

    def getProb(self, node, state: Optional[np.ndarray] = None) -> float:
        """
        Estimate steady-state probability of a specific state at a node.

        Uses time-weighted fraction from simulation trajectory.

        Args:
            node: The stateful node (or node index).
            state: Target state vector (per-class job counts). If None, uses
                   current model state.

        Returns:
            Estimated probability (0.0 if state not observed).
        """
        sample_result = self.sample(node, 0)
        if sample_result is None:
            return 0.0

        t = sample_result['t']
        state_matrix = sample_result['state']
        num_time_points = len(t)

        if num_time_points < 2:
            return 0.0

        if state is None:
            sn = self._sn
            node_idx = node if isinstance(node, int) else getattr(node, '_node_idx', 0)
            isf = int(np.asarray(sn.nodeToStateful).flatten()[node_idx])
            state = np.asarray(sn.state[isf]).flatten() if hasattr(sn, 'state') and sn.state else np.zeros(state_matrix.shape[1])

        target = np.asarray(state).flatten()
        total_time = t[-1] - t[0]
        if total_time <= 0:
            return 0.0

        time_in_state = 0.0
        for ti in range(num_time_points - 1):
            dt = t[ti + 1] - t[ti]
            if np.allclose(state_matrix[ti, :len(target)], target, atol=1e-10):
                time_in_state += dt

        return time_in_state / total_time

    def getProbAggr(self, node, state_aggr: Optional[np.ndarray] = None) -> float:
        """
        Estimate aggregated state probability at a node.

        For LDES, same as getProb since sample paths are already per-class.

        Returns:
            Estimated probability.
        """
        return self.getProb(node, state_aggr)

    def getProbSys(self) -> float:
        """
        Estimate joint steady-state probability of the current system state.

        Uses system-wide trajectory to compute time-weighted fraction.

        Returns:
            Estimated joint probability.
        """
        sys_result = self.sampleSys(0)
        if sys_result is None:
            return 0.0

        t = sys_result['t']
        states = sys_result['states']
        num_time_points = len(t)

        if num_time_points < 2 or not states:
            return 0.0

        sn = self._sn
        target_states = []
        for isf in range(len(states)):
            if hasattr(sn, 'state') and sn.state and isf in sn.state:
                target_states.append(np.asarray(sn.state[isf]).flatten())
            else:
                target_states.append(np.zeros(states[isf].shape[1]))

        total_time = t[-1] - t[0]
        if total_time <= 0:
            return 0.0

        time_in_state = 0.0
        for ti in range(num_time_points - 1):
            dt = t[ti + 1] - t[ti]
            all_match = True
            for isf in range(len(states)):
                if not np.allclose(states[isf][ti, :len(target_states[isf])],
                                   target_states[isf], atol=1e-10):
                    all_match = False
                    break
            if all_match:
                time_in_state += dt

        return time_in_state / total_time

    def getProbSysAggr(self) -> float:
        """
        Estimate aggregated joint system probability.

        For LDES, same as getProbSys since trajectories are already per-class.

        Returns:
            Estimated joint probability.
        """
        return self.getProbSys()

    # =========================================================================
    # Transient CDF methods
    # =========================================================================

    def getTranCdfRespT(self) -> Optional[dict]:
        """
        Get empirical CDF of response times from simulation samples.

        Returns:
            Dict with keys 'station_names', 'class_names', 'cdfs'
            where cdfs[i][k] is a numpy array (n x 2) with [time, CDF_value]
            columns, or None if no response time samples available.
        """
        if self._result is None or self._result.respTimeSamples is None:
            result = self._run_transient(0)
        else:
            result = self._result

        if result.respTimeSamples is None:
            return None

        cdfs = []
        for i, station_samples in enumerate(result.respTimeSamples):
            station_cdfs = []
            for k, samples in enumerate(station_samples):
                if samples and len(samples) > 0:
                    sorted_samples = np.sort(samples)
                    n = len(sorted_samples)
                    cdf_values = np.arange(1, n + 1) / n
                    cdf_matrix = np.column_stack([sorted_samples, cdf_values])
                    station_cdfs.append(cdf_matrix)
                else:
                    station_cdfs.append(None)
            cdfs.append(station_cdfs)

        return {
            'station_names': self._station_names,
            'class_names': self._class_names,
            'cdfs': cdfs,
        }

    def getTranCdfPassT(self) -> Optional[dict]:
        """
        Get empirical CDF of passage times from simulation samples.

        For LDES, passage times are approximated by response times.

        Returns:
            Same format as getTranCdfRespT().
        """
        return self.getTranCdfRespT()

    # =========================================================================
    # Transient probability methods
    # =========================================================================

    def getTranProb(self, node) -> Optional[dict]:
        """
        Estimate transient state probabilities at a node over time.

        Args:
            node: The stateful node (or node index).

        Returns:
            Dict with 't' (time points), 'states' (unique state vectors),
            'probabilities' (time-windowed probabilities), or None.
        """
        sample_result = self.sample(node, 0)
        if sample_result is None:
            return None

        t = sample_result['t']
        state_matrix = sample_result['state']

        unique_states = np.unique(np.round(state_matrix, 10), axis=0)
        num_windows = min(100, len(t) // 2)
        if num_windows < 1:
            return None

        window_size = len(t) // num_windows
        t_windows = np.zeros(num_windows)
        probs = np.zeros((num_windows, len(unique_states)))

        for w in range(num_windows):
            start = w * window_size
            end = min((w + 1) * window_size, len(t) - 1)
            t_windows[w] = t[start]
            window_total = t[end] - t[start]
            if window_total <= 0:
                continue
            for si, s in enumerate(unique_states):
                time_in = 0.0
                for ti in range(start, end):
                    dt = t[ti + 1] - t[ti]
                    if np.allclose(state_matrix[ti, :len(s)], s, atol=1e-10):
                        time_in += dt
                probs[w, si] = time_in / window_total

        return {'t': t_windows, 'states': unique_states, 'probabilities': probs}

    def getTranProbAggr(self, node) -> Optional[dict]:
        """Transient aggregated state probabilities. For LDES, same as getTranProb."""
        return self.getTranProb(node)

    def getTranProbSys(self) -> Optional[dict]:
        """
        Estimate transient joint system probabilities over time.

        Returns:
            Dict with 't', 'probabilities' for system state, or None.
        """
        sys_result = self.sampleSys(0)
        if sys_result is None:
            return None

        t = sys_result['t']
        states = sys_result['states']
        if len(t) < 2 or not states:
            return None

        # Build combined system state at each time point
        combined = np.hstack(states)
        unique_sys_states = np.unique(np.round(combined, 10), axis=0)

        num_windows = min(100, len(t) // 2)
        if num_windows < 1:
            return None

        window_size = len(t) // num_windows
        t_windows = np.zeros(num_windows)
        probs = np.zeros((num_windows, len(unique_sys_states)))

        for w in range(num_windows):
            start = w * window_size
            end = min((w + 1) * window_size, len(t) - 1)
            t_windows[w] = t[start]
            window_total = t[end] - t[start]
            if window_total <= 0:
                continue
            for si, s in enumerate(unique_sys_states):
                time_in = 0.0
                for ti in range(start, end):
                    dt = t[ti + 1] - t[ti]
                    if np.allclose(combined[ti], s, atol=1e-10):
                        time_in += dt
                probs[w, si] = time_in / window_total

        return {'t': t_windows, 'states': unique_sys_states, 'probabilities': probs}

    def getTranProbSysAggr(self) -> Optional[dict]:
        """Transient aggregated system probabilities. For LDES, same as getTranProbSys."""
        return self.getTranProbSys()

    # Method aliases (consistent with other solvers)
    avgT = getAvgTable
    aT = getAvgTable
    get_avg_chain_table = getAvgChainTable
    avg_chain_table = getAvgChainTable
    aCT = getAvgChainTable
    chainAvgT = getAvgChainTable
    get_avg_sys_table = getAvgSysTable
    avg_sys_table = getAvgSysTable

    @property
    def result(self) -> Optional[LDESResult]:
        """Get the LDES result (after runAnalyzer is called)."""
        return self._result

    def getName(self) -> str:
        """Get solver name."""
        return "LDES"

    def get_name(self) -> str:
        """Get solver name (Python convention)."""
        return self.getName()

    @staticmethod
    def defaultOptions() -> LDESOptions:
        """Get default LDES solver options."""
        return LDESOptions()

    @staticmethod
    def default_options() -> LDESOptions:
        """Get default options (Python convention)."""
        return LDESOptions()
