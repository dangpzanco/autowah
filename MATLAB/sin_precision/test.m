clc
close all
clearvars


N = 1000;
x = linspace(0,pi/2,N)';
x2 = linspace(0,0.5,N)';

y = sin(x);

y2 = x - x.^3/prod(1:3);
y3 = x - x.^3/prod(1:3) + x.^5/prod(1:5);
% y4 = 1 - pi^2*(0.5-x2).^2/prod(1:2);
% y4 = 1 - 5*(0.5-x2).^2;
y4 = -5*x2.*(x2 - 1) - 0.25;

e = 100*abs(y-x)./(y+1e-3);
e2 = 100*abs(y-y2)./(y+1e-3);
e3 = 100*abs(y-y3)./(y+1e-3);
e4 = 100*abs(y-y4)./(y+1e-3);

figure
plot(x,y,'-o',x,y2,x,y4)

figure
plot(x2,e2,x2,e4)
axis([x2(1),x2(end),0,10])

