# Overnight refresh plan — regenerate every report number against the final solver

**Companion to** [REFRESH-INVENTORY.md](REFRESH-INVENTORY.md) (what each number is and where it lives).
This document specifies the **overnight script** that re-derives all of them. The solver is still
changing, so the script cannot be validated yet; it is written defensively and the genuinely
ambiguous parts (ablation toggling) are specified as a build-it-yourself plan rather than guessed.

A starter scaffold is provided at the end (`report/scripts/refresh-overnight.sh`, created alongside
this doc) — it implements every **unambiguous** stage and leaves clearly-marked `TODO(ablation)`
hooks for the parts that depend on decisions only you can make.

---

## 0. Pre-flight decisions (must be settled before the script runs — see INVENTORY Part 5)

1. **Corpus membership (S1).** Decide whether the 4 new A4 fixtures + any C5/C6/C7 belong in the
   canonical corpus. Recommended: keep the corpus at the **same 61** the report is calibrated to by
   *not* registering the 4 activity-graph A4 fixtures in `SolverLNSimpleEvaluationTest` (move them to
   a separate non-corpus test), OR accept 65 and re-state every count in the report. The script
   asserts the captured row count equals an expected `EXPECTED_CORPUS_N` and aborts on mismatch.
2. **Operating point (S6).** The eval-chapter baseline must be captured with optimisations **off**
   (master α=0.5, tol=5e-3, warm-start/limit-cycle as they were at the 418-iter point) — i.e. the
   `SolverLNSimple` state the spine claim was measured against. If the "final solver" *is* the
   optimised one, then either (a) re-baseline the whole eval chapter at the new operating point and
   restate 418→new everywhere, or (b) keep a flag to disable the optimisations for the eval capture.
   **This is a content decision, not a scripting one — settle it first.**
3. **Ablation mechanism (S4).** The per-condition `SolverLN.java` patches are line-based and will not
   apply after the migration. Choose one:
   - **(Recommended) Convert ablations to runtime flags.** Add permanent `-Dablation.f24=off` style
     gates inside `SolverLN.java` (a single `AblationFlags` reader). Then the overnight script just
     sets a system property per condition — no patch/recompile churn, robust across refactors.
   - **(Fallback) Re-derive the diffs.** Re-express each `.modification.diff` against the current
     `SolverLN.java`, store them, and have the script `patch`/revert per condition (revert via
     `git show HEAD:…/SolverLN.java > …/SolverLN.java` because the file has the **skip-worktree** bit
     set — `git checkout` will NOT revert it; verify with `git hash-object`).

---

## 1. The full study matrix (what to capture)

### 1.1 Three-solver corpus captures (`SolverLNSimpleEvaluationTest`, `DataCollector`)

Each row below = one `mvn test` invocation producing `report/data/<dataset>.csv` (+ `.json`,
optionally `.tables.csv`). All feed the eval chapter.

| Dataset | Condition | SolverLN change | Purpose / report use |
|---|---|---|---|
| `baseline-r1..r4` | baseline | none | Medianed N=4 baseline → `baseline-medianed.csv`. Tables 4.x, 5.x; Figs e4.1, e5.2, e5.5, e6.1. ~42 min each. |
| `baseline` | baseline | none | Single-capture baseline used by e3.1, e7.1, e9.1, e10.1, and all per-fixture rel-diff numbers (§2.3, §2.4, §2.9). |
| `f24off` | F24off | skip `ptaskcallers` compute | §2.6, Fig e7.1, master table |
| `f20off` | F20off | disable iter_min floor + moving-avg + hard-reset | §2.7, master table, Fig e7.1 |
| `alloff` | allOff | F20off + F24off | §2.8, Fig e7.1, e9.1, master table |
| `f17off` | F17off | `options.config.interlocking = false` (constructor) | §2.6, Fig e6.1, master table |
| `f17_f24off` | F17+F24off | both | §2.6, master table |
| `f15off` | F15off | early-return `updateRoutingProbabilities` | §2.6/§2.9, master table |
| `A2_arm_A` | A2_arm_A | split iter_min into `iter_min_convergence=1` / `iter_min_avg=base` | §2.7 Arm A, master table |
| `f21+f20off` | f21+f20off | F20off + F21 (elevator) off | master table |
| `f23off` | f23off | (F23 gate) | master table (control) |
| `f30off` | f30off | (F30 gate) | master table (control) |
| `f19off` / `f19true` | F19 | force relax="fixed" / force adaptive | reported inert; intro/conclusion mention (S2) |
| `relaxauto_c1` | relaxauto | adaptive on C1 only | §discussion F19 finding |

