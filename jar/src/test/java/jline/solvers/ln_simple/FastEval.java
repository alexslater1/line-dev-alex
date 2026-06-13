package jline.solvers.ln_simple;

import jline.GlobalConstants;
import jline.VerboseLevel;
import jline.lang.layered.LayeredNetwork;
import jline.solvers.LayeredNetworkAvgTable;
import jline.solvers.ln_simple.fixtures.EvaluationSuite;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.function.Supplier;

/**
 * Fast LNSimple-only correctness + iteration + runtime harness for the
 * performance evaluation suite. Loads a cached LN reference
 * table from CSV; for each of the 59 Partition A/B/C fixtures (excluding
 * the two long-running B fan-outs), runs SolverLNSimple, captures iter
 * count + wall ms, and computes max relative slack vs the cached LN
 * reference using atol=1e-3, rtol=5e-2 (the project's standard tol).
 *
 * <p>Invocation (one-shot, NOT a JUnit @Test — drive via mvn exec or a
 * dedicated test wrapper):
 *
 * <pre>
 *   -Dfasteval.ref=report/data/f20axis-20260605T173243Z/baseline.tables.csv
 *   -Dfasteval.out=report/data/overnight-exploration/perf-port/run-XXX.csv
 *   -Dfasteval.repeats=5         (timed repeats; defaults to 1)
 *   -Dfasteval.warmup=3          (JIT warmup runs per fixture)
 *   -Dfasteval.fixtures=A1_*,C2_infTaskSimple   (optional filter)
 * </pre>
 */
public final class FastEval {

    private static final double ATOL = 1e-3;
    private static final double RTOL = 5e-2;
    // LQNS reference uses looser rtol — matches existing util.LQNS_DATASET_RTOL.
    private static final double LQNS_ATOL = 1e-3;
    private static final double LQNS_RTOL = 1e-1;

    /** Pre-existing baseline test failures (see SolverLNSimpleFullEvaluationTest
     *  run logs). LNSimple's coarser fan-out handling diverges from both LN and
     *  LQNS (which itself reports ResidT as NaN, leaving LN as the only
     *  reference). These are NOT regressions introduced by this session — they
     *  are flagged but excluded from the pass/fail gate so that an experiment
     *  whose only "failure" is matching baseline behaviour on these fixtures is
     *  still considered correct. */
    private static final java.util.Set<String> KNOWN_BASELINE_FAILURES = new java.util.HashSet<String>(java.util.Arrays.asList(
            "B_scaleAxis_tasks5_fanout",
            "B_scaleAxis_tasks10_fanout",
            "B_scaleAxis_tasks20_fanout"
    ));

    /** Baseline slack ceilings for the known-failing fixtures, recorded from
     *  the captured baseline. An experiment regresses if its slack on these
     *  exceeds the ceiling by more than {@link #REGRESSION_TOL}. */
    private static final java.util.Map<String, Double> BASELINE_FAIL_SLACK = new java.util.HashMap<String, Double>();
    static {
        BASELINE_FAIL_SLACK.put("B_scaleAxis_tasks5_fanout",  2.41);
        BASELINE_FAIL_SLACK.put("B_scaleAxis_tasks10_fanout", 2.42);
        BASELINE_FAIL_SLACK.put("B_scaleAxis_tasks20_fanout", 1.11);
    }
    private static final double REGRESSION_TOL = 0.05;  // 5 % over the baseline ceiling

