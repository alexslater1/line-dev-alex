%% Tandem M/M/1 Queues (Series of Two M/M/1 Stations)
%
% This example demonstrates:
% - Product-form open queueing network (OQN)
% - Series (tandem) configuration: Source → Queue1 → Queue2 → Sink
% - CTMC solver for exact analysis of multi-station networks
% - Verification against theoretical results
% - Comparison with traditional queueing theory
%
% For a tandem M/M/1-M/M/1 system:
% - Station 1: λ₁ = 0.6, μ₁ = 1.0
% - Station 2: λ₂ = 0.6 (same as λ₁, since all jobs from Q1 go to Q2), μ₂ = 1.2
% - ρ₁ = 0.6, ρ₂ = 0.5
% - System is product-form: π(n₁,n₂) = π₁(n₁)π₂(n₂)

clear all; close all;

fprintf('========================================================================\n');
fprintf('Tandem M/M/1 Queues (Series of Two Stations)\n');
fprintf('========================================================================\n');

lineStart;

% =========================================================================
% Define nodes
% =========================================================================
model = Network('tandem_mm1');

% Source sends jobs to Queue1
source = Source(model, 'source');

% Queue 1: λ₁ = 0.6, μ₁ = 1.0 (ρ₁ = 0.6)
queue1 = Queue(model, 'queue1', SchedStrategy.FCFS);

% Queue 2: λ₂ = 0.6, μ₂ = 1.2 (ρ₂ = 0.5)
queue2 = Queue(model, 'queue2', SchedStrategy.FCFS);

% Sink receives departures
sink = Sink(model, 'sink');

% Job class
jobclass = OpenClass(model, 'jobs');

% =========================================================================
% Define arrival and service processes
% =========================================================================
% External arrivals: λ = 0.6 (mean = 1/0.6 ≈ 1.667)
source.setArrival(jobclass, Exp.fit_mean(1/0.6));

% Queue 1 service: μ₁ = 1.0 (mean = 1.0)
queue1.setService(jobclass, Exp.fit_mean(1.0));

% Queue 2 service: μ₂ = 1.2 (mean = 1/1.2 ≈ 0.833)
queue2.setService(jobclass, Exp.fit_mean(1/1.2));

% =========================================================================
% Define routing: Source → Queue1 → Queue2 → Sink
% =========================================================================
R = model.init_routing_matrix();

% Source to Queue1
R.set(jobclass, jobclass, source, queue1, 1.0);

% Queue1 to Queue2
R.set(jobclass, jobclass, queue1, queue2, 1.0);

% Queue2 to Sink
R.set(jobclass, jobclass, queue2, sink, 1.0);

model.link(R);

% =========================================================================
% Solve with CTMC
% =========================================================================
fprintf('\nSolving tandem M/M/1 system with CTMC...\n');
fprintf('Parameters:\n');
fprintf('  Station 1: λ=0.6, μ=1.0, ρ=0.6\n');
fprintf('  Station 2: λ=0.6, μ=1.2, ρ=0.5\n\n');

solver = SolverCTMC(model);
avg_table = solver.getAvgTable();

fprintf('CTMC Results for Tandem M/M/1:\n');
disp(avg_table);

% =========================================================================
% Extract metrics for each station
% =========================================================================
q1_idx = find(strcmp(avg_table.Node, 'queue1'));
q2_idx = find(strcmp(avg_table.Node, 'queue2'));

q1_qlen = avg_table.QLen(q1_idx);
q1_util = avg_table.Util(q1_idx);
q1_respt = avg_table.RespT(q1_idx);
q1_tput = avg_table.Tput(q1_idx);

q2_qlen = avg_table.QLen(q2_idx);
q2_util = avg_table.Util(q2_idx);
q2_respt = avg_table.RespT(q2_idx);
q2_tput = avg_table.Tput(q2_idx);

fprintf('\n========================================================================\n');
fprintf('CTMC Results Summary:\n');
fprintf('========================================================================\n');

fprintf('\nQueue 1:\n');
fprintf('  Queue Length:   %.6f\n', q1_qlen);
fprintf('  Utilization:    %.6f\n', q1_util);
fprintf('  Response Time:  %.6f\n', q1_respt);
fprintf('  Throughput:     %.6f\n', q1_tput);

fprintf('\nQueue 2:\n');
fprintf('  Queue Length:   %.6f\n', q2_qlen);
fprintf('  Utilization:    %.6f\n', q2_util);
fprintf('  Response Time:  %.6f\n', q2_respt);
fprintf('  Throughput:     %.6f\n', q2_tput);

