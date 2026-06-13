"""Recover the 4-run overnight capture from maven log files.

Context: the overnight ran 4 × `SolverLNSimpleFullEvaluationTest` (61
fixtures each, 4×~42 min), all 244 fixture executions succeeded, but a
util.java bug (findEvaluationTestName matched only the everyday class
name, not the Full class name) silently dropped every row from
DataCollector — no baseline-r*.csv was written.

What we recover from: the per-fixture timing lines that util.java prints
to stdout AFTER every solver call. They survive in the per-run maven log
at report/data/overnight-logs/baseline-rN.log. Each fixture produces a
deterministic block of four lines:

    [SolverLNSimple] X.XXX s, N iters
    [paths] LD=N AMVA=M cap=K
    [SolverLN]       X.XXX s, N iters
    [SolverLQNS]     X.XXX s, N iters

Order: JUnit 5 default method ordering is deterministic but not source-
declared. Yesterday's Surefire XML for SolverLNSimpleEvaluationTest
records the order JUnit used. Two classes with identical declared method
names produce identical orderings under the JUnit default orderer, so
that order also applies to SolverLNSimpleFullEvaluationTest.

We cross-validate by predicting each block's iter triple from the
existing baseline.csv and comparing to what the log actually contains.
Match across 61 blocks = order confirmed; we then emit
baseline-r1.csv ... baseline-r4.csv with the recovered timings.
"""
import re
import sys
from pathlib import Path
from xml.etree import ElementTree as ET

import pandas as pd

ROOT = Path("/home/alex/Documents/FYP/line-dev")
LOG_DIR = ROOT / "report/data/overnight-logs"
DATA_DIR = ROOT / "report/data"
ORDER_XML = Path("/tmp/target/surefire-reports/TEST-jline.solvers.ln_simple.SolverLNSimpleEvaluationTest.xml")
BASELINE_CSV = DATA_DIR / "backup-pre-lqns" / "baseline.csv"
WARMUP_BLOCKS = 3  # three iterations of A1_chain_d3_N20 before real fixtures


def junit_order_from_xml(xml_path: Path) -> list[str]:
    """Extract the @Test method names in run order from a Surefire XML.
    Filters out Maven property names like 'tmp', 'test' etc that share
    the name=' attribute key."""
    tree = ET.parse(xml_path)
    root = tree.getroot()
    out = []
    for tc in root.iter("testcase"):
        n = tc.get("name")
        # Sanity: real fixture names start with A1_/A2_/.../C4_; system
        # properties don't match the pattern.
        if n and re.match(r"^[ABC][1-4]?_", n):
            out.append(n)
    return out


def parse_log_blocks(log_path: Path) -> list[dict]:
    """Read a maven log and return every 4-line solver-timing block in
    encounter order. Each dict has sim_ms, sim_iters, ln_ms, ln_iters,
    lqns_ms, lqns_iters, ld, amva. The first WARMUP_BLOCKS blocks are
    the @BeforeAll warmup runs and should be discarded by the caller."""
    text = log_path.read_text(errors="replace")
    # ANSI colour stripping (the maven log is colour-formatted).
    text = re.sub(r"\x1b\[[0-9;]*m", "", text)

    sim_re   = re.compile(r"^\[SolverLNSimple\] +([\d.]+) +s, +(\d+) +iters", re.M)
    paths_re = re.compile(r"^\[paths\] +LD=(\d+) +AMVA=(\d+) +cap=(\d+)", re.M)
    ln_re    = re.compile(r"^\[SolverLN\] +([\d.]+) +s, +(\d+) +iters", re.M)
    lqns_re  = re.compile(r"^\[SolverLQNS\] +([\d.]+) +s, +(-?\d+) +iters", re.M)

    sims   = sim_re.findall(text)
    paths  = paths_re.findall(text)
    lns    = ln_re.findall(text)
    lqnses = lqns_re.findall(text)

    n = min(len(sims), len(paths), len(lns), len(lqnses))
    if not (len(sims) == len(paths) == len(lns) == len(lqnses)):
        print(f"  [warn] {log_path.name}: line counts differ "
              f"sim={len(sims)} paths={len(paths)} ln={len(lns)} lqns={len(lqnses)}; "
              f"taking first {n}")

    blocks = []
    for i in range(n):
        sim_s, sim_i = sims[i]
        ld, amva, _cap = paths[i]
        ln_s, ln_i = lns[i]
        lqns_s, lqns_i = lqnses[i]
        blocks.append({
            "sim_time_ms": round(float(sim_s) * 1000),
            "sim_iters":   int(sim_i),
            "ld":          int(ld),
            "amva":        int(amva),
            "ln_time_ms":  round(float(ln_s) * 1000),
            "ln_iters":    int(ln_i),
            "lqns_time_ms": round(float(lqns_s) * 1000),
            "lqns_iters":  int(lqns_i),
        })
    return blocks


