clc
close all
clear all

rng(2)

% %% Input signal

% fs = 48e3;
% lenX = 10*fs;
% 
% f = linspace(0,fs/2,lenX)';
% n = (0:lenX-1)';
% 
% ftri = 0.4;
% t = (0:lenX-1)'/fs;
% 
% z = 0e-2*randn(lenX,1);
% x = 0.25*(sawtooth(2*pi*ftri*t,0.5) + 1).*z;


%% Triangular Envelope
fs = 48e3;
lenX = 10*fs;

ftri = 0.4;
n = (0:lenX-1)';
t = n/fs;

env = 0.5*(sawtooth(2*pi*ftri*t,0.5) + 1);

% figure
% plot(t,env)

%% Input signal

% v = 2*rand(lenX,1) - 1;
v = randn(lenX,1);
x = v.*env; x = x/max(max(x),min(x));
% x = x/max(max(x),min(x));
% figure
% plot(t,x)

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
xL = zeros(size(x));
yL = zeros(size(x));
y = zeros(size(x));

numTaus = 2;
% tau_vec = linspace(1e-3,1,numTaus);
tau_vec = [10 1000]*1e-3;
% tau_vec = logspace(-3,0,numTaus);

out = zeros(size(x,1),numTaus);

%% Auto-Wah

for tt = 1:numTaus
    for i = 1:lenX
        
        tauA = tau_vec(tt);
%         tauR = tau_vec(tt);
        
        % Level Detector
        xL(i) = abs(x(i));
        yL(i) = level_detector(xL(i), tauA, tauR, fs);
        
        y(i) = yL(i);
    end
    
    out(:,tt) = y;
end

figure
plot(x)


round_factor = 1;
% l = strtrim(cellstr(num2str(round(freqs'/10^-n)*10^-n))');
l{1} = ['Envelope of $$x[n]$$'];
for i = 1:numTaus
    l{i+1} = ['$$\tau_A =$$ ' num2str(round(1e3*tau_vec(i)*10^round_factor)*10^-round_factor) ' ms'];
end

figure
plot(n/fs,env,n/fs,out)
legend(l,'location','southwest','interpreter','latex')
xlabel('Time [s]')
ylabel('Amplitude')
grid on


% figure
% semilogx(2*f,magdb)
% axis([10,10e3,-60,20])
% % axis([10,10e3,1.5*min(reshape(magdb(10 < 2*f & 2*f < 10e3,:),1,[])),1.1*max(magdb(:))])
% legend(l,'location','southwest')
% xlabel('Frequency [Hz]')
% ylabel('Magnitude [dB]')
% grid on