    /** Final list of fixtures the gate evaluates. 59 entries = 61 in the
     *  baseline corpus minus the two long-running B fan-outs.
     *  Order matches the corpus / report grouping (A1, A2, A3, A4, B, C1, C2, C3, C4). */
    /** Package-private access for sibling tools (e.g., CaptureLnReference). */
    public static LinkedHashMap<String, Supplier<LayeredNetwork>> FIXTURES_PUBLIC() { return FIXTURES; }
    private static final LinkedHashMap<String, Supplier<LayeredNetwork>> FIXTURES = new LinkedHashMap<String, Supplier<LayeredNetwork>>();
    static {
        FIXTURES.put("A1_chain_d3_N50",  EvaluationSuite::A1_chain_d3_N50);
        FIXTURES.put("A1_chain_d5_N20",  EvaluationSuite::A1_chain_d5_N20);
        FIXTURES.put("A1_chain_d5_N50",  EvaluationSuite::A1_chain_d5_N50);
        FIXTURES.put("A1_chain_d5_N100", EvaluationSuite::A1_chain_d5_N100);
        FIXTURES.put("A1_chain_d8_N50",  EvaluationSuite::A1_chain_d8_N50);

        FIXTURES.put("A2_multiclassA_2refs_loadLow_N20",      EvaluationSuite::A2_multiclassA_2refs_loadLow_N20);
        FIXTURES.put("A2_multiclassA_2refs_loadHigh_N20",     EvaluationSuite::A2_multiclassA_2refs_loadHigh_N20);
        FIXTURES.put("A2_multiclassA_2refs_loadLow_N50",      EvaluationSuite::A2_multiclassA_2refs_loadLow_N50);
        FIXTURES.put("A2_multiclassA_2refs_loadHigh_N50",     EvaluationSuite::A2_multiclassA_2refs_loadHigh_N50);
        FIXTURES.put("A2_multiclassB_2refs_demands_3to1_N20", EvaluationSuite::A2_multiclassB_2refs_demands_3to1_N20);
        FIXTURES.put("A2_multiclassB_2refs_demands_5to1_N20", EvaluationSuite::A2_multiclassB_2refs_demands_5to1_N20);
        FIXTURES.put("A2_multiclassB_2refs_demands_3to1_N50", EvaluationSuite::A2_multiclassB_2refs_demands_3to1_N50);
        FIXTURES.put("A2_multiclassB_2refs_demands_5to1_N50", EvaluationSuite::A2_multiclassB_2refs_demands_5to1_N50);
        FIXTURES.put("A2_multiclassC_2refs_think_2to1_N20",   EvaluationSuite::A2_multiclassC_2refs_think_2to1_N20);
        FIXTURES.put("A2_multiclassC_2refs_think_4to1_N20",   EvaluationSuite::A2_multiclassC_2refs_think_4to1_N20);
        FIXTURES.put("A2_multiclassC_2refs_think_2to1_N50",   EvaluationSuite::A2_multiclassC_2refs_think_2to1_N50);
        FIXTURES.put("A2_multiclassC_2refs_think_4to1_N50",   EvaluationSuite::A2_multiclassC_2refs_think_4to1_N50);

        FIXTURES.put("A3_multiserver_c2_loadLow",    EvaluationSuite::A3_multiserver_c2_loadLow);
        FIXTURES.put("A3_multiserver_c2_loadMedium", EvaluationSuite::A3_multiserver_c2_loadMedium);
        FIXTURES.put("A3_multiserver_c2_loadHigh",   EvaluationSuite::A3_multiserver_c2_loadHigh);
        FIXTURES.put("A3_multiserver_c4_loadLow",    EvaluationSuite::A3_multiserver_c4_loadLow);
        FIXTURES.put("A3_multiserver_c4_loadMedium", EvaluationSuite::A3_multiserver_c4_loadMedium);
        FIXTURES.put("A3_multiserver_c4_loadHigh",   EvaluationSuite::A3_multiserver_c4_loadHigh);
        FIXTURES.put("A3_multiserver_c8_loadLow",    EvaluationSuite::A3_multiserver_c8_loadLow);
        FIXTURES.put("A3_multiserver_c8_loadMedium", EvaluationSuite::A3_multiserver_c8_loadMedium);
        FIXTURES.put("A3_multiserver_c8_loadHigh",   EvaluationSuite::A3_multiserver_c8_loadHigh);

        FIXTURES.put("A4_threeTier_multiclass",   EvaluationSuite::A4_threeTier_multiclass);
        FIXTURES.put("A4_multiclass_multiserver", EvaluationSuite::A4_multiclass_multiserver);
        FIXTURES.put("A4_multiclass_multientry",  EvaluationSuite::A4_multiclass_multientry);
        FIXTURES.put("A4_chainPlusFanout",        EvaluationSuite::A4_chainPlusFanout);
        FIXTURES.put("A4_webApplication",         EvaluationSuite::A4_webApplication);
        FIXTURES.put("A4_databaseBackend",        EvaluationSuite::A4_databaseBackend);
        FIXTURES.put("A4_threeTier_sequence",      EvaluationSuite::A4_threeTier_sequence);
        FIXTURES.put("A4_parallelService_andFork", EvaluationSuite::A4_parallelService_andFork);
        FIXTURES.put("A4_retryLoop",               EvaluationSuite::A4_retryLoop);
        FIXTURES.put("A4_replyThenLog",            EvaluationSuite::A4_replyThenLog);

        FIXTURES.put("A5_orForkUneven",      EvaluationSuite::A5_orForkUneven);
        FIXTURES.put("A5_orForkThreeWay",    EvaluationSuite::A5_orForkThreeWay);
        FIXTURES.put("A5_andForkThreeWay",   EvaluationSuite::A5_andForkThreeWay);
        FIXTURES.put("A5_andForkMultiAct",   EvaluationSuite::A5_andForkMultiAct);
        FIXTURES.put("A5_seqMultiEntry",     EvaluationSuite::A5_seqMultiEntry);
        FIXTURES.put("A5_replyAtBound",      EvaluationSuite::A5_replyAtBound);
        FIXTURES.put("A5_combinedDag",       EvaluationSuite::A5_combinedDag);

        FIXTURES.put("B_scaleAxis_tasks5_chain",   EvaluationSuite::B_scaleAxis_tasks5_chain);
        FIXTURES.put("B_scaleAxis_tasks10_chain",  EvaluationSuite::B_scaleAxis_tasks10_chain);
        FIXTURES.put("B_scaleAxis_tasks20_chain",  EvaluationSuite::B_scaleAxis_tasks20_chain);
        FIXTURES.put("B_scaleAxis_tasks40_chain",  EvaluationSuite::B_scaleAxis_tasks40_chain);
        FIXTURES.put("B_scaleAxis_tasks60_chain",  EvaluationSuite::B_scaleAxis_tasks60_chain);
        FIXTURES.put("B_scaleAxis_tasks5_fanout",  EvaluationSuite::B_scaleAxis_tasks5_fanout);
        FIXTURES.put("B_scaleAxis_tasks10_fanout", EvaluationSuite::B_scaleAxis_tasks10_fanout);
        FIXTURES.put("B_scaleAxis_tasks20_fanout", EvaluationSuite::B_scaleAxis_tasks20_fanout);
        // Session-2: added back for the full-corpus reconciliation. The LN
        // reference takes ~7 min and ~28 min respectively so these are still
        // excluded from the routine gate by default. Run with
        // -Dfasteval.fixtures=B_scaleAxis_tasks40_fanout,B_scaleAxis_tasks60_fanout
        // to target them specifically without re-running the 59-fixture corpus.
        FIXTURES.put("B_scaleAxis_tasks40_fanout", EvaluationSuite::B_scaleAxis_tasks40_fanout);
        FIXTURES.put("B_scaleAxis_tasks60_fanout", EvaluationSuite::B_scaleAxis_tasks60_fanout);

        FIXTURES.put("C1_fanOut2_caller_c1", EvaluationSuite::C1_fanOut2_caller_c1);
        FIXTURES.put("C1_fanOut2_caller_c2", EvaluationSuite::C1_fanOut2_caller_c2);
        FIXTURES.put("C1_fanOut3_caller_c1", EvaluationSuite::C1_fanOut3_caller_c1);
        FIXTURES.put("C1_fanOut3_caller_c2", EvaluationSuite::C1_fanOut3_caller_c2);
        FIXTURES.put("C1_fanOut4_caller_c1", EvaluationSuite::C1_fanOut4_caller_c1);
        FIXTURES.put("C1_fanOut4_caller_c2", EvaluationSuite::C1_fanOut4_caller_c2);

        FIXTURES.put("C2_infTaskSimple", EvaluationSuite::C2_infTaskSimple);
        FIXTURES.put("C2_infTaskShared", EvaluationSuite::C2_infTaskShared);
        FIXTURES.put("C2_infTaskDeep",   EvaluationSuite::C2_infTaskDeep);

        FIXTURES.put("C3_multiclassMultiEntry_2refs",      EvaluationSuite::C3_multiclassMultiEntry_2refs);
        FIXTURES.put("C3_multiclassMultiEntry_3refs",      EvaluationSuite::C3_multiclassMultiEntry_3refs);
        FIXTURES.put("C3_multiclassMultiEntry_asymmetric", EvaluationSuite::C3_multiclassMultiEntry_asymmetric);

        FIXTURES.put("C4_nearSaturation",     EvaluationSuite::C4_nearSaturation);
        FIXTURES.put("C4_predicateThreshold", EvaluationSuite::C4_predicateThreshold);
        FIXTURES.put("C4_lowDemand",          EvaluationSuite::C4_lowDemand);

        // C5/C6/C7 — boundary fixtures expected to diverge from one or both
        // references (saturation regime, finite-server-host AND-fork, deep-chain
        // stress). Registered in the measurement pipeline, which records
        // match-status into the CSV without asserting, rather than in the
        // hard-asserting JUnit gate (SolverLNSimpleEvaluationTest). The corpus
        // tables report them honestly as match-lqns-only / match-neither.
        FIXTURES.put("C5_satFanout_tasks5",     EvaluationSuite::C5_satFanout_tasks5);
        FIXTURES.put("C5_satFanout_tasks10",    EvaluationSuite::C5_satFanout_tasks10);
        FIXTURES.put("C5_satFanout_tasks20",    EvaluationSuite::C5_satFanout_tasks20);
        FIXTURES.put("C5_satFanoutCaller_w2",   EvaluationSuite::C5_satFanoutCaller_w2);
        FIXTURES.put("C5_satFanoutCaller_w3",   EvaluationSuite::C5_satFanoutCaller_w3);
        FIXTURES.put("C5_satFanoutCaller_w4",   EvaluationSuite::C5_satFanoutCaller_w4);
        FIXTURES.put("C6_andForkPsHost",        EvaluationSuite::C6_andForkPsHost);
        FIXTURES.put("C6_multiclassAndFork",    EvaluationSuite::C6_multiclassAndFork);
        FIXTURES.put("C7_deepChainMultiCall",   EvaluationSuite::C7_deepChainMultiCall);
        FIXTURES.put("C7_deepChainAllExp",      EvaluationSuite::C7_deepChainAllExp);
    }

