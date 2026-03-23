% Test gallery_hyphyp1_tandem with SolverMVA
model = gallery_hyphyp1_tandem();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
