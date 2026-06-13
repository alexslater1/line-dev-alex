"""Figure E5.1 - per-fixture wall-clock runtime, faceted by partition.

Reads report/data/baseline-medianed.csv (the 78-fixture corpus, per-run
median timings produced by e5_4_median.py) and shows three solver series
(SolverLNSimple blue, SolverLN red, LQNS V5 green) per fixture, arranged
by partition along the X-axis with a log Y-scale.
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

df = pd.read_csv(DATA_DIR / "baseline-medianed.csv")

records = []
for _, row in df.iterrows():
    records.append({
        "fixture_name": row["fixture_name"],
        "partition": row["partition"],
        "solver": "SolverLNSimple",
        "time_ms": float(row["sim_time_ms"]),
    })
    records.append({
        "fixture_name": row["fixture_name"],
        "partition": row["partition"],
        "solver": "SolverLN",
        "time_ms": float(row["ln_time_ms"]),
    })
    if pd.notna(row["lqns_time_ms"]):
        records.append({
            "fixture_name": row["fixture_name"],
            "partition": row["partition"],
            "solver": "LQNS V5",
            "time_ms": float(row["lqns_time_ms"]),
        })

long = pd.DataFrame.from_records(records)
# Floor at 1 ms so log-scale plots all points (sub-ms rows clamp to 1).
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
        ax.scatter(
            x,
            rows["time_ms"],
            s=24,
            color=SOLVER_COLORS[solver],
            alpha=0.78,
            edgecolor="white",
            linewidth=0.4,
            label=solver if part_idx == 0 else None,
            zorder=3,
        )

# 1-second reference line for orientation.
ax.axhline(1000, linestyle=":", color=REF_LINE_COLOR, linewidth=0.6, zorder=1)
ax.text(
    len(PARTITION_ORDER) - 0.5,
    1000 * 1.15,
    "1 s",
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
ax.set_ylabel("Wall-clock runtime (ms, log scale)")
ax.grid(axis="y", linestyle=":", linewidth=0.4, alpha=0.5, zorder=1)
ax.legend(loc="upper left", ncol=3, columnspacing=1.0, handletextpad=0.3)

out = save(fig, "eval-e51-runtime-by-solver")
print(f"Wrote {out}")
