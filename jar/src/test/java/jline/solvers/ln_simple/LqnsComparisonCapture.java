package jline.solvers.ln_simple;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import jline.GlobalConstants;
import jline.VerboseLevel;
import jline.lang.constant.SchedStrategy;
import jline.lang.layered.Activity;
import jline.lang.layered.Entry;
import jline.lang.layered.Host;
import jline.lang.layered.LayeredNetwork;
import jline.lang.layered.Task;
import jline.solvers.LayeredNetworkAvgTable;
import jline.solvers.ln_simple.fixtures.EvaluationSuite;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.Timeout;

import java.io.BufferedWriter;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.function.Supplier;

/**
 * Capture per-fixture LQNS-vs-LNSimple comparison with NaN-skip semantics so the
 * dataset {@code sim_vs_lqns_max_rel_diff} column becomes finite-and-meaningful
 * (instead of +Infinity, the +Infinity case being the existing
 * {@link util#rawRelDiff} policy of treating NaN-vs-finite as a definite
 * mismatch — see {@code report/data/lqns-diagnosis-findings.md}).
 *
 * <p>For each fixture this runs SolverLNSimple AND SolverLQNS (SolverLN is
 * skipped because its iters/time are condition-dependent and frozen in the
 * existing CSVs anyway), applies the same Util / ResidT normalisation
 * {@link util} does, then computes max-rel-diff and max-slack over (node,
 * metric) pairs where BOTH solvers report a finite value — instead of
 * penalising NaN-vs-finite as +Infinity.
 *
 * <p>Output is a per-fixture JSON record map:
 * <pre>{
 *   "A1_chain_d3_N20": {
 *     "lqns_iters": 50,
 *     "lqns_time_ms": 28,
 *     "sim_vs_lqns_max_rel_diff": 0.018,
 *     "sim_vs_lqns_max_slack_loose": 0.31,
 *     "comparable_pairs": 28,
 *     "first_skipped": "E1.Util (LNSimple=NaN, LQNS=0.0)"
 *   }, ...
 * }</pre>
 *
 * <p>Output path: {@code -Dlqns.cmp.out=<path>}, default
 * {@code ../report/data/lqns-comparison-patch.json}.
 */
public class LqnsComparisonCapture {

    private static final double LOOSE_RTOL = 1e-1;
    private static final double LOOSE_ATOL = 1e-3;

    private static final Map<String, Map<String, Object>> RESULTS =
            new LinkedHashMap<String, Map<String, Object>>();

    @BeforeAll
    public static void setUp() {
        GlobalConstants.setVerbose(VerboseLevel.SILENT);
        Runtime.getRuntime().addShutdownHook(new Thread(new Runnable() {
            @Override
            public void run() {
                String outPath = System.getProperty("lqns.cmp.out",
                        "../report/data/lqns-comparison-patch.json");
                try {
                    Path p = Paths.get(outPath);
                    if (p.getParent() != null) Files.createDirectories(p.getParent());
                    Gson gson = new GsonBuilder()
                            .setPrettyPrinting()
                            .serializeSpecialFloatingPointValues()
                            .serializeNulls()
                            .create();
                    try (BufferedWriter bw = Files.newBufferedWriter(p)) {
                        gson.toJson(RESULTS, bw);
                    }
                    System.out.println("[lqns-cmp] Wrote " + RESULTS.size()
                            + " entries to " + p.toAbsolutePath());
                } catch (IOException ex) {
                    ex.printStackTrace();
                }
            }
        }, "LqnsComparisonCapture-flush"));
    }

