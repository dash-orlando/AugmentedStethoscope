"""
bluetoothDataRelay.py

Victoria Lobo 08/16/2016
"""

# Import Modules
import os
import serial

# 1- Create RFCOMM-PORT
port_num = "0"
antenna_address = "00:06:66:7D:96:C4"
#antenna_address = "00:06:66:7D:80:D0"
terminal_string = "sudo rfcomm bind /dev/rfcomm" + port_num + " " + antenna_address
os.system(terminal_string)

# 2- Create a Serial-RF Object
rfcomm_port = "/dev/rfcomm" + port_num
baud_rate = "115200"
serial_obj = serial.Serial(rfcomm_port,baud_rate)

# 3- Read info. from RFCOMM Object
for i in range(0,20):
        inString = serial_obj.readline()
        print inString



