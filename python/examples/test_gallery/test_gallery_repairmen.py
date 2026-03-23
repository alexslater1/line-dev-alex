#!/usr/bin/env python3
"""Test gallery_repairmen with SolverMVA"""

import sys
sys.path.insert(0, '../gallery')
from gallery_repairmen import gallery_repairmen
from line_solver import SolverMVA

if __name__ == '__main__':
    model = gallery_repairmen()
    solver = SolverMVA(model)
    avg_table = solver.getAvgTable()
    print(f'Model: {model.getName()}')
    print(avg_table)