    private static void capture(Supplier<LayeredNetwork> factory) {
        String name = null;
        for (StackTraceElement el : Thread.currentThread().getStackTrace()) {
            if (el.getClassName().endsWith(".LqnsComparisonCapture")
                    && !el.getMethodName().equals("capture")) {
                name = el.getMethodName();
                break;
            }
        }
        if (name == null) return;

        // Run SolverLNSimple
        LayeredNetwork simpleModel = factory.get();
        LayeredNetworkAvgTable simpleTable;
        try {
            simpleTable = util.runSolverLNSimple(simpleModel);
        } catch (Throwable t) {
            recordFailure(name, "LNSimple failed: " + t.getClass().getSimpleName()
                    + ": " + safeMsg(t));
            return;
        }

        // Run SolverLQNS (fresh model — runSolverLN/LQNS each take their own)
        LayeredNetwork lqnsModel = factory.get();
        LayeredNetworkAvgTable lqnsTable;
        long lqnsStart = System.currentTimeMillis();
        try {
            lqnsTable = util.runSolverLQNS(lqnsModel);
        } catch (Throwable t) {
            recordFailure(name, "LQNS failed: " + t.getClass().getSimpleName()
                    + ": " + safeMsg(t));
            return;
        }
        long lqnsElapsed = System.currentTimeMillis() - lqnsStart;
        Integer lqnsIters = readUtilStaticInt("lastLqnsIters");
        Long lqnsTimeMs = readUtilStaticLong("lastLqnsTimeMs");
        if (lqnsTimeMs == null) lqnsTimeMs = lqnsElapsed;

        // Compare with NaN-skip semantics.
        Map<String, Object> rec = compareNanSkip(simpleModel, simpleTable, lqnsTable);
        rec.put("lqns_iters", lqnsIters);
        rec.put("lqns_time_ms", lqnsTimeMs);
        RESULTS.put(name, rec);

        System.out.printf("[lqns-cmp] %-45s rel=%-12s slack=%-12s comparable=%d/%d%n",
                name, rec.get("sim_vs_lqns_max_rel_diff"),
                rec.get("sim_vs_lqns_max_slack_loose"),
                rec.get("comparable_pairs"), rec.get("total_pairs"));
    }

    private static void recordFailure(String name, String why) {
        Map<String, Object> rec = new LinkedHashMap<String, Object>();
        rec.put("lqns_iters", null);
        rec.put("lqns_time_ms", null);
        rec.put("sim_vs_lqns_max_rel_diff", null);
        rec.put("sim_vs_lqns_max_slack_loose", null);
        rec.put("comparable_pairs", 0);
        rec.put("total_pairs", 0);
        rec.put("error", why);
        RESULTS.put(name, rec);
        System.out.printf("[lqns-cmp] %-45s FAILED: %s%n", name, why);
    }

    private static String safeMsg(Throwable t) {
        String m = t.getMessage();
        if (m == null) return "(no message)";
        int nl = m.indexOf('\n');
        return nl < 0 ? m : m.substring(0, nl);
    }

