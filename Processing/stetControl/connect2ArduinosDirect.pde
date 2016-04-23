/* ========================================
 * connect2ArduinosDirect
 *
 * The following function establishes a serial connection with multiple Arduinos loaded with the StandardFirmata script.
 * Instead of using a single integer input, such as the number of Arduinos, the algorithm takes a string array with the names of the specific ports corresponding to the target Arduinos.
 *
 * Fluvio L. Lobo Fenoglietto 04/23/2016
 ======================================= */
 
public void connect2ArduinosDirect(String[] targetComPorts) {
  
  int NtargetComPorts = targetComPorts.length;
  String[] availableComPorts = Arduino.list();
  int NavailableComPorts = availableComPorts.length;
  
  int[] portsMatchVector = new int[NtargetComPorts];
  
  for (int i = 0; i < NtargetComPorts; i ++) {
    
    for (int j = 0; j < NavailableComPorts; j ++) {
      
      if (targetComPorts[i].equals(availableComPorts[j])) {
        
        portsMatchVector[i] = j;
        println(j);
        
      } // End of if statement (String Comparison)
      
    } // End of for-loop (NavailableComPorts)
    
    try {
      
      // Establiching serial connection
      arduino[i] = new Arduino(this, availableComPorts[portsMatchVector[i]], 57600);
      
      // Validate connection
      int ledPin = 9;
      arduino[i].digitalWrite(ledPin, Arduino.HIGH);    
      
    } catch(RuntimeException e) {
       
      println("Serial Port Busy");
       
    } // End of try-catch statement
    
  } // End of for-loop (NtargetComPorts)
        
} // End of connect2Arduino function