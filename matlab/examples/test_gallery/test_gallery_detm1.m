% Test gallery_detm1 with SolverMVA
model = gallery_detm1();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
