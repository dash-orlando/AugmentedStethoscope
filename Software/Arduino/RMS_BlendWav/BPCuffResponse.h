#include "KorotkoffSound.h"               // WAV files converted to code by wav2sketch        
#include <MsTimer2.h>


float clipLength          =    0.2;
float heartRateF          =   80.0;       // Range: 7-300 bpm; default is 80

int         hrInterval    =  (60.0 / heartRateF) * 1000.0;
const int   bradycardia   =   60;
const int   tachycardia   =  120;
const int   normalSinus   =   80;


void SetRate( int hr )
{
  heartRateF  = (float)hr;
  hrInterval  = (60.0 / heartRateF) * 1000.0;
  Serial.print( "Heartrate set to " ); Serial.println( heartRateF );
}


void PlayHB()
{
  playMem_heartSoundSamp.play( KorotkoffSound );
}


bool BeginHB()
{
  MsTimer2::set( hrInterval, PlayHB );
  MsTimer2::start();
  return true;
}


bool EndHB()
{
  MsTimer2::stop();
  return false;
}

