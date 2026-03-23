% Test gallery_lukumar_reentrant with SolverMVA
model = gallery_lukumar_reentrant();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
