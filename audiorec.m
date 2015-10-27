%
%
% Audio Recording
%
% This function has been designed to record audio from all the audio inputs
% identified by Windows




% Identify Preferred Recording Device


info = audiodevinfo;

Nidev = length(info.input); % number of input devices

inputDevicesList = cell(Nidev,1);

for i = 1:Nidev
    inputDevicesList{i,1} = info.input(i).Name;   
end

[selection,status] = listdlg('ListString',inputDevicesList,...
                             'SelectionMode','single',...
                             'ListSize',[400, 100],...
                             'InitialValue',1,...
                             'Name','Recording Device Selection',...
                             'PromptString','Select one of the input devices listed',...
                             'OKString','OK',...
                             'CancelString','Cancel');
                         
inputDeviceID = info.input(selection).ID