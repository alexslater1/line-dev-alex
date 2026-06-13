# Report number inventory — every figure, table, and inline number, with its source

**Purpose.** The solver is being changed, so every measured number in the report will go stale.
This document catalogues **every number that must be re-derived**, where it appears in the report,
and which data artefact / harness run produces it. Pair it with
[REFRESH-OVERNIGHT-PLAN.md](REFRESH-OVERNIGHT-PLAN.md), which specifies the overnight script that
regenerates the underlying data.

Legend for the **Source** column:
- `baseline` / `<cond>` = a per-fixture CSV written by `SolverLNSimpleEvaluationTest` via
  `DataCollector` (columns: `fixture_name, partition, condition, sim_iters, sim_time_ms,
  ln_iters, ln_time_ms, lqns_iters, lqns_time_ms, sim_vs_ln_max_rel_diff, sim_vs_lqns_max_rel_diff,
  matches_within_tol, notes`). Lives in `report/data/<cond>.csv`.
- `*.tables.csv` = per-(fixture, condition, solver, node) metric dump (Q/U/R/ResidT/T) — F20-axis
  drift analysis and the LN-vs-LN per-cell drift table.
- `replay` = medianed N=3 capture under `report/data/replay-<ts>/<cond>-r{1..3}.csv`.
- `baseline-r{1..4}` = medianed N=4 baseline under `report/data/baseline-r*.csv` → `baseline-medianed.csv`.
- `FastEval` = LNSimple-only fast harness (`overnight/FastEval.java`) — drives the **contribution
  optimisation** numbers (iteration counts, sweep order, α/tol sweep, cache ablation).
- `interlock-probe` = `SolverLNInterlockAccuracyTest` → `report/data/interlock-probe.csv` (+ tables).
- `lqns-overhead` = `SolverLqnsRestOverheadTest` → `report/data/lqns-overhead.csv`.
- `lqsim` = LQNS simulator reference in the F20-axis bundle (`lqsim-reference.tables.csv`).
- **DERIVED** = computed by a figure/analysis script from one or more of the above, not stored raw.
- **ANALYTIC** = hand-computed worked example or exact reference value; refresh only if the *fixture*
  changes, not the solver.

> ⚠️ The `.csv` data files under `report/data/` were being deleted/regenerated while this inventory
> was written (only `*.run.log`, the `replay-*`, `f20axis-*` bundles, and `overnight-driver.log`
> survived). Treat the file-name references below as the **expected** output names of the refresh,
> not as guaranteed-present inputs.

---

## Part 0 — Cross-cutting constants (appear in many places; change once, fix everywhere)

| Constant | Current value | Where it appears | Source / how to refresh |
|---|---|---|---|
| Evaluation corpus size | **61 fixtures** | abstract, intro (×1), eval §hypotheses, §corpus, conclusion (×3), contribution ×3 | **Row count of `baseline.csv`.** ⚠️ See Part 5 item S1 — the harness now yields **65**. |
| Partition split | A=36 (A1 9 / A2 12 / A3 9 / A4 6), B=10, C=15 (C1 6 / C2 3 / C3 3 / C4 3) | eval §hypotheses, §corpus; conclusion | `cut -d, -f2 baseline.csv \| sort \| uniq -c`. ⚠️ A4 now 10 methods (S1). |
| Organic/regression corpus size | **63 fixtures** | eval §hypotheses ("63-fixture organic test corpus"), contribution | Count of `@Test` in `SolverLNSimpleTest` + DAG suite; verify or restate. |
| DAG demonstration suite | **14 DAG fixtures + D_combinedDag + 11 phase-walker unit tests** | contribution §step6, conclusion future-work | Count `DagFeatureFixtures` / `SolverLNSimpleDagTest` / `LqnGraphPhase1Test`. |
| Match tolerance (LN) | `atol = 1e-3, rtol = 5e-2` | eval §match-protocol, §correctness, contribution, conclusion | Constant in `util.java` / `FastEval.java`. Fixed unless changed. |
| Match tolerance (LQNS fallback) | `atol = 1e-3, rtol = 1e-1` | eval §match-protocol | `util.LQNS_DATASET_RTOL`. |
| Sim inner conv. tol | `iter_tol = 5e-3` (master) → contribution Move 4 changes to `1.5e-2` | eval §hypotheses, §boundary, contribution Move 4 | `SolverLNSimple` constant. **Will change with solver.** |
| Sim iteration cap | **100** | eval (A4_chainPlusFanout cap-hit, many places) | `SolverLNSimple` maxIter constant. |
| `iter_min` floor | **50** (→ reported `ln_iters = 53` w/ +2 +1 bias) | eval §iteration-comparison, §f20 | `SolverLN` floor; the **+2 hard-reset / +1 verification** offset is structural. |
| Population for Partition B | **N=30** (spec was N=50) | eval §corpus | EvaluationSuite B fixtures. |
| LQNS REST overhead | **median 18 ms/call** | eval §runtime | `lqns-overhead.csv` (`SolverLqnsRestOverheadTest`). |

