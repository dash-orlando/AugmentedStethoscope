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

// ==============================================================================================================
// Combined Functions
// * simulation
// The following functions combined several stethoscope features such as:
// -- recording & blending
// 
// Fluvio L Lobo Fenoglietto 05/02/2018
// ============================================================================================================== //
// String simFileName    = "";
String simFileName = ses.filePly2;
boolean startSimulation()
{
  Serial.println( "EXECUTING simulation()" );
  Serial.println( "sending: ACK..." ); 
  BTooth.write( ACK );

  // start recording -------------------------------------------------------------------------------------------- //
  Serial.println( "GENERATING RECORDING FILENAMES" );
  setRecordingFilename( inString, recExtension, recMode );                                                        // Create recording string with appropriate extension
  Serial.println( "STARTING MULTI-CHANNEL RECORDING" );                                                           // Starting multi-channel recording protocol
  AudioNoInterrupts();
  startRecording( recString );
  //startMultiChannelRecording( recStrings );                                                                       // Start custom filename recording

  // start blending --------------------------------------------------------------------------------------------- //
  Serial.println( "STARTING BLENDING" );                                                                          // Starting multi-channel recording protocol
  Serial.println( "blending Aortic Stenosis..." );                                                                // Still need to find this file
  simFileName = ses.filePly2;
  startBlending( simFileName );
  AudioInterrupts();

  // switching operation mode ----------------------------------------------------------------------------------- //
  switchMode( 6 );                                                                                                // Switch to "continue" mode
  
} // End of simulation()

boolean continueSimulation()
{
  // continue recording ----------------------------------------------------------------------------------------- //
  AudioNoInterrupts();
  continueRecording();
  
  // continue blending ------------------------------------------------------------------------------------------ //
  continueBlending( simFileName );
  AudioInterrupts();

  // switching operation mode ----------------------------------------------------------------------------------- //
  //switchMode( 6 );                                                                                                // Maintain operation mode at 6
  
} // End of simulation()

boolean stopSimulation()
{
  Serial.println( "EXECUTING stopSimulation()" );
  Serial.println( "sending: ACK..." ); 
  BTooth.write( ACK );
  
  // stop recording --------------------------------------------------------------------------------------------- //
  AudioNoInterrupts();
  stopRecording();

  // stop blending  --------------------------------------------------------------------------------------------- //
  stopBlending();
  AudioInterrupts();

  // switching operation mode ----------------------------------------------------------------------------------- //
  switchMode( 6 );                                                                                                // the continue blending function will terminate the simulation and switch to mode 0
  
} // End of simulation()

