package jline.solvers.ln_simple;

import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.junit.jupiter.api.Assertions.fail;

import java.io.ByteArrayOutputStream;
import java.io.PrintStream;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.function.Supplier;
import java.util.logging.Level;
import java.util.logging.Logger;

import jline.lang.constant.SchedStrategy;
import jline.lang.constant.SolverType;
import jline.lang.layered.*;
import jline.lang.processes.Exp;
import jline.lang.processes.Immediate;
import jline.solvers.LayeredNetworkAvgTable;
import jline.solvers.ln.SolverLN;
import jline.solvers.lqns.SolverLQNS;

public class util {

    private static final double RTOL = 5e-2;
    private static final double ATOL = 1e-3;

    /** Looser tolerance for the {@code match-lqns-only} dataset classification
     *  — LN and LQNS already disagree on several topologies the suite probes,
     *  so a slightly looser bar makes the "tracks at least one reference"
     *  signal meaningful. Mirrors the rtol/atol pair documented in the
     *  evaluation-suite spec. */
    private static final double LQNS_DATASET_RTOL = 1e-1;
    private static final double LQNS_DATASET_ATOL = 1e-3;

    /** When {@code true}, dataset rows are not appended by
     *  {@link #assertResultsMatchSolverLN(Supplier)}. Used to silence the
     *  JIT-warmup runs in {@code @BeforeAll} so they do not pollute the
     *  baseline. */
    public static volatile boolean dataCaptureSuspended = false;

    // Per-fixture solver telemetry. Each {@code run*} method writes its
    // iteration count and wall-clock time to these fields immediately
    // before returning; the caller reads them while still in the same
    // single-threaded sequence. The values are reset to {@code null} at
    // the top of each {@link #assertResultsMatchSolverLN} call.
    private static Integer lastSimpleIters;
    private static Long    lastSimpleTimeMs;
    private static Integer lastLnIters;
    private static Long    lastLnTimeMs;
    private static Integer lastLqnsIters;
    private static Long    lastLqnsTimeMs;
    private static boolean lastLqnsAvailable;

    static {
        Logger.getLogger("").setLevel(Level.OFF);
        Logger.getLogger("jline").setLevel(Level.OFF);
        Logger.getLogger("org.apache.commons.io.FileUtils").setLevel(Level.OFF);
    }

    public static void assertResultsMatchSolverLN(LayeredNetwork model) {
        assertResultsMatchSolverLN(() -> model);
    }

