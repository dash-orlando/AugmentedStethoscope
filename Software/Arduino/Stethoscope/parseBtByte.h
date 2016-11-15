/*
 * parseBtByte.h
 * 
 * The following script has been designed to control the byte-parsing of the main loop
 * 
 * Michael Xynidis
 * Fluvio L Lobo Fenoglietto
 * 10/27/2016
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
      case EOT :
      case ACK :
      case NAK :
      case CAN :
      break;
      
	  // Diagnostic Functions
      case DC1 :
        if ( BTooth.available() > 0 )
        {
          inByte = BTooth.read();                     										                            // get incoming byte
          displayByte( inByte );                      										                            // display on serial monitor
          switch ( inByte )
          {
            case DC1_DEVICEID :
              // DC1_DEVICEID : Device Identification
              deviceID(STE);
            break;
            case DC1_SDCHECK :
              // DC1_SDCHECK
				      sdCheck();
            break;

            // ...

            default :
              Serial.println( "DC1: 2nd byte not recieved" );
          }
        }
      break;
      
      // Operational Functions
      case DC2 :
        if ( BTooth.available() > 0 )
        {
          inByte = BTooth.read();                     										                              // get incoming byte
          displayByte( inByte );                      										                              // display on serial monitor
          switch ( inByte )
          {
            case 0x00 :
              // DC2_SENDWAV Send .WAV File
              sendWav(fn);
            break;
            case 0x01 :
              //  DC2_01();
            break;

            // ...

            default :
              Serial.println( "DC2: 2nd byte not recieved" );
          }
        }
      break;

      // Device-Specific Functions
      case DC3 :
        if ( BTooth.available() > 0 )
        {
          inByte = BTooth.read();                     										                                // get incoming byte
          displayByte( inByte );                      										                                // display on serial monitor
          switch ( inByte )
          {
            case DC3_STARTREC :
              // DC3_STARTREC : Start Recording
              startRecording();
            break;
            case DC3_STOPREC :
              // DC3_STOPREC : Stop Recording
              stopRecording();
            break;
            case DC3_STARTPLAY :
              // DC3_STARTPLAY : Start Playing
              startPlaying(ses.filePly1);
            break;
            case DC3_STOPPLAY :
              // DC3_STOPPLAY : Stop Playing
              stopPlaying();
            break;
            case DC3_STARTSTREAM :
              // DC3_STARTSTREAM : Start Microphone Stream
              startMicStream();
            case DC3_TRACKSTREAM :
              // DC3_TRACKSTREAM : Start Tracking Microphone Stream for Peaks
              trackingMicStream();
            // ...

            default :
              Serial.println( "DC3: 2nd byte not recieved" );
          }
        }
      break;

      // Simulation Functions
      case DC4 :
        if ( BTooth.available() > 0 )
        {
          inByte = BTooth.read();                     										                                // get incoming byte
          displayByte( inByte );                      										                                // display on serial monitor
          switch ( inByte )
          {
            case DC4_NORMALHB :
              // DC4_NORMALHB : Playback of Normal Heart Beat
              normalHBPlayback();
            break;
            case DC4_ESHMURMUR :
              // DC4_ESHMURMUR : Playback of Early Systolic Heart Murmur
              earlyHMPlayback();
            break;

            // ...

            default :
              Serial.println( "DC4: 2nd byte not recieved" );
          }
        }
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
