"""Figure E4.1 — per-fixture iteration counts, faceted by partition.

Reads report/data/baseline.csv and shows three solver series
(SolverLNSimple blue, SolverLN red, LQNS V5 green) per fixture,
arranged by partition along the X-axis with a log Y-scale.
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

df = pd.read_csv(DATA_DIR / "baseline.csv")

# Compose a long-form frame with one row per (fixture, solver) pair so
# we can colour-encode by solver.
records = []
for _, row in df.iterrows():
    records.append({
        "fixture_name": row["fixture_name"],
        "partition": row["partition"],
        "solver": "SolverLNSimple",
        "iters": float(row["sim_iters"]),
    })
    records.append({
        "fixture_name": row["fixture_name"],
        "partition": row["partition"],
        "solver": "SolverLN",
        "iters": float(row["ln_iters"]),
    })
    if pd.notna(row["lqns_iters"]):
        records.append({
            "fixture_name": row["fixture_name"],
            "partition": row["partition"],
            "solver": "LQNS V5",
            "iters": float(row["lqns_iters"]),
        })

long = pd.DataFrame.from_records(records)

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
        ax.scatter(
            x,
            rows["iters"],
            s=24,
            color=SOLVER_COLORS[solver],
            alpha=0.78,
            edgecolor="white",
            linewidth=0.4,
            zorder=3,
        )

# Mark the SolverLN "floor at 53" — visible as a horizontal cluster of
# red dots across most partitions.
ax.axhline(53, linestyle=":", color=REF_LINE_COLOR, linewidth=0.6, zorder=1)
ax.text(
    len(PARTITION_ORDER) - 0.5,
    53 * 1.08,
    "SolverLN iter_min floor (53)",
    fontsize=ANNOT_PT,
    color=REF_LINE_COLOR,
    ha="right",
    va="bottom",
)

ax.set_yscale("log")
ax.set_xticks(range(len(PARTITION_ORDER)))
ax.set_xticklabels(PARTITION_ORDER)
ax.set_xlim(-0.55, len(PARTITION_ORDER) - 0.45)
ax.set_xlabel("Partition")
ax.set_ylabel("Outer-loop iteration count (log scale)")
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

out = save(fig, "eval-e41-iters-by-solver")
print(f"Wrote {out}")
