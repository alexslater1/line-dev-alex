% Test gallery_mpar1 with SolverMVA
model = gallery_mpar1();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
