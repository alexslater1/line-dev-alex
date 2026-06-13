#!/usr/bin/env bash
# =============================================================================
# refresh-overnight.sh — regenerate every measured number in the FYP report.
#
# STATUS: SCAFFOLD. The solver is still changing, so this has NOT been run.
# It implements the unambiguous stages (baseline N=4, single baseline, replay,
# probes, figure regen, validation) and leaves the ablation toggling as clearly
# marked TODO hooks (see REFRESH-OVERNIGHT-PLAN.md §0.3 / §3).
#
# Read REFRESH-OVERNIGHT-PLAN.md and REFRESH-INVENTORY.md before running.
#
# Usage:
#   report/scripts/refresh-overnight.sh [--force] [--stages B,C,D,...] [--dry-run]
#
# Design rules (from CLAUDE.md / repo conventions):
#   * NO git commit / stash / reset. Reverting SolverLN.java via `git show HEAD:`
#     + file write is the only git write performed (skip-worktree-safe).
#   * tsp (task-spooler) serialises mvn runs for clean timing.
#   * Every capture is row-count-asserted against EXPECTED_CORPUS_N.
#   * Idempotent: existing non-empty outputs are skipped unless --force.
# =============================================================================
set -euo pipefail

# ----------------------------------------------------------------------------- config
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"   # repo root
JAR_DIR="$ROOT/jar"
REPORT_DIR="$ROOT/report"
DATA_DIR="$ROOT/report/data"
LOG_DIR="$DATA_DIR/refresh-logs"
FIG_SCRIPTS="$ROOT/report/source/figures/scripts"
SLN="$JAR_DIR/src/main/kotlin/jline/solvers/ln/SolverLN.java"
DIFF_DIR="$DATA_DIR/diffs"                 # re-derived <condition>.modification.diff files (fallback mech)
DRIVER_LOG="$DATA_DIR/refresh-driver.log"

# ⚠️ SET THIS once the S1 corpus decision is made (61 today; 65 if the 4 new A4 fixtures stay).
EXPECTED_CORPUS_N="${EXPECTED_CORPUS_N:-78}"

MVN="/usr/bin/mvn"
USE_TSP="${USE_TSP:-1}"                    # 0 to run mvn directly (no task-spooler)
BASELINE_REPEATS="${BASELINE_REPEATS:-3}"  # N for medianed baseline (was 4; report used 4)
REPLAY_REPEATS="${REPLAY_REPEATS:-2}"      # N for medianed ablation timing (was 3; N=2 ⇒ mean-of-2)
FORCE=0; DRY=0; ONLY_STAGES=""

# Ablation toggling mechanism: "flag" (recommended, requires AblationFlags in SolverLN.java)
# or "patch" (fallback, requires re-derived diffs in $DIFF_DIR). See plan §0.3.
ABLATION_MECH="${ABLATION_MECH:-patch}"

# --- F20-axis / table-capture fixture scope (item: corpus-set mismatch) -------
# The table captures iterate FastEval's full fixture set, which is larger than
# the 61-row corpus (extra A4, the two ~16-min B fan-outs, C5/C6/C7). Filters use
# FastEval's glob semantics ("A1_*,C2_infTaskSimple"). Empty = full set.
#   CAPTURE_FIXTURES — LN tables bundle (cheap; default full set, drift tolerates extras).
#   LQSIM_FIXTURES   — lqsim is slow / non-convergent on heavy fan-outs; default to the
#                      A+C partitions (all F20-drift fixtures live there; report documents
#                      the B/4-deep-A lqsim runs exceeding the 300 s simulator limit).
CAPTURE_FIXTURES="${CAPTURE_FIXTURES:-}"
LQSIM_FIXTURES="${LQSIM_FIXTURES:-A1_*,A2_*,A3_*,A4_*,C1_*,C2_*,C3_*,C4_*}"
# lqsim wall-time is driven by run_time (LQSIM_SAMPLES = lqsim's -T); LQSIM_BLOCKS
# (-B) only partitions that run for variance estimation and barely affects cost.
# These models are event-heavy: -T 30000 ~= 5 min/fixture, overrunning the 300 s
# read timeout, so default run_time small (~5k -> ~50 s/fixture). Mean SE scales
# ~1/sqrt(run_time): 5k vs the old 100k is ~4.5x noisier (~0.2% -> ~1%), still
# well under the 5%/10% drift tolerances. Raise for a final lock-the-numbers
# bundle (e.g. LQSIM_SAMPLES=100000), but expect multi-hour runtimes then.
LQSIM_SAMPLES="${LQSIM_SAMPLES:-5000}"
LQSIM_BLOCKS="${LQSIM_BLOCKS:-10}"

