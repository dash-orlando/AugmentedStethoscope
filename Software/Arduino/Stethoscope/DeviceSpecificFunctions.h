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

 
#include "SerialFrame.h"

//
// *** Variables
//

SerialFrame sf1 = SerialFrame();

Frame txFr = (Frame)
  {
    123,
    STETHOSCOPE,
    HEARTRATE,
    "0"
  };

File          frec;
File          hRate;

elapsedMillis msecs;
elapsedMillis triggerTime;
elapsedMillis elapsed;
elapsedMillis timeStamp;
elapsedMillis restTime;

int           ndx           = 0;
static int    heartRateI    = 0;
unsigned int  hrSample[3]   = { 0, 0, 0 };
unsigned int  intervalRate  = 0;
unsigned int  minS1S2       = 120;                      // must be greater-than 100ms
unsigned int  maxS1S2       = 350;                      // must be less-than 400ms
unsigned int  maxHBInterval = 5000;                     //ms

float         sigThreshold  = 0.30;

bool          beatHeard     = false;
bool          atRest        = true;
bool          transition    = atRest;
bool          soundTwo      = false;

String        lineOut       = "";


//
// *** Adjust Microphone Gain Level
//
void adjustMicLevel()
{
  // TODO: read the peak_QrsMeter1 object and adjust sgtl5000_1.micGain()
  // if anyone gets this working, please submit a github pull request :-)
}


// ==============================================================================================================
// Wave Amplitude Peaks
// Amplitude Peak Detection and Heart Rate Approximation
//
// The following function uses an amplitude peak detection
// tool to approximate heart-rate
// 
// Michael Xynidis
// Fluvio L. Lobo Fenoglietto 11/12/2017
// ==============================================================================================================
bool waveAmplitudePeaks()
{
  if ( msecs > 40 )                                     // sample at 25MHz
  {
     msecs = 0;
    if ( peak_QrsMeter.available() )
    {
      float peakNumber  = 0.0;

      peakNumber = peak_QrsMeter.read();

      int   leftPeak    = peakNumber  * 30.0;
      int   count;
      char  fillCharL   = ' ';
      char  fillCharR   = ' ';

      Serial.print( "Threshold: " );
      Serial.print( peakNumber );
      Serial.print( "/" );
      Serial.print( sigThreshold );
      Serial.print( " Peak: " );
      Serial.print( leftPeak );
      Serial.print( '\t' );

      for ( int i = 0; i < 30 - leftPeak; i++ )
      {
         if ( i == (30 - (sigThreshold * 30)) )
          Serial.print( "." );
        else
          Serial.print( " " );
      }

      if ( peakNumber >= sigThreshold )                 // Sound is heard.
      {
        fillCharL = '<';
        fillCharR = '>';
        if ( atRest )                                   // If heart is at rest, then a sound is heard...
          transition  = true;                           // ....there is a 'transition',
        else                                            // otherwise...
          transition  = false;                          // ....there is no transition
        atRest        = false;                          // ....but with either event, the heart is NOT at rest.
      }
      else                                              // No sound heard (or, below threshold)
      {
        fillCharL = ' ';
        fillCharR = ' ';
        if ( !atRest )                                  // If heart was NOT at rest, then NO sound is heard...
          transition  = true;                           // ....there is a 'transition',
        else                                            // otherwise...
          transition  = false;                          // ....there is no transition
        atRest        = true;                           // ....but with either event, the heart is presently at rest.
      }


      if ( transition && !atRest && !soundTwo )         // transitioning into (potential) 1st heart sound
      {
        if ( restTime > minS1S2 )
        {
          intervalRate  = triggerTime;
          triggerTime   = 0;
        }
      }

      else if ( !transition && !atRest && !soundTwo )   // within (potential) 1st heart sound *(may be unnecessary)*
      {
      }

      else if ( transition && atRest && !soundTwo )     // transitioning out of (potential) 1st heart sound
      {
        soundTwo      = true;
        restTime      = 0;
      }

      else if ( transition && !atRest && soundTwo )     // transitioning into (potential) 2nd heart sound
      {
        if ( ( triggerTime >= minS1S2 )                 // qualifies as 2nd heart sound if S1S2 interval falls...
          && ( triggerTime <= maxS1S2 ) )               // ....within the defined range
        {
          beatHeard     = true;
        }
      }

      else if ( !transition && !atRest && soundTwo )    // within (potential) 1st heart sound *(may be unnecessary)*
      {
        beatHeard     = false;
      }

      else if ( transition && atRest && soundTwo )      // transitioning out of (potential) 2nd heart sound
      {
        soundTwo      = false;
        beatHeard     = false;
        intervalRate  = 0;
      }

      else if ( !transition && atRest && restTime > maxS1S2 )
      {
        soundTwo = false;
      }
      
      else
        beatHeard     = false;


      if ( beatHeard )                                  // This section calculates a running average...
      {                                                 // heart rate from three successive samples
        hrSample[ndx] = elapsed;  // - timestamp;
        if ( ndx == 2 )
        {
          heartRateI = 60000 / ((hrSample[0] + hrSample[1] + hrSample[2]) / 3);
          for ( int i = 0; i < 3; i++ ) hrSample[i] = 0;
          ndx = 0;
        }
        else ndx++;
        elapsed = 0;
      }
//      else if ( elapsed > maxHBInterval )               // If no heartbeat is detected within this...
//        heartRateI = 0;                                 // ....interval, heart rate is set to zero.

      for ( int i = 0; i < leftPeak; i++ )
        Serial.print( fillCharL );

      if ( beatHeard )
        Serial.print( "|*|" );
      else if ( transition )
        Serial.print( "|.|" );
      else if ( atRest )
        Serial.print( "| |" );
      else
        Serial.print( "|-|" );

      for ( count = 0; count < leftPeak; count++ )
        Serial.print( fillCharR );

      while ( count++ <= 30 )
      {
         if ( count == sigThreshold * 30 )
          Serial.print( "." );
        else
          Serial.print( " " );
      }

      if ( transition )
      {
        Serial.print( "\ttime: " );
        Serial.print( triggerTime );
      }
      if ( beatHeard )
      {
        Serial.print( "\tHR: " );
        Serial.print( heartRateI );
      }
      Serial.println();
    }
  }
  return beatHeard;
} // End of waveAmplitudePeaks()

