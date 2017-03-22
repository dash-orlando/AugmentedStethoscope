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
                      "ESMUR.RAW",               // Early Systolic Heart Murmur
                      "EDMUR.RAW",               // Early Diastolic Heart Murmur
                      "PEJECT.RAW",
                      "PSPLITP.RAW",
                      "ASYSL.RAW",
                      "HRATE.DAT"                // 
                  };
}
