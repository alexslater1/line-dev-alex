/*
 * Copyright (c) 2012-2026, QORE Lab, Imperial College London
 * All rights reserved.
 */

package jline.io;

import com.google.gson.*;

import jline.GlobalConstants;
import jline.lang.*;
import jline.lang.constant.*;
import jline.lang.constant.DropStrategy;
import jline.lang.constant.HeteroSchedPolicy;
import jline.lang.constant.JoinStrategy;
import jline.lang.constant.ServerType;
import jline.lang.layered.*;
import jline.lang.nodes.*;
import jline.lang.processes.*;
import jline.lang.sections.ClassSwitcher;
import jline.lang.sections.Forker;
import jline.lang.sections.Joiner;
import jline.lang.workflow.Workflow;
import jline.lang.workflow.WorkflowActivity;
import jline.util.matrix.Matrix;

import java.io.*;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

/**
 * Provides save/load functionality for LINE queueing network models to/from JSON format.
 *
 * <p>Supports both {@link Network} (queueing network) and {@link LayeredNetwork} (layered
 * queueing network) models. The JSON format follows the line-model.schema.json specification.</p>
 *
 * <p>Usage:
 * <pre>
 *   // Save a Network model
 *   LineModelIO.save(model, "mymodel.json");
 *
 *   // Save a LayeredNetwork model
 *   LineModelIO.save(lqnModel, "mylqn.json");
 *
 *   // Load any model
 *   Object model = LineModelIO.load("mymodel.json");
 *   if (model instanceof Network) { ... }
 *   if (model instanceof LayeredNetwork) { ... }
 * </pre>
 * </p>
 */
public class LineModelIO {

    private static final String FORMAT_NAME = "line-model";
    private static final String FORMAT_VERSION = "1.0";

    // ========================================================================
    // SAVE: Network
    // ========================================================================

    /**
     * Builds a {@link JsonObject} representation of a {@link Network} model.
     *
     * @param model the queueing network model to serialize
     * @return the JSON document representing the model
     */
    public static JsonObject toJsonObject(Network model) {
        JsonObject doc = new JsonObject();
        doc.addProperty("format", FORMAT_NAME);
        doc.addProperty("version", FORMAT_VERSION);

        JsonObject modelObj = new JsonObject();
        modelObj.addProperty("type", "Network");
        modelObj.addProperty("name", model.getName());

        // Nodes
        modelObj.add("nodes", serializeNetworkNodes(model));

        // Classes
        modelObj.add("classes", serializeNetworkClasses(model));

        // Routing
        JsonObject routingResult = serializeNetworkRouting(model);
        // Move routingStrategies/routingWeights to model level for cross-codebase compatibility
        JsonObject routingStrategies = null;
        JsonObject routingWeights = null;
        if (routingResult.has("routingStrategies")) {
            routingStrategies = routingResult.getAsJsonObject("routingStrategies");
            routingResult.remove("routingStrategies");
        }
        if (routingResult.has("routingWeights")) {
            routingWeights = routingResult.getAsJsonObject("routingWeights");
            routingResult.remove("routingWeights");
        }
        modelObj.add("routing", routingResult);
        if (routingStrategies != null) {
            modelObj.add("routingStrategies", routingStrategies);
        }
        if (routingWeights != null) {
            modelObj.add("routingWeights", routingWeights);
        }

        // Finite capacity regions
        List<Region> regions = model.getRegions();
        if (regions != null && !regions.isEmpty()) {
            JsonArray fcrArr = new JsonArray();
            List<JobClass> classes = model.getClasses();
            for (Region region : regions) {
                JsonObject rj = new JsonObject();
                rj.addProperty("name", region.getName());
                // Stations
                JsonArray stationsArr = new JsonArray();
                for (Node n : region.getNodes()) {
                    JsonObject sj = new JsonObject();
                    sj.addProperty("node", n.getName());
                    // Per-class classWeight
                    JsonObject cwObj = new JsonObject();
                    for (JobClass jc : classes) {
                        double w = region.getClassWeight(jc);
                        if (w != 1.0) {
                            cwObj.addProperty(jc.getName(), w);
                        }
                    }
                    if (cwObj.size() > 0) sj.add("classWeight", cwObj);
                    // Per-class classSize
                    JsonObject csObj = new JsonObject();
                    for (JobClass jc : classes) {
                        int sz = region.getClassSize(jc);
                        if (sz != 1) {
                            csObj.addProperty(jc.getName(), sz);
                        }
                    }
                    if (csObj.size() > 0) sj.add("classSize", csObj);
                    stationsArr.add(sj);
                }
                rj.add("stations", stationsArr);
                // globalMaxJobs
                int gmj = region.getGlobalMaxJobs();
                if (gmj >= 0) {
                    rj.addProperty("globalMaxJobs", gmj);
                }
                // globalMaxMemory
                int gmm = region.getGlobalMaxMemory();
                if (gmm >= 0) {
                    rj.addProperty("globalMaxMemory", gmm);
                }
                // classMaxJobs
                JsonObject cmjObj = new JsonObject();
                for (JobClass jc : classes) {
                    int cmj = region.getClassMaxJobs(jc);
                    if (cmj >= 0 && cmj < Integer.MAX_VALUE) {
                        cmjObj.addProperty(jc.getName(), cmj);
                    }
                }
                if (cmjObj.size() > 0) rj.add("classMaxJobs", cmjObj);
                fcrArr.add(rj);
            }
            modelObj.add("finiteCapacityRegions", fcrArr);
        }

        doc.add("model", modelObj);
        return doc;
    }

    /**
     * Saves a {@link Network} model to a JSON file.
     *
     * @param model    the queueing network model to save
     * @param filename the output file path (should end with .json)
     * @throws IOException if the file cannot be written
     */
    public static void save(Network model, String filename) throws IOException {
        JsonObject doc = toJsonObject(model);
        Gson gson = new GsonBuilder().setPrettyPrinting().serializeSpecialFloatingPointValues().create();
        Writer writer = new BufferedWriter(new FileWriter(filename));
        try {
            gson.toJson(doc, writer);
        } finally {
            writer.close();
        }
    }

    // ========================================================================
    // SAVE: LayeredNetwork
    // ========================================================================

    /**
     * Builds a {@link JsonObject} representation of a {@link LayeredNetwork} model.
     *
     * @param model the layered queueing network model to serialize
     * @return the JSON document representing the model
     */
    public static JsonObject toJsonObject(LayeredNetwork model) {
        JsonObject doc = new JsonObject();
        doc.addProperty("format", FORMAT_NAME);
        doc.addProperty("version", FORMAT_VERSION);

        JsonObject modelObj = new JsonObject();
        modelObj.addProperty("type", "LayeredNetwork");
        modelObj.addProperty("name", model.getName());

        // Hosts
        modelObj.add("hosts", serializeHosts(model));
        // Tasks
        modelObj.add("tasks", serializeTasks(model));
        // Entries
        modelObj.add("entries", serializeEntries(model));
        // Activities
        modelObj.add("activities", serializeActivities(model));
        // Precedences
        modelObj.add("precedences", serializePrecedences(model));

        doc.add("model", modelObj);
        return doc;
    }

    /**
     * Saves a {@link LayeredNetwork} model to a JSON file.
     *
     * @param model    the layered queueing network model to save
     * @param filename the output file path (should end with .json)
     * @throws IOException if the file cannot be written
     */
    public static void save(LayeredNetwork model, String filename) throws IOException {
        JsonObject doc = toJsonObject(model);
        Gson gson = new GsonBuilder().setPrettyPrinting().serializeSpecialFloatingPointValues().create();
        Writer writer = new BufferedWriter(new FileWriter(filename));
        try {
            gson.toJson(doc, writer);
        } finally {
            writer.close();
        }
    }

    // ========================================================================
    // SAVE: Workflow
    // ========================================================================

    /**
     * Saves a {@link Workflow} model to a JSON file.
     *
     * @param model    the workflow model to save
     * @param filename the output file path (should end with .json)
     * @throws IOException if the file cannot be written
     */
    public static void save(Workflow model, String filename) throws IOException {
        JsonObject doc = new JsonObject();
        doc.addProperty("format", FORMAT_NAME);
        doc.addProperty("version", FORMAT_VERSION);

        JsonObject modelObj = new JsonObject();
        modelObj.addProperty("type", "Workflow");
        modelObj.addProperty("name", model.getName());

        // Activities
        JsonArray actsArr = new JsonArray();
        for (WorkflowActivity act : model.getActivities()) {
            JsonObject actObj = new JsonObject();
            actObj.addProperty("name", act.getName());
            if (act.getHostDemand() != null) {
                actObj.add("hostDemand", serializeDistribution(act.getHostDemand()));
            }
            actsArr.add(actObj);
        }
        modelObj.add("activities", actsArr);

        // Precedences
        JsonArray precsArr = new JsonArray();
        for (ActivityPrecedence prec : model.getPrecedences()) {
            JsonObject precObj = new JsonObject();
            JsonArray preActs = new JsonArray();
            for (String name : prec.getPreActs()) {
                preActs.add(name);
            }
            precObj.add("preActs", preActs);
            JsonArray postActs = new JsonArray();
            for (String name : prec.getPostActs()) {
                postActs.add(name);
            }
            precObj.add("postActs", postActs);
            precObj.addProperty("preType", prec.getPreType());
            precObj.addProperty("postType", prec.getPostType());
            if (prec.getPreParams() != null && !prec.getPreParams().isEmpty()) {
                precObj.add("preParams", matrixToJsonArray(prec.getPreParams()));
            }
            if (prec.getPostParams() != null && !prec.getPostParams().isEmpty()) {
                precObj.add("postParams", matrixToJsonArray(prec.getPostParams()));
            }
            precsArr.add(precObj);
        }
        modelObj.add("precedences", precsArr);

        doc.add("model", modelObj);

        Gson gson = new GsonBuilder().setPrettyPrinting().serializeSpecialFloatingPointValues().create();
        Writer writer = new BufferedWriter(new FileWriter(filename));
        try {
            gson.toJson(doc, writer);
        } finally {
            writer.close();
        }
    }

    // ========================================================================
    // SAVE: Environment
    // ========================================================================

    /**
     * Saves an {@link Environment} model to a JSON file.
     *
     * @param env      the environment model to save
     * @param filename the output file path (should end with .json)
     * @throws IOException if the file cannot be written
     */
    public static void save(Environment env, String filename) throws IOException {
        JsonObject doc = new JsonObject();
        doc.addProperty("format", FORMAT_NAME);
        doc.addProperty("version", FORMAT_VERSION);

        JsonObject modelObj = new JsonObject();
        modelObj.addProperty("type", "Environment");
        modelObj.addProperty("name", env.getName());

        int nStages = env.getNumberOfStages();
        modelObj.addProperty("numStages", nStages);

        // Stages
        JsonArray stagesArr = new JsonArray();
        for (int i = 0; i < nStages; i++) {
            String stageName = env.getStageName(i);
            if (stageName == null) {
                continue;
            }
            JsonObject stageObj = new JsonObject();
            stageObj.addProperty("name", stageName);
            // Serialize the stage's Network model using existing Network serialization
            Network stageModel = env.getEnsemble().size() > i ? env.getModel(i) : null;
            if (stageModel != null) {
                JsonObject netJson = new JsonObject();
                netJson.addProperty("type", "Network");
                netJson.addProperty("name", stageModel.getName());
                netJson.add("nodes", serializeNetworkNodes(stageModel));
                netJson.add("classes", serializeNetworkClasses(stageModel));
                JsonObject routingResult = serializeNetworkRouting(stageModel);
                JsonObject routingStrategies = null;
                JsonObject routingWeights = null;
                if (routingResult.has("routingStrategies")) {
                    routingStrategies = routingResult.getAsJsonObject("routingStrategies");
                    routingResult.remove("routingStrategies");
                }
                if (routingResult.has("routingWeights")) {
                    routingWeights = routingResult.getAsJsonObject("routingWeights");
                    routingResult.remove("routingWeights");
                }
                netJson.add("routing", routingResult);
                if (routingStrategies != null) {
                    netJson.add("routingStrategies", routingStrategies);
                }
                if (routingWeights != null) {
                    netJson.add("routingWeights", routingWeights);
                }
                stageObj.add("model", netJson);
            }
            stagesArr.add(stageObj);
        }
        modelObj.add("stages", stagesArr);

        // Transitions
        JsonArray transArr = new JsonArray();
        for (int i = 0; i < nStages; i++) {
            for (int j = 0; j < nStages; j++) {
                if (env.env[i][j] != null) {
                    JsonObject transObj = new JsonObject();
                    transObj.addProperty("from", i);
                    transObj.addProperty("to", j);
                    transObj.add("distribution", serializeDistribution(env.env[i][j]));
                    transArr.add(transObj);
                }
            }
        }
        modelObj.add("transitions", transArr);

        doc.add("model", modelObj);

        Gson gson = new GsonBuilder().setPrettyPrinting().serializeSpecialFloatingPointValues().create();
        Writer writer = new BufferedWriter(new FileWriter(filename));
        try {
            gson.toJson(doc, writer);
        } finally {
            writer.close();
        }
    }

    // ========================================================================
    // LOAD
    // ========================================================================

    /**
     * Loads a model from a JSON file.
     *
     * @param filename the input file path
     * @return a {@link Network} or {@link LayeredNetwork} depending on the model type
     * @throws IOException if the file cannot be read or the format is invalid
     */
    public static Object load(String filename) throws IOException {
        Reader reader = new BufferedReader(new FileReader(filename));
        JsonObject doc;
        try {
            doc = JsonParser.parseReader(reader).getAsJsonObject();
        } finally {
            reader.close();
        }

        String format = doc.has("format") ? doc.get("format").getAsString() : "";
        if (!FORMAT_NAME.equals(format)) {
            throw new IOException("Unknown format: " + format + " (expected " + FORMAT_NAME + ")");
        }

        JsonObject modelObj = doc.getAsJsonObject("model");
        String type = modelObj.get("type").getAsString();

        if ("Network".equals(type)) {
            return loadNetwork(modelObj);
        } else if ("LayeredNetwork".equals(type)) {
            return loadLayeredNetwork(modelObj);
        } else if ("Workflow".equals(type)) {
            return loadWorkflow(modelObj);
        } else if ("Environment".equals(type)) {
            return loadEnvironment(modelObj);
        } else {
            throw new IOException("Unknown model type: " + type);
        }
    }

    // ========================================================================
    // NETWORK SERIALIZATION HELPERS
    // ========================================================================

