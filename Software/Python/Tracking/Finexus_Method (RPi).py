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

# ------------------------------------------------------------------------

def setupIMU( N ):
    ''' Setup IMUS and callibrate them'''
    global IMU_Base
    global imu
    
    IMU_Base = np.empty((6,3), dtype='float64') # Construct matrix of size  (6x3)
    
    for i in range(0, N):
        setSensor( i )                          # Select the IMU
        imu = lib.lsm9ds1_create()              # Create an instance
        lib.lsm9ds1_begin(imu)                  # Initialize it

        if lib.lsm9ds1_begin(imu) == 0:         # In case it fails
            print("Failed to communicate with LSM9DS1.")
            quit()

        else:                                   # In case it doesn't, configure
            CALIBRATION_INDEX = 25             # Average over this many readings
            lib.lsm9ds1_calibrateMag(imu)       # Call built-in calibration routine
            lib.lsm9ds1_setMagScale(imu, 16)    # Set scale to +/-16Gauss
            lib.lsm9ds1_calibrateMag(imu)       # Call built-in calibration routine

            cmx, cmy, cmz = 0, 0, 0

            # Perform user-built calibration to further clear noise
            for j in range(0, CALIBRATION_INDEX):
                lib.lsm9ds1_readMag(imu)

                cmx += lib.lsm9ds1_calcMag(imu, lib.lsm9ds1_getMagX(imu))
                cmy += lib.lsm9ds1_calcMag(imu, lib.lsm9ds1_getMagY(imu))
                cmz += lib.lsm9ds1_calcMag(imu, lib.lsm9ds1_getMagZ(imu))

            # Average all the readings
            IMU_Base[i][0] = cmx/CALIBRATION_INDEX
            IMU_Base[i][1] = cmy/CALIBRATION_INDEX
            IMU_Base[i][2] = cmz/CALIBRATION_INDEX
            print( "Correction constant for sensor %d is:" %(i+1) )
            print( "x: %.5f, y: %.5f, z: %.5f\n" %(IMU_Base[i][0], IMU_Base[i][1], IMU_Base[i][2]) )

# ------------------------------------------------------------------------        

def argsort(seq):
    ''' Sort list from lowest to highest. Returns the indices of the respective values in a list '''
    ''' i.e. Given: [6, 20, 9, 18] || Returns: [1, 3, 2, 0] '''
    # http://stackoverflow.com/questions/3071415/efficient-method-to-calculate-the-rank-vector-of-a-list-in-python
    return sorted(range(len(seq)), key=seq.__getitem__)

# ------------------------------------------------------------------------

def bubbleSort(arr, N):
    ''' Sort list elements from smallest to largest '''
    ''' i.e. Given: t = [6, 20, 9, 18] || bubbleSort(t, 3) = [6, 9, 20]'''
    data = []
    for i in range(0, N):
        data.append( arr[i] )

    for i in range(0, len(data)):
        for j in range(0, len(data)-i-1):
            if (data[j] > data[j+1]):
                temp = data[j]
                data[j] = data[j+1]
                data[j+1] = temp
            else:
                continue
    return (data)

# ------------------------------------------------------------------------

def calcMag( N, Q ):
    ''' Collect readings from IMU '''
    B = np.empty((N,3), dtype='float64')                # Construct matrix of size  (Nx3)
    while(True):
        for i in range(0, N):
            setSensor( i )                                  # Select sensor ( i )

            while lib.lsm9ds1_magAvailable(imu) == 0:       # While no data is available,
                pass                                        # do nothing

            lib.lsm9ds1_readMag(imu)                        # Read values from sensor

            mx = lib.lsm9ds1_calcMag(imu, lib.lsm9ds1_getMagX(imu))
            my = lib.lsm9ds1_calcMag(imu, lib.lsm9ds1_getMagY(imu))
            mz = lib.lsm9ds1_calcMag(imu, lib.lsm9ds1_getMagZ(imu))

            
            B[i] = np.array( (mx, my, mz), dtype='float64' )   # Units { G }

        # Return array
        Q.put_nowait( B - IMU_Base )

