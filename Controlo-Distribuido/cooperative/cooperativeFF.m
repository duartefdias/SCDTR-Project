load constants.mat
global y_ant e_ant i_ant
i_ant = 0;
y_ant = 0;
e_ant = 0;

tau(mtau(1),btau(1),200);
G0(mG(1),bG(1),20);
lux2volt(100, m(1), C(1));
volt2lux(3.3048, m(1), C(1));
predictor(0, 5, 0, 0.01, tau(mtau(1), btau(1), 200));

ref = 80;
measure = ref-1;
Controller(ref, measure, mG(1), bG(1))