    private static JsonArray serializeNetworkNodes(Network model) {
        JsonArray nodesArr = new JsonArray();
        List<Node> nodes = model.getNodes();
        List<JobClass> classes = model.getClasses();

        for (Node node : nodes) {
            // Skip auto-added ClassSwitch nodes (recreated by link())
            if (node instanceof ClassSwitch && ((ClassSwitch) node).autoAdded) {
                continue;
            }

            JsonObject nodeObj = new JsonObject();
            nodeObj.addProperty("name", node.getName());

            if (node instanceof Source) {
                nodeObj.addProperty("type", "Source");
                Source src = (Source) node;
                JsonObject arrivals = new JsonObject();
                for (JobClass jc : classes) {
                    Distribution dist = src.getArrivalDistribution(jc);
                    if (dist != null && !(dist instanceof Disabled)) {
                        arrivals.add(jc.getName(), serializeDistribution(dist));
                    }
                }
                nodeObj.add("service", arrivals);

            } else if (node instanceof Sink) {
                nodeObj.addProperty("type", "Sink");

            } else if (node instanceof Delay) {
                // Delay extends Queue, check BEFORE Queue
                Delay delay = (Delay) node;
                nodeObj.addProperty("type", "Delay");
                JsonObject services = new JsonObject();
                for (JobClass jc : classes) {
                    Distribution dist = delay.getService(jc);
                    if (dist != null && !(dist instanceof Disabled)) {
                        services.add(jc.getName(), serializeDistribution(dist));
                    }
                }
                nodeObj.add("service", services);

            } else if (node instanceof Queue) {
                Queue queue = (Queue) node;
                nodeObj.addProperty("type", "Queue");
                nodeObj.addProperty("scheduling", queue.getSchedStrategy().toString());
                int nServers = queue.getNumberOfServers();
                if (nServers != Integer.MAX_VALUE && nServers > 1) {
                    nodeObj.addProperty("servers", nServers);
                }
                JsonObject services = new JsonObject();
                for (JobClass jc : classes) {
                    Distribution dist = queue.getService(jc);
                    if (dist != null && !(dist instanceof Disabled)) {
                        services.add(jc.getName(), serializeDistribution(dist));
                    }
                }
                nodeObj.add("service", services);

                // Scheduling parameters (for DPS, GPS, etc.)
                boolean hasSchedPar = false;
                JsonObject schedParObj = new JsonObject();
                for (JobClass jc : classes) {
                    double par = queue.getSchedStrategyPar(jc);
                    if (par != 0.0 && !Double.isNaN(par)) {
                        schedParObj.addProperty(jc.getName(), par);
                        hasSchedPar = true;
                    }
                }
                if (hasSchedPar) {
                    nodeObj.add("schedParams", schedParObj);
                }

                // Buffer capacity
                int cap = (int) queue.getCap();
                if (cap < Integer.MAX_VALUE && cap > 0) {
                    nodeObj.addProperty("buffer", cap);
                }

                // Per-class buffer capacity
                JsonObject classCapObj = new JsonObject();
                for (JobClass jc : classes) {
                    int ccap = (int) queue.getClassCap(jc);
                    if (ccap < Integer.MAX_VALUE && ccap > 0 && ccap != cap) {
                        classCapObj.addProperty(jc.getName(), ccap);
                    }
                }
                if (classCapObj.size() > 0) {
                    nodeObj.add("classCap", classCapObj);
                }

                // Drop rules
                JsonObject dropRuleObj = new JsonObject();
                for (JobClass jc : classes) {
                    DropStrategy dr = queue.getDropRule(jc);
                    if (dr != null && dr != DropStrategy.WaitingQueue) {
                        dropRuleObj.addProperty(jc.getName(), dropStrategyToStr(dr));
                    }
                }
                if (dropRuleObj.size() > 0) {
                    nodeObj.add("dropRule", dropRuleObj);
                }

                // Load-dependent scaling
                Matrix lld = queue.getLimitedLoadDependence();
                if (lld != null && !lld.isEmpty()) {
                    JsonObject ldObj = new JsonObject();
                    ldObj.addProperty("type", "loadDependent");
                    JsonArray scalingArr = new JsonArray();
                    for (int k = 0; k < lld.getNumElements(); k++) {
                        scalingArr.add(lld.get(k));
                    }
                    ldObj.add("scaling", scalingArr);
                    nodeObj.add("loadDependence", ldObj);
                }

                // Heterogeneous server types
                if (queue.isHeterogeneous()) {
                    JsonArray stArr = new JsonArray();
                    for (ServerType st : queue.getServerTypes()) {
                        JsonObject stj = new JsonObject();
                        stj.addProperty("name", st.getName());
                        stj.addProperty("count", st.getNumOfServers());
                        // Compatible classes
                        JsonArray ccArr = new JsonArray();
                        for (JobClass cc : st.getCompatibleClasses()) {
                            ccArr.add(cc.getName());
                        }
                        if (ccArr.size() > 0) stj.add("compatibleClasses", ccArr);
                        // Per-class service distributions
                        JsonObject svcObj = new JsonObject();
                        for (JobClass jc : classes) {
                            Distribution svcDist = queue.getService(jc, st);
                            if (svcDist != null) {
                                svcObj.add(jc.getName(), serializeDistribution(svcDist));
                            }
                        }
                        if (svcObj.size() > 0) stj.add("service", svcObj);
                        stArr.add(stj);
                    }
                    nodeObj.add("serverTypes", stArr);
                    HeteroSchedPolicy hsp = queue.getHeteroSchedPolicy();
                    if (hsp != null && hsp != HeteroSchedPolicy.ORDER) {
                        nodeObj.addProperty("heteroSchedPolicy", hsp.toText());
                    }
                }

                // Balking
                JsonObject balkObj = new JsonObject();
                for (JobClass jc : classes) {
                    if (queue.hasBalking(jc)) {
                        BalkingStrategy bs = queue.getBalkingStrategy(jc);
                        List<BalkingThreshold> thresholds = queue.getBalkingThresholds(jc);
                        JsonObject bjc = new JsonObject();
                        bjc.addProperty("strategy", bs.name());
                        JsonArray thArr = new JsonArray();
                        for (BalkingThreshold th : thresholds) {
                            JsonObject tjson = new JsonObject();
                            tjson.addProperty("minJobs", th.getMinJobs());
                            tjson.addProperty("maxJobs", th.getMaxJobs() == Integer.MAX_VALUE ? -1 : th.getMaxJobs());
                            tjson.addProperty("probability", th.getProbability());
                            thArr.add(tjson);
                        }
                        bjc.add("thresholds", thArr);
                        balkObj.add(jc.getName(), bjc);
                    }
                }
                if (balkObj.size() > 0) {
                    nodeObj.add("balking", balkObj);
                }

                // Retrial
                JsonObject retObj = new JsonObject();
                for (JobClass jc : classes) {
                    if (queue.hasRetrial(jc)) {
                        Distribution delayDist = queue.getRetrialDelayDistribution(jc);
                        int maxAttempts = queue.getMaxRetrialAttempts(jc);
                        JsonObject rjc = new JsonObject();
                        rjc.add("delay", serializeDistribution(delayDist));
                        rjc.addProperty("maxAttempts", maxAttempts);
                        retObj.add(jc.getName(), rjc);
                    }
                }
                if (retObj.size() > 0) {
                    nodeObj.add("retrial", retObj);
                }

                // Patience
                JsonObject patObj = new JsonObject();
                for (JobClass jc : classes) {
                    if (queue.hasPatience(jc)) {
                        Distribution patDist = queue.getPatience(jc);
                        JsonObject pjc = new JsonObject();
                        pjc.add("distribution", serializeDistribution(patDist));
                        ImpatienceType impType = queue.getImpatienceType(jc);
                        if (impType != null) {
                            pjc.addProperty("impatienceType", ImpatienceType.toText(impType));
                        }
                        patObj.add(jc.getName(), pjc);
                    }
                }
                if (patObj.size() > 0) {
                    nodeObj.add("patience", patObj);
                }

            } else if (node instanceof Fork) {
                nodeObj.addProperty("type", "Fork");
                Fork f = (Fork) node;
                Forker fkr = (Forker) f.getOutput();
                int tpl = (int) fkr.tasksPerLink;
                if (tpl > 1) {
                    nodeObj.addProperty("tasksPerLink", tpl);
                }

            } else if (node instanceof Join) {
                nodeObj.addProperty("type", "Join");
                Join join = (Join) node;
                if (join.joinOf != null) {
                    nodeObj.addProperty("forkNode", join.joinOf.getName());
                }
                // Join strategy and quorum
                Joiner joiner = (Joiner) join.getInput();
                if (joiner != null && joiner.joinStrategy != null) {
                    for (Map.Entry<JobClass, JoinStrategy> jsEntry : joiner.joinStrategy.entrySet()) {
                        if (jsEntry.getValue() != JoinStrategy.STD) {
                            // Save as PARTIAL for MATLAB compatibility (Quorum→PARTIAL)
                            String jsName = jsEntry.getValue() == JoinStrategy.Quorum ? "PARTIAL" : jsEntry.getValue().name();
                            nodeObj.addProperty("joinStrategy", jsName);
                            break;
                        }
                    }
                }
                if (joiner != null && joiner.joinRequired != null) {
                    for (Map.Entry<JobClass, Double> jqEntry : joiner.joinRequired.entrySet()) {
                        if (jqEntry.getValue() > 0) {
                            nodeObj.addProperty("joinQuorum", jqEntry.getValue().intValue());
                            break;
                        }
                    }
                }

            } else if (node instanceof Router) {
                nodeObj.addProperty("type", "Router");

            } else if (node instanceof ClassSwitch) {
                ClassSwitch cs = (ClassSwitch) node;
                nodeObj.addProperty("type", "ClassSwitch");
                // Serialize the class-switch matrix as nested dict (compatible with Python/MATLAB)
                int K = classes.size();
                ClassSwitcher switcher = (ClassSwitcher) cs.getServer();
                JsonObject csDict = new JsonObject();
                for (int r = 0; r < K; r++) {
                    JsonObject rowObj = new JsonObject();
                    for (int s = 0; s < K; s++) {
                        double val = switcher.applyCsFun(r, s);
                        if (val != 0.0) {
                            rowObj.addProperty(classes.get(s).getName(), val);
                        }
                    }
                    if (rowObj.size() > 0) {
                        csDict.add(classes.get(r).getName(), rowObj);
                    }
                }
                if (csDict.size() > 0) {
                    nodeObj.add("classSwitchMatrix", csDict);
                }

            } else if (node instanceof Cache) {
                Cache cache = (Cache) node;
                nodeObj.addProperty("type", "Cache");
                nodeObj.addProperty("numItems", cache.getNumberOfItems());
                nodeObj.add("itemLevelCap", matrixToJsonArray(cache.getItemLevelCap()));
                nodeObj.addProperty("replacementStrategy", cache.getReplacementStrategy().toString());

                // Hit/miss class mapping
                Matrix hitClass = cache.getHitClass();
                Matrix missClass = cache.getMissClass();
                if (hitClass != null && hitClass.getNumCols() > 0) {
                    JsonObject hitMap = new JsonObject();
                    for (int r = 0; r < hitClass.getNumCols(); r++) {
                        int hc = (int) hitClass.get(r);
                        if (hc >= 0 && hc < classes.size()) {
                            hitMap.addProperty(classes.get(r).getName(), classes.get(hc).getName());
                        }
                    }
                    nodeObj.add("hitClass", hitMap);
                }
                if (missClass != null && missClass.getNumCols() > 0) {
                    JsonObject missMap = new JsonObject();
                    for (int r = 0; r < missClass.getNumCols(); r++) {
                        int mc = (int) missClass.get(r);
                        if (mc >= 0 && mc < classes.size()) {
                            missMap.addProperty(classes.get(r).getName(), classes.get(mc).getName());
                        }
                    }
                    nodeObj.add("missClass", missMap);
                }

                // Popularity distributions
                JsonObject popObj = new JsonObject();
                for (JobClass jc : classes) {
                    int jcIdx = jc.getIndex() - 1;
                    Distribution pop = cache.popularityGet(0, jcIdx);
                    if (pop != null) {
                        popObj.add(jc.getName(), serializeDistribution(pop));
                    }
                }
                if (popObj.size() > 0) {
                    nodeObj.add("popularity", popObj);
                }

            } else if (node instanceof Place) {
                nodeObj.addProperty("type", "Place");
                Place place = (Place) node;
                Matrix st = place.getState();
                if (st != null && !st.isEmpty()) {
                    JsonArray stArr = new JsonArray();
                    for (int k = 0; k < st.getNumElements(); k++) {
                        stArr.add(st.get(k));
                    }
                    nodeObj.add("initialState", stArr);
                }

            } else if (node instanceof Transition) {
                Transition trans = (Transition) node;
                nodeObj.addProperty("type", "Transition");
                List<Mode> modesList = trans.getModes();
                if (!modesList.isEmpty()) {
                    List<Node> allNodes = model.getNodes();
                    JsonArray modesArr = new JsonArray();
                    for (int mi = 0; mi < modesList.size(); mi++) {
                        Mode mode = modesList.get(mi);
                        JsonObject modeObj = new JsonObject();
                        modeObj.addProperty("name", mode.getName());
                        // Distribution
                        Distribution dist = trans.getFiringDistribution(mode);
                        if (dist != null && !(dist instanceof Disabled)) {
                            modeObj.add("distribution", serializeDistribution(dist));
                        }
                        // Timing strategy
                        TimingStrategy ts = trans.timingStrategies.get(mode);
                        if (ts != null) {
                            modeObj.addProperty("timingStrategy",
                                ts == TimingStrategy.IMMEDIATE ? "IMMEDIATE" : "TIMED");
                        }
                        // Number of servers
                        int numSrv = trans.getNumberOfModeServers(mode);
                        if (numSrv != 1) {
                            if (numSrv == Integer.MAX_VALUE) {
                                modeObj.addProperty("numServers", "Infinity");
                            } else {
                                modeObj.addProperty("numServers", numSrv);
                            }
                        }
                        // Firing priority
                        if (trans.firingPriorities.getNumElements() > mi) {
                            double fp = trans.firingPriorities.get(mi);
                            if (fp > 0) {
                                modeObj.addProperty("firingPriority", fp);
                            }
                        }
                        // Firing weight
                        if (trans.firingWeights.getNumElements() > mi) {
                            double fw = trans.firingWeights.get(mi);
                            if (fw != 1.0) {
                                modeObj.addProperty("firingWeight", fw);
                            }
                        }
                        // Enabling conditions
                        Matrix ecMat = trans.enablingConditions.get(mode);
                        if (ecMat != null) {
                            JsonArray ecArr = new JsonArray();
                            for (int ni = 0; ni < ecMat.getNumRows(); ni++) {
                                for (int ci = 0; ci < ecMat.getNumCols(); ci++) {
                                    if (ecMat.get(ni, ci) > 0) {
                                        JsonObject ec = new JsonObject();
                                        ec.addProperty("node", allNodes.get(ni).getName());
                                        ec.addProperty("class", classes.get(ci).getName());
                                        ec.addProperty("count", ecMat.get(ni, ci));
                                        ecArr.add(ec);
                                    }
                                }
                            }
                            if (ecArr.size() > 0) {
                                modeObj.add("enablingConditions", ecArr);
                            }
                        }
                        // Inhibiting conditions
                        Matrix icMat = trans.inhibitingConditions.get(mode);
                        if (icMat != null) {
                            JsonArray icArr = new JsonArray();
                            for (int ni = 0; ni < icMat.getNumRows(); ni++) {
                                for (int ci = 0; ci < icMat.getNumCols(); ci++) {
                                    double val = icMat.get(ni, ci);
                                    if (Double.isFinite(val) && val > 0) {
                                        JsonObject ic = new JsonObject();
                                        ic.addProperty("node", allNodes.get(ni).getName());
                                        ic.addProperty("class", classes.get(ci).getName());
                                        ic.addProperty("count", val);
                                        icArr.add(ic);
                                    }
                                }
                            }
                            if (icArr.size() > 0) {
                                modeObj.add("inhibitingConditions", icArr);
                            }
                        }
                        // Firing outcomes
                        Matrix foMat = trans.firingOutcomes.get(mode);
                        if (foMat != null) {
                            JsonArray foArr = new JsonArray();
                            for (int ni = 0; ni < foMat.getNumRows(); ni++) {
                                for (int ci = 0; ci < foMat.getNumCols(); ci++) {
                                    if (foMat.get(ni, ci) != 0) {
                                        JsonObject fo = new JsonObject();
                                        fo.addProperty("node", allNodes.get(ni).getName());
                                        fo.addProperty("class", classes.get(ci).getName());
                                        fo.addProperty("count", foMat.get(ni, ci));
                                        foArr.add(fo);
                                    }
                                }
                            }
                            if (foArr.size() > 0) {
                                modeObj.add("firingOutcomes", foArr);
                            }
                        }
                        modesArr.add(modeObj);
                    }
                    nodeObj.add("modes", modesArr);
                }

            } else {
                nodeObj.addProperty("type", node.getClass().getSimpleName());
            }

            nodesArr.add(nodeObj);
        }
        return nodesArr;
    }

