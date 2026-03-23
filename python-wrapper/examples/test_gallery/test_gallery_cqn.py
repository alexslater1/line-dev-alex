#!/usr/bin/env python3
"""Test gallery_cqn with SolverMVA"""

import sys
sys.path.insert(0, '../gallery')
from gallery_cqn import gallery_cqn
from line_solver import SolverMVA

if __name__ == '__main__':
    model = gallery_cqn()
    solver = SolverMVA(model)
    avg_table = solver.getAvgTable()
    print(f'Model: {model.getName()}')
    print(avg_table)
