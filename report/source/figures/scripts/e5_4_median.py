"""Median + IQR post-processor for the overnight reproducibility capture.

Reads every report/data/baseline-r*.csv (4 by default from run-overnight.sh)
and emits:

  baseline-medianed.csv  — drop-in replacement for baseline.csv, with
                           sim/ln/lqns time columns = per-run median, plus
                           companion *_min/_q25/_q75/_max columns and an
                           n_runs count.

Iter counts are deterministic across runs; a discrepancy is logged but
should never happen.

Accuracy classification fields (matches_within_tol, sim_vs_*_max_rel_diff)
should also be invariant — taken from the first run; mismatches are logged.
"""
import sys
from pathlib import Path

import numpy as np
import pandas as pd

from _style import DATA_DIR


def load_runs():
    csvs = sorted(DATA_DIR.glob("baseline-r*.csv"))
    if not csvs:
        sys.exit(f"[e5_4] no baseline-r*.csv files in {DATA_DIR}")
    runs = []
    for c in csvs:
        df = pd.read_csv(c)
        df["__run"] = c.stem
        runs.append(df)
        print(f"[e5_4] loaded {c.name}: {len(df)} rows")
    if len(runs) < 2:
        print(f"[e5_4] WARN: only {len(runs)} run(s); medianing is degenerate "
              f"but proceeding so the pipeline is testable")
    return runs


def medianize(runs):
    big = pd.concat(runs, ignore_index=True)
    out_rows = []
    for fx, g in big.groupby("fixture_name"):
        row = {
            "fixture_name": fx,
            "partition": g["partition"].iloc[0],
            "n_runs": len(g),
        }
        # Iter counts: deterministic; complain loudly if they aren't.
        for col in ("sim_iters", "ln_iters", "lqns_iters"):
            vals = g[col].dropna().unique()
            if len(vals) > 1:
                print(f"[e5_4] WARN: {fx} {col} has {len(vals)} unique "
                      f"values across runs: {list(vals)}")
            row[col] = int(vals[0]) if len(vals) else None
        # Timing columns: median + quartile envelope.
        for col in ("sim_time_ms", "ln_time_ms", "lqns_time_ms"):
            vals = g[col].dropna().astype(float)
            if len(vals):
                row[col]            = float(vals.median())  # drop-in name
                row[f"{col}_median"] = float(vals.median())
                row[f"{col}_min"]    = float(vals.min())
                row[f"{col}_q25"]    = float(vals.quantile(0.25))
                row[f"{col}_q75"]    = float(vals.quantile(0.75))
                row[f"{col}_max"]    = float(vals.max())
            else:
                for s in ("", "_median", "_min", "_q25", "_q75", "_max"):
                    row[f"{col}{s}"] = None
        # LQNS per-call overhead breakdown — median only (no IQR needed,
        # these are diagnostic). The columns are absent in pre-instrumentation
        # baseline runs; tolerate that so old captures still medianize.
        for col in ("lqns_rest_call_ms", "lqns_elapsed_ms", "lqns_local_parse_ms"):
            if col not in g.columns:
                row[col] = None
                continue
            vals = g[col].dropna().astype(float)
            row[col] = float(vals.median()) if len(vals) else None
        # Accuracy + classification: invariant. Take from first row,
        # warn if any run disagrees.
        for col in ("sim_vs_ln_max_rel_diff", "sim_vs_lqns_max_rel_diff",
                    "matches_within_tol"):
            vals = g[col].dropna().unique()
            if len(vals) > 1:
                print(f"[e5_4] WARN: {fx} {col} varies across runs: {list(vals)}")
            row[col] = vals[0] if len(vals) else None
        row["notes"] = g["notes"].iloc[0]
        out_rows.append(row)
    return pd.DataFrame(out_rows)


def main():
    runs = load_runs()
    med = medianize(runs)

    # Stable column order — mirror baseline.csv then append the envelope cols.
    base_cols = [
        "fixture_name", "partition",
        "sim_iters", "sim_time_ms",
        "ln_iters", "ln_time_ms",
        "lqns_iters", "lqns_time_ms",
        "lqns_rest_call_ms", "lqns_elapsed_ms", "lqns_local_parse_ms",
        "sim_vs_ln_max_rel_diff", "sim_vs_lqns_max_rel_diff",
        "matches_within_tol", "notes",
        "n_runs",
    ]
    envelope_cols = []
    for c in ("sim_time_ms", "ln_time_ms", "lqns_time_ms"):
        for suf in ("_median", "_min", "_q25", "_q75", "_max"):
            envelope_cols.append(c + suf)
    # Tolerate older baseline-r*.csv files without overhead columns by
    # filling missing keys with None so the column order stays stable.
    for c in ("lqns_rest_call_ms", "lqns_elapsed_ms", "lqns_local_parse_ms"):
        if c not in med.columns:
            med[c] = None
    cols = base_cols + envelope_cols
    out = med[cols]

    out_path = DATA_DIR / "baseline-medianed.csv"
    out.to_csv(out_path, index=False)
    print(f"\n[e5_4] wrote {out_path} ({len(out)} fixtures, {len(runs)} runs each)")

    # Quick aggregate summary so the operator sees the headline immediately.
    sim_tot = int(out["sim_time_ms"].sum())
    ln_tot  = int(out["ln_time_ms"].sum())
    lqns_tot = int(out["lqns_time_ms"].fillna(0).sum())
    sim_q1 = int(out["sim_time_ms_q25"].sum())
    sim_q3 = int(out["sim_time_ms_q75"].sum())
    ln_q1  = int(out["ln_time_ms_q25"].sum())
    ln_q3  = int(out["ln_time_ms_q75"].sum())
    lqns_q1 = int(out["lqns_time_ms_q25"].fillna(0).sum())
    lqns_q3 = int(out["lqns_time_ms_q75"].fillna(0).sum())

    print("\n[e5_4] aggregate medianed totals (ms):")
    print(f"   Sim  median={sim_tot:>9,}   IQR=[{sim_q1:>9,}, {sim_q3:>9,}]")
    print(f"   LN   median={ln_tot:>9,}   IQR=[{ln_q1:>9,}, {ln_q3:>9,}]")
    print(f"   LQNS median={lqns_tot:>9,}   IQR=[{lqns_q1:>9,}, {lqns_q3:>9,}]")
    if sim_tot:
        print(f"   LN  / Sim  = {ln_tot/sim_tot:.1f}x")
        print(f"   LQNS / Sim = {lqns_tot/sim_tot:.2f}x")


if __name__ == "__main__":
    main()
