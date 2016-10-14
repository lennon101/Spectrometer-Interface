

from dronekit import connect
from time import sleep 

import sys

# Connect to UDP endpoint (and wait for default attributes to accumulate)
target = sys.argv[1] if len(sys.argv) >= 2 else 'udpin:0.0.0.0:14550'
print 'Connecting to ' + target + '...'
vehicle = connect(target, wait_ready=True)

while True:
	# Get vehicle velocity
	vehVel = vehicle.velocity 
	
	if (vehVel < 0.5):
		print("sampling...")
		sleep(2);

	print("------------------------------------")

	#sleep the system for 2 seconds to allow drone to start moving again
	sleep(2)

vehicle.close()
print "Done."
