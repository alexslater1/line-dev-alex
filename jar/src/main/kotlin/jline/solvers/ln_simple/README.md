# SolverLNSimple

A decomposition solver for Layered Queueing Networks (LQNs). The LQN is factored into a set of single-server closed PFQNs ("layers"), each solved by MVA. Layers are coupled through `Clients`-delay think times and queue service rates, and the solver iterates until per-layer throughputs converge.

> **This is the deep technical reference for the solver internals.** For the project context — what
> LN-Simple is, why it exists, how it was built up feature by feature, and the headline evaluation
> results against `SolverLN` and LQNS — see the [top-level README](../../../../../../../README.md).

---

## Source layout

```
src/main/kotlin/jline/solvers/ln_simple/
├── SolverLNSimple.java        ← public API + iteration loop + coupling stages
├── EnsembleInitialiser.java   ← one-shot init (demand fix-up, Case B, pop cap)
├── LqnGraph.java              ← pure read-only queries on the LQN model
├── mva/
│   ├── MvaInputs.java          ← MVA matrix builders; IS-partition + kernel dispatch
│   ├── MvaLd.java              ← primitive-array exact load-dependent MVA recursion
│   └── MvaAmva.java            ← primitive-array Seidmann multi-server AMVA (large prodN)
├── results/
│   ├── ResultsCollector.java   ← public entry, orchestrates phases 1, 2, 4
│   ├── LayerMetrics.java       ← Phase 1 (per-layer raw extraction)
│   ├── QueueLengthSolver.java  ← Phase 3 + REF inheritance + chain walker
│   ├── OutputTableBuilder.java ← Phase 5 (proc/task/entry/activity rows)
│   └── ResultsState.java       ← mutable bag of maps threaded between phases

Reference docs now live under the top-level docs/ln_simple/ folder.

src/test/java/jline/solvers/ln_simple/
├── SolverLNSimple*Test.java    ← JUnit tests vs SolverLN / SolverLQNS
├── FastEval.java               ← LNSimple-only iteration/runtime capture harness
├── util.java                   ← shared test harness
└── fixtures/
    ├── EvaluationSuite.java             ← the 78-fixture evaluation corpus
    ├── SingleclassLNExamples.java
    ├── MulticlassLNExamples.java
    └── DagFeatureFixtures.java          ← activity-graph fixtures
```

Responsibilities at a glance:

| File                        | Lines  | Owns                                                                |
|-----------------------------|--------|---------------------------------------------------------------------|
| `SolverLNSimple.java`       | ~1000  | Public API, iteration loop, the two coupling stages.                |
| `EnsembleInitialiser.java`  |  ~500  | Constructor-time setup — runs once.                                 |
| `LqnGraph.java`             | ~1330  | Stateless queries on the LQN model, incl. activity-graph DAG walks. Shared by solver and results. |
| `mva/MvaInputs.java`        |  ~390  | Build {L, N, Z, S}; partition IS stations; dispatch to `MvaLd` / `MvaAmva`. |
| `mva/MvaLd.java`            |  ~220  | Primitive-array exact load-dependent MVA recursion.                 |
| `mva/MvaAmva.java`          |  ~170  | Primitive-array Seidmann multi-server AMVA for large `prodN`.       |
| `results/ResultsCollector`  |  ~340  | Top-level entry; Phase 2 corrections; Phase 4 leaf-tput scaling.    |
| `results/LayerMetrics`      |  ~250  | Phase 1: final MVA + per-layer P:/T: metric extraction.             |
| `results/QueueLengthSolver` |  ~170  | Phase 3: Little's-law Q derivation + REF inheritance + chain walk.  |
| `results/OutputTableBuilder`|  ~780  | Phase 5: proc/task/entry/activity row construction.                 |
| `results/ResultsState`      |   ~55  | Maps shared across phases.                                          |

---

## Layer structure

`LayeredNetwork.getEnsemble()` produces two kinds of two-node closed networks. Each layer has exactly one `Clients` delay node and exactly one queue:

| Server queue        | Class kind                                  | Demand at the queue                          | Z at `Clients`              |
|---------------------|---------------------------------------------|----------------------------------------------|-----------------------------|
| `T:<task>` (task)   | `R:<caller>`, one per caller                | Σ over sync calls of `callMean × hostDemand` | Caller's blocking / chain   |
| `P:<proc>` (host)   | `T:<task>` (aggregate) or `R:<caller>` (multi-caller / rebuilt) | Population-weighted host demand or per-caller raw host demand | Hosted task's pacing |

