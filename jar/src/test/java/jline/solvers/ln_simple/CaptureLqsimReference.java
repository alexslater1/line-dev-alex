package jline.solvers.ln_simple;

import jline.GlobalConstants;
import jline.VerboseLevel;
import jline.lang.layered.LayeredNetwork;
import jline.solvers.LayeredNetworkAvgTable;
import jline.solvers.SolverOptions;
import jline.solvers.lqns.SolverLQNS;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.PrintStream;
import java.util.List;
import java.util.Map;
import java.util.function.Supplier;

/**
 * One-shot lqsim reference capture for the F20-axis bundle.
 *
 * <p>Runs the LQNS <strong>discrete-event simulator</strong> ({@code lqsim},
 * not the analytic LQNS solver) on each fast fixture and writes a tables CSV
 * using the solver label {@code lqsim}, matching the drift-analysis bundle
 * format. The simulator is the ground-truth reference the report's drift-
 * direction analysis (Evaluation §F20) and interlock probe rely on; running
 * analytic LQNS here would silently substitute a different reference.
 *
 * <pre>
 *   -Dlqsim.out=report/data/f20axis-<ts>/lqsim-reference.tables.csv
 *   -Dlqsim.samples=30000           (simulator run-time per block; default 30k)
 *   -Dlqsim.blocks=10               (batch-means blocks; default 10)
 *   -Dlqsim.seed=12345              (RNG seed for reproducibility)
 *   -Dcapture.condition=baseline    (value of the CSV condition column)
 *   -Dcapture.fixtures=A1_*,C2_infTaskSimple   (optional fixture filter)
 * </pre>
 *
 * <p>Invoke via the JUnit wrapper {@link #runLqsimCapture()} (so Surefire's
 * {@code -Dtest=CaptureLqsimReference} actually executes it — a bare
 * {@code main} is not run by {@code mvn test}).
 */
public final class CaptureLqsimReference {

    /** JUnit entry point — {@code mvn test -Dtest=CaptureLqsimReference} runs this. */
    @org.junit.jupiter.api.Test
    @org.junit.jupiter.api.Timeout(value = 7200)
    public void runLqsimCapture() throws Exception {
        main(new String[0]);
    }

