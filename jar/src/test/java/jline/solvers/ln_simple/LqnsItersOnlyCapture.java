package jline.solvers.ln_simple;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import jline.GlobalConstants;
import jline.VerboseLevel;
import jline.lang.layered.LayeredNetwork;
import jline.solvers.ln_simple.fixtures.EvaluationSuite;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.Timeout;

import java.io.BufferedWriter;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.function.Supplier;

/**
 * Patch helper: re-runs only the {@link jline.solvers.lqns.SolverLQNS} step
 * for each of the 61 evaluation-suite fixtures and writes
 * {@code <fixture> -> lqns_iters} to a JSON file. Used to back-fill the
 * {@code lqns_iters} column in {@code baseline.csv} / {@code baseline.json}
 * after the {@code SolverLQNS.result.iter} wiring was added.
 *
 * <p>Output path: {@code -Dlqns.patch.out=<path>}, default
 * {@code ../report/data/lqns-iters-patch.json}.
 */
public class LqnsItersOnlyCapture {

    private static final Map<String, Integer> RESULTS = new LinkedHashMap<String, Integer>();
    private static final Map<String, Long>    TIMINGS = new LinkedHashMap<String, Long>();

    @BeforeAll
    public static void setUp() {
        GlobalConstants.setVerbose(VerboseLevel.SILENT);
        Runtime.getRuntime().addShutdownHook(new Thread(new Runnable() {
            @Override
            public void run() {
                String outPath = System.getProperty("lqns.patch.out",
                        "../report/data/lqns-iters-patch.json");
                try {
                    Path p = Paths.get(outPath);
                    if (p.getParent() != null) Files.createDirectories(p.getParent());
                    Gson gson = new GsonBuilder().setPrettyPrinting().create();
                    Map<String, Map<String, Object>> wrapped = new LinkedHashMap<String, Map<String, Object>>();
                    for (Map.Entry<String, Integer> e : RESULTS.entrySet()) {
                        Map<String, Object> rec = new LinkedHashMap<String, Object>();
                        rec.put("lqns_iters", e.getValue());
                        rec.put("lqns_time_ms", TIMINGS.get(e.getKey()));
                        wrapped.put(e.getKey(), rec);
                    }
                    try (BufferedWriter bw = Files.newBufferedWriter(p)) {
                        gson.toJson(wrapped, bw);
                    }
                    System.out.println("[LqnsItersPatch] Wrote " + RESULTS.size()
                            + " entries to " + p.toAbsolutePath());
                } catch (IOException ex) {
                    ex.printStackTrace();
                }
            }
        }, "LqnsItersPatch-flush"));
    }

    private static void capture(Supplier<LayeredNetwork> factory) {
        // Find caller test name to use as the fixture key.
        String name = null;
        for (StackTraceElement el : Thread.currentThread().getStackTrace()) {
            if (el.getClassName().endsWith(".LqnsItersOnlyCapture")) {
                if (!el.getMethodName().equals("capture")) {
                    name = el.getMethodName();
                    break;
                }
            }
        }
        if (name == null) return;
        long start = System.currentTimeMillis();
        util.runSolverLQNS(factory.get());  // util writes lastLqnsIters as a side effect
        long elapsed = System.currentTimeMillis() - start;
        Integer iters = lastIters();
        RESULTS.put(name, iters);
        TIMINGS.put(name, elapsed);
        System.out.printf("[lqns-patch] %-40s iters=%s time=%dms%n", name,
                iters == null ? "?" : iters.toString(), elapsed);
    }

    /** Helper that exposes util's private telemetry via a small reflection
     *  hop. Simpler than threading state through the public API. */
    private static Integer lastIters() {
        try {
            java.lang.reflect.Field f = util.class.getDeclaredField("lastLqnsIters");
            f.setAccessible(true);
            return (Integer) f.get(null);
        } catch (ReflectiveOperationException e) {
            return null;
        }
    }

    // ===== A1 =====
    @Test @Timeout(60) public void A1_chain_d3_N50()  { capture(EvaluationSuite::A1_chain_d3_N50);  }
    @Test @Timeout(60) public void A1_chain_d5_N20()  { capture(EvaluationSuite::A1_chain_d5_N20);  }
    @Test @Timeout(60) public void A1_chain_d5_N50()  { capture(EvaluationSuite::A1_chain_d5_N50);  }
    @Test @Timeout(60) public void A1_chain_d5_N100() { capture(EvaluationSuite::A1_chain_d5_N100); }
    @Test @Timeout(60) public void A1_chain_d8_N50()  { capture(EvaluationSuite::A1_chain_d8_N50);  }

