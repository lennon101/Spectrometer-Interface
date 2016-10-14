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

import time
from time import sleep 
from math import trunc

import sys
import subprocess

PRINT_INTERVAL = 4 #seconds
epochTime = 0
filename = "null.csv"

def listener():
	print '\nCamera Feedback msg received \n'
	print "DTG: " , time.strftime("%d-%m-%Y %H:%M:%S", time.gmtime(epochTime))
	tStruct = time.gmtime(epochTime)

	sampleNum = 1
	pitch = 10
	roll = 20
	alt = 30
	lon = 200
	lat = 300
	yaw = 20

	global filename;

	#p = subprocess.Popen(["./seabreeze_test_posix","400",str(sampleNum), str(tStruct.tm_year), str(tStruct.tm_mon), str(tStruct.tm_mday), str(tStruct.tm_hour), str(tStruct.tm_min), str(tStruct.tm_sec), str(alt), str(roll), str(pitch), str(yaw), str(lat), str(lon),filename]);
	subprocess.call(["./data-collection","400",str(sampleNum), str(tStruct.tm_year), str(tStruct.tm_mon), str(tStruct.tm_mday), str(tStruct.tm_hour), str(tStruct.tm_min), str(tStruct.tm_sec), str(alt), str(roll), str(pitch), str(yaw), str(lat), str(lon),filename]);
	#sleep(4)
	#p.kill()

#set up Callback to print 'vehicle is now armed' once vehicle has been armed
#This will only print once 
def isArmed_callback():
	print "DTG: " , time.strftime("%d-%m-%Y %H:%M:%S", time.gmtime(epochTime))
	#filename = time.strftime("%d%m%Y_%H_%M_%S", time.gmtime(epochTime)) + ".csv"
	global filename;
	filename = "testing.csv"
	#filename = "test.csv"
	print "filename = ", filename
	subprocess.call(["./data-collection","0",filename]);
	#call = "./seabreeze_test_posix 0 " + filename
	#p = subprocess.Popen(call, shell = True);
	#sleep(4)
	#p.kill()
	
isArmed_callback();
while True:
	
	sleep(PRINT_INTERVAL);
	listener();

print "Done."
vehicle.close()
