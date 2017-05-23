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
const int                 myInput         = AUDIO_INPUT_MIC;

float                     mixer1ON        =    1.00;
float                     mixer1OFF       =    0.00;
float                     mixer2ON        =    1.00;
float                     mixer2OFF       =    0.00;
float                     mixerLvL        =    1.00;
float                     cornerFreq      =  750.00;                            // Hz
                          
float                     freq1           =  3000.0f;                           // Speaker output, low-shelf filter, Stage 1
float                     tGain1          =   -10.0f;
float                     slope1          =     0.05f;

float                     freq2           =  7700.0f;                           // Speaker output, low-shelf filter, Stage 2
float                     tGain2          =   -60.0f;
float                     slope2          =     0.05f;

float                     freq3           = 15700.0f;                           // Speaker output, low-shelf filter, Stage 3
float                     tGain3          =   -40.0f;
float                     slope3          =     0.05f;

/// Audio Shield Variables
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioPlaySdRaw           playRaw1;       //xy=77.19999694824219,360
AudioInputI2S            i2s2;           //xy=82.19999694824219,161
AudioMixer4              mixer2;         //xy=357.1999969482422,309
AudioMixer4              mixer1;         //xy=377.1999969482422,105
AudioFilterStateVariable filter2;        //xy=557.1999969482422,93
AudioFilterBiquad        biquad1;        //xy=560.2000122070312,283.20001220703125
AudioFilterStateVariable filter1;        //xy=567.1999969482422,407
AudioAnalyzePeak         peak2;          //xy=737.1999969482422,479
AudioRecordQueue         queue1;         //xy=745.1999969482422,44
AudioAnalyzePeak         peak1;          //xy=752.1999969482422,140
AudioOutputI2S           i2s1;           //xy=759.1999969482422,396
AudioRecordQueue         queue2;         //xy=768.1999969482422,263
AudioConnection          patchCord1(playRaw1, 0, mixer2, 2);
AudioConnection          patchCord2(i2s2, 0, mixer1, 0);
AudioConnection          patchCord3(i2s2, 0, mixer2, 0);
AudioConnection          patchCord4(i2s2, 1, mixer1, 1);
AudioConnection          patchCord5(i2s2, 1, mixer2, 1);
AudioConnection          patchCord6(mixer2, biquad1);
AudioConnection          patchCord7(mixer1, 0, filter2, 0);
AudioConnection          patchCord8(filter2, 0, queue1, 0);
AudioConnection          patchCord9(filter2, 0, peak1, 0);
AudioConnection          patchCord10(biquad1, 0, filter1, 0);
AudioConnection          patchCord11(filter1, 0, i2s1, 0);
AudioConnection          patchCord12(filter1, 0, i2s1, 1);
AudioConnection          patchCord13(filter1, 0, queue2, 0);
AudioConnection          patchCord14(filter1, 0, peak2, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=248.1999969482422,461
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

  biquad1.setHighShelf( 0, freq1, tGain1, slope1 );                             //stage, freq, gain, slope
  biquad1.setHighShelf( 0, freq2, tGain2, slope2 );                             //stage, freq, gain, slope
  biquad1.setHighShelf( 0, freq3, tGain3, slope3 );                             //stage, freq, gain, slope

  // Configure Filter
  filter1.frequency(cornerFreq);                                                // Set Corner Frequency to the default value of 1500.00 Hz or 1.5 kHz
  filter2.frequency(cornerFreq);                                                // Set Corner Frequency to the default value of 1500.00 Hz or 1.5 kHz
  
} // End of SetupAudioBoard()