    // ===== A2 =====
    @Test @Timeout(60) public void A2_multiclassA_2refs_loadLow_N20()      { capture(EvaluationSuite::A2_multiclassA_2refs_loadLow_N20);  }
    @Test @Timeout(60) public void A2_multiclassA_2refs_loadHigh_N20()     { capture(EvaluationSuite::A2_multiclassA_2refs_loadHigh_N20); }
    @Test @Timeout(60) public void A2_multiclassA_2refs_loadLow_N50()      { capture(EvaluationSuite::A2_multiclassA_2refs_loadLow_N50);  }
    @Test @Timeout(60) public void A2_multiclassA_2refs_loadHigh_N50()     { capture(EvaluationSuite::A2_multiclassA_2refs_loadHigh_N50); }
    @Test @Timeout(60) public void A2_multiclassB_2refs_demands_3to1_N20() { capture(EvaluationSuite::A2_multiclassB_2refs_demands_3to1_N20); }
    @Test @Timeout(60) public void A2_multiclassB_2refs_demands_5to1_N20() { capture(EvaluationSuite::A2_multiclassB_2refs_demands_5to1_N20); }
    @Test @Timeout(60) public void A2_multiclassB_2refs_demands_3to1_N50() { capture(EvaluationSuite::A2_multiclassB_2refs_demands_3to1_N50); }
    @Test @Timeout(60) public void A2_multiclassB_2refs_demands_5to1_N50() { capture(EvaluationSuite::A2_multiclassB_2refs_demands_5to1_N50); }
    @Test @Timeout(60) public void A2_multiclassC_2refs_think_2to1_N20()   { capture(EvaluationSuite::A2_multiclassC_2refs_think_2to1_N20); }
    @Test @Timeout(60) public void A2_multiclassC_2refs_think_4to1_N20()   { capture(EvaluationSuite::A2_multiclassC_2refs_think_4to1_N20); }
    @Test @Timeout(60) public void A2_multiclassC_2refs_think_2to1_N50()   { capture(EvaluationSuite::A2_multiclassC_2refs_think_2to1_N50); }
    @Test @Timeout(60) public void A2_multiclassC_2refs_think_4to1_N50()   { capture(EvaluationSuite::A2_multiclassC_2refs_think_4to1_N50); }

    // ===== A3 =====
    @Test @Timeout(60) public void A3_multiserver_c2_loadLow()    { capture(EvaluationSuite::A3_multiserver_c2_loadLow);    }
    @Test @Timeout(60) public void A3_multiserver_c2_loadMedium() { capture(EvaluationSuite::A3_multiserver_c2_loadMedium); }
    @Test @Timeout(60) public void A3_multiserver_c2_loadHigh()   { capture(EvaluationSuite::A3_multiserver_c2_loadHigh);   }
    @Test @Timeout(60) public void A3_multiserver_c4_loadLow()    { capture(EvaluationSuite::A3_multiserver_c4_loadLow);    }
    @Test @Timeout(60) public void A3_multiserver_c4_loadMedium() { capture(EvaluationSuite::A3_multiserver_c4_loadMedium); }
    @Test @Timeout(60) public void A3_multiserver_c4_loadHigh()   { capture(EvaluationSuite::A3_multiserver_c4_loadHigh);   }
    @Test @Timeout(60) public void A3_multiserver_c8_loadLow()    { capture(EvaluationSuite::A3_multiserver_c8_loadLow);    }
    @Test @Timeout(60) public void A3_multiserver_c8_loadMedium() { capture(EvaluationSuite::A3_multiserver_c8_loadMedium); }
    @Test @Timeout(60) public void A3_multiserver_c8_loadHigh()   { capture(EvaluationSuite::A3_multiserver_c8_loadHigh);   }

