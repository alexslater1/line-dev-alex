function MOMENTS=dmap_moment(DMAP,ORDERS)
% MOMENTS=dmap_moment(DMAP,ORDERS) - Compute factorial moments of
% inter-arrival times for a discrete-time MAP
%
%  Input:
%  DMAP: a D-MAP in the form of {D0,D1}
%  ORDERS: set of moment orders (1=>E[X], 2=>E[X^2], ...)
%
%  Output:
%  MOMENTS: moments returned in the same order of ORDERS

D0=DMAP{1};
D1=DMAP{2};
N=size(D0,1);
e=ones(N,1);
P=inv(eye(N)-D0)*D1;
al=dtmc_solve(P);
for t=1:length(ORDERS)
    i=ORDERS(t);
    if isnan(D0)
        MOMENTS(t)=NaN;
    else
        A=(inv(eye(N)-D0)^i);
        MOMENTS(t)=factorial(i)*al*A*e;
    end
end
end
