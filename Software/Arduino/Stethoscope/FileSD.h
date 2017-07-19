/*
 * FileSD
 * 
 * The following program has been built to verify the presence and functionality of an SD card
 * 
 * Michael Xynidis
 * Fluvio L Lobo Fenoglietto
 * 09/27/2016
 */


// Variables
Sd2Card   card;
SdVolume  volume;
char      buffer[512];
boolean   status;
int       type;
float     size;
File      rootDir;

// Functions
boolean sdCardCheck()
{
  // Configure SPI for the audio shield pins
  SPI.setMOSI( 7 );  // Audio shield has MOSI on pin 7
  SPI.setSCK( 14 );  // Audio shield has SCK on pin 14

  
  Serial.println( "SD Card Test" );
  //Serial1.println( "SD Card Test" );

 // First, detect the card
  status = card.init( SPI_FULL_SPEED, 10 ); // Audio shield has SD card SD on pin 10
  if ( status )
  {
    Serial.println( "SD card is connected" );
  }
  else
  {
    Serial.println( "SD card is not connected or unusable" );
    return false;
  }

  type = card.type();
  if ( ( type == SD_CARD_TYPE_SD1 ) || ( type == SD_CARD_TYPE_SD2 ) )
  {
    Serial.println( "Card type is SD" );
  } 
  else if ( type == SD_CARD_TYPE_SDHC ) 
  {
    Serial.println( "Card type is SDHC" );
  }
  else 
  {
    Serial.println( "Card is an unknown type (maybe SDXC?)" );
  }

  // Then look at the file system and print its capacity
  status = volume.init( card );
  if ( !status )
  {
    // boboPrint( 0, 3, ">INOPERABLE SD CARD<" );
    Serial.println( "Unable to access the filesystem on this card" );
    return false;
  }
  else 
  {
    size = volume.blocksPerCluster() * volume.clusterCount();
    size = size * (512.0 / 1e6); // convert blocks to millions of bytes
    Serial.print( "File system space is " );
    Serial.print( size );
    Serial.println( " Mbytes." );
  }
  SD.begin( 10 );
  return true;
}


void printDirectory( File dir, int numTabs )
{
  int filesize = 0;
  Serial.println( "Directory of files on SD:" );
  while ( true )
  {
    File    entry =  dir.openNextFile();
    String  s     = "";

    if ( ! entry ) 
    {
      break;
    }
    s = entry.name();
    if ( ( s.indexOf( "WAV" ) > -1 ) | ( s.indexOf( "RAW" ) > -1 ) )
    {
      for (uint8_t i = 0; i < numTabs; i++) {
        Serial.print('\t');
      }
      Serial.print(entry.name());
      if (entry.isDirectory()) {
        Serial.println("/");
        printDirectory(entry, numTabs + 1);
      } else {
        // files have sizes, directories do not
        Serial.print("\t");
  
        filesize = entry.size();
        if ( filesize >= 1048576 )
        {
          Serial.print( filesize / 1048576, DEC );
          Serial.println( " MB" );
        }
        else if ( filesize >= 1024 )
        {
          Serial.print( filesize / 1024, DEC );
          Serial.println( " KB" );
        }
        else
        {
          Serial.print( filesize, DEC );
          Serial.println( " B" );
        }
      }
    }
    entry.close();
  }
}

int const arrayElements = 20;
int const lcdRows       =  4;
int       fileCount     =  0;
int       lcdPtr        =  1;
int       listPtr       =  0;
int       endPtr        =  0;
String    fileEntry[arrayElements];


