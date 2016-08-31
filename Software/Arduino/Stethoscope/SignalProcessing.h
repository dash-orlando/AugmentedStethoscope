enum State
{
  CONNECTED, DISCONNECTED, RECORDING, PLAYING, STANDBY
};

State connectState  = DISCONNECTED;
State recordState   = STANDBY;

File      frec;
int       mode      = 0;                      // 0 = idle; 1 = record; 2 = play
const int myInput   = AUDIO_INPUT_MIC;


void adjustMicLevel()
{
  // TODO: read the peak1 object and adjust sgtl5000_1.micGain()
  // if anyone gets this working, please submit a github pull request :-)
}


boolean startRecording()
{
  Serial.println( "startRecording" );
  if ( SD.exists( "RECORD.RAW" ) )
  {
    // The SD library writes new data to the end of the
    // file, so to start a new recording, the old file
    // must be deleted before new data is written.
    SD.remove( "RECORD.RAW" );
  }
  frec = SD.open( "RECORD.RAW", FILE_WRITE );
  if ( frec )
  {
    queue1.begin();
    recordState = RECORDING;
    return true;
  }
  else return false;
}
/*

void continueRecording() 
{
  if ( queue1.available() >= 2 )
  {
    byte buffer[512];

    // Fetch 2 blocks from the audio library and copy
    // into a 512 byte buffer.  The Arduino SD library
    // is most efficient when full 512 byte sector size
    // writes are used.
    memcpy( buffer, queue1.readBuffer(), 256 );
    queue1.freeBuffer();
    memcpy( buffer + 256, queue1.readBuffer(), 256 );
    queue1.freeBuffer();

    // write all 512 bytes to the SD card
    elapsedMicros usec = 0;
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

    //Serial.print( "SD write, us=" );
    //Serial.println( usec );
  }
} // */


boolean stopRecording()
{
  Serial.println( "stopRecording" );
  queue1.end();
  if ( recordState == RECORDING )
  {
    while (queue1.available() > 0)
    {
      frec.write((byte*)queue1.readBuffer(), 256);
      queue1.freeBuffer();
    }
    frec.close();
  }
  recordState = STANDBY;
  return true;
}


boolean startPlaying()
{
  Serial.println( "startPlaying" );
  playRaw1.play( "RECORD.RAW" );
  recordState = PLAYING;
  return true;
}
/*

void continuePlaying()
{
  if ( !playRaw1.isPlaying() )
  {
    playRaw1.stop();
    mode = 0;
  }
} // */


boolean stopPlaying()
{
  Serial.println( "stopPlaying" );
  if ( recordState == PLAYING ) playRaw1.stop();
  recordState = STANDBY;
  return true;
}


/*
elapsedMillis msecs;
elapsedMillis triggered;
elapsedMillis elapsed;
unsigned int  hrSample[3] = { 0, 0, 0 };
int           ndx         = 0;
int           heartRate   = 0;
boolean       beat        = false;
boolean       captured    = false;

//*
void waveAmplitudePeaks( const char *filename )
{
  char    charArray[] = { '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'  };
  String  fileString  = "";

  for ( int i = 0; i < 13; i++ ) charArray[i] = filename[i];
  for ( int i = 0; i < 13; i++ ) fileString += charArray[i];

   Serial.print( "Filename passed to Peak meter is " ); Serial.println( fileString );
  while ( 1 )
  {
    float   vol         = analogRead( 15 ) / 1024.0;
    float   sensitivity = 1.0 - vol;

    buttonStop.update();
    if ( buttonStop.fallingEdge() ) 
    { 
      Serial.println( "please stop!" );
      if ( playSdWav1.isPlaying() ) playSdWav1.stop();
      if ( playRaw1.isPlaying()   ) playRaw1.stop();
//      mode = 0;
      break; 
    }
  
    if ( ( playSdWav1.isPlaying() == false ) && ( playRaw1.isPlaying() == false ) )
    {
      Serial.println( "Start heartbeat" );
// 
//
      if ( fileString.startsWith( "RECORD" ) )
      {
        Serial.println( "file is 'RAW' format" );
        playRaw1.play( "RECORD.RAW" );
      }
      else
      {
        playSdWav1.play( filename );
      }
//      
//      playSdWav1.play( filename );

      delay( 10 );        // wait for library to parse WAV info
  boboPrint( 0, 3, "     >amplitude<    " );
    }
    
    if ( msecs > 40 )
    {
      if ( peak2.available() && peak3.available() )
      {
        msecs             = 0;
        float leftNumber  = 0.0;
        float rightNumber = 0.0;

        if ( fileString.startsWith( "RECORD" ) )
        {
          leftNumber  = peak4.read();
          rightNumber = peak5.read();
        }
       else
        {
          leftNumber  = peak2.read();
          rightNumber = peak3.read();
        }
        int   leftPeak    = leftNumber  * 30.0;
        int   rightPeak   = rightNumber * 30.0;
        int   count;
        char  fillChar    = ' ';
  
        for ( count = 0; count < 30 - leftPeak; count++ )
          Serial.print(" ");
  
        if ( ( leftNumber >= sensitivity ) && ( rightNumber >= sensitivity ) )
        {
          fillChar = '<';
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
  
        if ( ( leftNumber >= sensitivity ) && ( rightNumber >= sensitivity ) )
          fillChar = '>';
        else
          fillChar = ' ';
   
        for ( count = 0; count < rightPeak; count++ )
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
  }
} // */


