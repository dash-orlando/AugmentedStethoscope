/*
 * protocol.h
 * 
 * The following script serves a table for variables to be used accross all smart devices
 * 
 * Michael Xynidis
 * Fluvio L Lobo Fenoglietto
 * 01/31/2017
 */

/// Device Identification Variables
byte STE[] = {0x53,0x54,0x45};                    // Stethoscope

/// BT Configuration
#define         SPEED       115200
#define         BTooth      Serial1

/// ASCII Byte Codes -- used for communication protocol
// General Commands
#define         ENQ                 0x05          // Enquiry: "Are you ready for commands?";                                    [resp: ACK | NAK]
#define         ACK                 0x06          // Positive Acknowledgement: "Command/Action successful."
#define         NAK                 0x15          // Negative Acknowledgement: "Command/Action UNsuccessful."

// Device Control Commands
#define         DEVICEID        	0x11          // Device Identification                                    					[resp: Device Code]
#define         SDCHECK         	0x12          // System Check: "Run system check and report"              					[resp: ACK | NAK]
#define         SENDWAV         	0x13          // Send .WAV file (audio recording) via serial port         					[resp: ACK | NAK]
#define         DELVOLATILE     	0x14          // Erase volatile files (all)                               					[resp: ACK | NAK]                     
#define         STARTREC        	0x16          // Start Recording                                          					[resp: ACK | NAK]
#define         STOPREC         	0x17          // Stop Recording                                           					[resp: ACK | NAK]
#define         STARTPLAY       	0x18          // Start Playback                                           					[resp: ACK | NAK]
#define         STOPPLAY        	0x19          // Stop Playback                                            					[resp: ACK | NAK]
#define         STARTPASSTHRU   	0x1A          // Start Audio passthrough from mic to ear monitors         					[resp: ACK | NAK]
#define         STARTTRACKING   	0x1B          // Start Tracking Microphone Stream for Peaks               					[resp: ACK | NAK]
#define         STOPTRACKING    	0x1C          // Stop Tracking Microphone Stream for Peaks                					[resp: ACK | NAK]
#define         NORMALHB        	0x1D          // Playback of Normal Heart Beat                            					[resp: ACK | NAK]
#define         ESHMURMUR       	0x1E          // Playback of Early Systolic Heart Murmur                  					[resp: ACK | NAK]



