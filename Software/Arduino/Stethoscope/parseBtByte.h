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
    Serial.print( "(pre) readyState    : " );  Serial.println( stateToText( readyState     ) );
    Serial.print( "(pre) connectState  : " );  Serial.println( stateToText( connectState   ) );
    Serial.print( "(pre) recordState   : " );  Serial.println( stateToText( recordState    ) );
    Serial.print( "(pre) passthruState : " );  Serial.println( stateToText( passthruState  ) );
    Serial.print( "(pre) HBDetectState : " );  Serial.println( stateToText( HBDetectState  ) );
    Serial.print( "(pre) BPAugmentState: " );  Serial.println( stateToText( BPAugmentState ) );
    switch ( inByte )
    {
      case ENQ :
        Serial.println( "received: ENQ..." );
        Serial.print( "readyState is " );      Serial.println( stateToText( readyState     ) );
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

      // Device-Specific Functions ==================================================================  //
	    case STARTREC :
        // STARTREC : Start Recording
        Serial.println( "received: STARTREC..." );
        Serial.print( "readyState is " ); Serial.println( stateToText( readyState ) );
        startRecording();
      break;
	    case STOPREC :
  	    // STOPREC : Stop Recording
        Serial.println( "received: STOPREC..." );
        Serial.print( "readyState is " ); Serial.println( stateToText( readyState ) );
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
  	  case STARTPASSTHRU :
  	    // STARTPASSTHRU : Start Audio Passthrough from Mic
  	    startAudioPassThrough();
  	  break;
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
      case STARTBPNORM :
        // STARTBPNORM : Start Augmenting BP Heart Sounds (normal rate)
        ses.heartRate = normalSinus;
        startAugmentingBP();
      break;     
      case STARTBPBRADY :
        // STARTBPBRADY : Start Augmenting BP Heart Sounds (slow rate)
        ses.heartRate = bradycardia;
        startAugmentingBP();
      break;     
      case STARTBPTACHY :
        // STARTBPTACHY : Start Augmenting BP Heart Sounds (rapid rate)
        ses.heartRate = tachycardia;
        startAugmentingBP();
      break;     
      case STOPBPALL :
        // STOPBPALL : Stop Augmenting BP Heart Sounds (return to passthrough)
        stopAugmentingBP();
      break;
      case KOROT :
        // KOROT : Start Augmenting KorotKoff Sound
        fileName = KOROTBlend(fileName);
      break;
      case PSTRING :
        // PSTRING : Parse String
        parseString();
      break;
      
      default :
        Serial.print( (char)inByte );
      break;
    }
    Serial.print( "(post) readyState    :  " ); Serial.println( stateToText( readyState     ) );
    Serial.print( "(post) connectState  :  " ); Serial.println( stateToText( connectState   ) );
    Serial.print( "(post) recordState   :  " ); Serial.println( stateToText( recordState    ) );
    Serial.print( "(post) passthruState :  " ); Serial.println( stateToText( passthruState  ) );
    Serial.print( "(post) HBDetectState :  " ); Serial.println( stateToText( HBDetectState  ) );
    Serial.print( "(post) BPAugmentState:  " ); Serial.println( stateToText( BPAugmentState ) );
    delay( 10 );
}

