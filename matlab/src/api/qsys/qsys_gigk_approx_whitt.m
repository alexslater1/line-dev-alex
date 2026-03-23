function [W,rhohat]=qsys_gigk_approx_whitt(lambda,mu,ca,cs,k)
% [W,RHOHAT]=QSYS_GIGK_APPROX_WHITT(LAMBDA,MU,CA,CS,K)
%
% G/G/k queue approximation using Whitt's QNA method.
% Uses the Queue Network Analyzer (QNA) methodology to provide accurate
% estimates for multi-server queues with general distributions.
%
% Inputs:
%   LAMBDA - Arrival rate
%   MU     - Service rate per server
%   CA     - Coefficient of variation of inter-arrival time
%   CS     - Coefficient of variation of service time
%   K      - Number of servers
%
% Returns:
%   W      - Average time in system (response time)
%   RHOHAT - Modified utilization (so that M/M/1 formulas still hold)

% Copyright (c) 2012-2026, Imperial College London
% All rights reserved.

ca2 = ca^2;
cs2 = cs^2;
rho = lambda / (k * mu);

% Get M/M/k baseline
W_mmk = qsys_mmk(lambda, mu, k);
Wq_mmk = W_mmk - 1/mu;

% Heavy traffic factor
if rho > 0.7
    htFactor = 1 + (4*(rho - 0.7))^2;
else
    htFactor = 1;
end

% Variability correction factor (Whitt's g function)
if ca2 >= 1 && cs2 >= 1
    g = 1;
elseif ca2 <= 1 && cs2 <= 1
    phi = (1 - ca2) * (1 - cs2) / (1 + cs2);
    g = 1 - phi * sqrt(k);
else
    g = (ca2 + cs2) / 2;
end

% QNA formula
Wq = Wq_mmk * ((ca2 + cs2) / 2) * g * htFactor;

% Ensure non-negative
Wq = max(Wq, 0);

W = Wq + 1/mu;
rhohat = W*lambda/(1+W*lambda); % so that M/M/1 formulas still hold

end