# Latest F20-axis bundle dir (set by Stage F; Stages H/J glob for it). Falls back
# to the most recent existing f20axis-* so H/J work even if F is skipped.
# NB: `|| true` is REQUIRED — with `set -o pipefail`, a non-matching glob makes `ls`
# exit non-zero, which would otherwise kill the script at `axdir="$(latest_f20axis)"`
# (Stage H runs before Stage F creates any f20axis-* dir).
latest_f20axis(){ ls -d "$DATA_DIR"/f20axis-* 2>/dev/null | sort | tail -1 || true; }

# ----------------------------------------------------------------------------- args
while [[ $# -gt 0 ]]; do
  case "$1" in
    --force) FORCE=1;;
    --dry-run) DRY=1;;
    --stages) ONLY_STAGES="$2"; shift;;
    *) echo "unknown arg: $1"; exit 2;;
  esac; shift
done

mkdir -p "$LOG_DIR"
exec > >(tee -a "$DRIVER_LOG") 2>&1

log(){ echo "[refresh $(date -Is)] $*"; }
have_stage(){ [[ -z "$ONLY_STAGES" || ",$ONLY_STAGES," == *",$1,"* ]]; }
csv_rows(){ [[ -f "$1" ]] && echo $(( $(wc -l < "$1") - 1 )) || echo 0; }

assert_rows(){ # <csv> — warn+return1 on mismatch, do not kill the night
  local csv="$1" n; n="$(csv_rows "$csv")"
  if [[ "$n" -ne "$EXPECTED_CORPUS_N" ]]; then
    log "WARN row-count mismatch in $(basename "$csv"): got $n expected $EXPECTED_CORPUS_N"; return 1
  fi
  log "OK $(basename "$csv"): $n rows"; return 0
}

skip_if_done(){ # <output-file> — true if we should skip
  [[ $FORCE -eq 0 && -s "$1" ]] && { log "skip (exists): $(basename "$1")"; return 0; }
  return 1
}

run_mvn(){ # <logfile> <mvn args...>
  local lf="$1"; shift
  if [[ $DRY -eq 1 ]]; then log "DRY: mvn $*"; return 0; fi
  if [[ "$USE_TSP" == "1" ]] && command -v tsp >/dev/null; then
    ( cd "$JAR_DIR" && tsp -f "$MVN" -Dtmp=false "$@" ) 2>&1 | tee "$lf"
  else
    ( cd "$JAR_DIR" && "$MVN" -Dtmp=false "$@" ) 2>&1 | tee "$lf"
  fi
}

# --- solver-state guard --------------------------------------------------------
HEAD_BLOB="$(cd "$ROOT" && git rev-parse "HEAD:jar/src/main/kotlin/jline/solvers/ln/SolverLN.java")"
sln_is_clean(){ [[ "$(git -C "$ROOT" hash-object "$SLN")" == "$HEAD_BLOB" ]]; }
revert_sln(){ ( cd "$ROOT" && git show HEAD:jar/src/main/kotlin/jline/solvers/ln/SolverLN.java > "$SLN" ); }

# --- ablation apply/revert (THE hand-off — see plan §0.3) ----------------------
apply_ablation(){  # <condition>
  local cond="$1"
  case "$ABLATION_MECH" in
    flag)  log "ablation=$cond via runtime flag (no source change)";;          # handled in run_capture via -Dablation
    patch)
      local d="$DIFF_DIR/$cond.modification.diff"
      [[ -f "$d" ]] || { log "ERROR no diff for $cond at $d — TODO: re-derive against current SolverLN.java"; return 1; }
      if [[ $DRY -eq 1 ]]; then log "DRY: would apply $d + recompile"; return 0; fi
      revert_sln; ( cd "$ROOT" && patch -p1 < "$d" )
      run_mvn "$LOG_DIR/$cond.compile.log" compile    # recompile patched solver
      ;;
    *) log "ERROR unknown ABLATION_MECH=$ABLATION_MECH"; return 1;;
  esac
}
revert_ablation(){
  [[ "$ABLATION_MECH" == "patch" ]] || return 0
  [[ $DRY -eq 1 ]] && { log "DRY: would revert SolverLN.java + recompile"; return 0; }
  revert_sln
  sln_is_clean && log "reverted SolverLN.java OK" || { log "FATAL revert failed"; exit 1; }
  run_mvn "$LOG_DIR/revert.compile.log" compile
}

