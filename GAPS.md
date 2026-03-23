# LINE Codebase: Incomplete Implementations and Draft Features

**Generated**: 2026-02-27
**Scope**: Comprehensive audit of draft/missing implementations across all language implementations (MATLAB, Java/Kotlin, Python native, Python wrapper)

---

## Summary Statistics

| Category | Count | Severity |
|----------|-------|----------|
| Python native stubs | 2 | Low (design pattern/constraint) |
| Python-wrapper gaps | 1 | Low (design pattern) |
| **TOTAL** | **~3** | **Low** |

---

## Python Native Stubs

### Base: to_java()
- **File**: `python/line_solver/lang/base.py` (line 318)
- **Type**: `NotImplementedError`
- **Issue**: Abstract base class stub; subclasses may override
- **Severity**: LOW (design pattern)

### LDES: Unsupported Distribution Fallback
- **File**: `python/line_solver/solvers/solver_des/distributions/factory.py` (line 512)
- **Type**: `NotImplementedError`
- **Issue**: Distribution factory raises error for distribution types without explicit construction logic
- **Severity**: LOW (fallback error for edge cases)

---

## Python Wrapper Gaps

### Scheduling: Abstract Base Class Methods
- **File**: `python-wrapper/line_solver/scheduling.py`
- **Type**: `NotImplementedError`
- **Methods** (3 locations):
  - `schedule()` (line 955)
  - `_getPriority()` (line 1267)
  - `_getPriority()` (line 1515)
- **Issue**: Abstract base class stubs; concrete subclasses must override
- **Severity**: LOW (design pattern)

---

## Non-Issues (Expected Design Choices)

### Sampling Not Supported for Analytical Solvers
- **MVA, MAM, FLD, NC, CTMC**: Cannot sample from analytical solutions
- **Files**: `solver_mva.py` (lines 6706-6790), `solver_mam/__init__.py` (lines 1148-1211), `solver_fld/__init__.py` (lines 2210-2237), `solver_nc.py` (lines 1758-1763), `solver_ctmc.py` (lines 2881-2892)
- **Status**: NOT A BUG — Expected behavior; use SSA or LDES for sampling
- **Count**: ~20 occurrences

### Distribution Base Class Methods
- **File**: `python/line_solver/distributions/base.py`
- **Methods**: `cdf()`, `pdf()`, `pmf()`, `sample()`, `get_D0()`, `get_D1()`, `get_mu()`, `get_phi()`, `get_pie()`, `get_num_phases()`
- **Status**: NOT A BUG — Abstract base class raises `NotImplementedError`; concrete subclasses override
- **Count**: ~10 occurrences

### Dense Matrix Rejects Sparse Internals
- **File**: `jar/src/main/kotlin/jline/util/matrix/DenseMatrix.java`
- **Status**: NOT A BUG — DenseMatrix correctly throws on sparse-only internal operations (column index arrays, non-zero length)
- **Count**: 4 occurrences

### MultivariateNormal CDF/LST
- **File**: `jar/src/main/kotlin/jline/lang/processes/MultivariateNormal.java`
- **Status**: NOT A BUG — Mathematically undefined for multivariate distributions

### MAM getProbMarg/getProb for Networks
- **Files**: `matlab/src/solvers/MAM/@SolverMAM/getProbMarg.m`, `getProb.m`, `jar/.../SolverMAM.java`
- **Status**: NOT A BUG — MAM uses QBD (quasi-birth-death) analysis which is fundamentally a single-queue method. Error messages recommend SolverCTMC or SolverSSA for multi-queue networks.

### BALKING via setPatience
- **Files**: `jar/.../JobClass.java` (line 313), `jar/.../Station.java` (line 634)
- **Status**: NOT A BUG — Timer-based BALKING intentionally unsupported. Error messages direct users to `setBalking()` API for state-based balking, which is fully implemented.

### JMT Event Processing
- **File**: `jar/.../SolverJMT.java` (line 3320)
- **Status**: NOT A BUG — Architectural limitation; JMT is an external simulator that does not expose event-level data through its results interface.

### JMT getTranProbAggr
- **File**: `jar/.../SolverJMT.java` (line 3665)
- **Status**: NOT A BUG — Transient probability aggregation not available for SolverJMT. This limitation also applies to the MATLAB implementation.

### Python Native JVM Independence
- `to_java()` stubs in Python native are expected — Python native must not depend on JVM
- **Status**: NOT A BUG — Design constraint documented in CLAUDE.md. Error messages now clearly explain this.

---

## Cross-Codebase Consistency Notes

### File I/O Intentionally Different
- **MATLAB**: Has QN2LQN, JMT2LINE, JMVA2LINE, JSIM2LINE
- **JAR**: Does not implement these transformations (intentional)
- **Python**: Does not implement these transformations (intentional)
- **Status**: NOT A BUG — Architecturally separate concerns

### Sampling Restrictions Consistent
- All analytical solvers (MVA, MAM, FLD, NC) prohibit sampling
- Only stochastic solvers (SSA, LDES, JMT) support sampling
- **Status**: Consistent design across all implementations

