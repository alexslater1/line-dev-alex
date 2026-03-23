#!/usr/bin/env python3
"""Test gallery_mapmk with SolverMVA"""

import sys
sys.path.insert(0, '../gallery')
from gallery_mapmk import gallery_mapmk
from line_solver import SolverMVA

if __name__ == '__main__':
    model = gallery_mapmk()
    solver = SolverMVA(model)
    avg_table = solver.getAvgTable()
    print(f'Model: {model.getName()}')
    print(avg_table)
