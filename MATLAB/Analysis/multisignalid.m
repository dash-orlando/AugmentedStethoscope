%
% Multiple Signal ID
%
% This function extends the functionality of signalid.m by allowing the
% user to identify several extractions or signatures within the same signal
%
% Fluvio L Lobo Fenoglietto
%

function [multiexcision] = multisignalid(audiodata,extract,conv,nmatch,vis)

    % loop around the desired number of matches/identifications
    for i = 1:nmatch
        % creating subdivision of output structure
        exid = strcat('ex',num2str(i));
        % performing single-signal-identification
        [multiexcision.(exid)] = signalid(audiodata,extract,conv);
        % deleting excision from signal
        audiodata.signal = audiodata.signal - multiexcision.(exid).compsignal;
    end % end of for-loop
    
    [stampedstring] = timeprefix('Multi excision completed');
    disp(stampedstring);
    [stampedstring] = timeprefix(horzcat(num2str(nmatch),' excisions performed'));
    disp(stampedstring);

    if strcmp(vis,'on') == 1
        
        [stampedstring] = timeprefix('User enabled output visualization');
        disp(stampedstring);
        
        % plotting
        figure,
        
        subplot 211
            plot(audiodata.time,audiodata.signal,'b');
            hold on
            legendlabels = cell(nmatch+1,1);
            legendlabels(1,1) = {'Original Signal'}; 
            for i = 1:nmatch
                exid = strcat('ex',num2str(i));
                plot(multiexcision.(exid).comptime,multiexcision.(exid).compsignal,'Color',rand(1,3));
                legendlabels(i+1,1) = {horzcat('Excision ',num2str(i))};
            end
            xlabel('Time (sec.)');
            ylabel('Amplitude (V)');
            title('Multiple Signal Identification');
            legend(legendlabels);
            grid on

        subplot 212
            plot(audiodata.time,audiodata.signal,'b');
            hold on
            lasttimepoint = zeros(nmatch,1);
            legendlabels = cell(nmatch+1,1);
            legendlabels(1,1) = {'Original Signal'}; 
            for i = 1:nmatch
                exid = strcat('ex',num2str(i));
                plot(multiexcision.(exid).comptime,multiexcision.(exid).compsignal,'Color',rand(1,3));
                lasttimepoint(i,1) = multiexcision.(exid).time(end);
                legendlabels(i+1,1) = {horzcat('Excision ',num2str(i))};
            end
            xlabel('Time (sec.)');
            ylabel('Amplitude (V)');
            legend(legendlabels);
            axis([0 max(lasttimepoint)+1 min(audiodata.signal) max(audiodata.signal)]); 
            grid on
            
    elseif strcmp(vis,'off') == 1
        
        [stampedstring] = timeprefix('User disabled output visualization');
        disp(stampedstring);
        
    end % end of if-statement

end % End of function