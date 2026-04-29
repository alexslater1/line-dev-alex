package jline.solvers.ln_simple;

import jline.GlobalConstants;
import jline.VerboseLevel;
import jline.lang.layered.LayeredNetwork;
import jline.solvers.ln_simple.utils.LayeredNetworkTestExamples;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Disabled;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.Timeout;

import java.io.ByteArrayOutputStream;
import java.io.PrintStream;
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

public class SolverLNSimpleTest {

    private static final String TABLE_SEPARATOR = "--------------------------------------------------------------------------------------------";
    private static final String TABLE_HEADER = String.format(
            "%-10s\t%-10s\t%-10s\t%-10s\t%-10s\t%-10s\t%-10s\t%-10s",
            "Node", "NodeType", "QLen", "Util", "RespT", "ResidT", "ArvR", "Tput");

    static {
        Logger.getLogger("").setLevel(Level.OFF);
        Logger.getLogger("jline").setLevel(Level.OFF);
        Logger.getLogger("org.apache.commons.io.FileUtils").setLevel(Level.OFF);
    }

    @BeforeAll
    public static void setUp() {
        GlobalConstants.setVerbose(VerboseLevel.SILENT);
    }

    @Test
    @Timeout(120)
    public void testTwoLayerSingleProcessorNetwork() {
        assertFinalOutputMatches(
                LayeredNetworkTestExamples.twoLayerSingleProcessorNetwork(),
                rows(
                        row("P1", "Processor", Double.NaN, 0.0, Double.NaN, Double.NaN, Double.NaN, Double.NaN),
                        row("P2", "Processor", Double.NaN, 0.31034, Double.NaN, Double.NaN, Double.NaN, Double.NaN),
                        row("T1", "RefTask", 0.31034, 0.0, Double.NaN, 0.0, Double.NaN, 0.34483),
                        row("T2", "Task", 0.31034, 0.31034, Double.NaN, 0.90000, Double.NaN, 0.34483),
                        row("E1", "Entry", 0.31034, Double.NaN, 0.90000, Double.NaN, Double.NaN, 0.34483),
                        row("E2", "Entry", 0.31034, Double.NaN, 0.90000, Double.NaN, Double.NaN, 0.34483),
                        row("AS1", "Activity", 0.31034, 0.0, 0.90000, 0.0, Double.NaN, 0.34483),
                        row("AS2", "Activity", 0.31034, 0.31034, 0.90000, 0.90000, Double.NaN, 0.34483)
                ));
    }

    @Test
    @Timeout(120)
    public void testThreeLayerSingleProcessorNetwork() {
        assertFinalOutputMatches(
                LayeredNetworkTestExamples.threeLayerSingleProcessorNetwork(),
                rows(
                        row("P1", "Processor", Double.NaN, 0.0, Double.NaN, Double.NaN, Double.NaN, Double.NaN),
                        row("P2", "Processor", Double.NaN, 0.0, Double.NaN, Double.NaN, Double.NaN, Double.NaN),
                        row("P3", "Processor", Double.NaN, 0.31034, Double.NaN, Double.NaN, Double.NaN, Double.NaN),
                        row("T1", "RefTask", 0.31034, 0.0, Double.NaN, 0.0, Double.NaN, 0.34483),
                        row("T2", "Task", 0.31034, 0.0, Double.NaN, 0.0, Double.NaN, 0.34483),
                        row("T3", "Task", 0.31034, 0.31034, Double.NaN, 0.90000, Double.NaN, 0.34483),
                        row("E1", "Entry", 0.31034, Double.NaN, 0.90000, Double.NaN, Double.NaN, 0.34483),
                        row("E2", "Entry", 0.31034, Double.NaN, 0.90000, Double.NaN, Double.NaN, 0.34483),
                        row("E3", "Entry", 0.31034, Double.NaN, 0.90000, Double.NaN, Double.NaN, 0.34483),
                        row("AS1", "Activity", 0.31034, 0.0, 0.90000, 0.0, Double.NaN, 0.34483),
                        row("AS2", "Activity", 0.31034, 0.0, 0.90000, 0.0, Double.NaN, 0.34483),
                        row("AS3", "Activity", 0.31034, 0.31034, 0.90000, 0.90000, Double.NaN, 0.34483)
                ));
    }

