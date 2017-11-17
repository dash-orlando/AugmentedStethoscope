'''
*
* Position tracking of magnet based on Finexus ported to Raspberry Pi
* https://ubicomplab.cs.washington.edu/pdfs/finexus.pdf
*
* VERSION: 0.1.1
*   - MODIFIED: Initial Release
*   - MODIFIED: Code is less horrible but still ugly af
*   - MODIFIED: Restructured code in order to streamline and
*               later on multithread code execution
*
* KNOWN ISSUES:
*   - Damn z-axis refuses to work!
*
* Author        :   Nicolas Maduro (aka Danny) 
* Last Modified :   Oct. 16th, 2017 Year of Our Lord
*
* Author        :   Mohammad Odeh 
* Last Modified :   Oct. 19th, 2017 Year of Our Lord
*
'''

# Import Modules
import  numpy               as      np              # Import Numpy
import  RPi.GPIO            as      GPIO            # Use GPIO pins
from    time                import  sleep, time     # Sleep for stability / time execution
from    scipy.optimize      import  root            # Solve System of Eqns for (x, y, z)
from    scipy.linalg        import  norm            # Calculate vector norms (magnitude)
from    Queue               import  LifoQueue       # Last in First out queue
from    threading           import  Thread          # Multithread data
from    ctypes              import  *               # Import ctypes (VERY IMPORTANT)


# ************************************************************************
# =====================> DEFINE NECESSARY FUNCTIONS <====================*
# ************************************************************************

def loadLib():
    ''' Load library and prime with ctypes '''
    global lib
    path = "/home/pi/LSM9DS1_RaspberryPi_Library/lib/liblsm9ds1cwrapper.so"
    lib = cdll.LoadLibrary(path)

    lib.lsm9ds1_create.argtypes = [c_uint]
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


# ------------------------------------------------------------------------

def setupMux():
    ''' Setup multiplexer '''
    GPIO.setmode(GPIO.BCM)
    GPIO.setwarnings(False)
    GPIO.setup(23, GPIO.OUT)
    GPIO.setup(24, GPIO.OUT)
    GPIO.setup(25, GPIO.OUT)

# ------------------------------------------------------------------------

def setSensor( sensorIndex ):
    ''' Switch between all the sensors '''
    # Sensor 1, 000
    if ( sensorIndex == 0 ):
        GPIO.output(23, GPIO.LOW)
        GPIO.output(24, GPIO.LOW)
        GPIO.output(25, GPIO.LOW)
        sleep( 0.1 )

    # Sensor 2, 001
    elif ( sensorIndex == 1 ):
        GPIO.output(23, GPIO.HIGH)
        GPIO.output(24, GPIO.LOW)
        GPIO.output(25, GPIO.LOW)
        sleep( 0.1 )

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

# ------------------------------------------------------------------------