---



## Part 2 — Evaluation chapter (the bulk) — `report/source/evaluation/evaluation.tex`

### 2.1 Spine / hypotheses / concessions (§eval-hypotheses, lines ~15–45)

| # | Value(s) | Line(s) | Source |
|---|---|---|---|
| H1 | 36/36 Partition A `match-ln` | 23 | `baseline.csv` filter partition∈A, `matches_within_tol` |
| H2 | every A: `sim_iters < ln_iters` | 25 | `baseline.csv` |
| H3 | f24off.ln_iters == baseline (all 61) | 27 | `f24off.csv` vs `baseline.csv` |
| H4 | F24 per-iter: 1.17× tasks40_chain, 1.45× tasks60_chain; 1.01×→1.45× chain trend; ±6% elsewhere | 29 | `replay` f24off-r{1..3} vs `baseline-r{1..4}`, per-iter = ln_time_ms/ln_iters |
| H5 | f20off total < 0.40×baseline; **3,801 → 1,248** (3.05×) | 31 | `f20off.csv` |
| H6 | baseline **50/61** match-ln, f20off **44/61**; 6 lost in A1/A2/C3/C4 | 33 | `baseline.csv` + `f20off.csv` |
| H8 | F17 per-iter: ~1.10× chains flat, **1.18× (tasks5) → 3.81× (tasks60)** fanout (single-capture); medianed 1.04–1.09× / 1.13–3.43× | 39 | `replay` f17off + `baseline-r*` |
| H9 | F15-off: **13** fixtures move match-ln→match-lqns-only (5 A2, 4 A4, 1 C2, 3 C3) | 41 | `f15off.csv` vs `baseline.csv` |
| Concession 1 | F24 single-capture 1.68× → **1.02×** medianed (vs N=4 baseline) | 43 | `replay/f24off-r*` vs `baseline-r*` |
| Concession 2 | floor = **~92%** of F20 iter effect | 43 | `A2_arm_A.csv`: `(3801−1452)/(3801−1248)` |
| Concession 3 | A4_chainPlusFanout: sim_vs_lqns **3.10e-02**, sim_vs_ln **4.76e-02**, 100-iter cap | 43 | `baseline.csv` row A4_chainPlusFanout |



### 2.3 Correctness (§eval-correctness, ~105–151) — **Table tab:eval-partition-a-reldiff**, **Fig e3.1**

| # | Value | Line | Source |
|---|---|---|---|
| Largest A residual | **2.81e-02** (A3_multiserver_c2_loadLow) | 113, 828 | `baseline.csv` `sim_vs_ln_max_rel_diff` |
| High-load agreement | ≤ **4.70e-04** | 113 | `baseline.csv` |
| **Table** A1 | N=9, range **9.50e-05–2.69e-04**, 9/9 | 122 | `baseline.csv` partition A1 |
| **Table** A2 | N=12, **4.14e-05–2.62e-03**, 12/12 | 124 | `baseline.csv` partition A2 |
| **Table** A3 | N=9, **4.70e-04–2.81e-02**, 9/9 | 126 | `baseline.csv` partition A3 |
| **Table** A4 | N=6, **1.89e-08–4.76e-02**, 6/6 | 128 | `baseline.csv` partition A4 (⚠️ N=6, S1) |
| A4_chainPlusFanout footnote | sim_vs_lqns 3.10e-02, sim_vs_ln 4.76e-02 | 111 | `baseline.csv` |
| **Fig e3.1** | per-fixture A `sim_vs_ln_max_rel_diff`, rtol=5e-2 line | 139 | `e3_1_partition_a_reldiff.py` ← `baseline.csv` |

