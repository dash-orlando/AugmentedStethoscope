'''
Position tracking of magnet

Author: Danny, Moe
Date: Aug. 4th, 2017th year of our Lord
'''

# Import Modules
#import  serial, random
import  random
import  numpy           as      np
from    time            import  sleep, time
from    math            import  *
from    scipy.optimize  import  fsolve
from    scipy.linalg    import  norm
##from    sympy           import  nsolve
#from    usbProtocol     import  createUSBPort

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
    try:
        line = ser.readline()[:-1]
        col = line.split(",")

        # Wait for the sensor to calibrate itself to ambient fields. 
        while( len(col) < 6 ):
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
            
            return B1, B2, B3

    except Exception as e:
        print( "Caught error in getData()"      )
        print( "Error type %s" %str(type(e))    )
        print( "Error Arguments " + str(e.args) )

# Construct equations to solve for:
def LHS( root, HNorm ):
    x, y, z = root
    H1_L2, H2_L2, H3_L2 = HNorm
    sensor1 = ( K*( x**2+y**2+z**2 )**(1/3.)*
                  ( cos(z/(x**2+y**2+z**2)**(1/2.)))**2 +
                K*( x**2+y**2+z**2 )**(1/3.))
    
    sensor2 = ( K*( (x+1)**2+(y-1)**2+z**2 )**(1/3.)*
                  ( cos(z/((x+1)**2+(y-1)**2+z**2)**(1/2.)))**2 +
                K*( (x+1)**2+(y-1)**2+z**2 )**(1/3.) )
    
    sensor3 = ( K*( (x-1)**2+(y-1)**2+z**2 )**(1/3.)*
                  ( cos(z/((x-1)**2+(y-1)**2+z**2)**(1/2.)))**2 +
                K*( (x-1)**2+(y-1)**2+z**2 )**(1/3.) )
    
    return ( sensor1 - H1_L2**2, sensor2 - H2_L2**2, sensor3 - H3_L2**2 )
    

######################################################
#                   SETUP PROGRAM
######################################################

# Magnetic field vector components
global B1_x, B1_y, B1_z  # IMU readings from sensor 1
global B2_x, B2_y, B2_z  # IMU readings from sensor 2
global B3_x, B3_y, B3_z  # IMU readings from sensor 3
global K

# Sensor 1 readings
B1_x = -0.035           #
B1_y = 0.2180           # Units { G }
B1_z = 0.8440           #

# Sensor 2 readings
B2_x = 0.1530           #
B2_y = 0.0550           # Units { G }
B2_z = 0.8750           #

# Sensor 3 readings
B3_x = -0.277           #
B3_y = 0.1650           # Units { G }
B3_z = 0.7950           #

# Magnet's constant (K)
K   = (19.081)**2    # Units { N.m/G }


### Declare arrays to store values:
# Numerically evaluated angles
angles = [[] for i in range(4)]     # [0]:alpha || [1]:beta || [2]:gamma || [3]:phi

# Location in virtual space
virLoc_1 = [[] for i in range(2)]   # [0]:x     || [1]:y
virLoc_2 = [[] for i in range(2)]   # [0]:x     || [1]:y

# Location in our real physical meaningful(less?) world
reaLoc_1 = [[] for i in range(3)]   # [0]:x     || [1]:y    || [3]:z
reaLoc_2 = [[] for i in range(3)]   # [0]:x     || [1]:y    || [3]:z


######################################################
#                   START PROGRAM
######################################################

# Pool data from Arduino
##H1, H2, H3 = getData(IMU)

# [FAKE] Sensor readings
H1 = np.array( ([B1_x],[B1_y],[B1_z]), dtype='f')
H2 = np.array( ([B2_x],[B2_y],[B2_z]), dtype='f')
H3 = np.array( ([B3_x],[B3_y],[B3_z]), dtype='f')

### Sensor 1 readings
print( "Sensor 1 readings:" )
print( "x = %.4f || y = %.4f || z = %.4f\n"
       %(H1.item(0), H1.item(1), H1.item(2)) )


### Sensor 2 readings
print( "Sensor 2 readings:" )
print( "x = %.4f || y = %.4f || z = %.4f\n"
       %(H2.item(0), H2.item(1), H2.item(2)) )

### Sensor 3 readings
print( "Sensor 3 readings:" )
print( "x = %.4f || y = %.4f || z = %.4f\n"
       %(H3.item(0), H3.item(1), H3.item(2)) )

# Start iteration
start = time()

# Find L2 vector norms
H1_norm = norm(H1)
H2_norm = norm(H2)
H3_norm = norm(H3)
HNorm = [float(H1_norm), float(H2_norm), float(H3_norm)]
print( H1_norm, H2_norm, H3_norm )
print( H1_norm**2, H2_norm**2, H3_norm**2 )

x,y,z = fsolve(LHS, (-1, -1, -1), args=(HNorm))

print( x, y, z )

end = time() - start
print( "Time to complete: %.5f" %(end) )


   



### List equations:
##H_x = B_x*( cos(bb)*cos(yy) )                         - B_y*( cos(bb)*sin(yy) )                         + B_z*( sin(bb) )
##H_y = B_x*( cos(aa)*sin(yy)+cos(yy)*sin(aa)*sin(bb) ) + B_y*( cos(aa)*cos(yy)-sin(aa)*sin(bb)*sin(yy) ) - B_z*( cos(bb)*sin(aa) )
##H_z = B_x*( sin(aa)*sin(yy)-cos(aa)*cos(yy)*sin(bb) ) + B_y*( cos(yy)*sin(aa)+cos(aa)*sin(bb)*sin(yy) ) + B_z*( cos(aa)*cos(bb) )




######################################################
#                   DEPRACATED
######################################################
'''
'''
