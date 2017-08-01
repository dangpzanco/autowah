clc
close all
clearvars


N = 1000;

x = linspace(0,10,N)';

y = exp(-x);

y2 = 1 - x + x.^2/2;


plot(x,y,x,y2)


