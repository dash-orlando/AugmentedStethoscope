%
% Export Audio Files
%
% The function allows the user to browse and select the directory and
% filename under which the audio recording will be saved/stored
%
% The function then saves the file, using a specified extension, to the
% selected path
%
% Fluvio L. Lobo Fenoglietto
%

function [audiodata] = exportaudio(audiodata)

    % the function sends a notification to the command window referring to the supported file extensions
    disp('exportaudio supports the following file extensions :: ');
    supportedextensions = {'*.wav';'*.mp3';'*.mp4';'*.m4a';'*.ogg';'*.flac';'*.au';'*.aiff';'*.aif';'*.aifc'};
    
    % using uiputfile, the user is prompted to browse for the desired directory
    [filename, pathname] = uiputfile(supportedextensions,'Select Destination Directory');
    
    % audiowrite is then used to save the file into the specified format
    signal = audiodata.signal;
    fs = audiodata.fs;
    audiowrite(fullfile(pathname,filename),signal,fs);
    
end