'''
*
* LSM9DS1 class for python based on:
* https://github.com/akimach/LSM9DS1_RaspberryPi_Library.git
*
* VERSION: 1.1
*   - ADDED   : Initial Release
*   - ADDED   : Incomplete definition for EMA filter
*
* KNOWN ISSUES:
*   - None ATM
*
* Author        :   Mohammad Odeh
* Date          :   Nov. 21st, 2017 Year of Our Lord
* Last Modified :   Jan.  3rd, 2018 Year of Our Lord
*
'''

import  numpy               as      np              # Import Numpy
import  RPi.GPIO            as      GPIO            # Use GPIO pins
from    time                import  sleep           # Sleep for stability
from    ctypes              import  *               # Import ctypes (VERY IMPORTANT)

class TheGreatMoesException(Exception):

    def __init__(self):
        Exception.__init__(self, "Number of sensors must be 1 OR an even integer. All hail Moe!")

# ------------------------------------------------------------------------

class IMU(object):

    def __init__( self, path, nSensors, pins ):
        '''
        LSM9DS1 IMU sensor.

        INPUTS:
            - path    : Absolute path to library as a string
            - nSensors: An integer type number of sensors to use
            - pins    : A list containing the BCM number of GPIO
                        pins to use for the multiplexer select lines
        '''

        # Load library and prime ctypes
        self.lib = cdll.LoadLibrary( path )
        self.lib.create.argtypes = [c_uint]
        self.lib.create.restype = c_void_p

        self.lib.begin.argtypes = [c_void_p]
        self.lib.begin.restype = None

        self.lib.calibrateMag.argtypes = [c_void_p]
        self.lib.calibrateMag.restype = None

        self.lib.magAvailable.argtypes = [c_void_p]
        self.lib.magAvailable.restype = c_int

        self.lib.readMag.argtypes = [c_void_p]
        self.lib.readMag.restype = c_int

        self.lib.getMagX.argtypes = [c_void_p]
        self.lib.getMagX.restype = c_float
        self.lib.getMagY.argtypes = [c_void_p]
        self.lib.getMagY.restype = c_float
        self.lib.getMagZ.argtypes = [c_void_p]
        self.lib.getMagZ.restype = c_float

        self.lib.calcMag.argtypes = [c_void_p, c_float]
        self.lib.calcMag.restype = c_float

        # Setup the multiplexer (enable pins)
        self.setupMux( pins )

        # Record the number of sensors being used
        self.nSensors = nSensors
        
        # Check if number of sensors is valid
        if( (nSensors != 1) and (nSensors%2 != 0) ):
            raise TheGreatMoesException
        
        # Create necessary matrices
        else:
            self.IMU_Base = np.zeros( (nSensors, 3), dtype='float64' )  # Baseline readings (to be subtracted)
            self.IMU_Raw  = np.zeros( (nSensors, 3), dtype='float64' )  # Raw readings (non calibrated)
            self.exp_avg  = np.zeros( (nSensors, 3), dtype='float64' )  # Exponential moving average array
            self.IMU = []

# ------------------------------------------------------------------------

    def setupMux( self, pins ):
        '''
        Setup multiplexer
        '''
        self.S0, self.S1, self.S2 = pins
        
        GPIO.setmode(GPIO.BCM)
        GPIO.setwarnings(False)
        GPIO.setup(self.S0, GPIO.OUT)
        GPIO.setup(self.S1, GPIO.OUT)
        GPIO.setup(self.S2, GPIO.OUT)

