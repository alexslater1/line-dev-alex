#!/usr/bin/env python3
"""Test gallery_dm1 with SolverMVA"""

import sys
sys.path.insert(0, '../gallery')
from gallery_dm1 import gallery_dm1
from line_solver import SolverMVA

if __name__ == '__main__':
    model = gallery_dm1()
    solver = SolverMVA(model)
    avg_table = solver.getAvgTable()
    print(f'Model: {model.getName()}')
    print(avg_table)
