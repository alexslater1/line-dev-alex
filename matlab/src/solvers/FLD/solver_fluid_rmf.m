function [QN, UN, RN, TN, xvec_iter, QNt, UNt, TNt, xvec_t, t] = solver_fluid_rmf(sn, options)
% SOLVER_FLUID_RMF Refined mean field solver for multi-list cache analysis
%
% Implements mean field and refined mean field (1/N correction) analysis
% for multi-list caches with RANDOM(m) replacement policy, based on the
% DDPP (Density-Dependent Population Process) framework.
%
% The cache is modeled as a population process where each item can be in
% one of h+1 states: list 1, list 2, ..., list h, or outside the cache
% (list 0). Transitions occur when an item is requested: the requested
% item moves up (toward list 1) while a displaced item moves down
% (toward list 0).
%
% Reference:
%   N. Gast, "Expected Values Estimated via Mean-Field Approximation are
%   1/N-Accurate", Proc. ACM Meas. Anal. Comput. Syst., 2017.

% Copyright (c) 2012-2026, Imperial College London
% All rights reserved.

M = sn.nstations;
K = sn.nclasses;

% Initialize output arrays
QN = NaN(M, K);
UN = NaN(M, K);
RN = NaN(M, K);
TN = NaN(M, K);

% Initialize transient outputs
QNt = cell(M, K);
UNt = cell(M, K);
TNt = cell(M, K);
for ist = 1:M
    for r = 1:K
        QNt{ist, r} = zeros(2, 1);
        UNt{ist, r} = zeros(2, 1);
        TNt{ist, r} = zeros(2, 1);
    end
end
t = [0; 1];
xvec_t = zeros(2, 1);
xvec_iter = {zeros(1, 1)};

% Scan for cache nodes
for ind = 1:sn.nnodes
    if sn.nodetype(ind) ~= NodeType.Cache
        continue
    end
    ch = sn.nodeparam{ind};
    if ch.nitems == 0
        continue
    end

    % Extract cache parameters
    n_items = ch.nitems;
    itemcap = ch.itemcap;
    pread = ch.pread;
    h = length(itemcap);   % number of lists
    m = itemcap(:)';       % row vector of list capacities

    % Initialize actual hit/miss probability arrays if needed
    if ~isfield(ch, 'actualhitprob') || isempty(ch.actualhitprob)
        ch.actualhitprob = zeros(1, K);
    end
    if ~isfield(ch, 'actualmissprob') || isempty(ch.actualmissprob)
        ch.actualmissprob = zeros(1, K);
    end

    % Process each class
    for r = 1:K
        if r > length(pread) || isempty(pread{r}) || all(isnan(pread{r}))
            continue
        end

        p = pread{r}(:)';
        if length(p) ~= n_items
            continue
        end

        % Normalize popularity distribution
        p_sum = sum(p);
        if p_sum <= 0
            continue
        end
        p = p / p_sum;

        % Build and solve the DDPP cache model
        model_dim = n_items * (h + 1);

        % Build initial state: first m(1) items in list 1, next m(2) in
        % list 2, etc.; remaining items outside cache (list 0)
        x0 = zeros(model_dim, 1);
        obj_idx = 0;
        for k = 1:h
            for jj = 1:m(k)
                obj_idx = obj_idx + 1;
                if obj_idx <= n_items
                    x0(rmf_index(obj_idx, k, n_items)) = 1.0;
                end
            end
        end
        for i = (obj_idx + 1):n_items
            x0(rmf_index(i, 0, n_items)) = 1.0;
        end

        % Compute mean field fixed point by ODE integration
        pi = rmf_fixed_point(x0, p, m, n_items, h, model_dim);

        % Compute hit/miss probabilities from fixed point
        hit_prob = 0.0;
        for k = 1:h
            hit_prob = hit_prob + rmf_hit_rate(pi, p, k, n_items);
        end
        miss_prob = rmf_hit_rate(pi, p, 0, n_items);

        % Try refined mean field (1/N correction)
        try
            [pi_mf, V] = rmf_expansion_steady_state(x0, p, m, n_items, h, model_dim);
            pi_refined = pi_mf + V / n_items;

            % Recompute hit/miss with refined approximation
            hit_prob_refined = 0.0;
            for k = 1:h
                hit_prob_refined = hit_prob_refined + rmf_hit_rate(pi_refined, p, k, n_items);
            end
            miss_prob_refined = rmf_hit_rate(pi_refined, p, 0, n_items);
            hit_prob = hit_prob_refined;
            miss_prob = miss_prob_refined;
        catch
            % Fall back to plain mean field
        end

        % Store actual hit/miss probabilities, clipped to [0, 1]
        ch.actualhitprob(r) = max(0, min(1, hit_prob));
        ch.actualmissprob(r) = max(0, min(1, miss_prob));
    end

    % Store back
    sn.nodeparam{ind} = ch;
