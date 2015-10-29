%
% Spectral Analysis using Welch's Method
%
% Fluvio L. Lobo Fenoglietto
%
%

[audiopath, audiofilename, audio] = importaudio();

[pxx, freq] = pwelch(audio(:,2),fs);

plot(freq,10*log10(pxx))

