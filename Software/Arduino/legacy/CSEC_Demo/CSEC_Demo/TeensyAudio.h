#include  <Audio.h>

// GUItool: begin automatically generated code
//*
AudioPlaySdWav           playSdWav1;     //xy=376.2,204.2   // ADDED b
AudioPlaySdRaw           playSdRaw1;     //xy=376.2,204.2   // ADDED b
AudioAnalyzePeak         peak2;          //xy=655.2,399.2   // ADDED b
AudioAnalyzePeak         peak3;          //xy=725.2,288.2   // ADDED b
AudioAnalyzePeak         peak4;
AudioAnalyzePeak         peak5;

AudioInputI2S            i2s2;           //xy=105,63
AudioAnalyzePeak         peak1;          //xy=278,108
AudioRecordQueue         queue1;         //xy=281,63
AudioPlaySdRaw           playRaw1;       //xy=302,157
AudioPlaySdWav           playWav1;                          // ADDED a
AudioOutputI2S           i2s1;           //xy=470,120
AudioConnection          patchCord5(playWav1, 0, i2s1, 0);  // ADDED a
AudioConnection          patchCord6(playWav1, 1, i2s1, 1);  // ADDED a
AudioConnection          patchCord1(i2s2, 0, queue1, 0);
AudioConnection          patchCord2(i2s2, 0, peak1, 0);
AudioConnection          patchCord3(playRaw1, 0, i2s1, 0);
AudioConnection          patchCord4(playRaw1, 0, i2s1, 1);
AudioConnection          patchCord7(playSdWav1, 0, i2s1, 0);   // ADDED b
AudioConnection          patchCord8(playSdWav1, 0, peak3, 0);  // ADDED b
AudioConnection          patchCord9(playSdWav1, 1, i2s1, 1);   // ADDED b
AudioConnection          patchCord10(playSdWav1, 1, peak2, 0); // ADDED b

AudioConnection          patchCord11(playRaw1, 0, i2s1, 0);   // ADDED b
AudioConnection          patchCord12(playRaw1, 0, peak4, 0);  // ADDED b
AudioConnection          patchCord13(playRaw1, 1, i2s1, 1);   // ADDED b
AudioConnection          patchCord14(playRaw1, 1, peak5, 0);  // ADDED b


AudioControlSGTL5000     sgtl5000_1;     //xy=265,212
// GUItool: end automatically generated code