    private static JsonArray serializeNetworkClasses(Network model) {
        JsonArray classesArr = new JsonArray();
        List<JobClass> classes = model.getClasses();

        for (JobClass jc : classes) {
            JsonObject classObj = new JsonObject();
            classObj.addProperty("name", jc.getName());

            if (jc instanceof OpenSignal) {
                OpenSignal os = (OpenSignal) jc;
                classObj.addProperty("type", "Signal");
                classObj.addProperty("openOrClosed", "Open");
                classObj.addProperty("signalType", SignalType.toText(os.getSignalType()));
                if (os.getTargetJobClass() != null) {
                    classObj.addProperty("targetClass", os.getTargetJobClass().getName());
                }
                if (os.getRemovalDistribution() != null) {
                    classObj.add("removalDistribution", serializeDistribution(os.getRemovalDistribution()));
                }
                if (os.getRemovalPolicy() != null && os.getRemovalPolicy() != RemovalPolicy.RANDOM) {
                    classObj.addProperty("removalPolicy", RemovalPolicy.toText(os.getRemovalPolicy()));
                }
            } else if (jc instanceof ClosedSignal) {
                ClosedSignal cs = (ClosedSignal) jc;
                classObj.addProperty("type", "Signal");
                classObj.addProperty("openOrClosed", "Closed");
                classObj.addProperty("signalType", SignalType.toText(cs.getSignalType()));
                if (cs.getReferenceStation() != null) {
                    classObj.addProperty("refNode", cs.getReferenceStation().getName());
                }
                if (cs.getTargetJobClass() != null) {
                    classObj.addProperty("targetClass", cs.getTargetJobClass().getName());
                }
                if (cs.getRemovalDistribution() != null) {
                    classObj.add("removalDistribution", serializeDistribution(cs.getRemovalDistribution()));
                }
                if (cs.getRemovalPolicy() != null && cs.getRemovalPolicy() != RemovalPolicy.RANDOM) {
                    classObj.addProperty("removalPolicy", RemovalPolicy.toText(cs.getRemovalPolicy()));
                }
            } else if (jc instanceof OpenClass) {
                classObj.addProperty("type", "Open");
            } else if (jc instanceof ClosedClass) {
                ClosedClass cc = (ClosedClass) jc;
                classObj.addProperty("type", "Closed");
                classObj.addProperty("population", cc.getPopulation());
                if (cc.getReferenceStation() != null) {
                    classObj.addProperty("refNode", cc.getReferenceStation().getName());
                }
            }

            int priority = jc.getPriority();
            if (priority != 0) {
                classObj.addProperty("priority", priority);
            }

            double deadline = jc.getDeadline();
            if (Double.isFinite(deadline)) {
                classObj.addProperty("deadline", deadline);
            }

            classesArr.add(classObj);
        }
        return classesArr;
    }

    private static JsonObject serializeNetworkRouting(Network model) {
        JsonObject routingObj = new JsonObject();
        routingObj.addProperty("type", "matrix");
        JsonObject matrixObj = new JsonObject();

        Map<JobClass, Map<JobClass, Matrix>> rtMap = model.getLinkedRoutingMatrix();
        // Use only original nodes (not auto-added ClassSwitch nodes) to match rtorig dimensions
        List<Node> allNodes = model.getNodes();
        List<Node> nodeList = new ArrayList<Node>();
        for (Node n : allNodes) {
            if (n instanceof ClassSwitch && ((ClassSwitch) n).autoAdded) {
                continue;
            }
            nodeList.add(n);
        }
        int M = nodeList.size();

        // Identify explicit (non-auto-added) ClassSwitch node indices
        Set<Integer> explicitCsIndices = new HashSet<Integer>();
        for (int i = 0; i < M; i++) {
            Node n = nodeList.get(i);
            if (n instanceof ClassSwitch && !((ClassSwitch) n).autoAdded) {
                explicitCsIndices.add(i);
            }
        }

        // Pre-compute same-class routing for explicit CS nodes:
        // csSame[s][i][j] = sum_r rtMap[r][s].get(i,j)
        // This gives routing from CS that _refresh_routing will reconstruct via Pcs.
        List<JobClass> classes = model.getClasses();
        int K = classes.size();
        double[][][] csSame = new double[K][M][M];
        if (rtMap != null && !explicitCsIndices.isEmpty()) {
            for (int s = 0; s < K; s++) {
                JobClass cd = classes.get(s);
                for (int r = 0; r < K; r++) {
                    JobClass cs = classes.get(r);
                    Map<JobClass, Matrix> destMap = rtMap.get(cs);
                    if (destMap == null) continue;
                    Matrix rt = destMap.get(cd);
                    if (rt == null) continue;
                    for (int ii : explicitCsIndices) {
                        for (int jj = 0; jj < M; jj++) {
                            csSame[s][ii][jj] += rt.get(ii, jj);
                        }
                    }
                }
            }
        }

        if (rtMap != null) {
            for (Map.Entry<JobClass, Map<JobClass, Matrix>> csEntry : rtMap.entrySet()) {
                JobClass cs = csEntry.getKey();
                int rIdx = classes.indexOf(cs);
                Map<JobClass, Matrix> destMap = csEntry.getValue();
                for (Map.Entry<JobClass, Matrix> cdEntry : destMap.entrySet()) {
                    JobClass cd = cdEntry.getKey();
                    int sIdx = classes.indexOf(cd);
                    Matrix rt = cdEntry.getValue();
                    if (rt == null) {
                        continue;
                    }
                    String key = cs.getName() + "," + cd.getName();
                    JsonObject fromTo = new JsonObject();
                    for (int i = 0; i < M; i++) {
                        if (explicitCsIndices.contains(i)) {
                            // For explicit CS nodes, save same-class routing only
                            if (rIdx == sIdx) {
                                JsonObject dests = null;
                                for (int j = 0; j < M; j++) {
                                    double val = csSame[sIdx][i][j];
                                    if (val > 1e-14) {
                                        if (dests == null) {
                                            dests = new JsonObject();
                                        }
                                        dests.addProperty(nodeList.get(j).getName(), val);
                                    }
                                }
                                if (dests != null) {
                                    fromTo.add(nodeList.get(i).getName(), dests);
                                }
                            }
                            // Skip cross-class entries from explicit CS
                            continue;
                        }
                        JsonObject dests = null;
                        for (int j = 0; j < M; j++) {
                            double val = rt.get(i, j);
                            if (val > 1e-14) {
                                if (dests == null) {
                                    dests = new JsonObject();
                                }
                                dests.addProperty(nodeList.get(j).getName(), val);
                            }
                        }
                        if (dests != null) {
                            fromTo.add(nodeList.get(i).getName(), dests);
                        }
                    }
                    if (fromTo.size() > 0) {
                        matrixObj.add(key, fromTo);
                    }
                }
            }
        }
        routingObj.add("matrix", matrixObj);

        // Save non-default routing strategies (RROBIN, WRROBIN, JSQ, etc.)
        // Skip PROB, RAND, and DISABLED: PROB/RAND are already captured by the
        // routing matrix and should not be reapplied on load.
        JsonObject routingStrategies = new JsonObject();
        JsonObject routingWeights = new JsonObject();
        for (Node n : allNodes) {
            JsonObject nodeStrats = null;
            JsonObject nodeWeights = null;
            for (JobClass jc : model.getClasses()) {
                RoutingStrategy rs = n.getRoutingStrategy(jc);
                if (rs != null && rs != RoutingStrategy.PROB && rs != RoutingStrategy.DISABLED
                        && rs != RoutingStrategy.RAND) {
                    if (nodeStrats == null) {
                        nodeStrats = new JsonObject();
                    }
                    nodeStrats.addProperty(jc.getName(), rs.toString());
                }
                // Save WRROBIN weights
                if (rs == RoutingStrategy.WRROBIN) {
                    List<OutputStrategy> osList = n.getOutput().getOutputStrategyByClass(jc);
                    if (osList != null) {
                        JsonObject destWeights = null;
                        for (OutputStrategy os : osList) {
                            Node dest = os.getDestination();
                            if (dest != null) {
                                if (destWeights == null) {
                                    destWeights = new JsonObject();
                                }
                                destWeights.addProperty(dest.getName(), os.getProbability());
                            }
                        }
                        if (destWeights != null) {
                            if (nodeWeights == null) {
                                nodeWeights = new JsonObject();
                            }
                            nodeWeights.add(jc.getName(), destWeights);
                        }
                    }
                }
            }
            if (nodeStrats != null) {
                routingStrategies.add(n.getName(), nodeStrats);
            }
            if (nodeWeights != null) {
                routingWeights.add(n.getName(), nodeWeights);
            }
        }
        if (routingStrategies.size() > 0) {
            routingObj.add("routingStrategies", routingStrategies);
        }
        if (routingWeights.size() > 0) {
            routingObj.add("routingWeights", routingWeights);
        }

        return routingObj;
    }

    // ========================================================================
    // LAYERED NETWORK SERIALIZATION HELPERS
    // ========================================================================

    private static JsonArray serializeHosts(LayeredNetwork model) {
        JsonArray hostsArr = new JsonArray();
        Map<Integer, Host> hosts = model.getHosts();
        for (Map.Entry<Integer, Host> entry : hosts.entrySet()) {
            Host host = entry.getValue();
            JsonObject hostObj = new JsonObject();
            hostObj.addProperty("name", host.getName());
            hostObj.addProperty("multiplicity", host.getMultiplicity());
            hostObj.addProperty("scheduling", host.getScheduling().toString());
            hostObj.addProperty("quantum", host.getQuantum());
            hostObj.addProperty("speedFactor", host.getSpeedFactor());
            int repl = host.getReplication();
            if (repl > 1) {
                hostObj.addProperty("replication", repl);
            }
            hostsArr.add(hostObj);
        }
        return hostsArr;
    }

    private static JsonArray serializeTasks(LayeredNetwork model) {
        JsonArray tasksArr = new JsonArray();
        Map<Integer, Task> tasks = model.getTasks();
        for (Map.Entry<Integer, Task> entry : tasks.entrySet()) {
            Task task = entry.getValue();
            JsonObject taskObj = new JsonObject();
            taskObj.addProperty("name", task.getName());
            if (task.getParent() != null) {
                taskObj.addProperty("host", task.getParent().getName());
            }
            taskObj.addProperty("multiplicity", task.getMultiplicity());
            taskObj.addProperty("scheduling", task.getScheduling().toString());

            int repl = task.getReplication();
            if (repl > 1) {
                taskObj.addProperty("replication", repl);
            }

            double thinkMean = task.getThinkTimeMean();
            if (thinkMean > 1e-8) {
                taskObj.addProperty("thinkTimeMean", thinkMean);
                taskObj.addProperty("thinkTimeSCV", task.getThinkTimeSCV());
            }

            int priority = task.getPriority();
            if (priority != 0) {
                taskObj.addProperty("priority", priority);
            }

            // Fan-in
            String fanInSrc = task.getFanInSource();
            if (fanInSrc != null && !fanInSrc.isEmpty()) {
                JsonObject fanInObj = new JsonObject();
                fanInObj.addProperty("source", fanInSrc);
                fanInObj.addProperty("value", task.getFanInValue());
                taskObj.add("fanIn", fanInObj);
            }

            // Fan-out
            Map<String, Integer> fanOutMap = task.getFanOutMap();
            if (fanOutMap != null && !fanOutMap.isEmpty()) {
                JsonObject fanOutObj = new JsonObject();
                for (Map.Entry<String, Integer> fo : fanOutMap.entrySet()) {
                    fanOutObj.addProperty(fo.getKey(), fo.getValue());
                }
                taskObj.add("fanOut", fanOutObj);
            }

            // CacheTask / FunctionTask properties
            if (task instanceof CacheTask) {
                CacheTask ct = (CacheTask) task;
                taskObj.addProperty("taskType", "CacheTask");
                taskObj.addProperty("totalItems", ct.getItems());
                JsonArray capArr = new JsonArray();
                for (int c : ct.getItemLevelCap()) {
                    capArr.add(c);
                }
                taskObj.add("cacheCapacity", capArr);
                taskObj.addProperty("replacementStrategy", ct.getReplacestrategy().toString());
            } else if (task instanceof FunctionTask) {
                taskObj.addProperty("taskType", "FunctionTask");
            }
            Distribution setupDist = task.getSetupTime();
            if (setupDist != null && !(setupDist instanceof Immediate)
                    && task.getSetupTimeMean() > GlobalConstants.FineTol) {
                taskObj.addProperty("setupTimeMean", task.getSetupTimeMean());
                taskObj.addProperty("setupTimeSCV", task.getSetupTimeSCV());
            }
            Distribution delayOffDist = task.getDelayOffTime();
            if (delayOffDist != null && !(delayOffDist instanceof Immediate)
                    && task.getDelayOffTimeMean() > GlobalConstants.FineTol) {
                taskObj.addProperty("delayOffTimeMean", task.getDelayOffTimeMean());
                taskObj.addProperty("delayOffTimeSCV", task.getDelayOffTimeSCV());
            }

            tasksArr.add(taskObj);
        }
        return tasksArr;
    }

    private static JsonArray serializeEntries(LayeredNetwork model) {
        JsonArray entriesArr = new JsonArray();
        Map<Integer, Entry> entries = model.getEntries();
        for (Map.Entry<Integer, Entry> entry : entries.entrySet()) {
            Entry e = entry.getValue();
            JsonObject entryObj = new JsonObject();
            entryObj.addProperty("name", e.getName());
            if (e.getParent() != null) {
                entryObj.addProperty("task", e.getParent().getName());
            }
            if (e.getArrival() != null) {
                entryObj.add("arrival", serializeDistribution(e.getArrival()));
            }

            // Forwarding
            Map<Integer, String> fwDests = e.getForwardingDests();
            Matrix fwProbs = e.getForwardingProbs();
            if (fwDests != null && !fwDests.isEmpty()) {
                JsonArray fwArr = new JsonArray();
                for (Map.Entry<Integer, String> fwEntry : fwDests.entrySet()) {
                    JsonObject fwObj = new JsonObject();
                    fwObj.addProperty("dest", fwEntry.getValue());
                    int idx = fwEntry.getKey();
                    if (fwProbs != null && !fwProbs.isEmpty() && idx < fwProbs.getNumCols()) {
                        fwObj.addProperty("prob", fwProbs.get(0, idx));
                    } else {
                        fwObj.addProperty("prob", 1.0);
                    }
                    fwArr.add(fwObj);
                }
                entryObj.add("forwarding", fwArr);
            }

            // ItemEntry properties
            if (e instanceof ItemEntry) {
                ItemEntry ie = (ItemEntry) e;
                entryObj.addProperty("entryType", "ItemEntry");
                entryObj.addProperty("totalItems", ie.getCardinality());
                Distribution pop = ie.getPopularity();
                if (pop != null) {
                    entryObj.add("accessProb", serializeDistribution(pop));
                }
            }

            entriesArr.add(entryObj);
        }
        return entriesArr;
    }

