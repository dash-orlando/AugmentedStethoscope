%
% Bluetooth Search
%
% The following function searches for oaired bluetooth devices
% The information about these devices is stored in a structure array
%
% Fluvio L. Lobo Fenoglietto
%

function [btdev] = btsearch()
    % [1] Search for bluetooth-enabled devices
    disp('Searching for bluetooth devices...'); % Warning message on command window or console
    btinstrinfo = instrhwinfo('Bluetooth'); % Defining structure with bluetooth devices found
    % [2] Storing device information in structure array
    btdev.devices = btinstrinfo.RemoteNames;
    btdev.id = btinstrinfo.RemoteIDs;
end % End of function
