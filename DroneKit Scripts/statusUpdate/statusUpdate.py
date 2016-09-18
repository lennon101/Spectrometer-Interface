'''
Status Update for the 3DR Solo by Dane Lennon @ James Cook University 
DTG:  11-08-2016

A script for the 3DR Solo that prints the attributes 
of the drone at a given time in a given interval

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

PRINT_INTERVAL = 1 #seconds

# Connect to UDP endpoint (and wait for default attributes to accumulate)
target = sys.argv[1] if len(sys.argv) >= 2 else 'udpin:0.0.0.0:14550'
print 'Connecting to ' + target + '...'
vehicle = connect(target, wait_ready=True)

'''
#set up a mavlink listener for the system time clcok (taken from GPS)
@vehicle.on_message('SYSTEM_TIME')
def listener(self, name, message):
	if (vehicle.armed == True):
		#truncate time to convert from usec to seconds
		epochTime = trunc(message.time_unix_usec/1000000)
		print "DTG: " , time.strftime("%d-%m-%Y %H:%M:%S", time.gmtime(epochTime))
'''

#set up a mavlink listener for the Camera Trigger
@vehicle.on_message('CAMERA_FEEDBACK')
def listener(self, name, message):
	print '\nCamera Feedback msg received \n'

	epochTime = trunc(message.time_usec/1000000)
	print "DTG: " , time.strftime("%d-%m-%Y %H:%M:%S", time.gmtime(epochTime))
	#print 'time msg: %.0f \n' % message.time_usec
	print 'img indx: %.0f \n' % message.img_idx

	print 'lat: %+.0f \n' % message.lat
	print 'long: %+.0f \n' % message.lng
	print 'alt relative: %.2f \n' % message.alt_rel
	print 'roll: %+.11f \n' % message.roll
	print 'pitch: %+.11f \n' % message.pitch
	print 'yaw: %+.11f \n' % message.alt_rel

	#print "cam triggered @: %s \n" % message.time_usec #time stamp
	#print "cam sequence num = %s \n" % message.cam_idx #sequence number

#set up Callback to print 'vehicle is now armed' once vehicle has been armed
#This will only print once 
def isArmed_callback(self, attr_name, value):
	print "Vehicle is now armed; value is: " , value

# Add a callback 'isArmed_callback' for the 'armed' attribute.
vehicle.add_attribute_listener('armed', isArmed_callback)

while True:
	'''
	# Get all vehicle attributes (state)
	globalLoc = " Global Location: %s \n" % vehicle.location.global_frame
	heading =  " Heading: %s \n" % vehicle.heading
	globalLocRel = " Global Location (relative altitude): %s \n" % vehicle.location.global_relative_frame
	localLoc = " Local Location: %s \n" % vehicle.location.local_frame
	alt = " Attitude: %s \n" % vehicle.attitude
	vel = " Velocity: %s \n" % vehicle.velocity
	gndSpeed = " Groundspeed: %s \n" % vehicle.groundspeed
	airSpeed = " Airspeed: %s \n" % vehicle.airspeed
	bat = " Battery: %s \n" % vehicle.battery
	heartBeat = " Last Heartbeat: %s \n" % vehicle.last_heartbeat
	mode = " Mode: %s \n" % vehicle.mode.name
	armable = " Is Armable?: %s \n" % vehicle.is_armable
	armed = " Armed: %s \n" % vehicle.armed 

	msg = "Vehicle state: \n" + globalLoc + heading + globalLocRel + localLoc + alt + vel + gndSpeed + airSpeed + bat + heartBeat + mode + armable + armed

	print msg
	'''
	print("------------------------------------")

	sleep(PRINT_INTERVAL);
	#print(chr(27) + "[2J")

vehicle.close()
print "Done."