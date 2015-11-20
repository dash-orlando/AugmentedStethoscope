function varargout = splitsgui(varargin)


% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @splitsgui_OpeningFcn, ...
                   'gui_OutputFcn',  @splitsgui_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before splitsgui is made visible.
function splitsgui_OpeningFcn(hObject, ~, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to splitsgui (see VARARGIN)

% Choose default command line output for splitsgui
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes splitsgui wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = splitsgui_OutputFcn(hObject, ~, handles) 

    varargout{1} = handles.output;

guidata(hObject, handles);


%
% Bluetooth Device Search
% The button executes the function "btsearch.m"
function searchbtpush_Callback(hObject, ~, handles)

        [btdev] = btsearch(); % searches for available bluetooth devices
        
        Nbtdev = length(btdev.devices); % determining number of devices found
        outString = horzcat(num2str(Nbtdev),' devices were found'); % reporting number of devices on string
        set(handles.searchbtstatus, 'String', outString); % printing report string
    
    handles.btdev = btdev; % stores bluetooth device information into handles structure
    
guidata(hObject, handles);

%
% Bluetooth Device Select
% The button executes the function "btselect.m"
function selectbtpush_Callback(hObject, ~, handles)

    btdev = handles.btdev; % extracting bluetooth device info from handles structure

        [btdevsel] = btselect(btdev); % allows the user to select from the list of found devices
        
        outString = horzcat('User selected ',btdevsel.device{1}); % reporting user selection
        set(handles.selectbtstatus, 'String', outString); % printing report string 
    
    handles.btdevsel = btdevsel; % stores selected bluetooth device info within handles structure

guidata(hObject, handles);

%
% Bluetooth Device Connect
% The button executes the function "btconnect.m"
function connectbtpush_Callback(hObject, ~, handles)

    btdevsel = handles.btdevsel; % extracting selected bluetooth device info from handles structure

        [btobj] = btconnect(btdevsel); % creates a bluetooth object from the selected bluetooth device
        
        outString = horzcat('Computer connected to ',btdevsel.device{1}); % reporting connection
        set(handles.connectbtstatus, 'String', outString); % printing report string
        
        set(handles.recordtoggle, 'Visible', 'on'); % set visibility of 'record' toggle button to 'on' 
        set(handles.or, 'Visible', 'on'); % set visibility of textbox toggle button to 'on' 
        set(handles.playbacktoggle, 'Visible', 'on'); % set visibility of 'playback' toggle button to 'on'
    
    handles.btobj = btobj; % stores bluetooth object into handles structure

guidata(hObject, handles);

%
% Recording
% The toggle button will activate the recording functions
function recordtoggle_Callback(hObject, ~, handles)

    % import variables
    
        % functionality
        % set all playback features to 'hidden'
        set(handles.playbacktoggle, 'Value', 0);
        set(handles.searchplaypush, 'Visible', 'off');
        set(handles.playtitle, 'Visible', 'off');
        
        % set all recording features to 'visible'
        set(handles.rectitle, 'Visible', 'on');
        set(handles.searchrecpush, 'Visible', 'on');
        set(handles.searchrecstatus, 'Visible', 'on');
        set(handles.selectrecpush, 'Visible', 'on');
        set(handles.selectrecstatus, 'Visible', 'on');
        set(handles.connectrecpush, 'Visible', 'on');
        set(handles.connectrecstatus, 'Visible', 'on');
    

guidata(hObject, handles);

%
% Playback
% The toggle button will activate the playback functions
function playbacktoggle_Callback(hObject, ~, handles)

    % import variables
    
        % functionality
        % set all recording features to 'hidden'
        set(handles.rectitle, 'Visible', 'off');
        set(handles.searchrecpush, 'Visible', 'off');
        set(handles.searchrecstatus, 'Visible', 'off');
        set(handles.selectrecpush, 'Visible', 'off');
        set(handles.selectrecstatus, 'Visible', 'off');
        set(handles.connectrecpush, 'Visible', 'off');
        set(handles.connectrecstatus, 'Visible', 'off');
        set(handles.startrecpush, 'Visible', 'off');
        set(handles.startrectitle, 'Visible', 'off');
        set(handles.stoprecpush, 'Visible', 'off');
        set(handles.stoprectitle, 'Visible', 'off');
        set(handles.displayrecpush, 'Visible', 'off');
        set(handles.displayrectitle, 'Visible', 'off');
        set(handles.writerecpush, 'Visible', 'off');
        set(handles.writerectitle, 'Visible', 'off');
        
        % set all playback features to 'visible'
        set(handles.playtitle, 'Visible', 'on');
        set(handles.searchplaypush, 'Visible', 'on');
         

guidata(hObject, handles);

%
% Audio Input/Recording Device Search
% The button executes the function "audiorecsearch.m"
function searchrecpush_Callback(hObject, ~, handles)

    % import data from handles
    
        % executions
        [audiorecdev] = audiorecsearch();
        
        Nrecdev = length(audiorecdev.devices); % determining number of devices found
        outString = horzcat(num2str(Nrecdev),' devices were found'); % reporting number of devices on string
        set(handles.searchrecstatus, 'String', outString); % printing report string
        
    % export to handles
    handles.audiorecdev = audiorecdev;

guidata(hObject, handles);

%
% Audio Input/Recording Device Selection
% The button executes the function "audiorecselect.m"
function selectrecpush_Callback(hObject, ~, handles)

    audiorecdev = handles.audiorecdev; % extracting bluetooth device info from handles structure

        [audiorecsel] = audiorecselect(audiorecdev); % allows the user to select from the list of found devices
        
        outString = horzcat('User selected ',audiorecsel.device{1}); % reporting user selection
        set(handles.selectrecstatus, 'String', outString); % printing report string 
    
    handles.audiorecsel = audiorecsel; % stores selected bluetooth device info within handles structure

guidata(hObject, handles);

%
% Audio Input/Recording Device Connect
% The button executes the function "audiorecon.m"
function connectrecpush_Callback(hObject, ~, handles)

    audiorecsel = handles.audiorecsel; % extracting selected bluetooth device info from handles structure
        
        Fs = 8000; % using default frequency
        nBits = 16; % usign 16 bits instead of 8 for default
        nChannels = 1; % using the default 'mono'
        [audiorecobj] = audiorecon(audiorecsel, Fs, nBits, nChannels); % creates a bluetooth object from the selected bluetooth device
        
        outString = horzcat('Computer connected to ',audiorecsel.device{1}); % reporting connection
        set(handles.connectrecstatus, 'String', outString); % printing report string
        
        % set recording controls to 'visible'
        set(handles.startrecpush, 'Visible', 'on');
        set(handles.startrectitle, 'Visible', 'on');
        set(handles.stoprecpush, 'Visible', 'on');
        set(handles.stoprectitle, 'Visible', 'on');
        set(handles.displayrecpush, 'Visible', 'on');
        set(handles.displayrectitle, 'Visible', 'on');
        set(handles.writerecpush, 'Visible', 'on');
        set(handles.writerectitle, 'Visible', 'on');
    
    handles.audiorecobj = audiorecobj; % stores bluetooth object into handles structure
    handles.recparams.Fs = Fs; % store recording hardware parameters
    handles.recparams.nBits = nBits;
    handles.recparams.nChannels = nChannels;

guidata(hObject, handles);

%
% Start Recording
% The button executes the commands to begin recording from the selected
% input device
function startrecpush_Callback(hObject, ~, handles)

    % import variables from handles
    audiorecobj = handles.audiorecobj;
    
        % functionality
        set(handles.recstatus, 'String', 'Begin recording');
        record(audiorecobj);
        
    % export variables to guidata

guidata(hObject, handles);

%
% Stop Recording
% The button executes the commands to stop recording from the selected
% input device
function stoprecpush_Callback(hObject, ~, handles)

    % import variables from handles
    audiorecobj = handles.audiorecobj;
    
        % functionality
        set(handles.recstatus, 'String', 'Recording stopped');
        stop(audiorecobj);
        
    % export variables to guidata

guidata(hObject, handles);

%
% Display Recording
% This button executes a routine that displays the recorded audio signal
function displayrecpush_Callback(hObject, ~, handles)

    % import variables from handles
    audiorecobj = handles.audiorecobj;
    
        % functionality
        signal = getaudiodata(audiorecobj);
        figure,
        plot(signal);
        
    % export variables to handles
    handles.signal = signal;

guidata(hObject, handles);

%
% Write Recording
% This button executes a routine to write the audio recording to an audio
% file (.mp3)
function writerecpush_Callback(hObject, ~, handles)

    %import variables from handles
    audiorecdata.signal = handles.signal;
    audiorecdata.fs = handles.recparams.Fs;
    
        %functionality
        exportaudio(audiorecdata);
        
    %export variables to handles

guidata(hObject, handles);

%
% Audio Output/Playback Device Search
% This button executes the 'audioplaysearch' function
function searchplaypush_Callback(hObject, ~, handles)

    % import data from handles
    
        % executions
        [audioplaydev] = audioplaysearch();
        
        Nplaydev = length(audioplaydev.devices);
        outString = horzcat(num2str(Nplaydev),' devices were found');
        set(handles.searchrecstatus, 'String', outString);
        
    % export to handles
    handles.audioplaydev = audioplaydev;

guidata(hObject, handles);

%
% Audio Output/Playback Device Selection
% This button executes the 'audioplayselect' function
function selectplaypush_Callback(hObject, ~, handles)

    audioplaydev = handles.audioplaydev;

        [audioplaysel] = audiorecselect(audioplaydev);
        
        outString = horzcat('User selected ',audiorecsel.device{1});
        set(handles.selectplaystatus, 'String', outString);
    
    handles.audioplaysel = audioplaysel;

guidata(hObject, handles);

%
% Audio Import
% This button executes the 'audioimport' function
function importplaypush_Callback(hObject, ~, handles)

    % import variables from handles
    
        % executions
        [audioplaydata] = importaudio();
        set(handles.importplaystatus, 'String', 'Accessing disk');
        
    % export variables to handles
    handles.audioplaydata = audioplaydata;

guidata(hObject, handles);


%
% Audio Output/Playback Device Connection
% This button executes the 'audioplaycon' function
function connectplaypush_Callback(hObject, ~, handles)

    % importing variables from handles
    audioplaysel = handles.audioplaysel;
    audioplaydata = handles.audioplaydata;
        
        % executions
        nBits = 16;
        [audioplayobj] = audioplaycon(audioplaydata, audioplaysel, nBits); % creates a bluetooth object from the selected bluetooth device
        
        outString = horzcat('Computer connected to ',audioplaysel.device{1}); % reporting connection
        set(handles.connectplaystatus, 'String', outString); % printing report string
        
        % set recording controls to 'visible'
        set(handles.startplaypush, 'Visible', 'on');
        set(handles.startplaytitle, 'Visible', 'on');
        set(handles.stopplaypush, 'Visible', 'on');
        set(handles.stopplaytitle, 'Visible', 'on');
        set(handles.displayplaypush, 'Visible', 'on');
        set(handles.displayplaytitle, 'Visible', 'on');
        set(handles.writeplaypush, 'Visible', 'on');
        set(handles.writeplaytitle, 'Visible', 'on');
    
    % exporting variables to handles
    handles.audioplayobj = audioplayobj;

guidata(hObject, handles);

%
% Start Playback
% This button begins the playback of the imported audio file
function startplaypush_Callback(hObject, ~, handles)

    % import variables from handles
    audioplayobj = handles.audioplayobj;
    
        % functionality
        set(handles.recstatus, 'String', 'Begin recording');
        play(audioplayobj);
        
    % export variables to guidata

guidata(hObject, handles);



% --- Executes on button press in displayplaypush.
function displayplaypush_Callback(hObject, ~, handles)
% hObject    handle to displayplaypush (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)