run_capture(){ # <dataset> <condition>  (3-solver corpus capture via DataCollector)
  local ds="$1" cond="$2"; local out="$DATA_DIR/$ds.csv"
  skip_if_done "$out" && return 0
  local flag=(); [[ "$ABLATION_MECH" == "flag" && "$cond" != "baseline" ]] && flag=(-Dablation="$cond")
  run_mvn "$LOG_DIR/$ds.log" test -DskipTests=false \
    -Dtest=SolverLNSimpleEvaluationTest \
    -Dbaseline.outdir="$DATA_DIR" -Dbaseline.dataset="$ds" -Dbaseline.condition="$cond" \
    "${flag[@]}" || log "WARN $ds exited non-zero (rows may still be captured)"
  assert_rows "$out" || true
}

run_table_capture(){ # <logfile> <outpath> <test> [mvn args...]
  local lf="$1" out="$2" test="$3"
  shift 3
  skip_if_done "$out" && return 0
  run_mvn "$lf" test -DskipTests=false -Dtest="$test" "$@" || log "WARN $test exited non-zero"
}

# =============================================================================
# Stage A — health checks
# =============================================================================
if have_stage A; then
  log "Stage A: health checks"
  sln_is_clean || { log "FATAL SolverLN.java has un-reverted edits — fix before baselining"; exit 1; }
  command -v "$MVN" >/dev/null || { log "FATAL no mvn"; exit 1; }
  [[ "$USE_TSP" == "1" ]] && ! command -v tsp >/dev/null && log "WARN tsp not found; timing may be noisier"
  # TODO: ping lqns-rest container — a dead container fails every LQNS column,
  # the lqsim reference (Stage F) and the lqns-overhead probe (Stage G).
  # test-compile (not just compile) so test-source breakage — incl. the capture
  # classes — is caught now, before the multi-hour Stage B, not mid-run.
  run_mvn "$LOG_DIR/compile.log" test-compile
fi

# #############################################################################
#  PERFORMANCE PHASE — timing-sensitive captures, run first on the quiet machine.
#  Order: B (medianed baseline) → D (ablations, medianed where it matters) →
#         H (FastEval optimisation studies).
#  Former Stage C is folded into B; former Stage E (replay) is folded into D, so
#  each ablation is patched/compiled ONCE instead of once for D + once for E.
# #############################################################################

# Which ablations need medianed *timing* (N=$REPLAY_REPEATS captures, for the
# per-iteration F17/F24 runtime tables) vs a single capture (the iteration-count
# cut dominates their runtime, so timing is far less noise-sensitive). f17/f24/
# f17_f24 have iteration counts identical to baseline — only their timing varies.
MEDIAN_CONDS="${MEDIAN_CONDS:-f17off f24off f17_f24off}"
SINGLE_CONDS="${SINGLE_CONDS:-f20off alloff}"

# =============================================================================
# Stage B — medianed baseline (N=$BASELINE_REPEATS) → baseline-medianed.csv,
#   plus baseline.csv (= run 1) for the deterministic iteration/accuracy figures.
#   [Folds in the old single-capture "Stage C": baseline.csv is just one of these
#   runs — iteration counts and per-fixture rel-diffs are identical across runs;
#   only timing differs, and the timing figures use baseline-medianed.csv.]
# =============================================================================
if have_stage B; then
  log "Stage B: medianed baseline N=$BASELINE_REPEATS"
  for r in $(seq 1 "$BASELINE_REPEATS"); do run_capture "baseline-r$r" baseline; done
  if [[ $DRY -eq 0 ]]; then
    python3 "$FIG_SCRIPTS/e5_4_median.py" || log "WARN e5_4_median failed"
    if [[ -s "$DATA_DIR/baseline-r1.csv" ]] && { [[ $FORCE -eq 1 ]] || [[ ! -s "$DATA_DIR/baseline.csv" ]]; }; then
      cp "$DATA_DIR/baseline-r1.csv" "$DATA_DIR/baseline.csv"
      log "baseline.csv <- baseline-r1.csv (single-capture alias for deterministic figures)"
    fi
  fi
