function linemodel_save(model, filename)
% LINEMODEL_SAVE Save a LINE model to JSON.
%
%   LINEMODEL_SAVE(MODEL, FILENAME) saves the model to the specified JSON
%   file, conforming to the line-model.schema.json specification.
%
% Parameters:
%   model    - Network, LayeredNetwork, Workflow, or Environment object
%   filename - output file path (should end in .json)
%
% Example:
%   model = Network('M/M/1');
%   source = Source(model, 'Source');
%   queue  = Queue(model, 'Queue', SchedStrategy.FCFS);
%   sink   = Sink(model, 'Sink');
%   oclass = OpenClass(model, 'Class1');
%   source.setArrival(oclass, Exp(1.0));
%   queue.setService(oclass, Exp(2.0));
%   P = model.initRoutingMatrix();
%   P{1}(1,2) = 1; P{1}(2,3) = 1;
%   model.link(P);
%   linemodel_save(model, 'mm1.json');
%
% Copyright (c) 2012-2026, Imperial College London
% All rights reserved.

if isa(model, 'LayeredNetwork')
    modelMap = layered2json(model);
elseif isa(model, 'Workflow')
    modelMap = workflow2json(model);
elseif isa(model, 'Environment')
    modelMap = environment2json(model);
else
    modelMap = network2json(model);
end

% Build the full document
sb = {};
sb{end+1} = '{';
sb{end+1} = '  "format": "line-model",';
sb{end+1} = '  "version": "1.0",';
sb{end+1} = ['  "model": ', encode_value(modelMap, 2)];
sb{end+1} = '}';
jsonStr = strjoin(sb, newline);

fid = fopen(filename, 'w');
if fid == -1
    error('linemodel_save:fileOpen', 'Cannot open file: %s', filename);
end
cleanupObj = onCleanup(@() fclose(fid));
fprintf(fid, '%s\n', jsonStr);
end


% =========================================================================
%  Network serialization
% =========================================================================

function result = network2json(model)
% Convert a Network to a containers.Map (preserves key ordering/commas)
result = containers.Map();
result('type') = 'Network';
result('name') = model.getName();

nodes = model.getNodes();
classes = model.getClasses();
K = length(classes);
M = length(nodes);

