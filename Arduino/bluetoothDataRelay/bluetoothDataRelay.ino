/*
 * bluetoothDataRelay.ino
 * 
 * The following script has been design to test bluetooth communication using the Teensy 3.2 board
 * 
 * Fluvio L Lobo Fenoglietoo 08/29/2016
 */

int ledPin = 13;

void setup() {
  Serial.begin(115200);

}

void loop() {

  Serial.print("hola");
  Serial.print("\n");
  
}
