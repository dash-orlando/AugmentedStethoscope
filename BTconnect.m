%
% Bluetooth Connect
%
% The following function connects the computer to the bluetooth device
% selected by the user
%
% Fluvio L. Lobo Fenoglietto
%

function [btobj] = btconnect(btdevsel)
    % [1] Generate bluetooth object with selected device
    btobj = Bluetooth(btdevsel.device{1}, 1);
end % End of function