    private static Map<String, Object> compareNanSkip(
            LayeredNetwork model,
            LayeredNetworkAvgTable simpleTable,
            LayeredNetworkAvgTable lqnsTable) {

        Map<String, Integer> serverCounts = buildNodeServerCountMap(model);
        Map<String, Double>  activityDemand = buildActivityHostDemandMap(model);

        List<String> names = simpleTable.getNodeNames();
        List<Double> sQ = simpleTable.getQLen();
        List<Double> sU = simpleTable.getUtil();
        List<Double> sR = simpleTable.getRespT();
        List<Double> sW = simpleTable.getResidT();
        List<Double> sT = simpleTable.getTput();

        List<Double> lQ = lqnsTable.getQLen();
        List<Double> lU = lqnsTable.getUtil();
        List<Double> lR = lqnsTable.getRespT();
        List<Double> lW = lqnsTable.getResidT();
        List<Double> lT = lqnsTable.getTput();

        String[] metrics = {"QLen", "Util", "RespT", "ResidT", "Tput"};

        double maxRel = 0.0;
        double maxSlack = 0.0;
        int comparable = 0;
        int total = 0;
        String firstSkipped = null;
        String worstRelDetail = null;
        boolean dumpAll = Boolean.parseBoolean(System.getProperty("lqns.cmp.dump", "false"));

        int n = names.size();
        for (int i = 0; i < n; i++) {
            String node = names.get(i);
            // LQNS does not natively populate ResidT (per-row residence time).
            // util.java synthesises Q/X here, but Q/X is the *cycle time* via
            // Little's Law — equal to LNSimple's ResidT only at the leaf where
            // service happens, and 0 elsewhere (LNSimple reports 0 for INF
            // tasks and Immediate activities since no service happens locally).
            // Treating Q/X as comparable to LNSimple's ResidT produces spurious
            // 100% relative differences on every intermediate row. We leave
            // LQNS.ResidT as NaN so the NaN-skip rule drops these pairs from
            // the comparison cleanly.
            double lqnsResid = (i < lW.size()) ? lW.get(i) : Double.NaN;
            double lqnsU = (i < lU.size()) ? lU.get(i) : Double.NaN;
            Integer c = serverCounts.get(node);
            if (c != null && c > 1 && Double.isFinite(lqnsU)) {
                lqnsU = lqnsU / c;
            } else if (Double.isNaN(lqnsU)) {
                Double hd = activityDemand.get(node);
                double x = i < lT.size() ? lT.get(i) : Double.NaN;
                if (hd != null && Double.isFinite(hd) && hd > 0
                        && Double.isFinite(x) && x > 0 && c != null && c > 0) {
                    lqnsU = x * hd / c;
                }
            }

            double[] s = { sQ.get(i), sU.get(i), sR.get(i), sW.get(i), sT.get(i) };
            double[] l = { lQ.get(i), lqnsU, lR.get(i), lqnsResid, lT.get(i) };

            for (int m = 0; m < metrics.length; m++) {
                double actual = s[m];
                double expected = l[m];
                total++;
                boolean sNaN = Double.isNaN(actual);
                boolean lNaN = Double.isNaN(expected);
                if (sNaN && lNaN) {
                    comparable++;
                    continue;
                }
                if (sNaN ^ lNaN) {
                    if (firstSkipped == null) {
                        firstSkipped = String.format(
                                "%s.%s (LNSimple=%s, LQNS=%s)",
                                node, metrics[m],
                                sNaN ? "NaN" : Double.toString(actual),
                                lNaN ? "NaN" : Double.toString(expected));
                    }
                    continue;
                }
                comparable++;
                double diff = Math.abs(actual - expected);
                double denom = Math.max(Math.abs(expected), 1e-6);
                double rel = diff / denom;
                if (rel > maxRel) {
                    maxRel = rel;
                    worstRelDetail = String.format("%s.%s (LNSimple=%.6g, LQNS=%.6g, rel=%.4g)",
                            node, metrics[m], actual, expected, rel);
                }
                double allowed = LOOSE_ATOL + LOOSE_RTOL * Math.abs(expected);
                double slack = diff / allowed;
                if (slack > maxSlack) maxSlack = slack;
                if (dumpAll) {
                    System.out.printf("[cmp] %-12s %-8s LNSimple=%-12.6g LQNS=%-12.6g rel=%-10.4g slack=%-10.4g%n",
                            node, metrics[m], actual, expected, rel, slack);
                }
            }
        }

        Map<String, Object> rec = new LinkedHashMap<String, Object>();
        rec.put("sim_vs_lqns_max_rel_diff", comparable > 0 ? maxRel : null);
        rec.put("sim_vs_lqns_max_slack_loose", comparable > 0 ? maxSlack : null);
        rec.put("comparable_pairs", comparable);
        rec.put("total_pairs", total);
        rec.put("first_skipped", firstSkipped);
        rec.put("worst_rel_detail", worstRelDetail);
        return rec;
    }

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

    private static int serverCountOf(Host h) {
        if (h.getScheduling() == SchedStrategy.INF) return 1;
        int m = h.getMultiplicity();
        if (m == Integer.MAX_VALUE || m <= 0) return 1;
        return m;
    }

    private static Integer readUtilStaticInt(String fieldName) {
        try {
            java.lang.reflect.Field f = util.class.getDeclaredField(fieldName);
            f.setAccessible(true);
            Object v = f.get(null);
            return (v instanceof Integer) ? (Integer) v : null;
        } catch (ReflectiveOperationException e) {
            return null;
        }
    }

    private static Long readUtilStaticLong(String fieldName) {
        try {
            java.lang.reflect.Field f = util.class.getDeclaredField(fieldName);
            f.setAccessible(true);
            Object v = f.get(null);
            return (v instanceof Long) ? (Long) v : null;
        } catch (ReflectiveOperationException e) {
            return null;
        }
    }

    // ===== A1 =====
    @Test @Timeout(120) public void A1_chain_d3_N20()  { capture(EvaluationSuite::A1_chain_d3_N20);  }
    @Test @Timeout(120) public void A1_chain_d3_N50()  { capture(EvaluationSuite::A1_chain_d3_N50);  }
    @Test @Timeout(120) public void A1_chain_d3_N100() { capture(EvaluationSuite::A1_chain_d3_N100); }
    @Test @Timeout(120) public void A1_chain_d5_N20()  { capture(EvaluationSuite::A1_chain_d5_N20);  }
    @Test @Timeout(120) public void A1_chain_d5_N50()  { capture(EvaluationSuite::A1_chain_d5_N50);  }
    @Test @Timeout(120) public void A1_chain_d5_N100() { capture(EvaluationSuite::A1_chain_d5_N100); }
    @Test @Timeout(120) public void A1_chain_d8_N20()  { capture(EvaluationSuite::A1_chain_d8_N20);  }
    @Test @Timeout(120) public void A1_chain_d8_N50()  { capture(EvaluationSuite::A1_chain_d8_N50);  }
    @Test @Timeout(120) public void A1_chain_d8_N100() { capture(EvaluationSuite::A1_chain_d8_N100); }

