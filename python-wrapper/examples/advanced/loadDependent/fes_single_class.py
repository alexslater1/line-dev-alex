"""
Single-Class FES Aggregation Example

This tests FES aggregation with a single job class, where it should be exact.

Copyright (c) 2012-2026, Imperial College London
All rights reserved.
"""

from line_solver import *

print('=== Single-Class FES Aggregation Example ===\n')

# Create original 4-station tandem network with 1 class
print('Creating original 4-station network...')

N1 = 5  # number of jobs

model = Network('OriginalModel')

# Create stations
delay = Delay(model, 'ThinkTime')
queue1 = Queue(model, 'Queue1', SchedStrategy.PS)
queue2 = Queue(model, 'Queue2', SchedStrategy.PS)
queue3 = Queue(model, 'Queue3', SchedStrategy.PS)

# Create job class
jobclass1 = ClosedClass(model, 'Class1', N1, delay, 0)

# Set service times
delay.set_service(jobclass1, Exp.fit_mean(5.0))
queue1.set_service(jobclass1, Exp.fit_mean(1.5))
queue2.set_service(jobclass1, Exp.fit_mean(1.0))
queue3.set_service(jobclass1, Exp.fit_mean(0.8))

# Set up tandem routing
P = model.init_routing_matrix()
P[0, 0] = model.serial_routing([delay, queue1, queue2, queue3])
model.link(P)

# Solve original model with MVA
print('\n--- Solving Original Model ---')
solver_original = MVA(model)
avg_table_original = solver_original.getAvgTable()
print('Original model results:')
print(avg_table_original)

# Aggregate stations Queue1 and Queue2 into a Flow-Equivalent Server
print('\n--- Creating FES Model ---')
print('Aggregating Queue1 and Queue2 into a single FES...')

station_subset = [queue1, queue2]

try:
    # Use default options (verbose and solver settings are handled by JAR defaults)
    result = ModelAdapter.aggregate_fes(model, station_subset)

    print('\nFES model created successfully!')
    print(f'FES station name: {result.fesStation.get_name()}')
    print(f'Number of stations in FES model: {result.model.get_number_of_stations()}')

    # Solve FES model
    print('\n--- Solving FES Model ---')
    solver_fes = MVA(result.model)
    avg_table_fes = solver_fes.getAvgTable()
    print('FES model results:')
    print(avg_table_fes)

    # Compare throughputs
    print('\n--- Throughput Comparison ---')

    # Find throughput at ThinkTime station
    tput_orig = None
    for _, row in avg_table_original.iterrows():
        if row['Station'] == 'ThinkTime':
            tput_orig = row['Tput']
            break

    tput_fes = None
    for _, row in avg_table_fes.iterrows():
        if row['Station'] == 'ThinkTime':
            tput_fes = row['Tput']
            break

    if tput_orig is not None and tput_fes is not None:
        rel_error = abs(tput_orig - tput_fes) / max(tput_orig, 1e-10) * 100
        print(f'Throughput: Original={tput_orig:.4f}, FES={tput_fes:.4f}, RelError={rel_error:.2f}%')

        if rel_error < 1.0:
            print('\nSUCCESS: FES aggregation is nearly exact (< 1% error)')
        else:
            print('\nWARNING: FES aggregation has significant error')

except Exception as e:
    print(f'Error during FES aggregation: {e}')
    import traceback
    traceback.print_exc()

print('\n=== Example Complete ===')
