% Test gallery_mapm1 with SolverMVA
model = gallery_mapm1();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
