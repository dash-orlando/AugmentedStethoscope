'''
*
* Stethoscope augmenting based on magnetic field proximity
*
* VERSION: 0.1
*   - ADDED   : First release
*
* KNOWN ISSUES:
*   - Ain't non that I can think of atm
*
*
* Author        :   Mohammad Odeh
* Written       :   Feb. 28th, 2018 Year of Our Lord
*
* Last Modified :   ________________________________
*
'''

# Import Tracking Modules
import  numpy               as      np              # Import Numpy
import  RPi.GPIO            as      GPIO            # Use GPIO pins
from    time                import  sleep, time     # Sleep for stability / time execution
from    scipy.linalg        import  norm            # Calculate vector norms (magnitude)
from    IMU_Class           import  *

# Stethoscope stuff
import  stethoscopeDefinitions       as     definitions
from    bluetoothProtocol_teensy32   import *
from    stethoscopeProtocol          import *

# Pygame for drawing
import  pygame

# ************************************************************************
# =====================> DEFINE NECESSARY FUNCTIONS <====================*
# ************************************************************************       

def argsort( seq ):
    '''
    Sort a list's elements from smallest to largest and
    return the sorted INDICES NOT VALUES!
    i.e. Given: [6, 20, 9, 18] || Returns: [0, 2, 3, 1]
    
    INPUTS:
        - seq: A list whose elements are to be sorted 

    OUTPUT:
        - A list containing the indices of the given list's elements
          arranged from the index of the element with the smallest
          value to the index of the element with the largest value
    '''
    
    # http://stackoverflow.com/questions/3071415/efficient-method-to-calculate-the-rank-vector-of-a-list-in-python
    return( sorted( range( len(seq) ), key=seq.__getitem__ ) )

# ------------------------------------------------------------------------

def dispRegion( norms, radius=15 ):
    global stethON, prvs_imu
    sort = argsort( norms )                                         # Determine which IMU corresponds to which norm (from small to large)
    sort.reverse()                                                  # Now flip order (large to small)

    max_norm = np.amax(norms)                                       # Maximum norm of any sensor being used
##    print( max_norm )
    norm_threshold = 0.89                                           # Threshold to be over in order to "track"
    crnt_imu = sort[0]                                              # Store the value of the IMU with the highest norm

    #
    # Perform all the logic we need here
    #
    if( max_norm > norm_threshold ):

        if( crnt_imu == prvs_imu ):                                 # If we are at the same region
            pass                                                    # don't do anything as it is useless

        elif( crnt_imu != prvs_imu ):
            
            # Clear previous drawing
            for name, sensor in loc.items():                        # Reset drawings
                pygame.draw.circle(gameDisplay, color['RED'], sensor, radius)
            if( stethON == True ): stopBlending( rfObject )         # Required in order to augment a different sound
            prvs_imu = crnt_imu                                     # Update previous IMU

            if( crnt_imu == 0 ):
                pygame.draw.circle(gameDisplay, color['GREEN'], loc['S0'], radius)
                print( "STET will BLEND SYNTHETIC, NORMAL HEART BEAT" )
                startBlending( rfObject, definitions.NHBSYN )       # Start augmenting
                stethON = True                                      # Set boolean to true
                
            elif( crnt_imu == 1 ):
                pygame.draw.circle(gameDisplay, color['GREEN'], loc['S1'], radius)
                print( "STET will BLEND SYNTHETIC, EARLY SYSTOLIC MURMUR" )
                startBlending( rfObject, definitions.ESMSYN )       # Start augmenting
                stethON = True                                      # Set boolean to true
                
            elif( crnt_imu == 2 ):
                pygame.draw.circle(gameDisplay, color['GREEN'], loc['S2'], radius)
                print( "STET will BLEND RECORDED, NORMAL HEART BEAT" )
                startBlending( rfObject, definitions.NHBREC )
                stethON = True
                
            elif( crnt_imu == 3 ):
                pygame.draw.circle(gameDisplay, color['GREEN'], loc['S3'], radius)
                print( "STET will BLEND RECORDED, EXERCISED HEART BEAT" )
                startBlending( rfObject, definitions.EHBREC )
                stethON = True
                
            elif( crnt_imu == 4 ):
                pygame.draw.circle(gameDisplay, color['GREEN'], loc['S4'], radius)
                print( "STET will BLEND SYNTHETIC, PULMONARY EJECTION" )
                startBlending( rfObject, definitions.PEJECT )
                stethON = True
                
            elif( crnt_imu == 5 ):
                pygame.draw.circle(gameDisplay, color['GREEN'], loc['S5'], radius)
                print( "STET will BLEND SYNTHETIC, PULMONARY SPLIT" )
                startBlending( rfObject, definitions.PSPLIT )
                stethON = True
                
            else: print( "i cri evrytim :(" )
    
    else:                                                           # If magnet is not within range
        for name, sensor in loc.items():                            # Reset drawing
            pygame.draw.circle(gameDisplay, color['RED'], sensor, radius)
        if( stethON == True ):
            print( "STET will STREAM MIC" )
            stopBlending( rfObject )                                # Stop augmenting
            stethON = False                                         # Set boolean to false
        prvs_imu = ''
    
