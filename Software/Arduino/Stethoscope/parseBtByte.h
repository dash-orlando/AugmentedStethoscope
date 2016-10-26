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
  Serial.print( "received: [" );
  Serial.print( (char)byteValue );
  Serial.print( "][" );
  Serial.print( byteValue, HEX );
  Serial.println( ']' );
}

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
        Serial.println( "received: ENQ..." );
        if ( readyState == READY )
        {
          connectState  = CONNECTED;
          Serial.println( "sending: ACK..." );
          BTooth.write( ACK );
        }
        else
        {
          Serial.println( "sending: NAK..." ); 
          BTooth.write( NAK );
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

      default :
        Serial.print( (char)inByte );
      break;
    }
    Serial.print( "(post) readyState: ");    Serial.println( stateToText( readyState   ) );
    Serial.print( "(post) connectState: ");  Serial.println( stateToText( connectState ) );
    Serial.print( "(post) recordState: ");   Serial.println( stateToText( recordState  ) );
    delay( 10 );
}