/*
void playWavFile( const char *filename )
{
  Serial.print( "Playing '.WAV' file: " );
  Serial.println( filename );
  boboPrint( 0, 3, "      >playing<     " );

  // Start playing the file.  This sketch continues to
  // run while the file plays.
  playWav1.play(filename);

  // A brief delay for the library read WAV info
  delay(5);

  // Simply wait for the file to finish playing.
  while (playWav1.isPlaying()) 
  {
    // uncomment these lines if you audio shield
    // has the optional volume pot soldered
    float vol = analogRead(15);
    vol = vol / 1024;
    sgtl5000_1.volume(vol);

    // First, read the buttons
    buttonRecord.update();
    buttonStop.update();
    buttonPlay.update();
    
    if ( buttonStop.fallingEdge() )       // "Stop" button
    {
      playWav1.stop();
      boboPrint( 0, 3, "    >sound files<   " );
      break;
    }
  }
} // */


/*
void playRawFile( const char *filename )
{
  Serial.print( "Playing '.RAW' file: " );
  Serial.println( filename );
  boboPrint( 0, 3, "      >playing<     " );

  // Start playing the file.  This sketch continues to
  // run while the file plays.
  playRaw1.play(filename);

  // A brief delay for the library read WAV info
  delay(5);

  // Simply wait for the file to finish playing.
  while (playRaw1.isPlaying()) 
  {
    // uncomment these lines if you audio shield
    // has the optional volume pot soldered
    float vol = analogRead(15);
    vol = vol / 1024;
    sgtl5000_1.volume(vol);

    // First, read the buttons
    buttonRecord.update();
    buttonStop.update();
    buttonPlay.update();
    
    if ( buttonStop.fallingEdge() )       // "Stop" button
    {
      playRaw1.stop();
      boboPrint( 0, 3, "    >sound files<   " );
      break;
    }
  }
} // */


/*
void filePlayer()
{
  rootDir = SD.open("/");
  loadDirectory( rootDir ) ;
  
//  mode = 3;
  while ( mode == 3 )
  {
    // First, read the buttons
    buttonRecord.update();
    buttonStop.update();
    buttonPlay.update();
    
    if ( buttonStop.fallingEdge() )       // "Select" button
    {
      Serial.println( "'Select' button pressed" );
//      boboPrint( 0, 3, "      >stopped<     " );
      if ( listPtr == -1 )
      {
        mode = 0;
      }
      else
      {
//        Serial.print( "Selection: " );
//        Serial.println( fileEntry[listPtr] );

        // Instead of the following, maybe this might work:  mystring.toCharArray( otherstring, mystring.length() + 1) ;
        // or...
                //int str_len = str.length() + 1; 
                //
                //// Prepare the character array (the buffer) 
                //char char_array[str_len];
                //
                //// Copy it over 
                //str.toCharArray(char_array, str_len);                       

char          c0 = fileEntry[listPtr][0],
              c1 = fileEntry[listPtr][1],
              c2 = fileEntry[listPtr][2],
              c3 = fileEntry[listPtr][3],
              c4 = fileEntry[listPtr][4],
              c5 = fileEntry[listPtr][5],
              c6 = fileEntry[listPtr][6],
              c7 = fileEntry[listPtr][7],
              c8 = fileEntry[listPtr][8],
              c9 = fileEntry[listPtr][9],
              c10 = fileEntry[listPtr][10],
              c11 = fileEntry[listPtr][11],
              c12 = '\0';

        char str[13] = { c0, c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12 };

        if ( true ) //( ( fileEntry[listPtr].startsWith( "HBEAT" ) ) || ( fileEntry[listPtr].endsWith( "RAW" ) ) )
        {
          Serial.println( "going to Amplitude meter..." );
          waveAmplitudePeaks( str );
        }
        else if ( fileEntry[listPtr].endsWith( "RAW" ) )
        {
          playRawFile( str );
        }
        else
        {
          playWavFile( str );
        }
        boboPrint( 0, 3, "    >sound files<   " );
        Serial.println( "Playback finished" );
      }
    }

    if ( buttonRecord.fallingEdge() )     // "Up" button
    {
      if ( ( lcdPtr == 0 ) & ( listPtr > -1 ) )
      {
        listPtr--;
        if ( listPtr == -1 )
        {
          boboPrint( 2, 0, "back        " );
          lcd.setCursor( 7, 0 );
          lcd.write( 3 );
        
          for ( int i = 1; i < lcdRows - 1; i++ )
          {
            boboPrint( 2, i, "            " );
            boboPrint( 2, i, fileEntry[i-1] );
          }
        }
        else
        {
          for ( int i = 0; i < lcdRows - 1; i++ )
          {
            boboPrint( 2, i, "            " );
            boboPrint( 2, i, fileEntry[listPtr + i] );
          }
        }
      }
      else if ( lcdPtr > 0 )
      {
        lcdPtr--;
        listPtr--;
        boboPrint( 0, lcdPtr + 1, " " );
        lcd.setCursor( 0, lcdPtr );
        lcd.write( 1 );
      }

    }
    if ( buttonPlay.fallingEdge() )       // "Down" button
    {
//
//      Serial.print( "\nlcdPtr: " ); Serial.println( lcdPtr );
//      Serial.print( "listPtr:  " ); Serial.println( listPtr );
//      Serial.print( "endPtr : " ); Serial.println( endPtr );
//      
      if ( ( lcdPtr == lcdRows - 2 ) & ( listPtr < endPtr ) )
      {
        listPtr++;
        for ( int i = 0; i < lcdRows - 1; i++ )
        {
          boboPrint( 2, i, "            " );
          boboPrint( 2, i, fileEntry[listPtr + i - 2] );
        }
      }
      else if ( lcdPtr < lcdRows - 2 )
      {
        lcdPtr++;
        listPtr++;
        boboPrint( 0, lcdPtr - 1, " " );
        lcd.setCursor( 0, lcdPtr );
        lcd.write( 1 );
      }
    }
  }
  mode = 0;
} // */

