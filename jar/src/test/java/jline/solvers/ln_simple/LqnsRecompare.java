// =============================================================================
// LqnsRecompare — post-run re-derivation harness for the D2/D3 LQNS patch.
//
// Recomputes ONLY the LQNS-comparison columns of the baseline capture, reusing
// the run's CACHED SolverLN per-node tables so the (slow) SolverLN is never
// re-run. SolverLNSimple and SolverLQNS are run fresh; the comparison uses the
// D2-fixed helpers in util.java. Emits recompare.csv, which
// report/scripts/patch-baseline-lqns.py then merges surgically.
//
// Run (after the D2 fix compiles, with the lqns-rest container up):
//   mvn -Dtmp=false test -DskipTests=false -Dtest=LqnsRecompare#run \
//     -Drecompare.lnref=report/data/f20axis-<ts>/baseline.tables.csv \
//     -Drecompare.out=report/data/recompare.csv
// =============================================================================
package jline.solvers.ln_simple;

import jline.GlobalConstants;
import jline.VerboseLevel;
import jline.lang.layered.LayeredNetwork;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.function.Supplier;

public final class LqnsRecompare {

    @org.junit.jupiter.api.Test
    @org.junit.jupiter.api.Timeout(value = 3600)
    public void run() throws Exception {
        String lnRefPath = System.getProperty("recompare.lnref",
                "report/data/baseline.tables.csv");
        String outPath = System.getProperty("recompare.out",
                "report/data/recompare.csv");
        // Optional comma-separated allow-list of fixture names (validation runs).
        String only = System.getProperty("recompare.only", "");
        java.util.Set<String> onlySet = new java.util.HashSet<String>();
        if (!only.isEmpty()) {
            for (String s : only.split(",")) onlySet.add(s.trim());
        }

        GlobalConstants.setVerbose(VerboseLevel.SILENT);

        // Cached SolverLN per-node tables: fixture -> node -> {q,u,respt,residt,tput}.
        Map<String, Map<String, double[]>> lnRef = FastEval.loadReference(lnRefPath, "LN");
        System.out.println("[recompare] loaded cached LN tables for " + lnRef.size() + " fixtures from " + lnRefPath);

        LinkedHashMap<String, Supplier<LayeredNetwork>> fixtures = FastEval.FIXTURES_PUBLIC();

        int written = 0, skipped = 0;
        BufferedWriter bw = new BufferedWriter(new FileWriter(outPath));
        try {
            bw.write("fixture_name,lqns_iters,lqns_time_ms,sim_vs_lqns_max_rel_diff,"
                    + "sim_vs_ln_max_rel_diff,matches_within_tol\n");

            for (Map.Entry<String, Supplier<LayeredNetwork>> e : fixtures.entrySet()) {
                String name = e.getKey();
                if (!onlySet.isEmpty() && !onlySet.contains(name)) continue;
                Map<String, double[]> lnRow = lnRef.get(name);
                if (lnRow == null) {
                    System.err.println("[recompare] no cached LN table for " + name + " — skipping");
                    skipped++;
                    continue;
                }
                util.Recompared r;
                try {
                    r = util.recompareCachedLN(e.getValue().get(), e.getValue().get(), lnRow);
                } catch (Throwable t) {
                    System.err.println("[recompare] FAILED " + name + ": " + t.getMessage());
                    skipped++;
                    continue;
                }
                bw.write(String.format("%s,%s,%s,%s,%s,%s%n",
                        name,
                        r.lqnsIters == null ? "" : r.lqnsIters.toString(),
                        r.lqnsTimeMs == null ? "" : r.lqnsTimeMs.toString(),
                        fmt(r.simVsLqns),
                        fmt(r.simVsLn),
                        r.match));
                bw.flush();
                written++;
                System.out.printf("[recompare] %-32s lqns_iters=%-5s sim_vs_lqns=%-14s match=%s%n",
                        name, r.lqnsIters == null ? "-" : r.lqnsIters, fmt(r.simVsLqns), r.match);
            }
        } finally {
            bw.close();
        }
        System.out.println("[recompare] wrote " + written + " rows (" + skipped + " skipped) to " + outPath);
    }

    private static String fmt(double v) {
        if (Double.isNaN(v)) return "";              // unavailable / not comparable
        if (Double.isInfinite(v)) return v > 0 ? "Infinity" : "-Infinity"; // must NEVER appear post-D2-fix
        return Double.toString(v);
    }
}