% --- Nodes ---
nodesJson = {};
for i = 1:M
    node = nodes{i};

    % Skip implicit ClassSwitch nodes (auto-created by link())
    if isa(node, 'ClassSwitch') && isprop(node, 'autoAdded') && node.autoAdded
        continue;
    end

    nj = containers.Map();
    nj('name') = node.name;
    nj('type') = node_type_str(node);

    % Scheduling
    if isa(node, 'Delay')
        nj('scheduling') = 'INF';
    elseif isa(node, 'Queue')
        sched = node.schedStrategy;
        if ~isempty(sched)
            nj('scheduling') = sched_id_to_str(sched);
        end
    end

    % Servers
    if isa(node, 'Queue') && ~isa(node, 'Delay')
        ns = node.numberOfServers;
        if isfinite(ns) && ns > 1
            nj('servers') = ns;
        end
    end

    % Buffer
    if isa(node, 'Queue')
        c = node.cap;
        if ~isempty(c) && isfinite(c) && c > 0
            nj('buffer') = c;
        end
    end

    % Per-class buffer capacity
    if isa(node, 'Queue') && ~isempty(node.classCap)
        ccMap = containers.Map();
        for r = 1:K
            jc = classes{r};
            if r <= length(node.classCap) && isfinite(node.classCap(r))
                ccMap(jc.name) = node.classCap(r);
            end
        end
        if ccMap.Count > 0
            nj('classCap') = ccMap;
        end
    end

    % Drop rules
    if isa(node, 'Queue') && ~isempty(node.dropRule)
        drMap = containers.Map();
        for r = 1:K
            jc = classes{r};
            if r <= length(node.dropRule)
                dr = node.dropRule(r);
                drStr = droprule_to_str(dr);
                if ~isempty(drStr)
                    drMap(jc.name) = drStr;
                end
            end
        end
        if drMap.Count > 0
            nj('dropRule') = drMap;
        end
    end

    % Load-dependent scaling
    if isa(node, 'Station') && ~isempty(node.lldScaling)
        ldMap = containers.Map();
        ldMap('type') = 'loadDependent';
        ldMap('scaling') = node.lldScaling(:)';
        nj('loadDependence') = ldMap;
    end

    % Service / arrival distributions
    svc = containers.Map();
    for r = 1:K
        jc = classes{r};
        dist = [];
        if isa(node, 'Source')
            try
                dist = node.getArrivalProcess(jc);
            catch
                dist = [];
            end
        elseif isa(node, 'Queue') || isa(node, 'Delay')
            try
                dist = node.getService(jc);
            catch
                dist = [];
            end
        end
        if ~isempty(dist) && ~isa(dist, 'Disabled')
            dj = dist2json(dist);
            if ~isempty(dj)
                svc(jc.name) = dj;
            end
        end
    end
    if svc.Count > 0
        nj('service') = svc;
    end

    % ClassSwitch matrix
    if isa(node, 'ClassSwitch')
        csm = node.server.csMatrix;
        if ~isempty(csm)
            csDict = containers.Map();
            for ri = 1:K
                row = containers.Map();
                for ci = 1:K
                    if ri <= size(csm,1) && ci <= size(csm,2) && csm(ri,ci) ~= 0
                        row(classes{ci}.name) = csm(ri,ci);
                    end
                end
                if row.Count > 0
                    csDict(classes{ri}.name) = row;
                end
            end
            if csDict.Count > 0
                nj('classSwitchMatrix') = csDict;
            end
        end
    end

    % Cache config
    if isa(node, 'Cache')
        cc = containers.Map();
        cc('items') = node.items.nitems;
        ilc = node.itemLevelCap;
        if isscalar(ilc)
            cc('capacity') = ilc;
        else
            cc('capacity') = ilc(:)';
        end
        cc('replacement') = repl_to_str(node.replacestrategy);

        % Hit/miss class mappings
        hc = full(node.server.hitClass);
        mc = full(node.server.missClass);
        if ~isempty(hc) && any(hc > 0)
            hitMap = containers.Map();
            for hi = 1:length(hc)
                if hc(hi) > 0 && hi <= K && hc(hi) <= K
                    hitMap(classes{hi}.name) = classes{hc(hi)}.name;
                end
            end
            if hitMap.Count > 0
                cc('hitClass') = hitMap;
            end
        end
        if ~isempty(mc) && any(mc > 0)
            missMap = containers.Map();
            for mi = 1:length(mc)
                if mc(mi) > 0 && mi <= K && mc(mi) <= K
                    missMap(classes{mi}.name) = classes{mc(mi)}.name;
                end
            end
            if missMap.Count > 0
                cc('missClass') = missMap;
            end
        end

        % Read popularity distributions (setRead)
        if ~isempty(node.popularity)
            popMap = containers.Map();
            for pi = 1:size(node.popularity, 1)
                for pj = 1:size(node.popularity, 2)
                    if pi <= size(node.popularity, 1) && pj <= size(node.popularity, 2) ...
                            && ~isempty(node.popularity{pi, pj})
                        popDist = node.popularity{pi, pj};
                        dj = dist2json(popDist);
                        if ~isempty(dj) && pj <= K
                            popMap(classes{pj}.name) = dj;
                        end
                    end
                end
            end
            if popMap.Count > 0
                cc('popularity') = popMap;
            end
        end

        nj('cache') = cc;
    end

    % Fork tasksPerLink
    if isa(node, 'Fork')
        if ~isempty(node.output) && isprop(node.output, 'tasksPerLink') && node.output.tasksPerLink > 1
            nj('tasksPerLink') = node.output.tasksPerLink;
        end
    end

    % Join paired fork and join strategy
    if isa(node, 'Join')
        if ~isempty(node.joinOf)
            nj('forkNode') = node.joinOf.name;
        end
        % Serialize per-class join strategy if non-default
        if ~isempty(node.input) && isprop(node.input, 'joinStrategy') && ~isempty(node.input.joinStrategy)
            for r = 1:K
                jc = classes{r};
                if r <= length(node.input.joinStrategy) && ~isempty(node.input.joinStrategy{r})
                    js = node.input.joinStrategy{r};
                    if js ~= JoinStrategy.STD
                        if js == JoinStrategy.PARTIAL
                            nj('joinStrategy') = 'PARTIAL';
                        end
                    end
                end
            end
        end
        if ~isempty(node.input) && isprop(node.input, 'joinRequired') && ~isempty(node.input.joinRequired)
            for r = 1:K
                jc = classes{r};
                if r <= length(node.input.joinRequired) && ~isempty(node.input.joinRequired{r})
                    jq = node.input.joinRequired{r};
                    if jq > 0
                        nj('joinQuorum') = jq;
                    end
                end
            end
        end
    end

    % DPS scheduling parameters (weights per class)
    if isa(node, 'Queue') && ~isa(node, 'Delay')
        sched = node.schedStrategy;
        if ~isempty(sched) && (sched == SchedStrategy.DPS || sched == SchedStrategy.GPS)
            sp = containers.Map();
            for r = 1:K
                jc = classes{r};
                try
                    w = node.schedStrategyPar(r);
                    if ~isempty(w) && isfinite(w) && w > 0
                        sp(jc.name) = w;
                    end
                catch
                end
            end
            if sp.Count > 0
                nj('schedParams') = sp;
            end
        end
    end

    % Transition modes
    if isa(node, 'Transition')
        modesJson = {};
        nModes = node.getNumberOfModes();
        allNodes = model.getNodes();
        for mi = 1:nModes
            mj = containers.Map();
            if mi <= length(node.modeNames) && ~isempty(node.modeNames{mi})
                mj('name') = node.modeNames{mi};
            else
                mj('name') = sprintf('Mode%d', mi);
            end
            % Distribution
            if mi <= length(node.distributions) && ~isempty(node.distributions{mi})
                dj = dist2json(node.distributions{mi});
                if ~isempty(dj)
                    mj('distribution') = dj;
                end
            end
            % Timing strategy
            if mi <= length(node.timingStrategies)
                if node.timingStrategies(mi) == TimingStrategy.TIMED
                    mj('timingStrategy') = 'TIMED';
                else
                    mj('timingStrategy') = 'IMMEDIATE';
                end
            end
            % Number of servers
            if mi <= length(node.numberOfServers) && node.numberOfServers(mi) > 1
                mj('numServers') = node.numberOfServers(mi);
            end
            % Firing priority
            if mi <= length(node.firingPriorities) && node.firingPriorities(mi) > 0
                mj('firingPriority') = node.firingPriorities(mi);
            end
            % Firing weight
            if mi <= length(node.firingWeights) && node.firingWeights(mi) ~= 1.0
                mj('firingWeight') = node.firingWeights(mi);
            end
            % Enabling conditions
            if mi <= length(node.enablingConditions)
                ecMat = node.enablingConditions{mi};
                ecList = {};
                for ni = 1:size(ecMat, 1)
                    for ci = 1:size(ecMat, 2)
                        if ecMat(ni, ci) > 0
                            ec = containers.Map();
                            ec('node') = allNodes{ni}.name;
                            ec('class') = classes{ci}.name;
                            ec('count') = ecMat(ni, ci);
                            ecList{end+1} = ec; %#ok<AGROW>
                        end
                    end
                end
                if ~isempty(ecList)
                    mj('enablingConditions') = ecList;
                end
            end
            % Inhibiting conditions
            if mi <= length(node.inhibitingConditions)
                icMat = node.inhibitingConditions{mi};
                icList = {};
                for ni = 1:size(icMat, 1)
                    for ci = 1:size(icMat, 2)
                        if isfinite(icMat(ni, ci))
                            ic = containers.Map();
                            ic('node') = allNodes{ni}.name;
                            ic('class') = classes{ci}.name;
                            ic('count') = icMat(ni, ci);
                            icList{end+1} = ic; %#ok<AGROW>
                        end
                    end
                end
                if ~isempty(icList)
                    mj('inhibitingConditions') = icList;
                end
            end
            % Firing outcomes
            if mi <= length(node.firingOutcomes)
                foMat = node.firingOutcomes{mi};
                foList = {};
                for ni = 1:size(foMat, 1)
                    for ci = 1:size(foMat, 2)
                        if foMat(ni, ci) ~= 0
                            fo = containers.Map();
                            fo('node') = allNodes{ni}.name;
                            fo('class') = classes{ci}.name;
                            fo('count') = foMat(ni, ci);
                            foList{end+1} = fo; %#ok<AGROW>
                        end
                    end
                end
                if ~isempty(foList)
                    mj('firingOutcomes') = foList;
                end
            end
            modesJson{end+1} = mj; %#ok<AGROW>
        end
        if ~isempty(modesJson)
            nj('modes') = modesJson;
        end
    end

    % Initial state for Place nodes (token counts)
    if isa(node, 'Place') && ~isempty(node.state)
        nj('initialState') = node.state(:)';
    end

    nodesJson{end+1} = nj; %#ok<AGROW>
end
result('nodes') = nodesJson;

% --- Classes ---
classesJson = {};
for r = 1:K
    jc = classes{r};
    cj = containers.Map();
    cj('name') = jc.name;
    if isa(jc, 'OpenSignal')
        cj('type') = 'Signal';
        cj('openOrClosed') = 'Open';
        cj('signalType') = SignalType.toText(jc.signalType);
        if ~isempty(jc.targetJobClass)
            cj('targetClass') = jc.targetJobClass.name;
        end
        if ~isempty(jc.removalDistribution)
            cj('removalDistribution') = dist2json(jc.removalDistribution);
        end
        if ~isempty(jc.removalPolicy) && jc.removalPolicy ~= RemovalPolicy.RANDOM
            cj('removalPolicy') = RemovalPolicy.toText(jc.removalPolicy);
        end
    elseif isa(jc, 'ClosedSignal')
        cj('type') = 'Signal';
        cj('openOrClosed') = 'Closed';
        cj('signalType') = SignalType.toText(jc.signalType);
        if ~isempty(jc.refstat) && isprop(jc.refstat, 'name')
            cj('refNode') = jc.refstat.name;
        end
        if ~isempty(jc.targetJobClass)
            cj('targetClass') = jc.targetJobClass.name;
        end
        if ~isempty(jc.removalDistribution)
            cj('removalDistribution') = dist2json(jc.removalDistribution);
        end
        if ~isempty(jc.removalPolicy) && jc.removalPolicy ~= RemovalPolicy.RANDOM
            cj('removalPolicy') = RemovalPolicy.toText(jc.removalPolicy);
        end
    elseif isa(jc, 'OpenClass')
        cj('type') = 'Open';
    elseif isa(jc, 'ClosedClass')
        cj('type') = 'Closed';
        cj('population') = jc.population;
        if ~isempty(jc.refstat) && isprop(jc.refstat, 'name')
            cj('refNode') = jc.refstat.name;
        end
    else
        cj('type') = 'Open';
    end
    if jc.priority ~= 0
        cj('priority') = jc.priority;
    end
    if isprop(jc, 'deadline') && isfinite(jc.deadline)
        cj('deadline') = jc.deadline;
    end
    classesJson{end+1} = cj; %#ok<AGROW>