`R:` classes have `N = caller multiplicity`; the aggregate `T:` class has `N = task multiplicity`.

---

## Algorithm

### 1. Initialisation — `SolverLNSimple` constructor → `EnsembleInitialiser.initialise`

The constructor delegates one-shot setup to `EnsembleInitialiser.initialise`, whose top docstring numbers these steps:

1. **`snapshotBaselines`** snapshots baseline `Clients` think times per layer and indexes server queues by name (`T:T3` / `P:P2`); **`snapshotTaskPlacement`** records each task's processor (and the reverse processor→tasks map).

2. **`fixCallerClassDemands`** sets per-class demand on every layer's server. The ensemble builder leaves caller classes as `Disabled`; `buildDemandMatrix`'s NaN-fallback would otherwise give every caller class the demand of the first one. Two sub-cases:
   - **Aggregate `T:<task>` class on a P: layer** — demand = population-weighted average host demand over all callers (Schmietendorf–Lazowska aggregation). Falls back to local activity demand for REF / self-contained tasks.
   - **Per-caller `R:<caller>` class** — demand = `Σ callMean × hostDemand_of_bound_activity` summed over all caller activities that issue a sync call into the server's tasks. Also fills `perCallerHostDemand` for T: layers so step 3 can split aggregate P: layers.

3. **`rebuildCaseBHostLayers`** — Case B: a P: host layer whose hosted task is `INF`-scheduled and called by ≥ 2 distinct REF tasks. The aggregate single-class form loses per-caller information; rebuild as multiclass with one `R:<caller>` class per REF caller (`N = caller mult`, `D = raw host demand`, `Z = caller think`). This matches what SolverLN constructs and lets the normal P:→T: coupling work without aggregate→per-class rescaling.

4. **`capTaskLayerPopulations`** caps each T:-layer class population in every P: layer to the sum of caller populations at that task. The processor's *server count* is left alone — that reflects physical multiplicity, not chain population.

### 2. Iteration — `iterateCoupledMva`

Outer loop, at most `maxIter` outer iterations (defaults `DEFAULT_MAX_ITER = 100`,
`DEFAULT_TOL = 1.5e-2`). The sweep uses the **elevator** order (hard-coded): a
single forward pass over the `N` layers on even iterations and a reverse pass on
odd ones:

```
even iter: 0, 1, 2, …, N−1
odd  iter: N−1, …, 2, 1, 0
```

so information flows forward and backward through the call chain over successive
iterations. This is the single-direction sweep the reference solvers (SolverLN,
LQNS) use — it was adopted (commit `switch to elevator sweep`) so the
evaluation's iteration-count comparison is like-for-like. The earlier **bounce**
sweep — a single `2N − 1` bidirectional pass per iteration (`0,…,N−1,N−2,…,1`) —
and the forward / reverse / double-forward orders were compared in the
sweep-order study (Contribution chapter) before elevator was chosen; they are
not retained as a runtime option.

For each layer in the sweep:

