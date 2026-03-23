% Test gallery_hyperlk with SolverMVA
model = gallery_hyperlk();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
