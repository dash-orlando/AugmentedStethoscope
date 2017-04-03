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

 
 // States
 
 enum State
{
  CONNECTED, DISCONNECTED, RECORDING, PLAYING, PASSTHRU, DETECTING, AUGMENT, STANDBY, READY, NOTREADY
};

State     connectState    = DISCONNECTED;
State     recordState     = STANDBY;
State     passthruState   = PASSTHRU;
State     HBDetectState   = STANDBY;
State     BPAugmentState  = STANDBY;
State     readyState;

String stateToText( int state )             // for Serial monitor diagnostics
{
  String value = "";
  switch ( state )
  {
    case CONNECTED :
      value = "CONNECTED";
      break;
    case DISCONNECTED :
      value = "DISCONNECTED";
      break;
    case RECORDING :
      value = "RECORDING";
      break;
    case PLAYING :
      value = "PLAYING";
      break;
    case PASSTHRU :
      value = "PASSTHRU";
      break;
    case DETECTING :
      value = "DETECTING";
      break;
    case AUGMENT :
      value = "AUGMENT";
      break;
    case STANDBY :
      value = "STANDBY";
      break;
    case READY :
      value = "READY";
      break;
    case NOTREADY :
      value = "NOTREADY";
      break;
    default:
      value = "unknown state";
      break;
  }
  return value;
}

/*	Modes
 * 
 *
 *	(int)	mode	Value	Description
 *					= 0		Idle
 *					= 1		Continue Recording
 *					= 2		Continue Playing
 *          = 3   Continue Tracking Mic Stream
 *          = 4   Continue Audio Pass-through
 *          = 5   Continue Blending
 *          = 6   Continue BP Augmentation
 *
 */

int			mode	= 4;

