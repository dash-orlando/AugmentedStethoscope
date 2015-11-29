%
% Simple Spectral Analysis (SSA)
%
% Simple spectral analysis program for input signal, using FFT and basic
% vector manipulations

function [pfft] = ssa(signal,fs)

    Nsignal = length(signal);
    
    freqbins = [0:Nsignal-1];
    freqHz = freqbins.*(fs/Nsignal);
    
    pfft = abs(fft(signal));

    semilogx(freqHz(1:Nsignal/2),10*log10(pfft(1:Nsignal/2)))

end