#define DEBUG	true
#define VERSION	0.01

#include "bluetooth.h"



enum State
{
  CONNECTED, DISCONNECTED, RECORDING, PLAYING, STANDBY
};
State connectState  = DISCONNECTED;
State recordState   = STANDBY;

void setup()
{
//  buttonLedSetup();
 // if ( DEBUG )
  {
    // Start the HW serial port for console messages
    Serial.begin( 115200 );                           // for debugging
    while ( !Serial )                                 // for debugging
    Serial.print( "Setting up Bluetooth...." );
    buttonLedSetup();
    BtSetup();
    Serial.println( "done." );

    Serial.println( "//Setup complete" );             // for debugging
  }
}

void loop()
{
  buttonPress.update();                               // Get button status
  if ( buttonPress.fallingEdge() )
  {
    if ( DEBUG ) Serial.println( "click" );           // for debugging
    buttonClicked = true;                             // set flag for the logic below
    Timer1.setPeriod( 1000000 );                      // start timer to determine long-click
    Timer1.start();
    if ( DEBUG ) Serial.printf( "Mode/Hx: %d / %d\n", clickMode, clickModeHx );
  }
  if (( buttonClicked ) && ( clickMode != 0 ) )       // clickModeHx ) )
  {
    if ( DEBUG ) Serial.printf( "Mode/Hx: %d / %d\n", clickMode, clickModeHx );


    /* okay do do whatever you want to do, here */
    switch ( clickMode )
    {

      case SHORT_CLICK :
        if ( DEBUG ) Serial.println( "Button pressed - short\n" );
     /* The following executes when the controller button is short-clicked */

        if ( ( connectState == CONNECTED ) && ( recordState == STANDBY ) )
        {
          // if CONNECTED send command to BT device to START recording ("DC1-STRTREC"),
          // when acknowledged, do the following...
          recordState = RECORDING;
          digitalWrite( REC, HIGH );
        }
        else
        {
          // if CONNECTED send command to BT device to STOP recording ("DC1-STPREC"),
          // when acknowledged, do the following...
          recordState = STANDBY;
          digitalWrite( REC, LOW );
        }
        
        break;

      case LONG_CLICK :
        if ( DEBUG ) Serial.println( "Button pressed - long\n" );
      /* The following executes when the controller button is long-clicked */

        if ( connectState == DISCONNECTED )
        {
          ledState = SEARCHING;
          setLEDState( ledState );
//        BtConnect();
//        BC127Connect();
          connectState = CONNECTED;     // test code to mimic the above comment-out lines
          setLEDState( ledState );
          // Tell BT device to stop recording if it had been disconnected while previously recording.
        }
        else
        {
          //tell BT device to stop recording and get acknowledgment before disconnecting, then...
          ledState = NEGOTIATING;       // this is just to test the led mode
          setLEDState( ledState );
//        //// whatever the code is to disconnect from the BT device ////
          connectState = DISCONNECTED;  // test code to mimic the above comment-out lines
          setLEDState( ledState );
          delay( 2000 );
          stopLEDs();
          ledState = ACQUIRED;          // this is just to test the led mode
          setLEDState( ledState );
           delay( 1000 );
          stopLEDs();
          ledState = INACTIVE;          // this is just to test the led mode
          setLEDState( ledState );
       }
        break;

      default:
        break;
    }
    buttonClicked = false;
    clickModeHx   = clickMode;
    clickMode     = 0;
  }
  if ( buttonClicked )          // WTF?
  {
//    setLEDState( ledState );
  }
}


void setLEDState( int state )
{
  switch ( state )
  {
    case INACTIVE :                                   // If doing nothing with BT, turn of LEDs
      setLEDs( LOW, LOW );
      break;
    case SEARCHING   :                                // If searching for BT connection, alternate LEDs
      flashLEDs( 250000, HIGH, LOW );
      break;
    case NEGOTIATING :                                // If negotiating a BT connection, flash both LEDs
      flashLEDs( 250000, HIGH, HIGH );
      break;
    case ACQUIRED :                                   // While connected, leave both LEDs on
      setLEDs( HIGH, HIGH );
      break;
    default:                                          // Anything else... turn them off
      setLEDs( LOW, LOW );
      break;
  }
}

