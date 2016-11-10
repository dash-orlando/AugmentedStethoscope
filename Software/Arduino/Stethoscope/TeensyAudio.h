#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <Audio.h>

const int        myInput    = AUDIO_INPUT_MIC;

// GUItool: begin automatically generated code
AudioInputI2S            i2s2;                                                //xy=228.1999969482422,100
AudioPlaySdRaw           playRaw1;                                            //xy=230.1999969482422,211.99998474121094
AudioMixer4              mixer1;                                              //xy=392.20001220703125,115.19999694824219
AudioMixer4              mixer2;                                              //xy=393.20001220703125,198.1999969482422
AudioOutputI2S           i2s1;                                                //xy=554.2000122070312,213.99998474121094
AudioAnalyzePeak         peak1;                                               //xy=557.2000122070312,159.99998474121094
AudioRecordQueue         queue1;                                              //xy=560.2000122070312,108
AudioConnection          patchCord1( i2s2,      0, mixer1,  0 );
AudioConnection          patchCord2( i2s2,      0, mixer2,  0 );
AudioConnection          patchCord3( i2s2,      1, mixer1,  1 );
AudioConnection          patchCord4( i2s2,      1, mixer2,  1 );
AudioConnection          patchCord5( playRaw1,  0, mixer2,  2 );
AudioConnection          patchCord6( mixer1,    peak1         );
AudioConnection          patchCord7( mixer1,    queue1        );
AudioConnection          patchCord8( mixer2,    0, i2s1,    0 );
AudioConnection          patchCord9( mixer2,    0, i2s1,    1 );
AudioControlSGTL5000     sgtl5000_1;                                          //xy=233.1999969482422,268
// GUItool: end automatically generated code

void SetupAudioBoard()
{
  // Audio connections require memory, and the record queue
  // uses this memory to buffer incoming audio.
  AudioMemory( 60 );

  // Enable the audio shield, select input, and enable output
  sgtl5000_1.enable();
  sgtl5000_1.volume( 0.25 );
  sgtl5000_1.inputSelect( myInput );
  sgtl5000_1.micGain( 50 );

 // Configure SPI for the audio shield pins
  SPI.setMOSI( 7 );  // Audio shield has MOSI on pin 7
  SPI.setSCK( 14 );  // Audio shield has SCK on pin 14

  mixer1.gain(0,0);                                                            // Set gain of mixer1, channel0 to 0
  mixer1.gain(1,0);                                                            // Set gain of mixer1, channel1 to 1
  mixer2.gain(0,0.5);                                                          // Set gain of mixer2, channel0 to 0.25 - Microphone on
  mixer2.gain(1,0.5);                                                          // Set gain of mixer2, channel0 to 0.25 - Microphone on
  mixer2.gain(2,0);                                                            // Set gain of mixer2, channel2 to 0
  
}
