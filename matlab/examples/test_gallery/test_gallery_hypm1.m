% Test gallery_hypm1 with SolverMVA
model = gallery_hypm1();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