    /** Per (fixture, node_name): the 5 reference metrics [qlen, util, respt, residt, tput].
     *  Filters rows by solver label (e.g. "LN" or "LQNS"). */
    static Map<String, Map<String, double[]>> loadReference(String csvPath, String solverLabel) throws Exception {
        Map<String, Map<String, double[]>> ref = new HashMap<String, Map<String, double[]>>();
        BufferedReader br = new BufferedReader(new FileReader(csvPath));
        String line = br.readLine();  // header
        // columns: fixture_name,partition,condition,solver,node_name,node_type,server_count,qlen,util,respt,residt,tput
        while ((line = br.readLine()) != null) {
            String[] f = line.split(",", -1);
            if (f.length < 12) continue;
            if (!solverLabel.equals(f[3])) continue;
            String fixture = f[0];
            String node = f[4];
            double q = parseD(f[7]);
            double u = parseD(f[8]);
            double rt = parseD(f[9]);
            double rd = parseD(f[10]);
            double t = parseD(f[11]);
            ref.computeIfAbsent(fixture, k -> new HashMap<String, double[]>())
                    .put(node, new double[]{q, u, rt, rd, t});
        }
        br.close();
        return ref;
    }

    /** Backwards-compatible alias for the older method name. */
    static Map<String, Map<String, double[]>> loadLnReference(String csvPath) throws Exception {
        return loadReference(csvPath, "LN");
    }

