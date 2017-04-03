/*
 * OperationalFunctions.h
 *
 * The following script comprises all of the functions used for the general operation of the smart device
 *
 * Michael Xynidis
 * Fluvio Lobo Fenoglietto
 * 10/26/2016
 */

 
 //
 // *** Send .WAV file
 //
 
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
