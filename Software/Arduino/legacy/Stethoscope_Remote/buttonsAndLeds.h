#include      <TimerOne.h>
#include      <TimerThree.h>                          // ***TEENSY***
#include      <Bounce.h>

#define       REC             LED_BUILTIN
#define       TX              15
#define       RX              14
#define       BTTX             2
#define       BTRX             3
#define       BTN              4
#define       SHORT_CLICK      1
#define       LONG_CLICK       2


int             TxLed           = LOW;
int             RxLed           = LOW;

boolean         buttonClicked   = false;
//boolean       connectedToggle = false;
//boolean       recordingToggle = false;

int           clickMode       = 0;
int           clickModeHx     = 0;

int           ledState        = 0;
int           ledStateXx      = 0;

Bounce        buttonPress     = Bounce( BTN, 8 );     // 8 ms debounce time

elapsedMillis blinkTime;


void userClick()
{
  unsigned int  t   = buttonPress.duration();
  Timer1.stop();
  clickModeHx = clickMode;
  if( t > 170 )
  {
    if ( DEBUG ) Serial.printf( "Long click detected (%d).\n", t );
    clickMode   = 2;
  }
  else
  {
    if ( DEBUG ) Serial.println( "Short click detected." );        // for debugging
    clickMode   = 1;
  }
}


void buttonLedSetup()
{
  // Set up the long-click timer
  Timer1.initialize( 1000000 );
  Timer1.attachInterrupt( userClick );
  Timer1.stop();
  
  // Configure the pushbutton pins
  pinMode( BTN, INPUT_PULLUP );
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

