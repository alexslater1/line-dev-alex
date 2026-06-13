"""Figure E3.1 — Partition A per-fixture max rel diff dot plot.

Reads report/data/baseline.csv, filters to Partition A
(A1/A2/A3/A4), and plots ``sim_vs_ln_max_rel_diff`` on a log
Y-scale coloured by subpartition. A dashed horizontal reference
line marks ``rtol = 5e-2``.
"""

from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

from _style import (
    ANNOT_PT,
    FIG_DIR,
    PARTITION_COLOR,
    REF_LINE_COLOR,
    RTOL,
    TEXTWIDTH_IN,
    DATA_DIR,
    configure,
    save,
)

configure()

df = pd.read_csv(DATA_DIR / "baseline.csv")
df = df[df["partition"].str.startswith("A")].copy()
df = df.sort_values(["partition", "fixture_name"]).reset_index(drop=True)

# The rel-diff metric can equal zero; clip to a very small floor so it
# renders on the log axis without distorting the visible range.
rd = df["sim_vs_ln_max_rel_diff"].clip(lower=1e-9)

fig, ax = plt.subplots(figsize=(TEXTWIDTH_IN, 3.4))

# Stable horizontal positions per subpartition (jittered within band).
sub_order = ["A1", "A2", "A3", "A4", "A5"]
rng = np.random.default_rng(seed=1)
x_centre = {p: i for i, p in enumerate(sub_order)}

for sub in sub_order:
    mask = df["partition"] == sub
    n = int(mask.sum())
    xs = x_centre[sub] + rng.uniform(-0.22, 0.22, size=n)
    ax.scatter(
        xs,
        rd[mask],
        s=28,
        color=PARTITION_COLOR[sub],
        alpha=0.85,
        edgecolor="white",
        linewidth=0.5,
        zorder=3,
    )

ax.axhline(
    RTOL,
    linestyle="--",
    color=REF_LINE_COLOR,
    linewidth=0.8,
    zorder=2,
)
ax.text(
    len(sub_order) - 0.55,
    RTOL * 1.18,
    r"$\mathrm{rtol}=5\times 10^{-2}$",
    color=REF_LINE_COLOR,
    fontsize=ANNOT_PT,
    ha="right",
    va="bottom",
)

ax.set_yscale("log")
ax.set_xticks(range(len(sub_order)))
ax.set_xticklabels(sub_order)
ax.set_xlim(-0.6, len(sub_order) - 0.4)
ax.set_xlabel("Subpartition")
ax.set_ylabel(r"Max relative difference vs SolverLN (log scale)")
ax.grid(axis="y", linestyle=":", linewidth=0.4, alpha=0.5, zorder=1)
ax.legend(loc="lower left", ncol=4, columnspacing=0.9, handletextpad=0.3)

out = save(fig, "eval-e31-partition-a-reldiff")
print(f"Wrote {out}")
