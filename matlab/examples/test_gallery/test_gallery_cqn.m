% Test gallery_cqn with SolverMVA
model = gallery_cqn();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
