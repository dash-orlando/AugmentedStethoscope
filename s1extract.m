%
% Extract S1
%
% This function has been designed to allow for an expert to identify an S1
% signal from an entire auscultation recording

function [s1] = s1extract(audiodata)

    % importing data from audiodata
    [stampedstring] = timeprefix('Importing audio signal from audiodata');
    disp(stampedstring);
    time = audiodata.time;
    signal = audiodata.signal;
    
    % identifying s1 signal using cursors
    [stampedstring] = timeprefix('Plotting audio signal');
    disp(stampedstring);
    signalfig = figure;
    plot(time, signal)
    title('Audio Signal');
    xlabel('Time (sec.)');
    ylabel('Apmplitude (V)');
    grid on
    
    [stampedstring] = timeprefix('Waiting for user to close figure');
    disp(stampedstring);
    waitfor(signalfig)
    
    % extracting cursor information
    if exist('cursor_info') == 1
        
        [stampedstring] = timeprefix('Cursor information found');
        disp(stampedstring);
        
        Npoints = length(cursor_info);
        
        if Npoints == 2
            
            [stampedstring] = timeprefix('Two points found');
            disp(stampedstring);
            
            pointX = zeros(Npoints,1);
            for i = 1:Npoints
                
                pointX(i) = cursor_info(i).Position(i);
                
            end
            
            pointXsorted = sort(pointX,'ascend');
            
            % extracting section of signal between cursor points
            
            pindex 
                
end 