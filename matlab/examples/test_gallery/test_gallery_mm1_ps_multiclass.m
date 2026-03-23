% Test gallery_mm1_ps_multiclass with SolverMVA
model = gallery_mm1_ps_multiclass();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
