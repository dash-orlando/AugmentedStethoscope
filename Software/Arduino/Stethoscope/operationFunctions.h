/*
 * operationFunctions.h
 *
 * The following script contains all the operation functions used by the smart devices
 *
 * Michael Xynidis
 * Fluvio L Lobo Fnenoglietto
 * 10/20/2016
 */

 //
 // *** System Check
 //
 void systemCheck()
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
