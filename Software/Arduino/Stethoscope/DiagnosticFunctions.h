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