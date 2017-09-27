clc
close all
clear all

rng(2)

% % filename = 'acoustic.wav';
% % filename = 'harpsi-cs.wav';
% % filename = 'trumpet.wav';
% % filename = 'jazztrio.wav';
% % filename = 'marimba.wav';
% filename = 'noise.wav';
% 
% [x, fs] = audioread(filename);
% % [x, fs] = wavread(filename);
% x = mean(x,2);
% lenX = length(x);

%% Triangular Envelope
fs = 16e3;
lenX = 10*fs;

ftri = 0.4;
t = (0:lenX-1)'/fs;

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
min_fc = 20;
max_fc = 3000;
% min_fc = 20;
% max_fc = 1000;

%% Init vectors
xL = zeros(size(x));
yL = zeros(size(x));
xF = zeros(size(x));
yF = zeros(size(x));
y = zeros(size(x));

%% Auto-Wah

for i = 1:lenX
    
    % Level Detector
    xL(i) = abs(x(i));
    yL(i) = level_detector(xL(i), tauA, tauR, fs);
    
%     yL(i) = env(i);
    
    % Variable cutoff frequency
    fc = yL(i) * (max_fc - min_fc) + min_fc;
    
    % State Variable Bandpass Filter
    xF(i) = x(i);
%     xF(i) = first_order_filter( x(i), fc, fs )*sqrt(2/Q);
%     xF(i) = x(i)*sqrt(2/Q);
    [~, yF(i)] = state_variable_filter( xF(i), fc, Q, fs );
    
    % Output mixing with input
    y(i) = mix*yF(i) + (1-mix)*x(i);
end

audiowrite('out_evaluation.wav', [x,y], fs);

%% Input/Output
% figure
% plot(t,x,t,y)
% title('Input/Output Signals','interpreter','latex','fontsize',14)
% xlabel('Time [s]','interpreter','latex','fontsize',14)
% ylabel('Amplitude','interpreter','latex','fontsize',14)
% l = {'$$x[n]$$','$$y[n]$$'};
% legend(l,'interpreter','latex','fontsize',12,'location','best')
% grid on
% savefig('input_output.fig')
% saveas(gcf,'input_output','epsc')

% figure
% subplot(1,2,1)
% plot(t,x)
% title('Input $$x[n]$$','interpreter','latex','fontsize',14)
% xlabel('Time [s]','interpreter','latex','fontsize',14)
% ylabel('Amplitude','interpreter','latex','fontsize',14)
% axis([t(1),t(end),-2,2])
% grid on
% 
% subplot(1,2,2);
% plot(t,y)
% title('Output signal ($$y[n]$$)','interpreter','latex','fontsize',14)
% xlabel('Time [s]','interpreter','latex','fontsize',14)
% ylabel('Amplitude','interpreter','latex','fontsize',14)
% axis([t(1),t(end),-2,2])
% grid on
% 
% savefig('input_output.fig')
% saveas(gcf,'input_output','epsc')

%% Level Detector
figure
plot(t,env,t,yL)
title('Level Detector output','interpreter','latex','fontsize',14)
xlabel('Time [s]','interpreter','latex','fontsize',14)
ylabel('Amplitude','interpreter','latex','fontsize',14)
l = {'Envelope of $$x[n]$$','$$y_L[n]$$'};
legend(l,'interpreter','latex','fontsize',12,'location','best')
grid on
savefig('level_detector.fig')
saveas(gcf,'level_detector','epsc')

%% Input Time

figure
plot(t,x)
title('Input signal ($$x[n]$$)','interpreter','latex','fontsize',14)
xlabel('Time [s]','interpreter','latex','fontsize',14)
ylabel('Amplitude','interpreter','latex','fontsize',14)
axis([t(1),t(end),-2,2])
grid on
savefig('input_time.fig')
saveas(gcf,'input_time','epsc')

%% Output Time

figure
plot(t,y)
title('Output signal ($$y[n]$$)','interpreter','latex','fontsize',14)
xlabel('Time [s]','interpreter','latex','fontsize',14)
ylabel('Amplitude','interpreter','latex','fontsize',14)
axis([t(1),t(end),-2,2])
grid on
savefig('output_time.fig')
saveas(gcf,'output_time','epsc')

%% Intput Spectrum
figure
spectrogram(x,blackmanharris(1024),0.25*1024,1024,fs);
title('Input signal ($$x[n]$$)','interpreter','latex','fontsize',14)
ylabel('Time [s]','interpreter','latex','fontsize',14)
xlabel('Frequency [kHz]','interpreter','latex','fontsize',14)
savefig('input_spectrum.fig')
saveas(gcf,'input_spectrum','epsc')

%% Output Spectrum
figure
spectrogram(y,blackmanharris(1024),0.25*1024,1024,fs);
title('Output signal ($$y[n]$$)','interpreter','latex','fontsize',14)
ylabel('Time [s]','interpreter','latex','fontsize',14)
xlabel('Frequency [kHz]','interpreter','latex','fontsize',14)
savefig('output_spectrum.fig')
saveas(gcf,'output_spectrum','epsc')





