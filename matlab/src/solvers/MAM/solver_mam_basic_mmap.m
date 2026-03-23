function [QN,UN,RN,TN,CN,XN,totiter] = solver_mam_basic_mmap(sn, options)
% [QN,UN,RN,TN,CN,XN,TOTITER] = SOLVER_MAM_BASIC_MMAP(SN, OPTIONS)
%
% MAM decomposition solver with fork-join synchronization via mmap_max.
% Follows the solver_mam.m (dec.mmap) iteration pattern with FJ-aware
% traffic analysis using solver_mam_traffic_mmap.
%
% Supports open networks with general FJ topologies and heterogeneous
% service distributions at parallel queues.
%
% Copyright (c) 2012-2026, Imperial College London
% All rights reserved.

config = options.config;
if ~isfield(config, 'fj_sync_q_len')
    config.fj_sync_q_len = 2;
end
if ~isfield(config, 'etaqa_trunc')
    config.etaqa_trunc = 8;
end

PH = sn.proc;
I = sn.nnodes;
M = sn.nstations;
K = sn.nclasses;
C = sn.nchains;
N = sn.njobs';
V = cellsum(sn.visits);
S = 1./sn.rates;

QN = zeros(M,K);
UN = zeros(M,K);
RN = zeros(M,K);
TN = zeros(M,K);
CN = zeros(1,K);
XN = zeros(1,K);

% Build FJ synchronization map
fjSyncMap = sn_build_fj_sync_map(sn);

% Determine per-chain arrival rates
lambda = zeros(1,K);
for c=1:C
    inchain = sn.inchain{c};
    lambdas_inchain = sn.rates(sn.refstat(inchain(1)),inchain);
    lambdas_inchain = lambdas_inchain(isfinite(lambdas_inchain));
    lambda(inchain) = sum(lambdas_inchain);
end

% Prepare PH service distributions
pie = {};
D0 = {};
for ist=1:M
    switch sn.sched(ist)
        case SchedStrategy.EXT
            TN(ist,:) = sn.rates(ist,:);
            TN(ist,isnan(TN(ist,:))) = 0;
        case {SchedStrategy.FCFS, SchedStrategy.HOL, SchedStrategy.FCFSPRPRIO}
            for k=1:K
                % divide service time by number of servers and put
                % later a surrogate delay server in tandem to compensate
                PH{ist}{k} = map_scale(PH{ist}{k}, map_mean(PH{ist}{k})/sn.nservers(ist));
                pie{ist}{k} = map_pie(PH{ist}{k});
                D0{ist,k} = PH{ist}{k}{1};
                if any(isnan(D0{ist,k}))
                    D0{ist,k} = -GlobalConstants.Immediate;
                    pie{ist}{k} = 1;
                    PH{ist}{k} = map_exponential(GlobalConstants.Immediate);
                end
            end
        case SchedStrategy.INF
            for k=1:K
                pie{ist}{k} = map_pie(PH{ist}{k});
                D0{ist,k} = PH{ist}{k}{1};
                if any(isnan(D0{ist,k}))
                    D0{ist,k} = -GlobalConstants.Immediate;
                    pie{ist}{k} = 1;
                    PH{ist}{k} = map_exponential(GlobalConstants.Immediate);
                end
            end
        case SchedStrategy.PS
            for k=1:K
                PH{ist}{k} = map_scale(PH{ist}{k}, map_mean(PH{ist}{k})/sn.nservers(ist));
                pie{ist}{k} = map_pie(PH{ist}{k});
                D0{ist,k} = PH{ist}{k}{1};
                if any(isnan(D0{ist,k}))
                    D0{ist,k} = -GlobalConstants.Immediate;
                    pie{ist}{k} = 1;
                    PH{ist}{k} = map_exponential(GlobalConstants.Immediate);
                end
            end
    end
end