    public static void main(String[] args) throws Exception {
        String outPath = System.getProperty("lqsim.out",
                "report/data/f20axis/lqsim-reference.tables.csv");
        int samples = Integer.getInteger("lqsim.samples", 5000);
        int seed = Integer.getInteger("lqsim.seed", 12345);

        // lqsim wall-time is driven almost entirely by run_time (the -T flag,
        // = "samples" here); -B just partitions that run into batch-means
        // blocks and barely affects cost. Empirically -T 30000 takes ~5 min
        // per fixture on these closed models, overrunning SolverLQNS's 300 s
        // read timeout, so the default run_time is kept small (~5k -> ~50 s).
        // Blocks default to 10 (down from SolverLQNS's 30) for a slightly
        // cheaper, still-valid variance estimate; SolverLQNS reads
        // "lqsim.blocks" directly, so set it only if the caller did not.
        if (System.getProperty("lqsim.blocks") == null) {
            System.setProperty("lqsim.blocks", "10");
        }
        String condition = System.getProperty("capture.condition", "baseline");
        String filter = System.getProperty("capture.fixtures", "");

        GlobalConstants.setVerbose(VerboseLevel.SILENT);

        PrintStream origOut = System.out;
        PrintStream origErr = System.err;
        java.io.OutputStream nullOut = new java.io.OutputStream() {
            @Override public void write(int b) {}
            @Override public void write(byte[] b, int o, int l) {}
        };
        PrintStream sink = new PrintStream(nullOut);

        java.io.File outFile = new java.io.File(outPath);
        outFile.getParentFile().mkdirs();
        BufferedWriter bw = new BufferedWriter(new FileWriter(outFile));
        bw.write("fixture_name,partition,condition,solver,node_name,node_type,server_count,qlen,util,respt,residt,tput\n");

        long globalStart = System.currentTimeMillis();
        int n = 0;
        for (Map.Entry<String, Supplier<LayeredNetwork>> e : FastEval.FIXTURES_PUBLIC().entrySet()) {
            String name = e.getKey();
            if (!matchesFilter(name, filter)) continue;
            String partition = partitionOf(name);
            LayeredNetwork model = e.getValue().get();

            // Discrete-event simulator path (method="lqsim"), NOT analytic LQNS.
            // Mirrors SolverLQNSRemoteTest's lqsim configuration.
            LayeredNetworkAvgTable table = null;
            long t0 = System.currentTimeMillis();
            System.setOut(sink); System.setErr(sink);
            try {
                SolverOptions options = new SolverOptions();
                options.method = "lqsim";
                options.samples = samples;
                options.seed = seed;
                SolverLQNS solver = new SolverLQNS(model, options);
                table = (LayeredNetworkAvgTable) solver.getAvgTable();
            } catch (Exception ex) {
                System.setOut(origOut); System.setErr(origErr);
                origOut.printf("%-45s  SKIPPED (%s) (%d/%d)%n",
                        name, ex.getMessage(), ++n, FastEval.FIXTURES_PUBLIC().size());
                bw.write(String.format("%s,%s,%s,lqsim,TIMEOUT,,1,NaN,NaN,NaN,NaN,NaN%n",
                        name, partition, condition));
                bw.flush();
                continue;
            } finally {
                System.setOut(origOut); System.setErr(origErr);
            }
            long t1 = System.currentTimeMillis();
            origOut.printf("%-45s  %5d ms  (%d/%d)%n",
                    name, (t1 - t0), ++n, FastEval.FIXTURES_PUBLIC().size());

            List<String> nodeNames = table.getNodeNames();
            List<String> nodeTypes = table.getNodeTypes();
            List<Double> q  = table.getQLen();
            List<Double> u  = table.getUtil();
            List<Double> rt = table.getRespT();
            List<Double> rd = table.getResidT();
            List<Double> tp = table.getTput();
            for (int i = 0; i < nodeNames.size(); i++) {
                String nodeName = nodeNames.get(i);
                String nodeType = (nodeTypes != null && i < nodeTypes.size()) ? nodeTypes.get(i) : "";
                bw.write(String.format("%s,%s,%s,lqsim,%s,%s,1,%s,%s,%s,%s,%s%n",
                        name, partition, condition, nodeName, nodeType,
                        fmt(q.get(i)), fmt(u.get(i)), fmt(rt.get(i)),
                        fmt(rd.get(i)), fmt(tp.get(i))));
            }
            bw.flush();
        }
        bw.close();
        long elapsed = System.currentTimeMillis() - globalStart;
        origOut.printf("%nWrote %s%nTotal lqsim capture time: %.1f s for %d fixtures%n",
                outPath, elapsed / 1000.0, n);
    }

    private static String fmt(Double d) {
        if (d == null) return "NaN";
        if (Double.isNaN(d)) return "NaN";
        if (Double.isInfinite(d)) return d > 0 ? "Infinity" : "-Infinity";
        return d.toString();
    }

    private static String partitionOf(String name) {
        if (name.startsWith("A1_")) return "A1";
        if (name.startsWith("A2_")) return "A2";
        if (name.startsWith("A3_")) return "A3";
        if (name.startsWith("A4_")) return "A4";
        if (name.startsWith("B_"))  return "B";
        if (name.startsWith("C1_")) return "C1";
        if (name.startsWith("C2_")) return "C2";
        if (name.startsWith("C3_")) return "C3";
        if (name.startsWith("C4_")) return "C4";
        return "?";
    }

    /** Comma-separated patterns; {@code foo*} = prefix match, else exact.
     *  Empty filter matches everything. Mirrors {@link FastEval}'s filter. */
    static boolean matchesFilter(String name, String filter) {
        if (filter == null || filter.isEmpty()) return true;
        for (String pat : filter.split(",")) {
            pat = pat.trim();
            if (pat.isEmpty()) continue;
            if (pat.endsWith("*")) {
                if (name.startsWith(pat.substring(0, pat.length() - 1))) return true;
            } else if (name.equals(pat)) {
                return true;
            }
        }
        return false;
    }
}