// ==============================================================================================================
// Wave Amplitude Peaks - Heart Beat Monitoring
// Amplitude Peak Detection and Heart Rate Approximation
//
// The following function uses an amplitude peak detection
// tool to approximate heart-rate
// 
// Fluvio L. Lobo Fenoglietto 11/13/2017
// ==============================================================================================================
uint8_t       first_peak;
uint8_t       second_peak;
uint8_t       peak_tolerance = 3;
int           i              = 0;
uint8_t       peaks[2]       = {0,0};
uint8_t       t[2]           = {0,0};
uint8_t       hr             = 0;
elapsedMillis elapsed_time;
void waveAmplitudePeaks2()
{
  if( fps > 24 )
  {
    if (   peak_QrsMeter.available() )
    {
      fps = 0;
      uint8_t micPeak = mic_peaks.read()  * 30.0;

      if ( peaks[0] == 0 )
      {
        peaks[0] = micPeak;
      }
      else if ( micPeak > (peaks[0]+4) )
      {
        peaks[0] = micPeak;
        //elapsedMillis elapsed_time;
      }
      else if ( micPeak < (peaks[0]+4) )
      {
        //elapsedMillis elapsed_time;
      }
      else if ( micPeak == peaks[0] )
      {
        Serial.println("hello");
        peaks[1]  = micPeak;
        t[1]      = elapsed_time;
      }

      hr = 60000*(1/(t[1])); 

      for ( cnt = 0; cnt < 30 - micPeak; cnt++ ) Serial.print( " "  );
      while ( cnt++ < 30 )                       Serial.print( "="  );
                                                 Serial.print( "||" );
      Serial.print("Mic. Peak = ");
      Serial.print(micPeak);
      Serial.print(" | Peak[0] = ");
      Serial.print(peaks[0]);
      Serial.print(" | Peak[1] = ");
      Serial.print(peaks[1]);
      Serial.print(" | Time[0] = ");
      Serial.print(t[0]);
      Serial.print(" | Time[1] = ");
      Serial.print(t[1]);
      Serial.print(" | HR = ");
      Serial.println(hr);

    }
  }
} // End of waveAmplitudePeaks2()

// ==============================================================================================================
// RMS, Amplituide Peaks - Single
// RMS and Amplitude Peak detection based on the input
// microphone
//
// ...
// 
// Fluvio L. Lobo Fenoglietto 11/12/2017
// ==============================================================================================================
void rmsAmplitudePeaksSingle()
{
  if( fps > 24 )
  {
    // only the microphone
    if (   mic_peaks.available()
        && mic_rms.available() )
    {
      fps = 0;
      uint8_t micPeak = mic_peaks.read()  * 30.0;
      uint8_t micRMS  = 0; //mic_rms.read()    * 30.0;
      float micRMSval = mic_rms.read();                               //... it is possible to make these values into floats

      for ( cnt = 0; cnt < 30 - micPeak; cnt++ ) Serial.print( " "  );
      while ( cnt++ < 29 && cnt < 30-micRMS )    Serial.print( "<"  );
      while ( cnt++ < 30 )                       Serial.print( "="  );
                                                 Serial.print( "||" );
      Serial.printf( "       | Mic. Peak = %d", micPeak);
      Serial.print(" | Mic. RMS = ");
      Serial.println(micRMSval);
    }
  }
} // End of rmsAmplitudePeaksSingle()
// ==============================================================================================================

