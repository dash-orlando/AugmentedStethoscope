%
% Bluetooth Select
%
% The following function allows the user to select the bluetooth device to
% connect to
%
% Fluvio L. Lobo Fenoglietto
%

function [btdevsel] = btselect(btdev)
    % [1] Generate list dialog for user to select bluetooth device
    devicenames = btdev.devices;
    [selection,status] = listdlg('ListString',devicenames,...
                                 'SelectionMode','single',...
                                 'ListSize', [200 200],...
                                 'InitialValue',[1],...
                                 'Name','Bluetooth Devices',...
                                 'PromptString','Select a bluetooth device:',...
                                 'OKString','Select',...
                                 'CancelString','Cancel');
    % [2] Storing selections
    if status == 1
        btdevsel.index = selection;
        btdevsel.device = devicenames(selection);
    elseif status == 0
        btdevsel = [];
        disp('User cancelled selection!');
    end % End of if-statement
end % End of function
