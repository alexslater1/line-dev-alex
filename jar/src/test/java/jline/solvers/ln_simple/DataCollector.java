package jline.solvers.ln_simple;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;

import java.io.BufferedWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Collections;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

/**
 * Side-effect data capture for the LN-simple evaluation suite.
 *
 * <p>One row is appended per fixture by
 * {@link util#assertResultsMatchSolverLN(java.util.function.Supplier)} when
 * invoked from a recognised evaluation-suite test method. Rows accumulate
 * in memory and are flushed to {@code <outdir>/<dataset>.{csv,json}} via a
 * JVM shutdown hook so a single {@code mvn test} invocation produces the
 * dataset as a side effect of running the suite.
 *
 * <p>Configuration is via system properties so the same harness captures
 * baseline and ablation runs without further code changes:
 * <ul>
 *   <li>{@code baseline.outdir} — output directory (default {@code ../report/data})</li>
 *   <li>{@code baseline.dataset} — dataset filename stem (default {@code baseline})</li>
 *   <li>{@code baseline.condition} — value of the {@code condition} column
 *       (default {@code baseline}; ablation runs use {@code F19off},
 *       {@code F20off}, {@code F24off}, or {@code allOff})</li>
 * </ul>
 *
 * <p>Capture can be suspended (during JIT warmup, for example) by setting
 * {@link util#dataCaptureSuspended} to {@code true}; rows recorded while
 * suspended are silently dropped.
 */
public final class DataCollector {

    private static final String OUTDIR_PROP   = "baseline.outdir";
    private static final String DATASET_PROP  = "baseline.dataset";
    private static final String CONDITION_PROP = "baseline.condition";
    private static final String DEFAULT_OUTDIR   = "../report/data";
    private static final String DEFAULT_DATASET  = "baseline";
    private static final String DEFAULT_CONDITION = "baseline";

    /** One row per fixture. Null-valued fields serialize as empty in CSV
     *  and {@code null} in JSON. */
    public static final class Row {
        public String  fixture_name;
        public String  partition;
        public String  condition;
        public Integer sim_iters;
        public Long    sim_time_ms;
        public Integer ln_iters;
        public Long    ln_time_ms;
        public Integer lqns_iters;
        public Long    lqns_time_ms;
        public Double  sim_vs_ln_max_rel_diff;
        public Double  sim_vs_lqns_max_rel_diff;
        public String  matches_within_tol;
        public String  notes;
    }

    private static final List<Row> rows = Collections.synchronizedList(new ArrayList<Row>());
    private static volatile boolean shutdownHookInstalled = false;

    private DataCollector() {}

    public static void addRow(Row row) {
        if (util.dataCaptureSuspended) return;
        if (row.condition == null) {
            row.condition = System.getProperty(CONDITION_PROP, DEFAULT_CONDITION);
        }
        installShutdownHookIfNeeded();
        rows.add(row);
    }

    /** Test-friendly accessor for sanity-checking; rows accumulated so far. */
    public static List<Row> snapshot() {
        synchronized (rows) {
            return new ArrayList<Row>(rows);
        }
    }

