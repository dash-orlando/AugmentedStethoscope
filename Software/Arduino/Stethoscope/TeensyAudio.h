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
float                    mixer1ON         = 1.00;
float                    mixer1OFF        = 0.00;
float                    mixer2ON         = 1.00;
float                    mixer2OFF        = 0.00;
float                    mixerLvL         = 1.00;
float                    cornerFreq       = 750.00;                            // Hz

/// Audio Shield Variables
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioPlaySdRaw           playRaw1;       //xy=581.5999755859375,586
AudioInputI2S            i2s2;           //xy=586.5999755859375,387
AudioMixer4              mixer2;         //xy=861.5999755859375,535
AudioMixer4              mixer1;         //xy=881.5999755859375,331
AudioFilterStateVariable filter2;        //xy=1061.5999969482423,319.1999969482422
AudioFilterStateVariable filter1;        //xy=1071.5999755859375,633
AudioAnalyzePeak         peak2;          //xy=1241.5999755859375,705
AudioRecordQueue         queue1;         //xy=1249.5999450683594,270.00000762939453
AudioAnalyzePeak         peak1;          //xy=1256.5999450683594,366.00000762939453
AudioOutputI2S           i2s1;           //xy=1263.5999755859375,622
AudioRecordQueue         queue2;         //xy=1272.5999755859375,489
AudioConnection          patchCord1(playRaw1, 0, mixer2, 2);
AudioConnection          patchCord2(i2s2, 0, mixer1, 0);
AudioConnection          patchCord3(i2s2, 0, mixer2, 0);
AudioConnection          patchCord4(i2s2, 1, mixer1, 1);
AudioConnection          patchCord5(i2s2, 1, mixer2, 1);
AudioConnection          patchCord6(mixer2, 0, filter1, 0);
AudioConnection          patchCord7(mixer1, 0, filter2, 0);
AudioConnection          patchCord8(filter2, 0, queue1, 0);
AudioConnection          patchCord9(filter2, 0, peak1, 0);
AudioConnection          patchCord10(filter1, 0, i2s1, 0);
AudioConnection          patchCord11(filter1, 0, i2s1, 1);
AudioConnection          patchCord12(filter1, 0, queue2, 0);
AudioConnection          patchCord13(filter1, 0, peak2, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=752.5999755859375,687
// GUItool: end automatically generated code

/// Functions
void SetupAudioBoard()
{
  // Audio connections require memory, and the record queue
  // uses this memory to buffer incoming audio.
  AudioMemory( 60 );

  // Enable the audio shield, select input, and enable output
  sgtl5000_1.enable();
  sgtl5000_1.volume( 0.60 );
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
  filter2.frequency(cornerFreq);                                                // Set Corner Frequency to the default value of 1500.00 Hz or 1.5 kHz
  
} // End of SetupAudioBoard()
