#!/usr/bin/env python3
"""LN-vs-LN drift analysis across the F20 ablation axis, with lqsim direction.

Replaces the LNSimple-vs-LN classification framing used in the original
§8.7 Finding 4 ("six fixtures move from match-ln to match-lqns-only")
with a direct LN-vs-LN measurement on LN's own per-metric output: how much
did SolverLN's output shift when F20 (or Arm A) was disabled, and did it
shift toward or away from a simulation reference (lqsim).

Inputs
------
A directory containing the four artifacts produced by run-f20-axis.sh:

  baseline.tables.csv          # LN/LNSimple/LQNS rows under baseline SolverLN
  f20off.tables.csv            # LN/LNSimple/LQNS rows under SolverLN with F20 disabled
  A2_arm_A.tables.csv          # LN/LNSimple/LQNS rows under SolverLN with only iter_min floor cut
  lqsim-reference.tables.csv   # lqsim rows; one sweep, independent of LN

Each tables.csv has one row per (fixture, condition, solver, node) with the
five per-node metrics (qlen, util, respt, residt, tput) plus the hosting
processor's server_count for per-server util normalisation.

Outputs
-------
A summary table printed to stdout (Markdown + LaTeX), plus an optional
CSV at <out_dir>/ln_drift_summary.csv.

Per-fixture metrics:
  drift_baseline_to_f20off  - max over (node, metric) of relative drift in LN's output
  drift_baseline_to_armA    - same for the A2_arm_A condition (sanity: should be near zero)
  dist_baseline_to_lqsim    - max over (node, metric) of LN_baseline vs lqsim distance
  dist_f20off_to_lqsim      - same for LN_f20off
  direction                 - 'toward', 'away', or 'mixed' depending on whether
                              f20off moved LN closer to or further from lqsim
"""

from __future__ import annotations

import argparse
import csv
import math
import sys
from collections import defaultdict
from pathlib import Path
from typing import Dict, List, Tuple

METRICS = ("qlen", "util", "respt", "residt", "tput")


def parse_float(s):
    if s is None or s == "" or s == "NaN":
        return float("nan")
    try:
        return float(s)
    except ValueError:
        return float("nan")


def parse_int(s):
    if s is None or s == "":
        return None
    try:
        return int(s)
    except ValueError:
        return None


def load_tables(path: Path):
    """Return {(fixture, solver, node): {metric: value, 'server_count': int}}."""
    out = {}
    with path.open() as f:
        reader = csv.DictReader(f)
        for row in reader:
            key = (row["fixture_name"], row["solver"], row["node_name"])
            entry = {m: parse_float(row[m]) for m in METRICS}
            entry["partition"] = row["partition"]
            entry["node_type"] = row.get("node_type", "")
            entry["server_count"] = parse_int(row.get("server_count"))
            out[key] = entry
    return out


def normalise_util(val: float, server_count: int | None) -> float:
    """LN reports per-server util; LQNS and lqsim report total util. Divide by
    server_count to bring the latter into per-server convention. INF processors
    and single-server tasks pass through unchanged (server_count == 1). Used
    only on lqsim/LQNS reference values, never on LN's own output."""
    if server_count and server_count > 1 and math.isfinite(val):
        return val / server_count
    return val


def relative_diff(a: float, b: float) -> float:
    """|a - b| / max(|a|, 1e-6). Returns NaN if either side is NaN. Returns
    0 if both are NaN (definite agreement on absence). This is the same
    denominator convention used by util.rawRelDiff in the Java harness."""
    a_nan = math.isnan(a) if isinstance(a, float) else False
    b_nan = math.isnan(b) if isinstance(b, float) else False
    if a_nan and b_nan:
        return 0.0
    if a_nan or b_nan:
        return float("nan")
    return abs(a - b) / max(abs(a), 1e-6)


def absolute_diff(a: float, b: float) -> float:
    a_nan = math.isnan(a) if isinstance(a, float) else False
    b_nan = math.isnan(b) if isinstance(b, float) else False
    if a_nan and b_nan:
        return 0.0
    if a_nan or b_nan:
        return float("nan")
    return abs(a - b)


