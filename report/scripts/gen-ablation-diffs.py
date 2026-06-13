#!/usr/bin/env python3
"""Generate the 4 ablation diffs (f24off, f17off, f20off, f17_f24off) against the
CURRENT SolverLN.java by string-matching, so the produced unified diffs are
guaranteed to apply with `patch -p1` from the repo root."""
import difflib, os, sys

ROOT = "/home/alex/Documents/FYP/line-dev"
REL = "jar/src/main/kotlin/jline/solvers/ln/SolverLN.java"
SRC = os.path.join(ROOT, REL)
OUT = os.path.join(ROOT, "report/data/diffs")
os.makedirs(OUT, exist_ok=True)

with open(SRC) as f:
    orig = f.readlines()

def find_exact(lines, target):
    """index of the single line whose stripped text == target (stripped)."""
    t = target.strip()
    hits = [i for i, l in enumerate(lines) if l.strip() == t]
    if len(hits) != 1:
        raise SystemExit(f"expected 1 match for {target!r}, got {len(hits)} at {hits}")
    return hits[0]

def indent(line):
    return line[:len(line) - len(line.lstrip())]

# ---- edit primitives (operate on a fresh list copy, return new list) --------
def edit_f24(lines):
    lines = lines[:]
    i = find_exact(lines, "this.ptaskcallers = new Matrix(this.ptaskcallers.getNumRows(), this.ptaskcallers.getNumCols(), this.ptaskcallers.getNumRows() * this.ptaskcallers.getNumCols() - 1);")
    ind = indent(lines[i])
    lines[i:i] = [
        f"{ind}// F24-off ablation: skip the ptaskcallers random walk (output is computed but never consumed).\n",
        f"{ind}if (true) return;\n",
        "\n",
    ]
    return lines

def edit_f17(lines):
    lines = lines[:]
    i = find_exact(lines, "this.lqn = lqnmodel.getStruct();")
    ind = indent(lines[i])
    lines[i+1:i+1] = [
        f"{ind}this.options.config.interlocking = false; // F17-off ablation: disable the interlock pipeline\n",
    ]
    return lines

def edit_f20(lines):
    lines = lines[:]
    # H1 — replace the iter_min floor with 1.
    i = find_exact(lines, "int iter_min = FastMath.max(2 * this.ensemble.length, (int) FastMath.ceil(this.options.iter_max / 4.0));")
    ind = indent(lines[i])
    lines[i] = f"{ind}int iter_min = 1; // F20-off: disable the iter_min convergence floor\n"

    # H2 — disable the moving-average smoothing gate (the live 12-space one, not
    # the dead if(false) branch's 16-space copy).
    target = "            if (it >= iter_min) {\n"
    hits = [k for k, l in enumerate(lines) if l == target]
    if len(hits) != 1:
        raise SystemExit(f"H2: expected 1 moving-avg gate, got {len(hits)} at {hits}")
    k = hits[0]
    lines[k] = "            if (false) { // F20-off: disable the moving-average smoothing\n"

    # H3 — replace the 3-iteration confirmation + hard-reset verification block
    # with single-iteration detection.
    start = next(j for j, l in enumerate(lines)
                 if l.strip().startswith("} else if ((it > iter_min) && (this.maxitererr.get(it) < this.options.iter_tol)"))
    # end = the "}" line immediately followed by "} else {"
    end = next(j for j in range(start + 1, len(lines))
               if lines[j].rstrip() == "            }" and lines[j+1].rstrip() == "        } else {")
    lines[start:end+1] = [
        "        } else if ((it > 1) && (this.maxitererr.get(it) < this.options.iter_tol)) {\n",
        "            // F20-off: single-iteration detection — no iter_min floor, no 3-iteration confirmation, no hard-reset verification pass.\n",
        "            bool = true;\n",
    ]
    return lines

EDITS = {
    "f24off":     [edit_f24],
    "f17off":     [edit_f17],
    "f20off":     [edit_f20],
    "f17_f24off": [edit_f17, edit_f24],
    "alloff":     [edit_f17, edit_f20, edit_f24],   # all spine machinery off
}

for cond, fns in EDITS.items():
    mod = orig
    for fn in fns:
        mod = fn(mod)
    diff = difflib.unified_diff(orig, mod,
                                fromfile=f"a/{REL}", tofile=f"b/{REL}", n=3)
    text = "".join(diff)
    if not text.strip():
        raise SystemExit(f"{cond}: empty diff!")
    path = os.path.join(OUT, f"{cond}.modification.diff")
    with open(path, "w") as f:
        f.write(text)
    print(f"wrote {path}  ({len(text.splitlines())} lines)")
