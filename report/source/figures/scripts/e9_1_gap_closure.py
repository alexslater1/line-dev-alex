"""Figure E9.1 — per-partition iteration-count gap decomposition.

For every partition (and a TOTAL row), plot a stacked horizontal
bar where the total length is ``baseline.ln_total − sim_total``
(the SolverLN-vs-SolverLNSimple iteration gap). The bar is split
into a light "closed by convergence-control cut" portion
(``baseline − alloff``) and a dark "residual algorithmic
difference" portion (``alloff − sim``). Every partition has a
positive residual on the current corpus, so all bars extend
rightward.
"""

from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

from _style import (
    ANNOT_PT,
    PARTITION_ORDER,
    REF_LINE_COLOR,
    TEXTWIDTH_IN,
    DATA_DIR,
    configure,
    save,
)

configure()

baseline = pd.read_csv(DATA_DIR / "baseline.csv")
alloff = pd.read_csv(DATA_DIR / "alloff.csv")

frame = baseline[["fixture_name", "partition", "sim_iters", "ln_iters"]].rename(
    columns={"ln_iters": "base"}
)
frame = frame.merge(
    alloff[["fixture_name", "ln_iters"]].rename(columns={"ln_iters": "alloff"}),
    on="fixture_name",
)

agg = (
    frame.groupby("partition")
    .agg(sim=("sim_iters", "sum"), base=("base", "sum"), alloff=("alloff", "sum"))
    .reindex(PARTITION_ORDER)
)
agg.loc["TOTAL"] = agg.sum(numeric_only=True)
agg["closed"] = agg["base"] - agg["alloff"]
agg["residual"] = agg["alloff"] - agg["sim"]
agg["gap_total"] = agg["base"] - agg["sim"]
agg["closure_pct"] = 100 * agg["closed"] / agg["gap_total"]

rows = list(agg.index)
ys = np.arange(len(rows))
bar_h = 0.62

closed_colour = "#9ecae1"   # light blue — "closed by ablation"
residual_colour = "#08519c" # dark blue — "residual algorithmic difference"

fig, ax = plt.subplots(figsize=(TEXTWIDTH_IN, 4.0))

for y, partition in zip(ys, rows):
    closed = agg.loc[partition, "closed"]
    residual = agg.loc[partition, "residual"]
    closure_pct = agg.loc[partition, "closure_pct"]
    # The "closed by ablation" portion always extends rightward from 0.
    ax.barh(y, closed, height=bar_h, color=closed_colour, edgecolor="white",
            linewidth=0.4, zorder=3,
            label="closed by convergence-control cut" if partition == rows[0] else None)
    # The residual is stacked on the right; if negative (A4 cap-anomaly)
    # it extends leftward from the right edge.
    ax.barh(y, residual, height=bar_h, left=closed, color=residual_colour,
            edgecolor="white", linewidth=0.4, zorder=3,
            label="residual algorithmic difference" if partition == rows[0] else None)
    # Caption-style closure percent on the right.
    end = closed + max(0.0, residual)
    ax.text(end + 30, y, f"{closure_pct:.1f}% closure",
            fontsize=ANNOT_PT, color="#333333", va="center", ha="left")

# Emphasise the TOTAL row.
total_y = ys[-1]
ax.axhline(total_y - 0.5, color="#cccccc", linewidth=0.5, zorder=1)

ax.set_yticks(ys)
ax.set_yticklabels(rows)
for label in ax.get_yticklabels():
    if label.get_text() == "TOTAL":
        label.set_fontweight("bold")
ax.invert_yaxis()
ax.set_xlabel("Outer-iteration gap (SolverLN total $-$ SolverLNSimple total)")
ax.grid(axis="x", linestyle=":", linewidth=0.4, alpha=0.5, zorder=1)
ax.legend(loc="lower right", bbox_to_anchor=(1.0, 1.001), ncol=2,
          columnspacing=1.0, handletextpad=0.4)
ax.set_xlim(left=min(0, agg["closed"].min()) * 1.05,
            right=agg["gap_total"].max() * 1.28)

out = save(fig, "eval-e91-gap-closure-decomposition")
print(f"Wrote {out}")
