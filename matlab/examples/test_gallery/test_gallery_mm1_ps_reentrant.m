% Test gallery_mm1_ps_reentrant with SolverMVA
model = gallery_mm1_ps_reentrant();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