    // ===== A2 =====
    @Test @Timeout(120) public void A2_multiclassA_2refs_loadLow_N20()      { capture(EvaluationSuite::A2_multiclassA_2refs_loadLow_N20);  }
    @Test @Timeout(120) public void A2_multiclassA_2refs_loadHigh_N20()     { capture(EvaluationSuite::A2_multiclassA_2refs_loadHigh_N20); }
    @Test @Timeout(120) public void A2_multiclassA_2refs_loadLow_N50()      { capture(EvaluationSuite::A2_multiclassA_2refs_loadLow_N50);  }
    @Test @Timeout(120) public void A2_multiclassA_2refs_loadHigh_N50()     { capture(EvaluationSuite::A2_multiclassA_2refs_loadHigh_N50); }
    @Test @Timeout(120) public void A2_multiclassB_2refs_demands_3to1_N20() { capture(EvaluationSuite::A2_multiclassB_2refs_demands_3to1_N20); }
    @Test @Timeout(120) public void A2_multiclassB_2refs_demands_5to1_N20() { capture(EvaluationSuite::A2_multiclassB_2refs_demands_5to1_N20); }
    @Test @Timeout(120) public void A2_multiclassB_2refs_demands_3to1_N50() { capture(EvaluationSuite::A2_multiclassB_2refs_demands_3to1_N50); }
    @Test @Timeout(120) public void A2_multiclassB_2refs_demands_5to1_N50() { capture(EvaluationSuite::A2_multiclassB_2refs_demands_5to1_N50); }
    @Test @Timeout(120) public void A2_multiclassC_2refs_think_2to1_N20()   { capture(EvaluationSuite::A2_multiclassC_2refs_think_2to1_N20); }
    @Test @Timeout(120) public void A2_multiclassC_2refs_think_4to1_N20()   { capture(EvaluationSuite::A2_multiclassC_2refs_think_4to1_N20); }
    @Test @Timeout(120) public void A2_multiclassC_2refs_think_2to1_N50()   { capture(EvaluationSuite::A2_multiclassC_2refs_think_2to1_N50); }
    @Test @Timeout(120) public void A2_multiclassC_2refs_think_4to1_N50()   { capture(EvaluationSuite::A2_multiclassC_2refs_think_4to1_N50); }

    // ===== A3 =====
    @Test @Timeout(120) public void A3_multiserver_c2_loadLow()    { capture(EvaluationSuite::A3_multiserver_c2_loadLow);    }
    @Test @Timeout(120) public void A3_multiserver_c2_loadMedium() { capture(EvaluationSuite::A3_multiserver_c2_loadMedium); }
    @Test @Timeout(120) public void A3_multiserver_c2_loadHigh()   { capture(EvaluationSuite::A3_multiserver_c2_loadHigh);   }
    @Test @Timeout(120) public void A3_multiserver_c4_loadLow()    { capture(EvaluationSuite::A3_multiserver_c4_loadLow);    }
    @Test @Timeout(120) public void A3_multiserver_c4_loadMedium() { capture(EvaluationSuite::A3_multiserver_c4_loadMedium); }
    @Test @Timeout(120) public void A3_multiserver_c4_loadHigh()   { capture(EvaluationSuite::A3_multiserver_c4_loadHigh);   }
    @Test @Timeout(120) public void A3_multiserver_c8_loadLow()    { capture(EvaluationSuite::A3_multiserver_c8_loadLow);    }
    @Test @Timeout(120) public void A3_multiserver_c8_loadMedium() { capture(EvaluationSuite::A3_multiserver_c8_loadMedium); }
    @Test @Timeout(120) public void A3_multiserver_c8_loadHigh()   { capture(EvaluationSuite::A3_multiserver_c8_loadHigh);   }