### 2.4 Iteration-count comparison (§eval-iteration-comparison, ~153–211) — **Table tab:eval-iter-counts-per-partition**, **Fig e4.1**

| # | Value | Line | Source |
|---|---|---|---|
| Totals | sim **418**, LN **3,801**, LQNS **774**; ratios **9.1×** vs LN, **1.9×** vs LQNS | 158 | `baseline.csv` sum of iters columns |
| Adjusted ratios | 5.3× (LN, −2 offset), 1.1× (LQNS) with 1.65× scaling | 158 | DERIVED |
| LN floor | mean **53** on 8/9 partitions | 160, 194 | `baseline.csv` |
| **Table** per-partition Sim/LN/LQNS means + ratios | A1 6.3/53/26.6; A2 2.0/53/9.7; A3 5.2/53/7.0; A4 20.5/53/9.3; B 8.4/109.8/19.9; C1 10.3/53/7.3; C2 2.3/53/2.0; C3 2.0/53/9.3; C4 2.7/53/7.7; **TOTAL 6.85/62.3/12.7** | 169–187 | `baseline.csv` grouped means |
| Win/loss vs LQNS | sim beats LQNS on 6/9 (A1,A2,A3,B,C3,C4) | 194 | DERIVED |
| A3 scaled flip | 5.2×1.65=8.58>6 | 194 | DERIVED |
| C1 underlying | 10.3 vs 7.3 | 196 | `baseline.csv` |
| Floor binding | **55 of 61** at ln_iters=53; 6 exceptions (chain 81/161/201, fanout 81/161/201) | 198 | `baseline.csv` |
| **Fig e4.1** | per-fixture iter counts ×3 solvers, log y, faceted | 205 | `e4_1_iters_by_solver.py` ← `baseline.csv` |

### 2.5 Runtime comparison (§eval-runtime, ~216–302) — **Table tab:eval-runtime-per-partition**, **Table tab:eval-per-iter-runtime-baseline**, **Figs e5.2, e5.5**

| # | Value | Line | Source |
|---|---|---|---|
| LQNS REST overhead | median **18 ms** | 219 | `lqns-overhead.csv` |
| Aggregate wall-clock | LN **2,464,593 ms**, LQNS **20,886 ms** (19,238 net), sim **19,259 ms**; 128× vs LN, 1.08× vs LQNS | 221 | `baseline-medianed.csv` (sim fair bracket); LN/LQNS sums |
| **Table** per-partition totals+means (ms) | A1 341/272/23068…; B 18235/19563/2402680; **TOTAL 19259/20886/2464593**, means 316/342/40403 | 233–242 | `baseline-medianed.csv` + `backup-pre-lqns/baseline.csv` |
| Partition B share | **97.5%** LN / **93.7%** LQNS / **94.7%** sim of corpus total | 249 | DERIVED |
| B-only sim vs LQNS | **1.07×** faster | 249 | DERIVED |
| **Table per-iter B** (10 rows) | LN it/ms-per-it, LQNS it/ms-per-it, Sim it/ms-per-it, LN/Sim, LQNS/Sim — e.g. tasks5_chain 53/51.15, 14/2.00, 6/3.58, 14.3×, 0.56× … tasks60_fanout 201/8317.1, 5/3011.0, 10/1281.0, 6.5×, 2.35× | 271–280 | `baseline-medianed.csv` per-fixture |
| LN/Sim per-iter range | **6.5–19.7×** | 286 | DERIVED |
| REST-net | LQNS 20,887→**19,238 ms**; ratio 1.08×→**1.00×** | 290 | `lqns-overhead.csv` + `baseline-medianed.csv` |
| **Fig e5.2** | per-fixture runtime ×3, fair-bracket sim, medianed | 253 | `e5_4_median.py`→`baseline-medianed.csv`; `e5_2_bracket_compare.py` |
| **Fig e5.5** | e5.2 with LQNS net of REST | 294 | `e5_5_lqns_no_rest.py` ← `baseline-medianed.csv` + `lqns-overhead-smoke.csv` |