    /**
     * Variant taking a {@link Supplier} so each solver runs on its own fresh
     * model. SolverLNSimple, SolverLN, and SolverLQNS all mutate the shared
     * {@link LayeredNetwork}'s ensemble (via {@code Queue.setService} /
     * {@code Delay.setService}); without fresh models the second and third
     * solvers see post-iteration state from the first and produce results that
     * depend on call order.
     */
    public static void assertResultsMatchSolverLN(Supplier<LayeredNetwork> modelFactory) {
        LayeredNetwork simpleModel = modelFactory.get();
        assertNotNull(simpleModel);

        // Reset per-fixture telemetry before each solver call writes back.
        lastSimpleIters = null;  lastSimpleTimeMs = null;
        lastLnIters = null;      lastLnTimeMs = null;
        lastLqnsIters = null;    lastLqnsTimeMs = null;
        lastLqnsAvailable = false;

        // Temporary instrumentation: count which MvaInputs.callMVA dispatch
        // path actually fired, so each test reports its model's exact mix.
        jline.solvers.ln_simple.mva.MvaInputs.resetDispatchCounters();
        LayeredNetworkAvgTable simpleTable = runSolverLNSimple(simpleModel);
        System.out.printf("[paths] LD=%d AMVA=%d cap=%d%n",
                jline.solvers.ln_simple.mva.MvaInputs.ldCalls,
                jline.solvers.ln_simple.mva.MvaInputs.amvaCalls,
                jline.solvers.ln_simple.mva.MvaInputs.exactLatticeMax);
        LayeredNetworkAvgTable lnTable = runSolverLN(modelFactory.get());

        // LQNS unavailability is non-fatal for the dataset — record null
        // and continue. Existing fixtures all have LQNS working, but
        // ablation runs (or environments without the lqns-rest container
        // up) must still produce comparable rows.
        LayeredNetworkAvgTable lqnsTable = null;
        String lqnsFailureNote = null;
        try {
            lqnsTable = runSolverLQNS(modelFactory.get());
            lastLqnsAvailable = true;
        } catch (Throwable t) {
            lqnsFailureNote = "LQNS unavailable: " + t.getClass().getSimpleName()
                    + (t.getMessage() != null ? (": " + t.getMessage().split("\n", 2)[0]) : "");
            System.out.printf("[SolverLQNS]     FAILED: %s%n", lqnsFailureNote);
            lastLqnsIters = null;
            lastLqnsTimeMs = null;
        }

        List<Double> simpleQLen  = simpleTable.getQLen();
        List<Double> simpleUtil  = simpleTable.getUtil();
        List<Double> simpleRespT = simpleTable.getRespT();
        List<Double> simpleResidT= simpleTable.getResidT();
        List<Double> simpleTput  = simpleTable.getTput();

        List<Double> lnQLen  = lnTable.getQLen();
        List<Double> lnUtil  = lnTable.getUtil();
        List<Double> lnRespT = lnTable.getRespT();
        List<Double> lnResidT= lnTable.getResidT();
        List<Double> lnTput  = lnTable.getTput();

        List<Double> lqnsQLen  = lqnsTable != null ? lqnsTable.getQLen()   : Collections.<Double>emptyList();
        List<Double> lqnsUtil  = lqnsTable != null ? lqnsTable.getUtil()   : Collections.<Double>emptyList();
        List<Double> lqnsRespT = lqnsTable != null ? lqnsTable.getRespT()  : Collections.<Double>emptyList();
        List<Double> lqnsResidT= lqnsTable != null ? lqnsTable.getResidT() : Collections.<Double>emptyList();
        List<Double> lqnsTput  = lqnsTable != null ? lqnsTable.getTput()   : Collections.<Double>emptyList();

        // LQNS reports Util as the total ΣX_r·D_r, while LN (and LNSimple) report
        // per-server Util = ΣX_r·D_r / c for finite multi-server PS processors.
        // For an INF processor, both conventions coincide (c_eff = 1). Build a
        // {node name → processor server count} map so we can normalise LQNS's
        // Util before the comparison.
        Map<String, Integer> nodeServerCount = buildNodeServerCountMap(simpleModel);

        // LQNS leaves Util NaN on activity rows. When LN reports a finite value
        // we can synthesize the LQNS-equivalent per-server activity util as
        // X_activity · D_activity / c, using the activity's own host demand and
        // throughput LQNS does report (the Tput column).
        Map<String, Double> activityHostDemand = buildActivityHostDemandMap(simpleModel);

        String context = "\n\n=== SolverLN ===\n" + formatTable(lnTable)
                    + "=== SolverLNSimple ===\n" + formatTable(simpleTable)
                    + "=== SolverLQNS ===\n" + (lqnsTable != null ? formatTable(lqnsTable) : "(unavailable)\n");

        System.out.println(context);

        // Compare against both reference solvers; pass if LNSimple is within
        // tolerance of EITHER. LN and LQNS disagree on some topologies
        // (notably fan-out with immediate caller activities or multi-server
        // PS callees with high per-server utilisation); when they diverge it
        // is enough for LNSimple to track one of them.
        double maxSlack = 0.0;
        String worstMsg = null;
        // Dataset accumulators — track the worst slack vs each reference
        // separately and the worst raw relative difference, all over every
        // (node, metric) pair. Used by DataCollector to classify the row.
        double maxSlackVsLn   = 0.0;
        double maxSlackVsLqns = 0.0;
        double maxRelDiffVsLn   = 0.0;
        double maxRelDiffVsLqns = 0.0;
        boolean anyComparableLn   = false;
        boolean anyComparableLqns = false;
        List<String> lnNames = lnTable.getNodeNames();
        for (int i = 0; i < lnNames.size(); i++) {
            String name = lnNames.get(i);
            String[] metrics = {"QLen", "Util", "RespT", "ResidT", "Tput"};
            double[] lnVals = {
                lnQLen.get(i), lnUtil.get(i), lnRespT.get(i), lnResidT.get(i), lnTput.get(i)
            };
            double[] lqnsVals;
            if (lqnsTable != null && i < lqnsQLen.size()) {
                // LQNS doesn't populate the ResidT column directly for tasks
                // even though the value is implicit (Q/X). Synthesize it so
                // the "match either reference" comparison still has something
                // to compare against on this metric.
                double lqnsResidDerived = lqnsResidT.get(i);
                if (Double.isNaN(lqnsResidDerived)) {
                    double q = lqnsQLen.get(i);
                    double x = lqnsTput.get(i);
                    if (Double.isFinite(q) && Double.isFinite(x) && x > 0) {
                        lqnsResidDerived = q / x;
                    }
                }
                // Normalise LQNS Util to LN's per-server convention. For
                // activity rows LQNS leaves Util as NaN even though X (Tput)
                // is reported; synthesize util = X·D/c from the model's host
                // demand so we have something to compare against.
                double lqnsUtilNormalised = lqnsUtil.get(i);
                Integer c = nodeServerCount.get(name);
                if (c != null && c > 1 && Double.isFinite(lqnsUtilNormalised)) {
                    lqnsUtilNormalised = lqnsUtilNormalised / c;
                } else if (Double.isNaN(lqnsUtilNormalised)) {
                    Double hd = activityHostDemand.get(name);
                    double x = lqnsTput.get(i);
                    if (hd != null && Double.isFinite(hd) && hd > 0
                            && Double.isFinite(x) && x > 0 && c != null && c > 0) {
                        lqnsUtilNormalised = x * hd / c;
                    }
                }
                lqnsVals = new double[]{
                    lqnsQLen.get(i), lqnsUtilNormalised, lqnsRespT.get(i),
                    lqnsResidDerived, lqnsTput.get(i)
                };
            } else {
                lqnsVals = new double[]{Double.NaN, Double.NaN, Double.NaN, Double.NaN, Double.NaN};
            }
            double[] simpleVals = {
                simpleQLen.get(i), simpleUtil.get(i), simpleRespT.get(i),
                simpleResidT.get(i), simpleTput.get(i)
            };
            for (int m = 0; m < metrics.length; m++) {
                double actual = simpleVals[m];
                double slackLn   = relativeSlack(lnVals[m],   actual);
                double slackLqns = relativeSlack(lqnsVals[m], actual);
                // NaN-vs-finite is treated as infinite slack; the helper
                // returns Double.POSITIVE_INFINITY for that case. NaN-vs-NaN
                // and both-finite-within-tol return < 1.0.
                double bestSlack = Math.min(slackLn, slackLqns);
                if (bestSlack > maxSlack) {
                    maxSlack = bestSlack;
                    worstMsg = String.format(
                        "%s.%s: SolverLNSimple=%.6f (SolverLN=%.6f slack=%.2f, SolverLQNS=%.6f slack=%.2f)%s",
                        name, metrics[m], actual,
                        lnVals[m], slackLn, lqnsVals[m], slackLqns, context);
                }
                // Dataset bookkeeping: track per-reference worst slack
                // (with reference-specific tolerance) and worst raw rel diff.
                if (slackLn > maxSlackVsLn) maxSlackVsLn = slackLn;
                double relLn = rawRelDiff(lnVals[m], actual);
                if (relLn > maxRelDiffVsLn) maxRelDiffVsLn = relLn;
                if (!(Double.isNaN(lnVals[m]) && Double.isNaN(actual))) anyComparableLn = true;
                if (lastLqnsAvailable) {
                    double slackLqnsLoose = relativeSlackWith(lqnsVals[m], actual,
                            LQNS_DATASET_ATOL, LQNS_DATASET_RTOL);
                    if (slackLqnsLoose > maxSlackVsLqns) maxSlackVsLqns = slackLqnsLoose;
                    double relLqns = rawRelDiff(lqnsVals[m], actual);
                    if (relLqns > maxRelDiffVsLqns) maxRelDiffVsLqns = relLqns;
                    if (!(Double.isNaN(lqnsVals[m]) && Double.isNaN(actual))) anyComparableLqns = true;
                }
            }
        }

        // Record a dataset row before any potential fail() — both passing
        // and failing fixtures contribute their data. DataCollector silently
        // drops the row if dataCaptureSuspended is true (JIT warmup).
        recordDatasetRow(
                lastLqnsAvailable,
                anyComparableLn   ? maxRelDiffVsLn   : Double.NaN,
                anyComparableLqns ? maxRelDiffVsLqns : Double.NaN,
                maxSlackVsLn,
                lastLqnsAvailable ? maxSlackVsLqns : Double.POSITIVE_INFINITY,
                lqnsFailureNote);

        if (maxSlack > 1.0) {
            fail(worstMsg);
        }
    }

