%
% Recording or Input Device Selection
%
% This function has been designed to allow for the user to select the input
% recording device of choice.
%
% Fluvio L. Lobo Fenoglietto

function [selectedInputDeviceName, selectedInputDeviceID, status] = selectInputDevice()
    info = audiodevinfo; % detect connected audio devices
    Nidev = length(info.input); % number of input devices
    inputDevicesList = cell(Nidev,1); % initialize list for the names of the detected input devices
    for i = 1:Nidev % this loop populates the list of input devices
        inputDevicesList{i,1} = info.input(i).Name;   
    end
    % a listbox GUI allows for the user to select the preferred input device
    [selection,status] = listdlg('ListString',inputDevicesList,...
                                 'SelectionMode','single',...
                                 'ListSize',[400, 100],...
                                 'InitialValue',1,...
                                 'Name','Recording Device Selection',...
                                 'PromptString','Select one of the input devices listed',...
                                 'OKString','OK',...
                                 'CancelString','Cancel');
    % the name and the id corresponding to the slected device are stored
    selectedInputDeviceName = info.input(selection).Name;
    selectedInputDeviceID = info.input(selection).ID;
end % End of function
