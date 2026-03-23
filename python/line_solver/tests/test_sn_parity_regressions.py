import os
import sys
import unittest
from types import SimpleNamespace

import numpy as np

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))

from line_solver.api.sn.getters import sn_get_arvr_from_tput
from line_solver.api.sn.network_struct import NetworkStruct, NodeType
from line_solver.api.sn.transforms import sn_get_product_form_params, sn_refresh_visits


class TestSNParityRegressions(unittest.TestCase):
    def test_refresh_visits_normalizes_with_station_to_stateful(self):
        sn = NetworkStruct()
        sn.rt = np.array([[0.8, 0.2], [0.4, 0.6]], dtype=float)
        sn.rt_visits = sn.rt.copy()
        sn.rtnodes = None
        sn.nstations = 2
        sn.nstateful = 2
        sn.nnodes = 2
        sn.nclasses = 1
        sn.nchains = 1
        sn.njobs = np.array([1.0])
        sn.nodetype = np.array([NodeType.QUEUE, NodeType.QUEUE], dtype=int)
        sn.nodeToStation = np.array([1, 0], dtype=int)
        sn.stationToNode = np.array([1, 0], dtype=int)
        sn.stationToStateful = np.array([0, 1], dtype=int)
        sn.statefulToNode = np.array([0, 1], dtype=int)
        sn.refstat = np.array([0], dtype=int)
        sn.inchain = {0: np.array([0], dtype=int)}

        sn_refresh_visits(sn)

        np.testing.assert_allclose(sn.visits[0][:, 0], np.array([1.0, 0.5]))

    def test_refresh_visits_sanitizes_station_routing_nans(self):
        sn = NetworkStruct()
        sn.rt = np.array([[np.nan, 1.0], [1.0, 0.0]], dtype=float)
        sn.rt_visits = sn.rt.copy()
        sn.rtnodes = None
        sn.nstations = 2
        sn.nstateful = 2
        sn.nnodes = 2
        sn.nclasses = 1
        sn.nchains = 1
        sn.njobs = np.array([1.0])
        sn.nodetype = np.array([NodeType.QUEUE, NodeType.QUEUE], dtype=int)
        sn.nodeToStation = np.array([0, 1], dtype=int)
        sn.stationToNode = np.array([0, 1], dtype=int)
        sn.stationToStateful = np.array([0, 1], dtype=int)
        sn.statefulToNode = np.array([0, 1], dtype=int)
        sn.refstat = np.array([0], dtype=int)
        sn.inchain = {0: np.array([0], dtype=int)}

        sn_refresh_visits(sn)

        np.testing.assert_allclose(sn.visits[0][:, 0], np.array([1.0, 1.0]))

    def test_product_form_params_accept_reference_class_zero(self):
        sn = NetworkStruct()
        sn.nstations = 2
        sn.nstateful = 2
        sn.nnodes = 2
        sn.nclasses = 1
        sn.njobs = np.array([1.0])
        sn.nodetype = np.array([NodeType.DELAY, NodeType.QUEUE], dtype=int)
        sn.nodeToStation = np.array([0, 1], dtype=int)
        sn.nodeToStateful = np.array([0, 1], dtype=int)
        sn.stationToStateful = np.array([0, 1], dtype=int)
        sn.statefulToStation = np.array([0, 1], dtype=int)
        sn.nservers = np.array([1, 1], dtype=int)
        sn.rates = np.array([[1.0], [1.0]], dtype=float)
        sn.chains = np.array([0], dtype=int)
        sn.refstat = np.array([0], dtype=int)
        sn.refclass = np.array([0], dtype=int)
        sn.visits = {0: np.array([[2.0], [8.0]], dtype=float)}

        params = sn_get_product_form_params(sn)

        self.assertAlmostEqual(params.D[0, 0], 4.0)
        self.assertAlmostEqual(params.Z[0], 1.0)

    def test_cache_hit_miss_throughput_keeps_class_zero(self):
        sn = NetworkStruct()
        sn.nstations = 2
        sn.nstateful = 3
        sn.nnodes = 3
        sn.nclasses = 2
        sn.nchains = 1
        sn.nodetype = np.array([NodeType.SOURCE, NodeType.CACHE, NodeType.QUEUE], dtype=int)
        sn.isstateful = np.array([True, True, True], dtype=bool)
        sn.nodeToStation = np.array([0, -1, 1], dtype=int)
        sn.stationToNode = np.array([0, 2], dtype=int)
        sn.inchain = {0: np.array([0, 1], dtype=int)}
        sn.refstat = np.array([0], dtype=int)
        sn.nodeparam = {
            1: SimpleNamespace(
                hitclass=np.array([0], dtype=int),
                missclass=np.array([1], dtype=int),
                actualhitprob=np.array([0.6], dtype=float),
                actualmissprob=np.array([0.4], dtype=float),
            )
        }

        sn.rt = np.zeros((sn.nstateful * sn.nclasses, sn.nstateful * sn.nclasses), dtype=float)
        cache_sf = 1
        queue_sf = 2
        sn.rt[cache_sf * sn.nclasses + 0, queue_sf * sn.nclasses + 0] = 1.0
        sn.rt[cache_sf * sn.nclasses + 1, queue_sf * sn.nclasses + 1] = 1.0

        TN = np.array([[5.0, 0.0], [0.0, 0.0]], dtype=float)

        AN = sn_get_arvr_from_tput(sn, TN)

        np.testing.assert_allclose(AN[1], np.array([3.0, 2.0]))


if __name__ == "__main__":
    unittest.main()
