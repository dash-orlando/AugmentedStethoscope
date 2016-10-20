#include  <Wire.h>
#include  <TimerOne.h>
#include  <LiquidCrystal_I2C.h>
#include  <SPI.h>
#include  "TeensyAudio.h"
#include  "LcdFunctions.h"
#include  "Buttons.h"
#include  "FileSD.h"
#include  "SignalProcessing.h"

//const int myInput = AUDIO_INPUT_LINEIN;
const int myInput = AUDIO_INPUT_MIC;

void setup()
{
  // Set up the double-click timer
  Timer1.initialize( 1000000 );
  Timer1.attachInterrupt( longClick );
  Timer1.stop();
  
  // Configure the pushbutton pins
  pinMode( 0, INPUT_PULLUP );
  pinMode( 1, INPUT_PULLUP );
  pinMode( 2, INPUT_PULLUP );

  // Audio connections require memory, and the record queue
  // uses this memory to buffer incoming audio.
  AudioMemory( 60 );

  // Enable the audio shield, select input, and enable output
  sgtl5000_1.enable();
  sgtl5000_1.inputSelect( myInput );
  sgtl5000_1.volume( 1.0 );

 // Configure SPI for the audio shield pins
  SPI.setMOSI( 7 );  // Audio shield has MOSI on pin 7
  SPI.setSCK( 14 );  // Audio shield has SCK on pin 14

  initLCD();
  
  boboPrint( 0, 0, "Audio Function Demo" );
  boboPrint( 6, 1, "this" );
  boboPrint( 10, 2, "is" );
  boboPrint( 0, 3, " A RAPID development" );

  // wait for the Arduino Serial Monitor to open
//  while ( !Serial ) ;
  delay( 50 );

  Serial.begin( 15200 );
  sdCardCheck();

  rootDir = SD.open( "/" );
  printDirectory( rootDir, 1 );
  delay( 2000 );

  lcd.clear();
  boboPrint(  0, 0, " Recorder Function " );
  boboPrint(  0, 2, "Record"              );
  boboPrint(  7, 2, "Stop"                );
  boboPrint( 12, 2, "Playback"            );
}

//void loop(){}

int           backlightState  =  LOW;
unsigned long previousMillis  =    0;
unsigned long interval        = 1000;

void loop()
{ 
  buttonRecord.update();
  buttonStop.update();
  buttonPlay.update();

  if ( mode == 0 ) boboPrint( 0, 3, "      >stopped<     " );

  if ( buttonRecord.fallingEdge() )       // Respond to button presses
  {
    Serial.println( "'Record' button pressed" );
    boboPrint( 0, 3, "     >recording<    " );
    if ( mode == 2 ) stopPlaying();
    if ( mode == 0 ) startRecording();
  }
  if ( buttonStop.fallingEdge() )
  {
    Serial.println( "'Stop' button pressed" );
    boboPrint( 0, 3, "      >stopped<     " );
    if ( mode == 1 ) stopRecording();
    if ( mode == 2 ) stopPlaying();
    Timer1.setPeriod( 1000000 );
    Timer1.start();
  }
  if ( buttonPlay.fallingEdge() )
  {
    Serial.println( "'Play' button pressed" );
    boboPrint( 0, 3, "      >playing<     " );
    if ( mode == 1 ) stopRecording();
    if ( mode == 0 ) startPlaying();
  }

  // If playing or recording, carry on...
  if ( mode == 1 )
  {
    continueRecording();
  }
  if ( mode == 2 )
  {
    continuePlaying();
  }
  if ( mode == 3 )
  {
    filePlayer();
    lcd.clear();
    boboPrint(  0, 0, " Recorder Function " );
    boboPrint(  0, 2, "Record"              );
    boboPrint(  7, 2, "Stop"                );
    boboPrint( 12, 2, "Playback"            );
  }
  if ( myInput == AUDIO_INPUT_MIC ) adjustMicLevel();
}

