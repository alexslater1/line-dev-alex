% Test gallery_dm1 with SolverMVA
model = gallery_dm1();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
