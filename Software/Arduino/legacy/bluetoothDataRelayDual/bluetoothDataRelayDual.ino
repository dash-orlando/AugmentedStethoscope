/*
 * bluetoothDataRelayDual.ino
 * 
 * The following script has been design to test bluetooth communication using the Teensy 3.2 board
 * 
 * Fluvio L Lobo Fenoglietoo 08/29/2016
 */


#define     HWSERIAL      Serial1
#define     ENQ           0x05

void setup() {
  Serial.begin(9600);
  HWSERIAL.begin(115200);

  //Serial.println(ENQ);
  establishContact();
}

void loop() {

  String inString;
  //HWSERIAL.println("hola");
        
  if (Serial.available() > 0) {
    inString = Serial.read();
    Serial.print("USB received: ");
    Serial.println(inString);
  }
  if (HWSERIAL.available() > 0) {
    HWSERIAL.println("hola");
  }
}

void establishContact()
{
  while ( true )
  {
    HWSERIAL.println("hola");
    delay( 300 );
  }
}

