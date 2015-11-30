%
% Extract Sound Signal
%
% This function has been designed to allow for an expert to identify an S1
% signal from an entire auscultation recording
%
% Fluvio L Lobo Fenoglietto

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
    % using cursor to select sections of the signal
    cursorobj = datacursormode(signalfig);
    [stampedstring] = timeprefix('Use data tips to excised a section of the signal');
    disp(stampedstring);
    [stampedstring] = timeprefix('Once data tips have been placed, hit enter on the keyboard');
    disp(stampedstring);
    pause % program is paused until the return/enter key is pressed
    cursordata = getCursorInfo(cursorobj);
    [stampedstring] = timeprefix('Waiting for user to close figure');
    disp(stampedstring);
    waitfor(signalfig);   
    % extracting cursor information
    if exist('cursordata','var') == 1
        [stampedstring] = timeprefix('Cursor information found');
        disp(stampedstring);
        Npoints = length(cursordata);
        if Npoints == 2
            [stampedstring] = timeprefix('Two points found');
            disp(stampedstring);
            % extracting point/datatip coordinates from cursordata
            pointX(1,1) = cursordata(1).Position(1);
            pointX(2,1) = cursordata(2).Position(1);
            % sorting points in ascending order
            pointXsorted = sort(pointX,'ascend');
            % extracting section of signal between cursor points
            pindex(1,1) = find(time == pointXsorted(1,1));
            pindex(2,1) = find(time == pointXsorted(2,1));
            % storing S1 signal
            s1.time = time(pindex(1,1):pindex(2,1));
            s1.signal = signal(pindex(1,1):pindex(2,1));
            % plotting S1
            figure,
            plot(s1.time, s1.signal)
            xlabel('Time (sec.)');
            ylabel('Amplitude (V)');
            grid on
        else
            [stampedstring] = timeprefix(horzcat(num2str(Npoints),' points found. The program needs 2 points'));
            disp(stampedstring);
            s1 = [];
        end % End of if-statement -check for number of points exported
    else 
        [stampedstring] = timeprefix('Cursor information was not exported to workspace');
        disp(stampedstring);
        s1 = [];
    end % End of if-statement -check for existance of cursordata

end
