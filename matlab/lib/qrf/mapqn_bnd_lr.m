function [result, x, fval, exitflag] = mapqn_bnd_lr(params, objective_queue, objective_phase, sense)
% MAPQN_BND_LR - General Linear Reduction Bounds for MAP Queueing Networks
%
% MATLAB port of the Python file bnd_lr.py
%
% Usage:
%   [result, x, fval, exitflag] = mapqn_bnd_lr(params)
%   [result, x, fval, exitflag] = mapqn_bnd_lr(params, objective_queue)
%   [result, x, fval, exitflag] = mapqn_bnd_lr(params, objective_queue, objective_phase)
%   [result, x, fval, exitflag] = mapqn_bnd_lr(params, objective_queue, objective_phase, sense)
%
% Inputs:
%   params          - Structure with model parameters:
%                     .M       - Number of queues
%                     .N       - Total population
%                     .K       - [M x 1] Number of phases for each queue
%                     .mu      - {M x 1} cell, each mu{i} is K(i) x K(i) completion rates
%                     .v       - {M x 1} cell, each v{i} is K(i) x K(i) background rates
%                     .r       - [M x M] Routing probabilities
%                     .verbose - (optional) boolean, default true
%
%   objective_queue - (optional) Queue index to optimize (1-based), default 1
%   objective_phase - (optional) Phase index to optimize (1-based), default 1
%   sense           - (optional) 'min' or 'max', default 'max'
%
% Outputs:
%   result   - Structure with results:
%              .objective - Objective function value
%              .exitflag  - Solver exit flag
%              .U         - [M x max(K)] Utilization matrix
%              .IT        - [M x max(K)] Idle time matrix
%              .Q         - [M x max(K)] Queue length matrix
%   x        - Raw solution vector
%   fval     - Objective function value
%   exitflag - Solver exit flag

    if nargin < 2 || isempty(objective_queue)
        objective_queue = 1;
    end
    if nargin < 3 || isempty(objective_phase)
        objective_phase = 1;
    end
    if nargin < 4 || isempty(sense)
        sense = 'max';
    end

    % Extract parameters
    M = params.M;
    N = params.N;
    K = params.K(:);
    mu = params.mu;
    v = params.v;
    r = params.r;
    if isfield(params, 'verbose')
        verbose = params.verbose;
    else
        verbose = true;
    end

    maxK = max(K);

    % Compute transition rates q{i,j}(k,h)
    q = cell(M, M);
    for i = 1:M
        for j = 1:M
            q{i,j} = zeros(K(i), K(i));
            for ki = 1:K(i)
                for hi = 1:K(i)
                    if j ~= i
                        q{i,j}(ki, hi) = r(i,j) * mu{i}(ki, hi);
                    else
                        q{i,j}(ki, hi) = v{i}(ki, hi) + r(i,i) * mu{i}(ki, hi);
                    end
                end
            end
        end
    end

    %% Build variable indexing
    if verbose; fprintf('Building variable index map...\n'); end
    varCount = 0;

    % U(i,k) variables: utilization at queue i, phase k
    Uidx = zeros(M, maxK);
    for i = 1:M
        for k = 1:K(i)
            varCount = varCount + 1;
            Uidx(i, k) = varCount;
        end
    end

    % IT(i,k) variables: idle time at queue i, phase k
    ITidx = zeros(M, maxK);
    for i = 1:M
        for k = 1:K(i)
            varCount = varCount + 1;
            ITidx(i, k) = varCount;
        end
    end

    % Q(i,k) variables: mean queue length at queue i, phase k
    Qidx = zeros(M, maxK);
    for i = 1:M
        for k = 1:K(i)
            varCount = varCount + 1;
            Qidx(i, k) = varCount;
        end
    end

    % UP(j,kj,i,hi) variables: utilization products
    UPidx = zeros(M, maxK, M, maxK);
    for j = 1:M
        for kj = 1:K(j)
            for i = 1:M
                for hi = 1:K(i)
                    varCount = varCount + 1;
                    UPidx(j, kj, i, hi) = varCount;
                end
            end
        end
    end

    % QP(j,kj,i,hi) variables: queue-length products
    QPidx = zeros(M, maxK, M, maxK);
    for j = 1:M
        for kj = 1:K(j)
            for i = 1:M
                for hi = 1:K(i)
                    varCount = varCount + 1;
                    QPidx(j, kj, i, hi) = varCount;
                end
            end
        end
    end

    % C(j,kj,i) variables: conditional queue lengths
    Cidx = zeros(M, maxK, M);
    for j = 1:M
        for kj = 1:K(j)
            for i = 1:M
                varCount = varCount + 1;
                Cidx(j, kj, i) = varCount;
            end
        end
    end

    % I_var(j,kj,i) variables: conditional idle lengths
    Iidx = zeros(M, maxK, M);
    for j = 1:M
        for kj = 1:K(j)
            for i = 1:M
                varCount = varCount + 1;
                Iidx(j, kj, i) = varCount;
            end
        end
    end

    % p1(j,kj,i,ni,hi) variables: marginal probabilities, ni from 0 to N
    % Stored as p1idx(j, kj, i, ni+1, hi)
    p1idx = zeros(M, maxK, M, N+1, maxK);
    for j = 1:M
        for kj = 1:K(j)
            for i = 1:M
                for ni = 0:N
                    for hi = 1:K(i)
                        varCount = varCount + 1;
                        p1idx(j, kj, i, ni+1, hi) = varCount;
                    end
                end
            end
        end
    end

    % p1c(j,kj,i,ni,hi) variables: complementary marginal probabilities, ni from 0 to N
    % Stored as p1cidx(j, kj, i, ni+1, hi)
    p1cidx = zeros(M, maxK, M, N+1, maxK);
    for j = 1:M
        for kj = 1:K(j)
            for i = 1:M
                for ni = 0:N
                    for hi = 1:K(i)
                        varCount = varCount + 1;
                        p1cidx(j, kj, i, ni+1, hi) = varCount;
                    end
                end
            end
        end
    end

    nVars = varCount;
    if verbose; fprintf('Total variables: %d\n', nVars); end

    %% Initialize bounds
    lb = zeros(nVars, 1);
    ub = inf(nVars, 1);

    % Set upper bounds for each variable type
    for i = 1:M
        for k = 1:K(i)
            ub(Uidx(i, k)) = 1;
            ub(ITidx(i, k)) = 1;
            ub(Qidx(i, k)) = N;
        end
    end
    for j = 1:M
        for kj = 1:K(j)
            for i = 1:M
                for hi = 1:K(i)
                    ub(UPidx(j, kj, i, hi)) = 1;
                    ub(QPidx(j, kj, i, hi)) = N;
                end
            end
        end
    end
    for j = 1:M
        for kj = 1:K(j)
            for i = 1:M
                ub(Cidx(j, kj, i)) = N;
                ub(Iidx(j, kj, i)) = N;
            end
        end
    end
    for j = 1:M
        for kj = 1:K(j)
            for i = 1:M
                for ni = 0:N
                    for hi = 1:K(i)
                        ub(p1idx(j, kj, i, ni+1, hi)) = 1;
                        ub(p1cidx(j, kj, i, ni+1, hi)) = 1;
                    end
                end
            end
        end
    end

    %% Build constraints
    Aeq = [];
    beq = [];
    Aineq = [];
    bineq = [];

    if verbose; fprintf('Building constraints...\n'); end

    %% ZER1: p1(j,k,j,0,k) = 0 for all j,k
    % Implemented via upper bounds
    if verbose; fprintf('  ZER1 constraints...\n'); end
    for j = 1:M
        for k = 1:K(j)
            idx = p1idx(j, k, j, 0+1, k);
            ub(idx) = 0;
        end
    end

    %% ZER2: p1(j,k,j,nj,h) = 0 for h ~= k, all j,k,nj
    if verbose; fprintf('  ZER2 constraints...\n'); end
    for j = 1:M
        for k = 1:K(j)
            for nj = 0:N
                for h = 1:K(j)
                    if h ~= k
                        idx = p1idx(j, k, j, nj+1, h);
                        ub(idx) = 0;
                    end
                end
            end
        end
    end

    %% ZER3: p1(j,k,i,N,h) = 0 for j ~= i, all j,k,i,h
    if verbose; fprintf('  ZER3 constraints...\n'); end
    for j = 1:M
        for k = 1:K(j)
            for i = 1:M
                if j ~= i
                    for h = 1:K(i)
                        idx = p1idx(j, k, i, N+1, h);
                        ub(idx) = 0;
                    end
                end
            end
        end
    end

    %% ZER4: p1c(j,k,j,nj,h) = 0 for nj >= 1, all j,k,nj,h
    if verbose; fprintf('  ZER4 constraints...\n'); end
    for j = 1:M
        for k = 1:K(j)
            for nj = 1:N
                for h = 1:K(j)
                    idx = p1cidx(j, k, j, nj+1, h);
                    ub(idx) = 0;
                end
            end
        end
    end

    %% CEQU: C(j,k,j) = Q(j,k) for all j,k
    if verbose; fprintf('  CEQU constraints...\n'); end
    for j = 1:M
        for k = 1:K(j)
            row = zeros(1, nVars);
            row(Cidx(j, k, j)) = 1;
            row(Qidx(j, k)) = -1;
            Aeq = [Aeq; row];
            beq = [beq; 0];
        end
    end

    %% ONE1: sum over kj,hi,ni of (p1 + p1c) = 1 for each (j,i)
    if verbose; fprintf('  ONE1 constraints...\n'); end
    for j = 1:M
        for i = 1:M
            row = zeros(1, nVars);
            for kj = 1:K(j)
                for hi = 1:K(i)
                    for ni = 0:N
                        row(p1idx(j, kj, i, ni+1, hi)) = 1;
                        row(p1cidx(j, kj, i, ni+1, hi)) = 1;
                    end
                end
            end
            Aeq = [Aeq; row];
            beq = [beq; 1];
        end
    end

    %% UTLB: U(i,k) = sum over t,nt,h of p1(i,k,t,nt,h) for each (i,k,t)
    if verbose; fprintf('  UTLB constraints...\n'); end
    for i = 1:M
        for k = 1:K(i)
            for t = 1:M
                row = zeros(1, nVars);
                row(Uidx(i, k)) = 1;
                for nt = 0:N
                    for h = 1:K(t)
                        row(p1idx(i, k, t, nt+1, h)) = -1;
                    end
                end
                Aeq = [Aeq; row];
                beq = [beq; 0];
            end
        end
    end

    %% UTLC: IT(i,k) = sum over t,nt,h of p1c(i,k,t,nt,h) for each (i,k,t)
    if verbose; fprintf('  UTLC constraints...\n'); end
    for i = 1:M
        for k = 1:K(i)
            for t = 1:M
                row = zeros(1, nVars);
                row(ITidx(i, k)) = 1;
                for nt = 0:N
                    for h = 1:K(t)
                        row(p1cidx(i, k, t, nt+1, h)) = -1;
                    end
                end
                Aeq = [Aeq; row];
                beq = [beq; 0];
            end
        end
    end

    %% QLEN: Q(i,k) = sum over ni of ni*p1(i,k,i,ni,k) for each (i,k)
    if verbose; fprintf('  QLEN constraints...\n'); end
    for i = 1:M
        for k = 1:K(i)
            row = zeros(1, nVars);
            row(Qidx(i, k)) = 1;
            for ni = 0:N
                row(p1idx(i, k, i, ni+1, k)) = row(p1idx(i, k, i, ni+1, k)) - ni;
            end
            Aeq = [Aeq; row];
            beq = [beq; 0];
        end
    end

    %% CLEN: C(j,k,i) = sum over ni,h of ni*p1(j,k,i,ni,h) for each (j,k,i)
    if verbose; fprintf('  CLEN constraints...\n'); end
    for j = 1:M
        for k = 1:K(j)
            for i = 1:M
                row = zeros(1, nVars);
                row(Cidx(j, k, i)) = 1;
                for ni = 0:N
                    for h = 1:K(i)
                        row(p1idx(j, k, i, ni+1, h)) = row(p1idx(j, k, i, ni+1, h)) - ni;
                    end
                end
                Aeq = [Aeq; row];
                beq = [beq; 0];
            end
        end
    end

    %% ONE: sum over k of (U(j,k) + IT(j,k)) = 1 for each j
    if verbose; fprintf('  ONE constraints...\n'); end
    for j = 1:M
        row = zeros(1, nVars);
        for k = 1:K(j)
            row(Uidx(j, k)) = 1;
            row(ITidx(j, k)) = 1;
        end
        Aeq = [Aeq; row];
        beq = [beq; 1];
    end

    %% POPC: sum over i,k of Q(i,k) = N
    if verbose; fprintf('  POPC constraint...\n'); end
    row = zeros(1, nVars);
    for i = 1:M
        for k = 1:K(i)
            row(Qidx(i, k)) = 1;
        end
    end
    Aeq = [Aeq; row];
    beq = [beq; N];

    %% MPCB: sum over i of C(j,k,i) = N*U(j,k) for each (j,k)
    if verbose; fprintf('  MPCB constraints...\n'); end
    for j = 1:M
        for k = 1:K(j)
            row = zeros(1, nVars);
            for i = 1:M
                row(Cidx(j, k, i)) = 1;
            end
            row(Uidx(j, k)) = -N;
            Aeq = [Aeq; row];
            beq = [beq; 0];
        end
    end

    %% UUB1: sum over k of U(i,k) <= 1 for each i (inequality)
    if verbose; fprintf('  UUB1 constraints...\n'); end
    for i = 1:M
        row = zeros(1, nVars);
        for k = 1:K(i)
            row(Uidx(i, k)) = 1;
        end
        Aineq = [Aineq; row];
        bineq = [bineq; 1];
    end

    %% QUB1: Q(j,k) <= N*U(j,k) for each (j,k) (inequality)
    if verbose; fprintf('  QUB1 constraints...\n'); end
    for j = 1:M
        for k = 1:K(j)
            row = zeros(1, nVars);
            row(Qidx(j, k)) = 1;
            row(Uidx(j, k)) = -N;
            Aineq = [Aineq; row];
            bineq = [bineq; 0];
        end
    end

    %% Build objective function
    if verbose; fprintf('Building objective function...\n'); end
    c = zeros(nVars, 1);
    c(Uidx(objective_queue, objective_phase)) = 1;

    if strcmp(sense, 'max')
        c = -c;
    end

    %% Solve LP
    if verbose
        fprintf('Solving LP with %d variables and %d equality + %d inequality constraints...\n', ...
            nVars, size(Aeq, 1), size(Aineq, 1));
    end

    if verbose
        options = optimoptions('linprog', 'Display', 'final', 'Algorithm', 'interior-point');
    else
        options = optimoptions('linprog', 'Display', 'off', 'Algorithm', 'interior-point');
    end

    [x, fval, exitflag] = linprog(c, Aineq, bineq, Aeq, beq, lb, ub, options);

    if strcmp(sense, 'max')
        fval = -fval;
    end

    %% Extract results
    result = struct();
    result.objective = fval;
    result.exitflag = exitflag;

    if exitflag > 0
        % Compute utilizations
        result.U = zeros(M, maxK);
        result.IT = zeros(M, maxK);
        result.Q = zeros(M, maxK);

        for i = 1:M
            for k = 1:K(i)
                result.U(i, k) = x(Uidx(i, k));
                result.IT(i, k) = x(ITidx(i, k));
                result.Q(i, k) = x(Qidx(i, k));
            end
        end
    end

    if verbose; fprintf('\n=== Results ===\n'); end
    if verbose; fprintf('Objective value: %f\n', fval); end
    if verbose; fprintf('Exit flag: %d\n', exitflag); end
    if exitflag > 0 && verbose
        fprintf('\nUtilizations:\n');
        for i = 1:M
            fprintf('  Queue %d: U = [', i);
            for k = 1:K(i)
                fprintf('%.6f ', result.U(i, k));
            end
            fprintf(']\n');
        end
        fprintf('\nIdle times:\n');
        for i = 1:M
            fprintf('  Queue %d: IT = [', i);
            for k = 1:K(i)
                fprintf('%.6f ', result.IT(i, k));
            end
            fprintf(']\n');
        end
        fprintf('\nQueue lengths:\n');
        for i = 1:M
            fprintf('  Queue %d: Q = [', i);
            for k = 1:K(i)
                fprintf('%.6f ', result.Q(i, k));
            end
            fprintf(']\n');
        end
    end
end
