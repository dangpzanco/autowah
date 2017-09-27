function [ y ] = first_order_filter( x, fc, fs )
%FIRST_ORDER_FILTER Summary of this function goes here
%   Detailed explanation goes here

persistent buffer;
if isempty(buffer)
    buffer = 0;
end

K = tan(pi*fc/fs);
% K = (pi*fc/fs) + (pi*fc/fs)^3/3;

b0 = K/(K+1);
b1 = b0;
% a1 = (K-1)/(K+1);
a1 = 2*(b0 - 0.5);

xh = x - a1*buffer;
y = b0*xh + b1*buffer;

buffer = xh;

end

