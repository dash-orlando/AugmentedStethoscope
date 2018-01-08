'''
*
* Position tracking of magnet based on Finexus ported to Raspberry Pi
* https://ubicomplab.cs.washington.edu/pdfs/finexus.pdf
*
* VERSION: 0.2
*   - ADDED   : Draw region on TFT screen
*   - MODIFIED: Use IMU class for ease of programming (hooray Moe)
*
* KNOWN ISSUES:
*   - No known issues for now (at least I, Moe, couldn't spot any)
*
* ROADMAP:
*   - Clean up code as it looks hideous.
*   - Update code so it is parallel in state to the windows version.
*   - Implement multithreading.
*
* Author        :   Nicolas Maduro (aka Danny) 
* Last Modified :   Oct. 16th, 2017 Year of Our Lord
*
* Author        :   Mohammad Odeh 
* Last Modified :   Jan.  8th, 2018 Year of Our Lord
*
'''

# Import Tracking Modules
import  numpy               as      np              # Import Numpy
import  RPi.GPIO            as      GPIO            # Use GPIO pins
from    time                import  sleep, time     # Sleep for stability / time execution
from    scipy.optimize      import  root            # Solve System of Eqns for (x, y, z)
from    scipy.linalg        import  norm            # Calculate vector norms (magnitude)
from    IMU_Class           import  *

# Import Drawing (TFT) Modules
import  Adafruit_ILI9341    as      TFT
import  Adafruit_GPIO       as      GPIO
import  Adafruit_GPIO.SPI   as      SPI
from    PIL                 import  Image
from    PIL                 import  ImageDraw
from    PIL                 import  ImageFont

# ************************************************************************
# =====================> DEFINE NECESSARY FUNCTIONS <====================*
# ************************************************************************       

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

def findIG( magField ):
    ''' Guess the magnet's initial position '''
    # Define IMU positions on the grid
    #      / sensor 1: (x, y, z)
    #     /  sensor 2: (x, y, z)
    # Mat=      :          :
    #     \     :          :
    #      \ sensor 6: (x, y, z)
    IMU_pos = np.array(((0.000, 0.000,   0.0) ,
                        (0.000, 0.125,   0.0) ,
                        (0.100, 0.175,   0.0) ,
                        (0.200, 0.125,   0.0) ,
                        (0.200, 0.000,   0.0) ,
                        (0.100,-0.050,   0.0)), dtype='float64')

    # Read current magnetic field from MCU
    H = magField

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
    r1 = float( ( (x+0.000)**2. + (y+0.000)**2. + (z+0.00)**2. )**(1/2.) )  # Sensor 1
    r2 = float( ( (x+0.000)**2. + (y-0.125)**2. + (z+0.00)**2. )**(1/2.) )  # Sensor 2
    r3 = float( ( (x-0.100)**2. + (y-0.175)**2. + (z+0.00)**2. )**(1/2.) )  # Sensor 3
    r4 = float( ( (x-0.200)**2. + (y-0.125)**2. + (z+0.00)**2. )**(1/2.) )  # Sensor 4 (ORIGIN)
    r5 = float( ( (x-0.200)**2. + (y+0.000)**2. + (z+0.00)**2. )**(1/2.) )  # Sensor 5
    r6 = float( ( (x-0.100)**2. + (y+0.050)**2. + (z+0.00)**2. )**(1/2.) )  # Sensor 6

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

# ------------------------------------------------------------------------

def dispRegion( norms ):
    global triggered
    sort = argsort( norms )
    sort.reverse()
    arr = bubbleSort(sort, 3)

    # Sum up numbers for determining what region we are in
    region = arr[0]*100 + arr[1]*10 + arr[2]

    if( region == 12 and triggered[0] == False ):
        image = Image.open( images[012] )
        # Resize the image and rotate it so it's 240x320 pixels.
        image = image.rotate(90).resize((240, 320))
        # Write buffer to display hardware, must be called to make things visible on the
        # display!
        disp.display(image)
        # Set triggered to True and everything else to False
        triggered = [ False ]*10
        triggered[0] = True
        
    elif( region == 123 and triggered[1] == False):
        image = Image.open( images[123] )
        # Resize the image and rotate it so it's 240x320 pixels.
        image = image.rotate(90).resize((240, 320))
        # Write buffer to display hardware, must be called to make things visible on the
        # display!
        disp.display(image)
        # Set triggered to True and everything else to False
        triggered = [ False ]*10
        triggered[1] = True
        
    elif( region == 234 and triggered[2] == False ):
        image = Image.open( images[234] )
        # Resize the image and rotate it so it's 240x320 pixels.
        image = image.rotate(90).resize((240, 320))
        # Write buffer to display hardware, must be called to make things visible on the
        # display!
        disp.display(image)
        # Set triggered to True and everything else to False
        triggered = [ False ]*10
        triggered[2] = True
        
    elif( region == 345 and triggered[3] == False ):
        image = Image.open( images[345] )
        # Resize the image and rotate it so it's 240x320 pixels.
        image = image.rotate(90).resize((240, 320))
        # Write buffer to display hardware, must be called to make things visible on the
        # display!
        disp.display(image)
        # Set triggered to True and everything else to False
        triggered = [ False ]*10
        triggered[3] = True
        
    elif( region == 45 and triggered[4] == False ):
        image = Image.open( images[045] )
        # Resize the image and rotate it so it's 240x320 pixels.
        image = image.rotate(90).resize((240, 320))
        # Write buffer to display hardware, must be called to make things visible on the
        # display!
        disp.display(image)
        # Set triggered to True and everything else to False
        triggered = [ False ]*10
        triggered[4] = True
        
    elif( region == 15 and triggered[5] == False ):
        image = Image.open( images[015] )
        # Resize the image and rotate it so it's 240x320 pixels.
        image = image.rotate(90).resize((240, 320))
        # Write buffer to display hardware, must be called to make things visible on the
        # display!
        disp.display(image)
        # Set triggered to True and everything else to False
        triggered = [ False ]*10
        triggered[5] = True
        
    elif( region == 25 and triggered[6] == False ):
        image = Image.open( images[025] )
        # Resize the image and rotate it so it's 240x320 pixels.
        image = image.rotate(90).resize((240, 320))
        # Write buffer to display hardware, must be called to make things visible on the
        # display!
        disp.display(image)
        # Set triggered to True and everything else to False
        triggered = [ False ]*10
        triggered[6] = True
        
    elif( region == 245 and triggered[7] == False ):
        image = Image.open( images[245] )
        # Resize the image and rotate it so it's 240x320 pixels.
        image = image.rotate(90).resize((240, 320))
        # Write buffer to display hardware, must be called to make things visible on the
        # display!
        disp.display(image)
        # Set triggered to True and everything else to False
        triggered = [ False ]*10
        triggered[7] = True
        
    elif( region == 125 and triggered[8] == False ):
        image = Image.open( images[125] )
        # Resize the image and rotate it so it's 240x320 pixels.
        image = image.rotate(90).resize((240, 320))
        # Write buffer to display hardware, must be called to make things visible on the
        # display!
        disp.display(image)
        # Set triggered to True and everything else to False
        triggered = [ False ]*10
        triggered[8] = True
        
    elif( region == 235 and triggered[9] == False ):
        image = Image.open( images[235] )
        # Resize the image and rotate it so it's 240x320 pixels.
        image = image.rotate(90).resize((240, 320))
        # Write buffer to display hardware, must be called to make things visible on the
        # display!
        disp.display(image)
        # Set triggered to True and everything else to False
        triggered = [ False ]*10
        triggered[9] = True
        
    else: print( "i cri evrytim rejin iz nut lst8" )
    
