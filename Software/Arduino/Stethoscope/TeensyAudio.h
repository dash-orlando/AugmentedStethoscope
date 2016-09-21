#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <Audio.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s2;           //xy=195.1999969482422,142
AudioAnalyzePeak         peak1;          //xy=368.1999969482422,187
AudioRecordQueue         queue1;         //xy=371.1999969482422,142
AudioPlaySdRaw           playRaw1;       //xy=392.1999969482422,236
AudioOutputI2S           i2s1;           //xy=560.1999969482422,199
AudioConnection          patchCord1(i2s2, 0, queue1, 0);
AudioConnection          patchCord2(i2s2, 0, peak1, 0);
AudioConnection          patchCord3(playRaw1, 0, i2s1, 0);
AudioConnection          patchCord4(playRaw1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=355.1999969482422,291
// GUItool: end automatically generated code