    @Test
    @Timeout(120)
    public void testTwoLayerMultiProcessorNetwork() {
        assertFinalOutputMatches(
                LayeredNetworkTestExamples.twoLayerMultiProcessorNetwork(),
                rows(
                        row("P1", "Processor", Double.NaN, 0.0, Double.NaN, Double.NaN, Double.NaN, Double.NaN),
                        row("P2", "Processor", Double.NaN, 0.45866, Double.NaN, Double.NaN, Double.NaN, Double.NaN),
                        row("T1", "RefTask", 0.96150, 0.0, Double.NaN, 0.0, Double.NaN, 1.01925),
                        row("T2", "Task", 0.96150, 0.45866, Double.NaN, 0.94334, Double.NaN, 1.01925),
                        row("E1", "Entry", 0.96150, Double.NaN, 0.94334, Double.NaN, Double.NaN, 1.01925),
                        row("E2", "Entry", 0.96150, Double.NaN, 0.94334, Double.NaN, Double.NaN, 1.01925),
                        row("AS1", "Activity", 0.96150, 0.0, 0.94334, 0.0, Double.NaN, 1.01925),
                        row("AS2", "Activity", 0.96150, 0.45866, 0.94334, 0.94334, Double.NaN, 1.01925)
                ));
    }

    @Test
    @Timeout(120)
    public void testThreeLayerMultiProcessorNetwork() {
        assertFinalOutputMatches(
                LayeredNetworkTestExamples.threeLayerMultiProcessorNetwork(),
                rows(
                        row("P1", "Processor", Double.NaN, 0.0, Double.NaN, Double.NaN, Double.NaN, Double.NaN),
                        row("P2", "Processor", Double.NaN, 0.0, Double.NaN, Double.NaN, Double.NaN, Double.NaN),
                        row("P3", "Processor", Double.NaN, 0.45866, Double.NaN, Double.NaN, Double.NaN, Double.NaN),
                        row("T1", "RefTask", 0.96150, 0.0, Double.NaN, 0.0, Double.NaN, 1.01925),
                        row("T2", "Task", 0.96150, 0.0, Double.NaN, 0.0, Double.NaN, 1.01925),
                        row("T3", "Task", 0.96150, 0.45866, Double.NaN, 0.94334, Double.NaN, 1.01925),
                        row("E1", "Entry", 0.96150, Double.NaN, 0.94334, Double.NaN, Double.NaN, 1.01925),
                        row("E2", "Entry", 0.96150, Double.NaN, 0.94334, Double.NaN, Double.NaN, 1.01925),
                        row("E3", "Entry", 0.96150, Double.NaN, 0.94334, Double.NaN, Double.NaN, 1.01925),
                        row("AS1", "Activity", 0.96150, 0.0, 0.94334, 0.0, Double.NaN, 1.01925),
                        row("AS2", "Activity", 0.96150, 0.0, 0.94334, 0.0, Double.NaN, 1.01925),
                        row("AS3", "Activity", 0.96150, 0.45866, 0.94334, 0.94334, Double.NaN, 1.01925)
                ));
    }

