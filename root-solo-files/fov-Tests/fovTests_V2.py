'''
Data-collection for the 3DR Solo by Dane Lennon @ James Cook University 
DTG:  18-09-2016

A script for the 3DR Solo that calls a c script to start sampling ocean-optics spectrometers

There are listeners implemented to catch mavlink msgs from the pixhawk 2

References: 
mavlink messages - https://github.com/mavlink/mavlink/blob/master/message_definitions/v1.0/common.xml
Creating a mavlink message listener - http://python.dronekit.io/guide/mavlink_messages.html

data-collection Usage:
$ ./data-collection sampleNum yr mm dd hr mm ss alt roll pitch yaw lat lon
filename

Where:

--sampleNum       An index declaring the sample number
if 0 then a header file is setup, else if 1 or more, samples are saved
--dtg             the date-time-group which will appended to the header of the csv
--basefile        prefix of output filename (i.e. '/path/to/data').  Will be automatically be
prefixed for each recorded acquisition.

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

global epochTime;
epochTime = 0

global filename;
filename = ""

#set up a mavlink listener for the Camera Trigger
@vehicle.on_message('CAMERA_FEEDBACK')
def listener(self, name, message):
	if (vehicle.armed == True):
		print '\nCamera Feedback msg received \n'
		print message
		global epochTime;
		global filename;
		alt = message.alt_rel
		vel = vehicle.velocity;
		gndSpeed = vehicle.groundspeed
		print "Velocity: %s \n" % vehicle.velocity

		if (alt < 10):
			#do nothing
			print ""
		else:
			while (gndSpeed >3):
				gndSpeed = vehicle.groundspeed
				vel = vehicle.velocity;
				print "Velocity: %s \n" % vehicle.velocity
				print " Groundspeed: %s \n" % vehicle:wq


			epochTime = trunc(message.time_usec/1000000) + 36000 #conversion from zulu to GM +10hrs 
			print "DTG: " , time.strftime("%d-%m-%Y %H:%M:%S", time.gmtime(epochTime))
			tStruct = time.gmtime(epochTime)

			#print 'time msg: %.0f \n' % message.time_usec
			print 'alt relative: %.2f \n' % message.alt_rel

			# Get all vehicle attributes (state)
			globalLoc = " Global Location: %s \n" % vehicle.location.global_frame
			heading =  " Heading: %s \n" % vehicle.heading
			globalLocRel = " Global Location (relative altitude): %s \n" % vehicle.location.global_relative_frame
			localLoc = " Local Location: %s \n" % vehicle.location.local_frame
			vel = " Velocity: %s \n" % vehicle.velocity
			gndSpeed = " Groundspeed: %s \n" % vehicle.groundspeed
			airSpeed = " Airspeed: %s \n" % vehicle.airspeed

			msg = "Vehicle state: \n" + globalLoc + heading + globalLocRel + localLoc + alt + vel + gndSpeed + airSpeed

			print msg

			#TODO: get updated data from vehicle instead of using old data from MAVLink 
			sampleNum = message.img_idx
			pitch = message.pitch
			roll = message.roll
			lon = message.lng
			lat = message.lat
			yaw = message.yaw
			alt = message.alt_rel
			subprocess.call(["./data-collection-flight-data",str(sampleNum), str(tStruct.tm_year), str(tStruct.tm_mon), str(tStruct.tm_mday), str(tStruct.tm_hour), str(tStruct.tm_min), str(tStruct.tm_sec), str(alt), str(roll), str(pitch), str(yaw), str(lat), str(lon),filename]);
			sleep(PRINT_INTERVAL)

#set up a mavlink listener for the system time clcok (taken from GPS)
@vehicle.on_message('SYSTEM_TIME')
def listener(self, name, message):
	global epochTime;
	epochTime = trunc(message.time_unix_usec/1000000) + 36000 #conversion from zulu to GM +10hrs 
	print "DTG: " , time.strftime("%d-%h-%Y %H:%M:%S", time.gmtime(epochTime))


#set up Callback to print 'vehicle is now armed' once vehicle has been armed
#This will only print once 
def isArmed_callback(self, attr_name, value):
	print "Vehicle is now armed; value is: " , value
	if (vehicle.armed == True):
		print "DTG: " , time.strftime("%d-%m-%Y %H:%M:%S", time.gmtime(epochTime))
		global filename;
		global epochTime;
		filename = time.strftime("%d-%m-%Y_%H|%M|%S", time.gmtime(epochTime))
		print "filename = ", filename
		subprocess.call(["./data-collection-flight-data", "0",filename]);

# Add a callback 'isArmed_callback' for the 'armed' attribute.
vehicle.add_attribute_listener('armed', isArmed_callback)

while True:
	sleep(PRINT_INTERVAL);

print "Done."
vehicle.close()
