package jline.solvers.ln_simple;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.junit.jupiter.api.Assertions.fail;

import java.io.ByteArrayOutputStream;
import java.io.PrintStream;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

import jline.lang.constant.SolverType;
import jline.lang.layered.LayeredNetwork;
import jline.solvers.LayeredNetworkAvgTable;
import jline.solvers.ln.SolverLN;
import jline.solvers.lqns.SolverLQNS;

public class util {

    private static final double TOLERANCE = 1e-3;

    static {
        Logger.getLogger("").setLevel(Level.OFF);
        Logger.getLogger("jline").setLevel(Level.OFF);
        Logger.getLogger("org.apache.commons.io.FileUtils").setLevel(Level.OFF);
    }

    public static void assertResultsMatchSolverLN(LayeredNetwork model) {
        assertNotNull(model);

        LayeredNetworkAvgTable simpleTable = runSolverLNSimple(model);
        LayeredNetworkAvgTable lnTable = runSolverLN(model);

        assertNotNull(simpleTable, "SolverLNSimple returned null avg table");
        assertNotNull(lnTable, "SolverLN returned null avg table");

        List<String> simpleNames = simpleTable.getNodeNames();
        List<String> lnNames = lnTable.getNodeNames();
        assertNotNull(simpleNames, "SolverLNSimple node names are null");
        assertNotNull(lnNames, "SolverLN node names are null");
        assertEquals(lnNames.size(), simpleNames.size(),
                "Row count mismatch: SolverLNSimple=" + simpleNames.size() + " SolverLN=" + lnNames.size());

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

        String context = "\n\n=== SolverLN ===\n" + formatTable(lnTable)
                       + "\n=== SolverLNSimple ===\n" + formatTable(simpleTable);

        for (int i = 0; i < lnNames.size(); i++) {
            String name = lnNames.get(i);
            assertClose(name, "QLen",  lnQLen.get(i),  simpleQLen.get(i),  context);
            assertClose(name, "Util",  lnUtil.get(i),  simpleUtil.get(i),  context);
            assertClose(name, "RespT", lnRespT.get(i), simpleRespT.get(i), context);
            assertClose(name, "ResidT",lnResidT.get(i),simpleResidT.get(i),context);
            assertClose(name, "Tput",  lnTput.get(i),  simpleTput.get(i),  context);
        }
    }

    public static LayeredNetworkAvgTable runSolverLNSimple(LayeredNetwork model) {
        SolverLNSimple solver = new SolverLNSimple(model);
        suppressOutput(() -> solver.iterateCoupledMva(100, 1e-4));
        return solver.getAvgTable();
    }

    public static LayeredNetworkAvgTable runSolverLN(LayeredNetwork model) {
        final LayeredNetworkAvgTable[] holder = new LayeredNetworkAvgTable[1];
        suppressOutput(() -> {
            SolverLN solver = new SolverLN(model);
            holder[0] = (LayeredNetworkAvgTable) solver.getAvgTable();
        });
        return holder[0];
    }

    public static LayeredNetworkAvgTable runSolverLQNS(LayeredNetwork model) {
        final LayeredNetworkAvgTable[] holder = new LayeredNetworkAvgTable[1];
        suppressOutput(() -> {
            SolverLQNS solver = new SolverLQNS(model);
            holder[0] = (LayeredNetworkAvgTable) solver.getAvgTable();
        });
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
        double scale = Math.max(1.0, Math.abs(expected));
        assertTrue(diff / scale <= TOLERANCE,
                String.format("%s.%s: SolverLN=%.6f SolverLNSimple=%.6f (relative diff=%.2e > %.2e)%s",
                        node, metric, expected, actual, diff / scale, TOLERANCE, context));
    }
    
}
