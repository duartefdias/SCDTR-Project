clear all
%L = logspace(0, 2, 20);
measuredL = [160, 222, 356, 1290, 733, 3, 1620 ];
measuredV = [3.7, 3.93, 4.11, 4.58, 4.35, 2.51, 4.61 ];

%Simulation Parameters
%gama = -1;
Vcc = 5;
%R1 = 1000;
%k = 100;

%R2 = k*(L.^gama);
%Vi = Vcc * (R1 ./ (R1 + R2));

xi = log10(measuredL);
yi = log10(Vcc./measuredV - 1);
plot(xi, yi, 'o');

%Getting slope parameters ( y = mx + b )
p = polyfit(xi, yi, 1);
m = p(1)
b = p(2)

C = 10^(-b/m)

%xi=logspace(0,2,20);
L2 = 10^(-b / m) * (Vcc ./ measuredV - 1).^(1 / m);
hold on
% L
% L2
norm(measuredL - L2)