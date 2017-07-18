#include "KorotkoffSound.h"               // WAV files converted to code by wav2sketch        
#include <MsTimer2.h>


float clipLength    =    0.2;
float heartRateF    =   80.0;             // Range: 7-300 bpm; default is 80

int         hrInterval    =  (60.0 / heartRateF) * 1000.0;
const int   bradycardia   =   55;
const int   tachycardia   =   60;
const int   normalSinus   =   70;


void SetRate( int hr )
{
  heartRateF  = (float)hr;
  hrInterval  = (60.0 / heartRateF) * 1000.0;
  Serial.println(hrInterval);
//  Serial.print( "Heartrate set to " ); Serial.println( heartRateF );
}


void PlayHB()
{
  playMem_heartSoundSamp.play( KorotkoffSound );
}


void BeginHB()
{
  mixer_allToSpk.gain( 0, 0.10  );                                            // Normal stethoscope mic input (off)
  mixer_allToSpk.gain( 1, micInputLvL    );                                            // Highpass mic input  (on)
  mixer_allToSpk.gain( 2, 0.35 );                                                       // HB-sample playback  (on)

  MsTimer2::set( hrInterval, PlayHB );
  MsTimer2::start();
}


void EndHB()
{
  MsTimer2::stop();
  mixer_allToSpk.gain( 0, mixerInputON  );                                              // Normal stethoscope mic input (on)
  mixer_allToSpk.gain( 1, mixerInputOFF );                                              // Highpass mic input (off)
  mixer_allToSpk.gain( 2, mixerInputOFF );                                              // HB-sample playback (off)
}

