#include "KorotkoffSound.h"               // WAV files converted to code by wav2sketch        
#include <MsTimer2.h>


///////////////////////////////////
// Design Tool code
///////////////////////////////////
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioPlayMemory          playMem1;        //xy=79.19999694824219,264.1999969482422
AudioInputI2S            i2s2;            //xy=82.19999694824219,194.1999969482422
AudioFilterStateVariable filter2;         //xy=299.1999969482422,244.1999969482422
AudioFilterStateVariable filter1;         //xy=339.20001220703125,149.1999969482422
AudioMixer4              mixer1;          //xy=496.20001220703125,181.1999969482422
AudioOutputI2S           i2s1;            //xy=664.2000122070312,141.1999969482422
AudioConnection          patchCord1( playMem1,  0, filter2, 0 );
AudioConnection          patchCord2( i2s2,      0, filter1, 0 );
AudioConnection          patchCord3( filter2,   0, mixer1,  1 );
AudioConnection          patchCord4( filter1,   2, mixer1,  0 );
AudioConnection          patchCord5( mixer1,    0, i2s1,    0 );
AudioConnection          patchCord6( mixer1,    0, i2s1,    1 );
AudioControlSGTL5000     sgtl5000_1;     //xy=434.20001220703125,399.20001220703125
// GUItool: end automatically generated code


float mainVolume    =    0.6;             // 2-speaker: 0.5; 1-speaker: 0.6
float sampGain      =    0.7;             // 
float micrGain      =    1.0;
float lowPassFreq   =  250.0;             // Low pass from HB sample
float highPassFreq  =  500.0;             // High pass from mic

float clipLength    =    0.2;
float heartRate     =   80.0;             // Range: 7-300 bpm

int   hrInterval    =  (60.0 / heartRate) * 1000.0;

bool  systolic      =  false;
bool  diastolic     =  false;


void setup()
{
  AudioMemory( 8 );
  sgtl5000_1.enable();
  sgtl5000_1.volume( mainVolume );
  sgtl5000_1.inputSelect( AUDIO_INPUT_MIC );
  sgtl5000_1.micGain( 36 );                 // Range: 0-63
  filter1.frequency( highPassFreq );
  filter2.frequency( lowPassFreq  );
  mixer1.gain( 0, micrGain );
  mixer1.gain( 1, sampGain );

//  Serial.begin( 9600 );
//  while ( !Serial ) {;}
//  Serial.print( "hrInterval = " ); Serial.println( hrInterval );
}


void SetRate( int hr )
{
  heartRate   = (float)hr;
  hrInterval  = (60.0 / heartRate) * 1000.0;
  systolic    = false;
  diastolic   = false;
//  Serial.print( "Heartrate set to " ); Serial.println( heartRate );
}


void BeginHB()
{
  systolic    = true;
  diastolic   = false;
  MsTimer2::set( hrInterval, PlayHB );
  MsTimer2::start();
}


void EndHB()
{
  diastolic   = true;
  MsTimer2::stop();
}


void PlayHB()
{
  if ( systolic and !diastolic )
  {
    playMem1.play( KorotkoffSound );
  }
}


void loop()
{
  for ( int i = 60; i <= 180; i += 40 )
  {
    SetRate( i );
    BeginHB();
    delay( 4000 );
    EndHB();
  }
}

