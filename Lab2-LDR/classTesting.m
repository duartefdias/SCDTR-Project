clear all
%L = logspace(0, 2, 20);

% Measured with luximeter
measuredL1 = [160, 222, 356, 1290, 733, 3, 1620];
measuredV1 = [3.7, 3.93, 4.11, 4.58, 4.35, 2.51, 4.61];

% Measured with smartphone on arduino 1
measuredL2 = [5 60 229 400 600 800 1000 1200 1600 2500];
measuredV2 = [1.75 2.52 3.5 3.92 4.09 4.19 4.27 4.31 4.44 4.54];

% Measured with smartphone on arduino 2
measuredL3 = [5,    68,   400,  600,  800,  1000, 1200, 1600, 2500];
measuredV3 = [0.76, 1.80, 2.95, 3.15, 3.37, 3.50, 3.57, 3.72, 3.94];

%Simulation Parameters
%gama = -1;
Vcc = 5;
%R1 = 1000;
%k = 100;

%R2 = k*(L.^gama);
%Vi = Vcc * (R1 ./ (R1 + R2));

xi = log10(measuredL3);
yi = log10(Vcc./measuredV3 - 1);
plot(xi, yi, 'o');

%Getting slope parameters ( y = mx + b )
p = polyfit(xi, yi, 1);
m = p(1)
b = p(2)

C = 10^(-b/m)