    // ===== A4 =====
    @Test @Timeout(120) public void A4_threeTier_multiclass()    { capture(EvaluationSuite::A4_threeTier_multiclass);    }
    @Test @Timeout(120) public void A4_multiclass_multiserver()  { capture(EvaluationSuite::A4_multiclass_multiserver);  }
    @Test @Timeout(120) public void A4_multiclass_multientry()   { capture(EvaluationSuite::A4_multiclass_multientry);   }
    @Test @Timeout(120) public void A4_chainPlusFanout()         { capture(EvaluationSuite::A4_chainPlusFanout);         }
    @Test @Timeout(120) public void A4_webApplication()          { capture(EvaluationSuite::A4_webApplication);          }
    @Test @Timeout(120) public void A4_databaseBackend()         { capture(EvaluationSuite::A4_databaseBackend);         }

    // ===== B =====
    @Test @Timeout(120) public void B_scaleAxis_tasks5_chain()   { capture(EvaluationSuite::B_scaleAxis_tasks5_chain);  }
    @Test @Timeout(120) public void B_scaleAxis_tasks10_chain()  { capture(EvaluationSuite::B_scaleAxis_tasks10_chain); }
    @Test @Timeout(180) public void B_scaleAxis_tasks20_chain()  { capture(EvaluationSuite::B_scaleAxis_tasks20_chain); }
    @Test @Timeout(240) public void B_scaleAxis_tasks40_chain()  { capture(EvaluationSuite::B_scaleAxis_tasks40_chain); }
    @Test @Timeout(300) public void B_scaleAxis_tasks60_chain()  { capture(EvaluationSuite::B_scaleAxis_tasks60_chain); }
    @Test @Timeout(120) public void B_scaleAxis_tasks5_fanout()  { capture(EvaluationSuite::B_scaleAxis_tasks5_fanout);  }
    @Test @Timeout(120) public void B_scaleAxis_tasks10_fanout() { capture(EvaluationSuite::B_scaleAxis_tasks10_fanout); }
    @Test @Timeout(180) public void B_scaleAxis_tasks20_fanout() { capture(EvaluationSuite::B_scaleAxis_tasks20_fanout); }
    @Test @Timeout(360) public void B_scaleAxis_tasks40_fanout() { capture(EvaluationSuite::B_scaleAxis_tasks40_fanout); }
    @Test @Timeout(1200) public void B_scaleAxis_tasks60_fanout() { capture(EvaluationSuite::B_scaleAxis_tasks60_fanout); }

    // ===== C1 =====
    @Test @Timeout(120) public void C1_fanOut2_caller_c1() { capture(EvaluationSuite::C1_fanOut2_caller_c1); }
    @Test @Timeout(120) public void C1_fanOut2_caller_c2() { capture(EvaluationSuite::C1_fanOut2_caller_c2); }
    @Test @Timeout(120) public void C1_fanOut3_caller_c1() { capture(EvaluationSuite::C1_fanOut3_caller_c1); }
    @Test @Timeout(120) public void C1_fanOut3_caller_c2() { capture(EvaluationSuite::C1_fanOut3_caller_c2); }
    @Test @Timeout(120) public void C1_fanOut4_caller_c1() { capture(EvaluationSuite::C1_fanOut4_caller_c1); }
    @Test @Timeout(120) public void C1_fanOut4_caller_c2() { capture(EvaluationSuite::C1_fanOut4_caller_c2); }

    // ===== C2 =====
    @Test @Timeout(120) public void C2_infTaskSimple() { capture(EvaluationSuite::C2_infTaskSimple); }
    @Test @Timeout(120) public void C2_infTaskShared() { capture(EvaluationSuite::C2_infTaskShared); }
    @Test @Timeout(120) public void C2_infTaskDeep()   { capture(EvaluationSuite::C2_infTaskDeep);   }

    // ===== C3 =====
    @Test @Timeout(120) public void C3_multiclassMultiEntry_2refs()      { capture(EvaluationSuite::C3_multiclassMultiEntry_2refs);      }
    @Test @Timeout(120) public void C3_multiclassMultiEntry_3refs()      { capture(EvaluationSuite::C3_multiclassMultiEntry_3refs);      }
    @Test @Timeout(120) public void C3_multiclassMultiEntry_asymmetric() { capture(EvaluationSuite::C3_multiclassMultiEntry_asymmetric); }

    // ===== C4 =====
    @Test @Timeout(120) public void C4_nearSaturation()     { capture(EvaluationSuite::C4_nearSaturation);     }
    @Test @Timeout(120) public void C4_predicateThreshold() { capture(EvaluationSuite::C4_predicateThreshold); }
    @Test @Timeout(120) public void C4_lowDemand()          { capture(EvaluationSuite::C4_lowDemand);          }
}
