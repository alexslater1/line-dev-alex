#!/usr/bin/env python3
"""Test gallery_hyphyp1_linear with SolverMVA"""

import sys
sys.path.insert(0, '../gallery')
from gallery_hyphyp1_linear import gallery_hyphyp1_linear
from line_solver import SolverMVA

if __name__ == '__main__':
    model = gallery_hyphyp1_linear()
    solver = SolverMVA(model)
    avg_table = solver.getAvgTable()
    print(f'Model: {model.getName()}')
    print(avg_table)
