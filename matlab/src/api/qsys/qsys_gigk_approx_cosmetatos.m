function [W,rhohat]=qsys_gigk_approx_cosmetatos(lambda,mu,ca,cs,k)
% [W,RHOHAT]=QSYS_GIGK_APPROX_COSMETATOS(LAMBDA,MU,CA,CS,K)
%
% G/G/k queue approximation using the Cosmetatos method.
% Adjusts the M/M/k results based on the variability of arrival
% and service processes.
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

% Get M/M/k baseline waiting time
W_mmk = qsys_mmk(lambda, mu, k);

% Cosmetatos approximation: adjust M/M/k queue waiting time
% based on variability factor
Wq_mmk = W_mmk - 1/mu;
variabilityFactor = (ca^2 + cs^2) / 2;
Wq = Wq_mmk * variabilityFactor;
W = Wq + 1/mu;

rhohat = W*lambda/(1+W*lambda); % so that M/M/1 formulas still hold

end
