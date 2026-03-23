function [QN,UN,RN,TN,CN,XN,runtime] = solver_ctmc_qrf_analyzer(sn, options)
% SOLVER_CTMC_QRF_ANALYZER Adapter for QRF library functions within SolverCTMC
%
% [QN,UN,RN,TN,CN,XN,RUNTIME] = SOLVER_CTMC_QRF_ANALYZER(SN, OPTIONS)
%
% Bridges the LINE sn struct to QRF (Quadratic Reduction Framework) library
% functions for approximating performance metrics of single-class closed
% queueing networks with PH service.
%
% Copyright (c) 2012-2026, Imperial College London
% All rights reserved.

Tstart = tic;

M = sn.nstations;
K = sn.nclasses;
N = sum(sn.njobs);
S = sn.nservers;
PH = sn.proc;

% QRF only supports single-class closed networks
if K ~= 1
    line_error(mfilename, 'QRF methods only support single-class networks (found %d classes).', K);
end
if any(isinf(sn.njobs))
    line_error(mfilename, 'QRF methods only support closed networks.');
end

% Extract MAPs as cell array: MAPs{i} = {D0, D1}
MAPs = cell(M, 1);
K_phases = zeros(M, 1);
for i = 1:M
    if ~isempty(PH{i}{1})
        MAPs{i} = PH{i}{1};
        K_phases(i) = size(MAPs{i}{1}, 1);
    else
        K_phases(i) = 1;
        MAPs{i} = {-1, 1}; % fallback exponential rate 1
    end
end

% Build routing matrix (M x M) from sn.rt (MK x MK)
rt = zeros(M);
for i = 1:M
    for j = 1:M
        rt(i,j) = sn.rt(i, j); % K=1, so indexing is direct
    end
end

% Extract mu and v arrays from MAPs
Kmax = max(K_phases);
mu = zeros(M, Kmax, Kmax);
v = zeros(M, Kmax, Kmax);
for i = 1:M
    D0 = MAPs{i}{1};
    D1 = MAPs{i}{2};
    for h = 1:K_phases(i)
        for k = 1:K_phases(i)
            mu(i, h, k) = D1(h, k);
            if h == k
                v(i, k, h) = 0;
            else
                v(i, k, h) = D0(h, k);
            end
        end
    end
end

