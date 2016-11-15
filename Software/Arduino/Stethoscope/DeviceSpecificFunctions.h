/*
 * DeviceSpecificFunctions.h
 *
 * The following script comprises all of the functions used for the operation of the device and are also unique to such device
 * 
 * Device: Stethoscope
 *
 * Michael Xynidis
 * Fluvio Lobo Fenoglietto
 * 10/27/2016
 */
 
//
// *** Variables
//

File          frec;
File          hRate;

elapsedMillis msecs;
elapsedMillis triggered;
elapsedMillis elapsed;
elapsedMillis timeStamp;

boolean       beat          = false;
boolean       captured      = false;

int           ndx           = 0;
static int    heartRate     = 0;
//const int     myInput       = AUDIO_INPUT_MIC;
unsigned int  hrSample[3]   = { 0, 0, 0 };

String        lineOut       = "";

//
// *** Adjust Microphone Gain Level
//
void adjustMicLevel()
{
  // TODO: read the peak1 object and adjust sgtl5000_1.micGain()
  // if anyone gets this working, please submit a github pull request :-)
}

//
// *** Wave Amplitude Peaks
//
void waveAmplitudePeaks()
{
  float   vol         = analogRead( 15 ) / 1024.0;
  float   sensitivity = 1.0 - vol;

  if ( msecs > 40 )
  {
    if ( peak1.available() )
    {
      msecs             = 0;
      float peakNumber  = 0.0;

      peakNumber        = peak1.read();

      int   leftPeak    = peakNumber  * 30.0;
      int   count;
      char  fillChar    = ' ';

      for ( count = 0; count < 30 - leftPeak; count++ )
        Serial.print( " " );

      if ( peakNumber >= sensitivity )
      {
        fillChar  = '<';
        triggered = 0;
        beat      = false;
        captured  = false;
      }
      else
      {
        fillChar = ' ';
        if ( ( triggered > 250 ) && ( !captured ) )    // this interval needs to be a calculated value....
        {
          beat      = true;
          captured  = true;
          triggered = 0;
          hrSample[ndx] = elapsed;  // - timestamp;
          if ( ndx == 2 )
          {
            heartRate = 60000 / ((hrSample[0] + hrSample[1] + hrSample[2]) / 3);
            for ( int i = 0; i < 3; i++ ) hrSample[i] = 0;
            ndx = 0;
          }
          else ndx++;
          elapsed   = 0;
        }
        else beat   = false;
      }
 
      while ( count++ < 30 )
        Serial.print( fillChar );

      Serial.print( "||" );

      if ( peakNumber >= sensitivity )
        fillChar = '>';
      else
        fillChar = ' ';
 
      for ( count = 0; count < leftPeak; count++ )
        Serial.print( fillChar );

      while ( count++ < 30 )
        Serial.print( " " );
 
      if ( beat ) Serial.print( "* " );
      Serial.print( "Sens: " );
      Serial.print( vol );
      Serial.print( "\tHR: " );
      Serial.println( heartRate );
    }
  }
} // End of waveAmplitudePeaks()

//
// *** Start Recording
//
boolean startRecording()
{
  Serial.println( "EXECUTING startRecording()" );                                                               // Identification of function executed
  
  char  fileRec[ses.fileRec.length()+1];                                                                        // Conversion from string to character array
  ses.fileRec.toCharArray( fileRec, sizeof( fileRec ) );

  char  fileDat[ses.fileDat.length()+1];                                                                        // Conversion from string to character array
  ses.fileDat.toCharArray( fileDat, sizeof( fileDat ) );
  
  if ( SD.exists( fileRec ) )                                                                                   // Check for existence of RECORD.RAW
  {
    SD.remove( fileRec );                                                                                       // If found, delete RECORD.RAW
  }
  if ( SD.exists( fileDat ) )                                                                                   // Check for existence of HRATE.DAT
  {
    SD.remove( fileDat );                                                                                       // If found, delete HRATE.DAT
  }
  frec  = SD.open( fileRec, FILE_WRITE );                                                                       // Create and open RECORD.RAW file
  Serial.println( frec );
  hRate = SD.open( fileDat,  FILE_WRITE );                                                                      // Create and open HRATE.DAT file
  Serial.println( hRate );
  if ( frec )
  {
    queue1.begin();
    recordState = RECORDING;
    mode = 1;                                                                                                   // Change value of operation mode for continous recording
    timeStamp   = 0;
    Serial.println( "Stethoscope Began RECORDING" );                                                            // Function execution confirmation over USB serial
    BTooth.write( ACK );                                                                                        // ACKnowledgement sent back through bluetooth serial
    return true;
  }
  else
    Serial.println( "Stethoscope CANNOT Begin RECORDING" );                                                     // Function execution failed, notification over USB serial
    BTooth.write( NAK );                                                                                        // Negative AcKnowledgement sent back through bluetooth serial
    return false;
}

