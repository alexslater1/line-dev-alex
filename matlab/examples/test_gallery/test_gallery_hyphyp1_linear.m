% Test gallery_hyphyp1_linear with SolverMVA
model = gallery_hyphyp1_linear();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
