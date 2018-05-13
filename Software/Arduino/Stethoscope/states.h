/*
 *	states.h
 * 
 *	The following script contains all of the operation states and modes that pertain to our smart devices
 *	Note that not all fo the modes will be used by all devices
 *
 *	Michael Xynidis
 *	Fluvio L Lobo Fenoglietto
 *	10/18/2016
 *
 */

 
// ==============================================================================================================
// Device States
//
// The following structure defines all the possible states of the augmented stethoscope
//
// Michael Xynidis
// Fluvio L Lobo Fenoglietto 05/07/2018
// ============================================================================================================== //

enum State
{
  READY,                                                                                                          // READY to execute functions ( = idle )
  NOTREADY,                                                                                                       // NOT READY to execute functions ( = error state )
  RECORDING,                                                                                                      // RECORDING single/multiple audio streams ( this may be broken down later )
  PLAYING,                                                                                                        // PLAYING audio file
  MONITORING,                                                                                                     // MONITORING heart beat ( may be paired with recording )
  BLENDING,                                                                                                       // BLENDING microphone input with audio file from SD card
  CONTINUING,
};

State     deviceState     = READY;                                                                                // Single state variable...
State     recState        = READY;
State     blendState      = READY;

// ==============================================================================================================
// State to Text
//
// Function used to print the current state of the augmented stethoscope (or device)
//
// Michael Xynidis
// Fluvio L Lobo Fenoglietto 05/07/2018
// ============================================================================================================== //

String stateToText( int state )
{
  String value = "";
  switch ( state )
  {
    case READY :
      value = "READY";
      break;
    case NOTREADY :
      value = "NOTREADY";
      break;
    case RECORDING :
      value = "RECORDING";
      break;
    case PLAYING :
      value = "PLAYING";
      break;
    case MONITORING   :
      value = "MONITORING";
      break;
    case BLENDING :
      value = "BLENDING";
      break;
    default:
      value = "unknown state";
      break;
  }
  return value;
} // End of stateToText()


// ==============================================================================================================
// Mode
//
// Modes are paired with states for function triggering on the main loop of the stethoscope script
//
// Michael Xynidis
// Fluvio L Lobo Fenoglietto 05/07/2018
// ============================================================================================================== //

/*	Modes
 * 
 *
 *	(int)	mode	Value	Description
 *					= 0		Ready/Idle --Do nothing
 *					= 1		Continue Recording
 *					= 2		Continue Playing
 *          = 3   Continue Tracking Mic Stream
 *          = 4   Continue Audio Pass-through
 *          = 5   Continue Blending
 *
 */
int			mode	= 0;


