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
    String    sessionID;
    String    fileRec;
    String    filePly1;
    String    filePly2;
    String    filePly3;
    String    filePly4;
    String    filePly5;
    String    filePly6;
    String    filePly7;
    String    filePly8;
    String    filePly9;
    String    filePly10;
    String    filePly11;
    String    fileDat;
    int       heartRate;
};

Session  ses;

// ==============================================================================================================
// Session Initialization
//
// Michael Xynidis
// Fluvio L Lobo Fenoglietto 05/07/2018
// ============================================================================================================== //
void SessionInit( String SID )
{
    /* ...read config XML (e.g. "CONFIG.XML")  */
    /* ...parse out file names and other       */
    /*    configurations data                  */
    /* ...load the data into the configuration */
    /*    object:                              */
    
    ses = (Session)                     // Note that the string must be 6 characters long
          {
              SID,
              "RECORD.RAW",             // Recording 
              "NHBSYN.RAW",             // Normal Heart Beat            - Synthetic
              "ESMSYN.RAW",             // Early Systolic Heart Murmur  - Synthetic
              "NHBREC.RAW",             // Normal Heart Beat            - Recorded
              "EHBREC.RAW",             // Exercised Heart Beat         - Recorded
              "KOROT.RAW",              // Korotkoff Sound (seg.)       - Recorded
              "EDMSYN.RAW",             // Early Diastolic Heart Murmur - Synthetic
              "ASYSYN.RAW",             // ...
              "PEJECT.RAW",             // ...
              "PSPLIT.RAW",             // ...
              "S4GALL.RAW",             // S4 Gallop
              "AORSTE.RAW",             // Severe Aortic Stenosis
              "HRATE.DAT"               // Heart Rate data file 
          };
}