//
// *** Continue Recording
//
void continueRecording()
{
  if ( queue1.available() >= 2 )
  {
    byte buffer[512];                                                                                           // Fetch 2 blocks from the audio library and copy into a 512 byte buffer.
                                                                                                                // The Arduino SD library is most efficient when full 512 byte sector size writes are used.
    memcpy( buffer, queue1.readBuffer(), 256 );
    queue1.freeBuffer();
    memcpy( buffer + 256, queue1.readBuffer(), 256 );
    queue1.freeBuffer();                                                                                        // write all 512 bytes to the SD card
    frec.write( buffer, 512 );
    waveAmplitudePeaks();                                                                                       // write HR and time to file at each heart beat
    if ( beat )
    {
      lineOut = String( heartRate, DEC ) + "," + String( timeStamp, DEC ) + "\r\n";
      hRate.print( lineOut );
    }
  }
} // End of continueRecording()

//
// *** Stop Recording
//
boolean stopRecording()
{
  Serial.println( "stopRecording" );
  queue1.end();
  if ( recordState == RECORDING )
  {
    while ( queue1.available() > 0 )
    {
      frec.write( (byte*)queue1.readBuffer(), 256 );
      queue1.freeBuffer();
    }
    frec.close();
    hRate.close();
  }
  recordState = STANDBY;
  mode = 0;                                                                                                     // Change operation mode to normal operation or idle
  return true;
}

//
// *** Start Playing
//
boolean startPlaying(String fileName)
{
  Serial.println( "EXECUTING startPlaying()" );                                                                 // Identification of function executed

  mixer2.gain(0,0.0);                                                                                           // Set the microphone channel 0 to mute (gain value = 0)
  mixer2.gain(1,0.0);                                                                                           // Set the microphone channel 1 to mute (gain value = 0)
  mixer2.gain(2,1);                                                                                           // Set the gain of the playback audio signal

  char  filePly[fileName.length()+1];                                                                           // Conversion from string to character array
  fileName.toCharArray( filePly, sizeof( filePly ) );
  
  if ( SD.exists( filePly ) )
  {
    playRaw1.play( filePly );
    recordState = PLAYING;
    mode = 2;                                                                                                   // Change operation mode to continue playing audio
    Serial.println( "Stethoscope Began PLAYING" );                                                              // Function execution confirmation over USB serial
    BTooth.write( ACK );                                                                                        // ACKnowledgement sent back through bluetooth serial
    return true;
  }
  else
    Serial.println( "Stethoscope CANNOT begin PLAYING" );                                                       // Function execution confirmation over USB serial
    BTooth.write( NAK );                                                                                        // Negative AcKnowledgement sent back through bluetooth serial
    return false;
}

//
// *** Continue Playing
//
void continuePlaying() 
{
  if ( !playRaw1.isPlaying() )
  {
    playRaw1.stop();
  }
}

//
// *** Stop Playing
//
boolean stopPlaying()
{
  Serial.println( "stopPlaying" );
  if ( recordState == PLAYING ) playRaw1.stop();
  recordState = STANDBY;
  mode = 0;                                                                                                     // Change operation mode to normal operation or idle
  return true;
}


//
// *** Start Microphone Stream
//     This function re-configures the stethoscope do stream data from the microphone to the speakers
//
boolean startMicStream()
{
  Serial.println( "EXECUTING startMicStream()" );
  if ( recordState == RECORDING ) stopRecording();                                                              // Stop recording if recording
  if ( recordState == PLAYING ) stopPlaying();                                                                  // Stop playback if playing

  if ( myInput == AUDIO_INPUT_MIC )
  {
    mixer1.gain(0,0);                                                                                           // Set gain of mixer1, channel0 to 0
    mixer1.gain(1,0);                                                                                           // Set gain of mixer1, channel1 to 1
    mixer2.gain(0,0.5);                                                                                         // Set gain of mixer2, channel0 to 0.5 - Microphone on
    mixer2.gain(1,0.5);                                                                                         // Set gain of mixer2, channel0 to 0.5 - Microphone on
    mixer2.gain(2,0);                                                                                           // Set gain of mixer2, channel2 to 0
    recordState = STREAMING;
    mode = 0;                                                                                                   // Change operation mode to continue streaming audio
    Serial.println( "Stethoscope Began STREAMING" );                                                            // Function execution confirmation over USB serial
    BTooth.write( ACK );                                                                                        // ACKnowledgement sent back through bluetooth serial
    return true;
  }
  else
  {
    Serial.println( "Stethoscope CANNOT begin STREAMING" );                                                     // Function execution confirmation over USB serial
    BTooth.write( NAK );                                                                                        // Negative AcKnowledgement sent back through bluetooth serial
    return false;
  }
} // End of startMicStream()

