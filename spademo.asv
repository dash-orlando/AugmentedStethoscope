%
% Signal Processing and Analysis Demo
%
% This script has been designed to demo the processign and analysis
% features designed for acoustic files, recordings, and/or signals.
%
% Fluvio L Lobo Fenoglietto
%

%% [1] Signal Import
%      Import signal from disk
[audiodata] = importaudio();

%% [2] Signal Extract
%      Prior to identifying acoustic signatures within a complete signal or
%      recording, an instance of the target signature must be obtained.
%      The following function allows for the user to extract the target
%      signature from an imported audio signal
[extract] = signalextract(audiodata);

%% [3] Signal Identification -Single
%      With a known target acoustic signature, the following function
%      sweeps through an imported audio signal, or recording, and finds A
%      SINGLE region of high correlation or match. The program further
%      excises this section, considerings its absolute position within the
%      time of the original audio signal
conv = 0.95; % the user must specify a correlation or convergence tolerance
             % for the function to identify a match. Here, the signature
             % must have 95% correlation with the signal in order to
             % declare a match
[excision] = signalid(audiodata,extract,conv);

%% [4] Signal Identification -Multiple
%      The following funtion expands on signal identification by allowing
%      the user to identify the same signature multiple times within the
%      original signal
conv = 0.95; % convergence criteria
nmatch = 2; % number of matches or convergences the user desired to find
vis = 'on'; % turns visualization of results 'on' within the function
[multiexcision] = multisignalid(audiodata,extract,conv,nmatch,vis);

%% [5] Simple Spectral Analysis
%      This function was designed to determine the power spectral density
%      of an input signal using Fourier transformations. The function
%      calculates the error between its own process and the MATLAB
%      periodogram functionality
%
% [5.1] Single File (Excision)
signal = excision.signal;
fs = excision.fs;
[psdx, error] = ssa(signal,fs);

% [5.2] Multiple Files (MultiExcision)
Nexcisions = length(fieldnames(multiexcision));
for i = 1:Nexcisions
    exid = strcat('ex',num2str(i));
    signal = multiexcision.(exid).signal;
    fs = multiexcision.(exid).fs;
    [psdx, error] = ssa(signal,fs);
end







