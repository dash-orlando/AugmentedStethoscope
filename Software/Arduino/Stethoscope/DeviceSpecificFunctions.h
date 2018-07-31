// ==============================================================================================================
// Device Specific Functions
//
// The following script comprises all of the functions used for the operation of the device and are also 
// unique to such device
// * Created on 10/27/2016
// 
// Michael Xynidis
// Fluvio L. Lobo Fenoglietto 11/13/2017
// ==============================================================================================================

// ==============================================================================================================
// Variables
// ============================================================================================================== //
File          frec;
File          micFileRec;
File          spkFileRec;
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
unsigned int  minS1S2       = 120;                                                                                // must be greater-than 100ms
unsigned int  maxS1S2       = 350;                                                                                // must be less-than 400ms
unsigned int  maxHBInterval = 5000;                                                                               // ms

float         sigThreshold  = 0.30;

bool          beatHeard     = false;
bool          atRest        = true;
bool          transition    = atRest;
bool          soundTwo      = false;

String        lineOut       = "";


// ==============================================================================================================
// Wave Amplitude Peaks
// Amplitude Peak Detection and Heart Rate Approximation
//
// The following function uses an amplitude peak detection
// tool to approximate heart-rate
// 
// Michael Xynidis 11/12/2017
// ==============================================================================================================
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
uint8_t         peak_zero      = 0;
uint8_t         peak_one       = 0;
uint8_t         peaks[2]       = {0,0};
uint8_t         peak_tolerance = 3;
uint8_t         peak_threshold = 5;
int             i              = 0;
unsigned long   current_time   = 0;
unsigned long   peak_zero_time = 0;
unsigned long   peak_one_time  = 1000;
unsigned long   peak_times[2]  = {0,0};
unsigned long   early_bound    = 500;                                                                           // msec.
unsigned long   late_bound     = 1250;                                                                          // msec.
float           hr             = 0;                                                                             // Heart Rate, bpm - beats per minute
elapsedMillis   timer;

