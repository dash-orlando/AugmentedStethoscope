%
% Determine DFT length for spectral analysis
%
% This function determines the appropriate DFT length on the basis of the
% signal's length (in samples)
%
% Fluvio L Lobo Fenoglietto

function [nfft, power] = dftlen(signal)
    Nsignal = length(signal); % length of raw signal (# of samples)
    power = ceil(log2(Nsignal)); % the function uses ceiling instead of round to ensure that power is greater than Nsignal
    nfft = 2^power; 
end % End of function