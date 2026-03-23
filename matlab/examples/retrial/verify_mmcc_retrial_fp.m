%% Verify M/M/c/c retrial fixed-point approximation against JMT simulation
% Parameters: lambda=2, mu=1, c=3

lambda = 2;
mu = 1;
c = 3;

%% Fixed-point approximation
[blocProb_fp, r_fp, niter] = qsys_mmcc_retrial_fp(lambda, mu, c);
fprintf('=== Fixed-Point Approximation ===\n');
fprintf('Blocking prob (per attempt): %.6f\n', blocProb_fp);
fprintf('Mean busy servers:           %.6f\n\n', (lambda+r_fp)/mu * (1 - blocProb_fp));

%% JMT simulation with retrial (single run)
mu_r = 1.0;

model = Network('MMcc_Retrial');
source = Source(model, 'Source');
queue  = Queue(model, 'Queue', SchedStrategy.FCFS);
sink   = Sink(model, 'Sink');

queue.setNumberOfServers(c);
queue.setCapacity(c);

oclass = OpenClass(model, 'Class1');
source.setArrival(oclass, Exp(lambda));
queue.setService(oclass, Exp(mu));
queue.setRetrial(oclass, Exp(mu_r));

model.link(Network.serialRouting(source, queue, sink));

fprintf('=== SolverJMT (mu_retrial=%.1f, 5000 samples) ===\n', mu_r);
solver = SolverJMT(model, 'seed', 23000, 'samples', 5000);
avgTable = solver.getAvgTable();
disp(avgTable);
