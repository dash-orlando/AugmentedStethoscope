/*
 * Stethoscope
 * 
 * The following program has been built to control the statehoscope module
 * 
 * Michael Xynidis
 * Fluvio L Lobo Fenoglietto
 * 09/14/2016
 */


#define DEBUG false
#define VERSION  0.05

#include  "TeensyAudio.h"
#include  "FileSD.h"
#include  "SignalProcessing.h"
#include  "buttonsAndLeds.h"
#include  "protocol.h"

byte      inByte = 0x00;


void setup()
{
  //buttonLEDSetup();

  //BTooth.setRX ( 26 );                      // comment out these two lines
  //BTooth.setTX ( 31 );                      // ...to NOT reassign Serial RX & TX pins
  BTooth.begin( SPEED );
  Serial.begin( SPEED );                    // for debugging

//  if ( DEBUG )
//  {
//    // Start the HW serial port for console messages
//    while ( !Serial ) ;                     // for debugging
//    Serial.println( "//Setup complete" );   // for debugging
//  }
//  delay( 50 );
//  if ( sdCardCheck() )
//  {
//    setLEDs( GOODTOGO );                    // SD file system is happy and working
//    rootDir = SD.open( "/" ); Serial.print( "rootDir: " ); Serial.println( rootDir );
//    printDirectory( rootDir, 1 );
//    if ( SD.exists( "RECORD.RAW" ) )
//    {
//      SD.remove( "RECORD.RAW" );
//      Serial.println( "Deleting 'RECORD.RAW' for testing." );
//      rootDir = SD.open( "/" );
//      printDirectory( rootDir, 1 );
//    }
//    readyState = READY;
//    delay( 2000 );
//  }
//  else
//  {
//    setLEDs( ERRORS );                      // SD file system is unhappy; loop until...
//    setLEDs( GOODTOGO );                    // SD file system is happy again
//    readyState = NOTREADY;
//  }
  digitalWrite( REC, HIGH );
  digitalWrite( TX,  HIGH );
  digitalWrite( RX,  HIGH );
  digitalWrite( LED_BUILTIN,  HIGH );
  delay( 5000 );
  digitalWrite( REC, LOW );
  digitalWrite( TX,  LOW );
  digitalWrite( RX,  LOW );
  digitalWrite( LED_BUILTIN,  LOW );
}

void loop()
{  // when using a microphone, continuously adjust gain
  if ( myInput == AUDIO_INPUT_MIC ) adjustMicLevel();

  // if we get a valid byte, read analog ins:
  if ( BTooth.available() > 0 )
  {
    Serial.print( "\nBTooth inbound queue, no. of bytes available: " ); 
    Serial.println( BTooth.available() );

    inByte = BTooth.read();                     // get incoming byte

    Serial.print( "received: [" );
    Serial.print( (char)inByte );
    Serial.print( "][" );
    Serial.print( inByte, HEX );
    Serial.println( ']' );
    Serial.print( "(pre) readyState: ");    Serial.println( stateToText( readyState   ) );
    Serial.print( "(pre) connectState: ");  Serial.println( stateToText( connectState ) );
    Serial.print( "(pre) recordState: ");   Serial.println( stateToText( recordState  ) );
    switch ( inByte )
    {
      case CHK :
        if ( sdCardCheck() )
        {
          setLEDs( GOODTOGO );                    // SD file system is happy and working
          rootDir = SD.open( "/" ); Serial.print( "rootDir: " ); Serial.println( rootDir );
          printDirectory( rootDir, 1 );
          if ( SD.exists( "RECORD.RAW" ) )
          {
            SD.remove( "RECORD.RAW" );
            Serial.println( "Deleting 'RECORD.RAW' for testing." );
            rootDir = SD.open( "/" );
            printDirectory( rootDir, 1 );
          }
            readyState = READY;
            BTooth.write( ENQ );
            BTooth.write( ACK );
            delay( 2000 );
        }
        else
        {
          setLEDs( ERRORS );                      // SD file system is unhappy; loop until...
          setLEDs( GOODTOGO );                    // SD file system is happy again
          BTooth.write( ENQ );
          BTooth.write( NAK );
          readyState = NOTREADY;
        }
      break;
      
      case ENQ :
        if ( readyState == READY )
        {
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
      
      //
      //  *** Start RECORDing by remote command
      //
      case DC1_STRTREC :
        if ( ( connectState == CONNECTED ) && ( recordState == STANDBY ) )
        {
          if ( startRecording() )
          {
            BTooth.write( DC1_STRTREC );
            BTooth.write( ACK );
            digitalWrite( REC, HIGH );
            Serial.println( "REC-ACK" );
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
          if ( stopRecording() )
          {
            BTooth.write( DC2_STPREC );
            BTooth.write( ACK );
            digitalWrite( REC, LOW );
            Serial.println( "STPR-ACK" );
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
          if ( startPlaying() )
          {
            BTooth.write( DC3_STRTPLY );
            BTooth.write( ACK );
            flashLEDs( 5000, LOW, LOW );
            Serial.println( "PLY-ACK" );
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
          if ( stopPlaying() )
          {
            BTooth.write( DC4_STPPLY );
            BTooth.write( ACK );
            stopLEDs();
            Serial.println( "STPP-ACK" );
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
          if ( SD.exists( "RECORD.RAW" ) )
          {
            SD.remove( "RECORD.RAW" );
            Serial.println( "Deleting 'RECORD.RAW'." );
            BTooth.write( ESC );
            BTooth.write( ACK );
            Serial.println( "ESC-ACK" );
          }
          else
          {
            Serial.println( "'RECORD.RAW' does not exist." );
            BTooth.write( ESC );
            BTooth.write( NAK );
            Serial.println( "ESC-NAK" );
          }
          rootDir = SD.open( "/" );
          printDirectory( rootDir, 1 );
        }
      break;
      default :
      break;
    }
    Serial.print( "(post) readyState: ");    Serial.println( stateToText( readyState   ) );
    Serial.print( "(post) connectState: ");  Serial.println( stateToText( connectState ) );
    Serial.print( "(post) recordState: ");   Serial.println( stateToText( recordState  ) );
  }
  inByte = 0x00;
} // */

