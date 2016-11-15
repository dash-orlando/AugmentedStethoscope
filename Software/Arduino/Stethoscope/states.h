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
  CONNECTED, DISCONNECTED, RECORDING, PLAYING, STREAMING, STANDBY, READY, NOTREADY
};

State     connectState  = DISCONNECTED;
State     recordState   = STANDBY;
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
    case STREAMING :
      value = "STREAMING";
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
 *
 *
 */

int			mode	= 0;

