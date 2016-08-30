%
% Audio Playback Device Selection
%
% The following function allows the user to select the preferred audio
% playback device
%
% Fluvio L. Lobo Fenoglietto
%

function [audioplaysel] = audioplayselect(audioplaydev)
    % [1] Generate list dialog for user to select audio recording devices
    disp('Select audio playback device...');
    devicenames = audioplaydev.devices;
    deviceid = audioplaydev.id;
    [selection,status] = listdlg('ListString',devicenames,...
                                 'SelectionMode','single',...
                                 'ListSize', [200 200],...
                                 'InitialValue',[1],...
                                 'Name','Audio Playback Devices',...
                                 'PromptString','Select an audio playback device:',...
                                 'OKString','Select',...
                                 'CancelString','Cancel');
    % [2] Storing selections
    if status == 1
        audioplaysel.index = selection;
        audioplaysel.device = devicenames(selection);
        audioplaysel.id = deviceid(selection);
        disp(horzcat('User selected ',devicenames{selection},'...'));
    elseif status == 0
        audioplaysel = [];
        disp('User cancelled selection!');
    end % End of if-statement
end % End of function