    @Test
    @Disabled("Multi class???")
    @Timeout(120)
    public void testLqnBasic() {
        LayeredNetwork model = LayeredNetworkTestExamples.lqnBasic();
        assertNotNull(model);

        assertFinalOutputMatches(
                model,
                rows(
                        row("P1", "Processor", Double.NaN, 0.99594, Double.NaN, Double.NaN, Double.NaN, Double.NaN),
                        row("P2", "Processor", Double.NaN, 0.04146, Double.NaN, Double.NaN, Double.NaN, Double.NaN),
                        row("T1", "RefTask", 23.43875, 0.66397, Double.NaN, 1.09001, Double.NaN, 13.27931),
                        row("T2", "Task", 8.70307, 0.33197, Double.NaN, 0.55513, Double.NaN, 13.27894),
                        row("T3", "Task", 0.12438, 0.04146, Double.NaN, 0.02000, Double.NaN, 6.21891),
                        row("E1", "Entry", 23.43875, Double.NaN, 1.76506, Double.NaN, Double.NaN, 13.27931),
                        row("E2", "Entry", 8.70307, Double.NaN, 0.65540, Double.NaN, Double.NaN, 13.27894),
                        row("E3", "Entry", 0.12438, Double.NaN, 0.02000, Double.NaN, Double.NaN, 6.21891),
                        row("AS1", "Activity", 23.44143, 0.66397, 1.76526, 1.09001, Double.NaN, 13.27931),
                        row("AS2", "Activity", 8.70441, 0.33197, 0.65550, 0.55513, Double.NaN, 13.27894),
                        row("AS3", "Activity", 0.12438, 0.04146, 0.02000, 0.02000, Double.NaN, 6.21891)
                ));
    }

//-------------------------CHATGPT SINGLE-CLASS MODELS---------------------------------------

    @Test
    @Timeout(120)
    public void testSc1SimpleChain() throws Exception {
        assertFinalOutputMatches(
                LayeredNetworkTestExamples.sc1_simple_chain(),
                rows(
                        row("P1", "Processor", Double.NaN, 1.00008, Double.NaN, Double.NaN, Double.NaN, Double.NaN),
                        row("P2", "Processor", Double.NaN, 0.50004, Double.NaN, Double.NaN, Double.NaN, Double.NaN),
                        row("T1", "RefTask", 44.99961, 1.00008, Double.NaN, 8.80651, Double.NaN, 5.00039),
                        row("T2", "Task", 0.96363, 0.50004, Double.NaN, 0.19271, Double.NaN, 5.00039),
                        row("E1", "Entry", 44.99961, Double.NaN, 8.99922, Double.NaN, Double.NaN, 5.00039),
                        row("E2", "Entry", 0.96363, Double.NaN, 0.19271, Double.NaN, Double.NaN, 5.00039),
                        row("A1", "Activity", 44.99961, 1.00008, 8.99922, 8.80651, Double.NaN, 5.00039),
                        row("A2", "Activity", 0.96363, 0.50004, 0.19271, 0.19271, Double.NaN, 5.00039)
                ));
    }

    @Test
    @Timeout(120)
    public void testSc2ThreeTier() throws Exception {
        assertFinalOutputMatches(
                LayeredNetworkTestExamples.sc2_three_tier(),
                rows(
                        row("P1", "Processor", Double.NaN, 0.99948, Double.NaN, Double.NaN, Double.NaN, Double.NaN),
                        row("P2", "Processor", Double.NaN, 0.74967, Double.NaN, Double.NaN, Double.NaN, Double.NaN),
                        row("P3", "Processor", Double.NaN, 0.99987, Double.NaN, Double.NaN, Double.NaN, Double.NaN),
                        row("T1", "RefTask", 96.99009, 0.99948, Double.NaN, 6.45740, Double.NaN, 5.99686),
                        row("T2", "Task", 58.27254, 0.74967, Double.NaN, 0.45281, Double.NaN, 5.99733),
                        row("T3", "Task", 55.57524, 0.99987, Double.NaN, 4.63188, Double.NaN, 11.99843),
                        row("E1", "Entry", 96.99009, Double.NaN, 16.17347, Double.NaN, Double.NaN, 5.99686),
                        row("E2", "Entry", 58.27254, Double.NaN, 9.71641, Double.NaN, Double.NaN, 5.99733),
                        row("E3", "Entry", 55.57524, Double.NaN, 4.63188, Double.NaN, Double.NaN, 11.99843),
                        row("A1", "Activity", 96.99912, 0.99948, 16.17498, 6.45740, Double.NaN, 5.99686),
                        row("A2", "Activity", 58.29184, 0.74967, 9.71963, 0.45281, Double.NaN, 5.99733),
                        row("A3", "Activity", 55.58231, 0.99987, 4.63247, 4.63188, Double.NaN, 11.99843)
                ));
    }

