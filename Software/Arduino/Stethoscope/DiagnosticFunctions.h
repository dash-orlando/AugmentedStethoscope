/*
 * DiagnosticFunctions.h
 *
 * The following script comprises all of the functions used for the hardware diagnostic of the smart device
 *
 * Michael Xynidis
 * Fluvio Lobo Fenoglietto
 * 10/26/2016
 */

 //
 // *** Device Identification
 //
 void deviceID( byte deviceHexCode[] )
 {
   int Nchars = sizeof( deviceHexCode) ;
   for ( int i = 0; i < Nchars; i ++ )
   {
    Serial.print( "sending: [" );
    Serial.print( (char)deviceHexCode[i] );
    Serial.print( "][" );
    Serial.print( deviceHexCode[i], HEX );
    Serial.println( ']' );
    BTooth.write( deviceHexCode[i] );
   } // End of for-loop
  
 } // End of deviceID()

 
 //
 // *** System Check
 //
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
 }