    private static synchronized void installShutdownHookIfNeeded() {
        if (shutdownHookInstalled) return;
        Runtime.getRuntime().addShutdownHook(new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    flush();
                } catch (Throwable t) {
                    PrintWriter pw = new PrintWriter(System.err);
                    pw.println("[DataCollector] Flush failed:");
                    t.printStackTrace(pw);
                    pw.flush();
                }
            }
        }, "DataCollector-flush"));
        shutdownHookInstalled = true;
    }

    /** Visible for direct invocation (e.g. from a debugger or capture script);
     *  the shutdown hook also calls this. Safe to call twice. */
    public static synchronized void flush() throws IOException {
        List<Row> snapshot;
        synchronized (rows) {
            if (rows.isEmpty()) return;
            snapshot = new ArrayList<Row>(rows);
        }
        Path outdir = Paths.get(System.getProperty(OUTDIR_PROP, DEFAULT_OUTDIR));
        String dataset = System.getProperty(DATASET_PROP, DEFAULT_DATASET);
        Files.createDirectories(outdir);
        Path csvPath  = outdir.resolve(dataset + ".csv");
        Path jsonPath = outdir.resolve(dataset + ".json");
        writeCsv(csvPath, snapshot);
        writeJson(jsonPath, snapshot);
        System.out.println("[DataCollector] Wrote " + snapshot.size() + " rows to "
                + csvPath.toAbsolutePath() + " and " + jsonPath.toAbsolutePath());
    }

    private static void writeCsv(Path path, List<Row> snapshot) throws IOException {
        try (BufferedWriter bw = Files.newBufferedWriter(path)) {
            bw.write("fixture_name,partition,condition,sim_iters,sim_time_ms,ln_iters,ln_time_ms,"
                    + "lqns_iters,lqns_time_ms,sim_vs_ln_max_rel_diff,sim_vs_lqns_max_rel_diff,"
                    + "matches_within_tol,notes\n");
            for (Row r : snapshot) {
                bw.write(csvEscape(r.fixture_name));        bw.write(',');
                bw.write(csvEscape(r.partition));           bw.write(',');
                bw.write(csvEscape(r.condition));           bw.write(',');
                bw.write(intOrEmpty(r.sim_iters));          bw.write(',');
                bw.write(longOrEmpty(r.sim_time_ms));       bw.write(',');
                bw.write(intOrEmpty(r.ln_iters));           bw.write(',');
                bw.write(longOrEmpty(r.ln_time_ms));        bw.write(',');
                bw.write(intOrEmpty(r.lqns_iters));         bw.write(',');
                bw.write(longOrEmpty(r.lqns_time_ms));      bw.write(',');
                bw.write(doubleOrEmpty(r.sim_vs_ln_max_rel_diff));  bw.write(',');
                bw.write(doubleOrEmpty(r.sim_vs_lqns_max_rel_diff)); bw.write(',');
                bw.write(csvEscape(r.matches_within_tol));  bw.write(',');
                bw.write(csvEscape(r.notes));               bw.write('\n');
            }
        }
    }

    private static void writeJson(Path path, List<Row> snapshot) throws IOException {
        Gson gson = new GsonBuilder()
                .setPrettyPrinting()
                .serializeSpecialFloatingPointValues()
                .serializeNulls()
                .create();
        List<Map<String, Object>> jsonRows = new ArrayList<Map<String, Object>>();
        for (Row r : snapshot) {
            Map<String, Object> m = new LinkedHashMap<String, Object>();
            m.put("fixture_name", r.fixture_name);
            m.put("partition", r.partition);
            m.put("condition", r.condition);
            m.put("sim_iters", r.sim_iters);
            m.put("sim_time_ms", r.sim_time_ms);
            m.put("ln_iters", r.ln_iters);
            m.put("ln_time_ms", r.ln_time_ms);
            m.put("lqns_iters", r.lqns_iters);
            m.put("lqns_time_ms", r.lqns_time_ms);
            m.put("sim_vs_ln_max_rel_diff", r.sim_vs_ln_max_rel_diff);
            m.put("sim_vs_lqns_max_rel_diff", r.sim_vs_lqns_max_rel_diff);
            m.put("matches_within_tol", r.matches_within_tol);
            m.put("notes", r.notes);
            jsonRows.add(m);
        }
        try (BufferedWriter bw = Files.newBufferedWriter(path)) {
            gson.toJson(jsonRows, bw);
        }
    }

    private static String csvEscape(String s) {
        if (s == null) return "";
        if (s.indexOf(',') >= 0 || s.indexOf('"') >= 0 || s.indexOf('\n') >= 0) {
            return "\"" + s.replace("\"", "\"\"") + "\"";
        }
        return s;
    }

    private static String intOrEmpty(Integer v) { return v == null ? "" : v.toString(); }
    private static String longOrEmpty(Long v)   { return v == null ? "" : v.toString(); }

    private static String doubleOrEmpty(Double v) {
        if (v == null) return "";
        if (v.isInfinite()) return v > 0 ? "Infinity" : "-Infinity";
        if (v.isNaN()) return "NaN";
        return v.toString();
    }
}