    // ===== A4 =====
    @Test @Timeout(60) public void A4_threeTier_multiclass()    { capture(EvaluationSuite::A4_threeTier_multiclass);    }
    @Test @Timeout(60) public void A4_multiclass_multiserver()  { capture(EvaluationSuite::A4_multiclass_multiserver);  }
    @Test @Timeout(60) public void A4_multiclass_multientry()   { capture(EvaluationSuite::A4_multiclass_multientry);   }
    @Test @Timeout(60) public void A4_chainPlusFanout()         { capture(EvaluationSuite::A4_chainPlusFanout);         }
    @Test @Timeout(60) public void A4_webApplication()          { capture(EvaluationSuite::A4_webApplication);          }
    @Test @Timeout(60) public void A4_databaseBackend()         { capture(EvaluationSuite::A4_databaseBackend);         }

    // ===== B =====
    @Test @Timeout(120) public void B_scaleAxis_tasks5_chain()   { capture(EvaluationSuite::B_scaleAxis_tasks5_chain);  }
    @Test @Timeout(120) public void B_scaleAxis_tasks10_chain()  { capture(EvaluationSuite::B_scaleAxis_tasks10_chain); }
    @Test @Timeout(120) public void B_scaleAxis_tasks20_chain()  { capture(EvaluationSuite::B_scaleAxis_tasks20_chain); }
    @Test @Timeout(120) public void B_scaleAxis_tasks40_chain()  { capture(EvaluationSuite::B_scaleAxis_tasks40_chain); }
    @Test @Timeout(120) public void B_scaleAxis_tasks60_chain()  { capture(EvaluationSuite::B_scaleAxis_tasks60_chain); }
    @Test @Timeout(120) public void B_scaleAxis_tasks5_fanout()  { capture(EvaluationSuite::B_scaleAxis_tasks5_fanout);  }
    @Test @Timeout(120) public void B_scaleAxis_tasks10_fanout() { capture(EvaluationSuite::B_scaleAxis_tasks10_fanout); }
    @Test @Timeout(120) public void B_scaleAxis_tasks20_fanout() { capture(EvaluationSuite::B_scaleAxis_tasks20_fanout); }
    @Test @Timeout(180) public void B_scaleAxis_tasks40_fanout() { capture(EvaluationSuite::B_scaleAxis_tasks40_fanout); }
    @Test @Timeout(300) public void B_scaleAxis_tasks60_fanout() { capture(EvaluationSuite::B_scaleAxis_tasks60_fanout); }

    // ===== C1 =====
    @Test @Timeout(60) public void C1_fanOut2_caller_c1() { capture(EvaluationSuite::C1_fanOut2_caller_c1); }
    @Test @Timeout(60) public void C1_fanOut2_caller_c2() { capture(EvaluationSuite::C1_fanOut2_caller_c2); }
    @Test @Timeout(60) public void C1_fanOut3_caller_c1() { capture(EvaluationSuite::C1_fanOut3_caller_c1); }
    @Test @Timeout(60) public void C1_fanOut3_caller_c2() { capture(EvaluationSuite::C1_fanOut3_caller_c2); }
    @Test @Timeout(60) public void C1_fanOut4_caller_c1() { capture(EvaluationSuite::C1_fanOut4_caller_c1); }
    @Test @Timeout(60) public void C1_fanOut4_caller_c2() { capture(EvaluationSuite::C1_fanOut4_caller_c2); }

    // ===== C2 =====
    @Test @Timeout(60) public void C2_infTaskSimple() { capture(EvaluationSuite::C2_infTaskSimple); }
    @Test @Timeout(60) public void C2_infTaskShared() { capture(EvaluationSuite::C2_infTaskShared); }
    @Test @Timeout(60) public void C2_infTaskDeep()   { capture(EvaluationSuite::C2_infTaskDeep);   }

    // ===== C3 =====
    @Test @Timeout(60) public void C3_multiclassMultiEntry_2refs()      { capture(EvaluationSuite::C3_multiclassMultiEntry_2refs);      }
    @Test @Timeout(60) public void C3_multiclassMultiEntry_3refs()      { capture(EvaluationSuite::C3_multiclassMultiEntry_3refs);      }
    @Test @Timeout(60) public void C3_multiclassMultiEntry_asymmetric() { capture(EvaluationSuite::C3_multiclassMultiEntry_asymmetric); }

    // ===== C4 =====
    @Test @Timeout(60) public void C4_nearSaturation()     { capture(EvaluationSuite::C4_nearSaturation);     }
    @Test @Timeout(60) public void C4_predicateThreshold() { capture(EvaluationSuite::C4_predicateThreshold); }
    @Test @Timeout(60) public void C4_lowDemand()          { capture(EvaluationSuite::C4_lowDemand);          }
}
