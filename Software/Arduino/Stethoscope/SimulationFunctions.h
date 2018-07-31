/*
 * SimulationFunctions.h
 *
 * The following script comprises all of the functions used for the simulated operation of the device
 *
 * Michael Xynidis
 * Fluvio Lobo Fenoglietto
 * 10/27/2016
 */

// ==============================================================================================================
// Blending Functions
// 
// Michael Xynidis
// Fluvio L Lobo Fenoglietto 05/02/2018
// ============================================================================================================== //
String audioBlend( int indexPly )
{
  if ( indexPly == -1 )
  {
    Serial.println( ">   ERR: Non-existent file cannot be played" );
    fileName = "";
  }
  else
  {
    fileName = ses.filePly[indexPly];
    Serial.print( ">   AOK: " );
    Serial.print( fileName );
    Serial.println( " will be played" );
    startBlending( fileName );
  }
   
  return fileName;

} // End of audioBlend()
// ============================================================================================================== //
