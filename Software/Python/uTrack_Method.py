'''
Position tracking of magnet

Author: Danny, Moe
Date: Aug. 4th, 2017th year of our Lord
'''

# Import Modules
import  serial, random
import  numpy           as      np
from    time            import  sleep, time
from    math            import  *
from    scipy.optimize  import  fsolve
from    sympy           import  nsolve

######################################################
#                   FUNCTION DEFINITIONS
######################################################

# Function to print obtained values:
def printVals(a_1, b_1, c_1, a_2, b_2, c_2, d, e, f, fprime, g_1, h_1, g_2, h_2):
    for i in range(len(a_1)):
        print( "SENSOR ONE 1:\n" )
        print( "Value of alpha: %.5f" %(d[i]) )
        print( "Value of beta : %.5f" %(e[i]) )
        print( "Value of gamma: %.5f" %(f[i]) )
        print( "H_x_1 = %.5f" %a_1[i])
        print( "H_y_1 = %.5f" %b_1[i])
        print( "H_z_1 = %.5f\n" %c_1[i])

        print( "Value of x_1: %.4f" %g_1[i] )
        print( "Value of y_1: %.4f" %h_1[i] )
        print( "=======================================" )
        print( "SENSOR TWO 2:\n" )
        print( "Value of alpha : %.5f" %(d[i]) )
        print( "Value of beta  : %.5f" %(e[i]) )
        print( "Value of gamma : %.5f" %(f[i]) )
        print( "Value of gamma': %.5f" %(fprime[i]) )
        print( "H_x_2 = %.5f" %a_2[i])
        print( "H_y_2 = %.5f" %b_2[i])
        print( "H_z_2 = %.5f\n" %c_2[i])

        print( "Value of x_2: %.4f" %g_2[i] )
        print( "Value of y_2: %.4f" %h_2[i] )
        print( "=======================================" )
        print( "=======================================" )
        print( "=======================================\n\n" )


# Define equations:
def H_X( aa, bb, yy, phi, B_x, B_y, B_z, sensor ):
    if sensor == 1:
        # Equation for sensor 1
        return( (B_x*( cos(bb)*cos(yy) ) - B_y*( cos(bb)*sin(yy) ) + B_z*( sin(bb) )) )

    elif sensor == 2:
        a_11 = cos(bb)*cos(yy)
        a_12 = -cos(bb)*sin(yy)
        a_13 = sin(bb)
        # Equation for sensor 2
        return( B_x*( a_11*cos(phi) + a_12*sin(phi) ) + B_y*(a_12*cos(phi)-a_11*sin(phi)) + B_z*(a_13) )

def H_Y( aa, bb, yy, phi, B_x, B_y, B_z, sensor ):
    if sensor == 1:
        # Equation for sensor 1
        return( (B_x*( cos(aa)*sin(yy)+cos(yy)*sin(aa)*sin(bb) ) +
                 B_y*( cos(aa)*cos(yy)-sin(aa)*sin(bb)*sin(yy) ) -
                 B_z*( cos(bb)*sin(aa) ) ) )

    elif sensor == 2:
        # Equation for sensor 2
        a_21 = cos(aa)*sin(yy)+cos(yy)*sin(aa)*sin(bb)
        a_22 = cos(aa)*cos(yy)-sin(aa)*sin(bb)*sin(yy)
        a_23 = -cos(bb)*sin(aa)
        return( B_x*(a_21*cos(phi)+a_22*sin(phi)) + B_y*(a_22*cos(phi)-a_21*sin(phi)) + B_z*(a_23) )


def H_Z( aa, bb, yy, phi, B_x, B_y, B_z, sensor ):
    if sensor == 1:
        # Equation for sensor 1
        return( (B_x*( sin(aa)*sin(yy)-cos(aa)*cos(yy)*sin(bb) ) +
                 B_y*( cos(yy)*sin(aa)+cos(aa)*sin(bb)*sin(yy) ) +
                 B_z*( cos(aa)*cos(bb) ) ) )

    elif sensor == 2:
        # Equation for sensor 2
        a_31 = sin(aa)*sin(yy)-cos(aa)*cos(yy)*sin(bb)
        a_32 = cos(yy)*sin(aa)+cos(aa)*sin(bb)*sin(yy)
        a_33 = cos(aa)*cos(bb)
        return( (B_x*( a_31*cos(phi) + a_32*sin(phi) ) +
                 B_y*( a_32*cos(phi) - a_31*sin(phi) ) +
                 B_z*( a_33 ) ) )

