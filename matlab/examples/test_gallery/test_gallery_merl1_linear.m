% Test gallery_merl1_linear with SolverMVA
model = gallery_merl1_linear();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