end
result('classes') = classesJson;

% --- Routing ---
routingMap = containers.Map();
try
    sn = model.getStruct();
    % Prefer rtorig (original P matrix before ClassSwitch expansion)
    if ~isempty(sn) && isfield(sn, 'rtorig') && iscell(sn.rtorig) && ~isempty(sn.rtorig) && ~isempty(sn.rtorig{1,1})
        P_orig = sn.rtorig;
        M_orig = size(P_orig{1,1}, 1);
        % Identify explicit ClassSwitch node indices (not auto-added)
        nodes = model.getNodes();
        explicit_cs = false(1, M_orig);
        for ii = 1:min(M_orig, length(nodes))
            if isa(nodes{ii}, 'ClassSwitch') && ~nodes{ii}.autoAdded
                explicit_cs(ii) = true;
            end
        end
        % For explicit CS sources, compute same-class routing:
        % P_same(s,ii,jj) = sum_r P_orig{r,s}(ii,jj)
        % This avoids saving cross-class entries that would cause
        % double-switching on load.
        cs_same = zeros(K, M_orig, M_orig);
        for ii = 1:M_orig
            if explicit_cs(ii)
                for s = 1:K
                    for jj = 1:M_orig
                        total = 0;
                        for r = 1:K
                            Prs = P_orig{r,s};
                            if issparse(Prs); Prs = full(Prs); end
                            total = total + Prs(ii, jj);
                        end
                        cs_same(s, ii, jj) = total;
                    end
                end
            end
        end
        for r = 1:K
            for s = 1:K
                fromTo = containers.Map();
                Prs = P_orig{r,s};
                if issparse(Prs)
                    Prs = full(Prs);
                end
                for ii = 1:M_orig
                    if explicit_cs(ii)
                        % For explicit CS, use same-class routing only
                        if r == s
                            for jj = 1:M_orig
                                val = cs_same(s, ii, jj);
                                if val > 1e-14
                                    ni = sn.nodenames{ii};
                                    njn = sn.nodenames{jj};
                                    if ~fromTo.isKey(ni)
                                        fromTo(ni) = containers.Map();
                                    end
                                    dest = fromTo(ni);
                                    dest(njn) = val;
                                    fromTo(ni) = dest;
                                end
                            end
                        end
                        % Skip cross-class entries from explicit CS
                        continue;
                    end
                    for jj = 1:M_orig
                        val = Prs(ii, jj);
                        if val > 1e-14
                            ni = sn.nodenames{ii};
                            njn = sn.nodenames{jj};
                            if ~fromTo.isKey(ni)
                                fromTo(ni) = containers.Map();
                            end
                            dest = fromTo(ni);
                            dest(njn) = val;
                            fromTo(ni) = dest;
                        end
                    end
                end
                if fromTo.Count > 0
                    key = char(sprintf('%s,%s', classes{r}.name, classes{s}.name));
                    routingMap(key) = fromTo;
                end
            end
        end
    elseif ~isempty(sn) && isfield(sn, 'rtnodes') && ~isempty(sn.rtnodes)
        % Fallback to rtnodes if rtorig not available
        rt = sn.rtnodes;
        N = sn.nnodes;
        for r = 1:K
            for s = 1:K
                fromTo = containers.Map();
                for ii = 1:N
                    for jj = 1:N
                        val = rt((ii-1)*K+r, (jj-1)*K+s);
                        if val > 1e-14
                            ni = sn.nodenames{ii};
                            njn = sn.nodenames{jj};
                            if ~fromTo.isKey(ni)
                                fromTo(ni) = containers.Map();
                            end
                            dest = fromTo(ni);
                            dest(njn) = val;
                            fromTo(ni) = dest;
                        end
                    end
                end
                if fromTo.Count > 0
                    key = char(sprintf('%s,%s', classes{r}.name, classes{s}.name));
                    routingMap(key) = fromTo;
                end
            end
        end
    end
catch
    % If struct not available, routing stays empty
end

routing = containers.Map();
routing('type') = 'matrix';
routing('matrix') = routingMap;
result('routing') = routing;

% --- Routing Strategies ---
try
    sn2 = model.getStruct();
    if ~isempty(sn2) && isfield(sn2, 'routing') && ~isempty(sn2.routing)
        routingStrategies = containers.Map();
        stratNames = containers.Map('KeyType','int32','ValueType','char');
        stratNames(int32(RoutingStrategy.RAND)) = 'RAND';
        stratNames(int32(RoutingStrategy.RROBIN)) = 'RROBIN';
        stratNames(int32(RoutingStrategy.WRROBIN)) = 'WRROBIN';
        stratNames(int32(RoutingStrategy.JSQ)) = 'JSQ';
        stratNames(int32(RoutingStrategy.KCHOICES)) = 'KCHOICES';
        stratNames(int32(RoutingStrategy.FIRING)) = 'FIRING';
        stratNames(int32(RoutingStrategy.RL)) = 'RL';
        stratNames(int32(RoutingStrategy.DISABLED)) = 'DISABLED';
        for i = 1:sn2.nnodes
            nodeStrats = containers.Map();
            for r = 1:K
                routVal = int32(sn2.routing(i, r));
                if routVal ~= int32(RoutingStrategy.PROB) && routVal ~= int32(RoutingStrategy.RAND) && stratNames.isKey(routVal)
                    nodeStrats(classes{r}.name) = stratNames(routVal);
                end
            end
            if nodeStrats.Count > 0
                routingStrategies(sn2.nodenames{i}) = nodeStrats;
            end
        end
        if routingStrategies.Count > 0
            result('routingStrategies') = routingStrategies;
        end

        % Save WRROBIN weights
        routingWeights = containers.Map();
        for i = 1:sn2.nnodes
            stationIdx = sn2.nodeToStation(i);
            if stationIdx < 1; continue; end
            nodeObj2 = nodes{stationIdx};
            nodeClassWeights = containers.Map();
            for r = 1:K
                if int32(sn2.routing(i, r)) == int32(RoutingStrategy.WRROBIN)
                    os = nodeObj2.output.outputStrategy;
                    if size(os,2) >= r
                        osEntry = os{1, r};
                        % osEntry = {className, stratName, forwardLinks}
                        if length(osEntry) >= 3
                            fwdLinks = osEntry{3};
                            destWeights = containers.Map();
                            for fi = 1:length(fwdLinks)
                                link = fwdLinks{fi};
                                % link = {destNode, weight}
                                if iscell(link) && length(link) >= 2 && isa(link{1}, 'Node')
                                    destWeights(link{1}.name) = link{2};
                                end
                            end
                            if destWeights.Count > 0
                                nodeClassWeights(classes{r}.name) = destWeights;
                            end
                        end
                    end
                end
            end
            if nodeClassWeights.Count > 0
                routingWeights(sn2.nodenames{i}) = nodeClassWeights;
            end
        end
        if routingWeights.Count > 0
            result('routingWeights') = routingWeights;
        end
    end
