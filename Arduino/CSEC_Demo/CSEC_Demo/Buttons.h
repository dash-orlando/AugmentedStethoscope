//*
#include <Bounce.h>

int mode = 0;  // state which indicates which button has been pressed

// Bounce objects to easily and reliably read the buttons
Bounce buttonRecord = Bounce( 0, 8 );
Bounce buttonStop   = Bounce( 1, 8 );  // 8 = 8 ms debounce time
Bounce buttonPlay   = Bounce( 2, 8 );
// */

//*
void longClick()
{
  unsigned int t = buttonStop.duration();
  Timer1.stop();
//  Serial.print( "click duration: " ); Serial.println( t );
  if( t > 170 )
  {
    Serial.print( "Long click detected (" );
    Serial.print( t );
    Serial.print( ")\n" );
    mode = 3;
//    filePlayer();
  }
} // */

/*
 // */