    @Test
    @Timeout(120)
    public void testSc3Fanout() throws Exception {
        assertFinalOutputMatches(
                LayeredNetworkTestExamples.sc3_fanout(),
                rows(
                        row("P1", "Processor", Double.NaN, 1.00010, Double.NaN, Double.NaN, Double.NaN, Double.NaN),
                        row("P2", "Processor", Double.NaN, 0.68580, Double.NaN, Double.NaN, Double.NaN, Double.NaN),
                        row("T1", "RefTask", 37.99973, 1.00010, Double.NaN, 18.04080, Double.NaN, 2.00021),
                        row("T2", "Task", 1.91439, 0.68580, Double.NaN, 0.47854, Double.NaN, 4.00050),
                        row("E1", "Entry", 37.99973, Double.NaN, 18.99788, Double.NaN, Double.NaN, 2.00021),
                        row("E2", "Entry", 1.11673, Double.NaN, 0.55829, Double.NaN, Double.NaN, 2.00025),
                        row("E3", "Entry", 0.79766, Double.NaN, 0.39878, Double.NaN, Double.NaN, 2.00025),
                        row("A1", "Activity", 37.99967, 1.00010, 18.99785, 18.04080, Double.NaN, 2.00021),
                        row("A2", "Activity", 1.11675, 0.40005, 0.55831, 0.27915, Double.NaN, 2.00025),
                        row("A3", "Activity", 0.79768, 0.28575, 0.39879, 0.19939, Double.NaN, 2.00025)
                ));
    }

    @Test
    @Timeout(120)
    public void testSc4Serial() throws Exception {
        assertFinalOutputMatches(
                LayeredNetworkTestExamples.sc4_serial(),
                rows(
                        row("P1", "Processor", Double.NaN, 1.00003, Double.NaN, Double.NaN, Double.NaN, Double.NaN),
                        row("P2", "Processor", Double.NaN, 0.16667, Double.NaN, Double.NaN, Double.NaN, Double.NaN),
                        row("T1", "RefTask", 29.74999, 1.00003, Double.NaN, 59.10121, Double.NaN, 0.50002),
                        row("T2", "Task", 0.19842, 0.16667, Double.NaN, 0.39683, Double.NaN, 0.50002),
                        row("E1", "Entry", 29.74999, Double.NaN, 59.49804, Double.NaN, Double.NaN, 0.50002),
                        row("E2", "Entry", 0.19842, Double.NaN, 0.39683, Double.NaN, Double.NaN, 0.50002),
                        row("A1", "Activity", 14.77578, 0.50002, 29.55060, 29.55060, Double.NaN, 0.50002),
                        row("A2", "Activity", 14.97421, 0.50002, 29.94744, 29.55060, Double.NaN, 0.50002),
                        row("A3", "Activity", 0.19842, 0.16667, 0.39683, 0.39683, Double.NaN, 0.50002)
                ));
    }

    @Test
    @Timeout(120)
    public void testSc5Loop() throws Exception {
        assertFinalOutputMatches(
                LayeredNetworkTestExamples.sc5_loop(),
                rows(
                        row("P1", "Processor", Double.NaN, 1.00000, Double.NaN, Double.NaN, Double.NaN, Double.NaN),
                        row("T1", "RefTask", 15.91736, 1.00000, Double.NaN, 35.66667, Double.NaN, 0.54545),
                        row("E1", "Entry", 15.91736, Double.NaN, 29.18182, Double.NaN, Double.NaN, 0.54545),
                        row("A1", "Activity", 15.91736, 0.81818, 9.72727, 29.18182, Double.NaN, 1.63636),
                        row("A2", "Activity", 3.53719, 0.18182, 6.48485, 6.48485, Double.NaN, 0.54545)
                ));
    }

