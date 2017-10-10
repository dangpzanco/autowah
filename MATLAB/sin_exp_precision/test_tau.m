clc
close all
clearvars

fs = 48e3;

tau_limits = [1e-3, 1];
pot_limits = exp(-1./(fs * tau_limits))

b = min(pot_limits);
a = pot_limits(2) - pot_limits(1);

N = 1024;
pot = linspace(0,1,N);
% pot_scale = a * pot + b;
pot_scale = exp(-1./(fs * pot));

% b_log = log10(pot_limits(1)/pot_limits(2))/(pot_scale(1) - pot_scale(end));
% a_log = pot_limits(1)/exp(b_log * pot_scale(1));
% pot_log = a_log * exp(b_log * pot_scale);

tau = -1./(fs*log(pot_scale));

plot(pot,tau)
grid on
axis([min(pot),max(pot),1e-3,1])



pot_hat = exp(-1./(fs * tau));

norm(pot_scale-pot_hat)
