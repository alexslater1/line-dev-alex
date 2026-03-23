#!/bin/bash
# Add Kotlin-style aliases to api_core.dox

echo "Adding Kotlin-style method aliases to api_core.dox..."

# Create a backup
cp api_core.dox api_core.dox.before_aliases

# LayeredNetwork aliases - add after getUsedLangFeatures
sed -i '/<tr><td><code>ulf = getUsedLangFeatures(model)<\/code><\/td><td>Get used language features per layer<\/td><\/tr>/a\
<tr><td colspan="2"><b>Kotlin-Style Aliases</b></td></tr>\
<tr><td><code>idx = nodeIndex(model, node)</code></td><td>Alias for getNodeIndex</td></tr>\
<tr><td><code>node = nodeByName(model, name)</code></td><td>Alias for getNodeByName</td></tr>\
<tr><td><code>names = nodeNames(model)</code></td><td>Alias for getNodeNames</td></tr>\
<tr><td><code>lqn = struct(model)</code></td><td>Alias for getStruct</td></tr>\
<tr><td><code>E = numberOfLayers(model)</code></td><td>Alias for getNumberOfLayers</td></tr>\
<tr><td><code>E = numberOfModels(model)</code></td><td>Alias for getNumberOfModels</td></tr>\
<tr><td><code>layers = layers(model)</code></td><td>Alias for getLayers</td></tr>\
<tr><td><code>ulf = usedLangFeatures(model)</code></td><td>Alias for getUsedLangFeatures</td></tr>' api_core.dox

# NetworkSolver aliases - add after the existing common solver methods table, before "Solver Options"
# Find the line with "Solver Options" and add aliases before it
sed -i '/<b>Solver Options<\/b>/i\
<tr><td colspan="2"><b>Kotlin-Style Aliases (for get* methods)</b></td></tr>\
<tr><td><code>AvgTable = avgTable(solver)</code></td><td>Alias for getAvgTable</td></tr>\
<tr><td><code>AvgTable = avgSysTable(solver)</code></td><td>Alias for getAvgSysTable</td></tr>\
<tr><td><code>AvgTable = avgNodeTable(solver)</code></td><td>Alias for getAvgNodeTable</td></tr>\
<tr><td><code>AvgTable = avgChainTable(solver)</code></td><td>Alias for getAvgChainTable</td></tr>\
<tr><td><code>[QNc,UNc,RNc,TNc] = avgChain(solver)</code></td><td>Alias for getAvgChain</td></tr>\
<tr><td><code>[CNc,XNc] = avgSys(solver)</code></td><td>Alias for getAvgSys</td></tr>\
<tr><td><code>[QN,UN,RN,TN,AN,WN] = avgNode(solver)</code></td><td>Alias for getAvgNode</td></tr>\
<tr><td><code>RN = avgSysRespT(solver)</code></td><td>Alias for getAvgSysRespT</td></tr>\
<tr><td><code>TN = avgSysTput(solver)</code></td><td>Alias for getAvgSysTput</td></tr>\
<tr><td><code>AN = avgArvRChain(solver)</code></td><td>Alias for getAvgArvRChain</td></tr>\
<tr><td><code>QN = avgQLenChain(solver)</code></td><td>Alias for getAvgQLenChain</td></tr>\
<tr><td><code>UN = avgUtilChain(solver)</code></td><td>Alias for getAvgUtilChain</td></tr>\
<tr><td><code>RN = avgRespTChain(solver)</code></td><td>Alias for getAvgRespTChain</td></tr>\
<tr><td><code>TN = avgTputChain(solver)</code></td><td>Alias for getAvgTputChain</td></tr>\
<tr><td><code>QN = avgQLen(solver)</code></td><td>Alias for getAvgQLen</td></tr>\
<tr><td><code>UN = avgUtil(solver)</code></td><td>Alias for getAvgUtil</td></tr>\
<tr><td><code>RN = avgRespT(solver)</code></td><td>Alias for getAvgRespT</td></tr>\
<tr><td><code>TN = avgTput(solver)</code></td><td>Alias for getAvgTput</td></tr>\
<tr><td><code>AN = avgArvR(solver)</code></td><td>Alias for getAvgArvR</td></tr>\
<tr><td><code>[QNt,UNt,TNt] = tranAvg(solver, Qt,Ut,Tt)</code></td><td>Alias for getTranAvg</td></tr>\
<tr><td><code>RD = cdfRespT(solver, R)</code></td><td>Alias for getCdfRespT</td></tr>\
<tr><td><code>Pnir = prob(solver, node, state)</code></td><td>Alias for getProb</td></tr>\
<tr><td><code>Pnir = probAggr(solver, node, state_a)</code></td><td>Alias for getProbAggr</td></tr>\
<tr><td><code>Pn = probSys(solver)</code></td><td>Alias for getProbSys</td></tr>\
<tr><td><code>Pn = probSysAggr(solver)</code></td><td>Alias for getProbSysAggr</td></tr>\
<tr><td><code>lNormConst = probNormConstAggr(solver)</code></td><td>Alias for getProbNormConstAggr</td></tr>\
<tr><td><code>[Q,U,R,T,A,W] = avgHandles(solver)</code></td><td>Alias for getAvgHandles</td></tr>\
<tr><td><code>[Qt,Ut,Tt] = tranHandles(solver)</code></td><td>Alias for getTranHandles</td></tr>' api_core.dox