    @Test
    @Timeout(120)
    public void testSc6MultiCall() throws Exception {
        assertFinalOutputMatches(
                LayeredNetworkTestExamples.sc6_multi_call(),
                rows(
                        row("P1", "Processor", Double.NaN, 0.80011, Double.NaN, Double.NaN, Double.NaN, Double.NaN),
                        row("P2", "Processor", Double.NaN, 1.00000, Double.NaN, Double.NaN, Double.NaN, Double.NaN),
                        row("T1", "RefTask", 58.40678, 0.80011, Double.NaN, 1.00557, Double.NaN, 3.20042),
                        row("T2", "Task", 55.18124, 1.00000, Double.NaN, 3.44884, Double.NaN, 15.99993),
                        row("E1", "Entry", 58.40678, Double.NaN, 18.24971, Double.NaN, Double.NaN, 3.20042),
                        row("E2", "Entry", 55.18124, Double.NaN, 3.44884, Double.NaN, Double.NaN, 15.99993),
                        row("A1", "Activity", 58.40357, 0.80011, 18.24871, 1.00557, Double.NaN, 3.20042),
                        row("A2", "Activity", 55.17942, 1.00000, 3.44873, 3.44884, Double.NaN, 15.99993)
                ));
    }

    @Test
    @Timeout(120)
    public void testSc7Multicore() throws Exception {
        assertFinalOutputMatches(
                LayeredNetworkTestExamples.sc7_multicore(),
                rows(
                        row("P1", "Processor", Double.NaN, 1.00000, Double.NaN, Double.NaN, Double.NaN, Double.NaN),
                        row("P2", "Processor", Double.NaN, 0.25000, Double.NaN, Double.NaN, Double.NaN, Double.NaN),
                        row("T1", "RefTask", 197.60000, 1.00000, Double.NaN, 16.30000, Double.NaN, 12.00000),
                        row("T2", "Task", 2.00000, 0.25000, Double.NaN, 0.16667, Double.NaN, 12.00000),
                        row("E1", "Entry", 197.60000, Double.NaN, 16.46667, Double.NaN, Double.NaN, 12.00000),
                        row("E2", "Entry", 2.00000, Double.NaN, 0.16667, Double.NaN, Double.NaN, 12.00000),
                        row("A1", "Activity", 197.60001, 1.00000, 16.46667, 16.30000, Double.NaN, 12.00000),
                        row("A2", "Activity", 2.00000, 0.25000, 0.16667, 0.16667, Double.NaN, 12.00000)
                ));
    }

    @Test
    @Timeout(120)
    public void testSc8Immediate() throws Exception {
        assertFinalOutputMatches(
                LayeredNetworkTestExamples.sc8_immediate(),
                rows(
                        row("P1", "Processor", Double.NaN, 1.00000, Double.NaN, Double.NaN, Double.NaN, Double.NaN),
                        row("P2", "Processor", Double.NaN, 0.0, Double.NaN, Double.NaN, Double.NaN, Double.NaN),
                        row("T1", "RefTask", 9.00000, 1.00000, Double.NaN, 9.00000, Double.NaN, 1.00000),
                        row("T2", "Task", 0.0, 0.0, Double.NaN, 0.0, Double.NaN, 1.00000),
                        row("E1", "Entry", 9.00000, Double.NaN, 9.00000, Double.NaN, Double.NaN, 1.00000),
                        row("E2", "Entry", 0.0, Double.NaN, 0.0, Double.NaN, Double.NaN, 1.00000),
                        row("A1", "Activity", 9.00000, 1.00000, 9.00000, 9.00000, Double.NaN, 1.00000),
                        row("A2", "Activity", 0.0, 0.0, 0.0, 0.0, Double.NaN, 1.00000)
                ));
    }