end

end

%% ========================================================================
%  Helper functions for DDPP cache model
%  ========================================================================

function idx = rmf_index(i, k, n_items)
% RMF_INDEX Map (item i, list k) to flat state index
%   i: item index (1-based)
%   k: list index (0 = outside cache, 1..h = cache lists)
%   n_items: total number of items
idx = i + k * n_items;
end

function hr = rmf_hit_rate(x, p, list_number, n_items)
% RMF_HIT_RATE Compute hit rate contribution from a specific list
%   hr = sum_i p(i) * x(index(i, list_number))
hr = 0.0;
for i = 1:n_items
    hr = hr + p(i) * x(rmf_index(i, list_number, n_items));
end
end

function dX = rmf_drift(x, p, m, n_items, h, model_dim)
% RMF_DRIFT Compute mean field drift F(x) for RANDOM(m) replacement
%
% dx[i,k]/dt = -p(i)*x[i,k] + hitRate[k]*x[i,k+1]/m(k)  (promotion from k to k+1)
% dx[i,k+1]/dt = p(i)*x[i,k] - hitRate[k]*x[i,k+1]/m(k)

hit_rates = zeros(1, h + 1);
for k = 0:h
    hit_rates(k + 1) = rmf_hit_rate(x, p, k, n_items);
end

dX = zeros(model_dim, 1);
for i = 1:n_items
    for k = 0:(h - 1)
        flow = p(i) * x(rmf_index(i, k, n_items)) ...
             - hit_rates(k + 1) * x(rmf_index(i, k + 1, n_items)) / m(k + 1);
        dX(rmf_index(i, k, n_items))     = dX(rmf_index(i, k, n_items))     - flow;
        dX(rmf_index(i, k + 1, n_items)) = dX(rmf_index(i, k + 1, n_items)) + flow;
    end
end
end

function Fp = rmf_jacobian(x, p, m, n_items, h, model_dim)
% RMF_JACOBIAN Compute Jacobian dF/dx at state x

hit_rates = zeros(1, h + 1);
for k = 0:h
    hit_rates(k + 1) = rmf_hit_rate(x, p, k, n_items);
end

Fp = zeros(model_dim, model_dim);
for i = 1:n_items
    for k = 0:(h - 1)
        ik  = rmf_index(i, k, n_items);
        ik1 = rmf_index(i, k + 1, n_items);

        % Direct rate terms
        Fp(ik,  ik)  = Fp(ik,  ik)  - p(i);
        Fp(ik1, ik)  = Fp(ik1, ik)  + p(i);
        Fp(ik,  ik1) = Fp(ik,  ik1) + hit_rates(k + 1) / m(k + 1);
        Fp(ik1, ik1) = Fp(ik1, ik1) - hit_rates(k + 1) / m(k + 1);

        % Indirect terms via hit rate dependence on x(j,k)
        for j = 1:n_items
            jk  = rmf_index(j, k, n_items);
            jk1 = rmf_index(j, k + 1, n_items);
            Fp(ik,  jk1) = Fp(ik,  jk1) - p(i) * x(ik) / m(k + 1);
            Fp(ik1, jk1) = Fp(ik1, jk1) + p(i) * x(ik) / m(k + 1);
            Fp(ik,  jk)  = Fp(ik,  jk)  + p(j) * x(ik1) / m(k + 1);
            Fp(ik1, jk)  = Fp(ik1, jk)  - p(j) * x(ik1) / m(k + 1);
        end
    end
