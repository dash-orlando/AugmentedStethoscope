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
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


%
% Bluetooth Device Search
% The button executes the function "btsearch.m"
function pushbutton1_Callback(hObject, ~, handles)

        [btdev] = btsearch(); % searches for available bluetooth devices
        
        Ndev = length(btdev.devices); % determining number of devices found
        outString = horzcat(num2str(Ndev),' devices were found'); % reporting number of devices on string
        set(handles.text5, 'String', outString); % printing report string
    
    handles.btdev = btdev; % stores bluetooth device information into handles structure
    
guidata(hObject, handles);

%
% Bluetooth Device Select
% The button executes the function "btselect.m"
function pushbutton2_Callback(hObject, ~, handles)

    btdev = handles.btdev; % extracting bluetooth device info from handles structure

        [btdevsel] = btselect(btdev); % allows the user to select from the list of found devices
        
        outString = horzcat('User selected ',btdevsel.device{1}); % reporting user selection
        set(handles.text6, 'String', outString); % printing report string 
    
    handles.btdevsel = btdevsel; % stores selected bluetooth device info within handles structure

guidata(hObject, handles);

%
% Bluetooth Device Connect
% The button executes the function "btconnect.m"
function pushbutton3_Callback(hObject, ~, handles)

    btdevsel = handles.btdevsel; % extracting selected bluetooth device info from handles structure

        [btobj] = btconnect(btdevsel); % creates a bluetooth object from the selected bluetooth device
        
        outString = horzcat('Computer connected to ',btdevsel.device{1}); % reporting connection
        set(handles.text7, 'String', outString); % printing report string
        
        set(handles.togglebutton1, 'Visible', 'on'); % set visibility of 'record' toggle button to 'on' 
        set(handles.text8, 'Visible', 'on'); % set visibility of textbox toggle button to 'on' 
        set(handles.togglebutton2, 'Visible', 'on'); % set visibility of 'playback' toggle button to 'on'
    
    handles.btobj = btobj; % stores bluetooth object into handles structure

guidata(hObject, handles);

%
% Recording
% The toggle button will activate the recording functions
function togglebutton1_Callback(hObject, ~, handles)

    % import variables
    
        % functionality
        set(handles.togglebutton2, 'Value', 0); % set the playback toggle button to 'off'
        set(handles.pushbutton5, 'Visible', 'off'); % set playback button visibility 'off'
        
        set(handles.pushbutton4, 'Visible', 'on'); % set record button visibility 'on'
    

guidata(hObject, handles);

%
% Playback
% The toggle button will activate the playback functions
function togglebutton2_Callback(hObject, ~, handles)

    % import variables
    
        % functionality
        set(handles.togglebutton1, 'Value', 0); % set the playback toggle button to 'off'
        set(handles.pushbutton4, 'Visible', 'off'); % set record button visibility 'off'
        
        set(handles.pushbutton5, 'Visible', 'on'); % set playback button visibility 'on'
    

guidata(hObject, handles);


% --- Executes on button press in pushbutton4.
function pushbutton4_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton4 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on button press in pushbutton5.
function pushbutton5_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton5 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
