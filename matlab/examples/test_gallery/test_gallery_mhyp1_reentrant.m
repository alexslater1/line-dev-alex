% Test gallery_mhyp1_reentrant with SolverMVA
model = gallery_mhyp1_reentrant();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