catch
end

% --- Switchover Times ---
try
    nodesCellTmp = result('nodes');
    for i = 1:M
        nodeObj = nodes{i};
        if isa(nodeObj, 'Queue')
            soTimes = {};
            if isprop(nodeObj, 'switchoverTimes') && ~isempty(nodeObj.switchoverTimes)
                for r = 1:K
                    for s = 1:K
                        if r ~= s
                            dist = [];
                            try
                                dist = nodeObj.getSwitchover(classes{r}, classes{s});
                            catch
                            end
                            if ~isempty(dist) && ~isa(dist, 'Disabled')
                                so = containers.Map();
                                so('from') = classes{r}.name;
                                so('to') = classes{s}.name;
                                so('distribution') = dist2json(dist);
                                soTimes{end+1} = so;
                            end
                        end
                    end
                end
            end
            if ~isempty(soTimes)
                for nj_idx = 1:length(nodesCellTmp)
                    nj = nodesCellTmp{nj_idx};
                    if strcmp(nj('name'), nodeObj.name)
                        nj('switchoverTimes') = soTimes;
                        nodesCellTmp{nj_idx} = nj;
                        break;
                    end
                end
            end
        end
    end
    result('nodes') = nodesCellTmp;
catch
end

% --- Heterogeneous Server Types ---
try
    nodesCellTmp = result('nodes');
    for i = 1:M
        nodeObj = nodes{i};
        if isa(nodeObj, 'Queue') && nodeObj.isHeterogeneous()
            stArr = {};
            for ti = 1:length(nodeObj.serverTypes)
                st = nodeObj.serverTypes{ti};
                stj = containers.Map();
                stj('name') = st.name;
                stj('count') = st.numOfServers;
                % Compatible classes
                ccNames = {};
                for cci = 1:length(st.compatibleClasses)
                    ccNames{end+1} = st.compatibleClasses{cci}.name; %#ok<AGROW>
                end
                if ~isempty(ccNames)
                    stj('compatibleClasses') = ccNames;
                end
                % Per-class service distributions
                svcMap = containers.Map();
                for r = 1:K
                    jc = classes{r};
                    dist = nodeObj.getHeteroService(jc, st);
                    if ~isempty(dist) && ~isa(dist, 'Disabled')
                        svcMap(jc.name) = dist2json(dist);
                    end
                end
                if svcMap.Count > 0
                    stj('service') = svcMap;
                end
                stArr{end+1} = stj; %#ok<AGROW>
            end
            if ~isempty(stArr)
                for nj_idx = 1:length(nodesCellTmp)
                    nj = nodesCellTmp{nj_idx};
                    if strcmp(nj('name'), nodeObj.name)
                        nj('serverTypes') = stArr;
                        % Scheduling policy
                        policy = nodeObj.getHeteroSchedPolicy();
                        if ~isempty(policy) && policy ~= HeteroSchedPolicy.ORDER
                            nj('heteroSchedPolicy') = HeteroSchedPolicy.toText(policy);
                        end
                        nodesCellTmp{nj_idx} = nj;
                        break;
                    end
                end
            end
        end
    end
    result('nodes') = nodesCellTmp;
catch
end

% --- Balking, Retrial, Patience ---
try
    nodesCellTmp = result('nodes');
    for nj_idx = 1:length(nodesCellTmp)
        nj = nodesCellTmp{nj_idx};
        nodeName = nj('name');
        nodeObj = node_map(nodeName);
        if ~isa(nodeObj, 'Queue'), continue; end
        % Balking
        balkJson = containers.Map();
        for r = 1:K
            jc = classes{r};
            if nodeObj.hasBalking(jc)
                [strategy, thresholds] = nodeObj.getBalking(jc);
                bjc = containers.Map();
                switch strategy
                    case BalkingStrategy.QUEUE_LENGTH, bjc('strategy') = 'QUEUE_LENGTH';
                    case BalkingStrategy.EXPECTED_WAIT, bjc('strategy') = 'EXPECTED_WAIT';
                    case BalkingStrategy.COMBINED, bjc('strategy') = 'COMBINED';
                end
                thArr = {};
                for ti = 1:length(thresholds)
                    th = thresholds{ti};
                    tjson = containers.Map();
                    tjson('minJobs') = th{1};
                    if isinf(th{2})
                        tjson('maxJobs') = -1;
                    else
                        tjson('maxJobs') = th{2};
                    end
                    tjson('probability') = th{3};
                    thArr{end+1} = tjson;
                end
                bjc('thresholds') = thArr;
                balkJson(jc.name) = bjc;
            end
        end
        if balkJson.Count > 0
            nj('balking') = balkJson;
        end
        % Retrial
        retrialJson = containers.Map();
        for r = 1:K
            jc = classes{r};
            if nodeObj.hasRetrial(jc)
                [delayDist, maxAttempts] = nodeObj.getRetrial(jc);
                rjc = containers.Map();
                rjc('delay') = dist2json(delayDist);
                rjc('maxAttempts') = maxAttempts;
                retrialJson(jc.name) = rjc;
            end
        end
        if retrialJson.Count > 0
            nj('retrial') = retrialJson;
        end
        % Patience
        patienceJson = containers.Map();
        for r = 1:K
            jc = classes{r};
            patDist = nodeObj.getPatience(jc);
            if ~isempty(patDist) && ~isa(patDist, 'Disabled')
                pjc = containers.Map();
                pjc('distribution') = dist2json(patDist);
                impType = nodeObj.getImpatienceType(jc);
                if ~isempty(impType)
                    pjc('impatienceType') = ImpatienceType.toText(impType);
                end
                patienceJson(jc.name) = pjc;
            end
        end
        if patienceJson.Count > 0
            nj('patience') = patienceJson;
        end
        nodesCellTmp{nj_idx} = nj;
    end
    result('nodes') = nodesCellTmp;
catch
end

