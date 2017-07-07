// Contents of the Config.h file:


struct Session
{
    String    sessionID;
    String    fileRec;
    String    filePly1;
    String    filePly2;
    String    filePly3;
    String    filePly4;
    String    fileDat;
    int       heartRate;
};

Session  ses;


void SessionInit( String SID )
{
    /* ...read config XML (e.g. "CONFIG.XML")  */
    /* ...parse out file names and other       */
    /*    configurations data                  */
    /* ...load the data into the configuration */
    /*    object:                              */
    
    ses = (Session)                             // Note that the string must be 6 characters long
                  {
                      SID,
                      "RECORD.RAW",             // Recording 
                      "NHBSYN.RAW",             // Normal Heart Beat            - Synthetic
                      "ESMSYN.RAW",             // Early Systolic Heart Murmur  - Synthetic
                      "NHBREC.RAW",             // Normal Heart Beat            - Recorded
                      "EHBREC.RAW",             // Exercised Heart Beat         - Recorded
                      "HRATE.DAT"               // Heart Rate data file 
                  };
}
