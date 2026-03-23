% Test gallery_erldk with SolverMVA
model = gallery_erldk();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
