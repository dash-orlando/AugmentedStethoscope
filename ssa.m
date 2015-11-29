%
% Simple Spectral Analysis (SSA)
%
% Simple spectral analysis program for input signal, using FFT and basic
% vector manipulations

function [pfft] = ssa(signal,fs)

    Nsignal = length(signal);
    
    freq = (fs/Nsignal)*[0:Nsignal-1];
    
    pfft = abs(fft(signal));

    semilogx(freq(1:Nsignal/2),10*log10(pfft(1:Nsignal/2)))

end