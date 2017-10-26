/*
   Obtain magnetic field vector using multiplexed LSM9DS1 IMU sensors.
   Obtained values take into account the magnetic field of the earth,
   and the immediately surrounding area by taking a baseline average for each axis
   and then subtracting each them from the readings to give a calibrated reading
   independent of the ambient magnetic field.

   AUTHOR     : Edward Daniel Nichols
   DATE       : Oct. 23rd, 2017th Year of Our Lord

   AUTHOR     : Mohammad Odeh
   DATE       : Oct. 6th, 2017th Year of Our Lord

   CHANGELOG:-
    1- Both possible addresses for the IMUs are implemented.
    2- Calibration speed has significantly improved.
    3- All six sensors have been added.

    There is room to add more sensors.
*/

// Include required libraries
#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>

// Define sensor address, for the setup.
#define LSM9DS1_M_HIGH                0x1E    // SDO_M on these IMU's are HIGH
#define LSM9DS1_AG_HIGH               0x6B    // SDO_AG on these IMU's are HIGH 
#define LSM9DS1_M_LOW                 0x1C    // SDO_M on these IMU's are LOW
#define LSM9DS1_AG_LOW                0x6B    // SDO_AG on these IMU's are HIGH [PINS NOT GROUNDED]***

// Useful defines.
#define CALIBRATION_INDEX             25      // Accounting for ambient magnetic fields
#define DECLINATION                   6.55    // Accounting for the Earth's magnetic field
#define BAUDRATE                      115200  // Serial communication baudrate
#define MUX_PIN                       10      // Multiplexer "Select pin"
#define DEBOUNCE                      5       // To ensure select pin voltage has enough time to settle.

// MUX lines are on these pins.
#define S0                            8
#define S1                            9
// S2 is grounded.
// MUX lines are selected in binary.
// For example, Y0 -> S0=LOW, S1=LOW, S2=LOW -> 000.

// Sensor Calibration variables: To store the averaged baseline values for each sensor.
double cal[6][3] = {0};

// Arrays that will house the sensor orientation adjusted readings, for printing.
// See the 'sensorOrientation()' definition.
double sens[6][3] = {0};

// Initialize two instances of the LSM object; one for each M_address.
LSM9DS1 high; //Odd sensors 1, 3, and 5
LSM9DS1 low;  //Even sensors 2, 4, and 6