// ==============================================================================================================
// RMS, Amplituide Peaks
// RMS and Amplitude Peak detection based on the input
// microphone and playback data
//
// ...
// 
// Fluvio L. Lobo Fenoglietto 11/12/2017
// ==============================================================================================================
uint32_t count;
uint8_t rmsAmplitudePeaksDuo()
{
  
  uint8_t returnValue = 0;
  uint8_t threshRMS   = 0;

  if( fps > 24 )
  {
    // both microphone and the play_raw module
    if (   mic_peaks.available() 
        && mic_rms.available() 
        && playRaw_peaks.available() 
        && playRaw_rms.available() )
    {
      fps = 0;
      uint8_t micPeak     = mic_peaks.read()    * 30.0;
      uint8_t micRMS      = mic_rms.read()      * 30.0;
      uint8_t playRawPeak = playRaw_peaks.read()* 30.0;
      uint8_t playRawRMS  = playRaw_rms.read()  * 30.0;

    // Print the moving waveform Serial display
      for ( cnt = 0; cnt < 30 - micPeak; cnt++ ) Serial.print( " "  );
      while ( cnt++ < 29 && cnt < 30 - micRMS )  Serial.print( "<"  );
      while ( cnt++ < 30 )                       Serial.print( "="  );
      if ( micPeak == 1 )                        Serial.print( " "  );
                                                 Serial.print( "||" );
      for( cnt = 0; cnt < playRawRMS; cnt++ )    Serial.print( "="  );
      while( cnt++ < playRawPeak )               Serial.print( ">"  );
      while( cnt++ < 30 )                        Serial.print( " "  );
      Serial.printf( "       | Mic. Peak = %d | Mic. RMS = %d |"
                          " playRaw Peak = %d | playRaw RMS = %d |\n",
                      micPeak,
                      micRMS,
                      playRawPeak,
                      playRawRMS
                   );  //*/

      // forward mixer muting (switching)
      if ( micRMS > threshRMS )
      {
        returnValue = 1;
        count = 0;
      }
      else if ( micRMS <= threshRMS )
      {
        if ( ++count == 12 ) returnValue = 2;
      } // End of RMS muting
    }
  }
  return returnValue;
} // End of rmsAmplitudePeaksDuo()
// ==============================================================================================================

// ==============================================================================================================
// RMS Modulation
// Playback RMS modulation based on the input signal RMS
//
// Instead of using RMS as a threshold/switch mechanism,
// this function actively modulates the playback input
// signal, using channel gains, to obtain the same RMS
// of the microphone signal
// 
// Fluvio L. Lobo Fenoglietto 11/10/2017
// ==============================================================================================================
//uint32_t count;
uint32_t min_count = 1;
uint8_t rmsModulation()
{
  
  uint8_t returnValue = 0;
  //uint8_t threshRMS   = 0;

  if( fps > 24 )
  {
    // both microphone and the play_raw module
    if (   mic_peaks.available() 
        && mic_rms.available() 
        && playRaw_peaks.available() 
        && playRaw_rms.available() )
    {
      fps = 0;
      uint8_t micPeak     = mic_peaks.read()      * 30.0;
      uint8_t micRMS      = mic_rms.read()        * 30.0;
      uint8_t playRawPeak = playRaw_peaks.read()  * 30.0;
      uint8_t playRawRMS  = playRaw_rms.read()    * 30.0;
      //float micPeak     = mic_peaks.read();
      //float micRMS      = mic_rms.read();
      //float playRawPeak = playRaw_peaks.read();
      //float playRawRMS  = playRaw_rms.read();

      // RMS comparison
      if (micRMS == playRawRMS && micRMS > 3)                                                                   // if the micRMS is greater then the playRawRMS
      {
        returnValue = 0;                                                                                        // do NOT change the value of the playback input gain
      }
      else if (micRMS > playRawRMS)                                                                             // if the micRMS is greater than the playRawRMS
      {
        returnValue = 1;                                                                                        // after minimum count is reached, increase the value of the playback input gain (g++)
      }
      else if (micRMS < playRawRMS)                                                                             // if the micRMS is smaller than the playRawRMS
      {
        returnValue = 2;                                                                                        // after minimum count is reached, decrease the value of the playback input gain (g--)
      }
      else if (micRMS < 3)
      {
        returnValue = 2;                                                                                        // after minimum count is reached, decrese the value of the playback input gain (g--)
      }// End of RMS comparison...

      // Print values for comparison
      Serial.print("micRMS = ");
      Serial.print(micRMS);
      Serial.print(" | playRawRMS = ");
      Serial.print(playRawRMS);
      Serial.print(" | Count =");
      Serial.print(count);
      Serial.print(" | returnValue = ");
      Serial.println(returnValue);
     
    } // End of availability check
  } // End of fps check
  return returnValue;
} // End of rmsModulation()
// ==============================================================================================================

