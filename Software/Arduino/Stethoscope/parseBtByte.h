/*
 * parseBtByte.h
 * 
 * The following script has been designed to control the byte-parsing of the main loop
 * 
 * Michael Xynidis
 * Fluvio L Lobo Fenoglietto
 * 01/31/2017
 */

//
// *** Imports and Includes
//

#include  "DiagnosticFunctions.h"
#include  "OperationalFunctions.h"
#include  "DeviceSpecificFunctions.h"
#include  "SimulationFunctions.h"

//
// *** displayByte
//     This function translates and displays byte information on a connected serial monitor
//     Serial monitors may be enabled for diagnostics
//

void displayByte( byte byteValue )
{
  Serial.print( "received: [" );
  Serial.print( (char)byteValue );
  Serial.print( "][" );
  Serial.print( byteValue, HEX );
  Serial.println( ']' );
}

//
// *** parseBtByte
//     This function parses incoming bytes (or byte sequences) and calls/executes functions associated with such bytes
//

void parseBtByte( String fn )
{
    byte  inByte = 0x00;

    char  filename[fn.length()+1];
    fn.toCharArray( filename, sizeof( filename ) );

    Serial.print( "\nBTooth inbound queue, no. of bytes available: " ); 
    Serial.println( BTooth.available() );

    inByte = BTooth.read();                     // get incoming byte
    displayByte( inByte );
    Serial.print( "(pre) readyState: ");    Serial.println( stateToText( readyState   ) );
    Serial.print( "(pre) connectState: ");  Serial.println( stateToText( connectState ) );
    Serial.print( "(pre) recordState: ");   Serial.println( stateToText( recordState  ) );
    switch ( inByte )
    {
      case ENQ :
        Serial.println( "received: ENQ..." );
        if ( readyState == READY )
        {
          connectState  = CONNECTED;                                                                  // The ENQ command has to always be sent first
          Serial.println( "sending: ACK..." );
          BTooth.write( ACK );
        }
        else
        {
          Serial.println( "sending: NAK..." ); 
          BTooth.write( NAK );
        }
      break;
      case ACK :
      case NAK :
      break;
      
	  // Diagnostic Functions
      case DEVICEID :
        // DC1_DEVICEID : Device Identification
		    deviceID(STE);
      break;
      case SDCHECK :
        // DC1_SDCHECK
		    sdCheck();
	    break;
      case SENDWAV :
        // SENDWAV : Send .WAV File
        sendWav(fn);
      break;
      case DELVOLATILE :
        //  DELVOLATILE : Delete Volatile Directory
      break;
	    case STARTREC :
        // STARTREC : Start Recording
        startRecording();
      break;
	    case STOPREC :
  	    // STOPREC : Stop Recording
  	    stopRecording();
  	  break;
  	  case STARTPLAY :
  	    // STARTPLAY : Start Playing
  	    startPlaying(ses.filePly1);
  	  break;
  	  case STOPPLAY :
  	    // STOPPLAY : Stop Playing
  	    stopPlaying();
  	  break;
  	  case STARTPASSTHRU :
  	    // STARTPASSTHRU : Start Audio Passthrough from Mic
  	    startAudioPassThru();
  	  break;
  	  case STARTTRACKING :
  	    // STARTTRACKING : Start Tracking Microphone Stream for Peaks
  	    startTrackingMicStream();
  	  break;
  	  case STOPTRACKING :
  	    // STOPTRACKING : Stop Tracking Microphone Stream for Peaks
  	    stopTrackingMicStream();
  	  break;
      case NORMALHB :
        // NORMALHB : Playback of Normal Heart Beat
        normalHBPlayback();
      break;
      case ESHMURMUR :
        // ESHMURMUR : Playback of Early Systolic Heart Murmur
        earlyHMPlayback();
      break;
      
      default :
        Serial.print( (char)inByte );
      break;
    }
    Serial.print( "(post) readyState: ");    Serial.println( stateToText( readyState   ) );
    Serial.print( "(post) connectState: ");  Serial.println( stateToText( connectState ) );
    Serial.print( "(post) recordState: ");   Serial.println( stateToText( recordState  ) );
    delay( 10 );
}
