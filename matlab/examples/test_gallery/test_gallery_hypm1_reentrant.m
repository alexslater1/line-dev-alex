% Test gallery_hypm1_reentrant with SolverMVA
model = gallery_hypm1_reentrant();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