void switchMode( int m )
{
    Serial.print( "\nMode = "  );  Serial.print( mode );
    mode = m;                                                                                                   // Change value of operation mode for continous recording
    Serial.print( " -> "  );  Serial.println( mode );
}


//
// *** Start Recording
//
boolean startRecording()
{
  Serial.println( "EXECUTING startRecording()" );                                                               // Identification of function executed

  mixer_mic_Sd.gain( 0, mixerInputON  );                                                                        // Set gain of mixer_mic_Sd, channel0 to 0.5 - Microphone on
  mixer_mic_Sd.gain( 1, mixerInputON  );                                                                        // Set gain of mixer_mic_Sd, channel0 to 0.5 - Microphone on
  mixer_mic_Sd.gain( 2, mixerInputOFF );                                                                        // Set gain of mixer_mic_Sd, channel2 to 0
  
  char  fileRec[ses.fileRec.length()+1];                                                                        // Conversion from string to character array
  ses.fileRec.toCharArray( fileRec, sizeof( fileRec ) );

  char  fileDat[ses.fileDat.length()+1];                                                                        // Conversion from string to character array
  ses.fileDat.toCharArray( fileDat, sizeof( fileDat ) );
  
  if ( SD.exists( fileRec ) ) SD.remove( fileRec );                                                             // Check for existence of RECORD.RAW
  if ( SD.exists( fileDat ) ) SD.remove( fileDat );                                                             // Check for existence of HRATE.DAT

  frec  = SD.open( fileRec, FILE_WRITE );                                                                       // Create and open RECORD.RAW file
  Serial.println( frec );
  hRate = SD.open( fileDat,  FILE_WRITE );                                                                      // Create and open HRATE.DAT file
  Serial.println( hRate );

  if (  SD.open( fileRec, FILE_WRITE ) &&                                                                       // Create and open RECORD.RAW file
        SD.open( fileDat, FILE_WRITE ) )                                                                        // Create and open HRATE.DAT file
  {

    Serial.print( "\nMinimum S1-S2 interval:\t\t"       );  Serial.print(   minS1S2       );  Serial.print( "ms" );
    Serial.print( "\nMaximum S1-S2 interval:\t\t"       );  Serial.print(   maxS1S2       );  Serial.print( "ms" );
    Serial.print( "\nNo-heartrate detect interval:\t"   );  Serial.print(   maxHBInterval );  Serial.print( "ms" );
    Serial.print( "\nHearsound detection threshold:\t"  );  Serial.print(   sigThreshold  );
    Serial.print( "\nLow-pass filter roll-off freq:\t"  );  Serial.print(   freqLowPass   );
    Serial.print( "\nHighshelf filter roll-off freq:\t" );  Serial.println( freqHighShelf );
    Serial.print( "\nRecord file on SD is named:\t"   );    Serial.print(   fileRec       );
    Serial.print( "\nHR Data file on SD is named:\t"    );  Serial.println( fileDat       );

    queue_recMic.begin();
    recordState = RECORDING;
    switchMode( 1 );
    timeStamp   = 0;
    sf1.StartSend( STRING, 1000 );                                                                              // Begin transmitting heartrate data as a String
    Serial.println( "Stethoscope began RECORDING" );                                                            // Function execution confirmation over USB serial
    Serial.println( "sending: ACK..." );
    BTooth.write( ACK );                                                                                        // ACKnowledgement sent back through bluetooth serial
    Serial.println( "continueRecording() called" );
    return true;
  }
  else
    Serial.println( "Stethoscope CANNOT begin RECORDING" );                                                     // Function execution failed, notification over USB serial
    Serial.println( "sending: NAK..." );
    BTooth.write( NAK );                                                                                        // Negative AcKnowledgement sent back through bluetooth serial
    return false;
}


//
// *** Continue Recording
//
void continueRecording()
{
  if ( queue_recMic.available() >= 2 )
  {
    byte buffer[512];                                                                                           // Fetch 2 blocks from the audio library and copy into a 512 byte buffer.
                                                                                                                // The Arduino SD library is most efficient when full 512 byte sector size writes are used.
    memcpy( buffer, queue_recMic.readBuffer(), 256 );
    queue_recMic.freeBuffer();
    memcpy( buffer + 256, queue_recMic.readBuffer(), 256 );
    queue_recMic.freeBuffer();                                                                                  // write all 512 bytes to the SD card
    //elapsedMicros usec = 0;
    frec.write( buffer, 512 );
    // Uncomment these lines to see how long SD writes
    // are taking.  A pair of audio blocks arrives every
    // 5802 microseconds, so hopefully most of the writes
    // take well under 5802 us.  Some will take more, as
    // the SD library also must write to the FAT tables
    // and the SD card controller manages media erase and
    // wear leveling.  The queue1 object can buffer
    // approximately 301700 us of audio, to allow time
    // for occasional high SD card latency, as long as
    // the average write time is under 5802 us.
    //Serial.print( "SD write, us = " );
    //Serial.println( usec );
    bool beatCaptured = waveAmplitudePeaks();                                                                   // write HR and time to file at each heart beat
    if ( beatCaptured )
    {
      lineOut = String( heartRateI, DEC ) + "," + String( timeStamp, DEC ) + "\r\n";
      hRate.print( lineOut );
      txFr = sf1.Get();                                                                                         // get values from existing TX data frame
      txFr.DataString = String( heartRateI );                                                                   // update data-string value with heartrate
      sf1.Set( txFr );                                                                                          // set TX data frame with new heartate value
    }
  }
} // End of continueRecording()


