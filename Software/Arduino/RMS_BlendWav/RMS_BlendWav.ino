#include "TeensyAudio.h"
#include "BPCuffResponse.h"

bool          equalizer = false;
bool          hSound1   = false;

float         rmsA        = 0.0f;
float         rmsB        = 0.0f;

int32_t       cnt         = 0;
int           HBDuration  = 600;                  // Heartbeat duration in ms
int           calcHRate   = 0;

elapsedMillis fps;
elapsedMillis elapsed;

const char   *fileNameA   = "HBNORMAL.RAW";
//const char   *fileNameA = "HBNORM_S.RAW";
//const char   *fileNameA = "HBNORMAL.WAV";

//const char   *fileNameB = "HBMURMUR.RAW";
//const char   *fileNameB = "HBMURM_S.RAW";
//const char   *fileNameB = "HBMURMUR.WAV";
const char   *fileNameB   = "mem_Korotkoff";

Sd2Card       card;
SdVolume      volume;
char          buffer[512];
float         size;
bool          reading     = true;
bool          playing     = false;
bool          synthHB     = false;


void setup()
{
  Serial.begin( 115200 );
  SPI.setMOSI( 7 );                               // Audio shield has MOSI on pin 7
  SPI.setSCK( 14 );                               // Audio shield has SCK on pin 14

  if ( ( !card.init( SPI_FULL_SPEED, 10  ) ) ||   // Fail if SD is not on Audio shield on pin 10,
    !( ( card.type() == SD_CARD_TYPE_SD1 )        // ...or if SD card is not of Type SD-1,
    || ( card.type() == SD_CARD_TYPE_SD2 )        //                            Type SD-2,
    || ( card.type() == SD_CARD_TYPE_SDHC ) ) )   //                         or Type SDHC.
  {
    Serial.println( "SD card is not connected or unusable" );
  }
  else
  {
    AudioMemory( 60 );
    audioShield.enable();
    audioShield.volume( 0.5 );                    // sets headphone level (only); set between 0.5 - 0.8
    switchInputs( mixer_A, WAVE );
    switchInputs( mixer_B, RAW );
    if ( SD.begin( 10 ) )
    {
      Serial.println( "SD card is connected" );
      fps     = 0;
      elapsed = 0;
      playing = true;
      startPlaying( fileNameA, fileNameB );
    }

/*
    Serial.println( fileNameA.available() );
    Serial.println( fileNameB.available() );
    if ( fileNameA.available() && fileNameB.available() )
    {
      do
      {
        byte b = fileNameA.read();
        cnt++;
        if ( cnt % (512 * 2048) == 0 ) Serial.print( "." );
        else reading = false;
      } while( reading ) ;
      Serial.print( "\nnumber of bytes: " ); Serial.println( cnt );
    }
    else Serial.println( "files not found" ); //*/
  }
}


void loop()
{
  if ( continuePlaying( fileNameA, fileNameB ) )
    rmsAmplitudePeaks();
}


void switchInputs( AudioMixer4 mixer, Source inputSource )
{
  switch ( inputSource )
  {
    case WAVE :
      mixer.gain( inpWavL, 0.5 );
      mixer.gain( inpWavR, 0.5 );
      mixer.gain( inpRaw,  0.0 );
      mixer.gain( inpMem,  0.0 );
      break;
    case RAW :
      mixer.gain( inpWavL, 0.0 );
      mixer.gain( inpWavR, 0.0 );
      mixer.gain( inpRaw,  1.0 );
      mixer.gain( inpMem,  0.0 );
      break;
    case MEMORY :
      mixer.gain( inpWavL, 0.0 );
      mixer.gain( inpWavR, 0.0 );
      mixer.gain( inpRaw,  0.0 );
      mixer.gain( inpMem,  1.0 );
      break;
    default  :
      break;
  }
}