def equations( x0, beta, gamma, B_x, B_y, B_z ):

    alpha = x0
    return( (B_x*( sin(alpha)*sin(gamma)-cos(alpha)*cos(gamma)*sin(beta) ) +
             B_y*( cos(gamma)*sin(alpha)+cos(alpha)*sin(beta)*sin(gamma) ) +
             B_z*( cos(alpha)*cos(gamma) ) ) )

def equations_2( x0, aa, beta, gamma, B_x, B_y, B_z ):

    gamma2 = x0
    a_31 = sin(aa)*sin(yy)-cos(aa)*cos(yy)*sin(bb)
    a_32 = cos(yy)*sin(aa)+cos(aa)*sin(bb)*sin(yy)
    a_33 = cos(aa)*cos(bb)
    return( (B_x*( a_31*cos(gamma2) + a_32*sin(gamma2) ) +
             B_y*( a_32*cos(gamma2) - a_31*sin(gamma2) ) +
             B_z*( a_33 ) ) )

def location_virtual (p, H_x, H_y):
    x, y = p
    return ( (K*(2*x*x+y*y)/pow(sqrt(x*x+y*y), 5) - H_x), (K*(3*x*y)/pow(sqrt(x*x+y*y), 5) - H_y) )

######################################################
#                   SETUP PROGRAM
######################################################

######################################################
#                   START PROGRAM
######################################################

# Magnetic field vector components
global B_x_1, B_y_1, B_z_1  # IMU readings from sensor 1
global B_x_2, B_y_2, B_z_2  # IMU readings from sensor 2
global K
K   = 120    /(4*np.pi)

# Sensor 1 readings
B_x_1 = random.random()*5
B_y_1 = random.random()*5
B_z_1 = random.random()*5
print( "Sensor 1 readings:" )
print( B_x_1, B_y_1, B_z_1 )

# Sensor 2 readings
B_x_2 = random.random()*5
B_y_2 = random.random()*5
B_z_2 = random.random()*5
print( "\nSensor 2 readings:" )
print( B_x_2, B_y_2, B_z_2 )

# Declare arrays to store values:
H_xx_1 = []
H_yy_1 = []
H_zz_1 = []
H_xx_2 = []
H_yy_2 = []
H_zz_2 = []
alpha = []
beta  = []
gamma = []
gammaprime = []
location_x_1 = []
location_y_1 = []
location_x_2 = []
location_y_2 = []

# Start iteration
start = time()
N=25
for i in range(1,N,1):
    for j in range(1,N,1):
        bb=i*np.pi/180.
        yy=j*np.pi/180.
        try:
            # Impose constraint 1 (sensor 1)
            aa = fsolve(equations, 0.1, args=(bb, yy, B_x_1, B_y_1, B_z_1) )
            sen1_constraint1 = equations(aa, bb, yy, B_x_1, B_y_1, B_z_1)
        except Exception as e:
            print( "Caught ERROR:\n%r" %type(e) )
            pass

        # Check if constraint 1 is met (sensor 1)
        if (sen1_constraint1 <= 1e-6) and (sen1_constraint1 >= -1e-6):
            H_x_1 = H_X(aa, bb, yy, 0, B_x_1, B_y_1, B_z_1, 1)
            H_y_1 = H_Y(aa, bb, yy, 0, B_x_1, B_y_1, B_z_1, 1)
            H_z_1 = H_Z(aa, bb, yy, 0, B_x_1, B_y_1, B_z_1, 1)

            # Obtain (x, y) in "virtual" space (sensor 1)
            try:
                x_1 , y_1 = fsolve(location_virtual, (0.1,0.1), args=(H_x_1, H_y_1))
            except:
                pass

            # Impose constraint 1 (sensor 2)
            gamma2 = fsolve(equations_2, 0.1, args=(aa, bb, yy, B_x_2, B_y_2, B_z_2))
            sen2_constraint1 = equations_2(gamma2, aa, bb, yy, B_x_2, B_y_2, B_z_2)

            # Check if constraint 1 is met (sensor 2)
            if (sen2_constraint1 <= 1e-6) and (sen2_constraint1 >= -1e-6):
                H_x_2 = H_X(aa, bb, yy, gamma2, B_x_2, B_y_2, B_z_2, 2)
                H_y_2 = H_Y(aa, bb, yy, gamma2, B_x_2, B_y_2, B_z_2, 2)
                H_z_2 = H_Z(aa, bb, yy, gamma2, B_x_2, B_y_2, B_z_2, 2)

                # Obtain (x, y) in "virtual" space (sensor 2)
                try:
                    x_2 , y_2 = fsolve(location_virtual, (0.1,0.1), args=(H_x_2, H_y_2))
                except:
                    pass

                
                # If constraint 1 is met find rotation in z-axis that can
                # impose constraint 2
                
                
                
                # Append values to array for printing
                H_xx_1.append(H_x_1)
                H_yy_1.append(H_y_1)
                H_zz_1.append(H_z_1)
                H_xx_2.append(H_x_2)
                H_yy_2.append(H_y_2)
                H_zz_2.append(H_z_2)
                alpha.append((aa*180/np.pi))
                beta.append ((bb*180/np.pi))
                gamma.append((yy*180/np.pi))
                gammaprime.append((gamma2*180/np.pi))
                location_x_1.append(x_1)
                location_y_1.append(y_1)
                location_x_2.append(x_2)
                location_y_2.append(y_2)
                continue
        
