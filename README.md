# LN-Simple — a bottom-up Layered Queueing Network solver

**A final-year project (MEng Computing, Imperial College London) by Alex Slater.**

> This repository is a fork of **[LINE](https://github.com/imperial-qore/line-solver)**, the
> open-source queueing-network solver developed by the
> [QORE Lab](https://qore.doc.ic.ac.uk/) at Imperial College London (BSD-3-Clause).
> **LINE is not my work.** My contribution is a new solver, **`SolverLNSimple`** (referred to as
> *LN-Simple*), built *on top of* LINE between late March and mid-June 2026, together with its
> evaluation harness and a written dissertation. The original LINE documentation is preserved
> verbatim in **[README-LINE.md](README-LINE.md)**.

---

## What this is

Distributed, multi-tier software (microservices, client–server systems) is hard to reason about
for performance. Synchronous request–reply calls, limited server concurrency and shared hardware
cause *blocking*, *contention* and non-linear behaviour, so response time and throughput can't be
read off from the parts in isolation.

**Queueing network models** are a classical, analytical answer: model the system as a set of
service stations, each with a queue, and compute steady-state metrics — response time, throughput,
queue length, utilisation — *without* building or simulating the real system. That makes them cheap
enough to sweep thousands of design permutations early in development.

Ordinary queueing networks assume *product form* and *no blocking*. Synchronous remote calls break
both: while a client waits for a server, it holds *both* resources at once (simultaneous resource
possession). **Layered Queueing Networks (LQNs)** were introduced to handle exactly this — they
model software tasks and entries (with their limited concurrency and rendezvous calls) *layered
over* hardware processors, capturing the blocking and nested demands that arise in real multi-tier
systems.

The catch is that LQN solvers are complicated. The two established general-purpose tools —
**LQNS** and LINE's own **`SolverLN`** — have accreted machinery over years to cover every case
(interlock corrections, convergence floors, moving-average smoothing, surrogate delays, AMVA
variants). They are accurate and general, but large and hard to reason about.

**LN-Simple takes the opposite approach.** It is built *bottom-up* from the smallest LQN solver
that works at all, adding a piece of machinery only when a specific class of model forces it. The
result is a solver that is deliberately *not* general-purpose, but is smaller, easier to follow,
and — on the class of closed LQN models it targets — **converges in fewer iterations and less
wall-clock time than either established solver, while staying within standard accuracy tolerance.**
Because it omits machinery by construction, the project can then *measure* what that machinery in
the mature solvers actually buys.

### How the solver works, in one paragraph

LN-Simple is a **decomposition solver**. It factors an LQN into an *ensemble* of small
single-server closed product-form queueing networks — one "layer" per software task and one per
processor — using LINE's existing ensemble builder. Each layer is solved with Mean Value Analysis
(MVA); the layers are then *coupled* by feeding each layer's results into its neighbours as
think-times (`Clients` delays) and service demands, and the whole thing is iterated to a
fixed point where per-layer throughputs stop changing. The MVA numerics and the ensemble
construction come from LINE deliberately (the inner kernels are re-implemented in
primitive-array form for speed — see the development story — but stay numerically identical),
so that iteration counts can be compared *like-for-like* against `SolverLN` and LQNS;
**the outer coupling iteration is written from scratch**, and is where the project's
contribution lives.

It targets **closed** LQN models and handles single-class chains, multi-class shared-callee
topologies, multi-server processors, deep infinite-server topologies, fan-out callers,
multi-entry / reference-task models, and activity graphs (OR-fork branching, AND-fork/join with an
`E[max]` join delay, loops, and phase-1/phase-2 reply semantics). It deliberately does **not**
cover open-class arrivals, call forwarding, cache-modelling nodes, class-switching at iteration
time, or the finite-server AND-fork overlap case — these are documented as out-of-scope in the
dissertation.

---

## What in this repository is mine

Everything else in the tree is upstream LINE. My contribution is:

| Area | Path | What it is |
|------|------|------------|
| **The solver** | [jar/src/main/kotlin/jline/solvers/ln_simple/](jar/src/main/kotlin/jline/solvers/ln_simple/) | `SolverLNSimple` — the coupling iteration, ensemble initialiser, `LqnGraph` queries, the primitive-array MVA kernels ([mva/](jar/src/main/kotlin/jline/solvers/ln_simple/mva/)), and the results pipeline ([results/](jar/src/main/kotlin/jline/solvers/ln_simple/results/)). See its own [technical README](jar/src/main/kotlin/jline/solvers/ln_simple/README.md). |
| **Evaluation harness** | [jar/src/test/java/jline/solvers/ln_simple/](jar/src/test/java/jline/solvers/ln_simple/) | The 78-fixture corpus ([fixtures/](jar/src/test/java/jline/solvers/ln_simple/fixtures/)), the correctness test that runs all three solvers ([`SolverLNSimpleEvaluationTest`](jar/src/test/java/jline/solvers/ln_simple/SolverLNSimpleEvaluationTest.java)), and the iteration/runtime capture harness ([`FastEval`](jar/src/test/java/jline/solvers/ln_simple/FastEval.java)). |
| **Modified LINE solver** | [jar/src/main/kotlin/jline/solvers/lqns/SolverLQNS.java](jar/src/main/kotlin/jline/solvers/lqns/SolverLQNS.java) | Small fixes needed to use LQNS as a fair comparator: carry LQNS's parsed iteration count through (it was dropped to `0`), skip serialization-only artifact activities emitted by `writeXML`, and make the `lqsim` block count configurable. |
| **Python fix** | [python/line_solver/layered.py](python/line_solver/layered.py) | A parent-index fix in the layered-network struct build (phase-2 reachable-activity walk). |
| **The dissertation** | [report/source/](report/source/) | The full write-up — [main.pdf](report/source/main.pdf) plus LaTeX sources for the [background](report/source/background/background.tex), [contribution](report/source/contribution/contribution.tex), [evaluation](report/source/evaluation/evaluation.tex) and [conclusion](report/source/conclusion/conclusion.tex). |
| **Data & figures** | [report/data/](report/data/), [report/source/figures/scripts/](report/source/figures/scripts/) | Raw evaluation results and the scripts that generate the report's figures. |
| **Notes** | [docs/ln_simple/](docs/ln_simple/) | Working notes on LINE internals and the interim report. |

---

## How it was built — the development story

The solver grew through six feature steps, each triggered by a concrete class of model the previous
design got wrong, followed by a performance-optimisation phase. This mirrors the real
commit history (`git log --author="Alex Slater" --oneline --reverse`).

1. **A minimal single-class solver + ensemble coupling.** The smallest thing that solves an LQN at
   all: decompose into the ensemble, wrap a coupling loop around the MVA kernel, and couple layers
   through the `T:`/`P:` queue-name prefixes that LINE's ensemble builder shares.
   *Commits: `simple single class solver` → `LQN single-class partially working`.*

2. **Graph-derived coupling.** Prefix-based coupling broke on deeper chains (immediate intermediate
   activities, zero-demand leaf subtrees, intermediate think-times). The fix reads coupling targets
   directly from the `LayeredNetwork` graph — root-reference walks, caller-chain demand sums,
   subtree checks — and adds multi-call support.
   *Commits: `fix depth and task multiplicity issues`, `add multi-call functionality`,
   `fix solver for models with service times at non-terminal activities`,
   `fix solver for models with zero-demand leafs/subtrees`, `handle intermediate task think times`.*

3. **Multi-class support.** When several reference tasks call a shared task, a single aggregate
   class cannot recover per-caller demands. The solver feeds true multi-class demands into MVA,
   caps per-layer populations, splits the overloaded `callMean` into throughput-scaling vs.
   queue-length-apportionment roles, and — for asymmetric demands — mutates the ensemble's class
   topology to add per-caller classes.
   *Commits: `initial multiclass implementation`, `add max multiplicity handling and fix bugs`,
   `rebuild layers to handle multiple asymmetric activities at a task`.*

4. **An MVA kernel for LQN-shaped inputs.** LINE's load-dependent kernel was quadratically slow on
   dense large-`N` access and numerically unstable on infinite-server topologies. The response:
   re-implement the LD recursion over primitive `double[]` arrays (`MvaLd`), add an AMVA path for
   large populations (`MvaAmva`, using the Seidmann/Rolia transform), and partition
   infinite-server stations out before dispatch. **This is the headline performance work** — an
   `N=200` model dropped from ~60 s to ~0.1 s per run.
   *Commits: `implement optimised mvald`, `add AMVA for large models`,
   `handle infinite server mvald instability`.*

5. **Fan-out callers.** When one caller makes synchronous calls to several distinct callees, the
   iteration locked into a two-state limit cycle and false-saturated. The fix adds
   time-at-siblings into the caller's think-time and a structural-predicate-gated under-relaxation
   (`Z_RELAX_ALPHA = 0.5`, on only when a caller genuinely fans out), plus multi-class×multi-entry
   refinements.
   *Commits: `add sibling-callee blocking and relaxation to fix fan-out models`,
   `improve host-layer infinite server handling`, `fix multi-class/entry task layer issues`.*

6. **Activity-graph awareness.** A flat "bag of activities" read of each entry ignored branch
   probabilities, loop counts, parallelism and the reply boundary. The fix does a per-activity
   visit-weight DAG walk over the activity precedences, an AND-fork `E[max]` join correction
   (the inclusion–exclusion expected-maximum of exponentials), and a phase-1/phase-2 split so a
   caller is charged only for work up to its reply.
   *Commits: `add activity graph support`, `add fork-join support`, `add phase-2 handling`.*

7. **Optimising the specialised solver.** With correctness in place, the solver exploits its
   non-generality: a limit-cycle detector in the outer loop (a false-saturating model that hit the
   100-iteration cap dropped to 6), memoisation of the invariant topology queries a JFR profile
   flagged as hotspots (a profile-driven ~2× speedup), and a re-tuned convergence rule (drop the
   demand-side damping, loosen the over-tight tolerance) — before migrating to the elevator sweep
   order the reference solvers use, so the final iteration comparison is like-for-like.
   *Commits: `add limit-cycle detection...`, `cache LqnGraph topological queries`,
   `cache per-network layer and class lookups...`, `drop INF-task under-relaxation`,
   `tighten the limit-cycle detection window`, `loosen convergence tolerance`,
   `switch to elevator sweep`.*

---

## Headline results

All figures below are from the dissertation's Evaluation chapter
([report/source/evaluation/evaluation.tex](report/source/evaluation/evaluation.tex)), measured on a
**78-fixture corpus** of closed LQN models against LINE's **`SolverLN`** and **LQNS V5**. The corpus
is split into Partition A (43 positive-evidence fixtures), Partition B (10 scale-axis fixtures) and
Partition C (25 deliberately adversarial boundary fixtures).

| Metric | LN-Simple | vs `SolverLN` | vs LQNS |
|--------|-----------|---------------|---------|
| **Total outer iterations** (corpus) | **539** | 4,835 → **9× fewer** | 985 → **1.8× fewer** |
| **Total wall-clock** (corpus) | **22,184 ms** | 2,917,118 ms → **131× faster** | 23,431 ms → **1.06× faster** (0.99× net of REST overhead) |
| **Correctness** | 71 / 78 pass within 5% | 57 fixtures match `SolverLN` | 14 more match LQNS where the two references disagree |

> The LQNS totals (985 iterations, 23,431 ms) are over the **77 of 78** fixtures it can
> solve; the one it cannot is an activity-graph fixture (`combinedDag`, a combined OR/AND-fork
> DAG). LN-Simple's totals are over all 78.

- **The 7 non-matching fixtures are all in the adversarial boundary partition** (saturated
  fan-outs at ρ≈0.99, AND-fork joins, and deep all-exponential chains) — cases built to push the
  closed-form kernels past where they are derived to hold. None fall in the positive-evidence set.
- **Where the iteration gap to `SolverLN` comes from:** disabling `SolverLN`'s convergence-control
  machinery closes **73.6%** of the gap (its `iter_min` floor pins 63 of 78 fixtures at exactly 53
  iterations); the remaining **26.4%** is genuine algorithmic difference. Removing that machinery
  moves 7 of `SolverLN`'s *own* outputs off the fixed point by >5%, quantifying what LN-Simple
  trades away for speed.
- **Where the runtime gap to `SolverLN` comes from:** its interlock correction is a pure
  per-iteration cost on this corpus (it never fires, since no fixture has a server shared by two
  distinct client tasks) — disabling it gives up to a **2.77×** per-iteration speedup on the widest
  fan-out, with zero change to iteration count or accuracy.

The comparison against LQNS (a mature C++ solver, reached over a REST call) is essentially even on
wall-clock despite LN-Simple being a JVM prototype, and LN-Simple uses far fewer iterations.

---

## Building and running

**Requirements:** Java SE 8+ and Maven. The `SolverLN` comparison is self-contained. The **LQNS**
comparison additionally needs an LQNS instance reachable by `SolverLQNS`, which the harness invokes
over a REST call — start one locally with Docker:

```bash
sudo docker run -d -p 8080:8080 imperialqore/lqns-rest:latest
```

Without it, only the LQNS side of the comparison is unavailable; the `SolverLN` comparison still runs.

### Use the solver

```java
import jline.lang.layered.LayeredNetwork;
import jline.solvers.LayeredNetworkAvgTable;
import jline.solvers.ln_simple.SolverLNSimple;

LayeredNetwork model = /* build or load a closed LQN */;

SolverLNSimple solver = new SolverLNSimple(model);
solver.iterateCoupledMva();                 // run the coupled-MVA fixed-point loop
LayeredNetworkAvgTable table = solver.getAvgTable();
table.print();                              // per-processor / task / entry / activity metrics
```

### Build

```bash
cd jar
mvn compile                 # compile the JAR sources
```

(See [README-LINE.md](README-LINE.md) for the full set of LINE build profiles and the packaged-JAR
targets.)

### Run the evaluation suite

```bash
cd jar

# Correctness: run LN-Simple, SolverLN and LQNS on every fixture and assert
# LN-Simple matches a reference within tolerance (atol=1e-3, rtol=5e-2).
mvn test -DskipTests=false -Dtest=SolverLNSimpleEvaluationTest

# Iteration + runtime capture for LN-Simple (writes a CSV; needs a cached
# reference table). See FastEval's Javadoc for the -Dfasteval.* options.
mvn test -DskipTests=false -Dtest=FastEval \
    -Dfasteval.ref=<reference.tables.csv> \
    -Dfasteval.out=<results.csv>
```

Tests are skipped by default in this repository, hence `-DskipTests=false`.

---

## Credit and licensing

The base **LINE** library — everything in this repository other than the paths listed under
[*What in this repository is mine*](#what-in-this-repository-is-mine) — was created by the
[QORE Lab](https://qore.doc.ic.ac.uk/) at Imperial College London and is distributed under the
BSD-3-Clause licence. Its original documentation is preserved unchanged in
[README-LINE.md](README-LINE.md), and the licence is in [LICENSE](LICENSE).

This project was completed as a final-year MEng project in the Department of Computing at Imperial
College London, building on LINE and using LINE's `SolverLN` and the external LQNS solver as
comparators. My additions are released under the same BSD-3-Clause terms as the base project.
</content>
</invoke>