% Dispatch based on method
switch options.method
    case 'qrf.mmi'
        MR = 1;
        [UN_qrf, QN_qrf] = qrf_noblo_mmi(M, MR, K_phases(:)', N, mu, v, rt);

    case 'qrf.mem'
        [UN_qrf, QN_qrf] = qrf_noblo_mem(MAPs, N, rt);

    case 'qrf.mmi.ld'
        if isfield(options.config, 'qrf_alpha') && ~isempty(options.config.qrf_alpha)
            alpha = options.config.qrf_alpha;
        else
            alpha = ones(M, N);
        end
        [UN_qrf, QN_qrf] = qrf_noblo_mmi_ld(MAPs, N, rt, alpha);

    case 'qrf.mmi.linear'
        if isfield(options.config, 'qrf_alpha') && ~isempty(options.config.qrf_alpha)
            alpha = options.config.qrf_alpha;
        else
            alpha = ones(M, N);
        end
        [UN_qrf, QN_qrf] = qrf_noblo_mmi_linear(MAPs, N, rt, alpha);

    case 'qrf.bas.mmi'
        qp = options.config.qrf_params;
        if isempty(qp)
            line_error(mfilename, 'qrf.bas.mmi requires options.config.qrf_params with fields: f, MR, BB, F.');
        end
        f = qp.f;
        MR = qp.MR;
        BB = qp.BB;
        F = qp.F;
        [UN_qrf, QN_qrf] = qrf_bas_mmi_simple(f, M, MR, BB, K_phases(:)', F, N, mu, v, rt);

    case 'qrf.bas.mem'
        qp = options.config.qrf_params;
        if isempty(qp)
            line_error(mfilename, 'qrf.bas.mem requires options.config.qrf_params with fields: f, MR, MM, MM1, ZZ, ZM, BB, F.');
        end
        f = qp.f;
        MR = qp.MR;
        MM = qp.MM;
        MM1 = qp.MM1;
        ZZ = qp.ZZ;
        ZM = qp.ZM;
        BB = qp.BB;
        F = qp.F;
        [UN_qrf, QN_qrf] = qrf_bas_mem(f, M, MR, MM, MM1, ZZ, ZM, BB, K_phases(:)', F, N, mu, v, rt);

    case 'qrf.bas'
        params = sn_to_qrf_params(sn, MAPs, K_phases, N, mu, v, rt, options);
        params.verbose = options.verbose > 0;
        [result] = qrf_bas(params);
        % qrf_bas returns utilization bounds; derive QN via visit ratios
        [UN_qrf, QN_qrf] = derive_qn_from_bounds(result.U(:)', M, N, S, PH, sn);

    case 'qrf.rsrd'
        params = sn_to_qrf_params(sn, MAPs, K_phases, N, mu, v, rt, options);
        params.verbose = options.verbose > 0;
        [result] = qrf_rsrd(params);
        [UN_qrf, QN_qrf] = derive_qn_from_bounds(result.U(:)', M, N, S, PH, sn);

    otherwise
        line_error(mfilename, 'Unknown QRF method: %s', options.method);
end

% Normalize QN to population constraint
if sum(QN_qrf) > 0
    QN_qrf = QN_qrf / sum(QN_qrf) * N;
end

% Map 1D QRF results to M x K matrices (K=1)
UN = zeros(M, K);
QN = zeros(M, K);
TN = zeros(M, K);
RN = zeros(M, K);
XN = zeros(1, K);
CN = zeros(1, K);

QN(:, 1) = QN_qrf(:);

% Derive all metrics from QN using Little's law and visit ratios.
% QRF's "UN" is the sum of phase effective utilizations, which can exceed 1
% for multi-phase PH service; it does not match LINE's utilization definition.
% Instead, we derive throughput from QN at a delay station (if present) or
% from the visit-ratio weighted relationship, then compute UN = TN * stime / S.

% Compute visit ratios
if isfield(sn, 'visits') && ~isempty(sn.visits)
    V = cellsum(sn.visits);
else
    [visits] = sn_refresh_visits(sn, sn.chains, sn.rt, sn.rtnodes);
    V = cellsum(visits);
end

refstat = sn.refstat(1);

% Find system throughput XN from QN using Little's law at reference station
stime_ref = 0;
if ~isempty(PH{refstat}{1})
    stime_ref = map_mean(PH{refstat}{1});
end
if stime_ref > 0 && V(refstat, 1) > 0
    % QN(refstat) = XN * V(refstat,1) * stime_ref (for INF server at delay)
    % QN(refstat) = TN(refstat) * RN(refstat) in general
    % For delay: RN = stime, so QN = TN * stime = XN * V(refstat,1) * stime
    XN(1) = QN(refstat, 1) / (V(refstat, 1) * stime_ref);
end

% Derive per-station metrics from XN and visit ratios
for i = 1:M
    if ~isempty(PH{i}{1})
        stime = map_mean(PH{i}{1});
        TN(i, 1) = XN(1) * V(i, 1);
        if isinf(S(i))
            % Delay (infinite server): UN = QN by LINE convention
            UN(i, 1) = QN(i, 1);
        else
            % Finite server: UN = TN * stime / S
            if stime > 0
                UN(i, 1) = TN(i, 1) * stime / S(i);
            end
        end
        % Response time via Little's law
        if TN(i, 1) > 0
            RN(i, 1) = QN(i, 1) / TN(i, 1);
        end
    end
end

% Cycle time
if XN(1) > 0
    CN(1) = N / XN(1);
end

% Clean NaN values
QN(isnan(QN)) = 0;
UN(isnan(UN)) = 0;
RN(isnan(RN)) = 0;
TN(isnan(TN)) = 0;
XN(isnan(XN)) = 0;
CN(isnan(CN)) = 0;

runtime = toc(Tstart);
end

function params = sn_to_qrf_params(sn, MAPs, K_phases, N, mu, v, rt, options)
% Build params struct for qrf_bas / qrf_rsrd from sn struct
M = sn.nstations;
params = struct();
params.M = M;
params.N = N;
params.K = K_phases(:);
params.r = rt;

% Convert mu/v from 3D arrays to cell arrays expected by qrf_bas/qrf_rsrd
% Each mu{i}, v{i} must be a [Ki x Ki] matrix (even for Ki=1)
params.mu = cell(M, 1);
params.v = cell(M, 1);
for i = 1:M
    Ki = K_phases(i);
    params.mu{i} = reshape(mu(i, 1:Ki, 1:Ki), Ki, Ki);
    params.v{i} = reshape(v(i, 1:Ki, 1:Ki), Ki, Ki);
end

% Capacity
params.F = zeros(M, 1);
for i = 1:M
    if isfield(sn, 'cap') && ~isempty(sn.cap)
        params.F(i) = sn.cap(i);
        if isinf(params.F(i))
            params.F(i) = N;
        end
    else
        params.F(i) = N;
    end
end

% Blocking parameters from options or auto-infer
qp = options.config.qrf_params;
if ~isempty(qp)
    % Validate required fields for BAS blocking configuration
    if strcmp(options.method, 'qrf.bas')
        required_bas = {'f', 'MR', 'BB', 'MM', 'MM1', 'ZZ', 'ZM'};
        for idx = 1:length(required_bas)
            if ~isfield(qp, required_bas{idx})
                line_error(mfilename, 'qrf_params must contain field ''%s'' for qrf.bas method.', required_bas{idx});
            end
        end
    end
    if isfield(qp, 'f'), params.f = qp.f; end
    if isfield(qp, 'MR'), params.MR = qp.MR; end
    if isfield(qp, 'BB'), params.BB = qp.BB; end
    if isfield(qp, 'MM'), params.MM = qp.MM; end
    if isfield(qp, 'ZZ'), params.ZZ = qp.ZZ; end
    if isfield(qp, 'ZM'), params.ZM = qp.ZM; end
    if isfield(qp, 'MM1'), params.MM1 = qp.MM1; end
else
    if strcmp(options.method, 'qrf.bas')
        line_warning(mfilename, 'qrf_params not provided for qrf.bas; using no-blocking defaults (MR=1).');
    end
    % Default: no blocking
    params.f = 1;
    params.MR = 1;
    params.BB = zeros(1, M);
    params.MM = zeros(1, 2);
    params.ZZ = 0;
    params.ZM = 0;
    params.MM1 = zeros(1, M);
end

% Load-dependent alpha
if isfield(options.config, 'qrf_alpha') && ~isempty(options.config.qrf_alpha)
    alpha_mat = options.config.qrf_alpha;
    params.alpha = cell(M, 1);
    for i = 1:M
        params.alpha{i} = alpha_mat(i, :)';
    end
end
end

function [UN_qrf, QN_qrf] = derive_qn_from_bounds(U_bounds, M, N, S, PH, sn)
% Derive QN from utilization bounds using visit ratios and Little's law.
% For bounds methods (qrf.bas, qrf.rsrd), only utilization is returned.
% We compute system throughput from the utilization at queue stations,
% then derive QN at all stations via Little's law.
UN_qrf = U_bounds;

% Compute visit ratios
if isfield(sn, 'visits') && ~isempty(sn.visits)
    V = cellsum(sn.visits);
else
    [visits] = sn_refresh_visits(sn, sn.chains, sn.rt, sn.rtnodes);
    V = cellsum(visits);
end

% Find XN from the first finite-server station with nonzero utilization
XN_est = 0;
for i = 1:M
    if ~isinf(S(i)) && ~isempty(PH{i}{1}) && U_bounds(i) > 0 && V(i,1) > 0
        stime = map_mean(PH{i}{1});
        if stime > 0
            % UN = XN * V(i) * stime / S(i), so XN = UN * S(i) / (V(i) * stime)
            XN_est = U_bounds(i) * S(i) / (V(i, 1) * stime);
            break;
        end
    end
end

% Derive QN at each station: QN(i) = XN * V(i) * RN(i)
% For single-server queues: RN(i) >= stime(i) (at least one service time)
% Use Little's law: QN(i) = TN(i) * RN(i) where TN(i) = XN * V(i)
QN_qrf = zeros(1, M);
for i = 1:M
    if ~isempty(PH{i}{1})
        stime = map_mean(PH{i}{1});
        TN_i = XN_est * V(i, 1);
        if isinf(S(i))
            % Delay: QN = TN * stime
            QN_qrf(i) = TN_i * stime;
            UN_qrf(i) = QN_qrf(i); % LINE convention for INF server
        else
            % Queue: QN = TN * stime / (1 - U) for M/G/1-like estimate
            if U_bounds(i) < 1
                QN_qrf(i) = TN_i * stime / (1 - U_bounds(i));
            else
                QN_qrf(i) = N; % saturated
            end
        end
    end
end

% Rescale to population constraint
if sum(QN_qrf) > 0
    QN_qrf = QN_qrf / sum(QN_qrf) * N;
end
end
