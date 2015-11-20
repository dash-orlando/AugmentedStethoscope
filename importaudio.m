%
% Import Audio Files
%
% The function allows the user to browse and select the audio file or
% recording of interested.
%
% The function then imports the file and gathers information related to its
% location (path), name, and data content (signal voltage and sampling
% frequency).
%
% Fluvio L. Lobo Fenoglietto
%

function [audiodata] = importaudio()

    % the function sends a notification to the command window referring to the supported file extensions
    disp('importaudio supports the following file extensions :: ');
    supportedextensions = {'*.wav';'*.mp3';'*.mp4';'*.m4a';'*.ogg';'*.flac';'*.au';'*.aiff';'*.aif';'*.aifc'};
    
    % using uigetfile, the user is prompted to browse for the desired audio file
    [filename, pathname] = uigetfile(supportedextensions,'Select Audio File');
    
    % audioread is then used to convert the audio file into a vector composed of amplitude or voltage values
    % using audioread, the sampling frequency of the selected audio file is determined
    [signal, fs] = audioread(fullfile(pathname,filename));
    
    % the length of the signal vector and the sampling frequency are then used to calculate the time vector corresponding the signal 
    tmax = length(signal)/fs;
    time = 0:1/fs:tmax-1/fs;
    
    audiodata.audiopath = pathname; % pathname of the audio file
    audiodata.audiofilename = filename; % name of the audio file
    audiodata.time = time; % time array of the audio file
    audiodata.signal = signal; % voltage or amplitude array of the audio file
    audiodata.fs = fs; % sampling frequency
    
end