# CTMC aliases - add after getMarkedCTMC method
sed -i '/<tr><td><code>MCTMC = getMarkedCTMC(solver, options)<\/code><\/td><td>Get marked CTMC object<\/td><\/tr>/a\
<tr><td colspan="2"><b>Kotlin-Style Aliases</b></td></tr>\
<tr><td><code>SS = stateSpace(solver, options)</code></td><td>Alias for getStateSpace</td></tr>\
<tr><td><code>Q = generator(solver, options)</code></td><td>Alias for getGenerator</td></tr>' api_core.dox

# Add note about EnsembleSolver aliases in LN and ENV sections
sed -i '/<tr><td><code>svcmatrix = getEntryServiceMatrix(solver)<\/code><\/td><td>Get entry service matrix<\/td><\/tr>/a\
<tr><td colspan="2"><b>Kotlin-Style Aliases (from EnsembleSolver)</b></td></tr>\
<tr><td><code>s = solver(obj, e)</code></td><td>Alias for getSolver - get solver for layer e</td></tr>\
<tr><td><code>it = iteration(obj)</code></td><td>Alias for getIteration - get current iteration</td></tr>\
<tr><td><code>E = numberOfModels(obj)</code></td><td>Alias for getNumberOfModels</td></tr>\
<tr><td><code>avg = ensembleAvg(obj)</code></td><td>Alias for getEnsembleAvg</td></tr>\
<tr><td><code>tables = ensembleAvgTables(obj)</code></td><td>Alias for getEnsembleAvgTables</td></tr>' api_core.dox

sed -i '/<tr><td><code>name = getName(solver)<\/code><\/td><td>Get solver name<\/td><\/tr>/a\
<tr><td colspan="2"><b>Kotlin-Style Aliases (from EnsembleSolver)</b></td></tr>\
<tr><td><code>s = solver(obj, e)</code></td><td>Alias for getSolver - get solver for stage e</td></tr>\
<tr><td><code>it = iteration(obj)</code></td><td>Alias for getIteration - get current iteration</td></tr>\
<tr><td><code>E = numberOfModels(obj)</code></td><td>Alias for getNumberOfModels</td></tr>\
<tr><td><code>avg = ensembleAvg(obj)</code></td><td>Alias for getEnsembleAvg</td></tr>\
<tr><td><code>tables = ensembleAvgTables(obj)</code></td><td>Alias for getEnsembleAvgTables</td></tr>' api_core.dox

echo ""
echo "Kotlin-style aliases added successfully!"
echo ""
echo "Summary:"
echo "  - LayeredNetwork: 8 aliases added"
echo "  - NetworkSolver: 27 common aliases added"
echo "  - CTMC: 2 aliases added"
echo "  - LN: 5 EnsembleSolver aliases added"
echo "  - ENV: 5 EnsembleSolver aliases added"
echo ""
echo "Total: 47 Kotlin-style aliases documented"