end
end

function Fpp = rmf_hessian(~, p, m, n_items, h, model_dim)
% RMF_HESSIAN Compute Hessian d^2F/dx^2 (constant for this quadratic drift)

Fpp = zeros(model_dim, model_dim, model_dim);
for i = 1:n_items
    for k = 0:(h - 1)
        ik  = rmf_index(i, k, n_items);
        ik1 = rmf_index(i, k + 1, n_items);
        for j = 1:n_items
            if j ~= i
                jk  = rmf_index(j, k, n_items);
                jk1 = rmf_index(j, k + 1, n_items);
                % d^2 F[ik] / (d x[jk] d x[ik1])
                Fpp(ik,  jk,  ik1) = Fpp(ik,  jk,  ik1) + p(j) / m(k + 1);
                Fpp(ik,  ik1, jk)  = Fpp(ik,  ik1, jk)  + p(j) / m(k + 1);
                % d^2 F[ik] / (d x[jk1] d x[ik])
                Fpp(ik,  jk1, ik)  = Fpp(ik,  jk1, ik)  - p(i) / m(k + 1);
                Fpp(ik,  ik,  jk1) = Fpp(ik,  ik,  jk1) - p(i) / m(k + 1);
                % Symmetric for ik1
                Fpp(ik1, jk,  ik1) = Fpp(ik1, jk,  ik1) - p(j) / m(k + 1);
                Fpp(ik1, ik1, jk)  = Fpp(ik1, ik1, jk)  - p(j) / m(k + 1);
                Fpp(ik1, jk1, ik)  = Fpp(ik1, jk1, ik)  + p(i) / m(k + 1);
                Fpp(ik1, ik,  jk1) = Fpp(ik1, ik,  jk1) + p(i) / m(k + 1);
            end
        end
    end
end
end

function Q = rmf_noise_matrix(x, p, m, n_items, h, model_dim)
% RMF_NOISE_MATRIX Compute noise intensity matrix Q(x) for the DDPP
%
% Q[a,b] = sum_ell ell[a]*ell[b]*beta_ell(x)
% Each transition swaps items i and j across lists k and k+1.

Q = zeros(model_dim, model_dim);
signs = [-1, 1, 1, -1];
for i = 1:n_items
    for k = 0:(h - 1)
        for j = 1:n_items
            rate = p(i) * x(rmf_index(i, k, n_items)) ...
                 * x(rmf_index(j, k + 1, n_items)) / m(k + 1);
            indices = [rmf_index(i, k, n_items), ...
                       rmf_index(j, k, n_items), ...
                       rmf_index(i, k + 1, n_items), ...
                       rmf_index(j, k + 1, n_items)];
            for ia = 1:4
                for ib = 1:4
                    Q(indices(ia), indices(ib)) = Q(indices(ia), indices(ib)) ...
                        + rate * signs(ia) * signs(ib);
                end
            end
        end
    end
end
end

function pi = rmf_fixed_point(x0, p, m, n_items, h, model_dim)
% RMF_FIXED_POINT Compute mean field fixed point by ODE integration
%
% Integrates dx/dt = F(x) until steady state using ode15s.

tmax = 10000;
ode_func = @(t, x) rmf_drift(x, p, m, n_items, h, model_dim);
odeopt = odeset('AbsTol', 1e-10, 'RelTol', 1e-8);
[~, xvec] = ode15s(ode_func, [0, tmax], x0, odeopt);
%[~, xvec] = lsoda_solve(ode_func, [0, tmax], x0, odeopt);
pi = xvec(end, :)';
end