it_max = options.iter_max;
for it=1:it_max
    % Initialize departure processes (node-indexed: DEP{ind,r})
    if it == 1
        DEP = cell(I,K);
        for ind=1:I
            isForkJoin = (sn.nodetype(ind) == NodeType.Fork || sn.nodetype(ind) == NodeType.Join);
            if sn.isstation(ind) && ~isForkJoin
                ist = sn.nodeToStation(ind);
                for r=1:K
                    if V(ist,r) > 0 && lambda(r) > 0
                        DEP{ind,r} = map_scale(PH{ist}{r}, 1 / (lambda(r) * V(ist,r)));
                    else
                        DEP{ind,r} = PH{ist}{r};
                    end
                end
            else
                % Non-station nodes or Fork/Join: pass-through
                % map_exponential takes MEAN interarrival time = 1/rate
                for r=1:K
                    if lambda(r) > 0
                        DEP{ind,r} = map_exponential(1/lambda(r));
                    else
                        DEP{ind,r} = map_exponential(1/GlobalConstants.Immediate);
                    end
                end
            end
        end
    end

    % Compute arrival processes with FJ synchronization
    ARV = solver_mam_traffic_mmap(sn, DEP, config, fjSyncMap);

    QN_1 = QN;
    for ist=1:M
        ind = sn.stationToNode(ist);
        switch sn.nodetype(ind)
            case NodeType.Join
                % Join: throughput = arrival rate (flow conservation)
                for k=1:K
                    TN(ist,k) = lambda(k);
                    UN(ist,k) = 0;
                    QN(ist,k) = 0;
                    RN(ist,k) = 0;
                end
            case NodeType.Queue
                if ~isempty(ARV{ind}) && iscell(ARV{ind})
                    % Compress arrival process if too large
                    if length(ARV{ind}{1}) > config.space_max
                        if options.verbose
                            line_printf('\nArrival process at node %d is now at %d states. Compressing.', ind, length(ARV{ind}{1}));
                        end
                        ARV{ind} = mmap_compress(ARV{ind});
                    end

                    switch sn.sched(ist)
                        case {SchedStrategy.FCFS, SchedStrategy.HOL, SchedStrategy.FCFSPRPRIO}
                            [Qret{1:K}, ~] = MMAPPH1FCFS({ARV{ind}{[1,3:end]}}, {pie{ist}{:}}, {D0{ist,:}}, 'ncMoms', 1, 'ncDistr', 2);
                            for k=1:K
                                QN(ist,k) = sum(Qret{k});
                            end
                            TN(ist,:) = mmap_lambda(ARV{ind});
                        case SchedStrategy.PS
                            TN(ist,:) = mmap_lambda(ARV{ind});
                            for k=1:K
                                UN(ist,k) = TN(ist,k) * S(ist,k);
                            end
                            Uden = min([1-GlobalConstants.FineTol, sum(UN(ist,:))]);
                            for k=1:K
                                QN(ist,k) = UN(ist,k)/(1-Uden);
                            end
                    end

                    for k=1:K
                        UN(ist,k) = TN(ist,k) * map_mean(PH{ist}{k});
                        % add number of jobs at the surrogate delay server
                        QN(ist,k) = QN(ist,k) + TN(ist,k)*(map_mean(PH{ist}{k})*sn.nservers(ist)) * (sn.nservers(ist)-1)/sn.nservers(ist);
                        RN(ist,k) = QN(ist,k) ./ TN(ist,k);
                    end
                end
            otherwise
                switch sn.sched(ist)
                    case SchedStrategy.INF
                        if ~isempty(ARV{ind}) && iscell(ARV{ind})
                            TN(ist,:) = mmap_lambda(ARV{ind});
                        end
                        for k=1:K
                            if TN(ist,k) > 0
                                UN(ist,k) = S(ist,k)*TN(ist,k);
                                QN(ist,k) = TN(ist,k)*S(ist,k);
                                RN(ist,k) = S(ist,k);
                            end
                        end
                    case SchedStrategy.EXT
                        % Source: already set TN above
                end
        end
    end

    % Check convergence
    if it >= 3 && max(abs(QN(:)-QN_1(:))./(QN_1(:)+GlobalConstants.FineTol)) < options.iter_tol
        break;
    end

    % Update departure processes
    for ist=1:M
        ind = sn.stationToNode(ist);
        switch sn.nodetype(ind)
            case NodeType.Queue
                if ~isempty(ARV{ind}) && iscell(ARV{ind})
                    switch sn.sched(ist)
                        case {SchedStrategy.FCFS, SchedStrategy.HOL, SchedStrategy.FCFSPRPRIO}
                            for r=1:K
                                A = mmap_hide(ARV{ind}, setdiff(1:K,r));
                                Srv = PH{ist}{r};
                                na = length(A{1});
                                ns = length(Srv{1});
                                etaqa_n = config.etaqa_trunc;
                                etaqa_sz = (etaqa_n+1)*na*ns;
                                rho = sum(UN(ist,:));
                                if etaqa_sz <= config.space_max && rho < 1-GlobalConstants.FineTol
                                    try
                                        DEP{ind,r} = qbd_depproc_etaqa(A, Srv, etaqa_n);
                                        DEP{ind,r} = map_normalize(DEP{ind,r});
                                    catch
                                        DEP{ind,r} = Srv;
                                    end
                                else
                                    DEP{ind,r} = Srv;
                                end
                                if V(ist,r) > 0 && lambda(r) > 0
                                    DEP{ind,r} = map_scale(DEP{ind,r}, 1 / (lambda(r) * V(ist,r)));
                                end
                            end
                        case SchedStrategy.PS
                            for r=1:K
                                A = mmap_hide(ARV{ind}, setdiff(1:K,r));
                                Srv = PH{ist}{r};
                                na = length(A{1});
                                ns = length(Srv{1});
                                etaqa_n = config.etaqa_trunc;
                                etaqa_sz = (etaqa_n+1)*na*ns;
                                rho = sum(UN(ist,:));
                                if V(ist,r) > 0 && lambda(r) > 0
                                    if etaqa_sz <= config.space_max && rho < 1-GlobalConstants.FineTol
                                        try
                                            DEP{ind,r} = qbd_depproc_etaqa_ps(A, Srv, etaqa_n);
                                            DEP{ind,r} = map_normalize(DEP{ind,r});
                                        catch
                                            DEP{ind,r} = Srv;
                                        end
                                    else
                                        DEP{ind,r} = Srv;
                                    end
                                    DEP{ind,r} = map_scale(DEP{ind,r}, 1 / (lambda(r) * V(ist,r)));
                                end
                            end
                    end
                end
            case NodeType.Join
                % Join: departure = synchronized arrival (pass-through)
                % map_exponential takes MEAN interarrival time = 1/rate
                for r=1:K
                    if TN(ist,r) > 0
                        DEP{ind,r} = map_exponential(1/TN(ist,r));
                    end
                end
        end
        % Fork nodes keep their initial departure processes (pass-through)
    end
