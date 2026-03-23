% Test gallery_parm1 with SolverMVA
model = gallery_parm1();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
