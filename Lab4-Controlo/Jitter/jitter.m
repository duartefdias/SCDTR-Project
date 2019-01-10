nbins = 21;

load jitterUno.csv
subplot(1,2,1);
j1 = std(jitterUno);
t1 = mean(jitterUno);
% fprintf('Arduino Uno\n');
% fprintf('Average sampling period of %s ms\n', num2str(t1/1000));
% fprintf('Jitter of %s us\n', num2str(j1));
histogram(jitterUno,nbins,'Normalization','probability');
xlabel('Sampling Period [\mus]');
title('Luminaire 1');

subplot(1,2,2);
load jitterDue.csv
j2 = std(jitterDue);
t2 = mean(jitterDue);
% fprintf('\nArduino Due\n');
% fprintf('Average sampling period of %s ms\n', num2str(t2/1000));
% fprintf('Jitter of %s us\n', num2str(j2));
histogram(jitterDue,nbins,'Normalization','probability');
xlabel('Sampling Period [\mus]');
title('Luminaire 2');