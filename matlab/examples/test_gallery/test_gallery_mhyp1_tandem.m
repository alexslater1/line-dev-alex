% Test gallery_mhyp1_tandem with SolverMVA
model = gallery_mhyp1_tandem();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
