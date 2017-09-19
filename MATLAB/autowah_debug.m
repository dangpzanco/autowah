clc
close all
clear all

% filename = 'acoustic.wav';
% filename = 'harpsi-cs.wav';
% filename = 'trumpet.wav';
% filename = 'jazztrio.wav';
% filename = 'marimba.wav';
filename = 'noise.wav';

[x, fs] = audioread(filename);
% [x, fs] = wavread(filename);
x = mean(x,2);
lenX = length(x);
n = (0:lenX-1)';

% Parameters
mix = 1;
tauA = 40e-3;
tauR = 2e-3;
% tauA = 50e-3;
% tauR = 5e-3;
Q = 5;
min_fc = 20;
max_fc = 5000;

% Init vectors
xL = zeros(size(x));
yL = zeros(size(x));
xF = zeros(size(x));
yF = zeros(size(x));
y = zeros(size(x));

for i = 1:lenX
    
    % Level Detector
    xL(i) = abs(x(i));
    yL(i) = level_detector(xL(i), tauA, tauR, fs);
    
    % Variable cutoff frequency
    fc = yL(i) * (max_fc - min_fc) + min_fc;
    
    % State Variable Bandpass Filter
    xF(i) = x(i);
%     fc = 1e3;
    [~, yF(i)] = state_variable_filter( xF(i), fc, Q, fs );
    
    % Output mixing with input
    y(i) = mix*yF(i) + (1-mix)*x(i);
end

% y = y/max(max(y),min(y));

audiowrite('out_autowah.wav', [x,y], fs);
% wavwrite(y, fs, 'out_autowah.wav');

figure
plot(n,yL)




