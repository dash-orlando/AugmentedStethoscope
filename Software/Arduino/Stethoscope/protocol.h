/// BT Configuration
#define         SPEED       115200
#define         BTooth      Serial1

/// ASCII Byte Codes -- used for communication protocol
#define         CHK                 0x01      // System Check: "Run system check and report"        [resp: --- | ---]
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