# ------------------------------------------------------------------------

    def selectSensor( self, sensorIndex ):
        '''
        Switch between all the sensors

        INPUTS:
            - sensorIndex: index of the sensor pair

        OUTPUT:
            - NON
        '''
        
        # Line 1, 000
        if ( sensorIndex == 0 ):
            GPIO.output(self.S0, GPIO.LOW)
            GPIO.output(self.S1, GPIO.LOW)
            GPIO.output(self.S2, GPIO.LOW)
            sleep( 0.1 )

        # Line 2, 001
        elif ( sensorIndex == 1 ):
            GPIO.output(self.S0, GPIO.HIGH)
            GPIO.output(self.S1, GPIO.LOW)
            GPIO.output(self.S2, GPIO.LOW)
            sleep( 0.1 )

        # Line 3, 010
        elif ( sensorIndex == 2 ):
            GPIO.output(self.S0, GPIO.LOW)
            GPIO.output(self.S1, GPIO.HIGH)
            GPIO.output(self.S2, GPIO.LOW)

        # Line 4, 011
        elif ( sensorIndex == 3):
            GPIO.output(self.S0, GPIO.HIGH)
            GPIO.output(self.S1, GPIO.HIGH)
            GPIO.output(self.S2, GPIO.LOW)

        # Line 5, 100
        elif ( sensorIndex == 4):
            GPIO.output(self.S0, GPIO.LOW)
            GPIO.output(self.S1, GPIO.LOW)
            GPIO.output(self.S2, GPIO.HIGH)

        # Line 6, 101
        elif (sensorIndex == 5):
            GPIO.output(self.S0, GPIO.HIGH)
            GPIO.output(self.S1, GPIO.LOW)
            GPIO.output(self.S2, GPIO.HIGH)

        else:
            print("Invalid Index")
        
# ------------------------------------------------------------------------

    def start( self, mScl=16, N_avg=50 ):
        print( "Starting %i IMUs" %self.nSensors )
        self.create()
        self.setMagScale( mScl )
        self.calibrateMag( N_avg )

# ------------------------------------------------------------------------

    def create( self ):
        '''
        Create an IMU object
        '''
        print( "Creating IMU objects..." ) ,
        # A list containing the sensors will alternate between HIGH
        # and LOW addresses (HIGH==even #s, LOW==odd #s)

        agAddrHi, mAddrHi = 0x6b, 0x1e
        agAddrLo, mAddrLo = 0x6b, 0x1c
        for i in range(0, self.nSensors):

            # Store HIGH I2C Address in even indices
            if( i%2 == 0 ):
                self.IMU.append( self.lib.create(agAddrHi, mAddrHi) )

            # Store LOW I2C Address in odd indices
            else:
                self.IMU.append( self.lib.create(agAddrLo, mAddrLo) )

            # Start the sensor
            if( self.lib.begin( self.IMU[i] ) == 0 ):
                print( "FAILED TO COMMUNICATE!" )
                quit()
        print( "DONE!" )
# ------------------------------------------------------------------------

    def setMagScale( self, mScl=16 ):
        '''
        Set scale of magnetic field readings

        INPUTS:
            - mScl: can be 4, 8, 12, or 16
        '''
        print( "Setting scale..." ) ,
        for i in range( 0, (self.nSensors/2) ):
            self.selectSensor( i )                          # Switch the select line
            
            self.lib.setMagScale( self.IMU[2 * i], mScl )   # Set Hi scale to mScl
            self.lib.setMagScale( self.IMU[2*i+1], mScl )   # Set Lo scale to mScl
        print( "DONE!" )
# ------------------------------------------------------------------------

    def calibrateMag( self, N_avg=50 ):
        '''
        Calibrate sensor using built-in function + average readings

        INPUTS:
            - N_avg: number of readings to overage over
        '''

        for i in range( 0, (self.nSensors/2) ):

            self.selectSensor( i )                      # Switch the select line
            
            self.lib.calibrateMag( self.IMU[2 * i] )    # Call built-in calibration routine
            self.lib.calibrateMag( self.IMU[2*i+1] )    # Call built-in calibration routine
            cmxHi, cmyHi, cmzHi = 0, 0, 0               # Temporary calibration ...
            cmxLo, cmyLo, cmzLo = 0, 0, 0               # ... variables for Hi & Lo

            # Perform user-built calibration to further clear noise
            print( "Performing calibration" ) ,
            for j in range(0, N_avg):
                self.lib.readMag( self.IMU[2 * i] )     # Read Hi I2C magnetic field
                self.lib.readMag( self.IMU[2*i+1] )     # Read Lo I2C magnetic field

                cmxHi += self.lib.calcMag( self.IMU[2 * i], self.lib.getMagX(self.IMU[2 * i]) )
                cmyHi += self.lib.calcMag( self.IMU[2 * i], self.lib.getMagY(self.IMU[2 * i]) )
                cmzHi += self.lib.calcMag( self.IMU[2 * i], self.lib.getMagZ(self.IMU[2 * i]) )

                cmxLo += self.lib.calcMag( self.IMU[2*i+1], self.lib.getMagX(self.IMU[2*i+1]) )
                cmyLo += self.lib.calcMag( self.IMU[2*i+1], self.lib.getMagY(self.IMU[2*i+1]) )
                cmzLo += self.lib.calcMag( self.IMU[2*i+1], self.lib.getMagZ(self.IMU[2*i+1]) )
            
            self.IMU_Base[2 * i][0] = cmxHi/N_avg           # Average the readings for ...
            self.IMU_Base[2 * i][1] = cmyHi/N_avg           # ... Hi I2C and store in ...
            self.IMU_Base[2 * i][2] = cmzHi/N_avg           # ... the calibration matrix.

            self.IMU_Base[2*i+1][0] = cmxLo/N_avg           # Average the readings for ...
            self.IMU_Base[2*i+1][1] = cmyLo/N_avg           # ... Lo I2C and store in ...
            self.IMU_Base[2*i+1][2] = cmzLo/N_avg           # ... the calibration matrix.

            print( "DONE!" )
            
            print( "Correction constant for Hi sensor %i is:" %(i+1) )
            print( "x: %.5f, y: %.5f, z: %.5f\n" %(self.IMU_Base[2 * i][0],
                                                   self.IMU_Base[2 * i][1],
                                                   self.IMU_Base[2 * i][2]) )

            print( "Correction constant for Lo sensor %i is:" %(i+1) )
            print( "x: %.5f, y: %.5f, z: %.5f\n" %(self.IMU_Base[2*i+1][0],
                                                   self.IMU_Base[2*i+1][1],
                                                   self.IMU_Base[2*i+1][2]) )

