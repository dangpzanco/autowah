function y = sample_hold( x, numSamples )
%SAMPLE_HOLD Summary of this function goes here
%   Detailed explanation goes here

persistent sample;
if isempty(sample)
    sample = 0;
end

persistent hold_counter;
if isempty(hold_counter)
    hold_counter = 0;
end

hold_counter = hold_counter + 1;
if(hold_counter > numSamples)
%     hold_counter = 0;
    hold_counter = hold_counter - numSamples;
    sample = x;
end

y = sample;

end

