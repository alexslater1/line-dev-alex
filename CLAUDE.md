# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## About LINE Solver

LINE is an open source package for analyzing queueing models via analytical methods and simulation. It supports:
- Queueing systems (M/M/1, M/M/k, M/G/1, etc.)
- Open and closed queueing networks
- Layered queueing networks
- Queueing networks in random environments

The repository is multi-language with implementations in:
- **MATLAB** (stable version) - Main distribution
- **Java/Kotlin** (beta version) - JAR-based API used by MATLAB and Python wrapper
- **Python Wrapper** (beta version) - Python wrapper using JAR via JPype
- **Python** (alpha version) - Native Python implementation (no JPype/JVM dependency)

## Repository Structure

### Core Components
- `jar/` - Java/Kotlin implementation (JAR API)
- `matlab/` - MATLAB implementation (main/stable)
- `python/` - Native Python implementation (alpha) - **must not use JPype or JVM**
- `python-wrapper/` - Python wrapper using JAR via JPype (alpha)
- `doc/` - Documentation (LINE-java.pdf, LINE-matlab.pdf, LINE-python.pdf)
- `doc/latex/` - LaTeX source files for manual generation
- `doc/web/` - Website source files

### Key Directories
- `matlab/src/solvers/` - Contains all solver implementations (MVA, JMT, SSA, CTMC, Fluid, etc.)
- `matlab/examples/` - Extensive collection of example models
- `matlab/examples/gettingstarted/` - Getting started examples
- `jar/src/main/kotlin/` - Core Kotlin/Java solver implementations
- `python/` - Native Python implementation (no JVM dependencies)
- `python-wrapper/` - JAR wrapper implementation using JPype

## Build Commands

### Java/Kotlin (jar/)

**Note**: Maven builds default to `/tmp/target/` to prevent conflicts across multiple worktrees. Use `-Dtmp=false` to build locally in `jar/target/` if needed.

```bash
# Compile (from jar/ directory)
tsp /usr/bin/mvn compile

# Default JAR (Java SE 17+)
tsp /usr/bin/mvn clean package && cp /tmp/target/jline-*.jar ../common/jline.jar

# JAR with bundled dependencies (Java SE 8 compatible for MATLAB compatibility)
tsp /usr/bin/mvn clean package -P b && cp /tmp/target/jline-*.jar ../common/jline.jar

# Build Maven dependency (also builds standalone LDES JAR)
tsp /usr/bin/mvn clean deploy -P jar-mvn

# Standalone LDES JAR (lightweight, excludes non-LDES deps)
tsp /usr/bin/mvn clean package -P ldes
# Output: ../common/ldes.jar
# LDES releases (JAR + native binary) are built from ldes-dev.git, not here
# See: ../ldes-dev.git/upload.sh

# Generate API documentation (using Dokka for Kotlin/Java interop)
mvn dokka:dokka
# or mvn dokka:javadoc for Javadoc-style output
```

### Python (Native)
Native Python implementation with no JVM dependencies:
```bash
# From python/ directory
python3 mm1.py  # Run basic example
```

### Python Wrapper
JAR wrapper using JPype (requires JVM):
```bash
# From python-wrapper/ directory
python3 mm1.py  # Run basic example
```

### MATLAB
```matlab
% Start LINE (required each session)
lineStart
```

## Documentation Generation

**When the user refers to "docs"**, update the following artifacts:
- `doc/html/index.html` - Doxygen-generated HTML documentation
- `doc/cheatsheet/` - Quick reference cheatsheet
- `doc/latex/manual.tex` - LaTeX manual source
- If coherent with their purpose: Sphinx and Doxygen configuration files

### Documentation Scripts (in doc/ directory)

#### generate-javadoc.sh
Generates API documentation for Java/Kotlin code using Dokka:
```bash
cd doc
./generate-javadoc.sh
# Output: doc/javadoc/index.html
```
- Uses Dokka to properly document both Kotlin and Java classes
- Generates Javadoc-style HTML documentation
- Includes all API packages (pfqn, npfqn, cache, etc.)

