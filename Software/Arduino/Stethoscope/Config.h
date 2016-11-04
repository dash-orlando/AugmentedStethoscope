// Contents of the Config.h file:

struct Session
{
    String    sessionID;
    String    fileRec;
    String    filePly;
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
    
    Session    ses = (Session)
                  {
                      SID,
                      "RECORD.RAW",                // from XML file
                      "",                        // from XML file
                      "HRATE.DAT"                // from XML file
                  };
}
