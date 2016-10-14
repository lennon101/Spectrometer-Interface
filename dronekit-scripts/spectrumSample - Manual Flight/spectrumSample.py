'''
Spectrum Sample for the 3DR Solo by Dane Lennon @ James Cook University 
DTG:  11-08-2016

A script for the 3DR Solo that samples two spectrometers using ocean optics Seabreeze device drivers.
The seabreeze libraries must be compiled on an RPI and copied across into the root directory of the solo 

There are listeners implemented that get the relevant mavlink msg and print the data

References: 
mavlink messages - https://github.com/mavlink/mavlink/blob/master/message_definitions/v1.0/common.xml
Creating a mavlink message listener - http://python.dronekit.io/guide/mavlink_messages.html
'''

from dronekit import connect
import time
from time import sleep 
from math import trunc

import sys

# Connect to UDP endpoint (and wait for default attributes to accumulate)
target = sys.argv[1] if len(sys.argv) >= 2 else 'udpin:0.0.0.0:14550'
print 'Connecting to ' + target + '...'
vehicle = connect(target, wait_ready=True)

#set up Callback to print 'vehicle is now armed' once vehicle has been armed
#This will only print once 
def isArmed_callback(self, attr_name, value):
	print "Vehicle is now armed; value is: " , value
	return value

# Add a callback 'isArmed_callback' for the 'armed' attribute.
vehicle.add_attribute_listener('armed', isArmed_callback)

#set up a mavlink listener for the Camera Trigger
@vehicle.on_message('CAMERA_TRIGGER')
def listener(self, name, message):
		print "cam triggered @: %s \n" % message.time_usec #time stamp
		print "cam sequence num = %s \n" % message.seq #sequence number

		#Sample the spectrometers by calling the C script




