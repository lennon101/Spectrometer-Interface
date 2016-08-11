

from dronekit import connect
from time import sleep 

import sys

# Connect to UDP endpoint (and wait for default attributes to accumulate)
target = sys.argv[1] if len(sys.argv) >= 2 else 'udpin:0.0.0.0:14550'
print 'Connecting to ' + target + '...'
vehicle = connect(target, wait_ready=True)

while True:

	# Get all vehicle attributes (state)
	vehVel = vehicle.velocity 

	globalLoc = " Global Location: %s \n" % vehicle.location.global_frame
	heading =  " Heading: %s" % vehicle.heading
	globalLocRel = " Global Location (relative altitude): %s \n" % vehicle.location.global_relative_frame
	localLoc = " Local Location: %s \n" % vehicle.location.local_frame
	alt = " Attitude: %s \n" % vehicle.attitude
	vel = " Velocity: %s \n" % vehVel
	gndSpeed = " Groundspeed: %s \n" % vehicle.groundspeed
	airSpeed = " Airspeed: %s \n" % vehicle.airspeed
	bat = " Battery: %s \n" % vehicle.battery
	heartBeat = " Last Heartbeat: %s \n" % vehicle.last_heartbeat
	mode = " Mode: %s \n" % vehicle.mode.name
	armable = " Is Armable?: %s \n" % vehicle.is_armable
	armed = " Armed: %s \n" % vehicle.armed 

	msg = "Vehicle state: \n" + globalLoc + heading + globalLocRel + localLoc + alt + vel + gndSpeed + airSpeed + bat + heartBeat + mode + armable + armed
	
	#print msg

	#get vehicle velocity
	
	if (vehVel < 0.5):
		print("sampling...")
		sleep(2);

	print("------------------------------------")

	#sleep(2);
	print(chr(27) + "[2J")

vehicle.close()
print "Done."