end = time() - start
print( "Time to complete %r iterations: %.5f" %(N*N, end) )
sleep( 2.5 )

printVals( H_xx_1, H_yy_1, H_zz_1,
           H_xx_2, H_yy_2, H_zz_2,
           alpha, beta, gamma, gammaprime,
           location_x_1, location_y_1,
           location_x_2, location_y_2)


### List equations:
##H_x = B_x*( cos(bb)*cos(yy) )                         - B_y*( cos(bb)*sin(yy) )                         + B_z*( sin(bb) )
##H_y = B_x*( cos(aa)*sin(yy)+cos(yy)*sin(aa)*sin(bb) ) + B_y*( cos(aa)*cos(yy)-sin(aa)*sin(bb)*sin(yy) ) - B_z*( cos(bb)*sin(aa) )
##H_z = B_x*( sin(aa)*sin(yy)-cos(aa)*cos(yy)*sin(bb) ) + B_y*( cos(yy)*sin(aa)+cos(aa)*sin(bb)*sin(yy) ) + B_z*( cos(aa)*cos(bb) )




######################################################
#                   DEPRACATED
######################################################
'''
# Angle of projection to xy-plane
angle_project = atan(B_z/sqrt( (B_x*B_x) + (B_y*B_y) ) )
print( angle_project*180/3.1415927 )

================================================================================

aa, bb, yy = 0, np.pi, np.pi/2

# Transformation matrix
T_x = np.mat  ( ((  1       ,   0       ,   0       ),
                (   0       , cos(aa)   , -sin(aa)  ),
                (   0       , sin(aa)   ,  cos(aa)  )), dtype='f')

T_y = np.mat  ( (( cos(bb)  ,   0       , sin(bb)   ),
                (   0       ,   1       ,   0       ),
                ( -sin(bb)  ,   0       , cos(bb)   )), dtype='f')

T_z = np.mat  ( (( cos(yy)  , -sin(yy)  ,   0       ),
                (  sin(yy)  ,  cos(yy)  ,   0       ),
                (   0       ,   0       ,   1       )), dtype='f')


T_xyz = np.mat(((           cos(bb)*cos(yy)                 ,               -cos(bb)*sin(yy)            ,   sin(bb)             ),
                (   cos(aa)*sin(yy)+cos(yy)*sin(aa)*sin(bb) , cos(aa)*cos(yy)-sin(aa)*sin(bb)*sin(yy)   ,   -cos(bb)*sin(aa)    ),
                (   sin(aa)*sin(yy)-cos(aa)*cos(yy)*sin(bb) , cos(yy)*sin(aa)+cos(aa)*sin(bb)*sin(yy)   ,   cos(aa)*cos(bb)     )), dtype='f')

print( "T_x:" )
print( T_x )
print( "\nT_y:" )
print( T_y )
print( "\nT_z:" )
print( T_z )

T = T_x * T_y * T_z
print( "\nT:" )
print( T )

print( "\nT_xyz:" )
print( T_xyz )

================================================================================

soln = nsolve(  [B_x*( sin(aa)*sin(yy)-cos(aa)*cos(yy)*sin(bb) ) +
                 B_y*( cos(yy)*sin(aa)+cos(aa)*sin(bb)*sin(yy) ) +
                 B_z*( cos(aa)*cos(bb) )],
                [aa],
                [1] )
print( soln )

'''
