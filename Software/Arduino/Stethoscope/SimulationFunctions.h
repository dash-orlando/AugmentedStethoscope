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
 // *** Blend Synthetic, Normal Heart Beat
 //
 void NHBSBlend()
 {
  
  startBlending( ses.filePly1 );                                                      // Blending of a Synthetic, Normal Heart Beat
  
 } // End of NHBSBlend() function


 //
 // *** Blend Synthetic, Early Systolic Heart Mumur
 //
 void ESMSBlend()
 {
  
  startBlending( ses.filePly2 );                                                      // Blending of a Synthetic, Early Systolic Heart Murmur
  
 } // End of ESMSBlend() function

 //
 // *** Blend Recorded, Normal Heart Beat
 //
 void NHBRBlend()
 {
  
  startBlending( ses.filePly3 );                                                      // Blending of a Recorded, Normal Heart Beat
  
 } // End of NHBRBlend() function

  //
 // *** Blend Recorded, Exercised Heart Beat
 //
 void EHBRBlend()
 {
  
  startBlending( ses.filePly4 );                                                      // Blending of a Recorded, Exercised Heart Beat
  
 } // End of EHBRBlend() function

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
  //switchMode( 6 );
  Serial.println( "Augmentation for BP reading STOPPED." );
  BTooth.write( ACK );
  return true;
}


void continueAugmentingBP()
{
  if ( BPAugmentState  == AUGMENTING )
  {
    if ( mixerLvL > 0.10 )
    {
    mixerLvL = mixerLvL - 0.00005;
    mixer_allToSpk.gain( 0, mixerLvL );
    mixer_allToSpk.gain( 1, mixerLvL );
    mixer_allToSpk.gain( 2, (0.25 - mixerLvL) );
    //Serial.println( mixerLvL );
    }
  }
  else if ( BPAugmentState == STANDBY )
  {
    if ( mixerLvL < 1 )
    {
    mixerLvL = mixerLvL + 0.00005;
    mixer_allToSpk.gain( 0, mixerLvL );
    mixer_allToSpk.gain( 1, mixerLvL );
    mixer_allToSpk.gain( 2, (0.25 - mixerLvL) );
    Serial.println( mixerLvL );
    }
    else if ( mixerLvL == 1)
    {
      switchMode( 4 );
    }
  }

}

