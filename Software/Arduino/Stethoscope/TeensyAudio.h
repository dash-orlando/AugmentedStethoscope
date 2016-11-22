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

/// Variable Definitions
const int                myInput          = AUDIO_INPUT_MIC;
float                    mixer1ON         = 0.50;
float                    mixer1OFF        = 0.00;
float                    mixer2ON         = 1.00;
float                    mixer2OFF        = 0.00;
float                    cornerFreq       = 1500.00;                            // Hz

/// Audio Shield Variables
// GUItool: begin automatically generated code
AudioPlaySdRaw           playRaw1;       //xy=562.5,537
AudioInputI2S            i2s2;           //xy=567.5,338
AudioMixer4              mixer2;         //xy=842.5,486
AudioMixer4              mixer1;         //xy=862.5,282
AudioFilterStateVariable filter1;        //xy=1052.5,584
AudioAnalyzePeak         peak1;          //xy=1069.5,301
AudioRecordQueue         queue1;         //xy=1077.5,238
AudioAnalyzePeak         peak2;          //xy=1222.5999969482423,656.1999969482422
AudioOutputI2S           i2s1;           //xy=1244.5,573
AudioRecordQueue         queue2;         //xy=1253.5,440
AudioConnection          patchCord1(playRaw1, 0, mixer2, 2);
AudioConnection          patchCord2(i2s2, 0, mixer1, 0);
AudioConnection          patchCord3(i2s2, 0, mixer2, 0);
AudioConnection          patchCord4(i2s2, 1, mixer1, 1);
AudioConnection          patchCord5(i2s2, 1, mixer2, 1);
AudioConnection          patchCord6(mixer2, 0, filter1, 0);
AudioConnection          patchCord7(mixer1, peak1);
AudioConnection          patchCord8(mixer1, queue1);
AudioConnection          patchCord9(filter1, 0, i2s1, 0);
AudioConnection          patchCord10(filter1, 0, i2s1, 1);
AudioConnection          patchCord11(filter1, 0, queue2, 0);
AudioConnection          patchCord12(filter1, 0, peak2, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=733.5000152587891,638.0000152587891
// GUItool: end automatically generated code

/// Functions
void SetupAudioBoard()
{
  // Audio connections require memory, and the record queue
  // uses this memory to buffer incoming audio.
  AudioMemory( 60 );

  // Enable the audio shield, select input, and enable output
  sgtl5000_1.enable();
  sgtl5000_1.volume( 0.80 );
  sgtl5000_1.inputSelect( myInput );
  sgtl5000_1.micGain( 35 );

  // Configure SPI for the audio shield pins
  SPI.setMOSI( 7 );                                                             // Audio shield has MOSI on pin 7
  SPI.setSCK( 14 );                                                             // Audio shield has SCK on pin 14

  mixer1.gain(0,mixer1OFF);                                                     // Set gain of mixer1, channel0 to 0
  mixer1.gain(1,mixer1OFF);                                                     // Set gain of mixer1, channel1 to 1
  mixer2.gain(0,mixer2ON);                                                      // Set gain of mixer2, channel0 to 0.25 - Microphone on
  mixer2.gain(1,mixer2ON);                                                      // Set gain of mixer2, channel0 to 0.25 - Microphone on
  mixer2.gain(2,mixer2OFF);                                                     // Set gain of mixer2, channel2 to 0

  // Configure Filter
  filter1.frequency(cornerFreq);                                                // Set Corner Frequency to the default value of 1500.00 Hz or 1.5 kHz
  
} // End of SetupAudioBoard()
