# Styling derived from report/source/main.tex:
#   - Document class: report, 11pt, a4paper, twoside, onehalfspacing
#   - Font: serif (lmodern / Latin Modern Roman)
#   - Solver palette: blue (SolverLNSimple), red (SolverLN), green (LQNS V5)
#   - Partition palette: Set2 categorical (used for partition / subpartition encodings)
# All six evaluation figures use the conventions defined here.

from pathlib import Path

import matplotlib as mpl
import matplotlib.pyplot as plt
import seaborn as sns

# Body font is 11pt; figure axis/tick labels at 9pt so figures read as
# contained elements rather than competing text.
BODY_PT = 11
AXIS_PT = 9
TICK_PT = 8.5
LEGEND_PT = 8.5
ANNOT_PT = 8

# Solver palette — used consistently across every figure that compares
# the three solvers. Blue = SolverLNSimple, Red = SolverLN,
# Green = LQNS V5. Tableau hex values, colour-blind-friendly.
SOLVER_COLORS = {
    "SolverLNSimple": "#1f77b4",
    "SolverLN": "#d62728",
    "LQNS V5": "#2ca02c",
}

# Partition / subpartition palette (categorical, used for A1/A2/A3/A4
# encoding in E3.1 and the per-partition encodings in E9.1).
# The first nine entries keep their original Set2 colours so the existing
# per-partition figures are unchanged; A5/C5/C6/C7 (added with the 78-fixture
# corpus) are appended with distinct hues.
PARTITION_PALETTE = list(sns.color_palette("Set2", n_colors=9)) + [
    "#7f7f7f", "#bcbd22", "#17becf", "#9467bd",
]
PARTITION_ORDER = [
    "A1", "A2", "A3", "A4", "A5", "B",
    "C1", "C2", "C3", "C4", "C5", "C6", "C7",
]
PARTITION_COLOR = dict(zip(PARTITION_ORDER, PARTITION_PALETTE))

# Reference-line and emphasis colours used across figures.
REF_LINE_COLOR = "#555555"          # for rtol = 5e-2 and 1.0x baselines
RTOL = 5e-2

# Figure widths in inches.
TEXTWIDTH_IN = 5.6     # ~85% of textwidth for inline figures
WIDE_IN = 6.8          # for wide / faceted figures

DATA_DIR = Path(__file__).resolve().parents[3] / "data"
FIG_DIR = Path(__file__).resolve().parents[1]


def configure() -> None:
    """Apply the shared rcParams for every figure in the report."""
    mpl.rcParams.update({
        "font.family": "serif",
        "font.serif": ["DejaVu Serif", "Liberation Serif", "Times New Roman"],
        "mathtext.fontset": "cm",  # closest matplotlib-bundled match to lmodern
        "font.size": AXIS_PT,
        "axes.labelsize": AXIS_PT,
        "axes.titlesize": AXIS_PT,
        "xtick.labelsize": TICK_PT,
        "ytick.labelsize": TICK_PT,
        "legend.fontsize": LEGEND_PT,
        "legend.frameon": False,
        "axes.spines.top": False,
        "axes.spines.right": False,
        "axes.edgecolor": "#333333",
        "axes.linewidth": 0.6,
        "xtick.color": "#333333",
        "ytick.color": "#333333",
        "xtick.major.width": 0.6,
        "ytick.major.width": 0.6,
        "savefig.bbox": "tight",
        "savefig.pad_inches": 0.03,
        "pdf.fonttype": 42,
        "ps.fonttype": 42,
    })


def save(fig, name: str) -> Path:
    """Save a figure as PDF into the report's figures/ directory."""
    out = FIG_DIR / f"{name}.pdf"
    fig.savefig(out)
    return out
