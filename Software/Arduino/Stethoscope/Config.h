// ==============================================================================================================
// SD Files Configuration
//
// The following script contains the configuration information for the contents of the SD card
//
// Michael Xynidis
// Fluvio L Lobo Fenoglietto 05/07/2018
// ============================================================================================================== //


// ==============================================================================================================
// Session (structure)
//
// Michael Xynidis
// Fluvio L Lobo Fenoglietto 05/07/2018
// ============================================================================================================== //
struct Session
{
    String    fileRec;
    String    filePly[ 11 ];
    int       lenPly;
    byte      byteList[ 11 ];
    int       lenByteList;
};

Session  ses;

// ==============================================================================================================
// Session Initialization
//
// Michael Xynidis
// Fluvio L Lobo Fenoglietto 05/07/2018
// ============================================================================================================== //
void SessionInit()
{
    ses.fileRec     =  "RECORD.RAW";
    ses.filePly[0]  =  "AORSTE.RAW";
    ses.filePly[1]  =  "S4GALL.RAW";
    ses.filePly[2]  =  "ESMSYN.RAW";
    ses.filePly[3]  =  "KOROT1.RAW";
    ses.filePly[4]  =  "KOROT2.RAW";
    ses.filePly[5]  =  "KOROT3.RAW";
    ses.filePly[6]  =  "KOROT4.RAW";
    ses.filePly[7]  =  "RECAOR.RAW";
    ses.filePly[8]  =  "RECMIT.RAW";
    ses.filePly[9]  =  "RECPUL.RAW";
    ses.filePly[10] =  "RECTRI.RAW";

    ses.lenPly      = sizeof(ses.filePly)/sizeof(ses.filePly[0]);

    for( int i = 50; i < 61; i ++ ) {
      ses.byteList[i] = (byte)i;
    }
    //ses.byteList[0] = 0x72;
    //ses.byteList[1] = 0x73;

    Serial.println( ses.byteList[52] );
    Serial.println( ses.filePly[8] );

    ses.lenByteList = sizeof(ses.byteList)/sizeof(ses.byteList[0]);
    Serial.println( ses.lenByteList );
     
}

