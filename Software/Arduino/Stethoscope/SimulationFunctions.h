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

// old format, without continuous blending...
void NHBSBlend(){ startBlending( ses.filePly1 ); }                                                                // Blending of a Synthetic, Normal Heart Beat
void ESMSBlend(){ startBlending( ses.filePly2 ); }                                                                // Blending of a Synthetic, Early Systolic Heart Murmur
void NHBRBlend(){ startBlending( ses.filePly3 ); }                                                                // Blending of a Recorded, Normal Heart Beat
void EHBRBlend(){ startBlending( ses.filePly4 ); }                                                                // Blending of a Recorded, Exercised Heart Beat

// new format, with continuous blending
String KOROTBlend(String fileName)                                                                                // Blending of a Recorded, Exercised Heart Beat
{
  fileName = ses.filePly5;
  startBlending( fileName );                                                      
  Serial.println( fileName );
  return fileName;
} // End of KOROTBlend() function

 //
 // *** Blend Synthetic, Early Diastolic Murmur 
 //
 String EDMSYNBlend(String fileName)
 {

  fileName = ses.filePly6;
  startBlending( fileName );                                                      // Blending of a Synthetic, Early Diastolic Murmur
  Serial.println( fileName );
  return fileName;
  
 } // End of EDMSYNBlend() function

 //
 // *** Blend Synthetic, Asys... 
 //
 String ASYSYNBlend(String fileName)
 {

  fileName = ses.filePly7;
  startBlending( fileName );                                                      // ...
  Serial.println( fileName );
  return fileName;
  
 } // End of ... function

 //
 // *** Blend Synthetic, Peject... 
 //
 String PEJECTBlend(String fileName)
 {

  fileName = ses.filePly8;
  startBlending( fileName );                                                      // ...
  Serial.println( fileName );
  return fileName;
  
 } // End of ... function

 //
 // *** Blend Synthetic, Psplit... 
 //
 String PSPLITBlend(String fileName)
 {

  fileName = ses.filePly9;
  startBlending( fileName );                                                      // ...
  Serial.println( fileName );
  return fileName;
  
 } // End of ... function
