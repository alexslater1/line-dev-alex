"""Figure E5.5 (TEMP / diagnostic) — per-fixture runtime with LQNS
shown net of the REST round-trip overhead.

Same axes, panels, and Sim/LN points as E5.2. Only the LQNS series is
modified: per fixture, we subtract the per-fixture median
``rest_call_ms`` measured by ``SolverLqnsRestOverheadTest`` (5 reps per
fixture, captured into ``report/data/lqns-overhead-smoke.csv``). This
isolates the work LQNS would do if it ran in-process — kernel solve plus
local LQXO parsing plus per-call wrappers — and removes the HTTP POST +
container fork/exec floor.

Outputs:
  * eval-e52b-lqns-no-rest.pdf  : the new scatter
  * stdout: aggregate totals so the operator can compare against the
    headline ``Sim total``, ``LQNS total``, ``LN total`` numbers cited
    in §8.5.
"""

from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

from _style import (
    ANNOT_PT,
    PARTITION_ORDER,
    REF_LINE_COLOR,
    SOLVER_COLORS,
    WIDE_IN,
    DATA_DIR,
    configure,
    save,
)

configure()

OVERHEAD_CSV = DATA_DIR / "lqns-overhead-smoke.csv"

# Measured median REST round-trip overhead per LQNS call (HTTP POST +
# container fork/exec, exclusive of the kernel solve). Used as a flat
# per-call correction when no per-fixture overhead capture is available.
REST_OVERHEAD_MS = 20.0

baseline = pd.read_csv(DATA_DIR / "baseline-medianed.csv")

# Per-fixture median REST overhead, subtracted from each baseline LQNS
# point. Method: for every overhead row compute (rest_call - elapsed)
# — the HTTP transport + container fork/exec, exclusive of the remote
# solve — then median over the five reps per fixture. The result is
# subtracted from the baseline-medianed lqns_time_ms, keeping the
# baseline run as the reference frame so Sim, LN, and LQNS series are
# all anchored in the same medianed capture.
has_baseline_overhead = (
    {"lqns_rest_call_ms", "lqns_elapsed_ms"}.issubset(baseline.columns)
    and baseline["lqns_rest_call_ms"].notna().any()
)

if has_baseline_overhead:
    print("[e5_5] data source: baseline-medianed.csv (self-consistent)")
    baseline["rest_overhead_ms_med"] = (
        baseline["lqns_rest_call_ms"] - baseline["lqns_elapsed_ms"].fillna(0)
    ).clip(lower=0)
    df = baseline
elif not OVERHEAD_CSV.exists():
    print(f"[e5_5] no per-call overhead capture; applying flat "
          f"{REST_OVERHEAD_MS:.0f} ms/call correction to every LQNS-solvable fixture")
    baseline["rest_overhead_ms_med"] = np.where(
        baseline["lqns_time_ms"].notna(), REST_OVERHEAD_MS, np.nan)
    df = baseline
else:
    print(f"[e5_5] data source: {OVERHEAD_CSV.name} for overhead; "
          "baseline-medianed for totals")
    overhead = pd.read_csv(OVERHEAD_CSV)
    per_call = overhead.dropna(subset=["rest_call_ms"]).copy()
    per_call["rest_overhead_ms"] = (
        per_call["rest_call_ms"] - per_call["lqns_elapsed_ms"].fillna(0)
    ).clip(lower=0)
    rest_med = (
        per_call.groupby("fixture")["rest_overhead_ms"]
        .median()
        .rename("rest_overhead_ms_med")
    )
    print(f"[e5_5] overhead CSV: {len(overhead)} rows -> "
          f"{len(rest_med)} fixtures with rest overhead")
    df = baseline.merge(rest_med, left_on="fixture_name", right_index=True, how="left")

missing = df[df["rest_overhead_ms_med"].isna()]["fixture_name"].tolist()
if missing:
    print(f"[e5_5] WARN: {len(missing)} fixtures have no rest overhead data: {missing[:5]}...")

# LQNS, net of REST: clip at 1 ms (log-scale floor) so degenerate cases
# where the kernel itself was sub-ms still appear on the plot.
df["lqns_no_rest_ms"] = (df["lqns_time_ms"] - df["rest_overhead_ms_med"]).clip(lower=1.0)