---

## Recently Resolved (Since 2026-02-24)

The following items from the previous audit have been resolved:

- **JAR CTMC QRF Analyzer** — Ported MATLAB `solver_ctmc_qrf_analyzer.m` to JAR (`Solver_ctmc_qrf_analyzer.kt`); dispatches qrf.mmi, qrf.mem, qrf.mmi.ld, qrf.mmi.linear, qrf.bas, qrf.rsrd methods
- **Matrix.java Dense Result Handling** — Added DenseMatrix branches for `countEachRow()` and `findNonNegative()`
- **Python JMT getCdfRespT** — Removed unnecessary `hasattr(self.model, 'copy')` guard; `Network.copy()` is fully implemented
- **Python Network.refresh_struct()** — Removed misleading `hard` parameter and TODO comment; always rebuilds from scratch
- **Python Network.to_java() / ServerType.to_java()** — Updated error messages to explain JVM independence design constraint
- **MAM getProbMarg/getProb error messages** — Updated in MATLAB and JAR to explain QBD solver limitation and recommend alternatives
- **JMT event processing / getTranProbAggr** — Improved error messages to clarify architectural limitations
- **JAR `getStageTable()`** — Now delegates to `getAvgTable()` (no longer throws)
- **JAR Fluid Solver Arrival Rate Bug** — Fixed; `snGetArvRFromTput` now functional
- **MATLAB MVA `getProbMarg/getProbAggr/getProbSysAggr` for open classes** — All implemented (Poisson/geometric product-form)
- **MATLAB FLD `getProbAggr` for open classes** — Implemented
- **Python Wrapper BUTools** — All 4 functions implemented (`phToMe`, `meToPh`, `phToDph`, `dphToPh`)
- **Python MAT2LINE** — Full `scipy.io.loadmat` implementation
- **JAR JSIM/JMT/JMVA loading** — `M2M.JSIM2LINE` and `M2M.JMVA2LINE` implemented
- **MATLAB MAM `solver_mam_basic` TODO** — Resolved
- **MATLAB LN join times** — Join time computation implemented in `updateMetricsDefault.m`
- **MATLAB QNA Fork Node Support** — Fork handling code exists
- **MATLAB AMVA Multi-Server DPS** — Implemented
- **MATLAB MAM BMAP/BMAP/1** — `solveBMAPBMAP1` implemented
- **MATLAB AUTO `chooseSolverHeur` LayeredNetwork** — Rewritten to support SolverLN for all method types
- **JAR Fluid: All TODOs** — ClosingAndStateDepMethodsODE, PassageTimeODE, SolverFluid options, ClosingAndStateDepMethodsAnalyzer, PStarSearcher — all resolved
- **JAR LDES: Solver_ssj distribution TODOs** — Resolved
- **JAR MAM: Router handling, DelayOff, Me_sample TODOs** — All resolved
- **JAR MG1_pi, MG1_EG TODOs** — Resolved
- **JAR JMT SaveHandlers TODOs** — Resolved
- **JAR FJConvert (4 TODOs), FJValidation TODO** — All resolved
- **JAR RAP fitting, MarkedMarkovProcess fromSampleSysAggr** — Resolved
- **Python LN: Weak connectivity, DelayOff, Loop detection** — All implemented
- **Python CTMC handler immediate state** — Resolved
- **Python retrial solver** — Resolved
- **Python deaggregate Cchain input** — Resolved
- **Python-wrapper `_lang.py` NodeParam, `solvers.py`** — Resolved
- **MATLAB JMT2LINE, JMVA2LINE, QN2LQN TODOs** — Resolved
- **MATLAB lyap.m, mamap2m_fit.m TODOs** — Resolved
- **JAR MarkovProcess.fromSampleSysAggr** — Now accepts `Ret.SampleResult` with multi-node state trajectories (Cartesian product, unique state enumeration, hold-time-based rate estimation), matching MATLAB
- **MATLAB JSIM2LINE distribution fallback** — Unrecognized arrival/service distributions now attempt APH moment-matching (mean/SCV from JMT parameters) instead of falling back to `Exp(1)`
- **MATLAB MAMAP22 hypoexponential conversion** — Canonical PH case now detects hypoexponential (SCV ≤ 1) and converts to non-canonical form via `aph2_fit_map()` before fitting
- **Python-wrapper MAPQN NLP solver** — Bridged via JPype `@JImplements('kotlin.jvm.functions.Function1')`, allowing Python objective functions to be passed to Java Augmented Lagrangian + BOBYQA solver
- **JAR CTMC Cache SSA test** — Added CTMC and SSA tests for `tut06_cache_lru_zipf` model in `SolverCTMCTest.java`

---

## Related Documentation

- See `CLAUDE.md` for implementation constraints (Java 8 compatibility, Python no-JVM requirement)
- See memory files for recent fixes to similar issues
- Parity test suite (`parity/test_example.sh`) can help validate implementations