//
// *** Stop Recording
//
boolean stopRecording()
{
  Serial.println( "EXECUTING stopRecording" );

  mixer_mic_Sd.gain( 0, mixerInputON  );                                                                        // Set gain of mixer_mic_Sd, channel0 to 0.5 - Microphone on
  mixer_mic_Sd.gain( 1, mixerInputON  );                                                                        // Set gain of mixer_mic_Sd, channel0 to 0.5 - Microphone on
  mixer_mic_Sd.gain( 2, mixerInputOFF );                                                                        // Set gain of mixer_mic_Sd, channel2 to 0
  
  queue_recMic.end();
  if ( recordState == RECORDING )
  { 
    sf1.StopSend( STRING );                                                                                     // Terminate transmitting heartrate data as a String
    Serial.println( "Stethoscope will STOP RECORDING" );                                                        // Function execution confirmation over USB serial
    Serial.println( "sending: ACK..." );
    BTooth.write( ACK );
    while ( queue_recMic.available() > 0 )
    {
      frec.write( (byte*)queue_recMic.readBuffer(), 256 );
      queue_recMic.freeBuffer();
    }
    frec.close();
    hRate.close();
    recordState = STANDBY;
    switchMode( 4 );
    return true;
  }
  else
    Serial.println( "Stethoscope CANNOT STOP RECORDING" );                                                      // Function execution confirmation over USB serial
    Serial.println( "sending: NAK..." );
    BTooth.write( NAK );                                                                                        // Negative AcKnowledgement sent back through bluetooth serial
    return false;
}


//
// *** Start Playing
//
boolean startPlaying( String fileName )
{
  Serial.println( "EXECUTING startPlaying()" );                                                                 // Identification of function executed

  mixer_mic_Sd.gain( 0, mixerInputOFF );                                                                        // Set the microphone channel 0 to mute (gain value = 0)
  mixer_mic_Sd.gain( 1, mixerInputON );                                                                         // Set the microphone channel 1 to mute (gain value = 0)
  //mixer_mic_Sd.gain( 2, mixerInputON  );                                                                      // Set the gain of the playback audio signal

  char  filePly[fileName.length()+1];                                                                           // Conversion from string to character array
  fileName.toCharArray( filePly, sizeof( filePly ) );
  
  if ( SD.exists( filePly ) )
  {
    playRaw_sdHeartSound.play( filePly );
    recordState = PLAYING;
    switchMode( 2 );
    Serial.println( "Stethoscope began PLAYING" );                                                              // Function execution confirmation over USB serial
    Serial.println( "sending: ACK..." );
    BTooth.write( ACK );                                                                                        // ACKnowledgement sent back through bluetooth serial
    return true;
  }
  else
    Serial.println( "Stethoscope CANNOT begin PLAYING" );                                                       // Function execution confirmation over USB serial
    Serial.println( "sending: NAK..." );
    BTooth.write( NAK );                                                                                        // Negative AcKnowledgement sent back through bluetooth serial
    return false;
}


//
// *** Continue Playing
//
void continuePlaying() 
{
  if ( !playRaw_sdHeartSound.isPlaying() )
  {
    playRaw_sdHeartSound.stop();
  }
}


//
// *** Stop Playing
//
boolean stopPlaying()
{
  Serial.println( "stopPlaying" );
  if ( recordState == PLAYING ) playRaw_sdHeartSound.stop();
  recordState = STANDBY;
  switchMode( 4 );
  Serial.println( "Stethoscope stopping PLAY" );                                                                // Function execution confirmation over USB serial
  Serial.println( "sending: ACK..." );
  BTooth.write( ACK );                                                                                          // ACKnowledgement sent back through bluetooth serial
  return true;
}



