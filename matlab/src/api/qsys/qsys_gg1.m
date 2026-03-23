function [W,rhohat]=qsys_gg1(lambda,mu,ca2,cs2)
% [W,RHOHAT]=QSYS_GG1(LAMBDA,MU,CA2,CS2) analyzes a G/G/1 queue.
%
% Uses exact methods for special cases (M/M/1, M/G/1, G/M/1) and
% Allen-Cunneen approximation for the general case.
%
% Inputs:
%   LAMBDA - Arrival rate
%   MU     - Service rate
%   CA2    - Squared coefficient of variation of inter-arrival time
%   CS2    - Squared coefficient of variation of service time
%
% Returns:
%   W      - Average time in system (response time)
%   RHOHAT - Modified utilization (so that M/M/1 formulas still hold)

% Copyright (c) 2012-2026, Imperial College London
% All rights reserved.

tol = 1e-8;

if abs(ca2 - 1) < tol && abs(cs2 - 1) < tol
    % M/M/1 case
    [W,rhohat] = qsys_mm1(lambda, mu);
elseif abs(ca2 - 1) < tol
    % M/G/1 case (ca2 = 1)
    [W,rhohat] = qsys_mg1(lambda, mu, sqrt(cs2));
elseif abs(cs2 - 1) < tol
    % G/M/1 case (cs2 = 1)
    rho = lambda / mu;
    W = qsys_gm1(rho, mu);
    rhohat = W * lambda / (1 + W * lambda);
else
    % General G/G/1 case - use Allen-Cunneen approximation
    [W,rhohat] = qsys_gig1_approx_allencunneen(lambda, mu, sqrt(ca2), sqrt(cs2));
end

end
