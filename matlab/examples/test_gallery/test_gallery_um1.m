% Test gallery_um1 with SolverMVA
model = gallery_um1();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
