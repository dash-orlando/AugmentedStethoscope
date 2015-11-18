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
    disp(horzcat(num2str(time),' seconds elapsed, recording completed...'));
    % [2] Store recording into double-precision array
    audiosignal = getaudiodata(audiorecobj,'double');
    disp('Audio data retrieved to workspace...');
end % End of function