% Test gallery_mm1_prio with SolverMVA
model = gallery_mm1_prio();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