    private static double parseD(String s) {
        if (s == null || s.isEmpty() || "NaN".equalsIgnoreCase(s)) return Double.NaN;
        if ("Infinity".equalsIgnoreCase(s)) return Double.POSITIVE_INFINITY;
        if ("-Infinity".equalsIgnoreCase(s)) return Double.NEGATIVE_INFINITY;
        try { return Double.parseDouble(s); } catch (NumberFormatException e) { return Double.NaN; }
    }

    /** atol=1e-3, rtol=5e-2 slack; NaN-vs-NaN -> 0; NaN-vs-finite -> NaN (skipped). */
    private static double slack(double expected, double actual) {
        return slackWith(expected, actual, ATOL, RTOL);
    }

    private static double slackWith(double expected, double actual, double atol, double rtol) {
        if (Double.isNaN(expected) && Double.isNaN(actual)) return 0.0;
        if (Double.isNaN(expected) || Double.isNaN(actual)) return Double.NaN;
        double diff = Math.abs(expected - actual);
        double allowed = atol + rtol * Math.abs(expected);
        return diff / allowed;
    }

    /** Match-either rule: per-metric slack is the min of (vs-LN with strict
     *  tol) and (vs-LQNS with loose tol). NaN-vs-NaN returns 0; finite-vs-NaN
     *  on one side falls through to the comparable side; NaN on both → NaN
     *  (caller skips). Mirrors {@code util.minSkipNaN}. */
    private static double slackEither(double lnExp, double lqnsExp, double actual) {
        double sLn   = slackWith(lnExp,   actual, ATOL,      RTOL);
        double sLqns = slackWith(lqnsExp, actual, LQNS_ATOL, LQNS_RTOL);
        boolean aNaN = Double.isNaN(sLn);
        boolean bNaN = Double.isNaN(sLqns);
        if (aNaN && bNaN) return Double.NaN;
        if (aNaN) return sLqns;
        if (bNaN) return sLn;
        return Math.min(sLn, sLqns);
    }

