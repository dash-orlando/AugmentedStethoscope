/*
   Obtain magnetic field vector using multiplexed LSM9DS1 IMU sensors.
   Obtained values take into account the magnetic field of the earth
   and the immediate surrounding area and SUBTRACTS them from the readings
   to give a calibrated reading independent of the surrounding magnetic field.

   AUTHOR     : Edward Daniel Nichols
   DATE       : Sept. 29th, 2017, Year of Our Lord

   AUTHOR     : Mohammad Odeh
   DATE       : Sept. 29th, 2017, Year of Our Lord

   CHANGELOG:-
    1- Incorporated 4th sensor
    2- Constrained code to defined values (to add an nth sensor only
       modify #define NSENS and nothing else)
*/

// Include required libraries
#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>

// Define important constants
#define LSM9DS1_M             0x1E    // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG            0x6B    // Would be 0x6A if SDO_AG is LOW
#define CALIBRATION_INDEX     50      // Accounting for ambient magnetic fields
#define BAUDRATE              115200  // Serial communication baudrate
#define NPINS                 3       // Number of select pins
#define NSENS                 6       // Number of sensors
#define NAXES                 3       // Number of axes
#define LED                   1       // "All iz Gud" Indicator LED

LSM9DS1 imu;                          // Instantiate sensors
byte Sx_pin[3]  = {8, 9, 10};         // Select pins: {S0, S1, S2}

// Calibration (BASE) readings
static double imu_BASE[NSENS][NAXES] =  { {0, 0, 0},    //  {1x, 1y, 1z}
                                          {0, 0, 0},    //  {2x, 2y, 2z}
                                          {0, 0, 0},    //  {3x, 3y, 3z}
                                          {0, 0, 0},    //  {4x, 4y, 4z}
                                          {0, 0, 0},    //  {5x, 5y, 5z}
                                          {0, 0, 0} };  //  {6x, 6y, 6z}

// Enumerate sensor states ( 0==sensor1, 1==sensor2, ..., n==sensor(n+1) )
enum State { SEN_ERR, SEN_OK };
State sensorState[NSENS]  = { };      // Is the sensor detected and working?

void setup() {
  Serial.begin( BAUDRATE );           // Start serial monitor

  pinMode( 13, OUTPUT );              // Turn LED ON to know
  digitalWrite( 13, HIGH );           // Teensy ain't dead
  pinMode( LED, OUTPUT );             // Set LED pin to OUTPUT

  for ( byte i = 0; i < NPINS; i++ ) {
    pinMode( Sx_pin[i], OUTPUT );     // Set "Select Pins" as output
  }

  // Initialize sensors and load settings
  for (byte i = 0; i < NSENS; i++) {
    setSensor(i);                     // Switch between sensors
    setupIMU();                       // Setup IMUs
    calibrateIMU(i);                  // Calibrate Sensors
  } Serial.println( F("Success.") );  // SUCCESS!
    digitalWrite( LED, HIGH );        // Turn LED ON!
}

void loop() {

  // IMU readings (CALIBRATED) matrix
  static double B[NSENS][NAXES] = { {0, 0, 0},    //  {B1_x, B1_y, B1_z}
                                    {0, 0, 0},    //  {B2_x, B2_y, B2_z}
                                    {0, 0, 0},    //  {B3_x, B3_y, B3_z}
                                    {0, 0, 0},    //  {B4_x, B4_y, B4_z}
                                    {0, 0, 0},    //  {B5_x, B5_y, B5_z}
                                    {0, 0, 0} };  //  {B6_x, B6_y, B6_z}

  // IMU readings (RAW)
  static double imu_RAW = 0;

  // Obtain readings from ALL sensors
  for (byte i = 0; i < NSENS; i++) {      // Loop over ROWS     (Sensors)
    for (byte j = 0; j < NAXES; j++) {    // Loop over COLUMNS  (Axes)

      if (sensorState[i] == SEN_OK) {     // Set Sensor IFF it was INITIALIZED
        setSensor(i);                     // Loop over sensors
        while ( !imu.magAvailable() );    // Check if sensor is ready

        imu.readMag();                    // Do readings
        if (j == 0) {
          imu_RAW = double( imu.calcMag(imu.mx) );
        }
        else if ( j == 1) {
          imu_RAW = double( imu.calcMag(imu.my) );
        }
        else if ( j == 2) {
          imu_RAW = double( imu.calcMag(imu.mz) );
        }

        // Subtract BASE readings from RAW readings to get CALIBRATED readings
        B[i][j] =  imu_RAW - imu_BASE[i][j];
      }

      // Make output look pretty
      Serial.print(B[i][j], 5);           // Print readings to Serial
      if ( (i == NSENS - 1) && (j == NAXES - 1) ) {
        Serial.print("\n");               // Print new line after last sensor and last axis
      } else {
        Serial.print(", ");               // Print a comma between sensor axes
      }
    }
  }
}

