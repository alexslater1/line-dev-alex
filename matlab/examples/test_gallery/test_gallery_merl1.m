% Test gallery_merl1 with SolverMVA
model = gallery_merl1();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