    private static JsonArray serializeActivities(LayeredNetwork model) {
        JsonArray actsArr = new JsonArray();
        Map<Integer, Activity> activities = model.getActivities();
        Map<Integer, Entry> entries = model.getEntries();

        for (Map.Entry<Integer, Activity> entry : activities.entrySet()) {
            Activity act = entry.getValue();
            int actIdx = entry.getKey();
            JsonObject actObj = new JsonObject();
            actObj.addProperty("name", act.getName());
            if (act.getParent() != null) {
                actObj.addProperty("task", act.getParent().getName());
            }

            // Host demand
            Distribution hostDem = act.getHostDemand();
            if (hostDem != null) {
                actObj.add("hostDemand", serializeDistribution(hostDem));
            }

            // Bound-to entry
            String boundTo = act.getBoundToEntry();
            if (boundTo != null && !boundTo.isEmpty()) {
                actObj.addProperty("boundToEntry", boundTo);
            }

            // Replies-to entries
            for (Map.Entry<Integer, Entry> entryEntry : entries.entrySet()) {
                Entry e = entryEntry.getValue();
                Map<Integer, String> replyMap = e.getReplyActivity();
                if (replyMap != null) {
                    for (Map.Entry<Integer, String> replyEntry : replyMap.entrySet()) {
                        if (replyEntry.getValue().equals(act.getName())) {
                            actObj.addProperty("repliesTo", e.getName());
                        }
                    }
                }
            }

            // Think time
            double thinkMean = act.getThinkTimeMean();
            if (thinkMean > 1e-8) {
                Distribution thinkDist = act.getThinkTime();
                if (thinkDist != null) {
                    actObj.add("thinkTime", serializeDistribution(thinkDist));
                }
            }

            // Synchronous calls
            Map<Integer, String> syncDests = act.getSyncCallDests();
            Matrix syncMeans = act.getSyncCallMeans();
            if (syncDests != null && !syncDests.isEmpty()) {
                JsonArray syncArr = new JsonArray();
                for (Map.Entry<Integer, String> syncEntry : syncDests.entrySet()) {
                    JsonObject callObj = new JsonObject();
                    callObj.addProperty("dest", syncEntry.getValue());
                    int idx = syncEntry.getKey();
                    double mean = 1.0;
                    if (syncMeans != null && idx < syncMeans.length()) {
                        mean = syncMeans.get(idx);
                    }
                    callObj.addProperty("mean", mean);
                    syncArr.add(callObj);
                }
                actObj.add("synchCalls", syncArr);
            }

            // Asynchronous calls
            Map<Integer, String> asyncDests = act.getAsyncCallDests();
            Matrix asyncMeans = act.getAsyncCallMeans();
            if (asyncDests != null && !asyncDests.isEmpty()) {
                JsonArray asyncArr = new JsonArray();
                for (Map.Entry<Integer, String> asyncEntry : asyncDests.entrySet()) {
                    JsonObject callObj = new JsonObject();
                    callObj.addProperty("dest", asyncEntry.getValue());
                    int idx = asyncEntry.getKey();
                    double mean = 1.0;
                    if (asyncMeans != null && idx < asyncMeans.length()) {
                        mean = asyncMeans.get(idx);
                    }
                    callObj.addProperty("mean", mean);
                    asyncArr.add(callObj);
                }
                actObj.add("asynchCalls", asyncArr);
            }

            // Call order
            String callOrder = act.getCallOrder();
            if (callOrder != null && !"STOCHASTIC".equals(callOrder)) {
                actObj.addProperty("callOrder", callOrder);
            }

            actsArr.add(actObj);
        }
        return actsArr;
    }

    private static JsonArray serializePrecedences(LayeredNetwork model) {
        JsonArray precsArr = new JsonArray();
        Map<Integer, Task> tasks = model.getTasks();

        for (Map.Entry<Integer, Task> taskEntry : tasks.entrySet()) {
            Task task = taskEntry.getValue();
            List<ActivityPrecedence> precs = task.getPrecedences();
            if (precs == null) {
                continue;
            }
            for (ActivityPrecedence prec : precs) {
                JsonObject precObj = new JsonObject();
                precObj.addProperty("task", task.getName());

                JsonArray preActs = new JsonArray();
                for (String name : prec.getPreActs()) {
                    preActs.add(name);
                }
                precObj.add("preActs", preActs);

                JsonArray postActs = new JsonArray();
                for (String name : prec.getPostActs()) {
                    postActs.add(name);
                }
                precObj.add("postActs", postActs);

                precObj.addProperty("preType", prec.getPreType());
                precObj.addProperty("postType", prec.getPostType());

                if (prec.getPreParams() != null && !prec.getPreParams().isEmpty()) {
                    precObj.add("preParams", matrixToJsonArray(prec.getPreParams()));
                }
                if (prec.getPostParams() != null && !prec.getPostParams().isEmpty()) {
                    precObj.add("postParams", matrixToJsonArray(prec.getPostParams()));
                }

                precsArr.add(precObj);
            }
        }
        return precsArr;
    }

    // ========================================================================
    // DISTRIBUTION SERIALIZATION
    // ========================================================================

    private static JsonObject serializeDistribution(Distribution dist) {
        JsonObject obj = new JsonObject();
        String name = dist.getName();
        obj.addProperty("type", name);

        if (dist instanceof Immediate) {
            // No params
        } else if (dist instanceof Disabled) {
            obj.addProperty("type", "Disabled");
        } else if ("Exp".equals(name)) {
            JsonObject params = new JsonObject();
            params.addProperty("lambda", ((Number) dist.getParam(1).getValue()).doubleValue());
            obj.add("params", params);
        } else if ("Det".equals(name)) {
            JsonObject params = new JsonObject();
            params.addProperty("value", ((Number) dist.getParam(1).getValue()).doubleValue());
            obj.add("params", params);
        } else if ("Erlang".equals(name)) {
            JsonObject params = new JsonObject();
            params.addProperty("lambda", ((Number) dist.getParam(1).getValue()).doubleValue());
            params.addProperty("k", ((Number) dist.getParam(2).getValue()).intValue());
            obj.add("params", params);
        } else if ("HyperExp".equals(name)) {
            JsonObject params = new JsonObject();
            double p = ((Number) dist.getParam(1).getValue()).doubleValue();
            double lambda1 = ((Number) dist.getParam(2).getValue()).doubleValue();
            double lambda2 = ((Number) dist.getParam(3).getValue()).doubleValue();
            JsonArray pArr = new JsonArray();
            pArr.add(p);
            pArr.add(1.0 - p);
            params.add("p", pArr);
            JsonArray lambdaArr = new JsonArray();
            lambdaArr.add(lambda1);
            lambdaArr.add(lambda2);
            params.add("lambda", lambdaArr);
            obj.add("params", params);
        } else if ("Gamma".equals(name)) {
            JsonObject params = new JsonObject();
            params.addProperty("alpha", ((Number) dist.getParam(1).getValue()).doubleValue());
            params.addProperty("beta", ((Number) dist.getParam(2).getValue()).doubleValue());
            obj.add("params", params);
        } else if ("Lognormal".equals(name)) {
            JsonObject params = new JsonObject();
            params.addProperty("mu", ((Number) dist.getParam(1).getValue()).doubleValue());
            params.addProperty("sigma", ((Number) dist.getParam(2).getValue()).doubleValue());
            obj.add("params", params);
        } else if ("Uniform".equals(name)) {
            JsonObject params = new JsonObject();
            params.addProperty("a", ((Number) dist.getParam(1).getValue()).doubleValue());
            params.addProperty("b", ((Number) dist.getParam(2).getValue()).doubleValue());
            obj.add("params", params);
        } else if ("Weibull".equals(name)) {
            JsonObject params = new JsonObject();
            params.addProperty("alpha", ((Number) dist.getParam(1).getValue()).doubleValue());
            params.addProperty("beta", ((Number) dist.getParam(2).getValue()).doubleValue());
            obj.add("params", params);
        } else if ("Pareto".equals(name)) {
            JsonObject params = new JsonObject();
            params.addProperty("alpha", ((Number) dist.getParam(1).getValue()).doubleValue());
            params.addProperty("beta", ((Number) dist.getParam(2).getValue()).doubleValue());
            obj.add("params", params);
        } else if ("Normal".equals(name)) {
            JsonObject params = new JsonObject();
            params.addProperty("mu", ((Number) dist.getParam(1).getValue()).doubleValue());
            params.addProperty("sigma", ((Number) dist.getParam(2).getValue()).doubleValue());
            obj.add("params", params);
        } else if ("Geometric".equals(name)) {
            JsonObject params = new JsonObject();
            params.addProperty("p", ((Number) dist.getParam(1).getValue()).doubleValue());
            obj.add("params", params);
        } else if ("Binomial".equals(name)) {
            JsonObject params = new JsonObject();
            params.addProperty("n", ((Number) dist.getParam(1).getValue()).intValue());
            params.addProperty("p", ((Number) dist.getParam(2).getValue()).doubleValue());
            obj.add("params", params);
        } else if ("Poisson".equals(name)) {
            JsonObject params = new JsonObject();
            params.addProperty("lambda", ((Number) dist.getParam(1).getValue()).doubleValue());
            obj.add("params", params);
        } else if ("Bernoulli".equals(name)) {
            JsonObject params = new JsonObject();
            params.addProperty("p", ((Number) dist.getParam(1).getValue()).doubleValue());
            obj.add("params", params);
        } else if ("DiscreteUniform".equals(name)) {
            JsonObject params = new JsonObject();
            params.addProperty("min", ((Number) dist.getParam(1).getValue()).doubleValue());
            params.addProperty("max", ((Number) dist.getParam(2).getValue()).doubleValue());
            obj.add("params", params);
        } else if ("Zipf".equals(name)) {
            JsonObject params = new JsonObject();
            // Zipf params: 1=p (Matrix), 2=x (Matrix), 3=s (double), 4=n (int)
            params.addProperty("s", ((Number) dist.getParam(3).getValue()).doubleValue());
            params.addProperty("n", ((Number) dist.getParam(4).getValue()).intValue());
            obj.add("params", params);
        } else if (dist instanceof DiscreteSampler) {
            obj.addProperty("type", "DiscreteSampler");
            JsonObject params = new JsonObject();
            Matrix pMat = (Matrix) dist.getParam(1).getValue();
            Matrix xMat = (Matrix) dist.getParam(2).getValue();
            JsonArray pArr = new JsonArray();
            JsonArray xArr = new JsonArray();
            for (int k = 0; k < pMat.length(); k++) {
                pArr.add(pMat.get(k));
            }
            for (int k = 0; k < xMat.length(); k++) {
                xArr.add(xMat.get(k));
            }
            params.add("p", pArr);
            params.add("x", xArr);
            obj.add("params", params);
        } else if (dist instanceof MMPP2) {
            obj.addProperty("type", "MMPP2");
            JsonObject params = new JsonObject();
            params.addProperty("lambda0", ((Number) dist.getParam(1).getValue()).doubleValue());
            params.addProperty("lambda1", ((Number) dist.getParam(2).getValue()).doubleValue());
            params.addProperty("sigma0", ((Number) dist.getParam(3).getValue()).doubleValue());
            params.addProperty("sigma1", ((Number) dist.getParam(4).getValue()).doubleValue());
            obj.add("params", params);
        } else if (dist instanceof MAP) {
            obj.addProperty("type", "MAP");
            MAP mapDist = (MAP) dist;
            JsonObject mapObj = new JsonObject();
            mapObj.add("D0", matrixToJson2D(mapDist.D(0)));
            mapObj.add("D1", matrixToJson2D(mapDist.D(1)));
            obj.add("map", mapObj);
        } else if (dist instanceof APH) {
            obj.addProperty("type", "APH");
            APH aphDist = (APH) dist;
            JsonObject phObj = new JsonObject();
            phObj.add("alpha", matrixToJsonRowVector(aphDist.getInitProb()));
            phObj.add("T", matrixToJson2D(aphDist.getSubgenerator()));
            obj.add("ph", phObj);
        } else if (dist instanceof Coxian) {
            obj.addProperty("type", "Coxian");
            Coxian coxDist = (Coxian) dist;
            JsonObject params = new JsonObject();
            Matrix mu = coxDist.getMu();
            Matrix phi = coxDist.getPhi();
            JsonArray muArr = new JsonArray();
            JsonArray phiArr = new JsonArray();
            for (int k = 0; k < mu.getNumElements(); k++) {
                muArr.add(mu.get(k));
            }
            for (int k = 0; k < phi.getNumElements(); k++) {
                phiArr.add(phi.get(k));
            }
            params.add("mu", muArr);
            params.add("phi", phiArr);
            obj.add("params", params);
        } else if (dist instanceof PH) {
            obj.addProperty("type", "PH");
            PH phDist = (PH) dist;
            JsonObject phObj = new JsonObject();
            phObj.add("alpha", matrixToJsonRowVector(phDist.getInitProb()));
            phObj.add("T", matrixToJson2D(phDist.getSubgenerator()));
            obj.add("ph", phObj);
        } else if (dist instanceof Prior) {
            obj.addProperty("type", "Prior");
            Prior priorDist = (Prior) dist;
            JsonArray distsArr = new JsonArray();
            for (int i = 0; i < priorDist.getNumAlternatives(); i++) {
                distsArr.add(serializeDistribution(priorDist.getAlternative(i)));
            }
            obj.add("distributions", distsArr);
            JsonArray probsArr = new JsonArray();
            double[] probs = priorDist.getProbabilities();
            for (double p : probs) {
                probsArr.add(p);
            }
            obj.add("probabilities", probsArr);
        } else {
            // Fallback: store mean and SCV
            JsonObject params = new JsonObject();
            try {
                params.addProperty("mean", dist.getMean());
                params.addProperty("scv", dist.getSCV());
            } catch (Exception e) {
                // Ignore errors for distributions without mean/scv
            }
            obj.add("params", params);
        }

        return obj;
    }

    // ========================================================================
    // DISTRIBUTION DESERIALIZATION
    // ========================================================================

