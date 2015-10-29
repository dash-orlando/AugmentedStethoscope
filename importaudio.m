%
% Get recording or audio file
%
% Fluvio L. Lobo Fenoglietto
%

function [audiopath, audiofilename, audio] = importaudio()

    [filename, pathname] = uigetfile({'*.wav';'*.mp3'},'Select Audio File');
    
    [signal, fs] = audioread(fullfile(pathname,filename));
       
    tmax = length(signal)/fs;
    time = 0:1/fs:tmax-1/fs;
    
    audiopath = pathname;
    audiofilename = filename;
    audio(:,1) = time;
    audio(:,2) = signal;
    
end