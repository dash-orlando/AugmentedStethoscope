%
% Identify S1
%
% This function uses the principles of autocorrelation to identify the S1
% signals within auscultation recordings
%
% Fluvio L Lobo Fenoglietto


function [excision] = signalid(audiodata,extract,conv)

    x = audiodata.signal;
    y = s1.signal;
    conv = 0.96;

    % vector lengths
    lenx = length(x);
    leny = length(y);

    % signal autocorrelation
    aC = 0;
    for i = 1:leny
        aC = aC + y(i)*y(i);
    end

    % signal cross-correlation
    C = zeros(lenx-leny,1);
    %nC = zeros(lenx-leny,1);
    for i = 1:lenx-leny  
        for j = 1:leny       
            C(i) = C(i) + x(i+j-1)*y(j);       
        end
        % cross correlation normalization
        nC(i) = C(i)/aC;
        if nC(i) > conv
            nCp(i) = nC(i);
            break
        else
            nCp(i) = 0;
        end
    end

    % excising region of correlation
    corrindex = find(nC > conv);

    excisedx = x(corrindex:corrindex+leny-1);
    
end % End of function