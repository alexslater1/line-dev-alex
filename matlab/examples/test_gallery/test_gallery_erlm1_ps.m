% Test gallery_erlm1_ps with SolverMVA
model = gallery_erlm1_ps();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
