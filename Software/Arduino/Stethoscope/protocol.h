/*
 * protocol.h
 * 
 * The following script serves a table for variables to be used accross all smart devices
 * 
 * Michael Xynidis
 * Fluvio L Lobo Fenoglietto
 * 10/20/2016
 */


/// BT Configuration
#define         SPEED       115200
#define         BTooth      Serial1

/// ASCII Byte Codes -- used for communication protocol
#define         CHK                 0x01          // System Check: "Run system check and report"                                [resp: --- | ---]
#define         ENQ                 0x05          // Enquiry: "Are you ready for commands?";                                    [resp: ACK | NAK]
#define         EOT                 0x04          // End of Transmission: "Done; you may disconnect."                           [resp: ACK | NAK]
#define         ACK                 0x06          // Positive Acknowledgement: "Command/Action successful."
#define         NAK                 0x15          // Negative Acknowledgement: "Command/Action UNsuccessful."
#define         CAN                 0x18          // Cancel: "Cancel current command/action.."                                  [resp: ACK | NAK]
#define         ESC                 0x1B          // Escape: "Operate in alternate mode." (reserved for future use)

#define         DC1                 0x11          // Device Command 1: Operation Functions                                      [resp: ACK | NAK]
#define         DC1_CHK             0x11,0x01     // System Check: "Run system check and report"                                [resp: --- | ---]

#define         DC2                 0x12          // Device Command 2: Communication Functions                                  [resp: ACK | NAK]
#define         DC2_SNDWAV          0x12,0x01     // Send .WAV file (audio recording) via serial port   

#define         DC3                 0x13          // Device Command 3: Device-Specific Functions                                [resp: ACK | NAK]

#define         DC4                 0x14          // Device Command 4: Simulated Output                                         [resp: ACK | NAK]