void waveAmplitudePeaks2()
{
  if( fps > 24 )
  {
    if ( peak_QrsMeter.available() )                                                                            // if peak is available
    {
      fps = 0;                                                                                                  // reset fps... 
      uint8_t micPeak = mic_peaks.read()  * 30.0;                                                               // read peak value

      if ( micPeak > peak_threshold )
      {
                                                                               // time sample
        if ( timer < early_bound )                                                                              // if time is below the low bound limit (normal = 500 msec.)
        {
          if ( peak_zero == 0 )                                                                                 // ...if peak_zero = 0 or no values have been stored
          {
            peak_zero       = micPeak;                                                                          // ...store or keep the first value
            timer           = 0;
            peak_zero_time  = timer;
            peaks[0]        = peak_zero;
            peak_times[0]   = peak_zero_time;
          }
          else if ( peak_zero > 0 )                                                                             // ...if peak_zero > 0 or values have been stored
          {
            if ( micPeak > ( peak_zero + peak_tolerance ) )                                                     // ...if micPeak is greater than the stored peak_zero (with added tolerance) 
            {
              peak_zero       = micPeak;                                                                        // ...store the new micPeak as peak_zero
              timer           = 0;                                                                              // ...reset the timer
              peak_zero_time  = timer;                                                                          // ...store timer
              peaks[0]        = peak_zero;
              peak_times[0]   = peak_zero_time;
            }
          }
          //Serial.print(" S0 = ");
          //Serial.println(timer);
        } 
        else if ( timer > early_bound && timer <= late_bound )
        {
          if ( micPeak > ( peak_zero - peak_tolerance ) )
          {
            peak_one      = micPeak;                                                                            // ...store new micPeak as peak_one
            peak_one_time = timer;                                                                              // ...
            peaks[1]        = peak_one;
            peak_times[1]   = peak_one_time;

            // calculate heart rate
            hr            = 60000/( peak_one_time - peak_zero_time );

            // reset params
            peak_zero = peak_one;
            peak_one  = 0;
          }
          //Serial.print(" S1 = ");
          //Serial.println(timer);
        }
        else if ( timer > late_bound )
        {
          peak_zero      = 0;
          peak_one       = 0;
          timer          = 0;
          peaks[0]       = 0;
          peaks[1]       = 0;
          peak_times[0]  = 0;
          peak_times[1]  = 1;
        } // End of time-based segmentation
      } // End of peak threshold check
      
     
      // plotting amplitude data
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
      Serial.print(peak_times[0]);
      Serial.print(" | Time[1] = ");
      Serial.print(peak_times[1]);
      Serial.print(" | HR = ");
      Serial.println(hr); 

    } // End of peak availability()
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
      //Serial.print("micRMS = ");
      //Serial.print(micRMS);
      //Serial.print(" | playRawRMS = ");
      //Serial.print(playRawRMS);
      //Serial.print(" | Count =");
      //Serial.print(count);
      //Serial.print(" | returnValue = ");
      //Serial.println(returnValue);
     
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

// ==============================================================================================================
// Set Gains
// 
// This function sets the gains of the stethoscope by mode of operation or simulation scenario
// **In the future, this function will consolidate all independent gain-adjustment functions
//
// Fluvio L. Lobo Fenoglietto 05/14/2018
// ============================================================================================================== //
int gainMode = 0;                                                                                                 // default gain mode to 0
boolean setGains( int gainMode )
{
  switch( gainMode )
  {
    case 0:                                                                                                       // default settings
      // normal - nothing
      // calls setup functions to ensure similar settings
      setupMicToSpeaker();
      setupSDToSpeaker();
    break;

    case 1:                                                                                                       // mute mic. for bpc simulation
      // mute the mic. channel for simulation purposes
      mixer_mic_Sd.gain(   0, 0.10  );
      //rms_mic_mixer.gain(   1, 0.10  );
    break;
    
  } // End of gainMode switch()
  
} // End of setGains()

// ==============================================================================================================
// Parse String over Bluetooth
// Read information from the serial port passed
// as a string
//
// Fluvio L. Lobo Fenoglietto 11/30/2017
// ==============================================================================================================
String inString = "";
String parseString()
{
  if ( BTooth.available() > 0 )
  {
    inString = BTooth.readString();
  }
  if ( inString.length() > 1 )
  {
    Serial.println( "Stethoscope received STRING" );                                                            // Function execution confirmation over USB serial
    Serial.print(   "Stethoscope received ");
    Serial.println( inString );
    Serial.println( "sending: ACK..." );
    BTooth.write( ACK );                                                                                        // ACKnowledgement sent back through bluetooth serial
    return inString;
  }
  else
  {
    Serial.println( "Stethoscope did NOT receive STRING" );                                                     // Function execution confirmation over USB serial
    Serial.println( "sending: NAK..." );
    BTooth.write( NAK );                                                                                        // ACKnowledgement sent back through bluetooth serial
    return inString;
  }
} // End of parseString() function

// ==============================================================================================================
// Set Recording Mode
// Function that sets the recording mode
//
// mode   = 0   -- custom filename, uses the "inString" variable passed through serial/bluetooth com.
//        = 1   -- multiple channel recording ( - all channels to be specific )
//
// Fluvio L. Lobo Fenoglietto 05/12/2017
// ============================================================================================================== //
int     recMode       = 0;                                                                                        // Default -- rec. mode 0 ( will be expanded later )
int     recChannels   = 2;
boolean setRecordingMode()
{
  if ( BTooth.available() > 0 )
  {
    inString = BTooth.readString();
  }
  if ( inString.length() == 1 )
  {
    Serial.println( "Stethoscope received RECORDING MODE" );                                                      // Function execution confirmation over USB serial
    Serial.print(   "Stethoscope received ");
    Serial.println( inString );
    Serial.println( "sending: ACK..." );
    BTooth.write( ACK );                                                                                          // ACKnowledgement sent back through bluetooth serial
    recMode = inString.toInt();                                                                                   // Converting input string to integer
    return recMode;
  }
  else
  {
    Serial.println( "Stethoscope did NOT receive STRING" );                                                     // Function execution confirmation over USB serial
    Serial.println( "Setting RECORDING MODE to default (0)" );
    Serial.println( "sending: NAK..." );
    BTooth.write( NAK );                                                                                        // ACKnowledgement sent back through bluetooth serial
    recMode = 0;                                                                                                // ...leaving the default value
    return recMode;
  }
} // End of setRecordingMode()

// ==============================================================================================================
// Set Recording Filename
// Receive text information to generate a recording filename and avoid overwriting
//
// Fluvio L. Lobo Fenoglietto 11/30/2017
// ============================================================================================================== //
String  recExtension  = ".RAW";                                                                                   // Recording file extension definition
String  recString     = "";                                                                                       // Recording file string name definiton
String  recStrings[]  = {"",""};                                                                                  // Default string array dpending on the rec mode
boolean setRecordingFilename( String inString, String recExtension, int recMode )
{
  Serial.println( "EXECUTING setRecordingFilename()" );

  switch( recMode )
  {
    // single channel, custom string ---------------------------------------------------------------------------- //
    case 0:
    {
      Serial.println( "Recording Mode 0 : " );
      String recChannelPrefix = "R0";
      recString = recChannelPrefix + inString + recExtension;                                                     // Concatenating extension to input filename
      Serial.print( "Recording using filename : " );
      Serial.println( recString );
      return recString;
    }
    break;

    // multiple channels, custom string ------------------------------------------------------------------------- //
    case 1:
    {
      Serial.println( "Recording Mode 1 : " );
      Serial.print( "Recording from " );
      Serial.print( recChannels );
      Serial.println( " channels " );

      for( int i = 0; i < recChannels; i ++ )
      {
        String recChannelPrefix = "R";
        String recChannelID = recChannelPrefix + i;
        recStrings[i] = recChannelID + inString + recExtension;
        Serial.println( "Recording using filename : " + recStrings[i] );
      }
      // add section for file existence check, otherwise it gets deleted later
      return recStrings;
    }
    break;
    
  } // End of switch( recMode )
} // End of setRecordingFilename() function


// ==============================================================================================================
// Set REC GAINS
//
// Fluvio L. Lobo Fenoglietto 05/09/2018
// ============================================================================================================== //
void setRecGains()
{
  // Control Mixer Channels and Gains
  rms_mic_mixer.gain(     0,  mixerInputON  );                                                                  // Set mic input, channel 0 of mic&Sd mixer ON      (g = 1)
  rms_mic_mixer.gain(     1,  mixerInputON  );                                                                  // Set mic input, channel 1 of mic&Sd mixer ON      (g = 1)
  rms_playRaw_mixer.gain( 0,  mixerInputOFF );                                                                  // Set plaback, channel 0 of rms mixer OFF          (g = 0)
  mixer_mic_Sd.gain(      0,  mixerInputON  );                                                                  // Set mic input, channel 0 of mic&Sd mixer ON      (g = 1)
  mixer_mic_Sd.gain(      1,  mixerInputOFF );                                                                  // Set playback, channel 1 of mic&Sd mixer OFF      (g = 0)
  mixer_allToSpk.gain(    0,  mixerInputON  );                                                                  // Set mic input, channel 0 of speaker mixer ON     (g = 1)
  mixer_allToSpk.gain(    1,  mixerInputOFF );                                                                  // Set mic effect, channel 1 of speaker mixer ON    (g = 0)
  mixer_allToSpk.gain(    2,  mixerInputOFF );                                                                  // Set mem playback, channel 1 of speaker mixer ON  (g = 0)
  
} // End of setRecGains()

// ==============================================================================================================
// Start Recording
// Record audio from the input microphone line into
// the SD card onboard
// 
// This function triggers/begins the recording of
// the input audio signal
//
// Michael Xynidis
// Fluvio L. Lobo Fenoglietto 11/21/2017
// ==============================================================================================================
boolean startRecording(String recString)
{
  Serial.println( "EXECUTING startRecording()" );                                                               // Identification of function executed
  setRecGains();

  Serial.println( recString );
  char  recChar[recString.length()+1];                                                                          // Conversion from string to character array
  recString.toCharArray( recChar, sizeof( recChar ) );
  
  if ( SD.exists( recChar ) ) SD.remove( recChar );                                                             // Check for existence of HRATE.DAT

  frec = SD.open( recChar, FILE_WRITE );                                                                       // Create and open RECORD.RAW file
  Serial.println( frec );

  if (  SD.open( recChar, FILE_WRITE ) )                                                                        // Create and open HRATE.DAT file
  {
    queue_recMic.begin();
    deviceState = RECORDING;
    recState    = RECORDING;
    switchMode( 1 );
    Serial.println( "Stethoscope began RECORDING" );                                                            // Function execution confirmation over USB serial
    Serial.println( "sending: ACK..." );
    BTooth.write( ACK );                                                                                        // ACKnowledgement sent back through bluetooth serial
    Serial.println( "continueRecording() called" );
    return true;
  }
  else
  {
    Serial.println( "Stethoscope CANNOT begin RECORDING" );                                                     // Function execution failed, notification over USB serial
    Serial.println( "sending: NAK..." );
    BTooth.write( NAK );                                                                                        // Negative AcKnowledgement sent back through bluetooth serial
    return false;
  } 
} // End of startRecording()
// ==============================================================================================================

// ==============================================================================================================
// Start Multi-Channel Recording
// Record audio from multiple on-board audio channels, specifically:
// -- microphone
// -- speaker
// 
// This function also uses custome naming convention retrieved through bluetooth com.
//
// Michael Xynidis
// Fluvio L. Lobo Fenoglietto 05/09/2018
// ============================================================================================================== //
boolean startMultiChannelRecording( String recStrings[] )
{
  Serial.println( "EXECUTING startRecording()" );                                                                 // Identification of function executed
  setRecGains();                                                                                                  // Set-up gains for microphone recording

  // microphone channel ----------------------------------------------------------------------------------------- //
  Serial.println( recStrings[0] );
  char  micRecChar[recStrings[0].length()+1];                                                                     // Conversion from string to character array
  recStrings[0].toCharArray( micRecChar, sizeof( micRecChar ) );
  
  if ( SD.exists( micRecChar ) )
  {
    Serial.println( "File with the same name was found..." );
    Serial.println( "Old file removed..." );
    SD.remove( micRecChar );
  }
  
  micFileRec = SD.open( micRecChar, FILE_WRITE );
  //Serial.println( micFileRec );
  
  // speaker channel -------------------------------------------------------------------------------------------- //
  Serial.println( recStrings[1] );
  char  spkRecChar[recStrings[1].length()+1];                                                                     // Conversion from string to character array
  recStrings[1].toCharArray( spkRecChar, sizeof( spkRecChar ) );
  
  if ( SD.exists( spkRecChar ) )
  {
    Serial.println( "File with the same name was found..." );
    Serial.println( "Old file removed..." );
    SD.remove( spkRecChar );
  }
  
  spkFileRec = SD.open( spkRecChar, FILE_WRITE );
  //Serial.println( spkFileRec );
  
  // confirmation ----------------------------------------------------------------------------------------------- //
  if (  SD.open( micRecChar, FILE_WRITE ) &&                                                                      // Create and open RECORD.RAW file
        SD.open( spkRecChar, FILE_WRITE ) )                                                                       // Create and open HRATE.DAT file
  {
    queue_recMic.begin();
    queue_recSpk.begin();
    deviceState = RECORDING;
    recState    = RECORDING;
    //switchMode( 1 );
    recMode = 1;
    timeStamp   = 0;
    Serial.println( "Stethoscope began MULTI RECORDING" );                                                        // Function execution confirmation over USB serial
    Serial.println( "sending: ACK..." );
    BTooth.write( ACK );                                                                                          // ACKnowledgement sent back through bluetooth serial
    Serial.println( "continueRecording() called" );
    return true;
  }
  else
  {
    Serial.println( "Stethoscope CANNOT begin MULTI RECORDING" );                                                 // Function execution failed, notification over USB serial
    Serial.println( "sending: NAK..." );
    BTooth.write( NAK );                                                                                          // Negative AcKnowledgement sent back through bluetooth serial
    return false;
  }
} // End of startMultiChannelRecording()

// ==============================================================================================================
// Continue Recording
// Continue recording audio to SD card
// * now with recMode capabilities
//
// Michael Xynidis
// Fluvio L. Lobo Fenoglietto 11/21/2017
// ==============================================================================================================
boolean continueRecording()
{
  switch( recMode )
  {
    case 0:
      if ( queue_recMic.available() >= 2 )
      {
        byte buffer[512];                                                                                           // Fetch 2 blocks from the audio library and copy into a 512 byte buffer.
        memcpy( buffer, queue_recMic.readBuffer(), 256 );
        queue_recMic.freeBuffer();
        memcpy( buffer + 256, queue_recMic.readBuffer(), 256 );
        queue_recMic.freeBuffer();                                                                                  // write all 512 bytes to the SD card
        frec.write( buffer, 512 );
      }
      return true;
    break;
    case 1:
      if ( queue_recMic.available() >= 2 )
      {
        //Serial.println( " Recording mic out... " );
        byte buffer[512];                                                                                           // Fetch 2 blocks from the audio library and copy into a 512 byte buffer.
                                                                                                                    // The Arduino SD library is most efficient when full 512 byte sector size writes are used.
        memcpy( buffer, queue_recMic.readBuffer(), 256 );
        queue_recMic.freeBuffer();
        memcpy( buffer + 256, queue_recMic.readBuffer(), 256 );
        queue_recMic.freeBuffer();                                                                                  // write all 512 bytes to the SD card
        micFileRec.write( buffer, 512 );
      }
    
      if ( queue_recSpk.available() >= 2 )
      {
        //Serial.println( " Recording speaker out... " );
        byte buffer[512];                                                                                           // Fetch 2 blocks from the audio library and copy into a 512 byte buffer.
                                                                                                                    // The Arduino SD library is most efficient when full 512 byte sector size writes are used.
        memcpy( buffer, queue_recSpk.readBuffer(), 256 );
        queue_recSpk.freeBuffer();
        memcpy( buffer + 256, queue_recSpk.readBuffer(), 256 );
        queue_recSpk.freeBuffer();                                                                                  // write all 512 bytes to the SD card
        spkFileRec.write( buffer, 512 );
      }
      return true;
    break;
  } // End of switch( recMode )
} // End of continueRecording()
// ==============================================================================================================

// ==============================================================================================================
// Stop Recording
// Stops recording audio to SD card
// * now with multi-channel mode
//
// Michael Xynidis
// Fluvio L. Lobo Fenoglietto 11/21/2017
// ==============================================================================================================
boolean stopRecording()
{
  Serial.println( "EXECUTING stopRecording" );

  switch( recMode )
  {
    case 0:
      queue_recMic.end();
      if ( recState == RECORDING )
      {
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
        deviceState = READY;
        recState = READY;
        switchMode( 0 );
        return true;
      }
      else
        Serial.println( "Stethoscope CANNOT STOP RECORDING" );                                                      // Function execution confirmation over USB serial
        Serial.println( "sending: NAK..." );
        BTooth.write( NAK );                                                                                        // Negative AcKnowledgement sent back through bluetooth serial
        return false;

    case 1:
      Serial.println( " Closing Recording Files " );
      queue_recMic.end();
      queue_recSpk.end();
      if ( recState == RECORDING )
      {
        Serial.println( "Stethoscope will STOP MULTI RECORDING" );                                                 // Function execution confirmation over USB serial
        Serial.println( "sending: ACK..." );
        BTooth.write( ACK );
        
        while ( queue_recMic.available() > 0 && queue_recSpk.available() > 0  )
        {
          micFileRec.write( (byte*)queue_recMic.readBuffer(), 256 );
          queue_recMic.freeBuffer();

          spkFileRec.write( (byte*)queue_recSpk.readBuffer(), 256 );
          queue_recSpk.freeBuffer();
        }
        micFileRec.close();
        spkFileRec.close();
        deviceState = READY;
        recState    = READY;
        //switchMode( 0 );
        return true;
      }
      else
      {
        Serial.println( "Stethoscope CANNOT STOP RECORDING" );                                                      // Function execution confirmation over USB serial
        Serial.println( "sending: NAK..." );
        BTooth.write( NAK );                                                                                        // Negative AcKnowledgement sent back through bluetooth serial
        return false;
      }
  } // End of switch( recMode )
} // End of stopRecording()
// ==============================================================================================================

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
    deviceState = PLAYING;
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
  if ( deviceState == PLAYING ) playRaw_sdHeartSound.stop();
  deviceState = READY;
  switchMode( 4 );
  Serial.println( "Stethoscope stopping PLAY" );                                                                // Function execution confirmation over USB serial
  Serial.println( "sending: ACK..." );
  BTooth.write( ACK );                                                                                          // ACKnowledgement sent back through bluetooth serial
  return true;
}

// ==============================================================================================================
// Set Blend Gains
// Setting gains on mixers associated with the blending functions
//
// Fluvio L. Lobo Fenoglietto 05/13/2017
// ==============================================================================================================
boolean setBlendGains()
{
  // Control Mixer Channels and Gains
  //rms_mic_mixer.gain(     0,  mixerInputON  );                                                                  // Set mic input, channel 0 of mic&Sd mixer ON      (g = 1)
  //rms_mic_mixer.gain(     1,  mixerInputON  );                                                                  // Set mic input, channel 1 of mic&Sd mixer ON      (g = 1)
  rms_playRaw_mixer.gain(   0,  mixerInputON );                                                                   // Set plaback, channel 0 of rms mixer OFF          (g = 0)
  mixer_mic_Sd.gain(        0,  mixerInputON  );                                                                  // Set mic input, channel 0 of mic&Sd mixer ON      (g = 1)
  mixer_mic_Sd.gain(        1,  mixerInputOFF );                                                                  // Set playback, channel 1 of mic&Sd mixer OFF      (g = 0)
  mixer_allToSpk.gain(      0,  mixerInputON  );                                                                  // Set mic input, channel 0 of speaker mixer ON     (g = 1)
  mixer_allToSpk.gain(      1,  mixerInputOFF ); 
} // End of setBlendGains()

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
  setBlendGains();
  
  char  filePly[fileName.length()+1];                                                                           // Conversion from string to character array
  fileName.toCharArray( filePly, sizeof( filePly ) );
  Serial.println( filePly );
  Serial.println( SD.exists( filePly ) );
  
  if ( SD.exists( filePly ) )
  {
    deviceState = BLENDING;
    blendState  = BLENDING;
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
float   mic_mixer_lvl_step            = 0.0001;
float   playback_mixer_lvl_step       = mic_mixer_lvl_step;
float   playback_rms_mixer_lvl        = 0.25;
float   playback_rms_mixer_lvl_step   = 0.10;                                                                   // mixer level step for rms-based amplitude manipulation
boolean continueBlending(String fileName) 
{
  if ( !playRaw_sdHeartSound.isPlaying() )                                                                      // check if playback sound is playing/running                                                                 
  {
    Serial.println( "File NOT PLAYING... RESTARTING playback" );
    char  filePly[fileName.length()+1];                                                                         // Conversion from string to character array
    fileName.toCharArray( filePly, sizeof( filePly ) );
    Serial.println( filePly );
    Serial.println( SD.exists( filePly ) );
    playRaw_sdHeartSound.play( filePly );
  }

  // 
  // Using blending states, the function fades sound in/continously/out
  //
  if ( blendState == BLENDING )                                                                                 // if deviceState == STARTING, begin the blending of the signals
  {
    if ( mic_mixer_lvl > 0.10 )                                                                                 // check the value of the gain levels to trigger a gradual blending
    {
      mic_mixer_lvl       = mic_mixer_lvl - mic_mixer_lvl_step;                                                 // gradually decrease mic. gain level
      playback_mixer_lvl  = playback_mixer_lvl + playback_mixer_lvl_step;                                       // gradually increase playback gain level
      mixer_mic_Sd.gain(0, mic_mixer_lvl);                                                                      // apply mic. gain
      mixer_mic_Sd.gain(1, playback_mixer_lvl);                                                                 // apply playback gain
      //Serial.print(" mic. gain = ");                                                                          // print gain values for debugging...
      //Serial.print(mic_mixer_lvl);
      //Serial.print(" || playback gain = ");
      //Serial.println(playback_mixer_lvl);
    }
    else
    {
      blendState = CONTINUING;                                                                                   // Switch state to CONTINUING for dynamic blending, other...
      return true;
    } // End of blend mixer level check
  }
  else if ( blendState == CONTINUING )                                                                           // if deviceState == CONTINUING, maintain or vary mixer levels using functions
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
    return true;
  }
  else if ( blendState == READY )
  {
    if ( mic_mixer_lvl < 0.90 )
    {
      mic_mixer_lvl       = mic_mixer_lvl + mic_mixer_lvl_step;
      playback_mixer_lvl  = playback_mixer_lvl - playback_mixer_lvl_step;    
      mixer_mic_Sd.gain(0, mic_mixer_lvl);
      mixer_mic_Sd.gain(1, playback_mixer_lvl);
      //Serial.print(" mic. gain = ");
      //Serial.print(mic_mixer_lvl);
      //Serial.print(" || playback gain = ");
      //Serial.println(playback_mixer_lvl);
    }
    else
    {
      playRaw_sdHeartSound.stop();                                                                              // stop playback file
      setGains(0);
      switchMode( 0 );
      // switch to pre-defined mode (preferably idle/standby)
    } // End of blend mixer level check
  } // End of deviceState check
  return true; 
} // End of continueBlending();
// ==============================================================================================================

// ==============================================================================================================
// Stop Blending
// Blending or mixing the input microphone line with an 
// audio file from the SD card
// 
// This function stops the blending of the playback signal
// onto the microphone signal
//
// Fluvio L. Lobo Fenoglietto 11/13/2017
// ==============================================================================================================
boolean stopBlending()
{
  Serial.println( "EXECUTING stopBlending()" );
  deviceState = READY;                                                                                        // This will trigger the bleding down and stopping
  blendState  = READY;
  Serial.println( "Stethoscope will STOP BLENDING" );                                                           // Function execution confirmation over USB serial
  Serial.println( "sending: ACK..." );
  BTooth.write( ACK );                                                                                          // ACKnowledgement sent back through bluetooth serial
  return true;
} // End of stopBlending()
// ==============================================================================================================

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
  if ( deviceState == RECORDING ) stopRecording();                                                              // Stop recording if recording
  if ( deviceState == PLAYING ) stopPlaying();                                                                  // Stop playback if playing
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
    deviceState = MONITORING;
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
  if ( deviceState == MONITORING )
  {
    deviceState = READY;
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

// ==============================================================================================================
// Device RESET
// Based on a status enquiy, this function sets the stethoscope to an idle mode/state
// 
// Fluvio L. Lobo Fenoglietto 05/08/2017
// ============================================================================================================== //
void setToIdle()
{
  Serial.println( "received: SETIDLE..." );                                                                       // signal receipt of function-specific byte
  Serial.print( "deviceState is " );
  Serial.println( stateToText( deviceState ) );                                                                   // print state of the device to serial monitor

  switch( deviceState )
  {
    case READY :
      Serial.println( "Device already in IDLE and READY" );
      Serial.println( "sending: ACK..." );
      BTooth.write( ACK );
    break;
    case NOTREADY :
      Serial.println( "Device CANNOT TURN to IDLE..." );
      Serial.println( "Troubleshoot device..." );
      Serial.println( "sending: NAK..." ); 
      BTooth.write( NAK );
    break;
    case RECORDING :                                                                                               // RECORDING single/multiple audio streams ( this may be broken down later )
      Serial.println( "Device RECORDING..." );
      stopRecording();
    break;
    case PLAYING :                                                                                                 // PLAYING audio file
      Serial.println( "Device PLAYING..." );
      stopPlaying();
    break;
    case MONITORING :                                                                                              // MONITORING heart beat ( may be paired with recording )
      Serial.println( "Device MONITORING..." );
      stopHeartBeatMonitoring();
    break;
    case BLENDING :                                                                                                // BLENDING microphone input with audio file from SD card
      Serial.println( "Device BLENDING..." );
      stopBlending();
    break;
    case CONTINUING :
      Serial.println( "Device BLENDING..." );
      stopBlending();
    break;
  }
} // End setToIdle()

// ================= //
void testFilters()
{
  // rms mic mixer ---------------------------------------------------------------------------------------------- //
  rms_mic_mixer.gain(   0, mixerInputOFF  );
  rms_mic_mixer.gain(   1, mixerInputOFF  );
  // mixer mic SD ----------------------------------------------------------------------------------------------- //
  mixer_mic_Sd.gain(    0, mixerInputOFF  );                                                                      // Set gain of mixer_mic_Sd, channel0 to 1.00
  // mixer all to speaker  -------------------------------------------------------------------------------------- //
  mixer_allToSpk.gain(  0, mixerInputOFF  );                                                                      // Normal stethoscope mic input (on)
  mixer_allToSpk.gain(  1, mixerInputON );                                                                       // Highpass mic input (off)
}

