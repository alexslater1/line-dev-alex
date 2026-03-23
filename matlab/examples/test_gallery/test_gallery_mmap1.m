% Test gallery_mmap1 with SolverMVA
model = gallery_mmap1();
solver = SolverMVA(model);
avgTable = solver.getAvgTable();
fprintf('Model: %s\n', model.getName());
disp(avgTable);