# ************************************************************************
# ===========================> SETUP PROGRAM <===========================
# ************************************************************************

# Useful variables
global loc, prvs_imu, stethON
stethON     = False                                                 # Boolean to determine if steth. is augmenting or not
prvs_imu    = ''                                                    # Store previously drawn IMU for future reference
NSENS       = 6                                                     # Number of sensors used
H           = np.empty((NSENS, 3), dtype='float64')                 # Construct matrix of size  (6x3)


# Setup sensors
path    = "/home/pi/LSM9DS1_RaspberryPi_Library/lib/liblsm9ds1cwrapper.so"
muxPins = [22, 23, 24]                                              # GPIO pins used for the multiplexer
imus    = IMU(path, NSENS, muxPins)                                 # Instantiate IMU objects
imus.start()                                                        # Start IMUs


# Setup BT communications
BT_addr = { "AS003": "00:06:66:D0:E4:94",
            "AS004": "00:06:66:D0:C9:A5" }
            #"AS005": "00:06:66:D0:C9:AE" }
BT_port = 1                                                         # Port != 0 or it ain't workin'

try:
    rfObject = createBTPort( BT_addr[ "AS003" ], BT_port )          # Connect to AS003
except:
    rfObject = createBTPort( BT_addr[ "AS004" ], BT_port )          # If not available, try AS004

if( statusEnquiry( rfObject ) != True ):                            # Check system status
    print( 'Something went awry :(' )                               # If system not ready spend
    print( 'REBOOT SYSTEM pls!!1!1' )                               # ...
    while( True ): pass                                             # time in a perpetual loop of misery
else:
    pass

# Initialize engine
pygame.init()                                                       # Initialize pygame engine

color = { 'WHITE': (255, 255, 255),                                 # Assign values
          'BLACK': (  0,   0,   0),                                 # ...
          'RED'  : (255,   0,   0),                                 # to the colors
          'GREEN': (  0, 255,   0),                                 # ...
          'BLUE' : (  0,   0, 255) }                                # we want to use

# Create display
w = 500                                                             # Desired UI width
h = 500                                                             # Desired UI height
gameDisplay = pygame.display.set_mode( (w, h) )                     # Create UI screen
gameDisplay.fill( color['BLACK'] )                                  # Fill background with black

# Define IMU positions on screen
x0 = w/2                                                            # Center along the x-axis
y0 = h/2                                                            # Center along the y-axis
loc = { 'S0': ( x0-200, y0+100 ), 'S1': ( x0-200,  y0-100 ) ,       # Sensors 0, 1
        'S2': ( x0    , y0-200 ), 'S3': ( x0+200,  y0-100 ) ,       # Sensors 2, 3
        'S4': ( x0+200, y0+100 ), 'S5': ( x0    ,  y0+200 )  }      # Sensors 4, 5

# Draw IMUs
radius = 15                                                         # Radius of circle
for name, sensor in loc.items():                                    # Loop over sensors ...
##    print( "{0} center is {1}".format(name, sensor) )
    pygame.draw.circle(gameDisplay, color['RED'], sensor, radius)   # ... and draw them

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

    # Get magnetic field readings
    H = imus.calcMag()
    
    # Compute L2 vector norms
    HNorm = [ float(norm(H[0])), float(norm(H[1])),
              float(norm(H[2])), float(norm(H[3])),
              float(norm(H[4])), float(norm(H[5])) ]

    # Draw region on TFT screen
    dispRegion( HNorm )

    # Check events (did we hit the close button to shutdown program or nah?)
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            closeBTPort( rfObject )
            quit()

    # Update screen
    pygame.display.update()
    

# ************************************************************************
# =============================> DEPRECATED <=============================
# ************************************************************************
#
