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
from    scipy.linalg            import  norm
##from    sympy                   import  nsolve
from    usbProtocol             import  createUSBPort
from    sympy.core.symbol       import symbols
from    sympy.solvers.solveset   import nonlinsolve

######################################################
#                   FUNCTION DEFINITIONS
######################################################

# Solve matrices for location
def solveMatrix(a, b, y, p, loc_x, loc_y, sensor):
    pass

# Pool data from Arduino
def getData(ser):
    
    # Flush buffer
    ser.reset_input_buffer()
    ser.reset_output_buffer()
    sleep(.1)
    try:
        line = ser.readline()[:-1]
        col = line.split(",")

        # Wait for the sensor to calibrate itself to ambient fields.
        check = True
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
            B1 = np.array( ([Bx],[By],[Bz]), dtype='f')

            # Sensor 2
            Bx=float(col[3])
            By=float(col[4])
            Bz=float(col[5])
            B2 = np.array( ([Bx],[By],[Bz]), dtype='f')

            # Sensor 3
            Bx=float(col[6])
            By=float(col[7])
            Bz=float(col[8])
            B3 = np.array( ([Bx],[By],[Bz]), dtype='f')
            
            return (B1, B2, B3)

    except Exception as e:
        print( "Caught error in getData()"      )
        print( "Error type %s" %str(type(e))    )
        print( "Error Arguments " + str(e.args) )

# Construct equations to solve for:
def LHS( root, HNorm ):
    x, y, z = root
    H1_L2, H2_L2, H3_L2 = HNorm
    sensor1 = ( K*( x**2 + y**2 + z**2 )**(-3.) *
              ( 3*( z**2/(x**2 + y**2 + z**2) ) + 1 ))
    
    sensor2 = ( K*( (x+5)**2 + (y-5)**2 + z**2 )**(-3.) *
              ( 3*( z**2/((x+5)**2 + (y-5)**2 + z**2) ) + 1 ))
    
    sensor3 = ( K*( (x-5)**2 + (y-5)**2 + z**2 )**(-3.) *
              ( 3*( z**2/((x-5)**2 + (y-5)**2 + z**2) ) + 1 ))
    
    return ( sensor1 - H1_L2**2, sensor2 - H2_L2**2, sensor3 - H3_L2**2 )
    

######################################################
#                   SETUP PROGRAM
######################################################

# Magnetic field vector components
global B1_x, B1_y, B1_z  # IMU readings from sensor 1
global B2_x, B2_y, B2_z  # IMU readings from sensor 2
global B3_x, B3_y, B3_z  # IMU readings from sensor 3
global K

### Sensor 1 readings
##B1_x = 0.0880           #
##B1_y = -0.059           # Units { G }
##B1_z = 2.0600           #
##
### Sensor 2 readings
##B2_x = 0.0030           #
##B2_y = 0.0200           # Units { G }
##B2_z = 0.1890           #
##
### Sensor 3 readings
##B3_x = -0.003           #
##B3_y = 0.0200           # Units { G }
##B3_z = 0.1940           #

# Magnet's constant (K)
K   = (19.081)**2    # Units { N.m/T }

# Establish connection with Arduino
try:
    IMU = createUSBPort( "Arduino", 39, 115200 )
    if IMU.is_open == False:
        IMU.open()
    print( "Serial Port OPEN" )

except Exception as e:
    print( "Could NOT open serial port" )
    print( "Error type %s" %str(type(e)) )
    print( " Error Arguments " + str(e.args) )
    sleep( 5 )
    quit()

######################################################
#                   START PROGRAM
######################################################
while( True ):
    # Pool data from Arduino
    (H1, H2, H3) = getData(IMU)

    # [FAKE] Sensor readings
    ##H1 = np.array( ([B1_x],[B1_y],[B1_z]), dtype='f')
    ##H2 = np.array( ([B2_x],[B2_y],[B2_z]), dtype='f')
    ##H3 = np.array( ([B3_x],[B3_y],[B3_z]), dtype='f')

##    ### Sensor 1 readings
##    print( "Sensor 1 readings:" )
##    print( "x = %.4f || y = %.4f || z = %.4f\n"
##           %(H1.item(0), H1.item(1), H1.item(2)) )
##
##
##    ### Sensor 2 readings
##    print( "Sensor 2 readings:" )
##    print( "x = %.4f || y = %.4f || z = %.4f\n"
##           %(H2.item(0), H2.item(1), H2.item(2)) )
##
##    ### Sensor 3 readings
##    print( "Sensor 3 readings:" )
##    print( "x = %.4f || y = %.4f || z = %.4f\n"
##           %(H3.item(0), H3.item(1), H3.item(2)) )
##
    # Start iteration
    start = time()

    # Find L2 vector norms
    H1_norm = norm(H1)
    H2_norm = norm(H2)
    H3_norm = norm(H3)
    HNorm = [float(H1_norm), float(H2_norm), float(H3_norm)]
##    print( "H Norms:" )
##    print( H1_norm, H2_norm, H3_norm )
##    print( "(H Norms)^2:" )
##    print( H1_norm**2, H2_norm**2, H3_norm**2 )

    x,y,z = fsolve(LHS, (0.1, 0.1, 0.1), args=(HNorm), xtol=1e-10)
    
    print( "SOLUTION:" )
    print( "x = %.5f || y = %.5f || z = %.5f" %(x, y, z) )

    end = time() - start
    print( "Time to complete: %.5f\n" %(end) )
    sleep( 1 )


   



### List equations:
##H_x = B_x*( cos(bb)*cos(yy) )                         - B_y*( cos(bb)*sin(yy) )                         + B_z*( sin(bb) )
##H_y = B_x*( cos(aa)*sin(yy)+cos(yy)*sin(aa)*sin(bb) ) + B_y*( cos(aa)*cos(yy)-sin(aa)*sin(bb)*sin(yy) ) - B_z*( cos(bb)*sin(aa) )
##H_z = B_x*( sin(aa)*sin(yy)-cos(aa)*cos(yy)*sin(bb) ) + B_y*( cos(yy)*sin(aa)+cos(aa)*sin(bb)*sin(yy) ) + B_z*( cos(aa)*cos(bb) )




######################################################
#                   DEPRACATED
######################################################
'''
'''