def analyse(out_dir: Path) -> List[dict]:
    baseline = load_tables(out_dir / "baseline.tables.csv")
    f20off   = load_tables(out_dir / "f20off.tables.csv")
    arm_a_path = out_dir / "A2_arm_A.tables.csv"
    arm_a    = load_tables(arm_a_path) if arm_a_path.exists() else {}
    lqsim_path = out_dir / "lqsim-reference.tables.csv"
    lqsim = load_tables(lqsim_path) if lqsim_path.exists() else {}

    # Collect (fixture, node) pairs present in the LN rows of every condition.
    ln_keys: Dict[str, set] = defaultdict(set)
    for src, name in ((baseline, "baseline"), (f20off, "f20off"), (arm_a, "A2_arm_A")):
        for (fix, solver, node), _ in src.items():
            if solver == "LN":
                ln_keys[name].add((fix, node))

    fixtures = sorted(set(fix for (fix, _) in ln_keys["baseline"]))
    rows = []
    for fix in fixtures:
        partition = None
        worst_drift_f20  = ("", "", float("-inf"), float("nan"), float("nan"))
        worst_drift_arm  = ("", "", float("-inf"), float("nan"), float("nan"))
        worst_dist_base  = ("", "", float("-inf"), float("nan"), float("nan"))
        worst_dist_f20   = ("", "", float("-inf"), float("nan"), float("nan"))
        closer_count = 0
        further_count = 0
        comparable_lqsim = 0
        nodes = sorted({node for (f, _, node) in baseline.keys() if f == fix})
        for node in nodes:
            base = baseline.get((fix, "LN", node))
            f20  = f20off.get((fix, "LN", node))
            arm  = arm_a.get((fix, "LN", node))
            lq   = lqsim.get((fix, "lqsim", node))
            if base is None:
                continue
            if partition is None:
                partition = base["partition"]
            sc = base.get("server_count")
            for m in METRICS:
                b_val = base[m]
                # LN-vs-LN drift across the F20 axis. No normalisation needed
                # because both sides are LN's own output (same convention).
                if f20 is not None:
                    rel = relative_diff(b_val, f20[m])
                    if not math.isnan(rel) and rel > worst_drift_f20[2]:
                        worst_drift_f20 = (node, m, rel, b_val, f20[m])
                if arm is not None:
                    rel = relative_diff(b_val, arm[m])
                    if not math.isnan(rel) and rel > worst_drift_arm[2]:
                        worst_drift_arm = (node, m, rel, b_val, arm[m])

                # LN-vs-lqsim distance, separately for baseline and f20off.
                # Normalise lqsim util to per-server before comparing.
                if lq is not None and f20 is not None:
                    lq_val = lq[m]
                    if m == "util":
                        lq_val = normalise_util(lq_val, sc)
                    dist_b = relative_diff(b_val,   lq_val)
                    dist_f = relative_diff(f20[m], lq_val)
                    if not math.isnan(dist_b) and dist_b > worst_dist_base[2]:
                        worst_dist_base = (node, m, dist_b, b_val, lq_val)
                    if not math.isnan(dist_f) and dist_f > worst_dist_f20[2]:
                        worst_dist_f20 = (node, m, dist_f, f20[m], lq_val)
                    # Direction count: only when both distances are real and the
                    # baseline LN differs from f20off LN at the cell (otherwise
                    # the cell isn't on the drift axis).
                    if (not math.isnan(dist_b) and not math.isnan(dist_f)
                            and absolute_diff(b_val, f20[m]) > 1e-9):
                        comparable_lqsim += 1
                        if dist_f < dist_b:
                            closer_count += 1
                        elif dist_f > dist_b:
                            further_count += 1
        # Direction summary.
        if comparable_lqsim == 0:
            direction = "no-drift"
        elif closer_count > 0 and further_count == 0:
            direction = "toward"
        elif further_count > 0 and closer_count == 0:
            direction = "away"
        else:
            direction = f"mixed ({closer_count}→closer, {further_count}→farther)"

        rows.append({
            "fixture":                  fix,
            "partition":                partition,
            "drift_baseline_to_f20off": worst_drift_f20[2],
            "drift_baseline_to_f20off_at": f"{worst_drift_f20[0]}.{worst_drift_f20[1]}",
            "drift_baseline_to_f20off_values": (worst_drift_f20[3], worst_drift_f20[4]),
            "drift_baseline_to_armA":   worst_drift_arm[2],
            "drift_baseline_to_armA_at": f"{worst_drift_arm[0]}.{worst_drift_arm[1]}",
            "dist_baseline_to_lqsim":   worst_dist_base[2],
            "dist_f20off_to_lqsim":     worst_dist_f20[2],
            "direction":                direction,
            "closer_cells":             closer_count,
            "further_cells":            further_count,
        })
    return rows


