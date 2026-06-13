"""Fact-check: compare three captures of SolverLNSimple and LQNS:
  * Original baseline   (backup-pre-lqns/baseline.csv) — Sim was iter-only
  * Sim-only fair       (baseline-fairsim.csv)          — Sim under fair bracket
  * Sim + LQNS fair     (baseline-fairsim-lqns.csv)     — both in one run

We look for:
  - Sim_fair vs Sim_fairlqns: JIT-confound check (mixed-warmup matters?)
  - LQNS_original vs LQNS_fairlqns: LQNS reproducibility (sanity on the
    20--30 ms REST overhead floor and B-partition costs).
"""

from pathlib import Path

import numpy as np
import pandas as pd

from _style import DATA_DIR

original = pd.read_csv(DATA_DIR / "backup-pre-lqns" / "baseline.csv")[
    ["fixture_name", "partition", "sim_iters", "sim_time_ms",
     "lqns_iters", "lqns_time_ms"]
].rename(columns={
    "sim_iters": "sim_iters_orig",
    "sim_time_ms": "sim_ms_orig",
    "lqns_iters": "lqns_iters_orig",
    "lqns_time_ms": "lqns_ms_orig",
})

simfair = pd.read_csv(DATA_DIR / "baseline-fairsim.csv")[
    ["fixture_name", "sim_iters", "sim_time_ms"]
].rename(columns={"sim_iters": "sim_iters_sf", "sim_time_ms": "sim_ms_sf"})

mix = pd.read_csv(DATA_DIR / "baseline-fairsim-lqns.csv")[
    ["fixture_name", "sim_iters", "sim_time_ms", "lqns_iters", "lqns_time_ms"]
].rename(columns={
    "sim_iters": "sim_iters_mx",
    "sim_time_ms": "sim_ms_mx",
    "lqns_iters": "lqns_iters_mx",
    "lqns_time_ms": "lqns_ms_mx",
})

df = original.merge(simfair, on="fixture_name", how="outer") \
             .merge(mix, on="fixture_name", how="outer")

print(f"Rows: {len(df)} (orig={len(original)} simfair={len(simfair)} mix={len(mix)})")

# Drop rows missing any of the three captures (shouldn't happen).
df = df.dropna(subset=["sim_ms_orig", "sim_ms_sf", "sim_ms_mx",
                       "lqns_ms_orig", "lqns_ms_mx"])

# ---------------------------------------------------------------------------
# Iter count consistency.
# ---------------------------------------------------------------------------
iter_drift_sim = df[df["sim_iters_orig"].astype(int) != df["sim_iters_mx"].astype(int)]
iter_drift_lqns = df[df["lqns_iters_orig"].astype(int) != df["lqns_iters_mx"].astype(int)]
if not iter_drift_sim.empty:
    print("\n[iter-drift Sim] (algorithm is deterministic — drift = bug)")
    print(iter_drift_sim[["fixture_name", "sim_iters_orig", "sim_iters_sf", "sim_iters_mx"]].to_string(index=False))
else:
    print("\n[iter-drift Sim]  none — Sim iter counts identical across all three captures.")
if not iter_drift_lqns.empty:
    print("\n[iter-drift LQNS]")
    print(iter_drift_lqns[["fixture_name", "lqns_iters_orig", "lqns_iters_mx"]].to_string(index=False))
else:
    print("[iter-drift LQNS] none — LQNS iter counts identical across both captures.")

# ---------------------------------------------------------------------------
# Per-partition wall-clock totals.
# ---------------------------------------------------------------------------
PARTITION_ORDER = ["A1", "A2", "A3", "A4", "B", "C1", "C2", "C3", "C4"]
print("\n[per-partition totals — ms]")
hdr = ("part",
       "Sim orig", "Sim fair", "Sim mix",
       "LQNS orig", "LQNS mix")
print("  {:<4} {:>9} {:>9} {:>9}    {:>9} {:>9}".format(*hdr))
totals = {k: 0 for k in ["sim_orig", "sim_sf", "sim_mx", "lqns_orig", "lqns_mx"]}
for p in PARTITION_ORDER:
    sub = df[df["partition"] == p]
    so = int(sub["sim_ms_orig"].sum())
    sf = int(sub["sim_ms_sf"].sum())
    sm = int(sub["sim_ms_mx"].sum())
    lo = int(sub["lqns_ms_orig"].sum())
    lm = int(sub["lqns_ms_mx"].sum())
    totals["sim_orig"]  += so
    totals["sim_sf"]    += sf
    totals["sim_mx"]    += sm
    totals["lqns_orig"] += lo
    totals["lqns_mx"]   += lm
    print(f"  {p:<4} {so:>9,} {sf:>9,} {sm:>9,}    {lo:>9,} {lm:>9,}")
print(f"  TOT  {totals['sim_orig']:>9,} {totals['sim_sf']:>9,} {totals['sim_mx']:>9,}    "
      f"{totals['lqns_orig']:>9,} {totals['lqns_mx']:>9,}")

def safe_ratio(a, b):
    return a / b if b else float("nan")

print("\n[summary]")
print(f"  Sim_orig  -> Sim_fair  = {safe_ratio(totals['sim_sf'], totals['sim_orig']):.2f}x  (bracket effect, sim-only run)")
print(f"  Sim_fair  -> Sim_mix   = {safe_ratio(totals['sim_mx'], totals['sim_sf']):.2f}x  (JIT-mix vs sim-only; ~1 means clean)")
print(f"  Sim_orig  -> Sim_mix   = {safe_ratio(totals['sim_mx'], totals['sim_orig']):.2f}x  (bracket + mix combined)")
print(f"  LQNS_orig -> LQNS_mix  = {safe_ratio(totals['lqns_mx'], totals['lqns_orig']):.2f}x  (LQNS reproducibility)")

# ---------------------------------------------------------------------------
# Per-fixture Sim_fair vs Sim_mix (worst drift).
# ---------------------------------------------------------------------------
df["sim_delta"] = df["sim_ms_mx"] - df["sim_ms_sf"]
df["sim_drift_pct"] = (df["sim_delta"] / df["sim_ms_sf"].clip(lower=1)) * 100
df["lqns_delta"] = df["lqns_ms_mx"] - df["lqns_ms_orig"]
df["lqns_drift_pct"] = (df["lqns_delta"] / df["lqns_ms_orig"].clip(lower=1)) * 100

print("\n[top-10 Sim mix vs fair drift — absolute ms]")
top = df.reindex(df["sim_delta"].abs().sort_values(ascending=False).index).head(10)
print(top[["fixture_name", "partition",
          "sim_ms_orig", "sim_ms_sf", "sim_ms_mx",
          "sim_delta", "sim_drift_pct"]].to_string(index=False,
                                                    float_format=lambda x: f"{x:7.1f}"))

print("\n[top-10 LQNS mix vs orig drift — absolute ms]")
top = df.reindex(df["lqns_delta"].abs().sort_values(ascending=False).index).head(10)
print(top[["fixture_name", "partition",
          "lqns_ms_orig", "lqns_ms_mx",
          "lqns_delta", "lqns_drift_pct"]].to_string(index=False,
                                                      float_format=lambda x: f"{x:7.1f}"))