fi

# =============================================================================
# Stage D — ablation captures (3-solver corpus). Combines the old single-capture
#   ablation stage with the medianed replay so each condition is patched/compiled
#   exactly ONCE:
#     MEDIAN_CONDS → $REPLAY_REPEATS captures into replay-<ts>/<cond>-r*.csv (the
#                    medianed per-iteration tables read these); run 1 is copied to
#                    $DATA_DIR/<cond>.csv for the figures + (deterministic) iters.
#     SINGLE_CONDS → one capture into $DATA_DIR/<cond>.csv.
#   Diffs: $DIFF_DIR/<name>.modification.diff  (or set ABLATION_MECH=flag).
# =============================================================================
if have_stage D; then
  ts="$(date -u +%Y%m%dT%H%M%SZ)"; rep="$DATA_DIR/replay-$ts"; mkdir -p "$rep"
  log "Stage D: ablations (mech=$ABLATION_MECH); replay dir $rep"

  for cond in $MEDIAN_CONDS; do
    [[ -s "$DATA_DIR/$cond.csv" && $FORCE -eq 0 ]] && { log "skip ablation $cond (exists)"; continue; }
    apply_ablation "$cond" || { log "WARN skipping $cond (apply failed)"; continue; }
    for r in $(seq 1 "$REPLAY_REPEATS"); do
      run_mvn "$rep/$cond-r$r.log" test -DskipTests=false -Dtest=SolverLNSimpleEvaluationTest \
        -Dbaseline.outdir="$rep" -Dbaseline.dataset="$cond-r$r" -Dbaseline.condition="$cond" \
        || log "WARN $cond-r$r non-zero"
    done
    revert_ablation
    # Representative single capture for the figures + (deterministic) iteration counts.
    [[ $DRY -eq 0 && -s "$rep/$cond-r1.csv" ]] && { cp "$rep/$cond-r1.csv" "$DATA_DIR/$cond.csv"; assert_rows "$DATA_DIR/$cond.csv" || true; }
  done

  for cond in $SINGLE_CONDS; do
    apply_ablation "$cond" || { log "WARN skipping $cond (apply failed)"; continue; }
    run_capture "$cond" "$cond"
    revert_ablation
  done
  log "replay dir: $rep (medianed per-iteration F17/F24 tables; matching N=$BASELINE_REPEATS baseline-r* reference)"
fi

