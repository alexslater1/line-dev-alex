% Test gallery_erlm1ps with SolverMVA
model = gallery_erlm1ps();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
