#!/usr/bin/env python3
"""Test gallery_mm1_ps_reentrant with SolverMVA"""

import sys
sys.path.insert(0, '../gallery')
from gallery_mm1_ps_reentrant import gallery_mm1_ps_reentrant
from line_solver import SolverMVA

if __name__ == '__main__':
    model = gallery_mm1_ps_reentrant()
    solver = SolverMVA(model)
    avg_table = solver.getAvgTable()
    print(f'Model: {model.getName()}')
    print(avg_table)
