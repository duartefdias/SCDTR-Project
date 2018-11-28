
close all; 
clear all;
%The system
k11 = 70; k12 = 2.87; k21 = 2.80; k22 = 30;
K = [k11, k12 ; k21 , k22];

% case 1
%L1 = 150; o1 = 30; L2 = 80; o2 = 0;

% case 2
%L1 = 80; o1 = 50; L2 = 150; o2 = 50;

% case 3
L1 = 200; o1 = 0.12; L2 = 100; o2 = 0.08;

%symmetric costs
%c1 = 1; c2 = 1; 

%asymmetric costs
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

%Initial contition (iteration = 1)
d11(1) = node1.d(1);
d12(1) = node1.d(2);
d21(1) = node2.d(1);
d22(1) = node2.d(2);
av1(1) = (d11(1)+d21(1))/2;
av2(1) = (d12(1)+d22(1))/2;
%iterations
for i=2:50
   % node 1
   [d1, cost1] = primal_solve(node1, rho);
   node1.d = d1;
   
   %DEBUG: check with matlab quadprog
   Q_ = [rho, 0; 0 rho];
   c_ = [c1+node1.y(1)-rho*node1.d_av(1),node1.y(2)-rho*node1.d_av(2)];
   A_ = [-k11 -k12; -1 0; 1 0];
   b_ = [o1-L1, 0, 5];
   d1_ = quadprog(Q_,c_,A_,b_,[],[],[],[]);
  
   %node2
   [d2, cost2] = primal_solve(node2, rho);
   node2.d = d2;
   
   %DEBUG: check with matlab quadprog
   Q_ = [rho, 0; 0 rho];
   c_ = [node2.y(1)-rho*node2.d_av(1),c2+node2.y(2)-rho*node2.d_av(2)];
   A_ = [-k21 -k22;0 -1; 0 1];
   b_ = [o2-L2, 0, 5];
   d2_ = quadprog(Q_,c_,A_,b_,[],[],[],[]);
   
   % Compute average with available data
   %node 1
   node1.d_av = (node1.d+node2.d)/2;
   
   %node 2
   node2.d_av = (node1.d+node2.d)/2;
   
   % Update local lagrangians
   %node 1
   node1.y = node1.y + rho*(node1.d-node1.d_av);
   
   %node 2
   node2.y = node2.y + rho*(node2.d-node2.d_av);
   
   
   %save data for plots
   d11(i) = node1.d(1);
   d12(i) = node1.d(2);
   d21(i) = node2.d(1);
   d22(i) = node2.d(2);
   av1(i) = (d11(i)+d21(i))/2;
   av2(i) = (d12(i)+d22(i))/2;
end;

% SOLVE WITH MATLAB QUADPROG
Q_ = [0, 0; 0 0];
A_ = -K; 
c_ = c;
b_ = [o1-L1; o2-L2];
lb = [0;0]; ub = [5;5];
disp('Matlab solutions')
d_ = quadprog(Q_,c_,A_,b_,[],[],lb,ub)
l_ = K*d_+o

disp('Consensus Solutions')
d = node1.d_av
l = K*d+o

%Plots
figure(10);
plot(1:50, av1, 1:50, av2, 1:50, d11, 1:50, d12, 1:50, d21, 1:50, d22);
legend('av1','av2', 'd11', 'd12', 'd21', 'd22');
title('time convergence');
xlabel('iter');

figure(15);
t = 0:5;
constr1 = (L1-o1)/k12-(k11/k12)*t;
constr2 = (L2-o2)/k22-(k21/k22)*t;
[x,y]=meshgrid(t,t);
hold on;
z = c1*x+c2*y;
contour(x,y,z);
plot(t,constr1,t,constr2,'LineWidth',2);
plot(t,zeros(size(t)),'k','LineWidth',2);
plot(zeros(size(t)),t,'k','LineWidth',2);
plot(t,5*ones(size(t)),'k','LineWidth',2);
plot(5*ones(size(t)),t,'k','LineWidth',2);
plot(av1,av2,'--','LineWidth',2);
plot(av1,av2,'bo');
% plot(d11,d12,'m-.','LineWidth',1);
% plot(d11,d12,'mx');
% plot(d21,d22,'k-.','LineWidth',1);
% plot(d21,d22,'kx');
title('solution space convergence');
xlabel('d_1');
ylabel('d_2');
plot(d_(1),d_(2),'r*')
axis([-1,6,-1,6]);
hold off;




   
