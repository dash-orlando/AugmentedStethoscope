// Advanced Microcontroller-based Audio Workshop
//
// https://github.com/PaulStoffregen/AudioWorkshop2015/raw/master/workshop.pdf
// https://hackaday.io/project/8292-microcontroller-audio-workshop-had-supercon-2015
// 
// Part 2-4: Using The Microphone

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=297,224
AudioMixer4              mixer2;         //xy=639,311
AudioFilterStateVariable filter1;        //xy=866,376
AudioOutputI2S           i2s2;           //xy=1104,140
AudioConnection          patchCord1(i2s1, 0, mixer2, 0);
AudioConnection          patchCord2(i2s1, 1, mixer2, 1);
AudioConnection          patchCord3(mixer2, 0, filter1, 0);
AudioConnection          patchCord4(filter1, 0, i2s2, 0);
AudioConnection          patchCord5(filter1, 0, i2s2, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=322,468
// GUItool: end automatically generated code


void setup() {
  Serial.begin(115200);
  AudioMemory(60);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  sgtl5000_1.inputSelect(AUDIO_INPUT_MIC);
  sgtl5000_1.micGain(50);
  mixer2.gain(0,1);
  mixer2.gain(1,1);
  filter1.frequency(1500);
  // delay(1000);
}

void loop() {
  // do nothing
}




