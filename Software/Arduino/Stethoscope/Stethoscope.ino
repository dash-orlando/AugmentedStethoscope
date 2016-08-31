#define DEBUG true
#define VERSION  0.01

#include  "TeensyAudio.h"
#include  "FileSD.h"
#include  "SignalProcessing.h"
#include  "bluetooth.h"

byte      inByte;


void setup()
{
  buttonLEDSetup();
  BtSetup();
  Serial.begin( 115200 );                   // for debugging
  if ( DEBUG )
  {
    // Start the HW serial port for console messages
    while ( !Serial ) ;                     // for debugging
    Serial.println( "//Setup complete" );   // for debugging
  }
  delay( 50 );
  if ( sdCardCheck() )
  {
    setLEDs( GOODTOGO );                    // SD file system is happy and working
    rootDir = SD.open( "/" );
    printDirectory( rootDir, 1 );
//    delay( 2000 );
  }
  else
  {
    setLEDs( ERRORS );                      // SD file system is unhappy; loop until...
    setLEDs( GOODTOGO );                    // SD file system is happy again
  }
  delay( 1000 );                            // Give everybody a sec to get composed.
  establishContact();                       // send a string to establish contact until receiver responds
}


void loop()
{  // when using a microphone, continuously adjust gain
  if ( myInput == AUDIO_INPUT_MIC ) adjustMicLevel();

  // if we get a valid byte, read analog ins:
  if ( Serial.available() > 0 )
  {
    inByte = Serial.read();                 // get incoming byte
    switch ( inByte )
    {
      case 0x41 :
      Serial.write( 0x42 );
      break;
      case ENQ :
      case EOT :
      case ACK :
      case NAK :
      case CAN :
      case ESC :
      break;
      
      //
      //  *** Start RECORDing by remote command
      //
      case DC1_STRTREC :
      if ( ( connectState == CONNECTED ) && ( recordState == STANDBY ) )
      {
        if ( startRecording() )
        {
          Serial.write( DC1_STRTREC ); Serial.write( ACK );
          digitalWrite( REC, HIGH );
        }
        else Serial.write( NAK );
      }
      else Serial.write( DC1_STRTREC ); Serial.write( NAK );
      break;

      //
      //  *** Stop RECORDing by remote command
      //
      case DC2_STPREC :
      if ( ( connectState == CONNECTED ) && ( recordState == RECORDING ) )
      {
        if ( stopRecording() )
        {
          Serial.write( DC2_STPREC ); Serial.write( ACK );
          digitalWrite( REC, LOW );
        }
        else Serial.write( NAK );
      }
      else Serial.write( DC2_STPREC ); Serial.write( NAK );
      break;

      //
      //  *** Start PLAYing by remote command
      //
      case DC3_STRTPLY :
      if ( ( connectState == CONNECTED ) && ( recordState == STANDBY ) )
      {
        if ( startPlaying() )
        {
          Serial.write( DC3_STRTPLY ); Serial.write( ACK );
          digitalWrite( REC, HIGH );
        }
        else Serial.write( NAK );
      }
      else Serial.write( DC3_STRTPLY ); Serial.write( NAK );
      break;

      //
      //  *** Stop PLAYing by remote command
      //
      case DC4_STPPLY :
      if ( ( connectState == CONNECTED ) && ( recordState == PLAYING ) )
      {
        if ( stopPlaying() )
        {
          Serial.write( DC4_STPPLY ); Serial.write( ACK );
          digitalWrite( REC, LOW );
        }
        else Serial.write( NAK );
        
      }
      else Serial.write( DC4_STPPLY ); Serial.write( NAK );
      break;
      default :
      break;
    }
  }
}


void establishContact()
{
  while ( Serial.available() <= 0 )
  {
    Serial.println( "ENQ" );                // send an initial string
    delay( 300 );
  }
}



