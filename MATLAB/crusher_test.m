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

crushNum = linspace(2,20,lenX)';

for i = 1:lenX
    
    
%     numSamples = sampleTime*fs;
    numSamples = 3.5;
%     numSamples = crushNum(i);
    yC(i) = sample_hold(x(i), numSamples);
%     yC(i) = quantizer( 3*sample_hold(x(i), numSamples), 4);
    
end

audiowrite('out_crusher.wav', yC, fs);

figure
plot(n,yC)




