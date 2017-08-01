clc
close all
clearvars

N = 1000;

% tau = linspace(0,1,N)';
tau = logspace(-3,0,N)'; % from 1ms to 1s of Attack/Release Time
fs = 44.1e3;

x = -1./(tau.*fs);

y = exp(x);

% y2 = 1 + x + x.^2/prod(1:2);
y2 = 1 + x.*(1 + 0.5*x);
y3 = 1 + x + x.^2/prod(1:2) + x.^3/prod(1:3);
y4 = 1 + x + x.^2/prod(1:2) + x.^3/prod(1:3) + x.^4/prod(1:4);

figure
plot(tau,y,tau,y2)
grid on;

figure
plot(tau,100*abs(y-y2)./y,tau,100*abs(y-y3)./y,tau,100*abs(y-y4)./y)
grid on
