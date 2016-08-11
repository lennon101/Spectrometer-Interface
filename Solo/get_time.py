import time
from time import sleep 
from math import trunc

def get_time(message):
	#truncate time to convert from usec to seconds
	epochTime = trunc(message/1000000)
	print "DTG: " , time.strftime("%d-%m-%Y %H:%M:%S", time.gmtime(epochTime))


get_time(1470904216218000)