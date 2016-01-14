%
% Prefix Time Stamp
%
% This function was built forthe sake of automatically adding the computer
% time stamp to messages sent to the MATLAB command window. This is
% particularly useful in GUIs or applications running multiple interacting
% features or functions
%
% Fluvio L. Lobo Fenoglietto

function [stampedstring] = timeprefix(string)   
    % determining computer time
    cputime = clock;
    year = num2str(cputime(1));
    month = num2str(cputime(2));
    day = num2str(cputime(3));
    hour = num2str(cputime(4));
    minutes = num2str(cputime(5));
    seconds = num2str(cputime(6));    
    % concatenating strings
    stampedstring = horzcat(month,'/',day,'/',year,' ',hour,':',minutes,':',seconds,' ',string);
end % End of function