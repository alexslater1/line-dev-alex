# Post-run patch plan — fixes to apply AFTER the current overnight run completes

**Context.** A full refresh run is in progress (started 2026-06-10 ~12:17). Do **not** touch
solver/harness code or compete for CPU while it runs — the run is timing-sensitive. Three known
defects need fixing, but all are deferred until the run finishes so its (already-flawed-but-complete)
dataset is captured first. Then we fix the code and **cheaply re-derive only the affected columns by
re-running SolverLNSimple + SolverLQNS against the run's *cached* SolverLN output** — never re-running
the slow SolverLN (which dominates wall-clock, esp. the ~16-min B fan-outs).

The run currently bakes in all three defects below. That's fine — we patch post-hoc.

---

## Defects to fix

### D1 — SolverLNSimple timer excludes construction — ✅ DONE (no action needed)
Fixed in `util.java` **before** the run started (file mtime 12:12, run started 12:16), so the entire
current run captures correct `sim_time_ms` (construction included, consistent with LN/LQNS). Nothing
to re-run for D1. *(Original description retained below for the record.)*

#### D1 (original) — SolverLNSimple timer excludes construction (FAIRNESS bug)
`util.runSolverLNSimple` ([util.java:428-437](jar/src/test/java/jline/solvers/ln_simple/util.java#L428-L437))
starts the clock *after* `new SolverLNSimple(model)`, whereas `runSolverLN` and `runSolverLQNS`
construct *inside* the timed block. So `sim_time_ms` excludes construction while LN/LQNS include it —
an unfair comparison favouring Sim.
- **Fix:** move `long start = …` above `new SolverLNSimple(model)`. Same in
  `FastEval.runOne` ([FastEval.java ~268-295](jar/src/test/java/jline/solvers/ln_simple/overnight/FastEval.java))
  which brackets only `iterateCoupledMva`.
- **Scope of effect:** every `sim_time_ms` in every CSV + FastEval timing.
- **Key simplification:** SolverLNSimple is identical across all conditions (ablations only change
  SolverLN), so `sim_time_ms` is the *same series* in baseline and every ablation CSV. Re-capture it
  **once** (Sim-only) and write it into all of them.

### D2 — NaN-vs-finite → Infinity in `sim_vs_lqns` (CONFIRMED from baseline-r1: 75/78 fixtures)
**Confirmed systematic, not ablation-specific.** In `baseline-r1.json`, `sim_vs_lqns_max_rel_diff =
Infinity` on **75 of 78** fixtures (the other 3 are the OR-fork D3 failures with `lqns=null`);
`sim_vs_ln` has **zero** Infinity. Root cause from the actual tables (a chain fixture):
- Sim and LQNS have **different NaN patterns**, and the LQNS-side synthesis *creates* finite values
  where Sim is NaN:
  - Entry/task **ResidT**: Sim reports `NaN`; the helper synthesizes LQNS `ResidT = Q/X` (finite,
    e.g. 9.68) ([util.java:180-186](jar/src/test/java/jline/solvers/ln_simple/util.java#L180-L186))
    → `NaN(Sim)` vs `finite(LQNS)` → `Infinity`.
  - Activity **Util**: Sim finite (`0/0/1.0`), LQNS `NaN` (synthesis `X·D/c` fails when host-demand
    isn't in the map) → finite-vs-NaN → `Infinity`.
- `sim_vs_ln` is clean because Sim and LN share NaN patterns (NaN-vs-NaN → 0).
- Secondary, finite issue (the "funny LQNS Util"): processor Util convention — chain leaf `P3` shows
  Sim `1.0` vs LQNS `3.93` (LQNS total ΣX·D / INF busy-server-count vs Sim's clamp). This is a real
  finite disagreement (the report's C2 "definitional Util" finding), and would still inflate
  `sim_vs_lqns` even after the NaN fix — so verify post-fix and consider skipping Util on IS/INF
  processor rows.

**Proposed fix (D2):**
1. `rawRelDiff` ([:358-364](jar/src/test/java/jline/solvers/ln_simple/util.java#L358-L364)): return
   `Double.NaN` for NaN-vs-finite (skip), and have the max-loops
   ([:231-241](jar/src/test/java/jline/solvers/ln_simple/util.java#L231-L241)) ignore NaN (only
   update the max when the value is finite). `sim_vs_lqns` then = max over genuinely-comparable cells.
2. Re-check: after (1), does `sim_vs_lqns` look like the report's small finite values? If the
   processor-Util convention still dominates, add Util-on-IS/INF skipping or fix the ÷c normalisation
   to cover single-server INF/PS (currently only `c > 1`,
   [:193-194](jar/src/test/java/jline/solvers/ln_simple/util.java#L193-L194)).
3. Optional: stop synthesizing LQNS ResidT/Util for rows where **Sim** is NaN (the synthesis is what
   manufactures the mismatch) — i.e. only synthesize when it enables a both-finite comparison.

(Pass/fail is unaffected throughout — match-either `min(slackLn, slackLqns)` already masks it, and
fully-unavailable LQNS records empty, not Infinity.)

### D2-OLD — original deferred note (superseded by the confirmation above)
`rawRelDiff` / `relativeSlack` / `relativeSlackWith`
([util.java:345-426](jar/src/test/java/jline/solvers/ln_simple/util.java#L345-L426)) return
`Double.POSITIVE_INFINITY` for NaN-vs-finite (only NaN-vs-NaN → 0). For the **reported**
`sim_vs_ln_max_rel_diff` / `sim_vs_lqns_max_rel_diff` (a *max* over cells), a single unsynthesisable
NaN cell inflates the whole fixture to `Infinity` — which lands in the CSV and breaks log-scale figs.
- Pass/fail is unaffected (match-either `min(slackLn, slackLqns)` masks it; fully-unavailable LQNS is
  recorded as empty/NaN via `lastLqnsAvailable=false`, **not** Infinity — confirmed
  [util.java:285-291](jar/src/test/java/jline/solvers/ln_simple/util.java#L285-L291)).
- **Suspected hot spot (per user): the ablation captures.** A modified SolverLN (e.g. f20off/alloff)
  can leave a metric NaN where Sim is finite → `sim_vs_ln = Infinity` / an `assertEquals` failure
  (cf. the report's "C3 anomaly" on alloff). **Investigation is deferred until more of the run lands**
  so we can confirm exactly which condition/column/fixtures show Infinity from real data.
- **Likely fix:** make `rawRelDiff` return `Double.NaN` (skip) for NaN-vs-finite and have the max-loop
  ignore NaN (only update the max when finite). Keep pass/fail logic identical. Decide separately
  whether NaN-vs-finite should ever count as a real disagreement (for LQNS activity-Util it should be
  skipped; confirm for the LN side from the data).
- **Do not finalise D2 until the deferred investigation runs** (see "Deferred investigation" below).

### D3 — LQNX export rejected by lqns on OR-FORK fixtures (CONFIRMED from baseline-r1)
`baseline-r1.json` shows exactly **3** `lqns_iters=null` fixtures — all **OR-fork** (Partition A5):
`A5_orForkUneven`, `A5_orForkThreeWay`, and `A5_combinedDag` (which contains an OR-fork). Crucially,
the **AND-fork** A5 fixtures (`A5_andForkMultiAct`, `A5_andForkThreeWay`, `A5_multiclassAndFork`),
reply, sequence, and loop fixtures all solved on LQNS. So the bug is **OR-fork emission specifically**,
not activity graphs in general.
- `model:19: error: Task …`  → the standalone OR-fork fixtures
- `model:34: error: Symbol …` → `combinedDag` (deeper file, hits the OR-fork region later)

Generator: `LayeredNetwork.writeXML` → `<precedence-activity type=pre/post/end>`
([:761-771](jar/src/main/kotlin/jline/lang/layered/LayeredNetwork.java#L761-L771); fork/loop logic
[:2218-2277](jar/src/main/kotlin/jline/lang/layered/LayeredNetwork.java#L2218-L2277)). `error: Symbol`
= a name referenced but not declared; `error: Task` = malformed task/entry-binding region.

**Strong lead — direct analogue already fixed for AND-fork.** The contribution chapter records that
`ActivityPrecedence.AndJoin`'s default quorum was `Matrix.ones(1, n)`, which the XML writer surfaced
as `quorum="1"` and **lqns rejected** ("The quorum feature is not supported in this version"); fixing
the default to an empty matrix unblocked lqns for the AND-fork fixtures. The OR-fork rejection is the
same *class* of writer bug, un-fixed. Look for the OR-fork analogue: an attribute or branch the writer
emits that lqns rejects, or an OR-branch activity referenced but not declared. The fix is in
`LayeredNetwork.writeXML` (the OR-fork branch), not `SolverLQNS`.

---

## Study impact — exactly what each bug touches (and what it does NOT)

Both bugs are confined to the **LQNS comparison columns** (`sim_vs_lqns_max_rel_diff`,
`matches_within_tol`, `lqns_iters/lqns_time_ms`). Neither touches iteration counts, runtimes, the
SolverLN output, the LN-vs-LN drift, or `sim_vs_ln` (which is clean — Sim & LN share NaN patterns, 0
Infinities in baseline-r1).

### D2 — affects only the BASELINE LQNS-classification + reported `sim_vs_lqns`
- **`sim_vs_lqns_max_rel_diff` → `Infinity`** on 75/78 baseline fixtures (via `rawRelDiff`).
- **`match-lqns-only` → `match-neither`**: `relativeSlackWith` (the loose LQNS slack feeding the
  classification, [util.java:236-238](jar/src/test/java/jline/solvers/ln_simple/util.java#L236-L238))
  also returns `+Inf` for NaN-vs-finite, so fixtures that should be `match-lqns-only` (LN-divergent
  but LQNS-close, e.g. C1, B-fanout) are mis-tagged `match-neither`. **Confirmed:** baseline-r1 shows
  C1_* / B-fanout / C5 as `match-neither`; the report says they're `match-lqns-only`.
- **`match-ln` is CORRECT** (strict LN slack is clean), so anything that only counts `match-ln` is fine.

**Affected report numbers (all baseline):** §match-protocol "50 / **11 / 0**" → the 11-lqns-only /
0-neither split is wrong; §Boundary C1 / B-fanout / C2 / A4_chainPlusFanout `sim_vs_lqns` values +
their `match-lqns-only` tags; Table divergent-partitions-reldiff (`sim_vs_lqns` column); the
"within tolerance of ≥1 reference on every fixture" claim.

**NOT affected:** iteration-count comparison (gap-closure 70.9%, floor-binding, per-partition means);
runtime comparison + per-iter tables; F17/F24 ablations; **F20 accuracy (Table f20-accuracy-losses is
LN-vs-LN drift via `ln_drift_analysis.py`)**; F15 "13 leave match-ln" (the *count* is right — only the
destination label lqns-only-vs-neither is wrong); master table `match-ln` column (all `match-ln`
counts correct); Correctness Table partition-a-reldiff (`sim_vs_ln`); figures e3.1 / e10.1 (`sim_vs_ln`).
→ **No ablation CSV needs re-deriving.**

### D3 — affects only the 3 OR-fork fixtures
`A5_orForkUneven`, `A5_orForkThreeWay`, `A5_combinedDag` (Partition A5 / activity-graph). Their
`lqns_iters/lqns_time_ms/sim_vs_lqns/match` are wrong (LQNS unavailable). Every other A5 fixture
(AND-fork, reply, sequence, loop) and the whole A1–C4 corpus are unaffected.

---

## DECIDED: do NOT re-run the f20axis `lqsim` reference capture — leave it alone

**Decision (2026-06-11):** the `CaptureLqsimReference` lqsim run into
`report/data/f20axis-*/lqsim-reference.tables.csv` is **abandoned, on purpose**. Do not re-run it,
do not chase it by raising the REST read timeout, restarting the `lqns-rest` container in a loop, or
editing `SolverLQNS` for it. It is not worth the time and it does not gate any number we keep.

**Why it kept timing out (for the record, so nobody re-discovers it the hard way):**
- lqsim wall-time is driven by `run_time` (lqsim's `-T`, = the `lqsim.samples` knob); `-B`/blocks only
  partitions the run for variance estimation and barely affects cost. These closed models are
  event-heavy: even `-T 30000` ≈ **5 min/fixture**, overrunning `SolverLQNS`'s hard-coded 300 s REST
  read timeout ([SolverLQNS.java:871](jar/src/main/kotlin/jline/solvers/lqns/SolverLQNS.java#L871)).
- On timeout the Java client abandons the request but the container keeps the lqsim child alive;
  the server is single-threaded, so orphaned runs pile up and choke the next call (container shows
  `(unhealthy)`). Restarting the container clears it only until the next over-budget run re-orphans.

**What this run feeds — and what it does NOT (the reason skipping is safe):**
- Feeds **only** the F20 drift-*direction* sub-study: the `→lqsim` columns in
  `ln_drift_analysis.py`, which are **optional** there
  ([guarded by `lqsim_path.exists()`](report/source/figures/scripts/ln_drift_analysis.py)) and
  supplementary (directional context for 2 of 6 fixtures).
- The report **already documents the timeout as expected**:
  [evaluation.tex:543](report/source/evaluation/evaluation.tex#L543) states the four heavy fixtures
  "exceed the 300-second simulator-side limit of the LQNS REST container even at low `run_time`
  budgets, so their lqsim direction is undetermined and not reported." The two fixtures that *do*
  carry an lqsim distance (`C4_lowDemand`, `C3_multiclassMultiEntry_asymmetric`) were captured in an
  **earlier successful run**; those written numbers stand — do not regenerate them.
- **NOT affected:** the LN-vs-LN drift numbers (the main F20 signal — no lqsim input); the **F17
  interlock probe**, whose lqsim "ground truth" is a *separate* capture
  (`report/data/interlock-probe.csv` via `InterlockProbeFixtures`, 100k samples), unrelated to this
  f20axis run; `sim_vs_ln`; iteration-count and runtime comparisons.
  → **Nothing to re-derive. The f20axis bundle is complete enough as-is.**

**Stray knobs left available but unused** (harmless; revert if you want a clean diff): `lqsim.blocks`
is now configurable in `SolverLQNS` (default 30 preserved); `CaptureLqsimReference` defaults
`lqsim.samples=5000`/`lqsim.blocks=10`; `refresh-overnight.sh` exposes `LQSIM_SAMPLES`/`LQSIM_BLOCKS`.
None of this changes any kept number — it only exists if someone later deliberately revisits lqsim
capture with a much smaller budget.

---

## Minimal patch (post-run)

**Step 1 — diagnose & fix D3 (writer).** Generate `A5_orForkUneven`'s `model.lqnx` (tiny `@Test`/main
calling `model.writeXML` + the same POST as `SolverLQNS`), read the full lqns error at lines 19/34,
fix the OR-fork emission in `LayeredNetwork.writeXML` (analogous to the AND-join quorum fix — look for
an attribute lqns rejects or an OR-branch activity referenced-but-not-declared). Verify the re-POST solves.

**Step 2 — fix D2 (comparison helpers).** In `util.java`, make `rawRelDiff` **and** `relativeSlackWith`
(and `relativeSlack` for symmetry) return `Double.NaN` for NaN-vs-finite, and have the max-loops skip
NaN. Re-check the residual `sim_vs_lqns`: if the processor-Util convention (`P3` 1.0-vs-3.93,
IS/INF) still inflates it, extend the ÷c normalisation to single-server INF or skip Util on IS/INF rows.

**Step 3 — ONE cheap re-derivation pass (Sim + LQNS only, vs cached LN).** Recompile, then run the
re-derivation harness over the **baseline** corpus: load the run's cached LN per-node tables
(`f20axis-<ts>/baseline.tables.csv`, Stage F), run Sim + LQNS fresh, recompute `sim_vs_ln`,
`sim_vs_lqns`, `matches_within_tol` with the fixed helpers, and capture `lqns_*` for the 3 OR-fork
fixtures (now solvable) → emit `report/data/recompare.csv`. Rel-diff/match are **deterministic** → a
single pass suffices (no medianing). **No SolverLN re-run. No ablation re-run.** ~10 min.
- Harness template: **`report/scripts/LqnsRecompare.java.template`** — drop into the test tree
  *after* the D2/D3 fixes compile (it lists the small `util.java` refactor it needs). Do NOT add it
  while the run is live.

**Step 4 — surgical patch + verify.** Merge `recompare.csv` into the run's `baseline.csv` with
**`report/scripts/patch-baseline-lqns.py`** (built + verified against `baseline-r1.csv`): it overwrites
ONLY `lqns_iters, lqns_time_ms, sim_vs_lqns_max_rel_diff, matches_within_tol`, leaves every other
column byte-identical, and aborts if any non-target cell would change.
```bash
python3 report/scripts/patch-baseline-lqns.py \
  --baseline report/data/baseline.csv --recompare report/data/recompare.csv \
  --out report/data/baseline.patched.csv          # review, then mv over baseline.csv
```
Leave all ablation CSVs untouched. Update the prose numbers (§match-protocol 50/**11/0** split,
§Boundary `sim_vs_lqns` values + C1/B-fanout `match-lqns-only` tags, F15 destination label). Spot-check
no `Infinity` remains and the 3 OR-fork rows have `lqns_iters`.

**Dependency:** needs `baseline.tables.csv` (Stage F, accuracy phase — late in the run). If a faster
turnaround is wanted, the re-derivation can instead re-run all three solvers on baseline — but that
re-runs LN (slow), so the cached-LN path is preferred.

## Note for future runs (not this one)
To make the re-derivation **pure-offline** (zero solver re-runs), have the main capture also dump the
Sim and LQNS per-node tables (like Stage F does for LN). Then any comparison-logic fix is a CSV
recompute. Not possible for the current run (already in flight).
