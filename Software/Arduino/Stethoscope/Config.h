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
    String    filePly[2];
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
    ses.fileRec = "RECORD.RAW";
    ses.filePly[0] = "NHBSYN.RAW";
    ses.filePly[1] = "ESMSYN.RAW";
}

