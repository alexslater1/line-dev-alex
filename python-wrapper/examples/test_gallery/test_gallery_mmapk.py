#!/usr/bin/env python3
"""Test gallery_mmapk with SolverMVA"""

import sys
sys.path.insert(0, '../gallery')
from gallery_mmapk import gallery_mmapk
from line_solver import SolverMVA

if __name__ == '__main__':
    model = gallery_mmapk()
    solver = SolverMVA(model)
    avg_table = solver.getAvgTable()
    print(f'Model: {model.getName()}')
    print(avg_table)
