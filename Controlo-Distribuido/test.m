%The system
k11 = 70; k12 = 2.87; k21 = 2.80; k22 = 30;
K = [k11, k12 ; k21 , k22];

L1 = -10; o1 = 0.12; L2 = -10; o2 = 0.08;
%costs
c1 = 1; c2 = 2;
c = [c1 c2]; 
L = [L1;L2]; 
o = [o1;o2];

% SOLVE WITH CONSENSUS
rho = 0.07;
%node 1 initialization
node1.index = 1;
node1.d = [0;0];
node1.d_av = [0;0];
node1.y = [0;0];
node1.k = [k11;k12]; 
node1.n = norm(node1.k)^2;
node1.m = node1.n-k11^2;
node1.c = c1;
node1.o = o1;
node1.L = L1;

%node 2 initialization
node2.index = 2;
node2.d = [0;0];
node2.d_av = [0;0];
node2.y = [0;0];
node2.k = [k21;k22]; 
node2.n = norm(node2.k)^2;
node2.m = node2.n-k22^2;
node2.c = c2;
node2.o = o2;
node2.L = L2;

% d = [2,2];
% evaluate_cost(node1,d,rho);
% evaluate_cost(node2,d,rho);
% check_feasibility(node1, d);
% check_feasibility(node2, d);
clc;
[d5, c5] = primal_solve(node1,0.07)
[d5, c5] = primal_solve(node2,0.07)
