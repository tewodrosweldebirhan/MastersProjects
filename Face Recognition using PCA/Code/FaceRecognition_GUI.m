function varargout = FaceRecognition_GUI(varargin)
% FACERECOGNITION_GUI MATLAB code for FaceRecognition_GUI.fig
%      FACERECOGNITION_GUI, by itself, creates a new FACERECOGNITION_GUI or raises the existing
%      singleton*.
%
%      H = FACERECOGNITION_GUI returns the handle to a new FACERECOGNITION_GUI or the handle to
%      the existing singleton*.
%
%      FACERECOGNITION_GUI('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in FACERECOGNITION_GUI.M with the given input arguments.
%
%      FACERECOGNITION_GUI('Property','Value',...) creates a new FACERECOGNITION_GUI or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before FaceRecognition_GUI_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to FaceRecognition_GUI_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help FaceRecognition_GUI

% Last Modified by GUIDE v2.5 27-Dec-2018 17:55:35

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @FaceRecognition_GUI_OpeningFcn, ...
                   'gui_OutputFcn',  @FaceRecognition_GUI_OutputFcn, ...
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


% --- Executes just before FaceRecognition_GUI is made visible.
function FaceRecognition_GUI_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to FaceRecognition_GUI (see VARARGIN)

% Choose default command line output for FaceRecognition_GUI
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes FaceRecognition_GUI wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = FaceRecognition_GUI_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on button press in chooseImageBtn.
function chooseImageBtn_Callback(hObject, eventdata, handles)
% hObject    handle to chooseImageBtn (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
[filename pathname] = uigetfile('*.jpg','Image Selector');
faceImg_path = strcat(pathname,filename);
faceImage= imread(faceImg_path);
axes(handles.originalImgAxes);
imshow(faceImage,[]);
set(handles.filenameTxt,'string',filename);
%save the image path
handles.img_path = faceImage;
guidata(hObject, handles);
% --- Executes on button press in faceMatchBtn.
function faceMatchBtn_Callback(hObject, eventdata, handles)
% hObject    handle to faceMatchBtn (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
faceImg_Input = handles.img_path;

[fcimg1, fcimg2, fcimg3] = connectPCA(faceImg_Input);

axes(handles.matchedImgAxes2);
imshow(fcimg1,[]);

axes(handles.matchedImgAxes3);
imshow(fcimg2,[]);

axes(handles.matchedImgAxes4);
imshow(fcimg3,[]);
% Face recognition Algorithm using PCA
