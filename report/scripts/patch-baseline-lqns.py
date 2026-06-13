#!/usr/bin/env python3
"""Surgically patch ONLY the LQNS-comparison cells of a captured baseline CSV.

Post-run fix for D2 (sim_vs_lqns NaN->Infinity + match-lqns-only mis-tagged as
match-neither) and D3 (3 OR-fork fixtures had LQNS unavailable). After the
re-derivation harness (LqnsRecompare) produces a `recompare.csv` with corrected
values, this merges it into the run's `baseline.csv` overwriting ONLY:

    lqns_iters, lqns_time_ms, sim_vs_lqns_max_rel_diff, matches_within_tol
    (+ sim_vs_ln_max_rel_diff only if --also-sim-vs-ln is passed)

Every other column (sim_iters, sim_time_ms, ln_iters, ln_time_ms, sim_vs_ln,
partition, condition, notes, ...) is left byte-identical. The script then
VERIFIES cell-by-cell that no non-target column changed and aborts if it did.

Usage:
    patch-baseline-lqns.py --baseline report/data/baseline.csv \
                           --recompare report/data/recompare.csv \
                           --out report/data/baseline.patched.csv
    # review, then: mv baseline.patched.csv baseline.csv

    --also-sim-vs-ln   also overwrite sim_vs_ln_max_rel_diff (default: keep — it is clean)
    --in-place         write back to --baseline (keeps a .bak)
    --dry-run          report what would change, write nothing
"""
import argparse, csv, shutil, sys
from pathlib import Path

# Columns the LQNS re-derivation is allowed to overwrite. Nothing else is touched.
#
# Scope note (post-run, D3 resolved as "leave the 3 A5 OR-fork fixtures LQNS-
# unavailable"): the only columns the D2/D3 fixes actually change are the
# comparison results. lqns_iters is deterministic and verified identical to the
# original capture (no OR-fork fixture became newly solvable), and lqns_time_ms
# is a per-run timing the medianed figures depend on — overwriting it with a
# single fresh measurement would corrupt them. Both are therefore EXCLUDED.
TARGET_COLS = ["sim_vs_lqns_max_rel_diff", "matches_within_tol"]
OPTIONAL_LN_COL = "sim_vs_ln_max_rel_diff"
KEY = "fixture_name"


def read_csv(path):
    with open(path, newline="") as f:
        r = csv.DictReader(f)
        return r.fieldnames, list(r)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--baseline", required=True)
    ap.add_argument("--recompare", required=True)
    ap.add_argument("--out")
    ap.add_argument("--also-sim-vs-ln", action="store_true")
    ap.add_argument("--in-place", action="store_true")
    ap.add_argument("--dry-run", action="store_true")
    a = ap.parse_args()

    cols = list(TARGET_COLS) + ([OPTIONAL_LN_COL] if a.also_sim_vs_ln else [])

    base_fields, base_rows = read_csv(a.baseline)
    _, rec_rows = read_csv(a.recompare)
    rec = {r[KEY]: r for r in rec_rows}

    # sanity: every overwrite column must exist in the baseline header
    missing = [c for c in cols if c not in base_fields]
    if missing:
        sys.exit(f"ERROR: baseline missing target columns {missing}")

    # snapshot originals for verification + change reporting
    orig = [dict(r) for r in base_rows]

    changes = {c: 0 for c in cols}
    patched_fixtures, unmatched = 0, []
    for row in base_rows:
        src = rec.get(row[KEY])
        if src is None:
            unmatched.append(row[KEY])
            continue
        patched_fixtures += 1
        for c in cols:
            if c not in src:
                continue  # recompare didn't provide this column -> leave as is
            if row[c] != src[c]:
                changes[c] += 1
            row[c] = src[c]

    # ---- VERIFY: only target columns may differ from the original ----
    violations = []
    for o, n in zip(orig, base_rows):
        for c in base_fields:
            if c in cols:
                continue
            if o[c] != n[c]:
                violations.append((n[KEY], c, o[c], n[c]))
    if violations:
        print("FATAL: non-target columns changed (aborting, nothing written):")
        for fx, c, ov, nv in violations[:20]:
            print(f"  {fx}: {c}: {ov!r} -> {nv!r}")
        sys.exit(2)

    # ---- report ----
    print(f"baseline rows: {len(base_rows)} | recompare rows: {len(rec_rows)}")
    print(f"fixtures patched: {patched_fixtures}")
    if unmatched:
        print(f"WARN: {len(unmatched)} baseline fixtures NOT in recompare (left untouched): "
              f"{', '.join(unmatched[:8])}{' ...' if len(unmatched) > 8 else ''}")
    extra = [k for k in rec if k not in {r[KEY] for r in base_rows}]
    if extra:
        print(f"WARN: {len(extra)} recompare fixtures NOT in baseline (ignored): {', '.join(extra[:8])}")
    print("cells overwritten per column:")
    for c in cols:
        print(f"  {c}: {changes[c]}")
    print("VERIFY: no non-target column changed. OK")

    if a.dry_run:
        print("(dry-run: nothing written)")
        return

    out = a.baseline if a.in_place else (a.out or str(Path(a.baseline).with_suffix(".patched.csv")))
    if a.in_place:
        shutil.copy2(a.baseline, a.baseline + ".bak")
        print(f"backup: {a.baseline}.bak")
    with open(out, "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=base_fields)
        w.writeheader()
        w.writerows(base_rows)
    print(f"wrote: {out}")


if __name__ == "__main__":
    main()