# =============================================================================
# Stage H — FastEval contribution-optimisation studies (LNSimple-only, quiet HW)
# =============================================================================
if have_stage H; then
  log "Stage H: FastEval optimisation studies (contribution DRAFT tables)"
  perf_dir="$REPORT_DIR/data/overnight-exploration/perf-port"; mkdir -p "$perf_dir"
  REF="$REPORT_DIR/data/overnight-exploration/data/ln-reference.tables.csv"
  # LQNS ref: freshest f20axis baseline tables (LQNS rows), else the bundled one.
  axdir="$(latest_f20axis)"
  LQNSREF="${axdir:+$axdir/baseline.tables.csv}"
  [[ -s "$LQNSREF" ]] || LQNSREF="$REF"

  # Ensure a current-HEAD LN reference exists for the correctness gate.
  if [[ ! -s "$REF" ]]; then
    log "Stage H: capturing LN reference -> $REF"
    run_table_capture "$LOG_DIR/h-ln-ref.log" "$REF" CaptureLnReference \
      -Dcaptureln.out="$REF" -Dcapture.condition=baseline -Dcapture.fixtures="$CAPTURE_FIXTURES"
  fi

  # run_fasteval <out-stem> <extra -D flags...>
  run_fasteval(){ local stem="$1"; shift; local out="$perf_dir/$stem.csv"
    if skip_if_done "$out"; then return 0; fi
    run_mvn "$LOG_DIR/fasteval-$stem.log" test -DskipTests=false -Dtest=FastEval \
      -Dfasteval.ref="$REF" -Dfasteval.lqnsref="$LQNSREF" \
      -Dfasteval.repeats="${FASTEVAL_REPEATS:-5}" -Dfasteval.warmup=3 \
      -Dfasteval.out="$out" "$@" || log "WARN FastEval $stem failed (verify -Dlnsimple.* flag names against current SolverLNSimple)"
  }

  # The -Dlnsimple.{zalpha,dalpha,tol,sweep} flags below are wired into
  # SolverLNSimple via FastEval (setZRelaxAlpha/setDRelaxAlpha/setSweepOrder +
  # the tol overload of iterateCoupledMva). Only the genuinely-tunable studies
  # are kept: the alpha-tol grid and the sweep-order study. The warmstart and
  # accelerator studies are NOT regenerated here — those experiments were run by
  # editing+recompiling the solver and the code was removed, so they cannot be
  # reproduced by a runtime flag. Their report numbers are kept as-is (from the
  # original manual exploration). The cache ablation is likewise out of scope.
  if [[ $DRY -eq 0 ]]; then
    # Headline operating points: optimised default (solver defaults: zalpha=0.5,
    # dalpha=1.0, tol=1.5e-2) vs the pre-optimisation master settings.
    run_fasteval 00-optimised-default
    run_fasteval 01-master-settings -Dlnsimple.zalpha=0.5 -Dlnsimple.dalpha=0.5 -Dlnsimple.tol=5e-3

    # Joint (alpha, tol) sweep -> Table DRAFT-alpha-tol.
    for a in 0.5 0.7 1.0 1.2 1.5; do
      for t in 5e-3 7.5e-3 1e-2 1.5e-2; do
        run_fasteval "alphatol-a${a}-t${t}" -Dlnsimple.zalpha="$a" -Dlnsimple.dalpha="$a" -Dlnsimple.tol="$t"
      done
    done

    # Sweep-order study -> Table DRAFT-sweep.
    for s in bounce forward reverse double-forward elevator; do
      run_fasteval "sweep-$s" -Dlnsimple.sweep="$s"
    done
  fi
fi

# #############################################################################
#  ACCURACY PHASE — timing-insensitive captures + derived outputs, run last.
#  Order: F (LN-vs-LN drift + lqsim) → G (probes) → I (figures) → J (validation).
# #############################################################################

# =============================================================================
# Stage F — F20-axis bundle (.tables.csv) + lqsim, then drift analysis
#   Only f20off is captured (the 4-ablation scope). The drift table's ΔArmA
#   column needs an A2_arm_A capture — out of scope here; ln_drift_analysis
#   tolerates its absence (the baseline-vs-f20off LN drift is the main signal).
# =============================================================================
if have_stage F; then
  ts="$(date -u +%Y%m%dT%H%M%SZ)"; ax="$DATA_DIR/f20axis-$ts"; mkdir -p "$ax"
  log "Stage F: f20-axis tables bundle -> $ax"

  # LN tables under baseline (clean solver — no ablation applied).
  run_table_capture "$LOG_DIR/f20axis-baseline.log" "$ax/baseline.tables.csv" CaptureLnReference \
    -Dcaptureln.out="$ax/baseline.tables.csv" -Dcapture.condition=baseline \
    -Dcapture.fixtures="$CAPTURE_FIXTURES"

  # LN tables under f20off — capture ONLY if the ablation actually applied,
  # else the un-ablated solver would be silently mislabelled.
  for cond in f20off; do
    out="$ax/$cond.tables.csv"
    if skip_if_done "$out"; then continue; fi
    if apply_ablation "$cond"; then
      run_table_capture "$LOG_DIR/f20axis-$cond.log" "$out" CaptureLnReference \
        -Dcaptureln.out="$out" -Dcapture.condition="$cond" -Dcapture.fixtures="$CAPTURE_FIXTURES"
      revert_ablation
    else
      log "WARN F20-axis: $cond ablation failed to apply — skipping its tables capture (no mislabelled data written)"
    fi
  done

  # lqsim ground-truth reference (DISCRETE-EVENT simulator, not analytic LQNS).
  run_table_capture "$LOG_DIR/f20axis-lqsim.log" "$ax/lqsim-reference.tables.csv" CaptureLqsimReference \
    -Dlqsim.out="$ax/lqsim-reference.tables.csv" -Dlqsim.samples="$LQSIM_SAMPLES" \
    -Dlqsim.blocks="$LQSIM_BLOCKS" \
    -Dcapture.condition=baseline -Dcapture.fixtures="$LQSIM_FIXTURES"

  if [[ $DRY -eq 0 ]]; then python3 "$FIG_SCRIPTS/ln_drift_analysis.py" "$ax" || log "WARN ln_drift_analysis failed"; fi