**Medianed replay (`replay-<ts>/`).** Re-capture `f17off`, `f24off`, `f17_f24off` at **N=3**
(one capture per condition per run, 3 runs back-to-back) — these are the load-bearing per-iteration
F17/F24 numbers (§2.6 Findings 2–4, H4, H8). Compare against the **N=4** medianed baseline.
The eval prose explicitly flags these three as the only conditions whose single-capture aggregates
were JIT-noise-corrupted; the future-work note (eval §discussion) suggests extending the medianed
replay to `f15off, f20off, combined, alloff` — **do this in the refresh** so every aggregate-runtime
row in the master table is medianed, not single-capture.

> **B fan-out cost.** `B_scaleAxis_tasks40_fanout` (~7 min LN) and `tasks60_fanout` (~16–28 min LN)
> dominate every baseline/ablation run (tasks60_fanout alone ≈ 1.76M of 2.56M ms). They are
> `@Disabled` by default in the JUnit test but were enabled for the report baseline. Budget for them
> explicitly; they are why each baseline run is ~42 min.

### 1.2 F20-axis bundle (`*.tables.csv` + `lqsim`) — for the LN-vs-LN per-cell drift

Re-capture the `f20axis-<ts>/` bundle: `baseline`, `f20off`, `A2_arm_A`, `lqsim-reference`, each with
`.tables.csv` (per-node Q/U/R/ResidT/T). Then run `ln_drift_analysis.py <bundle-dir>` →
`ln_drift_summary.csv`, which produces Table `f20-accuracy-losses` (§2.7) and the lqsim-direction
numbers (C4 0.220, C3 0.119, etc.).

### 1.3 Targeted probes (separate from the corpus)

| Capture | Harness | Output | Report use |
|---|---|---|---|
| Interlock accuracy probe | `SolverLNInterlockAccuracyTest` (+ `InterlockProbeFullTableTest` for MC SE) | `interlock-probe.csv`, `interlock-probe-tables.md` | Table `interlock-accuracy-probe` (§2.6), lqsim 100k samples |
| LQNS REST overhead | `SolverLqnsRestOverheadTest` | `lqns-overhead.csv`, `lqns-overhead-smoke.csv` | 18 ms/call (§2.5), Fig e5.5 |

### 1.4 Contribution optimisation numbers (`FastEval`, LNSimple-only, optimised stack)

These are the `[DRAFT, provisional]` numbers (INVENTORY §3.3). All from `FastEval` with the
relevant `-Dlnsimple.*` flags, on **quiet hardware, mean of ≥3 runs**:

| Study | FastEval flags | Output → table/number |
|---|---|---|
| Optimised vs baseline | default optimised vs `-Dlnsimple.*` master | 418→192 iters, 1704→853 ms (§3.3 headline) |
| Joint α/tol sweep | grid over `lnsimple.zalpha`,`lnsimple.dalpha`,`lnsimple.tol` | Table `DRAFT-alpha-tol` |
| Sweep order | `lnsimple.sweep=bounce/forward/reverse/double-forward/elevator` | Table `DRAFT-sweep` (+ DS2 stress via `SweepStressFixtures`) |
| Cache ablation | `lnsimple.nameCache=off`, `lnsimple.derivedCache=off`, struct vs OO `findCallerTask` | Table `DRAFT-ablation` |
| Warm-start | `lnsimple.warmStart=false` | 5→3 iters fanout, −10 iters corpus |
| Rejected accel | `lnsimple.accel=nesterov/broyden/aitken/anderson` | 24–65% / 60–150% inflation |
| JFR profile (if kept) | run under `-XX:StartFlightRecording`, parse top-N | Fig `DRAFT-jfr-profile` (currently SCHEMATIC/TEMP) |

