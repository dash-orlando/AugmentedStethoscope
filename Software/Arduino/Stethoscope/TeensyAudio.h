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
<<<<<<< HEAD
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
=======
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



/* GUItool: begin automatically generated code
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
>>>>>>> 8171699695e084e3472c9e4fa6b7f3dcf8c8b535
AudioConnection          patchCord1(playRaw1, 0, mixer2, 2);
AudioConnection          patchCord2(i2s2, 0, mixer1, 0);
AudioConnection          patchCord3(i2s2, 0, mixer2, 0);
AudioConnection          patchCord4(i2s2, 1, mixer1, 1);
AudioConnection          patchCord5(i2s2, 1, mixer2, 1);
AudioConnection          patchCord6(mixer2, biquad1);
AudioConnection          patchCord7(mixer1, 0, filter2, 0);
AudioConnection          patchCord8(filter2, 0, queue1, 0);
AudioConnection          patchCord9(filter2, 0, peak1, 0);
<<<<<<< HEAD
AudioConnection          patchCord10(biquad1, 0, filter1, 0);
AudioConnection          patchCord11(filter1, 0, i2s1, 0);
AudioConnection          patchCord12(filter1, 0, i2s1, 1);
AudioConnection          patchCord13(filter1, 0, queue2, 0);
AudioConnection          patchCord14(filter1, 0, peak2, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=248.1999969482422,461
// GUItool: end automatically generated code


=======
AudioConnection          patchCord10(filter1, 0, i2s1, 0);
AudioConnection          patchCord11(filter1, 0, i2s1, 1);
AudioConnection          patchCord12(filter1, 0, queue2, 0);
AudioConnection          patchCord13(filter1, 0, peak2, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=752.5999755859375,687
// GUItool: end automatically generated code  */


// GUItool: begin automatically generated code
AudioPlaySdRaw            playRaw1;                                             //xy=155.1999969482422,214.99998474121094
AudioPlayMemory           playMem1;                                             //xy=159.1999969482422,306.20001220703125
AudioInputI2S             i2s2;                                                 //xy=165.1999969482422,141
AudioFilterStateVariable  filter4;                                              //xy=442.20001220703125,372.20001220703125
AudioFilterStateVariable  filter3;                                              //xy=443.1999969482422,311.1999969482422
AudioMixer4               mixer1;                                               //xy=447.1999969482422,120
AudioMixer4               mixer2;                                               //xy=448.20001220703125,225.99998474121094
AudioFilterStateVariable  filter2;                                              //xy=627.1999969482422,108
AudioFilterStateVariable  filter1;                                              //xy=628.2000122070312,210.99998474121094
AudioMixer4               mixer3;                                               //xy=631.2000122070312,323.20001220703125
AudioAnalyzePeak          peak1;                                                //xy=830.2000122070312,104
AudioAnalyzePeak          peak2;                                                //xy=831.2000122070312,220.99998474121094
AudioRecordQueue          queue2;                                               //xy=832.2000122070312,167.99998474121094
AudioRecordQueue          queue1;                                               //xy=833.2000122070312,47
AudioOutputI2S            i2s1;                                                 //xy=833.2000122070312,289
AudioConnection           patchCord1(  playRaw1, 0, mixer2,  2 );
AudioConnection           patchCord2(  playMem1, 0, filter4, 0 );
AudioConnection           patchCord3(  i2s2,     0, mixer1,  0 );
AudioConnection           patchCord4(  i2s2,     0, mixer2,  0 );
AudioConnection           patchCord5(  i2s2,     0, filter3, 0 );
AudioConnection           patchCord6(  i2s2,     1, mixer1,  1 );
AudioConnection           patchCord7(  i2s2,     1, mixer2,  1 );
AudioConnection           patchCord8(  i2s2,     1, filter3, 1 );
AudioConnection           patchCord9(  filter4,  0, mixer3,  2 );
AudioConnection           patchCord10( filter3,  2, mixer3,  1 );
AudioConnection           patchCord11( mixer1,   0, filter2, 0 );
AudioConnection           patchCord12( mixer2,   0, filter1, 0 );
AudioConnection           patchCord13( filter2,  0, queue1,  0 );
AudioConnection           patchCord14( filter2,  0, peak1,   0 );
AudioConnection           patchCord15( filter1,  0, queue2,  0 );
AudioConnection           patchCord16( filter1,  0, peak2,   0 );
AudioConnection           patchCord17( filter1,  0, mixer3,  0 );
AudioConnection           patchCord18( mixer3,   0, i2s1,    0 );
AudioConnection           patchCord19( mixer3,   0, i2s1,    1 );
AudioControlSGTL5000      sgtl5000_1;                                           //xy=158.1999969482422,65.99998474121094
// GUItool: end automatically generated code



>>>>>>> 8171699695e084e3472c9e4fa6b7f3dcf8c8b535
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

  mixer1.gain( 0, mixerInputOFF );                                              // Set gain of mixer1, channel0 to 0
  mixer1.gain( 1, mixerInputOFF );                                              // Set gain of mixer1, channel1 to 1
  mixer2.gain( 0, mixerInputON  );                                              // Set gain of mixer2, channel0 to 0.25 - Microphone on
  mixer2.gain( 1, mixerInputON  );                                              // Set gain of mixer2, channel0 to 0.25 - Microphone on
  mixer2.gain( 2, mixerInputOFF );                                              // Set gain of mixer2, channel2 to 0
  mixer3.gain( 0, mixerInputON  );                                              // Normal stethoscope mic input (on)
  mixer3.gain( 1, mixerInputOFF );                                              // Highpass mic input (off)
  mixer3.gain( 2, mixerInputOFF );                                              // HB-sample playback (off)

  // Configure filters for heartsound listening
  filter1.frequency( cornerFreq );                                              // Set Corner Frequency to the default value of 1500.00 Hz or 1.5 kHz
  filter2.frequency( cornerFreq );                                              // Set Corner Frequency to the default value of 1500.00 Hz or 1.5 kHz

<<<<<<< HEAD
  biquad1.setHighShelf( 0, freq1, tGain1, slope1 );                             //stage, freq, gain, slope
  biquad1.setHighShelf( 0, freq2, tGain2, slope2 );                             //stage, freq, gain, slope
  biquad1.setHighShelf( 0, freq3, tGain3, slope3 );                             //stage, freq, gain, slope

  // Configure Filter
  filter1.frequency(cornerFreq);                                                // Set Corner Frequency to the default value of 1500.00 Hz or 1.5 kHz
  filter2.frequency(cornerFreq);                                                // Set Corner Frequency to the default value of 1500.00 Hz or 1.5 kHz
=======
  // Configure filters for BP Augmentation
  filter3.frequency( bpHighPassFreq );
  filter4.frequency( bpLowPassFreq  );
>>>>>>> 8171699695e084e3472c9e4fa6b7f3dcf8c8b535
  
} // End of SetupAudioBoard()
