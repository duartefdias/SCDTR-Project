clear
load constants.mat
global y_ant e_ant i_ant
i_ant = 0;
y_ant = 0;
e_ant = 0;

% tau(mtau(1),btau(1),200);
% G0(mG(1),bG(1),20);
% lux2volt(100, m(1), C(1));
% volt2lux(3.3048, m(1), C(1));
% predictor(0, 5, 0, 0.01, tau(mtau(1), btau(1), 200));
% ref = 80;
% measure = ref-1;
% Controller(ref, measure, mG(1), bG(1));

ref1 = 100;
ref2 = 70;
u1(1) = FeedForwardController(ref1, G0(mG(1), bG(1),ref1));
u1(1) = saturate(u1(1));
u2(1) = FeedForwardController(ref2, G0(mG(2), bG(2),ref2));
u2(1) = saturate(u2(1));
k11 = G0(mG(1), bG(1),ref1);
k22 = G0(mG(2), bG(2),ref2);
k12 = 2.8;
k21 = 2.9;
iter = 20;

y1(1) = k11*u1(1);
y2(1) = k22*u2(1);

for i = 2:iter
    d1 = k12 * u2(i-1);                     % compute the influence of the
    y1(i) = k11*u1(i-1) + d1;                   % other agent
    % compute u1 to compensate d1              
    u1(i) = FeedForwardController(ref1-d1, G0(mG(1), bG(1),ref1));
    u1(i) = saturate(u1(i));
    
    d2 = k21 * u1(i);
    y2(i) = k22*u2(i-1) + d2;
    u2(i) = FeedForwardController(ref2-d2, G0(mG(1), bG(1),ref2));
    u2(i) = saturate(u2(i));
end


scatter(1:iter,u1);
hold on
scatter(1:iter,u2);
title('Applied u');
legend('u1', 'u2');

figure();
scatter(1:iter,y1);
hold on
scatter(1:iter,y2);
title('Illuminance');
legend('y1', 'y2');

