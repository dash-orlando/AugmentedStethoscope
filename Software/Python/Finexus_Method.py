'''
* NOTE: the current method as it stands has a limitation in which
*       the magnet has to be placed at a predefined initial position
*       for the solver to successfully converge (find x, y, and z)
*
* Position tracking of magnet based on Finexus
* https://ubicomplab.cs.washington.edu/pdfs/finexus.pdf
*
* VERSION: 0.2.0
*   - MODIFIED: Removed limitation of needing to place magnet at a
*               predefined position at program start up.
*   - ADDED   : 6 sensors placed around an ellipse
*
* KNOWN ISSUES:
*   - Calculations are accurate to around +/-3mm
*     (Look into improving K estimate)
*   - Calculations are accurate up to the x=175mm
*     mark, beyond that, solutions lose accuracy.
*     (i.e. x_actual=200 | x_calc=181 <-- no bueno)
*   - At certain times, the z position goes cray-cray
*     while x & y are correct.
*     (Look into what triggers this behaviour)
*
* AUTHOR  :   Edward Nichols
* DATE    :   Sept. 29th, 2017 Year of Our Lord
* 
* Modified:   Mohammad Odeh 
* DATE    :   Oct. 04th, 2017 Year of Our Lord
*
'''

# Import Modules
import  numpy               as      np              # Import Numpy
from    time                import  sleep           # Sleep for stability
from    scipy.optimize      import  root            # Solve System of Eqns for (x, y, z)
from    scipy.linalg        import  norm            # Calculate vector norms (magnitude)
from    usbProtocol         import  createUSBPort   # Create USB port (serial comm. w\ Arduino)
import  argparse                                    # Feed in arguments to the program

# ************************************************************************
# =====================> CONSTRUCT ARGUMENT PARSER <=====================*
# ************************************************************************
ap = argparse.ArgumentParser()

ap.add_argument("-hd", "--hardDebug", action='store_true',
                help="invoke flag to enable debugging")
ap.add_argument("-sd", "--softDebug", action='store_true',
                help="invoke flag to enable debugging")

args = vars( ap.parse_args() )

##args["hardDebug"] = True
args["softDebug"] = True

# ************************************************************************
# =====================> DEFINE NECESSARY FUNCTIONS <====================*
# ************************************************************************

# ****************************************************
# Define function to sort from lowest->highest value *
# ****************************************************
def argsort(seq):
    # http://stackoverflow.com/questions/3071415/efficient-method-to-calculate-the-rank-vector-of-a-list-in-python
    return sorted(range(len(seq)), key=seq.__getitem__)


# ****************************************************
# Define function to pool & return data from Arduino *
# ****************************************************
def getData(ser):
    global CALIBRATING

    # Flush buffer
    ser.reset_input_buffer()
    ser.reset_output_buffer()

    # Allow data to fill-in buffer
    sleep(0.1)

    try:
        # Read incoming data and seperate
        line    = ser.readline()[:-1]
        col     = line.split(",")

        # Wait for the sensor to calibrate itself to ambient fields.
        while( len(col) < 18 ):
            line    = ser.readline()[:-1]
            col     = line.split(",")
            if(CALIBRATING == True):
                print( "Calibrating...\n" )
                CALIBRATING = False

        # Construct magnetic field array
        else:
            # Sensor 1
            Bx = float(col[0])
            By = float(col[1])
            Bz = float(col[2])
            B1 = np.array( ([Bx],[By],[Bz]), dtype='float64') # Units { G }

            # Sensor 2
            Bx = float(col[3])
            By = float(col[4])
            Bz = float(col[5])
            B2 = np.array( ([Bx],[By],[Bz]), dtype='float64') # Units { G }

            # Sensor 3
            Bx = float(col[6])
            By = float(col[7])
            Bz = float(col[8])
            B3 = np.array( ([Bx],[By],[Bz]), dtype='float64') # Units { G }

            # Sensor 4
            Bx = float(col[9] )
            By = float(col[10])
            Bz = float(col[11])
            B4 = np.array( ([Bx],[By],[Bz]), dtype='float64') # Units { G }

            # Sensor 5
            Bx = float(col[12])
            By = float(col[13])
            Bz = float(col[14])
            B5 = np.array( ([Bx],[By],[Bz]), dtype='float64') # Units { G }

            # Sensor 6
            Bx = float(col[15])
            By = float(col[16])
            Bz = float(col[17])
            B6 = np.array( ([Bx],[By],[Bz]), dtype='float64') # Units { G }
            
            # Return vectors
            return (B1, B2, B3, B4, B5, B6)

    except Exception as e:
        print( "Caught error in getData()"      )
        print( "Error type %s" %str(type(e))    )
        print( "Error Arguments " + str(e.args) )