void setup() {
  // Start serial monitor.
  Serial.begin( BAUDRATE );

  // Initialize the necessary MUX selection pins.
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);

  for (int whichPair = 1; whichPair <= 3; whichPair++) {
    //Selecting between sensor pair 1, 2, or 3.
    pairSelect(whichPair);
    setupIMU();

    //Initialize the sensors, and print error code if failure.
    if ( !high.begin() || !low.begin() ) {
      Serial.print( F("Failed to communicate with LSM9DS1 in pair:") );
      Serial.println(whichPair);
      while (1);
    } else {

      Serial.println( F("Calibrating, please wait.") );
      delay(25);
      //Calibration function:
      double hold[6] = {0};
      for (int i = 0; i < CALIBRATION_INDEX; i++) {

        //        //DEBUG//
        //        //Where am I in the loop?
        //        Serial.println(i+1);
        //        Serial.println("*************");
        //        Serial.print("The pair is: ");
        //        Serial.print(whichPair);
        //        Serial.println();

        //Declaring an index, to make it easier to assign values to/from the correct sensor.
        int n_hi = (whichPair - 1) * 2;
        int n_lo = (2 * whichPair) - 1;

        //        //DEBUG//
        //        //Checking to see what the indices are.
        //        Serial.print("The High Sensor is: ");
        //        Serial.println(n_hi);
        //        Serial.print("The Low Sensor is: ");
        //        Serial.println(n_lo);
        //        Serial.println();
        //        delay(500);

        //Wait until sensors are available.
        delay(10);
        while ( !low.magAvailable() && !high.magAvailable() );

        //Take readings.
        high.readMag();
        low.readMag();

        //        //DEBUG//
        //        //Check to see if the readings have been taken.
        //        //These readings have not been oriented!
        //        Serial.print("These are the RAW readings from the HIGH sensor: ");
        //        Serial.print(high.calcMag(high.mx));
        //        Serial.print(", ");
        //        Serial.print(high.calcMag(high.my));
        //        Serial.print(", ");
        //        Serial.print(high.calcMag(high.mz));
        //        Serial.println();
        //        Serial.print("These are the RAW readings from the LOW sensor: ");
        //        Serial.print(low.calcMag(low.mx));
        //        Serial.print(", ");
        //        Serial.print(low.calcMag(low.my));
        //        Serial.print(", ");
        //        Serial.println(low.calcMag(low.mz));
        //        Serial.println("These should be rearranged in the next printout");

        //Reorient the readings and push to the array.
        orientRead(whichPair);

        //        //DEBUG//
        //        //Check to see if the reading have been reoriented properly.
        //        Serial.print("These are the ORIENTED readings from the HIGH sensor: ");
        //        Serial.print(sens[n_hi][0]);
        //        Serial.print(", ");
        //        Serial.print(sens[n_hi][1]);
        //        Serial.print(", ");
        //        Serial.print(sens[n_hi][2]);
        //        Serial.println();
        //        Serial.print("These are the ORIENTED readings from the LOW sensor: ");
        //        Serial.print(sens[n_lo][0]);
        //        Serial.print(", ");
        //        Serial.print(sens[n_lo][1]);
        //        Serial.print(", ");
        //        Serial.print(sens[n_lo][2]);
        //        Serial.println("\n");
        //        delay(1500);

        //Populating the temporary hold value array for the High sensor.
        hold[0] += sens[n_hi][0];
        hold[1] += sens[n_hi][1];
        hold[2] += sens[n_hi][2];

        //Populating the temporary hold value array for the Low sensor.
        hold[3] += sens[n_lo][0];
        hold[4] += sens[n_lo][1];
        hold[5] += sens[n_lo][2];

        //        //DEBUG//
        //        //Check to see if the reading have stored properly in the holding array.
        //        Serial.print("This is the running sum of the readings from the HIGH sensor: ");
        //        Serial.print(hold[0]);
        //        Serial.print(", ");
        //        Serial.print(hold[1]);
        //        Serial.print(", ");
        //        Serial.print(hold[2]);
        //        Serial.println();
        //        Serial.print("This is the running sum of the readings from the LOW sensor: ");
        //        Serial.print(hold[3]);
        //        Serial.print(", ");
        //        Serial.print(hold[4]);
        //        Serial.print(", ");
        //        Serial.print(hold[5]);
        //        Serial.println("\n\n");
        //        delay(500);

        //
        if ( i == CALIBRATION_INDEX - 1) {
          //Computing, finally, the actual calibration value for the High sensor.
          cal[n_hi][0] = hold[0] / CALIBRATION_INDEX;
          cal[n_hi][1] = hold[1] / CALIBRATION_INDEX;
          cal[n_hi][2] = hold[2] / CALIBRATION_INDEX;

          //Computing, finally, the actual calibration value for the Low sensor.
          cal[n_lo][0] = hold[3] / CALIBRATION_INDEX;
          cal[n_lo][1] = hold[4] / CALIBRATION_INDEX;
          cal[n_lo][2] = hold[5] / CALIBRATION_INDEX;

          //          //DEBUG//
          //          //Check to see if the calibration values have been properly placed.
          //          Serial.print("This is the calibration result from the HIGH sensor: ");
          //          Serial.print(cal[n_hi][0]);
          //          Serial.print(", ");
          //          Serial.print(cal[n_hi][1]);
          //          Serial.print(", ");
          //          Serial.print(cal[n_hi][2]);
          //          Serial.println();
          //          Serial.print("This is the calibration result from the LOW sensor: ");
          //          Serial.print(cal[n_lo][0]);
          //          Serial.print(", ");
          //          Serial.print(cal[n_lo][1]);
          //          Serial.print(", ");
          //          Serial.print(cal[n_lo][2]);
          //          Serial.println();
          //          Serial.print("******************************************************************ENDLOOP");
          //          Serial.println("\n\n");
          //          delay(2000);
        };
      };

      Serial.print( F("Calibration success for pair: ") );
      Serial.println(whichPair);
    };
  };
};

