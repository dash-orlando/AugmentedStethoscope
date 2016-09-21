/// BT Configuration
#define         SPEED       115200
#define         BTooth      Serial2

/// BT Connection States
#define         BTTX                2
#define         BTRX                3
#define         INACTIVE            0
#define         SEARCHING           1
#define         NEGOTIATING         2
#define         ACQUIRED            3

/// ASCII Byte Codes -- used for communication protocol
#define         ENQ                 0x05      // Enquiry: "Are you ready for commands?";            [resp: ACK | NAK]
#define         EOT                 0x04      // End of Transmission: "Done; you may disconnect."   [resp: ACK | NAK]
#define         ACK                 0x06      // Positive Acknowledgement: "Command/Action successful."
#define         NAK                 0x15      // Negative Acknowledgement: "Command/Action UNsuccessful."
#define         CAN                 0x18      // Cancel: "Cancel current command/action.."          [resp: ACK | NAK]
#define         ESC                 0x1B      // Escape: "Operate in alternate mode." (reserved for future use)
#define         DC1_STRTREC         0x11      // Device Command 1: "Start recording."               [resp: ACK | NAK]
#define         DC2_STPREC          0x12      // Device Command 2: "Stop recording."                [resp: ACK | NAK]
#define         DC3_STRTPLY         0x13      // Device Command 3: "Start playback."                [resp: ACK | NAK]
#define         DC4_STPPLY          0x14      // Device Command 4: "Stop playback."                 [resp: ACK | NAK]


/// BT Module AT Command (subset)
const String    commandMode         = "$$$";            // 1-sec inactivity before & after; no CRLF
const String    endCommandMode      = "---";
const String    devInquiry          = "I,03,001F00";    // 0x1F00 is the Class of Device for Smirf, search 3 sec
const String    btConnect           = "C,";
const String    btDisconnect        = "K,";
const String    connectStatus       = "GK,";
const String    localMAC            = "GB";
const String    remoteMAC           = "GF";
const String    reboot              = "R,1";

const char      basicCfg            = 'D';
const char      storedCfg           = 'X';
const char      commandList         = 'H';
const char      lowPowerSleep       = 'Z';              // reset or cycle power to recover

const String    BT_CONNECTED        = "1,0,0";
const String    BT_DISCONNECTED     = "0,0,0";
const String    BT_ERROR            = "ERR";

const String    respCommandMode     = "CMD";
const String    respEndCommandMode  = "END";
const String    respBtConnect       = "TRYING";
const String    respBtDisonnect     = "KILL";
const String    respReboot          = "Reboot!";
const String    respDevInquiry      = "Inquiry Done";

/// 
struct BtRemote 
{
  String btName;
  String btMAC;
};

#define         _817D               0
#define         _81D0               0
#define         _978F               0

BtRemote        stethoscope[]       = { { "RNBT-817D", "0006667D817D" },
                                        { "RNBT-81D0", "0006667D81D0" },
                                        { "RNBT-978F", "0006667D978F" } };

int             maxRetries          = 2;
int             retries             = maxRetries;


/// Functions to set up BT as Master
String waitBtResponse( String terminator )
{
  String  btBuffer  = "";
  boolean done      = false;
  Serial.print( "Waiting for '" ); Serial.print( terminator ); Serial.println( "'." );
  while ( ! done )
  {
    if ( BTooth.available() > 0 ) btBuffer += (char)BTooth.read();
    if ( btBuffer.indexOf( terminator ) > -1 ) done = true;
  }
  Serial.print( "Got '" ); Serial.print( terminator ); Serial.println( "'.\n" );
  return btBuffer;
}

boolean btCommandMode()
{
  Serial.println( "Putting BT module into Command mode." );
  BTooth.print( '$' );                          // Send '$' three times
  BTooth.print( '$' );                          //  ...to smirf, individually
  BTooth.print( '$' );                          //   ...to enter command mode.
  if ( waitBtResponse( respCommandMode ) > "" ) // Wait until it does.
    return true;
  else
  {
    Serial.println( "unable to start Command mode" );
    return false;
  }
}

boolean btCommandMode( String bypass )
{
  Serial.println( "Putting BT module into Command mode (no response wait)." );
  BTooth.print( '$' );                          // Send '$' three times
  BTooth.print( '$' );                          //  ...to smirf, individually
  BTooth.print( '$' );                          //   ...to enter command mode.
  if ( bypass > "" )
    return true;
  else
    return false;
}


boolean pairWithBtDevice( String btDevice )
{
  String  response  = "";
  String  command   = btConnect;
  int     ndx       = -1;

  Serial.print( "Inquiring about BT MAC [" ); Serial.print( btDevice ); Serial.println( "]." );
  BTooth.println( devInquiry );
  response  = waitBtResponse( respDevInquiry );
  ndx       = response.indexOf( btDevice );
  if ( ndx > -1 )
  {
    command += btDevice; 
    Serial.print( "Sending command [" ); Serial.print( command ); Serial.println( "]." );
    BTooth.println( command );
    if ( waitBtResponse( respBtConnect ) > "" )
      return true;
    else
      return false;
  }
  pairWithBtDevice( btDevice );   // added...may cause looping
  return true;
}

