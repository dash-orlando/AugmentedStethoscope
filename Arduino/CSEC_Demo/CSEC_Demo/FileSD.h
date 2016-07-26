#include  <SD.h>

  Sd2Card   card;
  SdVolume  volume;
  char      buffer[512];
  boolean   status;
  int       type;
  float     size;

void sdCardCheck()
{
  Serial.println( "SD Card Test" );
  Serial.println( "------------" );

 // First, detect the card
  status = card.init(10); // Audio shield has SD card SD on pin 10
  if (status) {
    Serial.println("SD card is connected :-)");
  } else {
    Serial.println("SD card is not connected or unusable :-(");
    boboPrint( 0, 3, ">INOPERABLE SD CARD<" );
    while ( ! status ) status = card.init(10);
//    return;
  }

  type = card.type();
  if (type == SD_CARD_TYPE_SD1 || type == SD_CARD_TYPE_SD2) {
    Serial.println("Card type is SD");
  } else if (type == SD_CARD_TYPE_SDHC) {
    Serial.println("Card type is SDHC");
  } else {
    Serial.println("Card is an unknown type (maybe SDXC?)");
  }

  // Then look at the file system and print its capacity
  status = volume.init(card);
  if (!status)
  {
    boboPrint( 0, 3, ">INOPERABLE SD CARD<" );
    Serial.println("Unable to access the filesystem on this card. :-(");
    return;
  }

  size = volume.blocksPerCluster() * volume.clusterCount();
  size = size * (512.0 / 1e6); // convert blocks to millions of bytes
  Serial.print("File system space is ");
  Serial.print(size);
  Serial.println(" Mbytes.");

  // Now open the SD card normally
  status = SD.begin(10); // Audio shield has SD card CS on pin 10
  if (status) {
    Serial.println("SD library is able to access the filesystem");
  } else {
    Serial.println("SD library can not access the filesystem!");
    boboPrint( 0, 3, ">INOPERABLE SD CARD<" );
  } 
}

void printDirectory( File dir, int numTabs )
{
  int filesize = 0;
  while ( true )
  {
    File entry  =  dir.openNextFile();
    String s    = "";
 
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

  Serial.print( "Confirm file count: [" );
  Serial.print( fileCount );
  Serial.println( "]" );
//  for ( int i = 0; i < fileCount; i++ )
//  {
//    Serial.println( fileEntry[i] );
//  }
  fileCount = 0;
}


