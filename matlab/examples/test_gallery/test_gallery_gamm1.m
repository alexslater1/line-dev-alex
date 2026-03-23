% Test gallery_gamm1 with SolverMVA
model = gallery_gamm1();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