1. **`solveLayer`** builds `L`, `N`, `Z`, `S` from the layer's current state and calls `MvaInputs.callMVA`, which partitions out infinite-server stations and then dispatches to the local primitive-array kernels — exact load-dependent MVA (`MvaLd`), or Seidmann multi-server AMVA (`MvaAmva`) once the population lattice `prodN` exceeds the exact-MVA threshold.
2. **`updateDeltaX`** tracks `max |X_l^(k) − X_l^(k−1)|`; the loop exits when it falls below `tol`, or when the outer-loop **limit-cycle detector** fires (see [Convergence and optimisations](#convergence-and-optimisations)).
3. **Coupling** dispatches on the server queue's prefix:

#### Task-layer coupling — `propagateTaskLayerCoupling`

(executed when the layer's server is `T:<callee>`)

1. **Refresh Clients think for every caller class.** For a saturated zero-think intermediate caller we use a cycle-time formula:
   $$
   Z = \frac{N_\text{caller}}{X_\text{caller}} - \text{callMean} \cdot \text{sojourn}_\text{callee}
   $$
   (drives Z → 0 as the caller saturates). Otherwise:
   $$
   Z = \max\bigl(Z_\text{root REF} + D_\text{caller-chain},\ Z_\text{caller} + D_\text{caller local}\bigr).
   $$
2. **Refresh the caller task's own T: layer Clients** with `Z_root_REF + D_caller_chain`.
3. (If throughput is non-finite or ≤ ε, skip the rest — but steps 1 and 2 still happen.)
4. **`R_task_total = Q_server / X`** — server-only response time. Per-call for intermediate tasks; per-caller-visit for leaves (whose T: demand already embeds `callMean`).
5. **Cache** the callee's sojourn and throughput.
6. **Push response to the caller's T: layer server**: `D = D_local_caller + propagatedResp`, where `propagatedResp = R_task_total × callMean` for intermediates and `R_task_total` for leaves.
7. **Update the callee's host-layer Clients think**:
   - Leaf callee, no demand in subtree: `Z = Z_callee`.
   - Leaf callee, with demand: `Z = max(Z_callee, (N_total − Q_server_total) / X_total)` — Little's law on customers not currently at the server.
   - Intermediate callee: `Z = max(Z_callee, Z_caller_chain) + (R_task_total − D_local_callee)` — adds the callee's wait for its own callees.
   - **Skipped for rebuilt Case-B layers** (they have constant per-caller Z set during the rebuild).
8. **Per-caller update of each caller's host-layer Clients** with `Z = Z_root_REF + D_caller_chain + propagated_r`, where `propagated_r` uses `Q_r/X_r` (or `D` for INF callees) and the same intermediate-vs-leaf rule as step 6.

#### Host-layer coupling — `propagateHostLayerCoupling`

(executed when the layer's server is `P:<proc>`)

- **Rebuilt Case-B layer**: each `R:<caller>` class's `Q_r/X_r` is the hosted task's per-class service time. Write each into the hosted task's T: layer server (matched by class name).
- **Otherwise** for each hosted-task class with `R_proc = Q/X`:
  - If the hosted task has a T: layer (not the current one, not itself an intermediate caller): write `R_proc` into every non-Disabled class of the T:-layer server.
  - If the hosted task is REF with no T: layer: inject `R_proc` into the `Clients` delay of every T: layer where this REF task is a caller class, as `Z = baseClientsThink[l] + R_proc`.

All injected values are clamped to `[1e-9, 1e6]` to keep MVA inputs finite during transient overshoot.

### 3. Result collection — `ResultsCollector.collectAndPrintFinalResults`

One last MVA on each layer, then five phases:

**Phase 1 — Per-layer raw extraction.**
- P: layers: per-server util, total demand, processor server count. Per-class metrics stored as `hostLayerTput / hostLayerResid`. REF tasks: `refTaskProcQLen / taskUtil / taskRespT / taskResidT / taskTput`.
- Rebuilt Case-B P: layers: aggregate Q,X attributed back to the hosted task.
- T: layers: `taskQLen`, per-caller `taskClassQLen`, `taskUtil` (inherited from processor), `taskResidT` (per-call, divided by callMean for leaves), `taskTput`.

**Phase 2 — Cross-layer corrections.**
- Non-leaf non-REF tasks: override `taskResidT` with `hostLayerResid` (per-call processor response). T:-layer residual already includes callee response; the P:-layer residual is `D_local` only, eliminating double-counting when the chain walker adds callee response back.
- Throughput: prefer `hostLayerTput` when the host has demand or the task has Z > 0; keep T:-layer X for zero-think immediate intermediates.
- Single-task processors: align `processorUtil` with the final task throughput.
- Asymmetric multiclass: re-derive `processorUtil = Σ_c X_c · D_c / c_servers`. Skip when hosted task has Z > 0 (its P: layer already paces itself correctly).

**Phase 3 — Queue lengths.**
- `applyRefTaskInheritance` runs once: REF Q via Little's law on the think station — `Q = N − X · Z` (or, for REF tasks with no sync callees, a demand-weighted share of `refTaskProcQLen`).
- Non-REF Q via `X · R_eff`, where `R_eff` walks the sync-call chain summing per-call residuals (`resolveTaskResponseTime`, cycle-safe).
- `applyRefTaskInheritance` runs again so REF Q can pull from the just-computed non-REF callee Q.

**Phase 4 — Leaf throughput scaling.**
Multi-call leaves report tput per *call* rather than per caller-visit. Scale `taskTput[leaf] *= maxInboundCallMean`. Must happen *after* Phase 3 — Phase 3 used the caller-rate X.

**Phase 5 — Output table assembly.**
Append rows in order: Processors → Tasks → Entries → Activities. Entry/activity splits use a demand-weighted entry fraction (`callerTput × hostDemand`) so multi-entry tasks get a correct per-entry breakdown. REF-task activities further split their parent's Q, Util, ResidT proportionally to visit-weighted demand (`computeRefActivityDemandFraction`), and their throughput is multiplied by activity visit count (loop bodies fire `loopCount` times per visit).

---

## Numerical safety constants

| Constant         | Value | Used as                                                                       |
|------------------|-------|-------------------------------------------------------------------------------|
| `EPS`            | 1e-12 | Min positive in finite-check guards (`X > EPS`).                              |
| `CLAMP_FLOOR`    | 1e-9  | Lower bound on any value injected into MVA (Z, D).                            |
| `MAX_PROTECTION` | 1e6   | Upper bound on injected values — keeps MVA finite during transient overshoot. |

All injected think / service times go through `clamp(v) = clip(v, CLAMP_FLOOR, MAX_PROTECTION)`.

---

## The MVA kernel — `mva/`

Every per-layer solve goes through `MvaInputs.callMVA(L, N, Z, S)`, which partitions out infinite-server stations and then dispatches to one of two local primitive-array kernels. The kernels exist because LINE's shared load-dependent kernel (`jline.api.pfqn.ld.Pfqn_mvald`, reached via `pfqn_mvams`) was both quadratically slow on the dense large-`N` access an LQN ensemble produces and numerically unstable on IS-only layers. Three moves address this:

1. **IS-station partitioning (`callMVA`).** Before any recursion, each station with `S[i] ≥ Ntot` — every Delay, every INF `Queue`, and any finite-multiplicity station large enough to absorb the whole population — is treated as infinite-server: its per-class demand `L[i,r]` is folded into the effective think time `zEff[r]` and its `L` row zeroed. If *every* station is IS, `analyticalISResult` returns the closed form `X[r] = N[r]/Zeff[r]`, `Q[i,r] = X[r]·L[i,r]`, `U[i] = 0` with no recursion. Otherwise the recursion runs on the non-IS subset only and `rebuildInfStationQ` patches the IS rows' queue lengths back in by Little's law (`Q = X·L`). This is the same `infSET`/`qSET` split `Solver_mva` uses; without it the LD recursion's `pi(0|n) = 1 − Σ pi(k|n)` underflows on IS+IS layers and corrupts `X`.

2. **Exact load-dependent MVA (`MvaLd`).** When the population lattice `prodN = ∏_r (N_r + 1) ≤ exactLatticeMax` (default 500), the non-IS subproblem is solved by `MvaLd.solve(L, N, Z, mu)`, where `mu[i][j]` is the service rate of station `i` with `j+1` jobs present (so a `c`-server station enters as `min(j+1, c)`). `MvaLd` is a *faithful, numerically identical* re-implementation of LINE's `pfqn_mvald` — same recursion, accumulation order and stabilisation rule — but holds the marginal queue-length tables `pi(k|n)` and per-population throughputs in flat `double[]` arrays indexed by a lexicographic population hash, rather than the sparse-CSC `Matrix` the shared routine uses. The shared routine's dense `O(prodN·Nsum)` reads each cost `O(nnz)` against the sparse backing, which turned a single `N=200` multi-server layer solve into seconds; the primitive-array copy runs it at array speed. Single-server layers still route through LINE's `pfqn_mvams`.

3. **Seidmann multi-server AMVA (`MvaAmva`).** Once `prodN` exceeds `exactLatticeMax`, the exact lattice enumeration is too expensive, so `callMVA` falls back to `MvaAmva.solve` — the same Seidmann/Rolia transform LINE's `SolverMVA` uses on closed multi-server PFQNs. Each `c`-server station is split into a bottleneck demand `L'[i,r] = L[i,r]/c[i]` plus an in-service delay `ΔZ[r] = Σ_q L[q,r]·(c_q−1)/c_q` moved into the think vector; plain Bard–Schweitzer then runs on `(L', N, Z + ΔZ)`, and the total queue length is reconstructed as `Q_total[i,r] = Q_BS[i,r] + X[r]·L[i,r]·(c_i−1)/c_i`. The `exactLatticeMax = 500` dispatch threshold was chosen by an empirical model×threshold sweep (Contribution chapter); it is a `volatile` field so characterisation tests can drive other values.

`MvaInputs` also memoises each layer's fixed shape (node list, closed-class set, server queue, `Clients` delay) behind a `WeakHashMap<Network, LayerInfo>` with a volatile most-recent fast path, rebuilt once at the end of the constructor (`invalidateAllLayerCaches`) after the Case-B rebuild has finished reshaping layers.

---

## Activity-graph awareness — `LqnGraph` DAG walks

A pre-activity-graph read treated each entry as its bound activity's demand and each task's activities as a flat unordered bag, dropping OR-branch probabilities, loop counts, AND-fork parallelism and the reply boundary. `LqnGraph` replaces the bag read with a DAG walk over each entry's `ActivityPrecedence` chain, in three parts:

- **Visit-weight DAG walk.** `computeWeightsFromBound(task, bound)` assigns each activity a *visit weight* by a fixed-point propagation: seed the bound activity at weight `1.0`, then apply every precedence via `applyPrecedence` until the weight map stops changing. `applyPrecedence` aggregates fan-in by pre-type (`PRE_AND` collapses predecessors by `max`; `PRE_OR` / `PRE_SEQ` sum / inherit) and distributes fan-out by post-type (`POST_SEQ` single forward edge; `POST_AND` each successor inherits the fan-in; `POST_OR` each successor inherits `fanIn × branchProb`; `POST_LOOP` scales by loop count). `computeActivityVisitWeights(task)` sums per-entry walks for multi-entry tasks. Every call-accounting site that used to read activities as a bag now weights by these — `hostDemandOfEntry`, `perVisitCallsFromCallerToTarget`, the per-class loop in `propagateTaskLayerCoupling`, and `OutputTableBuilder.appendActivityRows` (where the weight scales reported per-activity throughput, so a loop body reports `loopCount` firings per visit).

- **AND-fork `E[max]` join correction (post-iteration).** Parallel branches must be *joined*, not summed — the caller waits for the slowest branch. `andForkMaxCorrection(task, weights)` finds each `POST_AND` fork with ≥ 2 branch-starters, matches it to its AND-join (`findMatchingAndJoin`), sums each branch's weighted demand (`collectBranchDemand`), and replaces the branch-sum with the expected maximum of independent exponentials, computed by inclusion–exclusion (`expectedMaxOfExponentials`): for `K` branches with mean demands `D_i`,

  ```
  E[max] = Σ_{∅≠S⊆{1..K}} (−1)^(|S|+1) · (Σ_{i∈S} 1/D_i)^(−1)
  ```

  (Heidelberger–Trivedi / Franks's join-delay formula for exponential branches). The correction `E[max] − Σ branchDemand` is non-positive and is applied to the **reported** demand only, *after* the fixed point converges — never inside the coupling loop. The iteration itself keeps a plain additive SUM, because `E[max]` is non-additive: feeding it back each sweep makes downstream layers treat it as an ordinary summable demand and converges to a wrong fixed point. The finite-server-host overlap case (sibling branches contending on the same PS host) is out of scope — it needs CCD overlap compensation, not the closed-form `E[max]`.

- **REPLY phase-1 / phase-2 split.** A `repliesTo(E)` activity ends the caller's synchronous wait; activities after the reply are post-reply background work the processor still pays for but the caller does not perceive. `phase1ActivitiesFromBound(entry, bound)` walks the DAG forward from the bound activity and stops expansion at any reply activity (the reply is included, its successors are not); `hasPhase2Background` is the reachable set minus phase 1. The four per-entry demand getters share one body, `hostDemandOfEntryImpl(sumBranches, phase1Only)`, under different flags: `hostDemandOfEntry` (phase-1, `E[max]` join — the caller-perceived wait), `processorDemandOfEntry` (full DAG, summed — processor occupancy), and `phase1SumDemandOfEntry` / `andForkCollapsedDemandOfEntry` (the two mixed views the corrections need). `phase1AdjustedResponseTime` reports `R = S1 + prOt·S2`, where the overtake probability `prOt` (a fresh caller arriving while phase-2 is still busy) comes from a three-state — idle / phase-1 / phase-2 — CTMC over the server, using an effective arrival rate `λ_eff = λ/(1+ρ)` to avoid over-stating arrivals near saturation.

---

## Convergence and optimisations

Once correct, the solver exploits knowing exactly which models it targets. Three optimisations wrap the iteration, all preserving the fixed point:

- **Outer-loop limit-cycle detection.** A near-saturated fan-out can trap the coupling in a two-state cycle (state alternating A, B, A, B, …) in which `maxDeltaX` repeats to many digits and `maxDeltaX < tol` never fires — the solver previously ran to `maxIter = 100`. After the `Δ < tol` check each iteration, the loop tests `|Δ_k − Δ_{k−1}| / max(Δ_k, ε) < LIMIT_CYCLE_REL` for `LIMIT_CYCLE_WINDOW` consecutive iterations, gated by `Δ_k < LIMIT_CYCLE_DELTA_CAP` so the detector stays silent through the initial transient, and then declares convergence and stops at the current ensemble state. Constants: `WINDOW = 1`, `REL = 1e-3`, `DELTA_CAP = 10`. This catches both the exact two-cycle and the slow contraction tail of long chains; the false-saturating fixture that hit 100 iterations now stops at 6, and chain fixtures each save a tail iteration.

- **Memoising invariant topology queries.** A JFR profile flagged `LqnGraph.findEntry` and `findCallerTask` — linear scans over the `LayeredNetwork` re-run every sweep, even though the topology never changes during a solve — as the top CPU hotspots. `findCallerTask` is replaced by an `int[] callerOfT` read off `model.getStruct().issynccaller`; everything else is memoised in a `WeakHashMap<LayeredNetwork, ModelCache>` with a volatile pointer-identity fast path (`cacheFor`), holding both the name lookups and the derived quantities. `MvaInputs` and the per-submodel `Network` caches follow the same pattern. The iteration count is unchanged (same algorithm, same fixed point); the win is far fewer short-lived allocations and a profile-driven ~2× wall-clock speedup on the corpus. Values whose meaning differs from the struct's (e.g. AND-fork `max`-of-weights vs the struct `graph`'s branch-tip sum) are memoised from the solver's own computation, not read from the struct.

- **Convergence-rule and damping retune.** With the two-cycle now handled structurally, the damping that guarded against it can be relaxed. A joint `(α_Z, tol)` sweep (Contribution chapter) exposed an asymmetry: the D-side write into INF-task `T:` layers is un-damped (`D_RELAX_ALPHA = 1.0`, up from 0.5) with no accuracy regression, whereas the fan-out caller Z-write keeps its `Z_RELAX_ALPHA = 0.5` under-relaxation — which is what holds the saturated fan-out fixtures off the iteration cap. The convergence tolerance is loosened from `5e-3` to `DEFAULT_TOL = 1.5e-2`, still well inside the harness's `rtol = 5e-2` pass bar.

---

## Known limitations

- **Single active queue per layer** — coupling treats the first non-Delay node as *the* server. Layers with multiple service stations are not handled.
- **`P:` / `T:` prefix dispatch** — all coupling logic keys off the queue name prefix; non-standard naming is not supported.
- **Last-wins chain-walk maps** — `buildRefTaskCalledTaskMap` / `buildTaskCalledTaskMap` (used by the results-side chain walker) map each caller to a single called task, so the last enumerated callee wins. Fan-out to multiple distinct callees is itself supported — it is handled through the sibling-callee blocking and the activity-graph walks, not through these two helper maps.
- **Case-B rebuild is REF-callers-only** — rebuilding triggers only when an INF-scheduled task is called by at least two distinct REF callers and the hosted task itself issues no sync calls.
- **Mean-only inner kernel** — the local `MvaLd` / `MvaAmva` kernels carry only mean service demand per class; there is no SCV / higher-moment (3-moment APH) propagation, so service-time variability beyond the mean is not modelled.

---

## Reading guide

Open in this order for the smoothest path:

1. **`SolverLNSimple.java`** — top docstring, then `iterateCoupledMva` → `propagateTaskLayerCoupling` → `propagateHostLayerCoupling`. The two coupling methods reference the eight numbered steps above.
2. **`EnsembleInitialiser.java`** — the constructor's heavy lifting. The numbered steps in its top docstring correspond to the `private static` methods `initialise` calls: `snapshotBaselines`, `snapshotTaskPlacement`, `fixCallerClassDemands`, `rebuildCaseBHostLayers`, `capTaskLayerPopulations`.
3. **`mva/MvaInputs.java`** — what each MVA matrix means, IS-station partitioning, and the `MvaLd` / `MvaAmva` dispatch.
4. **`LqnGraph.java`** — query helpers organised by family (lookups / predicates / quantities / caller maps).
5. **`results/ResultsCollector.java`** — the five-phase orchestration. Phases 1, 3, 5 delegate to sibling files; Phases 2 and 4 are inline.

For the underlying mathematics see `docs/ln_simple/notes/LN-docs-java.pdf` and the thesis sources under `docs/ln_simple/fyp-thesis/`.
