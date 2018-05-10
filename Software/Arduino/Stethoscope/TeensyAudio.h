/*
 * Teensy Audio
 * This script can be best described as the configuration program for the Teensy audio shield
 * 
 * Michael Xynidis
 * Fluvio L Lobo Fenoglietto
 * 11/17/2016
 */

// ==============================================================================================================
// Import libraries and/or modules
// ============================================================================================================== //
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s_mic;        //xy=115,238
AudioPlaySdRaw           playRaw_sdHeartSound; //xy=164,464
AudioMixer4              rms_mic_mixer;  //xy=455,186
AudioMixer4              rms_playRaw_mixer; //xy=457,281
AudioAnalyzePeak         mic_peaks;      //xy=631,64
AudioAnalyzePeak         playRaw_peaks;  //xy=646,386
AudioAnalyzeRMS          mic_rms;        //xy=660,122
AudioAnalyzeRMS          playRaw_rms;    //xy=670,331
AudioMixer4              mixer_mic_Sd;   //xy=723,233
AudioFilterStateVariable filter_LowPass_2; //xy=746,470
AudioFilterStateVariable filter_LowPass_1; //xy=935,160
AudioMixer4              mixer_allToSpk; //xy=1002,490
AudioRecordQueue         queue_recMic;   //xy=1187,220
AudioRecordQueue         queue_recSpk;         //xy=1191,620
AudioOutputI2S           i2s_speaker;    //xy=1236,515
AudioAnalyzePeak         peak_QrsMeter;  //xy=1243,433
AudioConnection          patchCord1(i2s_mic, 0, filter_LowPass_2, 0);
AudioConnection          patchCord2(i2s_mic, 0, rms_mic_mixer, 0);
AudioConnection          patchCord3(i2s_mic, 1, filter_LowPass_2, 1);
AudioConnection          patchCord4(i2s_mic, 1, rms_mic_mixer, 1);
AudioConnection          patchCord5(playRaw_sdHeartSound, 0, rms_playRaw_mixer, 0);
AudioConnection          patchCord6(rms_mic_mixer, 0, mixer_mic_Sd, 0);
AudioConnection          patchCord7(rms_mic_mixer, mic_peaks);
AudioConnection          patchCord8(rms_mic_mixer, mic_rms);
AudioConnection          patchCord9(rms_playRaw_mixer, 0, mixer_mic_Sd, 1);
AudioConnection          patchCord10(rms_playRaw_mixer, playRaw_rms);
AudioConnection          patchCord11(rms_playRaw_mixer, playRaw_peaks);
AudioConnection          patchCord12(mixer_mic_Sd, 0, filter_LowPass_1, 0);
AudioConnection          patchCord13(filter_LowPass_2, 0, mixer_allToSpk, 1);
AudioConnection          patchCord14(filter_LowPass_1, 0, queue_recMic, 0);
AudioConnection          patchCord15(filter_LowPass_1, 0, mixer_allToSpk, 0);
AudioConnection          patchCord16(mixer_allToSpk, peak_QrsMeter);
AudioConnection          patchCord17(mixer_allToSpk, 0, i2s_speaker, 0);
AudioConnection          patchCord18(mixer_allToSpk, 0, i2s_speaker, 1);
AudioConnection          patchCord19(mixer_allToSpk, queue_recSpk);
AudioControlSGTL5000     sgtl5000_1;     //xy=124,136
// GUItool: end automatically generated code


// ==============================================================================================================
// Variables
// ============================================================================================================== //
elapsedMillis             fps;
uint8_t                   cnt             =     0;
const int                 selectedInput   = AUDIO_INPUT_MIC;
int                       microphoneGain  =     30;
float                     micInputLvL     =     0.50;
float                     sampleInputLvL  =     0.50;
float                     speakerVolume   =     0.65;                           // 2-speaker: 0.50; 1-speaker: 0.60

float                     mixerInputON    =     1.00;
float                     mixerInputOFF   =     0.00;
float                     mixerLvL        =     1.00;

float                     bpLowPassFreq   =   250.0;                            // Low pass from HB sample
float                     bpHighPassFreq  =   500.0;                            // High pass from mic

float                     freqHighShelf   =  2000.0f;
float                     tGain1          =   -20.0f;                           // ...may need to fiddle with this one
float                     slope1          =    1.0f;

float                     freqLowPass     = 2000.0f;                            // prjc example uses 800.0
float                     Q2              =    0.707f;                          // 0.707 = Butterworth Filter value;  min: 0.1;
String                    fileName        = "";                                 // String with sound file name

// ==============================================================================================================
// Setup
// ============================================================================================================== //
void setupMicToSpeaker()
{
  Serial.println( "SETTING UP lines from Microphone to Speaker" );
  // rms mic mixer ---------------------------------------------------------------------------------------------- //
  rms_mic_mixer.gain(   0, mixerInputON  );
  rms_mic_mixer.gain(   1, mixerInputON  );
  // mixer mic SD ----------------------------------------------------------------------------------------------- //
  mixer_mic_Sd.gain(    0, mixerInputON  );                                                                       // Set gain of mixer_mic_Sd, channel0 to 1.00
  // mixer all to speaker  -------------------------------------------------------------------------------------- //
  mixer_allToSpk.gain(  0, mixerInputON  );                                                                       // Normal stethoscope mic input (on)
  mixer_allToSpk.gain(  1, mixerInputOFF );                                                                       // Highpass mic input (off)
}

void setupSDToSpeaker()
{
  // rms play raw mixer ----------------------------------------------------------------------------------------- //
  rms_playRaw_mixer.gain(   0, mixerInputON  );
  // mixer mic SD  ---------------------------------------------------------------------------------------------- //
  mixer_mic_Sd.gain(    1, mixerInputOFF  );                                                                      // Set gain of mixer_mic_Sd, channel1 to 0.00 --SD file input
}


void SetupAudioBoard()
{
  // Audio connections require memory, and the record queue
  // uses this memory to buffer incoming audio.
  AudioMemory( 60 );

  // Enable the audio shield, select input, and enable output
  sgtl5000_1.enable();
  sgtl5000_1.volume(      speakerVolume  );
  sgtl5000_1.inputSelect( selectedInput  );
  sgtl5000_1.micGain(     microphoneGain );

  // Configure SPI for the audio shield pins
  SPI.setMOSI( 7 );                                                             // Audio shield has MOSI on pin 7
  SPI.setSCK( 14 );                                                             // Audio shield has SCK on pin 14

  setupMicToSpeaker();
  setupSDToSpeaker();

  filter_LowPass_1.frequency( 500 );
  filter_LowPass_2.frequency( 500 );
  
} // End of SetupAudioBoard()

