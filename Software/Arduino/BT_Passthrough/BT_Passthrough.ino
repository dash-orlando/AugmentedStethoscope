/*  Pass serial data bidirctionally between the USB com port and Bluetooth  */

#define VERSION   0.01
#define SPEED     115200
#define BTooth    Serial2


//String  stethoscopeName = "RNBT-817D";
String  stethoscopeName = "RNBT-96C4:";
String  tempbuff        = "read: ";
int     cnt             = 0;


void setup()
{
  Serial.begin( SPEED );
  while ( ! Serial ) ;                          // for Teensy to wait for the Monitor to open
  Serial.println( "getting ready..." );         // *** debug ***
  delay( 70 );                                  // *** debug ***

  BTooth.begin( SPEED );                        // The Bluetooth Smirf defaults to 115200bps
  BTooth.print( '$' );                          // Print three times individually
  BTooth.print( '$' );
  BTooth.print( '$' );                          // Enter command mode
  Serial.println( BTooth.available() );         // *** debug ***
  while ( BTooth.available() < 1 ) ;
  cnt = BTooth.available();
  Serial.print( cnt );                          // *** debug ***
  Serial.println( " bytes came back." );        // *** debug ***
  cnt += 5;                                     // *** debug ***
  while ( BTooth.available() > 0 ) tempbuff += (char)BTooth.read(); // *** debug ***
  Serial.println( tempbuff );                   // *** debug ***
  for ( int i = 5; i < cnt; i++ )               // *** debug ***
  {                                             // *** debug ***
    Serial.print( '[' ); Serial.print( tempbuff[i], HEX ); Serial.print( ']' );
  }                                             // *** debug ***
  Serial.println( '\n' );                       // *** debug ***
  delay( 100 );  // Short delay, wait for the Smirf to send back "CMD"
//  BTooth.println( 'D' );
//  BTooth.println( 'X' );
  BTooth.println( 'h' );

  pairWithBtDevice( stethoscopeName );
}


boolean pairWithBtDevice( String btDevice )
{
  return true;
}


void loop()
{
  if( BTooth.available() )                      // If data are received from bluetooth...
    Serial.print( (char)BTooth.read() );        // ....print as characters on the serial monitor.

  if( Serial.available() )                      // If characters are received from serial monitor...
    BTooth.print( (char)Serial.read() );        // ....transmit on bluetooth.
}
