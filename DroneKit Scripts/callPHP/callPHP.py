from dronekit import connect
from time import sleep 

import sys
import urllib2 

# Connect to UDP endpoint (and wait for default attributes to accumulate)
target = sys.argv[1] if len(sys.argv) >= 2 else 'udpin:0.0.0.0:14550'
print 'Connecting to ' + target + '...'
vehicle = connect(target, wait_ready=True)

urllib2.urlopen("http://192.168.42.1/cgi-bin/getTwoSpectrum.php");

print "Done."
