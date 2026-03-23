% Test gallery_hyperl1_feedback with SolverMVA
model = gallery_hyperl1_feedback();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