% --- Finite Capacity Regions ---
try
    regions = model.regions;
    if ~isempty(regions)
        fcrArray = {};
        for ri = 1:length(regions)
            reg = regions{ri};
            rj = containers.Map();
            rj('name') = reg.name;
            % Stations with per-class details
            stationsJson = {};
            for ni = 1:length(reg.nodes)
                sj = containers.Map();
                sj('node') = reg.nodes{ni}.name;
                % Per-class classCap
                if isprop(reg, 'classMaxJobs') && ~isempty(reg.classMaxJobs)
                    ccMap = containers.Map();
                    for r = 1:K
                        jc = classes{r};
                        if r <= length(reg.classMaxJobs) && isfinite(reg.classMaxJobs(r))
                            ccMap(jc.name) = reg.classMaxJobs(r);
                        end
                    end
                    if ccMap.Count > 0
                        sj('classCap') = ccMap;
                    end
                end
                % Per-class classWeight
                if isprop(reg, 'classWeight') && ~isempty(reg.classWeight)
                    cwMap = containers.Map();
                    for r = 1:K
                        jc = classes{r};
                        if r <= length(reg.classWeight) && reg.classWeight(r) ~= 1
                            cwMap(jc.name) = reg.classWeight(r);
                        end
                    end
                    if cwMap.Count > 0
                        sj('classWeight') = cwMap;
                    end
                end
                % Per-class classSize
                if isprop(reg, 'classSize') && ~isempty(reg.classSize)
                    csMap = containers.Map();
                    for r = 1:K
                        jc = classes{r};
                        if r <= length(reg.classSize) && reg.classSize(r) ~= 1
                            csMap(jc.name) = reg.classSize(r);
                        end
                    end
                    if csMap.Count > 0
                        sj('classSize') = csMap;
                    end
                end
                stationsJson{end+1} = sj; %#ok<AGROW>
            end
            rj('stations') = stationsJson;
            if isprop(reg, 'globalMaxJobs') && isfinite(reg.globalMaxJobs)
                rj('globalMaxJobs') = reg.globalMaxJobs;
            end
            if isprop(reg, 'globalMaxMemory') && isfinite(reg.globalMaxMemory)
                rj('globalMaxMemory') = reg.globalMaxMemory;
            end
            % Per-class classMaxJobs at region level
            if isprop(reg, 'classMaxJobs') && ~isempty(reg.classMaxJobs)
                cmjMap = containers.Map();
                for r = 1:K
                    jc = classes{r};
                    if r <= length(reg.classMaxJobs) && isfinite(reg.classMaxJobs(r))
                        cmjMap(jc.name) = reg.classMaxJobs(r);
                    end
                end
                if cmjMap.Count > 0
                    rj('classMaxJobs') = cmjMap;
                end
            end
            % Drop rule
            if isprop(reg, 'dropRule') && ~isempty(reg.dropRule)
                drMap = containers.Map();
                for r = 1:K
                    jc = classes{r};
                    if r <= length(reg.dropRule)
                        drStr = droprule_to_str(reg.dropRule(r));
                        if ~isempty(drStr)
                            drMap(jc.name) = drStr;
                        end
                    end
                end
                if drMap.Count > 0
                    rj('dropRule') = drMap;
                end
            end
            fcrArray{end+1} = rj; %#ok<AGROW>
        end
        if ~isempty(fcrArray)
            result('finiteCapacityRegions') = fcrArray;
        end
    end
catch
end
end


% =========================================================================
%  LayeredNetwork serialization
% =========================================================================

function result = layered2json(model)
result = containers.Map();
result('type') = 'LayeredNetwork';
result('name') = model.getName();

% --- Processors ---
procsJson = {};
hosts = model.hosts;
for i = 1:length(hosts)
    h = hosts{i};
    pj = containers.Map();
    pj('name') = h.name;
    mult = h.multiplicity;
    if isfinite(mult) && mult > 1
        pj('multiplicity') = mult;
    end
    schedStr = h.scheduling;
    if ~isempty(schedStr) && ~strcmpi(schedStr, 'inf')
        pj('scheduling') = upper(schedStr);
    end
    q = h.quantum;
    if q > 0 && q ~= 0.001
        pj('quantum') = q;
    end
    sf = h.speedFactor;
    if sf ~= 1.0
        pj('speedFactor') = sf;
    end
    repl = h.replication;
    if repl > 1
        pj('replication') = repl;
    end
    procsJson{end+1} = pj; %#ok<AGROW>
end
result('processors') = procsJson;

% --- Tasks ---
tasksJson = {};
tasksList = model.tasks;
for i = 1:length(tasksList)
    t = tasksList{i};
    tj = containers.Map();
    tj('name') = t.name;
    if ~isempty(t.parent)
        tj('processor') = t.parent.name;
    end
    mult = t.multiplicity;
    if isfinite(mult) && mult > 1
        tj('multiplicity') = mult;
    end
    schedStr = t.scheduling;
    if ~isempty(schedStr)
        tj('scheduling') = upper(schedStr);
    end
    % Think time
    ttMean = t.thinkTimeMean;
    if ~isempty(ttMean) && ttMean > GlobalConstants.FineTol
        if ~isempty(t.thinkTime) && isa(t.thinkTime, 'Distribution')
            tj('thinkTime') = dist2json(t.thinkTime);
        else
            params = containers.Map();
            params('lambda') = 1.0 / ttMean;
            dj = containers.Map();
            dj('type') = 'Exp';
            dj('params') = params;
            tj('thinkTime') = dj;
        end
    end
    % Fan in
    if ~isempty(t.fanInSource) && ischar(t.fanInSource) && ~isempty(t.fanInSource)
        fi = containers.Map();
        fi(t.fanInSource) = t.fanInValue;
        tj('fanIn') = fi;
    end
    % Fan out
    if ~isempty(t.fanOutDest)
        fo = containers.Map();
        for fi_idx = 1:length(t.fanOutDest)
            fo(t.fanOutDest{fi_idx}) = t.fanOutValue(fi_idx);
        end
        tj('fanOut') = fo;
    end
    repl = t.replication;
    if repl > 1
        tj('replication') = repl;
    end
    % FunctionTask detection
    if isa(t, 'FunctionTask')
        tj('taskType') = 'FunctionTask';
    end
    % Setup time / delay-off time (on any Task)
    if ~isempty(t.setupTime) && isa(t.setupTime, 'Distribution')
        stMean = t.setupTimeMean;
        if stMean > GlobalConstants.FineTol
            tj('setupTime') = dist2json(t.setupTime);
        end
    end
    if ~isempty(t.delayOffTime) && isa(t.delayOffTime, 'Distribution')
        dotMean = t.delayOffTimeMean;
        if dotMean > GlobalConstants.FineTol
            tj('delayOffTime') = dist2json(t.delayOffTime);
        end
    end
    % CacheTask detection
    if isa(t, 'CacheTask')
        tj('taskType') = 'CacheTask';
        tj('totalItems') = t.items;
        tj('cacheCapacity') = t.itemLevelCap;
        rs = t.replacestrategy;
        rsNameMap = containers.Map({ReplacementStrategy.RR, ReplacementStrategy.FIFO, ...
            ReplacementStrategy.SFIFO, ReplacementStrategy.LRU}, ...
            {'RR', 'FIFO', 'SFIFO', 'LRU'});
        if rsNameMap.isKey(rs)
            tj('replacementStrategy') = rsNameMap(rs);
        else
            tj('replacementStrategy') = 'FIFO';
        end
    end
    tasksJson{end+1} = tj; %#ok<AGROW>
end
result('tasks') = tasksJson;

% --- Entries ---
entriesJson = {};
entriesList = model.entries;
for i = 1:length(entriesList)
    e = entriesList{i};
    ej = containers.Map();
    ej('name') = e.name;
    if ~isempty(e.parent)
        ej('task') = e.parent.name;
    end
    % Entry arrival distribution
    if ~isempty(e.arrival) && isa(e.arrival, 'Distribution')
        ej('arrival') = dist2json(e.arrival);
    end
    % ItemEntry detection
    if isa(e, 'ItemEntry')
        ej('entryType') = 'ItemEntry';
        ej('totalItems') = e.cardinality;
        if ~isempty(e.popularity)
            if isa(e.popularity, 'Distribution')
                ej('accessProb') = dist2json(e.popularity);
            end
        end
    end
    entriesJson{end+1} = ej; %#ok<AGROW>
end
result('entries') = entriesJson;

% --- Build reply map: activityName -> entryName ---
replyMap = containers.Map();
for i = 1:length(entriesList)
    e = entriesList{i};
    if ~isempty(e.replyActivity)
        for j = 1:length(e.replyActivity)
            replyMap(e.replyActivity{j}) = e.name;
        end
    end
