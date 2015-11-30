%
% Multiple Signal ID
%
% This function extends the functionality of signalid.m by allowing the
% user to identify several extractions or signatures within the same signal
%
% Fluvio L Lobo Fenoglietto
%

nmatch = 2;
conv = 0.95;

baseaudio = audiodata;
extract = s1;

for i = 1:nmatch
    
    [excision] = signalid(baseaudio,extract,conv);
    
    % deleting excision from signal
    baseaudio.signal = baseaudio.signal - excision.compsignal;

end