    public static final class FixtureResult {
        public final String name;
        public final int iters;
        public final long timeMsMedian;
        public final long timeMsMin;
        public final long timeMsMax;
        public final double maxSlack;
        public final String worstMsg;
        public final boolean pass;
        public final int nodesCompared;
        public final long[] timesMs;
        public FixtureResult(String n, int it, long med, long mn, long mx,
                             double ms, String wm, boolean p, int nc, long[] tm) {
            name = n; iters = it; timeMsMedian = med; timeMsMin = mn; timeMsMax = mx;
            maxSlack = ms; worstMsg = wm; pass = p; nodesCompared = nc; timesMs = tm;
        }
    }

    public static FixtureResult runOne(String fixtureName,
                                       Supplier<LayeredNetwork> factory,
                                       Map<String, double[]> ref,
                                       int warmup, int repeats) {
        return runOne(fixtureName, factory, ref, null, warmup, repeats);
    }

    public static FixtureResult runOne(String fixtureName,
                                       Supplier<LayeredNetwork> factory,
                                       Map<String, double[]> lnRef,
                                       Map<String, double[]> lqnsRef,
                                       int warmup, int repeats) {
        // JIT warmup — untimed runs to settle compilation
        for (int w = 0; w < warmup; w++) {
            LayeredNetwork m = factory.get();
            SolverLNSimple s = new SolverLNSimple(m);
            int[] iters = {0};
            s.iterateCoupledMva(() -> iters[0]++);
        }

        // Timed runs
        long[] times = new long[repeats];
        int iters = -1;
        LayeredNetworkAvgTable lastTable = null;
        for (int r = 0; r < repeats; r++) {
            LayeredNetwork m = factory.get();
            SolverLNSimple s = new SolverLNSimple(m);
            int[] it = {0};
            long t0 = System.nanoTime();
            s.iterateCoupledMva(() -> it[0]++);
            long t1 = System.nanoTime();
            times[r] = (t1 - t0) / 1_000_000L;
            iters = it[0];
            lastTable = s.getAvgTable();
        }
        long[] sorted = times.clone();
        Arrays.sort(sorted);
        long med = sorted[sorted.length / 2];
        long mn = sorted[0];
        long mx = sorted[sorted.length - 1];

        // Correctness vs cached LN (strict) and LQNS (loose) references —
        // match-either rule: per (node, metric), pass if EITHER reference is
        // within tolerance. Mirrors the existing util.assertResultsMatchSolverLN
        // semantic where LNSimple is accepted if it tracks LN OR LQNS.
        double maxSlack = 0.0;
        String worstMsg = null;
        int compared = 0;
        if (lastTable != null && lnRef != null) {
            List<String> names = lastTable.getNodeNames();
            List<Double> q = lastTable.getQLen();
            List<Double> u = lastTable.getUtil();
            List<Double> rt = lastTable.getRespT();
            List<Double> rd = lastTable.getResidT();
            List<Double> tp = lastTable.getTput();
            String[] metrics = {"QLen", "Util", "RespT", "ResidT", "Tput"};
            for (int i = 0; i < names.size(); i++) {
                String node = names.get(i);
                double[] lnRow   = lnRef.get(node);
                double[] lqnsRow = lqnsRef != null ? lqnsRef.get(node) : null;
                if (lnRow == null && lqnsRow == null) continue;
                double[] actual = {q.get(i), u.get(i), rt.get(i), rd.get(i), tp.get(i)};
                for (int mi = 0; mi < 5; mi++) {
                    double lnExp   = lnRow   != null ? lnRow[mi]   : Double.NaN;
                    double lqnsExp = lqnsRow != null ? lqnsRow[mi] : Double.NaN;
                    double s = (lqnsRow != null)
                            ? slackEither(lnExp, lqnsExp, actual[mi])
                            : slack(lnExp, actual[mi]);
                    if (Double.isNaN(s)) continue;
                    compared++;
                    if (s > maxSlack) {
                        maxSlack = s;
                        worstMsg = String.format("%s.%s: LNSimple=%.6f vs LN=%.6f LQNS=%.6f slack=%.3f",
                                node, metrics[mi], actual[mi], lnExp, lqnsExp, s);
                    }
                }
            }
        }
        boolean pass = maxSlack <= 1.0;
        return new FixtureResult(fixtureName, iters, med, mn, mx, maxSlack, worstMsg, pass,
                compared, times);
    }

