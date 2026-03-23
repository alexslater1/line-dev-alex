% Test gallery_erlerl1 with SolverMVA
model = gallery_erlerl1();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
