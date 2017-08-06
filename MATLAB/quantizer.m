function [ y ] = quantizer( x, bits )
%QUANTIZER Summary of this function goes here
%   Detailed explanation goes here

maxVal = 2.^bits;

y = round(x.*maxVal)./maxVal;

end
