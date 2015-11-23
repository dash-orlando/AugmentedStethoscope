%
% Audio Recording Device Search
%
% The following function searches for audio recording devices within
% windows. Given that this function searches for 'recording' devices, the
% program only looks for 'audio-input' devices. This function will be
% automated in the near future
%
% Fluvio L. Lobo Fenoglietto
%

function [audiorecdev] = audiorecsearch()
    % [1] Search for audio recording devices installed in the computer
    disp('Searching for audio recording devices...'); % Warning message on command window or console
    audiorecinfo = audiodevinfo;
    % [2] Storing device information in structure array
    Ndevices = length(audiorecinfo.input); % Number of audio input devices
    if Ndevices == 0
        disp('%%===============ERROR: No recording devices were found===============%%');
        disp('>>Possible Solutions');
        disp('>>[1] Input/Recording devices disconnected - Ensure Connection');
        disp('>>[2] MATLAB internal error reading input devices - Restart MATLAB');
        disp('%%====================================================================%%');
    elseif Ndevices ~= 0     
        for i = 1:Ndevices
            audiorecdev.devices{i} = audiorecinfo.input(i).Name;
            audiorecdev.id(i) = audiorecinfo.input(i).ID;
        end % End of for-loop
        disp('Audio recording devices found and stored within "audiorecdev"...');
    end % End of if-statement
end % End of function