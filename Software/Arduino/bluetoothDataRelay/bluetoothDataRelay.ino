/*
 * bluetoothDataRelay.ino
 * 
 * The following script has been design to test bluetooth communication using the Teensy 3.2 board
 * 
 * Fluvio L Lobo Fenoglietoo 08/29/2016
 */
char INBYTE;
int  LED = 13; // LED on pin 13

HardwareSerial bt = HardwareSerial();

void setup() {
  pinMode(LED, OUTPUT);
  
  bt.begin(115200);
  bt.println(); 
}

void loop() {
  bt.println("Press 1 to turn Arduino pin 13 LED ON or 0 to turn it OFF:");
  //while (!bt.available());   // stay here so long as COM port is empty   
  //INBYTE = bt.read();        // read next available byte
  //if( INBYTE == '0' ) digitalWrite(LED, LOW);  // if it's a 0 (zero) tun LED off
  //if( INBYTE == '1' ) digitalWrite(LED, HIGH); // if it's a 1 (one) turn LED on
  //delay(50);
}
