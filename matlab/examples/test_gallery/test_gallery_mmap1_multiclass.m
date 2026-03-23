% Test gallery_mmap1_multiclass with SolverMVA
model = gallery_mmap1_multiclass();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
