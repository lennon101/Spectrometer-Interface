'''
Spectrum Sample for the 3DR Solo by Dane Lennon @ James Cook University 
DTG:  11-08-2016

A script for the 3DR Solo that samples two spectrometers using ocean optics Seabreeze libraries 

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