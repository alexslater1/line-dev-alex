% Test gallery_mmapk with SolverMVA
model = gallery_mmapk();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
