// ==============================================================================================================
// Diagnostic Functions
//
// Functions related to the diagnostic and status check of the augmented device
// * Created on 10/26/2016
//
// Michael Xynidis 
// Fluvio L. Lobo Fenoglietto 11/12/2017
// ==============================================================================================================

// ==============================================================================================================
// Device ID
// Identify device in question
// 
// Fluvio L. Lobo Fenoglietto 11/12/2017
// ==============================================================================================================
void deviceID( byte deviceHexCode[] )
{
  for ( int i = 0; deviceHexCode[i] != 0x00; i ++ )
  {
    Serial.print( "sending: [" );
    Serial.print( (char)deviceHexCode[i] );
    Serial.print( "][" );
    Serial.print( deviceHexCode[i], HEX );
    Serial.println( ']' );
    BTooth.write( deviceHexCode[i] );
  } // End of for-loop
  
} // End of deviceID()

// ==============================================================================================================
// SD Card Check
// Check status of SD Card
// 
// Michael Xynidis --2016
// ==============================================================================================================
void sdCheck()
{
  if ( sdCardCheck() )
  {
    rootDir = SD.open( "/" );
    Serial.print( "rootDir: " );
    Serial.println( rootDir );
    printDirectory( rootDir, 1 );
    /*
    if ( SD.exists( "RECORD.RAW" ) )
    {
      SD.remove( "RECORD.RAW" );
			Serial.println( "Deleting 'RECORD.RAW' for testing." );
      rootDir = SD.open( "/" );
      printDirectory( rootDir, 1 );
    }
    */
    deviceState = READY;
    BTooth.write( ACK );
    delay( 2000 );
  }
  else
  {
    BTooth.write( NAK );
    deviceState = NOTREADY;
  }
} // End of sdCheck

// ==============================================================================================================
// Status or State Enquiry
// Check status or state of the devices
// The function now retrieves the byte associated with the current state
// 
// Fluvio L Lobo Fenoglietto 05/08/2018
// ============================================================================================================== //
void statusEnquiry()
{ 
  Serial.println( "received: ENQ..." );
  Serial.print( "deviceState is " );
  Serial.println( stateToText( deviceState ) );

  switch( deviceState )
  {
    case READY :
      Serial.println( "sending: ACK..." );
      BTooth.write( ACK );
    break;
    case NOTREADY :
      Serial.println( "sending: NAK..." ); 
      BTooth.write( NAK );
    break;
    case RECORDING :                                                                                               // RECORDING single/multiple audio streams ( this may be broken down later )
      Serial.println( "sending: STARTREC..." );
      BTooth.write( STARTREC );
    break;
    case PLAYING :                                                                                                 // PLAYING audio file
      Serial.println( "sending: STARTREC..." );
      BTooth.write( STARTPLAY );
    break;
    case MONITORING :                                                                                              // MONITORING heart beat ( may be paired with recording )
      Serial.println( "sending: STARTHBMONITOR..." );
      BTooth.write( STARTHBMONITOR );
    break;
    case BLENDING :                                                                                                // BLENDING microphone input with audio file from SD card
      Serial.println( "sending: BLENDING..." );
      BTooth.write( STARTBLEND );
    break;
    case CONTINUING :
      Serial.println( "sending: BLENDING..." );                                                                    // Currently, blending is the only function that uses the continue state (this should be deprecated in the future)
      BTooth.write( STARTBLEND );
    break;
  }
} // End of statusEnquiry()
