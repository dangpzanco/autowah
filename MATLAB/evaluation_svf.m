clc
close all
clear all

rng(2)

%% Input signal

fs = 48e3;
lenX = 10*fs;

f = linspace(0,fs/2,lenX)';
n = (0:lenX-1)';
x = sin(pi*f/fs.*n);

%% Parameters
mix = 1;
% tauA = 40e-3;
% tauR = 2e-3;
tauA = 20e-3;
tauR = 100e-3;
% tauA = 1e-3;
% tauR = 1;
Q = 5;
% Q = 1000;
min_fc = 20;
max_fc = 3000;
% min_fc = 20;
% max_fc = 1000;

%% Init vectors
xF = zeros(size(x));
yF = zeros(size(x));
y = zeros(size(x));

numFreqs = 10;
% freqs = linspace(min_fc,max_fc,numFreqs);
freqs = logspace(log10(min_fc),log10(max_fc),numFreqs);
out = zeros(size(x,1),numFreqs);

%% Auto-Wah

for n = 1:numFreqs
    for i = 1:lenX
        
        % Variable cutoff frequency
        fc = freqs(n);

        % State Variable Bandpass Filter
%         xF(i) = x(i);
        xF(i) = first_order_filter( x(i), fc, fs )*sqrt(0.5/Q);
%         xF(i) = first_order_filter( xF(i), fc, fs );
%         xF(i) = x(i)*sqrt(0.5/Q);
        [~, yF(i)] = state_variable_filter( xF(i), fc, Q, fs );
        
        y(i) = yF(i);
    end
    
    out(:,n) = y;
end

audiowrite('out_evaluation.wav', [x,y/max(y)], fs);
% wavwrite([x,y], fs, 'out_evaluation.wav');

% out = out/max(out(:));

inSpectrum = fft(x)/sqrt(lenX);
outSpectrum = fft(out)/sqrt(lenX);

% B = repmat(inSpectrum,[1,numFreqs]);
% ./repmat(abs(inSpectrum),[1,numFreqs])
magdb = 20*log10(abs(outSpectrum));

figure
semilogx(2*f,20*log10(abs(inSpectrum)))
axis([10,10e3,-120,0])
grid on

figure
semilogx(2*f,magdb(:,:))
axis([10,10e3,-120,0])
grid on



