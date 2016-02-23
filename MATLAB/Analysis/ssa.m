%
% Simple Spectral Analysis (SSA)
%
% Simple spectral analysis program for input signal, using FFT and basic
% vector manipulations. The function has been designed to process any
% signal with known sampling frequency
%
% Fluvio L Lobo Fenoglietto

function [psdx,error] = ssa(signal,fs)

    [stampedstring] = timeprefix('Signal processign started');
    disp(stampedstring);
    Nsignal = length(signal);   
    xdft = fft(signal); % transformation of raw signal into Fourier space
    xdft = xdft(1:Nsignal/2+1); % editing signal by cropping half of the array - second half of the array is a mirror image
    psdx = (1/(fs*Nsignal)) * abs(xdft).^2; % normalization of the absolute value of the signal's Fourier transform
    psdx(2:end-1) = 2*psdx(2:end-1); % doubling power
    freq = 0:fs/Nsignal:fs/2; % creating frequency array which matches Fourier transform

    [stampedstring] = timeprefix('Calculating error');
    disp(stampedstring);
    % error calculation
    error = max(psdx - periodogram(signal,rectwin(Nsignal),Nsignal,fs));
    
    [stampedstring] = timeprefix('Plotting results');
    disp(stampedstring);
    % plotting
    figure,
    semilogx(freq,10*log10(psdx))
    grid on
    title('Periodogram Using FFT')
    xlabel('Frequency (Hz)')
    ylabel('Power/Frequency (dB/Hz)')
    
end % End of function