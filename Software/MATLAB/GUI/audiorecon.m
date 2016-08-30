%
% Audio Recording Connect
%
% The following function connects the computer to the bluetooth device
% selected by the user
%
% Fluvio L. Lobo Fenoglietto
%

function [audiorecobj] = audiorecon(audiorecsel, Fs, nBits, nChannels)
    % [1] Generate audio recording object
    selectedInputDeviceID = audiorecsel.id;
    audiorecobj = audiorecorder(Fs,nBits,nChannels,selectedInputDeviceID);
    disp(horzcat('Computer connected to ',audiorecsel.device{1},'...'));
end % End of function