#### generate-doxygen.sh
Generates unified documentation for MATLAB, Java/Kotlin, and Python:
```bash
cd doc
./generate-doxygen.sh
# Output: doc/html/index.html
```
- Comprehensive documentation including source code browsing
- UML-style class diagrams and call graphs
- Tree view navigation sidebar
- Best for MATLAB code documentation
- Uses kotlin-doxygen-filter.py to parse Kotlin files (limited support)

#### generate-manuals.sh
Compiles LaTeX documentation into PDF manuals:
```bash
cd doc
./generate-manuals.sh
# Output: LINE-java.pdf, LINE-kotlin.pdf, LINE-matlab.pdf, LINE-python.pdf
```
- Compiles all four language-specific manuals
- Includes bibliography and index generation
- Cleans up intermediate LaTeX files

## Testing

### Java
```bash
# Run tests (from jar/ directory)
tsp /usr/bin/mvn test -DskipTests=false
```

**Note**: Tests are skipped by default (`skipTests=true`), so `mvn package` builds without running tests. Use `-DskipTests=false` to explicitly run tests. The `tsp` command queues test runs through task-spooler to ensure only one test execution runs at a time.

### Python (Native)
Test files in `python/tests/`:
- Native Python implementation tests (no JVM dependency)

### Python Wrapper
Test files in `python-wrapper/tests/`:
- `test_allExamples.py`
- `test_allGettingStarted.py`
- `test_api_native.py`
- `test_api_wrapper.py`

### MATLAB
- Various test files in line-test.git

**CRITICAL TEST RULES**: 
- Whenever MATLAB is available on bash, go for running it via bash
- Never modify expected values in test assertions (assertEquals, assertThat, etc.) unless explicitly approved by the user. Test assertions define the expected behavior and changing them masks bugs instead of fixing them. If tests fail, fix the implementation, not the test expectations.

## Solver Architecture

LINE implements multiple solver algorithms:

### Available Solvers
- **MVA** (Mean Value Analysis) - `matlab/src/solvers/MVA/`
- **JMT** (Java Modelling Tools) - `matlab/src/solvers/JMT/`
- **SSA** (Stochastic State-space Analysis) - `matlab/src/solvers/SSA/`
- **LDES** (LINE Discrete Event Simulator) - `jar/src/main/kotlin/jline/solvers/ldes/` (JAR backend using SSJ library, called via subprocess from MATLAB and Python)
- **CTMC** (Continuous Time Markov Chain) - `matlab/src/solvers/CTMC/`
- **Fluid** - `matlab/src/solvers/Fluid/`
- **MAM** (Matrix Analytic Methods) - `matlab/src/solvers/MAM/`
- **NC** (Normalizing Constant) - `matlab/src/solvers/NC/`
- **AUTO** - Automatic solver selection - `matlab/src/solvers/AUTO/`

### Solver Pattern
Each solver follows a consistent pattern:
- `@Solver[Name]/Solver[Name].m` - Main solver class
- `solver_[name].m` - Core solver logic
- `solver_[name]_analyzer.m` - Analysis implementation
- `runAnalyzer.m` - Standard analyzer interface

## Key Classes and Architecture

### MATLAB Core Classes
- `Network` - Main model container
- `Source`, `Queue`, `Sink` - Network nodes
- `OpenClass`, `ClosedClass` - Job classes
- Solver hierarchy with base `NetworkSolver` class

### JAR Implementation
- Kotlin/Java implementation in `jar/src/main/kotlin/jline/`
- Parallel structure to MATLAB with similar class hierarchy
- Used as backend by both MATLAB and Python versions
- Matrix class has extensive functionality - check existing methods before implementing new ones                                       
- **API Layer**: `jar/src/main/kotlin/jline/api/` contains Kotlin implementations of algorithms                                          
- Organized by domain: CACHE, PFQN, MAM, MC, SN, etc.                                                                                    
- Each domain contains multiple specialized algorithms                                                                                   
- **Core Java Classes**: `jar/src/main/kotlin/jline/lang/` contains Main model classes                                                            
- **Solvers**: `jar/src/main/kotlin/jline/solvers/` contains Solver implementations by type                                                       
- Documentation uses Dokka instead of standard javadoc due to Kotlin interop - run `mvn dokka:dokka` to generate API docs