    private static Distribution deserializeDistribution(JsonObject obj) {
        String type = obj.get("type").getAsString();

        if ("Immediate".equals(type)) {
            return Immediate.getInstance();
        } else if ("Disabled".equals(type)) {
            return new Disabled();
        } else if ("Exp".equals(type)) {
            JsonObject params = obj.getAsJsonObject("params");
            double lambda = params.get("lambda").getAsDouble();
            return new Exp(lambda);
        } else if ("Det".equals(type)) {
            JsonObject params = obj.getAsJsonObject("params");
            double value = params.get("value").getAsDouble();
            return new Det(value);
        } else if ("Erlang".equals(type)) {
            JsonObject params = obj.getAsJsonObject("params");
            double lambda = params.get("lambda").getAsDouble();
            int k = params.get("k").getAsInt();
            return new Erlang(lambda, k);
        } else if ("HyperExp".equals(type)) {
            JsonObject params = obj.getAsJsonObject("params");
            JsonArray pArr = params.getAsJsonArray("p");
            JsonArray lambdaArr = params.getAsJsonArray("lambda");
            double p = pArr.get(0).getAsDouble();
            double lambda1 = lambdaArr.get(0).getAsDouble();
            double lambda2 = lambdaArr.get(1).getAsDouble();
            return new HyperExp(p, lambda1, lambda2);
        } else if ("Gamma".equals(type)) {
            JsonObject params = obj.getAsJsonObject("params");
            double alpha = params.get("alpha").getAsDouble();
            double beta = params.get("beta").getAsDouble();
            return new Gamma(alpha, beta);
        } else if ("Lognormal".equals(type)) {
            JsonObject params = obj.getAsJsonObject("params");
            double mu = params.get("mu").getAsDouble();
            double sigma = params.get("sigma").getAsDouble();
            return new Lognormal(mu, sigma);
        } else if ("Uniform".equals(type)) {
            JsonObject params = obj.getAsJsonObject("params");
            double a = params.get("a").getAsDouble();
            double b = params.get("b").getAsDouble();
            return new Uniform(a, b);
        } else if ("Weibull".equals(type)) {
            JsonObject params = obj.getAsJsonObject("params");
            double alpha = params.get("alpha").getAsDouble();  // scale
            double beta = params.get("beta").getAsDouble();    // shape
            return new Weibull(beta, alpha);  // constructor: Weibull(shape, scale)
        } else if ("Pareto".equals(type)) {
            JsonObject params = obj.getAsJsonObject("params");
            double alpha = params.get("alpha").getAsDouble();
            double beta = params.get("beta").getAsDouble();
            return new Pareto(alpha, beta);
        } else if ("Normal".equals(type)) {
            JsonObject params = obj.getAsJsonObject("params");
            double mu = params.get("mu").getAsDouble();
            double sigma = params.get("sigma").getAsDouble();
            return new Normal(mu, sigma);
        } else if ("Geometric".equals(type)) {
            JsonObject params = obj.getAsJsonObject("params");
            double p = params.get("p").getAsDouble();
            return new Geometric(p);
        } else if ("Binomial".equals(type)) {
            JsonObject params = obj.getAsJsonObject("params");
            int n = params.get("n").getAsInt();
            double p = params.get("p").getAsDouble();
            return new Binomial(n, p);
        } else if ("Poisson".equals(type)) {
            JsonObject params = obj.getAsJsonObject("params");
            double lambda = params.get("lambda").getAsDouble();
            return new Poisson(lambda);
        } else if ("Bernoulli".equals(type)) {
            JsonObject params = obj.getAsJsonObject("params");
            double p = params.get("p").getAsDouble();
            return new Bernoulli(p);
        } else if ("DiscreteUniform".equals(type)) {
            JsonObject params = obj.getAsJsonObject("params");
            double min = params.get("min").getAsDouble();
            double max = params.get("max").getAsDouble();
            return new DiscreteUniform(min, max);
        } else if ("Zipf".equals(type)) {
            JsonObject params = obj.getAsJsonObject("params");
            double s = params.get("s").getAsDouble();
            int n = params.get("n").getAsInt();
            return new Zipf(s, n);
        } else if ("DiscreteSampler".equals(type)) {
            JsonObject params = obj.getAsJsonObject("params");
            JsonArray pArr = params.getAsJsonArray("p");
            JsonArray xArr = params.getAsJsonArray("x");
            int n = pArr.size();
            Matrix pMat = new Matrix(1, n);
            Matrix xMat = new Matrix(1, n);
            for (int k = 0; k < n; k++) {
                pMat.set(0, k, pArr.get(k).getAsDouble());
                xMat.set(0, k, xArr.get(k).getAsDouble());
            }
            return new DiscreteSampler(pMat, xMat);
        } else if ("MMPP2".equals(type)) {
            JsonObject p = obj.getAsJsonObject("params");
            return new MMPP2(
                p.get("lambda0").getAsDouble(),
                p.get("lambda1").getAsDouble(),
                p.get("sigma0").getAsDouble(),
                p.get("sigma1").getAsDouble()
            );
        } else if ("MAP".equals(type)) {
            JsonObject mapObj = obj.getAsJsonObject("map");
            Matrix D0 = jsonToMatrix2D(mapObj.getAsJsonArray("D0"));
            Matrix D1 = jsonToMatrix2D(mapObj.getAsJsonArray("D1"));
            return new MAP(D0, D1);
        } else if ("APH".equals(type)) {
            JsonObject phObj = obj.getAsJsonObject("ph");
            Matrix alpha = jsonToRowVector(phObj.getAsJsonArray("alpha"));
            Matrix T = jsonToMatrix2D(phObj.getAsJsonArray("T"));
            return new APH(alpha, T);
        } else if ("Coxian".equals(type)) {
            // Try mu/phi params format first
            if (obj.has("params")) {
                JsonObject params = obj.getAsJsonObject("params");
                if (params.has("mu") && params.has("phi")) {
                    JsonArray muArr = params.getAsJsonArray("mu");
                    JsonArray phiArr = params.getAsJsonArray("phi");
                    java.util.List<Double> muList = new java.util.ArrayList<Double>();
                    java.util.List<Double> phiList = new java.util.ArrayList<Double>();
                    for (int k = 0; k < muArr.size(); k++) {
                        muList.add(muArr.get(k).getAsDouble());
                    }
                    for (int k = 0; k < phiArr.size(); k++) {
                        phiList.add(phiArr.get(k).getAsDouble());
                    }
                    return new Coxian(muList, phiList);
                }
            }
            // Legacy: D0/D1 or alpha/T format
            JsonObject phObj = obj.getAsJsonObject("ph");
            if (phObj != null) {
                if (phObj.has("alpha") && phObj.has("T")) {
                    Matrix alpha = jsonToRowVector(phObj.getAsJsonArray("alpha"));
                    Matrix T = jsonToMatrix2D(phObj.getAsJsonArray("T"));
                    return new PH(alpha, T);
                }
                if (phObj.has("D0") && phObj.has("D1")) {
                    Matrix D0 = jsonToMatrix2D(phObj.getAsJsonArray("D0"));
                    int n = D0.getNumRows();
                    Matrix alpha = new Matrix(1, n);
                    alpha.set(0, 0, 1.0);
                    return new PH(alpha, D0);
                }
            }
            // Final fallback: mean/scv
            if (obj.has("params")) {
                JsonObject params = obj.getAsJsonObject("params");
                double mean = params.get("mean").getAsDouble();
                double scv = params.get("scv").getAsDouble();
                return Coxian.fitMeanAndSCV(mean, scv);
            }
            return new Exp(1.0);
        } else if ("PH".equals(type)) {
            JsonObject phObj = obj.getAsJsonObject("ph");
            Matrix alpha = jsonToRowVector(phObj.getAsJsonArray("alpha"));
            Matrix T = jsonToMatrix2D(phObj.getAsJsonArray("T"));
            return new PH(alpha, T);
        } else if ("Prior".equals(type)) {
            JsonArray distsArr = obj.getAsJsonArray("distributions");
            JsonArray probsArr = obj.getAsJsonArray("probabilities");
            java.util.List<Distribution> alternatives = new java.util.ArrayList<Distribution>();
            for (int i = 0; i < distsArr.size(); i++) {
                alternatives.add(deserializeDistribution(distsArr.get(i).getAsJsonObject()));
            }
            double[] probs = new double[probsArr.size()];
            for (int i = 0; i < probsArr.size(); i++) {
                probs[i] = probsArr.get(i).getAsDouble();
            }
            return new Prior(alternatives, probs);
        }

        // Fallback: if we have mean, create an Exp distribution with equivalent mean
        if (obj.has("params")) {
            JsonObject params = obj.getAsJsonObject("params");
            if (params.has("mean")) {
                double mean = params.get("mean").getAsDouble();
                if (mean > 0) {
                    return new Exp(1.0 / mean);
                }
            }
        }
        return Immediate.getInstance();
    }

    // ========================================================================
    // NETWORK LOAD
    // ========================================================================

