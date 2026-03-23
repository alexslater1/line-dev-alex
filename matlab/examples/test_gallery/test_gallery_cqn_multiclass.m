% Test gallery_cqn_multiclass with SolverMVA
model = gallery_cqn_multiclass();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
