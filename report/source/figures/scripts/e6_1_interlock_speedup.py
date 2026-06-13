"""Figure E6.1 — interlock-off per-iteration speedup on Partition B.

Single panel showing the per-iteration speedup from disabling the whole
interlock subsystem (the task-caller random walk plus the correction
pipeline), as a function of task count, split by topology (chain vs
fan-out). Ten bars: five task counts (5, 10, 20, 40, 60) crossed with
two shapes. Speedup is ``baseline ms/iter / interlock-off ms/iter`` per
fixture. The interlock-off timing is the median of the two repeated
captures in ``replay-.../f17_f24off-r{1,2}.csv``; the baseline is
``baseline-medianed.csv`` (the same source the runtime section uses).
A reference line at 1.0x marks no per-iteration change. The fan-out
series rises monotonically with width while the chain series stays near
the noise floor: the correction pipeline's per-iteration cost is borne
almost entirely on wide fan-outs.
"""

from pathlib import Path
from statistics import median

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

from _style import (
    ANNOT_PT,
    REF_LINE_COLOR,
    TEXTWIDTH_IN,
    DATA_DIR,
    configure,
    save,
)

configure()

baseline = pd.read_csv(DATA_DIR / "baseline-medianed.csv")
replay_dir = DATA_DIR / "replay-20260610T135432Z"
interlock_runs = [
    pd.read_csv(replay_dir / "f17_f24off-r1.csv"),
    pd.read_csv(replay_dir / "f17_f24off-r2.csv"),
]


def parse(name: str):
    tasks = int([p for p in name.split("_") if p.startswith("tasks")][0].replace("tasks", ""))
    shape = "fanout" if "fanout" in name else "chain"
    return tasks, shape


# Median interlock-off ms/iter across the repeated captures, per fixture.
def ms_per_iter(frame: pd.DataFrame) -> dict:
    return {
        r["fixture_name"]: r["ln_time_ms"] / r["ln_iters"]
        for _, r in frame.iterrows()
    }


interlock_msit = [ms_per_iter(run) for run in interlock_runs]

rows = []
b = baseline[baseline["partition"] == "B"]
for _, r in b.iterrows():
    name = r["fixture_name"]
    tasks, shape = parse(name)
    base_msit = r["ln_time_ms"] / r["ln_iters"]
    x_msit = median(d[name] for d in interlock_msit)
    rows.append({"tasks": tasks, "shape": shape, "speedup": base_msit / x_msit})
frame = pd.DataFrame(rows)

task_counts = sorted(frame["tasks"].unique())
shapes = ["chain", "fanout"]
shape_colors = {"chain": "#1f77b4", "fanout": "#ff7f0e"}
shape_label = {"chain": "Chain", "fanout": "Fan-out"}

bar_width = 0.38
ymax = frame["speedup"].max() * 1.20

fig, ax = plt.subplots(figsize=(TEXTWIDTH_IN, 3.2))

for i, shape in enumerate(shapes):
    offset = (i - 0.5) * bar_width
    series = frame[frame["shape"] == shape].set_index("tasks").reindex(task_counts)
    xs = np.arange(len(task_counts)) + offset
    ax.bar(
        xs,
        series["speedup"].values,
        width=bar_width,
        color=shape_colors[shape],
        alpha=0.85,
        edgecolor="white",
        linewidth=0.5,
        label=shape_label[shape],
        zorder=3,
    )
    for x, v in zip(xs, series["speedup"].values):
        ax.text(x, v + 0.03, f"{v:.2f}x", ha="center", va="bottom",
                fontsize=ANNOT_PT, color="#333333")

ax.axhline(1.0, linestyle="--", color=REF_LINE_COLOR, linewidth=0.8, zorder=2)
ax.set_xticks(range(len(task_counts)))
ax.set_xticklabels([str(t) for t in task_counts])
ax.set_xlabel("Task count")
ax.set_ylabel(r"Per-iteration speedup (baseline $\div$ interlock-off)")
ax.set_ylim(0, ymax)
ax.grid(axis="y", linestyle=":", linewidth=0.4, alpha=0.5, zorder=1)
ax.legend(loc="upper left", ncol=2, columnspacing=1.0, handletextpad=0.3)

fig.tight_layout()

out = save(fig, "eval-e61-interlock-speedup-partitionb")
print(f"Wrote {out}")