> Note: **Fig e5.1** (`e5_1_runtime_by_solver.py` ← `backup-pre-lqns/baseline.csv`) and the
> fact-check script `e5_3_factcheck.py` exist in the pipeline but e5.1 is not currently `\includegraphics`'d
> in the chapter — confirm whether it should be (or is superseded by e5.2).

### 2.6 Interlock ablation F17/F24 (§eval-interlock, ~322–492) — **Tables f24-per-partition, interlock-per-iter, f17_f24-per-iter, interlock-accuracy-probe**, **Fig e6.1**

| # | Value | Line | Source |
|---|---|---|---|
| Zero iter/accuracy effect | f17off & f24off ln_iters == baseline (3,801) | 328 | `f17off.csv`, `f24off.csv` |
| F24 deep-chain | tasks40_chain **1.17×**, tasks60_chain **1.45×**; 1.01×→1.45×; fanout 0.98–1.00× | 330 | `replay/f24off-r*` + `baseline-r*` |
| **Table f24-per-partition** (9 rows) | base/f24off ms-per-iter, mean speedup, range, F24%, sim ms-per-iter, f24off/Sim — A1 48.4/46.5/1.04×/…/7.75× … C3 18.1/20.4/0.89× … | 340–356 | `replay` + `baseline-r*` |
| **Table interlock-per-iter** (10 B rows) | base/f17off/f24off ms-per-it + speedups; tasks60_fanout 8317.1/2422.7(3.43×)/8390.6(0.99×) | 380–398 | `replay` (f17off,f24off) + `baseline-r*` |
| F17 residual gap close | tasks60_fanout 6.5×→1.9×; tasks40_fanout 7.4×→3.0× | 405 | DERIVED |
| Joint f17+f24 | corpus **921,012 ms** vs **2,464,596 ms** (2.68×); B **2,402,680→868,039** (2.77×, 1,098 iters) | 407 | `replay/f17_f24off-r*` + `baseline-r*` |
| **Table f17_f24-per-iter** (10 B rows) | base/joint ms-per-it, joint speedup, pred(f17×f24), meas÷pred (**1.01–1.07**), joint/Sim | 421–439 | `replay` |
| Finding 4 single vs median | single f24off 1.68×/f17off 2.67×/joint 2.04× → median 1.02×/2.45×/2.68× | 446 | `replay` + `baseline-r*` |
| **Interlock accuracy probe** | P1_n8 +0.73pp(53/53), P1_n16 +0.19pp(53/53), P2_n8 +0.28pp(83/83), P4 +1.41pp(53/68); lqsim 100,000 samples; RespT trim 0.04–0.10s on 5–6s | 467–480 | `interlock-probe.csv` (`SolverLNInterlockAccuracyTest`) |
| Probe dropped-fixture deltas | ln_baseline 19–49% vs lqsim, lqns 4–6%, F17 ≤2pp | 457 | interlock-probe (dropped) |
| **Accuracy-preserving frontier** | combined **7.99×**, iters **1,452**, wall **320,472 ms**, 50/61 match-ln; f20off **8.29×**, 6 lost | 486–488 | `combined.csv`, `f20off.csv` |
| Frontier trade | combined gives up ~0.30× for 6 fixtures | 488, 492 | DERIVED |
| **Fig e6.1** | F17-off & F24-off per-iter speedup on Partition B (single-capture data plotted) | 365 | `e6_1_interlock_speedup.py` ← `baseline-medianed.csv` + `f17off.csv` + `f24off.csv` |

