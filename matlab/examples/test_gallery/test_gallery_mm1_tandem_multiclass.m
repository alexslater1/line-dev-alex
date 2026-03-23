% Test gallery_mm1_tandem_multiclass with SolverMVA
model = gallery_mm1_tandem_multiclass();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
