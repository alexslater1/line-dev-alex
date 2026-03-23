function [Q,U,R,T,C,X,lG,runtime,iter,method] = solver_nc_conv(sn, options)
% [Q,U,R,T,C,X,LG,RUNTIME,ITER,METHOD] = SOLVER_NC_CONV(SN, OPTIONS)
%
% Exact normalizing constant solver for closed networks with Limited
% Joint Class Dependence (LJCD) service rates, using the multichain
% convolution algorithm of Sauer (1983), Section 5.2.
%
% This solver handles models where some stations have LJCD scaling
% (e.g., Flow-Equivalent Servers from aggregateFES).

% Copyright (c) 2012-2026, Imperial College London
% All rights reserved.

Tstart = tic;
method = 'conv';
iter = 1;

M = sn.nstations;
K = sn.nclasses;
NK = sn.njobs';
nservers = sn.nservers;

V = cellsum(sn.visits);
ST = 1 ./ sn.rates;
ST(isnan(ST)) = 0;

% Demands: L(ist,k) = V(ist,k) * ST(ist,k)
Ldemand = V .* ST;

% Separate delay and queue stations
isDelay = isinf(nservers);
delayIdx = find(isDelay);
queueIdx = find(~isDelay);
nQueues = length(queueIdx);

% Build pfqn_conv inputs
% Z: total delay demand per class
Z_conv = zeros(1, K);
for ist = delayIdx(:)'
    Z_conv = Z_conv + Ldemand(ist, :);
end

% L_conv: demands for queue stations only
L_conv = Ldemand(queueIdx, :);

% mu_conv: load-dependent rates for queue stations (ones for LI, lldscaling otherwise)
Nt = sum(NK);
mu_conv = ones(nQueues, Nt);
if ~isempty(sn.lldscaling)
    for qi = 1:nQueues
        ist = queueIdx(qi);
        nEntries = min(Nt, size(sn.lldscaling, 2));
        mu_conv(qi, 1:nEntries) = sn.lldscaling(ist, 1:nEntries);
    end
end

% LJCD scaling tables for queue stations
ljcdscaling_conv = cell(nQueues, 1);
ljcdcutoffs_conv = cell(nQueues, 1);
hasLjcd = ~isempty(sn.ljcdscaling);
if hasLjcd
    for qi = 1:nQueues
        ist = queueIdx(qi);
        if ist <= length(sn.ljcdscaling) && ~isempty(sn.ljcdscaling{ist})
            ljcdscaling_conv{qi} = sn.ljcdscaling{ist};
            ljcdcutoffs_conv{qi} = sn.ljcdcutoffs{ist};
        end
    end
end

%% Compute G(N)
[G_N, lG] = pfqn_conv(L_conv, NK, Z_conv, mu_conv, ljcdscaling_conv, ljcdcutoffs_conv);

%% Compute G(N - e_k) for each class -> throughput
XN = zeros(1, K);
for k = 1:K
    if NK(k) > 0
        NK_minus = NK;
        NK_minus(k) = NK_minus(k) - 1;
        [G_Nk, ~] = pfqn_conv(L_conv, NK_minus, Z_conv, mu_conv, ljcdscaling_conv, ljcdcutoffs_conv);
        XN(k) = G_Nk / G_N;
    end
end

%% Compute per-station throughput
TN = V .* repmat(XN, M, 1);

%% Compute queue lengths
QN = zeros(M, K);

% Delay stations: Q = L * X
for ist = delayIdx(:)'
    for k = 1:K
        QN(ist, k) = Ldemand(ist, k) * XN(k);
    end
end

% Queue stations: use marginal distribution
% P_m(n|N) = X_m(n) * G_{-m}(N-n) / G(N)
% Q_m_k = sum_{n: n_k>=1} n_k * P_m(n|N)
%
% G_{-m}(N-n) is computed by pfqn_conv on all stations except m
stateSpaceSize = prod(NK + 1);

for qi = 1:nQueues
    ist = queueIdx(qi);

    % Build X_m(n) for this station
    Xm = zeros(stateSpaceSize, 1);
    Xm(1) = 1; % X_m(0) = 1
    isLjcdStation = ~isempty(ljcdscaling_conv{qi});

    n = pprod_init(NK);
    while n(1) >= 0
        idx = hashpop(n, NK);
        if sum(n) > 0
            if isLjcdStation
                % LJCD: X_m(n) = (L/mu_km(n)) * X_m(n-e_k) via eq. (40)
                % Pick any k with n_k > 0 (result is path-independent)
                for r = 1:K
                    if n(r) > 0
                        cutoffs = ljcdcutoffs_conv{qi};
                        nClamped = min(n, cutoffs);
                        lidx = ljd_linearize(nClamped, cutoffs);
                        mu_km = ljcdscaling_conv{qi}{r}(lidx);
                        n(r) = n(r) - 1;
                        idx_prev = hashpop(n, NK);
                        n(r) = n(r) + 1;
                        if mu_km > 0
                            Xm(idx) = (L_conv(qi, r) / mu_km) * Xm(idx_prev);
                        end
                        break
                    end
                end
            else
                % LI: X_m(n) = Σ_r L(m,r) * X_m(n-e_r) (multinomial form)
                for r = 1:K
                    if n(r) > 0
                        n(r) = n(r) - 1;
                        idx_prev = hashpop(n, NK);
                        n(r) = n(r) + 1;
                        Xm(idx) = Xm(idx) + L_conv(qi, r) * Xm(idx_prev);
                    end
                end
            end
        end
        n = pprod_next(n, NK);
    end

    % Build complement: all stations except qi
    L_comp = L_conv; L_comp(qi, :) = [];
    mu_comp = mu_conv; mu_comp(qi, :) = [];
    ljcd_comp = ljcdscaling_conv; ljcd_comp(qi) = [];
    ljcdc_comp = ljcdcutoffs_conv; ljcdc_comp(qi) = [];

    % Compute Q_m_k using marginal
    n = pprod_init(NK);
    while n(1) >= 0
        if any(n > 0)
            idx = hashpop(n, NK);
            nmi = NK - n;
            if all(nmi >= 0)
                % G_{-m}(N-n) with delay
                [G_comp, ~] = pfqn_conv(L_comp, nmi, Z_conv, mu_comp, ljcd_comp, ljcdc_comp);
                prob = Xm(idx) * G_comp / G_N;
                for k = 1:K
                    QN(ist, k) = QN(ist, k) + n(k) * prob;
                end
            end
        end
        n = pprod_next(n, NK);
    end
end

%% Compute remaining metrics
RN = QN ./ TN;
RN(TN == 0) = 0;
UN = TN .* ST;
CN = NK ./ XN;
CN(XN == 0) = 0;
CN = CN - sum(Z_conv .* (repmat(1, M, 1) .* isDelay(:)), 1); % subtract delay

% Output
Q = QN;
U = UN;
R = RN;
T = TN;
C = CN;
X = XN;
runtime = toc(Tstart);
end

%% --- Local helper functions ---

function idx = hashpop(n, N)
idx = 1;
R = length(N);
for r = 1:R
    idx = idx + prod(N(1:r-1) + 1) * n(r);
end
end

function n = pprod_init(N)
n = zeros(size(N));
end

function n = pprod_next(n, N)
R = length(N);
if all(n == N)
    n = -1 * ones(1, R);
    return
end
s = R;
while s > 0 && n(s) == N(s)
    n(s) = 0;
    s = s - 1;
end
if s > 0
    n(s) = n(s) + 1;
end
end
