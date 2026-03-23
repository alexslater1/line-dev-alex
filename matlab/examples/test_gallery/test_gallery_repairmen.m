% Test gallery_repairmen with SolverMVA
model = gallery_repairmen();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
