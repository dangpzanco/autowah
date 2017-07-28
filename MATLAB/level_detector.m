function [ y ] = level_detector( x, tauA, tauR, fs )
%LEVEL_DETECTOR Summary of this function goes here
%   Detailed explanation goes here

persistent bufferY;
if isempty(bufferY)
    bufferY = zeros(2,1);
end

alphaA = exp(-1/(tauA*fs));
alphaR = exp(-1/(tauR*fs));

y1 = max(x, alphaR * bufferY(1) + (1 - alphaR) * x);
y = alphaA * bufferY(2) + (1 - alphaA) * y1;

bufferY(1) = y1;
bufferY(2) = y;

end

