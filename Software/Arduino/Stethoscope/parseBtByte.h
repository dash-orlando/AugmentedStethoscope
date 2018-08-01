/*
 * parseBtByte.h
 * 
 * The following script has been designed to control the byte-parsing of the main loop
 * 
 * Michael Xynidis
 * Fluvio L Lobo Fenoglietto
 * 01/31/2017
 */


// ==============================================================================================================
// Import Libraries and/or Modules
// ============================================================================================================== //

#include  "DiagnosticFunctions.h"
#include  "DeviceSpecificFunctions.h"
#include  "SimulationFunctions.h"

// ==============================================================================================================
// Variables
// ============================================================================================================== //
byte      inByte          = 0x00;
int       blendByteIndex;


// ==============================================================================================================
// Display Byte
//
// This function translates and displays byte information on a connected serial monitor
// Serial monitors may be enabled for diagnostics
//
// Michael Xynidis
// Fluvio L Lobo Fenoglietto 05/02/2018
// ============================================================================================================== //

void displayByte( byte byteValue ) {
  Serial.println( "> ============================================================= >" );
  Serial.print(   "> Received: " );
  Serial.println( byteValue, HEX );
}

// ==============================================================================================================
// Display Byte
//
// This function prints the different states of the stethoscope
//
// Michael Xynidis
// Fluvio L Lobo Fenoglietto 05/02/2018
// ============================================================================================================== //

void displayStatus() {
  Serial.print( "(pre) deviceState    : " );  Serial.println( stateToText( deviceState ) );
  Serial.print( "(pre) recState       : " );  Serial.println( stateToText( recState    ) );
  Serial.print( "(pre) blendState     : " );  Serial.println( stateToText( blendState  ) );
} // End of displayState()


// ==============================================================================================================
// Blend Byte
//
// This function cross references the input byte with the list of sounds files available for playback
//
// Fluvio L Lobo Fenoglietto 07/30/2018
// ============================================================================================================== //

void blendByteCheck( byte inByte ) {

  //Serial.println( ">  Cross-referencing playback/blending bytes" );
  for( int i = 0; i < ses.lenBlendByteList; i ++ ) {
    if( ses.blendByteList[i] == inByte ) {
      blendByteIndex = i;
      audioBlend( blendByteIndex );
    }
  } // Search within playlist
  
} // End of blendByte()

// ==============================================================================================================
// Parse Byte
//
// This function parses incoming bytes (or byte sequences) and calls/executes functions associated with such bytes
//
// Michael Xynidis
// Fluvio L Lobo Fenoglietto 05/02/2018
// ============================================================================================================== //
void parseBtByte( String fn ) {
    char  filename[fn.length()+1];
    fn.toCharArray( filename, sizeof( filename ) );

    inByte = BTooth.read();                                                                                       // read incoming byte
    displayByte( inByte );
    //displayStatus();

    // Cross-reference function-specific bytes ================================================================= //
    Serial.println( ">  Cross-referencing function-specific bytes" );
    switch ( inByte ) {
      case ENQ :
        statusEnquiry();
      break;
      case ACK :
      case NAK :
      break;
      
	    // Diagnostic Functions =======================================================================  //
      case DEVICEID :
        // DC1_DEVICEID : Device Identification
		    deviceID( STE );
      break;
      case SDCHECK :
        // DC1_SDCHECK
		    sdCheck();
	    break;

      // Operational Functions ====================================================================== //
      
      // Device-Specific Functions ================================================================== //
      case PSTRING :
        // PSTRING : Parse String
        inString = parseString();
      break;
      // Recording ---------------------------------------------------------------------------------- //
      case SETIDLE :
        // SETIDLE : Set to Idle
        setToIdle();
      break;
      case RECMODE :
        // RECMODE : Set Recording Mode
        recMode = setRecordingMode();
      break;
	    case STARTREC :
        // STARTREC : Start Recording
        //Serial.println( "received: STARTREC..." );
        //startRecording();
      break;
      case STARTCREC :
        // STARTCREC : Start Custom Recording
        setRecordingFilename( inString, recExtension, recMode );                                       // Create recording string with appropriate extension
        startRecording( recString );                                                                   // Start custom filename recording
      break;
      case STARTMREC :
        // STARTMREC : Start Multi Channel, Custom Recording
        Serial.println( "received: STARTMREC..." );
        Serial.println( "recording Mode (recMode): 1..." );
        recMode = 1;                                                                                   // Default recording mode (recMode) for the multi-recording is recMode = 1
        inString = parseString();                                                                      // Parse input string
        setRecordingFilename( inString, recExtension, recMode );                                       // Create recording string with appropriate extension
        startMultiChannelRecording( recStrings );                                                      // Start custom filename recording
      break;
	    case STOPREC :
  	    // STOPREC : Stop Recording
        Serial.println( "received: STOPREC..." );
  	    stopRecording();
  	  break;
  	  case STARTPLAY :
  	    // STARTPLAY : Start Playing
  	    //startPlaying( ses.filePly1 );
  	  break;
  	  case STOPPLAY :
  	    // STOPPLAY : Stop Playing
  	    stopPlaying();
  	  break;
  	  //case STARTPASSTHRU :
  	    // STARTPASSTHRU : Start Audio Passthrough from Mic
  	    //startAudioPassThrough();
  	  //break;
  	  case STARTHBMONITOR :
  	    // STARTTRACKING : Start Heart Rate monitoring
  	    startHeartBeatMonitoring();
  	  break;
  	  case STOPHBMONITOR :
  	    // STOPTRACKING : Stop Heart Rate monitoring
  	    stopHeartBeatMonitoring();
  	  break;
      case STARTBLEND :
        // STARTBLEND : Start Blending default audio file
      case STOPBLEND :
        // STOPBLEND : Stop Blending default audio file
        stopBlending();
      break;
      default :
        //Serial.print( (char)inByte );
      break;
    }

    // Cross-reference blend/playback bytes ==================================================================== //
    blendByteCheck( inByte );
    
    //displayStatus();
    delay( 10 );
}