# ------------------------------------------------------------------------

def findIG( Q ):
    ''' Guess the magnet's initial position '''
    # Define IMU positions on the grid
    #      / sensor 1: (x, y, z)
    #     /  sensor 2: (x, y, z)
    # Mat=      :          :
    #     \     :          :
    #      \ sensor 6: (x, y, z)
    IMU_pos = np.array(((0.0  , 0.125,   0.0) ,
                        (0.100, 0.175,   0.0) ,
                        (0.200, 0.125,   0.0) ,
                        (0.0  , 0.0  ,   0.0) ,
                        (0.100,-0.050,   0.0) ,
                        (0.200, 0.0  ,   0.0)), dtype='float64')

    # Read current magnetic field from MCU
    H = Q.get()

    # Compute L2 vector norms
    HNorm = [ float(norm(H[0])), float(norm(H[1])),
              float(norm(H[2])), float(norm(H[3])),
              float(norm(H[4])), float(norm(H[5])) ]
    
    # Determine which sensors to use based on magnetic field value (smallValue==noBueno!)
    sort = argsort(HNorm)               # Auxiliary function sorts norms from smallest to largest
    sort.reverse()                      # Python built-in function reverses elements of list

    IMUS = bubbleSort(sort, 3)

    # Return the initial guess as the centroid of the detected triangle
    return ( np.array(((IMU_pos[IMUS[0]][0]+IMU_pos[IMUS[1]][0]+IMU_pos[IMUS[2]][0])/3.,
                       (IMU_pos[IMUS[0]][1]+IMU_pos[IMUS[1]][1]+IMU_pos[IMUS[2]][1])/3.,
                       (IMU_pos[IMUS[0]][2]+IMU_pos[IMUS[1]][2]+IMU_pos[IMUS[2]][2])/3. -0.01), dtype='float64') )

# ------------------------------------------------------------------------

def LHS( root, K, norms ):

    # Extract x, y, and z
    x, y, z = root
    
    # Construct the (r) terms for each sensor
    # NOTE: Relative distance terms are in meters
    #     : Standing on sensor(n), how many units in
    #       the x/y/z direction should I march to get
    #       back to sensor1 (origin)?
    r1 = float( ( (x+0.000)**2. + (y-0.125)**2. + (z+0.00)**2. )**(1/2.) )  # Sensor 1
    r2 = float( ( (x-0.100)**2. + (y-0.175)**2. + (z+0.00)**2. )**(1/2.) )  # Sensor 2
    r3 = float( ( (x-0.200)**2. + (y-0.125)**2. + (z+0.00)**2. )**(1/2.) )  # Sensor 3
    r4 = float( ( (x+0.000)**2. + (y+0.000)**2. + (z+0.00)**2. )**(1/2.) )  # Sensor 4 (ORIGIN)
    r5 = float( ( (x-0.100)**2. + (y+0.050)**2. + (z+0.00)**2. )**(1/2.) )  # Sensor 5
    r6 = float( ( (x-0.200)**2. + (y-0.000)**2. + (z+0.00)**2. )**(1/2.) )  # Sensor 6

    # Construct the equations
    Eqn1 = ( K*( r1 )**(-6.) * ( 3.*( z/r1 )**2. + 1 ) ) - norms[0]**2.     # Sensor 1
    Eqn2 = ( K*( r2 )**(-6.) * ( 3.*( z/r2 )**2. + 1 ) ) - norms[1]**2.     # Sensor 2
    Eqn3 = ( K*( r3 )**(-6.) * ( 3.*( z/r3 )**2. + 1 ) ) - norms[2]**2.     # Sensor 3
    Eqn4 = ( K*( r4 )**(-6.) * ( 3.*( z/r4 )**2. + 1 ) ) - norms[3]**2.     # Sensor 4
    Eqn5 = ( K*( r5 )**(-6.) * ( 3.*( z/r5 )**2. + 1 ) ) - norms[4]**2.     # Sensor 5
    Eqn6 = ( K*( r6 )**(-6.) * ( 3.*( z/r6 )**2. + 1 ) ) - norms[5]**2.     # Sensor 6

    # Construct a vector of the equations
    Eqns = [Eqn1, Eqn2, Eqn3, Eqn4, Eqn5, Eqn6]

    # Determine which sensors to use based on magnetic field value (smallValue==noBueno!)
    sort = argsort(norms)               # Auxiliary function sorts norms from smallest to largest
    sort.reverse()                      # Python built-in function reverses elements of list
    f=[]                                # Declare vector to hold relevant functions

    for i in range(0, 3):               # Fill functions' array with the equations that correspond to
        f.append(Eqns[sort[i]])         # the sensors with the highest norm, thus closest to magnet
        
    # Return vector
    return ( f )

