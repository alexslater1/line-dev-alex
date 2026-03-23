% Test gallery_mm1_ps_feedback with SolverMVA
model = gallery_mm1_ps_feedback();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
