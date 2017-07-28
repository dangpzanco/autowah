clc
close all
clearvars

fs = 44100;
N = 379000;

x = randn(N,1);
x = x / max(abs(x));

vol = linspace(0,1,N)';

y = vol.*x;

wavwrite(y,fs,'testSound.wav');


