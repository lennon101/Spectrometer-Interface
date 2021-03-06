'''
Data-collection for the 3DR Solo by Dane Lennon @ James Cook University 
DTG:  18-09-2016

A script for the 3DR Solo that calls a c script to start sampling ocean-optics spectrometers

There are listeners implemented to catch mavlink msgs from the pixhawk 2

References: 
mavlink messages - https://github.com/mavlink/mavlink/blob/master/message_definitions/v1.0/common.xml
Creating a mavlink message listener - http://python.dronekit.io/guide/mavlink_messages.html
'''

from dronekit import connect
import time
from time import sleep 
from math import trunc

import sys
import subprocess

PRINT_INTERVAL = 1 #seconds

# Connect to UDP endpoint (and wait for default attributes to accumulate)
#target = sys.argv[1] if len(sys.argv) >= 2 else 'udpin:0.0.0.0:14550'
target ='udpin:0.0.0.0:14550'
print 'Connecting to ' + target + '...'
vehicle = connect(target, wait_ready=True)

print "Enter file name for csv: "
filename = raw_input()

epochTime = 0

#set up a mavlink listener for the Camera Trigger
@vehicle.on_message('CAMERA_FEEDBACK')
def listener(self, name, message):
	print '\nCamera Feedback msg received \n'

	epochTime = trunc(message.time_usec/1000000)
	print "DTG: " , time.strftime("%d-%m-%Y %H:%M:%S", time.gmtime(epochTime))
	tStruct = time.gmtime(epochTime)

	#print 'time msg: %.0f \n' % message.time_usec
	print 'img indx: %.0f \n' % message.img_idx
	sampleNum = message.img_idx
	print 'lat: %+.0f \n' % message.lat
	lat = message.lat
	print 'long: %+.0f \n' % message.lng
	lon = message.lng
	print 'alt relative: %.2f \n' % message.alt_rel
	alt = message.alt_rel
	print 'roll: %+.11f \n' % message.roll
	roll = message.roll
	print 'pitch: %+.11f \n' % message.pitch
	pitch = message.pitch
	print 'yaw: %+.11f \n' % message.yaw
	yaw = message.yaw

	subprocess.call(["./../../home/root/data-collection", "0", str(sampleNum), str(tStruct.tm_year), str(tStruct.tm_mon), str(tStruct.tm_mday), str(tStruct.tm_hourstr), str(tStruct.tm_min), str(tStruct.tm_sec), str(roll), str(pitch), str(yaw), str(lat), str(lon),filename]);
	subprocess.call(["./../../home/root/data-collection", "1", str(sampleNum), str(tStruct.tm_year), str(tStruct.tm_mon), str(tStruct.tm_mday), str(tStruct.tm_hourstr), str(tStruct.tm_min), str(tStruct.tm_sec), str(roll), str(pitch), str(yaw), str(lat), str(lon),filename]);

	sleep(PRINT_INTERVAL)

#set up a mavlink listener for the system time clcok (taken from GPS)
@vehicle.on_message('SYSTEM_TIME')
def listener(self, name, message):
	if (vehicle.armed == True):
		#truncate time to convert from usec to seconds
		epochTime = trunc(message.time_unix_usec/1000000)
		#print "DTG: " , time.strftime("%d-%m-%Y %H:%M:%S", time.gmtime(epochTime))


#set up Callback to print 'vehicle is now armed' once vehicle has been armed
#This will only print once 
def isArmed_callback(self, attr_name, value):
	print "Vehicle is now armed; value is: " , value
	if (vehicle.armed == True):
		print "DTG: " , time.strftime("%d-%m-%Y %H:%M:%S", time.gmtime(epochTime))
		filename = time.strftime("%d-%m-%Y_%H:%M:%S", time.gmtime(epochTime))
		print "filename = ", filename


# Add a callback 'isArmed_callback' for the 'armed' attribute.
vehicle.add_attribute_listener('armed', isArmed_callback)

while True:
	sleep(PRINT_INTERVAL);

	#print(chr(27) + "[2J")

vehicle.close()
print "Done."