def baseline_iter_triples() -> dict[str, tuple]:
    """fixture_name -> (sim_iters, ln_iters, lqns_iters) from the
    existing baseline.csv. The triples may collide across fixtures —
    they're a verification fingerprint, not an identification key."""
    df = pd.read_csv(BASELINE_CSV)
    out = {}
    for _, r in df.iterrows():
        out[r["fixture_name"]] = (int(r["sim_iters"]),
                                   int(r["ln_iters"]),
                                   int(r["lqns_iters"]))
    return out


def partition_of(name: str) -> str:
    if not name:
        return "?"
    for prefix in ("A1_", "A2_", "A3_", "A4_", "B_", "C1_", "C2_", "C3_", "C4_"):
        if name.startswith(prefix):
            return prefix.rstrip("_")
    return "?"


def main():
    if not ORDER_XML.exists():
        sys.exit(f"missing JUnit order XML at {ORDER_XML}")
    order = junit_order_from_xml(ORDER_XML)
    print(f"[order] {len(order)} fixtures from {ORDER_XML.name}")
    if len(order) != 61:
        print(f"  [warn] expected 61 fixtures, got {len(order)}")

    expected = baseline_iter_triples()

    log_files = sorted(LOG_DIR.glob("baseline-r*.log"))
    if not log_files:
        sys.exit(f"no logs in {LOG_DIR}")

    for log_path in log_files:
        run_id = log_path.stem  # baseline-rN
        print(f"\n[run] {run_id}")
        blocks = parse_log_blocks(log_path)
        print(f"  parsed {len(blocks)} timing blocks "
              f"(expect {WARMUP_BLOCKS} warmup + {len(order)} fixtures = {WARMUP_BLOCKS + len(order)})")
        if len(blocks) != WARMUP_BLOCKS + len(order):
            print(f"  [warn] block count mismatch — proceeding with what we have")

        run_blocks = blocks[WARMUP_BLOCKS:WARMUP_BLOCKS + len(order)]

        mismatches = 0
        rows = []
        for fx_name, block in zip(order, run_blocks):
            triple_log = (block["sim_iters"], block["ln_iters"], block["lqns_iters"])
            triple_exp = expected.get(fx_name)
            ok = triple_exp is not None and triple_log == triple_exp
            if not ok:
                mismatches += 1
                if mismatches <= 5:
                    print(f"  [iter-mismatch] {fx_name}: log={triple_log} baseline={triple_exp}")

            rows.append({
                "fixture_name": fx_name,
                "partition": partition_of(fx_name),
                "condition": run_id,
                "sim_iters":   block["sim_iters"],
                "sim_time_ms": block["sim_time_ms"],
                "ln_iters":    block["ln_iters"],
                "ln_time_ms":  block["ln_time_ms"],
                "lqns_iters":  block["lqns_iters"],
                "lqns_time_ms": block["lqns_time_ms"],
                # Accuracy classification columns are not recoverable
                # from the maven log; leave them blank. Downstream
                # scripts already tolerate missing values here.
                "sim_vs_ln_max_rel_diff": "",
                "sim_vs_lqns_max_rel_diff": "",
                "matches_within_tol": "recovered-from-log",
                "notes": "recovered from overnight-logs/" + log_path.name,
            })

        print(f"  iter-triple matches: {len(run_blocks) - mismatches}/{len(run_blocks)}")

        out_path = DATA_DIR / f"{run_id}.csv"
        pd.DataFrame(rows).to_csv(out_path, index=False)
        print(f"  wrote {out_path} ({len(rows)} rows)")

    print("\nDone. Now run:")
    print("  python3 report/source/figures/scripts/e5_4_median.py")
    print("  python3 report/source/figures/scripts/e5_2_bracket_compare.py")


if __name__ == "__main__":
    main()
