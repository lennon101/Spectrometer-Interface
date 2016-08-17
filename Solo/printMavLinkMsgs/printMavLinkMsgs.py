'''
Print Mavlink msgs the 3DR Solo by Dane Lennon @ James Cook University 
DTG:  11-08-2016

A script for the 3DR Solo that prints all the mavlink msgs

There are listeners implemented that get the relevant mavlink msg and print the data

References: 
mavlink messages - https://github.com/mavlink/mavlink/blob/master/message_definitions/v1.0/common.xml
Creating a mavlink message listener - http://python.dronekit.io/guide/mavlink_messages.html
'''

from dronekit import connect
from time import sleep 
from math import trunc
import time 

import sys

# Connect to UDP endpoint (and wait for default attributes to accumulate)
target = sys.argv[1] if len(sys.argv) >= 2 else 'udpin:0.0.0.0:14550'
print 'Connecting to ' + target + '...'
vehicle = connect(target, wait_ready=True)

epochTime = 0

#set up a mavlink listener for the system time clcok (taken from GPS)
@vehicle.on_message('SYSTEM_TIME')
def listener(self, name, message):
	#truncate time to convert from usec to seconds
	global epochTime 
	epochTime = trunc(message.time_unix_usec/1000000)
	#print "systime DTG: " , time.strftime("%d-%m-%Y %H:%M:%S", time.gmtime(epochTime))


#Create a message listener for all messages
#register a callback for all messages by setting the message name as the wildcard string ('*')
@vehicle.on_message('*')
def listener(self, name, message):
	if (vehicle.armed == True):
		print '@ DTG: ' , time.strftime("%d-%m-%Y %H:%M:%S", time.gmtime(epochTime))
		print 'mavlink msg: %s \n\n--------\n' % message


#sleep the system periodically otherwise system crashes with: 
'''Exception in thread Thread-2 (most likely raised during interpreter shutdown):
Traceback (most recent call last):
  File "/usr/lib/python2.7/threading.py", line 551, in __bootstrap_inner
  File "/usr/lib/python2.7/threading.py", line 504, in run
  File "/log/solo-script/env/lib/python2.7/site-packages/dronekit/mavlink.py", line 166, in mavlink_thread_out
<type 'exceptions.AttributeError'>: 'NoneType' object has no attribute 'error'
'''
#it is assumed this is caused by the flood of mavlink msgs that can occur

print 'waiting for vehicle to arm...'
while True:
	sleep(1);