# ****************************************************
# Define function to construct equations to solve for
# ****************************************************
def LHS( root, K, norms ):
    global PRINT
    global sort
    
    # Extract x, y, and z
    x, y, z = root
    
    # Construct the (r) terms for each sensor
    # NOTE: Relative distance terms are in meters
    #     : Standing on sensor(n), how many units in
    #       the x/y/z direction should I march to get
    #       back to sensor1 (origin)?

    # NOTE TO MOE 9/29 6pm: The sensor array is rejigged to a circle. Motion doesn't follow the coordinate axis written on the paper:
    # i.e. If the magnet was moved 25mm in positive X, the Y and Z axes were the ones that shifted.
    # I was in the process of verifying the definitions to match the axis definitions on the IMUs and our physical motion.
    # Needless to say, the magnitude of motion is not accurate either.

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

    # If in debug/verbose mode:
    if (args["hardDebug"]):
        if (PRINT):
            print( "\n=========================================" )
            print( "Norms: %f, %f, %f, %f, %f, %f"  %(norms[0], norms[1],
                                                      norms[2], norms[3],
                                                      norms[4], norms[5]) )
            print( "Using sensors: #%d, #%d, #%d"   %(sort[0]+1, sort[1]+1, sort[2]+1) )
            print( "=========================================\n" )
            sleep(1.5)
            PRINT = False
        
    # Return vector
    return ( f )

# ****************************************************
# Determine initial guess based on magnitude of
# magnetic field relative to all the sensors
# ****************************************************
def findIG(H_fields):
    # Read current magnetic field from MCU
    (H1, H2, H3, H4, H5, H6) = getData(IMU)

    # Compute L2 vector norms
    HNorm = [ float(norm(H1)), float(norm(H2)),
              float(norm(H3)), float(norm(H4)),
              float(norm(H5)), float(norm(H6)) ]
    
    # Determine which sensors to use based on magnetic field value (smallValue==noBueno!)
    sort = argsort(HNorm)               # Auxiliary function sorts norms from smallest to largest
    sort.reverse()                      # Python built-in function reverses elements of list

    ### Magnet between sensors (124)
    if (sort[0] and sort[1] and sort[3]) > (sort[2] and sort[4] and sort[5]):
        # Centroid of triangle at: (x, y) = (100/3, 100)mm
        return ( np.array((100/3000., 0.100, -0.010), dtype='float64') )

    ### Magnet between sensors (123)
    elif (sort[0] and sort[1] and sort[2]) > (sort[3] and sort[4] and sort[5]):
        # Centroid of triangle at: (x, y) = (100, 425/3)mm
        return ( np.array((0.100, 425/3000., -0.010), dtype='float64') )

    ### Magnet between sensors (236)
    elif (sort[1] and sort[2] and sort[5]) > (sort[0] and sort[3] and sort[4]):
        # Centroid of triangle at: (x, y) = (500/3, 100)mm
        return ( np.array((500/3000., 0.100, -0.010), dtype='float64') )

    ### Magnet between sensors (356)
    elif (sort[2] and sort[4] and sort[5]) > (sort[0] and sort[1] and sort[3]):
        # Centroid of triangle at: (x, y) = (500/3, 025)mm
        return ( np.array((500/3000., 0.025, -0.010), dtype='float64') )

    ### Magnet between sensors (456)
    elif (sort[3] and sort[4] and sort[5]) > (sort[0] and sort[1] and sort[2]):
        # Centroid of triangle at: (x, y) = (100, -50/3)mm
        return ( np.array((0.100, -50/3000., -0.010), dtype='float64') )

    ### Magnet between sensors (145)
    elif (sort[0] and sort[3] and sort[4]) > (sort[1] and sort[2] and sort[5]):
        # Centroid of triangle at: (x, y) = (100/3, 025)mm
        return ( np.array((100/3000., 0.025, -0.010), dtype='float64') )

    ### Magnet in No Man's Land
    else:
        return ( np.array((0.100, 225/3000., -0.010), dtype='float64') )