# ------------------------------------------------------------------------

    def calcMag( self ):
        '''
        Calibrated magnitude readings.
        INPUTS:
            - N: Number of sensors

        OUTPUT:
            - CALIBRATED magnetic field
        '''
        
        for i in range(0, (self.nSensors/2) ):
            self.selectSensor( i )                  # Switch the select line
            
            self.lib.readMag( self.IMU[2 * i] )     # Read Hi I2C magnetic field
            self.lib.readMag( self.IMU[2*i+1] )     # Read Lo I2C magnetic field

            mxHi = self.lib.calcMag( self.IMU[2 * i], self.lib.getMagX(self.IMU[2 * i]) )
            myHi = self.lib.calcMag( self.IMU[2 * i], self.lib.getMagY(self.IMU[2 * i]) )
            mzHi = self.lib.calcMag( self.IMU[2 * i], self.lib.getMagZ(self.IMU[2 * i]) )

            mxLo = self.lib.calcMag( self.IMU[2*i+1], self.lib.getMagX(self.IMU[2*i+1]) )
            myLo = self.lib.calcMag( self.IMU[2*i+1], self.lib.getMagY(self.IMU[2*i+1]) )
            mzLo = self.lib.calcMag( self.IMU[2*i+1], self.lib.getMagZ(self.IMU[2*i+1]) )

            self.IMU_Raw[2 * i] = np.array( (mxHi, myHi, mzHi),     # Store RAW readings for Hi I2C
                                            dtype='float64' )       # Units { G }
            self.IMU_Raw[2*i+1] = np.array( (mxLo, myLo, mzLo),     # Store RAW readings for Lo I2C
                                            dtype='float64' )       # Units { G }

        return(self.IMU_Raw - self.IMU_Base)            # Return CALIBRATED readings

# ------------------------------------------------------------------------

    def ema_filter( self, alpha=0.25 ):
        '''
        Exponential Moving Average for further smoothing of data.
        Recall that the exponential moving average has the form of: 

            s_n = ALPHA*x_n + ( 1-ALPHA )*s_{n-1}
            where 0 < ALPHA < 1 is the smoothing factor
            High ALPHA: NO smoothing.
            Low ALPHA : YES smoothing.
            VERY Low ALPHA: GREAT smoothing but less responsive to recent changes.

        INPUTS:
            - N: Number of sensors

        OUTPUT:
            - CALIBRATED magnetic field
        '''
        # Filter data
        exp_avg[sens][axis] = ALPHA*current_value + (1 - ALPHA)*exp_avg[sens][axis];
# ------------------------------------------------------------------------

path = "/home/pi/LSM9DS1_RaspberryPi_Library/lib/liblsm9ds1cwrapper.so"
nSensors = 6
muxPins = [27, 18, 17]

imus = IMU(path, nSensors, muxPins)
imus.start()
