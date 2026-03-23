% Test gallery_mm1 with SolverMVA
model = gallery_mm1();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
