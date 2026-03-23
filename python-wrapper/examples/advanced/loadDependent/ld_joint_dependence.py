"""
Load-Dependent Service - Joint Dependence

This example demonstrates joint-dependent service rates, where the service
rate scaling depends on the per-class population vector (n1, n2) at a station.
The scaling table is stored as a linearized lookup table.

Copyright (c) 2012-2025, Imperial College London
All rights reserved.
"""

from line_solver import *
import numpy as np

N1 = 3  # number of class-1 jobs
N2 = 2  # number of class-2 jobs

print('=== Load-Dependent Service - Joint Dependence ===\n')

# Create model
model = Network('JointDependenceModel')

node_delay = Delay(model, 'Delay')
node_queue = Queue(model, 'Queue1', SchedStrategy.PS)

jobclass1 = ClosedClass(model, 'Class1', N1, node_delay, 0)
jobclass2 = ClosedClass(model, 'Class2', N2, node_delay, 0)

node_delay.set_service(jobclass1, Exp.fit_mean(1.0))
node_delay.set_service(jobclass2, Exp.fit_mean(2.0))
node_queue.set_service(jobclass1, Exp.fit_mean(1.5))
node_queue.set_service(jobclass2, Exp.fit_mean(2.5))

# Define cutoffs for per-class population
cutoffs = [N1, N2]  # max population per class

# Create scaling table for joint dependence
# Table size = (N1+1) * (N2+1) = 4 * 3 = 12
# Linearized index: idx = 1 + n1 + n2*(N1+1) (1-indexed)

# Helper: inline linearize (matching MATLAB ljd_linearize)
def ljd_linearize(nvec, cutoffs):
    idx = 1
    multiplier = 1
    for k in range(len(nvec)):
        nk = min(nvec[k], cutoffs[k])
        idx += nk * multiplier
        multiplier *= (cutoffs[k] + 1)
    return idx

scaling_table = np.zeros(int(np.prod(np.array(cutoffs) + 1)))
for n1 in range(N1 + 1):
    for n2 in range(N2 + 1):
        idx = ljd_linearize([n1, n2], cutoffs)
        # Example: scaling decreases as total population increases
        total_n = n1 + n2
        if total_n == 0:
            scaling_table[idx - 1] = 1.0
        elif total_n <= 2:
            scaling_table[idx - 1] = 1.0 / total_n  # slower service with more jobs
        else:
            scaling_table[idx - 1] = 0.5 / total_n  # even slower at high load

# Set joint dependence on Queue1
node_queue.set_joint_dependence(scaling_table.tolist(), cutoffs)

# Routing
P = model.init_routing_matrix()
P[0, 0] = model.serial_routing([node_delay, node_queue])
P[1, 1] = model.serial_routing([node_delay, node_queue])
model.link(P)

# Solve with MVA (qd method supports LJD)
print('MVA with QD method:')
avg_table_mva = MVA(model, method='qd').getAvgTable()
print(avg_table_mva)

print('\nNote: Joint dependence allows class-specific scaling.')
print('This example uses total population scaling as a simpler illustration.')
