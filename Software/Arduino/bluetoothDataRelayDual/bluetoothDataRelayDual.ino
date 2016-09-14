/*
 * bluetoothDataRelayDual.ino
 * 
 * The following script has been design to test bluetooth communication using the Teensy 3.2 board
 * 
 * Fluvio L Lobo Fenoglietoo 08/29/2016
 */

//#include <definitions.ino>
// Variables
#define     HWSERIAL      Serial2

void setup() {
  Serial.begin(9600);
  HWSERIAL.begin(115200);

  Serial.println(ENQ);
  //establishContact();
}

void loop() {

  String inString;
        
//  if (Serial.available() > 0) {
//    incomingByte = Serial.read();
//    Serial.print("USB received: ");
//    Serial.println(incomingByte, DEC);
//                HWSERIAL.print("USB received:");
//                HWSERIAL.println(incomingByte, DEC);
//  }
  if (HWSERIAL.available() > 0) {
    inString = HWSERIAL.read();
    Serial.println(inString);
    if (inString.equals("REC")) {
      HWSERIAL.print("ACK");
      HWSERIAL.print("\n");
    }
  }
}

void establishContact()
{
  while ( Serial.available() <= 0 )
  {
    HWSERIAL.print("IDLE");                // send an initial string
    HWSERIAL.print("\n");
    delay( 300 );
  }
}