void loop() {
  //Serial.print("<");

  for (int whichPair = 1; whichPair <= 3; whichPair++) {
    //Choose the pair of sensors through the MUX
    pairSelect(whichPair);

    //Wait until the sensors are available.
    while (!low.magAvailable() && !high.magAvailable() );

    //Take a reading for both sensors.
    high.readMag(); //Take a reading and store it in the object LSM9DS1.high
    low.readMag();  //Take a reading and store it in the object LSM9DS1.low

    //Reorient the readings along the predefined C.S. and store in the global array.
    orientRead(whichPair);
  };

  //When all of the values have been collected, print them all at once!
  //Adjusting for the calibration baseline!
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 3; j++) {
      Serial.print(sens[i][j] - cal[i][j], 3);
      if (i == 5 && j == 2) {
        Serial.println();
        //continue;
      } else {
        Serial.print(", ");
      };
    };
  };

  //Serial.println(">");
};

// ===================    ReOrient Readings       ===================
//This is for setup A
//void orientRead(int pair) {
//  // The sensors need to output data along a consistent Coordinate System.
//  // Their orientations have been predefined; when they change this has to as well.
//  switch (pair) {
//    case 1:
//      //Sensor 1: mz -> X, -mx -> Y, my -> Z
//      sens[0][0] = double( high.calcMag(high.mz) );
//      sens[0][1] = -1 * double( high.calcMag(high.mx) );
//      sens[0][2] = double( high.calcMag(high.my) );
//
//      //Sensor 2: mz -> X, -mx -> Y, my -> Z
//      sens[1][0] = double( low.calcMag(low.mz) );
//      sens[1][1] = -1 * double( low.calcMag(low.mx) );
//      sens[1][2] = double( low.calcMag(low.my) );
//      break;
//
//    case 2:
//      //Sensor 3: mz -> X, my -> Y, mx -> Z
//      sens[2][0] = double( high.calcMag(high.mz) );
//      sens[2][1] = double( high.calcMag(high.my) );
//      sens[2][2] = double( high.calcMag(high.mx) );
//
//      //Sensor 4: mz -> X, -my -> Y, -mx -> Z
//      sens[3][0] = double( low.calcMag(low.mz) );
//      sens[3][1] = -1 * double( low.calcMag(low.my) );
//      sens[3][2] = -1 * double( low.calcMag(low.mx) );
//      break;
//
//    case 3:
//      //Sensor 5: mz -> X, mx -> Y, -my -> Z
//      sens[4][0] = double( high.calcMag(high.mz) );
//      sens[4][1] = double( high.calcMag(high.mx) );
//      sens[4][2] = -1 * double( high.calcMag(high.my) );
//
//      //Sensor 6: mz -> X, mx -> Y, -my -> Z
//      sens[5][0] = double( low.calcMag(low.mz) );
//      sens[5][1] = double( low.calcMag(low.mx) );
//      sens[5][2] = -1 * double( low.calcMag(low.my) );
//      break;
//
//    default:
//      Serial.print( F("Error. This sensor pair doesn't exist!") );
//      while (1);
//      break;
//  };
//}

//This is for setup B
void orientRead(int pair) {
  // No orientation attempt has been made. RHR is not enforced.
  // Because fuck math and physics, we're programming out of our asses!
  // Haha... I hate my life.
  switch (pair) {
    case 1:
      //Sensor 1:
      sens[0][0] = double( high.calcMag(high.mx) );
      sens[0][1] = double( high.calcMag(high.my) );
      sens[0][2] = double( high.calcMag(high.mz) );

      //Sensor 2: 
      sens[1][0] = double( low.calcMag(low.mx) );
      sens[1][1] = double( low.calcMag(low.my) );
      sens[1][2] = double( low.calcMag(low.mz) );
      break;

    case 2:
      //Sensor 3: 
      sens[2][0] = double( high.calcMag(high.mx) );
      sens[2][1] = double( high.calcMag(high.my) );
      sens[2][2] = double( high.calcMag(high.mz) );

      //Sensor 4: 
      sens[3][0] = double( low.calcMag(low.mx) );
      sens[3][1] = double( low.calcMag(low.my) );
      sens[3][2] = double( low.calcMag(low.mz) );
      break;

    case 3:
      //Sensor 5: 
      sens[4][0] = double( high.calcMag(high.mx) );
      sens[4][1] = double( high.calcMag(high.my) );
      sens[4][2] = double( high.calcMag(high.mz) );

      //Sensor 6: 
      sens[5][0] = double( low.calcMag(low.mx) );
      sens[5][1] = double( low.calcMag(low.my) );
      sens[5][2] = double( low.calcMag(low.mz) );
      break;

    default:
      Serial.print( F("Error. This sensor pair doesn't exist!") );
      while (1);
      break;
  };
}


