/*
 * protocol.h
 * 
 * The following script serves a table for variables to be used accross all smart devices
 * 
 * Michael Xynidis
 * Fluvio L Lobo Fenoglietto
 * 01/31/2017
 */

// =================================================================================================================================== //
// Variables and Definitions
// =================================================================================================================================== //
 /// Device Identification Variables
byte STE[] = { 0x53, 0x54, 0x45, 0x00 };        // Stethoscope Device IDs

/// BT Configuration
#define         SPEED       115200
#define         BTooth      Serial1

/// ASCII Byte Codes -- used for communication protocol
// General Commands
#define         ENQ               0x05          // Enquiry: "Are you ready for commands?"                             [resp: ACK | NAK]
#define         ACK               0x06          // Positive Acknowledgement: "Command/Action successful."
#define         NAK               0x15          // Negative Acknowledgement: "Command/Action UNsuccessful."

/// Device Control Commands
//  Diagnostic Functions ============================================================================================================= //
#define         DEVICEID         0x11          // Device Identification                                              [resp: Device Code]
#define         SDCHECK           0x12          // System Check: "Run system check and report"                        [resp: ACK | NAK]
#define         SENDWAV           0x13          // Send .WAV file (audio recording) via serial port                   [resp: ACK | NAK]
#define         DELVOLATILE       0x14          // Erase volatile files (all)                                         [resp: ACK | NAK]
#define         SENDRAW           0x37          // Send .RAW file (or any file within the SD card)
#define         SETIDLE           0x26          // Set device from any state to IDLE ( mode = 0 )

//  Device-Specific Functions ======================================================================================================== //                     
#define         STARTREC          0x16          // Start Recording                                                    [resp: ACK | NAK]
#define         STARTCREC         0x32          // Start Custom Recording                                             [resp: ACK | NAK]
#define         STARTMREC         0x38          // Start Multi Channel, Custom Recording
#define         STOPREC           0x17          // Stop Recording                                                     [resp: ACK | NAK]
#define         STARTPLAY         0x18          // Start Playback                                                     [resp: ACK | NAK]
#define         STOPPLAY          0x19          // Stop Playback                                                      [resp: ACK | NAK]
#define         STARTHBMONITOR    0x1B          // Start Monitoring Heart Beat                                        [resp: ACK | NAK]
#define         STOPHBMONITOR     0x1C          // Stop Monitoring Heart Beat                                         [resp: ACK | NAK]
#define         STARTBLEND        0x1F          // Start Blending
#define         STOPBLEND         0x20          // Stop Blending
#define         PSTRING           0x31          // Parse string data                                                  [resp: ACK | NAK]
#define         RECMODE           0x41          // Parse recording mode                                               ...
#define         SETGAINS          0x44          // Set device gains 

//  Simulation Functions ============================================================================================================= //
#define         STARTSIM          0x70
#define         STOPSIM           0x71


// =================================================================================================================================== //
// Session (structure)
//
// Michael Xynidis
// Fluvio L Lobo Fenoglietto 05/07/2018
// =================================================================================================================================== //
struct Session {
    String    fileRec;
    String    filePly[ 11 ];
    int       lenPly;
    char      blendByteList[ 11 ];
    int       lenBlendByteList;
};

Session  ses;

// =================================================================================================================================== //
// Session Initialization
//
// Michael Xynidis
// Fluvio L Lobo Fenoglietto 05/07/2018
// =================================================================================================================================== ///
void SessionInit() {
    ses.fileRec     =  "RECORD.RAW";
    ses.filePly[0]  =  "AORSTE.RAW";
    ses.filePly[1]  =  "S4GALL.RAW";
    ses.filePly[2]  =  "ESMSYN.RAW";
    ses.filePly[3]  =  "KOROT1.RAW";
    ses.filePly[4]  =  "KOROT2.RAW";
    ses.filePly[5]  =  "KOROT3.RAW";
    ses.filePly[6]  =  "KOROT4.RAW";
    ses.filePly[7]  =  "RECAOR.RAW";
    ses.filePly[8]  =  "RECMIT.RAW";
    ses.filePly[9]  =  "RECPUL.RAW";
    ses.filePly[10] =  "RECTRI.RAW";

    ses.lenPly      = sizeof(ses.filePly)/sizeof(ses.filePly[0]);

    // Generate list of blend/playback bytes ==================================================================== //
    int blendOffset = 50;
    for( int i = 0; i < ses.lenPly; i ++ ) {
      ses.blendByteList[i] = (byte)(i + blendOffset);
    }

    ses.lenBlendByteList = sizeof(ses.blendByteList)/sizeof(ses.blendByteList[0]);
     
} // End of SessionInit()

