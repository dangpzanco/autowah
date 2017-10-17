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

% -5(x^2 - x + 1/20)


% e = 100*abs(y-x)./(y+1e-3);
% e2 = 100*abs(y-y2)./(y+1e-3);
% e3 = 100*abs(y-y3)./(y+1e-3);
% e4 = 100*abs(y-y4)./(y+1e-3);

e = 100*abs(x-asin(x))./(x+1e-6);
e2 = 100*abs(x-asin(y2))./(x+1e-6);
e3 = 100*abs(x-asin(y3))./(x+1e-6);
e4 = 100*abs(x-asin(y4))./(x+1e-6);

figure
% plot(x,y,x,y2,x,y4)
plot(x2,y,x2,y2,x2,y3)

figure
% plot(x2,asin(y)/pi,x2,asin(y2)/pi,x2,abs(asin(y4))/pi)
plot(x2,asin(y)/pi,x2,asin(y2)/pi,x2,asin(y3)/pi)
xlabel('Desired normalized frequency (\times fs)')
ylabel('Actual normalized frequency (\times fs)')
l = {'$$sin(x)$$','$$x - x^3/6$$','$$x - x^3/6 + x^5/120$$'};
legend(l,'interpreter','latex','location','best','FontSize',12)


figure
plot(x2,e2,x2,e3,x2,e4)
axis([x2(1),x2(end),0,10])
grid on
xlabel('Desired normalized frequency (\times fs)')
ylabel('Relative frequency error (%)')
l = {'$$x - x^3/6$$','$$x - x^3/6 + x^5/120$$','$$ 1/4 + 5x - 5x^2 $$'};
legend(l,'interpreter','latex','location','best','FontSize',12)