end

% --- Activities ---
actsJson = {};
actsList = model.activities;
for i = 1:length(actsList)
    a = actsList{i};
    aj = containers.Map();
    aj('name') = a.name;
    if ~isempty(a.parent)
        if isa(a.parent, 'Task') || isa(a.parent, 'Entry')
            aj('task') = a.parent.name;
        elseif ischar(a.parent) || isstring(a.parent)
            aj('task') = char(a.parent);
        elseif ischar(a.parentName) && ~isempty(a.parentName)
            aj('task') = a.parentName;
        end
    elseif ~isempty(a.parentName) && ischar(a.parentName)
        aj('task') = a.parentName;
    end
    % Host demand
    if ~isempty(a.hostDemand) && isa(a.hostDemand, 'Distribution')
        if ~isa(a.hostDemand, 'Immediate')
            aj('hostDemand') = dist2json(a.hostDemand);
        end
    elseif ~isempty(a.hostDemandMean) && a.hostDemandMean > GlobalConstants.FineTol
        params = containers.Map();
        params('lambda') = 1.0 / a.hostDemandMean;
        dj = containers.Map();
        dj('type') = 'Exp';
        dj('params') = params;
        aj('hostDemand') = dj;
    end
    % Bound to entry
    if ~isempty(a.boundToEntry)
        aj('boundTo') = a.boundToEntry;
    end
    % Replies to entry
    if replyMap.isKey(a.name)
        aj('repliesTo') = replyMap(a.name);
    end
    % Synch calls
    if ~isempty(a.syncCallDests)
        synchCalls = {};
        for j = 1:length(a.syncCallDests)
            sc = containers.Map();
            sc('entry') = a.syncCallDests{j};
            if j <= length(a.syncCallMeans) && a.syncCallMeans(j) ~= 1.0
                sc('mean') = a.syncCallMeans(j);
            end
            synchCalls{end+1} = sc; %#ok<AGROW>
        end
        aj('synchCalls') = synchCalls;
    end
    % Asynch calls
    if ~isempty(a.asyncCallDests)
        asynchCalls = {};
        for j = 1:length(a.asyncCallDests)
            ac = containers.Map();
            ac('entry') = a.asyncCallDests{j};
            if j <= length(a.asyncCallMeans) && a.asyncCallMeans(j) ~= 1.0
                ac('mean') = a.asyncCallMeans(j);
            end
            asynchCalls{end+1} = ac; %#ok<AGROW>
        end
        aj('asynchCalls') = asynchCalls;
    end
    actsJson{end+1} = aj; %#ok<AGROW>
end
result('activities') = actsJson;

% --- Precedences ---
precsJson = {};
for i = 1:length(tasksList)
    t = tasksList{i};
    precs = t.precedences;
    if isempty(precs), continue; end
    for j = 1:length(precs)
        p = precs(j);
        pj = containers.Map();
        pj('task') = t.name;

        preType  = p.preType;
        postType = p.postType;

        % Determine JSON precedence type and collect activity names
        if preType == ActivityPrecedenceType.PRE_SEQ && postType == ActivityPrecedenceType.POST_SEQ
            pj('type') = 'Serial';
            pj('activities') = [p.preActs, p.postActs];
        elseif preType == ActivityPrecedenceType.PRE_SEQ && postType == ActivityPrecedenceType.POST_AND
            pj('type') = 'AndFork';
            pj('activities') = [p.preActs, p.postActs];
        elseif preType == ActivityPrecedenceType.PRE_AND && postType == ActivityPrecedenceType.POST_SEQ
            pj('type') = 'AndJoin';
            pj('activities') = [p.preActs, p.postActs];
        elseif preType == ActivityPrecedenceType.PRE_SEQ && postType == ActivityPrecedenceType.POST_OR
            pj('type') = 'OrFork';
            pj('activities') = [p.preActs, p.postActs];
            if ~isempty(p.postParams)
                pj('probabilities') = p.postParams(:)';
            end
        elseif preType == ActivityPrecedenceType.PRE_OR && postType == ActivityPrecedenceType.POST_SEQ
            pj('type') = 'OrJoin';
            pj('activities') = [p.preActs, p.postActs];
        elseif postType == ActivityPrecedenceType.POST_LOOP
            pj('type') = 'Loop';
            % For Loop, preActs is the trigger, postActs is the loop body
            pj('activities') = p.postActs;
            if ~isempty(p.preActs)
                pj('preActivity') = p.preActs{1};
            end
            if ~isempty(p.postParams)
                pj('loopCount') = p.postParams(1);
            end
        elseif preType == ActivityPrecedenceType.PRE_SEQ && postType == ActivityPrecedenceType.POST_CACHE
            pj('type') = 'CacheAccess';
            pj('activities') = [p.preActs, p.postActs];
        else
            continue;
        end
        precsJson{end+1} = pj; %#ok<AGROW>
    end
end
if ~isempty(precsJson)
    result('precedences') = precsJson;
end
end


% =========================================================================
%  Workflow serialization
% =========================================================================

function result = workflow2json(model)
% Convert a Workflow to a containers.Map for JSON output.
result = containers.Map();
result('type') = 'Workflow';
result('name') = model.getName();

% --- Activities ---
actsJson = {};
acts = model.activities;
for i = 1:length(acts)
    act = acts{i};
    aj = containers.Map();
    aj('name') = act.name;
    if ~isempty(act.hostDemand) && isa(act.hostDemand, 'Distribution')
        dj = dist2json(act.hostDemand);
        if ~isempty(dj)
            aj('hostDemand') = dj;
        end
    end
    actsJson{end+1} = aj; %#ok<AGROW>
end
result('activities') = actsJson;

% --- Precedences ---
precsJson = {};
precs = model.precedences;
for i = 1:length(precs)
    p = precs(i);
    pj = containers.Map();

    % preActs
    preActsJson = {};
    for a = 1:length(p.preActs)
        preActsJson{end+1} = p.preActs{a}; %#ok<AGROW>
    end
    pj('preActs') = preActsJson;

    % postActs
    postActsJson = {};
    for a = 1:length(p.postActs)
        postActsJson{end+1} = p.postActs{a}; %#ok<AGROW>
    end
    pj('postActs') = postActsJson;

    % preType / postType - convert numeric IDs to JAR-compatible strings
    pj('preType') = prectype_to_str(p.preType);
    pj('postType') = prectype_to_str(p.postType);

    % preParams
    if ~isempty(p.preParams)
        pj('preParams') = p.preParams(:)';
    end

    % postParams
    if ~isempty(p.postParams)
        pj('postParams') = p.postParams(:)';
    end

    precsJson{end+1} = pj; %#ok<AGROW>
end
result('precedences') = precsJson;
end


% =========================================================================
%  Environment serialization
% =========================================================================

function result = environment2json(model)
% Convert an Environment to a containers.Map for JSON output.
result = containers.Map();
result('type') = 'Environment';
result('name') = model.getName();

E = height(model.envGraph.Nodes);
result('numStages') = E;

% --- Stages ---
stagesJson = {};
for e = 1:E
    sj = containers.Map();
    sj('name') = model.envGraph.Nodes.Name{e};
    % Serialize the stage's Network model
    if e <= length(model.ensemble) && ~isempty(model.ensemble{e})
        sj('model') = network2json(model.ensemble{e});
    end
    stagesJson{end+1} = sj; %#ok<AGROW>