//
// *** Continue Microphone Stream
//
boolean continueMicStream()
{
  mixer1.gain(0,0);                                                                                             // Set gain of mixer1, channel0 to 0
  mixer1.gain(1,0);                                                                                             // Set gain of mixer1, channel1 to 1
  mixer2.gain(0,0.5);                                                                                           // Set gain of mixer2, channel0 to 0.5 - Microphone on
  mixer2.gain(1,0.5);                                                                                           // Set gain of mixer2, channel0 to 0.5 - Microphone on
  mixer2.gain(2,0);                                                                                             // Set gain of mixer2, channel2 to 0
  recordState = STREAMING;
  return true;
  
} // End of continueMicStream()

//
// *** Tracking Microphone Stream for Peaks
//     This function uses the wave peak detection tool to find and track peaks within the microphone stream
//     The function does not record data to a file, rather displays the information through the serial ports
//     Note that the function acts as an alternative to startMicStream()
//
boolean trackingMicStream()
{
  Serial.println( "EXECUTING trackingMicStream()" );
  if ( recordState == RECORDING ) stopRecording();                                                              // Stop recording if recording
  if ( recordState == PLAYING ) stopPlaying();                                                                  // Stop playback if playing

  if ( myInput == AUDIO_INPUT_MIC )
  {
    mixer1.gain(0,0);                                                                                           // Set gain of mixer1, channel0 to 0
    mixer1.gain(1,0);                                                                                           // Set gain of mixer1, channel1 to 1
    mixer2.gain(0,0.5);                                                                                         // Set gain of mixer2, channel0 to 0.5 - Microphone on
    mixer2.gain(1,0.5);                                                                                         // Set gain of mixer2, channel0 to 0.5 - Microphone on
    mixer2.gain(2,0);                                                                                           // Set gain of mixer2, channel2 to 0
    queue2.begin();
    recordState = STREAMING;
    mode = 3;                                                                                                   // Change operation mode to continue streaming audio
    Serial.println( "Stethoscope Began TRACKING MIC STREAM" );                                                  // Function execution confirmation over USB serial
    BTooth.write( ACK );                                                                                        // ACKnowledgement sent back through bluetooth serial
    return true;
  }
  else
  {
    Serial.println( "Stethoscope CANNOT begin TRACKING MIC STREAM" );                                           // Function execution confirmation over USB serial
    BTooth.write( NAK );                                                                                        // Negative AcKnowledgement sent back through bluetooth serial
    return false;
  }
} // End of trackingMicStream()

//
// *** Continue Tracking Microphone Stream
//     This is the companion function to trackingMicStream()
//     The function continues the tracking of audio peaks
//
boolean continueTrackingMicStream()
{
  Serial.println( "EXECUTING continueTrackingMicStream()" );                                                    // Terminal message indicating function execution
  if ( queue2.available() >= 2 )
  {
    byte buffer[512];                                                                                           // Fetch 2 blocks from the audio library and copy into a 512 byte buffer.
                                                                                                                // The Arduino SD library is most efficient when full 512 byte sector size writes are used.
    memcpy( buffer, queue2.readBuffer(), 256 );
    queue2.freeBuffer();
    memcpy( buffer + 256, queue2.readBuffer(), 256 );
    queue2.freeBuffer();                                                                                        // write all 512 bytes to the SD card
    frec.write( buffer, 512 );
    waveAmplitudePeaks();                                                                                       // write HR and time to file at each heart beat
    if ( beat )
    {
      lineOut = String( heartRate, DEC ) + "," + String( timeStamp, DEC ) + "\r\n";
      Serial.println( lineOut );
    }
    return true;
  }
  return false;
} // End of continueTrackingMicStream()

