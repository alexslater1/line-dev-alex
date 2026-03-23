% Test gallery_mdk with SolverMVA
model = gallery_mdk();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
