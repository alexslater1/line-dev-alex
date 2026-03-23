% Test gallery_merl1_tandem with SolverMVA
model = gallery_merl1_tandem();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