    // =========================================================================
    //  Dataset capture helpers
    // =========================================================================

    /** Classify-and-record helper called once per fixture from
     *  {@link #assertResultsMatchSolverLN(Supplier)}. */
    private static void recordDatasetRow(boolean lqnsAvailable,
                                         double maxRelDiffVsLn,
                                         double maxRelDiffVsLqns,
                                         double maxSlackVsLn,
                                         double maxSlackVsLqns,
                                         String lqnsFailureNote) {
        String fixtureName = findEvaluationTestName();
        if (fixtureName == null) return;  // not invoked from the eval suite
        DataCollector.Row row = new DataCollector.Row();
        row.fixture_name = fixtureName;
        row.partition = partitionOf(fixtureName);
        row.sim_iters = lastSimpleIters;
        row.sim_time_ms = lastSimpleTimeMs;
        row.ln_iters = lastLnIters;
        row.ln_time_ms = lastLnTimeMs;
        if (lqnsAvailable) {
            row.lqns_iters = lastLqnsIters;
            row.lqns_time_ms = lastLqnsTimeMs;
            row.sim_vs_lqns_max_rel_diff = Double.isNaN(maxRelDiffVsLqns) ? null : maxRelDiffVsLqns;
        } else {
            row.lqns_iters = null;
            row.lqns_time_ms = null;
            row.sim_vs_lqns_max_rel_diff = null;
        }
        row.sim_vs_ln_max_rel_diff = Double.isNaN(maxRelDiffVsLn) ? null : maxRelDiffVsLn;
        if (maxSlackVsLn <= 1.0) {
            row.matches_within_tol = "match-ln";
        } else if (lqnsAvailable && maxSlackVsLqns <= 1.0) {
            row.matches_within_tol = "match-lqns-only";
        } else {
            row.matches_within_tol = "match-neither";
        }
        row.notes = lqnsFailureNote != null ? lqnsFailureNote : "";
        DataCollector.addRow(row);
    }

