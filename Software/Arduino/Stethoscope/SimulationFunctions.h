/*
 * SimulationFunctions.h
 *
 * The following script comprises all of the functions used for the simulated operation of the device
 *
 * Michael Xynidis
 * Fluvio Lobo Fenoglietto
 * 10/27/2016
 */


#include "BPCuffResponse.h"


 //
 // *** Normal Heart Beat
 //
 void normalHBPlayback()
 {
  
  startPlaying( ses.filePly1 );                                                       // Playback of a normal heart beat
  
 } // End of normalHBPlayback() function


 //
 // *** Early Systolic Heart Mumur
 //
 void earlyHMPlayback()
 {
  
  startPlaying( ses.filePly2 );                                                       // Playback of a normal heart beat
  
 } // End of normalHBPlayback() function


 //
 // *** Augment BP Reading
 //
void startAugmentingBP()
{
  switch ( ses.heartRate )
  {
    case normalSinus :
      SetRate( ses.heartRate );
      BeginHB();
      BPAugmentState  = AUGMENT;
      recordState     = STANDBY;
      mode            = 6;
    break;
    case bradycardia :
      SetRate( ses.heartRate );
      BeginHB();
      BPAugmentState  = AUGMENT;
      recordState     = STANDBY;
      mode            = 6;
    break;
    case tachycardia :
      SetRate( ses.heartRate );
      BeginHB();
      BPAugmentState  = AUGMENT;
      recordState     = STANDBY;
      mode            = 6;
    break;
    default :
    BPAugmentState    = STANDBY;
    recordState       = PASSTHRU;
      mode            = 4;
      Serial.print( ses.heartRate );
    break;
  }
}

 //
 // *** Augment BP Reading
 //
void stopAugmentingBP()
{
  EndHB();
  BPAugmentState  = STANDBY;
  recordState     = PASSTHRU;
  mode            = 4;
}


void continueAugmentingBP()
{
}

