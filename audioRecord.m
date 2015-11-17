%
% Audio Record
%
% The following function uses the connected input audio device to record
% audio
%
% Fluvio L. Lobo Fenoglietto
%

function [audiosignal] = audiorecord(audiorecobj, time)
    % [1] Begin recording
    disp('Begin recording...');
    recordblocking(audiorecobj, time); % time in seconds
    % [2] Store recording into double-precision array
    audiosignal = getaudiodata(audiorecobj,'double');
end % End of function