    /** Walk the stack and return the method name on
     *  {@code SolverLNSimpleEvaluationTest}, or {@code null} if not invoked
     *  from that class. We only want to capture rows from the canonical
     *  evaluation suite, not from {@code SolverLNSimpleFailingTest} or the
     *  other ad-hoc test classes that share the same helper. */
    private static String findEvaluationTestName() {
        StackTraceElement[] stack = Thread.currentThread().getStackTrace();
        for (StackTraceElement e : stack) {
            if (e.getClassName().endsWith(".SolverLNSimpleEvaluationTest")) {
                return e.getMethodName();
            }
        }
        return null;
    }

    /** Map fixture name → partition string ({@code A1}, {@code A2}, {@code A3},
     *  {@code A4}, {@code B}, {@code C1}, {@code C2}, {@code C3}, {@code C4}).
     *  Returns {@code "?"} if the name does not start with one of the known
     *  prefixes — which would indicate a manifest/test drift worth surfacing. */
    private static String partitionOf(String fixtureName) {
        if (fixtureName == null) return "?";
        if (fixtureName.startsWith("A1_")) return "A1";
        if (fixtureName.startsWith("A2_")) return "A2";
        if (fixtureName.startsWith("A3_")) return "A3";
        if (fixtureName.startsWith("A4_")) return "A4";
        if (fixtureName.startsWith("A5_")) return "A5";
        if (fixtureName.startsWith("B_"))  return "B";
        if (fixtureName.startsWith("C1_")) return "C1";
        if (fixtureName.startsWith("C2_")) return "C2";
        if (fixtureName.startsWith("C3_")) return "C3";
        if (fixtureName.startsWith("C4_")) return "C4";
        if (fixtureName.startsWith("C5_")) return "C5";
        if (fixtureName.startsWith("C6_")) return "C6";
        if (fixtureName.startsWith("C7_")) return "C7";
        return "?";
    }

    /** Slack ratio with caller-supplied tolerances. Same semantics as
     *  {@link #relativeSlack(double, double)} but parameterised so the LQNS
     *  "match-lqns-only" classification can use the looser
     *  ({@code atol=1e-3, rtol=1e-1}) bar. */
    private static double relativeSlackWith(double expected, double actual,
                                             double atol, double rtol) {
        if (Double.isNaN(expected) && Double.isNaN(actual)) return 0.0;
        if (Double.isNaN(expected) || Double.isNaN(actual)) return Double.POSITIVE_INFINITY;
        double diff = Math.abs(expected - actual);
        double allowed = atol + rtol * Math.abs(expected);
        return diff / allowed;
    }

    /** Raw relative difference {@code |actual − expected| / max(|expected|, 1e-6)}
     *  for dataset reporting. The {@code 1e-6} floor prevents division blowups
     *  when {@code expected} is zero. NaN handling matches the slack helpers:
     *  NaN-vs-NaN → 0, NaN-vs-finite → {@code +Infinity}. */
    private static double rawRelDiff(double expected, double actual) {
        if (Double.isNaN(expected) && Double.isNaN(actual)) return 0.0;
        if (Double.isNaN(expected) || Double.isNaN(actual)) return Double.POSITIVE_INFINITY;
        double diff = Math.abs(actual - expected);
        double denom = Math.max(Math.abs(expected), 1e-6);
        return diff / denom;
    }

    /**
     * Map each node name in the model (processor, task, entry, activity) to
     * the server count of its hosting processor. INF processors and any task
     * whose processor cannot be resolved map to {@code 1} so that the LQNS
     * Util normalisation is a no-op for them.
     */
    private static Map<String, Integer> buildNodeServerCountMap(LayeredNetwork model) {
        Map<String, Integer> out = new HashMap<String, Integer>();
        for (Host h : model.getHosts().values()) {
            int c = serverCountOf(h);
            out.put(h.getName(), c);
            for (Task t : h.getTasks()) {
                out.put(t.getName(), c);
                for (Entry e : t.getEntries()) {
                    out.put(e.getName(), c);
                }
                for (Activity a : t.getActivities()) {
                    out.put(a.getName(), c);
                }
            }
        }
        return out;
    }

