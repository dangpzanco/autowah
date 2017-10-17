clc
close all
clearvars

fs = 48e3;
N = 1024;
Nbits = 16;

tau_limits = [1e-3, 1];
alpha_limits = exp(-1./(fs * tau_limits))
b = min(alpha_limits);
a = N/(N-1)*(alpha_limits(2) - alpha_limits(1));

% pot = linspace(0,1,N);
pot = (0:N-1)/N;

x = -1./(fs*(tau_limits(2)*pot + tau_limits(1)));

alpha1 = a*pot + b;
alpha2 = exp(x);


tau1 = -1./(fs*log(alpha1));
tau2 = -1./(fs*log(alpha2));


figure
semilogy(pot,tau1,pot,tau2)
% plot(pot,tau1,pot,tau2)
% axis([pot(1),pot(end),1e-3,1])
grid on

