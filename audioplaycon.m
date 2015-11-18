%
% Audio Playback Connect
%
% The following function connects the computer to the playback device of
% choice
%
% Fluvio L. Lobo Fenoglietto
%

function [audioplayer] = audioplaycon(audioplaysel, Fs, nBits)
    % [1] Generate audio player
    selectedInputDeviceID = audioplaysel.id;
    audioplayer = audiorecorder(Fs,nBits,nChannels,selectedInputDeviceID);
    disp(horzcat('Computer connected to ',audioplaysel.device{1},'...'));
end % End of function