    /** Map each activity name to its host demand mean. Used to synthesize
     *  LQNS's per-server activity Util when LQNS reports NaN. */
    private static Map<String, Double> buildActivityHostDemandMap(LayeredNetwork model) {
        Map<String, Double> out = new HashMap<String, Double>();
        for (Activity a : model.getActivities().values()) {
            double hd = a.getHostDemandMean();
            if (Double.isFinite(hd) && hd > 0) {
                out.put(a.getName(), hd);
            }
        }
        return out;
    }

    /** Effective server count for a host — INF or {@code Integer.MAX_VALUE}
     *  collapse to 1, matching LN's "X·D" reporting convention for IS hosts. */
    private static int serverCountOf(Host h) {
        if (h.getScheduling() == SchedStrategy.INF) return 1;
        int m = h.getMultiplicity();
        if (m == Integer.MAX_VALUE || m <= 0) return 1;
        return m;
    }

    /**
     * Slack ratio for a single (expected, actual) pair: {@code |diff| /
     * (atol + rtol·|expected|)}. {@code <= 1.0} means within tolerance.
     *
     * <p>NaN handling: NaN-vs-NaN returns 0 (match); NaN-vs-finite or
     * finite-vs-NaN returns {@code Double.POSITIVE_INFINITY} so the metric is
     * treated as a definite mismatch.
     */
    private static double relativeSlack(double expected, double actual) {
        if (Double.isNaN(expected) && Double.isNaN(actual)) return 0.0;
        if (Double.isNaN(expected) || Double.isNaN(actual)) return Double.POSITIVE_INFINITY;
        double diff = Math.abs(expected - actual);
        double allowed = ATOL + RTOL * Math.abs(expected);
        return diff / allowed;
    }

    public static LayeredNetworkAvgTable runSolverLNSimple(LayeredNetwork model) {
        SolverLNSimple solver = new SolverLNSimple(model);
        int[] iters = {0};
        long start = System.currentTimeMillis();
        suppressOutput(() -> solver.iterateCoupledMva(() -> iters[0]++));
        long elapsed = System.currentTimeMillis() - start;
        lastSimpleIters = iters[0];
        lastSimpleTimeMs = elapsed;
        System.out.printf("[SolverLNSimple] %.3f s, %d iters%n", elapsed / 1000.0, iters[0]);
        return solver.getAvgTable();
    }

    public static LayeredNetworkAvgTable runSolverLN(LayeredNetwork model) {
        final LayeredNetworkAvgTable[] holder = new LayeredNetworkAvgTable[1];
        final SolverLN[] solverHolder = new SolverLN[1];
        long start = System.currentTimeMillis();
        suppressOutput(() -> {
            solverHolder[0] = new SolverLN(model, SolverType.MVA);
            holder[0] = (LayeredNetworkAvgTable) solverHolder[0].getAvgTable();
        });
        long elapsed = System.currentTimeMillis() - start;
        int iters = solverHolder[0] != null && solverHolder[0].maxitererr != null ? solverHolder[0].maxitererr.size() : -1;
        lastLnIters = iters;
        lastLnTimeMs = elapsed;
        System.out.printf("[SolverLN]       %.3f s, %d iters%n", elapsed / 1000.0, iters);
        return holder[0];
    }

    public static LayeredNetworkAvgTable runSolverLQNS(LayeredNetwork model) {
        final LayeredNetworkAvgTable[] holder = new LayeredNetworkAvgTable[1];
        final SolverLQNS[] solverHolder = new SolverLQNS[1];
        long start = System.currentTimeMillis();
        suppressOutput(() -> {
            solverHolder[0] = new SolverLQNS(model);
            holder[0] = (LayeredNetworkAvgTable) solverHolder[0].getAvgTable();
        });
        long elapsed = System.currentTimeMillis() - start;
        int iters = solverHolder[0] != null && solverHolder[0].result != null ? solverHolder[0].result.iter : -1;
        lastLqnsIters = iters;
        lastLqnsTimeMs = elapsed;
        System.out.printf("[SolverLQNS]     %.3f s, %d iters%n", elapsed / 1000.0, iters);
        return holder[0];
    }

    public static void suppressOutput(Runnable action) {
        PrintStream originalOut = System.out;
        PrintStream originalErr = System.err;
        ByteArrayOutputStream sink = new ByteArrayOutputStream();
        PrintStream sinkStream = new PrintStream(sink);

        Logger rootLogger = Logger.getLogger("");
        Logger jlineLogger = Logger.getLogger("jline");
        Logger fileUtilsLogger = Logger.getLogger("org.apache.commons.io.FileUtils");
        Level savedRoot = rootLogger.getLevel();
        Level savedJline = jlineLogger.getLevel();
        Level savedFileUtils = fileUtilsLogger.getLevel();

        try {
            System.setOut(sinkStream);
            System.setErr(sinkStream);
            rootLogger.setLevel(Level.OFF);
            jlineLogger.setLevel(Level.OFF);
            fileUtilsLogger.setLevel(Level.OFF);
            action.run();
        } finally {
            System.setOut(originalOut);
            System.setErr(originalErr);
            rootLogger.setLevel(savedRoot);
            jlineLogger.setLevel(savedJline);
            fileUtilsLogger.setLevel(savedFileUtils);
            sinkStream.close();
        }
    }