// ==============================================================================================================
// Start Blending
// Blending or mixing the input microphone line with an 
// audio file from the SD card
// 
// This function triggers/begins the blending of the playback signal
// onto the microphone signal
//
// Fluvio L. Lobo Fenoglietto 11/12/2017
// ==============================================================================================================
boolean startBlending( String fileName )
{
  Serial.println( "EXECUTING startBlending()" );                                                                // Identification of function executed

  // Control Mixer Channels and Gains
  mixer_mic_Sd.gain( 0, mixerInputON  );                                                                        // Turn ON the input mic channel (0, gain = 1)
  mixer_mic_Sd.gain( 1, mixerInputOFF  );                                                                       // Turn OFF the playback channel (1, gain = 0)
  mixer_allToSpk.gain( 1, mixerInputOFF );                                                                      // Turn OFF the high-pass-filtered channel (1, gain = 0)
  mixer_allToSpk.gain( 2, mixerInputOFF );                                                                      // Turn OFF the play-from memmory channel (2, gain = 0)

  char  filePly[fileName.length()+1];                                                                           // Conversion from string to character array
  fileName.toCharArray( filePly, sizeof( filePly ) );
  Serial.println( filePly );
  Serial.println( SD.exists( filePly ) );
  
  if ( SD.exists( filePly ) )
  {
    blendState = STARTING;
    recordState = PLAYING; //...do we need this?
    Serial.println( "Stethoscope will begin BLENDING" );
    playRaw_sdHeartSound.play( filePly );                                                                       // Start playing recorded HB
    BTooth.write( ACK );
    switchMode( 5 );
    return true;    
  }
  else
  {
    Serial.println( "Stethoscope CANNOT begin BLENDING" );                                                      // Function execution confirmation over USB serial
    BTooth.write( NAK );                                                                                        // Negative AcKnowledgement sent back through bluetooth serial
    return false;
  }
} // End of startBlending()


// ==============================================================================================================
// Continue Blending
// Blending or mixing the input microphone line with an 
// audio file from the SD card
// 
// This function continuosly blends the playback signal
// onto the microphone signal
//
// Fluvio L. Lobo Fenoglietto 11/10/2017
// ==============================================================================================================
float   mixer_lvl_ON                  = 1.0;
float   mixer_lvl_OFF                 = 0.0;
float   mic_mixer_lvl                 = 1.0;                                                                    // microphone mixer gain level (standard and initial)
float   playback_mixer_lvl            = 0.0;                                                                    // playback mixer gain level (standard and initial)
float   mic_mixer_lvl_step            = 0.00005;
float   playback_mixer_lvl_step       = mic_mixer_lvl_step;

float   playback_rms_mixer_lvl        = 0.25;
float   playback_rms_mixer_lvl_step   = 0.10;                                                                   // mixer level step for rms-based amplitude manipulation
void continueBlending(String fileName) 
{
  if ( !playRaw_sdHeartSound.isPlaying() )                                                                      // check if playback sound is playing/running                                                                 
  {
    //Serial.print(" Re-playing = ");
    //Serial.println(fileName);
    char  filePly[fileName.length()+1];                                                                           // Conversion from string to character array
    fileName.toCharArray( filePly, sizeof( filePly ) );
    Serial.println( filePly );
    Serial.println( SD.exists( filePly ) );
    playRaw_sdHeartSound.play( filePly );
  }

  // 
  // Using blending states, the function fades sound in/continously/out
  //
  if ( blendState == STARTING )                                                                                 // if blendState == STARTING, begin the blending of the signals
  {
    if ( mic_mixer_lvl > 0.10 )                                                                                 // check the value of the gain levels to trigger a gradual blending
    {
      mic_mixer_lvl       = mic_mixer_lvl - mic_mixer_lvl_step;                                                 // gradually decrease mic. gain level
      playback_mixer_lvl  = playback_mixer_lvl + playback_mixer_lvl_step;                                       // gradually increase playback gain level
      mixer_mic_Sd.gain(0, mic_mixer_lvl);                                                                      // apply mic. gain
      mixer_mic_Sd.gain(1, playback_mixer_lvl);                                                                 // apply playback gain
      Serial.print(" mic. gain = ");                                                                            // print gain values for debugging...
      Serial.print(mic_mixer_lvl);
      Serial.print(" || playback gain = ");
      Serial.println(playback_mixer_lvl);
    }
    else
    {
      blendState = CONTINUING;                                                                                  // Switch state to CONTINUING for dynamic blending, other...
    } // End of blend mixer level check
  }
  else if ( blendState == CONTINUING )                                                                          // if blendState == CONTINUING, maintain or vary mixer levels using functions
  {
    //uint8_t rms_switch = rmsAmplitudePeaksDuo();
    uint8_t rms_switch = rmsModulation();
    if ( rms_switch == 0 )                                                                                      // RMS value of mic. and playback signal are similar
    {
      // nothing
      //mixer_mic_Sd.gain(0, mic_mixer_lvl);
      //mixer_mic_Sd.gain(1, playback_mixer_lvl);
    }
    else if ( rms_switch == 1 )                                                                                 // RMS value of mic. > playback signal
    {
      playback_rms_mixer_lvl = playback_rms_mixer_lvl + playback_rms_mixer_lvl_step;                            // ...increase gain value
      if ( playback_rms_mixer_lvl > 1.25 ) playback_rms_mixer_lvl = 1.25;
      rms_playRaw_mixer.gain(0, playback_rms_mixer_lvl);                                                        // ...apply gain value
    }
    else if ( rms_switch == 2 )                                                                                 // RMS value of mic. < playback signal
    {
      playback_rms_mixer_lvl = playback_rms_mixer_lvl - playback_rms_mixer_lvl_step;                            // ...increase gain value
      if ( playback_rms_mixer_lvl < 0 ) playback_rms_mixer_lvl = 0;                                             // ...sign check, gain values are taken as the absolute so anything below zero will also generate sounds
      rms_playRaw_mixer.gain(0, playback_rms_mixer_lvl);                                                        // ...apply gain value
    }
    //Serial.print(" RMS Switch = ");
    //Serial.print(rms_switch);
    //Serial.print(" | PlayBack RMS Mixer Gain = ");
    //Serial.println(playback_rms_mixer_lvl);
  }
  else if ( blendState == STOPPING )
  {
    if ( mic_mixer_lvl < 0.90 )
    {
      mic_mixer_lvl       = mic_mixer_lvl + mic_mixer_lvl_step;
      playback_mixer_lvl  = playback_mixer_lvl - playback_mixer_lvl_step;    
      mixer_mic_Sd.gain(0, mic_mixer_lvl);
      mixer_mic_Sd.gain(1, playback_mixer_lvl);
      Serial.print(" mic. gain = ");
      Serial.print(mic_mixer_lvl);
      Serial.print(" || playback gain = ");
      Serial.println(playback_mixer_lvl);
    }
    else
    {
      playRaw_sdHeartSound.stop();                                                                              // stop playback file
      switchMode( 0 );                                                                                          // switch to pre-defined mode (preferably idle/standby)
    } // End of blend mixer level check
  } // End of blendState check 
} // End of continueBlending();
// ==============================================================================================================

