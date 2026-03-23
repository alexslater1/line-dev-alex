#!/usr/bin/env python3
"""Test gallery_mhyp1_tandem with SolverMVA"""

import sys
sys.path.insert(0, '../gallery')
from gallery_mhyp1_tandem import gallery_mhyp1_tandem
from line_solver import SolverMVA

if __name__ == '__main__':
    model = gallery_mhyp1_tandem()
    solver = SolverMVA(model)
    avg_table = solver.getAvgTable()
    print(f'Model: {model.getName()}')
    print(avg_table)