    /** JUnit wrapper so we can invoke the harness via mvn-surefire without
     *  needing exec-maven-plugin classpath wiring. Pure delegation to main. */
    @org.junit.jupiter.api.Test
    @org.junit.jupiter.api.Timeout(value = 1800)
    public void runFastEval() throws Exception {
        main(new String[0]);
    }

    public static void main(String[] args) throws Exception {
        String refPath = System.getProperty("fasteval.ref",
            "report/data/overnight-exploration/data/ln-reference.tables.csv");
        // The LQNS reference comes from the existing baseline.tables.csv —
        // LQNS values are stable across sessions (REST call to a Docker
        // service), so the existing capture is reusable.
        String lqnsRefPath = System.getProperty("fasteval.lqnsref",
                "report/data/f20axis-20260605T173243Z/baseline.tables.csv");
        String outPath = System.getProperty("fasteval.out",
            "report/data/overnight-exploration/perf-port/fasteval-latest.csv");
        int repeats = Integer.getInteger("fasteval.repeats", 1);
        int warmup  = Integer.getInteger("fasteval.warmup", 3);
        String filter = System.getProperty("fasteval.fixtures", "");

        GlobalConstants.setVerbose(VerboseLevel.SILENT);

        Map<String, Map<String, double[]>> ref = loadReference(refPath, "LN");
        Map<String, Map<String, double[]>> lqnsRef = null;
        try {
            lqnsRef = loadReference(lqnsRefPath, "LQNS");
        } catch (Exception e) {
            System.err.println("[fasteval] WARN: no LQNS reference loaded: " + e.getMessage());
        }

        // Silence stdout/stderr from SolverLNSimple (it prints per-iteration lines).
        PrintStream origOut = System.out;
        PrintStream origErr = System.err;
        java.io.OutputStream nullOut = new java.io.OutputStream() {
            @Override public void write(int b) {}
            @Override public void write(byte[] b, int o, int l) {}
        };
        PrintStream sink = new PrintStream(nullOut);

        List<FixtureResult> results = new ArrayList<FixtureResult>();
        long globalStart = System.currentTimeMillis();
        int failed = 0;

        // Cross-fixture JVM warmup: run the smallest fixture a few times before
        // any timed work, so the first measured fixture isn't penalised.
        Supplier<LayeredNetwork> warmupFactory = FIXTURES.get("A1_chain_d3_N50");
        if (warmupFactory != null) {
            System.setOut(sink); System.setErr(sink);
            try {
                for (int w = 0; w < 3; w++) {
                    LayeredNetwork m = warmupFactory.get();
                    new SolverLNSimple(m).iterateCoupledMva(() -> {});
                }
            } finally {
                System.setOut(origOut); System.setErr(origErr);
            }
        }

        for (Map.Entry<String, Supplier<LayeredNetwork>> e : FIXTURES.entrySet()) {
            String name = e.getKey();
            if (!filter.isEmpty()) {
                boolean match = false;
                for (String pat : filter.split(",")) {
                    pat = pat.trim();
                    if (pat.endsWith("*")) {
                        if (name.startsWith(pat.substring(0, pat.length() - 1))) { match = true; break; }
                    } else if (name.equals(pat)) { match = true; break; }
                }
                if (!match) continue;
            }
            System.setOut(sink); System.setErr(sink);
            FixtureResult fr;
            try {
                Map<String, double[]> lqnsRow = lqnsRef != null ? lqnsRef.get(name) : null;
                fr = runOne(name, e.getValue(), ref.get(name), lqnsRow, warmup, repeats);
            } finally {
                System.setOut(origOut); System.setErr(origErr);
            }
            results.add(fr);
            String status;
            if (fr.pass) {
                status = "PASS";
            } else if (KNOWN_BASELINE_FAILURES.contains(fr.name)) {
                Double ceiling = BASELINE_FAIL_SLACK.get(fr.name);
                if (ceiling != null && fr.maxSlack <= ceiling * (1.0 + REGRESSION_TOL)) {
                    status = "KNOWN-FAIL (no regression)";
                } else {
                    status = String.format("REGRESSION over known baseline (%.3f > %.3f) %s",
                            fr.maxSlack, ceiling, fr.worstMsg == null ? "" : fr.worstMsg);
                    failed++;
                }
            } else {
                status = "FAIL  " + (fr.worstMsg == null ? "" : fr.worstMsg);
                failed++;
            }
            origOut.printf("%-45s iter=%2d  time_med=%5d ms  time_min=%5d ms  slack=%.3f  %s%n",
                    fr.name, fr.iters, fr.timeMsMedian, fr.timeMsMin, fr.maxSlack, status);
        }

        long elapsed = System.currentTimeMillis() - globalStart;

        // Aggregates
        int totalIters = 0;
        long totalTimeMs = 0;
        for (FixtureResult fr : results) {
            totalIters += fr.iters;
            totalTimeMs += fr.timeMsMedian;
        }
        origOut.printf("%n=== FastEval summary: %d fixtures, %d failed, total elapsed %.1f s ===%n",
                results.size(), failed, elapsed / 1000.0);
        origOut.printf("Total iters: %d   Total median time: %d ms%n", totalIters, totalTimeMs);

        // Write per-fixture CSV
        java.io.File outFile = new java.io.File(outPath);
        outFile.getParentFile().mkdirs();
        BufferedWriter bw = new BufferedWriter(new FileWriter(outFile));
        bw.write("fixture_name,iters,time_ms_median,time_ms_min,time_ms_max,max_slack_vs_ln,nodes_compared,pass,all_times_ms\n");
        for (FixtureResult fr : results) {
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < fr.timesMs.length; i++) {
                if (i > 0) sb.append('|');
                sb.append(fr.timesMs[i]);
            }
            bw.write(String.format("%s,%d,%d,%d,%d,%.6f,%d,%s,%s%n",
                    fr.name, fr.iters, fr.timeMsMedian, fr.timeMsMin, fr.timeMsMax,
                    fr.maxSlack, fr.nodesCompared, fr.pass ? "true" : "false", sb.toString()));
        }
        bw.close();
        origOut.printf("Wrote: %s%n", outPath);

        if (failed > 0) {
            System.exit(1);
        }
    }
}
