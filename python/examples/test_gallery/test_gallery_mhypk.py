#!/usr/bin/env python3
"""Test gallery_mhypk with SolverMVA"""

import sys
sys.path.insert(0, '../gallery')
from gallery_mhypk import gallery_mhypk
from line_solver import SolverMVA

if __name__ == '__main__':
    model = gallery_mhypk()
    solver = SolverMVA(model)
    avg_table = solver.getAvgTable()
    print(f'Model: {model.getName()}')
    print(avg_table)
