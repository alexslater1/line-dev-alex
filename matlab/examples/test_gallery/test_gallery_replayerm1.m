% Test gallery_replayerm1 with SolverMVA
model = gallery_replayerm1();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
