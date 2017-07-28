#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>

LSM9DS1 imu;
#define LSM9DS1_M           0x1E    // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG          0x6B    // Would be 0x6A if SDO_AG is LOW
#define PRINT_SPEED          40
#define CALIBRATION_INDEX    50    // Accounting for ambient magnetic fields
#define DECLINATION         6.55    // Accounting for the Earth's magnetic field

static byte setSensor = 0;
static unsigned long lastPrint = 0;
static double cal_imu0_x, cal_imu0_y, cal_imu0_z, cal_imu1_x, cal_imu1_y, cal_imu1_z, d0, d1;

void setup() {
  Serial.begin(115200);

  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  digitalWrite(12, LOW);
  digitalWrite(11, LOW);

  imu.settings.device.commInterface = IMU_MODE_I2C;
  imu.settings.device.mAddress = LSM9DS1_M;
  imu.settings.device.agAddress = LSM9DS1_AG;


  // Initialize sensor 0
  digitalWrite(13, LOW);
  if (!imu.begin()) {
    Serial.println("Failed to communicate with LSM9DS1 0.");
    while (1);
  } else {
    
    //Serial.println("Calibrating. Please Wait.");
    double imux_hold = 0, imuy_hold = 0, imuz_hold = 0;

    for (int i = 0; i < CALIBRATION_INDEX ; i++)
    {
      delay(50);
      while ( !imu.magAvailable() );
      imu.readMag();

      imux_hold += imu.calcMag(imu.mx);
      imuy_hold += imu.calcMag(imu.my);
      imuz_hold += imu.calcMag(imu.mz);

      if (i == CALIBRATION_INDEX - 1) {
        cal_imu0_x = imux_hold / CALIBRATION_INDEX;
        cal_imu0_y = imuy_hold / CALIBRATION_INDEX;
        cal_imu0_z = imuz_hold / CALIBRATION_INDEX;
      };
    };
  };

  // Initialize sensor 1
  digitalWrite(13, HIGH);
  if (!imu.begin()) {
    Serial.println("Failed to communicate with LSM9DS1 0.");
    while (1);
  } else {
    double imux_hold = 0, imuy_hold = 0, imuz_hold = 0;

    for (int i = 0; i < CALIBRATION_INDEX ; i++)
    {
      delay(50);
      while ( !imu.magAvailable() );
      imu.readMag();

      imux_hold += imu.calcMag(imu.mx);
      imuy_hold += imu.calcMag(imu.my);
      imuz_hold += imu.calcMag(imu.mz);

      if (i == CALIBRATION_INDEX - 1) {
        cal_imu1_x = imux_hold / CALIBRATION_INDEX;
        cal_imu1_y = imuy_hold / CALIBRATION_INDEX;
        cal_imu1_z = imuz_hold / CALIBRATION_INDEX;
      };
    };
  };

  //Serial.println("Success.");
  //  Serial.println("Calibration: ");
  //  Serial.println(cal_imu0_x, 5);
  //  Serial.println(cal_imu0_y, 5);
  //  Serial.println(cal_imu0_z, 5);
  //  Serial.println(cal_imu1_x, 5);
  //  Serial.println(cal_imu1_y, 5);
  //  Serial.println(cal_imu1_z, 5);
}

void loop() {
double imux0, imuy0, imuz0, imuB0, factor0, imux1, imuy1, imuz1, imuB1, factor1;
  
  //Sensor Selector Loop
  if (setSensor == 0) {
    
    digitalWrite(13, LOW);
    delay(PRINT_SPEED / 2);

    while ( !imu.magAvailable() );
    imu.readMag();

    imux0=double(imu.calcMag(imu.mx));
    imuy0=double(imu.calcMag(imu.my));
    imuz0=double(imu.calcMag(imu.mz));
//    imuB0=sqrt(pow(imux0,2) + pow(imuy0,2) + pow(imuz0,2));
//    factor0=pow(imuB0,0.294);
//    d0=95.198/factor0;

//    Serial.print(imuB0, 5);
//    Serial.print(", ");
//    Serial.print(d0, 4);
//    Serial.print(", ");
    
    delay(PRINT_SPEED / 2);
    Serial.print((imu.calcMag(imu.mx) - cal_imu0_x), 5);
    Serial.print(", ");
    Serial.print((imu.calcMag(imu.my) - cal_imu0_y), 5);
    Serial.print(", ");
    Serial.print((imu.calcMag(imu.mz) - cal_imu0_z), 5);
    Serial.print(", ");
  }
  else if (setSensor == 1) {
    digitalWrite(13, HIGH);
    delay(PRINT_SPEED / 2);
    
    while ( !imu.magAvailable() );
    imu.readMag();

    imux1=double(imu.calcMag(imu.mx));
    imuy1=double(imu.calcMag(imu.my));
    imuz1=double(imu.calcMag(imu.mz));
//    imuB1=sqrt(pow(imux1,2) + pow(imuy1,2) + pow(imuz1,2));
//    factor1=pow(imuB1,0.294);
//    d1=95.198/factor1;
//    
//    Serial.print(imuB1, 5);
//    Serial.print(", ");
//    Serial.print(d1, 4);
//    Serial.print(", ");
//    Serial.println();
//    
    delay(PRINT_SPEED / 2);
    Serial.print((imu.calcMag(imu.mx) - cal_imu1_x), 5);
    Serial.print(", ");
    Serial.print((imu.calcMag(imu.my) - cal_imu1_y), 5);
    Serial.print(", ");
    Serial.print((imu.calcMag(imu.mz) - cal_imu1_z), 5);
  }
  if (setSensor == 0) {
    setSensor++;
  }
  else {
    Serial.print("\n");
    setSensor = 0;
  }
}

