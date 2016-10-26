/* include the "include statement in void loop(),

	#include  "parseBtByte.h"

   and modify void loop() as follows:

void loop()
{  // when using a microphone, continuously adjust gain
  if ( myInput == AUDIO_INPUT_MIC ) adjustMicLevel();

  // if we get a valid byte, read analog from BT:
  if ( BTooth.available() > 0 ) parseBtByte( "RECORD.RAW" );

  // If playing or recording, carry on...
  if ( mode == 1 ) continueRecording();
  if ( mode == 2 ) continuePlaying();
  
  // Clear the input byte variable
  inByte = 0x00;								// this line of code may be unnecessary
}

*/


void displayByte( byte byteValue )
{
  blipRX();
  Serial.print( "received: [" );
  Serial.print( (char)byteValue );
  Serial.print( "][" );
  Serial.print( byteValue, HEX );
  Serial.println( ']' );
}

#define         DC1                 0x00      // temp definition, so that code will compile
#define         DC2                 0x01      // temp definition, so that code will compile
#define         DC3                 0x02      // temp definition, so that code will compile
#define         DC4                 0x03      // temp definition, so that code will compile

// Place functions here...


void parseBtByte( String fn )
{
    byte  inByte = 0x00;

    char  filename[fn.length()+1];
    fn.toCharArray( filename, sizeof( filename ) );

    Serial.print( "\nBTooth inbound queue, no. of bytes available: " ); 
    Serial.println( BTooth.available() );

    inByte = BTooth.read();                     // get incoming byte
    displayByte( inByte );
    Serial.print( "(pre) readyState: ");    Serial.println( stateToText( readyState   ) );
    Serial.print( "(pre) connectState: ");  Serial.println( stateToText( connectState ) );
    Serial.print( "(pre) recordState: ");   Serial.println( stateToText( recordState  ) );
    switch ( inByte )
    {
      case ENQ :
        if ( readyState == READY )
        {
          blipTX();
          connectState  = CONNECTED;
          Serial.println( "received: ENQ" );
          Serial.print( "sending: ENQ..." );
          Serial.print( ENQ );
          BTooth.write( ENQ ); 
          Serial.print( "...done.\nsending: ACK..." );
          Serial.print( ACK );
          BTooth.write( ACK );
          Serial.println( "...done." );
        }
        else
        {
          BTooth.write( ENQ ); 
          BTooth.write( NAK );
          Serial.println( "sending: ENQ-NAK" );
        }
      break;
      case EOT :
      case ACK :
      case NAK :
      case CAN :
      break;
      
      case DC1 :
        if ( BTooth.available() > 0 )
        {
          inByte = BTooth.read();                     // get incoming byte
          displayByte( inByte );                      // display on serial monitor
          switch ( inByte )
          {
            case 0x00 :
              //  DC1_00();
            break;
            case 0x01 :
              //  DC1_01();
            break;

            // ...

            default :
              Serial.println( "DC1: 2nd byte not recieved" );
          }
        }
      break;
      case DC2 :
        if ( BTooth.available() > 0 )
        {
          inByte = BTooth.read();                     // get incoming byte
          displayByte( inByte );                      // display on serial monitor
          switch ( inByte )
          {
            case 0x00 :
              //  DC2_00();
            break;
            case 0x01 :
              //  DC2_01();
            break;

            // ...

            default :
              Serial.println( "DC2: 2nd byte not recieved" );
          }
        }
      break;
      case DC3 :
        if ( BTooth.available() > 0 )
        {
          inByte = BTooth.read();                     // get incoming byte
          displayByte( inByte );                      // display on serial monitor
          switch ( inByte )
          {
            case 0x00 :
              //  DC3_00();
            break;
            case 0x01 :
              //  DC3_01();
            break;

            // ...

            default :
              Serial.println( "DC3: 2nd byte not recieved" );
          }
        }
      break;
      case DC4 :
        if ( BTooth.available() > 0 )
        {
          inByte = BTooth.read();                     // get incoming byte
          displayByte( inByte );                      // display on serial monitor
          switch ( inByte )
          {
            case 0x00 :
              //  DC4_00();
            break;
            case 0x01 :
              //  DC4_01();
            break;

            // ...

            default :
              Serial.println( "DC4: 2nd byte not recieved" );
          }
        }
      break;

      //
      //  *** Start RECORDing by remote command
      //
      case DC1_STRTREC :
        if ( ( connectState == CONNECTED ) && ( recordState == STANDBY ) )
        {
          blipTX();
          if ( startRecording() )
          {
            BTooth.write( DC1_STRTREC );
            BTooth.write( ACK );
            digitalWrite( REC, HIGH );
            Serial.println( "REC-ACK" );
//			continueRecording();
			mode = 1;
          }
          else
          {
            BTooth.write( DC1_STRTREC );
            BTooth.write( NAK );
            Serial.println( "REC-NAK" );
          }
        }
        else
        {
          BTooth.write( DC1_STRTREC );
          BTooth.write( NAK );
          Serial.println( "REC-NAK2" );
        }
      break;

      //
      //  *** Stop RECORDing by remote command
      //
      case DC2_STPREC :
        if ( ( connectState == CONNECTED ) && ( recordState == RECORDING ) )
        {
          blipTX();
          if ( stopRecording() )
          {
            BTooth.write( DC2_STPREC );
            BTooth.write( ACK );
            digitalWrite( REC, LOW );
            Serial.println( "STPR-ACK" );
			mode = 0;
          }
          else
          {
            BTooth.write( DC2_STPREC );
            BTooth.write( NAK );
            Serial.println( "STPR-NAK" );
          }
        }
        else
        {
          BTooth.write( DC2_STPREC );
          BTooth.write( NAK );
          Serial.println( "STPR-NAK2" );
        }
      break;

      //
      //  *** Start PLAYing by remote command
      //
      case DC3_STRTPLY :
        if ( ( connectState == CONNECTED ) && ( recordState == STANDBY ) )
        {
          blipTX();
          if ( startPlaying() )
          {
            BTooth.write( DC3_STRTPLY );
            BTooth.write( ACK );
            flashLEDs( 1000, LOW, LOW );
            Serial.println( "PLY-ACK" );
	//		continuePlaying();
			mode = 2;
          }
          else
          {
            BTooth.write( DC3_STRTPLY );
            BTooth.write( NAK );
            Serial.println( "PLY-NAK" );
          }
        }
        else
        {
          BTooth.write( DC3_STRTPLY );
          BTooth.write( NAK );
          Serial.println( "PLY-NAK2" );
        }
      break;

      //
      //  *** Stop PLAYing by remote command
      //
      case DC4_STPPLY :
        if ( ( connectState == CONNECTED ) && ( recordState == PLAYING ) )
        {
          blipTX();
          if ( stopPlaying() )
          {
            BTooth.write( DC4_STPPLY );
            BTooth.write( ACK );
            stopLEDs();
            Serial.println( "STPP-ACK" );
			mode = 0;
          }
          else
          {
            BTooth.write( DC4_STPPLY );
            BTooth.write( NAK );
            Serial.println( "STPP-NAK" );
          }
        }
        else
        {
          BTooth.write( DC4_STPPLY );
          BTooth.write( NAK );
          Serial.println( "STPP-NAK" );
        }
      break;
      //
      //  *** Delete 'RECORD.RAW' file from SD card
      //
      case ESC :
        if ( connectState == CONNECTED )
        {
          blipTX();
          if ( SD.exists( filename ) )
          {
            Serial.print( "Sending '" );
            Serial.print( fn );
            Serial.print( "'.\n" );
            delay( 6000 );                                            // ...optional; in case time is needed to start host receive
            sendFileSerial( SD.open( filename ) );                    // <--**** new function that sends WAV over Serial
            BTooth.write( ESC );
            BTooth.write( ACK );
            Serial.println( "ESC-ACK" );
          }
          else
          {
            Serial.print( "'" );
			      Serial.print( fn );
			      Serial.print( "' does not exist.\n" );
            BTooth.write( ESC );
            BTooth.write( NAK );
            Serial.println( "ESC-NAK" );
          }
          rootDir = SD.open( "/" );
          printDirectory( rootDir, 1 );
        }
      break;
      default :
        Serial.print( (char)inByte );
      break;
    }
    Serial.print( "(post) readyState: ");    Serial.println( stateToText( readyState   ) );
    Serial.print( "(post) connectState: ");  Serial.println( stateToText( connectState ) );
    Serial.print( "(post) recordState: ");   Serial.println( stateToText( recordState  ) );
    delay( 10 );
}
