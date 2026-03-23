% Test gallery_erlm1 with SolverMVA
model = gallery_erlm1();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
