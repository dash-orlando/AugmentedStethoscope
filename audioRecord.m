%
%
% Record Audio
%
% This function records audio from the input recording device selected by
% the user

% Fluvio L. Lobo Fenoglietto


function [recObj] = audioRecord(Fs, nBits, nChannels, selectedInputDeviceID)

    recObj = audiorecorder(Fs,nBits,nChannels,selectedInputDeviceID);
    
    
    recordingdlg = questdlg('Would you like to begin recording?', ...
                            'Start Recording', ...
                            'Yes','No','Yes');
    % Handle response
    switch recordingdlg
        case 'Yes'
            
            record(recObj);
            
            
        case 'No'
            break
    end 
    
    
end