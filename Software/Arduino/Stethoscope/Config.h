// Contents of the Config.h file:


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
    
    ses = (Session)
                  {
                      SID,
                      "RECORD.RAW",              // 
                      "NORMAL.RAW",              // Normal Heart Beat
                      "ESMURE.RAW",               // Early Systolic Heart Murmur
                      "EDMURE.RAW",               // Early Diastolic Heart Murmur
                      "PEJECT.RAW",
                      "EDMUR.RAW",
                      "ESMUR.RAW",
                      "HRATE.DAT"                // 
                  };
}
