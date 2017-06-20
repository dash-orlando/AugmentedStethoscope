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
boolean startAugmentingBP()
{
  switch ( ses.heartRate )
  {
    case normalSinus :
      SetRate( ses.heartRate );
      BeginHB();
      BPAugmentState  = AUGMENTING;
      recordState     = STANDBY;
      switchMode( 6 );
      Serial.println( "Augmentation for BP reading STARTED." );
      BTooth.write( ACK );
      return true;
    break;
    case bradycardia :
      SetRate( ses.heartRate );
      BeginHB();
      BPAugmentState  = AUGMENTING;
      recordState     = STANDBY;
      switchMode( 6 );
      Serial.println( "Augmentation for BP reading STARTED." );
      BTooth.write( ACK );
      return true;
    break;
    case tachycardia :
      SetRate( ses.heartRate );
      BeginHB();
      BPAugmentState  = AUGMENTING;
      recordState     = STANDBY;
      switchMode( 6 );
      Serial.println( "Augmentation for BP reading STARTED." );
      BTooth.write( ACK );
      return true;
    break;
    default :
      BPAugmentState  = STANDBY;
      recordState     = PASSTHRU;
      switchMode( 4 );
      Serial.println( ses.heartRate );
      Serial.println( "BP Augmentation called with invalid parameter." );
      BTooth.write( NAK );
      return false;
    break;
  }
}

 //
 // *** Augment BP Reading
 //
boolean stopAugmentingBP()
{
  EndHB();
  BPAugmentState  = STANDBY;
  recordState     = PASSTHRU;
  switchMode( 4 );
  Serial.println( "Augmentation for BP reading STOPPED." );
  BTooth.write( ACK );
  return true;
}


void continueAugmentingBP()
{
}

