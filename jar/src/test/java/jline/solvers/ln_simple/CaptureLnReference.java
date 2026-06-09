package jline.solvers.ln_simple;

import jline.GlobalConstants;
import jline.VerboseLevel;
import jline.lang.layered.LayeredNetwork;
import jline.solvers.LayeredNetworkAvgTable;
import jline.lang.constant.SolverType;
import jline.solvers.ln.SolverLN;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.PrintStream;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.function.Supplier;

/**
 * One-shot LN reference capture: runs SolverLN on each of the 59 fast
 * fixtures from {@link FastEval#FIXTURES} and writes a tables CSV that
 * {@link FastEval#loadLnReference(String)} can load as the cached
 * reference. Lets the evaluation suite start from a reference that
 * matches the current HEAD's SolverLN (the shipped one used to derive
 * the project's baseline tables CSV had drifted enough on B fan-outs
 * and C1 fan-outs that 9 fixtures failed against it).
 *
 * <pre>
 *   -Dcaptureln.out=jar/overnight-exploration/data/ln-reference.tables.csv
 * </pre>
 */
public final class CaptureLnReference {

    public static void main(String[] args) throws Exception {
        String outPath = System.getProperty("captureln.out",
                "jar/overnight-exploration/data/ln-reference.tables.csv");

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
            String partition = partitionOf(name);
            LayeredNetwork model = e.getValue().get();

            LayeredNetworkAvgTable table;
            long t0 = System.currentTimeMillis();
            System.setOut(sink); System.setErr(sink);
            try {
                SolverLN solver = new SolverLN(model, SolverType.MVA);
                table = (LayeredNetworkAvgTable) solver.getAvgTable();
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
                bw.write(String.format("%s,%s,baseline,LN,%s,%s,1,%s,%s,%s,%s,%s%n",
                        name, partition, nodeName, nodeType,
                        fmt(q.get(i)), fmt(u.get(i)), fmt(rt.get(i)),
                        fmt(rd.get(i)), fmt(tp.get(i))));
            }
            bw.flush();
        }
        bw.close();
        long elapsed = System.currentTimeMillis() - globalStart;
        origOut.printf("%nWrote %s%nTotal LN capture time: %.1f s for %d fixtures%n",
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
}
