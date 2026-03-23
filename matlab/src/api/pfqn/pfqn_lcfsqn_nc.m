%{
 % @file pfqn_lcfsqn_nc.m
 % @brief Normalizing constant for LCFS queueing networks
 %
 % @author LINE Development Team
%}

%{
 % @brief Computes the normalizing constant for LCFS queueing networks
 % @fn pfqn_lcfsqn_nc(alpha, beta, N)
 % @param alpha Service rates at LCFS station (1xR vector).
 % @param beta Service rates at LCFS-PR station (1xR vector).
 % @param N Population vector (default: ones(1,R)).
 % @return G Normalizing constant.
 % @return Ax Cell array of A matrices for each state.
%}
function [G,Ax] = pfqn_lcfsqn_nc(alpha,beta,N)
% [G,AX] = PFQN_LCFSQN_NC(ALPHA, BETA, N)
% Normalizing constant for multiclass LCFS queueing networks
%
% This function computes the normalizing constant for a 2-station closed
% queueing network with:
%   - Station 1: LCFS (Last-Come-First-Served, non-preemptive)
%   - Station 2: LCFS-PR (LCFS with Preemption-Resume)
%
% Parameters:
%   alpha - vector of inverse service rates at station 1 (LCFS)
%           alpha(r) = 1/mu(1,r) for class r
%   beta  - vector of inverse service rates at station 2 (LCFS-PR)
%           beta(r) = 1/mu(2,r) for class r
%   N     - population vector, N(r) = number of jobs of class r
%
% Returns:
%   G  - normalizing constant
%   Ax - cell array of A matrices for each state x=0:K
%
% Reference:
%   G. Casale, "A family of multiclass LCFS queueing networks with
%   order-dependent product-form solutions", QUESTA 2026.
%
% Copyright (c) 2012-2026, Imperial College London
% All rights reserved.

K = sum(N);
R = length(N);
G = 0;
Ax=cell(1,K+1);
for x=0:K
    Ax{1+x} = make_A(alpha, beta, x, K, R);
    G = G + perm(Ax{1+x}, N);
end
end


function A = make_A(alpha, beta, x, K, R)
% alpha : vector of length R
% beta  : vector of length R
% x     : integer
% K     : matrix size
if issym(alpha)
    A = sym(zeros(K, K));
else
    A = zeros(K, K);
end
for i = 1:R
    for j = 1:x
        A(i, j) = alpha(i)^j;
    end
    for j = 1:(K-x)
        A(i, x+j) = alpha(i)^(x+j-1) * beta(i);
    end
end
end
