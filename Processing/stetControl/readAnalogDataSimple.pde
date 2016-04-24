/* =====================================
 * readAnalogDataSimple
 *
 * The following function reads the analog iput data from multiple arduino boards.
 * The number of arduino boards to be accessed is specified by (int) "Nardus".
 * The number of analog pins to be read is specified by (int) "Npins".
 * This script was modeled from the readAnalogData function build within the Control System Repo.
 *
 * Fluvio L. Lobo Fenoglietto 04/23/2016
 ==================================== */
    
public void readAnalogDataSimple(int Nardus, int Npins) {
  
  for (int h = 0; h < Nardus; h ++) {
    
    // Variables
    int[] analogPin = new int[Npins];
    int[] analogVal = new int[Npins];
    
    // The following for-loop circles around all the analog ports of the micro-controller board
    for (int i = 0; i < Npins; i ++) {
      
      // Reading and storing analog data into local variables
      analogPin[i] = i; // Define analog pin in array 
      analogVal[i] = arduino[h].analogRead(analogPin[i]); // Read analog value of respective pin
      
    } // End of for-loop "Analog Ports Loop"
    
    printArray(analogVal);
  
  } // End of for-loop "Arduinos Loop"
    
} // End of readSensorData function