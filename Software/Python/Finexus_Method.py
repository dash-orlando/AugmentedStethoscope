'''
* NOTE: the current method as it stands has a limitation in which
*       the magnet has to be placed at a predefined initial position
*       for the solver to successfully converge (find x, y, and z)
*
* Position tracking of magnet based on Finexus
* https://ubicomplab.cs.washington.edu/pdfs/finexus.pdf
*
* VERSION: 0.1.1
*   - First working version.
*   - FIXED: Resolved buffer issue by switching from Arduino to Teensy
*     for the data acquisition and communications.
*   - FIXED: Added a check to verify that solutions (distances) make
*     physical sense. If check fails, the code readjusts calculations.
*
* KNOWN ISSUES:
*   - Small grid size
*     (Look into a better approach for seperating the incoming data)
*   - Calculations are accurate to around +/-3mm
*     (Look into improving K estimate)
*
* AUTHOR  :   Daniel Nichols, Mohammad Odeh
* DATE    :   Aug. 04th, 2017 Year of Our Lord
* 
* MODIFIED:   Mohammad Odeh
* DATE    :   Sep. 25th, 2017 Year of Our Lord
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
# =====================> CONSTRUCT ARGUMENT PARSER <=====================
# ************************************************************************
ap = argparse.ArgumentParser()

ap.add_argument("-d", "--debug", action='store_true',
                help="invoke flag to enable debugging")

args = vars( ap.parse_args() )

#args["debug"] = False

# ************************************************************************
# =====================> DEFINE NECESSARY FUNCTIONS <=====================
# ************************************************************************

# ****************************************************
# Define function to pool & return data from Arduino
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
        line = ser.readline()[:-1]
        col = line.split(",")

        # Wait for the sensor to calibrate itself to ambient fields.
        while( len(col) < 9 ):
            line = ser.readline()[:-1]
            col = line.split(",")
            if(CALIBRATING == True):
                print( "Calibrating...\n" )
                CALIBRATING = False

        # Construct magnetic field array
        else:
            # Sensor 1
            Bx=float(col[0])
            By=float(col[1])
            Bz=float(col[2])
            B1 = np.array( ([Bx],[By],[Bz]), dtype='float64') # Units { G }

            # Sensor 2
            Bx=float(col[3])
            By=float(col[4])
            Bz=float(col[5])
            B2 = np.array( ([Bx],[By],[Bz]), dtype='float64') # Units { G }

            # Sensor 3
            Bx=float(col[6])
            By=float(col[7])
            Bz=float(col[8])
            B3 = np.array( ([Bx],[By],[Bz]), dtype='float64') # Units { G }

            # Return vectors
            return (B1, B2, B3)

    except Exception as e:
        print( "Caught error in getData()"      )
        print( "Error type %s" %str(type(e))    )
        print( "Error Arguments " + str(e.args) )

# ****************************************************
# Define function to construct equations to solve for
# ****************************************************
def LHS( root, K, norms ):
    # Extract x, y, and z
    x, y, z = root
    
    # Construct the (r) terms for each sensor
    # NOTE: relative distance terms are in meters
    r1 = float( ( (x+0.00)**2. + (y+0.00)**2. + (z+0.00)**2. )**(1/2.) )    # Sensor 1
    r2 = float( ( (x+0.10)**2. + (y-0.10)**2. + (z+0.00)**2. )**(1/2.) )    # Sensor 2
    r3 = float( ( (x-0.10)**2. + (y-0.10)**2. + (z+0.00)**2. )**(1/2.) )    # Sensor 3

    # Construct the equations
    Eqn1 = ( K*( r1 )**(-6.) * ( 3.*( z/r1 )**2. + 1 ) ) - norms[0]**2.     # Sensor 1
    Eqn2 = ( K*( r2 )**(-6.) * ( 3.*( z/r2 )**2. + 1 ) ) - norms[1]**2.     # Sensor 2
    Eqn3 = ( K*( r3 )**(-6.) * ( 3.*( z/r3 )**2. + 1 ) ) - norms[2]**2.     # Sensor 3

    # Construct a vector of the equations
    f = [Eqn1, Eqn2, Eqn3]

    # Return vector
    return ( f )

# ************************************************************************
# ===========================> SETUP PROGRAM <===========================
# ************************************************************************

# Useful variables
global CALIBRATING

CALIBRATING = True                              # Boolean to indicate that device is calibrating
READY       = False                             # Give time for user to palce magnet

K           = 1.09e-6                           # Magnet's constant (K) || Units { G^2.m^6}
dx          = 1e-5                              # Differential step size (Needed for solver)
initialGuess= np.array((.01, .075, -.01), 
                        dtype='float64' )       # Initial position/guess

# Establish connection with Arduino
try:
    IMU = createUSBPort( "Arduino", 31, 115200 )
    if IMU.is_open == False:
        IMU.open()
    print( "Serial Port OPEN" )

except Exception as e:
    print( "Could NOT open serial port" )
    print( "Error type %s" %str(type(e)) )
    print( "Error Arguments " + str(e.args) )
    sleep( 5 )
    quit()


# ************************************************************************
# =========================> MAKE IT ALL HAPPEN <=========================
# ************************************************************************

# Start iteration
while( True ):
    # Pool data from Arduino
    (H1, H2, H3) = getData(IMU)

    # Inform user that system is almost ready
    if(READY == False):
        print( "Place magnet @ (x, y, z) = (0, 75, 0)mm\n" )
        sleep( 2.5 )
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

        # Set the device to ready!!
        READY = True
        
    # Compute L2 vector norms
    HNorm = [float(norm(H1)), float(norm(H2)), float(norm(H3))]

    # Invoke solver
    sol = root(LHS, initialGuess, args=(K, HNorm), method='lm',
               options={'ftol':1e-10, 'xtol':1e-10, 'maxiter':1000,
                        'eps':1e-8, 'factor':0.001})

    # Print solution (coordinates) to screen
    print( "Current position (x , y , z):" )
    print( "(%.5f , %.5f , %.5f)mm\n" %(sol.x[0]*1000, sol.x[1]*1000, sol.x[2]*1000) )

    # If in debug/verbose mode:
    if (args["debug"]):
        # Print complete solution returned by vector
        print( sol )
        print("")

    # Sleep for stability
    sleep( 0.1 )

    # Check if solution makes sense
    if (abs(sol.x[0]*1000) > 500) or (abs(sol.x[1]*1000) > 500) or (abs(sol.x[2]*1000) > 500):
        print( "Invalid solution. Resetting Calculations" )
        initialGuess = np.array( (.01, .075, -.01), dtype='float64' )
        
    # Update initial guess with current position and feed back to solver
    else:    
        initialGuess = np.array( (sol.x[0]+dx, sol.x[1]+dx, sol.x[2]+dx), dtype='float64' )

# ************************************************************************
# =============================> DEPRECATED <=============================
# ************************************************************************
#