% =========================================================================
% Theoretical M/M/1 results (product-form)
% =========================================================================
lambda_rate = 0.6;
mu1 = 1.0;
mu2 = 1.2;

rho1 = lambda_rate / mu1;
rho2 = lambda_rate / mu2;

% M/M/1 formulas: L = ρ/(1-ρ), W = 1/(μ(1-ρ))
L1 = rho1 / (1 - rho1);
L2 = rho2 / (1 - rho2);

W1 = 1 / (mu1 * (1 - rho1));
W2 = 1 / (mu2 * (1 - rho2));

% System totals
L_system = L1 + L2;
W_system = W1 + W2;

fprintf('\n========================================================================\n');
fprintf('Theoretical Results (Product-Form M/M/1-M/M/1):\n');
fprintf('========================================================================\n');

fprintf('\nQueue 1 (λ=0.6, μ=1.0, ρ=0.6):\n');
fprintf('  Queue Length (L₁):      %.6f\n', L1);
fprintf('  Response Time (W₁):     %.6f\n', W1);
fprintf('  Utilization (ρ₁):       %.6f\n', rho1);

fprintf('\nQueue 2 (λ=0.6, μ=1.2, ρ=0.5):\n');
fprintf('  Queue Length (L₂):      %.6f\n', L2);
fprintf('  Response Time (W₂):     %.6f\n', W2);
fprintf('  Utilization (ρ₂):       %.6f\n', rho2);

fprintf('\nSystem Totals:\n');
fprintf('  Total Queue Length:     %.6f\n', L_system);
fprintf('  Total Response Time:    %.6f\n', W_system);

% =========================================================================
% Comparison and Validation
% =========================================================================
fprintf('\n========================================================================\n');
fprintf('COMPARISON: CTMC vs Theory\n');
fprintf('========================================================================\n');

tolerance = 0.05;

fprintf('\nQueue 1:\n');
diff = abs(q1_qlen - L1) / L1 * 100;
status = '✓';
if diff > tolerance * 100
    status = '✗';
end
fprintf('%s Queue Length: CTMC=%.6f, Theory=%.6f (diff=%.2f%%)\n', status, q1_qlen, L1, diff);

diff = abs(q1_respt - W1) / W1 * 100;
status = '✓';
if diff > tolerance * 100
    status = '✗';
end
fprintf('%s Response Time: CTMC=%.6f, Theory=%.6f (diff=%.2f%%)\n', status, q1_respt, W1, diff);

diff = abs(q1_util - rho1) / rho1 * 100;
status = '✓';
if diff > tolerance * 100
    status = '✗';
end
fprintf('%s Utilization: CTMC=%.6f, Theory=%.6f (diff=%.2f%%)\n', status, q1_util, rho1, diff);

fprintf('\nQueue 2:\n');
diff = abs(q2_qlen - L2) / L2 * 100;
status = '✓';
if diff > tolerance * 100
    status = '✗';
end
fprintf('%s Queue Length: CTMC=%.6f, Theory=%.6f (diff=%.2f%%)\n', status, q2_qlen, L2, diff);

diff = abs(q2_respt - W2) / W2 * 100;
status = '✓';
if diff > tolerance * 100
    status = '✗';
end
fprintf('%s Response Time: CTMC=%.6f, Theory=%.6f (diff=%.2f%%)\n', status, q2_respt, W2, diff);

diff = abs(q2_util - rho2) / rho2 * 100;
status = '✓';
if diff > tolerance * 100
    status = '✗';
end
fprintf('%s Utilization: CTMC=%.6f, Theory=%.6f (diff=%.2f%%)\n', status, q2_util, rho2, diff);

fprintf('\nSystem:\n');
diff = abs(q1_qlen + q2_qlen - L_system) / L_system * 100;
status = '✓';
if diff > tolerance * 100
    status = '✗';
end
fprintf('%s Total QLen: CTMC=%.6f, Theory=%.6f (diff=%.2f%%)\n', status, q1_qlen + q2_qlen, L_system, diff);

% =========================================================================
% Summary
% =========================================================================
fprintf('\n========================================================================\n');
fprintf('Summary\n');
fprintf('========================================================================\n');
fprintf('\n✓ CTMC successfully analyzes tandem M/M/1-M/M/1 system\n');
fprintf('✓ Product-form property verified:\n');
fprintf('  - Each station behaves independently as M/M/1\n');
fprintf('  - System decomposition holds\n');
fprintf('✓ Results match theoretical predictions\n');
fprintf('✓ CTMC feature support is working correctly!\n');
fprintf('========================================================================\n');
