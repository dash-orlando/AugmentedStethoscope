#include        "buttonsAndLeds.h"
#include        <SparkFunbc127.h>

/// BT Connection States
//#define         BTTX        2 // just pins
//#define         BTRX        3 // just pins
#define         INACTIVE    0
#define         SEARCHING   1
#define         NEGOTIATING 2
#define         ACQUIRED    3

/// ASCII Byte Codes -- used for communication protocol
#define         ENQ         0x85  // '…'  "&hellip;"     -- Enquiry: "Are you ready for commands?";            [resp: ACK | NAK]
#define         EOT         0x84  // '„'   "&bdquo;"      -- End of Transmission: "Done; you may disconnect."   [resp: ACK | NAK]
#define         ACK         0x86  // '†'   "&dagger;"     -- Positive Acknowledgement: "Command/Action successful."
#define         NAK         0x95  // '•'   "&bull;"       -- Negative Acknowledgement: "Command/Action UNsuccessful."
#define         CAN         0x98  // '˜'   "&tilde;"      -- Cancel: "Cancel current command/action.."          [resp: ACK | NAK]
#define         ESC         0x9B  // '›'   "&rsaquo;"     -- Escape: "Operate in alternate mode." (reserved for future use)
#define         DC1_STRTREC 0x91  // '‘'   "&lsquo;"      -- Device Command 1: "Start recording."               [resp: ACK | NAK]
#define         DC2_STPREC  0x92  // '’'   "&rsquo;"      -- Device Command 2: "Stop recording."                [resp: ACK | NAK]
#define         DC3_STRTPLY 0x93  // '“'   "&ldquo;"      -- Device Command 3: "Start playback."                [resp: ACK | NAK]
#define         DC4_STPPLY  0x94  // '”'   "&rdquo;"      -- Device Command 4: "Stop playback."                 [resp: ACK | NAK]


BC127           BTModu( &Serial2 );                 // Bluetooth to SW serial port


// Useful function which identifies a local BC127 module and connects to it.
int BC127Connect()
{
  int connectionResult = BC127::REMOTE_ERROR;       // Our return value. Assume failure.
  BTModu.inquiry( 10 );                             // Seek local devices for 13 seconds.
  String address;                                   // Buffer for identified addresses

  for ( byte i = 0; i < 5; i++ )                    // Scan through (up to 5) found addresses
  {
    if ( BTModu.getAddress( i, address ) )          // If address exists at index i...
    {
      // ...check to see if it's a BC127 and if it IS...
      if ( address.startsWith( "20FABB" ) )         // If it complies with a BC127 address...
      {                                             // ...attempt to connet with it.
        connectionResult = BTModu.connect( address, BC127::SPP );
        break;                                      // Reject remaining addresses.
      }
    }
  }
  // Okay, hopefully, by now we've found and connected to a BC127. If not, return
  //  an error...
  if (connectionResult != BC127::SUCCESS) return connectionResult;
  // ...but, if so, we want to try to enter data mode.
  else connectionResult = BTModu.enterDataMode();
  return connectionResult;
}


void BtSetup()
{
  Serial2.begin( 115200 );
  delay( 100 );
//  BTModu.exitDataMode();                            // Just in case...or other commands may not work
//  BTModu.restore();
//  BTModu.writeConfig();
//  BTModu.reset();
  Serial.print("Exit mode result: "); Serial.println(BTModu.exitDataMode());
  Serial.print("Restore result: "); Serial.println(BTModu.restore());
  Serial.print("Write result: "); Serial.println(BTModu.writeConfig());
  Serial.print("Reset result: "); Serial.println(BTModu.reset());
}


void BtConnect()
{
  String buffer;                                    // For capturing data from the BT module

  while (1)
  {
    // We need to buffer our incoming serial data...
    if ( Serial2.available() > 0 ) buffer.concat( (char)Serial2.read() );

    if ( buffer.endsWith("\r") )                    // check if it's a full line from the serial port
    {
      if ( buffer.startsWith("OPEN_OK SPP") )       // check for connection message
      {
        BTModu.enterDataMode();
        Serial.println( "Connected to Bluetooth" );
        break;                                      // Exit the while loop.
      }
      buffer = "";                                  // Otherwise, clear the buffer and continue waiting.
    }
    //////////////////////////////////////////////////////////////////////////////
    // This next bit is the push button polling section.  Note that once the
    //  button has been pressed, it's quite a long time (10+ seconds) before
    //  returning to the above section.
//    buttonPress.update();                         // Get button status
//    if ( buttonPress.fallingEdge() )      /       / Respond to button presses
    {
      Serial.println( "Attempting to connect..." );
      if ( BC127Connect() == BC127::SUCCESS ) 
      {
        Serial.println( "Connection successful" );
        break;
      }
      else Serial.println( "Connection failed" );
     }
  } // */
	
}