---

## 2. Execution order & time budget

Total ≈ **8–11 h** wall-clock (hence overnight). Order chosen so the cheapest/most-load-bearing
data lands first and the long B-fanout-bearing runs are batched.

```
Stage A  health check + build           ~10 min   (lqns-rest up? mvn compile clean?)
Stage B  baseline-r1..r4 (N=4)          ~3.0 h    (4 × ~42 min)  → baseline-medianed.csv
Stage C  single-capture baseline        ~42 min   → baseline.csv (+ tables)
Stage D  ablations: f24off,f20off,alloff,
         f17off,f17_f24off,f15off,
         A2_arm_A,f21+f20off,f23off,f30off ~5–6 h  (floor-cut ones are fast ~5 min;
                                                    interlock/baseline-like ones ~25–42 min)
Stage E  replay N=3 (f17off,f24off,f17_f24off
         [+ f15off,f20off,combined,alloff]) ~3–4 h
Stage F  f20axis bundle + lqsim          ~1 h      → ln_drift_summary.csv
Stage G  probes (interlock, lqns-overhead) ~30 min
Stage H  FastEval studies (contribution) ~30 min   (LNSimple-only is fast)
Stage I  figure + analysis regen         ~5 min    (python scripts)
Stage J  validation + summary report     ~2 min
```

Stages D and E are the long poles. If time-boxed, prioritise: B (medianed baseline) → C → the
ablations that feed the master table → replay → everything else.

---

## 3. Per-condition capture command (template)

For the **runtime-flag** mechanism (recommended):

```bash
tsp /usr/bin/mvn -q -Dtmp=false test \
  -Dtest=SolverLNSimpleEvaluationTest \
  -DskipTests=false \
  -Dablation=<f24off|f20off|...> \          # read by AblationFlags in SolverLN.java
  -Dbaseline.outdir=report/data \
  -Dbaseline.dataset=<dataset> \
  -Dbaseline.condition=<condition>
```

For the **patch** mechanism (fallback):

```bash
SLN=jar/src/main/kotlin/jline/solvers/ln/SolverLN.java
git show HEAD:$SLN > $SLN                                  # clean revert (skip-worktree-safe)
patch -p1 < report/data/diffs/<condition>.modification.diff
tsp /usr/bin/mvn -q -Dtmp=false compile                   # recompile patched solver
tsp /usr/bin/mvn -q -Dtmp=false test -Dtest=SolverLNSimpleEvaluationTest \
  -Dbaseline.dataset=<dataset> -Dbaseline.condition=<condition> -Dbaseline.outdir=report/data
git show HEAD:$SLN > $SLN                                  # revert
git hash-object $SLN                                        # MUST equal HEAD blob hash
```

Notes:
- `tsp` (task-spooler) serialises runs so only one capture uses the CPU at a time — essential for
  clean timing. Keep it.
- The DataCollector flushes via a JVM **shutdown hook**; a clean `mvn` exit (even with assertion
  failures — those still capture the row) writes the CSV. A crash/`kill -9` loses the in-memory rows.
- The warmup (`@BeforeAll`, 3× A1_chain_d3_N20, capture suspended) must stay — it JIT-warms the
  kernels so timing is reproducible.

---

## 4. Post-processing (after captures land)