records = []
for _, row in df.iterrows():
    records.append({"fixture_name": row["fixture_name"],
                    "partition": row["partition"],
                    "solver": "SolverLNSimple",
                    "time_ms": float(row["sim_time_ms"])})
    records.append({"fixture_name": row["fixture_name"],
                    "partition": row["partition"],
                    "solver": "SolverLN",
                    "time_ms": float(row["ln_time_ms"])})
    if pd.notna(row.get("lqns_no_rest_ms")):
        records.append({"fixture_name": row["fixture_name"],
                        "partition": row["partition"],
                        "solver": "LQNS V5 (net of REST)",
                        "time_ms": float(row["lqns_no_rest_ms"])})

long = pd.DataFrame.from_records(records)
long["time_ms"] = long["time_ms"].clip(lower=1.0)

fig, ax = plt.subplots(figsize=(WIDE_IN, 3.5))

rng = np.random.default_rng(seed=2)
solver_order = ["SolverLNSimple", "SolverLN", "LQNS V5 (net of REST)"]
# Reuse the same green for the corrected LQNS so the eye lines it up
# with the baseline figure visually.
color_for = {
    "SolverLNSimple":          SOLVER_COLORS["SolverLNSimple"],
    "SolverLN":                SOLVER_COLORS["SolverLN"],
    "LQNS V5 (net of REST)":   SOLVER_COLORS["LQNS V5"],
}

for part_idx, partition in enumerate(PARTITION_ORDER):
    sub = long[long["partition"] == partition]
    for solver in solver_order:
        rows = sub[sub["solver"] == solver]
        if rows.empty:
            continue
        x = part_idx + rng.uniform(-0.18, 0.18, size=len(rows))
        ax.scatter(x, rows["time_ms"],
                   s=24, color=color_for[solver], alpha=0.78,
                   edgecolor="white", linewidth=0.4,
                   label=solver if part_idx == 0 else None, zorder=3)

ax.axhline(1000, linestyle=":", color=REF_LINE_COLOR, linewidth=0.6, zorder=1)
ax.text(len(PARTITION_ORDER) - 0.5, 1000 * 1.15, "1 s",
        fontsize=ANNOT_PT, color=REF_LINE_COLOR, ha="right", va="bottom")
ax.set_yscale("log")
ax.set_xticks(range(len(PARTITION_ORDER)))
ax.set_xticklabels(PARTITION_ORDER)
ax.set_xlim(-0.55, len(PARTITION_ORDER) - 0.45)
ax.set_xlabel("Partition")
ax.set_ylabel("Wall-clock runtime (ms, log scale)")
ax.grid(axis="y", linestyle=":", linewidth=0.4, alpha=0.5, zorder=1)
ax.legend(loc="upper left", ncol=3, columnspacing=1.0, handletextpad=0.3)

out = save(fig, "eval-e52b-lqns-no-rest")
print(f"Wrote {out}\n")

# Aggregate totals so the user can sanity-check the correction.
sim_tot       = int(df["sim_time_ms"].sum())
ln_tot        = int(df["ln_time_ms"].sum())
lqns_tot      = int(df["lqns_time_ms"].fillna(0).sum())
rest_med_tot  = int(df["rest_overhead_ms_med"].fillna(0).sum())
# Use the unclipped column for aggregate accounting: the 1-ms log floor
# is a plot artifact, not a real time.
lqns_corr_raw = (df["lqns_time_ms"] - df["rest_overhead_ms_med"]).fillna(0).clip(lower=0)
lqns_corr_tot = int(lqns_corr_raw.sum())

print(f"[summary] Sim total            = {sim_tot:>9,} ms")
print(f"[summary] LN total             = {ln_tot:>9,} ms")
print(f"[summary] LQNS total (raw)     = {lqns_tot:>9,} ms")
print(f"[summary] LQNS total (no REST) = {lqns_corr_tot:>9,} ms  "
      f"(removed {rest_med_tot:,} ms = {rest_med_tot/max(lqns_tot,1):.1%} of LQNS)")
print(f"[summary] LN  / LQNS (raw)     = {ln_tot/max(lqns_tot,1):.2f}x")
print(f"[summary] LN  / LQNS (no REST) = {ln_tot/max(lqns_corr_tot,1):.2f}x")
print(f"[summary] Sim / LQNS (no REST) = {sim_tot/max(lqns_corr_tot,1):.2f}x")