end
result('stages') = stagesJson;

% --- Transitions ---
transJson = {};
for e = 1:E
    for h = 1:E
        if ~isempty(model.env) && e <= size(model.env, 1) && h <= size(model.env, 2) ...
                && ~isempty(model.env{e,h}) && ~isa(model.env{e,h}, 'Disabled')
            tj = containers.Map();
            tj('from') = e - 1;  % Convert to 0-indexed for JAR compatibility
            tj('to') = h - 1;    % Convert to 0-indexed for JAR compatibility
            dj = dist2json(model.env{e,h});
            if ~isempty(dj)
                tj('distribution') = dj;
                transJson{end+1} = tj; %#ok<AGROW>
            end
        end
    end
end
result('transitions') = transJson;
end


% =========================================================================
%  Distribution serialization
% =========================================================================

function d = dist2json(dist)
% Convert a Distribution to a containers.Map for JSON output.
if isempty(dist)
    d = [];
    return;
end
d = containers.Map();
cn = builtin('class', dist);
switch cn
    case 'Disabled'
        d('type') = 'Disabled';
    case 'Immediate'
        d('type') = 'Immediate';
    case 'Exp'
        d('type') = 'Exp';
        params = containers.Map();
        params('lambda') = dist.getParam(1).paramValue;
        d('params') = params;
    case 'Det'
        d('type') = 'Det';
        params = containers.Map();
        params('value') = dist.getParam(1).paramValue;
        d('params') = params;
    case 'Erlang'
        d('type') = 'Erlang';
        params = containers.Map();
        params('lambda') = dist.getParam(1).paramValue;
        params('k') = dist.getParam(2).paramValue;
        d('params') = params;
    case 'HyperExp'
        d('type') = 'HyperExp';
        params = containers.Map();
        p = dist.getParam(1).paramValue;
        l1 = dist.getParam(2).paramValue;
        l2 = dist.getParam(3).paramValue;
        if isscalar(p)
            params('p') = [p, 1-p];
            params('lambda') = [l1, l2];
        else
            params('p') = p(:)';
            params('lambda') = [l1, l2];
        end
        d('params') = params;
    case 'Gamma'
        d('type') = 'Gamma';
        params = containers.Map();
        params('alpha') = dist.getParam(1).paramValue;
        params('beta') = dist.getParam(2).paramValue;
        d('params') = params;
    case 'Lognormal'
        d('type') = 'Lognormal';
        params = containers.Map();
        params('mu') = dist.getParam(1).paramValue;
        params('sigma') = dist.getParam(2).paramValue;
        d('params') = params;
    case 'Uniform'
        d('type') = 'Uniform';
        params = containers.Map();
        params('a') = dist.getParam(1).paramValue;
        params('b') = dist.getParam(2).paramValue;
        d('params') = params;
    case 'Zipf'
        d('type') = 'Zipf';
        params = containers.Map();
        params('s') = dist.getParam(3).paramValue;
        params('n') = dist.getParam(4).paramValue;
        d('params') = params;
    case 'Pareto'
        d('type') = 'Pareto';
        params = containers.Map();
        params('alpha') = dist.getParam(1).paramValue;
        params('scale') = dist.getParam(2).paramValue;
        d('params') = params;
    case 'Weibull'
        d('type') = 'Weibull';
        params = containers.Map();
        params('alpha') = dist.getParam(1).paramValue;
        params('beta') = dist.getParam(2).paramValue;
        d('params') = params;
    case 'Normal'
        d('type') = 'Normal';
        params = containers.Map();
        params('mu') = dist.getParam(1).paramValue;
        params('sigma') = dist.getParam(2).paramValue;
        d('params') = params;
    case 'Geometric'
        d('type') = 'Geometric';
        params = containers.Map();
        params('p') = dist.getParam(1).paramValue;
        d('params') = params;
    case 'Binomial'
        d('type') = 'Binomial';
        params = containers.Map();
        params('n') = dist.getParam(1).paramValue;
        params('p') = dist.getParam(2).paramValue;
        d('params') = params;
    case 'Poisson'
        d('type') = 'Poisson';
        params = containers.Map();
        params('lambda') = dist.getParam(1).paramValue;
        d('params') = params;
    case 'Bernoulli'
        d('type') = 'Bernoulli';
        params = containers.Map();
        params('p') = dist.getParam(1).paramValue;
        d('params') = params;
    case 'DiscreteUniform'
        d('type') = 'DiscreteUniform';
        params = containers.Map();
        params('min') = dist.getParam(1).paramValue;
        params('max') = dist.getParam(2).paramValue;
        d('params') = params;
    case {'Coxian', 'Cox2'}
        d('type') = 'Coxian';
        params = containers.Map();
        params('mu') = dist.getMu()';
        params('phi') = dist.getPhi()';
        d('params') = params;
    case {'PH', 'APH'}
        d('type') = 'PH';
        ph = containers.Map();
        alpha = dist.getInitProb();
        T = dist.getSubgenerator();
        if isvector(alpha)
            ph('alpha') = alpha(:)';
        else
            ph('alpha') = alpha;
        end
        ph('T') = T;
        d('ph') = ph;
    case 'MAP'
        d('type') = 'MAP';
        mapSpec = containers.Map();
        mapSpec('D0') = dist.getParam(1).paramValue;
        mapSpec('D1') = dist.getParam(2).paramValue;
        d('map') = mapSpec;
    case 'MMPP2'
        d('type') = 'MMPP2';
        params = containers.Map();
        params('lambda0') = dist.getParam(1).paramValue;
        params('lambda1') = dist.getParam(2).paramValue;
        params('sigma0') = dist.getParam(3).paramValue;
        params('sigma1') = dist.getParam(4).paramValue;
        d('params') = params;
    case 'DiscreteSampler'
        d('type') = 'DiscreteSampler';
        params = containers.Map();
        params('p') = dist.getParam(1).paramValue(:)';
        params('x') = dist.getParam(2).paramValue(:)';
        d('params') = params;
    case 'Replayer'
        d('type') = 'Replayer';
        params = containers.Map();
        params('fileName') = dist.getParam(1).paramValue;
        try
            params('mean') = dist.getMean();
        catch
        end
        d('params') = params;
        % Save APH fit as fallback
        try
            aphDist = dist.fitAPH();
            if ~isempty(aphDist) && isa(aphDist, 'Distribution')
                ph = containers.Map();
                alpha = aphDist.getParam(1).paramValue;
                T = aphDist.getParam(2).paramValue;
                if isvector(alpha)
                    ph('alpha') = alpha(:)';
                else
                    ph('alpha') = alpha;
                end
                ph('T') = T;
                d('ph') = ph;
            end
        catch
        end
    case 'Prior'
        d('type') = 'Prior';
        alts = {};
        for ai = 1:dist.getNumAlternatives()
            altDist = dist.getAlternative(ai);
            altJson = dist2json(altDist);
            if ~isempty(altJson)
                alts{end+1} = altJson; %#ok<AGROW>
            end
        end
        d('distributions') = alts;
        d('probabilities') = dist.probabilities(:)';
    otherwise
        % Fallback: fit from mean
        try
            m = dist.getMean();
            d('type') = 'Exp';
            fit = containers.Map();
            fit('method') = 'fitMean';
            fit('mean') = m;
            d('fit') = fit;
        catch
            d = [];
        end
