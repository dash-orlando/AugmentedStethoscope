%
% Simple Spectral Analysis (SSA)
%
% Simple spectral analysis program for input signal, using FFT and basic
% vector manipulations

function [psdx] = ssa(signal,fs)

    Nsignal = length(signal);   
    xdft = fft(signal);
    xdft = xdft(1:Nsignal/2+1);
    psdx = (1/(fs*Nsignal)) * abs(xdft).^2;
    psdx(2:end-1) = 2*psdx(2:end-1);
    freq = 0:fs/Nsignal:fs/2;

    semilogx(freq,10*log10(psdx))
    grid on
    title('Periodogram Using FFT')
    xlabel('Frequency (Hz)')
    ylabel('Power/Frequency (dB/Hz)')

end