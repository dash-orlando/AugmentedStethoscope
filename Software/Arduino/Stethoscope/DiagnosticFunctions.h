/*
 * DiagnosticFunctions.h
 *
 * The following script comprises all of the functions used for the hardware diagnostic of the smart device
 *
 * Michael Xynidis
 * Fluvio Lobo Fenoglietto
 * 10/26/2016
 */

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
    if ( SD.exists( "RECORD.RAW" ) )
    {
      SD.remove( "RECORD.RAW" );
			Serial.println( "Deleting 'RECORD.RAW' for testing." );
      rootDir = SD.open( "/" );
      printDirectory( rootDir, 1 );
    }
    readyState = READY;
    BTooth.write( ACK );
    delay( 2000 );
  }
  else
  {
    BTooth.write( NAK );
    readyState = NOTREADY;
  }
} // End of sdCheck