void rmsAmplitudePeaks()
{
  if( fps > 24 )
  {
    if ( peak_QrsMeter.available()
        && rms_A.available()
        ) //&& rms_B.available() )
    {
      fps = 0;
//      uint8_t mixedPeak = peak_QrsMeter.read() * 30.0;
      uint8_t fileAPeak = peak_A.read() * 30.0;
      uint8_t fileA_RMS = rms_A.read()  * 30.0;
      if ( fileA_RMS >= 8 ) PlayHB();
      
      uint8_t fileBPeak = peak_B.read() * 30.0;
      uint8_t fileB_RMS = rms_B.read()  * 30.0;


      Serial.printf(  "  %2.1f / %2.1f\t",
                      ((float)fileA_RMS / 30.0) * 100,
                      ((float)fileAPeak / 30.0) * 100 );
      for ( cnt = 0; cnt < 30 - fileAPeak; cnt++ )        // print Amplitude
      {
        Serial.print( " " );
      }
      while ( ++cnt <= 30 - fileA_RMS )
      {
        Serial.print( "<" );
      }
      while ( cnt++ <= 30 )
      {
        Serial.print( "=" );
      }
      
      
      Serial.print( "||" );

      hSound1 = ( (int)elapsed > HBDuration ) && ( fileBPeak < 30 );

      for( cnt = 0; cnt < fileB_RMS; cnt++ )
      {
        if ( hSound1 )
          Serial.print( "=" );
      }
      for( ; cnt < fileBPeak; cnt++ )
      {
        if ( hSound1 )
          Serial.print( ">" );
      }
      for ( int i = 0; i < 32 - fileBPeak; i++ )
      {
        if ( hSound1 )
          Serial.print( " " );
      }
      if ( hSound1 ) 
      {
        Serial.printf(  " %ums HR: %d, %2.1f / %2.1f\t",
                        (int)elapsed, calcHRate,
                        ((float)fileB_RMS / 30.0) * 100,
                        ((float)fileBPeak / 30.0) * 100 );
        calcHRate = 60000 / elapsed;
        elapsed   = 0;
      }
//      Serial.print( "\tAPU: " );
//      Serial.print( AudioProcessorUsage() );
      Serial.println();
    }
  }
}


bool startPlaying( const char *fileA, const char *fileB )
{
  bool  returnState = true;

  Serial.print( "\nEXECUTING startPlaying( " ); Serial.print( fileA ); Serial.println( " )" );

  if ( String( fileA ).startsWith( "mem" ) )
    Serial.println( "'Play sample from memory' is only an option for input 'B'." );
  
  else if ( SD.exists( fileA ) )
  {
    if ( String( fileA ).endsWith( ".WAV" ) )
    {
      Serial.print( "WAV file: " );
      playSdWav_sdHeartSound_A.play( fileA );      
    }
    if ( String( fileA ).endsWith( ".RAW" ) )
    {
      Serial.print( "RAW file: " );
      playRaw_sdHeartSound_A.play( fileA );
    }
    mixer_SD.gain( inpA, 0.5 );
    Serial.print( "Playing " ); Serial.print( fileA ); Serial.print( " - " ); Serial.println( elapsed/1000.0 );
  }
  else
  {
    Serial.print( "Failed to play [" ); Serial.print( fileA ); Serial.println( "]" );
    returnState = false;
  }

  Serial.print( "\nEXECUTING startPlaying( " ); Serial.print( fileB ); Serial.println( " )" );
  Serial.println( "...but not really. ;)" );  /*
  if ( String( fileB ).startsWith( "mem" ) )
    synthHB = BeginHB();
  
  else if ( SD.exists( fileB ) )
  {
    if ( String( fileB ).endsWith( ".WAV" ) )
    {
      Serial.print( "WAV file: " );
      playSdWav_sdHeartSound_B.play( fileB );      
    }
    if ( String( fileB ).endsWith( ".RAW" ) )
    {
      Serial.print( "RAW file: " );
      playRaw_sdHeartSound_B.play( fileB );
    }
    mixer_SD.gain( inpB, 0.5 );
    Serial.print( "Playing " ); Serial.print( fileB ); Serial.print( " - " ); Serial.println( elapsed/1000.0 );
  }
  else
  {
    Serial.print( "Failed to play [" ); Serial.print( fileB ); Serial.println( "]" );
    returnState = false;
  } //*/
  Serial.println();
  return returnState;
}


bool stoppedA = false;
bool stoppedB = false;
bool continuePlaying( const char *fileA, const char *fileB )
{
  bool returnState = false;
  
  if ( ( !playSdWav_sdHeartSound_A.isPlaying() ) && ( !playRaw_sdHeartSound_A.isPlaying() ) )
  {
    if ( !stoppedA )
    {
      playSdWav_sdHeartSound_A.stop();
      playRaw_sdHeartSound_A.stop();
      stoppedA = true;
      Serial.print( fileA ); Serial.print( " ended - " ); Serial.println( elapsed/1000.0 );
      if ( synthHB )
      {
        stoppedB = true;
        Serial.print( fileB ); Serial.print( " ended - " ); Serial.println( elapsed/1000.0 );
      }
    }
  }
  if ( ( !playSdWav_sdHeartSound_B.isPlaying() ) && ( !playRaw_sdHeartSound_B.isPlaying() ) && !synthHB )
  {
    if ( !stoppedB )
    {
      playSdWav_sdHeartSound_B.stop();
      playRaw_sdHeartSound_B.stop();
      stoppedB = true;
      Serial.print( fileB ); Serial.print( " ended - " ); Serial.println( elapsed/1000.0 );
    }
  }
  if ( stoppedA && stoppedB && playing )
  {
    playing = false;
    if ( synthHB ) synthHB = EndHB();
    Serial.print( "*all play stopped* - " ); Serial.println( elapsed/1000.0 );
  }
  else returnState = true;
  return returnState;
}

