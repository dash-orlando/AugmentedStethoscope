%
% Bluetooth Connection
%
% The following function establishes a bluetooth communication with the
% digital or electronic stethoscope of choice.
%
% This function has been built to work with the 3M 3200 Littmann 
% Electronic Stethoscope - 10/22/2015 -
%
% Fluvio L. Lobo Fenoglietto
%

% [1] Search for bluetooth-enabled devices
disp('Searching for bluetooth devices...'); % Warning message on command window or console
btdevices = instrhwinfo('Bluetooth'); % Defining structure with bluetooth devices found

% [2] Identifying device of interest
%     - As of 10/22/2015, the program searches for the 3M Littmann
%       s1tethoscope only
Ndevices = length(devices.RemoteNames); % Define the number of devices found
for i = 1:Ndevices % Loop through the names of the devices within range   
    name = devices.RemoteNames(i,1);
    comp_index = strcmp(name{1}(1:5),'M3200'); % Name of each device is compared to the default name of the desired device    
    if comp_index == 1 % If the name of the device does match that of the desire device, the program...        
        disp('DEVICE FOUND') % ...displays a message
        btobj = Bluetooth(name{1}, 1) % ...creates a bluetooth object in order to communicate with the device
        break        
    end   
    if i == Ndevices % If the name of the device does not match, the program...       
        disp('DEVICE NOT FOUND'); %...displays a message        
    end   
end


