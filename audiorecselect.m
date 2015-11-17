%
% Audio Recording Device Selection
%
% The following function allows the user to select the preferred audio
% recording device
%
% Fluvio L. Lobo Fenoglietto
%

function [audiorecsel] = audiorecselect(audiorecdev)
    % [1] Generate list dialog for user to select audio recording devices
    disp('Select audio recording device...');
    devicenames = audiorecdev.devices;
    [selection,status] = listdlg('ListString',devicenames,...
                                 'SelectionMode','single',...
                                 'ListSize', [200 200],...
                                 'InitialValue',[1],...
                                 'Name','Audio Recording Devices',...
                                 'PromptString','Select an audio recording device:',...
                                 'OKString','Select',...
                                 'CancelString','Cancel');
    % [2] Storing selections
    if status == 1
        audiorecsel.index = selection;
        audiorecsel.device = devicenames(selection);
        disp(horzcat('User selected ',devicenames{selection},'...'));
    elseif status == 0
        audiorecsel = [];
        disp('User cancelled selection!');
    end % End of if-statement
end % End of function