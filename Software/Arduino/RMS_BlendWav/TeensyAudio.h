#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioPlaySdWav           playSdWav_sdHeartSound_B;     //xy=197.1999969482422,386.20001220703125
AudioPlayMemory          playMem_heartSoundSamp;       //xy=204.1999969482422,484.20001220703125
AudioPlaySdRaw           playRaw_sdHeartSound_B;     //xy=206.1999969482422,436.20001220703125
AudioPlaySdWav           playSdWav_sdHeartSound_A;     //xy=214.1999969482422,73.19999694824219
AudioPlaySdRaw           playRaw_sdHeartSound_A; //xy=223.1999969482422,125.99998474121094
AudioMixer4              mixer_A;         //xy=469.1999969482422,74.19999694824219
AudioMixer4              mixer_B;         //xy=475.20001220703125,447.20001220703125
AudioMixer4              mixer_SD;   //xy=505.20001220703125,245.99998474121094
AudioAnalyzeRMS          rms_B;           //xy=633.2000122070312,447.20001220703125
AudioAnalyzePeak         peak_B;          //xy=633.1999969482422,492.1999969482422
AudioAnalyzePeak         peak_A;          //xy=635.2000122070312,30.199996948242188
AudioAnalyzeRMS          rms_A;           //xy=635.2000122070312,74.19999694824219
AudioOutputI2S           i2s_speaker;    //xy=714.2000122070312,284
AudioAnalyzePeak         peak_QrsMeter;  //xy=722.2000122070312,208.99998474121094
AudioConnection          patchCord1(playSdWav_sdHeartSound_B, 0, mixer_B, 0);
AudioConnection          patchCord2(playSdWav_sdHeartSound_B, 1, mixer_B, 1);
AudioConnection          patchCord3(playMem_heartSoundSamp, 0, mixer_B, 3);
AudioConnection          patchCord4(playRaw_sdHeartSound_B, 0, mixer_B, 2);
AudioConnection          patchCord5(playSdWav_sdHeartSound_A, 0, mixer_A, 0);
AudioConnection          patchCord6(playSdWav_sdHeartSound_A, 1, mixer_A, 1);
AudioConnection          patchCord7(playRaw_sdHeartSound_A, 0, mixer_A, 2);
AudioConnection          patchCord8(mixer_A, rms_A);
AudioConnection          patchCord9(mixer_A, 0, mixer_SD, 0);
AudioConnection          patchCord10(mixer_A, peak_A);
AudioConnection          patchCord11(mixer_B, rms_B);
AudioConnection          patchCord12(mixer_B, 0, mixer_SD, 1);
AudioConnection          patchCord13(mixer_B, peak_B);
AudioConnection          patchCord14(mixer_SD, peak_QrsMeter);
AudioConnection          patchCord15(mixer_SD, 0, i2s_speaker, 0);
AudioConnection          patchCord16(mixer_SD, 0, i2s_speaker, 1);
AudioControlSGTL5000     audioShield;     //xy=475.20001220703125,552
// GUItool: end automatically generated code

// const int  myInput   = AUDIO_INPUT_LINEIN;
// const int  myInput   = AUDIO_INPUT_MIC;

#define       inpA      0
#define       inpB      1

#define       inpWavL   0
#define       inpWavR   1
#define       inpRaw    2
#define       inpMem    3

 enum Source
{
  WAVE, RAW, MEMORY
};

Source        inputSourceA = WAVE;
Source        inputSourceB = MEMORY;