### LDES Solver (JAR only)
The LDES (LINE Discrete Event Simulator) solver uses the SSJ library for simulation:
- **Location**: `jar/src/main/kotlin/jline/solvers/ldes/`
- **Supported features**: Open queueing networks with FCFS queues, Delay nodes, multiserver (M/M/c), and multiclass workloads
- **Distributions**: Exponential, Erlang, HyperExp, PH, APH, Coxian
- **Test validation**: Uses two-sample t-tests against SolverJMT at 1% significance level

### Implementation differences across codebases
- LDES uses the same JAR backend across all codebases (called via subprocess), so results should match exactly
- FLD uses different ODE solvers in matlab, JAR and python, so slight numerical deviations are possible
- JMT must always match across the codebases for fixed assignmetns of the samples and seed options

## NetworkStruct Reference

The `NetworkStruct` class describes an extended multiclass queueing network with class-switching and acyclic phase-type (APH) service times. Obtain via `model.getStruct()`.

**Index notation**: `i,j` = stations; `r,s` = classes; `c` = chain; `k` = phase

### NetworkStruct Static Properties (MATLAB)

| Field | Dimensions | Type | Description |
|-------|------------|------|-------------|
| `nstations` | scalar | integer | Number of stations |
| `nstateful` | scalar | integer | Number of stateful nodes |
| `nnodes` | scalar | integer | Number of nodes |
| `nclasses` | scalar | integer | Number of classes |
| `nclosedjobs` | scalar | integer | Total jobs in closed classes |
| `nchains` | scalar | integer | Number of chains |
| `nservers(i)` | (nstations,1) | integer | Number of servers at station i |
| `njobs(r)` | (1,nclasses) | integer | Jobs in class r (Inf for open) |
| `classprio(r)` | (1,nclasses) | integer | Priority of class r (0=highest) |
| `connmatrix(i,j)` | (nnodes,nnodes) | logical | True if node i routes to node j |
| `isstation(i)` | (nnodes,1) | logical | True if node i is a station |
| `isstateful(i)` | (nnodes,1) | logical | True if node i is stateful |
| `isstatedep(i,s)` | (nnodes,3) | logical | State-dependent section (s=1:input, 2:service, 3:routing) |
| `isslc(r)` | (1,nclasses) | logical | True if class r is self-looping |
| `nodetype{i}` | (nnodes,1) | integer | NodeType id for node i |
| `nodenames{i}` | (nnodes,1) | string | Name of node i |
| `classnames{r}` | (1,nclasses) | char | Name of class r |
| `sched(i)` | (nstations,1) | integer | SchedStrategy id at station i |
| `schedparam(i,r)` | (nstations,nclasses) | double | Scheduling parameter for class r at station i |
| `phases(i,r)` | (nstations,nclasses) | integer | Number of service phases for class r at station i |
| `phasessz(i,r)` | (nstations,nclasses) | integer | State vector elements for phase |
| `phaseshift(i,r)` | (nstations,nclasses) | integer | Position shift to read phase in state |
| `mu{i,r}(k)` | cell(nstations,nclasses) | double | Service rate in phase k (NaN=Disabled, 1e7=Immediate) |
| `phi{i,r}(k)` | cell(nstations,nclasses) | double | Completion probability in phase k |
| `pie{i,r}(k)` | cell(nstations,nclasses) | double | Entry probability in phase k |
| `proc{i,r}` | cell(nstations,nclasses) | cell | (D0,D1) MAP representation of service process |
| `procid(i,r)` | (nstations,nclasses) | integer | ProcessType id for service/arrival |
| `lst{i,r}` | cell(nstations,nclasses) | function | Laplace-Stieltjes transform |
| `routing(i,r)` | (nnodes,nclasses) | integer | RoutingStrategy id upon departing node i |
| `rtorig{r,s}` | cell(nclasses,nclasses) | double | User-specified class-switch probability matrix |
| `refstat(r)` | (1,nclasses) | integer | Reference station index for class r |
| `refclass(c)` | (1,nchains) | integer | Reference class index for chain c |
| `inchain{c}` | cell(nchains,1) | integer | Class indexes in chain c |
| `droprule(i,r)` | (nstations,nclasses) | integer | Drop rule for class r at station i |
| `nvars(i,r)` | (nstateful,2*nclasses+) | integer | Local state variables for stateful node i |
| `nodeparam{i}` | cell(nstateful,1) | struct | Parameters for stateful node i |
| `stateprior{i}` | cell(nstateful,1) | double | Prior probability for states of node i |
| `sync{s}` | cell | struct | Synchronization data structure |
| `gsync{s}` | cell | struct | Global synchronization data structure |
| `fj(f,j)` | (nforks,nnodes) | logical | True if fork f joins at node j |
| `lldscaling(i,n)` | varies | double | Load-dependent scaling for n jobs at station i |
| `ljdscaling{i}` | cell | cell | Limited joint-dependent scaling (linearized per station) |
| `ljdcutoffs(i,r)` | (nstations,nclasses) | double | Per-class cutoffs for joint dependence |
| `cdscaling{n_ir}` | cell | double | Class-dependent scaling |
| `nregions` | scalar | integer | Number of finite capacity regions |
| `region{f}(i,r)` | cell | double | Per-class capacity in region f |
| `regionrule(f)` | (nregions,1) | integer | DropStrategy id for region f |
| `regionweight(f,r)` | (nregions,nclasses) | double | Class weight for class r in region f |
| `regionsz(f,r)` | (nregions,nclasses) | double | Class size/memory for class r in region f |
| `reward{n}` | cell | struct | Reward function definitions for CTMC (with .name, .fn, .type fields) |
| `classdeadline(r)` | (1,nclasses) | double | Deadline for class r (Inf = no deadline) |
| `immfeed(i,r)` | (nstations,nclasses) | logical | True if class r uses immediate feedback at station i |
| `issignal(r)` | (1,nclasses) | logical | True if class r is a signal class |
| `signaltype{r}` | cell(nclasses,1) | integer | Signal type for class r (NaN for non-signals) |
| `syncreply(r)` | (1,nclasses) | integer | Reply signal class index for class r (-1 if no reply) |
| `signalRemovalDist{r}` | cell(nclasses,1) | struct | Removal distribution for signal class r |
| `signalRemovalPolicy(r)` | (1,nclasses) | integer | Removal policy id for signal class r |
| `isCatastrophe(r)` | (1,nclasses) | logical | True if class r is a catastrophe signal |
| `nservertypes(i)` | (nstations,1) | integer | Number of server types at station i (0=homogeneous) |
| `servertypenames{i}{t}` | cell | char | Name of server type t at station i |
| `serverspertype{i}(t)` | cell | integer | Number of servers of type t at station i |
| `servercompat{i}(t,r)` | cell | logical | True if server type t can serve class r at station i |
| `heterorates{i}{t,r}` | cell | double | Service rate for server type t and class r at station i |
| `heteroproc{i}{t,r}` | cell | cell | Service process matrices for server type t and class r |
| `heteroprocid(i,t,r)` | varies | integer | Service process type id for server type t and class r |
| `heteroschedpolicy(i)` | (nstations,1) | integer | Heterogeneous scheduling policy id at station i |

