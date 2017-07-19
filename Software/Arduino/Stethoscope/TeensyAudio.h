/*
 * Teensy Audio
 * This script can be best described as the configuration program for the Teensy audio shield
 * 
 * Michael Xynidis
 * Fluvio L Lobo Fenoglietto
 * 11/17/2016
 */


#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s_mic;        //xy=165,290
AudioPlaySdRaw           playRaw_sdHeartSound; //xy=214,516
AudioPlayMemory          playMem_heartSoundSamp; //xy=226,615
AudioMixer4              rms_mic_mixer;  //xy=505,238
AudioMixer4              rms_playRaw_mixer; //xy=507,333
AudioAnalyzePeak         mic_peaks;      //xy=681,116
AudioAnalyzePeak         playRaw_peaks;  //xy=696,438
AudioAnalyzeRMS          mic_rms;        //xy=710,174
AudioAnalyzeRMS          playRaw_rms;    //xy=720,383
AudioMixer4              mixer_mic_Sd;   //xy=773,285
AudioFilterStateVariable filter_LowPass_Rec; //xy=792,621
AudioFilterStateVariable filter_HighPass_Amb; //xy=796,522
AudioFilterBiquad        biquad_micSpk_EQ; //xy=971,286
AudioMixer4              mixer_allToSpk; //xy=1052,542
AudioRecordQueue         queue_recMic;   //xy=1237,272
AudioOutputI2S           i2s_speaker;    //xy=1286,567
AudioAnalyzePeak         peak_QrsMeter;  //xy=1293,485
AudioConnection          patchCord1(i2s_mic, 0, filter_HighPass_Amb, 0);
AudioConnection          patchCord2(i2s_mic, 0, rms_mic_mixer, 0);
AudioConnection          patchCord3(i2s_mic, 1, filter_HighPass_Amb, 1);
AudioConnection          patchCord4(i2s_mic, 1, rms_mic_mixer, 1);
AudioConnection          patchCord5(playRaw_sdHeartSound, 0, rms_playRaw_mixer, 0);
AudioConnection          patchCord6(playMem_heartSoundSamp, 0, filter_LowPass_Rec, 0);
AudioConnection          patchCord7(rms_mic_mixer, 0, mixer_mic_Sd, 0);
AudioConnection          patchCord8(rms_mic_mixer, mic_peaks);
AudioConnection          patchCord9(rms_mic_mixer, mic_rms);
AudioConnection          patchCord10(rms_playRaw_mixer, 0, mixer_mic_Sd, 1);
AudioConnection          patchCord11(rms_playRaw_mixer, playRaw_rms);
AudioConnection          patchCord12(rms_playRaw_mixer, playRaw_peaks);
AudioConnection          patchCord13(mixer_mic_Sd, biquad_micSpk_EQ);
AudioConnection          patchCord14(filter_LowPass_Rec, 0, mixer_allToSpk, 2);
AudioConnection          patchCord15(filter_HighPass_Amb, 0, mixer_allToSpk, 1);
AudioConnection          patchCord16(biquad_micSpk_EQ, queue_recMic);
AudioConnection          patchCord17(biquad_micSpk_EQ, 0, mixer_allToSpk, 0);
AudioConnection          patchCord18(mixer_allToSpk, peak_QrsMeter);
AudioConnection          patchCord19(mixer_allToSpk, 0, i2s_speaker, 0);
AudioConnection          patchCord20(mixer_allToSpk, 0, i2s_speaker, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=174,188
// GUItool: end automatically generated code



/// Variable Definitions
const int                 selectedInput   = AUDIO_INPUT_MIC;
int                       microphoneGain  =     28;
float                     micInputLvL     =     0.50;
float                     sampleInputLvL  =     0.50;
float                     speakerVolume   =     0.65;                           // 2-speaker: 0.50; 1-speaker: 0.60

float                     mixerInputON    =     1.00;
float                     mixerInputOFF   =     0.00;
float                     mixerLvL        =     1.00;

float                     bpLowPassFreq   =   250.0;                            // Low pass from HB sample
float                     bpHighPassFreq  =   500.0;                            // High pass from mic

float                     freqHighShelf   =  2000.0f;
float                     tGain1          =   -20.0f;                           // ...may need to fiddle with this one
float                     slope1          =    1.0f;

float                     freqLowPass     = 2000.0f;                            // prjc example uses 800.0
float                     Q2              =    0.707f;                          // 0.707 = Butterworth Filter value;  min: 0.1;



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

  rms_mic_mixer.gain(   0, mixerInputON  );
  rms_mic_mixer.gain(   1, mixerInputON  );
  
  rms_playRaw_mixer.gain(   0, mixerInputON  );

  mixer_mic_Sd.gain(    0, mixerInputON  );                                     // Set gain of mixer_mic_Sd, channel0 to 0.25 - Microphone on
  mixer_mic_Sd.gain(    1, mixerInputON  );                                     // Set gain of mixer_mic_Sd, channel1 to 0.25 - Microphone on
  //mixer_mic_Sd.gain(    2, mixerInputOFF );                                     // Set gain of mixer_mic_Sd, channel2 to 0

  mixer_allToSpk.gain(  0, mixerInputON  );                                     // Normal stethoscope mic input (on)
  mixer_allToSpk.gain(  1, mixerInputOFF );                                     // Highpass mic input (off)
  mixer_allToSpk.gain(  2, mixerInputOFF );                                     // HB-sample playback (off)

  // Configure filters for BP Augmentation
  filter_HighPass_Amb.frequency( bpHighPassFreq );
  filter_LowPass_Rec.frequency(  bpLowPassFreq  );


  // Configure Biquad filter to EQ between mic and speaker
  biquad_micSpk_EQ.setLowpass(   0, freqLowPass, Q2             );              //stage, freq, Q; Butterworth filter, 12 db/octave
  biquad_micSpk_EQ.setHighShelf( 1, freqHighShelf, tGain1, slope1 );            //stage, freq, gain, slope

  // Butterworth filter, 12 db/octave
  biquad_micSpk_EQ.setLowpass( 0, freqLowPass, 0.707) ;

  // Linkwitz-Riley filter, 48 dB/octave
  //biquad_micSpk_EQ.setLowpass( 0, freqLowPass, 0.54 );
  //biquad_micSpk_EQ.setLowpass( 1, freqLowPass, 1.3  );
  //biquad_micSpk_EQ.setLowpass( 2, freqLowPass, 0.54 );
  //biquad_micSpk_EQ.setLowpass( 3, freqLowPass, 1.3  );
  
} // End of SetupAudioBoard()
