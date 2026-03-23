#!/usr/bin/env python3
"""Test gallery_mmk with SolverMVA"""

import sys
sys.path.insert(0, '../gallery')
from gallery_mmk import gallery_mmk
from line_solver import SolverMVA

if __name__ == '__main__':
    model = gallery_mmk()
    solver = SolverMVA(model)
    avg_table = solver.getAvgTable()
    print(f'Model: {model.getName()}')
    print(avg_table)