// =======================    Select IMU pair       ====================
void pairSelect(int desiredPair) {
  int pair = desiredPair;
  if (pair == 1) {
    //s.t. the selection lines hit Y0, at 000
    digitalWrite(S0, LOW);
    digitalWrite(S1, LOW);
    delay(DEBOUNCE);
  } else if (pair == 2) {
    //s.t. the selection lines hit Y1, at 001
    digitalWrite(S0, HIGH);
    digitalWrite(S1, LOW);
    delay(DEBOUNCE);
  } else if (pair == 3) {
    //s.t. the selection lines hit Y1, at 010
    digitalWrite(S0, LOW);
    digitalWrite(S1, HIGH);
    delay(DEBOUNCE);
  } else {
    Serial.println( F("Error. This sensor pair doesn't exist!") );
    while (1);
  }
};

// =========================    Setup IMU       ========================
void setupIMU() {
  //Here we "setup" two instances of the IMU objects.

  //The one at the "HIGH" address, s.t. SDO_M is at default.
  high.settings.device.commInterface = IMU_MODE_I2C; //
  high.settings.device.mAddress      = LSM9DS1_M_HIGH;    // Load IMU settings
  high.settings.device.agAddress     = LSM9DS1_AG_HIGH;   //

  high.settings.gyro.enabled         = false;        // Disable gyro
  high.settings.accel.enabled        = false;        // Disable accelerometer
  high.settings.mag.enabled          = true;         // Enable magnetometer
  high.settings.temp.enabled         = true;         // Enable temperature sensor

  high.settings.mag.scale            = 16;           // Set mag scale to +/-12 Gs
  high.settings.mag.sampleRate       = 7;            // Set sampling rate to 80Hz
  high.settings.mag.tempCompensationEnable = true;   // Enable temperature compensation (good stuff!)

  high.settings.mag.XYPerformance    = 3;            // 0 = Low || 1 = Medium || 2 = High || 3 = Ultra-high
  high.settings.mag.ZPerformance     = 3;            // Ultra-high performance

  high.settings.mag.lowPowerEnable   = false;        // Disable low power mode
  high.settings.mag.operatingMode    = 0;            // 0 = Continuous || 1 = Single || 2 = OFF

  //The one at the "LOW" address, s.t. SDO_M is grounded.
  //SDO_AG need not be grounded, since we aren't using it.
  low.settings.device.commInterface = IMU_MODE_I2C; //
  low.settings.device.mAddress      = LSM9DS1_M_LOW;    // Load IMU settings
  low.settings.device.agAddress     = LSM9DS1_AG_LOW;   //

  low.settings.gyro.enabled         = false;        // Disable gyro
  low.settings.accel.enabled        = false;        // Disable accelerometer
  low.settings.mag.enabled          = true;         // Enable magnetometer
  low.settings.temp.enabled         = true;         // Enable temperature sensor

  low.settings.mag.scale            = 16;           // Set mag scale to +/-12 Gs
  low.settings.mag.sampleRate       = 7;            // Set sampling rate to 80Hz
  low.settings.mag.tempCompensationEnable = true;   // Enable temperature compensation (good stuff!)

  low.settings.mag.XYPerformance    = 3;            // 0 = Low || 1 = Medium || 2 = High || 3 = Ultra-high
  low.settings.mag.ZPerformance     = 3;            // Ultra-high performance

  low.settings.mag.lowPowerEnable   = false;        // Disable low power mode
  low.settings.mag.operatingMode    = 0;            // 0 = Continuous || 1 = Single || 2 = OFF
}