    public static String formatTable(LayeredNetworkAvgTable table) {
        List<String> names = table.getNodeNames();
        List<String> types = table.getNodeTypes();
        List<Double> qlen  = table.getQLen();
        List<Double> util  = table.getUtil();
        List<Double> respT = table.getRespT();
        List<Double> residT= table.getResidT();
        List<Double> arvR  = table.getArvR();
        List<Double> tput  = table.getTput();

        StringBuilder sb = new StringBuilder();
        sb.append(String.format("%-10s\t%-10s\t%-10s\t%-10s\t%-10s\t%-10s\t%-10s\t%-10s%n",
                "Node", "NodeType", "QLen", "Util", "RespT", "ResidT", "ArvR", "Tput"));
        for (int i = 0; i < names.size(); i++) {
            sb.append(String.format("%-10s\t%-10s\t%-10s\t%-10s\t%-10s\t%-10s\t%-10s\t%-10s%n",
                    names.get(i), types.get(i),
                    fv(qlen.get(i)), fv(util.get(i)), fv(respT.get(i)),
                    fv(residT.get(i)), fv(arvR.get(i)), fv(tput.get(i))));
        }
        return sb.toString();
    }

    public static String fv(double v) {
        if (Double.isNaN(v)) return "NaN";
        if (v == 0.0) return "0";
        return String.format("%.5f", v);
    }

    public static void assertClose(String node, String metric, double expected, double actual, String context) {
        if (Double.isNaN(expected) && Double.isNaN(actual)) return;
        if (Double.isNaN(expected) || Double.isNaN(actual)) {
            fail(String.format("%s.%s: SolverLN=%.6f SolverLNSimple=%.6f (one is NaN)%s",
                    node, metric, expected, actual, context));
        }
        double diff = Math.abs(expected - actual);
        double allowed = ATOL + RTOL * Math.abs(expected);
        assertTrue(diff <= allowed,
                String.format("%s.%s: SolverLN=%.6f SolverLNSimple=%.6f (|diff|=%.2e > atol+rtol*|exp|=%.2e)%s",
                        node, metric, expected, actual, diff, allowed, context));
    }

    // =========================================================================
    //  Model builders
    // =========================================================================

    /**
     * {@code tiers}-tier chain {@code T1 → T2 → … → T_tiers}. {@code T1} is REF
     * with the given population; intermediate tasks are INF; the leaf task
     * holds the only non-trivial service time. Each processor has 4 PS servers.
     */
    public static LayeredNetwork chain(int tiers, int n) {
        LayeredNetwork m = new LayeredNetwork("chain" + tiers + "_N" + n);
        Processor[] P = new Processor[tiers];
        Task[] T = new Task[tiers];
        Entry[] E = new Entry[tiers];
        for (int i = 0; i < tiers; i++) {
            P[i] = new Processor(m, "P" + (i + 1), 4, SchedStrategy.PS);
            if (i == 0) {
                T[i] = new Task(m, "T1", n, SchedStrategy.REF).on(P[i]).setThinkTime(new Exp(2));
            } else {
                T[i] = new Task(m, "T" + (i + 1), Integer.MAX_VALUE, SchedStrategy.INF).on(P[i]);
            }
            E[i] = new Entry(m, "E" + (i + 1)).on(T[i]);
        }
        // Intermediates: immediate activity that calls forward and replies to caller's entry.
        new Activity(m, "AS1", Immediate.getInstance()).on(T[0]).boundTo(E[0]).synchCall(E[1], 1);
        for (int i = 1; i < tiers - 1; i++) {
            new Activity(m, "AS" + (i + 1), Immediate.getInstance())
                    .on(T[i]).boundTo(E[i]).synchCall(E[i + 1], 1).repliesTo(E[i - 1]);
        }
        // Leaf: real service time, replies to its caller.
        new Activity(m, "AS" + tiers, Exp.fitMean(0.8))
                .on(T[tiers - 1]).boundTo(E[tiers - 1]).repliesTo(E[tiers - 2]);
        return m;
    }

