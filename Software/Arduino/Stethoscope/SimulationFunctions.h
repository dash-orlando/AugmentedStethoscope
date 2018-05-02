/*
 * SimulationFunctions.h
 *
 * The following script comprises all of the functions used for the simulated operation of the device
 *
 * Michael Xynidis
 * Fluvio Lobo Fenoglietto
 * 10/27/2016
 */


 //
 // *** Blend Synthetic, Normal Heart Beat
 //
 void NHBSBlend()
 {
  
  startBlending( ses.filePly1 );                                                      // Blending of a Synthetic, Normal Heart Beat
  
 } // End of NHBSBlend() function


 //
 // *** Blend Synthetic, Early Systolic Heart Mumur
 //
 void ESMSBlend()
 {
  
  startBlending( ses.filePly2 );                                                      // Blending of a Synthetic, Early Systolic Heart Murmur
  
 } // End of ESMSBlend() function

 //
 // *** Blend Recorded, Normal Heart Beat
 //
 void NHBRBlend()
 {
  
  startBlending( ses.filePly3 );                                                      // Blending of a Recorded, Normal Heart Beat
  
 } // End of NHBRBlend() function

  //
 // *** Blend Recorded, Exercised Heart Beat
 //
 void EHBRBlend()
 {
  
  startBlending( ses.filePly4 );                                                      // Blending of a Recorded, Exercised Heart Beat
  
 } // End of EHBRBlend() function

 //
 // *** Blend Recorded, Korotkoff sound
 //
 String KOROTBlend(String fileName)
 {

  fileName = ses.filePly5;
  startBlending( fileName );                                                      // Blending of a Recorded, Exercised Heart Beat
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


