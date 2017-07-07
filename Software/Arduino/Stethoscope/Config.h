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
    
    ses = (Session)                             // Note that the string must be 6 characters long
                  {
                      SID,
                      "RECORD.RAW",             // Recording 
                      "NORMAL.RAW",             // Normal Heart Beat
                      "ESMURE.RAW",             // Early Systolic Heart Murmur
                      "EDMURE.RAW",             // Early Diastolic Heart Murmur
                      "PEJECT.RAW",
                      "NHBREC.RAW",
                      "EHBREC.RAW",
                      "HRATE.DAT"                // 
                  };
}
