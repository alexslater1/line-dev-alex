function [nodeStateSpace, sn, capacityc] = spaceGeneratorNodes(sn, cutoff, options)
if nargin<3
    options = Solver.defaultOptions;
end
N = sn.njobs';
sn.space = {};
capacityc = zeros(sn.nnodes, sn.nclasses);
% Draft SPN support
% for n=1:sn.nnodes
%     if isfield(sn.varsparam{n}, "capacityc")
%         capacityc(n) = sn.varsparam{n}.capacityc;
%     end
% end
for ind=1:sn.nnodes
    if sn.isstation(ind) % place jobs across stations
        ist = sn.nodeToStation(ind);
        isf = sn.nodeToStateful(ind);
        for r=1:sn.nclasses %cut-off open classes to finite capacity
            c = find(sn.chains(:,r));
            if ~isempty(sn.visits{c}) && sn.visits{c}(isf,r) == 0
                capacityc(ind,r) = 0;
            % Draft SPN support
            %elseif isfield(sn.varsparam{ind}, 'capacityc') 
            %    capacityc(ind,r) =  min(cutoff(ist,r), sn.classcap(ist,r));                
            elseif sn.nodetype(ind) ~= NodeType.Place && ~isempty(sn.proc) && ~isempty(sn.proc{ist}{r}) && any(any(isnan(sn.proc{ist}{r}{1}))) % disabled (not Places - they hold tokens without service)
                capacityc(ind,r) = 0;
            else
                if isinf(N(r))
                    capacityc(ind,r) =  min(cutoff(ist,r), sn.classcap(ist,r));
                else
                    capacityc(ind,r) =  sum(sn.njobs(sn.chains(c,:)));
                end
            end
        end
        if sn.isstation(ind)
            % in this case, the local variables are produced within
            % fromMarginalBounds, e.g., for RROBIN routing
            sn.space{isf} = State.fromMarginalBounds(sn, ind, [], capacityc(ind,:), sn.cap(ist), options);
        else
            % this is the case for example of cache nodes
            state_bufsrv = State.fromMarginalBounds(sn, ind, [], capacityc(ind,:), sn.cap(ist), options);
            state_var = State.spaceLocalVars(sn, ind);
            sn.space{isf} = State.cartesian(state_bufsrv,state_var); % generate all possible states for local variables
        end
        if isinf(sn.nservers(ist))
            sn.nservers(ist) = sum(capacityc(ind,:));
        end
    elseif sn.isstateful(ind) % generate state space of other stateful nodes that are not stations
        %ist = sn.nodeToStation(ind);
        isf = sn.nodeToStateful(ind);
        switch sn.nodetype(ind)
            case NodeType.Cache
                for r=1:sn.nclasses % restrict state space generation to immediate events
                    if isnan(sn.nodeparam{ind}.pread{r})
                        capacityc(ind,r) =  1; %
                    else
                        capacityc(ind,r) =  1; %
                    end
                end
            case NodeType.Router
                % For Router nodes, only allow capacity for classes that have
                % non-zero nodevisits (classes that actually visit the Router)
                for r=1:sn.nclasses
                    c = find(sn.chains(:,r));
                    if ~isempty(sn.nodevisits{c}) && sn.nodevisits{c}(ind,r) > 0
                        capacityc(ind,r) = 1;
                    else
                        capacityc(ind,r) = 0;
                    end
                end
            case NodeType.Transition
                capacityc(ind,:) = 0; % Transitions don't hold class-based jobs
                % Generate per-mode state space (bypass fromMarginalBounds)
                nmodes = sn.nodeparam{ind}.nmodes;
                firingphases = sn.nodeparam{ind}.firingphases;
                if any(isnan(firingphases))
                    firingphases = zeros(1, nmodes);
                    for m = 1:nmodes
                        if iscell(sn.nodeparam{ind}.firingproc) && ~isempty(sn.nodeparam{ind}.firingproc{m})
                            firingphases(m) = size(sn.nodeparam{ind}.firingproc{m}{1}, 1);
                        else
                            firingphases(m) = 1;
                        end
                    end
                end
                fK = firingphases;
                nmodeservers = sn.nodeparam{ind}.nmodeservers;
                max_jobs = sum(sn.njobs(~isinf(sn.njobs)));
                if any(isinf(sn.njobs))
                    max_jobs = max_jobs + sum(cutoff(1,:));
                end
                mode_spaces = cell(1, nmodes);
                for m = 1:nmodes
                    max_srv_m = nmodeservers(m);
                    if isinf(max_srv_m)
                        max_srv_m = max_jobs;
                    end
                    max_srv_m = min(max_srv_m, max_jobs);
                    mode_states = [];
                    for total = 0:max_srv_m
                        phase_combs = multichoose(fK(m), total);
                        buf_m = nmodeservers(m);
                        if isinf(buf_m)
                            buf_m = GlobalConstants.MaxInt();
                        end
                        buf_m = buf_m - total;
                        mode_states = [mode_states; repmat(buf_m, size(phase_combs,1), 1), phase_combs]; %#ok<AGROW>
                    end
                    mode_spaces{m} = mode_states;
                end
                trans_space = mode_spaces{1};
                for m = 2:nmodes
                    trans_space = State.cartesian(trans_space, mode_spaces{m});
                end
                trans_space = [trans_space, zeros(size(trans_space,1), nmodes)]; % append fired counts
                state_var = State.spaceLocalVars(sn, ind);
                sn.space{isf} = State.cartesian(trans_space, state_var);
                continue; % skip fromMarginalBounds below
            otherwise
                capacityc(ind,:) =  1; %
        end
        state_bufsrv = State.fromMarginalBounds(sn, ind, [], capacityc(ind,:), 1, options);
        state_var = State.spaceLocalVars(sn, ind);
        sn.space{isf} = State.cartesian(state_bufsrv,state_var); % generate all possible states for local variables
    end
end
nodeStateSpace = sn.space;
end