fi

# =============================================================================
# Stage G — probes
# =============================================================================
if have_stage G; then
  log "Stage G: probes"
  run_mvn "$LOG_DIR/interlock-probe.log" test -DskipTests=false \
    -Dsurefire.failIfNoSpecifiedTests=false \
    -Dtest=SolverLNInterlockAccuracyTest || log "WARN interlock probe"
  run_mvn "$LOG_DIR/lqns-overhead.log" test -DskipTests=false \
    -Dsurefire.failIfNoSpecifiedTests=false \
    -Dtest=SolverLqnsRestOverheadTest || log "WARN lqns overhead"
fi

# =============================================================================
# Stage I — regenerate all figures (runs last, after every capture exists).
#   Baseline-only: e3.1 e4.1 e5.2 e5.5 e10.1
#   Ablation-dependent: e6.1 (f17off/f24off) e7.1 (f24off/f20off/alloff) e9.1 (alloff)
#   All inputs are produced by Stage D's f24off/f20off/f17off/alloff captures.
# =============================================================================
if have_stage I; then
  log "Stage I: figure regen"
  for s in e3_1_partition_a_reldiff e4_1_iters_by_solver e5_2_bracket_compare \
           e5_5_lqns_no_rest e10_1_bfanout_reldiff \
           e6_1_interlock_speedup e7_1_iter_ablation e9_1_gap_closure; do
    if [[ $DRY -eq 0 ]]; then python3 "$FIG_SCRIPTS/$s.py" || log "WARN $s skipped (missing input?)"; fi
  done
fi

# =============================================================================
# Stage J — validation summary (recompute headline numbers from fresh CSVs)
# =============================================================================
if have_stage J; then
  log "Stage J: validation summary"
  summary="$DATA_DIR/refresh-summary.md"
  axdir="$(latest_f20axis)"
  if [[ $DRY -eq 0 ]]; then
    python3 - "$DATA_DIR" "$summary" "${axdir:-}" <<'PY'
from pathlib import Path
import sys

data_dir = Path(sys.argv[1])
summary_path = Path(sys.argv[2])
f20 = Path(sys.argv[3]) if len(sys.argv) > 3 and sys.argv[3] else None

def rows(path: Path) -> int:
    if not path.exists():
        return 0
    with path.open() as f:
        return max(sum(1 for _ in f) - 1, 0)

lines = []
lines.append("# Refresh Summary")
lines.append("")
for name in ["baseline.csv", "baseline-medianed.csv", "f20off.csv", "A2_arm_A.csv", "f24off.csv", "f17off.csv", "f17_f24off.csv", "alloff.csv"]:
    p = data_dir / name
    lines.append(f"- {name}: {rows(p)} rows")

if f20 is not None and f20.exists():
    lines.append(f"- f20axis bundle: {f20.name}")
    for name in ["baseline.tables.csv", "f20off.tables.csv", "A2_arm_A.tables.csv", "lqsim-reference.tables.csv", "ln_drift_summary.csv"]:
        lines.append(f"  - {name}: {rows(f20 / name)} rows")
else:
    lines.append("- f20axis bundle: NONE FOUND (Stage F not run?)")

lines.append("")
lines.append("## Stale Text To Update")
lines.append("- report/source/introduction/introduction.tex")
lines.append("- report/source/conclusion/conclusion.tex")
lines.append("- report/source/evaluation/evaluation.tex")

summary_path.write_text("\n".join(lines) + "\n")
print(summary_path)
PY
    log "Wrote summary: $summary"
  fi
  log "STALE TEXT TO UPDATE: report/source/introduction/introduction.tex; report/source/conclusion/conclusion.tex; report/source/evaluation/evaluation.tex"
fi

log "refresh-overnight.sh complete."