# ************************************************************************
# ===========================> SETUP PROGRAM <===========================
# ************************************************************************

# Useful variables


K           = 1.09e-6                           # Magnet's constant (K) || Units { G^2.m^6}
dx          = 1e-7                              # Differential step size (Needed for solver)
H           = np.empty((6,3), dtype='float64')  # Construct matrix of size  (6x3)
NSENS       = 6                                 # Number of sensors used

# Setup sensors
path    = "/home/pi/LSM9DS1_RaspberryPi_Library/lib/liblsm9ds1cwrapper.so"
muxPins = [22, 23, 24]                          # GPIO pins used for the multiplexer
imus    = IMU(path, NSENS, muxPins)             # Instantiate IMU objects
imus.start()                                    # Start IMUs

# Setup TFT screen
DC = 25                                         # DC line used by ILI9340 (needs to be this pin)
RST = 16                                        # Reset line
SPI_PORT = 0                                    # SPI port (as per adafruit)
SPI_DEVICE = 0                                  # SPI devc (as per adafruit)

# Create TFT LCD display class.
disp = TFT.ILI9341( DC, rst=RST,
                    spi=SPI.SpiDev(SPI_PORT,
                                   SPI_DEVICE,
                                   max_speed_hz=64000000) )
disp.begin()                                    # Initialize display.
disp.clear()                                    # Clear the display to a black background.

global images
images = { 012: '1.jpg', 123: '2.jpg',          # ...
           234: '3.jpg', 345: '4.jpg',          # Tie each region
           045: '5.jpg', 015: '6.jpg',          # with its corres-
           025: '7.jpg', 245: '8.jpg',          # ponding image.
           125: '9.jpg', 235:'10.jpg' }         # ...

global triggered
triggered = [ False ]*10         # ...

# ************************************************************************
# =========================> MAKE IT ALL HAPPEN <=========================
# ************************************************************************

print( "Place magnet on grid" )
print( "Ready in 3" )
sleep( 1.0 )
print( "Ready in 2" )
sleep( 1.0 )
print( "Ready in 1" )
sleep( 1.0 )
print( "GO!" )

initialGuess = findIG( imus.calcMag() )


# Start iteration
while( True ):

    startTime = time()
    # Get magnetic field readings
    H = imus.calcMag()
    
    # Compute L2 vector norms
    HNorm = [ float(norm(H[0])), float(norm(H[1])),
              float(norm(H[2])), float(norm(H[3])),
              float(norm(H[4])), float(norm(H[5])) ]

    # Invoke solver (using Levenberg-Marquardt)
    sol = root(LHS, initialGuess, args=(K, HNorm), method='lm',
               options={'ftol':1e-10, 'xtol':1e-10, 'maxiter':500,
                        'eps':1e-8, 'factor':0.001})

    # Draw region on TFT screen
    dispRegion( HNorm )                  
    
    # Print solution (coordinates) to screen
    print( "Current position (x , y , z):" )
    print( "(%.5f , %.5f , %.5f)mm\n" %(sol.x[0]*1000, sol.x[1]*1000, sol.x[2]*1000) )

    # Check if solution makes sense
    if (abs(sol.x[0]*1000) > 500) or (abs(sol.x[1]*1000) > 500) or (abs(sol.x[2]*1000) > 500):
        print( "Invalid solution. Resetting Calculations" )
        # Determine initial guess based on magnet's location
        initialGuess = findIG( imus.calcMag() )
        
    # Update initial guess with current position and feed back to solver
    else:    
        initialGuess = np.array( (sol.x[0]+dx, sol.x[1]+dx, sol.x[2]+dx), dtype='float64' )

    print( "t = %.5f" %(time()-startTime) )
    

# ************************************************************************
# =============================> DEPRECATED <=============================
# ************************************************************************
#
