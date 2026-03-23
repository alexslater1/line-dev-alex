% Test gallery_mm1_linear with SolverMVA
model = gallery_mm1_linear();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
