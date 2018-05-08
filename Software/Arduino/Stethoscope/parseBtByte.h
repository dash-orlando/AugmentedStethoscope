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
#include  "OperationalFunctions.h"
#include  "DeviceSpecificFunctions.h"
#include  "SimulationFunctions.h"

// ==============================================================================================================
// Display Byte
//
// This function translates and displays byte information on a connected serial monitor
// Serial monitors may be enabled for diagnostics
//
// Michael Xynidis
// Fluvio L Lobo Fenoglietto 05/02/2018
// ============================================================================================================== //

void displayByte( byte byteValue )
{
  Serial.print( "received: [" );
  Serial.print( (char)byteValue );
  Serial.print( "][" );
  Serial.print( byteValue, HEX );
  Serial.println( ']' );
}

// ==============================================================================================================
// Display Byte
//
// This function prints the different states of the stethoscope
//
// Michael Xynidis
// Fluvio L Lobo Fenoglietto 05/02/2018
// ============================================================================================================== //

void displayStatus()
{
  Serial.print( "(pre) deviceState    : " );  Serial.println( stateToText( deviceState     ) );
} // End of displayState()

// ==============================================================================================================
// Parse Byte
//
// This function parses incoming bytes (or byte sequences) and calls/executes functions associated with such bytes
//
// Michael Xynidis
// Fluvio L Lobo Fenoglietto 05/02/2018
// ============================================================================================================== //
void parseBtByte( String fn )
{
    byte  inByte = 0x00;

    char  filename[fn.length()+1];
    fn.toCharArray( filename, sizeof( filename ) );

    Serial.print( "\nBTooth inbound queue, no. of bytes available: " ); 
    Serial.println( BTooth.available() );

    inByte = BTooth.read();                                                                                       // get incoming byte
    displayByte( inByte );
    displayStatus();
    switch ( inByte )
    {
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
      case SENDWAV :
        // SENDWAV : Send .WAV File
        sendWav( fn );
      break;
      case DELVOLATILE :
        //  DELVOLATILE : Delete Volatile Directory
      break;

      // Operational Functions ====================================================================== //
      case SENDRAW :
        // SENDRAW : Send Raw Data from SD Card
        Serial.println( "received: SENDRAW..." );
        sendRAW( "KOROT.RAW" );
      break;
      
      // Device-Specific Functions ================================================================== //
	    case STARTREC :
        // STARTREC : Start Recording
        Serial.println( "received: STARTREC..." );
        //startRecording();
      break;
      case STARTCREC :
        // STARTCREC : Start Custom Recording
        Serial.println( "received: STARTCREC..." );
        inString = parseString();                                                                      // Parse input string
        recString = setRecordingFilename( inString, recExtension );                                    // Create recording string with appropriate extension
        startRecording( recString );                                                                   // Start custom filename recording
      break;
	    case STOPREC :
  	    // STOPREC : Stop Recording
        Serial.println( "received: STOPREC..." );
  	    stopRecording();
  	  break;
  	  case STARTPLAY :
  	    // STARTPLAY : Start Playing
  	    startPlaying( ses.filePly1 );
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

      // Simulation Functions =======================================================================  //
      case NHBSYN :
        // NHBSYN : Blending of Synthetic, Normal Heart Beat
        NHBSBlend();
      break;
      case ESMSYN :
        // ESMSYN : Blending of Synthetic, Early Systolic Heart Murmur
        ESMSBlend();
      break;
      case NHBREC   :
        // NHBREC   : Blending of Recorded, Normal Heart Beat
        NHBRBlend();
      break;
      case EHBREC   :
        // EHBREC   : Blending of Recorded, Exercised Heart Beat
        EHBRBlend();
      break;
      case KOROT :
        // KOROT : Start Augmenting KorotKoff Sound
        fileName = KOROTBlend(fileName);
      break;
      case PSTRING :
        // PSTRING : Parse String
        inString = parseString();
        recString = setRecordingFilename( inString, recExtension );
        startRecording( recString );
      break;
      
      case EDMSYN :
        // EDMSYN : Start Augmenting Early Diastolic Heart Murmur
        fileName = EDMSYNBlend(fileName);
      break;

      case ASYSYN :
        // ASYSYN : ...
        fileName = ASYSYNBlend(fileName);
      break;

      case PEJECT :
        // PEJECT : ...
        fileName = PEJECTBlend(fileName);
      break;

      case PSPLIT :
        // PSPLIT : ...
        fileName = ses.filePly9;
        fileName = PSPLITBlend(fileName);
      break;

      
      default :
        Serial.print( (char)inByte );
      break;
    }
    displayStatus();
    delay( 10 );
}

