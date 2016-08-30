%
% Audio Playback Connect
%
% The following function connects the computer to the playback device of
% choice
%
% Fluvio L. Lobo Fenoglietto
%

function [audioplayobj] = audioplaycon(audiodata, audioplaysel, nBits)
    % [1] Generate audio player
    audioSignal = audiodata.signal;
    audioFs = audiodata.fs;
    selectedInputDeviceID = audioplaysel.id;
    audioplayobj = audioplayer(audioSignal,audioFs,nBits,selectedInputDeviceID);
    disp(horzcat('Computer connected to ',audioplaysel.device{1},'...'));
end % End of function