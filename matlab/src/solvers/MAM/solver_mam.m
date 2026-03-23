function [QN,UN,RN,TN,CN,XN,totiter,method,runtime] = solver_mam(sn, options)
%[Q,U,R,T,C,X,totiter] = SOLVER_MAM(QN, PH, OPTIONS)

%Copyright (c) 2012-2026, Imperial College London
%All rights reserved.

method = options.method;
config = options.config;
totiter = NaN;
PH = sn.proc;
I = sn.nnodes;
M = sn.nstations;
K = sn.nclasses;
C = sn.nchains;
N = sn.njobs';
V = cellsum(sn.visits);
Tstart=tic;
QN = zeros(M,K);
UN = zeros(M,K);
RN = zeros(M,K);
TN = zeros(M,K);
CN = zeros(1,K);
XN = zeros(1,K);

lambda = zeros(1,K);
for c=1:C
    inchain = sn.inchain{c};
    lambdas_inchain = sn.rates(sn.refstat(inchain(1)),inchain);
    lambdas_inchain = lambdas_inchain(isfinite(lambdas_inchain));
    lambda(inchain) = sum(lambdas_inchain);
end

chain = zeros(1,K);
for k=1:K
    chain(k) = find(sn.chains(:,k));
end

for ist=1:sn.nstations
    switch sn.sched(ist)
        case SchedStrategy.EXT
            % no-op
        case {SchedStrategy.FCFS, SchedStrategy.HOL, SchedStrategy.FCFSPRPRIO, SchedStrategy.PS}
            % no-op
        otherwise
            if options.verbose
                line_warning(mfilename,'The dec.mmap method does not support this scheduling strategy.\n');
            end
            [QN,UN,RN,TN,CN,XN] = deal([],[],[],[],[],[]);
            totiter = 0;
            method = '';
            runtime = toc(Tstart);
            return
    end
end

if all(isinf(sn.njobs)) % is open
    %    open queueing system (one node is the external world)
    pie = {};
    D0 = {};
    for ist=1:M
        switch sn.sched(ist)
            case SchedStrategy.EXT
                TN(ist,:) = sn.rates(ist,:);
                TN(ist,isnan(TN(ist,:)))=0;
            case {SchedStrategy.FCFS, SchedStrategy.HOL, SchedStrategy.FCFSPRPRIO, SchedStrategy.PS}
                for k=1:K
                    %                    divide service time by number of servers and put
                    %                    later a surrogate delay server in tandem to compensate
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
        %it
        %        now estimate arrival processes
        if it == 1
            %            initially form departure processes using scaled service
            DEP = PH;
            for ind=1:M
                for r=1:K
                    ist = sn.nodeToStation(ind);
                    DEP{ind,r} = map_scale(PH{ist}{r}, 1 / (lambda(r) * V(ind,r)) );
                end
            end
        end

        ARV = solver_mam_traffic(sn, DEP, config);

        QN_1 = QN;
        for ist=1:M
            ind = sn.stationToNode(ist);
            switch sn.nodetype(ind)
                case NodeType.Queue
                    if length(ARV{ind}{1}) > config.space_max
                        line_printf('\nArrival process at node %d is now at %d states. Compressing.',ind,length(ARV{ind}{1}));
                        ARV{ind} = mmap_compress(ARV{ind});
                    end
                    TN(ist,:) = mmap_lambda(ARV{ind});
                    switch sn.sched(ist)
                        case {SchedStrategy.FCFS, SchedStrategy.HOL, SchedStrategy.FCFSPRPRIO}
                            [Qret{1:K}, ~] = MMAPPH1FCFS({ARV{ind}{[1,3:end]}}, {pie{ist}{:}}, {D0{ist,:}}, 'ncMoms', 1, 'ncDistr',2);
                            for k=1:K
                                QN(ist,k) = sum(Qret{k});
                            end
                        case SchedStrategy.PS
                            for k=1:K
                                UN(ist,k) = TN(ist,k) * map_mean(PH{ist}{k});
                            end
                            Uden = min([1-GlobalConstants.FineTol, sum(UN(ist,:))]);
                            for k=1:K
                                QN(ist,k) = UN(ist,k)/(1-Uden);
                            end
                    end
            end
            for k=1:K
                UN(ist,k) = TN(ist,k) * map_mean(PH{ist}{k});
                %add number of jobs at the surrogate delay server
                QN(ist,k) = QN(ist,k) + TN(ist,k)*(map_mean(PH{ist}{k})*sn.nservers(ist)) * (sn.nservers(ist)-1)/sn.nservers(ist);
                RN(ist,k) = QN(ist,k) ./ TN(ist,k);
            end
        end

        if it >=3 && max(abs(QN(:)-QN_1(:))./QN_1(:)) < options.iter_tol
            break;
        end

        for ist=1:M
            ind = sn.stationToNode(ist);
            switch sn.nodetype(ind)
                case NodeType.Queue
                    for r=1:K
                        % extract class-r arrival MAP
                        A = mmap_hide(ARV{ind},setdiff(1:K,r));
                        S = PH{ist}{r};
                        na = length(A{1});
                        ns = length(S{1});
                        etaqa_n = config.etaqa_trunc;
                        etaqa_sz = (etaqa_n+1)*na*ns;
                        rho = sum(UN(ist,:));
                        % use ETAQA if state space is manageable and queue is stable
                        if etaqa_sz <= config.space_max && rho < 1-GlobalConstants.FineTol
                            try
                                switch sn.sched(ist)
                                    case {SchedStrategy.FCFS, SchedStrategy.HOL, SchedStrategy.FCFSPRPRIO}
                                        DEP{ind,r} = qbd_depproc_etaqa(A, S, etaqa_n);
                                    case SchedStrategy.PS
                                        DEP{ind,r} = qbd_depproc_etaqa_ps(A, S, etaqa_n);
                                end
                                DEP{ind,r} = map_normalize(DEP{ind,r});
                            catch
                                % fall back to scaled service on ETAQA failure
                                DEP{ind,r} = PH{ist}{r};
                            end
                        else
                            DEP{ind,r} = PH{ist}{r};
                        end
                        DEP{ind,r} = map_scale(DEP{ind,r}, 1 / (lambda(r) * V(ind,r)) );
                        SCVd(ind,r) = map_scv(DEP{ind,r});
                        IDCd(ind,r) = map_idc(DEP{ind,r});
                    end
            end
        end
    end
    totiter = it;
    if options.verbose
        line_printf('\nMAM parametric decomposition completed in %d iterations.',it);
    end
else
    if options.verbose
        line_warning(mfilename,'This model is not supported by SolverMAM yet. Returning with no result.\n');
    end
end
runtime = toc(Tstart);
end
