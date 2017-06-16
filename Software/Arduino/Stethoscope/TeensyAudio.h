/*
 * Teensy Audio
 * This script can be best described as the configuration program for the Teensy audio shield
 * 
 * Michael Xynidis
 * Fluvio L Lobo Fenoglietto
 * 11/17/2016
 */


/// Import or Include Libraries
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s_mic;           //xy=80.99998474121094,127
AudioPlaySdRaw           playRaw_sdHeartSound;       //xy=86.99998474121094,185.99998474121094
AudioPlayMemory          playMem_heartSoundSamp;       //xy=87.99998474121094,245.99998474121094
AudioFilterStateVariable filter3;        //xy=302,126.99998474121094
AudioMixer4              mixer2;         //xy=302,201.99998474121094
AudioFilterStateVariable filter4;        //xy=302,278
AudioFilterStateVariable filter1;        //xy=456,141.99998474121094
AudioMixer4              mixer3;         //xy=533,259
AudioRecordQueue         queue_recMic;         //xy=688,130
AudioAnalyzePeak         peak_QrsMeter;          //xy=688,177.99998474121094
AudioOutputI2S           i2s_speaker;           //xy=688,228.99998474121094
AudioConnection          patchCord2(i2s_mic, 0, mixer2, 0);
AudioConnection          patchCord3(i2s_mic, 0, filter3, 0);
AudioConnection          patchCord5(i2s_mic, 1, mixer2, 1);
AudioConnection          patchCord6(i2s_mic, 1, filter3, 1);
AudioConnection          patchCord7(playRaw_sdHeartSound, 0, mixer2, 2);
AudioConnection          patchCord8(playMem_heartSoundSamp, 0, filter4, 0);
AudioConnection          patchCord10(filter3, 2, mixer3, 1);
AudioConnection          patchCord11(mixer2, 0, filter1, 0);
AudioConnection          patchCord12(filter4, 0, mixer3, 2);
AudioConnection          patchCord15(filter1, 0, queue_recMic, 0);
AudioConnection          patchCord16(filter1, 0, peak_QrsMeter, 0);
AudioConnection          patchCord17(filter1, 0, mixer3, 0);
AudioConnection          patchCord18(mixer3, 0, i2s_speaker, 0);
AudioConnection          patchCord19(mixer3, 0, i2s_speaker, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=88.99998474121094,69
// GUItool: end automatically generated code


/// Variable Definitions
const int                 selectedInput   = AUDIO_INPUT_MIC;
int                       microphoneGain  =  36;
float                     micInputLvL     =   0.50;
float                     sampleInputLvL  =   0.50;
float                     speakerVolume   =   0.80;                             // 2-speaker: 0.50; 1-speaker: 0.60
float                     mixerInputON    =   1.00;
float                     mixerInputOFF   =   0.00;
float                     mixerLvL        =   1.00;
float                     cornerFreq      = 750.00;                             // Hz

float                     bpLowPassFreq   = 250.0;                              // Low pass from HB sample
float                     bpHighPassFreq  = 500.0;                              // High pass from mic


/// Functions
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

  mixer2.gain( 0, mixerInputON  );                                              // Set gain of mixer2, channel0 to 0.25 - Microphone on
  mixer2.gain( 1, mixerInputON  );                                              // Set gain of mixer2, channel1 to 0.25 - Microphone on
  mixer2.gain( 2, mixerInputOFF );                                              // Set gain of mixer2, channel2 to 0
  mixer3.gain( 0, mixerInputON  );                                              // Normal stethoscope mic input (on)
  mixer3.gain( 1, mixerInputOFF );                                              // Highpass mic input (off)
  mixer3.gain( 2, mixerInputOFF );                                              // HB-sample playback (off)

  // Configure filters for heartsound listening
  filter1.frequency( cornerFreq );                                              // Set Corner Frequency to the default value of 1500.00 Hz or 1.5 kHz

  // Configure filters for BP Augmentation
  filter3.frequency( bpHighPassFreq );
  filter4.frequency( bpLowPassFreq  );
  
} // End of SetupAudioBoard()