function [C, Cinv, rk] = rmf_dimension_reduction(Fp, n_items, h, model_dim)
% RMF_DIMENSION_REDUCTION Compute change-of-basis for singular Jacobian
%
% The Jacobian is singular because item populations are conserved
% (sum over lists for each item = 1). Returns matrices to project
% onto the non-singular subspace.

rk = rank(Fp);

C = zeros(model_dim, model_dim);
d = 0;
for l_idx = 0:h
    for i = 1:(n_items - 1)
        d = d + 1;
        C(d, rmf_index(i, l_idx, n_items)) = 1.0;
    end
end

[U, ~, ~] = svd(Fp);
C((rk + 1):model_dim, :) = U(:, (rk + 1):model_dim)';
Cinv = inv(C);
end

function [pi, V] = rmf_expansion_steady_state(x0, p, m, n_items, h, model_dim)
% RMF_EXPANSION_STEADY_STATE Compute refined mean field steady-state expansion
%
% Computes the mean field fixed point pi and the 1/N correction V
% using the Lyapunov equation approach with dimension reduction.
%
% The refined approximation for a system of N items is:
%   E[X] ~ pi + V/N + O(1/N^2)

pi = rmf_fixed_point(x0, p, m, n_items, h, model_dim);

Fp  = rmf_jacobian(pi, p, m, n_items, h, model_dim);
Fpp = rmf_hessian(pi, p, m, n_items, h, model_dim);
Q   = rmf_noise_matrix(pi, p, m, n_items, h, model_dim);

% Dimension reduction: project onto non-singular subspace
[C, Cinv, rk] = rmf_dimension_reduction(Fp, n_items, h, model_dim);

Fp_r = (C * Fp * Cinv);
Fp_r = Fp_r(1:rk, 1:rk);

% Reduce Hessian: Fpp_r(a,b,c) = sum_{i,j,k} C(a,i)*Fpp(i,j,k)*Cinv(j,b)*Cinv(k,c)
% First contraction: tmp1(a,j,k) = sum_i C(a,i)*Fpp(i,j,k)
tmp1 = zeros(model_dim, model_dim, model_dim);
for a = 1:rk
    for j = 1:model_dim
        for k = 1:model_dim
            tmp1(a, j, k) = C(a, :) * Fpp(:, j, k);
        end
    end
end
% Second contraction: tmp2(a,b,k) = sum_j tmp1(a,j,k)*Cinv(j,b)
tmp2 = zeros(rk, rk, model_dim);
for a = 1:rk
    for b = 1:rk
        for k = 1:model_dim
            tmp2(a, b, k) = tmp1(a, :, k) * Cinv(:, b);
        end
    end
end
% Third contraction: Fpp_r(a,b,c) = sum_k tmp2(a,b,k)*Cinv(k,c)
Fpp_r = zeros(rk, rk, rk);
for a = 1:rk
    for b = 1:rk
        Fpp_r(a, b, :) = reshape(tmp2(a, b, :), 1, []) * Cinv(:, 1:rk);
    end
end

Q_r = C * Q * C';
Q_r = Q_r(1:rk, 1:rk);

% Solve Lyapunov equation: Fp_r * W_r + W_r * Fp_r' + Q_r = 0
W_r = lyap(Fp_r, Q_r);

% First-order correction: V_r = -Fp_r \ (C_r / 2)
% where C_r = sum_{b,c} Fpp_r(:,b,c) * W_r(b,c)
C_r = zeros(rk, 1);
for a = 1:rk
    for b = 1:rk
        for c = 1:rk
            C_r(a) = C_r(a) + Fpp_r(a, b, c) * W_r(b, c);
        end
    end
end
V_r = -Fp_r \ (C_r / 2.0);

% Expand back to full dimension
V = Cinv(:, 1:rk) * V_r;
end
