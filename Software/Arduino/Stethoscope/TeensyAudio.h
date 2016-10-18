#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <Audio.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s2;           //xy=228.1999969482422,100
AudioPlaySdRaw           playRaw1;       //xy=379.20001220703125,304
AudioMixer4              mixer1;         //xy=392.20001220703125,115.19999694824219
AudioOutputI2S           i2s1;           //xy=550.2000122070312,240.99998474121094
AudioAnalyzePeak         peak1;          //xy=557.2000122070312,159.99998474121094
AudioRecordQueue         queue1;         //xy=560.2000122070312,108
AudioConnection          patchCord1(i2s2, 0, mixer1, 0);
AudioConnection          patchCord2(i2s2, 1, mixer1, 1);
AudioConnection          patchCord3(playRaw1, 0, i2s1, 0);
AudioConnection          patchCord4(playRaw1, 0, i2s1, 1);
AudioConnection          patchCord5(mixer1, peak1);
AudioConnection          patchCord6(mixer1, queue1);
AudioControlSGTL5000     sgtl5000_1;     //xy=361.20001220703125,376
// GUItool: end automatically generated code

const int                myInput         = AUDIO_INPUT_MIC;

void SetupAudioBoard()
{
  // Audio connections require memory, and the record queue
  // uses this memory to buffer incoming audio.
  AudioMemory( 60 );

  // Enable the audio shield, select input, and enable output
  sgtl5000_1.enable();
  sgtl5000_1.inputSelect( myInput );
  sgtl5000_1.volume( 1.0 );

 // Configure SPI for the audio shield pins
  SPI.setMOSI( 7 );  // Audio shield has MOSI on pin 7
  SPI.setSCK( 14 );  // Audio shield has SCK on pin 14
}
