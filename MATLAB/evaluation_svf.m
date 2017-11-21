clc
close all
clear all

rng(2)

%% Input signal

fs = 48e3;
lenX = 2*fs;

f = linspace(0,fs/2,lenX)';
n = (0:lenX-1)';
% x = sin(pi*f/fs.*n);
x = [sqrt(lenX); zeros(lenX-1,1)];
% x = [1; zeros(lenX-1,1)];

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

numFreqs = 5;
% freqs = linspace(min_fc,max_fc,numFreqs);
% freqs = logspace(log10(min_fc),log10(max_fc),numFreqs);
freqs = [20 70 300 800 3000];
% quality = logspace(-1,log10(5e3),numFreqs);
quality = [1 2 10 100 1000];
out = zeros(size(x,1),numFreqs);

%% Auto-Wah

for ff = 1:numFreqs
    for i = 1:lenX
        
        % Variable cutoff frequency
        fc = freqs(ff);
%         fc = 300;
%         Q = quality(ff);

        % State Variable Bandpass Filter
%         xF(i) = x(i);
        xF(i) = first_order_filter( x(i), fc, fs )*sqrt(0.5/Q);
%         xF(i) = first_order_filter( xF(i), fc, fs );
%         xF(i) = x(i)*sqrt(0.5/Q);
        [~, yF(i)] = state_variable_filter( xF(i), fc, Q, fs );
%         [~, ~, yF(i)] = state_variable_filter( xF(i), fc, Q, fs );
%         [yF(i)] = state_variable_filter( xF(i), fc, Q, fs );
        
        y(i) = yF(i);
    end
    
    out(:,ff) = y;
end

audiowrite('out_evaluation.wav', [x/max(x)*(1-2^-15),y/max(y)*(1-2^-15)], fs);
% wavwrite([x,y], fs, 'out_evaluation.wav');

% out = out/max(out(:));

% inSpectrum = fft(x)/sqrt(lenX);
% outSpectrum = fft(out)/sqrt(lenX);

inSpectrum = fft(x);
outSpectrum = fft(out);

% B = repmat(inSpectrum,[1,numFreqs]);
% ./repmat(abs(inSpectrum),[1,numFreqs])
magdb = 20*log10(abs(outSpectrum)) - 20*log10(repmat(abs(inSpectrum),[1,numFreqs]));

% figure
% semilogx(2*f,20*log10(abs(inSpectrum)))
% axis([10,10e3,-120,0])
% grid on

%%

round_factor = 1;
% l = strtrim(cellstr(num2str(round(freqs'/10^-n)*10^-n))');
for i = 1:numFreqs
    l{i} = [num2str(round(freqs(i)*10^round_factor)*10^-round_factor) 'Hz'];
%     l{i} = ['Q = ' num2str(round(quality(i)*10^round_factor)*10^-round_factor)];
end


figure
plot(n/fs,out)
legend(l,'location','southwest')
xlabel('Time [s]')
ylabel('Amplitude')
grid on

figure
semilogx(2*f,magdb)
axis([10,10e3,-60,20])
% axis([10,10e3,1.5*min(reshape(magdb(10 < 2*f & 2*f < 10e3,:),1,[])),1.1*max(magdb(:))])
legend(l,'location','southwest')
xlabel('Frequency [Hz]')
ylabel('Magnitude [dB]')
grid on



