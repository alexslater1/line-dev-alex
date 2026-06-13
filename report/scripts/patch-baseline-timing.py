#!/usr/bin/env python3
"""Patch elevator SolverLNSimple timing into the baseline capture.

The BOUNCE->ELEVATOR default switch changed only the SolverLNSimple side of the
capture (sim_iters / sim_time_ms); the SolverLN and SolverLQNS reference columns
are sweep-order-invariant and are NOT re-run. SimTimingCapture re-measured the
sim side under the elevator default (sim-timing-elevator.csv, one timing sample
per round). This script surgically overwrites ONLY the sim_iters and
sim_time_ms columns of:

  - baseline-r{1..N}.csv   (round r -> column t{r})
  - baseline.csv           (alias of baseline-r1 -> column t1)

leaving ln_*, lqns_*, accuracy and classification columns exactly as captured.
Re-run report/source/figures/scripts/e5_4_median.py afterwards to rebuild
baseline-medianed.csv from the patched r-files.

  python3 report/scripts/patch-baseline-timing.py \
      --timing report/data/sim-timing-elevator.csv \
      --data-dir report/data
"""
import argparse
import csv
from pathlib import Path


def load_timing(path):
    """fixture -> (sim_iters:int, [t1, t2, ...]:list[int])."""
    out = {}
    with open(path, newline="") as f:
        rd = csv.DictReader(f)
        tcols = [c for c in rd.fieldnames if c.startswith("t") and c[1:].isdigit()]
        tcols.sort(key=lambda c: int(c[1:]))
        for row in rd:
            out[row["fixture_name"]] = (
                int(row["sim_iters"]),
                [int(row[c]) for c in tcols],
            )
    return out, len(tcols)


def _median(xs):
    s = sorted(xs)
    n = len(s)
    return s[n // 2] if n % 2 else (s[n // 2 - 1] + s[n // 2]) / 2


def patch_csv(path, timing, round_idx):
    """Overwrite sim_iters/sim_time_ms in `path` from the elevator timing.

    round_idx is 0-based and selects a single round's sample (used for the
    per-run baseline-r{r}.csv files). Pass round_idx=None to use the per-fixture
    median across rounds (used for baseline.csv, the medianed drop-in figure).
    """
    with open(path, newline="") as f:
        rd = csv.DictReader(f)
        header = rd.fieldnames
        rows = list(rd)
    missing = []
    for row in rows:
        fx = row["fixture_name"]
        if fx not in timing:
            missing.append(fx)
            continue
        sim_iters, samples = timing[fx]
        row["sim_iters"] = str(sim_iters)
        val = _median(samples) if round_idx is None else samples[round_idx]
        row["sim_time_ms"] = str(int(val) if float(val).is_integer() else val)
    if missing:
        raise SystemExit(f"[patch-timing] {path.name}: no elevator timing for "
                         f"{len(missing)} fixtures: {missing[:5]}...")
    with open(path, "w", newline="") as f:
        wr = csv.DictWriter(f, fieldnames=header)
        wr.writeheader()
        wr.writerows(rows)
    tot_it = sum(int(r["sim_iters"]) for r in rows)
    tot_ms = sum(int(r["sim_time_ms"]) for r in rows)
    print(f"[patch-timing] {path.name:24s} sim_iters={tot_it}  sim_time_ms={tot_ms}")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--timing", default="report/data/sim-timing-elevator.csv")
    ap.add_argument("--data-dir", default="report/data")
    args = ap.parse_args()

    timing, n_rounds = load_timing(args.timing)
    data = Path(args.data_dir)
    print(f"[patch-timing] loaded elevator timing for {len(timing)} fixtures, "
          f"{n_rounds} round(s)")

    r_files = sorted(data.glob("baseline-r*.csv"))
    if not r_files:
        raise SystemExit(f"[patch-timing] no baseline-r*.csv in {data}")
    for rf in r_files:
        # baseline-r1 -> round 0, r2 -> round 1, ...  Reuse the last available
        # round if there are more r-files than timing rounds.
        try:
            r_idx = int(rf.stem.split("baseline-r")[1]) - 1
        except (IndexError, ValueError):
            raise SystemExit(f"[patch-timing] cannot parse round from {rf.name}")
        r_idx = min(r_idx, n_rounds - 1)
        patch_csv(rf, timing, r_idx)

    baseline = data / "baseline.csv"
    if baseline.exists():
        patch_csv(baseline, timing, None)   # per-fixture median across rounds

    print("[patch-timing] done. Now re-run e5_4_median.py to rebuild "
          "baseline-medianed.csv")


if __name__ == "__main__":
    main()
