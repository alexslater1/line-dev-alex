% Test gallery_mhypk with SolverMVA
model = gallery_mhypk();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