### NetworkStruct Computed Properties (MATLAB)

| Field | Dimensions | Type | Description |
|-------|------------|------|-------------|
| `cap(i)` | (nstations,1) | integer | Total capacity at station i |
| `classcap(i,r)` | (nstations,nclasses) | integer | Buffer capacity for class r at station i |
| `chains(c,r)` | (nchains,nclasses) | logical | True if class r is in chain c |
| `csmask(r,s)` | (nclasses,nclasses) | logical | True if class r can switch to s |
| `rates(i,r)` | (nstations,nclasses) | double | Service/arrival rate for class r at station i |
| `scv(i,r)` | (nstations,nclasses) | double | Squared coefficient of variation |
| `visits{c}(i,r)` | cell(nchains) | double | Visits by chain c to stateful node i in class r |
| `nodevisits{c}(i,r)` | cell(nchains) | double | Visits by chain c to node i in class r |
| `rt(idx_ir,idx_js)` | (nstateful*nclasses, nstateful*nclasses) | double | Routing probability from (i,r) to (j,s), idx=(i-1)*nclasses+r |
| `rtnodes(idx_ir,idx_js)` | (nnodes*nclasses, nnodes*nclasses) | double | Same as rt but for all nodes |
| `rtfun(st1,st2)` | function | handle | State-dependent routing given initial/final states |
| `state{i}` | cell(nstateful,1) | integer | Current state of stateful node i |
| `space{t}` | cell | integer | State space (or portion thereof) |