    @Test
    @Timeout(120)
    public void testSc9TwoCalls() throws Exception {
        assertFinalOutputMatches(
                LayeredNetworkTestExamples.sc9_two_calls(),
                rows(
                        row("P1", "Processor", Double.NaN, 1.00007, Double.NaN, Double.NaN, Double.NaN, Double.NaN),
                        row("P2", "Processor", Double.NaN, 0.25002, Double.NaN, Double.NaN, Double.NaN, Double.NaN),
                        row("T1", "RefTask", 24.74998, 1.00007, Double.NaN, 48.84124, Double.NaN, 0.50004),
                        row("T2", "Task", 0.32761, 0.25002, Double.NaN, 0.32758, Double.NaN, 1.00007),
                        row("E1", "Entry", 24.74998, Double.NaN, 49.49641, Double.NaN, Double.NaN, 0.50004),
                        row("E2", "Entry", 0.32761, Double.NaN, 0.32758, Double.NaN, Double.NaN, 1.00007),
                        row("A1", "Activity", 12.37499, 0.50004, 24.74820, 24.42062, Double.NaN, 0.50004),
                        row("A2", "Activity", 12.37499, 0.50004, 24.74820, 24.42062, Double.NaN, 0.50004),
                        row("A3", "Activity", 0.32761, 0.25002, 0.32758, 0.32758, Double.NaN, 1.00007)
                ));
    }

    @Test
    @Timeout(120)
    public void testSc10DeepChain() throws Exception {
        assertFinalOutputMatches(
                LayeredNetworkTestExamples.sc10_deep_chain(),
                rows(
                        row("P1", "Processor", Double.NaN, 1.00002, Double.NaN, Double.NaN, Double.NaN, Double.NaN),
                        row("P2", "Processor", Double.NaN, 0.66672, Double.NaN, Double.NaN, Double.NaN, Double.NaN),
                        row("P3", "Processor", Double.NaN, 0.50007, Double.NaN, Double.NaN, Double.NaN, Double.NaN),
                        row("P4", "Processor", Double.NaN, 0.40009, Double.NaN, Double.NaN, Double.NaN, Double.NaN),
                        row("T1", "RefTask", 79.33290, 1.00002, Double.NaN, 37.91178, Double.NaN, 2.00003),
                        row("T2", "Task", 3.50834, 0.66672, Double.NaN, 0.93693, Double.NaN, 2.00015),
                        row("T3", "Task", 1.63442, 0.50007, Double.NaN, 0.48824, Double.NaN, 2.00026),
                        row("T4", "Task", 0.65788, 0.40009, Double.NaN, 0.32886, Double.NaN, 2.00047),
                        row("E1", "Entry", 79.33290, Double.NaN, 39.66584, Double.NaN, Double.NaN, 2.00003),
                        row("E2", "Entry", 3.50834, Double.NaN, 1.75404, Double.NaN, Double.NaN, 2.00015),
                        row("E3", "Entry", 1.63442, Double.NaN, 0.81710, Double.NaN, Double.NaN, 2.00026),
                        row("E4", "Entry", 0.65788, Double.NaN, 0.32886, Double.NaN, Double.NaN, 2.00047),
                        row("A1", "Activity", 79.33257, 1.00002, 39.66568, 37.91178, Double.NaN, 2.00003),
                        row("A2", "Activity", 3.50877, 0.66672, 1.75426, 0.93693, Double.NaN, 2.00015),
                        row("A3", "Activity", 1.63461, 0.50007, 0.81720, 0.48824, Double.NaN, 2.00026),
                        row("A4", "Activity", 0.65793, 0.40009, 0.32889, 0.32886, Double.NaN, 2.00047)
                ));
    }

//----------------------------------------------------------------

    private void assertFinalOutputMatches(LayeredNetwork model, List<ExpectedRow> expectedRows) {
        assertNotNull(model);

        String output = captureOutput(() -> {
            SolverLNSimple solver = new SolverLNSimple(model);
            solver.iterateCoupledMva(100, 1e-4);
        });

        String actualBlock = extractFinalTableBlock(output);
        String expectedBlock = buildExpectedTable(expectedRows);

        assertEquals(expectedBlock, actualBlock);
    }

