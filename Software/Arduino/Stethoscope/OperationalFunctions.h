/*
 * OperationalFunctions.h
 *
 * The following script comprises all of the functions used for the general operation of the smart device
 *
 * Michael Xynidis
 * Fluvio Lobo Fenoglietto
 * 10/26/2016
 */

// ==============================================================================================================
// Send RAW file
// ...
// 
// Fluvio L Lobo Fenoglietto 05/03/2018
// ==============================================================================================================
void sendRAW( String filename )
{
  char  filePly[fileName.length()+1];                                                                           // Conversion from string to character array
  fileName.toCharArray( filePly, sizeof( filePly ) );
  File dataFile = SD.open( filePly );
  BTooth.write( ACK );
  Serial.println("...Starting...");
  int fileSize = dataFile.size();
  Serial.println( fileSize );
  if( dataFile )
  {
    while( dataFile.available() )
    {
      BTooth.write( dataFile.read() );
    }
    Serial.println("...completed...");
    dataFile.close();
  }
} // End of sendRAW()

// ==============================================================================================================
// Send WAV file
// ...
// 
// Michael Xynidis --2016
// ==============================================================================================================
void sendWav( String fn )
{
  char  filename[fn.length()+1];
  fn.toCharArray( filename, sizeof( filename ) );
  
  if ( connectState == CONNECTED )
  {
    if ( SD.exists( filename ) )
    {
	   Serial.print( "Sending '" );
		 Serial.print( fn );
     Serial.print( "'.\n" );
     delay( 6000 );                                            // ...optional; in case time is needed to start host receive
     sendFileSerial( SD.open( filename ) );                    // <--**** new function that sends WAV over Serial
     BTooth.write( ACK );
     Serial.println( "ESC-ACK" );
    }
    else
    {
     Serial.print( "'" );
		 Serial.print( fn );
		 Serial.print( "' does not exist.\n" );
     BTooth.write( NAK );
     Serial.println( "ESC-NAK" );
    }
    rootDir = SD.open( "/" );
    printDirectory( rootDir, 1 );
  }
}