//
// *** Stop Blending
//
boolean stopBlending()
{
  Serial.println( "EXECUTING stopBlending()" );
  blendState = STOPPING;                                                                                        // This will trigger the bleding down and stopping
  
  //if ( recordState == PLAYING ) playRaw_sdHeartSound.stop();
  //mixerLvL    = 1;
  //recordState = STANDBY;
  //switchMode( 4 );
  Serial.println( "Stethoscope will BLENDING" );                                                                // Function execution confirmation over USB serial
  Serial.println( "sending: ACK..." );
  BTooth.write( ACK );                                                                                          // ACKnowledgement sent back through bluetooth serial
  return true;
}


//
// *** Start Microphone Passthrough Mode
//     This function re-configures the stethoscope to pass audio through the microphone to the speakers
//
boolean startAudioPassThrough()
{
  Serial.println( "EXECUTING startAudioPassThrough()" );
  if ( recordState == RECORDING ) stopRecording();                                                              // Stop recording if recording
  if ( recordState == PLAYING   ) stopPlaying();                                                                // Stop playback if playing

  if ( selectedInput == AUDIO_INPUT_MIC )
  {
    mixer_mic_Sd.gain( 0, mixerInputON  );                                                                      // Set gain of mixer_mic_Sd, channel0 to 0.5 - Microphone on
    mixer_mic_Sd.gain( 1, mixerInputOFF  );                                                                     // Set gain of mixer_mic_Sd, channel0 to 0.5 - Microphone on
    //mixer_mic_Sd.gain( 2, mixerInputOFF );                                                                    // Set gain of mixer_mic_Sd, channel2 to 0
    recordState = PASSTHRU;
    switchMode( 4 );
    Serial.println( "Stethoscope switched AUDIO PASSTHROUGH mode." );                                           // Function execution confirmation over USB serial
    Serial.println( "sending: ACK..." );
    BTooth.write( ACK );                                                                                        // ACKnowledgement sent back through bluetooth serial
    return true;
  }
  else
  {
    Serial.println( "Stethoscope CANNOT switch to AUDIO PASSTHROUGH mode." );                                   // Function execution confirmation over USB serial
    Serial.println( "sending: NAK..." );
    BTooth.write( NAK );                                                                                        // Negative AcKnowledgement sent back through bluetooth serial
    return false;
  }
} // End of startAudioPassThrough()


//
// *** Continue Microphone Passthrough Mode
//
boolean continueAudioPassThrough()
{
  mixer_mic_Sd.gain( 0, mixerInputON  );                                                                        // Set gain of mixer_mic_Sd, channel0 to 0.5 - Microphone on
  mixer_mic_Sd.gain( 1, mixerInputOFF  );                                                                       // Set gain of mixer_mic_Sd, channel0 to 0.5 - Microphone on
  //mixer_mic_Sd.gain( 2, mixerInputOFF );                                                                      // Set gain of mixer_mic_Sd, channel2 to 0
  rmsAmplitudePeaksSingle();
  recordState = PASSTHRU;
  return true;
} // End of continueAudioPassThrough()



