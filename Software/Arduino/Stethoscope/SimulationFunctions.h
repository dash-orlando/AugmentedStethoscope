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

//
// *** Identify within structure
//
int     indexPly;
String  file;
int listPlyID( String file )
{
  Serial.print( " Using file " );
  Serial.println( file );
  Serial.println( ses.lenPly ); 
  for( int i = 0; i < ses.lenPly; i ++ )
  {
    if( ses.filePly[i] == file )
    {
      indexPly = i;
      Serial.print( "AOK: File found under index = " );
      Serial.println( indexPly );
      break;
    }
    else if( i == ses.lenPly - 1 )
    {
      indexPly = -1;
      Serial.println( "ERR: File not found within Play List" );
    }
  }

  return indexPly;
  
} // End of listPlyID()

//
// *** Blend Synthetic, Aortic Stenosis... 
//
String audioBlend( int indexPly )
{
  if ( indexPly == -1 )
  {
    Serial.println( "ERR: Non-existent file cannot be played" );
    fileName = "";
  }
  else
  {
    Serial.println( ses.lenPly );
    Serial.println( ses.filePly[indexPly] );
    fileName = ses.filePly[indexPly];
    //startBlending( fileName );
    //Serial.println( fileName );
  }
   
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
//String simFileName = ses.filePly2;
String simFileName = ses.filePly[0];
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
  //simFileName = ses.filePly2;
  simFileName = ses.filePly[0];
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

