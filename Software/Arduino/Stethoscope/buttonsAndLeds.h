#include      <TimerOne.h>
#include      <TimerThree.h>                          // ***TEENSY***
//#include      <Bounce.h>

#define       REC       16  //LED_BUILTIN
#define       TX        15
#define       RX        14
#define       BTTX       2
#define       BTRX       3
//#define       BTN        4

#define       GOODTOGO   0
#define       ERRORS     1

int           TxLed      = LOW;
int           RxLed      = LOW;

//boolean       buttonClicked   = false;


void buttonLEDSetup()
{
  pinMode( REC, OUTPUT );
  pinMode( TX,  OUTPUT );
  pinMode( RX,  OUTPUT );
  digitalWrite( REC, LOW );
  digitalWrite( TX,  LOW );
  digitalWrite( RX,  LOW );
}


void setLEDs( int initTxLED, int initRxLED )
{
  TxLed       = initTxLED;
  RxLed       = initRxLED;
  digitalWrite( TX, TxLed );
  digitalWrite( RX, RxLed );
}


void alternateLEDs( void )
{
  Timer3.stop();
  setLEDs( !TxLed,!RxLed );
  Timer3.restart();
}


void flashLEDs( int duration, int initTxLED, int initRxLED )
{
  setLEDs( initTxLED, initRxLED );
  Timer3.initialize( duration );
  Timer3.attachInterrupt( alternateLEDs );
}


void stopLEDs( void )
{
  Timer3.detachInterrupt();
  setLEDs( LOW, LOW );
}


//*
void setLEDs( int error )
{
  if ( error == 1 )
  {
      if ( DEBUG ) Serial.println( "LED state: ERROR" );
      while ( !sdCardCheck() )
      {
        digitalWrite( TX, HIGH );
        digitalWrite( RX, HIGH );
        delay( 500 );
        digitalWrite( TX, LOW );
        digitalWrite( RX, LOW );
        delay( 500 );
      }
  }
  else if ( error == 2 )
  {
    ; // for when another error state is defined
  }
  else
  {
    if ( DEBUG ) Serial.println( "LED state: NO_ERROR" );
    for ( int i = 0; i < 4; i++ )
    {
      digitalWrite( TX, HIGH );
      digitalWrite( RX, HIGH );
      delay( 50 );
      digitalWrite( TX, LOW );
      digitalWrite( RX, LOW );
      delay( 75 );
    }
  }
} // */ 

