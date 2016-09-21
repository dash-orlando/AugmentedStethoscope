enum State
{
  CONNECTED, DISCONNECTED, RECORDING, PLAYING, STANDBY, READY, NOTREADY
};

State connectState  = DISCONNECTED;
State recordState   = STANDBY;
State readyState;


File      frec;
int       mode      = 0;                      // 0 = idle; 1 = record; 2 = play
const int myInput   = AUDIO_INPUT_MIC;


String stateToText( int state )
{
  String value = "";
  switch ( state )
  {
    case CONNECTED :
      value = "CONNECTED";
      break;
    case DISCONNECTED :
      value = "DISCONNECTED";
      break;
    case RECORDING :
      value = "RECORDING";
      break;
    case PLAYING :
      value = "PLAYING";
      break;
    case STANDBY :
      value = "STANDBY";
      break;
    case READY :
      value = "READY";
      break;
    case NOTREADY :
      value = "NOTREADY";
      break;
    default:
      value = "unknown state";
      break;
  }
  return value;
}
    

void adjustMicLevel()
{
  // TODO: read the peak1 object and adjust sgtl5000_1.micGain()
  // if anyone gets this working, please submit a github pull request :-)
}


boolean startRecording()
{
  Serial.println( "startRecording" );
  if ( SD.exists( "RECORD.RAW" ) )
  {
    // The SD library writes new data to the end of the
    // file, so to start a new recording, the old file
    // must be deleted before new data is written.
    SD.remove( "RECORD.RAW" );
  }
  frec = SD.open( "RECORD.RAW", FILE_WRITE );
  Serial.println( frec );
  if ( frec )
  {
    queue1.begin();
    recordState = RECORDING;
    return true;
  }
  else
    return false;
}


boolean stopRecording()
{
  Serial.println( "stopRecording" );
  queue1.end();
  if ( recordState == RECORDING )
  {
    while ( queue1.available() > 0 )
    {
      frec.write( (byte*)queue1.readBuffer(), 256 );
      queue1.freeBuffer();
    }
    frec.close();
  }
  recordState = STANDBY;
  return true;
}


boolean startPlaying()
{
  Serial.println( "startPlaying" );
  playRaw1.play( "RECORD.RAW" );
  recordState = PLAYING;
  return true;
}


boolean stopPlaying()
{
  Serial.println( "stopPlaying" );
  if ( recordState == PLAYING ) playRaw1.stop();
  recordState = STANDBY;
  return true;
}

