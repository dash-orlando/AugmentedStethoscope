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
void sendRAW( String fileName )
{
  Serial.println( fileName );
  char  filePly[fileName.length()+1];                                                                           // Conversion from string to character array
  fileName.toCharArray( filePly, sizeof( filePly ) );
  File dataFile = SD.open( filePly );
  BTooth.write( ACK );
  Serial.println("...Starting...");
  // determine the size of the file
  int fileSize = dataFile.size();
  Serial.println( fileSize );
  
  if( dataFile )
  {
    //for( int i = 0; i < 100; i++)
    int i = 0;
    while( dataFile.available() )
    {
      byte buf[64];
      Serial.print( i );
      Serial.print( "," );
      for( int j = 0; j < 64; j++ )
      {
        byte inByte = dataFile.read();
        buf[j] = inByte;
        Serial.print( inByte, HEX );
        Serial.print( "," );
      }
      Serial.println();
      //Serial.println( sizeof(buf) );
      //byte inByte = dataFile.read();
      //Serial.print( i );
      //Serial.print( ", " );
      //Serial.print( buf[0], HEX );
      //Serial.print( "," );
      //Serial.print( buf[1], HEX );
      BTooth.write( buf, sizeof(buf) );
      i = i + 1;
    }
    BTooth.write( "END" );
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
  
  if ( deviceState == READY )
  {
    if ( SD.exists( filename ) )
    {
	   Serial.print( "Sending '" );
		 Serial.print( fn );
     Serial.print( "'.\n" );
     delay( 2000 );                                            // ...optional; in case time is needed to start host receive
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

