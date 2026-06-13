"""Figure E5.2 + tables - re-render §8.5's runtime artifacts using the
fair-bracket SolverLNSimple values.

Data source resolution (in priority order):
  1. baseline-medianed.csv (overnight 4-run median, produced by e5_4_median.py)
     — preferred: medianed Sim/LN/LQNS all from same environment.
  2. baseline-fairsim.csv (single-run sim-only fair bracket)
     + backup-pre-lqns/baseline.csv for LN/LQNS (single original run).
     — fallback used when no overnight capture is available yet.

Produces:
  * eval-e52-fair-runtime-by-solver.pdf  : per-fixture scatter, parallel
                                            structure to figure E5.1.
  * stdout: LaTeX-ready rows for the
    "Per-partition runtime" and "Per-iteration runtime (Partition B)"
    tables — paste into evaluation.tex.
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


def load_data():
    """Resolve the source CSV(s). Returns (df, mode) where mode is
    "medianed" or "single-run". In both modes the returned df has
    sim_time_ms / ln_time_ms / lqns_time_ms columns ready to consume."""
    medianed = DATA_DIR / "baseline-medianed.csv"
    if medianed.exists():
        df = pd.read_csv(medianed)
        # Hand back IQR columns too so the summary can print noise bands.
        return df, "medianed"

    print("[e5_2] no baseline-medianed.csv — falling back to single-run "
          "(baseline-fairsim.csv + backup-pre-lqns/baseline.csv)")
    old = pd.read_csv(DATA_DIR / "backup-pre-lqns" / "baseline.csv")
    fair = pd.read_csv(DATA_DIR / "baseline-fairsim.csv")[
        ["fixture_name", "sim_iters", "sim_time_ms"]
    ].rename(columns={"sim_iters": "sim_iters_fair", "sim_time_ms": "sim_time_ms_fair"})
    df = old.merge(fair, on="fixture_name", how="left")
    # Swap the Sim columns to the fair-bracket values; carry LN/LQNS through.
    df["sim_time_ms"] = df["sim_time_ms_fair"].fillna(df["sim_time_ms"])
    df["sim_iters"] = df["sim_iters_fair"].fillna(df["sim_iters"])
    return df, "single-run"


df, mode = load_data()
print(f"[e5_2] data source: {mode} ({len(df)} fixtures)\n")

# =============================================================================
# Figure E5.2: per-fixture scatter — parallel to E5.1.
# =============================================================================
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
    if pd.notna(row.get("lqns_time_ms")):
        records.append({"fixture_name": row["fixture_name"],
                        "partition": row["partition"],
                        "solver": "LQNS V5",
                        "time_ms": float(row["lqns_time_ms"])})

long = pd.DataFrame.from_records(records)
long["time_ms"] = long["time_ms"].clip(lower=1.0)

fig, ax = plt.subplots(figsize=(WIDE_IN, 3.5))

rng = np.random.default_rng(seed=2)
solver_order = ["SolverLNSimple", "SolverLN", "LQNS V5"]

for part_idx, partition in enumerate(PARTITION_ORDER):
    sub = long[long["partition"] == partition]
    for solver in solver_order:
        rows = sub[sub["solver"] == solver]
        if rows.empty:
            continue
        x = part_idx + rng.uniform(-0.18, 0.18, size=len(rows))
        ax.scatter(x, rows["time_ms"],
                   s=24, color=SOLVER_COLORS[solver], alpha=0.78,
                   edgecolor="white", linewidth=0.4, zorder=3)

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
legend_handles = [
    plt.Line2D(
        [0], [0],
        marker="s",
        linestyle="none",
        markersize=7,
        markerfacecolor=SOLVER_COLORS[solver],
        markeredgecolor="white",
        markeredgewidth=0.4,
        label=solver,
    )
    for solver in solver_order
]
ax.legend(
    handles=legend_handles,
    loc="upper right",
    ncol=1,
    handletextpad=0.3,
)

out = save(fig, "eval-e52-fair-runtime-by-solver")
print(f"Wrote {out}\n")

# =============================================================================
# Per-partition wall-clock totals/means table (paste into evaluation.tex).
# =============================================================================
PARTITION_LABEL = {
    "A1": "A1 (single-class chains)",
    "A2": "A2 (multi-class shared callee)",
    "A3": "A3 (multi-server PS)",
    "A4": "A4 (mixed topology)",
    "A5": "A5 (activity-graph precedence)",
    "B":  "B (F24 scale axis)",
    "C1": "C1 (fan-out callers)",
    "C2": "C2 (INF-task hosts)",
    "C3": "C3 (multi-class \\texttimes{} multi-entry)",
    "C4": "C4 (boundary cases)",
    "C5": "C5 (saturated fan-out)",
    "C6": "C6 (AND-fork join boundary)",
    "C7": "C7 (deep chains)",
}
print("% --- per-partition wall-clock (Sim / LQNS / LN) ---")
# LQNS means divide by the count of LQNS-solvable fixtures in the partition
# (A5_combinedDag is LQNS-unavailable), matching the iteration-count table's
# per-solvable convention. Sim/LN means divide by the full partition N.
totals = {"N": 0, "sim": 0, "lqns": 0, "ln": 0, "lqns_n": 0}
for p in PARTITION_ORDER:
    sub = df[df["partition"] == p]
    N = len(sub)
    n_lqns = int(sub["lqns_time_ms"].notna().sum())
    sim_t  = int(sub["sim_time_ms"].sum())
    lqns_t = int(sub["lqns_time_ms"].fillna(0).sum())
    ln_t   = int(sub["ln_time_ms"].sum())
    totals["N"]      += N
    totals["sim"]    += sim_t
    totals["lqns"]   += lqns_t
    totals["ln"]     += ln_t
    totals["lqns_n"] += n_lqns
    sim_m  = round(sim_t  / N) if N else 0
    lqns_m = round(lqns_t / n_lqns) if n_lqns else 0
    ln_m   = round(ln_t   / N) if N else 0
    print(f"{PARTITION_LABEL[p]} & {N} & {sim_t:,} & {lqns_t:,} & {ln_t:,} & "
          f"{sim_m:,} & {lqns_m:,} & {ln_m:,} \\\\ \\hline")
N_all = totals["N"]
print(f"\\textbf{{TOTAL}} & \\textbf{{{N_all}}} & "
      f"\\textbf{{{totals['sim']:,}}} & \\textbf{{{totals['lqns']:,}}} & "
      f"\\textbf{{{totals['ln']:,}}} & "
      f"\\textbf{{{round(totals['sim']/N_all):,}}} & "
      f"\\textbf{{{round(totals['lqns']/totals['lqns_n']):,}}} & "
      f"\\textbf{{{round(totals['ln']/N_all):,}}} \\\\ \\hline")

# =============================================================================
# Per-iteration Partition B (paste into evaluation.tex).
# =============================================================================
B_ORDER = [
    "B_scaleAxis_tasks5_chain",   "B_scaleAxis_tasks5_fanout",
    "B_scaleAxis_tasks10_chain",  "B_scaleAxis_tasks10_fanout",
    "B_scaleAxis_tasks20_chain",  "B_scaleAxis_tasks20_fanout",
    "B_scaleAxis_tasks40_chain",  "B_scaleAxis_tasks40_fanout",
    "B_scaleAxis_tasks60_chain",  "B_scaleAxis_tasks60_fanout",
]
print("\n% --- per-iteration runtime on Partition B ---")
b = df.set_index("fixture_name").reindex(B_ORDER)
for fx in B_ORDER:
    row = b.loc[fx]
    if row.isna().get("ln_iters", True):
        print(f"% {fx}: missing from dataset")
        continue
    ln_it     = int(row["ln_iters"])
    ln_ms_it  = row["ln_time_ms"] / ln_it
    lqns_it   = int(row["lqns_iters"])
    lqns_ms_it = row["lqns_time_ms"] / lqns_it
    sim_it    = int(row["sim_iters"])
    sim_ms_it = row["sim_time_ms"] / sim_it
    cells = [ln_ms_it, lqns_ms_it, sim_ms_it]
    minidx = int(np.argmin(cells))
    fmt = []
    for i, val in enumerate(cells):
        body = f"{val:,.1f}" if val >= 100 else f"{val:.2f}"
        fmt.append(f"\\textbf{{{body}}}" if i == minidx else body)
    fx_disp = "\\texttt{" + fx.replace("_", "\\_") + "}"
    print(f"{fx_disp} & {ln_it} & {fmt[0]} & {lqns_it} & {fmt[1]} & {fmt[2]} & "
          f"{ln_ms_it/lqns_ms_it:.1f}x & {ln_ms_it/sim_ms_it:.1f}x \\\\ \\hline")

# =============================================================================
# Summary block including IQR noise bands when medianed data is in use.
# =============================================================================
sim_tot, lqns_tot, ln_tot = totals["sim"], totals["lqns"], totals["ln"]
print(f"\n[summary] mode={mode}")
print(f"  Sim total  = {sim_tot:,} ms")
print(f"  LQNS total = {lqns_tot:,} ms")
print(f"  LN total   = {ln_tot:,} ms")
print(f"  LN / Sim   = {ln_tot/max(sim_tot,1):.1f}x")
print(f"  LQNS / Sim = {lqns_tot/max(sim_tot,1):.2f}x")
print(f"  LN / LQNS  = {ln_tot/max(lqns_tot,1):.1f}x")

if mode == "medianed" and "sim_time_ms_q25" in df.columns:
    s_q1 = int(df["sim_time_ms_q25"].sum())
    s_q3 = int(df["sim_time_ms_q75"].sum())
    l_q1 = int(df["lqns_time_ms_q25"].fillna(0).sum())
    l_q3 = int(df["lqns_time_ms_q75"].fillna(0).sum())
    n_q1 = int(df["ln_time_ms_q25"].sum())
    n_q3 = int(df["ln_time_ms_q75"].sum())
    print("\n[summary] IQR (sum of per-fixture quartiles, ms):")
    print(f"  Sim  IQR=[{s_q1:,}, {s_q3:,}]   width/median={(s_q3-s_q1)/max(sim_tot,1):.0%}")
    print(f"  LQNS IQR=[{l_q1:,}, {l_q3:,}]   width/median={(l_q3-l_q1)/max(lqns_tot,1):.0%}")
    print(f"  LN   IQR=[{n_q1:,}, {n_q3:,}]   width/median={(n_q3-n_q1)/max(ln_tot,1):.0%}")
