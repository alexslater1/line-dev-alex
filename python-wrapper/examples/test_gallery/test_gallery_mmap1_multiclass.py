#!/usr/bin/env python3
"""Test gallery_mmap1_multiclass with SolverMVA"""

import sys
sys.path.insert(0, '../gallery')
from gallery_mmap1_multiclass import gallery_mmap1_multiclass
from line_solver import SolverMVA

if __name__ == '__main__':
    model = gallery_mmap1_multiclass()
    solver = SolverMVA(model)
    avg_table = solver.getAvgTable()
    print(f'Model: {model.getName()}')
    print(avg_table)
