/*
 * protocol.h
 * 
 * The following script serves a table for variables to be used accross all smart devices
 * 
 * Michael Xynidis
 * Fluvio L Lobo Fenoglietto
 * 10/20/2016
 */

/// Device Identification Variables
byte STE[] = {0x53,0x54,0x45};                    // Stethoscope

/// BT Configuration
#define         SPEED       115200
#define         BTooth      Serial1

/// ASCII Byte Codes -- used for communication protocol
// General Commands
#define         ENQ                 0x05          // Enquiry: "Are you ready for commands?";                                    [resp: ACK | NAK]
#define         EOT                 0x04          // End of Transmission: "Done; you may disconnect."                           [resp: ACK | NAK]
#define         ACK                 0x06          // Positive Acknowledgement: "Command/Action successful."
#define         NAK                 0x15          // Negative Acknowledgement: "Command/Action UNsuccessful."
#define         CAN                 0x18          // Cancel: "Cancel current command/action.."                                  [resp: ACK | NAK]

// Device Control Commands
#define         DC1                 0x11          // Device Control 1: Diagnostic Functions                                     [resp: ACK | NAK]
#define         DC1_DEVICEID        0x00          //                   Device Identification                                    [resp: Device Code]
#define         DC1_SDCHECK         0x01          //                   System Check: "Run system check and report"              [resp: ACK | NAK]
//                                  0xFF          (255 DEC)

#define         DC2                 0x12          // Device Control 2: Operational Functions                                    [resp: ACK | NAK]
#define         DC2_SENDWAV         0x00          //                   Send .WAV file (audio recording) via serial port         [resp: ACK | NAK]
#define         DC2_DELVOLATILE     0x01          //                   Erase volatile files (all)                               [resp: ACK | NAK]                     
//                                  0xFF          (255 DEC)

#define         DC3                 0x13          // Device Control 3: Device-Specific Functions                                [resp: ACK | NAK]
#define         DC3_STARTREC        0x00          //                   Start Recording                                          [resp: ACK | NAK]
#define         DC3_STOPREC         0x01          //                   Stop Recording                                           [resp: ACK | NAK]
#define         DC3_STARTPLAY       0x02          //                   Start Playback                                           [resp: ACK | NAK]
#define         DC3_STOPPLAY        0x03          //                   Stop Playback                                            [resp: ACK | NAK]
#define         DC3_STARTSTREAM     0x04          //                   Start Microphone Stream                                  [resp: ACK | NAK]
#define         DC3_STARTTRACKING   0x05          //                   Start Tracking Microphone Stream for Peaks               [resp: ACK | NAK]
#define         DC3_STOPTRACKING    0x06          //                   Stop Tracking Microphone Stream for Peaks               [resp: ACK | NAK]
//                                  0xFF          (255 DEC)

#define         DC4                 0x14          // Device Control 4: Simulation Functions                                     [resp: ACK | NAK]
#define         DC4_NORMALHB        0x00          //                   Playback of Normal Heart Beat                            [resp: ACK | NAK]
#define         DC4_ESHMURMUR       0x01          //                   Playback of Early Systolic Heart Murmur                  [resp: ACK | NAK]
//                                  0xFF          (255 DEC)



