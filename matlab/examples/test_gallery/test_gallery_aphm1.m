% Test gallery_aphm1 with SolverMVA
model = gallery_aphm1();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