def fmt(x):
    if x is None or (isinstance(x, float) and math.isnan(x)):
        return "-"
    if isinstance(x, float):
        if abs(x) < 1e-3:
            return f"{x:.2e}"
        return f"{x:.4f}"
    return str(x)


def print_summary(rows: List[dict], detail_threshold: float = 1e-2) -> None:
    print(f"# LN-vs-LN drift across the F20 ablation axis")
    print(f"# {len(rows)} fixtures analysed")
    print()
    hdr = ("fixture", "part", "drift→f20off", "where", "drift→armA",
           "base→lqsim", "f20off→lqsim", "direction")
    print(" | ".join(hdr))
    print(" | ".join("-" * len(h) for h in hdr))
    rows_sorted = sorted(rows, key=lambda r: (-(r["drift_baseline_to_f20off"]
                                               if not math.isnan(r["drift_baseline_to_f20off"]) else -1),
                                              r["fixture"]))
    for r in rows_sorted:
        print(" | ".join([
            r["fixture"],
            r["partition"],
            fmt(r["drift_baseline_to_f20off"]),
            r["drift_baseline_to_f20off_at"],
            fmt(r["drift_baseline_to_armA"]),
            fmt(r["dist_baseline_to_lqsim"]),
            fmt(r["dist_f20off_to_lqsim"]),
            r["direction"],
        ]))

    print()
    print(f"# Fixtures with LN drift > {detail_threshold:.0%} between baseline and f20off")
    big = [r for r in rows_sorted if (not math.isnan(r["drift_baseline_to_f20off"])
                                       and r["drift_baseline_to_f20off"] > detail_threshold)]
    if not big:
        print("  (none)")
    else:
        for r in big:
            b, f = r["drift_baseline_to_f20off_values"]
            print(f"  {r['fixture']}  ({r['partition']})")
            print(f"    drift   = {fmt(r['drift_baseline_to_f20off'])} at {r['drift_baseline_to_f20off_at']}"
                  f"  (baseline {fmt(b)} -> f20off {fmt(f)})")
            print(f"    armA    = {fmt(r['drift_baseline_to_armA'])} at {r['drift_baseline_to_armA_at']}")
            print(f"    lqsim:  baseline dist {fmt(r['dist_baseline_to_lqsim'])}, "
                  f"f20off dist {fmt(r['dist_f20off_to_lqsim'])}")
            print(f"    direction: {r['direction']}  "
                  f"({r['closer_cells']} cells closer, {r['further_cells']} further)")


def write_csv(rows: List[dict], path: Path) -> None:
    fields = [
        "fixture", "partition",
        "drift_baseline_to_f20off", "drift_baseline_to_f20off_at",
        "drift_baseline_to_armA",   "drift_baseline_to_armA_at",
        "dist_baseline_to_lqsim",   "dist_f20off_to_lqsim",
        "direction", "closer_cells", "further_cells",
    ]
    with path.open("w") as f:
        w = csv.DictWriter(f, fieldnames=fields)
        w.writeheader()
        for r in rows:
            w.writerow({k: r.get(k, "") for k in fields})
    print(f"\nWrote summary CSV: {path}", file=sys.stderr)


def main():
    p = argparse.ArgumentParser()
    p.add_argument("out_dir", help="Directory containing *.tables.csv files from run-f20-axis.sh")
    p.add_argument("--csv", default=None,
                   help="Optional path for the summary CSV (default <out_dir>/ln_drift_summary.csv)")
    p.add_argument("--threshold", type=float, default=1e-2,
                   help="Highlight fixtures whose drift exceeds this rel diff (default 0.01)")
    args = p.parse_args()
    out_dir = Path(args.out_dir)
    if not out_dir.is_dir():
        sys.exit(f"not a directory: {out_dir}")
    rows = analyse(out_dir)
    print_summary(rows, detail_threshold=args.threshold)
    csv_path = Path(args.csv) if args.csv else out_dir / "ln_drift_summary.csv"
    write_csv(rows, csv_path)


if __name__ == "__main__":
    main()
