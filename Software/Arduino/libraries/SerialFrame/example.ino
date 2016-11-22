#include <SerialFrame.h>


SerialFrame sf1 = SerialFrame();

SerialFrame sf2 = SerialFrame( 1, GENERIC, GENERAL, "1234" );

Frame f1, f2, f3;

Frame fr = (Frame)
  {
    2,
    STETHOSCOPE,
    HEARTRATE,
    "Giasou Cosmos"
  };

SerialFrame sf3 = SerialFrame( fr );


void setup()
{
  Serial.begin( 115200 );
  f1 = sf2.Get();
  sf2.Set( 1, OSCOPE, HEARTRATE, "42" );
  f2 = sf2.Get();
}


void loop()
{
/*
  Serial.print( sf1.frame.DevID ); Serial.print( sf1.frame.DataString ); Serial.print( ", " );
  Serial.print( sf2.frame.DevID ); Serial.print( sf2.frame.DataString ); Serial.print( ", " );
  Serial.print( sf3.frame.DevID ); Serial.print( sf3.frame.DataString ); Serial.print( ", " );
  
  Serial.print( sf3.frame.DevID ); Serial.println();
  Serial.print( fr.DevID ); Serial.print( ", " );  Serial.print( fr.DevType ); Serial.print( ", " );  Serial.println( fr.DatType );  // 2
  Serial.print( f1.DevID ); Serial.print( ", " );  Serial.print( f1.DevType ); Serial.print( ", " );  Serial.println( f1.DatType );  // 1
  Serial.print( f2.DevID ); Serial.print( ", " );  Serial.print( f2.DevType ); Serial.print( ", " );  Serial.println( f2.DatType );  // 1
  Serial.println();
 */
 
}