/*
 * Stethoscope
 * 
 * The following program has been built to control the stethoscope module
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
  // Serial Communication Initialization
  Serial.begin( SPEED );                                                                            // USB Serial Communication
  BTooth.begin( SPEED );                                                                            // RF/Bluetooth Serial Communication

  // SD Reader and Card Check
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
  }
  else
  {
    readyState = NOTREADY;
  }
} // End of setup()

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
      //
      //  *** Perform a systems check via remote trigger
      //
      case CHK :
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
      break;

      //
      //  *** Check system status
      //
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
      
      //
      //  *** Start RECORDing by remote command
      //
      case DC1_STRTREC :
        Serial.println( "received: DC1_STRTREC..." );
        if ( ( connectState == CONNECTED ) && ( recordState == STANDBY ) )
        {
          if ( startRecording() )
          {
            Serial.println( "sending: ACK..." );
            BTooth.write( ACK );
            digitalWrite( REC, HIGH );
            
          }
          else
          {
            Serial.println( "sending: NAK..." );
            BTooth.write( NAK );
          }
        }
        else
        {
          Serial.println( "sending: NAK..." );
          BTooth.write( NAK );
        }
      break;

      //
      //  *** Stop RECORDing by remote command
      //
      case DC2_STPREC :
        Serial.println( "received: DC2_STPREC..." );
        if ( ( connectState == CONNECTED ) && ( recordState == RECORDING ) )
        {
          if ( stopRecording() )
          {
            Serial.println( "sending: ACK..." );
            BTooth.write( ACK );
            digitalWrite( REC, LOW );
          }
          else
          {
            Serial.println( "sending: NAK..." );
            BTooth.write( NAK );
          }
        }
        else
        {
          Serial.println( "sending: NAK..." );
          BTooth.write( NAK );
        }
      break;

      //
      //  *** Start PLAYing by remote command
      //
      case DC3_STRTPLY :
        Serial.println( "received: DC3_STRTPLY..." );
        if ( ( connectState == CONNECTED ) && ( recordState == STANDBY ) )
        {
          if ( startPlaying() )
          {
            Serial.println( "sending: ACK..." );
            BTooth.write( ACK );
          }
          else
          {
            Serial.println( "sending: NAK..." );
            BTooth.write( NAK );
          }
        }
        else
        {
          Serial.println( "sending: NAK..." );
          BTooth.write( NAK );
        }
      break;

      //
      //  *** Stop PLAYing by remote command
      //
      case DC4_STPPLY :
        Serial.println( "received: DC4_STPPLY..." );
        if ( ( connectState == CONNECTED ) && ( recordState == PLAYING ) )
        {
          if ( stopPlaying() )
          {
            Serial.println( "sending: ACK..." );
            BTooth.write( ACK );
          }
          else
          {
            Serial.println( "sending: NAK..." );
            BTooth.write( NAK );
          }
        }
        else
        {
          Serial.println( "sending: NAK..." );
          BTooth.write( NAK );
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
}

