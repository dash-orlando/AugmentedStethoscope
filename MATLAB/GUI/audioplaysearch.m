%
% Audio Playback Device Search
%
% The following function searches for audio playback devices within
% windows. Given that this function searches for 'playback' devices, the
% program only looks for 'audio-output' devices. This function will be
% automated in the near future
%
% Fluvio L. Lobo Fenoglietto
%

function [audioplaydev] = audioplaysearch()
    % [1] Search for audio recording devices installed in the computer
    disp('Searching for audio playback devices...'); % Warning message on command window or console
    audioplayinfo = audiodevinfo;
    % [2] Storing device information in structure array
    Ndevices = length(audioplayinfo.output); % Number of audio input devices
    for i = 1:Ndevices
        audioplaydev.devices{i} = audioplayinfo.output(i).Name;
        audioplaydev.id(i) = audioplayinfo.output(i).ID;
    end % End of for-loop
    disp('Audio playback devices found and stored within "audioplaydev"...');
end % End of function