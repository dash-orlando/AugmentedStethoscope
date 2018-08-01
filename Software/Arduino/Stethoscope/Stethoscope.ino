// ==============================================================================================================
// Stethoscope
//
// Main script
// * Created on 09/14/2016
//
// Michael Xynidis
// Fluvio L Lobo Fenoglietto 05/07/2018
// ============================================================================================================== //

// ==============================================================================================================
// Import libraries and/or modules
// ============================================================================================================== //
#include  "TeensyAudio.h"
#include  "Config.h"
#include  "states.h"
//#include  "protocol.h"
#include  "FileSD.h"
#include  "parseBtByte.h"

// ==============================================================================================================
// SETUP LOOP
// ============================================================================================================== //
void setup() {
  // Serial Communication Initialization
  Serial.begin( SPEED );                                                                                          // USB Serial Communication
  BTooth.begin( SPEED );                                                                                          // RF/Bluetooth Serial Communication

  // Setup Audio Board
  SetupAudioBoard();

  // Configuration File
  SessionInit();

  // SD Reader and Card Check
  sdCheck();

} // End of setup()

// ==============================================================================================================
// MAIN LOOP
// ============================================================================================================== //
void loop() {
  // if we get a valid byte, read analog from BT:
  if ( BTooth.available() > 0 ) parseBtByte( "RECORD.RAW" );

  // If playing or recording, carry on...
  if ( mode == 1 ) continueRecording();
  if ( mode == 2 ) continuePlaying();
  if ( mode == 3 ) continueHeartBeatMonitoring();
  if ( mode == 5 ) continueBlending( fileName );
  
  // Clear the input byte variable
  inByte = 0x00;                                // this line of code may be unnecessary
}