end
end


% =========================================================================
%  JSON encoding
% =========================================================================

function s = encode_value(val, indent)
% Recursively encode a MATLAB value to JSON string.
if nargin < 2, indent = 0; end
pad  = repmat(' ', 1, indent);
pad2 = repmat(' ', 1, indent + 2);

if isa(val, 'containers.Map')
    ks = val.keys();
    if isempty(ks)
        s = '{}';
    else
        parts = cell(1, length(ks));
        for i = 1:length(ks)
            k = ks{i};
            v = val(k);
            parts{i} = sprintf('%s"%s": %s', pad2, json_escape(k), encode_value(v, indent + 2));
        end
        s = sprintf('{\n%s\n%s}', strjoin(parts, sprintf(',\n')), pad);
    end
elseif ischar(val) || isstring(val)
    s = sprintf('"%s"', json_escape(char(val)));
elseif islogical(val) && isscalar(val)
    if val, s = 'true'; else, s = 'false'; end
elseif isnumeric(val) && isscalar(val)
    if isnan(val)
        s = 'null';
    elseif isinf(val)
        if val > 0, s = '"Infinity"'; else, s = '"-Infinity"'; end
    elseif val == floor(val) && abs(val) < 1e15
        s = sprintf('%d', val);
    else
        s = sprintf('%.15g', val);
    end
elseif isnumeric(val) && isvector(val) && ~isscalar(val)
    parts = cell(1, length(val));
    for i = 1:length(val)
        parts{i} = encode_value(val(i), 0);
    end
    s = ['[', strjoin(parts, ', '), ']'];
elseif isnumeric(val) && ismatrix(val) && ~isvector(val)
    rows = cell(1, size(val, 1));
    for i = 1:size(val, 1)
        rows{i} = encode_value(val(i,:), 0);
    end
    s = ['[', strjoin(rows, ', '), ']'];
elseif iscell(val)
    if isempty(val)
        s = '[]';
    else
        parts = cell(1, length(val));
        for i = 1:length(val)
            parts{i} = sprintf('%s%s', pad2, encode_value(val{i}, indent + 2));
        end
        s = sprintf('[\n%s\n%s]', strjoin(parts, sprintf(',\n')), pad);
    end
elseif isstruct(val) && isscalar(val)
    fnames = fieldnames(val);
    if isempty(fnames)
        s = '{}';
    else
        parts = cell(1, length(fnames));
        for i = 1:length(fnames)
            fn = fnames{i};
            fv = val.(fn);
            parts{i} = sprintf('%s"%s": %s', pad2, json_escape(fn), encode_value(fv, indent + 2));
        end
        s = sprintf('{\n%s\n%s}', strjoin(parts, sprintf(',\n')), pad);
    end
else
    s = 'null';
end
end

function s = json_escape(str)
% Escape special characters for JSON strings.
s = strrep(str, '\', '\\');
s = strrep(s, '"', '\"');
s = strrep(s, sprintf('\n'), '\n');
s = strrep(s, sprintf('\r'), '\r');
s = strrep(s, sprintf('\t'), '\t');
end


% =========================================================================
%  Helper functions
% =========================================================================

function s = node_type_str(node)
% Get the JSON node type string for a node object.
if isa(node, 'Source'),      s = 'Source';
elseif isa(node, 'Sink'),   s = 'Sink';
elseif isa(node, 'Delay'),  s = 'Delay';
elseif isa(node, 'Cache'),  s = 'Cache';
elseif isa(node, 'Place'),  s = 'Place';
elseif isa(node, 'Transition'), s = 'Transition';
elseif isa(node, 'Queue'),  s = 'Queue';
elseif isa(node, 'Fork'),   s = 'Fork';
elseif isa(node, 'Join'),   s = 'Join';
elseif isa(node, 'Router'), s = 'Router';
elseif isa(node, 'ClassSwitch'), s = 'ClassSwitch';
else,                        s = 'Queue';
end
end

function s = sched_id_to_str(id)
% Map SchedStrategy numeric ID to schema-compatible string.
if id == SchedStrategy.INF,        s = 'INF';
elseif id == SchedStrategy.FCFS,   s = 'FCFS';
elseif id == SchedStrategy.LCFS,   s = 'LCFS';
elseif id == SchedStrategy.LCFSPR, s = 'LCFSPR';
elseif id == SchedStrategy.PS,     s = 'PS';
elseif id == SchedStrategy.DPS,    s = 'DPS';
elseif id == SchedStrategy.GPS,    s = 'GPS';
elseif id == SchedStrategy.SIRO,   s = 'SIRO';
elseif id == SchedStrategy.SJF,    s = 'SJF';
elseif id == SchedStrategy.LJF,    s = 'LJF';
elseif id == SchedStrategy.SEPT,   s = 'SEPT';
elseif id == SchedStrategy.LEPT,   s = 'LEPT';
elseif id == SchedStrategy.HOL,    s = 'HOL';
elseif id == SchedStrategy.FORK,   s = 'FORK';
elseif id == SchedStrategy.EXT,    s = 'EXT';
elseif id == SchedStrategy.REF,    s = 'REF';
elseif id == SchedStrategy.POLLING, s = 'POLLING';
elseif id == SchedStrategy.PSPRIO, s = 'PSPRIO';
elseif id == SchedStrategy.DPSPRIO, s = 'DPSPRIO';
elseif id == SchedStrategy.GPSPRIO, s = 'GPSPRIO';
elseif id == SchedStrategy.FCFSPRIO, s = 'FCFSPRIO';
else,                              s = 'FCFS';
end
end

function s = repl_to_str(id)
% Map ReplacementStrategy numeric ID to schema string.
if id == ReplacementStrategy.LRU,   s = 'LRU';
elseif id == ReplacementStrategy.FIFO, s = 'FIFO';
elseif id == ReplacementStrategy.RR, s = 'RR';
elseif id == ReplacementStrategy.SFIFO, s = 'SFIFO';
else,                                s = 'LRU';
end
end

function s = droprule_to_str(id)
% Map DropStrategy numeric ID to schema-compatible string.
if id == DropStrategy.DROP,        s = 'drop';
elseif id == DropStrategy.WAITQ,   s = 'waitingQueue';
elseif id == DropStrategy.BAS,     s = 'blockingAfterService';
elseif id == DropStrategy.RETRIAL, s = 'retrial';
elseif id == DropStrategy.RETRIAL_WITH_LIMIT, s = 'retrialWithLimit';
else,                              s = '';
end
end

function s = prectype_to_str(id)
% Map ActivityPrecedenceType numeric ID to JAR-compatible string.
if id == ActivityPrecedenceType.PRE_SEQ,        s = 'pre';
elseif id == ActivityPrecedenceType.PRE_AND,    s = 'pre-AND';
elseif id == ActivityPrecedenceType.PRE_OR,     s = 'pre-OR';
elseif id == ActivityPrecedenceType.POST_SEQ,   s = 'post';
elseif id == ActivityPrecedenceType.POST_AND,   s = 'post-AND';
elseif id == ActivityPrecedenceType.POST_OR,    s = 'post-OR';
elseif id == ActivityPrecedenceType.POST_LOOP,  s = 'post-LOOP';
elseif id == ActivityPrecedenceType.POST_CACHE, s = 'post-CACHE';
else,                                           s = 'pre';
end
end
