#!/usr/bin/env python3
"""Test gallery_mm1_tandem_multiclass with SolverMVA"""

import sys
sys.path.insert(0, '../gallery')
from gallery_mm1_tandem_multiclass import gallery_mm1_tandem_multiclass
from line_solver import SolverMVA

if __name__ == '__main__':
    model = gallery_mm1_tandem_multiclass()
    solver = SolverMVA(model)
    avg_table = solver.getAvgTable()
    print(f'Model: {model.getName()}')
    print(avg_table)