end

totiter = it;
if options.verbose
    line_printf('\nMAM FJ parametric decomposition completed in %d iterations.', it);
end

% Join nodes represent synchronization delay, not service demand. Recover
% their queue length and response time from the parallel branch means.
for joinIdx = find(sn.nodetype == NodeType.Join)'
    joinStat = sn.nodeToStation(joinIdx);
    if isnan(joinStat)
        continue;
    end
    syncGroups = unique(fjSyncMap.nodeSync(joinIdx, :));
    syncGroups = syncGroups(syncGroups > 0);
    for r = 1:K
        if TN(joinStat, r) <= 0
            continue;
        end
        syncDelay = 0;
        joinArrivalRate = 0;
        for gid = syncGroups
            branchNodes = find(fjSyncMap.nodeSync(joinIdx, :) == gid);
            branchRt = zeros(1, numel(branchNodes));
            branchTput = zeros(1, numel(branchNodes));
            used = 0;
            for b = 1:numel(branchNodes)
                branchStat = sn.nodeToStation(branchNodes(b));
                if isnan(branchStat) || RN(branchStat, r) <= 0
                    continue;
                end
                used = used + 1;
                branchRt(used) = RN(branchStat, r);
                branchTput(used) = TN(branchStat, r);
            end
            branchRt = branchRt(1:used);
            branchTput = branchTput(1:used);
            if numel(branchRt) < 2
                continue;
            end
            lambdai = 1 ./ branchRt;
            maxBranchRt = 0;
            for pow = 0:(numel(branchRt) - 1)
                maxBranchRt = maxBranchRt + (-1)^pow * sum(1 ./ sum(nchoosek(lambdai, pow + 1), 2));
            end
            syncDelay = syncDelay + max(maxBranchRt - mean(branchRt), 0);
            joinArrivalRate = joinArrivalRate + sum(branchTput);
        end
        RN(joinStat, r) = syncDelay;
        QN(joinStat, r) = joinArrivalRate * syncDelay;
        UN(joinStat, r) = 0;
    end
end

% Post-processing
CN = sum(RN,1);
QN(isnan(QN)) = 0;
RN(isnan(RN)) = 0;
UN(isnan(UN)) = 0;
TN(isnan(TN)) = 0;
end