### Index Conversion Methods

`NetworkStruct` provides methods to convert between node, station, and stateful node indices:
- `nodeToStation(i)` - Node index to station index (-1 if not a station)
- `nodeToStateful(i)` - Node index to stateful index (-1 if not stateful)
- `stationToNode(i)` - Station index to node index
- `stationToStateful(i)` - Station index to stateful index
- `statefulToNode(i)` - Stateful index to node index
- `statefulToStation(i)` - Stateful index to station index (-1 if not a station)

## LayeredNetworkStruct Reference

The `LayeredNetworkStruct` class describes a layered queueing network. Obtain via `model.getStruct()` on a `LayeredNetwork`.

**Index notation**: `idx` = absolute index in 1...nidx; `hidx/tidx/eidx/aidx/cidx` = host/task/entry/activity/call absolute indices

### Absolute Index System

Elements are indexed absolutely using shift values:
- Host h: absolute index = `h + hshift`
- Task t: absolute index = `t + tshift`
- Entry e: absolute index = `e + eshift`
- Activity a: absolute index = `a + ashift`
- Call c: absolute index = `c + cshift`

### LayeredNetworkStruct Static Properties (MATLAB)

| Field | Dimensions | Type | Description |
|-------|------------|------|-------------|
| `nidx` | scalar | integer | Total number of LQN elements |
| `nhosts` | scalar | integer | Number of hosts/processors |
| `ntasks` | scalar | integer | Number of tasks |
| `nentries` | scalar | integer | Number of entries |
| `nacts` | scalar | integer | Number of activities |
| `ncalls` | scalar | integer | Number of calls |
| `hshift` | scalar | integer | Shift to convert host index to absolute |
| `tshift` | scalar | integer | Shift to convert task index to absolute |
| `eshift` | scalar | integer | Shift to convert entry index to absolute |
| `ashift` | scalar | integer | Shift to convert activity index to absolute |
| `cshift` | scalar | integer | Shift to convert call index to absolute |
| `names{idx}` | cell(nidx) | char | Name of element |
| `hashnames{idx}` | cell(nidx) | char | Name with type prefix (H:/R:/T:/C:/E:/I:/A:) |
| `type{idx}` | (nidx,1) | integer | LayeredNetworkElement type id |
| `parent(idx)` | (nidx,1) | integer | Absolute index of parent element |

**Hierarchy mappings:**
| Field | Type | Description |
|-------|------|-------------|
| `tasksof{hidx}` | cell | Task absolute indexes on host hidx |
| `entriesof{tidx}` | cell | Entry absolute indexes on task tidx |
| `actsof{eidx}` or `actsof{tidx}` | cell | Activity indexes reachable from entry/task |
| `callsof{aidx}` | cell | Call absolute indexes from activity aidx |

**Host/Task properties:**
| Field | Dimensions | Type | Description |
|-------|------------|------|-------------|
| `mult(idx)` | (nidx,1) | integer | Multiplicity for host or task |
| `repl(idx)` | (nidx,1) | integer | Replication factor |
| `sched(idx)` | (nidx,1) | integer | SchedStrategy id |
| `isref(tidx)` | (ntasks,1) | logical | True if reference task |
| `isfunction(tidx)` | (ntasks,1) | logical | True if function task |
| `iscache(tidx)` | (ntasks,1) | logical | True if CacheTask |

**Distribution fields** (for each of: `hostdem`, `think`, `actthink`, `setuptime`, `delayofftime`, `arrival`):
| Field Pattern | Type | Description |
|---------------|------|-------------|
| `*_type(idx)` | integer | ProcessType id |
| `*_params{idx}` | array | Distribution parameters |
| `*_mean(idx)` | double | Mean value |
| `*_scv(idx)` | double | Squared coefficient of variation |
| `*_proc{idx}` | cell | PH/MAP matrix representation |