// ==============================================================================================================
// Start Heart Beat Monitoring
// 
// This function uses the wave peak detection tool to find 
// and measure heartbeat/rate from the microphone audio.
// The function does not record data to a file, rather is 
// needed to send information for remote display via serial
// port communication.
// 
// Michael Xynidis
// Fluvio L. Lobo Fenoglietto 11/12/2017
// ==============================================================================================================
boolean startHeartBeatMonitoring()
{
  Serial.println( "EXECUTING startTrackingMicStream()" );
  if ( recordState == RECORDING ) stopRecording();                                                              // Stop recording if recording
  if ( recordState == PLAYING ) stopPlaying();                                                                  // Stop playback if playing
  if ( selectedInput == AUDIO_INPUT_MIC )
  {
    // Set-up the initial channel gains
    rms_mic_mixer.gain(   0, mixerInputON   );                                                                  // turn rms mic mixer channel "0" ON (=1)
    rms_mic_mixer.gain(   1, mixerInputON   );                                                                  // turn rms mic mixer channel "1" ON (=1)
    mixer_mic_Sd.gain(    0, mixerInputON   );                                                                  // turn sd mic mixer channel "0" ON (=1)
    mixer_mic_Sd.gain(    1, mixerInputOFF  );                                                                  // turn sd playback mixer channel "1" OFF (=0)
    mixer_allToSpk.gain(  0, mixerInputON   );                                                                  // turn spk mic mixer channel "0" ON (=1)
    mixer_allToSpk.gain(  1, mixerInputOFF  );                                                                  // turn spk mic (fileterd) mixer channel "0" OFF (=0)
    mixer_allToSpk.gain(  2, mixerInputOFF  );                                                                  // turn spk playmem mixer channel "0" OFF (=0)
    
    queue_recMic.begin();
    recordState = DETECTING;
    switchMode( 3 );
    //sf1.StartSend( STRING, 1000 );                                                                              // Begin transmitting heartrate data as a String
    Serial.println( "Stethoscope STARTED DETECTING heartbeat from MIC audio." );                                // Function execution confirmation over USB serial
    Serial.println( "sending: ACK..." );
    BTooth.write( ACK );                                                                                        // ACKnowledgement sent back through bluetooth serial
    return true;
  }
  else
  {
    Serial.println( "Stethoscope CANNOT START DETECTING heartbeat from MIC audio." );                           // Function execution confirmation over USB serial
    Serial.println( "sending: NAK..." );
    BTooth.write( NAK );                                                                                        // Negative AcKnowledgement sent back through bluetooth serial
    return false;
  }
} // End of startMonitoring()
// ==============================================================================================================

// ==============================================================================================================
// Continue Heart Beat Monitoring
// 
// Continues heart rate monitoring/tracking using the
// waveAmplitudePeaks() function
// 
// Michael Xynidis
// Fluvio L. Lobo Fenoglietto 11/12/2017
// ==============================================================================================================
boolean continueHeartBeatMonitoring()
{
    //bool beatCaptured = waveAmplitudePeaks();                                                                   // write HR and time to file at each heart beat
    waveAmplitudePeaks2();
    //if ( beatCaptured )
    //{
    //  txFr = sf1.Get();                                                                                         // get values from existing TX data frame
    //  txFr.DataString = String( heartRateI );                                                                   // update data-string value with heartrate
    //  sf1.Set( txFr );                                                                                          // set TX data frame with new heartate value
    //}
  return true;
} // End of continueMonitoring()
// ==============================================================================================================

// ==============================================================================================================
// Stop Heart Beat Monitoring
// 
// Terminates heart rate monitoring/tracking using the
// waveAmplitudePeaks() function
// 
// Michael Xynidis
// Fluvio L. Lobo Fenoglietto 11/12/2017
// ==============================================================================================================
boolean stopHeartBeatMonitoring()
{
  Serial.println( "EXECUTING stopTrackingMicStream()" );
  if ( recordState == DETECTING )
  {
    recordState = PASSTHRU;
    switchMode( 0 );
    //sf1.StopSend( STRING );                                                                                     // Terminate transmitting heartrate data as a String
    Serial.println( "Stethoscope will STOP DETECTING heartbeat from MIC audio." );                              // Function execution confirmation over USB serial
    Serial.println( "sending: ACK..." );
    BTooth.write( ACK );                                                                                        // ACKnowledgement sent back through bluetooth serial
    return true;
  }
  else
  {
    Serial.println( "Stethoscope CANNOT STOP DETECTING heartbeat from MIC audio." );                            // Function execution confirmation over USB serial
    Serial.println( "sending: NAK..." );
    BTooth.write( NAK );                                                                                        // Negative AcKnowledgement sent back through bluetooth serial
    return false;
  }
} // End of stopHeartBeatMonitoring()
// ==============================================================================================================
