/*
 * Stethoscope Control
 * The following script and subfuctions were designed to control our stethoscope prototypes
 * Fluvio L. Lobo Fenoglietto 04/23/2016
 *
 */
 
// Importing Libraries
import processing.serial.*;
import cc.arduino.*;
import controlP5.*;

// Arduinos

String[] comPorts = {"COM6"}; // This is the COM port dedicated to the paired bluetooth antenna
int Nardus = comPorts.length;

Arduino[] arduino = new Arduino[Nardus];

void setup() {
  
  size(200,200);
  background(0,0,0);
  
  // Connect to Arduino(s)
  printArray(Arduino.list());
  connect2ArduinosDirect(comPorts);
  
} // End of setup loop

void draw() {
  
  int Npins = 3;
  readAnalogDataSimple(Nardus, Npins);
  
} // End of draw loop