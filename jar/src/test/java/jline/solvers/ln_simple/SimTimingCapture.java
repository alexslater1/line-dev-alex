// =============================================================================
// SimTimingCapture — sim-only timing re-capture under the production default
// sweep order (ELEVATOR), WITHOUT re-running the slow SolverLN.
//
// Motivation: the BOUNCE->ELEVATOR default switch changed SolverLNSimple's
// per-fixture wall time and iteration count, but the SolverLN / SolverLQNS
// reference columns are sweep-order-invariant. This harness re-measures ONLY
// the SolverLNSimple side, reusing the exact timing path of the baseline
// capture (util.runSolverLNSimple: currentTimeMillis around construct+solve,
// default sweep = ELEVATOR), so the new sim_iters / sim_time_ms values are
// methodology-compatible with baseline-r*.csv.
//
// It sweeps the full fixture corpus once UNTIMED (global JIT warmup), then
// performs <rounds> timed sweeps. Round r corresponds to baseline-r{r}.csv:
// whole-corpus interleaving between samples mirrors the original 3-JVM
// baseline-r1/r2/r3 structure. report/scripts/patch-baseline-timing.py then
// overwrites ONLY the sim_iters/sim_time_ms columns of baseline-r{1..N}.csv
// and baseline.csv; e5_4_median.py rebuilds baseline-medianed.csv as usual.
//
//   mvn -Dtmp=false test -DskipTests=false -Dtest=SimTimingCapture#run \
//     -Dsimtiming.rounds=3 \
//     -Dsimtiming.out=report/data/sim-timing-elevator.csv
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

public final class SimTimingCapture {

    @org.junit.jupiter.api.Test
    @org.junit.jupiter.api.Timeout(value = 3600)
    public void run() throws Exception {
        int rounds = Integer.getInteger("simtiming.rounds", 3);
        String outPath = System.getProperty("simtiming.out",
                "report/data/sim-timing-elevator.csv");

        GlobalConstants.setVerbose(VerboseLevel.SILENT);
        // Drop any rows the timed runSolverLNSimple calls might try to record.
        util.dataCaptureSuspended = true;

        LinkedHashMap<String, Supplier<LayeredNetwork>> fixtures = FastEval.FIXTURES_PUBLIC();
        System.out.println("[simtiming] " + fixtures.size() + " fixtures, rounds="
                + rounds + " (sweep = ELEVATOR default), skipping SolverLN");

        // Global JIT warmup: one untimed sweep of the whole corpus so every
        // fixture shape has compiled before the first timed round (mirrors the
        // warmed JVMs the original baseline-r* captures ran in).
        for (Map.Entry<String, Supplier<LayeredNetwork>> e : fixtures.entrySet()) {
            util.runSolverLNSimple(e.getValue().get());
        }

        // sim_iters[fixture] (deterministic) and per-round timing samples.
        LinkedHashMap<String, Integer> iters = new LinkedHashMap<String, Integer>();
        LinkedHashMap<String, long[]>  times = new LinkedHashMap<String, long[]>();
        for (String name : fixtures.keySet()) times.put(name, new long[rounds]);

        for (int r = 0; r < rounds; r++) {
            for (Map.Entry<String, Supplier<LayeredNetwork>> e : fixtures.entrySet()) {
                String name = e.getKey();
                util.runSolverLNSimple(e.getValue().get());
                times.get(name)[r] = util.getLastSimpleTimeMs();
                Integer it = util.getLastSimpleIters();
                Integer prev = iters.get(name);
                if (prev != null && !prev.equals(it)) {
                    System.err.printf("[simtiming] WARN %s iters varies across rounds: %d vs %d%n",
                            name, prev, it);
                }
                iters.put(name, it);
            }
            System.out.printf("[simtiming] round %d/%d done%n", r + 1, rounds);
        }

        BufferedWriter bw = new BufferedWriter(new FileWriter(outPath));
        try {
            StringBuilder hdr = new StringBuilder("fixture_name,sim_iters,rounds");
            for (int r = 0; r < rounds; r++) hdr.append(",t").append(r + 1);
            bw.write(hdr.toString());
            bw.write("\n");
            for (String name : fixtures.keySet()) {
                StringBuilder sb = new StringBuilder();
                sb.append(name).append(',').append(iters.get(name)).append(',').append(rounds);
                long[] ts = times.get(name);
                for (int r = 0; r < rounds; r++) sb.append(',').append(ts[r]);
                bw.write(sb.toString());
                bw.write("\n");
            }
        } finally {
            bw.close();
        }

        int totalIters = 0;
        for (Integer v : iters.values()) totalIters += v;
        System.out.println("[simtiming] wrote " + fixtures.size() + " rows to " + outPath
                + "  (corpus sim_iters total = " + totalIters + ")");
    }
}
