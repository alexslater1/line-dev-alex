% Test gallery_coxm1 with SolverMVA
model = gallery_coxm1();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
