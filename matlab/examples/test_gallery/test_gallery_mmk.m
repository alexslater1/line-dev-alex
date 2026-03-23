% Test gallery_mmk with SolverMVA
model = gallery_mmk();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