void loadDirectory( File dir )
{
  while ( true )
  {
    File entry  =  dir.openNextFile();
    String s    = "";
 
    if ( ( ! entry ) | ( fileCount >= arrayElements ) ) 
    {
      break;
    }
    s = entry.name();
    if ( ( s.indexOf( "WAV" ) > -1 ) | ( s.indexOf( "RAW" ) > -1 ) )
    {
      fileEntry[fileCount] = s;
      fileCount++;
    }
    endPtr = fileCount - 1;
    entry.close();
  }

/*
  lcd.clear();
  boboPrint( 1, 0, " back       " );
  boboPrint( 1, 1, "            " );
  boboPrint( 1, 2, "            " );
  boboPrint( 0, 3, "    >sound files<   " );
  lcd.setCursor( 7, 0 );
  lcd.write( 3 );

  for ( int i = 1; i < lcdRows - 1; i++ )
  {
    boboPrint( 2, i, "            " );
    boboPrint( 2, i, fileEntry[i-1] );
  }
  boboPrint( 15, 0, "L=Up " );
  boboPrint( 15, 1, "M=Sel" );
  boboPrint( 15, 2, "R=Dwn" );
  
  lcd.setCursor( 0, lcdPtr );
  lcd.write( 1 );
  // */

  Serial.print( "Confirm file count: [" );
  Serial.print( fileCount );
  Serial.println( "]" );
//  for ( int i = 0; i < fileCount; i++ )
//  {
//    Serial.println( fileEntry[i] );
//  }
  fileCount = 0;
}


void sendFileSerial( File  file )
{
//  File  file              = SD.open( filename );

  int   dataSize          = file.size();
  int   headerSize        =    44;
  int   dataChunkSize     = dataSize + headerSize - 8;

  int   subDataChunkSize  =    16;
  byte  format            =   0x1;                                //PCM
  byte  channels          =   0x1;                                //mono
  int   sampleRate        = 44100;
  int   bytesPerSample    =     4;
  int   bitsPerSample     =    16;

  byte  b;
  bool  reading           = true;

  BTooth.write( "RIFF" );                                         // 00 - RIFF
  for( int n = 0; n < 4; ++n )
    BTooth.write( (byte)((dataChunkSize >> (n * 8)) & 0xFF) );    // 04 - how big is the rest of this file?
  BTooth.write( "WAVE" );                                         // 08 - WAVE
  BTooth.write( "fmt ");                                          // 12 - fmt
  for( int n = 0; n < 4; ++n )
    BTooth.write( (byte)((subDataChunkSize >> (n * 8)) & 0xFF) ); // 16 - size of this chunk
  for( int n = 0; n < 2; ++n )
    BTooth.write( (byte)((format >> (n * 8)) & 0xFF) );           // 20 - what is the audio format? 1 for PCM = Pulse Code Modulation
  for( int n = 0; n < 2; ++n )
    BTooth.write( (byte)((channels >> (n * 8)) & 0xFF) );         // 22 - mono or stereo? 1 or 2? (or 5 or ???)
  for( int n = 0; n < 4; ++n )
    BTooth.write( (byte)((sampleRate >> (n * 8)) & 0xFF) );       // 24 - samples per second (numbers per second)
  for( int n = 0; n < 4; ++n )
    BTooth.write( (byte)(((sampleRate * bytesPerSample) >> (n * 8)) & 0xFF) ); // 28 - bytes per second
  for( int n = 0; n < 2; ++n )
    BTooth.write( (byte)(((bytesPerSample) >> (n * 8)) & 0xFF) ); // 32 - # of bytes in one sample, for all channels
  for( int n = 0; n < 2; ++n )
    BTooth.write( (byte)((bitsPerSample >> (n * 8)) & 0xFF) );    // 34 - how many bits in a sample(number)? usually 16 or 24
  BTooth.write("data");                       // 36 - data
  for( int n = 0; n < 4; ++n )
    BTooth.write( (byte)((dataSize >> (n * 8)) & 0xFF) );         // 40 - how big is this data chunk

  // 44 - the actual data itself
  do
  {
    if( file.available() )
    {
      b = file.read();
      BTooth.write( b );
    }
    else reading = false;
  }
  while( reading ) ;

  file.close();
 }



