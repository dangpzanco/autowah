% wah_wah.m   state variable band pass
% written by Ronan O'Malley
% October 2nd 2005
% 
% BP filter with narrow pass band, Fc oscillates up and down the spectrum
% Difference equation taken from DAFX chapter 2
%
% Changing this from a BP to a BS (notch instead of a bandpass) converts this effect to a phaser
%
% yl(n) = F1*yb(n) + yl(n-1)
% yb(n) = F1*yh(n) + yb(n-1)
% yh(n) = x(n) - yl(n-1) - Q1*yb(n-1)
%
% vary Fc from 500 to 5000 Hz
% 44100 samples per sec

clc
clear all
close all

infile = 'acoustic.wav';

% read in wav sample
[ x, Fs ] = audioread(infile);

%%%%%%% EFFECT COEFFICIENTS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% damping factor
% lower the damping factor the smaller the pass band
damp = 0.05;

% min and max centre cutoff frequency of variable bandpass filter
minf=500;
maxf=3000;

% wah frequency, how many Hz per second are cycled through
Fw = 2000; 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


% change in centre frequency per sample (Hz)
%delta=0.1;
delta = Fw/Fs;
%0.1 => at 44100 samples per second should mean  4.41kHz Fc shift per sec



% create triangle wave of centre frequency values
Fc=minf:delta:maxf;
while(length(Fc) < length(x) )
    Fc = [Fc, (maxf:-delta:minf)];
    Fc = [Fc, (minf:delta:maxf)];
end

% trim tri wave to size of input
Fc = Fc(1:length(x));


% difference equation coefficients
F1 = 2*sin((pi*Fc(1))/Fs);  % must be recalculated each time Fc changes
Q1 = 2*damp;                % this dictates size of the pass bands


yh=zeros(size(x));          % create emptly out vectors
yb=zeros(size(x));
yl=zeros(size(x));

% % first sample, to avoid referencing of negative signals
% yh(1) = x(1);
% yb(1) = F1*yh(1);
% yl(1) = F1*yb(1);

% apply difference equation to the sample
for n=2:length(x),
    
    yh(n) = x(n) - yl(n-1) - Q1*yb(n-1);
    yb(n) = F1*yh(n) + yb(n-1);
    yl(n) = F1*yb(n) + yl(n-1);
    
    [Yl(n), Yb(n), Yh(n)] = state_variable_filter( x(n), Fc(n-1), Q1, Fs );
    
    F1 = 2*sin((pi*Fc(n))/Fs);
end

Fc2 = zeros(1,2*length(Fc)); Fc2(1:2:end) = 8*Fc;
Fc2 = filtfilt(fir1(10,0.5),1,Fc2);

ym = moogvcf(x,Fc2,1);

%normalise

maxyb = max(abs(yb));
yb = yb/maxyb;
yl = yl/max(abs(yl));

ym = ym/max(abs(ym));
ym = ym - mean(ym);

% write output wav files
audiowrite('out_wah.wav', yb, Fs);

figure
hold on
plot(x)
plot(yl)
plot(ym)
title('Wah-wah and original Signal');