def setupIMU_2( N ):
    ''' Setup IMUS and callibrate them'''
    global IMU_Base
    global imu
    global imu_LOW
    global imu2
    global imu_LOW2

    agAddrLow = 0x6b
    mAddrLow = 0x1c
    
    agAddrHigh = 0x6b
    mAddrHigh = 0x1e

    CALIBRATION_INDEX = 25              # Average over this many readings
    
    IMU_Base = np.empty((N,3), dtype='float64') # Construct matrix of size  (Nx3)

    # Setup sensor pair 1 and 1`
    print( "SETTING UP HIGH/LOW SENSORS 1 and 1`" )
    setSensor( 0 )
    imu = lib.lsm9ds1_create(agAddrHigh, mAddrHigh) # Create an instance
    imu_LOW = lib.lsm9ds1_create(agAddrLow, mAddrLow) # Create an instance

    if ( lib.lsm9ds1_begin(imu) or lib.lsm9ds1_begin(imu_LOW) )== 0:         # In case it fails
        print("Failed to communicate with LSM9DS1.")
        quit()

    else:                                   # In case it doesn't, configure

        lib.lsm9ds1_setMagScale(imu, 16)    # Set scale to +/-16Gauss
        lib.lsm9ds1_setMagScale(imu_LOW, 16)    # Set scale to +/-16Gauss
        lib.lsm9ds1_calibrateMag(imu)       # Call built-in calibration routine
        lib.lsm9ds1_calibrateMag(imu_LOW)       # Call built-in calibration routine

        cmx, cmy, cmz = 0, 0, 0
        cmx_LOW, cmy_LOW, cmz_LOW = 0, 0, 0

        print( "performing calibration" )
        # Perform user-built calibration to further clear noise
        for j in range(0, CALIBRATION_INDEX):
            lib.lsm9ds1_readMag(imu)
            lib.lsm9ds1_readMag(imu_LOW)

            cmx += lib.lsm9ds1_calcMag(imu, lib.lsm9ds1_getMagX(imu))
            cmy += lib.lsm9ds1_calcMag(imu, lib.lsm9ds1_getMagY(imu))
            cmz += lib.lsm9ds1_calcMag(imu, lib.lsm9ds1_getMagZ(imu))
            
            cmx_LOW += lib.lsm9ds1_calcMag(imu, lib.lsm9ds1_getMagX(imu_LOW))
            cmy_LOW += lib.lsm9ds1_calcMag(imu, lib.lsm9ds1_getMagY(imu_LOW))
            cmz_LOW += lib.lsm9ds1_calcMag(imu, lib.lsm9ds1_getMagZ(imu_LOW))

        # Average all the readings
        IMU_Base[0][0] = cmx/CALIBRATION_INDEX
        IMU_Base[0][1] = cmy/CALIBRATION_INDEX
        IMU_Base[0][2] = cmz/CALIBRATION_INDEX

        IMU_Base[1][0] = cmx_LOW/CALIBRATION_INDEX
        IMU_Base[1][1] = cmy_LOW/CALIBRATION_INDEX
        IMU_Base[1][2] = cmz_LOW/CALIBRATION_INDEX
        
        print( "Correction constant for HIGH sensor 1 is:" )
        print( "x: %.5f, y: %.5f, z: %.5f\n" %(IMU_Base[0][0], IMU_Base[0][1], IMU_Base[0][2]) )

        print( "Correction constant for LOW sensor 1` is:" )
        print( "x: %.5f, y: %.5f, z: %.5f\n" %(IMU_Base[1][0], IMU_Base[1][1], IMU_Base[1][2]) )

    # Setup sensor pair 2 and 2`
    print( "SETTING UP HIGH/LOW SENSORS 2 and 2`" )
    setSensor( 1 )
    imu2 = lib.lsm9ds1_create(agAddrHigh, mAddrHigh) # Create an instance
    imu_LOW2 = lib.lsm9ds1_create(agAddrLow, mAddrLow) # Create an instance

    if ( lib.lsm9ds1_begin(imu2) or lib.lsm9ds1_begin(imu_LOW2) )== 0:         # In case it fails
        print("Failed to communicate with LSM9DS1.")
        quit()

    else:                                   # In case it doesn't, configure
        
        lib.lsm9ds1_setMagScale(imu2, 16)    # Set scale to +/-16Gauss
        lib.lsm9ds1_setMagScale(imu_LOW2, 16)    # Set scale to +/-16Gauss
        lib.lsm9ds1_calibrateMag(imu2)       # Call built-in calibration routine
        lib.lsm9ds1_calibrateMag(imu_LOW2)       # Call built-in calibration routine

        cmx, cmy, cmz = 0, 0, 0
        cmx_LOW, cmy_LOW, cmz_LOW = 0, 0, 0

        # Perform user-built calibration to further clear noise
        for j in range(0, CALIBRATION_INDEX):
            lib.lsm9ds1_readMag(imu2)
            lib.lsm9ds1_readMag(imu_LOW2)

            cmx += lib.lsm9ds1_calcMag(imu2, lib.lsm9ds1_getMagX(imu2))
            cmy += lib.lsm9ds1_calcMag(imu2, lib.lsm9ds1_getMagY(imu2))
            cmz += lib.lsm9ds1_calcMag(imu2, lib.lsm9ds1_getMagZ(imu2))
            
            cmx_LOW += lib.lsm9ds1_calcMag(imu2, lib.lsm9ds1_getMagX(imu_LOW2))
            cmy_LOW += lib.lsm9ds1_calcMag(imu2, lib.lsm9ds1_getMagY(imu_LOW2))
            cmz_LOW += lib.lsm9ds1_calcMag(imu2, lib.lsm9ds1_getMagZ(imu_LOW2))

        # Average all the readings
        IMU_Base[2][0] = cmx/CALIBRATION_INDEX
        IMU_Base[2][1] = cmy/CALIBRATION_INDEX
        IMU_Base[2][2] = cmz/CALIBRATION_INDEX

        IMU_Base[3][0] = cmx_LOW/CALIBRATION_INDEX
        IMU_Base[3][1] = cmy_LOW/CALIBRATION_INDEX
        IMU_Base[3][2] = cmz_LOW/CALIBRATION_INDEX
        
        print( "Correction constant for HIGH sensor 2 is:" )
        print( "x: %.5f, y: %.5f, z: %.5f\n" %(IMU_Base[2][0], IMU_Base[2][1], IMU_Base[2][2]) )

        print( "Correction constant for LOW sensor 2` is:" )
        print( "x: %.5f, y: %.5f, z: %.5f\n" %(IMU_Base[3][0], IMU_Base[3][1], IMU_Base[3][2]) )