**Call properties:**
| Field | Dimensions | Type | Description |
|-------|------------|------|-------------|
| `calltype(cidx)` | (ncalls,1) | integer | CallType id (sync/async) |
| `callpair(cidx,j)` | (ncalls,2) | integer | j=1: activity issuing call; j=2: entry called |
| `callnames{cidx}` | cell(ncalls) | char | Call name |
| `callhashnames{cidx}` | cell(ncalls) | char | Call name with type prefixes |
| `callproc_type(cidx)` | (ncalls,1) | integer | Call count distribution type |
| `callproc_mean(cidx)` | (ncalls,1) | double | Mean number of calls |

**Activity precedence:**
| Field | Dimensions | Type | Description |
|-------|------------|------|-------------|
| `actpretype(aidx)` | (nacts,1) | integer | ActivityPrecedenceType before activity |
| `actposttype(aidx)` | (nacts,1) | integer | ActivityPrecedenceType after activity |

**Graph structures:**
| Field | Dimensions | Type | Description |
|-------|------------|------|-------------|
| `graph(idx_i,idx_j)` | (nidx,nidx) | float | Adjacency: !=0 if i runs-on/calls/precedes j |
| `dag(idx_i,idx_j)` | (nidx,nidx) | float | DAG version (loops broken, entry-task edges flipped) |
| `replygraph(aidx,eidx)` | (nacts,nentries) | logical | True if activity replies ending entry call |
| `taskgraph(tidx_i,tidx_j)` | (ntasks,ntasks) | logical | True if task i calls task j |
| `iscaller(idx_i,idx_j)` | (nidx,nidx) | logical | True if element i calls element j |
| `issynccaller(idx_i,idx_j)` | (nidx,nidx) | logical | True if synchronous call |
| `isasynccaller(idx_i,idx_j)` | (nidx,nidx) | logical | True if asynchronous call |

**Cache properties (for CacheTask):**
| Field | Type | Description |
|-------|------|-------------|
| `nitems(idx)` | integer | Total items in CacheTask or ItemEntry |
| `itemcap{idx}(l)` | integer | Cache capacity for list l |
| `replacement(tidx)` | integer | ReplacementPolicy id |
| `itemproc_*` | various | Item popularity distribution fields |

## Common Development Tasks

### Adding New Examples
- MATLAB: Add to `matlab/examples/`
- Python (Native): Add to `python/examples/` (no JVM dependencies allowed)
- Python Wrapper: Add to `python-wrapper/examples/`
- Follow existing naming conventions (e.g., `example_[modeltype]_[number].m`)

### Working with Solvers
- Solver implementations are in language-specific solver directories
- Most solver logic is mirrored between MATLAB and JAR implementations
- JMT solver acts as a bridge to Java Modelling Tools

### JAR Updates
When modifying JAR code, the build process automatically copies the compiled JAR to:
- `common/jline.jar`

## Important Notes

- MATLAB version is the main/stable implementation. It should be used as a ground truth when unsure which method is giving correct results.
- JAR compilation supports both Java 8 (for MATLAB compatibility) and Java 17+
- Python (native) and Python wrapper are early releases with some incomplete functionality
- **Python native (`python/`) must never depend on JPype or JVM**
- **Python wrapper (`python-wrapper/`) uses JPype to interface with JAR**
- The codebase includes extensive examples - refer to them for usage patterns
- JMT solver requires the JMT.jar dependency for simulation capabilities
- Bug fixes in JAR must cross-check the MATLAB implementation to ensure parity

## Java 8 Compatibility Requirements

**CRITICAL**: All Java/Kotlin code modifications in the `jar/` directory MUST maintain Java 8 compatibility for MATLAB integration.

