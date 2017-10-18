#!/usr/bin/env python
# coding: utf-8

#import ctypes
import  RPi.GPIO    as      GPIO
from    time        import  sleep
from    ctypes      import  *

NPINS = 3
NSENS = 6

# Setup IMU settings
path = "../lib/liblsm9ds1cwrapper.so"
lib = cdll.LoadLibrary(path)

lib.lsm9ds1_create.argtypes = []
lib.lsm9ds1_create.restype = c_void_p

lib.lsm9ds1_begin.argtypes = [c_void_p]
lib.lsm9ds1_begin.restype = None

lib.lsm9ds1_calibrate.argtypes = [c_void_p]
lib.lsm9ds1_calibrate.restype = None

lib.lsm9ds1_magAvailable.argtypes = [c_void_p]
lib.lsm9ds1_magAvailable.restype = c_int

lib.lsm9ds1_readMag.argtypes = [c_void_p]
lib.lsm9ds1_readMag.restype = c_int

lib.lsm9ds1_getMagX.argtypes = [c_void_p]
lib.lsm9ds1_getMagX.restype = c_float
lib.lsm9ds1_getMagY.argtypes = [c_void_p]
lib.lsm9ds1_getMagY.restype = c_float
lib.lsm9ds1_getMagZ.argtypes = [c_void_p]
lib.lsm9ds1_getMagZ.restype = c_float

lib.lsm9ds1_calcMag.argtypes = [c_void_p, c_float]
lib.lsm9ds1_calcMag.restype = c_float

# Setup multiplexer
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(23, GPIO.OUT)
GPIO.setup(24, GPIO.OUT)
GPIO.setup(25, GPIO.OUT)

# Switch between the multiplexer channels (sensors)
# =========================    Set Sensor      ========================
# Set the value on the multiplexer to set the sensor to use
def setSensor( sensorIndex ):
    # Sensor 1, 000
    if ( sensorIndex == 0 ):
        GPIO.output(23, GPIO.LOW)
        GPIO.output(24, GPIO.LOW)
        GPIO.output(25, GPIO.LOW)

    # Sensor 2, 001
    elif ( sensorIndex == 1 ):
        GPIO.output(23, GPIO.HIGH)
        GPIO.output(24, GPIO.LOW)
        GPIO.output(25, GPIO.LOW)

    # Sensor 3, 010
    elif ( sensorIndex == 2 ):
        GPIO.output(23, GPIO.LOW)
        GPIO.output(24, GPIO.HIGH)
        GPIO.output(25, GPIO.LOW)

    # Sensor 4, 011
    elif ( sensorIndex == 3):
        GPIO.output(23, GPIO.HIGH)
        GPIO.output(24, GPIO.HIGH)
        GPIO.output(25, GPIO.LOW)

    # Sensor 5, 100
    elif ( sensorIndex == 4):
        GPIO.output(23, GPIO.LOW)
        GPIO.output(24, GPIO.LOW)
        GPIO.output(25, GPIO.HIGH)

    # Sensor 6, 101
    elif (sensorIndex == 5):
        GPIO.output(23, GPIO.HIGH)
        GPIO.output(24, GPIO.LOW)
        GPIO.output(25, GPIO.HIGH)

    else:
        print("Invalid Index")

# ---------------------------------------------

B = [ [0, 0, 0],
      [0, 0, 0],
      [0, 0, 0],
      [0, 0, 0],
      [0, 0, 0],
      [0, 0, 0] ]

if __name__ == "__main__":
    # Initialize all IMUs
    for i in range(0, NSENS):
        setSensor( i )
        imu = lib.lsm9ds1_create()
        lib.lsm9ds1_begin(imu)
        if lib.lsm9ds1_begin(imu) == 0:
            print("Failed to communicate with LSM9DS1.")
            quit()
        lib.lsm9ds1_setMagScale(imu, 16)
        lib.lsm9ds1_calibrateMag(imu)

    # Collect data
    while True:
        for i in range(0, NSENS):
            setSensor( i )
            while lib.lsm9ds1_magAvailable(imu) == 0:
                pass
            lib.lsm9ds1_readMag(imu)

            mx = lib.lsm9ds1_getMagX(imu)
            my = lib.lsm9ds1_getMagY(imu)
            mz = lib.lsm9ds1_getMagZ(imu)

            cmx = lib.lsm9ds1_calcMag(imu, mx)
            cmy = lib.lsm9ds1_calcMag(imu, my)
            cmz = lib.lsm9ds1_calcMag(imu, mz)

            B[i][0] = cmx
            B[i][1] = cmy
            B[i][2] = cmz
        # Print solution (coordinates) to screen
        print( "    Sensor 1        Sensor 2        Sensor 3        Sensor 4        Sensor 5        Sensor 6" )
        print( "   ==========      ==========      ==========      ==========      ==========      ==========" )
        print( ">>> READING:" )
        print( "x:  %8.5f       %9.5f       %9.5f       %9.5f       %9.5f       %9.5f" %(B[0][0], B[1][0], B[2][0], B[3][0], B[4][0], B[5][0]) )
        print( "y:  %8.5f       %9.5f       %9.5f       %9.5f       %9.5f       %9.5f" %(B[0][1], B[1][1], B[2][1], B[3][1], B[4][1], B[5][1]) )
        print( "z:  %8.5f       %9.5f       %9.5f       %9.5f       %9.5f       %9.5f" %(B[0][2], B[1][2], B[2][2], B[3][2], B[4][2], B[5][2]) )
        print( "\n\n" )
        sleep( 1.0 )