    /**
     * One REF task with {@code N} customers calls {@code k} independent INF
     * callees, each on its own 4-server PS processor.
     */
    public static LayeredNetwork fanOut(int k, int n) {
        LayeredNetwork m = new LayeredNetwork("fanOut" + k + "_N" + n);
        Processor P0 = new Processor(m, "P0", 4, SchedStrategy.PS);
        Task T0 = new Task(m, "T0", n, SchedStrategy.REF).on(P0).setThinkTime(new Exp(2));
        Entry E0 = new Entry(m, "E0").on(T0);

        Entry[] calleeEntries = new Entry[k];
        for (int i = 0; i < k; i++) {
            Processor Pi = new Processor(m, "P" + (i + 1), 4, SchedStrategy.PS);
            Task Ti = new Task(m, "T" + (i + 1), Integer.MAX_VALUE, SchedStrategy.INF).on(Pi);
            calleeEntries[i] = new Entry(m, "E" + (i + 1)).on(Ti);
            new Activity(m, "AS" + (i + 1), Exp.fitMean(0.6 + 0.2 * i))
                    .on(Ti).boundTo(calleeEntries[i]).repliesTo(calleeEntries[i]);
        }
        // One caller activity chains a synchCall to every callee.
        Activity caller = new Activity(m, "A0", Immediate.getInstance()).on(T0).boundTo(E0);
        for (Entry ce : calleeEntries) caller = caller.synchCall(ce, 1);
        return m;
    }