### 2.7 F20 ablation (§eval-f20, ~501–618) — **Tables f20-iter-counts, f20-accuracy-losses**, **Fig e7.1**

| # | Value | Line | Source |
|---|---|---|---|
| F20-off iter cut | **3,801 → 1,248** (67.2%, 3.05×); 55 fixtures 53→5–32 | 505 | `f20off.csv` |
| **Table f20-iter-counts** (9 rows) | baseline/f20off mean + reduction× — A1 53/15.4/3.4×; B 109.8/52.1/2.1×; **TOTAL 62.3/20.5/3.05×** | 514–532 | `f20off.csv` vs `baseline.csv` |
| Above-floor drops | chain 81/161/201→63/123/184; fanout 81/161/201→21/22/22 | 539 | `f20off.csv` |
| Per-iter work | wall **309,023** vs **2,561,103 ms** (8.29×); naive 8.29/3.05≈2.72×; median **1.58×** (p25 1.51, p75 1.69) | 541 | `f20off.csv` + `baseline.csv` (single-capture) |
| Accuracy drift | 6 fixtures 5.6%–32.0% | 543 | `*.tables.csv` (LN-vs-LN per-cell) via `ln_drift_analysis.py` |
| **Table f20-accuracy-losses** (6 rows) | fixture, Δf20off, worst cell (base→f20off), ΔArmA, mechanism — A2_…loadHigh_N50 32.0% (ER2.qlen 49.3→65.1) 3.0%; C4_lowDemand 28.2%; C3 15.8%; A2…N20 12.6%; A2…loadLow_N50 12.3%; A1_chain_d3_N100 5.6% | 552–562 | `f20axis/*.tables.csv` |
| ArmA corpus drift | peaks **3.5%** (C4_lowDemand E1.respt), only 2/61 >3% | 565, 583 | `A2_arm_A.tables.csv` |
| lqsim direction | C4: base 8.55e-04→f20off 0.220; C3: 3.7e-3→0.119 (6 closer/26 farther) | 571 | `lqsim-reference.tables.csv` + drift |
| **Arm A** | 3,801→**1,452** (62%); floor = (3801−1452)/(3801−1248)=**91.99%** (~92%); remaining ~8% = 204 iters | 581 | `A2_arm_A.csv` |
| Arm A wall | **4.20×** = 2.62× iters × 1.60× per-iter (Simpson's); per-fixture median **1.00×** | 585 | `A2_arm_A.csv` |
| Arm B fanout overhead | tasks20/40/60_fanout 81/161/201→21/22/22; LN drift <0.45% | 589 | `f20off.csv` + tables |
| F20-off summary | "~three-quarters" iter cut | 616 | DERIVED |
| **Fig e7.1** | per-fixture ln_iters across baseline/f24off/f20off/alloff + sim dot | 595 | `e7_1_iter_ablation.py` ← `baseline.csv`+`f24off.csv`+`f20off.csv`+`alloff.csv` |

### 2.8 Residual (§eval-residual, ~622–689) — **Table gap-closure**, **Fig e9.1**

| # | Value | Line | Source |
|---|---|---|---|
| **Table gap-closure** (13 rows) | sim/baseline-LN/alloff-LN totals + closure% — A1 15/265/74/76.4%; A2 24/636/194/72.2%; A3 27/477/108/82.0%; A4 36/530/135/80.0%; A5 20/424/78/85.6%; B 62/1098/521/55.7%; C1 52/318/96/83.5%; C2 7/159/30/84.9%; C3 6/159/40/77.8%; C4 8/159/32/84.1%; C5 104/448/309/40.4%; C6 3/53/8/90.0%; C7 7/109/47/60.8%; **TOTAL 371/4835/1672/70.9%** | 637–655 | `alloff.csv`+`baseline.csv` |
| A4 anomaly | sim_iters=100, alloff.ln_iters=17; de-anomalised ~82% | 658, 664 | `baseline.csv`+`alloff.csv` |
| **Fig e9.1** | per-partition gap-closure stacked (closed vs residual) | 668 | `e9_1_gap_closure.py` ← `baseline.csv`+`alloff.csv` |

### 2.9 Boundary regimes (§eval-boundary, ~694–772) — **Fig e10.1**, **Table divergent-partitions-reldiff**

| # | Value | Line | Source |
|---|---|---|---|
| C1 sim_vs_ln | fanOut2 **0.2939**, fanOut3 **0.3624**, fanOut4 **0.3029**; vs LQNS 4.44e-2/4.28e-2/4.14e-2 | 699 | `baseline.csv` |
| C2 sim_vs_ln | **1.48e-08, 2.30e-08, 4.63e-08** (Util definitional) | 701 | `baseline.csv` |
| B-fanout sim_vs_ln | **0.281, 0.466, 0.594, 0.723, 0.808** (tasks 5/10/20/40/60); vs LQNS 5.14e-2/4.93e-2/3.08e-2/1.20e-2/1.08e-2 | 703 | `baseline.csv` |
| A4_chainPlusFanout | sim_vs_lqns 3.10e-02, sim_vs_ln 4.76e-02 | 705 | `baseline.csv` |
| C3 anomaly | f20off 0.066 vs alloff 0.134 | 707 | `f20off.csv`+`alloff.csv` |
| F15 | **13** fixtures move (5 A2: multiclassB 3to1/5to1, multiclassC 4to1; 4 A4; 1 C2 infTaskShared; 3 C3) | 709 | `f15off.csv` |
| tasks60_chain LN-LQNS | sim_vs_ln 3.26e-03, sim_vs_lqns **5.87** | 711 | `baseline.csv` |
| **Fig e10.1** | B-fanout sim_vs_ln vs task count (0.281…0.808) | 715 | `e10_1_bfanout_reldiff.py` ← `baseline.csv` |
| **Table divergent-partitions** | C1 (6), C2 (3), B-fanout (5) rel diffs + mechanism | 739–765 | `baseline.csv` |

### 2.10 Failure modes (§eval-failure-modes, ~777–815) — **Table failure-modes**

Mostly qualitative, but these numbers must be re-verified against the failing suite
(`SolverLNSimpleFailingTest`, all `@Disabled`):

| # | Value | Line | Source |
|---|---|---|---|
| Mode 1 | sim_vs_ln 4.76e-2, f20off converges in **17** iters | 782 | `baseline.csv` + `f20off.csv` |
| Mode 3 | `ER1.RespT ≈ 50%` high; `ResultsCollector.java:254–268`, guard line 259 | 786 | code + failing-suite run |
| Mode 4 | 6 fixtures, **8–14%** disagreement; ρ≈0.99 | 788 | failing-suite (C5_*) |
| Mode 5 | finite-server PS AND-fork; `ResultsCollector` line 294 | 790 | failing-suite (C6) |

### 2.11 Discussion (§eval-discussion, ~820–924) — **Table aggregate-runtime-conditions** (the 12-condition master table)

| # | Value | Line | Source |
|---|---|---|---|
| H1–H9 restatements | all numbers above re-cited | 828–844 | (same sources) |
| **Table aggregate-runtime-conditions** (12 rows) | condition, pass, ln_iters, wall-clock(s), speedup, ms/iter, per-iter×, match-ln — f30off, baseline, f23off, f24off†, f15off, f17off†, f17_f24off†, A2_arm_A, f21+f20off, combined, f20off, alloff | 866–888 | one CSV per condition (`<cond>.csv`); † rows from `replay` |
| Medianed-baseline restate | f24off 1.02×, f17off 2.45×, f17_f24off 2.68× (vs N=4 baseline 2,465 s) | 891 | `replay` + `baseline-r*` |
| LQNS aggregate | **20,837 ms (~21 s)**, invariant; alloff 13.2× slower, combined 15.4×, baseline 123× | 899 | `baseline-medianed.csv` LQNS column |
| Sim aggregate | **19,259 ms**, within 9% of LQNS fair bracket | 899 | `baseline-medianed.csv` |
| Outlier note | tasks60_fanout = **1,757,244 ms** of 2,561,103 ms baseline | 901 | `baseline.csv` |
| Per-fixture median per-iter (F20-axis rows) | ArmA 1.00×, f21+f20off 1.00×, f20off 1.58×, combined 1.03×, alloff 1.59×; combined tasks60_fanout 2.59× | 901 | per-condition CSVs |
| F15 not removable | 21% drop in match-ln | 913 | `f15off.csv` |

---

## Part 3 — Contribution chapter — `report/source/contribution/contribution.tex`

### 3.1 Design-history / worked-example numbers — **ANALYTIC** (refresh only if fixture changes)

| # | Value | Line | Note |
|---|---|---|---|
| lqn_basic collapse | tput 2.0E8, service 1.0E-8 | 72 | symptom value; stable |
| MVA worked example | R/X/Q table (0.5/0.667/0.333 … 0.9/1.579/1.421); Z=1.00, R=0.90 | 95–102 | ANALYTIC |
| 3:1 asymmetry | 1.0 vs {0.5,1.5} | 307 | ANALYTIC |
| AND-fork values | X=5.71 (LN), 6.67 (textbook), E[max] 1.5/1.069/1.833; reply 7.00/6.56/9.0 | 603–639 | reference outputs; re-verify if LN/LQNS change |

### 3.2 Performance / kernel numbers — **measured, refresh against final solver**

| # | Value | Line | Source |
|---|---|---|---|
| Multi-class tolerance | within **4.9%** | 353 | FastEval multi-class subset vs LN |
| LD kernel slowness | N=200 ~**60 s** total, ~**2.5 s/layer**; instability N=20/N=100 | 362–366 | FastEval / micro-bench |
| Kernel speedup | N=200 60 s → **0.1 s**, **600×** | 446 | micro-bench |
| BS errors | naive Schweitzer **17–50%**; Linearizer negative; cliffs | 399 | FastEval |
| Fan-out fix | false fixed point at **3** iters → **~24** outer iters | 458, 511 | FastEval |
| TikZ perf curves | Figs lines 380–429 (sparse-CSC vs double[], AMVA, lattice) | 380–429 | **ILLUSTRATIVE TikZ** — schematic; redraw only if shape changes |

### 3.3 Optimisation section (`sec:DRAFT-perf-optimisation`) — **all marked "[DRAFT, provisional] pending re-measurement"** → **FastEval**, optimised operating point

| # | Value | Line | Source |
|---|---|---|---|
| Headline | **418 → 192** iters (**54.1%**); **1704 → 853 ms** (50%) | 664, 773 | FastEval (optimised stack) |
| A4_chainPlusFanout 2-cycle | Δ=4.146278e-01 identical from iter 14; ~¼ of corpus iter budget | 672 | FastEval trace |
| Master tol vs harness | 5e-3 vs 5e-2 (5× tighter); α=0.5 | 672 | constants |
| Name memo | 1010→497 ms (−51%); now within 1.7× noise | 692, 775 | FastEval (vs master) |
| **Table DRAFT-ablation** (cache) | 522.7 / 516.3 / 505.0 / 532.0 ms; −1.2% / −3.4% / +1.8%; derived-cache +9.8% | 700–707 | FastEval, mean of 3, quiet HW |
| Warm-start | tasks40/60_fanout 5→3 iters (18%); corpus −10 iters | 713 | FastEval |
| **Table DRAFT-alpha-tol** (5×4 grid) | iter counts 307…192 + fail flags | 727–731 | FastEval joint α/tol sweep |
| Chosen cell | α=1.0, tol=1.5e-2 → **192** | 729 | FastEval |
| α=1.2 break | slack 1e15; 429 on tasks5_fanout | 719 | FastEval |
| **Table DRAFT-sweep** | bounce 192/627 ms; forward 414/857; reverse 191/505; double-forward 260/765; true-elevator 317/687; DS2 14.23 vs 20.05 (29%), bounce 0.2% | 760–767 | FastEval sweep-order study |
| Rejected accel | Nesterov 24–65% inflation; Broyden 60–150%, 8 regressions | 748 | FastEval |
| JFR profile | findEntry ~54, findCallerTask ~26, … | 783–801 | **TEMP/SCHEMATIC** block — marked for delete; re-capture from JFR if kept |

### 3.4 Feature delta / F-numbers

The F-number catalogue (F2, F7, F8, F12, F15, F17, F18, F19, F20, F21, F23, F24, F30 …) and
`SolverLN.java` line citations (159, 3963, 3238, 4002, etc.) appear throughout. **Re-verify every
`SolverLN.java:<line>` citation** — the active `lnstruct-migration` branch shifts line numbers.

---

## Part 4 — Conclusion — `report/source/conclusion/conclusion.tex`

| # | Value | Line | Source / note |
|---|---|---|---|
| 61-fixture | 25, 42, 48 | `baseline.csv` |
| 36/36 Partition A | 42, 44, 180 | `baseline.csv` |
| A3 largest 2.81e-02 | 42 | `baseline.csv` |
| A4 3.10e-02 / 4.76e-02 | 46 | `baseline.csv` |
| F20 3,801→1,248 (67.2%); 55/61 floor; ArmA→1,452 (~92%) | 62 | `f20off.csv`, `A2_arm_A.csv` |
| alloff 1,672 vs sim 371; 1,301 residual; B 55.7%–C1 83.5% | 68 | `alloff.csv` |
| F20-off 6 fixtures (1 A1, 3 A2, 1 C3, 1 C4); 10% of corpus | 72 | `f20off.csv` |

> ⚠️ **S2 (stale):** conclusion.tex:64 still carries the **old single-capture F24 numbers**
> ("38% to 61%", "1.65× at tasks=5 to 2.58× at tasks=60", fanout "1.57× to 1.74×", aggregate
> "1.68×", "roughly uniform"). The evaluation chapter has since **superseded these** with the
> medianed replay (F24 ≈ noise except deep chains). conclusion.tex:66 + intro.tex:41 also foreground
> **F19** as a headline ablation, which the eval chapter demoted to inert. These must be reconciled
> with the eval chapter regardless of the re-run.

---

## Part 5 — Identified staleness / inconsistencies to resolve (the high-value findings)

- **S1 — Corpus size drift (61 → 65).** `SolverLNSimpleEvaluationTest` now registers **10** A4
  `@Test` methods (`threeTier_multiclass, multiclass_multiserver, multiclass_multientry,
  chainPlusFanout, webApplication, databaseBackend, threeTier_sequence, parallelService_andFork,
  retryLoop, replyThenLog`); `partitionOf` tags all `A4_*` as A4. The report's corpus is A4=6 /
  total=61. A fresh capture yields **65 rows** (A4=10). **Decide before the re-run:** are the 4 new
  fixtures (`threeTier_sequence`, `parallelService_andFork`, `retryLoop`, `replyThenLog` — these look
  like activity-graph fixtures the report explicitly excludes) part of the canonical corpus or not?
  If not, exclude them in the harness; if yes, every "61"/"A4=6"/"36" count and every per-partition
  aggregate in the report changes. This single decision gates the whole re-run.
- **S2 — F24 / F19 stale framing in conclusion + intro** (see Part 4 note).
- **S3 — `SolverLN.java:<line>` citations** will be wrong after the `lnstruct-migration` branch lands
  (line numbers shift; some methods may move). Re-grep every citation.
- **S4 — Ablation patches are line-based diffs against the *old* `SolverLN.java`.** They will not
  apply cleanly post-migration (see plan §"Ablation toggling").
- **S5 — `report/data/*.csv` were wiped** during this session; only logs / `replay-*` / `f20axis-*`
  survive. The figure scripts will fail until the captures are re-run.
- **S6 — Two operating points.** Eval chapter = **unoptimised** baseline (sim 418 iters). Contribution
  optimisation = **optimised** (sim 192 iters). Keep them distinct in the re-run: the eval baseline
  must be captured with optimisations **off** (default flags), the contribution table with them on.
- **S7 — `e5_1` figure** may be orphaned (script exists, not `\includegraphics`'d). Confirm intent.
