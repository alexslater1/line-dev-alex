function [QN, UN, RN, TN, CN, XN, t, QNt, UNt, TNt, xvec_iter, hitprob, missprob, runtime, it] = solver_fld_cacheqn_analyzer(sn, options)
% SOLVER_FLD_CACHEQN_ANALYZER Fluid solver for integrated caching-queueing networks
%
% Iterates between:
%   1. Isolated cache analysis (using cache_miss_fpi / RMF)
%   2. Fluid ODE solution of the surrounding queueing network
% until arrival rates to the cache converge.

% Copyright (c) 2012-2026, Imperial College London
% All rights reserved.

T0 = tic;
I = sn.nnodes;
K = sn.nclasses;
M = sn.nstations;

statefulNodes = find(sn.isstateful)';
statefulNodesClasses = [];
for ind = statefulNodes %#ok<FXSET>
    statefulNodesClasses(end+1:end+K) = ((ind-1)*K+1):(ind*K);
end
lambda = zeros(1, K);
lambda_1 = zeros(1, K);
caches = find(sn.nodetype == NodeType.Cache);

hitprob = zeros(length(caches), K);
missprob = zeros(length(caches), K);

for it = 1:options.iter_max
    for cIdx = 1:length(caches)
        ind = caches(cIdx);
        ch = sn.nodeparam{ind};
        hitClass = ch.hitclass;
        missClass = ch.missclass;
        inputClass = find(hitClass);
        m = ch.itemcap;
        n = ch.nitems;
        if it == 1
            % initial random value of arrival rates to the cache
            lambda_1(inputClass) = rand(1, length(inputClass));
            lambda = lambda_1;
            sn.nodetype(ind) = NodeType.ClassSwitch;
        end

        % solution of isolated cache
        h = length(m);
        u = length(lambda);
        lambda_cache = zeros(u, n, h);

        for v = 1:u
            for k = 1:n
                for l = 1:(h+1)
                    if ~isnan(ch.pread{v})
                        lambda_cache(v, k, l) = lambda(v) * ch.pread{v}(k);
                    end
                end
            end
        end

        Rcost = ch.accost;
        if isempty(Rcost)
            % Default linear cache routing: items flow from list l to list l+1
            Rcost = cell(u, n);
            for v = 1:u
                for k = 1:n
                    Rmat = diag(ones(1, h), 1);
                    Rmat(h+1, h+1) = 1;
                    Rcost{v, k} = Rmat;
                end
            end
        end
        gamma = cache_gamma_lp(lambda_cache, Rcost);

        [~, missrate(cIdx, :)] = cache_miss_fpi(gamma, m, lambda_cache); %#ok<AGROW>

        missprob(cIdx, :) = missrate(cIdx, :) ./ lambda;
        hitprob(cIdx, :) = 1 - missprob(cIdx, :);
        hitprob(isnan(hitprob)) = 0;
        missprob(isnan(missprob)) = 0;

        % bring back the isolated model results into the queueing model
        for r = inputClass
            sn.rtnodes((ind-1)*K+r, :) = 0;
            for jnd = 1:I
                if sn.connmatrix(ind, jnd)
                    sn.rtnodes((ind-1)*K+r, (jnd-1)*K+hitClass(r)) = hitprob(cIdx, r);
                    sn.rtnodes((ind-1)*K+r, (jnd-1)*K+missClass(r)) = missprob(cIdx, r);
                end
            end
        end
        sn.rt = dtmc_stochcomp(sn.rtnodes, statefulNodesClasses);
    end
    [visits, nodevisits, sn] = sn_refresh_visits(sn, sn.chains, sn.rt, sn.rtnodes);
    sn.visits = visits;
    sn.nodevisits = nodevisits;

    % Solve the queueing network using the fluid matrix method
    fluid_options = options;
    fluid_options.method = 'matrix';
    fluid_options.init_sol = solver_fluid_initsol(sn, fluid_options);
    [QN, UN, RN, TN, xvec_iter, QNt, UNt, TNt, ~, t] = solver_fluid_matrix(sn, fluid_options);

    % Compute system throughputs
    XN = zeros(1, K);
    for k = 1:K
        if sn.refstat(k) > 0
            XN(k) = TN(sn.refstat(k), k);
        end
    end

    % Update arrival rates to the cache
    nodevisits = cellsum(nodevisits);
    for cIdx = 1:length(caches)
        ind = caches(cIdx);
        ch = sn.nodeparam{ind};
        inputClass = find(ch.hitclass);
        for r = inputClass
            c = find(sn.chains(:, r));
            inchain = find(sn.chains(c, :));
            if sn.refclass(c) > 0
                lambda(r) = sum(XN(inchain)) * nodevisits(ind, r) / nodevisits(sn.stationToNode(sn.refstat(r)), sn.refclass(c));
            else
                lambda(r) = sum(XN(inchain)) * nodevisits(ind, r) / nodevisits(sn.stationToNode(sn.refstat(r)), r);
            end
        end
    end
    if norm(lambda - lambda_1, 1) < options.iter_tol
        break
    end
    lambda_1 = lambda;
end

% Compute CN
CN = zeros(1, K);
for k = 1:K
    if sn.refstat(k) > 0
        CN(k) = sn.njobs(k) ./ XN(k);
    end
end

% xvec_iter is already a cell array from solver_fluid_matrix
runtime = toc(T0);
end
