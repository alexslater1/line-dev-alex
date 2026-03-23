#!/usr/bin/env python3
"""Test gallery_merlk with SolverMVA"""

import sys
sys.path.insert(0, '../gallery')
from gallery_merlk import gallery_merlk
from line_solver import SolverMVA

if __name__ == '__main__':
    model = gallery_merlk()
    solver = SolverMVA(model)
    avg_table = solver.getAvgTable()
    print(f'Model: {model.getName()}')
    print(avg_table)