### Required Practices:
- **Always use** `Collectors.toList()` instead of `Stream.toList()` (Java 16+)
- **Never use** `var` keyword (Java 10+)
- **Never use** `List.of()`, `Set.of()`, `Map.of()` factory methods (Java 9+)
- **Never use** text blocks with `"""` (Java 15+)
- **Never use** switch expressions with `->` syntax (Java 14+)
- **Never use** record classes (Java 14+)
- **Never use** pattern matching in instanceof (Java 14+)
- **Never use** Java 9+ Optional methods: `.or()`, `.ifPresentOrElse()`, `.isEmpty()`
- **Never use** Java 11+ String methods: `.isBlank()`, `.strip()`, `.lines()`, `.repeat()`

### Build Profiles:
- Use `b` profile for production MATLAB/Python distribution
- Examples in `jar/src/main/kotlin/jline/examples/` are excluded from Java 8 builds
- Kotlin compiler is configured with `jvmTarget=1.8` for compatibility

## Python Implementation Guidelines

### Native Python (`python/`)
- **CRITICAL**: Must NOT use JPype, JVM, or any Java dependencies
- Pure Python implementation with standard Python libraries (numpy, scipy, etc.)
- No Matrix class from JAR - use numpy arrays or Python lists

### Python Wrapper (`python-wrapper/`)
Uses JPype to interface with JAR:
- **NEVER use Matrix class directly in examples or user-facing code**
- Use Python lists, numpy arrays, or other native Python data structures instead
- Internal usage of Matrix class in the wrapper implementation (e.g., `line_solver/lang.py`) for Java interoperability is acceptable
- Replace patterns like:
  - `Matrix.uniformDistribution(n)` → `[1.0/n] * n`
  - `Matrix.singleton(x)` → `[x]` or appropriate Python equivalent
  - `Matrix.zeros(m, n)` → `[[0] * n for _ in range(m)]` or numpy equivalent

## Development Guidelines

### Commit Message Format

Use the following format for all commits:

```
[language] [type]: [description]
```

**Language prefixes** (use first letters):
- `m` = MATLAB only
- `j` = Java/Kotlin only
- `p` = Python only
- Combinations: `m,j` `j,p` `m,j,p` for multi-language changes

**Type** (choose one):
- `feat:` - New feature
- `fix:` - Bug fix
- `refactor:` - Code restructuring without behavior change
- `perf:` - Performance improvement
- `docs:` - Documentation update

**Examples:**
- `j feat: add class priority support to LDES solver`
- `m,j fix: align capacity with Kendall notation`
- `p docs: update Python wrapper examples`

### Implementation Philosophy
- CRITICAL: Avoid at all costs simplified implementations (e.g., stubs, prototypes, partial implementations with a comment left as a placeholder, etc), they must be comprehensive and complete.

### Memory Flags

- It is absolutely forbidden to disable tests unless explicitly instructed to disable them
- Never changes tolerances in test assertions unless explicitly approved by the user
- For all bugs, fix the root cause - never add defensive workarounds (bounds checks, try-catch, null checks) that mask the underlying problem or produce incorrect data
- **Documentation modifications must be done using the Sonnet model** - If unable to switch automatically, stop and remind the user to switch to Sonnet before proceeding with documentation changes
- **DTMC solver order**: In `sn_refresh_visits` (both MATLAB and Python), `dtmc_solve` must be used as the primary solver with `dtmc_solve_reducible` as fallback. Never change this order - it affects visit ratio computations in fork-join models.

### Git Operations

- **ALWAYS ask for user confirmation before running `git commit` or `git stash`**
- **ALWAYS ask for user confirmation before running `git reset --hard`**
- **NEVER run `git checkout --`, `git restore`, or `git revert` on files that would overwrite changes without explicit user approval**
- Before committing, propose the commit message and wait for explicit user approval
- Never commit or stash changes without the user's explicit consent

## MATLAB Development Setup

### Running MATLAB code from matlab/ directory
The `dev/` directory has been renamed to `matlab/` and now contains the main MATLAB implementation. When executing MATLAB code in the `matlab/` directory, use these commands to set up the environment:

```matlab
cd 'matlab'
lineStart
```

These commands:
- Navigate to the matlab directory (which contains the main implementation)
- Start the LINE solver environment

This is the standard setup for running MATLAB examples and tests from the main MATLAB implementation directory.