# ************************************************************************
# ===========================> SETUP PROGRAM <===========================
# ************************************************************************

# Useful variables
global CALIBRATING
global PRINT
global sort

CALIBRATING = True                              # Boolean to indicate that device is calibrating
READY       = False                             # Give time for user to place magnet
PRINT       = True                              # A flag to print ONLY once!
sort        = 'null'                            # Variable used for sorting 

K           = 1.09e-6                           # Magnet's constant (K) || Units { G^2.m^6}
dx          = 1e-7                              # Differential step size (Needed for solver)

initialGuess= np.array((0.10, 0.01, -0.01), 
                        dtype='float64' )       # Initial position/guess

# Establish connection with Arduino
DEVC = "Arduino"                                # Device Name (not very important)
PORT = 29                                       # Port number (VERY important)
BAUD = 115200                                   # Baudrate    (VERY VERY important)

# Error handling in case serial communcation fails (1/2)
try:
    IMU = createUSBPort( DEVC, PORT, BAUD )     # Create serial connection
    if IMU.is_open == False:                    # Make sure port is open
        IMU.open()
    print( "Serial Port OPEN" )

# Error handling in case serial communcation fails (2/2)
except Exception as e:
    print( "Could NOT open serial port" )
    print( "Error type %s" %str(type(e)) )
    print( "Error Arguments " + str(e.args) )
    sleep( 2.5 )
    quit()                                      # Shutdown entire program


# Determine initial guess based on magnet's location
initialGuess = findIG(getData(IMU))

# ************************************************************************
# =========================> MAKE IT ALL HAPPEN <=========================
# ************************************************************************

# Start iteration
while( True ):
    # Pool data from Arduino
    (H1, H2, H3, H4, H5, H6) = getData(IMU)

    # Inform user that system is almost ready
    if(READY == False):
        print( "Place magnet @ the starting location! \n" )
        sleep( 2.5 )
        print( "Ready in 3" )
        sleep( 1.0 )
        print( "Ready in 2" )
        sleep( 1.0 )
        print( "Ready in 1" )
        sleep( 1.0 )
        print( "GO!" )

        # Set the device to ready!!
        READY = True
        
    # Compute L2 vector norms
    HNorm = [ float(norm(H1)), float(norm(H2)),
              float(norm(H3)), float(norm(H4)),
              float(norm(H5)), float(norm(H6)) ]

    # Invoke solver (using Levenberg-Marquardt)
    sol = root(LHS, initialGuess, args=(K, HNorm), method='lm',
               options={'ftol':1e-10, 'xtol':1e-10, 'maxiter':1000,
                        'eps':1e-8, 'factor':0.001})

    # Print solution (coordinates) to screen
    print( "Current position (x , y , z):" )
    print( "(%.5f , %.5f , %.5f)mm" %(sol.x[0]*1000, sol.x[1]*1000, sol.x[2]*1000) )

    # If in softDebug mode:
    if (args["softDebug"]):
        print( "Using sensors: #%d, #%d, #%d\n\n" %(sort[0]+1, sort[1]+1, sort[2]+1) )

    print('')
    
    # If in debug/verbose mode:
    if (args["hardDebug"]):
        PRINT = True
        # Print complete solution returned by vector
        print( sol )
        print("")

    # Sleep for stability
    sleep( 0.1 )

    # Check if solution makes sense
    if (abs(sol.x[0]*1000) > 500) or (abs(sol.x[1]*1000) > 500) or (abs(sol.x[2]*1000) > 500):
        print( "Invalid solution. Resetting Calculations" )
        # Determine initial guess based on magnet's location
        initialGuess = findIG(getData(IMU))
        
    # Update initial guess with current position and feed back to solver
    else:    
        initialGuess = np.array( (sol.x[0]+dx, sol.x[1]+dx, sol.x[2]+dx), dtype='float64' )

# ************************************************************************
# =============================> DEPRECATED <=============================
# ************************************************************************
#
