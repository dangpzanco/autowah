function [ yl, yb, yh ] = state_variable_filter( x, fc, Q, fs )
%STATE_VARIABLE_FILTER Summary of this function goes here
%   Detailed explanation goes here

persistent bufferY;
if isempty(bufferY)
    bufferY = zeros(2,1);
end

% yh(n) = x(n) - yl(n-1) - Q1*yb(n-1);
% yb(n) = F1*yh(n) + yb(n-1);
% yl(n) = F1*yb(n) + yl(n-1);

f = 2*sin(pi*fc/fs);
q = 1/Q;
% q = Q;

yh = x - bufferY(1) - q * bufferY(2);
yb = f * yh + bufferY(2);
yl = f * yb + bufferY(1);

bufferY(1) = yl;
bufferY(2) = yb;

end

