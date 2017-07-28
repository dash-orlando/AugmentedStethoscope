"""

Plotting magnetometer data

"""

import serial, time

#Defining the serial port connection.

#possible timeout values:
#    1. None: wait forever, block call
#    2. 0: non-blocking mode, return immediately
#    3. x, x is bigger than 0, float allowed, timeout block call

ser = serial.Serial()
ser.port = "COM4"
ser.baudrate = 115200
ser.bytesize = serial.EIGHTBITS                         #number of bits per bytes
ser.parity = serial.PARITY_NONE                         #set parity check: no parity
ser.stopbits = serial.STOPBITS_ONE                      #number of stop bits
ser.timeout = None                                  
ser.xonxoff = False                                     #disable software flow control
ser.rtscts = False                                      #disable hardware (RTS/CTS) flow control
ser.dsrdtr = False                                      #disable hardware (DSR/DTR) flow control
ser.writeTimeout = 5                                    #timeout for write

#Imports for doing math quickly
import numpy as np                                      #Optimized for array math
import math                                             #Optimized for single values

#Imports for animating the data
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from drawnow import *

#Opening the serial port.
if ser.is_open == False:
    ser.open()

#Entering the main loop. Reading a line of input data (6 comma separated values) and splitting them into an array.
while True:
    line = ser.readline()[:-1]
    col = line.split(",")
    
#Throwing out bad data. Waiting for the sensor to calibrate itself to ambient fields. 
    if len(col) < 6:
        print "Calibrating. Please wait."
    else:
        
#Casting the split input values as floats.
        imux0=float(col[0])
        imuy0=float(col[1])
        imuz0=float(col[2])
        
        imux1=float(col[3])
        imuy1=float(col[4])
        imuz1=float(col[5])

#Doing math to find the magnitude of the magnetic field, B, from the POV of each IMU
        imuB0 = math.sqrt((imux0*imux0 + imuy0*imuy0 + imuz0*imuz0))
        imuB1 = math.sqrt((imux1*imux1 + imuy1*imuy1 + imuz1*imuz1))

#Calculating distance from IMUx using the value of Bx
#Empirically derived: distance=15.608*(magnitudeB)^(-0.528) [1] r^2 = .906
#Empirically derived: distance=19.818*(magnitudeB)^(-0.516) [2] r^2 = .891
#Empirically derived: distance=95.198*(magnitudeB)^(-0.294) [3] r^2=0.9931 <-(AFTER CALIBRATING FIRST! WOW!)

        factorB0= math.pow(imuB0,0.294)
        factorB1= math.pow(imuB1,0.294)

#The radial distance of the magnet from each Sensor, result
        c = 95.198/factorB0     #Radial distance from first sensor
        a = 95.198/factorB1     #Radial distance from second sensor
        #Really accurate! ^_^

#Printing values to stdout to verify that calculations are being done appropriately
        #print imuB0, imuB1, a, c
             

#Calculating angle above the x-axis, Theta from known values
#d is the minimum distance between the center of the two IMU sensors in [millimeters], given by design.
        l=225
        
        cosnum= (l*l)+(c*c)-(a*a)
        cosden= 2*l*c
        thetarad= np.arccos(cosnum/cosden)
        theta=math.degrees(thetarad)

        inv=-1
        x= a*(math.cos(thetarad))
        y= a*(math.sin(thetarad))*(np.sign(imux0))*(inv)
        
        print c, a, x, y, theta

        
ser.close() 
        
      