    /**
     * {@code k} REF callers (each with {@code n} customers) onto a shared
     * {@code c}-server PS callee task. Each caller has its own entry on the
     * callee for proper per-class accounting.
     */
    public static LayeredNetwork sharedCallee(int k, int n, int cShared) {
        LayeredNetwork m = new LayeredNetwork("shared" + k + "_N" + n);
        Processor P1 = new Processor(m, "P1", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PS = new Processor(m, "PS", cShared, SchedStrategy.PS);
        Task TS = new Task(m, "TS", Integer.MAX_VALUE, SchedStrategy.INF).on(PS);
        Task[] callers = new Task[k];
        Entry[] callerEntries = new Entry[k];
        Entry[] calleeEntries = new Entry[k];
        for (int r = 0; r < k; r++) {
            callers[r] = new Task(m, "T" + (r + 1), n, SchedStrategy.REF)
                    .on(P1).setThinkTime(Exp.fitMean(2.0 + 0.3 * r));
            callerEntries[r] = new Entry(m, "E" + (r + 1)).on(callers[r]);
            calleeEntries[r] = new Entry(m, "ES" + (r + 1)).on(TS);
        }
        for (int r = 0; r < k; r++) {
            new Activity(m, "A" + (r + 1), Immediate.getInstance())
                    .on(callers[r]).boundTo(callerEntries[r]).synchCall(calleeEntries[r], 1);
            new Activity(m, "AS" + (r + 1), Exp.fitMean(0.7 + 0.1 * r))
                    .on(TS).boundTo(calleeEntries[r]).repliesTo(calleeEntries[r]);
        }
        return m;
    }

    /**
     * Four REF callers (N=30 each) share callee TS on an 8-server PS, and a
     * separate solo REF caller T5 (N=10) drives its own private callee TX on
     * a 4-server PS. The TS-side layers carry 4 caller classes
     * (prodN ≈ 8 × 10⁵, AMVA); the TX-side layer carries just R:T5
     * (prodN = 11 with S=4 < N=10 so the PX station is a contention point,
     * not IS, LD). The two submodels share no callers, so every activity has
     * exactly one synchCall — this keeps SolverLN's per-activity accounting
     * correct while still forcing the per-layer dispatch split.
     */
    public static LayeredNetwork mixedSharedPlusSolo() {
        LayeredNetwork m = new LayeredNetwork("mixed_shared4_solo1");
        Processor P1 = new Processor(m, "P1", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PS = new Processor(m, "PS", 8, SchedStrategy.PS);
        Processor PX = new Processor(m, "PX", 4, SchedStrategy.PS);
        Task TS = new Task(m, "TS", Integer.MAX_VALUE, SchedStrategy.INF).on(PS);
        Task TX = new Task(m, "TX", Integer.MAX_VALUE, SchedStrategy.INF).on(PX);
        Task[] callers = new Task[4];
        Entry[] callerEntries = new Entry[4];
        Entry[] sharedCalleeEntries = new Entry[4];
        for (int r = 0; r < 4; r++) {
            callers[r] = new Task(m, "T" + (r + 1), 30, SchedStrategy.REF)
                    .on(P1).setThinkTime(Exp.fitMean(2.0 + 0.3 * r));
            callerEntries[r] = new Entry(m, "E" + (r + 1)).on(callers[r]);
            sharedCalleeEntries[r] = new Entry(m, "ES" + (r + 1)).on(TS);
        }
        // Four shared callers, each calls its own entry on TS.
        for (int r = 0; r < 4; r++) {
            new Activity(m, "A" + (r + 1), Immediate.getInstance())
                    .on(callers[r]).boundTo(callerEntries[r])
                    .synchCall(sharedCalleeEntries[r], 1);
        }
        for (int r = 0; r < 4; r++) {
            new Activity(m, "AS" + (r + 1), Exp.fitMean(0.7 + 0.1 * r))
                    .on(TS).boundTo(sharedCalleeEntries[r]).repliesTo(sharedCalleeEntries[r]);
        }
        // Solo caller T5 on its own private callee TX. N=10 > PX's 4 servers
        // keeps the TX-side layer non-IS so it dispatches to LD, not the
        // pure-IS short-circuit.
        Task T5 = new Task(m, "T5", 10, SchedStrategy.REF)
                .on(P1).setThinkTime(Exp.fitMean(2.0));
        Entry E5 = new Entry(m, "E5").on(T5);
        Entry EX = new Entry(m, "EX").on(TX);
        new Activity(m, "A5", Immediate.getInstance())
                .on(T5).boundTo(E5).synchCall(EX, 1);
        new Activity(m, "AX", Exp.fitMean(1.0)).on(TX).boundTo(EX).repliesTo(EX);
        return m;
    }

    /** Three callers, equal N, three orders of magnitude of demand at TS
     *  (0.2, 1.0, 4.0). */
    public static LayeredNetwork asymDemands(int n) {
        LayeredNetwork m = new LayeredNetwork("asymDemands3_N" + n);
        Processor P1 = new Processor(m, "P1", Integer.MAX_VALUE, SchedStrategy.INF);
        Processor PS = new Processor(m, "PS", 8, SchedStrategy.PS);
        Task T1 = new Task(m, "T1", n, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(2.0));
        Task T2 = new Task(m, "T2", n, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(2.0));
        Task T3 = new Task(m, "T3", n, SchedStrategy.REF).on(P1).setThinkTime(Exp.fitMean(2.0));
        Task TS = new Task(m, "TS", Integer.MAX_VALUE, SchedStrategy.INF).on(PS);
        Entry E1  = new Entry(m, "E1").on(T1);
        Entry E2  = new Entry(m, "E2").on(T2);
        Entry E3  = new Entry(m, "E3").on(T3);
        Entry ES1 = new Entry(m, "ES1").on(TS);
        Entry ES2 = new Entry(m, "ES2").on(TS);
        Entry ES3 = new Entry(m, "ES3").on(TS);
        new Activity(m, "A1", Immediate.getInstance()).on(T1).boundTo(E1).synchCall(ES1, 1);
        new Activity(m, "A2", Immediate.getInstance()).on(T2).boundTo(E2).synchCall(ES2, 1);
        new Activity(m, "A3", Immediate.getInstance()).on(T3).boundTo(E3).synchCall(ES3, 1);
        new Activity(m, "AS1", Exp.fitMean(0.2)).on(TS).boundTo(ES1).repliesTo(ES1);
        new Activity(m, "AS2", Exp.fitMean(1.0)).on(TS).boundTo(ES2).repliesTo(ES2);
        new Activity(m, "AS3", Exp.fitMean(4.0)).on(TS).boundTo(ES3).repliesTo(ES3);
        return m;
    }

    /** k REF callers each driving its own armDepth-deep INF chain ending in
     *  a leaf with real Exp demand. Each arm independent → no shared
     *  contention, just sheer layer count. */
    public static LayeredNetwork wideWithChainArms(int k, int armDepth, int n) {
        LayeredNetwork m = new LayeredNetwork("wide" + k + "_arm" + armDepth);
        Processor PRef = new Processor(m, "PR", Integer.MAX_VALUE, SchedStrategy.INF);
        for (int caller = 0; caller < k; caller++) {
            Task R = new Task(m, "R" + caller, n, SchedStrategy.REF)
                    .on(PRef).setThinkTime(Exp.fitMean(1.0 + 0.1 * caller));
            Entry ER = new Entry(m, "ER" + caller).on(R);

            // Build a private chain of armDepth INF tasks for this caller.
            Processor[] P = new Processor[armDepth];
            Task[] T = new Task[armDepth];
            Entry[] E = new Entry[armDepth];
            for (int d = 0; d < armDepth; d++) {
                P[d] = new Processor(m, "P" + caller + "_" + d, 4, SchedStrategy.PS);
                T[d] = new Task(m, "T" + caller + "_" + d, Integer.MAX_VALUE, SchedStrategy.INF).on(P[d]);
                E[d] = new Entry(m, "E" + caller + "_" + d).on(T[d]);
            }
            new Activity(m, "A" + caller, Immediate.getInstance())
                    .on(R).boundTo(ER).synchCall(E[0], 1);
            for (int d = 0; d < armDepth - 1; d++) {
                new Activity(m, "AS" + caller + "_" + d, Immediate.getInstance())
                        .on(T[d]).boundTo(E[d]).synchCall(E[d + 1], 1).repliesTo(d == 0 ? E[0] : E[d - 1]);
            }
            new Activity(m, "AS" + caller + "_leaf", Exp.fitMean(0.8))
                    .on(T[armDepth - 1]).boundTo(E[armDepth - 1])
                    .repliesTo(armDepth == 1 ? E[0] : E[armDepth - 2]);
        }
        return m;
    }
    
}
