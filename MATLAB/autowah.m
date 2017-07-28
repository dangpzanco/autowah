clc
close all
clear all

% filename = 'acoustic.wav';
% filename = 'harpsi-cs.wav';
% filename = 'trumpet.wav';
filename = 'jazztrio.wav';
% filename = 'marimba.wav';

[x, fs] = audioread(filename);
x = mean(x,2);
lenX = length(x);
n = (0:lenX-1)';

tauA = 40e-3;
tauR = 2e-3;
Q = 0.2;
% min_fc = 500;
% max_fc = 3000;
min_fc = 20;
max_fc = 5000;

xL = zeros(size(x));
yL = zeros(size(x));
xF = zeros(size(x));
yF = zeros(size(x));

for i = 1:lenX
    xL(i) = abs(x(i));
%     xL(i) = 20*log10(abs(x(i))+1e-3);
    yL(i) = level_detector(xL(i), tauA, tauR, fs);
    
    fc = yL(i) * (max_fc - min_fc) + min_fc;
%     fc = (1-10^(yL(i)/20)-1e-3) * (max_fc - min_fc) + min_fc;
    xF(i) = x(i);
    [~, yF(i)] = state_variable_filter( xF(i), fc, Q, fs );
end

yF = yF/max(abs(yF));

audiowrite('out_autowah.wav', yF, fs);

figure
plot(n,yL)