    private String captureOutput(Runnable action) {
        PrintStream originalOut = System.out;
        PrintStream originalErr = System.err;
        ByteArrayOutputStream captured = new ByteArrayOutputStream();
        PrintStream captureStream = new PrintStream(captured);

        Logger rootLogger = Logger.getLogger("");
        Logger jlineLogger = Logger.getLogger("jline");
        Logger fileUtilsLogger = Logger.getLogger("org.apache.commons.io.FileUtils");
        Level originalRootLevel = rootLogger.getLevel();
        Level originalJlineLevel = jlineLogger.getLevel();
        Level originalFileUtilsLevel = fileUtilsLogger.getLevel();

        try {
            System.setOut(captureStream);
            System.setErr(captureStream);
            rootLogger.setLevel(Level.OFF);
            jlineLogger.setLevel(Level.OFF);
            fileUtilsLogger.setLevel(Level.OFF);
            action.run();
        } finally {
            System.setOut(originalOut);
            System.setErr(originalErr);
            rootLogger.setLevel(originalRootLevel);
            jlineLogger.setLevel(originalJlineLevel);
            fileUtilsLogger.setLevel(originalFileUtilsLevel);
            captureStream.close();
        }

        return new String(captured.toByteArray());
    }

    private String extractFinalTableBlock(String output) {
        String normalized = output.replace("\r\n", "\n");
        int start = normalized.lastIndexOf(TABLE_HEADER);
        assertTrue(start >= 0, "Final table header not found in solver output\n" + normalized);

        int end = normalized.lastIndexOf(TABLE_SEPARATOR);
        assertTrue(end > start, "Final table separator not found in solver output\n" + normalized);

        return normalized.substring(start, end + TABLE_SEPARATOR.length()).trim();
    }

    private String buildExpectedTable(List<ExpectedRow> rows) {
        StringBuilder builder = new StringBuilder();
        builder.append(TABLE_HEADER).append('\n');
        builder.append(TABLE_SEPARATOR).append('\n');

        for (ExpectedRow row : rows) {
            builder.append(formatRow(row)).append('\n');
        }

        builder.append(TABLE_SEPARATOR);
        return builder.toString().trim();
    }

    private String formatRow(ExpectedRow row) {
        return String.format(
                "%-10s\t%-10s\t%-10s\t%-10s\t%-10s\t%-10s\t%-10s\t%-10s",
                row.name,
                row.type,
                formatValue(row.qlen),
                formatValue(row.util),
                formatValue(row.respT),
                formatValue(row.residT),
                formatValue(row.arvR),
                formatValue(row.tput));
    }

    private String formatValue(double value) {
        if (Double.isNaN(value)) {
            return "NaN";
        }
        if (value == 0.0) {
            return "0";
        }

        DecimalFormat formatter = new DecimalFormat("#0.#####");
        formatter.setMinimumFractionDigits(5);
        return formatter.format(value);
    }

    private List<ExpectedRow> rows(ExpectedRow... rows) {
        List<ExpectedRow> result = new ArrayList<ExpectedRow>();
        for (ExpectedRow row : rows) {
            result.add(row);
        }
        return result;
    }

    private ExpectedRow row(String name, String type, double qlen, double util, double respT, double residT, double arvR, double tput) {
        return new ExpectedRow(name, type, qlen, util, respT, residT, arvR, tput);
    }

    private static final class ExpectedRow {
        private final String name;
        private final String type;
        private final double qlen;
        private final double util;
        private final double respT;
        private final double residT;
        private final double arvR;
        private final double tput;

        private ExpectedRow(String name, String type, double qlen, double util, double respT, double residT, double arvR, double tput) {
            this.name = name;
            this.type = type;
            this.qlen = qlen;
            this.util = util;
            this.respT = respT;
            this.residT = residT;
            this.arvR = arvR;
            this.tput = tput;
        }
    }
}