% Test gallery_mm1_ps with SolverMVA
model = gallery_mm1_ps();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