# ************************************************************************
# ===========================> SETUP PROGRAM <===========================
# ************************************************************************

# Useful variables
K           = 1.09e-6                           # Magnet's constant (K) || Units { G^2.m^6}
dx          = 1e-7                              # Differential step size (Needed for solver)
H           = np.empty((6,3), dtype='float64')  # Construct matrix of size  (6x3)
NSENS       = 6                                 # Number of sensors used

LSM9DS1_AG_HGIH = 0x6B
LSM9DS1_M_HGIH = 0x1E

LSM9DS1_AG_LOW = 0x6A
LSM9DS1_M_LOW = 0x1C

# Start sensors
loadLib()
setupMux()
setupIMU( NSENS )

Q_calcMag = LifoQueue( maxsize=0 )              # LIFO queue qith infinite size

# Start pooling data from serial port
t_calcMag = Thread( target=calcMag, args=(NSENS, Q_calcMag,) )
t_calcMag.daemon = True
t_calcMag.start()

# ************************************************************************
# =========================> MAKE IT ALL HAPPEN <=========================
# ************************************************************************

print( "Place magnet on grid" )
print( "Ready in 5" )
sleep( 1.0 )
print( "Ready in 4" )
sleep( 1.0 )
print( "Ready in 3" )
sleep( 1.0 )
print( "Ready in 2" )
sleep( 1.0 )
print( "Ready in 1" )
sleep( 1.0 )
print( "GO!" )

initialGuess = findIG( Q_calcMag )


# Start iteration
while( True ):

    startTime = time()
    # Get magnetic field readings
    H = Q_calcMag.get_nowait()
    
    # Compute L2 vector norms
    HNorm = [ float(norm(H[0])), float(norm(H[1])),
              float(norm(H[2])), float(norm(H[3])),
              float(norm(H[4])), float(norm(H[5])) ]

    # Invoke solver (using Levenberg-Marquardt)
    sol = root(LHS, initialGuess, args=(K, HNorm), method='lm',
               options={'ftol':1e-10, 'xtol':1e-10, 'maxiter':500,
                        'eps':1e-8, 'factor':0.001})

    # Print solution (coordinates) to screen
    print( "Current position (x , y , z):" )
    print( "(%.5f , %.5f , %.5f)mm\n" %(sol.x[0]*1000, sol.x[1]*1000, sol.x[2]*1000) )

    # Check if solution makes sense
    if (abs(sol.x[0]*1000) > 500) or (abs(sol.x[1]*1000) > 500) or (abs(sol.x[2]*1000) > 500):
        print( "Invalid solution. Resetting Calculations" )
        # Determine initial guess based on magnet's location
        initialGuess = findIG( Q_calcMag )
        
    # Update initial guess with current position and feed back to solver
    else:    
        initialGuess = np.array( (sol.x[0]+dx, sol.x[1]+dx, sol.x[2]+dx), dtype='float64' )

    print( "t = %.5f" %(time()-startTime) )
    

# ************************************************************************
# =============================> DEPRECATED <=============================
# ************************************************************************
#
