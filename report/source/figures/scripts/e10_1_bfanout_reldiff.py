"""Figure E10.1 — B-fanout max relative difference vs task count.

Reads report/data/baseline.csv, filters to the five
``B_scaleAxis_tasks*_fanout`` fixtures, and plots
``sim_vs_ln_max_rel_diff`` against task count. The Y-axis is
log-scaled; a dashed horizontal reference line marks
``rtol = 5e-2``.
"""

from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

from _style import (
    ANNOT_PT,
    REF_LINE_COLOR,
    RTOL,
    TEXTWIDTH_IN,
    DATA_DIR,
    configure,
    save,
)

configure()

df = pd.read_csv(DATA_DIR / "baseline.csv")
mask = (df["partition"] == "B") & df["fixture_name"].str.contains("fanout")
sub = df[mask].copy()
sub["tasks"] = (
    sub["fixture_name"].str.extract(r"tasks(\d+)").astype(int)
)
sub = sub.sort_values("tasks").reset_index(drop=True)

fig, ax = plt.subplots(figsize=(TEXTWIDTH_IN, 3.2))

ax.plot(
    sub["tasks"],
    sub["sim_vs_ln_max_rel_diff"],
    marker="o",
    color="#1f77b4",
    linewidth=1.4,
    markersize=6,
    label=r"max $|sim - LN| / \max(|LN|, 10^{-6})$",
    zorder=3,
)

for _, row in sub.iterrows():
    ax.annotate(
        f"{row['sim_vs_ln_max_rel_diff']:.3f}",
        xy=(row["tasks"], row["sim_vs_ln_max_rel_diff"]),
        xytext=(0, 8),
        textcoords="offset points",
        fontsize=ANNOT_PT,
        color="#333333",
        ha="center",
    )

ax.axhline(RTOL, linestyle="--", color=REF_LINE_COLOR, linewidth=0.8, zorder=2)
ax.text(
    sub["tasks"].max(),
    RTOL * 0.7,
    r"$\mathrm{rtol}=5\times 10^{-2}$",
    color=REF_LINE_COLOR,
    fontsize=ANNOT_PT,
    ha="right",
    va="top",
)

ax.set_yscale("log")
ax.set_xticks(sub["tasks"])
ax.set_xticklabels([str(int(t)) for t in sub["tasks"]])
ax.set_xlabel("Task count (B-fanout fixtures)")
ax.set_ylabel("Max relative difference vs SolverLN (log)")
ax.grid(axis="y", linestyle=":", linewidth=0.4, alpha=0.5, zorder=1)
ax.set_ylim(RTOL * 0.5, sub["sim_vs_ln_max_rel_diff"].max() * 1.6)

out = save(fig, "eval-e101-bfanout-reldiff")
print(f"Wrote {out}")