// =========================    Setup IMU       ========================
void setupIMU() {
  imu.settings.device.commInterface = IMU_MODE_I2C; //
  imu.settings.device.mAddress      = LSM9DS1_M;    // Load IMU settings
  imu.settings.device.agAddress     = LSM9DS1_AG;   //

  imu.settings.gyro.enabled         = false;        // Disable gyro
  imu.settings.accel.enabled        = false;        // Disable accelerometer
  imu.settings.mag.enabled          = true;         // Enable magnetometer
  imu.settings.temp.enabled         = true;         // Enable temperature sensor

  imu.settings.mag.scale            = 16;           // Set mag scale to +/-12 Gs
  imu.settings.mag.sampleRate       = 7;            // Set OD rate to 80Hz
  imu.settings.mag.tempCompensationEnable = true;   // Enable temperature compensation (good stuff!)

  imu.settings.mag.XYPerformance    = 3;            // 0 = Low || 1 = Medium || 2 = High || 3 = Ultra-high
  imu.settings.mag.ZPerformance     = 3;            // Ultra-high performance

  imu.settings.mag.lowPowerEnable   = false;        // Disable low power mode
  imu.settings.mag.operatingMode    = 0;            // 0 = Continuous || 1 = Single || 2 = OFF
}

// =========================   Calibrate IMU    ========================
void calibrateIMU(byte i) {
  // Temporary value holders needed for calibration
  double imu_hold[NAXES] = {0, 0, 0};     // {x, y, z}

  if ( !imu.begin() ) {                   // Check if device is initialized
    sensorState[i] = SEN_ERR;             // Sensor ERROR

    Serial.print( F("Failed to communicate with LSM9DS1 ") );
    Serial.println( i + 1 );

  } else {
    sensorState[i] = SEN_OK;              // Sensor is OK

    Serial.println( F("Calibrating. Please Wait.") );
    imu.calibrateMag();                   // Call built-in function to calculate bias

    // Perform our own calibration process to further clear noise
    for (int j = 0; j < CALIBRATION_INDEX ; j++) {
      delay( 50 );                        // Delay for stability
      while ( !imu.magAvailable() );      // Wait until readings are available

      imu.readMag();                      // Perform readings and store into temp holders
      imu_hold[0] += imu.calcMag( imu.mx );
      imu_hold[1] += imu.calcMag( imu.my );
      imu_hold[2] += imu.calcMag( imu.mz );

      // Get average
      if (j == CALIBRATION_INDEX - 1) {
        for ( byte k = 0; k < NSENS; k++) {
          imu_BASE[i][k] = imu_hold[k] / CALIBRATION_INDEX;
        }
      }
    }
  }
}

// =========================    Set Sensor      ========================
// Set the value on the multiplexer to set the sensor to use
void setSensor( byte sensorIndex ) {
  // Sensor 1, 000
  if (sensorIndex == 0 ) {
    for (byte i = 0; i < NPINS; i++) {
      digitalWrite(Sx_pin[i], LOW);
    }
  }

  // Sensor 2, 001
  else if (sensorIndex == 1 ) {
    for (byte i = 0; i < NPINS; i++) {
      if (i == 0) {
        digitalWrite(Sx_pin[i], HIGH);
      } else digitalWrite(Sx_pin[i], LOW);
    }
  }

  // Sensor 3, 010
  else if (sensorIndex == 2) {
    for (byte i = 0; i < NPINS; i++) {
      if (i == 1) {
        digitalWrite(Sx_pin[i], HIGH);
      } else digitalWrite(Sx_pin[i], LOW);
    }
  }

  // Sensor 4, 011
  else if (sensorIndex == 3) {
    for (byte i = 0; i < NPINS; i++) {
      if (i == 2) {
        digitalWrite(Sx_pin[i], LOW);
      } else digitalWrite(Sx_pin[i], HIGH);
    }
  }

  // Sensor 5, 100
  else if (sensorIndex == 4) {
    for (byte i = 0; i < NPINS; i++) {
      if (i == 2) {
        digitalWrite(Sx_pin[i], HIGH);
      } else digitalWrite(Sx_pin[i], LOW);
    }
  }

  // Sensor 6, 101
  else if (sensorIndex == 5) {
    for (byte i = 0; i < NPINS; i++) {
      if (i == 1) {
        digitalWrite(Sx_pin[i], LOW);
      } else digitalWrite(Sx_pin[i], HIGH);
    }
  }

  else Serial.println( F("Invalid Index") );
}

