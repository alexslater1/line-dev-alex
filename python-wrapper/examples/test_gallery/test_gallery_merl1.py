#!/usr/bin/env python3
"""Test gallery_merl1 with SolverMVA"""

import sys
sys.path.insert(0, '../gallery')
from gallery_merl1 import gallery_merl1
from line_solver import SolverMVA

if __name__ == '__main__':
    model = gallery_merl1()
    solver = SolverMVA(model)
    avg_table = solver.getAvgTable()
    print(f'Model: {model.getName()}')
    print(avg_table)
