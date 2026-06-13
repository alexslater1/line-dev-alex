# SolverLNSimple

A decomposition solver for Layered Queueing Networks (LQNs). The LQN is factored into a set of single-server closed PFQNs ("layers"), each solved by MVA. Layers are coupled through `Clients`-delay think times and queue service rates, and the solver iterates until per-layer throughputs converge.

---

## Source layout

```
src/main/kotlin/jline/solvers/ln_simple/
├── SolverLNSimple.java        ← public API + iteration loop + coupling stages
├── EnsembleInitialiser.java   ← one-shot init (demand fix-up, Case B, pop cap)
├── MvaInputs.java             ← MVA matrix builders + pfqn_mvams reflection call
├── LqnGraph.java              ← pure read-only queries on the LQN model
├── results/
│   ├── ResultsCollector.java   ← public entry, orchestrates phases 1, 2, 4
│   ├── LayerMetrics.java       ← Phase 1 (per-layer raw extraction)
│   ├── QueueLengthSolver.java  ← Phase 3 + REF inheritance + chain walker
│   ├── OutputTableBuilder.java ← Phase 5 (proc/task/entry/activity rows)
│   └── ResultsState.java       ← mutable bag of maps threaded between phases

Reference docs now live under the top-level docs/ln_simple/ folder.

src/test/java/jline/solvers/ln_simple/
├── SolverLNSimple*Test.java    ← Junit tests vs SolverLN / SolverLQNS
├── util.java                   ← shared test harness
└── fixtures/
    ├── LayeredNetworkTestExamples.java
    ├── MulticlassLayeredNetworkExamples.java
    └── BugModels.java
```

Responsibilities at a glance:

| File                        | Lines | Owns                                                                |
|-----------------------------|-------|---------------------------------------------------------------------|
| `SolverLNSimple.java`       |  ~550 | Public API, iteration loop, the two coupling stages.                |
| `EnsembleInitialiser.java`  |  ~340 | Constructor-time setup — runs once.                                 |
| `MvaInputs.java`            |  ~170 | Build {L, N, Z, S}; invoke `pfqn_mvams` via reflection.             |
| `LqnGraph.java`             |  ~330 | Stateless queries on the LQN model. Shared by solver and results.   |
| `results/ResultsCollector`  |  ~200 | Top-level entry; Phase 2 corrections; Phase 4 leaf-tput scaling.    |
| `results/LayerMetrics`      |  ~190 | Phase 1: final MVA + per-layer P:/T: metric extraction.             |
| `results/QueueLengthSolver` |  ~150 | Phase 3: Little's-law Q derivation + REF inheritance + chain walk.  |
| `results/OutputTableBuilder`|  ~380 | Phase 5: proc/task/entry/activity row construction.                 |
| `results/ResultsState`      |   ~50 | Maps shared across phases.                                          |

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

### 1. Initialisation — `SolverLNSimple` constructor

1. **`buildTopologyLookups`** snapshots baseline `Clients` think times per layer and indexes server queues by name (`T:T3` / `P:P2`). Records each task's processor.

2. **`fixCallerClassDemandsInAllLayers`** sets per-class demand on every layer's server. The ensemble builder leaves caller classes as `Disabled`; `buildDemandMatrix`'s NaN-fallback would otherwise give every caller class the demand of the first one. Two sub-cases:
   - **Aggregate `T:<task>` class on a P: layer** — demand = population-weighted average host demand over all callers (Schmietendorf–Lazowska aggregation). Falls back to local activity demand for REF / self-contained tasks.
   - **Per-caller `R:<caller>` class** — demand = `Σ callMean × hostDemand_of_bound_activity` summed over all caller activities that issue a sync call into the server's tasks. Also fills `perCallerHostDemand` for T: layers so step 3 can split aggregate P: layers.

3. **`rebuildCaseBHostLayers`** — Case B: a P: host layer whose hosted task is `INF`-scheduled and called by ≥ 2 distinct REF tasks. The aggregate single-class form loses per-caller information; rebuild as multiclass with one `R:<caller>` class per REF caller (`N = caller mult`, `D = raw host demand`, `Z = caller think`). This matches what SolverLN constructs and lets the normal P:→T: coupling work without aggregate→per-class rescaling.

4. **`capTaskLayerPopulations`** caps each T:-layer class population in every P: layer to the sum of caller populations at that task. The processor's *server count* is left alone — that reflects physical multiplicity, not chain population.

### 2. Iteration — `iterateCoupledMva`

Outer loop, at most `maxIter` outer iterations. The sweep order is configurable
(`SweepOrder`); the production default is **elevator**, which runs a single
forward pass over the `N` layers on even iterations and a reverse pass on odd
ones:

```
even iter: 0, 1, 2, …, N−1
odd  iter: N−1, …, 2, 1, 0
```

so information flows forward and backward through the call chain over successive
iterations. This matches the single-direction sweep used by the reference
solvers (SolverLN, LQNS). The original **bounce** sweep — a single `2N − 1`
bidirectional pass per iteration (`0,…,N−1,N−2,…,1`) — and the other orders are
retained for the sweep-order sensitivity study.

For each layer in the sweep:

1. **`solveLayer`** builds `L`, `N`, `Z`, `S` from the layer's current state and calls `pfqn_mvams` via `MvaUtils.callMVA` (reflection into the Kotlin module).
2. **`updateDeltaX`** tracks `max |X_l^(k) − X_l^(k−1)|`; loop exits when it falls below `tol`.
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

### 3. Result collection — `SolverLNSimpleResultsUtils.collectAndPrintFinalResults`

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

## Known limitations

- **Single active queue per layer** — coupling treats the first non-Delay node as *the* server. Layers with multiple service stations are not handled.
- **`P:` / `T:` prefix dispatch** — all coupling logic keys off the queue name prefix; non-standard naming is not supported.
- **Last-wins call maps** — `buildRefTaskCalledTaskMap` and `buildTaskCalledTaskMap` track only one called task per caller. Tasks with multiple synchronous callees lose all but the last enumerated one.
- **Case-B rebuild is REF-callers-only** — rebuilding triggers only when an INF-scheduled task is called by at least two distinct REF callers and the hosted task itself issues no sync calls.
- **Reflection into Kotlin** — `MvaUtils.callMVA` looks up `pfqn_mvams` by name at runtime; renaming or refactoring that Kotlin method will silently break the solver (the call returns `null` and the outer loop terminates early).

---

## Reading guide

Open in this order for the smoothest path:

1. **`SolverLNSimple.java`** — top docstring, then `iterateCoupledMva` → `propagateTaskLayerCoupling` → `propagateHostLayerCoupling`. The two coupling methods reference the eight numbered steps above.
2. **`EnsembleInitialiser.java`** — the constructor's heavy lifting. The four numbered steps in its top docstring correspond to the four `private static` methods in the file.
3. **`MvaInputs.java`** — short; what each MVA matrix means.
4. **`LqnGraph.java`** — query helpers organised by family (lookups / predicates / quantities / caller maps).
5. **`results/ResultsCollector.java`** — the five-phase orchestration. Phases 1, 3, 5 delegate to sibling files; Phases 2 and 4 are inline.

For the underlying mathematics see `docs/ln_simple/notes/LN-docs-java.pdf` and the thesis sources under `docs/ln_simple/fyp-thesis/`.
