% Test gallery_merlk with SolverMVA
model = gallery_merlk();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