```bash
cd report/source/figures/scripts
python3 e5_4_median.py                 # baseline-r*.csv → baseline-medianed.csv  (RUN FIRST)
python3 e3_1_partition_a_reldiff.py    # → figures/eval-e31-partition-a-reldiff.pdf
python3 e4_1_iters_by_solver.py        # → eval-e41-iters-by-solver.pdf
python3 e5_2_bracket_compare.py        # → eval-e52-fair-runtime-by-solver.pdf
python3 e5_5_lqns_no_rest.py           # → eval-e52b-lqns-no-rest.pdf
python3 e6_1_interlock_speedup.py      # → eval-e61-interlock-speedup-partitionb.pdf
python3 e7_1_iter_ablation.py          # → eval-e71-iter-ablation.pdf
python3 e9_1_gap_closure.py            # → eval-e91-gap-closure-decomposition.pdf
python3 e10_1_bfanout_reldiff.py       # → eval-e101-bfanout-reldiff.pdf
python3 ln_drift_analysis.py ../../../data/f20axis-<ts>   # → ln_drift_summary.csv
# (e5_1_runtime_by_solver.py, e5_3_factcheck.py: confirm whether still needed — see S7)
```

The script should run these automatically once the required inputs exist, and **skip with a warning**
any whose inputs are missing (don't fail the whole run).

---

## 5. Error-safety requirements (build these into the script)

- **Idempotent / resumable.** Tag each stage's output; on restart, skip stages whose output already
  exists and is non-empty (unless `--force`). Overnight runs get interrupted.
- **Per-stage logging.** `tee` every `mvn` run to `report/data/refresh-logs/<dataset>.log` and keep a
  top-level `refresh-driver.log` with timestamps + per-stage rc + row counts (mirror the existing
  `overnight-driver.log` format).
- **Row-count assertion.** After each capture, assert `wc -l <csv>-1 == EXPECTED_CORPUS_N`; abort the
  stage (not the run) and log loudly on mismatch. This is the S1 guard.
- **Solver-state guard.** Before Stage B, record `git hash-object SolverLN.java` and refuse to start
  if it has uncommitted/un-reverted ablation edits (a leftover patch would silently poison the
  baseline). After every patch-based ablation, re-assert the hash equals HEAD.
- **`set -euo pipefail`**, but wrap each capture so one failing condition logs and continues to the
  next rather than aborting the night.
- **Health checks up front:** lqns-rest container reachable (the baseline/ablation captures call
  LQNS over REST — a dead container fails every fixture's LQNS column); `mvn compile` green;
  task-spooler (`tsp`) available; ≥ N GB free disk for logs.
- **No `git commit`/`stash`/`reset`** in the script (repo rule). Reverting `SolverLN.java` via
  `git show HEAD:` + file write is allowed and is the only git write needed.
- **Don't trust single-capture aggregate timings** for any headline — that lesson is baked into the
  report. Anything quoted as a speedup ratio must come from the medianed (N≥3) path.

---

## 6. Validation step (script's final stage)

After everything regenerates, emit `report/data/refresh-summary.md` cross-checking the new data
against the INVENTORY's expected fields, so you can diff old-vs-new at a glance:

- corpus N per condition (flag ≠ EXPECTED_CORPUS_N);
- per-condition `ln_iters` total, `match-ln` count, aggregate wall-clock;
- the headline derived numbers recomputed from the fresh CSVs:
  - iteration split `(base−alloff)/(base−sim)` (was 70.9%);
  - F20 `base→f20off` total (was 3801→1248) and Arm A share (was ~92%);
  - per-partition means (Tables iter-counts, runtime, gap-closure);
  - medianed f24off / f17off / f17_f24off aggregate speedups (were 1.02× / 2.45× / 2.68×).
- a printed "STALE TEXT TO UPDATE" list pointing at the exact report lines from the INVENTORY whose
  literal values changed.

This turns the report update from "hunt for numbers" into "apply the diff the summary prints".

---

## 7. What the script cannot decide for you (hand-offs)

1. **The S1 corpus decision** — the script asserts a count; you choose the count.
2. **The S2/S3 stale-text reconciliation** — mechanical once the data is fresh, but the F19/F24
   framing in intro+conclusion is an editorial choice, not a number swap.
3. **The ablation toggling mechanism** (runtime flags vs re-derived patches) — pick one in §0.3 and
   the script's `apply_ablation()` / `revert_ablation()` functions follow.
4. **Whether the "final solver" re-baselines the eval chapter** (S6) — if optimisations are now
   default-on, the eval baseline of 418 iters is itself stale and the whole spine recalibrates.
