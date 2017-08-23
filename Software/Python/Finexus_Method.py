'''
Position tracking of magnet

Author: Danny, Moe
Date: Aug. 4th, 2017th year of our Lord
'''

# Import Modules
#import  serial, random
import  random
import  numpy                   as      np
from    time                    import  sleep, time
from    math                    import  *
from    scipy.optimize          import  fsolve
from    scipy.optimize          import  broyden1
from    scipy.optimize          import  newton_krylov
from    scipy.optimize          import  root
from    scipy.linalg            import  norm
##from    sympy                   import  nsolve
from    usbProtocol             import  createUSBPort
from    sympy.core.symbol       import symbols
from    sympy.solvers.solveset   import nonlinsolve

######################################################
#                   FUNCTION DEFINITIONS
######################################################

# Pool data from Arduino
def getData(ser):
    global check
    # Flush buffer
    ser.reset_input_buffer()
    ser.reset_output_buffer()
    sleep(0.5)
    try:
        line = ser.readline()[:-1]
        col = line.split(",")

        # Wait for the sensor to calibrate itself to ambient fields.
        while( len(col) < 9 ):
            line = ser.readline()[:-1]
            col = line.split(",")
            if(check == True):
                print "Waiting..."
                check = False

        # Construct magnetic field array
        else:
            # Sensor 1
            Bx=float(col[0])
            By=float(col[1])
            Bz=float(col[2])
            B1 = np.array( ([Bx],[By],[Bz]), dtype='f') # Units { G }

            # Sensor 2
            Bx=float(col[3])
            By=float(col[4])
            Bz=float(col[5])
            B2 = np.array( ([Bx],[By],[Bz]), dtype='f') # Units { G }

            # Sensor 3
            Bx=float(col[6])
            By=float(col[7])
            Bz=float(col[8])
            B3 = np.array( ([Bx],[By],[Bz]), dtype='f') # Units { G }
            
            return (B1, B2, B3)

    except Exception as e:
        print( "Caught error in getData()"      )
        print( "Error type %s" %str(type(e))    )
        print( "Error Arguments " + str(e.args) )

# Construct equations to solve for:
def LHS( root ):
    x, y, z = root
    H1_L2, H2_L2, H3_L2 = HNorm
    sensor1 = ( K*( x**2. + y**2. + z**2. )**(-3.) *
              ( 3.*( z**2./(x**2. + y**2. + z**2.) ) + 1 ))
    
    sensor2 = ( K*( (x+.05)**2. + (y-.05)**2. + z**2. )**(-3.) *
              ( 3.*( z**2./((x+.05)**2. + (y-.05)**2. + z**2.) ) + 1 ))
    
    sensor3 = ( K*( (x-.05)**2. + (y-.05)**2. + z**2. )**(-3.) *
              ( 3.*( z**2./((x-.05)**2. + (y-.05)**2. + z**2.) ) + 1 ))
    
    return ( sensor1 - H1_L2**2, sensor2 - H2_L2**2, sensor3 - H3_L2**2 )
    
# Construct equations to solve for:
def LHS1( root ):
    # Extract x, y, and z
    x, y, z = root
    
    # Construct the (r) terms for each sensor
    r1 = float( ( (x+0.00)**2. + (y+0.00)**2. + (z+0.00)**2. )**(1/2.) )
    r2 = float( ( (x+0.05)**2. + (y-0.05)**2. + (z+0.00)**2. )**(1/2.) )
    r3 = float( ( (x-0.05)**2. + (y-0.05)**2. + (z+0.00)**2. )**(1/2.) )

    # Extract the vector norms
    H1_L2, H2_L2, H3_L2 = HNorm

    # Construct the equations
    sensor1 = ( K*( r1 )**(-6.) * ( 3.*( z/r1 )**2. + 1 ) )
    
    sensor2 = ( K*( r2 )**(-6.) * ( 3.*( z/r2 )**2. + 1 ) )
    
    sensor3 = ( K*( r3 )**(-6.) * ( 3.*( z/r3 )**2. + 1 ) )

    # Construct a vector of the equations
    f = [sensor1 - H1_L2**2, sensor2 - H2_L2**2, sensor3 - H3_L2**2]

    # Return vector
    return ( f )

######################################################
#                   SETUP PROGRAM
######################################################

# Useful variables
global K, check, HNorm
check = True

# Magnet's constant (K)
K   = 1.09              # Units { G^2.m^6}

# Establish connection with Arduino
try:
    IMU = createUSBPort( "Arduino", 39, 115200 )
    if IMU.is_open == False:
        IMU.open()
    print( "Serial Port OPEN" )

except Exception as e:
    print( "Could NOT open serial port" )
    print( "Error type %s" %str(type(e)) )
    print( "Error Arguments " + str(e.args) )
    sleep( 5 )
    quit()

######################################################
#                   START PROGRAM
######################################################
initialGuess = np.array( (.5, .1, .1), dtype='f' )
# Start iteration
while( True ):
    # Pool data from Arduino
    (H1, H2, H3) = getData(IMU)

    # Compute L2 vector norms
    H1_norm = norm(H1)
    H2_norm = norm(H2)
    H3_norm = norm(H3)
    HNorm = [float(H1_norm), float(H2_norm), float(H3_norm)]

    # Invoke solver
    sol = root(LHS1, initialGuess, method='lm',
               options={'ftol': 1e-10, 'xtol':1e-10, 'maxiter':250000, 'epsfcn':1e-8, 'factor':0.1})
    print( sol )
    
    print( "\nSOLUTION:" )
    print( "x = %.5f || y = %.5f || z = %.5f" %(sol.x[0], sol.x[1], sol.x[2]) )
    solution = np.array((sol.x[0], sol.x[1], sol.x[2]), dtype='f')
    print( LHS1(solution) )
    sleep( .5 )
    #initialGuess = np.array( (sol.x[0], sol.x[1], sol.x[2]), dtype='f' )


######################################################
#                   DEPRACATED
######################################################
'''
'''
