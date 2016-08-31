/*
 * bluetoothDataRelay.ino
 * 
 * The following script has been design to test bluetooth communication using the Teensy 3.2 board
 * 
 * Fluvio L Lobo Fenoglietoo 08/29/2016
 */

// Variable Definitions

#define btSerial Serial2
/* Here, the user may specify different serial ports
 * Teensy 3.2 has the following ports available;
 * Serial# :: RX#(pin) :: TX#(pin)
 * ------------------------------
 * Serial1 :: RX1(0)   :: TX1(1)
 * Serial2 :: RX2(9)   :: TX2(10)
 * Serial3 :: RX3(7)   :: TX2(8)
 */

int ledPin = 13; // Onboard LED Pin

int state = 0;
/* The variable "state" defines the modality or satet of the program
 * state = # :: name :: definition
 * state = 0 :: idle :: the program transmits identification data
 * state = 1 :: 
 */


void setup() {

  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  btSerial.begin(115200);
  
} // End of void setup

void loop() {

  //btSerial.println("hola");
  
        int incomingByte;
        
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    Serial.print("USB received: ");
    Serial.println(incomingByte, DEC);
                btSerial.print("USB received:");
                btSerial.println(incomingByte, DEC);
  }
  if (btSerial.available() > 0) {
    incomingByte = btSerial.read();
    Serial.print("UART received: ");
    Serial.println(incomingByte, DEC);
                btSerial.print("UART received:");
                btSerial.println(incomingByte, DEC);
  }
  
} // End of void loop