# ------------------------------------------------------------------------

def calcMag_2():
    ''' Collect readings from IMU '''
    B = np.zeros((4,3), dtype='float64')                # Construct matrix of size  (Nx3)

    setSensor( 0 )                                  # Select sensor ( i )

    lib.lsm9ds1_readMag(imu)                        # Read values from sensor
    lib.lsm9ds1_readMag(imu_LOW)
    
    mx = lib.lsm9ds1_calcMag(imu, lib.lsm9ds1_getMagX(imu))
    my = lib.lsm9ds1_calcMag(imu, lib.lsm9ds1_getMagY(imu))
    mz = lib.lsm9ds1_calcMag(imu, lib.lsm9ds1_getMagZ(imu))

    mx_LOW = lib.lsm9ds1_calcMag(imu_LOW, lib.lsm9ds1_getMagX(imu_LOW))
    my_LOW = lib.lsm9ds1_calcMag(imu_LOW, lib.lsm9ds1_getMagY(imu_LOW))
    mz_LOW = lib.lsm9ds1_calcMag(imu_LOW, lib.lsm9ds1_getMagZ(imu_LOW))

    B[0] = np.array( (mx, my, mz), dtype='float64' )   # Units { G }
    B[1] = np.array( (mx_LOW, my_LOW, mz_LOW), dtype='float64' )   # Units { G }

    setSensor( 1 )                                  # Select sensor ( i )

    lib.lsm9ds1_readMag(imu2)                        # Read values from sensor
    lib.lsm9ds1_readMag(imu_LOW2)
    
    mx = lib.lsm9ds1_calcMag(imu2, lib.lsm9ds1_getMagX(imu2))
    my = lib.lsm9ds1_calcMag(imu2, lib.lsm9ds1_getMagY(imu2))
    mz = lib.lsm9ds1_calcMag(imu2, lib.lsm9ds1_getMagZ(imu2))

    mx_LOW = lib.lsm9ds1_calcMag(imu_LOW2, lib.lsm9ds1_getMagX(imu_LOW2))
    my_LOW = lib.lsm9ds1_calcMag(imu_LOW2, lib.lsm9ds1_getMagY(imu_LOW2))
    mz_LOW = lib.lsm9ds1_calcMag(imu_LOW2, lib.lsm9ds1_getMagZ(imu_LOW2))

    B[2] = np.array( (mx, my, mz), dtype='float64' )   # Units { G }
    B[3] = np.array( (mx_LOW, my_LOW, mz_LOW), dtype='float64' )   # Units { G }

    print( B )

    # Return array
    return(B - IMU_Base)


# ************************************************************************
# ===========================> SETUP PROGRAM <===========================
# ************************************************************************

# Useful variables
K           = 1.09e-6                               # Magnet's constant (K) || Units { G^2.m^6}
dx          = 1e-7                                  # Differential step size (Needed for solver)
NSENS       = 4                                     # Number of sensors used
H           = np.empty((NSENS,3), dtype='float64')  # Construct matrix of size  (NSENSx3)

# Start sensors
loadLib()
setupMux()
setupIMU_2( NSENS )


# ************************************************************************
# =========================> MAKE IT ALL HAPPEN <=========================
# ************************************************************************

print( "Ready in 2" )
sleep( 1.0 )
print( "Ready in 1" )
sleep( 1.0 )
print( "GO!" )



# Start iteration
while( True ):

    startTime = time()
    # Get magnetic field readings
    H = calcMag_2()
    

    print ( "Sensor 1' (UpRight) : <%8.5f , %8.5f , %8.5f>" %(H[0][0], H[0][1], H[0][2]) )
    print ( "Sensor 1 (DownRight): <%8.5f , %8.5f , %8.5f>" %(H[1][0], H[1][1], H[1][2]) )
    print ( "Sensor 2' (UpLeft)  : <%8.5f , %8.5f , %8.5f>" %(H[2][0], H[2][1], H[2][2]) )
    print ( "Sensor 2 (DownLeft) : <%8.5f , %8.5f , %8.5f>" %(H[3][0], H[3][1], H[3][2]) )

    print( "t = %.5f" %(time()-startTime) )
    sleep( 0.25 )
    

# ************************************************************************
# =============================> DEPRECATED <=============================
# ************************************************************************
#
