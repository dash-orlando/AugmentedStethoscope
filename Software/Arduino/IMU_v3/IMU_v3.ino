// Include required libraries
#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>

// Define important constants
#define LSM9DS1_M             0x1E    // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG            0x6B    // Would be 0x6A if SDO_AG is LOW
#define PRINT_DELAY           200      // 40Hz data readings @ 20 [Milliseconds]
#define CALIBRATION_INDEX     50      // Accounting for ambient magnetic fields
#define DECLINATION           6.55    // Accounting for the Earth's magnetic field
#define BAUDRATE              115200  // Serial communication baudrate
#define SELECT_PIN            13      // Multiplexer "Select pin"

static byte setSensor = 0;
static unsigned long lastPrint = 0;

// Sensor 0 calibration
static double cal_imu0_x = 0,
              cal_imu0_y = 0,
              cal_imu0_z = 0;

// Sensor 1 calibration
static double cal_imu1_x = 0,
              cal_imu1_y = 0,
              cal_imu1_z = 0;

static double d0 = 0,
              d1 = 0;

// Declare sensor class
LSM9DS1 imu;

void setup() {
  Serial.begin( BAUDRATE );           // Start serial monitor

  pinMode( SELECT_PIN, OUTPUT );      // Set "Select pin" as an output

  imu.settings.device.commInterface = IMU_MODE_I2C; //
  imu.settings.device.mAddress = LSM9DS1_M;         // Load IMU settings
  imu.settings.device.agAddress = LSM9DS1_AG;       //


  // Initialize sensor 0
  digitalWrite( SELECT_PIN, LOW );
  if ( !imu.begin() ) {
    Serial.println( F("Failed to communicate with LSM9DS1 0.") );
    while (1);
  } else {
    Serial.println( F("Calibrating. Please Wait.") );
    double imux_hold = 0, imuy_hold = 0, imuz_hold = 0;

    for (int i = 0; i < CALIBRATION_INDEX ; i++) {
      delay( 50 );
      while ( !imu.magAvailable() );
      imu.readMag();

      imux_hold += imu.calcMag( imu.mx );
      imuy_hold += imu.calcMag( imu.my );
      imuz_hold += imu.calcMag( imu.mz );

      if (i == CALIBRATION_INDEX - 1) {
        cal_imu0_x = imux_hold / CALIBRATION_INDEX;
        cal_imu0_y = imuy_hold / CALIBRATION_INDEX;
        cal_imu0_z = imuz_hold / CALIBRATION_INDEX;
      };
    };
  };

  // Initialize sensor 1
  digitalWrite( SELECT_PIN, HIGH );
  if ( !imu.begin() ) {
    Serial.println( F("Failed to communicate with LSM9DS1 0.") );
    while (1);
  } else {
    double imux_hold = 0, imuy_hold = 0, imuz_hold = 0;

    for (int i = 0; i < CALIBRATION_INDEX ; i++) {
      delay( 50 );
      while ( !imu.magAvailable() );
      imu.readMag();

      imux_hold += imu.calcMag( imu.mx );
      imuy_hold += imu.calcMag( imu.my );
      imuz_hold += imu.calcMag( imu.mz );

      if (i == CALIBRATION_INDEX - 1) {
        cal_imu1_x = imux_hold / CALIBRATION_INDEX;
        cal_imu1_y = imuy_hold / CALIBRATION_INDEX;
        cal_imu1_z = imuz_hold / CALIBRATION_INDEX;
      };
    };
  };

  Serial.println( F("Success.") );
}

void loop() {
  // Initialize IMU 0 reading variables
  double  imux0 = 0,
          imuy0 = 0,
          imuz0 = 0,
          imuB0 = 0;

  // Initialize IMU 1 reading variables
  double  imux1 = 0,
          imuy1 = 0,
          imuz1 = 0,
          imuB1 = 0;

  //Sensor Selector Loop
  if (setSensor == 0) {
    digitalWrite( SELECT_PIN, LOW );  // Toggle sensor 0 ON, sensor 1 OFF
    delay( PRINT_DELAY / 2 );         // Print speed
    
    while ( !imu.magAvailable() );    // Check if sensor is ready

    // Do readings
    imu.readMag();
    imux0 = double( imu.calcMag(imu.mx) ) - cal_imu0_x;
    imuy0 = double( imu.calcMag(imu.my) ) - cal_imu0_y;
    imuz0 = double( imu.calcMag(imu.mz) ) - cal_imu0_z;

    // Print magnetic field magnitude
    //      imuB0 = sqrt(pow(imux0, 2) + pow(imuy0, 2) + pow(imuz0, 2));
    //      Serial.print(imuB0, 5);
    //      Serial.print(", ");

    // Print readings to Serial
    Serial.print(imux0, 5);
    Serial.print(", ");
    Serial.print(imuy0, 5);
    Serial.print(", ");
    Serial.print(imuz0, 5);
    Serial.print(", ");
  }

  else if (setSensor == 1) {
    digitalWrite( SELECT_PIN, HIGH ); // Toggle sensor 1 ON, sensor 0 OFF
    delay( PRINT_DELAY / 2 );         // Print speed
    
    while ( !imu.magAvailable() );    // Check if sensor is ready

    // Do readings
    imu.readMag();
    imux1 = double( imu.calcMag(imu.mx) ) - cal_imu1_x;
    imuy1 = double( imu.calcMag(imu.my) ) - cal_imu1_y;
    imuz1 = double( imu.calcMag(imu.mz) ) - cal_imu1_z;

    //      imuB1 = sqrt(pow(imux1, 2) + pow(imuy1, 2) + pow(imuz1, 2));
    //      Serial.print(imuB1, 5);
    //      Serial.print(", ");

    // Print readings to Serial
    Serial.print(imux1, 5);
    Serial.print(", ");
    Serial.print(imuy1, 5);
    Serial.print(", ");
    Serial.print(imuz1, 5);
    Serial.print("\n");
  }

  // Switch sensors
  if (setSensor == 0) setSensor++;
  else setSensor = 0;

}

