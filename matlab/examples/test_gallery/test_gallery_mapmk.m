% Test gallery_mapmk with SolverMVA
model = gallery_mapmk();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
