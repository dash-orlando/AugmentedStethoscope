/*  Pass serial data bidirctionally between the USB com port and Bluetooth  */

#include  "protocol.h"
#define   VERSION     0.03


byte inByte   = 0x00;
byte outByte  = 0x00;


void setup()
{
  BTooth.begin( SPEED );                        // The Bluetooth Smirf defaults to 115200bps.
  Serial.begin( SPEED );                        // Match Serial Monitor to BT--although Teensy always runs 155200.
  setupBT();
//  test( 1 );
}


void test( int cnt )
{
  for ( int i = 0; i < cnt; i++ )
  {
    
  }
}


void setupBT()
{
  while ( ! Serial ) ;                          // Make Teensy wait for the Serial Monitor to open.
  if ( btCommandMode() )
  {
//*
    if ( pairWithBtDevice( stethoscope[_817D].btMAC ) )
    {
      Serial.println( "*******************************************" );
      Serial.println( "*******************************************" );
      Serial.println( "********** Stethoscope Connected **********" );
      Serial.println( "*******************************************" );
      Serial.println( "*******************************************" );
    } // */
//    BTooth.flush();
  }
}


void loop()
{
  if( BTooth.available() != 0 )
  {
    Serial.print( BTooth.available() );
    Serial.println( " bytes in the BT input queue" );
  }
  if( BTooth.available() > 0 )                  // If data are received from bluetooth...
  {
    Serial.print( "got a byte: [" );
    inByte = BTooth.read();
    Serial.print( (char)inByte );
    Serial.print( "]-[" );
    Serial.print( inByte, HEX );                // ....print as characters on the serial monitor.
    Serial.println( ']' );
    inByte = 0x00;
  }

  if( Serial.available() > 0 )                  // If characters are received from serial monitor...
  {
    inByte = (char)Serial.read();
    switch ( inByte )
    {
      case '0' :
        outByte = ENQ;
        break;
      case '1' :
        outByte = DC1_STRTREC;
        break;
      case '2' :
        outByte = DC2_STPREC;
        break;
      case '3' :
        outByte = DC3_STRTPLY;
        break;
      case '4' :
        outByte = DC4_STPPLY;
        break;
      case '5' :
        outByte = ESC;
        break;
      case 'd' :
        Serial.println( "disconecting..." );
        outByte = 0x00;
        btCommandMode();
        BTooth.println( btDisconnect );
        waitBtResponse( respBtDisonnect );
        break;
      case 'c' :
        Serial.println( "connecting..." );
        outByte = 0x00;
        setupBT();
        break;
      default :
        outByte = 0x00;
        break;
    }
    if ( outByte > 0x00 )
    {
      Serial.print( "Sending to BT: " ); Serial.print( outByte ); Serial.print( " / " ); Serial.println( outByte, HEX );
      BTooth.write( outByte );                    // ....transmit on bluetooth.
      delay( 100 );
    }
  }
}