    private static Network loadNetwork(JsonObject modelObj) throws IOException {
        String name = modelObj.has("name") ? modelObj.get("name").getAsString() : "model";
        Network model = new Network(name);

        JsonArray nodesArr = modelObj.getAsJsonArray("nodes");
        JsonArray classesArr = modelObj.getAsJsonArray("classes");
        JsonObject routingRaw = modelObj.has("routing") ? modelObj.getAsJsonObject("routing") : null;
        JsonObject routingObj = null;
        if (routingRaw != null) {
            if (routingRaw.has("matrix")) {
                routingObj = routingRaw.getAsJsonObject("matrix");
            } else {
                // Flat format (no wrapper)
                routingObj = routingRaw;
            }
        }

        // Two-pass node creation: first pass creates nodes, second sets services
        // Phase 1: Create nodes
        Map<String, Node> nodeMap = new LinkedHashMap<String, Node>();
        Map<String, JsonObject> nodeJsonMap = new LinkedHashMap<String, JsonObject>();

        for (JsonElement nodeEl : nodesArr) {
            JsonObject nodeObj = nodeEl.getAsJsonObject();
            String nodeName = nodeObj.get("name").getAsString();
            String nodeType = nodeObj.get("type").getAsString();
            nodeJsonMap.put(nodeName, nodeObj);

            Node node;
            if ("Source".equals(nodeType)) {
                node = new Source(model, nodeName);
            } else if ("Sink".equals(nodeType)) {
                node = new Sink(model, nodeName);
            } else if ("Delay".equals(nodeType)) {
                node = new Delay(model, nodeName);
            } else if ("Queue".equals(nodeType)) {
                String schedStr = nodeObj.has("scheduling") ? nodeObj.get("scheduling").getAsString() : "PS";
                SchedStrategy sched = parseSchedStrategy(schedStr);
                node = new Queue(model, nodeName, sched);
                if (nodeObj.has("servers")) {
                    int servers = nodeObj.get("servers").getAsInt();
                    if (servers > 0 && servers != Integer.MAX_VALUE) {
                        ((Queue) node).setNumberOfServers(servers);
                    }
                }
                if (nodeObj.has("buffer")) {
                    ((Queue) node).setCapacity(nodeObj.get("buffer").getAsInt());
                }
            } else if ("Fork".equals(nodeType)) {
                node = new Fork(model, nodeName);
                if (nodeObj.has("tasksPerLink")) {
                    ((Fork) node).setTasksPerLink(nodeObj.get("tasksPerLink").getAsInt());
                }
            } else if ("Join".equals(nodeType)) {
                node = new Join(model, nodeName);
            } else if ("Router".equals(nodeType)) {
                node = new Router(model, nodeName);
            } else if ("ClassSwitch".equals(nodeType)) {
                node = new ClassSwitch(model, nodeName);
            } else if ("Cache".equals(nodeType)) {
                int numItems = nodeObj.get("numItems").getAsInt();
                JsonArray capArr = nodeObj.getAsJsonArray("itemLevelCap");
                Matrix itemLevelCap = jsonToRowVector(capArr);
                String replStr = nodeObj.has("replacementStrategy") ? nodeObj.get("replacementStrategy").getAsString() : "LRU";
                ReplacementStrategy replPolicy = parseReplacementStrategy(replStr);
                node = new Cache(model, nodeName, numItems, itemLevelCap, replPolicy);
            } else if ("Place".equals(nodeType)) {
                node = new Place(model, nodeName);
            } else if ("Transition".equals(nodeType)) {
                node = new Transition(model, nodeName);
            } else {
                // Unknown node type; skip
                continue;
            }
            nodeMap.put(nodeName, node);
        }

        // Phase 2: Create classes (after all nodes exist)
        Map<String, JobClass> classMap = new LinkedHashMap<String, JobClass>();
        for (JsonElement classEl : classesArr) {
            JsonObject classObj = classEl.getAsJsonObject();
            String className = classObj.get("name").getAsString();
            String classType = classObj.get("type").getAsString();

            JobClass jc;
            if ("Open".equals(classType)) {
                int priority = classObj.has("priority") ? classObj.get("priority").getAsInt() : 0;
                jc = new OpenClass(model, className, priority);
            } else if ("Closed".equals(classType)) {
                double population = classObj.get("population").getAsDouble();
                String refNodeName = classObj.has("refNode") ? classObj.get("refNode").getAsString() : null;
                Station refStat = null;
                if (refNodeName != null) {
                    Node refNode = nodeMap.get(refNodeName);
                    if (refNode instanceof Station) {
                        refStat = (Station) refNode;
                    }
                }
                int priority = classObj.has("priority") ? classObj.get("priority").getAsInt() : 0;
                if (refStat != null) {
                    jc = new ClosedClass(model, className, population, refStat, priority);
                } else {
                    throw new IOException("Reference station '" + refNodeName + "' not found for closed class '" + className + "'");
                }
            } else if ("Signal".equals(classType)) {
                int priority = classObj.has("priority") ? classObj.get("priority").getAsInt() : 0;
                SignalType sigType = SignalType.NEGATIVE;
                if (classObj.has("signalType")) {
                    sigType = SignalType.fromText(classObj.get("signalType").getAsString());
                }
                String openOrClosed = classObj.has("openOrClosed") ? classObj.get("openOrClosed").getAsString() : "Open";
                if ("Closed".equals(openOrClosed)) {
                    String refNodeName = classObj.has("refNode") ? classObj.get("refNode").getAsString() : null;
                    Station refStat = null;
                    if (refNodeName != null) {
                        Node refNode = nodeMap.get(refNodeName);
                        if (refNode instanceof Station) {
                            refStat = (Station) refNode;
                        }
                    }
                    if (refStat == null) {
                        throw new IOException("Reference station not found for closed signal '" + className + "'");
                    }
                    jc = new ClosedSignal(model, className, sigType, refStat, priority);
                } else {
                    jc = new OpenSignal(model, className, sigType, priority);
                }
                // Removal distribution
                if (classObj.has("removalDistribution")) {
                    Distribution remDist = deserializeDistribution(classObj.getAsJsonObject("removalDistribution"));
                    if (remDist instanceof DiscreteDistribution) {
                        DiscreteDistribution dremDist = (DiscreteDistribution) remDist;
                        if (jc instanceof OpenSignal) ((OpenSignal) jc).setRemovalDistribution(dremDist);
                        else if (jc instanceof ClosedSignal) ((ClosedSignal) jc).setRemovalDistribution(dremDist);
                    }
                }
                // Removal policy
                if (classObj.has("removalPolicy")) {
                    RemovalPolicy rp = RemovalPolicy.fromText(classObj.get("removalPolicy").getAsString());
                    if (jc instanceof OpenSignal) ((OpenSignal) jc).setRemovalPolicy(rp);
                    else if (jc instanceof ClosedSignal) ((ClosedSignal) jc).setRemovalPolicy(rp);
                }
            } else {
                throw new IOException("Unknown class type: " + classType);
            }
            // Class deadline
            if (classObj.has("deadline")) {
                double deadline = classObj.get("deadline").getAsDouble();
                if (Double.isFinite(deadline)) {
                    jc.setDeadline(deadline);
                }
            }

            classMap.put(className, jc);
        }

        // Resolve signal targetClass associations (after all classes are created)
        for (JsonElement classEl : classesArr) {
            JsonObject classObj = classEl.getAsJsonObject();
            if (!"Signal".equals(classObj.get("type").getAsString())) continue;
            if (!classObj.has("targetClass")) continue;
            String sigName = classObj.get("name").getAsString();
            String targetName = classObj.get("targetClass").getAsString();
            JobClass sigCls = classMap.get(sigName);
            JobClass targetCls = classMap.get(targetName);
            if (sigCls != null && targetCls != null) {
                if (sigCls instanceof OpenSignal) ((OpenSignal) sigCls).forJobClass(targetCls);
                else if (sigCls instanceof ClosedSignal) ((ClosedSignal) sigCls).forJobClass(targetCls);
            }
        }

        // Phase 3: Set service distributions and node-specific parameters
        List<JobClass> jobClasses = model.getClasses();
        for (Map.Entry<String, JsonObject> njEntry : nodeJsonMap.entrySet()) {
            String nodeName = njEntry.getKey();
            JsonObject nodeObj = njEntry.getValue();
            String nodeType = nodeObj.get("type").getAsString();
            Node node = nodeMap.get(nodeName);
            if (node == null) {
                continue;
            }

            if ("Source".equals(nodeType)) {
                Source src = (Source) node;
                JsonObject arrivals = null;
                if (nodeObj.has("service")) {
                    arrivals = nodeObj.getAsJsonObject("service");
                } else if (nodeObj.has("arrivals")) {
                    arrivals = nodeObj.getAsJsonObject("arrivals");
                }
                if (arrivals != null) {
                    for (Map.Entry<String, JsonElement> ae : arrivals.entrySet()) {
                        JobClass jc = classMap.get(ae.getKey());
                        if (jc != null) {
                            Distribution dist = deserializeDistribution(ae.getValue().getAsJsonObject());
                            src.setArrival(jc, dist);
                        }
                    }
                }
            } else if ("Delay".equals(nodeType) || "Queue".equals(nodeType)) {
                Queue queue = (Queue) node;
                if (nodeObj.has("service")) {
                    JsonObject services = nodeObj.getAsJsonObject("service");
                    for (Map.Entry<String, JsonElement> se : services.entrySet()) {
                        JobClass jc = classMap.get(se.getKey());
                        if (jc != null) {
                            Distribution dist = deserializeDistribution(se.getValue().getAsJsonObject());
                            queue.setService(jc, dist);
                        }
                    }
                }
                if (nodeObj.has("schedParams")) {
                    JsonObject schedParams = nodeObj.getAsJsonObject("schedParams");
                    for (Map.Entry<String, JsonElement> sp : schedParams.entrySet()) {
                        JobClass jc = classMap.get(sp.getKey());
                        if (jc != null) {
                            queue.setSchedStrategyPar(jc, sp.getValue().getAsDouble());
                        }
                    }
                }
                // Per-class buffer capacity
                if (nodeObj.has("classCap")) {
                    JsonObject classCapObj = nodeObj.getAsJsonObject("classCap");
                    for (Map.Entry<String, JsonElement> ccEntry : classCapObj.entrySet()) {
                        JobClass jc = classMap.get(ccEntry.getKey());
                        if (jc != null) {
                            queue.setClassCap(jc, ccEntry.getValue().getAsInt());
                        }
                    }
                }
                // Drop rules
                if (nodeObj.has("dropRule")) {
                    JsonObject dropRuleObj = nodeObj.getAsJsonObject("dropRule");
                    for (Map.Entry<String, JsonElement> drEntry : dropRuleObj.entrySet()) {
                        JobClass jc = classMap.get(drEntry.getKey());
                        if (jc != null) {
                            DropStrategy ds = parseDropStrategy(drEntry.getValue().getAsString());
                            queue.setDropRule(jc, ds);
                        }
                    }
                }
                // Load-dependent scaling
                if (nodeObj.has("loadDependence")) {
                    JsonObject ldObj = nodeObj.getAsJsonObject("loadDependence");
                    String ldType = ldObj.has("type") ? ldObj.get("type").getAsString() : "";
                    if ("loadDependent".equals(ldType) && ldObj.has("scaling")) {
                        JsonArray scalingArr = ldObj.getAsJsonArray("scaling");
                        Matrix alpha = new Matrix(1, scalingArr.size());
                        for (int k = 0; k < scalingArr.size(); k++) {
                            alpha.set(0, k, scalingArr.get(k).getAsDouble());
                        }
                        queue.setLoadDependence(alpha);
                    }
                }
                // Heterogeneous server types
                if (nodeObj.has("serverTypes")) {
                    for (JsonElement stEl : nodeObj.getAsJsonArray("serverTypes")) {
                        JsonObject stObj = stEl.getAsJsonObject();
                        String stName = stObj.get("name").getAsString();
                        int stCount = stObj.get("count").getAsInt();
                        ServerType st = new ServerType(stName, stCount);
                        // Compatible classes
                        if (stObj.has("compatibleClasses")) {
                            for (JsonElement ccEl : stObj.getAsJsonArray("compatibleClasses")) {
                                JobClass jc = classMap.get(ccEl.getAsString());
                                if (jc != null) {
                                    st.addCompatibleClass(jc);
                                }
                            }
                        }
                        queue.addServerType(st);
                        // Per-class service distributions
                        if (stObj.has("service")) {
                            JsonObject svcObj = stObj.getAsJsonObject("service");
                            for (Map.Entry<String, JsonElement> svcEntry : svcObj.entrySet()) {
                                JobClass jc = classMap.get(svcEntry.getKey());
                                if (jc != null) {
                                    Distribution dist = deserializeDistribution(svcEntry.getValue().getAsJsonObject());
                                    queue.setService(jc, st, dist);
                                }
                            }
                        }
                    }
                    // Scheduling policy
                    if (nodeObj.has("heteroSchedPolicy")) {
                        HeteroSchedPolicy hsp = HeteroSchedPolicy.fromText(nodeObj.get("heteroSchedPolicy").getAsString());
                        queue.setHeteroSchedPolicy(hsp);
                    }
                }

                // Balking
                if (nodeObj.has("balking")) {
                    JsonObject balkObj = nodeObj.getAsJsonObject("balking");
                    for (Map.Entry<String, JsonElement> balkEntry : balkObj.entrySet()) {
                        JobClass jc = classMap.get(balkEntry.getKey());
                        if (jc == null) continue;
                        JsonObject bjc = balkEntry.getValue().getAsJsonObject();
                        String stratStr = bjc.get("strategy").getAsString();
                        BalkingStrategy bs = BalkingStrategy.valueOf(stratStr);
                        List<BalkingThreshold> thresholds = new ArrayList<BalkingThreshold>();
                        JsonArray thArr = bjc.getAsJsonArray("thresholds");
                        for (JsonElement thEl : thArr) {
                            JsonObject tObj = thEl.getAsJsonObject();
                            int minJobs = tObj.get("minJobs").getAsInt();
                            int maxJobs = tObj.get("maxJobs").getAsInt();
                            if (maxJobs < 0) maxJobs = Integer.MAX_VALUE;
                            double prob = tObj.get("probability").getAsDouble();
                            thresholds.add(new BalkingThreshold(minJobs, maxJobs, prob));
                        }
                        queue.setBalking(jc, bs, thresholds);
                    }
                }

                // Retrial
                if (nodeObj.has("retrial")) {
                    JsonObject retObj = nodeObj.getAsJsonObject("retrial");
                    for (Map.Entry<String, JsonElement> retEntry : retObj.entrySet()) {
                        JobClass jc = classMap.get(retEntry.getKey());
                        if (jc == null) continue;
                        JsonObject rjc = retEntry.getValue().getAsJsonObject();
                        Distribution delayDist = deserializeDistribution(rjc.getAsJsonObject("delay"));
                        int maxAttempts = -1;
                        if (rjc.has("maxAttempts")) {
                            maxAttempts = rjc.get("maxAttempts").getAsInt();
                        }
                        queue.setRetrial(jc, delayDist, maxAttempts);
                    }
                }

                // Patience
                if (nodeObj.has("patience")) {
                    JsonObject patObj = nodeObj.getAsJsonObject("patience");
                    for (Map.Entry<String, JsonElement> patEntry : patObj.entrySet()) {
                        JobClass jc = classMap.get(patEntry.getKey());
                        if (jc == null) continue;
                        JsonObject pjc = patEntry.getValue().getAsJsonObject();
                        Distribution patDist = deserializeDistribution(pjc.getAsJsonObject("distribution"));
                        ImpatienceType impType = ImpatienceType.RENEGING;
                        if (pjc.has("impatienceType")) {
                            String itStr = pjc.get("impatienceType").getAsString();
                            if ("balking".equals(itStr)) impType = ImpatienceType.BALKING;
                            else if ("retrial".equals(itStr)) impType = ImpatienceType.RETRIAL;
                        }
                        queue.setPatience(jc, impType, patDist);
                    }
                }
            } else if ("Join".equals(nodeType)) {
                Join join = (Join) node;
                // Support both "forkNode" (MATLAB/Python format) and "joinOf" (JAR format)
                String forkNodeKey = nodeObj.has("forkNode") ? "forkNode" : "joinOf";
                if (nodeObj.has(forkNodeKey)) {
                    String forkName = nodeObj.get(forkNodeKey).getAsString();
                    Node forkNode = nodeMap.get(forkName);
                    if (forkNode != null) {
                        join.joinOf = forkNode;
                    }
                }
                // Join strategy
                if (nodeObj.has("joinStrategy")) {
                    String jsStr = nodeObj.get("joinStrategy").getAsString();
                    JoinStrategy js = JoinStrategy.STD;
                    try {
                        // Map aliases from other codebases
                        if ("PARTIAL".equals(jsStr) || "QUORUM".equals(jsStr)) {
                            js = JoinStrategy.Quorum;
                        } else {
                            js = JoinStrategy.valueOf(jsStr);
                        }
                    } catch (IllegalArgumentException e) {
                        // keep STD as default
                    }
                    for (JobClass jc : jobClasses) {
                        join.setStrategy(jc, js);
                    }
                }
                // Join quorum
                if (nodeObj.has("joinQuorum")) {
                    int jq = nodeObj.get("joinQuorum").getAsInt();
                    Joiner joiner = (Joiner) join.getInput();
                    if (joiner != null) {
                        for (JobClass jc : jobClasses) {
                            joiner.setRequired(jc, jq);
                        }
                    }
                }
            } else if ("ClassSwitch".equals(nodeType)) {
                ClassSwitch cs = (ClassSwitch) node;
                if (nodeObj.has("classSwitchMatrix")) {
                    // Dict format: {"Class1": {"Class1": 0.3, "Class2": 0.7}, ...}
                    JsonObject csDict = nodeObj.getAsJsonObject("classSwitchMatrix");
                    ClassSwitchMatrix csm = cs.initClassSwitchMatrix();
                    for (Map.Entry<String, JsonElement> fromEntry : csDict.entrySet()) {
                        JobClass fromClass = classMap.get(fromEntry.getKey());
                        if (fromClass == null) continue;
                        int r = jobClasses.indexOf(fromClass);
                        if (r < 0) continue;
                        JsonObject toObj = fromEntry.getValue().getAsJsonObject();
                        for (Map.Entry<String, JsonElement> toEntry : toObj.entrySet()) {
                            JobClass toClass = classMap.get(toEntry.getKey());
                            if (toClass == null) continue;
                            int s = jobClasses.indexOf(toClass);
                            if (s < 0) continue;
                            csm.set(r, s, toEntry.getValue().getAsDouble());
                        }
                    }
                    cs.setClassSwitchingMatrix(csm);
                } else if (nodeObj.has("csMatrix")) {
                    // Legacy 2D array format: [[0.3, 0.7], [1.0, 0.0]]
                    JsonArray csMatrixArr = nodeObj.getAsJsonArray("csMatrix");
                    int K = csMatrixArr.size();
                    ClassSwitchMatrix csm = cs.initClassSwitchMatrix();
                    for (int r = 0; r < K; r++) {
                        JsonArray row = csMatrixArr.get(r).getAsJsonArray();
                        for (int s = 0; s < row.size(); s++) {
                            csm.set(r, s, row.get(s).getAsDouble());
                        }
                    }
                    cs.setClassSwitchingMatrix(csm);
                }
            } else if ("Cache".equals(nodeType)) {
                Cache cache = (Cache) node;
                // Hit/miss class mapping
                if (nodeObj.has("hitClass")) {
                    JsonObject hitMap = nodeObj.getAsJsonObject("hitClass");
                    for (Map.Entry<String, JsonElement> he : hitMap.entrySet()) {
                        JobClass inClass = classMap.get(he.getKey());
                        JobClass outClass = classMap.get(he.getValue().getAsString());
                        if (inClass != null && outClass != null) {
                            cache.setHitClass(inClass, outClass);
                        }
                    }
                }
                if (nodeObj.has("missClass")) {
                    JsonObject missMap = nodeObj.getAsJsonObject("missClass");
                    for (Map.Entry<String, JsonElement> me : missMap.entrySet()) {
                        JobClass inClass = classMap.get(me.getKey());
                        JobClass outClass = classMap.get(me.getValue().getAsString());
                        if (inClass != null && outClass != null) {
                            cache.setMissClass(inClass, outClass);
                        }
                    }
                }
                // Popularity distributions
                if (nodeObj.has("popularity")) {
                    JsonObject popObj = nodeObj.getAsJsonObject("popularity");
                    for (Map.Entry<String, JsonElement> pe : popObj.entrySet()) {
                        JobClass jc = classMap.get(pe.getKey());
                        if (jc != null) {
                            Distribution popDist = deserializeDistribution(pe.getValue().getAsJsonObject());
                            // Skip Disabled distributions (hit/miss classes have no popularity)
                            if (popDist.isDiscrete()) {
                                cache.setRead(jc, popDist);
                            }
                        }
                    }
                }
            }
        }

        // Phase 3b: Configure Transition modes (after all classes exist)
        for (Map.Entry<String, JsonObject> njEntry : nodeJsonMap.entrySet()) {
            String nodeName = njEntry.getKey();
            JsonObject nodeObj = njEntry.getValue();
            String nodeType = nodeObj.get("type").getAsString();
            if (!"Transition".equals(nodeType)) continue;
            if (!nodeObj.has("modes")) continue;

            Node node = nodeMap.get(nodeName);
            if (!(node instanceof Transition)) continue;
            Transition tnode = (Transition) node;

            JsonArray modesArr = nodeObj.getAsJsonArray("modes");
            for (JsonElement modeEl : modesArr) {
                JsonObject md = modeEl.getAsJsonObject();
                String modeName = md.has("name") ? md.get("name").getAsString() : "Mode";
                Mode mode = tnode.addMode(modeName);

                // Distribution
                if (md.has("distribution")) {
                    Distribution dist = deserializeDistribution(md.getAsJsonObject("distribution"));
                    if (dist != null) {
                        tnode.setDistribution(mode, dist);
                    }
                }
                // Timing strategy
                if (md.has("timingStrategy")) {
                    String tsStr = md.get("timingStrategy").getAsString();
                    if ("IMMEDIATE".equals(tsStr)) {
                        tnode.setTimingStrategy(mode, TimingStrategy.IMMEDIATE);
                    } else {
                        tnode.setTimingStrategy(mode, TimingStrategy.TIMED);
                    }
                }
                // Number of servers
                if (md.has("numServers")) {
                    JsonElement nsEl = md.get("numServers");
                    int ns;
                    if (nsEl.isJsonPrimitive() && nsEl.getAsJsonPrimitive().isString()) {
                        ns = "Infinity".equalsIgnoreCase(nsEl.getAsString()) ? Integer.MAX_VALUE : Integer.parseInt(nsEl.getAsString());
                    } else {
                        ns = nsEl.getAsInt();
                    }
                    tnode.setNumberOfServers(mode, ns);
                }
                // Firing priority
                if (md.has("firingPriority")) {
                    tnode.setFiringPriorities(mode, (int) md.get("firingPriority").getAsDouble());
                }
                // Firing weight
                if (md.has("firingWeight")) {
                    tnode.setFiringWeights(mode, md.get("firingWeight").getAsDouble());
                }
                // Enabling conditions
                if (md.has("enablingConditions")) {
                    for (JsonElement ecEl : md.getAsJsonArray("enablingConditions")) {
                        JsonObject ec = ecEl.getAsJsonObject();
                        Node ecNode = nodeMap.get(ec.get("node").getAsString());
                        JobClass ecClass = classMap.get(ec.get("class").getAsString());
                        if (ecNode instanceof Place && ecClass != null) {
                            tnode.setEnablingConditions(mode, ecClass, (Place) ecNode, (int) ec.get("count").getAsDouble());
                        }
                    }
                }
                // Inhibiting conditions
                if (md.has("inhibitingConditions")) {
                    for (JsonElement icEl : md.getAsJsonArray("inhibitingConditions")) {
                        JsonObject ic = icEl.getAsJsonObject();
                        Node icNode = nodeMap.get(ic.get("node").getAsString());
                        JobClass icClass = classMap.get(ic.get("class").getAsString());
                        if (icNode instanceof Place && icClass != null) {
                            tnode.setInhibitingConditions(mode, icClass, (Place) icNode, (int) ic.get("count").getAsDouble());
                        }
                    }
                }
                // Firing outcomes
                if (md.has("firingOutcomes")) {
                    for (JsonElement foEl : md.getAsJsonArray("firingOutcomes")) {
                        JsonObject fo = foEl.getAsJsonObject();
                        Node foNode = nodeMap.get(fo.get("node").getAsString());
                        JobClass foClass = classMap.get(fo.get("class").getAsString());
                        if (foNode != null && foClass != null) {
                            tnode.setFiringOutcome(mode, foClass, foNode, (int) fo.get("count").getAsDouble());
                        }
                    }
                }
            }
        }

        // Phase 3c: Restore initial state for Place nodes
        for (Map.Entry<String, JsonObject> njEntry : nodeJsonMap.entrySet()) {
            JsonObject nodeObj = njEntry.getValue();
            if (!nodeObj.has("initialState")) continue;
            Node node = nodeMap.get(njEntry.getKey());
            if (!(node instanceof Place)) continue;

            JsonElement isElem = nodeObj.get("initialState");
            Matrix stateVec;
            if (isElem.isJsonArray()) {
                JsonArray isArr = isElem.getAsJsonArray();
                stateVec = new Matrix(1, isArr.size());
                for (int k = 0; k < isArr.size(); k++) {
                    stateVec.set(0, k, isArr.get(k).getAsDouble());
                }
            } else {
                stateVec = Matrix.singleton(isElem.getAsDouble());
            }
            node.setState(stateVec);
        }

        // Phase 4: Build routing matrix and link
        if (routingObj != null && routingObj.size() > 0) {
            List<Node> nodeList = model.getNodes();
            RoutingMatrix P = new RoutingMatrix(model, jobClasses, nodeList);

            // Build reverse index maps
            Map<String, Node> nodeNameMap = new HashMap<String, Node>();
            for (Node n : nodeList) {
                nodeNameMap.put(n.getName(), n);
            }
            Map<String, JobClass> classNameMap = new HashMap<String, JobClass>();
            for (JobClass jc : jobClasses) {
                classNameMap.put(jc.getName(), jc);
            }

            for (Map.Entry<String, JsonElement> rtEntry : routingObj.entrySet()) {
                String key = rtEntry.getKey();
                String[] parts = key.split(",", 2);
                if (parts.length != 2) {
                    continue;
                }
                JobClass originClass = classNameMap.get(parts[0].trim());
                JobClass targetClass = classNameMap.get(parts[1].trim());
                if (originClass == null || targetClass == null) {
                    continue;
                }

                JsonObject fromToObj = rtEntry.getValue().getAsJsonObject();
                for (Map.Entry<String, JsonElement> fromEntry : fromToObj.entrySet()) {
                    Node srcNode = nodeNameMap.get(fromEntry.getKey());
                    if (srcNode == null) {
                        continue;
                    }
                    JsonObject destsObj = fromEntry.getValue().getAsJsonObject();
                    for (Map.Entry<String, JsonElement> destEntry : destsObj.entrySet()) {
                        Node destNode = nodeNameMap.get(destEntry.getKey());
                        if (destNode == null) {
                            continue;
                        }
                        double prob = destEntry.getValue().getAsDouble();
                        P.addConnection(srcNode, destNode, originClass, targetClass, prob);
                    }
                }
            }

            model.link(P);
        }

        // Phase 5: Restore non-PROB routing strategies and WRROBIN weights
        if (routingRaw != null) {
            JsonObject rsObj = null;
            JsonObject rwObj = null;
            // routingStrategies/routingWeights may be inside the routing object or at model level
            if (routingRaw.has("routingStrategies")) {
                rsObj = routingRaw.getAsJsonObject("routingStrategies");
            }
            if (routingRaw.has("routingWeights")) {
                rwObj = routingRaw.getAsJsonObject("routingWeights");
            }
            // Also check model level (MATLAB/Python save format)
            if (rsObj == null && modelObj.has("routingStrategies")) {
                rsObj = modelObj.getAsJsonObject("routingStrategies");
            }
            if (rwObj == null && modelObj.has("routingWeights")) {
                rwObj = modelObj.getAsJsonObject("routingWeights");
            }

            if (rsObj != null) {
                for (Map.Entry<String, JsonElement> nodeEntry : rsObj.entrySet()) {
                    Node node = nodeMap.get(nodeEntry.getKey());
                    if (node == null) continue;
                    JsonObject classStrats = nodeEntry.getValue().getAsJsonObject();
                    for (Map.Entry<String, JsonElement> classEntry : classStrats.entrySet()) {
                        JobClass jc = classMap.get(classEntry.getKey());
                        if (jc == null) continue;
                        String stratName = classEntry.getValue().getAsString();
                        RoutingStrategy rs = parseRoutingStrategy(stratName);
                        // Only apply non-default strategies: RAND and PROB are already
                        // handled by the probability routing matrix from model.link(P).
                        // WRROBIN is handled separately below with weights.
                        if (rs != null && rs != RoutingStrategy.WRROBIN
                                && rs != RoutingStrategy.RAND && rs != RoutingStrategy.PROB) {
                            node.setRouting(jc, rs);
                        }
                    }
                }
            }

            if (rwObj != null) {
                for (Map.Entry<String, JsonElement> nodeEntry : rwObj.entrySet()) {
                    Node node = nodeMap.get(nodeEntry.getKey());
                    if (node == null) continue;
                    JsonObject classWeights = nodeEntry.getValue().getAsJsonObject();
                    for (Map.Entry<String, JsonElement> classEntry : classWeights.entrySet()) {
                        JobClass jc = classMap.get(classEntry.getKey());
                        if (jc == null) continue;
                        JsonObject destWeights = classEntry.getValue().getAsJsonObject();
                        for (Map.Entry<String, JsonElement> destEntry : destWeights.entrySet()) {
                            Node dest = nodeMap.get(destEntry.getKey());
                            if (dest == null) continue;
                            double weight = destEntry.getValue().getAsDouble();
                            node.setRouting(jc, RoutingStrategy.WRROBIN, dest, weight);
                        }
                    }
                }
            }
        }

        // Finite capacity regions
        if (modelObj.has("finiteCapacityRegions")) {
            JsonArray fcrArr = modelObj.getAsJsonArray("finiteCapacityRegions");
            for (JsonElement fcrEl : fcrArr) {
                JsonObject rj = fcrEl.getAsJsonObject();
                // Collect region nodes from "stations" array or legacy "nodes" array
                List<Node> regionNodes = new ArrayList<Node>();
                if (rj.has("stations")) {
                    for (JsonElement stEl : rj.getAsJsonArray("stations")) {
                        JsonObject sj = stEl.getAsJsonObject();
                        String nodeName = sj.get("node").getAsString();
                        Node n = nodeMap.get(nodeName);
                        if (n != null) regionNodes.add(n);
                    }
                } else if (rj.has("nodes")) {
                    for (JsonElement nEl : rj.getAsJsonArray("nodes")) {
                        String nodeName = nEl.getAsString();
                        Node n = nodeMap.get(nodeName);
                        if (n != null) regionNodes.add(n);
                    }
                }
                if (!regionNodes.isEmpty()) {
                    Region region = model.addRegion(regionNodes);
                    if (rj.has("name")) {
                        region.setName(rj.get("name").getAsString());
                    }
                    if (rj.has("globalMaxJobs")) {
                        region.setGlobalMaxJobs(rj.get("globalMaxJobs").getAsInt());
                    }
                    if (rj.has("globalMaxMemory")) {
                        region.setGlobalMaxMemory(rj.get("globalMaxMemory").getAsInt());
                    }
                    if (rj.has("classMaxJobs")) {
                        JsonObject cmjObj = rj.getAsJsonObject("classMaxJobs");
                        for (Map.Entry<String, JsonElement> entry : cmjObj.entrySet()) {
                            JobClass jc = classMap.get(entry.getKey());
                            if (jc != null) {
                                region.setClassMaxJobs(jc, entry.getValue().getAsInt());
                            }
                        }
                    }
                    // Per-station classWeight and classSize
                    if (rj.has("stations")) {
                        for (JsonElement stEl : rj.getAsJsonArray("stations")) {
                            JsonObject sj = stEl.getAsJsonObject();
                            if (sj.has("classWeight")) {
                                JsonObject cwObj = sj.getAsJsonObject("classWeight");
                                for (Map.Entry<String, JsonElement> entry : cwObj.entrySet()) {
                                    JobClass jc = classMap.get(entry.getKey());
                                    if (jc != null) {
                                        region.setClassWeight(jc, entry.getValue().getAsDouble());
                                    }
                                }
                            }
                            if (sj.has("classSize")) {
                                JsonObject csObj = sj.getAsJsonObject("classSize");
                                for (Map.Entry<String, JsonElement> entry : csObj.entrySet()) {
                                    JobClass jc = classMap.get(entry.getKey());
                                    if (jc != null) {
                                        region.setClassSize(jc, entry.getValue().getAsInt());
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        return model;
    }

    // ========================================================================
    // LAYERED NETWORK LOAD
    // ========================================================================

    private static LayeredNetwork loadLayeredNetwork(JsonObject modelObj) throws IOException {
        String name = modelObj.has("name") ? modelObj.get("name").getAsString() : "model";
        LayeredNetwork model = new LayeredNetwork(name);

        // Create hosts (using Processor, which extends Host, so Task.on() accepts them)
        Map<String, Processor> hostMap = new LinkedHashMap<String, Processor>();
        if (modelObj.has("hosts")) {
            JsonArray hostsArr = modelObj.getAsJsonArray("hosts");
            for (JsonElement hostEl : hostsArr) {
                JsonObject hostObj = hostEl.getAsJsonObject();
                String hostName = hostObj.get("name").getAsString();
                int mult = hostObj.has("multiplicity") ? hostObj.get("multiplicity").getAsInt() : 1;
                String schedStr = hostObj.has("scheduling") ? hostObj.get("scheduling").getAsString() : "PS";
                SchedStrategy sched = parseSchedStrategy(schedStr);
                double quantum = hostObj.has("quantum") ? hostObj.get("quantum").getAsDouble() : 0.01;
                double speedFactor = hostObj.has("speedFactor") ? hostObj.get("speedFactor").getAsDouble() : 1.0;

                Processor host = new Processor(model, hostName, mult, sched, quantum, speedFactor);
                if (hostObj.has("replication")) {
                    host.setReplication(hostObj.get("replication").getAsInt());
                }
                hostMap.put(hostName, host);
            }
        }

        // Create tasks
        Map<String, Task> taskMap = new LinkedHashMap<String, Task>();
        if (modelObj.has("tasks")) {
            JsonArray tasksArr = modelObj.getAsJsonArray("tasks");
            for (JsonElement taskEl : tasksArr) {
                JsonObject taskObj = taskEl.getAsJsonObject();
                String taskName = taskObj.get("name").getAsString();
                int mult = taskObj.has("multiplicity") ? taskObj.get("multiplicity").getAsInt() : 1;
                String schedStr = taskObj.has("scheduling") ? taskObj.get("scheduling").getAsString() : "INF";
                SchedStrategy sched = parseSchedStrategy(schedStr);

                Task task;
                String taskType = taskObj.has("taskType") ? taskObj.get("taskType").getAsString() : "";
                if ("CacheTask".equals(taskType)) {
                    int totalItems = taskObj.has("totalItems") ? taskObj.get("totalItems").getAsInt() : 1;
                    int[] itemCap;
                    if (taskObj.has("cacheCapacity")) {
                        JsonArray capArr = taskObj.getAsJsonArray("cacheCapacity");
                        itemCap = new int[capArr.size()];
                        for (int ci = 0; ci < capArr.size(); ci++) {
                            itemCap[ci] = capArr.get(ci).getAsInt();
                        }
                    } else {
                        itemCap = new int[]{1};
                    }
                    String replStr = taskObj.has("replacementStrategy") ? taskObj.get("replacementStrategy").getAsString() : "FIFO";
                    ReplacementStrategy repl = parseReplacementStrategy(replStr);
                    task = new CacheTask(model, taskName, totalItems, itemCap, repl, mult, sched);
                } else if ("FunctionTask".equals(taskType)) {
                    task = new FunctionTask(model, taskName, mult, sched);
                } else {
                    task = new Task(model, taskName, mult, sched);
                }
                if (taskObj.has("host")) {
                    String hostName = taskObj.get("host").getAsString();
                    Processor host = hostMap.get(hostName);
                    if (host != null) {
                        task.on(host);
                    }
                }
                if (taskObj.has("replication")) {
                    task.setReplication(taskObj.get("replication").getAsInt());
                }
                if (taskObj.has("thinkTimeMean")) {
                    double thinkMean = taskObj.get("thinkTimeMean").getAsDouble();
                    task.setThinkTime(thinkMean);
                }
                if (taskObj.has("priority")) {
                    task.setPriority(taskObj.get("priority").getAsInt());
                }
                if (taskObj.has("fanIn")) {
                    JsonObject fanInObj = taskObj.getAsJsonObject("fanIn");
                    String src = fanInObj.get("source").getAsString();
                    int val = fanInObj.get("value").getAsInt();
                    task.setFanIn(src, val);
                }
                if (taskObj.has("fanOut")) {
                    JsonObject fanOutObj = taskObj.getAsJsonObject("fanOut");
                    for (Map.Entry<String, JsonElement> fo : fanOutObj.entrySet()) {
                        task.setFanOut(fo.getKey(), fo.getValue().getAsInt());
                    }
                }
                if (taskObj.has("setupTimeMean")) {
                    double setupMean = taskObj.get("setupTimeMean").getAsDouble();
                    task.setSetupTime(setupMean);
                }
                if (taskObj.has("delayOffTimeMean")) {
                    double delayOffMean = taskObj.get("delayOffTimeMean").getAsDouble();
                    task.setDelayOffTime(delayOffMean);
                }
                taskMap.put(taskName, task);
            }
        }

        // Create entries
        Map<String, Entry> entryMap = new LinkedHashMap<String, Entry>();
        if (modelObj.has("entries")) {
            JsonArray entriesArr = modelObj.getAsJsonArray("entries");
            for (JsonElement entryEl : entriesArr) {
                JsonObject entryObj = entryEl.getAsJsonObject();
                String entryName = entryObj.get("name").getAsString();
                Entry entry;
                boolean isItemEntry = entryObj.has("entryType") && "ItemEntry".equals(entryObj.get("entryType").getAsString());
                if (isItemEntry) {
                    int cardinality = entryObj.has("totalItems") ? entryObj.get("totalItems").getAsInt() : 1;
                    Distribution popDist = Immediate.getInstance();
                    if (entryObj.has("accessProb")) {
                        popDist = deserializeDistribution(entryObj.getAsJsonObject("accessProb"));
                    }
                    entry = new ItemEntry(model, entryName, cardinality, popDist);
                } else {
                    entry = new Entry(model, entryName);
                }
                if (entryObj.has("task")) {
                    String taskName = entryObj.get("task").getAsString();
                    Task task = taskMap.get(taskName);
                    if (task != null) {
                        entry.on(task);
                    }
                }
                if (entryObj.has("arrival")) {
                    Distribution arrDist = deserializeDistribution(entryObj.getAsJsonObject("arrival"));
                    entry.setArrival(arrDist);
                }
                if (entryObj.has("forwarding")) {
                    // Defer forwarding until all entries are created
                }
                entryMap.put(entryName, entry);
            }

            // Second pass: set forwarding
            for (JsonElement entryEl : entriesArr) {
                JsonObject entryObj = entryEl.getAsJsonObject();
                if (entryObj.has("forwarding")) {
                    String entryName = entryObj.get("name").getAsString();
                    Entry entry = entryMap.get(entryName);
                    if (entry != null) {
                        JsonArray fwArr = entryObj.getAsJsonArray("forwarding");
                        for (JsonElement fwEl : fwArr) {
                            JsonObject fwObj = fwEl.getAsJsonObject();
                            String destName = fwObj.get("dest").getAsString();
                            double prob = fwObj.has("prob") ? fwObj.get("prob").getAsDouble() : 1.0;
                            Entry destEntry = entryMap.get(destName);
                            if (destEntry != null) {
                                entry.forward(destEntry, prob);
                            } else {
                                entry.forward(destName, prob);
                            }
                        }
                    }
                }
            }
        }

        // Create activities
        Map<String, Activity> activityMap = new LinkedHashMap<String, Activity>();
        if (modelObj.has("activities")) {
            JsonArray actsArr = modelObj.getAsJsonArray("activities");
            for (JsonElement actEl : actsArr) {
                JsonObject actObj = actEl.getAsJsonObject();
                String actName = actObj.get("name").getAsString();

                Distribution hostDemand = Immediate.getInstance();
                if (actObj.has("hostDemand")) {
                    hostDemand = deserializeDistribution(actObj.getAsJsonObject("hostDemand"));
                }

                Activity activity = new Activity(model, actName, hostDemand);

                if (actObj.has("task")) {
                    String taskName = actObj.get("task").getAsString();
                    Task task = taskMap.get(taskName);
                    if (task != null) {
                        activity.on(task);
                    }
                }
                if (actObj.has("boundToEntry")) {
                    String entryName = actObj.get("boundToEntry").getAsString();
                    Entry entry = entryMap.get(entryName);
                    if (entry != null) {
                        activity.boundTo(entry);
                    } else {
                        activity.boundTo(entryName);
                    }
                }
                if (actObj.has("repliesTo")) {
                    String entryName = actObj.get("repliesTo").getAsString();
                    Entry entry = entryMap.get(entryName);
                    if (entry != null) {
                        activity.repliesTo(entry);
                    }
                }
                if (actObj.has("thinkTime")) {
                    Distribution thinkDist = deserializeDistribution(actObj.getAsJsonObject("thinkTime"));
                    activity.setThinkTime(thinkDist);
                }
                if (actObj.has("callOrder")) {
                    activity.setCallOrder(actObj.get("callOrder").getAsString());
                }
                if (actObj.has("synchCalls")) {
                    JsonArray syncArr = actObj.getAsJsonArray("synchCalls");
                    for (JsonElement callEl : syncArr) {
                        JsonObject callObj = callEl.getAsJsonObject();
                        String destName = callObj.get("dest").getAsString();
                        double mean = callObj.has("mean") ? callObj.get("mean").getAsDouble() : 1.0;
                        Entry destEntry = entryMap.get(destName);
                        if (destEntry != null) {
                            activity.synchCall(destEntry, mean);
                        } else {
                            activity.synchCall(destName, mean);
                        }
                    }
                }
                if (actObj.has("asynchCalls")) {
                    JsonArray asyncArr = actObj.getAsJsonArray("asynchCalls");
                    for (JsonElement callEl : asyncArr) {
                        JsonObject callObj = callEl.getAsJsonObject();
                        String destName = callObj.get("dest").getAsString();
                        double mean = callObj.has("mean") ? callObj.get("mean").getAsDouble() : 1.0;
                        Entry destEntry = entryMap.get(destName);
                        if (destEntry != null) {
                            activity.asynchCall(destEntry, mean);
                        } else {
                            activity.asynchCall(destName, mean);
                        }
                    }
                }
                activityMap.put(actName, activity);
            }
        }

        // Create precedences
        if (modelObj.has("precedences")) {
            JsonArray precsArr = modelObj.getAsJsonArray("precedences");
            for (JsonElement precEl : precsArr) {
                JsonObject precObj = precEl.getAsJsonObject();
                String taskName = precObj.get("task").getAsString();
                Task task = taskMap.get(taskName);
                if (task == null) {
                    continue;
                }

                JsonArray preActsArr = precObj.getAsJsonArray("preActs");
                JsonArray postActsArr = precObj.getAsJsonArray("postActs");

                List<String> preActs = new ArrayList<String>();
                for (JsonElement e : preActsArr) {
                    preActs.add(e.getAsString());
                }
                List<String> postActs = new ArrayList<String>();
                for (JsonElement e : postActsArr) {
                    postActs.add(e.getAsString());
                }

                String preType = precObj.has("preType") ? precObj.get("preType").getAsString() : ActivityPrecedenceType.PRE_SEQ;
                String postType = precObj.has("postType") ? precObj.get("postType").getAsString() : ActivityPrecedenceType.POST_SEQ;

                Matrix preParams = null;
                if (precObj.has("preParams")) {
                    preParams = jsonToRowVector(precObj.getAsJsonArray("preParams"));
                }
                // Default quorum for AND-join: all predecessors required
                if (preParams == null && preType.equals(ActivityPrecedenceType.PRE_AND)) {
                    preParams = Matrix.ones(1, preActs.size());
                }
                Matrix postParams = null;
                if (precObj.has("postParams")) {
                    postParams = jsonToRowVector(precObj.getAsJsonArray("postParams"));
                }

                ActivityPrecedence prec = new ActivityPrecedence(preActs, postActs, preType, postType, preParams, postParams);
                task.addPrecedence(prec);
            }
        }

        return model;
    }

    // ========================================================================
    // WORKFLOW LOAD
    // ========================================================================

    private static Workflow loadWorkflow(JsonObject modelObj) throws IOException {
        String name = modelObj.has("name") ? modelObj.get("name").getAsString() : "workflow";
        Workflow wf = new Workflow(name);

        if (modelObj.has("activities")) {
            JsonArray actsArr = modelObj.getAsJsonArray("activities");
            for (JsonElement actEl : actsArr) {
                JsonObject actObj = actEl.getAsJsonObject();
                String actName = actObj.get("name").getAsString();
                if (actObj.has("hostDemand")) {
                    Distribution hostDemand = deserializeDistribution(actObj.getAsJsonObject("hostDemand"));
                    wf.addActivity(actName, hostDemand);
                } else {
                    wf.addActivity(actName, 1.0);
                }
            }
        }

        if (modelObj.has("precedences")) {
            JsonArray precsArr = modelObj.getAsJsonArray("precedences");
            for (JsonElement precEl : precsArr) {
                JsonObject precObj = precEl.getAsJsonObject();
                List<String> preActs = new ArrayList<String>();
                for (JsonElement e : precObj.getAsJsonArray("preActs")) {
                    preActs.add(e.getAsString());
                }
                List<String> postActs = new ArrayList<String>();
                for (JsonElement e : precObj.getAsJsonArray("postActs")) {
                    postActs.add(e.getAsString());
                }
                String preType = precObj.has("preType") ? precObj.get("preType").getAsString() : ActivityPrecedenceType.PRE_SEQ;
                String postType = precObj.has("postType") ? precObj.get("postType").getAsString() : ActivityPrecedenceType.POST_SEQ;
                Matrix preParams = null;
                if (precObj.has("preParams")) {
                    preParams = jsonToRowVector(precObj.getAsJsonArray("preParams"));
                }
                Matrix postParams = null;
                if (precObj.has("postParams")) {
                    postParams = jsonToRowVector(precObj.getAsJsonArray("postParams"));
                }
                wf.addPrecedence(new ActivityPrecedence(preActs, postActs, preType, postType, preParams, postParams));
            }
        }

        return wf;
    }

    // ========================================================================
    // ENVIRONMENT LOAD
    // ========================================================================

    private static Environment loadEnvironment(JsonObject modelObj) throws IOException {
        String name = modelObj.has("name") ? modelObj.get("name").getAsString() : "env";
        int numStages = modelObj.has("numStages") ? modelObj.get("numStages").getAsInt() : 0;
        Environment env = new Environment(name, numStages);

        if (modelObj.has("stages")) {
            JsonArray stagesArr = modelObj.getAsJsonArray("stages");
            for (int i = 0; i < stagesArr.size(); i++) {
                JsonObject stageObj = stagesArr.get(i).getAsJsonObject();
                String stageName = stageObj.has("name") ? stageObj.get("name").getAsString() : "Stage" + i;
                String stageType = stageObj.has("type") ? stageObj.get("type").getAsString() : "";
                Network stageModel = null;
                if (stageObj.has("model")) {
                    stageModel = loadNetwork(stageObj.getAsJsonObject("model"));
                }
                if (stageModel != null) {
                    env.addStage(i, stageName, stageType, stageModel);
                }
            }
        }

        if (modelObj.has("transitions")) {
            JsonArray transArr = modelObj.getAsJsonArray("transitions");
            for (JsonElement transEl : transArr) {
                JsonObject transObj = transEl.getAsJsonObject();
                int from = transObj.get("from").getAsInt();
                int to = transObj.get("to").getAsInt();
                Distribution dist = deserializeDistribution(transObj.getAsJsonObject("distribution"));
                if (dist instanceof Markovian) {
                    env.addTransition(from, to, (Markovian) dist);
                }
            }
        }

        env.init();
        return env;
    }

    // ========================================================================
    // MATRIX <-> JSON CONVERSION UTILITIES
    // ========================================================================

    /**
     * Converts a Matrix to a JSON array. If the matrix is a row vector (1 row),
     * returns a flat array. Otherwise returns a 2D array.
     */
    private static JsonArray matrixToJsonArray(Matrix m) {
        if (m == null) {
            return new JsonArray();
        }
        int rows = m.getNumRows();
        int cols = m.getNumCols();

        JsonArray arr = new JsonArray();
        if (rows == 1) {
            // Row vector: flat array
            for (int j = 0; j < cols; j++) {
                arr.add(m.get(0, j));
            }
        } else if (cols == 1) {
            // Column vector: flat array
            for (int i = 0; i < rows; i++) {
                arr.add(m.get(i, 0));
            }
        } else {
            // 2D matrix
            for (int i = 0; i < rows; i++) {
                JsonArray row = new JsonArray();
                for (int j = 0; j < cols; j++) {
                    row.add(m.get(i, j));
                }
                arr.add(row);
            }
        }
        return arr;
    }

    /**
     * Converts a Matrix to a 2D JSON array (always nested, even for 1x1).
     */
    private static JsonArray matrixToJson2D(Matrix m) {
        JsonArray arr = new JsonArray();
        if (m == null) {
            return arr;
        }
        int rows = m.getNumRows();
        int cols = m.getNumCols();
        for (int i = 0; i < rows; i++) {
            JsonArray row = new JsonArray();
            for (int j = 0; j < cols; j++) {
                row.add(m.get(i, j));
            }
            arr.add(row);
        }
        return arr;
    }

    /**
     * Converts a row-vector Matrix to a flat JSON array.
     */
    private static JsonArray matrixToJsonRowVector(Matrix m) {
        JsonArray arr = new JsonArray();
        if (m == null) {
            return arr;
        }
        // Treat as 1D regardless of shape
        int len = m.getNumCols();
        if (m.getNumRows() > 1 && m.getNumCols() == 1) {
            // Column vector
            len = m.getNumRows();
            for (int i = 0; i < len; i++) {
                arr.add(m.get(i, 0));
            }
        } else {
            for (int j = 0; j < len; j++) {
                arr.add(m.get(0, j));
            }
        }
        return arr;
    }

    /**
     * Converts a 2D JSON array to a Matrix.
     */
    private static Matrix jsonToMatrix2D(JsonArray arr) {
        if (arr == null || arr.size() == 0) {
            return new Matrix(0, 0);
        }
        int rows = arr.size();
        JsonArray firstRow = arr.get(0).getAsJsonArray();
        int cols = firstRow.size();
        Matrix m = new Matrix(rows, cols);
        for (int i = 0; i < rows; i++) {
            JsonArray row = arr.get(i).getAsJsonArray();
            for (int j = 0; j < cols; j++) {
                m.set(i, j, row.get(j).getAsDouble());
            }
        }
        return m;
    }

    /**
     * Converts a flat JSON array to a row-vector Matrix (1 x n).
     */
    private static Matrix jsonToRowVector(JsonArray arr) {
        if (arr == null || arr.size() == 0) {
            return new Matrix(1, 0);
        }
        // Check if this is a nested 2D array or flat
        if (arr.get(0).isJsonArray()) {
            // 2D: return first row as vector (or full matrix if multi-row)
            return jsonToMatrix2D(arr);
        }
        int n = arr.size();
        Matrix m = new Matrix(1, n);
        for (int j = 0; j < n; j++) {
            m.set(0, j, arr.get(j).getAsDouble());
        }
        return m;
    }

    // ========================================================================
    // ENUM PARSING UTILITIES
    // ========================================================================

    /**
     * Parses a scheduling strategy string to a {@link SchedStrategy} enum value.
     */
    private static SchedStrategy parseSchedStrategy(String str) {
        if (str == null || str.isEmpty()) {
            return SchedStrategy.PS;
        }
        // Try direct enum match first
        try {
            return SchedStrategy.valueOf(str);
        } catch (IllegalArgumentException e) {
            // Fall through to manual matching
        }
        // Try common string representations
        String upper = str.toUpperCase();
        if ("FCFS".equals(upper) || "FIFO".equals(upper)) return SchedStrategy.FCFS;
        if ("LCFS".equals(upper) || "LIFO".equals(upper)) return SchedStrategy.LCFS;
        if ("PS".equals(upper)) return SchedStrategy.PS;
        if ("INF".equals(upper)) return SchedStrategy.INF;
        if ("SIRO".equals(upper) || "RAND".equals(upper)) return SchedStrategy.SIRO;
        if ("HOL".equals(upper)) return SchedStrategy.HOL;
        if ("DPS".equals(upper)) return SchedStrategy.DPS;
        if ("GPS".equals(upper)) return SchedStrategy.GPS;
        if ("SEPT".equals(upper)) return SchedStrategy.SEPT;
        if ("LEPT".equals(upper)) return SchedStrategy.LEPT;
        if ("SJF".equals(upper)) return SchedStrategy.SJF;
        if ("LJF".equals(upper)) return SchedStrategy.LJF;
        if ("LCFSPR".equals(upper)) return SchedStrategy.LCFSPR;
        if ("FCFSPRIO".equals(upper)) return SchedStrategy.FCFSPRIO;
        if ("PSPRIO".equals(upper)) return SchedStrategy.PSPRIO;
        if ("DPSPRIO".equals(upper)) return SchedStrategy.DPSPRIO;
        if ("GPSPRIO".equals(upper)) return SchedStrategy.GPSPRIO;
        if ("REF".equals(upper)) return SchedStrategy.REF;
        if ("EXT".equals(upper)) return SchedStrategy.EXT;
        if ("FORK".equals(upper)) return SchedStrategy.FORK;
        if ("POLLING".equals(upper)) return SchedStrategy.POLLING;
        if ("LPS".equals(upper)) return SchedStrategy.LPS;
        // Default
        return SchedStrategy.PS;
    }

    /**
     * Parses a replacement strategy string to a {@link ReplacementStrategy} enum value.
     */
    private static ReplacementStrategy parseReplacementStrategy(String str) {
        if (str == null || str.isEmpty()) {
            return ReplacementStrategy.LRU;
        }
        try {
            return ReplacementStrategy.valueOf(str.toUpperCase());
        } catch (IllegalArgumentException e) {
            // Fall through
        }
        String upper = str.toUpperCase();
        if ("LRU".equals(upper)) return ReplacementStrategy.LRU;
        if ("FIFO".equals(upper)) return ReplacementStrategy.FIFO;
        if ("RANDOM".equals(upper) || "RR".equals(upper)) return ReplacementStrategy.RR;
        return ReplacementStrategy.LRU;
    }

    /**
     * Parses a routing strategy string to a {@link RoutingStrategy} enum value.
     */
    private static RoutingStrategy parseRoutingStrategy(String str) {
        if (str == null || str.isEmpty()) {
            return null;
        }
        try {
            return RoutingStrategy.valueOf(str);
        } catch (IllegalArgumentException e) {
            // Fall through to manual matching
        }
        String upper = str.toUpperCase();
        if ("RAND".equals(upper)) return RoutingStrategy.RAND;
        if ("PROB".equals(upper)) return RoutingStrategy.PROB;
        if ("RROBIN".equals(upper)) return RoutingStrategy.RROBIN;
        if ("WRROBIN".equals(upper)) return RoutingStrategy.WRROBIN;
        if ("JSQ".equals(upper)) return RoutingStrategy.JSQ;
        if ("KCHOICES".equals(upper)) return RoutingStrategy.KCHOICES;
        if ("DISABLED".equals(upper)) return RoutingStrategy.DISABLED;
        if ("FIRING".equals(upper)) return RoutingStrategy.FIRING;
        if ("RL".equals(upper)) return RoutingStrategy.RL;
        return null;
    }

    /**
     * Converts a DropStrategy enum to a schema-compatible string.
     */
    private static String dropStrategyToStr(DropStrategy ds) {
        if (ds == DropStrategy.Drop) return "drop";
        if (ds == DropStrategy.WaitingQueue) return "waitingQueue";
        if (ds == DropStrategy.BlockingAfterService) return "blockingAfterService";
        if (ds == DropStrategy.Retrial) return "retrial";
        if (ds == DropStrategy.RetrialWithLimit) return "retrialWithLimit";
        return "waitingQueue";
    }

    /**
     * Parses a drop strategy string to a DropStrategy enum value.
     */
    private static DropStrategy parseDropStrategy(String str) {
        if (str == null || str.isEmpty()) return DropStrategy.WaitingQueue;
        if ("drop".equals(str)) return DropStrategy.Drop;
        if ("waitingQueue".equals(str)) return DropStrategy.WaitingQueue;
        if ("blockingAfterService".equals(str)) return DropStrategy.BlockingAfterService;
        if ("retrial".equals(str)) return DropStrategy.Retrial;
        if ("retrialWithLimit".equals(str)) return DropStrategy.RetrialWithLimit;
        return DropStrategy.WaitingQueue;
    }
}
